#include "fs.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int make_dir(char *filename, bool make_parents) {
  int len = strlen(filename);
  char buf[BUFSIZ];
  if (sizeof(buf) < len) {
    return -1;
  }
  if (!make_parents) {
    if (!mkdir(filename, 0700)) {
      printf("Created new directory %s\n", buf);
    } else if (errno == EEXIST) {
      return -1;
    } else {
      return -1;
    }
  } else {
    int r;
    for (r = 0; r < len; r++) {
      buf[r] = filename[r];
      if (filename[r] == '/') {
        if (!mkdir(buf, 0700)) {
        } else if (errno == EEXIST) {
          continue;
        } else {
          return -1;
        }
      }
    }

    if (!mkdir(buf, 0700)) {
    } else if (errno == EEXIST) {
      return 0;
    } else {
      return -1;
    }
  }
  return 0;
}

int create_file(char *filename) {
  if (access(filename, F_OK) == 0) {
    fprintf(stderr, "File already exists\n");
    exit(EXIT_FAILURE);
  }
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    fprintf(stderr, "Could not create a file\n");
    return -1;
  }
  fclose(f);
  return 0;
}

int delete_file(char *filename) { return remove(filename); }

int open_file(char *filename) {
  int pid = fork();
  if (pid == 0) {
    execl("/usr/bin/xdg-open", "xdg-open", filename, (char *)0);
    return 0;
  }
  return -1;
}

dir_list *read_dir(char *directory) {
  DIR *dir = opendir(directory);
  if (dir == NULL) {
    return NULL;
  }
  dir_list *dl = malloc(sizeof(dir_list));
  memset(dl, 0, sizeof(dir_list));
  dl->dir_path = directory;
  struct dirent *entry;
  int c = 0;
  while ((entry = readdir(dir)) != NULL)
    c++;
  rewinddir(dir);
  if (c > 0) {
    int index = 0;
    dl->dir_names = calloc(c, sizeof(char *));
    while ((entry = readdir(dir)) != NULL) {
      int len = strlen(entry->d_name);
      char *dir_name = calloc(len + 1, sizeof(char));
      strcpy(dir_name, entry->d_name);
      dl->dir_names[index++] = dir_name;
    }
    dl->dir_count = c;
  }
  closedir(dir);
  return dl;
}

void close_dir(dir_list *dir_list) {
  if (dir_list == NULL)
    return;
  for (int i = 0; i < dir_list->dir_count; i++) {
    free(dir_list->dir_names[i]);
  }
  free(dir_list->dir_names);
  free(dir_list->dir_path);
  free(dir_list);
}

bool is_dir(char *filename) {
  struct stat path_stat;
  stat(filename, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

bool is_file(char *filename) {
  struct stat path_stat;
  stat(filename, &path_stat);
  return S_ISREG(path_stat.st_mode);
}
