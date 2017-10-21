#include "ompu_test.hpp"
#include "ompu_test/bridge.hpp"


GTEST_API_ int main(int argc, char* argv[])
{
    ompu_test::bridge brg;
    brg.init(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
