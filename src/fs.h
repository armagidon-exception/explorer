#include <stdbool.h>
#include <stdlib.h>

#ifndef FS
#define FS

#define EMKDIR_EXISTS 1
#define EMKDIR_IO 2

int make_dirs(char *filename, bool make_parents);

#endif /* end of include guard: FS */
