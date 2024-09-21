/*
 * Interface functions between VFS and host-fs. added @lab4_1.
 */
#include "hostfs.h"

#include "pmm.h"
#include "spike_interface/spike_file.h"
#include "spike_interface/spike_utils.h"
#include "util/string.h"
#include "util/types.h"
#include "vfs.h"

/**** host-fs vinode interface ****/
const struct vinode_ops hostfs_i_ops = {
    .viop_read = hostfs_read,
    .viop_write = hostfs_write,
    .viop_create = hostfs_create,
    .viop_lseek = hostfs_lseek,
    .viop_lookup = hostfs_lookup,

    .viop_hook_open = hostfs_hook_open,
    .viop_hook_close = hostfs_hook_close,

    .viop_write_back_vinode = hostfs_write_back_vinode,

    // not implemented
    .viop_link = hostfs_link,
    .viop_unlink = hostfs_unlink,
    .viop_readdir = hostfs_readdir,
    .viop_mkdir = hostfs_mkdir,
};

/**** hostfs utility functions ****/
//
// append hostfs to the fs list.
//
int register_hostfs()
{
  struct file_system_type *fs_type = (struct file_system_type *)alloc_page();
  fs_type->type_num = HOSTFS_TYPE;
  fs_type->get_superblock = hostfs_get_superblock;

  for (int i = 0; i < MAX_SUPPORTED_FS; i++)
  {
    if (fs_list[i] == NULL)
    {
      fs_list[i] = fs_type;
      return 0;
    }
  }
  return -1;
}

//
// append new device under "name" to vfs_dev_list.
//
struct device *init_host_device(char *name)
{
  // find rfs in registered fs list
  struct file_system_type *fs_type = NULL;
  for (int i = 0; i < MAX_SUPPORTED_FS; i++)
  {
    if (fs_list[i] != NULL && fs_list[i]->type_num == HOSTFS_TYPE)
    {
      fs_type = fs_list[i];
      break;
    }
  }
  if (!fs_type)
    panic("init_host_device: No HOSTFS file system found!\n");

  // allocate a vfs device
  struct device *device = (struct device *)alloc_page();
  // set the device name and index
  strcpy(device->dev_name, name);
  // we only support one host-fs device
  device->dev_id = 0;
  device->fs_type = fs_type;

  // add the device to the vfs device list
  for (int i = 0; i < MAX_VFS_DEV; i++)
  {
    if (vfs_dev_list[i] == NULL)
    {
      vfs_dev_list[i] = device;
      break;
    }
  }

  return device;
}

//
// recursive call to assemble a path.
//
void path_backtrack(char *path, struct dentry *dentry)
{
  if (dentry->parent == NULL)
  {
    return;
  }
  path_backtrack(path, dentry->parent);
  strcat(path, "/");
  strcat(path, dentry->name);
}

//
// obtain the absolute path for "dentry", from root to file.
//
void get_path_string(char *path, struct dentry *dentry)
{
  strcpy(path, H_ROOT_DIR);
  path_backtrack(path, dentry);
}

//
// allocate a vfs inode for an host fs file.
//
struct vinode *hostfs_alloc_vinode(struct super_block *sb)
{
  struct vinode *vinode = default_alloc_vinode(sb);
  vinode->inum = -1;
  vinode->i_fs_info = NULL;
  vinode->i_ops = &hostfs_i_ops;
  return vinode;
}

int hostfs_write_back_vinode(struct vinode *vinode) { return 0; }

//
// populate the vfs inode of an hostfs file, according to its stats.
//
int hostfs_update_vinode(struct vinode *vinode)
{
  spike_file_t *f = vinode->i_fs_info;
  if ((int64)f < 0)
  { // is a direntry
    vinode->type = H_DIR;
    return -1;
  }

  struct stat stat;
  spike_file_stat(f, &stat);

  vinode->inum = stat.st_ino;
  vinode->size = stat.st_size;
  vinode->nlinks = stat.st_nlink;
  vinode->blocks = stat.st_blocks;

  if (S_ISDIR(stat.st_mode))
  {
    vinode->type = H_DIR;
  }
  else if (S_ISREG(stat.st_mode))
  {
    vinode->type = H_FILE;
  }
  else
  {
    // sprint("hostfs_lookup:unknown file type!");
    return -1;
  }

  return 0;
}

/**** vfs-host-fs interface functions ****/
//
// read a hostfs file.
//
ssize_t hostfs_read(struct vinode *f_inode, char *r_buf, ssize_t len,
                    int *offset)
{
  spike_file_t *pf = (spike_file_t *)f_inode->i_fs_info;
  if (pf < 0)
  {
    // sprint("hostfs_read: invalid file handle!\n");
    return -1;
  }
  int read_len = spike_file_read(pf, r_buf, len);
  // obtain current offset
  *offset = spike_file_lseek(pf, 0, 1);
  return read_len;
}

//
// write a hostfs file.
//
ssize_t hostfs_write(struct vinode *f_inode, const char *w_buf, ssize_t len,
                     int *offset)
{
  spike_file_t *pf = (spike_file_t *)f_inode->i_fs_info;
  if (pf < 0)
  {
    // sprint("hostfs_write: invalid file handle!\n");
    return -1;
  }
  int write_len = spike_file_write(pf, w_buf, len);
  // obtain current offset
  *offset = spike_file_lseek(pf, 0, 1);
  return write_len;
}

//
// lookup a hostfs file, and establish its vfs inode in PKE vfs.
//
struct vinode *hostfs_lookup(struct vinode *parent, struct dentry *sub_dentry)
{
  // get complete path string
  char path[MAX_PATH_LEN];
  get_path_string(path, sub_dentry);

  spike_file_t *f = spike_file_open(path, O_RDWR, 0);

  struct vinode *child_inode = hostfs_alloc_vinode(parent->sb);
  child_inode->i_fs_info = f;
  hostfs_update_vinode(child_inode);

  child_inode->ref = 0;
  return child_inode;
}

//
// creates a hostfs file, and establish its vfs inode.
//
struct vinode *hostfs_create(struct vinode *parent, struct dentry *sub_dentry)
{
  char path[MAX_PATH_LEN];
  get_path_string(path, sub_dentry);

  spike_file_t *f = spike_file_open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if ((int64)f < 0)
  {
    // sprint("hostfs_create cannot create the given file.\n");
    return NULL;
  }

  struct vinode *new_inode = hostfs_alloc_vinode(parent->sb);
  new_inode->i_fs_info = f;

  if (hostfs_update_vinode(new_inode) != 0)
    return NULL;

  new_inode->ref = 0;
  return new_inode;
}

//
// reposition read/write file offset
//
int hostfs_lseek(struct vinode *f_inode, ssize_t new_offset, int whence,
                 int *offset)
{
  spike_file_t *f = (spike_file_t *)f_inode->i_fs_info;
  if (f < 0)
  {
    // sprint("hostfs_lseek: invalid file handle!\n");
    return -1;
  }

  *offset = spike_file_lseek(f, new_offset, whence);
  if (*offset >= 0)
    return 0;
  return -1;
}

int hostfs_link(struct vinode *parent, struct dentry *sub_dentry,
                struct vinode *link_node)
{
  panic("hostfs_link not implemented!\n");
  return -1;
}

int hostfs_unlink(struct vinode *parent, struct dentry *sub_dentry, struct vinode *unlink_node)
{
  panic("hostfs_unlink not implemented!\n");
  return -1;
}

int hostfs_readdir(struct vinode *dir_vinode, struct dir *dir, int *offset)
{
  // panic("hostfs_readdir not implemented!\n");
  // return -1;
  
  char c[128];
  spike_file_t *fd = (spike_file_t *)dir_vinode->i_fs_info;
  if (fd < 0)
    return -1;
  
  spike_file_readdir(fd, c, 128);

  for (int i = 0; i < 128; i++)
    sprint("%c", c[i]);
  sprint("\n");

  (*offset)++;
  return 0;
}

struct vinode *hostfs_mkdir(struct vinode *parent, struct dentry *sub_dentry)
{
  panic("hostfs_mkdir not implemented!\n");
  return NULL;
}

/**** vfs-hostfs hook interface functions ****/
//
// open a hostfs file (after having its vfs inode).
//
int hostfs_hook_open(struct vinode *f_inode, struct dentry *f_dentry)
{
  if (f_inode->i_fs_info != NULL)
    return 0;

  char path[MAX_PATH_LEN];
  get_path_string(path, f_dentry);
  spike_file_t *f = spike_file_open(path, O_RDWR, 0);
  if ((int64)f < 0)
  {
    // sprint("hostfs_hook_open cannot open the given file.\n");
    return -1;
  }

  f_inode->i_fs_info = f;
  return 0;
}

//
// close a hostfs file.
//
int hostfs_hook_close(struct vinode *f_inode, struct dentry *dentry)
{
  spike_file_t *f = (spike_file_t *)f_inode->i_fs_info;
  spike_file_close(f);
  return 0;
}

/**** vfs-hostfs file system type interface functions ****/
struct super_block *hostfs_get_superblock(struct device *dev)
{
  // set the data for the vfs super block
  struct super_block *sb = alloc_page();
  sb->s_dev = dev;

  struct vinode *root_inode = hostfs_alloc_vinode(sb);
  root_inode->type = H_DIR;

  struct dentry *root_dentry = alloc_vfs_dentry("/", root_inode, NULL);
  sb->s_root = root_dentry;

  return sb;
}
