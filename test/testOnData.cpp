#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <sys/resource.h>
#include <cstdint>
#include <initializer_list>

// g++ -std=c++11 test/testOnData.cpp -o testOnData

#include "../cf/LDCF.h"

typedef uint32_t fp_type;
// static const int bits_per_fp = 25;

void performTest(int bits_per_fp, int k, const std::string &genome, const std::string &filepath)
{
    struct rusage usageBefore, usageAfter;
    getrusage(RUSAGE_SELF, &usageBefore);
    long memoryUsageBefore = usageBefore.ru_maxrss;

    LDCF<fp_type> cf(20, 10, 5, bits_per_fp);
    Victim victim;

    std::cout << "GENOME LEN: " << genome.length() << "\n";

    auto startInsertion = std::chrono::high_resolution_clock::now();
    int numOfAdded = 0;
    for (size_t i = 0; i < genome.length() / k; ++i)
    {
        std::string subsequence = genome.substr(i * k, k);
        if (cf.insert(subsequence.c_str(), victim))
        {
            numOfAdded++;
        }
    }
    auto endInsertion = std::chrono::high_resolution_clock::now();
    auto insertionTime = std::chrono::duration_cast<std::chrono::microseconds>(endInsertion - startInsertion).count();

    int counter = 0;
    auto startQuery = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < genome.length() / k; ++i)
    {
        std::string subsequence = genome.substr(i * k, k);
        int result = cf.lookup(subsequence.c_str());
        if (result)
        {
            counter++;
        }
    }
    auto endQuery = std::chrono::high_resolution_clock::now();
    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(endQuery - startQuery).count();

    std::cout << "Number of added: " << numOfAdded << " Number of correctly found: " << counter << "\n";
    getrusage(RUSAGE_SELF, &usageAfter);
    long memoryUsageAfter = usageAfter.ru_maxrss;

    long memoryUsage = usageAfter.ru_maxrss / 1024; // Corrected division factor

    auto startFalseLookUp = std::chrono::high_resolution_clock::now();
    std::string filename = filepath + std::to_string(k) + ".txt";
    std::ifstream simulatedData(filename);
    std::string line;
    float sum = 0;
    float cnt = 0;
    while (std::getline(simulatedData, line))
    {
        int result = cf.lookup(line.c_str());
        std::cout << result;
        if (result)
        {
            sum++;
        }
        cnt++;
    }
    std::cout << "\n";
    std::cout << "FALSE POSITIVES: " << sum / cnt * 100 << "%\n";
    simulatedData.close();

    auto endFalseLookUp = std::chrono::high_resolution_clock::now();
    auto falseLookUpTime = std::chrono::duration_cast<std::chrono::microseconds>(endFalseLookUp - startFalseLookUp).count();

    auto startDelete = std::chrono::high_resolution_clock::now();
    int numOfDeleted = 0;
    for (size_t i = 0; i < genome.length() / k; ++i)
    {
        std::string subsequence = genome.substr(i * k, k);
        int result = cf.lookup(subsequence.c_str());
        if (result)
        {
            numOfDeleted++;
        }
    }
    auto endDelete = std::chrono::high_resolution_clock::now();
    auto deleteTime = std::chrono::duration_cast<std::chrono::microseconds>(endDelete - startDelete).count();

    std::cout << "k = " << k << ", Insertion Time: " << insertionTime << " microseconds, Query Time: " << queryTime << " microseconds, False Look Up Time: " << falseLookUpTime << " microseconds, Delete Time: " << deleteTime << " microseconds\n"
              << std::endl;
}

std::string readGenomeFromFile(const std::string &filename)
{
    std::ifstream genomeFile(filename);
    std::string line, genome;
    while (std::getline(genomeFile, line))
    {
        if (line[0] != '>')
        {
            genome += line;
        }
    }
    genomeFile.close();
    return genome;
}

int main(int argc, char *argv[])
{
    std::string genome = readGenomeFromFile(argv[1]);

    srand(static_cast<unsigned>(time(NULL)));

    std::cout << "Starting... k = " << argv[3] << " bits per fingerprint = " << argv[4] << "\n";
    performTest(std::stoi(argv[4]), std::stoi(argv[3]), genome, argv[2]);
}
