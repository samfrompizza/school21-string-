#include "scan.h"

#include <ctype.h>

#include "../common/s21_format.h"

const char* scan_char(ScanState* state);
const char* scan_decimal(ScanState* state);
const char* scan_float(ScanState* state);
const char* scan_string(ScanState* state);
const char* scan_nread(ScanState* state);
const char* scan_pointer(ScanState* state);
const char* scan_symbol(ScanState* state);

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

const char* scan_char(ScanState* state) {
  *(char*)state->param = *state->str;
  return state->str + 1;
}

const char* scan_decimal(ScanState* state) {
  if (state) {  // TODO: replace check with signed number
    long long value = 0;
    int sign = 1;
    int parsed_digits = 0;

    if (*(state->str) == '+' || *(state->str) == '-') {
      sign = *(state->str) == '+' ? +1 : -1;
      ++state->str;
      if (state->spec->width) {
        state->spec->width--;
      }
    }

    while (*(state->str) && isdigit(*(state->str)) && state->spec->width--) {
      value = value * 10 + *(state->str++ - '0');
      ++parsed_digits;
    }

    if (parsed_digits == 0) {
      return state->str;
    }

    value *= sign;

    switch (state->spec->len) {
      case LEN_DEFAULT:
        *(int*)(state->param) = value;
        break;
      case LEN_SHORT:
        *(short*)(state->param) = value;
        break;
      case LEN_LONG:
        *(long*)(state->param) = value;
        break;
      case LEN_LONG_LONG:
        *(long long*)(state->param) = value;
        break;
      default:
        break;
    }

    return state->str;
  } else {
    // TODO: add unsigned
  }
}
