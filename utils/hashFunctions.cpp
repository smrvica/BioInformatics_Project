#include "hashFunctions.h"


const uint64_t hash(uint32_t key) {
    return std::hash < uint32_t > {}(key);
}


const uint64_t hash(std::string key) {
    return std::hash < std::string > {}(key);
}


