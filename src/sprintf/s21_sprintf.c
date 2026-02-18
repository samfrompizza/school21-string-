#include "s21_sprintf_helpers.h"
#include "s21_sprintf.h"
#include "../common/s21_format.h"

static int append_to_output(char **dst, const char *src, int len) {
  if (dst == S21_NULL || *dst == S21_NULL || src == S21_NULL || len < 0) return -1;

  for (int i = 0; i < len; i++) {
    **dst = src[i];
    (*dst)++;
  }

  return 0;
}

int s21_sprintf(char *str, const char *format, ...) {
  if (str == S21_NULL || format == S21_NULL) return -1;

  va_list ap;
  va_start(ap, format);

  const char *fmt = format;
  char *start = str;

  while (*fmt != '\0') {
    if (*fmt != '%') {
      *str = *fmt;
      str++;
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

    read_width_precision_from_args(&spec, &ap);

    switch (spec.var) {
      case VAR_DECIMAL: {
        if (spec.val != 'd' && spec.val != 'i') {
          va_end(ap);
          return -1;
        }

        char out_buf[S21_INT_BUF] = {0};
        int out_len = handle_decimal_spec(out_buf, sizeof(out_buf), &spec, &ap);
        if (out_len < 0 || append_to_output(&str, out_buf, out_len) < 0) {
          va_end(ap);
          return -1;
        }
        break;
      }
      default:
        va_end(ap);
        return -1;
    }
  }

  *str = '\0';
  va_end(ap);
  return (int)(str - start);
}
