#include "../tests_includes/s21_tests.h"

#include <stdlib.h>

void run_testcase(Suite* testcase) {
  SRunner* sr = srunner_create(testcase);
  // srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  if (failed) {
    exit(EXIT_FAILURE);
  }
}

void run_tests(void) {
  Suite* suites[] = {suite_strlen(),  suite_strncpy(), suite_strncat(),
                     suite_strncmp(), suite_memchr(),  suite_memcmp(),
                     suite_memcpy(),  suite_memset(),  suite_strchr(),
                     suite_strrchr(), suite_strpbrk(), suite_strcspn(),
                     suite_strstr(),  suite_strtok(),  suite_strerror(),
                     suite_sprintf(), suite_sscanf(),  NULL};

  for (size_t i = 0; suites[i] != NULL; ++i) {
    run_testcase(suites[i]);
  }
}

int main(void) {
  run_tests();
  return 0;
}
