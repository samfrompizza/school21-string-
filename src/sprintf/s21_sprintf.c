#include "s21_sprintf.h"
#include "../common/s21_format.h"
#include "../string/s21_string.h"

static void handle_spec_error(s21_specifier *spec);
static char *handle_char(char *str, s21_specifier *spec, char c);
static char *handle_decimal(char *str, s21_specifier *spec, ...);
static char *handle_float(char *str, s21_specifier *spec, ...);
static char *handle_nread(char *str, s21_specifier *spec, int n);
static char *handle_pointer(char *str, s21_specifier *spec, void *p);
static char *handle_string(char *str, s21_specifier *spec, const char *s);
static char *handle_percent(char *str, s21_specifier *spec);

int s21_sprintf(char *str, const char *format, ...) {
  if (str == NULL || format == NULL) return 0;

  va_list ap;
  va_start(ap, format);

  const char *fmt = format;
  char *start = str;

  while (*fmt != '\0') {
    if (*fmt != '%') {
      *str = *fmt;
      str++;
      fmt++;
    } else {
      fmt++;
      s21_specifier spec = {};
      fmt = s21_parse_spec(fmt, &spec);
      s21_validate_spec(&spec);

      if (!spec.valid) {
        handle_spec_error(&spec);
        va_end(ap);
        return -1;
      }

      switch (spec.var)
      {
      case VAR_CHAR:
        /* code */
        break;
      case VAR_DECIMAL:
        /* code */
        break;
      case VAR_FLOAT:
        /* code */
        break;
      case VAR_NREAD:
        /* code */
        break;
      case VAR_POINTER:
        /* code */
        break;
      case VAR_STRING:
        /* code */
        break;
      case VAR_SYMBOL:
        /* code */
        break;
      
      default:
        break;
      }
    }
  }
  
  *str = '\0';
  va_end(ap);
  return str - start;
}