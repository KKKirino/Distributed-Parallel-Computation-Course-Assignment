#include <mpi.h>
#include <math.h>
#include <iostream>

using namespace std;

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
  long proc_size = n / pcount;             // 每个线程的尺寸
  long low_range = pid * proc_size;        // 线程负责的开始位置
  long high_range = (pid + 1) * proc_size; // 线程负责的终止位置
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

  bool *marked = new bool[proc_array_size](); // 标记数组

  // 1. 直接从 3 开始进行计算
  for (long prime = 3; prime * prime <= n;)
  {
    // 确定本线程的开始素数
    long first = (low_range / prime) * prime;
    for (int x = first; x < high_range; x += prime)
    {
      // 跳过没有在范围内的数字，以及素数本身
      if (x < low_range || x == prime || x % 2 == 0)
        continue; // 2. 跳过偶数标记，40% 左右的性能提升，主要来自于访存优化
      marked[x - low_range] = 1;
    }

    if (!pid)
    {
      // 找到下一个素数，并广播
      do {
        prime += 1 + (prime % 2); // 3. 寻找素数时跳过偶数
      } while (prime < high_range && marked[prime]);
    }

    MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // 保证从 2 开始计数
  for (int i = max(2l, low_range); i < high_range; i += 1 + (i % 2))
  {
    // 4. 统计数字时同样跳过偶数
    count += !marked[i - low_range] && (i & 1);
  }

  // 求解所有素数个数和
  MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // 停止计时
  timer += MPI_Wtime();

  if (!pid)
  {
    global_count += (n >= 2l); // 把 2 计算进去
    printf("There are %ld primes less than or equal to %ld\n",
           global_count, n);
    printf("SIEVE (%d) %10.6f\n", pid, timer);
  }

  MPI_Finalize();
  return 0;
}