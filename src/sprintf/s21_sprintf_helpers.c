#include "s21_sprintf_helpers.h"
#include "../common/s21_format.h"
#include "../string/s21_string.h"

#include <stdlib.h>

long long get_signed_arg(va_list *ap, s21_specifier spec) {
  long long res = 0;

  switch (spec.len)
  {
  case LEN_SHORT: {
    int tmp = va_arg(*ap, int);
    // In fact here: (long long)(short)tmp
    res = (short)tmp;
    break;
    }
  case LEN_LONG: {
    long tmp = va_arg(*ap, long);
    res = (long long)tmp;
    break;
    }
  case LEN_LONG_LONG: {
    res = va_arg(*ap, long long);
    break;
    }
  case LEN_DEFAULT:
  default: {
    int tmp = va_arg(*ap, int);
    res = (long long)tmp;
    break;
    }
  }

  return res;
}

unsigned long long get_unsigned_arg(va_list *ap, s21_specifier spec) {
  unsigned long long res = 0;

  switch (spec.len)
  {
  case LEN_SHORT: {
    unsigned int tmp = va_arg(*ap, unsigned int);
    res = (unsigned short)tmp;
    break;
    }
  case LEN_LONG: {
    unsigned long tmp = va_arg(*ap, unsigned long);
    res = (unsigned long long)tmp;
    break;
    }
  case LEN_LONG_LONG: {
    res = va_arg(*ap, unsigned long long);
    break;
    }
  case LEN_DEFAULT:
  default: {
    unsigned int tmp = va_arg(*ap, unsigned int);
    res = (unsigned long long)tmp;
    break;
    }
  }

  return res;
}

long double get_float_arg(va_list *ap, s21_specifier spec) {
  long double res = 0.0L;

  switch (spec.len)
  {
  case LEN_LONG_DOUBLE: {
    res = va_arg(*ap, long double);
    break;
    }
  case LEN_DEFAULT:
  default: {
    double tmp = va_arg(*ap, double);
    res = (long double)tmp;
    break;
    }
  }

  return res;
}

void read_width_precision_from_args(s21_specifier *spec, va_list *ap) {
  if (!spec->precision_from_arg && !spec->precision_from_arg) return;

  if (spec->width_from_arg) {
    int width = va_arg(*ap, int);
    if (width < 0) {
      width = -width;
      spec->flags.left_align = true;
    }
    spec->width = width;
  }

  if (spec->precision_from_arg) {
    int prec = va_arg(*ap, int);
    if (prec < 0) {
      spec->has_precision = false;
    } else {
      spec->precision = prec;
    }
  }
}

int uint_to_str(unsigned long long val, int base, int precision, char *buf, s21_size buf_size, bool uppercase) {
  if (buf == S21_NULL || buf_size == 0 || (base < 2 || base > 36) || precision > S21_INT_BUF - 1) return -1;
  if (precision < 0) precision = 0;
  if (precision == 0 && val == 0) {
    *buf = '\0';
    return 0;
  }

  const char *digits;
  if (uppercase) digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  else digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  char tmp[S21_INT_BUF] = "";
  unsigned long long num_digits = 0;

  do {
    tmp[num_digits++] = digits[val % base];
    val /= base;
  } while (val);

  while (num_digits < precision)
  {
    tmp[num_digits++] = '0';
  }
  if (num_digits > buf_size - 1) return -1;

  for (s21_size i = 0; i < num_digits; i++) {
    buf[i] = tmp[num_digits - i - 1];
  }
  buf[num_digits] = '\0';

  return num_digits;
}

int int_to_str_signed(long long val, int base, int precision, char *buf, s21_size buf_size, bool uppercase) {
  if (val < 0) val = -val;
  return uint_to_str((unsigned long long)val, base, precision, buf, buf_size, uppercase);
}

const char *determine_int_prefix(const s21_specifier *spec, unsigned long long val) {
  const char *prefix;
  if (!spec->flags.alt_form) {
    prefix = "";
  } else if (spec->val == 'x' && val != 0) {
    prefix = "0x";
  } else if (spec->val == 'X' && val != 0) {
    prefix = "0X";
  } else if (spec->val == 'p') {
    prefix = "0x";
  } else if (spec->val == 'o') {
    prefix = "0";
  } else {
    prefix = "";
  }

  return prefix;
}

int pointer_to_hex_str(void *ptr, char *buf, s21_size buf_size) {
  if (buf == S21_NULL) return -1;
  int base = 16;
  int precision = 0;
  bool uppercase = false;
  return uint_to_str((unsigned long long)ptr, base, precision, buf, buf_size, uppercase);
}

int format_char(char *out_buf, s21_size out_size, int ch, const s21_specifier *spec) {
  if (out_buf == S21_NULL || out_size == 0 || spec == S21_NULL) return -1;

  int width = spec->has_width ? spec->width : 0;
  if (width < 1) width = 1;

  if ((s21_size)width + 1 > out_size) return -1;

  s21_size pos = 0;
  s21_size pad = (s21_size)(width - 1);

  if (!spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos++] = (char)ch;

  if (spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos] = '\0';
  return (int)pos;
}

int format_string(char *out_buf, s21_size out_size, const char *src, const s21_specifier *spec) {
  if (out_buf == S21_NULL || out_size == 0 || spec == S21_NULL) return -1;

  if (src == S21_NULL) src = "(null)";

  s21_size src_len = s21_strlen(src);
  s21_size content_len = src_len;

  if (spec->has_precision && spec->precision >= 0 && (s21_size)spec->precision < content_len) {
    content_len = (s21_size)spec->precision;
  }

  int width = spec->has_width ? spec->width : 0;
  if (width < (int)content_len) width = (int)content_len;

  if ((s21_size)width + 1 > out_size) return -1;

  s21_size pad = (s21_size)width - content_len;
  s21_size pos = 0;

  if (!spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  s21_memcpy(out_buf + pos, src, content_len);
  pos += content_len;

  if (spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos] = '\0';
  return (int)pos;
}

int format_percent(char *out_buf, s21_size out_size, const s21_specifier *spec) {
  if (out_buf == S21_NULL || out_size == 0 || spec == S21_NULL) return -1;

  int width = spec->has_width ? spec->width : 0;
  if (width < 1) width = 1;

  if ((s21_size)width + 1 > out_size) return -1;

  s21_size pos = 0;
  s21_size pad = (s21_size)(width - 1);

  if (!spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos++] = '%';

  if (spec->flags.left_align) {
    for (s21_size i = 0; i < pad; i++) {
      out_buf[pos++] = ' ';
    }
  }

  out_buf[pos] = '\0';
  return (int)pos;
}


int apply_integer_format(char *out_buf, s21_size out_size, const char *digits, int digits_len, char sign_char, const char *prefix, const s21_specifier *spec) {
  
}