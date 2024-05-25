#ifndef DYNAMIC_CUCKOO_FILTER_H
#define DYNAMIC_CUCKOO_FILTER_H

#include "linkedCuckooList.h"

/**
 * DynamicCuckooFilter class is a data structure being used for storing fingerprints of keys.
 * It is a dynamic version of CuckooFilter, which means that it can store more fingerprints than the initial capacity.
 * The capacity of the filter is increased by creating a linked list of CuckooFilters.
 * @tparam fp_size size of the fingerprint
 */
template <typename fp_size>
class DynamicCuckooFilter {
private:
    int singleTableCapacity;
    int bucketSize;
    int maxNoOfMoves;
    int bitsPerFp;
    LinkedCuckooList<fp_size> *list;

    /**
     * Stores the victim of insertion in the first available CuckooFilter.
     * @param victim object containing information about the victim
     */
    void storeVictim(Victim &victim);

public:
    /**
     * Constructor of the DynamicCuckooFilter class.
     * @param singleTableCapacity capacity of a single CuckooFilter
     * @param bucketSize size of one bucket
     * @param maxNoOfMoves maximum number of kicks during insertion
     * @param bitsPerFp size of the fingerprint
     */
    DynamicCuckooFilter(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);

    /**
     * Destructor of the DynamicCuckooFilter class.
     */
    ~DynamicCuckooFilter();

    /**
     * Inserts a key to the filter.
     * @param key key to be inserted
     * @param victim object containing information about the victim
     * @return true if the key was inserted successfully, false otherwise
     */
    bool insert(const char *key, Victim &victim);

    /**
     * Looks up for a key in the filter.
     * @param key
     * @return
     */
    bool lookup(const char *key);

    /**
     * Deletes a key from the filter.
     * @param key
     * @return
     */
    bool deleteKey(const char *key);

    /**
     * Prints the content of the filter.
     */
    void print();
};

template <typename fp_size>
DynamicCuckooFilter<fp_size>::DynamicCuckooFilter(int singleTableCapacity,
                                                  int bucketSize,
                                                  int maxNoOfMoves,
                                                  int bitsPerFp) {
    this->bucketSize = bucketSize;
    this->maxNoOfMoves = maxNoOfMoves;
    this->bitsPerFp = bitsPerFp;
    list = new LinkedCuckooList<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
    this->singleTableCapacity = list->head->getCapacity();
}

template <typename fp_size>
DynamicCuckooFilter<fp_size>::~DynamicCuckooFilter() {
    delete list;
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::insert(const char *key, Victim &victim) {
    CuckooFilter<fp_size> *current = list->current;
    if (current->isFull()) {
        current = list->getNewCF();
    }
    bool success = current->insert(key, victim);
    if (success) {
        return true;
    }
    storeVictim(victim);
    return true;
}

template <typename fp_size>
void DynamicCuckooFilter<fp_size>::storeVictim(Victim &victim) {
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr) {
        if (!temp->isFull()) {
            if (temp->insertFP(victim.fingerprint, victim.index, victim)) {
                return;
            }
        }
        temp = temp->next;
    }
    CuckooFilter<fp_size> *newCF = list->getNewCF();
    newCF->insertFP(victim.fingerprint, victim.index, victim);
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::lookup(const char *key) {
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr) {
        if (temp->lookup(key)) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::deleteKey(const char *key) {
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr) {
        if (temp->deleteKey(key)) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template <typename fp_size>
void DynamicCuckooFilter<fp_size>::print() {
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr) {
        std::cout << "CuckooFilter\n";
        temp->print();
        temp = temp->next;
    }
}

#endif