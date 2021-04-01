for p in 2 4 8 16
do
  echo "Process Number: $p"
  echo "No Even: "
  mpic++ ./no_even.cpp -o no_even.o && mpirun -np $p ./no_even.o 1000000000 | grep "SIEVE (0)"
  echo "No Cast: "
  mpic++ ./no_cast.cpp -o no_cast.o && mpirun -np $p ./no_cast.o 1000000000 | grep "SIEVE (0)"
done