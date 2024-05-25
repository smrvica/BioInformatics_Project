
#include <string>

/**
 * Hash function for keys of type uint32_t
 * @param key to be hashed
 * @return hash value of type uint64_t
 */
uint64_t hash(uint32_t key) {
    return std::hash < uint32_t > {}(key);
}

/**
 * Hash function for keys of type std::string
 * @param key to be hashed
 * @return hash value of type uint64_t
 */
uint64_t hash(const std::string& key) {
    return std::hash < std::string > {}(key);
}
