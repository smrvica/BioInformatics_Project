#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <sys/resource.h>
#include <stdint.h>
#include <initializer_list>

// g++ -std=c++11 test/testOnData.cpp -o testOnData

// #include "../cf/cuckooFilter.h"
#include "../src/cuckoofilter.h"

typedef uint16_t fp_type;
static const int bits_per_fp = 16;

void performTest(int k, const std::string &genome)
{
    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(1000, 100, 10, bits_per_fp);
    Victim victim;

    auto startInsertion = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i)
    {
        std::string randomSubsequence = genome.substr(i * k, k);
        cf.insert(randomSubsequence.c_str(), victim);
    }
    std::cout << "TU SAM\n";
    auto endInsertion = std::chrono::high_resolution_clock::now();
    auto insertionTime = std::chrono::duration_cast<std::chrono::microseconds>(endInsertion - startInsertion).count();

    auto startQuery = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i)
    {
        std::string randomSubsequence = genome.substr(i * k, k);
        int result = cf.lookup(randomSubsequence.c_str());
        if (i < 3)
        {
            std::cout << "Look up for: " << randomSubsequence << ", Result: " << result << "\n";
        }
    }
    auto endQuery = std::chrono::high_resolution_clock::now();
    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(endQuery - startQuery).count();

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long memoryUsage = usage.ru_maxrss / 1028;

    auto startFalseLookUp = std::chrono::high_resolution_clock::now();
    std::string false_lookup1(k, 'A');
    std::cout << "Look up for: " << false_lookup1 << ", Result: " << cf.lookup(false_lookup1.c_str()) << "\n";
    std::string false_lookup2(k, 'G');
    std::cout << "Look up for: " << false_lookup2 << ", Result: " << cf.lookup(false_lookup2.c_str()) << "\n";
    std::string false_lookup3(k, 'C');
    std::cout << "Look up for: " << false_lookup3 << ", Result: " << cf.lookup(false_lookup3.c_str()) << "\n";
    std::string false_lookup4(k, 'T');
    std::cout << "Look up for: " << false_lookup4 << ", Result: " << cf.lookup(false_lookup4.c_str()) << "\n";
    auto endFalseLookUp = std::chrono::high_resolution_clock::now();
    auto falseLookUpTime = std::chrono::duration_cast<std::chrono::microseconds>(endFalseLookUp - startFalseLookUp).count();

    std::cout << "k = " << k << ", Insertion Time: " << insertionTime << " microseconds, Query Time: " << queryTime << " microseconds, False Look Up Time: " << falseLookUpTime << " microseconds, Memory Usage: " << memoryUsage << " MB" << std::endl;
}

int main()
{
    std::ifstream eColiGenome("data/e_coli_genome.fna");
    std::string line, genome;
    while (std::getline(eColiGenome, line))
    {
        if (line[0] != '>')
        {
            genome += line;
        }
    }
    eColiGenome.close();

    srand(time(NULL));

    std::cout << "\n\n===============================\n"
              << "SIMULATING ON REAL E. COLI GENOME\n"
              << "===============================\n";
    for (int k : {10, 20, 50, 100, 200})
    {
        std::cout << "Starting... k = " << k << "\n";
        performTest(k, genome);
    }

    std::ifstream simulatedData("data/simulatedData.fna");
    genome = "";
    while (std::getline(simulatedData, line))
    {
        if (line[0] != '>')
        {
            genome += line;
        }
    }
    simulatedData.close();

    std::cout << "===============================\n"
              << "SIMULATING ON RANDOMLY GENERATED GENOME\n"
              << "===============================\n";
    for (int k : {10, 20, 50, 100, 200})
    {
        std::cout << "\n\nStarting... k = " << k << "\n";
        performTest(k, genome);
    }

    return 0;
}
