#include "fs.h"
#include "path.h"
#include "ui.h"
#include <bits/getopt_core.h>
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
  if (parse_options(&options, argv, argc))
    exit(EXIT_FAILURE);

  char path[PATH_MAX + 1];
  realpath(argv[optind], path);

  ui_state *state = initui();
  set_cwd(state, path);
  while (1) {
    render_ui(state);
    switch (getch()) {
    case 'k': {
      state->selected_item--;
    } break;
    case 'j': {
      state->selected_item++;
    } break;
    case '\n': {
      char *selected_item = get_selected_item(state);
      char new_path[PATH_MAX + 1];
      path_resolve(state->dir_list->dir_path, selected_item, new_path);

      if (is_dir(new_path))
        set_cwd(state, new_path);
      else if (is_file(selected_item))
        open_file(new_path);
    } break;
    case 'q':
      goto stop_event_loop;
    }
  }
  stop_event_loop:

  close_ui(state);
  exit(EXIT_SUCCESS);
}
