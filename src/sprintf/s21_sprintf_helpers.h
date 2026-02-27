#ifndef S21_FORMAT_HELPERS_H
#define S21_FORMAT_HELPERS_H

#include <stdarg.h>
#include <stdbool.h>

#include "../common/s21_format.h"
#include "../string/s21_string.h"

#define S21_INT_BUF 128
#define S21_FLOAT_BUF 512

long long s21_get_signed_arg(va_list *ap, const s21_specifier *spec);
unsigned long long s21_get_unsigned_arg(va_list *ap, const s21_specifier *spec);
long double s21_get_float_arg(va_list *ap, const s21_specifier *spec);

void s21_read_width_precision_from_args(s21_specifier *spec, va_list *ap);

int s21_format_char(char *out_buf, s21_size out_size, int ch,
                    const s21_specifier *spec);
int s21_format_string(char *out_buf, s21_size out_size, const char *src,
                      const s21_specifier *spec);
int s21_format_percent(char *out_buf, s21_size out_size,
                       const s21_specifier *spec);
int s21_format_signed_decimal(char *out_buf, s21_size out_size,
                              const s21_specifier *spec, va_list *ap);
int s21_format_unsigned(char *out_buf, s21_size out_size,
                        const s21_specifier *spec, va_list *ap);
int s21_format_pointer(char *out_buf, s21_size out_size,
                       const s21_specifier *spec, va_list *ap);
int s21_format_float_fixed(char *out_buf, s21_size out_size,
                           const s21_specifier *spec, va_list *ap);

#endif
