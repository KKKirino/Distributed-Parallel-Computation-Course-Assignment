#include <iostream>
#include <time.h>
#include <math.h>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 3) return printf("请指定 n 和 seg_size");

  long n = atol(argv[1]);
  long seg_size = atol(argv[2]);

  int* arr = new int[seg_size];
  memset(arr, 0, sizeof(arr));

  for (long j = 0; j < n; j += seg_size) {
    for (long i = 0; i < seg_size; i += 1) {
      if (arr[i]);
    }
  }

  return 0;
}