#include "path.h"
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>

int last_index_of(char *string, char c, int start) {
  int len = strlen(string);
  if (start >= len)
    start = len - 1;
  for (int i = start; i >= 0; i--) {
    if (string[i] == c)
      return i;
  }
  return -1;
}

void path_resolve(char *base, char *child, char* dest) {
  int base_len = strlen(base);
  int child_len = strlen(child);
  strcpy(dest, base);
  if (base[base_len - 1] != '/') {
    dest[base_len++] = '/';
  }
  for (int i = 0; i <= child_len; i++) {
    dest[i + base_len] = child[i];
  }
  realpath(dest, dest);
}
