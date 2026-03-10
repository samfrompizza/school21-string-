#ifndef S21_SCAN_HELPERS_H_
#define S21_SCAN_HELPERS_H_

#include "scan.h"

#define SCAN_WIDTH_UNLIMITED 8192

int scan_get_width(const s21_specifier* spec);

void scan_decimal_parse_prefix(const char** p_out, int* width_left,
                               char spec_val, int* base);

unsigned long long
scan_decimal_parse_digits(const char** p_out, int* width_left, int base);

void scan_assign_signed(ScanState* state, long long value);
void scan_assign_unsigned(ScanState* state, unsigned long long value);

#endif
