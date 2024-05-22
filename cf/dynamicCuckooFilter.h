
#include "linkedCuckooList.h"

template <typename fp_size>
class DynamicCuckooFilter
{
private:
    int singleTableCapacity;
    int bucketSize;
    int maxNoOfMoves;
    int bitsPerFp;
    LinkedCuckooList<fp_size> *list;

    void storeVictim(Victim &victim);

public:
    DynamicCuckooFilter(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp);
    ~DynamicCuckooFilter();

    bool insert(const char *key, Victim &victim);
    bool lookup(const char *key);
    bool deleteKey(const char *key);

    void print();
};

template <typename fp_size>
DynamicCuckooFilter<fp_size>::DynamicCuckooFilter(int singleTableCapacity, int bucketSize, int maxNoOfMoves, int bitsPerFp)
{
    this->singleTableCapacity = singleTableCapacity;
    this->bucketSize = bucketSize;
    this->maxNoOfMoves = maxNoOfMoves;
    this->bitsPerFp = bitsPerFp;
    list = new LinkedCuckooList<fp_size>(singleTableCapacity, bucketSize, maxNoOfMoves, bitsPerFp);
}

template <typename fp_size>
DynamicCuckooFilter<fp_size>::~DynamicCuckooFilter()
{
    delete list;
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::insert(const char *key, Victim &victim)
{
    CuckooFilter<fp_size> *current = list->current;
    if (current->isFull())
    {
        current = list->getNewCF();
    }
    bool success = current->insert(key, victim);
    if (success)
    {
        return true;
    }
    storeVictim(victim);
    return true;
}

template <typename fp_size>
void DynamicCuckooFilter<fp_size>::storeVictim(Victim &victim)
{
    std::cout << "STORING VICTIM\n";
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr)
    {
        if (!temp->isFull())
        {
            if (temp->insertFP(victim.fingerprint, victim.index, victim))
            {
                return;
            }
        }
    }
    CuckooFilter<fp_size> *newCF = list->getNewCF();
    newCF->insertFP(victim.fingerprint, victim.index, victim);
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::lookup(const char *key)
{
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr)
    {
        if (temp->lookup(key))
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template <typename fp_size>
bool DynamicCuckooFilter<fp_size>::deleteKey(const char *key)
{
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr)
    {
        if (temp->deleteKey(key))
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

template <typename fp_size>
void DynamicCuckooFilter<fp_size>::print()
{
    CuckooFilter<fp_size> *temp = list->head;
    while (temp != nullptr)
    {
        std::cout << "CuckooFilter\n";
        temp->print();
        temp = temp->next;
    }
}