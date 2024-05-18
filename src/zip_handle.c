#include "zip_handle.h"
#include "utils.h"
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

string *zip_list_files(int *plen, string zip_file, string rp) {
  zip_t *za;
  int err;
  if ((za = zip_open(zip_file, 0, &err)) == NULL) {
    zip_error_t error;
    zip_error_init_with_code(&error, err);
    fprintf(stderr, "%s: cannot open zip archive '%s': %s\n", "explorer",
            zip_file, zip_error_strerror(&error));
    zip_error_fini(&error);
    *plen = 0;
    return NULL;
  }

  struct zip_stat finfo = {0};

  zip_stat_init(&finfo);

  int c = 0;
  for (int ri = 0; zip_stat_index(za, ri, 0, &finfo); ri++) {
    if (strstartswith(rp, finfo.name)) {
      c++;
    }
  }
  *plen = c;
  string* dest = calloc()

  for (int ri = 0, wi = 0; zip_stat_index(za, ri, 0, &finfo); ri++) {
    if (strstartswith(rp, finfo.name)) {
      dest[wi++] = strdup(finfo.name);
    }
  }

  return 0;
}
