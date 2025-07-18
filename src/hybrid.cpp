#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <cstring>
using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

vector<fs::path> get_txt_files(const string& directoryPath) {
    vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path());
        }
    }
    return files;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    string directoryPath = "../data/reviews/";
    string keyword = "movie";
    vector<string> allFiles;
    vector<string> assignedFiles;
    int localCount = 0;

    auto start = high_resolution_clock::now();

    // Step 1: Master process gathers file list
    if (world_rank == 0) {
        auto files = get_txt_files(directoryPath);
        for (const auto& f : files) {
            allFiles.push_back(f.string());
        }
    }

    // Step 2: Broadcast total number of files
    int totalFiles = allFiles.size();
    MPI_Bcast(&totalFiles, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Step 3: Broadcast file names
    if (world_rank != 0) {
        allFiles.resize(totalFiles);
    }

    for (int i = 0; i < totalFiles; ++i) {
        char buffer[512] = {0};
        if (world_rank == 0) {
            strncpy(buffer, allFiles[i].c_str(), sizeof(buffer));
        }
        MPI_Bcast(buffer, sizeof(buffer), MPI_CHAR, 0, MPI_COMM_WORLD);
        if (world_rank != 0) {
            allFiles[i] = string(buffer);
        }
    }

    // Step 4: Assign files to each MPI process
    for (int i = 0; i < totalFiles; ++i) {
        if (i % world_size == world_rank) {
            assignedFiles.push_back(allFiles[i]);
        }
    }

    // Step 5: OpenMP parallel file processing
    #pragma omp parallel for reduction(+:localCount) schedule(dynamic)
    for (int i = 0; i < assignedFiles.size(); ++i) {
        ifstream file(assignedFiles[i]);
        if (!file.is_open()) {
            #pragma omp critical
            cerr << "Rank " << world_rank << ": Cannot open file: " << assignedFiles[i] << "\n";
            continue;
        }

        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        // Parallel keyword search in lines
        #pragma omp parallel for reduction(+:localCount)
        for (int j = 0; j < lines.size(); ++j) {
            if (lines[j].find(keyword) != string::npos) {
                localCount++;
            }
        }
    }

    // Step 6: Reduce counts across all MPI ranks
    int globalCount = 0;
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Step 7: Output (only from master)
    if (world_rank == 0) {
        cout << "== HYBRID Version (MPI + OpenMP) ==\n";
        cout << "Directory: " << directoryPath << "\n";
        cout << "Keyword used: \"" << keyword << "\"\n";
        cout << "Total Reviews Containing the Keyword: " << globalCount << "\n";
        cout << "No of MPI Processes: " << world_size << "\n";
        cout << "OpenMP Threads per Process: " << omp_get_max_threads() << "\n";
        cout << "Time Taken: " << duration.count() << " ms\n";
        cout << "=====================================\n";
    }

    MPI_Finalize();
    return 0;
}

