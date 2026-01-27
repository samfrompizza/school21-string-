#ifndef S21_FORMAT_H
#define S21_FORMAT_H

#include <stdbool.h>

typedef enum {
  c,
  d,
  i,
  e,
  E,
  f,
  g,
  G,
  o,
  s,
  u,
  x,
  X,
  p,
  n,
  percent,
} s21_format_spec;

typedef enum {
  VAR_CHAR,
  VAR_DECIMAL,
  VAR_FLOAT,
  VAR_STRING,
  VAR_NREAD,
  VAR_POINTER,
  VAR_SYMBOL,
} s21_variable_type;

typedef enum {
  LEN_SHORT,
  LEN_DEFAULT,
  LEN_LONG,
  LEN_LONG_LONG,
} s21_format_length;

typedef struct {
  bool left_align;
  bool show_sign;
  bool space_sign;
  bool alt_form;
  bool zero_pad;
} s21_format_flags;

typedef struct {
  char val;
  s21_format_spec spec;
  s21_variable_type var;
  s21_format_length len;
  s21_format_flags flags;
  int width;
  int precision;
  bool has_width;
  bool has_precision;
  bool width_from_arg;
  bool precision_from_arg;
  bool valid;
} s21_specifier;

void s21_set_default_spec(s21_specifier* spec);
void s21_parse_spec(const char* format, s21_specifier* spec);
void s21_validate_spec(s21_specifier* spec);

#endif