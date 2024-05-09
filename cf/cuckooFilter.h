
#include <iostream>
#include <string>
#include "../utils/hashFunctions.h"
#include "bucket.h"

template<size_t fp_size>
class CuckooFilter {
private:
    int capacity;
    int bucket_size;
    Bucket **buckets;
    int maxNoOfMoves;
    uint32_t fp_mask;

public:
    CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves);
    ~CuckooFilter();
    bool insert(const char* key);
    bool lookup(const char* key);
    bool deleteKey(const char* key);
    void print();
    uint32_t fingerprint(const char* key);
};

