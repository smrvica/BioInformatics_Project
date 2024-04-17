
#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <iostream>
#include <string>
#include "hashFunctions.h"
#include "bucket.h"


class CuckooFilter {
private:
    int capacity;
    int fingerprint_size;
    int bucket_size;
    int maxNoOfMoves;
    Bucket **buckets;

public:
    CuckooFilter(int capacity, int fingerprint_size, int bucket_size, int maxNoOfMoves);
    ~CuckooFilter();
    bool insert(const char* key);
    bool lookup(const char* key);
    bool deleteKey(const char* key);
};


#endif //CUCKOOFILTER_H
