#include "cuckooFilter.h"

template<size_t fp_size>
CuckooFilter::CuckooFilter(int capacity, int fingerprint_size, int bucket_size, int maxNoOfMoves) {
    this->capacity = capacity;
    this->fingerprint_size = fingerprint_size;
    this->bucket_size = bucket_size;
    this->buckets = new Bucket*[capacity];
    for (int i = 0; i < capacity; i++) {
        this->buckets[i] = new Bucket(bucket_size);
    }
    this->maxNoOfMoves = maxNoOfMoves;
}

template<size_t fp_size>
CuckooFilter::~CuckooFilter() {
    delete[] buckets;
}

template<size_t fp_size>
bool CuckooFilter::insert(const char* key) {
    string fp = fingerprint(key);
    i1 = hash(key);
    i2 = i1 ^ hash(fp);

    if (buckets[i1].hasEmptyEntry()) {
        buckets[i1].insert(fp);
        return true;
    } else if (buckets[i2].hasEmptyEntry()) {
        buckets[i2].insert(fp);
        return true;
    }
    i = rand() % 2 == 0 ? i1 : i2;
    for (int n = 0; n < maxNoOfMoves; n++) {
        int e = rand() % bucket_size;
        string temp = buckets[i].getFingerprint(e);
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
    string fp = fingerprint(key);
    i1 = hash(key);
    i2 = i1 ^ hash(fp);
    if (buckets[i1].lookup(fp) || buckets[i2].lookup(fp)) {
        return true;
    }
    return false;
}

template<size_t fp_size>
bool CuckooFilter::deleteKey(const char* key) {
    string fp = fingerprint(key);
    i1 = hash(key);
    i2 = i1 ^ hash(fp);
    if (buckets[i1].deleteKey(fp) || buckets[i2].deleteKey(fp)) {
        return true;
    }
    return false;
}
