/* POSIX sscanf: %[*][width][length]specifier.
 * Assumption: %* before specifier means suppress assignment (not
 * width-from-arg); we pre-parse and skip * so s21_parse_spec sees
 * width/length/spec only. */

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>

#include "../common/s21_format.h"
#include "../string/s21_string.h"
#include "scan.h"

#define DUMMY_SIZE 64

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int assigned = 0;
  const char* format_p = format;
  const char* str_p = str;
  char dummy[DUMMY_SIZE];

  while (*format_p) {
    if (isspace((unsigned char)*format_p)) {
      while (*format_p && isspace((unsigned char)*format_p)) {
        format_p++;
      }
      while (*str_p && isspace((unsigned char)*str_p)) {
        str_p++;
      }
    } else if (*format_p != '%') {
      if (*str_p != *format_p) {
        break;
      }
      format_p++;
      str_p++;
    } else {
      format_p++;
      if (!*format_p) {
        break;
      }

      bool suppress = (*format_p == '*');
      if (suppress) {
        format_p++;
      }

      s21_specifier spec = {0};
      format_p = s21_parse_spec(format_p, &spec);
      s21_validate_spec(&spec);

      if (!spec.valid) {
        break;
      }

      if (spec.var == VAR_SYMBOL) {
        if (*str_p != '%') {
          break;
        }
        str_p++;
        continue;
      }

      void* param = suppress ? dummy : va_arg(args, void*);

      ScanState state = {
          .spec = &spec, .str = str_p, .str_start = str, .param = param};

      const char* next = scan_string_var(&state);
      if (spec.var != VAR_NREAD && next == str_p) {
        break;
      }
      str_p = next;

      if (!suppress) {
        assigned++;
      }
    }
  }

  va_end(args);
  return assigned;
}
