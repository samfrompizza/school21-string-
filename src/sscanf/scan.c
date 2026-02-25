#include "scan.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "../common/s21_format.h"
#include "../string/s21_string.h"

static void copy_bytes(void* dest, const void* src, s21_size n) {
  unsigned char* d = (unsigned char*)dest;
  const unsigned char* s = (const unsigned char*)src;
  for (s21_size i = 0; i < n; i++) {
    d[i] = s[i];
  }
}

#define WIDTH_UNLIMITED 8192

/* Returns 1 if ch is octal digit */
static int isoctal(int ch) { return ch >= '0' && ch <= '7'; }

/* Returns 1 if ch is hex digit */
static int isxdigit_custom(int ch) {
  return isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

static int xdigit_value(int ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  }
  if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  }
  if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  }
  return -1;
}

const char* scan_string_var(ScanState* state);

static const char* scan_char(ScanState* state);
static const char* scan_decimal(ScanState* state);
static void assign_signed_decimal(ScanState* state, long long value);
static void assign_unsigned_decimal(ScanState* state, unsigned long long value);
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
    copy_bytes(state->param, state->str, n);
    return state->str + n;
  }
  return state->str;
}

static const char* scan_decimal(ScanState* state) {
  const char* p = state->str;
  while (*p && isspace((unsigned char)*p)) {
    p++;
  }

  int width_left =
      state->spec->has_width ? state->spec->width : WIDTH_UNLIMITED;
  if (width_left <= 0) {
    return state->str;
  }

  int sign = 1;
  int base = 10;
  int allow_sign = 1;

  if (state->spec->val == 'i') {
    if (width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      base = 16;
      p += 2;
      width_left -= 2;
      allow_sign = 0;
    } else if (width_left >= 1 && p[0] == '0') {
      base = 8;
      allow_sign = 0;
    }
  } else if (state->spec->val == 'o') {
    base = 8;
    allow_sign = 0;
  } else if (state->spec->val == 'x' || state->spec->val == 'X') {
    if (width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      p += 2;
      width_left -= 2;
    }
    base = 16;
    allow_sign = 0;
  }

  if (allow_sign && width_left > 0 && (*p == '+' || *p == '-')) {
    sign = (*p == '-') ? -1 : 1;
    p++;
    width_left--;
  }

  unsigned long long value = 0;
  int parsed = 0;

  if (base == 10) {
    while (width_left > 0 && *p && isdigit(*p)) {
      value = value * 10 + (unsigned long long)(*p - '0');
      p++;
      width_left--;
      parsed = 1;
    }
  } else if (base == 8) {
    while (width_left > 0 && *p && isoctal(*p)) {
      value = value * 8 + (unsigned long long)(*p - '0');
      p++;
      width_left--;
      parsed = 1;
    }
  } else {
    while (width_left > 0 && *p && isxdigit_custom(*p)) {
      value = value * 16 + (unsigned long long)xdigit_value(*p);
      p++;
      width_left--;
      parsed = 1;
    }
  }

  if (!parsed) {
    if (p != state->str) {
      state->str = p;
    }
    return state->str;
  }

  state->str = p;
  if (state->spec->val == 'u' || state->spec->val == 'o' ||
      state->spec->val == 'x' || state->spec->val == 'X') {
    assign_unsigned_decimal(state, value);
  } else {
    assign_signed_decimal(state, sign * (long long)value);
  }
  return state->str;
}

static void assign_signed_decimal(ScanState* state, long long value) {
  switch (state->spec->len) {
    case LEN_DEFAULT:
      *(int*)(state->param) = (int)value;
      break;
    case LEN_SHORT:
      *(short*)(state->param) = (short)value;
      break;
    case LEN_LONG:
      *(long*)(state->param) = (long)value;
      break;
    case LEN_LONG_LONG:
      *(long long*)(state->param) = value;
      break;
    default:
      break;
  }
}

static void assign_unsigned_decimal(ScanState* state,
                                    unsigned long long value) {
  switch (state->spec->len) {
    case LEN_DEFAULT:
      *(unsigned int*)(state->param) = (unsigned int)value;
      break;
    case LEN_SHORT:
      *(unsigned short*)(state->param) = (unsigned short)value;
      break;
    case LEN_LONG:
      *(unsigned long*)(state->param) = (unsigned long)value;
      break;
    case LEN_LONG_LONG:
      *(unsigned long long*)(state->param) = value;
      break;
    default:
      break;
  }
}

#define FLOAT_BUF_SIZE 512

static const char* scan_float(ScanState* state) {
  const char* p = state->str;
  while (*p && isspace((unsigned char)*p)) {
    p++;
  }
  state->str = p;

  int width_left = state->spec->has_width && state->spec->width > 0
                       ? state->spec->width
                       : WIDTH_UNLIMITED;
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
  copy_bytes(buf, src, i);
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
  while (*p && isspace((unsigned char)*p)) {
    p++;
  }

  int width_left = state->spec->has_width && state->spec->width > 0
                       ? state->spec->width
                       : WIDTH_UNLIMITED;
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
  while (*p && isspace((unsigned char)*p)) {
    p++;
  }

  int width_left = state->spec->has_width && state->spec->width > 0
                       ? state->spec->width
                       : WIDTH_UNLIMITED;
  if (width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
    p += 2;
    width_left -= 2;
  }
  unsigned long long value = 0;
  int parsed = 0;
  while (width_left > 0 && *p && isxdigit_custom(*p)) {
    value = value * 16 + (unsigned long long)xdigit_value(*p);
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
