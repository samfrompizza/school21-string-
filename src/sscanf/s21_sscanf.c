#include <ctype.h>
#include <stdarg.h>

#include "../common/format.h"
#include "../string/s21_string.h"

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int status = 0;
  s21_size specifier_counter = 0;
  s21_specifier specifier = {};

  while (*format && !status) {
    if (isspace(*format)) {
      while (isspace(*format)) {
        format++;
      }
      while (isspace(*str)) {
        str++;
      }
    } else if (*format != '%') {
      if (*format == *str) {
        format++;
        str++;
      } else {
        status = 1;
      }
    } else {
      s21_parse_spec(format, &specifier);
      s21_validate_spec(&specifier);
      if (!specifier.valid) {
        status = 2;
      } else {
        specifier_counter++;
      }
    }
  }

  va_end(args);

  
  return status;
}
