#include "ui.h"
#include "fs.h"
#include <ncurses.h>
#include <string.h>

#define SELECTED_COLOR_PAIR 1
#define UNSELECTED_COLOR_PAIR 2

static void clear_line(int row) {
  int y, x;            // to store where you are
  getyx(stdscr, y, x); // save current pos
  move(row, 0);        // move to begining of line
  clrtoeol();          // clear line
  move(y, x);          // move back to where you were
}

static void highlight(ui_state *state, int item_index) {

  int max_x = getmaxx(stdscr);
  if (item_index >= state->dir_list->dir_count || item_index < 0) {
    return;
  }
  char *dir_name = state->dir_list->dir_names[item_index];

  set_cursor(0, item_index);
  clear_line(item_index);
  wattron(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
  addstr(dir_name);
  for (int i = 0; i < getmaxx(stdscr) - strlen(dir_name); i++) {
    addstr(" ");
  }
  wattroff(stdscr, COLOR_PAIR(SELECTED_COLOR_PAIR));
  set_cursor(0, item_index);
}

ui_state *create_ui() {
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
  int row, col;
  getmaxyx(stdscr, row, col);
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

void render_top_bar(ui_state *state) {
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
  for (int i = 0; i < state->dir_list->dir_count; i++) {
    set_cursor(0, i);
    addstr(state->dir_list->dir_names[i]);
  }
  highlight(state, state->selected_item);
  render_top_bar(state);
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
  return 0;
}
