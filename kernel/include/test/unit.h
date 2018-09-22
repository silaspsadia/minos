#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void (*fn_ptr)();

typedef struct test_info_t {
	char* name;
	size_t error_line_num;
	fn_ptr fn;
} test_info_t;

#define NEW_TEST_SUITE(suite_name, num_tests)   \
	void suite_name##_run(void) {           \
		char* name  = #suite_name;      \
		test_info_t tests[num_tests];   \
		fn_ptr setup_suite_fn = NULL;   \
		size_t test_count = 0;

#define SETUP_SUITE()                 \
	auto void setup_suite();      \
	setup_suite_fn = setup_suite; \
	void setup_suite()

#define END_SUITE()                                                              \
	if (setup_suite_fn != NULL) {                                            \
		setup_suite_fn();                                                \
	}                                                                        \
	size_t failed_tests = 0;                                                 \
	for (size_t i = 0; i < test_count; i++) {                                \
		int passed = 1;                                                  \
		test_info_t *cur_test  = &tests[i];                              \
		cur_test->fn(cur_test, &passed);                                 \
		if (!passed) {                                                   \
			failed_tests++;                                          \
		}                                                                \
	}                                                                        \
	printf("%s suite: %d/%d tests passed.\n", name, test_count, test_count); \
        }	

#define RUN_SUITE(suite_name)                                        \
	do {                                                         \
		printf("[TEST] Running suite " #suite_name "...\n"); \
		printf("### START ###\n");                           \
		suite_name##_run();                                  \
		printf("### DONE. ###\n");                           \
	} while (0)																		
			
#define TEST(fn_name)                                 \
	auto void fn_name(test_info_t*, int *passed); \
	tests[test_count].name = #fn_name;            \
	tests[test_count].fn = fn_name;               \
	test_count++;                                 \
	void fn_name(test_info_t *info, int *passed)

#define TEST_FAILED(__message, ...)                           \
	{                                                     \
		printf("Test failed - %s: \n", info->name);     \
		printf((__message), __VA_ARGS__);             \
		printf("Found at %s:%d\n", __FILE__, __LINE__); \
		*passed = 0;                                  \
		return;                                       \
	}

#define EXPECT_EQ(expected, result) 						\
	do { 													\
		if (expected != result)								\
			TEST_FAILED("EQ - Expected: %d, Actual: %d\n", 	\
				(int) expected, (int) result);				\
	} while (0)

#define EXPECT_NQ(expected, result) 						\
	do { 													\
		if (expected == result)								\
			TEST_FAILED("NE - Expected: %d, Actual: %d\n", 	\
				(int) expected, (int) result);				\
	} while (0)

#define EXPECT_GT(max, min) 								\
	do { 													\
		if (max <= min)										\
			TEST_FAILED("GT - %d isn't greater than %d\n", 	\
				(int) max, (int) min);						\
	} while (0)

#define EXPECT_LT(min, max)								\
	do {												\
		if (min >= max)									\
			TEST_FAILED("LT - %d isn't less than %d\n",	\
				(int) min, (int) max);					\
	} while (0)

#define EXPECT_TRUE(expression)											\
	do {																\
		if (!(expression))												\
			TEST_FAILED("Expression isn't true: %d\n", (int) expression);	\
	} while (0)

#define EXPECT_FALSE(expression)											\
	do {																	\
		if (expression)														\
			TEST_FAILED("Expression isn't false: %d\n", (int) expression);	\
	} while (0)

