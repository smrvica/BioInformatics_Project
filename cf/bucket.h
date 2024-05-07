
template<size_t fp_size>
class Bucket {
private:
    int bucket_size;
    fp_size *fingerprints;

public:
    Bucket(int bucket_size);

    ~Bucket();

    bool insert(fp_size fingerprint);

    bool lookup(fp_size fingerprint);

    bool deleteKey(fp_size fingerprint);

    bool hasEmptyEntry();

    int getBucketSize();

    fp_size getFingerprint(int index);
};

