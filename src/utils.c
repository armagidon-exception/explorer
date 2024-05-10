#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *strtoheap(char *str) {
  char *buf = calloc(strlen(str) + 1, sizeof(char));
  strcpy(buf, str);
  return buf;
}


int min(int a, int b) { return a < b ? a : b; }
