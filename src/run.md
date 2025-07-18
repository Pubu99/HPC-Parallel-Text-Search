mpic++ -std=c++17 -o mpi mpi.cpp          mpirun -np 4 ./mpi
mpic++ -fopenmp -std=c++17 -o hybrid hybrid.cpp     mpirun -np 4 ./hybrid
g++ -std=c++17 -fopenmp -o openmp openmp.cpp     ./openmp 4
gcc serial.cpp -o serial          ./serial



