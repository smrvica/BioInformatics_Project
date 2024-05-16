#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <sys/resource.h>
#include <stdint.h>
#include <initializer_list>

#include "cf/cuckooFilter.h"

typedef uint16_t fp_type;
static const int bits_per_fp = 16;

std::string generateRandomSubsequence(const std::string &genome, int k)
{
    int start = rand() % (genome.size() - k + 1);
    return genome.substr(start, k);
}

void performTest(int k, const std::string &genome)
{
    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(1000, 100, 10, bits_per_fp);

    auto startInsertion = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i)
    {
        std::string randomSubsequence = generateRandomSubsequence(genome, k);
        cf.insert(randomSubsequence.c_str());
    }
    auto endInsertion = std::chrono::high_resolution_clock::now();
    auto insertionTime = std::chrono::duration_cast<std::chrono::microseconds>(endInsertion - startInsertion).count();

    auto startQuery = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i)
    {
        std::string randomSubsequence = generateRandomSubsequence(genome, k);
        cf.lookup(randomSubsequence.c_str());
    }
    auto endQuery = std::chrono::high_resolution_clock::now();
    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(endQuery - startQuery).count();

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long memoryUsage = usage.ru_maxrss / 1028;

    std::cout << "k = " << k << ", Insertion Time: " << insertionTime << " microseconds, Query Time: " << queryTime << ", Memory Usage: " << memoryUsage << " MB" << std::endl;
}

int main()
{
    std::ifstream eColiGenome("e_coli_genome.fna");
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

    for (int k : {10, 20, 50, 100, 200})
    {
        std::cout << "Starting... k = " << k << "\n";
        performTest(k, genome);
    }

    return 0;
}
