#include "scan.h"

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
