#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"

int main(int argc, char *argv[]) {
  int fd;
  char str[] = "hello world";
  char *filename = argv[0];
  printu("\n======== echo command ========\n");
  printu("echo: %s\n", filename);

  fd = open(filename, O_RDWR | O_CREAT);
  printu("file descriptor fd: %d\n", fd);

  write_u(fd, str, strlen(str));
  printu("write content: \n%s\n", str);
  close(fd);
  exit(0);
  return 0;
}
