#include <check.h>
#include <stdio.h>

#include "../../src/sprintf/s21_sprintf.h"

#define BUF_SZ 512

static void assert_one_ll(const char *fmt, long long value) {
  char got[BUF_SZ] = {0};
  char exp[BUF_SZ] = {0};

  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);

  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}

static void assert_one_ull(const char *fmt, unsigned long long value) {
  char got[BUF_SZ] = {0};
  char exp[BUF_SZ] = {0};

  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);

  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}

static void assert_one_f(const char *fmt, long double value) {
  char got[BUF_SZ] = {0};
  char exp[BUF_SZ] = {0};

  int got_n = s21_sprintf(got, fmt, (double)value);
  int exp_n = sprintf(exp, fmt, (double)value);

  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}

START_TEST(test_char_string_percent) {
  char got[BUF_SZ] = {0};
  char exp[BUF_SZ] = {0};

  int got_n = s21_sprintf(got, "[%5c][%-8.3s][%%]", 'A', "school");
  int exp_n = sprintf(exp, "[%5c][%-8.3s][%%]", 'A', "school");

  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}
END_TEST

START_TEST(test_signed_decimal) {
  assert_one_ll("%d", 0);
  assert_one_ll("%+8.4d", 42);
  assert_one_ll("% 08d", 314);
  assert_one_ll("%-10.0d", 0);
  assert_one_ll("%ld", -9223372036854775807LL);
}
END_TEST

START_TEST(test_unsigned_hex_oct) {
  assert_one_ull("%u", 4294967295ULL);
  assert_one_ull("%8.5u", 12ULL);
  assert_one_ull("%#x", 48879ULL);
  assert_one_ull("%#X", 48879ULL);
  assert_one_ull("%#o", 511ULL);
  assert_one_ull("%.0x", 0ULL);
}
END_TEST

START_TEST(test_pointer) {
  char got[BUF_SZ] = {0};
  char exp[BUF_SZ] = {0};
  int x = 42;

  int got_n = s21_sprintf(got, "ptr:%20p", (void *)&x);
  int exp_n = sprintf(exp, "ptr:%20p", (void *)&x);

  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}
END_TEST

START_TEST(test_float_fixed) {
  assert_one_f("%f", 3.5L);
  assert_one_f("%.0f", 2.6L);
  assert_one_f("%10.3f", -12.3456L);
  assert_one_f("%+08.2f", 1.25L);
  assert_one_f("%-10.1f", 0.04L);
}
END_TEST

Suite *s21_sprintf_suite(void) {
  Suite *suite = suite_create("s21_sprintf");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_char_string_percent);
  tcase_add_test(tc, test_signed_decimal);
  tcase_add_test(tc, test_unsigned_hex_oct);
  tcase_add_test(tc, test_pointer);
  tcase_add_test(tc, test_float_fixed);

  suite_add_tcase(suite, tc);
  return suite;
}

int main(void) {
  Suite *suite = s21_sprintf_suite();
  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
