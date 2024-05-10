#include "fs.h"
#include "path.h"
#include "ui.h"
#include <getopt.h>
#include <linux/limits.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *file;
} s_options;

int parse_options(s_options *options, char **argv, int argc) {
  int optc;
  while ((optc = getopt(argc, argv, "h")) != -1)
    ;
  if (optind >= argc) {
    fprintf(stderr, "Wrong command syntax. Use %s -h to get help.\n", argv[0]);
    return 1;
  }

  options->file = argv[optind];
  return 0;
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  s_options options = {0};
  if (parse_options(&options, argv, argc)) {
    exit(EXIT_FAILURE);
  }

  char *rp = calloc(PATH_MAX + 1, sizeof(char));
  realpath(argv[optind], rp);

  ui_state *state = create_ui();
  set_cwd(state, rp);
  render_ui(state);
  while (1) {
    switch (getch()) {
    case 'k': {
      state->selected_item--;
      render_ui(state);
    } break;
    case 'j': {
      state->selected_item++;
      render_ui(state);
    } break;
    case '\n': {
      char *selected_item = get_selected_item(state);
      if (is_dir(selected_item)) {
        char *new_path = path_resolve(state->dir_list->dir_path, selected_item);
        if (set_cwd(state, new_path)) {
          free(new_path);
          break;
        }
        render_ui(state);
      } else if (is_file(selected_item)) {
        char *filepath = path_resolve(state->dir_list->dir_path, selected_item);
        open_file(filepath);
        free(filepath);
      }
    } break;
    case 'q': {
      close_ui(state);
      exit(EXIT_SUCCESS);
    } break;
    }
  }

  close_ui(state);
  exit(EXIT_SUCCESS);
}
