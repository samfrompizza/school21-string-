#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "../../src/sprintf/s21_sprintf.h"

#define BUF_SIZE 256

static int g_failed = 0;

static void check_format(const char *name, const char *fmt, long long value) {
  char s21_buf[BUF_SIZE] = {0};
  char std_buf[BUF_SIZE] = {0};

  int s21_ret = s21_sprintf(s21_buf, fmt, value);
  int std_ret = sprintf(std_buf, fmt, value);

  if (s21_ret != std_ret || strcmp(s21_buf, std_buf) != 0) {
    g_failed++;
    printf("[FAIL] %s | fmt='%s' value=%lld\n", name, fmt, value);
    printf("  s21: ret=%d str='%s'\n", s21_ret, s21_buf);
    printf("  std: ret=%d str='%s'\n", std_ret, std_buf);
  }
}

int main(void) {
  check_format("basic_zero", "%d", 0);
  check_format("basic_pos", "%d", 42);
  check_format("basic_neg", "%d", -42);

  check_format("flag_plus", "%+d", 42);
  check_format("flag_space", "% d", 42);
  check_format("flag_plus_neg", "%+d", -42);

  check_format("width_right", "%5d", 42);
  check_format("width_left", "%-5d", 42);
  check_format("width_zero", "%05d", 42);
  check_format("width_zero_neg", "%05d", -42);

  check_format("prec_zero_zero", "%.0d", 0);
  check_format("prec_simple", "%.3d", 42);
  check_format("width_prec", "%8.3d", 42);
  check_format("width_prec_left", "%-8.3d", 42);

  check_format("length_h", "%hd", (int)(short)-123);
  check_format("length_l", "%ld", LONG_MIN);

  if (g_failed == 0) {
    printf("All sprintf %%d tests passed.\n");
    return 0;
  }

  printf("Failed tests: %d\n", g_failed);
  return 1;
}
