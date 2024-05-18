#include "cuckooFilter.h"

template<typename fp_size>
class LinkedCuckooList {
public:
    CuckooFilter<fp_size> *head;
    CuckooFilter<fp_size> *current;
    CuckooFilter<fp_size> *tail;

    LinkedCuckooList(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);

    ~LinkedCuckooList();

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
    while (current != nullptr) {
        next = temp->next;
        delete temp;
        temp = next;
    }
    head = current = tail = nullptr;  // just to be sure
}