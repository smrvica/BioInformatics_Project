#include <stdint.h>
#include "../cf/LDCF.h"
#include "../cf/dynamicCuckooFilter.h"

typedef uint16_t fp_type;
static const int bits_per_fp = 16;

/**
 * Testing general functionality of CuckooFilter.
 */
void test1() {
    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(5, 2, 6, bits_per_fp);
    Victim victim;

    std::cout << "insert test: " << cf.insert("test", victim) << "\n";
    std::cout << "insert test2: " << cf.insert("test2", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("something3", victim) << "\n";
    std::cout << "insert test4: " << cf.insert("fjsjghdfkjhg", victim) << "\n";
    std::cout << "insert test5: " << cf.insert("ddsdfhsdjkh5", victim) << "\n";
    std::cout << "insert test6: " << cf.insert("test6", victim) << "\n";

    std::cout << "\n";
    cf.print();

    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "lookup test2: " << cf.lookup("test2") << "\n";
    std::cout << "lookup test3: " << cf.lookup("something3") << "\n";

    std::cout << "\n";

    std::cout << "delete test: " << cf.deleteKey("test") << "\n";
    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "delete test: " << cf.deleteKey("test") << "\n";

    std::cout << "\n";
    cf.print();
}

/**
 * Testing general functionality of CuckooFilter.
 */
void test2() {
    std::cout << "TEST 2\n";

    CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(5, 2, 6, bits_per_fp);
    Victim victim;

    for (int i = 0; i < 12; i++)
    {
     std::string key = "test" + std::to_string(i);
     std::cout << "insert " << key << ": " << cf.insert(key.c_str(), victim) << "\n";
    }

    std::cout << "\n";
    cf.print();
}

/**
 * Testing general functionality of DynamicCuckooFilter.
 */
void test3() {
    std::cout << "TEST 3\n";

    DynamicCuckooFilter<fp_type> dcf = DynamicCuckooFilter<fp_type>(5, 2, 6, bits_per_fp);

    for (int i = 0; i < 12; i++)
    {
     std::string key = "test" + std::to_string(i);
     Victim victim;
     std::cout << "insert " << key << ": " << dcf.insert(key.c_str(), victim) << "\n";
    }

    std::cout << "\n";
    dcf.print();
}

/**
 * Testing general functionality of DynamicCuckooFilter.
 */
void test4() {
    std::cout << "TEST 4\n";

    DynamicCuckooFilter<fp_type> dcf = DynamicCuckooFilter<fp_type>(5, 2, 6, bits_per_fp);
    Victim victim;

    std::cout << "insert test: " << dcf.insert("test", victim) << "\n";
    std::cout << "insert test2: " << dcf.insert("test2", victim) << "\n";
    std::cout << "insert test3: " << dcf.insert("test3", victim) << "\n";

    std::cout << "\n";
    dcf.print();

    std::cout << "lookup test: " << dcf.lookup("test") << "\n";
    std::cout << "lookup test2: " << dcf.lookup("test2") << "\n";
    std::cout << "lookup test3: " << dcf.lookup("test3") << "\n";

    std::cout << "\n";
    dcf.print();

    std::cout << "delete test: " << dcf.deleteKey("test") << "\n";
    std::cout << "lookup test: " << dcf.lookup("test") << "\n";
    std::cout << "delete test: " << dcf.deleteKey("test") << "\n";

    std::cout << "\n";
    dcf.print();
}

/**
 * Testing general functionality of LCDF.
 */
void test5() {
    std::cout << "TEST 5\n";

    LCDF<fp_type> cf = LCDF<fp_type>(5, 2, 6, bits_per_fp);
    Victim victim;

    std::cout << "insert test: " << cf.insert("test", victim) << "\n";
    std::cout << "insert test2: " << cf.insert("test2", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("something3", victim) << "\n";
    std::cout << "insert test4: " << cf.insert("fjsjghdfkjhg", victim) << "\n";
    std::cout << "insert test5: " << cf.insert("ddsdfhsdjkh5", victim) << "\n";
    std::cout << "insert test6: " << cf.insert("test6", victim) << "\n";

    std::cout << "\n";
    cf.print();

    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "lookup test2: " << cf.lookup("test2") << "\n";
    std::cout << "lookup test3: " << cf.lookup("something3") << "\n";

    std::cout << "\n";

    std::cout << "delete test: " << cf.deleteKey("test") << "\n";
    std::cout << "lookup test: " << cf.lookup("test") << "\n";
    std::cout << "delete test: " << cf.deleteKey("test") << "\n";

    std::cout << "\n";
    cf.print();
}

/**
 * Testing general functionality of LCDF.
 */
void test6() {
    std::cout << "TEST 6\n";

    LCDF<fp_type> cf = LCDF<fp_type>(5, 2, 6, bits_per_fp);
    Victim victim;

    for (int i = 0; i < 12; i++)
    {
        std::string key = "test" + std::to_string(i);
        std::cout << "insert " << key << ": " << cf.insert(key.c_str(), victim) << "\n";
    }

    std::cout << "\n";
    cf.print();
}


/**
 * Test function for cuckoo filter, dynamic cuckoo filter and logarithmic dynamic cuckoo filter.
 * Tests general functionality of the filters.
 * @return 0 if successful
 */
int main() {

    test1();
    std::cout << "\n\n";
    test2();
    std::cout << "\n\n";
    test3();
    std::cout << "\n\n";
    test4();
    std::cout << "\n\n";
    test5();
    std::cout << "\n\n";
    test6();

    return 0;
}