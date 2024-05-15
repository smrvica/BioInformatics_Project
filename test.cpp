#include <stdint.h>
#include "cf/cuckooFilter.h"


typedef uint16_t fp_type;
static const int bits_per_fp = 16;

int main() {

    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(5, 2, 6, bits_per_fp);

    std::cout << "insert test: " << cf.insert("test") << "\n";
    std::cout << "insert test2: " << cf.insert("test2")  << "\n";
    std::cout << "insert test3: " << cf.insert("test3")  << "\n";

    std::cout << "\n";
    cf.print();

    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "lookup test2: " << cf.lookup("test2") << "\n";
    std::cout << "lookup test3: " << cf.lookup("test3") << "\n";

    std::cout << "\n";
    cf.print();

    std::cout << "delete test: " << cf.deleteKey("test") << "\n";
    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "delete test: " << cf.deleteKey("test") << "\n";

    std::cout << "\n";
    cf.print();

    return 0;
}