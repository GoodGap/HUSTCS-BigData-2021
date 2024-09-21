#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"


int main(int argc, char *argv[]) {
  // asm volatile("csrw sscratch, 0");
  char *new_dir = argv[0];
  printu("\n======== mkdir command ========\n");

  mkdir_u(new_dir);

  printu("mkdir: %s\n", new_dir);

  exit(0);
  return 0;
}
