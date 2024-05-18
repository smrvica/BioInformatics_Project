
#include <string>

uint64_t hash(uint32_t key) {
    return std::hash < uint32_t > {}(key);
}


uint64_t hash(const std::string& key) {
    return std::hash < std::string > {}(key);
}
