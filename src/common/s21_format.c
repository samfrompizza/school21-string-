#include "s21_format.h"

#include <ctype.h>
#include <stdlib.h>

#include "../string/s21_string.h"

// Helper static functions declaration
static const char *parse_flags(const char *format, s21_specifier *spec);
static const char *parse_width(const char *format, s21_specifier *spec);
static const char *parse_precision(const char *format, s21_specifier *spec);
static const char *parse_length(const char *format, s21_specifier *spec);

// Helper static functions implementation
static const char *parse_flags(const char *format, s21_specifier *spec) {
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

static const char *parse_width(const char *format, s21_specifier *spec) {
  if (*format == '*') {
    spec->width_from_arg = true;
    spec->has_width = true;
    format++;
  } else if (isdigit(*format)) {
    spec->has_width = true;
    spec->width = atoi(format);
  }

  return format;
}

static const char *parse_precision(const char *format, s21_specifier *spec) {
  if (*format == '.') {
    spec->has_precision = true;
    format++;
  }
  // TODO: precision set by *
  /*
  if (spec->has_precision) {
    spec->precision = atoi(format);
  }
  */
  return format;
}

static const char *parse_length(const char *format, s21_specifier *spec) {
  // логика длины
  return format;
}

// Public API
const char *s21_parse_spec(const char *format, s21_specifier *spec) {
  if (!format) return;

  format = parse_flags(*format, spec);
  format = parse_width(*format, spec);
  format = parse_precision(*format, spec);
  format = parse_length(*format, spec);

  return format;
}

void s21_validate_spec(s21_specifier *spec) {
  // логика валидации
}
