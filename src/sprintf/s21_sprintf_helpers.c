#include "s21_sprintf_helpers.h"

#include <limits.h>
#include <stdlib.h>

static s21_size s21_local_strlen(const char *str) {
  s21_size len = 0;
  if (str == S21_NULL) return 0;
  while (str[len] != '\0') len++;
  return len;
}

static void s21_local_memcpy(char *dst, const char *src, s21_size n) {
  for (s21_size i = 0; i < n; i++) dst[i] = src[i];
}

long long get_signed_arg(va_list *ap, s21_specifier spec) {
  long long res = 0;

  switch (spec.len) {
    case LEN_SHORT:
      res = (short)va_arg(*ap, int);
      break;
    case LEN_LONG:
      res = (long long)va_arg(*ap, long);
      break;
    case LEN_LONG_LONG:
      res = va_arg(*ap, long long);
      break;
    case LEN_DEFAULT:
    default:
      res = (long long)va_arg(*ap, int);
      break;
  }

  return res;
}

unsigned long long get_unsigned_arg(va_list *ap, s21_specifier spec) {
  unsigned long long res = 0;

  switch (spec.len) {
    case LEN_SHORT:
      res = (unsigned short)va_arg(*ap, unsigned int);
      break;
    case LEN_LONG:
      res = (unsigned long long)va_arg(*ap, unsigned long);
      break;
    case LEN_LONG_LONG:
      res = va_arg(*ap, unsigned long long);
      break;
    case LEN_DEFAULT:
    default:
      res = (unsigned long long)va_arg(*ap, unsigned int);
      break;
  }

  return res;
}

long double get_float_arg(va_list *ap, s21_specifier spec) {
  long double res = 0.0L;

  switch (spec.len) {
    case LEN_LONG_DOUBLE:
      res = va_arg(*ap, long double);
      break;
    case LEN_DEFAULT:
    default:
      res = (long double)va_arg(*ap, double);
      break;
  }

  return res;
}

void read_width_precision_from_args(s21_specifier *spec, va_list *ap) {
  if (!spec->width_from_arg && !spec->precision_from_arg) return;

  if (spec->width_from_arg) {
    int width = va_arg(*ap, int);
    if (width < 0) {
      width = -width;
      spec->flags.left_align = true;
      spec->flags.zero_pad = false;
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

int uint_to_str(unsigned long long val, int base, int precision, char *buf,
                s21_size buf_size, bool uppercase) {
  if (buf == S21_NULL || buf_size == 0 || base < 2 || base > 36) return -1;
  if (precision < 0) precision = 0;

  if (precision == 0 && val == 0) {
    buf[0] = '\0';
    return 0;
  }

  const char *digits =
      uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                : "0123456789abcdefghijklmnopqrstuvwxyz";

  char tmp[S21_INT_BUF] = {0};
  int len = 0;
  do {
    if (len >= S21_INT_BUF - 1) return -1;
    tmp[len++] = digits[val % (unsigned long long)base];
    val /= (unsigned long long)base;
  } while (val != 0);

  while (len < precision) {
    if (len >= S21_INT_BUF - 1) return -1;
    tmp[len++] = '0';
  }

  if ((s21_size)len + 1 > buf_size) return -1;

  for (int i = 0; i < len; i++) buf[i] = tmp[len - i - 1];
  buf[len] = '\0';

  return len;
}

int int_to_str_signed(long long val, int base, int precision, char *buf,
                      s21_size buf_size, bool uppercase) {
  unsigned long long mag = 0;
  if (val < 0) {
    mag = (unsigned long long)(-(val + 1)) + 1ULL;
  } else {
    mag = (unsigned long long)val;
  }

  return uint_to_str(mag, base, precision, buf, buf_size, uppercase);
}

const char *determine_int_prefix(const s21_specifier *spec,
                                 unsigned long long val) {
  const char *prefix = "";

  if (spec->flags.alt_form) {
    if (spec->val == 'x' && val != 0) prefix = "0x";
    if (spec->val == 'X' && val != 0) prefix = "0X";
    if (spec->val == 'p') prefix = "0x";
    if (spec->val == 'o' && (val != 0 || !spec->has_precision || spec->precision == 0))
      prefix = "0";
  }

  return prefix;
}

int pointer_to_hex_str(void *ptr, char *buf, s21_size buf_size) {
  if (buf == S21_NULL) return -1;
  return uint_to_str((unsigned long long)(unsigned long)ptr, 16, 1, buf,
                     buf_size, false);
}

int format_char(char *out_buf, s21_size out_size, int ch,
                const s21_specifier *spec) {
  if (out_buf == S21_NULL || out_size == 0 || spec == S21_NULL) return -1;

  int width = spec->has_width ? spec->width : 1;
  if (width < 1) width = 1;

  if ((s21_size)width + 1 > out_size) return -1;

  int pad = width - 1;
  int pos = 0;

  if (!spec->flags.left_align) {
    for (int i = 0; i < pad; i++) out_buf[pos++] = ' ';
  }

  out_buf[pos++] = (char)ch;

  if (spec->flags.left_align) {
    for (int i = 0; i < pad; i++) out_buf[pos++] = ' ';
  }

  out_buf[pos] = '\0';
  return pos;
}

int format_string(char *out_buf, s21_size out_size, const char *src,
                  const s21_specifier *spec) {
  if (out_buf == S21_NULL || out_size == 0 || spec == S21_NULL) return -1;

  if (src == S21_NULL) src = "(null)";

  s21_size content_len = s21_local_strlen(src);
  if (spec->has_precision && spec->precision >= 0 &&
      (s21_size)spec->precision < content_len) {
    content_len = (s21_size)spec->precision;
  }

  int width = spec->has_width ? spec->width : (int)content_len;
  if (width < (int)content_len) width = (int)content_len;

  if ((s21_size)width + 1 > out_size) return -1;

  int pad = width - (int)content_len;
  int pos = 0;

  if (!spec->flags.left_align) {
    for (int i = 0; i < pad; i++) out_buf[pos++] = ' ';
  }

  s21_local_memcpy(out_buf + pos, src, content_len);
  pos += (int)content_len;

  if (spec->flags.left_align) {
    for (int i = 0; i < pad; i++) out_buf[pos++] = ' ';
  }

  out_buf[pos] = '\0';
  return pos;
}

int format_percent(char *out_buf, s21_size out_size, const s21_specifier *spec) {
  return format_char(out_buf, out_size, '%', spec);
}

int apply_integer_format(char *out_buf, s21_size out_size, const char *digits,
                         int digits_len, char sign_char, const char *prefix,
                         const s21_specifier *spec) {
  if (out_buf == S21_NULL || digits == S21_NULL || prefix == S21_NULL ||
      spec == S21_NULL || out_size == 0 || digits_len < 0) {
    return -1;
  }

  int prefix_len = (int)s21_local_strlen(prefix);
  int sign_len = sign_char != '\0' ? 1 : 0;
  int raw_len = sign_len + prefix_len + digits_len;

  int width = spec->has_width ? spec->width : raw_len;
  if (width < raw_len) width = raw_len;

  if ((s21_size)width + 1 > out_size) return -1;

  int pad_len = width - raw_len;
  bool use_zero_pad = spec->flags.zero_pad && !spec->flags.left_align && !spec->has_precision;
  char pad_char = use_zero_pad ? '0' : ' ';

  int pos = 0;

  if (!spec->flags.left_align && pad_char == ' ') {
    for (int i = 0; i < pad_len; i++) out_buf[pos++] = ' ';
  }

  if (sign_len) out_buf[pos++] = sign_char;
  for (int i = 0; i < prefix_len; i++) out_buf[pos++] = prefix[i];

  if (!spec->flags.left_align && pad_char == '0') {
    for (int i = 0; i < pad_len; i++) out_buf[pos++] = '0';
  }

  for (int i = 0; i < digits_len; i++) out_buf[pos++] = digits[i];

  if (spec->flags.left_align) {
    for (int i = 0; i < pad_len; i++) out_buf[pos++] = ' ';
  }

  out_buf[pos] = '\0';
  return pos;
}

int handle_decimal_spec(char *out_buf, s21_size out_buf_size,
                        const s21_specifier *spec, va_list *ap) {
  if (out_buf == S21_NULL || spec == S21_NULL || ap == S21_NULL) return -1;

  long long value = get_signed_arg(ap, *spec);

  int precision = spec->has_precision ? spec->precision : 1;

  char digits[S21_INT_BUF] = {0};
  int digits_len = int_to_str_signed(value, 10, precision, digits, sizeof(digits), false);
  if (digits_len < 0) return -1;

  char sign_char = '\0';
  if (value < 0) {
    sign_char = '-';
  } else if (spec->flags.show_sign) {
    sign_char = '+';
  } else if (spec->flags.space_sign) {
    sign_char = ' ';
  }

  return apply_integer_format(out_buf, out_buf_size, digits, digits_len,
                              sign_char, "", spec);
}
