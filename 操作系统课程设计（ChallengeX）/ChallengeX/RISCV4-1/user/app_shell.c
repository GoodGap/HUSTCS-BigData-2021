#include "user_lib.h"
#include "string.h"
#include "util/types.h"

int main(int argc, char *argv[])
{
  char cmd[64], para[64], cwd[64];
  printu("RISCV-PKE Shell. Type 'leave' to leave.\n");
  while (1)
  {
    read_cwd(cwd);
    printu("Shell >>> %s$ ", cwd);
    scanu("%s %s", &cmd, &para);
    if (strcmp("leave", cmd) == 0)
      break;
    else if (strcmp("cd", cmd) == 0)
    {
      change_cwd(para);
      continue;
    }
    
    int pid = fork();
    if (pid == 0)
    {
      exec(cmd, para);
      return 1;
    }
    else
      wait(pid);
  }
  exit(0);
  return 0;
}
