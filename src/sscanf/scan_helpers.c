#include "scan_helpers.h"

#include <ctype.h>

#include "../common/s21_parse_helpers.h"

int scan_get_width(const s21_specifier* spec) {
  return spec->has_width && spec->width > 0 ? spec->width
                                            : SCAN_WIDTH_UNLIMITED;
}

void scan_decimal_parse_prefix(const char** p_out, int* width_left,
                               char spec_val, int* base, int* allow_sign) {
  const char* p = *p_out;

  if (spec_val == 'i') {
    if (*width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      *base = 16;
      p += 2;
      *width_left -= 2;
      *allow_sign = 0;
    } else if (*width_left >= 1 && p[0] == '0') {
      *base = 8;
      *allow_sign = 0;
    }
  } else if (spec_val == 'o') {
    *base = 8;
    *allow_sign = 0;
  } else if (spec_val == 'x' || spec_val == 'X') {
    if (*width_left >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      p += 2;
      *width_left -= 2;
    }
    *base = 16;
    *allow_sign = 0;
  }

  *p_out = p;
}

unsigned long long
scan_decimal_parse_digits(const char** p_out, int* width_left, int base) {
  const char* p = *p_out;
  unsigned long long value = 0;

  if (base == 10) {
    while (*width_left > 0 && *p && isdigit(*p)) {
      value = value * 10 + (unsigned long long)(*p - '0');
      p++;
      (*width_left)--;
    }
  } else if (base == 8) {
    while (*width_left > 0 && *p && s21_isoctal(*p)) {
      value = value * 8 + (unsigned long long)(*p - '0');
      p++;
      (*width_left)--;
    }
  } else {
    while (*width_left > 0 && *p && s21_isxdigit_parse(*p)) {
      value = value * 16 + (unsigned long long)s21_xdigit_value(*p);
      p++;
      (*width_left)--;
    }
  }

  *p_out = p;
  return value;
}

void scan_assign_signed(ScanState* state, long long value) {
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

void scan_assign_unsigned(ScanState* state, unsigned long long value) {
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
