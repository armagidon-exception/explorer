#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <string.h>

typedef char *string;

int min(int a, int b) { return a < b ? a : b; }

bool strstartswith(const char *pre, const char *str) {
  size_t lenpre = strlen(pre), lenstr = strlen(str);
  return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

#endif /* end of include guard: UTILS_H */
