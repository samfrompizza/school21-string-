// GitHub reference test header (adapted for local project)
#ifndef SRC_TESTS_INCLUDES_S21_TESTS_H_
#define SRC_TESTS_INCLUDES_S21_TESTS_H_

#ifndef BUFF_SIZE
#define BUFF_SIZE 512
#endif

#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/string/s21_string.h"

/* Compatibility helpers for upstream GitHub tests */
#ifndef ck_assert_pstr_eq
#define ck_assert_pstr_eq(p1, p2) ck_assert_ptr_eq((p1), (p2))
#endif

#ifndef ck_assert_mem_ge
#define ck_assert_mem_ge(b1, b2, n) ck_assert_int_eq(memcmp((b1), (b2), (n)), 0)
#endif

/* Size/type aliases used by upstream tests */
typedef s21_size s21_size_t;


Suite* suite_memchr(void);
Suite* suite_memcmp(void);
Suite* suite_memcpy(void);
Suite* suite_memset(void);
Suite* suite_strchr(void);
Suite* suite_strcspn(void);
Suite* suite_strerror(void);
Suite* suite_strlen(void);
Suite* suite_strncat(void);
Suite* suite_strncmp(void);
Suite* suite_strncpy(void);
Suite* suite_strpbrk(void);
Suite* suite_strrchr(void);
Suite* suite_strstr(void);
Suite* suite_strtok(void);

Suite* suite_sprintf(void);
Suite* suite_sscanf(void);

Suite* suite_to_upper(void);
Suite* suite_to_lower(void);
Suite* suite_insert(void);
Suite* suite_trim(void);

void run_tests(void);
void run_testcase(Suite* testcase);

#endif  // SRC_TESTS_INCLUDES_S21_TESTS_H_
