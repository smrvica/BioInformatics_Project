#include <stdint.h>
#include "../cf/LCDF.h"

typedef uint16_t fp_type;
static const int bits_per_fp = 16;

void test1()
{
    std::cout << "TEST 1\n";

    LCDF<fp_type> cf = LCDF<fp_type>(2, 1, 6, bits_per_fp);
    Victim victim;

    std::cout << "insert test: " << cf.insert("something", victim) << "\n";
    std::cout << "insert test2: " << cf.insert("test2", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("something3 something3something3something3something3something3something3something3something3something3something3something3", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("fjsjghdfkjhg", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("ddsdfhsdjkh5", victim) << "\n";
    std::cout << "insert test3: " << cf.insert("fsdkjghdfjkghdk jfhkdjhg jdkfchgjd fgkjdhr fxhkjg dkjfhgkj fxckjfhgf xh bkj dxfhkj gdxkjhdkjfxhdg dkjxfhgkjdf hkjhfbkjdfhgkjhdxfv dxkdjfghys jk gyshk jfhkgjdhfgkjh dksrhfjykghd kfjg dykhg kydfhgkdyhg kysrh se hkdyh hgjkh rggkhy sk jghy kdh ", victim) << "\n";

    // std::cout << "\n";
    // cf.print();

    // std::cout << "lookup test: " << cf.lookup("test") << "\n";
    // std::cout << "lookup test2: " << cf.lookup("test2") << "\n";
    // std::cout << "lookup test3: " << cf.lookup("test3") << "\n";

    // std::cout << "\n";
    // // cf.print();

    // std::cout << "delete test: " << cf.deleteKey("test") << "\n";
    // std::cout << "lookup test: " << cf.lookup("test") << "\n";
    // std::cout << "delete test: " << cf.deleteKey("test") << "\n";

    // std::cout << "\n";
    // cf.print();
}

// void test2()
// {
//     std::cout << "TEST 2\n";

//     CuckooFilter<fp_type> cf = CuckooFilter<fp_type>(5, 2, 6, bits_per_fp);
//     Victim victim;

//     for (int i = 0; i < 12; i++)
//     {
//         std::string key = "test" + std::to_string(i);
//         std::cout << "insert " << key << ": " << cf.insert(key.c_str(), victim) << "\n";
//     }

//     std::cout << "\n";
//     cf.print();
// }

// void test3()
// {
//     std::cout << "TEST 3\n";

//     DynamicCuckooFilter<fp_type> dcf = DynamicCuckooFilter<fp_type>(5, 2, 6, bits_per_fp);

//     for (int i = 0; i < 12; i++)
//     {
//         std::string key = "test" + std::to_string(i);
//         Victim victim;
//         std::cout << "insert " << key << ": " << dcf.insert(key.c_str(), victim) << "\n";
//     }

//     std::cout << "\n";
//     dcf.print();
// }

// void test4()
// {
//     std::cout << "TEST 4\n";

//     DynamicCuckooFilter<fp_type> dcf = DynamicCuckooFilter<fp_type>(5, 2, 6, bits_per_fp);
//     Victim victim;

//     std::cout << "insert test: " << dcf.insert("test", victim) << "\n";
//     std::cout << "insert test2: " << dcf.insert("test2", victim) << "\n";
//     std::cout << "insert test3: " << dcf.insert("test3", victim) << "\n";

//     std::cout << "\n";
//     dcf.print();

//     std::cout << "lookup test: " << dcf.lookup("test") << "\n";
//     std::cout << "lookup test2: " << dcf.lookup("test2") << "\n";
//     std::cout << "lookup test3: " << dcf.lookup("test3") << "\n";

//     std::cout << "\n";
//     dcf.print();

//     std::cout << "delete test: " << dcf.deleteKey("test") << "\n";
//     std::cout << "lookup test: " << dcf.lookup("test") << "\n";
//     std::cout << "delete test: " << dcf.deleteKey("test") << "\n";

//     std::cout << "\n";
//     dcf.print();
// }

int main()
{

    test1();
    std::cout << "\n\n";
    // test2();
    // std::cout << "\n\n";
    // test3();
    // std::cout << "\n\n";
    // test4();

    return 0;
}