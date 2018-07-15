#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum { SHORT_SHORT, SHORT, DEFAULT, LONG, LONG_LONG } length_specifier;

// TODO(psamora) These don't have to be this bad, refactor
#define print_num(__base, __casted_num)                         \
  {                                                             \
    if (__casted_num == 0) {                                    \
      putchar('0');                                             \
    }                                                           \
    char buffer[20];                                            \
    int count = 0;                                              \
    while (__casted_num != 0) {                                 \
      buffer[count++] = digits[__casted_num % __base];          \
      __casted_num /= __base;                                   \
    }                                                           \
    for (size_t i = count; i != 0; i--) putchar(buffer[i - 1]); \
  }

#define print_short(__type, __base, __parameters) \
  {                                               \
    __type num = va_arg(__parameters, int);       \
    if (num < 0) {                                \
      putchar('-');                               \
      num *= -1;                                  \
    }                                             \
    print_num(__base, num);                       \
  };

#define print_ushort(__type, __base, __parameters) \
  {                                                \
    __type num = va_arg(__parameters, int);        \
    print_num(__base, num);                        \
  };

#define print_int(__type, __base, __parameters) \
  {                                             \
    __type num = va_arg(__parameters, __type);  \
    if (num < 0) {                              \
      putchar('-');                             \
      num *= -1;                                \
    }                                           \
    print_num(__base, num);                     \
  };

#define print_uint(__type, __base, __parameters) \
  {                                              \
    __type num = va_arg(__parameters, __type);   \
    print_num(__base, num);                      \
  };

const char digits[] = "0123456789abcdef";

static void print(const char* data, size_t data_length) {
  for (size_t i = 0; i < data_length; i++)
    putchar((int)((const unsigned char*)data)[i]);
}

int printf(const char* restrict format, ...) {
  char cur_specifier;
  length_specifier length;
  bool is_signed;
  bool is_number;
  int base;

  va_list parameters;
  va_start(parameters, format);

  int written = 0;
  size_t amount = 0;

  while (*format != '\0') {
    if (*format != '%') {
      amount += 1;
      written += 1;
      format++;
      continue;
    }

    if (amount > 0) {
      print(format - amount, amount);
      amount = 0;
    }

    cur_specifier = *++format;
    length = DEFAULT;
    is_number = true;
    is_signed = true;
    base = 10;

    // Deals with the length specifiers
    switch (cur_specifier) {
      case 'l':
        ++format;
        if (*format == 'l') {
          length = LONG_LONG;
          ++format;
        } else
          length = LONG;
        break;
      case 'h':
        if (*(format + 1) == 'l') {
          length = SHORT_SHORT;
          ++format;
        } else
          length = SHORT;
        break;
    }

    cur_specifier = *format;

    // Deals with the general specifiers
    switch (cur_specifier) {
      case 'c':;
        const char c = (char)va_arg(parameters, int /* char promotes to int */);
        print(&c, 1);
        is_number = false;
        break;
      case 's':;
        const char* s = va_arg(parameters, const char*);
        print(s, strlen(s));
        is_number = false;
        break;
      case 'd':
      case 'i':;
        // default values is_signed and base
        break;
      case 'f':
      case 'F':
        break;
      case 'u':
        is_signed = false;
        break;
      case 'o':
        is_signed = false;
        base = 8;
        break;
      case 'x':;
        is_signed = false;
        base = 16;
        print("0x", 2);
        break;
      case 'X':
        break;
      case 'a':
        break;
      case 'A':
        break;
      case 'p':
        break;
      case 'n':
        break;
      case '%':
        break;
      default:
        break;
    }

    if (is_number) {
      switch (length) {
        case SHORT_SHORT:
          if (is_signed) {
            print_short(signed char, base, parameters);
          } else {
            print_ushort(unsigned char, base, parameters);
          }
          break;
        case SHORT:
          if (is_signed) {
            print_short(short int, base, parameters);
          } else {
            print_ushort(unsigned short int, base, parameters);
          }
          break;
        case DEFAULT:
          if (is_signed) {
            print_int(int, base, parameters);
          } else {
            print_uint(unsigned int, base, parameters);
          }
          break;
        case LONG:
          if (is_signed) {
            print_int(long int, base, parameters);
          } else {
            print_uint(unsigned long int, base, parameters);
          }
          break;
        case LONG_LONG:
          if (is_signed) {
            print_int(long long int, base, parameters);
          } else {
            print_uint(unsigned long long int, base, parameters);
          }
          break;
      }
    }

    format++;
  }

  if (amount > 0) {
    print(format - amount, amount);
  }

  va_end(parameters);

  return written;
}
