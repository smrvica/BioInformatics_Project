#include "cuckooFilter.h"

/**
 * LinkedCuckooList class is a collection of CuckooFilters that are a linked list.
 * @tparam fp_size size of the fingerprint
 */
template<typename fp_size>
class LinkedCuckooList {
public:
    CuckooFilter<fp_size> *head;
    CuckooFilter<fp_size> *current;
    CuckooFilter<fp_size> *tail;

    /**
     * Constructor for LinkedCuckooList.
     * @param singleTableCapacity capacity of the single table
     * @param bucketSize size of one bucket
     * @param maxNoOfMoves maximum number of moves
     * @param bitsPerFp size of the fingerprint
     */
    LinkedCuckooList(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);

    /**
     * Destructor for LinkedCuckooList.
     */
    ~LinkedCuckooList();

    /**
     * Method that returns a new CuckooFilter.
     * @return new CuckooFilter
     */
    CuckooFilter<fp_size> *getNewCF();

};


template<typename fp_size>
LinkedCuckooList<fp_size>::LinkedCuckooList(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp) {
    head = new CuckooFilter<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
    current = tail = head;
}

template<typename fp_size>
LinkedCuckooList<fp_size>::~LinkedCuckooList() {
    CuckooFilter<fp_size> *temp = head;
    CuckooFilter<fp_size> *next;
    while (temp != nullptr) {
        next = temp->next;
        delete temp;
        temp = next;
    }
    head = current = tail = nullptr;  // just to be sure
}

template<typename fp_size>
CuckooFilter<fp_size> *LinkedCuckooList<fp_size>::getNewCF() {
    CuckooFilter<fp_size> *newCF = new CuckooFilter<fp_size>(current->getCapacity(), current->getBucketSize(), current->getMaxNoOfMoves(), current->getBitsPerFp());

    newCF->next = current->next;
    current->next = newCF;
    newCF->prev = current;
    if (newCF->next != nullptr) {
        newCF->next->prev = newCF;
    } else {
        tail = newCF;
    }
    current = newCF;
    return newCF;
}