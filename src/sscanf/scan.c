#include "scan.h"

#include <ctype.h>

#include "../common/s21_format.h"

// Public function declaration
const char* scan_string_var(ScanState* state);

// Static scan functions declaration
static const char* scan_char(ScanState* state);

static const char* scan_decimal(ScanState* state);
static void assign_signed_decimal(ScanState* state, long long value);
static void assign_unsigned_decimal(ScanState* state, unsigned long long value);

static const char* scan_float(ScanState* state);
static const char* scan_string(ScanState* state);
static const char* scan_nread(ScanState* state);
static const char* scan_pointer(ScanState* state);
static const char* scan_symbol(ScanState* state);

// Public function implementation
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

// Static functions implementation
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
    value = value * 10 + (*state->str++ - '0');
    ++parsed_digits;
  }

  if (parsed_digits == 0) {
    return state->str;
  }

  if (state->spec->val == 'u') {
    assign_unsigned_decimal(state, value);
  } else {
    assign_signed_decimal(state, sign * (long long)(value));
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
      *(long long*)(state->param) = (long long)value;
      break;
    default:
      break;
  }
}

static void assign_unsigned_decimal(ScanState* state,
                                    unsigned long long value) {
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
}

static const char* scan_float(ScanState* state) { return state->str; }

static const char* scan_string(ScanState* state) { return state->str; }

static const char* scan_nread(ScanState* state) { return state->str; }

static const char* scan_pointer(ScanState* state) { return state->str; }

static const char* scan_symbol(ScanState* state) { return state->str; }
