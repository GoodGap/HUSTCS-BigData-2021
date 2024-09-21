#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

typedef int bool;

typedef signed long ssize_t;
typedef unsigned long size_t;


#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0

#define LSEEK_SET 0      
#define LSEEK_CUR 1      

#define MOUNT_DEFAULT 0
#define MOUNT_AS_ROOT 1

#define FILE_I 0
#define DIR_I 1

#define MASK_FILEMODE 0x003

#define O_RDONLY  00  // read-only access
#define O_WRONLY  01  // write-only access
#define O_RDWR    02  // read-write
#define O_CREAT 0100  // create

#define FD_NONE 0
#define FD_OPENED 1

#define MAX_FILE_NAME_LEN 32

struct dir {
  char name[MAX_FILE_NAME_LEN];
  int inum;
};

struct istat {
  int st_inum;
  int st_size;
  int st_type;
  int st_nlinks;
  int st_blocks;
};

#endif
