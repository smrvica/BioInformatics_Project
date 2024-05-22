#include "cuckooFilter.h"

template <typename fp_size>
struct CuckooNode
{
    CuckooFilter<fp_size> *curr;
    CuckooNode<fp_size> *left;
    CuckooNode<fp_size> *right;

    CuckooNode(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);

    virtual ~CuckooNode()
    {
        delete curr;
        delete left;
        delete right;
    }

    CuckooNode<fp_size> *getNewLeftCF();
    CuckooNode<fp_size> *getNewRightCF();
    void print();
};

template <typename fp_size>
inline CuckooNode<fp_size>::CuckooNode(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    CuckooFilter<fp_size> *newCF = new CuckooFilter<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
    curr = newCF;
}

template <typename fp_size>
CuckooNode<fp_size> *CuckooNode<fp_size>::getNewRightCF()
{
    CuckooNode<fp_size> *newNode = new CuckooNode<fp_size>(curr->getCapacity(), curr->getBucketSize(), curr->getMaxNoOfMoves(), curr->getBitsPerFp() - 1);

    right = newNode;

    return newNode;
}

template <typename fp_size>
CuckooNode<fp_size> *CuckooNode<fp_size>::getNewLeftCF()
{
    CuckooNode<fp_size> *newNode = new CuckooNode<fp_size>(curr->getCapacity(), curr->getBucketSize(), curr->getMaxNoOfMoves(), curr->getBitsPerFp() - 1);

    left = newNode;

    return newNode;
}

template <typename fp_size>
void CuckooNode<fp_size>::print()
{
    curr->print();
    left->print();
    right->print();
}