#include <string>
#include <random>
#include "murmurHash3.cpp"


unsigned __int128 multiply = 0;
unsigned __int128 add = 0;


void setMultiplyAdd() {
    srand(1);
    for (auto v : {&multiply, &add}) {
        *v = rand();
        for (int i = 1; i <= 4; ++i) {
            *v = *v << 32;
            *v |= rand();
        }
    }
}


uint64_t murmurHash3(std::string *key, size_t len) {
    uint64_t hash[2];
    MurmurHash3_x86_128(key->c_str(), len, 5, hash);
    return hash[0];
}

/**
 * Hash function for keys of type uint32_t
 * @param key to be hashed
 * @return hash value of type uint64_t
 */
uint64_t hash(uint32_t key) {
    //return std::hash < uint32_t > {}(key);
    if (multiply == 0) {
        setMultiplyAdd();
    }
    return (add + multiply * static_cast<decltype(multiply)>(key)) >> 64;
}

/**
 * Hash function for keys of type std::string
 * @param key to be hashed
 * @return hash value of type uint64_t
 */
uint64_t hash(std::string key) {
    //return std::hash < std::string > {}(key);
    return murmurHash3(&key, (size_t) key.size());
}
