#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

int main() {
  int n = 1000000000 / 4;

  bool* arr = new bool[n]();

  time_t t = time(NULL);

  int ln = (int)log(log(n));

  for (int j = 0; j < ln; j += 1) {
    for (int i = 0; i < n; i += 1) {
      arr[i] = i * j;
    }
  }

  return 0;
}