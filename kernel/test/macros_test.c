#include <test/macros_test.h>
#include <test/unit.h>

NEW_TEST_SUITE(MacrosTest, 5);

TEST(EmptyMacrosTest)
{
}

TEST(ExpectEqTest)
{
	EXPECT_EQ(23, 23);
}

TEST(ExpectNqTest)
{
	EXPECT_NQ(-331, 31231);
}

TEST(ExpectTrueTest)
{
	EXPECT_TRUE(23332);
}

TEST(ExpectFalseTest)
{
	EXPECT_FALSE(0);
}

END_SUITE();

void macros_test(void) { RUN_SUITE(MacrosTest); }
