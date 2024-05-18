#include "filesystem.h"
#include "dir_handle.h"
#include <asm-generic/errno-base.h>
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

const string ROOT_PATH = "/";

static const char *get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

static fs_type get_type(string path) {
  string ext;
  if (strcmp(get_filename_ext(path), (ext = "zip")) == 0)
    return FS_TYPE_DIR;
  else if (strcmp(get_filename_ext(path), (ext = "xz")) == 0)
    return FS_TYPE_XZ;
  else if (strcmp(get_filename_ext(path), (ext = "rar")) == 0)
    return FS_TYPE_RAR;
  else if (strcmp(get_filename_ext(path), (ext = "tar")) == 0)
    return FS_TYPE_TAR;
  else
    return FS_TYPE_DIR;
}

fs_view *create_view() {
  fs_view *view = malloc(sizeof(fs_view));
  memset(view, 0, sizeof(fs_view));
  view->cwd = calloc(sizeof(char), PATH_MAX + 1);
  view->type = get_type(view->cwd);
  return view;
}

void close_fs_view(fs_view *view) {
  if (view == NULL)
    return;
  free(view->cwd);
  if (view->dirs != NULL) {
    for (int i = 0; i < view->dirc; i++)
      free(view->dirs[i]);
    free(view->dirs);
  }
  free(view);
}

int fs_list_files(fs_view* view) {
  switch(view->type) {
    case FS_TYPE_DIR: {
      view->dirs = dir_list_files(&view->dirc, view->cwd);
    } break;
    case FS_TYPE_ZIP: {

    }
  }
}
