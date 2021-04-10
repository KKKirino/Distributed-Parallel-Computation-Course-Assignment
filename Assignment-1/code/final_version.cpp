#include <mpi.h>
#include <math.h>
#include <iostream>

using namespace std;

int solve(long low_range, long high_range, char *marked,
          int mark_size, int *sqn_mark, int sqn_size)
{
  long count = 0;

  memset(marked, 0, sizeof(char) * mark_size); // 提前清零

  // 过滤所有的素数
  for (long pi = 2, prime = sqn_mark[pi]; pi < sqn_size;)
  {
    // 确定本线程的开始素数
    long first = (low_range / prime) * prime;

    // 跳过小于 low_range 的初始值、素数值以及偶数值
    while (first < low_range) first += prime;
    if (first % 2 == 0) first += prime;

    // 由于保证了 first 是奇数，所以每次都跳素数的偶数倍
    for (long x = first, step = prime * 2; x < high_range; x += step)
    {
      if (x != prime)
        marked[x - low_range] = 1;
    }

    prime = sqn_mark[++ pi];
  }

  // 保证从 5 开始计数
  long start = max(5l, low_range);
  if (start % 2 == 0) start += 1;
  for (long i = start; i < high_range; i += 2)
  {
    // 统计数字时同样跳过偶数
    count += !marked[i - low_range] && (i % 3);
  }

  return count;
}

int main(int argc, char *argv[])
{
  long count = 0, global_count = 0; // 素数个数
  int pid, pcount;                  // 线程 id 和线程总数

  // 初始化 mpi 环境
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);    // 获取当前线程 id
  MPI_Comm_size(MPI_COMM_WORLD, &pcount); // 总线程数量
  MPI_Barrier(MPI_COMM_WORLD);

  if (argc != 2)
  {
    if (!pid)
    {
      printf("Command line: %s <m>\n", argv[0]);
      MPI_Finalize();
    }
    return -1;
  }

  long n = atol(argv[1]);      // 素数总量
  double timer = -MPI_Wtime(); // 计时开始

  // 计算每个线程掌管的数据段
  long proc_size = n / pcount + 1;             // 每个线程的尺寸
  long low_range = pid * proc_size;        // 线程负责的开始位置
  long high_range = min(n, (pid + 1) * proc_size); // 线程负责的终止位置
  long proc_array_size = proc_size + 1;    // 线程标记数组大小

  // 限制线程数量在 sqrt(n) 以内，所以保证线程 0 可以获取到所有素数
  if (proc_size + 2 < (long)sqrtl(n))
  {
    if (!pid)
    {
      printf("Too many processes\n");
    }
    MPI_Finalize();
    return -1;
  }

  // 计算前 sqrt(n) 个素数
  int sqrtn = (int)sqrtl(high_range) + 1;
  int *sqn_mark = new int[sqrtn];
  memset(sqn_mark, 0, sizeof(int) * sqrtn);
  for (int prime = 2; prime * prime < sqrtn; prime += 1)
  {
    if (sqn_mark[prime])
      continue;
    for (int i = 2; i * prime < sqrtn; i += 1)
    {
      sqn_mark[i * prime] = 1;
    }
  }

  // 得到更紧凑的素数表
  int wi = 0, ri = 2;
  while (ri < sqrtn) {
    if (!sqn_mark[ri]) {
      sqn_mark[wi ++] = ri;
    }
    ri += 1;
  }
  sqrtn = wi;

  // 使用分块算法优化 cache 访存
  long seg_size = 500000;
  char *marked = new char[seg_size]; // 标记数组
  for (long i = low_range; i < high_range; i += seg_size)
  {
    // if (i / seg_size % 100 == 0) cout << i << endl;
    count += solve(i, min(high_range, i + seg_size), marked,
                   seg_size, sqn_mark, sqrtn);
  }

  delete sqn_mark;
  delete marked;

  // 求解所有素数个数和
  MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  // 停止计时
  timer += MPI_Wtime();

  if (!pid)
  {
    global_count += (n >= 2l) + (n >= 3l); // 把 2 和 3 计算进去
    printf("There are %ld primes less than or equal to %ld\n",
           global_count, n);
    printf("SIEVE (%d) %10.6f\n", pid, timer);
  }

  MPI_Finalize();
  return 0;
}
