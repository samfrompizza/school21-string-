#include "s21_sprintf.h"

#include "../common/s21_format.h"
#include "s21_sprintf_helpers.h"

static int s21_append_fragment(char **dst, const char *src, int src_len) {
  if (dst == S21_NULL || *dst == S21_NULL || src == S21_NULL || src_len < 0) {
    return -1;
  }

  for (int i = 0; i < src_len; i++) {
    **dst = src[i];
    (*dst)++;
  }

  return 0;
}

static int s21_handle_specifier(char *fragment, s21_size fragment_size,
                                s21_specifier *spec, va_list *ap) {
  int len = -1;

  switch (spec->var) {
    case VAR_CHAR:
      len = s21_format_char(fragment, fragment_size, va_arg(*ap, int), spec);
      break;
    case VAR_STRING:
      len = s21_format_string(fragment, fragment_size, va_arg(*ap, const char *),
                              spec);
      break;
    case VAR_DECIMAL:
      if (spec->val == 'd' || spec->val == 'i') {
        len = s21_format_signed_decimal(fragment, fragment_size, spec, ap);
      } else if (spec->val == 'u' || spec->val == 'o' || spec->val == 'x' ||
                 spec->val == 'X') {
        len = s21_format_unsigned(fragment, fragment_size, spec, ap);
      }
      break;
    case VAR_FLOAT:
      if (spec->val == 'f') {
        len = s21_format_float_fixed(fragment, fragment_size, spec, ap);
      }
      break;
    case VAR_POINTER:
      len = s21_format_pointer(fragment, fragment_size, spec, ap);
      break;
    case VAR_SYMBOL:
      len = s21_format_percent(fragment, fragment_size, spec);
      break;
    default:
      break;
  }

  return len;
}

int s21_sprintf(char *str, const char *format, ...) {
  if (str == S21_NULL || format == S21_NULL) return -1;

  va_list ap;
  va_start(ap, format);

  const char *fmt = format;
  char *out = str;
  char *start = str;

  while (*fmt != '\0') {
    if (*fmt != '%') {
      *out = *fmt;
      out++;
      fmt++;
      continue;
    }

    fmt++;
    s21_specifier spec = {0};
    fmt = s21_parse_spec(fmt, &spec);
    s21_validate_spec(&spec);

    if (!spec.valid) {
      va_end(ap);
      return -1;
    }

    s21_read_width_precision_from_args(&spec, &ap);

    char fragment[S21_FLOAT_BUF] = {0};
    int fragment_len = s21_handle_specifier(fragment, sizeof(fragment), &spec, &ap);
    if (fragment_len < 0 || s21_append_fragment(&out, fragment, fragment_len) < 0) {
      va_end(ap);
      return -1;
    }
  }

  *out = '\0';
  va_end(ap);
  return (int)(out - start);
}
