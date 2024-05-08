#include "fs.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int make_dirs(char *filename, bool make_parents) {
  int len = strlen(filename);
  char buf[BUFSIZ];
  if (sizeof(buf) < len) {
    errno = EMKDIR_IO;
    return -1;
  }
  if (!make_parents) {
    if (!mkdir(filename, 0700)) {
      printf("Created new directory %s\n", buf);
    } else if (errno == EEXIST) {
      errno = EMKDIR_EXISTS;
      return -1;
    } else {
      errno = EMKDIR_IO;
      return -1;
    }
  } else {
    int r;
    for (r = 0; r < len; r++) {
      buf[r] = filename[r];
      if (filename[r] == '/') {
        if (!mkdir(buf, 0700)) {
          printf("Created new directory %s\n", buf);
        } else if (errno == EEXIST) {
          printf("Dir exists %s\n", buf);
          continue;
        } else {
          errno = EMKDIR_IO;
          return -1;
        }
      }
    }

    if (!mkdir(buf, 0700)) {
      printf("Created new directory %s\n", buf);
    } else if (errno == EEXIST) {
      printf("Dir exists %s\n", buf);
      return 0;
    } else {
      errno = EMKDIR_IO;
      return -1;
    }
  }
  return 0;
}
