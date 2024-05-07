#ifndef BUCKET_H
#define BUCKET_H


class Bucket {
private:
    int bucket_size;
    int *fingerprints;

public:
    Bucket(int bucket_size);
    ~Bucket();
    bool insert(string fingerprint);  // TODO no idea what the type of the fingerprint is
    bool lookup(string fingerprint);
    bool deleteKey(string fingerprint);
    bool hasEmptyEntry();
    string getFingerprint(int index);
};


#endif //BUCKET_H
