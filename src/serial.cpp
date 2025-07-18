#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <filesystem>

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;

int main() {
    string directoryPath = "../data/reviews/"; 
    string keyword = "movie";
    int totalCount = 0;

    auto start = high_resolution_clock::now();

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        cerr << "Error: Invalid directory path.\n";
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt") {
            ifstream file(entry.path());
            if (!file.is_open()) {
                cerr << "Warning: Cannot open file: " << entry.path() << "\n";
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
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "== SERIAL Version (Multi-file) ==\n";
    cout << "Directory: " << directoryPath << "\n";
    cout << "Keyword used: \"" << keyword << "\"\n";
    cout << "Total Reviews Containing the Keyword: " << totalCount << "\n";
    cout << "No of Threads: 1\n";
    cout << "Time Taken: " << duration.count() << " ms\n";
    cout << "===============================\n";

    return 0;
}

