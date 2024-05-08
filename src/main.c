#include "fs.h"
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum { NONE, LIST, DELETE, CREATE_FILE, MAKE_DIR, OPEN } modes;

typedef struct {
  modes mode;
  char *file;
} s_options;

void print_help() {
  printf("Help:\n");
  printf("-l  list files\n");
  printf("-d  delete file\n");
  printf("-c  create file\n");
  printf("-o  open file\n");
  printf("-m  make directory\n");
  printf("-h  print this help\n");
}

void handle_list_mode(s_options *options) {
  char *path = options->file;
  DIR *dir = opendir(path);
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    fprintf(stdout, "%s\n", entry->d_name);
  }
  closedir(dir);
}

int parse_options(s_options *options, char **argv, int argc) {
  int optc;
  while ((optc = getopt(argc, argv, "ldcohm")) != -1) {
    switch (optc) {
    case 'l': {
      if (options->mode != NONE) {
        fprintf(stderr, "Mode is already set.");
        return 1;
      }
      options->mode = LIST;
    } break;
    case 'd': {
      if (options->mode != NONE) {
        fprintf(stderr, "Mode is already set.");
        return 1;
      }
      options->mode = DELETE;
    } break;
    case 'c': {
      if (options->mode != NONE) {
        fprintf(stderr, "Mode is already set.");
        return 1;
      }
      options->mode = CREATE_FILE;
    } break;
    case 'o': {
      if (options->mode != NONE) {
        fprintf(stderr, "Mode is already set.");
        return 1;
      }
      options->mode = OPEN;
    } break;
    case 'm': {
      if (options->mode != NONE) {
        fprintf(stderr, "Mode is already set.");
        return 1;
      }
      options->mode = MAKE_DIR;
    } break;
    case 'h': {
      print_help();
      return 1;
    }
    default: {
      fprintf(stderr, "Wrong command syntax. Use %s --help to get help.\n",
              argv[0]);
      return 1;
    }
    }
  }
  if (optind >= argc) {
    fprintf(stderr, "Wrong command syntax. Use %s --help to get help.\n",
            argv[0]);
    print_help();
    return 1;
  }

  options->file = argv[optind];
  return 0;
}

int main(int argc, char **argv) {
  s_options options = {.mode = NONE};
  if (parse_options(&options, argv, argc)) {
    exit(EXIT_FAILURE);
  }

  switch (options.mode) {
  case LIST: {
    handle_list_mode(&options);
  } break;
  case CREATE_FILE: {
    if (access(options.file, F_OK) == 0) {
      fprintf(stderr, "File already exists\n");
      exit(EXIT_FAILURE);
    }
    FILE *f = fopen(options.file, "w");
    if (f == NULL) {
      fprintf(stderr, "Could not create a file\n");
      exit(EXIT_FAILURE);
    }
    fclose(f);
    printf("File successfully created");
  } break;
  case MAKE_DIR: {
    if (!make_dirs(options.file, true)) {
      printf("File successfully created");
    } else if (errno == EMKDIR_EXISTS) {
      fprintf(stderr, "File already exists\n");
      exit(EXIT_FAILURE);
    } else if (errno == EMKDIR_IO) {
      fprintf(stderr, "IO error\n");
      exit(EXIT_FAILURE);
    } else {
      fprintf(stderr, "IO error\n");
      exit(EXIT_FAILURE);
    }
  } break;
  case DELETE: {
    if (!remove(options.file)) {
      printf("File deleted successfully");
    } else {
      printf("Error: unable to delete the file");
    }
  } break;
  case OPEN: {
    int pid = fork();
    if (pid == 0) {
      execl("/usr/bin/xdg-open", "xdg-open", options.file, (char *)0);
    }
  } break;
  case NONE: {
    fprintf(stderr, "Wrong command syntax. Use %s --help to get help.\n",
            argv[0]);
    print_help();
    exit(EXIT_FAILURE);
  } break;
  default: {
    exit(EXIT_FAILURE);
  }
  }

  exit(EXIT_SUCCESS);
}
