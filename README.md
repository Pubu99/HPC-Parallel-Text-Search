# Parallel Text Search & Analysis using OpenMP, MPI, and Hybrid Models

This project implements a High-Performance Computing (HPC) solution for parallel text search and analysis on a large-scale dataset. It demonstrates multiple levels of parallelism using OpenMP (multithreading), MPI (multiprocessing), and their Hybrid integration, enabling significant performance improvements over traditional serial processing.

## 🛠️ Project Structure

```
.
├── data/                   # Input files and datasets
│   └── reviews/            # Text files (e.g., IMDB reviews)
├── src/                    # Source code
│   ├── serial.cpp          # Serial version of the text search
│   ├── openmp.cpp          # OpenMP version using multithreading
│   ├── mpi.cpp             # MPI version using multiprocessing
│   ├── hybrid.cpp          # Hybrid version (MPI + OpenMP)
│   └── preprocess.py       # Preprocessing script for dataset
├── Makefile                # Compilation rules
├── results/                # Output logs and processed data
└── README.md
```

## ⚙️ Models Explained

### 🔸 Serial Model (src/serial.cpp)
- Baseline version.
- Processes each file sequentially using standard C++ file I/O.
- No parallelism applied.

### 🔸 OpenMP Model (src/openmp.cpp)
- Uses `#pragma omp parallel for` to distribute the file processing workload across multiple threads.
- Ideal for shared-memory systems.
- Offers speedup with minimal code restructuring.

### 🔸 MPI Model (src/mpi.cpp)
- Uses `MPI_Scatter`, `MPI_Gather`, and `MPI_Bcast` for file distribution and result collection.
- Suited for distributed-memory architectures (multi-node environments).
- Each process independently processes part of the dataset.

### 🔸 Hybrid Model (src/hybrid.cpp)
- Combines MPI (process-level parallelism) with OpenMP (thread-level parallelism inside each process).
- Offers scalability across clusters with shared-memory nodes.
- Demonstrates two-level parallelism: inter-process and intra-process.

### 🔸 Preprocessing Script (src/preprocess.py)
- Python script for preparing the dataset (e.g., cleaning or formatting text files).
- Run before executing the C++ programs if preprocessing is needed.

## 📦 Dependencies

Make sure the following tools and libraries are installed:

```bash
sudo apt update
sudo apt install build-essential libopenmpi-dev openmpi-bin python3
```

Ensure Python 3 is installed for running `preprocess.py`.

## 🚀 How to Build

Use the provided Makefile to compile all versions (if available):

```bash
make all
```

Or compile individually:

```bash
# Serial
g++ -std=c++17 -o serial src/serial.cpp

# OpenMP
g++ -std=c++17 -fopenmp -o openmp src/openmp.cpp

# MPI
mpic++ -std=c++17 -o mpi src/mpi.cpp

# Hybrid
mpic++ -fopenmp -std=c++17 -o hybrid src/hybrid.cpp
```

## 🏃 How to Run

### 🔹 Preprocessing (Optional)
If preprocessing is required, run:

```bash
python3 src/preprocess.py
```

### 🔹 Serial Version

```bash
./serial
```

### 🔹 OpenMP Version

```bash
./openmp 4
```

Note: The `4` specifies the number of threads (adjust as needed).

### 🔹 MPI Version

```bash
mpirun -np 4 ./mpi
```

### 🔹 Hybrid Version (MPI + OpenMP)

```bash
mpirun -np 4 ./hybrid
```

Note: Set `OMP_NUM_THREADS` environment variable if specific thread counts per process are needed, e.g., `export OMP_NUM_THREADS=2` before running.

This runs the hybrid model with 4 MPI processes. Combine with OpenMP threads for additional parallelism (total logical units depend on `OMP_NUM_THREADS`).

## 📁 Dataset Notes

- Place your `.txt` review files inside `data/reviews/`.
- Each file should be a plain text file for keyword searching.
- Use `src/preprocess.py` to prepare or clean the dataset if necessary.

## 🧪 Customization

- Change the search keyword in the `.cpp` files (look for the line with `string keyword = "yourword";`).
- Adjust the dataset directory if different (`data/reviews/`).
- Modify `src/preprocess.py` for specific preprocessing needs.

## 📜 License

This project is for educational and academic use. Please give credit if used in research or publications.