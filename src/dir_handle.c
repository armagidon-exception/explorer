#include "dir_handle.h"
#include <stdlib.h>
string* dir_list_files(int *plen, string abs_path) {
  DIR *dir = opendir(abs_path);
  if (dir == NULL) {
    *plen = 0;
    return NULL;
  }

  int i = 0, c = 0;
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    c++;
  }
  string* dirs = calloc(c, sizeof(string));
  *plen = c;



  rewinddir(dir);
  while ((entry = readdir(dir)) != NULL) {
    dirs[i++] = strdup(entry->d_name);
  }

  closedir(dir);
  return dirs;
}
