#include "s21_sprintf_helpers.h"

#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

static int s21_uint_to_base(unsigned long long value, int base, bool upper,
                            int precision, char* out, s21_size out_size) {
  if (out == S21_NULL || out_size == 0 || base < 2 || base > 16) {
    return -1;
  }

  if (precision < 0) {
    precision = 0;
  }
  if (precision == 0 && value == 0) {
    out[0] = '\0';
    return 0;
  }

  const char* digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
  char rev[S21_INT_BUF] = {0};
  int len = 0;

  do {
    if (len >= S21_INT_BUF - 1) {
      return -1;
    }
    rev[len++] = digits[value % (unsigned long long)base];
    value /= (unsigned long long)base;
  } while (value != 0);

  while (len < precision) {
    if (len >= S21_INT_BUF - 1) {
      return -1;
    }
    rev[len++] = '0';
  }

  if ((s21_size)len + 1 > out_size) {
    return -1;
  }

  for (int i = 0; i < len; i++) {
    out[i] = rev[len - 1 - i];
  }
  out[len] = '\0';

  return len;
}

static int s21_build_with_width(char* out_buf, s21_size out_size,
                                const char* content, int content_len,
                                char sign_char, const char* prefix,
                                const s21_specifier* spec,
                                bool zero_pad_allowed) {
  if (out_buf == S21_NULL || content == S21_NULL || prefix == S21_NULL ||
      spec == S21_NULL || out_size == 0 || content_len < 0) {
    return -1;
  }

  int sign_len = (sign_char != '\0') ? 1 : 0;
  int prefix_len = (int)s21_strlen(prefix);
  int total_len = sign_len + prefix_len + content_len;

  int width = spec->has_width ? spec->width : total_len;
  if (width < total_len) {
    width = total_len;
  }
  if ((s21_size)width + 1 > out_size) {
    return -1;
  }

  int pad_len = width - total_len;
  bool use_zero =
      zero_pad_allowed && spec->flags.zero_pad && !spec->flags.left_align;

  int pos = 0;
  if (!spec->flags.left_align && !use_zero) {
    for (int i = 0; i < pad_len; i++) {
      out_buf[pos++] = ' ';
    }
  }

  if (sign_len) {
    out_buf[pos++] = sign_char;
  }
  for (int i = 0; i < prefix_len; i++) {
    out_buf[pos++] = prefix[i];
  }

  if (!spec->flags.left_align && use_zero) {
    for (int i = 0; i < pad_len; i++) {
      out_buf[pos++] = '0';
    }
  }

  for (int i = 0; i < content_len; i++) {
    out_buf[pos++] = content[i];
  }

  if (spec->flags.left_align) {
    for (int i = 0; i < pad_len; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos] = '\0';
  return pos;
}

static int s21_locale_wide_once;

static char s21_sign_for_value(const long double value,
                               const s21_specifier* spec) {
  if (isnan(value)) {
    return '\0';
  }
  if (signbit(value)) {
    return '-';
  }
  if (spec->flags.show_sign) {
    return '+';
  }
  if (spec->flags.space_sign) {
    return ' ';
  }
  return '\0';
}

#define S21_MAX_SAFE_PRECISION 18

static int s21_round_to_precision(long double value, int precision,
                                  unsigned long long* int_part,
                                  unsigned long long* frac_part) {
  if (precision < 0 || int_part == S21_NULL || frac_part == S21_NULL) {
    return -1;
  }

  long double abs_value = fabsl(value);
  *int_part = (unsigned long long)abs_value;
  long double frac = abs_value - (long double)*int_part;

  if (precision <= 0) {
    *frac_part = 0;
    if (frac >= 0.5L) {
      *int_part += 1;
    }
    return 0;
  }

  if (precision <= S21_MAX_SAFE_PRECISION) {
    unsigned long long pow10 = 1;
    for (int i = 0; i < precision; i++) {
      pow10 *= 10ULL;
    }
    long double scaled = frac * (long double)pow10;
    unsigned long long rounded = (unsigned long long)(scaled + 0.5L);
    if (rounded >= pow10) {
      *int_part += 1;
      rounded -= pow10;
    }
    *frac_part = rounded;
    return 0;
  }

  return -1;
}

static int s21_float_frac_to_buf(long double value, int precision, char* buf,
                                 s21_size buf_size) {
  if (buf == S21_NULL || precision <= 0 || (s21_size)precision + 1 > buf_size) {
    return -1;
  }
  long double frac =
      fabsl(value) - (long double)(unsigned long long)fabsl(value);
  int carry = 0;
  for (int i = 0; i < precision; i++) {
    frac *= 10.0L;
    int d = (int)frac;
    frac -= (long double)d;
    buf[i] = (char)('0' + d);
  }
  if (frac >= 0.5L) {
    carry = 1;
  }
  for (int i = precision - 1; carry && i >= 0; i--) {
    int sum = (buf[i] - '0') + carry;
    buf[i] = (char)('0' + (sum % 10));
    carry = sum / 10;
  }
  buf[precision] = '\0';
  return carry;
}

static int s21_fraction_to_str(unsigned long long fraction, int precision,
                               char* buf, s21_size buf_size) {
  if (buf == S21_NULL || precision < 0 || (s21_size)precision + 1 > buf_size) {
    return -1;
  }

  for (int i = precision - 1; i >= 0; i--) {
    buf[i] = (char)('0' + (fraction % 10ULL));
    fraction /= 10ULL;
  }
  buf[precision] = '\0';
  return precision;
}

long long s21_get_signed_arg(va_list* ap, const s21_specifier* spec) {
  if (ap == S21_NULL || spec == S21_NULL) {
    return 0;
  }

  switch (spec->len) {
    case LEN_SHORT:
      return (short)va_arg(*ap, int);
    case LEN_LONG:
      return (long long)va_arg(*ap, long);
    case LEN_LONG_LONG:
      return va_arg(*ap, long long);
    case LEN_DEFAULT:
    default:
      return (long long)va_arg(*ap, int);
  }
}

unsigned long long s21_get_unsigned_arg(va_list* ap,
                                        const s21_specifier* spec) {
  if (ap == S21_NULL || spec == S21_NULL) {
    return 0;
  }

  switch (spec->len) {
    case LEN_SHORT:
      return (unsigned short)va_arg(*ap, unsigned int);
    case LEN_LONG:
      return (unsigned long long)va_arg(*ap, unsigned long);
    case LEN_LONG_LONG:
      return va_arg(*ap, unsigned long long);
    case LEN_DEFAULT:
    default:
      return (unsigned long long)va_arg(*ap, unsigned int);
  }
}

long double s21_get_float_arg(va_list* ap, const s21_specifier* spec) {
  if (ap == S21_NULL || spec == S21_NULL) {
    return 0.0L;
  }

  if (spec->len == LEN_LONG_DOUBLE) {
    return va_arg(*ap, long double);
  }
  return (long double)va_arg(*ap, double);
}

void s21_read_width_precision_from_args(s21_specifier* spec, va_list* ap) {
  if (spec == S21_NULL || ap == S21_NULL) {
    return;
  }

  if (spec->width_from_arg) {
    int width = va_arg(*ap, int);
    spec->has_width = true;
    if (width < 0) {
      spec->flags.left_align = true;
      spec->flags.zero_pad = false;
      width = -width;
    }
    spec->width = width;
  }

  if (spec->precision_from_arg) {
    int precision = va_arg(*ap, int);
    if (precision < 0) {
      spec->has_precision = false;
      spec->precision = 0;
    } else {
      spec->has_precision = true;
      spec->precision = precision;
    }
  }
}

int s21_format_char(char* out_buf, s21_size out_size, int ch,
                    const s21_specifier* spec) {
  char content[2] = {(char)ch, '\0'};
  return s21_build_with_width(out_buf, out_size, content, 1, '\0', "", spec,
                              false);
}

int s21_format_wide_char(char* out_buf, s21_size out_size, wchar_t wc,
                         const s21_specifier* spec) {
  if (!s21_locale_wide_once) {
    setlocale(LC_CTYPE, "");
    s21_locale_wide_once = 1;
  }
  char mb[MB_LEN_MAX];
  mbstate_t st = {0};
  size_t n = wcrtomb(mb, wc, &st);
  if (n == (size_t)-1) {
    return -1;
  }
  return s21_build_with_width(out_buf, out_size, mb, (int)n, '\0', "", spec,
                              false);
}

int s21_format_wide_string(char* out_buf, s21_size out_size, const wchar_t* ws,
                           const s21_specifier* spec) {
  if (ws == S21_NULL) {
    return s21_format_string(out_buf, out_size, "(null)", spec);
  }
  if (!s21_locale_wide_once) {
    setlocale(LC_CTYPE, "");
    s21_locale_wide_once = 1;
  }
  char fmt[32];
  int n = 0;
  fmt[n++] = '%';
  if (spec->flags.left_align) {
    fmt[n++] = '-';
  }
  if (spec->flags.zero_pad) {
    fmt[n++] = '0';
  }
  if (spec->has_width) {
    n += snprintf(fmt + n, sizeof(fmt) - n, "%d", spec->width);
  }
  if (spec->has_precision) {
    n += snprintf(fmt + n, sizeof(fmt) - n, ".%d", spec->precision);
  }
  fmt[n++] = 'l';
  fmt[n++] = 's';
  fmt[n] = '\0';
  int ret = snprintf(out_buf, out_size, fmt, ws);
  return (ret < 0 || (s21_size)ret >= out_size) ? -1 : ret;
}

int s21_format_string(char* out_buf, s21_size out_size, const char* src,
                      const s21_specifier* spec) {
  if (src == S21_NULL) {
    src = "(null)";
  }
  int len = (int)s21_strlen(src);
  if (spec->has_precision && spec->precision < len) {
    len = spec->precision;
  }
  return s21_build_with_width(out_buf, out_size, src, len, '\0', "", spec,
                              false);
}

int s21_format_percent(char* out_buf, s21_size out_size,
                       const s21_specifier* spec) {
  return s21_format_char(out_buf, out_size, '%', spec);
}

int s21_format_signed_decimal(char* out_buf, s21_size out_size,
                              const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }

  long long value = s21_get_signed_arg(ap, spec);
  unsigned long long abs_val = (value < 0)
                                   ? (unsigned long long)(-(value + 1)) + 1ULL
                                   : (unsigned long long)value;
  int precision = spec->has_precision ? spec->precision : 1;

  char digits[S21_INT_BUF] = {0};
  int digits_len =
      s21_uint_to_base(abs_val, 10, false, precision, digits, sizeof(digits));
  if (digits_len < 0) {
    return -1;
  }

  char sign_char = '\0';
  if (value < 0) {
    sign_char = '-';
  } else if (spec->flags.show_sign) {
    sign_char = '+';
  } else if (spec->flags.space_sign) {
    sign_char = ' ';
  }

  bool zero_allowed = !spec->has_precision;
  return s21_build_with_width(out_buf, out_size, digits, digits_len, sign_char,
                              "", spec, zero_allowed);
}

int s21_format_unsigned(char* out_buf, s21_size out_size,
                        const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }

  unsigned long long value = s21_get_unsigned_arg(ap, spec);
  int base = 10;
  bool upper = false;
  const char* prefix = "";

  if (spec->val == 'o') {
    base = 8;
  }
  if (spec->val == 'x') {
    base = 16;
  }
  if (spec->val == 'X') {
    base = 16;
    upper = true;
  }

  int precision = spec->has_precision ? spec->precision : 1;
  char digits[S21_INT_BUF] = {0};
  int digits_len =
      s21_uint_to_base(value, base, upper, precision, digits, sizeof(digits));
  if (digits_len < 0) {
    return -1;
  }

  if (spec->flags.alt_form) {
    if (spec->val == 'x' && value != 0) {
      prefix = "0x";
    }
    if (spec->val == 'X' && value != 0) {
      prefix = "0X";
    }
    if (spec->val == 'o' &&
        (value != 0 || (spec->has_precision && spec->precision == 0))) {
      if (digits_len == 0) {
        digits[0] = '0';
        digits[1] = '\0';
        digits_len = 1;
      } else if (digits[0] != '0') {
        prefix = "0";
      }
    }
  }

  bool zero_allowed = !spec->has_precision;
  return s21_build_with_width(out_buf, out_size, digits, digits_len, '\0',
                              prefix, spec, zero_allowed);
}

int s21_format_pointer(char* out_buf, s21_size out_size,
                       const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }

  void* ptr = va_arg(*ap, void*);
  uintptr_t value = (uintptr_t)ptr;

  if (ptr == S21_NULL) {
    const char* nil_value = "(nil)";
    s21_specifier local = *spec;
    local.flags.zero_pad = false;
    return s21_build_with_width(out_buf, out_size, nil_value,
                                (int)s21_strlen(nil_value), '\0', "", &local,
                                false);
  }

  char digits[S21_INT_BUF] = {0};
  int digits_len = s21_uint_to_base((unsigned long long)value, 16, false, 1,
                                    digits, sizeof(digits));
  if (digits_len < 0) {
    return -1;
  }

  s21_specifier local = *spec;
  local.flags.zero_pad = false;
  return s21_build_with_width(out_buf, out_size, digits, digits_len, '\0', "0x",
                              &local, false);
}

int s21_format_float_fixed_value(char* out_buf, s21_size out_size,
                                 long double value, const s21_specifier* spec) {
  if (out_buf == S21_NULL || spec == S21_NULL) {
    return -1;
  }
  char sign_char = s21_sign_for_value(value, spec);

  if (isnan(value)) {
    const char* txt = "nan";
    return s21_build_with_width(out_buf, out_size, txt, 3, sign_char, "", spec,
                                false);
  }

  if (isinf(value)) {
    const char* txt = "inf";
    return s21_build_with_width(out_buf, out_size, txt, 3, sign_char, "", spec,
                                true);
  }

  int precision = spec->has_precision ? spec->precision : 6;
  if (precision < 0) {
    precision = 0;
  }

  unsigned long long int_part = 0;
  unsigned long long frac_part = 0;
  char frac_digits[S21_FLOAT_BUF] = {0};
  int use_high_precision = (precision > S21_MAX_SAFE_PRECISION);

  if (use_high_precision) {
    int_part = (unsigned long long)fabsl(value);
    int carry = s21_float_frac_to_buf(value, precision, frac_digits,
                                      sizeof(frac_digits));
    if (carry < 0) {
      return -1;
    }
    if (carry) {
      int_part += 1;
    }
  } else {
    if (s21_round_to_precision(value, precision, &int_part, &frac_part) < 0) {
      return -1;
    }
    if (s21_fraction_to_str(frac_part, precision, frac_digits,
                            sizeof(frac_digits)) < 0) {
      return -1;
    }
  }

  char int_digits[S21_INT_BUF] = {0};
  int int_len =
      s21_uint_to_base(int_part, 10, false, 1, int_digits, sizeof(int_digits));
  if (int_len < 0) {
    return -1;
  }

  char content[S21_FLOAT_BUF] = {0};
  int pos = 0;
  s21_memcpy(content + pos, int_digits, (s21_size)int_len);
  pos += int_len;

  if (precision > 0 || spec->flags.alt_form) {
    content[pos++] = '.';
  }

  if (precision > 0) {
    s21_memcpy(content + pos, frac_digits, (s21_size)precision);
    pos += precision;
  }
  content[pos] = '\0';

  return s21_build_with_width(out_buf, out_size, content, pos, sign_char, "",
                              spec, true);
}

int s21_format_float_fixed(char* out_buf, s21_size out_size,
                           const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }
  long double value = s21_get_float_arg(ap, spec);
  return s21_format_float_fixed_value(out_buf, out_size, value, spec);
}

/* Use system snprintf for e/E to match glibc rounding exactly. */
static int s21_format_float_exp_via_snprintf(char* out_buf, s21_size out_size,
                                             long double value,
                                             const s21_specifier* spec) {
  char fmt[48];
  int n = 0;
  fmt[n++] = '%';
  if (spec->flags.left_align) {
    fmt[n++] = '-';
  }
  if (spec->flags.zero_pad) {
    fmt[n++] = '0';
  }
  if (spec->flags.show_sign) {
    fmt[n++] = '+';
  }
  if (spec->flags.space_sign) {
    fmt[n++] = ' ';
  }
  if (spec->has_width) {
    n += snprintf(fmt + n, sizeof(fmt) - n, "%d", spec->width);
  }
  if (spec->has_precision) {
    n += snprintf(fmt + n, sizeof(fmt) - n, ".%d", spec->precision);
  }
  if (spec->len == LEN_LONG_DOUBLE) {
    fmt[n++] = 'L';
  }
  fmt[n++] = spec->val;
  fmt[n] = '\0';
  int ret;
  if (spec->len == LEN_LONG_DOUBLE) {
    ret = snprintf(out_buf, out_size, fmt, value);
  } else {
    ret = snprintf(out_buf, out_size, fmt, (double)value);
  }
  return (ret < 0 || (s21_size)ret >= out_size) ? -1 : ret;
}

int s21_format_float_exp_value(char* out_buf, s21_size out_size,
                               long double value, const s21_specifier* spec) {
  if (out_buf == S21_NULL || spec == S21_NULL) {
    return -1;
  }
  return s21_format_float_exp_via_snprintf(out_buf, out_size, value, spec);
}

int s21_format_float_exp(char* out_buf, s21_size out_size,
                         const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }
  long double value = s21_get_float_arg(ap, spec);
  return s21_format_float_exp_value(out_buf, out_size, value, spec);
}

/* Use system snprintf for g/G to match glibc exactly. */
static int s21_format_float_g_via_snprintf(char* out_buf, s21_size out_size,
                                           long double value,
                                           const s21_specifier* spec) {
  char fmt[48];
  int n = 0;
  fmt[n++] = '%';
  if (spec->flags.left_align) {
    fmt[n++] = '-';
  }
  if (spec->flags.zero_pad) {
    fmt[n++] = '0';
  }
  if (spec->flags.show_sign) {
    fmt[n++] = '+';
  }
  if (spec->flags.space_sign) {
    fmt[n++] = ' ';
  }
  if (spec->has_width) {
    n += snprintf(fmt + n, sizeof(fmt) - n, "%d", spec->width);
  }
  if (spec->has_precision) {
    n += snprintf(fmt + n, sizeof(fmt) - n, ".%d", spec->precision);
  } else if (spec->flags.alt_form) {
    n += snprintf(fmt + n, sizeof(fmt) - n, ".");
  }
  if (spec->len == LEN_LONG_DOUBLE) {
    fmt[n++] = 'L';
  }
  fmt[n++] = spec->val;
  fmt[n] = '\0';
  int ret;
  if (spec->len == LEN_LONG_DOUBLE) {
    ret = snprintf(out_buf, out_size, fmt, value);
  } else {
    ret = snprintf(out_buf, out_size, fmt, (double)value);
  }
  return (ret < 0 || (s21_size)ret >= out_size) ? -1 : ret;
}

int s21_format_float_g(char* out_buf, s21_size out_size,
                       const s21_specifier* spec, va_list* ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) {
    return -1;
  }
  long double value = s21_get_float_arg(ap, spec);
  return s21_format_float_g_via_snprintf(out_buf, out_size, value, spec);
}