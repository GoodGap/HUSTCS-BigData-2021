#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"

int main(int argc, char *argv[]) {
  char *path = argv[0];
  int dir_fd = opendir_u(path);
  printu("---------- ls command -----------\n");
  printu("ls \"%s\":\n", path);
  printu("[name]               [inode_num]\n");
  struct dir dir;
  int width = 20;
  while(readdir_u(dir_fd, &dir) == 0) {
    // we do not have %ms :(
    char name[width + 1];
    memset(name, ' ', width + 1);
    name[width] = '\0';
    if (strlen(dir.name) < width) {
      strcpy(name, dir.name);
      name[strlen(dir.name)] = ' ';
      printu("%s %d\n", name, dir.inum);
    }
    else
      printu("%s %d\n", dir.name, dir.inum);
  }
  printu("------------------------------\n");
  closedir_u(dir_fd);

  exit(0);
  return 0;
}