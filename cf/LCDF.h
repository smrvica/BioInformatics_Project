#include "cuckooNode.h"

template <typename fp_size>
class LCDF
{
private:
    int singleTableCapacity;
    int bucketSize;
    int maxNoOfMoves;
    int bitsPerFp;

    CuckooNode<fp_size> *root;

    void storeVictim(Victim &victim);

public:
    LCDF(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);
    ~LCDF();

    bool insert(const char *key, Victim &victim);
    bool lookup(const char *key);
    bool deleteKey(const char *key);

    void print();
};

template <typename fp_size>
inline LCDF<fp_size>::LCDF(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    this->singleTableCapacity = singleTableCapacity;
    this->bucketSize = bucketSize;
    this->maxNoOfMoves = maxNoOfMoves;
    this->bitsPerFp = bitsPerFp;

    root = new CuckooNode<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
}

template <typename fp_size>
inline LCDF<fp_size>::~LCDF()
{
    delete root;
}

template <typename fp_size>
inline bool LCDF<fp_size>::insert(const char *key, Victim &victim)
{
    CuckooNode<fp_size> *curr = root;
    bool bit = 0;
    int depth = 0;
    while (curr->curr->isFull() && depth < bitsPerFp)
    {
        bool bit = (curr->curr->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            if (!curr->right)
            {
                curr = curr->getNewRightCF();
            }
            else
            {
                curr = curr->right;
            }
        }
        else
        {
            if (!curr->left)
            {
                curr = curr->getNewLeftCF();
            }
            else
            {
                curr = curr->left;
            }
        }
        depth++;
    }
    if (!curr->curr->isFull())
    {
        bool success = curr->curr->insert(key, victim);
        return success;
    }
    return false;
}

template <typename fp_size>
inline bool LCDF<fp_size>::lookup(const char *key)
{
    CuckooNode<fp_size> *curr = root;
    bool bit = 0;
    int depth = 0;
    while (curr->curr && depth < bitsPerFp)
    {
        if (curr->curr->lookup(key))
        {
            return true;
        }
        bool bit = (curr->curr->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            curr = curr->right;
        }
        else
        {
            curr = curr->left;
        }
        depth++;
    }
    return false;
}

template <typename fp_size>
inline bool LCDF<fp_size>::deleteKey(const char *key)
{
    CuckooNode<fp_size> *curr = root;
    bool bit = 0;
    int depth = 0;
    while (curr->curr && depth < bitsPerFp)
    {
        if (curr->curr->deleteKey(key))
        {
            return true;
        }
        bool bit = (curr->curr->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            curr = curr->right;
        }
        else
        {
            curr = curr->left;
        }
        depth++;
    }
}

template <typename fp_size>
inline void LCDF<fp_size>::print()
{
}
