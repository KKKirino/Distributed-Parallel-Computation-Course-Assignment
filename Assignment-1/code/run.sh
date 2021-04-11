n=1000000000

echo $n

for p in 1 2 4 8 16
do
  echo "Process Number: $p"
  echo "Example: "
  mpic++ ./example.cpp -o example.o && mpirun -np $p ./example.o $n | grep "SIEVE (0)"
  echo "Baseline: "
  mpic++ ./baseline.cpp -o baseline.o && mpirun -np $p ./baseline.o $n | grep "SIEVE (0)"
  echo "No Even: "
  mpic++ ./no_even.cpp -o no_even.o && mpirun -np $p ./no_even.o $n | grep "SIEVE (0)"
  echo "No Cast: "
  mpic++ ./no_cast.cpp -o no_cast.o && mpirun -np $p ./no_cast.o $n | grep "SIEVE (0)"
  echo "Better Cache: "
  mpic++ ./better_cache.cpp -o better_cache.o && mpirun -np $p ./better_cache.o $n | grep "SIEVE (0)"
  echo "Final Version: "
  mpic++ ./final_version.cpp -o final_version.o && mpirun -np $p ./final_version.o $n | grep "SIEVE (0)"
done

# mpic++ ./example.cpp -o example.o && mpirun -np 8 ./example.o 1000000000