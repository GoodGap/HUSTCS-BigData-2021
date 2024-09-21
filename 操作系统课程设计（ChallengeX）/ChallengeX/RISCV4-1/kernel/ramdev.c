/*
 * Utility functions operating the devices. support only RAM disk device. added @lab4_1.
 */

#include "ramdev.h"
#include "vfs.h"
#include "pmm.h"
#include "riscv.h"
#include "util/types.h"
#include "util/string.h"
#include "spike_interface/spike_utils.h"
#include "rfs.h"

struct rfs_device *rfs_device_list[MAX_RAMDISK_COUNT];

//
// write the content stored in "buff" to the "blkno"^th block of disk.
//
int ramdisk_write(struct rfs_device *rfs_device, int blkno){
  if ( blkno < 0 || blkno >= RAMDISK_BLOCK_COUNT )
    panic("ramdisk_write: write block No %d out of range!\n", blkno);
  void * dst = (void *)((uint64)rfs_device->d_address + blkno * RAMDISK_BLOCK_SIZE);
  memcpy(dst, rfs_device->iobuffer, RAMDISK_BLOCK_SIZE);
  return 0;
}

//
// read the "blkno"^th block from the RAM disk and store its content into buffer.
//
int ramdisk_read(struct rfs_device *rfs_device, int blkno){
  if ( blkno < 0 || blkno >= RAMDISK_BLOCK_COUNT )
    panic("ramdisk_read: read block No out of range!\n");
  void * src = (void *)((uint64)rfs_device->d_address + blkno * RAMDISK_BLOCK_SIZE);
  memcpy(rfs_device->iobuffer, src, RAMDISK_BLOCK_SIZE);
  return 0;
}

//
// alloc RAMDISK_BLOCK_COUNT continuous pages (blocks) for the RAM Disk
// setup an vfs node, initialize RAM disk device, and attach the device with the vfs node.
//
struct device *init_rfs_device(const char *dev_name) {
  // find rfs in registered fs list
  struct file_system_type *fs_type = NULL;
  for (int i = 0; i < MAX_SUPPORTED_FS; i++) {
    if (fs_list[i] != NULL && fs_list[i]->type_num == RFS_TYPE) {
      fs_type = fs_list[i];
      break; 
    }
  }
  if (!fs_type) {
    panic("No RFS file system found!\n");
  }

  // alloc blocks for the RAM Disk
  void *curr_addr = NULL;
  void *last_addr = NULL;
  void *ramdisk_addr = NULL;
  for ( int i = 0; i < RAMDISK_BLOCK_COUNT; ++ i ){
    last_addr = curr_addr;
    curr_addr = alloc_page();
    if ( last_addr != NULL && last_addr - curr_addr != PGSIZE ){
      panic("RAM Disk0: address is discontinuous!\n");
    }
  }
  ramdisk_addr = curr_addr;

  // find a free rfs device
  struct rfs_device **rfs_device = NULL;
  int device_id = 0;
  for (int i = 0; i < MAX_RAMDISK_COUNT; i++) {
    if (rfs_device_list[i] == NULL) {
      rfs_device = &rfs_device_list[i];
      device_id = i;
      break;
    }
  }
  if (!rfs_device) {
    panic("RAM Disk0: no free device!\n");
  }
  
  *rfs_device = (struct rfs_device *)alloc_page();
  (*rfs_device)->d_blocks = RAMDISK_BLOCK_COUNT;
  (*rfs_device)->d_blocksize = RAMDISK_BLOCK_SIZE;
  (*rfs_device)->d_write = ramdisk_write;
  (*rfs_device)->d_read = ramdisk_read;
  (*rfs_device)->d_address = ramdisk_addr;
  (*rfs_device)->iobuffer = alloc_page();

  // allocate a vfs device
  struct device * device = (struct device *)alloc_page();
  // set the device name and index
  strcpy(device->dev_name, dev_name);
  device->dev_id = device_id;
  device->fs_type = fs_type;

  // add the device to the vfs device list
  for(int i = 0; i < MAX_VFS_DEV; i++) {
    if (vfs_dev_list[i] == NULL) {
      vfs_dev_list[i] = device;
      break;
    }
  }

  // sprint("%s: base address of %s is: %p\n",dev_name, dev_name, ramdisk_addr);
  return device;
}