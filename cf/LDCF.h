#include "cuckooNode.h"

/**
 * @brief LDCF is a tree structure of CuckooFilters
 * When a fingerpring cannot be stored in a current CuckooFilter, then based on the fingerprint bits we store the fingerprint in the right or left branch of the binary tree.
 * @tparam fp_size
 *
 * author: Petra Habjanec
 */

template <typename fp_size>
class LDCF
{
private:
    int singleTableCapacity;
    int bucketSize;
    int maxNoOfMoves;
    int bitsPerFp;

    /**
     * Stores the root node of the LDCF
     */
    CuckooNode<fp_size> *root;

    /**
     * @brief Stores the victim of insertion in the first available CuckooFilter.
     *
     * @param victim object containing information about the victim
     */
    void storeVictim(Victim &victim);

public:
    /**
     * @brief Construct a new LDCF class
     *
     * @param singleTableCapacity capacity of a single CuckooFilter
     * @param bucketSize size of one bucket
     * @param maxNoOfMoves maximum number of kicks during insertion
     * @param bitsPerFp size of the fingerprint
     */
    LDCF(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);
    /**
     * @brief Destroy the LDCF class
     */
    ~LDCF();

    /**
     * @brief Inserts a key into LDCF
     *
     * @param key key to be inserted
     * @param victim object containing information about the victim
     * @return true if the key was inserted successfully
     * @return false any other case
     */
    bool insert(const char *key, Victim &victim);
    /**
     * @brief Look up a key in the filter.
     *
     * @param key
     * @return true if the key was found
     * @return false otherwise
     */
    bool lookup(const char *key);
    /**
     * @brief Deletes a key from the filter
     *
     * @param key
     * @return true if deletion was successfull
     * @return false otherwise
     */
    bool deleteKey(const char *key);

    void print();
};

template <typename fp_size>
void LDCF<fp_size>::storeVictim(Victim &victim)
{
    CuckooNode<fp_size> *curr = root;
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
LDCF<fp_size>::LDCF(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    this->singleTableCapacity = singleTableCapacity;
    this->bucketSize = bucketSize;
    this->maxNoOfMoves = maxNoOfMoves;
    this->bitsPerFp = bitsPerFp;

    root = new CuckooNode<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
}

template <typename fp_size>
LDCF<fp_size>::~LDCF()
{
    delete root;
}

template <typename fp_size>
bool LDCF<fp_size>::insert(const char *key, Victim &victim)
{
    CuckooNode<fp_size> *curr = root;
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
bool LDCF<fp_size>::lookup(const char *key)
{
    CuckooNode<fp_size> *curr = root;
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
bool LDCF<fp_size>::deleteKey(const char *key)
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
void LDCF<fp_size>::print()
{
    std::cout << "LDCF\n";
    root->print();
}
