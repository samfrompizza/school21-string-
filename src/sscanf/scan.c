#include "scan.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "../common/s21_format.h"
#include "../common/s21_parse_helpers.h"
#include "../string/s21_string.h"
#include "scan_helpers.h"

const char* scan_string_var(ScanState* state);

static const char* scan_char(ScanState* state);
static const char* scan_decimal(ScanState* state);
static const char* scan_float(ScanState* state);
static const char* scan_string(ScanState* state);
static const char* scan_nread(ScanState* state);
static const char* scan_pointer(ScanState* state);
static const char* scan_symbol(ScanState* state);

const char* scan_string_var(ScanState* state) {
  switch (state->spec->var) {
    case VAR_CHAR:
      state->str = scan_char(state);
      break;
    case VAR_DECIMAL:
      state->str = scan_decimal(state);
      break;
    case VAR_FLOAT:
      state->str = scan_float(state);
      break;
    case VAR_STRING:
      state->str = scan_string(state);
      break;
    case VAR_NREAD:
      state->str = scan_nread(state);
      break;
    case VAR_POINTER:
      state->str = scan_pointer(state);
      break;
    case VAR_SYMBOL:
      state->str = scan_symbol(state);
      break;
    default:
      break;
  }
  return state->str;
}

static const char* scan_char(ScanState* state) {
  int width =
      state->spec->has_width && state->spec->width > 0 ? state->spec->width : 1;
  s21_size n = (s21_size)width;
  if (n > 0) {
    s21_memcpy(state->param, state->str, n);
    return state->str + n;
  }
  return state->str;
}

static const char* scan_decimal(ScanState* state) {
  const char* p = state->str;
  s21_skip_whitespace(&p);

  int width_left =
      state->spec->has_width ? state->spec->width : SCAN_WIDTH_UNLIMITED;
  if (width_left <= 0) {
    return state->str;
  }

  int sign = 1;
  int base = 10;
  int allow_sign = 1;

  scan_decimal_parse_prefix(&p, &width_left, state->spec->val, &base,
                            &allow_sign);

  if (allow_sign && width_left > 0 && (*p == '+' || *p == '-')) {
    sign = (*p == '-') ? -1 : 1;
    p++;
    width_left--;
  }

  const char* start = p;
  unsigned long long value = scan_decimal_parse_digits(&p, &width_left, base);
  int parsed = (p != start);

  if (!parsed) {
    if (p != state->str) {
      state->str = p;
    }
    return state->str;
  }

  state->str = p;
  if (state->spec->val == 'u' || state->spec->val == 'o' ||
      state->spec->val == 'x' || state->spec->val == 'X') {
    scan_assign_unsigned(state, value);
  } else {
    scan_assign_signed(state, sign * (long long)value);
  }
  return state->str;
}

#define FLOAT_BUF_SIZE 512

static const char* scan_float(ScanState* state) {
  s21_skip_whitespace(&state->str);

  int width_left = scan_get_width(state->spec);
  if (width_left <= 0 || !*state->str) {
    return state->str;
  }

  char buf[FLOAT_BUF_SIZE];
  s21_size n = (s21_size)width_left;
  if (n >= FLOAT_BUF_SIZE) {
    n = FLOAT_BUF_SIZE - 1;
  }
  const char* src = state->str;
  s21_size i = 0;
  while (i < n && src[i]) {
    i++;
  }
  s21_memcpy(buf, src, i);
  buf[i] = '\0';

  char* end = (char*)(uintptr_t)0;
  double d = strtod(buf, &end);
  s21_size consumed = (s21_size)(end - buf);
  if (consumed == 0) {
    return state->str;
  }

  if (state->spec->len == LEN_LONG_DOUBLE) {
    *(long double*)(state->param) = (long double)d;
  } else {
    *(double*)(state->param) = d;
  }
  state->str = state->str + consumed;
  return state->str;
}

static const char* scan_string(ScanState* state) {
  const char* p = state->str;
  s21_skip_whitespace(&state->str);

  int width_left = scan_get_width(state->spec);
  if (width_left <= 0 || !*p) {
    return state->str;
  }

  char* dest = (char*)state->param;
  s21_size n = 0;
  while (width_left > 0 && *p && !isspace((unsigned char)*p)) {
    *dest++ = *p++;
    n++;
    width_left--;
  }
  *dest = '\0';
  if (n == 0) {
    return state->str;
  }
  state->str = p;
  return state->str;
}

static const char* scan_nread(ScanState* state) {
  int n = (int)(state->str - state->str_start);
  switch (state->spec->len) {
    case LEN_DEFAULT:
      *(int*)(state->param) = n;
      break;
    case LEN_SHORT:
      *(short*)(state->param) = (short)n;
      break;
    case LEN_LONG:
      *(long*)(state->param) = (long)n;
      break;
    case LEN_LONG_LONG:
      *(long long*)(state->param) = (long long)n;
      break;
    default:
      break;
  }
  return state->str;
}

static const char* scan_pointer(ScanState* state) {
  const char* p = state->str;
  s21_skip_whitespace(&state->str);

  int width_left = scan_get_width(state->spec);
  if (width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
    p += 2;
    width_left -= 2;
  }
  unsigned long long value = 0;
  int parsed = 0;
  while (width_left > 0 && *p && s21_isxdigit_parse(*p)) {
    value = value * 16 + (unsigned long long)s21_xdigit_value(*p);
    p++;
    width_left--;
    parsed = 1;
  }
  if (!parsed) {
    return state->str;
  }
  *(void**)(state->param) = (void*)(uintptr_t)value;
  state->str = p;
  return state->str;
}

static const char* scan_symbol(ScanState* state) {
  if (*state->str == '%') {
    return state->str + 1;
  }
  return state->str;
}
