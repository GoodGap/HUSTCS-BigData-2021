#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"

int main(int argc, char *argv[]) {
  int fd;
  int MAXBUF = 512;
  char buf[MAXBUF];
  char *filename = argv[0];

  printu("\n======== cat command ========\n");
  printu("cat: %s\n", filename);

  fd = open(filename, O_RDWR);
  printu("file descriptor fd: %d\n", fd);

  read_u(fd, buf, MAXBUF);
  printu("read content: \n%s\n", buf);
  close(fd);

  exit(0);
  return 0;
}
