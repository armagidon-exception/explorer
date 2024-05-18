#ifndef DIR_HANDLE_H
#define DIR_HANDLE_H

#include "utils.h"
#include <dirent.h>

string *dir_list_files(int *plen, string abs_path);
#endif /* end of include guard: DIR_HANDLE_H */
