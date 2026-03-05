#include "../s21_string.h"

#define MAX_KNOWN_ERROR 41
#define UNKNOWN_BUFFER_SIZE 50

static const char* error_descriptions[] = {"Success",
                                           "Operation not permitted",
                                           "No such file or directory",
                                           "No such process",
                                           "Interrupted system call",
                                           "Input/output error",
                                           "No such device or address",
                                           "Argument list too long",
                                           "Exec format error",
                                           "Bad file descriptor",
                                           "No child processes",
                                           "Resource temporarily unavailable",
                                           "Cannot allocate memory",
                                           "Permission denied",
                                           "Bad address",
                                           "Block device required",
                                           "Device or resource busy",
                                           "File exists",
                                           "Invalid cross-device link",
                                           "No such device",
                                           "Not a directory",
                                           "Is a directory",
                                           "Invalid argument",
                                           "Too many open files in system",
                                           "Too many open files",
                                           "Inappropriate ioctl for device",
                                           "Text file busy",
                                           "File too large",
                                           "No space left on device",
                                           "Illegal seek",
                                           "Read-only file system",
                                           "Too many links",
                                           "Broken pipe",
                                           "Numerical argument out of domain",
                                           "Numerical result out of range",
                                           "Resource deadlock avoided",
                                           "File name too long",
                                           "No locks available",
                                           "Function not implemented",
                                           "Directory not empty",
                                           "Too many levels of symbolic links"};

static char unknown_error_message[UNKNOWN_BUFFER_SIZE];

char* s21_strerror(int errnum) {
  char* result;

  if (errnum >= 0 && errnum < MAX_KNOWN_ERROR) {
    result = (char*)error_descriptions[errnum];
  } else {
    s21_sprintf(unknown_error_message, "Unknown error %d", errnum);
    result = unknown_error_message;
  }

  return result;
}
