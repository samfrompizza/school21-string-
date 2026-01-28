#include <ctype.h>
#include <stdarg.h>

#include "../common/s21_format.h"
#include "../string/s21_string.h"
#include "scan.h"

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int status = 0;
  s21_size specifier_counter = 0;

  const char* format_p = format;
  const char* str_p = str;

  while (*format_p && !status) {
    s21_specifier specifier = {};

    if (isspace(*format_p)) {  // skip spaces in format and str
      for (; isspace(*format_p); ++format_p);
      for (; isspace(*str_p); ++str_p);
    } else if (*format_p != '%') {  // compare symbol
      if (*format_p == *str_p) {
        format_p++;
        str_p++;
      } else {
        status = 1;
      }
    } else {  // parsing specifier
      format_p = s21_parse_spec(format_p, &specifier);
      s21_validate_spec(&specifier);
      void* param = va_arg(args, void*);
      if (specifier.width_from_arg) {
        ScanState state = {&specifier, str_p, param};
        str_p = scan_string_var(&state);
        ++specifier_counter;
      }
    }
  }

  va_end(args);

  return status;
}
