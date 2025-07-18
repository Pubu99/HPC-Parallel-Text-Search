#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <omp.h>

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // Check for number of threads
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <num_threads>\n";
        return 1;
    }

    string directoryPath = "../data/reviews/";  
    int numThreads = stoi(argv[1]);
    string keyword = "movie";
    int totalCount = 0;

    auto start = high_resolution_clock::now();

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        cerr << "Error: Invalid directory path: " << directoryPath << "\n";
        return 1;
    }

    // Collect all .txt files from directory
    vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path());
        }
    }

    omp_set_num_threads(numThreads); // Set the number of threads

    // Parallel search
    #pragma omp parallel for reduction(+:totalCount)
    for (int i = 0; i < files.size(); ++i) {
        ifstream file(files[i]);
        if (!file.is_open()) {
            #pragma omp critical
            cerr << "Warning: Cannot open file: " << files[i] << "\n";
            continue;
        }

        string line;
        while (getline(file, line)) {
            if (line.find(keyword) != string::npos) {
                totalCount++;
            }
        }
        file.close();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "== OPENMP Version (Multi-file) ==\n";
    cout << "Directory: " << directoryPath << "\n";
    cout << "Keyword used: \"" << keyword << "\"\n";
    cout << "Total Reviews Containing the Keyword: " << totalCount << "\n";
    cout << "No of Threads: " << numThreads << "\n";
    cout << "Time Taken: " << duration.count() << " ms\n";
    cout << "==================================\n";

    return 0;
}

