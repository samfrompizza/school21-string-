#include "format.h"

#include "../string/s21_string.h"

void s21_set_default_spec(s21_specifier* spec) {
  spec->val = 0;
  spec->spec = 0;
  spec->var = 0;
  spec->len = 0;

  spec->flags.left_align = false;
  spec->flags.show_sign = false;
  spec->flags.space_sign = false;
  spec->flags.alt_form = false;
  spec->flags.zero_pad = false;

  spec->width = 0;
  spec->precision = 0;

  spec->has_width = false;
  spec->has_precision = false;
  spec->width_from_arg = false;
  spec->precision_from_arg = false;
  spec->valid = false;
}

void s21_parse_spec(const char* format, s21_specifier* spec) {
  if (!format) return;
}

void s21_validate_spec(s21_specifier* spec);
