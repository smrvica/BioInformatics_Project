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
void LCDF<fp_size>::storeVictim(Victim &victim)
{
    // std::cout << "STORING VICTIM\n"; // debug
    CuckooNode<fp_size> *curr = root;
    // bool bit = 0;
    int depth = 0;
    while (!curr->getCurr()->insertFP(victim.fingerprint, victim.index, victim) && depth < bitsPerFp)
    {
        bool bit = (victim.fingerprint >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            if (curr->hasRight())
            {
                curr = curr->getRight();
            }
            else
            {
                curr = curr->getNewRightCF();
            }
        }
        else
        {
            if (curr->hasLeft())
            {
                curr = curr->getLeft();
            }
            else
            {
                curr = curr->getNewLeftCF();
            }
        }
        depth++;
    }
}

template <typename fp_size>
LCDF<fp_size>::LCDF(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    this->singleTableCapacity = singleTableCapacity;
    this->bucketSize = bucketSize;
    this->maxNoOfMoves = maxNoOfMoves;
    this->bitsPerFp = bitsPerFp;

    root = new CuckooNode<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
}

template <typename fp_size>
LCDF<fp_size>::~LCDF()
{
    delete root;
}

template <typename fp_size>
bool LCDF<fp_size>::insert(const char *key, Victim &victim)
{
    CuckooNode<fp_size> *curr = root;
    // bool bit = 0;
    int depth = 0;
    CuckooFilter<fp_size> *cf = curr->getCurr();
    while (depth < bitsPerFp)
    {
        if (cf->isFull())
        {
            bool bit = (cf->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
            if (bit)
            {
                if (curr->hasRight())
                {
                    curr = curr->getRight();
                }
                else
                {
                    curr = curr->getNewRightCF();
                }
                cf = curr->getCurr();
            }
            else
            {
                if (curr->hasLeft())
                {
                    curr = curr->getLeft();
                }
                else
                {
                    curr = curr->getNewLeftCF();
                }
                cf = curr->getCurr();
            }
            depth++;
            continue;
        }
        if (cf->insert(key, victim))
        {
            return true;
        }
        storeVictim(victim);
        return true;
    }
    return false;
}

template <typename fp_size>
bool LCDF<fp_size>::lookup(const char *key)
{
    CuckooNode<fp_size> *curr = root;
    // bool bit = false;
    int depth = 0;
    CuckooFilter<fp_size> *cf = curr->getCurr();
    while (depth < bitsPerFp)
    {
        if (cf->lookup(key))
        {
            return true;
        }
        bool bit = (cf->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            if (curr->hasRight())
            {
                curr = curr->getRight();
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (curr->hasLeft())
            {
                curr = curr->getLeft();
            }
            else
            {
                return false;
            }
        }
        cf = curr->getCurr();
        depth++;
    }
    return false;
}

template <typename fp_size>
bool LCDF<fp_size>::deleteKey(const char *key)
{
    CuckooNode<fp_size> *curr = root;
    // bool bit = 0;
    int depth = 0;
    CuckooFilter<fp_size> *cf = curr->getCurr();
    while (depth < bitsPerFp)
    {
        if (cf->deleteKey(key))
        {
            return true;
        }
        bool bit = (cf->fingerprint(key) >> (bitsPerFp - 1 - depth)) & 0x1;
        if (bit)
        {
            if (curr->hasRight())
            {
                curr = curr->getRight();
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (curr->hasLeft())
            {
                curr = curr->getLeft();
            }
            else
            {
                return false;
            }
        }
        cf = curr->getCurr();
        depth++;
    }
    return false;
}

template <typename fp_size>
void LCDF<fp_size>::print()
{
    std::cout << "LCDF\n";
    root->print();
}
