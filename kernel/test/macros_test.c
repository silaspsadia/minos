#include <test/macros_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(MacrosTest, 5);

TEST(EmptyMacrosTest) {}

TEST(ExpectEqTestPass) { EXPECT_EQ(23, 23); }

TEST(ExpectEqTestFail) { EXPECT_EQ(-331, 31231); }

TEST(ExpectTrueTestPass) { EXPECT_TRUE(23332); }

TEST(ExpectTrueTestFail) { EXPECT_TRUE(0); }

END_SUITE();

void macros_test(void) { RUN_SUITE(MacrosTest); }
