#ifndef FS
#define FS

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char **dir_names;
  int dir_count;
  char* dir_path;
} dir_list;

int make_dir(char *filename, bool make_parents);

int create_file(char *filename);

int delete_file(char *filename);

int open_file(char *filename);

dir_list *read_dir(char *directory);

void close_dir(dir_list *dir_list);

bool is_dir(char* filename);


bool is_file(char *filename);

#endif /* end of include guard: FS */
