for p in 2 4 8 16
do
  echo "Process Number: $p"
    echo "Example: "
  mpic++ ./example.cpp -o example.o && mpirun -np $p ./example.o 1000000000 | grep "SIEVE (0)"
  echo "Baseline: "
  mpic++ ./baseline.cpp -o baseline.o && mpirun -np $p ./baseline.o 1000000000 | grep "SIEVE (0)"
  echo "No Even: "
  mpic++ ./no_even.cpp -o no_even.o && mpirun -np $p ./no_even.o 1000000000 | grep "SIEVE (0)"
  echo "No Cast: "
  mpic++ ./no_cast.cpp -o no_cast.o && mpirun -np $p ./no_cast.o 1000000000 | grep "SIEVE (0)"
  echo "Better Cache: "
  mpic++ ./better_cache.cpp -o better_cache.o && mpirun -np $p ./better_cache.o 1000000000 | grep "SIEVE (0)"
done

# mpic++ ./example.cpp -o example.o && mpirun -np 8 ./example.o 1000000000