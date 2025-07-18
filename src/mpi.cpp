#include <mpi.h>
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
    int localCount = 0;
    vector<string> assignedFiles;

    auto start = high_resolution_clock::now();

    // Step 1: Rank 0 gathers and distributes files
    vector<string> allFiles;

    if (world_rank == 0) {
        auto filePaths = get_txt_files(directoryPath);
        for (const auto& path : filePaths) {
            allFiles.push_back(path.string());
        }
    }

    // Step 2: Broadcast number of files
    int totalFiles = allFiles.size();
    MPI_Bcast(&totalFiles, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Step 3: Broadcast file list
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

    // Step 4: Divide files among processes
    for (int i = 0; i < totalFiles; ++i) {
        if (i % world_size == world_rank) {
            assignedFiles.push_back(allFiles[i]);
        }
    }

    // Step 5: Search keyword in assigned files
    for (const auto& filePath : assignedFiles) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Rank " << world_rank << ": Warning: Cannot open " << filePath << "\n";
            continue;
        }

        string line;
        while (getline(file, line)) {
            if (line.find(keyword) != string::npos) {
                localCount++;
            }
        }
        file.close();
    }

    // Step 6: Reduce total counts
    int globalCount = 0;
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Step 7: Master prints results
    if (world_rank == 0) {
        cout << "== MPI Version (Multi-file) ==\n";
        cout << "Directory: " << directoryPath << "\n";
        cout << "Keyword used: \"" << keyword << "\"\n";
        cout << "Total Reviews Containing the Keyword: " << globalCount << "\n";
        cout << "No of MPI Processes: " << world_size << "\n";
        cout << "Time Taken: " << duration.count() << " ms\n";
        cout << "===============================\n";
    }

    MPI_Finalize();
    return 0;
}

