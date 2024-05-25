
/**
 * structure for a victim of insertion, consists of index and fingerprint
 */
typedef struct Victim {
    size_t index = 0;
    uint64_t fingerprint = 0;
} Victim;

/**
 * Bucket class for storing fingerprints in a Cuckoo Filter
 * @tparam fp_size size of fingerprint
 */
template<typename fp_size>
class Bucket {
private:
    int bucket_size;
    int bits_per_fp;
    fp_size *fingerprints;

public:
    /**
     * Constructor for Bucket with bucket size and bits per fingerprint
     * @param bucket_size capacity of a bucket
     * @param bits_per_fp size of fingerprint
     */
    Bucket(int bucket_size, int bits_per_fp);

    /**
     * Default constructor for Bucket, expects setBucket to be called
     */
    Bucket();

    /**
     * Destructor for Bucket
     */
    ~Bucket();

    /**
     * Set the bucket size and bits per fingerprint, must be called after default constructor
     * @param bucket_size
     * @param bits_per_fp
     */
    void setBucket(int bucket_size, int bits_per_fp);

    /**
     * Insert a fingerprint into the bucket
     * @param fingerprint fingerprint to insert
     * @return true if inserted, false if bucket is full
     */
    bool insert(fp_size fingerprint);

    /**
     * Lookup a fingerprint in the bucket
     * @param fingerprint fingerprint to lookup
     * @return true if found, false if not found
     */
    bool lookup(fp_size fingerprint);

    /**
     * Delete a fingerprint from the bucket
     * @param fingerprint fingerprint to delete
     * @return true if deleted, false if not found
     */
    bool deleteKey(fp_size fingerprint);

    /**
     * Check if the bucket has an empty entry
     * @return true if empty entry found, false if no empty entry
     */
    bool hasEmptyEntry();

    /**
     * Get the size of the bucket
     * @return size of the bucket
     */
    int getBucketSize();

    /**
     * Get the fingerprint at an index
     * @param index index of fingerprint
     * @return fingerprint at index
     */
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


