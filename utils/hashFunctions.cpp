
#include "hashFunctions.h"

const uint64_t hash(std::string key) {
    return std::hash<std::string>{}(key);
}


