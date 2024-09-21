#ifndef _HOSTFS_H_
#define _HOSTFS_H_
#include "vfs.h"

#define HOSTFS_TYPE 1

// dinode type
#define H_FILE FILE_I
#define H_DIR DIR_I

// root directory
#define H_ROOT_DIR "./hostfs_root"

// hostfs utility functin declarations
int register_hostfs();
struct device *init_host_device(char *name);
void get_path_string(char *path, struct dentry *dentry);
struct vinode *hostfs_alloc_vinode(struct super_block *sb);
int hostfs_write_back_vinode(struct vinode *vinode);
int hostfs_update_vinode(struct vinode *vinode);

// hostfs interface function declarations
ssize_t hostfs_read(struct vinode *f_inode, char *r_buf, ssize_t len,
                    int *offset);
ssize_t hostfs_write(struct vinode *f_inode, const char *w_buf, ssize_t len,
                     int *offset);
struct vinode *hostfs_lookup(struct vinode *parent, struct dentry *sub_dentry);
struct vinode *hostfs_create(struct vinode *parent, struct dentry *sub_dentry);
int hostfs_lseek(struct vinode *f_inode, ssize_t new_offset, int whence,
                  int *offset);
int hostfs_link(struct vinode *parent, struct dentry *sub_dentry, struct vinode *link_node);
int hostfs_unlink(struct vinode *parent, struct dentry *sub_dentry, struct vinode *unlink_node);
int hostfs_hook_open(struct vinode *f_inode, struct dentry *f_dentry);
int hostfs_hook_close(struct vinode *f_inode, struct dentry *dentry);
int hostfs_readdir(struct vinode *dir_vinode, struct dir *dir, int *offset);
struct vinode *hostfs_mkdir(struct vinode *parent, struct dentry *sub_dentry);
struct super_block *hostfs_get_superblock(struct device *dev);

extern const struct vinode_ops hostfs_node_ops;

#endif
