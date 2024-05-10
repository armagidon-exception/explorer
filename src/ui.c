#include "ui.h"
#include "fs.h"
#include "utils.h"
#include <ncurses.h>
#include <string.h>

#define SELECTED_COLOR_PAIR 1
#define UNSELECTED_COLOR_PAIR 2

static void clear_line(int row) {
  int y, x;
  getyx(stdscr, y, x);
  move(row, 0);
  clrtoeol();
  move(y, x);
}

static void highlight(ui_state *state, int item_index) {

  if (item_index >= state->dir_list->dir_length || item_index < 0) {
    return;
  }
  int max_x = getmaxx(stdscr);
  char *dir_name = state->dir_list->dir_names[item_index];
  int len = strlen(dir_name);

  set_cursor(0, item_index);
  clear_line(item_index);
  wattron(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
  addstr(dir_name);
  for (int i = 0; i < max_x - len; i++) {
    addstr(" ");
  }
  wattroff(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
  set_cursor(0, item_index);
}

ui_state *initui() {
  ui_state *state = malloc(sizeof(ui_state));
  memset(state, 0, sizeof(ui_state));

  initscr();
  if (!has_colors()) {
    fprintf(stderr, "Colors are not supported.\n");
    exit(EXIT_FAILURE);
  }
  start_color();
  noecho();
  init_pair(SELECTED_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
  init_pair(UNSELECTED_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
  curs_set(0);
  keypad(stdscr, true);

  int height = getmaxy(stdscr);
  state->view_size = height - 1;
  refresh();

  return state;
}

void close_ui(ui_state *state) {
  close_dir(state->dir_list);
  free(state);
  endwin();
}

void set_cursor(int x, int y) { move(y, x); }

void move_cursor(int dx, int dy) {
  int cx, cy;
  getyx(stdscr, cy, cx);
  set_cursor(cx + dx, cy + dy);
}

void render_bottom_bar(ui_state *state) {
  int cx, cy;
  getyx(stdscr, cy, cx);
  int height = getmaxy(stdscr);
  set_cursor(0, height - 1);
  clrtoeol();
  addstr(state->dir_list->dir_path);
  set_cursor(cx, cy);
}

void render_ui(ui_state *state) {
  clear();
  for (int i = 0; i < min(get_dir_length(state), get_view_size(state)); i++) {
    set_cursor(0, i);
    char *dir_name = state->dir_list->dir_names[i + state->view_offset];
    if (i + state->view_offset == state->selected_item) {
      clear_line(i);
      wattron(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
      int max_x = getmaxx(stdscr);
      int len = strlen(dir_name);
      addstr(dir_name);
      for (int i = 0; i < max_x - len; i++) {
        addstr(" ");
      }
      wattroff(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
    } else {
      addstr(dir_name);
    }
  }
  render_bottom_bar(state);
  refresh();
}

char *get_selected_item(ui_state *state) {
  return state->dir_list->dir_names[state->selected_item];
}

int set_cwd(ui_state *state, char *cwd) {
  dir_list *new_dir = read_dir(cwd);
  if (new_dir == NULL) {
    return 1;
  }
  close_dir(state->dir_list);
  state->dir_list = new_dir;
  state->selected_item = 0;
  state->view_offset = 0;
  return 0;
}

int get_view_size(ui_state *state) { return state->view_size; }

int get_dir_length(ui_state *state) { return state->dir_list->dir_length; }
