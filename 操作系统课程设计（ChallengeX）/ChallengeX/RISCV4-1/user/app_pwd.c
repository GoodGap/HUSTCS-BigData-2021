#include "user_lib.h"
#include "util/string.h"
#include "util/types.h"

int main(int argc, char *argv[])
{
    // asm volatile("csrw sscratch, 0");
    printu("\n======== pwd command ========\n");
    char path[30];
    read_cwd(path);
    printu("cwd:%s\n", path);
    exit(0);
    return 0;
}