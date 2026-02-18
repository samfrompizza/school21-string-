#ifndef S21_FORMAT_HELPERS_H
#define S21_FORMAT_HELPERS_H

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../common/s21_format.h"
#include "../string/s21_string.h"

#define S21_INT_BUF     64
#define S21_FLOAT_BUF   1024

long long get_signed_arg(va_list *ap, s21_specifier spec);
unsigned long long get_unsigned_arg(va_list *ap, s21_specifier spec);
long double get_float_arg(va_list *ap, s21_specifier spec);

void read_width_precision_from_args(s21_specifier *spec, va_list *ap);

int uint_to_str(unsigned long long val, int base, int precision,
                char *buf, s21_size buf_size, bool uppercase);
int int_to_str_signed(long long val, int base, int precision,
                      char *buf, s21_size buf_size, bool uppercase);

const char *determine_int_prefix(const s21_specifier *spec, unsigned long long val);
int pointer_to_hex_str(void *ptr, char *buf, s21_size buf_size);

/* ---------------------------
 * 5) Применение форматирования к целым (padding, signs)
 * ---------------------------
 */

/**
 * Форматирует целое число, когда уже есть:
 * - digits: строка цифр (без знака), длина digits_len
 * - sign_char: '\0' (нет знака) или '-', '+' или ' ' (space)
 * - prefix: строка префикса, может быть ""
 *
 * Учитывает spec->width, spec->flags.left_align, spec->flags.zero_pad и т.п.
 * Пишет результат в out_buf (не более out_size). Возвращает длину записанного текста (без '\0') или -1.
 *
 * Примечание: для целых zero_pad считается недействительным, если spec->has_precision == true.
 */
int apply_integer_format(char *out_buf, s21_size out_size,
                         const char *digits, int digits_len,
                         char sign_char, const char *prefix,
                         const s21_specifier *spec);

/*
 6) Форматирование char/string/percent
 */

int format_char(char *out_buf, s21_size out_size, int ch, const s21_specifier *spec);
int format_string(char *out_buf, s21_size out_size, const char *src, const s21_specifier *spec);
int format_percent(char *out_buf, s21_size out_size, const s21_specifier *spec);

/* ---------------------------
 * 7) Форматирование float — конвертеры
 * ---------------------------
 */

/**
 * Форматирует double/long double в fixed-point представление (эквивалент %f).
 * - precision: количество цифр после точки (по умолчанию 6 если has_precision == false — это логика вызывающего).
 * - buf — куда писать, buf_size — объём.
 * - is_nan, is_inf — флаги, устанавливаются при соответствующих значениях.
 *
 * Возвращает длину строки (без '\0') или -1 при ошибке.
 *
 * Формат результата: "[-]integer.fraction" (точка присутствует если precision > 0 или alt_form).
 * Внутри реализовать корректное округление дробной части.
 */
int float_to_fixed(long double val, int precision,
                   char *buf, s21_size buf_size, bool *is_nan, bool *is_inf);

/**
 * Форматирует число в научном представлении (эквивалент %e/%E).
 * spec_letter — 'e' или 'E' (для регистра).
 * Возвращает длину строки или -1.
 * Формат: "[-]d.ddd e±NN" (где экспонента минимум 2 цифры, знак экспоненты обязателен).
 */
int float_to_sci(long double val, int precision, char spec_letter,
                 char *buf, s21_size buf_size, bool *is_nan, bool *is_inf);

/**
 * Форматирует число в общем формате (эквивалент %g/%G).
 * - precision: число значащих цифр (по умолчанию 6).
 * - spec_letter: 'g' или 'G'
 * - alt_form: если true — вести себя как флаг '#'
 * Возвращает длину строки или -1.
 */
int float_to_g(long double val, int precision, char spec_letter, bool alt_form,
               char *buf, s21_size buf_size, bool *is_nan, bool *is_inf);

/* ---------------------------
 * 8) Вспомогательные функции для float (округление/экспонента)
 * ---------------------------
 */

/**
 * Округляет дробную часть frac_digits (строка цифр длины frac_len) до precision цифр.
 * Если при округлении происходит перенос в целую часть, int_part корректируется.
 * int_part_size — размер буфера int_part.
 * Возвращает 0 при успешном округлении, -1 при ошибке (например, переполнение буфера).
 */
int round_fractional_str(char *int_part, s21_size int_part_size,
                         char *frac_digits, int frac_len, int precision);

/**
 * Форматирует экспоненту (целое число exponent) в виде "e+NN" или "E-NN".
 * spec_letter используется для выбора регистра 'e' или 'E'.
 * Возвращает длину записанного фрагмента (без '\0') или -1.
 */
int format_exponent(int exponent, char spec_letter, char *buf, s21_size buf_size);

/* ---------------------------
 * 9) Общая функция применения width/flags
 * ---------------------------
 */

/**
 * Универсальная функция: принимает уже готовый контент content (content_len),
 * optional sign_char ('\0' или '-', '+' или ' '), и prefix (может быть "").
 * Применяет spec->width, spec->flags.left_align, spec->flags.zero_pad и т.п.,
 * и пишет итог в out_buf.
 *
 * zero_pad_allowed — указывает, разрешено ли применять zero_pad (например, для целых
 * zero_pad игнорируется, если spec->has_precision == true).
 *
 * Возвращает длину записанного результата или -1 на ошибку.
 */
int apply_width_and_flags(char *out_buf, s21_size out_size,
                          const char *content, int content_len,
                          char sign_char, const char *prefix,
                          const s21_specifier *spec, bool zero_pad_allowed);

/* ---------------------------
 * 10) Обработчики спецификаторов — фасады (интерфейс для s21_sprintf)
 * ---------------------------
 *
 * Каждая функция читает необходимые аргументы из va_list (через get_*_arg),
 * формирует фрагмент в локальный буфер (out_buf) и возвращает длину.
 * out_buf_size должен быть достаточным (например S21_FLOAT_BUF).
 */

/**
 * Форматирует спецификатор 'c'.
 */
int handle_char_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификатор 's'.
 */
int handle_string_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификатор 'd' (signed decimal).
 */
int handle_decimal_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификаторы 'u', 'o', 'x', 'X' (unsigned).
 * spec->val указывает конкретный вид ('u','o','x','X').
 */
int handle_unsigned_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификатор 'p' (pointer).
 */
int handle_pointer_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификаторы с плавающей точкой ('f','e','E','g','G').
 */
int handle_float_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec, va_list *ap);

/**
 * Форматирует спецификатор '%%'.
 */
int handle_percent_spec(char *out_buf, s21_size out_buf_size, const s21_specifier *spec);

/* ---------------------------
 * 11) Безопасная копирующая функция
 * ---------------------------
 */

/**
 * Безопасно конкатенирует src_len байт из src в dst.
 * - dst_ptr указывает на текущую позицию в буфере (меняется)
 * - rem_ptr содержит оставшийся размер буфера (меняется)
 * Возвращает 0 при успехе, -1 при переполнении.
 *
 * Пример использования:
 *   if (safe_memcat(&dst, &rem, out_buf, out_len) < 0) return -1;
 */
int safe_memcat(char **dst_ptr, s21_size *rem_ptr, const char *src, s21_size src_len);

#endif /* S21_FORMAT_HELPERS_H */
