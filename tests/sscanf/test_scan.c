/**
 * Unit tests for scan_string_var (sscanf parsing core).
 * Compares behavior against standard sscanf; uses %n to measure consumed
 * length. Requires Check library (libcheck-dev / check). Build: make test_scan
 */

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/common/s21_format.h"
#include "../../src/sscanf/scan.h"

#ifndef NO_WIDTH_SENTINEL
#define NO_WIDTH_SENTINEL 4096
#endif

/* ---- Helpers: build format string and run sscanf reference ---- */

static void build_format(char* out, size_t cap, int width,
                         s21_format_length len, char val, int with_n) {
  const char* len_str = "";
  if (len == LEN_SHORT) {
    len_str = "h";
  } else if (len == LEN_LONG) {
    len_str = "l";
  } else if (len == LEN_LONG_LONG) {
    len_str = "ll";
  }

  if (with_n) {
    if (width > 0 && width < NO_WIDTH_SENTINEL) {
      (void)snprintf(out, cap, "%%%d%s%c%%n", width, len_str, val);
    } else {
      (void)snprintf(out, cap, "%%%s%c%%n", len_str, val);
    }
  } else {
    if (width > 0 && width < NO_WIDTH_SENTINEL) {
      (void)snprintf(out, cap, "%%%d%s%c", width, len_str, val);
    } else {
      (void)snprintf(out, cap, "%%%s%c", len_str, val);
    }
  }
}

static void spec_init(s21_specifier* spec, int width, s21_format_length len,
                      char val) {
  memset(spec, 0, sizeof(*spec));
  spec->valid = 1;
  spec->val = val;
  spec->var = VAR_DECIMAL;
  spec->len = len;
  spec->has_width = 1;
  spec->width =
      (width > 0 && width < NO_WIDTH_SENTINEL) ? width : NO_WIDTH_SENTINEL;
}

/* ---- VAR_CHAR tests (compare vs sscanf %c) ---- */

START_TEST(scan_char_single_letter) {
  const char* input = "a";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_char_digit) {
  const char* input = "7";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_char_space) {
  const char* input = " ";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_char_null_byte) {
  /* sscanf %c does not read \0; we do. Only verify our impl: read \0, advance
   * by 1. */
  const char input[] = {'\0', 'x'};
  char got = '?';
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)'\0');
  ck_assert_ptr_eq(next, input + 1);
}
END_TEST

START_TEST(scan_char_consumes_one) {
  const char* input = "ab";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + 1);
  ck_assert_int_eq(n, 1);
}
END_TEST

/* ---- VAR_DECIMAL %d: signed, all lengths, widths, corners ---- */

START_TEST(scan_decimal_d_positive) {
  const char* input = "42";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_negative) {
  const char* input = "-99";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_plus_prefix) {
  const char* input = "+123";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_zero) {
  const char* input = "0";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_trailing_garbage) {
  const char* input = "123abc";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_int_eq(got, 123);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_width_limits_digits) {
  const char* input = "99999";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 3, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 3, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_int_eq(got, 999);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_width_one) {
  const char* input = "7x";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 1, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 1, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_int_eq(got, 7);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_len_short) {
  const char* input = "32767";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_SHORT, 'd', 1);
  short ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_SHORT, 'd');
  short got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((int)got, (int)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_len_long) {
  const char* input = "2147483647";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_LONG, 'd', 1);
  long ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_LONG, 'd');
  long got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((long)got, (long)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_len_long_long) {
  const char* input = "9223372036854775807";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_LONG_LONG, 'd', 1);
  long long ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_LONG_LONG, 'd');
  long long got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert(got == ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_negative_long_long) {
  const char* input = "-9223372036854775808";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_LONG_LONG, 'd', 1);
  long long ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_LONG_LONG, 'd');
  long long got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert(got == ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_no_digits_after_sign) {
  const char* input = "+";
  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = -999;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  /* No digits: we must not overwrite param. Match glibc: no advance. */
  ck_assert_int_eq(got, -999);
  ck_assert_ptr_eq(next, input);
}
END_TEST

START_TEST(scan_decimal_d_empty_string) {
  const char* input = "x";
  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = -999;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_int_eq(got, -999);
}
END_TEST

START_TEST(scan_decimal_d_width_zero) {
  const char* input = "42";
  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  spec.width = 0;
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_int_eq(got, 0);
}
END_TEST

/* ---- VAR_DECIMAL %u: unsigned, all lengths, widths ---- */

START_TEST(scan_decimal_u_positive) {
  const char* input = "100";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'u', 1);
  unsigned int ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'u');
  unsigned int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_uint_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_zero) {
  const char* input = "0";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'u', 1);
  unsigned int ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'u');
  unsigned int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_uint_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_plus_prefix) {
  const char* input = "+255";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'u', 1);
  unsigned int ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'u');
  unsigned int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_uint_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_width) {
  const char* input = "12345";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 2, LEN_DEFAULT, 'u', 1);
  unsigned int ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 2, LEN_DEFAULT, 'u');
  unsigned int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_uint_eq(got, ref);
  ck_assert_uint_eq(got, 12u);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_len_short) {
  const char* input = "65535";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_SHORT, 'u', 1);
  unsigned short ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_SHORT, 'u');
  unsigned short got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_uint_eq((unsigned int)got, (unsigned int)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_len_long) {
  const char* input = "4294967295";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_LONG, 'u', 1);
  unsigned long ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_LONG, 'u');
  unsigned long got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert(got == ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_len_long_long) {
  const char* input = "18446744073709551615";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_LONG_LONG, 'u', 1);
  unsigned long long ref = 0;
  int n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_LONG_LONG, 'u');
  unsigned long long got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert(got == ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_u_empty) {
  const char* input = "z";
  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'u');
  unsigned int got = 999;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_uint_eq(got, 999u);
}
END_TEST

/* ---- Strange formats and corner cases ---- */

START_TEST(scan_decimal_d_large_width) {
  const char* input = "123";
  s21_specifier spec;
  spec_init(&spec, NO_WIDTH_SENTINEL, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  int ref = 0, n = 0;
  sscanf(input, "%d%n", &ref, &n);
  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_negative_with_width) {
  const char* input = "-12xy";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 4, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 4, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_int_eq(got, -12);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_single_zero) {
  const char* input = "0";
  s21_specifier spec;
  spec_init(&spec, 1, LEN_DEFAULT, 'd');
  int got = -1;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, 0);
  ck_assert_ptr_eq(next, input + 1);
}
END_TEST

START_TEST(scan_decimal_d_minus_zero) {
  const char* input = "-0";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 0, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_width_larger_than_input) {
  const char* input = "12";
  char fmt[64];
  build_format(fmt, sizeof(fmt), 10, LEN_DEFAULT, 'd', 1);
  int ref = 0, n = 0;
  sscanf(input, fmt, &ref, &n);

  s21_specifier spec;
  spec_init(&spec, 10, LEN_DEFAULT, 'd');
  int got = 0;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, ref);
  ck_assert_int_eq(got, 12);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_decimal_d_minus_only) {
  const char* input = "-";
  s21_specifier spec;
  spec_init(&spec, 0, LEN_DEFAULT, 'd');
  int got = -999;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq(got, -999);
  ck_assert_ptr_eq(next, input);
}
END_TEST

START_TEST(scan_char_special) {
  const char* input = "\t";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_char_newline) {
  const char* input = "\n";
  char ref = 0, got = 0;
  int n = 0;
  sscanf(input, "%c%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_CHAR;
  spec.val = 'c';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_int_eq((unsigned char)got, (unsigned char)ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

/* ---- VAR_FLOAT ---- */

START_TEST(scan_float_f) {
  const char* input = "3.14";
  double ref = 0.0;
  int n = 0;
  sscanf(input, "%lf%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'f';
  ScanState state = {&spec, input, input, &ref};
  const char* next = scan_string_var(&state);

  ck_assert_double_eq_tol(ref, ref, 1e-9);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_float_e) {
  const char* input = "1e-10";
  double ref = 0.0;
  int n = 0;
  sscanf(input, "%le%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'e';
  ScanState state = {&spec, input, input, &ref};
  const char* next = scan_string_var(&state);

  ck_assert_double_eq_tol(ref, ref, 1e-15);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_float_g) {
  const char* input = "1.5";
  double ref = 0.0;
  int n = 0;
  sscanf(input, "%lg%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'g';
  ScanState state = {&spec, input, input, &ref};
  const char* next = scan_string_var(&state);

  ck_assert_double_eq_tol(ref, ref, 1e-9);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_float_E) {
  const char* input = "2E+3";
  double ref = 0.0;
  int n = 0;
  sscanf(input, "%lE%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'E';
  ScanState state = {&spec, input, input, &ref};
  const char* next = scan_string_var(&state);

  ck_assert_double_eq_tol(ref, ref, 1e-9);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_float_G) {
  const char* input = "0.5";
  double ref = 0.0;
  int n = 0;
  sscanf(input, "%lG%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'G';
  ScanState state = {&spec, input, input, &ref};
  const char* next = scan_string_var(&state);

  ck_assert_double_eq_tol(ref, ref, 1e-9);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_float_long_double) {
  const char* input = "1.5";
  long double ref = 0.0L, got = 99.0L;
  int n = 0;
  sscanf(input, "%Lf%n", &ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_FLOAT;
  spec.val = 'f';
  spec.len = LEN_LONG_DOUBLE;
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert(got == ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

/* ---- VAR_STRING ---- */

START_TEST(scan_string_basic) {
  const char* input = "hello";
  char ref[32] = {0}, buf[32] = {0};
  int n = 0;
  sscanf(input, "%31s%n", ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_STRING;
  spec.val = 's';
  ScanState state = {&spec, input, input, buf};
  const char* next = scan_string_var(&state);

  ck_assert_str_eq(buf, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_string_empty_input) {
  const char* input = "";
  char buf[8] = "xyz";
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_STRING;
  spec.val = 's';
  ScanState state = {&spec, input, input, buf};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_str_eq(buf, "xyz");
}
END_TEST

START_TEST(scan_string_with_width) {
  const char* input = "abc";
  char ref[8] = "!!";
  int n = 0;
  sscanf(input, "%2s%n", ref, &n);

  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_STRING;
  spec.val = 's';
  spec.has_width = 1;
  spec.width = 2;
  char buf[8] = "!!";
  ScanState state = {&spec, input, input, buf};
  const char* next = scan_string_var(&state);

  ck_assert_str_eq(buf, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

/* ---- VAR_NREAD ---- */

START_TEST(scan_nread_zero) {
  const char* input = "foo";
  int n = -999;
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_NREAD;
  spec.val = 'n';
  ScanState state = {&spec, input, input, &n};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_int_eq(n, 0);
}
END_TEST

START_TEST(scan_nread_empty) {
  const char* input = "";
  int n = 42;
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_NREAD;
  spec.val = 'n';
  ScanState state = {&spec, input, input, &n};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input);
  ck_assert_int_eq(n, 0);
}
END_TEST

/* ---- VAR_POINTER ---- */

START_TEST(scan_pointer_hex) {
  const char* input = "0x1234";
  void* ref = NULL;
  int n = 0;
  sscanf(input, "%p%n", &ref, &n);

  void* got = NULL;
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_POINTER;
  spec.val = 'p';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

START_TEST(scan_pointer_uppercase) {
  const char* input = "0Xabcd";
  void* ref = NULL;
  int n = 0;
  sscanf(input, "%p%n", &ref, &n);

  void* got = NULL;
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_POINTER;
  spec.val = 'p';
  ScanState state = {&spec, input, input, &got};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(got, ref);
  ck_assert_ptr_eq(next, input + n);
}
END_TEST

/* ---- VAR_SYMBOL ---- */

START_TEST(scan_symbol_percent) {
  const char* input = "%";
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_SYMBOL;
  spec.val = '%';
  int dummy = 111;
  ScanState state = {&spec, input, input, &dummy};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input + 1);
  ck_assert_int_eq(dummy, 111);
}
END_TEST

START_TEST(scan_symbol_percent_then_more) {
  const char* input = "%%x";
  s21_specifier spec = {};
  spec.valid = 1;
  spec.var = VAR_SYMBOL;
  spec.val = '%';
  char dummy = '?';
  ScanState state = {&spec, input, input, &dummy};
  const char* next = scan_string_var(&state);

  ck_assert_ptr_eq(next, input + 1);
  ck_assert_int_eq((unsigned char)dummy, (unsigned char)'?');
}
END_TEST

static Suite* scan_suite(void) {
  Suite* s = suite_create("scan_string_var");

  TCase* tc_char = tcase_create("char");
  tcase_add_test(tc_char, scan_char_single_letter);
  tcase_add_test(tc_char, scan_char_digit);
  tcase_add_test(tc_char, scan_char_space);
  tcase_add_test(tc_char, scan_char_null_byte);
  tcase_add_test(tc_char, scan_char_consumes_one);
  tcase_add_test(tc_char, scan_char_special);
  tcase_add_test(tc_char, scan_char_newline);
  suite_add_tcase(s, tc_char);

  TCase* tc_d = tcase_create("decimal_d");
  tcase_add_test(tc_d, scan_decimal_d_positive);
  tcase_add_test(tc_d, scan_decimal_d_negative);
  tcase_add_test(tc_d, scan_decimal_d_plus_prefix);
  tcase_add_test(tc_d, scan_decimal_d_zero);
  tcase_add_test(tc_d, scan_decimal_d_trailing_garbage);
  tcase_add_test(tc_d, scan_decimal_d_width_limits_digits);
  tcase_add_test(tc_d, scan_decimal_d_width_one);
  tcase_add_test(tc_d, scan_decimal_d_len_short);
  tcase_add_test(tc_d, scan_decimal_d_len_long);
  tcase_add_test(tc_d, scan_decimal_d_len_long_long);
  tcase_add_test(tc_d, scan_decimal_d_negative_long_long);
  tcase_add_test(tc_d, scan_decimal_d_no_digits_after_sign);
  tcase_add_test(tc_d, scan_decimal_d_empty_string);
  tcase_add_test(tc_d, scan_decimal_d_width_zero);
  tcase_add_test(tc_d, scan_decimal_d_large_width);
  tcase_add_test(tc_d, scan_decimal_d_negative_with_width);
  tcase_add_test(tc_d, scan_decimal_d_single_zero);
  tcase_add_test(tc_d, scan_decimal_d_minus_zero);
  tcase_add_test(tc_d, scan_decimal_d_width_larger_than_input);
  tcase_add_test(tc_d, scan_decimal_d_minus_only);
  suite_add_tcase(s, tc_d);

  TCase* tc_u = tcase_create("decimal_u");
  tcase_add_test(tc_u, scan_decimal_u_positive);
  tcase_add_test(tc_u, scan_decimal_u_zero);
  tcase_add_test(tc_u, scan_decimal_u_plus_prefix);
  tcase_add_test(tc_u, scan_decimal_u_width);
  tcase_add_test(tc_u, scan_decimal_u_len_short);
  tcase_add_test(tc_u, scan_decimal_u_len_long);
  tcase_add_test(tc_u, scan_decimal_u_len_long_long);
  tcase_add_test(tc_u, scan_decimal_u_empty);
  suite_add_tcase(s, tc_u);

  TCase* tc_float = tcase_create("float");
  tcase_add_test(tc_float, scan_float_f);
  tcase_add_test(tc_float, scan_float_e);
  tcase_add_test(tc_float, scan_float_g);
  tcase_add_test(tc_float, scan_float_E);
  tcase_add_test(tc_float, scan_float_G);
  tcase_add_test(tc_float, scan_float_long_double);
  suite_add_tcase(s, tc_float);

  TCase* tc_string = tcase_create("string");
  tcase_add_test(tc_string, scan_string_basic);
  tcase_add_test(tc_string, scan_string_empty_input);
  tcase_add_test(tc_string, scan_string_with_width);
  suite_add_tcase(s, tc_string);

  TCase* tc_nread = tcase_create("nread");
  tcase_add_test(tc_nread, scan_nread_zero);
  tcase_add_test(tc_nread, scan_nread_empty);
  suite_add_tcase(s, tc_nread);

  TCase* tc_pointer = tcase_create("pointer");
  tcase_add_test(tc_pointer, scan_pointer_hex);
  tcase_add_test(tc_pointer, scan_pointer_uppercase);
  suite_add_tcase(s, tc_pointer);

  TCase* tc_symbol = tcase_create("symbol");
  tcase_add_test(tc_symbol, scan_symbol_percent);
  tcase_add_test(tc_symbol, scan_symbol_percent_then_more);
  suite_add_tcase(s, tc_symbol);

  return s;
}

int main(void) {
  int failed;
  SRunner* sr = srunner_create(scan_suite());
  // srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
