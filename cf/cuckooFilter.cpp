#include "cuckooFilter.h"

template<size_t fp_size>
CuckooFilter::CuckooFilter(int capacity, int bucket_size, int maxNoOfMoves) {
    this->capacity = capacity;
    this->bucket_size = bucket_size;
    this->buckets = new Bucket*[capacity];
    for (int i = 0; i < capacity; i++) {
        this->buckets[i] = new Bucket(bucket_size);
    }
    this->maxNoOfMoves = maxNoOfMoves;
    this->fp_mask = (1 << fp_size) - 1;  // can we use template parameter here? TODO
}

template<size_t fp_size>
CuckooFilter::~CuckooFilter() {
    for (int i = 0; i < capacity; i++) {
        delete buckets[i];
    }
    delete[] buckets;
}

template<size_t fp_size>
bool CuckooFilter::insert(const char* key) {
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

template<size_t fp_size>
bool CuckooFilter::lookup(const char* key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) % capacity;
    uint64_t i2 = (i1 ^ hash(fp)) % capacity;
    if (buckets[i1].lookup(fp) || buckets[i2].lookup(fp)) {
        return true;
    }
    return false;
}

template<size_t fp_size>
bool CuckooFilter::deleteKey(const char* key) {
    fp_size fp = fingerprint(key);
    uint64_t i1 = hash(key) % capacity;
    uint64_t i2 = (i1 ^ hash(fp)) % capacity;
    if (buckets[i1].deleteKey(fp) || buckets[i2].deleteKey(fp)) {
        return true;
    }
    return false;
}

template<size_t fp_size>
void CuckooFilter::print() {
    for (int i = 0; i < capacity; i++) {
        printf("Bucket %d: ", i);
        for (int j = 0; j < bucket_size; j++) {
            printf("%d ", buckets[i].getFingerprint(j));
        }
        printf("\n");
    }
}

template<size_t fp_size>
uint32_t CuckooFilter::fingerprint(const char* key) {
    uint32_t hashValue = hash(key);
    uint32_t fingerprint = hashValue & fp_mask;
    if (fingerprint == 0) {
        fingerprint = 1;
    }
    return fingerprint;
}
