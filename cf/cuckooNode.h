#include "cuckooFilter.h"
/**
 * CuckooNode is a class structure which stores a CuckooFilter, and it is used to store next left and right node, in a tree structure
 * @tparam fp_size size of the fingerprint
 */
template <typename fp_size>
class CuckooNode
{
private:
    CuckooFilter<fp_size> *curr;
    CuckooNode<fp_size> *left;
    CuckooNode<fp_size> *right;

public:
    /**
     * @brief Construct a new Cuckoo Node object
     *
     * @param capacity number of rows in the cuckoo table
     * @param bucket_size size of one bucket (row)
     * @param maxNoOfMoves maximum number of kicks before giving up
     * @param bits_per_fp size of the fingerprint
     */
    CuckooNode(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);

    /**
     * @brief Destroy the Cuckoo Node object
     */
    virtual ~CuckooNode()
    {
        delete curr;
        delete left;
        delete right;
    }

    /**
     * @brief Makes a new Left Cuckoo Filter Node
     * @return a new left Cuckoo Filter Node
     */
    CuckooNode<fp_size> *getNewLeftCF();
    /**
     * @brief Makes a new Right Cuckoo Filter Node
     * @return a new right Cuckoo Filter Node
     */
    CuckooNode<fp_size> *getNewRightCF();

    /**
     * @brief Prints the current Cuckoo Filter and then left and right Cuckoo Filter Nodes
     */
    void print();

    /**
     * @brief Get the current CuckooFilter
     * @return returns current CuckooFilter
     */
    CuckooFilter<fp_size> *getCurr() { return curr; }
    /**
     * @brief Get the left node
     * @return returns left node
     */
    CuckooNode<fp_size> *getLeft() { return left; }
    /**
     * @brief Get the right node
     * @return returns right node
     */
    CuckooNode<fp_size> *getRight() { return right; }

    /**
     * @brief Returns if the left node exists
     * @return true if there is a right node
     * @return false if there no right node
     */
    bool hasRight() { return right != nullptr; }
    /**
     * @brief Returns if the left node exists
     * @return true if there is a left node
     * @return false if there no left node
     */
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
void CuckooNode<fp_size>::print()
{
    if (curr == nullptr)
    {
        return;
    }
    curr->print();
    if (left != nullptr)
    {
        left->print();
    }
    if (right != nullptr)
    {
        right->print();
    }
}