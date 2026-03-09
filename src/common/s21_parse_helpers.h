#ifndef S21_PARSE_HELPERS_H_
#define S21_PARSE_HELPERS_H_

int s21_isdigit(int ch);
int s21_isspace(int ch);

int s21_isoctal(int ch);
int s21_isxdigit_parse(int ch);
int s21_xdigit_value(int ch);

void s21_skip_whitespace(const char** p);

#endif
