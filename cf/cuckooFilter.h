#ifndef CUCKOOFILTER_H
#define CUCKOOFILTER_H

#include <iostream>
#include <string>
#include <stdint.h>
#include "../utils/hashFunctions.h"
#include "bucket.h"

/**
 * CuckooFilter class is a data structure that stores fingerprints of keys and is used to check if a key is present in the filter.
 * @tparam fp_size size of the fingerprint
 */
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

    /**
     * Constructor for CuckooFilter.
     * @param capacity number of rows in the cuckoo table
     * @param bucket_size size of one bucket (row)
     * @param maxNoOfMoves maximum number of kicks before giving up
     * @param bits_per_fp size of the fingerprint
     */
    CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves, int bits_per_fp);

    /**
     * Destructor for CuckooFilter.
     */
    ~CuckooFilter();

    /**
     * Inserts a key into the filter.
     * @param key key to be inserted
     * @param victim allocated space for the victim
     * @return true if the key was inserted successfully, false otherwise
     */
    bool insert(const char *key, Victim &victim);

    /**
     * Inserts a fingerprint into the filter.
     * @param fp fingerprint to be inserted
     * @param index index of the bucket where the fingerprint should be inserted
     * @param victim allocated space for the victim
     * @return true if the fingerprint was inserted successfully, false otherwise
     */
    bool insertFP(fp_size fp, uint64_t index, Victim &victim);

    /**
     * Looks up a key in the filter.
     * @param key key to be looked up
     * @return true if the key is present in the filter, false otherwise
     */
    bool lookup(const char *key);

    /**
     * Deletes a key from the filter.
     * @param key key to be deleted
     * @return true if the key was deleted successfully, false otherwise
     */
    bool deleteKey(const char *key);

    /**
     * Prints the contents of the filter.
     */
    void print();

    /**
     * Calculates the fingerprint of a key.
     * @param key key for which the fingerprint is calculated
     * @return fingerprint of the key
     */
    uint32_t fingerprint(const char *key);

    /**
     * Checks if the filter is full.
     * @return true if the filter is full, false otherwise
     */
    bool isFull() {
        return ctr == capacity * bucket_size;
    }

    /**
     * Checks if the filter is empty.
     * @return true if the filter is empty, false otherwise
     */
    bool isEmpty() {
        return ctr == 0;
    }

    /**
     * Returns the number of buckets in the filter.
     * @return number of buckets in the filter
     */
    int getCapacity() {
        return capacity;
    }

    /**
     * Returns the size of one bucket in the filter.
     * @return size of one bucket in the filter
     */
    int getBucketSize() {
        return bucket_size;
    }

    /**
     * Returns the number of bits in the fingerprint.
     * @return number of bits in the fingerprint
     */
    int getBitsPerFp() {
        return bits_per_fp;
    }

    /**
     * Returns the maximum number of moves before giving up.
     * @return maximum number of moves before giving up
     */
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
    return fingerprint;
}

#endif //CUCKOOFILTER_H
