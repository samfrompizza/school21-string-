#include <check.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "../../src/sprintf/s21_sprintf.h"

#define BUF_SZ 1024

static void assert_int(const char *fmt, int value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
  ck_assert_str_eq(got, exp);
}

static void assert_short(const char *fmt, short value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_long(const char *fmt, long value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_ll(const char *fmt, long long value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_uint(const char *fmt, unsigned int value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_ushort(const char *fmt, unsigned short value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_ulong(const char *fmt, unsigned long value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_ull(const char *fmt, unsigned long long value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_double(const char *fmt, double value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_long_double(const char *fmt, long double value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_char(const char *fmt, char value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_str(const char *fmt, const char *value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_ptr(const char *fmt, void *value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, value);
  int exp_n = sprintf(exp, fmt, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_wildcard_int(const char *fmt, int width, int value) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, width, value);
  int exp_n = sprintf(exp, fmt, width, value);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_wildcard_precision_str(const char *fmt, int prec, const char *s) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, prec, s);
  int exp_n = sprintf(exp, fmt, prec, s);
  ck_assert_int_eq(got_n, exp_n);
}

static void assert_wildcard_width_ptr(const char *fmt, int width, void *p) {
  char got[BUF_SZ] = {0}, exp[BUF_SZ] = {0};
  int got_n = s21_sprintf(got, fmt, width, p);
  int exp_n = sprintf(exp, fmt, width, p);
  ck_assert_int_eq(got_n, exp_n);
}

START_TEST(test_char_string_percent) {
  assert_char("[%5c]", 'A');
  assert_char("[%-5c]", 'B');
  assert_str("[%8.3s]", "school");
  assert_str("[%-8.3s]", "school");
  assert_str("[%.0s]", "abc");
  assert_str("[%10s]", "");
  /* %% с width 
  char g[BUF_SZ] = {0}, e[BUF_SZ] = {0};
  int gn = s21_sprintf(g, "[%5%][%-5%]");
  int en = sprintf(e, "[%5%][%-5%]");
  ck_assert_int_eq(gn, en);
  ck_assert_str_eq(g, e);*/
}
END_TEST

START_TEST(test_signed_decimal_basic_and_lengths) {
  assert_int("%d", 0);
  assert_int("%d", INT_MAX);
  assert_int("%d", INT_MIN);
  assert_int("%+d", 42);
  assert_int("% d", 42);
  assert_int("%05d", 42);
  assert_int("%05d", -42);

  assert_short("%hd", (short)32767);
  assert_short("%hd", (short)-32768);

  assert_long("%ld", LONG_MAX);
  assert_long("%ld", LONG_MIN);

  assert_ll("%lld", LLONG_MAX);
  assert_ll("%lld", LLONG_MIN);

  assert_wildcard_int("%*d", 8, 123);
  assert_wildcard_int("%*d", -8, 123);
}
END_TEST

START_TEST(test_unsigned_and_bases_and_flags) {
  assert_uint("%u", 0u);
  assert_uint("%u", UINT_MAX);
  assert_ull("%llu", ULLONG_MAX);

  assert_ull("%#o", 511ULL);
  assert_ull("%#o", 0ULL);
  assert_ull("%#x", 48879ULL);
  assert_ull("%#X", 48879ULL);
  assert_ull("%#x", 0ULL);

  assert_ull("%.0x", 0ULL);
  assert_ull("%.0u", 0ULL);
  assert_ull("%.0o", 0ULL);

  assert_ull("%8.5u", 12ULL);
  assert_ull("%#8.5x", 48879ULL);
}
END_TEST

START_TEST(test_pointer_cases) {
  int x = 42;
  void *p = &x;

  assert_ptr("%p", p);
  assert_ptr("%20p", p);
  assert_ptr("%-20p", p);
  assert_ptr("%p", NULL);
  assert_wildcard_width_ptr("%*p", 20, p);
  assert_wildcard_width_ptr("%*p", -20, p);
}
END_TEST

START_TEST(test_float_fixed_and_lengths) {
  assert_double("%f", 3.5);
  assert_double("%.0f", 2.6);
  assert_double("%10.3f", -12.3456);
  assert_double("%+08.2f", 1.25);
  assert_double("%-10.1f", 0.04);

  assert_double("%f", NAN);
  assert_double("%f", INFINITY);
  assert_double("%f", -INFINITY);

  assert_long_double("%Lf", 1.234567890123456789L);
  assert_long_double("%20.10Lf", 1.234567890123456789L);
  assert_long_double("%+0*.2Lf", 8L);
}
END_TEST

START_TEST(test_string_precision_and_wildcard) {
  assert_str("%.3s", "abcdef");
  assert_str("%.10s", "abc");
  assert_wildcard_precision_str("%.*s", 3, "abcdef");
  assert_wildcard_precision_str("%.*s", 0, "abcdef");
  assert_wildcard_precision_str("%.*s", -3, "abcdef");
}
END_TEST

START_TEST(test_hex_oct_various_formats) {
  assert_ull("%08x", 0xBEEF);
  assert_ull("%#010x", 48879ULL);
  assert_ull("%#010X", 48879ULL);
  assert_ull("%#o", 0ULL);
  assert_ull("%#.0o", 0ULL);
}
END_TEST

START_TEST(test_length_combo_signed_unsigned) {
  assert_short("%hd", (short)-1);
  assert_long("%ld", -1L);
  assert_ll("%lld", -1LL);

  assert_ushort("%hu", (unsigned short)65535u);
  assert_ulong("%lu", (unsigned long)ULONG_MAX);
  assert_ull("%llu", (unsigned long long)ULLONG_MAX);
}
END_TEST

START_TEST(test_flags_interactions) {
  assert_int("%+ d", 42);
  assert_int("%0+8d", 42);
  assert_int("%-+8d", 42);
  assert_ull("%#8.4x", 0x1a3ULL);
  assert_ull("%#8.4o", 0x1a3ULL);
}
END_TEST

Suite *s21_sprintf_suite(void) {
  Suite *suite = suite_create("s21_sprintf");
  TCase *tc_core = tcase_create("core");
  tcase_set_timeout(tc_core, 30);

  tcase_add_test(tc_core, test_char_string_percent);
  tcase_add_test(tc_core, test_signed_decimal_basic_and_lengths);
  tcase_add_test(tc_core, test_unsigned_and_bases_and_flags);
  tcase_add_test(tc_core, test_pointer_cases);
  tcase_add_test(tc_core, test_float_fixed_and_lengths);
  tcase_add_test(tc_core, test_string_precision_and_wildcard);
  tcase_add_test(tc_core, test_hex_oct_various_formats);
  tcase_add_test(tc_core, test_length_combo_signed_unsigned);
  tcase_add_test(tc_core, test_flags_interactions);

  suite_add_tcase(suite, tc_core);
  return suite;
}

int main(void) {
  Suite *suite = s21_sprintf_suite();
  SRunner *runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? 0 : 1;
}