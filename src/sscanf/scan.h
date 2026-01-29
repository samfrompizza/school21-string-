#ifndef S21_SCAN_H_
#define S21_SCAN_H_

#include <stdarg.h>

#include "../common/s21_format.h"

typedef struct {
  s21_specifier* spec;
  const char* str;

  void* param;
} ScanState;

extern const char* scan_string_var(ScanState* state);

#endif
