#ifndef UI_H
#define UI_H

#include "fs.h"

typedef struct {
  dir_list *dir_list;
  int selected_item;
  int view_offset;
  int view_size;
} ui_state;

ui_state *initui();

void select_item(ui_state *state, int item_index);

void close_ui(ui_state *state);

void set_cursor(int x, int y);

void move_cursor(int dx, int dy);

void render_ui(ui_state *state);

char *get_selected_item(ui_state *state);

int set_cwd(ui_state *state, char *cwd);

char *get_cwd();

int get_view_size(ui_state *state);

int get_dir_length(ui_state *state);

#endif /* end of include guard: UI_H */
