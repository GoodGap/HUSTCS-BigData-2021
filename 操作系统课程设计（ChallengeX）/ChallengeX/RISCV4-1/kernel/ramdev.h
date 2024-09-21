#ifndef _RAMDEV_H_
#define _RAMDEV_H_

#include "riscv.h"
#include "util/types.h"

#define RAMDISK_BLOCK_COUNT 128
#define RAMDISK_BLOCK_SIZE  PGSIZE

#define MAX_RAMDISK_COUNT 10

#define RAMDISK_FREE 0
#define RAMDISK_USED 1

struct rfs_device {
  void *d_address;  // the ramdisk base address
  int d_blocks;     // the number of blocks of the device
  int d_blocksize;  // the blocksize (bytes) per block
  void *iobuffer;   // iobuffer for write/read
  int (*d_write)(struct rfs_device *rdev, int blkno);  // device write funtion
  int (*d_read)(struct rfs_device *rdev, int blkno); // device read funtion
};

#define dop_write(rdev, blkno) ((rdev)->d_write(rdev, blkno))
#define dop_read(rdev, blkno)  ((rdev)->d_read(rdev, blkno))

struct device *init_rfs_device(const char *dev_name);
struct rfs_device *alloc_rfs_device(void);

extern struct rfs_device *rfs_device_list[MAX_RAMDISK_COUNT];

#endif
