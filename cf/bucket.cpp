#include "bucket.h"

template<size_t fp_size>
Bucket::Bucket(int bucket_size) {
    this->bucket_size = bucket_size;
    this->fingerprints = new fp_size[bucket_size];
    for (int i = 0; i < bucket_size; i++) {
        this->fingerprints[i] = 0;
    }
}

template<size_t fp_size>
Bucket::~Bucket() {
    delete[] fingerprints;
}

template<size_t fp_size>
bool Bucket::insert(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == 0) {
            fingerprints[i] = fingerprint;
            return true;
        }
    }
    return false;
}

template<size_t fp_size>
bool Bucket::lookup(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == fingerprint) {
            return true;
        }
    }
    return false;
}

template<size_t fp_size>
bool Bucket::deleteKey(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == fingerprint) {
            fingerprints[i] = 0;
            return true;
        }
    }
    return false;
}

template<size_t fp_size>
bool Bucket::hasEmptyEntry() {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == 0) {
            return true;
        }
    }
    return false;
}

template<size_t fp_size>
int Bucket::getBucketSize() {
    return bucket_size;
}

template<size_t fp_size>
int Bucket::getFingerprint(int index) {
    return fingerprints[index];
}
