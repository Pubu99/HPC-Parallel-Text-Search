# Makefile for Parallel Text Search & Analysis project

# Compiler settings
CXX = g++
MPICXX = mpic++
CXXFLAGS = -std=c++17 -O2
OPENMP_FLAGS = -fopenmp

# Directories
SRC_DIR = src
BIN_DIR = .

# Executable names
SERIAL = $(BIN_DIR)/serial
OPENMP = $(BIN_DIR)/openmp
MPI = $(BIN_DIR)/mpi
HYBRID = $(BIN_DIR)/hybrid

# Source files
SERIAL_SRC = $(SRC_DIR)/serial.cpp
OPENMP_SRC = $(SRC_DIR)/openmp.cpp
MPI_SRC = $(SRC_DIR)/mpi.cpp
HYBRID_SRC = $(SRC_DIR)/hybrid.cpp

# Default target
all: $(SERIAL) $(OPENMP) $(MPI) $(HYBRID)

# Serial version
$(SERIAL): $(SERIAL_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

# OpenMP version
$(OPENMP): $(OPENMP_SRC)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $<

# MPI version
$(MPI): $(MPI_SRC)
	$(MPICXX) $(CXXFLAGS) -o $@ $<

# Hybrid version (MPI + OpenMP)
$(HYBRID): $(HYBRID_SRC)
	$(MPICXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $<

# Clean up executables
clean:
	rm -f $(SERIAL) $(OPENMP) $(MPI) $(HYBRID)

.PHONY: all clean