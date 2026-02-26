#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../common/s21_format.h"
#include "../string/s21_string.h"
#include "scan.h"
#include "../common/s21_parse_helpers.h"

#define DUMMY_SIZE 64

static inline void
sscanf_skip_whitespace(const char** format_p, const char** str_p) {
  s21_skip_whitespace(format_p);
  s21_skip_whitespace(str_p);
}

static inline int
sscanf_match_literal(const char** format_p, const char** str_p) {
  if (**str_p != **format_p) {
    return 0;
  }
  (*format_p)++;
  (*str_p)++;
  return 1;
}

static int sscanf_process_spec(const char** format_p, const char** str_p,
                               const char* str_start, va_list* args,
                               int* assigned) {
  const char* format = *format_p;
  const char* str = *str_p;

  format++;
  if (!*format) {
    return -1;
  }

  bool suppress = (*format == '*');
  if (suppress) {
    format++;
  }

  s21_specifier spec = {0};
  format = s21_parse_spec(format, &spec);
  s21_validate_spec(&spec);

  if (!spec.valid) {
    return -1;
  }

  if (spec.var == VAR_SYMBOL) {
    if (*str != '%') {
      return -1;
    }
    *format_p = format;
    *str_p = str + 1;
    return 0;
  }

  char dummy[DUMMY_SIZE];
  void* param = suppress ? dummy : va_arg(*args, void*);

  ScanState state = {
      .spec = &spec, .str = str, .str_start = str_start, .param = param};

  const char* next = scan_string_var(&state);
  if (spec.var != VAR_NREAD && next == str) {
    return -1;
  }

  *format_p = format;
  *str_p = next;
  if (!suppress) {
    (*assigned)++;
  }
  return 0;
}

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int assigned = 0;
  const char* format_p = format;
  const char* str_p = str;

  while (*format_p) {
    if (isspace((unsigned char)*format_p)) {
      sscanf_skip_whitespace(&format_p, &str_p);
    } else if (*format_p != '%') {
      if (!sscanf_match_literal(&format_p, &str_p)) {
        break;
      }
    } else {
      int ret = sscanf_process_spec(&format_p, &str_p, str, &args, &assigned);
      if (ret < 0) {
        break;
      }
    }
  }

  va_end(args);
  return assigned;
}
