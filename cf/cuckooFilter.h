
#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

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

public:
    CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves);
    ~CuckooFilter();
    bool insert(const char* key);
    bool lookup(const char* key);
    bool deleteKey(const char* key);
    void print();
};


#endif //CUCKOOFILTER_H
