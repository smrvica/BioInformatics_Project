
#include <iostream>
#include <string>
#include <stdint.h>
#include "../utils/hashFunctions.h"
#include "bucket.h"

template<typename fp_size>
class CuckooFilter {
private:
    int capacity;
    int bucket_size;
    Bucket<fp_size> *buckets;
    int maxNoOfMoves;
    int bits_per_fp;
    uint32_t fp_mask;

public:
    CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves, int bits_per_fp);
    ~CuckooFilter();
    bool insert(const char* key);
    bool lookup(const char* key);
    bool deleteKey(const char* key);
    void print();
    uint32_t fingerprint(const char* key);
};



template<typename fp_size>
CuckooFilter<fp_size>::CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves, int bits_per_fp) {
    this->capacity = capacity;
    this->bucket_size = bucket_size;
    this->buckets = new Bucket<fp_size>[capacity];
    for (int i = 0; i < capacity; i++) {
        this->buckets[i].setBucket(bucket_size, bits_per_fp);
    }
    this->maxNoOfMoves = maxNoOfMoves;
    this->bits_per_fp = bits_per_fp;
    this->fp_mask = (1 << bits_per_fp) - 1;
}

template<typename fp_size>
CuckooFilter<fp_size>::~CuckooFilter() {
    delete[] buckets;
}

template<typename fp_size>
bool CuckooFilter<fp_size>::insert(const char* key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) % capacity;
    uint64_t i2 = (i1 ^ hash(fp)) % capacity;

    if (buckets[i1].hasEmptyEntry()) {
        buckets[i1].insert(fp);
        return true;
    } else if (buckets[i2].hasEmptyEntry()) {
        buckets[i2].insert(fp);
        return true;
    }
    uint64_t i = rand() % 2 == 0 ? i1 : i2;
    for (int n = 0; n < maxNoOfMoves; n++) {
        int e = rand() % bucket_size;
        fp_size temp = buckets[i].getFingerprint(e);
        buckets[i].insert(fp);
        fp = temp;
        i = i ^ hash(fp);
        if (buckets[i].hasEmptyEntry()) {
            buckets[i].insert(fp);
            return true;
        }
    }
    return false;
}

template<typename fp_size>
bool CuckooFilter<fp_size>::lookup(const char* key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) % capacity;
    uint64_t i2 = (i1 ^ hash(fp)) % capacity;
    if (buckets[i1].lookup(fp) || buckets[i2].lookup(fp)) {
        return true;
    }
    return false;
}

template<typename fp_size>
bool CuckooFilter<fp_size>::deleteKey(const char* key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) % capacity;
    uint64_t i2 = (i1 ^ hash(fp)) % capacity;
    if (buckets[i1].deleteKey(fp) || buckets[i2].deleteKey(fp)) {
        return true;
    }
    return false;
}

template<typename fp_size>
void CuckooFilter<fp_size>::print() {
    for (int i = 0; i < capacity; i++) {
        printf("Bucket %d: ", i);
        for (int j = 0; j < bucket_size; j++) {
            printf("%d ", buckets[i].getFingerprint(j));
        }
        printf("\n");
    }
}

template<typename fp_size>
uint32_t CuckooFilter<fp_size>::fingerprint(const char* key) {
    uint32_t hashValue = hash(key);
    uint32_t fingerprint = hashValue & fp_mask;
    if (fingerprint == 0) {
        fingerprint = 1;
    }
    return fingerprint;
}


