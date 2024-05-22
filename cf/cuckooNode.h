#include "cuckooFilter.h"

template <typename fp_size>
class CuckooNode
{
private:
    CuckooFilter<fp_size> *curr;
    CuckooNode<fp_size> *left;
    CuckooNode<fp_size> *right;

public:
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

    CuckooFilter<fp_size> *getCurr() { return curr; }
    CuckooNode<fp_size> *getLeft() { return left; }
    CuckooNode<fp_size> *getRight() { return right; }

    bool hasRight() { return right != nullptr; }
    bool hasLeft() { return left != nullptr; }
};

template <typename fp_size>
CuckooNode<fp_size>::CuckooNode(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    curr = new CuckooFilter<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
    left = nullptr;
    right = nullptr;
}

template <typename fp_size>
CuckooNode<fp_size> *CuckooNode<fp_size>::getNewRightCF()
{
    right = new CuckooNode<fp_size>(curr->getCapacity(), curr->getBucketSize(), curr->getMaxNoOfMoves(), curr->getBitsPerFp());
    return right;
}

template <typename fp_size>
CuckooNode<fp_size> *CuckooNode<fp_size>::getNewLeftCF()
{
    left = new CuckooNode<fp_size>(curr->getCapacity(), curr->getBucketSize(), curr->getMaxNoOfMoves(), curr->getBitsPerFp());
    return left;
}

template <typename fp_size>
void CuckooNode<fp_size>::print() {
    if (curr == nullptr) {
        return;
    }
    curr->print();
    if (left != nullptr) {
        left->print();
    }
    if (right != nullptr) {
        right->print();
    }
}