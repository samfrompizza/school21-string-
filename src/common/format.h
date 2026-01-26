#ifndef S21_FORMAT_H
#define S21_FORMAT_H

#include <stdbool.h>

typedef enum {
  c, d, i, e, E, f, g, G, o, s, u, x, X, p, n, symbol,
} s21_format_spec;

typedef enum {
  CHAR,
  DECIMAL,
  FLOAT,
  STRING,
  NREAD,
  POINTER,
  SYMBOL,
} s21_variable_type;

typedef enum {
  SHORT,
  DEFAULT,
  LONG,
  LONG_LONG,
} s21_format_length;

typedef struct {
  bool left_align;
  bool show_sign;
  bool space_sign;
  bool alt_form;
  bool zero_pad;
} s21_format_flags;

typedef struct {
  s21_format_spec spec;
  s21_variable_type var;
  s21_format_length len;
  s21_format_flags flags;
  int width;
  bool has_width;
  int precision;
  bool has_precision;
  bool width_from_arg;
  bool precision_from_arg;
  bool valid;
} s21_specifier;

void init_spec(s21_specifier *spec);
void parse_spec(const char **format, int pos, s21_specifier *spec);
void validate_spec(s21_specifier);

#endif