#include <libkern/heap.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*fn_ptr)();

typedef struct test_info_t {
  char* name;
  size_t error_line_num;
  fn_ptr fn;
} test_info_t;

// Defines macros for testing
#define NEW_SUITE(suite_name, max_num_tests) \
  void suite_name##_run() {                  \
    char* name = #suite_name;                \
    test_info_t tests[max_num_tests];        \
    fn_ptr setup_suite_fn = NULL;            \
    size_t test_count = 0;

#define SETUP_SUITE()                                                      \
  auto void setup_suite();                                                 \
  setup_suite_fn = setup_suite;                                            \
  void setup_suite()                                                       

#define END_SUITE()                                                        \
  if (setup_suite_fn != NULL) {                                            \
    setup_suite_fn();                                                      \
  }                                                                        \
  size_t failed_tests = 0;                                                 \
  for (size_t i = 0; i < test_count; i++) {                                \
    bool passed = true;                                                    \
    test_info_t* cur_test = &tests[i];                                     \
    track_memory_malloced();                                               \
    cur_test->fn(cur_test, &passed);                                       \
    if (!passed) {                                                         \
      failed_tests++;                                                      \
    }                                                                      \
    untrack_memory_malloced();                                             \
    print_memory_report(false);                                            \
  }                                                                        \
  if (failed_tests == 0) {                                                 \
    printf("Test suite %s passed! %d/%d\n", name, test_count, test_count); \
  } else {                                                                 \
    printf("Test suite %s failed! %d/%d passed\n", name,                   \
           test_count - failed_tests, test_count);                         \
    \    
                                                                 \
  }                                                                        \
  }

#define RUN_SUITE(suite_name)                     \
  do {                                            \
    printf("Running suite " #suite_name "...\n"); \
    suite_name##_run();                           \
    printf("Suite done...\n");                    \
  } while (0)

#define TEST(fn_name)                            \
  auto void fn_name(test_info_t*, bool* passed); \
  tests[test_count].name = #fn_name;             \
  tests[test_count++].fn = fn_name;              \
  void fn_name(test_info_t* info, bool* passed)

#define TEST_FAILED(__message, ...)                                    \
  {                                                                    \
    printf("Test failed - %s. ", info->name);                          \
    printf((__message), __VA_ARGS__);                                  \
    printf(" Found at %s:%d\n", __FILE__, __LINE__);                   \
    *passed = false;                                                   \
    return;                                                            \
  }

#define EXPECT_EQ(expected, result)                                   \
  do {                                                                \
    if (expected != result)                                           \
      TEST_FAILED("EQ - Expected: %d, Actual: %d",                    \
        (int) expected, (int) result);                                \
  } while (0)

#define EXPECT_NE(expected, result)                                   \
  do {                                                                \
    if (expected == result)                                           \
      TEST_FAILED("NE - Expected: %d, Actual: %d",                    \
        (int) expected, (int) result);                                \
  } while (0)

#define EXPECT_GT(max, min)                                           \
  do {                                                                \
    if (min >= max)                                                   \
      TEST_FAILED("%d isn't greater than %d",                         \
        (int) max, (int) min);                                        \
  } while (0)

#define EXPECT_GTE(max, min)                                          \
  do {                                                                \
    if (min > max)                                                    \
      TEST_FAILED("%d isn't greater than or equal to %d",             \
        (int) max, (int) min);                                        \
  } while (0)

#define EXPECT_LT(min, max)                                           \
  do {                                                                \
    if (max <= min)                                                   \
      TEST_FAILED("%d isn't lesser than %d",                          \
        (int) min, (int) max);                                        \
  } while (0)

#define EXPECT_LTE(min, max)                                          \
  do {                                                                \
    if (max < min)                                                    \
      TEST_FAILED("%d isn't lesser than or equal to %d",              \
        (int) min, (int) max);                                        \
  } while (0)

#define EXPECT_TRUE(expression)                                       \
  do {                                                                \
    if (!expression)                                                  \
      TEST_FAILED("Variable isn't true %d.", (int) expression);       \
  } while (0)

#define EXPECT_FALSE(expression)                                      \
  do {                                                                \
    if (expression)                                                   \
      TEST_FAILED("Variable isn't false %d.", (int) expression);      \
  } while (0)
