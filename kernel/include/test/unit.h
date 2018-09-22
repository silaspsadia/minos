#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void (*fn_ptr)()

#define NEW_TEST_SUITE(suite_name, num_tests) 	\
	void suite_name##_run(void) { 				\
		char* name  = #suite_name; 				\
		test_info_t tests[num_tests];	 		\
		fn_ptr setup_suite_fn = NULL;			\
		size_t test_count = 0;

#define SETUP_SUITE()				\
	auto void setup_suite();		\
	setup_suite_fn = setup_suite;	\
	void setup_suite

#define END_SUITE()	\
	if (setup_suite_fn != NULL) { 												\
		setup_suite_fn();														\
	}																			\
	size_t failed_tests = 0;													\
	for (size_t i = 0; i < test_count; i++) {									\
		int passed = true;														\
		test_info_t *cur_test  = &tests[i];										\
		cur_test->fn(cur_test, &passed);										\
		if (!passed) {															\
			failed_tests++;														\
		}																		\
	}																		  	\
	printf("%s suite: %d/%d tests passed.\n", name, test_count, test_count); 	\			
	}	

#define RUN_SUITE(suite_name)								\
	do {													\
		printf("[TEST] Running suite " #suite_name "...");	\
		suite##_run();										\
		printf("done.\n");									\
	} while (0)												

#define TEST(fn_name) 								\
	auto void fn_name(test_info_t*, int *passed);	\
	tests[test_count].name = #fn_name;				\
	tests[test_count].fn = fn_name;					\
	test_count++;									\
	void fn_name(test_info_t *info, int *passed)		

#define TEST_FAILED(__message, ...)						\
	{													\
		printf("Test failed - %s: ", info->name);		\
		printf((__message), __VA_ARGS__);				\
		printf("Found at %s:%d", __FILE__, __LINE__);	\
		*passed = 0;									\
		return;											\
	}

#define EXPECT_EQ(expected, result) 						\
	do { 													\
		if (expected != result)								\
			TEST_FAILED("EQ - Expected: %d, Actual: %d", 	\
				(int) expected, (int) result);				\
	} while (0)

#define EXPECT_NQ(expected, result) 						\
	do { 													\
		if (expected == result)								\
			TEST_FAILED("NE - Expected: %d, Actual: %d", 	\
				(int) expected, (int) result);				\
	} while (0)

#define EXPECT_GT(max, min) 								\
	do { 													\
		if (max <= min)										\
			TEST_FAILED("GT - %d isn't greater than %d", 	\
				(int) max, (int) min);						\
	} while (0)

#define EXPECT_LT(min, max)								\
	do {												\
		if (min >= max)									\
			TEST_FAILED("LT - %d isn't less than %d",	\
				(int) min, (int) max);					\
	} while (0)

#define EXPECT_TRUE(expression)											\
	do {																\
		if (!(expression))												\
			TEST_FAILED("Expression isn't true: %d", (int) expression);	\
	} while (0)

#define EXPECT_FALSE(expression)											\
	do {																	\
		if (expression)														\
			TEST_FAILED("Expression isn't false: %d", (int) expression);	\
	} while (0)

