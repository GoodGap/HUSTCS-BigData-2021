#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"

int main(int argc, char *argv[]) {
  int fd;
  char *filename = argv[0];
  printu("\n======== touch command ========\n");
  printu("touch: %s\n", filename);

  fd = open(filename, O_CREAT);
  printu("file descriptor fd: %d\n", fd);

  close(fd);
  exit(0);
  return 0;
}
