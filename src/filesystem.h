#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "utils.h"


typedef enum {
  FS_TYPE_DIR = 0,
  FS_TYPE_ZIP,
  FS_TYPE_TAR,
  FS_TYPE_RAR,
  FS_TYPE_XZ
} fs_type;

typedef struct {
  fs_type type;
  string cwd;
  string* dirs;
  int dirc;
} fs_view;

fs_view* create_view();

int fs_set_cwd(fs_view* view, string path);

int fs_move_to(fs_view* view, string dir);

int fs_list_files(fs_view* view);

void close_fs_view(fs_view* view);

#endif /* end of include guard: FILESYSTEM_H */
