#ifndef _VFS_H_
#define _VFS_H_

#include "util/types.h"

#define MAX_VFS_DEV 10            // the maximum number of vfs_dev_list
#define MAX_DENTRY_NAME_LEN 30    // the maximum length of dentry name
#define MAX_DEVICE_NAME_LEN 30    // the maximum length of device name
#define MAX_MOUNTS 10             // the maximum number of mounts
#define MAX_DENTRY_HASH_SIZE 100  // the maximum size of dentry hash table
#define MAX_PATH_LEN 30           // the maximum length of path
#define MAX_SUPPORTED_FS 10       // the maximum number of supported file systems

#define DIRECT_BLKNUM 10          // the number of direct blocks

/**** vfs initialization function ****/
int vfs_init();

/**** vfs interfaces ****/

// device interfaces
struct super_block *vfs_mount(const char *dev_name, int mnt_type);

// file interfaces
struct file *vfs_open(const char *path, int flags);
ssize_t vfs_read(struct file *file, char *buf, size_t count);
ssize_t vfs_write(struct file *file, const char *buf, size_t count);
ssize_t vfs_lseek(struct file *file, ssize_t offset, int whence);
int vfs_stat(struct file *file, struct istat *istat);
int vfs_disk_stat(struct file *file, struct istat *istat);
int vfs_link(const char *oldpath, const char *newpath);
int vfs_unlink(const char *path);
int vfs_close(struct file *file);

// directory interfaces
struct file *vfs_opendir(const char *path);
int vfs_readdir(struct file *file, struct dir *dir);
int vfs_mkdir(const char *path);
int vfs_closedir(struct file *file);

/**** vfs abstract object types ****/
// system root direntry
extern struct dentry *vfs_root_dentry;

// vfs abstract dentry
struct dentry {
  char name[MAX_DENTRY_NAME_LEN];
  int d_ref;
  struct vinode *dentry_inode;
  struct dentry *parent;
  struct super_block *sb;
};


// dentry constructor and destructor
struct dentry *alloc_vfs_dentry(const char *name, struct vinode *inode,
                            struct dentry *parent);
int free_vfs_dentry(struct dentry *dentry);

// ** dentry hash table **
extern struct hash_table dentry_hash_table;

// dentry hash table key type
struct dentry_key {
  struct dentry *parent;
  char *name;
};

// generic hash table method implementation
int dentry_hash_equal(void *key1, void *key2);
size_t dentry_hash_func(void *key);

// dentry hash table interface
struct dentry *hash_get_dentry(struct dentry *parent, char *name);
int hash_put_dentry(struct dentry *dentry);
int hash_erase_dentry(struct dentry *dentry);

// data structure of an openned file
struct file {
  int status;
  int readable;
  int writable;
  int offset;
  struct dentry *f_dentry;
};

// file constructor and destructor(use free_page to destruct)
struct file *alloc_vfs_file(struct dentry *dentry, int readable, int writable,
                        int offset);

// abstract device entry in vfs_dev_list
struct device {
  char dev_name[MAX_DEVICE_NAME_LEN];  // the name of the device
  int dev_id;  // the id of the device (the meaning of an id is interpreted by
               // the specific file system, all we need to know is that it is
               // a unique identifier)
  struct file_system_type *fs_type;  // the file system type in the device
};

// device list in vfs layer
extern struct device *vfs_dev_list[MAX_VFS_DEV];

// supported file system types
struct file_system_type {
  int type_num;  // the number of the file system type
  struct super_block *(*get_superblock)(struct device *dev);
};

extern struct file_system_type *fs_list[MAX_SUPPORTED_FS];

// general-purpose super_block structure
struct super_block {
  int magic;              // magic number of the file system
  int size;               // size of file system image (blocks)
  int nblocks;            // number of data blocks
  int ninodes;            // number of inodes.
  struct dentry *s_root;  // root dentry of inode
  struct device *s_dev;   // device of the superblock
  void *s_fs_info;        // filesystem-specific info. for rfs, it points bitmap
};

// abstract vfs inode
struct vinode {
  int inum;                  // inode number of the disk inode
  int ref;                   // reference count
  int size;                  // size of the file (in bytes)
  int type;                  // one of FILE_I, DIR_I
  int nlinks;                // number of hard links to this file
  int blocks;                // number of blocks
  int addrs[DIRECT_BLKNUM];  // direct blocks
  void *i_fs_info;           // filesystem-specific info (see s_fs_info)
  struct super_block *sb;          // super block of the vfs inode
  const struct vinode_ops *i_ops;  // vfs inode operations
};

struct vinode_ops {
  // file operations
  ssize_t (*viop_read)(struct vinode *node, char *buf, ssize_t len,
                       int *offset);
  ssize_t (*viop_write)(struct vinode *node, const char *buf, ssize_t len,
                        int *offset);
  struct vinode *(*viop_create)(struct vinode *parent, struct dentry *sub_dentry);
  int (*viop_lseek)(struct vinode *node, ssize_t new_off, int whence, int *off);
  int (*viop_disk_stat)(struct vinode *node, struct istat *istat);
  int (*viop_link)(struct vinode *parent, struct dentry *sub_dentry,
                   struct vinode *link_node);
  int (*viop_unlink)(struct vinode *parent, struct dentry *sub_dentry,
                     struct vinode *unlink_node);
  struct vinode *(*viop_lookup)(struct vinode *parent,
                                struct dentry *sub_dentry);

  // directory operations
  int (*viop_readdir)(struct vinode *dir_vinode, struct dir *dir, int *offset);
  struct vinode *(*viop_mkdir)(struct vinode *parent, struct dentry *sub_dentry);

  // write back inode to disk
  int (*viop_write_back_vinode)(struct vinode *node);

  // hook functions
  // In the vfs layer, we do not assume that hook functions will do anything,
  // but simply call them (when they are defined) at the appropriate time.
  // Hook functions exist because the fs layer may need to do some additional
  // operations (such as allocating additional data structures) at some critical
  // times.
  int (*viop_hook_open)(struct vinode *node, struct dentry *dentry);
  int (*viop_hook_close)(struct vinode *node, struct dentry *dentry);
  int (*viop_hook_opendir)(struct vinode *node, struct dentry *dentry);
  int (*viop_hook_closedir)(struct vinode *node, struct dentry *dentry);
};

// vinode operation interface
// the implementation depends on the vinode type and the specific file system

// virtual file system inode interfaces
#define viop_read(node, buf, len, offset)      (node->i_ops->viop_read(node, buf, len, offset))
#define viop_write(node, buf, len, offset)     (node->i_ops->viop_write(node, buf, len, offset))
#define viop_create(node, name)                (node->i_ops->viop_create(node, name))
#define viop_lseek(node, new_off, whence, off) (node->i_ops->viop_lseek(node, new_off, whence, off))
#define viop_disk_stat(node, istat)            (node->i_ops->viop_disk_stat(node, istat))
#define viop_link(node, name, link_node)       (node->i_ops->viop_link(node, name, link_node))
#define viop_unlink(node, name, unlink_node)   (node->i_ops->viop_unlink(node, name, unlink_node))
#define viop_lookup(parent, sub_dentry)        (parent->i_ops->viop_lookup(parent, sub_dentry))
#define viop_readdir(dir_vinode, dir, offset)  (dir_vinode->i_ops->viop_readdir(dir_vinode, dir, offset))
#define viop_mkdir(dir, sub_dentry)            (dir->i_ops->viop_mkdir(dir, sub_dentry))
#define viop_write_back_vinode(node)           (node->i_ops->viop_write_back_vinode(node))

// vinode hash table
extern struct hash_table vinode_hash_table;

// vinode hash table key type
struct vinode_key {
  int inum;
  struct super_block *sb;
};

// generic hash table method implementation
int vinode_hash_equal(void *key1, void *key2);
size_t vinode_hash_func(void *key);

// vinode hash table interface
struct vinode *hash_get_vinode(struct super_block *sb, int inum);
int hash_put_vinode(struct vinode *vinode);
int hash_erase_vinode(struct vinode *vinode);

// other utility functions
struct vinode *default_alloc_vinode(struct super_block *sb);
struct dentry *lookup_final_dentry(const char *path, struct dentry **parent,
                                   char *miss_name);
void get_base_name(const char *path, char *base_name);

#endif
