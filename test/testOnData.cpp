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

#include "../cf/LCDF.h"

typedef uint16_t fp_type;
static const int bits_per_fp = 16;

void performTest(int k, const std::string &genome, const std::string &filepath)
{
    struct rusage usageBefore, usageAfter;
    getrusage(RUSAGE_SELF, &usageBefore);
    long memoryUsageBefore = usageBefore.ru_maxrss;

    LCDF<fp_type> cf(20, 10, 5, bits_per_fp);
    Victim victim;

    std::cout << "GENOME LEN: " << genome.length() << "\n";

    auto startInsertion = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < genome.length() / k; ++i)
    {
        std::string subsequence = genome.substr(i * k, k);
        cf.insert(subsequence.c_str(), victim);
    }
    auto endInsertion = std::chrono::high_resolution_clock::now();
    auto insertionTime = std::chrono::duration_cast<std::chrono::microseconds>(endInsertion - startInsertion).count();

    auto startQuery = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < genome.length() / k; ++i)
    {
        std::string subsequence = genome.substr(i * k, k);
        int result = cf.lookup(subsequence.c_str());
    }
    auto endQuery = std::chrono::high_resolution_clock::now();
    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(endQuery - startQuery).count();

    getrusage(RUSAGE_SELF, &usageAfter);
    long memoryUsageAfter = usageAfter.ru_maxrss;

    long memoryUsage = usageAfter.ru_maxrss / 1024; // Corrected division factor

    auto startFalseLookUp = std::chrono::high_resolution_clock::now();
    std::string filename = filepath + std::to_string(k) + ".txt";
    std::ifstream simulatedData(filename);
    std::string line;
    int sum = 0;
    while (std::getline(simulatedData, line))
    {
        int result = cf.lookup(line.c_str());
        if (result)
        {
            sum++;
        }
    }
    std::cout << "FALSE POSITIVES: " << sum << "%\n";
    simulatedData.close();

    auto endFalseLookUp = std::chrono::high_resolution_clock::now();
    auto falseLookUpTime = std::chrono::duration_cast<std::chrono::microseconds>(endFalseLookUp - startFalseLookUp).count();

    std::cout << "k = " << k << ", Insertion Time: " << insertionTime << " microseconds, Query Time: " << queryTime << " microseconds, False Look Up Time: " << falseLookUpTime << " microseconds, Memory Usage: " << memoryUsage << " MB" << std::endl;
    std::cout << "Memory Usage Before: " << memoryUsageBefore << " KB, Memory Usage After: " << memoryUsageAfter << " KB, Memory difference: " << (memoryUsageAfter - memoryUsageBefore) << " KB\n\n\n";
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

int main()
{
    std::string genome = readGenomeFromFile("data/e_coli_genome.fna");

    srand(static_cast<unsigned>(time(NULL)));

    std::cout << "\n\n\n===============================\n"
              << "SIMULATING ON REAL E. COLI GENOME\n"
              << "===============================\n";
    for (int k : {10, 20, 50, 100, 200})
    {
        std::cout << "Starting... k = " << k << "\n";
        performTest(k, genome, "data/new_kmers_sim_");
    }

    for (int i : {3, 4, 5, 6, 7})
    {
        std::string filename = "data/len_" + std::to_string(i) + "/sequence.fna";
        std::string genome = readGenomeFromFile(filename);
        std::string filepath = "data/len_" + std::to_string(i) + "/new_kmers_" + std::to_string(i) + "_";

        std::cout << "===============================\n"
                  << "SIMULATING ON RANDOMLY GENERATED GENOME 10^" << i << "\n"
                  << "===============================\n";
        for (int k : {10, 20, 50, 100, 200})
        {
            std::cout << "\n\nStarting... k = " << k << "\n";
            performTest(k, genome, filepath);
        }
    }
    return 0;
}
