#include "scan.h"

#include <ctype.h>

#include "../common/s21_format.h"

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
  *(char*)state->param = *state->str;
  return state->str + 1;
}

static const char* scan_decimal(ScanState* state) {
  int sign = 1;

  if (*(state->str) == '+' || *(state->str) == '-') {
    sign = *(state->str) == '+' ? +1 : -1;
    ++state->str;
    if (state->spec->width) {
      state->spec->width--;
    }
  }

  unsigned long long value = 0;
  int parsed_digits = 0;

  while (*(state->str) && isdigit(*(state->str)) && state->spec->width--) {
    value = value * 10 + *(state->str++ - '0');
    ++parsed_digits;
  }

  if (parsed_digits == 0) {
    return state->str;
  }

  if (state->spec->val == 'u') {
    switch (state->spec->len) {
      case LEN_DEFAULT:
        *(unsigned int*)(state->param) = value;
        break;
      case LEN_SHORT:
        *(unsigned short*)(state->param) = value;
        break;
      case LEN_LONG:
        *(unsigned long*)(state->param) = value;
        break;
      case LEN_LONG_LONG:
        *(unsigned long long*)(state->param) = value;
        break;
      default:
        break;
    }
  } else {
    long long signed_value = sign * (long long)value;
    switch (state->spec->len) {
      case LEN_DEFAULT:
        *(int*)(state->param) = signed_value;
        break;
      case LEN_SHORT:
        *(short*)(state->param) = signed_value;
        break;
      case LEN_LONG:
        *(long*)(state->param) = signed_value;
        break;
      case LEN_LONG_LONG:
        *(long long*)(state->param) = signed_value;
        break;
      default:
        break;
    }
  }
}

static const char* scan_float(ScanState* state) {}

static const char* scan_string(ScanState* state) {}

static const char* scan_nread(ScanState* state) {}

static const char* scan_pointer(ScanState* state) {}

static const char* scan_symbol(ScanState* state) {}
