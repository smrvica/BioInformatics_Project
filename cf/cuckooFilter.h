#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

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

    int ctr;

public:

    CuckooFilter<fp_size> *next;
    CuckooFilter<fp_size> *prev;

    CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves, int bits_per_fp);

    ~CuckooFilter();

    bool insert(const char *key, Victim &victim);

    bool insertFP(fp_size fp, uint64_t index, Victim &victim);

    bool lookup(const char *key);

    bool deleteKey(const char *key);

    void print();

    uint32_t fingerprint(const char *key);

    bool isFull() {
        return ctr == capacity * bucket_size;
    }

    bool isEmpty() {
        return ctr == 0;
    }

    int getCapacity() {
        return capacity;
    }

    int getBucketSize() {
        return bucket_size;
    }

    int getBitsPerFp() {
        return bits_per_fp;
    }

    int getMaxNoOfMoves() {
        return maxNoOfMoves;
    }
};


template<typename fp_size>
CuckooFilter<fp_size>::CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves, int bits_per_fp) {
    int findCapacity = 1;
    while (findCapacity <= capacity / 2) {
        findCapacity *= 2;
    }
    this->capacity = findCapacity;
    this->bucket_size = bucket_size;
    this->buckets = new Bucket<fp_size>[findCapacity];
    for (int i = 0; i < findCapacity; i++) {
        this->buckets[i].setBucket(bucket_size, bits_per_fp);
    }
    this->maxNoOfMoves = maxNoOfMoves;
    this->bits_per_fp = bits_per_fp;
    this->fp_mask = (1 << bits_per_fp) - 1;
    this->next = nullptr;
    this->prev = nullptr;
    this->ctr = 0;
}

template<typename fp_size>
CuckooFilter<fp_size>::~CuckooFilter() {
    delete[] buckets;
}

template<typename fp_size>
bool CuckooFilter<fp_size>::insert(const char *key, Victim &victim) {
    fp_size fp = fingerprint(key);
    uint64_t index = hash(key);
    return insertFP(fp, index, victim);
}


template<typename fp_size>
bool CuckooFilter<fp_size>::insertFP(fp_size fp, uint64_t index, Victim &victim) {
    uint64_t i1 = index & (capacity - 1);
    uint64_t i2 = (i1 ^ hash(fp)) & (capacity - 1);

    if (buckets[i1].hasEmptyEntry()) {
        buckets[i1].insert(fp);
        ctr += 1;
        return true;
    } else if (buckets[i2].hasEmptyEntry()) {
        buckets[i2].insert(fp);
        ctr += 1;
        return true;
    }
    uint64_t i = (rand() % 2 == 0) ? i1 : i2;
    for (int n = 0; n < maxNoOfMoves; n++) {
        int e = rand() % bucket_size;
        fp_size temp = buckets[i].getFingerprint(e);
        buckets[i].deleteKey(temp);
        buckets[i].insert(fp);
        fp = temp;
        i = (i ^ hash(fp)) & (capacity - 1);
        if (buckets[i].hasEmptyEntry()) {
            buckets[i].insert(fp);
            ctr += 1;
            return true;
        }
    }
    // std::cout << "Victim: " << fp << " " << i << std::endl;  // debug
    victim.index = i;
    victim.fingerprint = fp;
    return false;
}


template<typename fp_size>
bool CuckooFilter<fp_size>::lookup(const char *key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) & (capacity - 1);
    uint64_t i2 = (i1 ^ hash(fp)) & (capacity - 1);
    if (buckets[i1].lookup(fp) || buckets[i2].lookup(fp)) {
        return true;
    }
    return false;
}

template<typename fp_size>
bool CuckooFilter<fp_size>::deleteKey(const char *key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) & (capacity - 1);
    uint64_t i2 = (i1 ^ hash(fp)) & (capacity - 1);
    if (buckets[i1].deleteKey(fp) || buckets[i2].deleteKey(fp)) {
        ctr -= 1;
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
uint32_t CuckooFilter<fp_size>::fingerprint(const char *key) {
    uint32_t hashValue = hash(key);
    uint32_t fingerprint = hashValue & fp_mask;
    if (fingerprint == 0) {
        fingerprint = 1;
    }
    //std::cout << "Fingerprint: " << fingerprint << "for: " << key << std::endl;  // debug
    return fingerprint;
}

#endif //CUCKOOFILTER_H
