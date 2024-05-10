#include "fs.h"
#include "utils.h"
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

static int get_dir_length(DIR *dir) {
  struct dirent *entry;
  int c = 0;
  while ((entry = readdir(dir)) != NULL)
    c++;
  return c;
}

static dir_list *__make_dir_list(DIR *dir, char *path) {
  dir_list *dl = malloc(sizeof(dir_list));
  memset(dl, 0, sizeof(dir_list));
  dl->dir_path = strtoheap(path);
  dl->dir_length = get_dir_length(dir);
  return dl;
}

dir_list *read_dir(char *directory) {
  DIR *dir = opendir(directory);
  if (dir == NULL)
    return NULL;

  dir_list *dl = __make_dir_list(dir, directory);
  rewinddir(dir);

  if (dl->dir_length) {
    dl->dir_names = calloc(dl->dir_length, sizeof(char *));

    struct dirent *entry;
    for (int index = 0; (entry = readdir(dir)) != NULL; index++)
      dl->dir_names[index] = strtoheap(entry->d_name);

  }
  closedir(dir);
  return dl;
}

void close_dir(dir_list *dir_list) {
  if (dir_list == NULL)
    return;

  for (int i = 0; i < dir_list->dir_length; i++)
    free(dir_list->dir_names[i]);

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
