#include <stdint.h>
#include "../cf/cuckooFilter.h"


typedef uint16_t fp_type;
static const int bits_per_fp = 16;

void test1() {
    std::cout << "TEST 1\n";

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
}


void test2() {
    std::cout << "TEST 2\n";

    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(5, 2, 6, bits_per_fp);

    for(int i = 0; i < 12; i++) {
        std::string key = "test" + std::to_string(i);
        std::cout << "insert " << key << ": " << cf.insert(key.c_str()) << "\n";
    }

    std::cout << "\n";
    cf.print();
}


int main() {

    test1();
    std::cout << "\n\n";
    test2();

    return 0;
}