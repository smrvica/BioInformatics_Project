

typedef struct Victim {
    size_t index;
    uint64_t fingerprint;
} Victim;


template<typename fp_size>
class Bucket {
private:
    int bucket_size;
    int bits_per_fp;
    fp_size *fingerprints;

public:
    Bucket(int bucket_size, int bits_per_fp);

    Bucket();

    ~Bucket();

    void setBucket(int bucket_size, int bits_per_fp);

    bool insert(fp_size fingerprint);

    bool lookup(fp_size fingerprint);

    bool deleteKey(fp_size fingerprint);

    bool hasEmptyEntry();

    int getBucketSize();

    fp_size getFingerprint(int index);
};


template<typename fp_size>
Bucket<fp_size>::Bucket(int bucket_size, int bits_per_fp) {
    this->bucket_size = bucket_size;
    this->fingerprints = new fp_size[bucket_size];
    this->bits_per_fp = bits_per_fp;
    for (int i = 0; i < bucket_size; i++) {
        this->fingerprints[i] = 0;
    }
}

template<typename fp_size>
Bucket<fp_size>::Bucket() {
    this->bucket_size = 0;
    this->fingerprints = new fp_size[bucket_size];
    this->bits_per_fp = 0;
    for (int i = 0; i < bucket_size; i++) {
        this->fingerprints[i] = 0;
    }
}

template<typename fp_size>
Bucket<fp_size>::~Bucket() {
    delete[] fingerprints;
}

template<typename fp_size>
void Bucket<fp_size>::setBucket(int bucket_size, int bits_per_fp) {
    this->bucket_size = bucket_size;
    this->fingerprints = new fp_size[bucket_size];
    this->bits_per_fp = bits_per_fp;
    for (int i = 0; i < bucket_size; i++) {
        this->fingerprints[i] = 0;
    }
}

template<typename fp_size>
bool Bucket<fp_size>::insert(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == 0) {
            fingerprints[i] = fingerprint;
            return true;
        }
    }
    return false;
}

template<typename fp_size>
bool Bucket<fp_size>::lookup(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == fingerprint) {
            return true;
        }
    }
    return false;
}

template<typename fp_size>
bool Bucket<fp_size>::deleteKey(fp_size fingerprint) {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == fingerprint) {
            fingerprints[i] = 0;
            return true;
        }
    }
    return false;
}

template<typename fp_size>
bool Bucket<fp_size>::hasEmptyEntry() {
    for (int i = 0; i < bucket_size; i++) {
        if (fingerprints[i] == 0) {
            return true;
        }
    }
    return false;
}

template<typename fp_size>
int Bucket<fp_size>::getBucketSize() {
    return bucket_size;
}

template<typename fp_size>
fp_size Bucket<fp_size>::getFingerprint(int index) {
    return fingerprints[index];
}


