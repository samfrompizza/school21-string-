#include "s21_format.h"

#include <ctype.h>

#include "../string/s21_string.h"

// Helper static functions declaration
static const char* parse_flags(const char* format, s21_specifier* spec);
static const char* parse_width(const char* format, s21_specifier* spec);
static const char* parse_precision(const char* format, s21_specifier* spec);
static const char* parse_length(const char* format, s21_specifier* spec);
static const char* parse_spec_letter(const char* format, s21_specifier* spec);

// Helper static functions implementation
static const char* parse_flags(const char* format, s21_specifier* spec) {
  while (true) {
    switch (*format) {
      case '-':
        spec->flags.left_align = true;
        break;
      case '+':
        spec->flags.show_sign = true;
        break;
      case ' ':
        spec->flags.space_sign = true;
        break;
      case '#':
        spec->flags.alt_form = true;
        break;
      case '0':
        spec->flags.zero_pad = true;
        break;
      default:
        return format;
    }
    format++;
  }
}

static const char* parse_width(const char* format, s21_specifier* spec) {
  if (*format == '*') {
    spec->width_from_arg = true;
    spec->has_width = true;
    format++;
  } else if (isdigit(*format)) {
    spec->has_width = true;
    spec->width = 0;
    while (isdigit(*format)) {
      spec->width = spec->width * 10 + (*format - '0');
      format++;
    }
  }

  return format;
}

static const char* parse_precision(const char* format, s21_specifier* spec) {
  if (*format == '.') {
    spec->has_precision = true;
    format++;
  }

  if (spec->has_precision) {
    if (*format == '*') {
      spec->precision_from_arg = true;
      format++;
    } else if (isdigit(*format)) {
      spec->precision = 0;
      while (isdigit(*format)) {
        spec->precision = spec->precision * 10 + (*format - '0');
        format++;
      }
    } else {
      spec->precision = 0;
    }
  }

  return format;
}

static const char* parse_length(const char* format, s21_specifier* spec) {
  if (*format == 'h') {
    spec->len = LEN_SHORT;
    format++;
  } else if (*format == 'l') {
    format++;
    if (*format == 'l') {
      spec->len = LEN_LONG_LONG;
      format++;
    } else {
      spec->len = LEN_LONG;
    }
  } else if (*format == 'L') {
    spec->len = LEN_LONG_DOUBLE;
    format++;
  }

  return format;
}

static const char* parse_spec_letter(const char* format, s21_specifier* spec) {
  spec->valid = true;  // Single responsibility??
  spec->val = *format;

  if (*format == 'c') {
    spec->var = VAR_CHAR;
  } else if (s21_strchr("diouxX", *format)) {
    spec->var = VAR_DECIMAL;
  } else if (s21_strchr("eEfgG", *format)) {
    spec->var = VAR_FLOAT;
  } else if (*format == 's') {
    spec->var = VAR_STRING;
  } else if (*format == 'p') {
    spec->var = VAR_POINTER;
  } else if (*format == 'n') {
    spec->var = VAR_NREAD;
  } else if (*format == '%') {
    spec->var = VAR_SYMBOL;
  } else {
    spec->valid = false;
  }

  if (spec->valid) {
    format++;
  }

  return format;
}

// Public API
const char* s21_parse_spec(const char* format, s21_specifier* spec) {
  if (!format) {
    return S21_NULL;
  }

  format = parse_flags(format, spec);
  format = parse_width(format, spec);
  format = parse_precision(format, spec);
  format = parse_length(format, spec);
  format = parse_spec_letter(format, spec);

  return format;
}

void s21_validate_spec(s21_specifier* spec) {
  if (!spec->valid) {
    return;
  }

  // '-' declines '0'
  if (spec->flags.left_align) {
    spec->flags.zero_pad = false;
  }

  // '+' declines ' '
  if (spec->flags.show_sign) {
    spec->flags.space_sign = false;
  }

  // precision declines '0' for some types
  if (spec->has_precision && spec->var == VAR_DECIMAL) {
    spec->flags.zero_pad = false;
  }

  if (spec->val == 'u' || spec->val == 'o' || spec->val == 'x' ||
      spec->val == 'X' || spec->val == 's' || spec->val == 'c' ||
      spec->val == 'p') {
    spec->flags.show_sign = false;
    spec->flags.space_sign = false;
  }
}