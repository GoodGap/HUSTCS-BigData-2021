/*
 * contains the implementation of all syscalls.
 */

#include <stdint.h>
#include <errno.h>

#include "memlayout.h"
#include "util/types.h"
#include "syscall.h"
#include "string.h"
#include "process.h"
#include "util/functions.h"
#include "pmm.h"
#include "vmm.h"
#include "vfs.h"
#include "sched.h"
#include "proc_file.h"

#include "spike_interface/spike_utils.h"

//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char *buf, size_t n)
{
  // buf is now an address in user space of the given app's user stack,
  // so we have to transfer it into phisical address (kernel is running in direct mapping).
  assert(current[read_tp()]);
  char *pa = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void *)buf);
  sprint(pa);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
extern process procs[NPROC];

static int exit_count = 0;

ssize_t sys_user_exit(uint64 code)
{
	uint64 hartid = read_tp();
	sprint("hartid = %lld: User exit with code: %d.\n", hartid, code); //multi
	if (current[hartid]->parent != NULL && current[hartid]->parent->status == BLOCKED) {
		if (current[hartid]->parent->waitpid == -1)
			insert_to_ready_queue(current[hartid]->parent);
		else if (current[hartid]->parent->waitpid == current[hartid]->pid) {
			current[hartid]->parent->status = READY;
			insert_to_ready_queue(current[hartid]->parent);
		}
	}
	free_process(current[hartid]);
	schedule();
	return 0;
}

//
// maybe, the simplest implementation of malloc in the world ... added @lab2_2
//
uint64 sys_user_naive_allocate_page()
{
  void *pa = alloc_page();
  uint64 va;
  // if there are previously reclaimed pages, use them first (this does not change the
  // size of the heap)
  if (current[read_tp()]->user_heap.free_pages_count > 0)
  {
    va = current[read_tp()]->user_heap.free_pages_address[--(current[read_tp()])->user_heap.free_pages_count];
    assert(va < current[read_tp()]->user_heap.heap_top);
  }
  else
  {
    // otherwise, allocate a new page (this increases the size of the heap by one page)
    va = current[read_tp()]->user_heap.heap_top;
    current[read_tp()]->user_heap.heap_top += PGSIZE;

    current[read_tp()]->mapped_info[HEAP_SEGMENT].npages++;
  }
  user_vm_map((pagetable_t)current[read_tp()]->pagetable, va, PGSIZE, (uint64)pa,
              prot_to_type(PROT_WRITE | PROT_READ, 1));

  return va;
}

// updated
uint64 sys_user_allocate_page(uint64 n) {
  uint64 addr = malloc((int)n);
  return addr;
}

//
// reclaim a page, indicated by "va". added @lab2_2
//
uint64 sys_user_naive_free_page(uint64 va)
{
  user_vm_unmap((pagetable_t)current[read_tp()]->pagetable, va, PGSIZE, 1);
  // add the reclaimed page to the free page list
  current[read_tp()]->user_heap.free_pages_address[current[read_tp()]->user_heap.free_pages_count++] = va;
  return 0;
}


//
// reclaim a page, indicated by "va". added @lab2_2
//
uint64 sys_user_free_page(uint64 va) {
  free((void *)va);
  return 0;
}

//
// kerenl entry point of naive_fork
//
ssize_t sys_user_fork()
{
  // sprint("User call fork.\n");
  return do_fork(current[read_tp()]);
}

//
// kerenl entry point of yield. added @lab3_2
//
ssize_t sys_user_yield()
{
  // TODO (lab3_2): implment the syscall of yield.
  // hint: the functionality of yield is to give up the processor. therefore,
  // we should set the status of currently running process to READY, insert it in
  // the rear of ready queue, and finally, schedule a READY process to run.
  current[read_tp()]->status = READY;
  insert_to_ready_queue(current[read_tp()]);
  schedule();
  return 0;
}


//
// open file
//
ssize_t sys_user_open(char *pathva, int flags)
{
  char apath[MAX_PATH_LEN * 2];
  char *pathpa = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), pathva);
  relative_path_to_absolute_path(pathpa, apath);
  return do_open(apath, flags);
}

//
// read file
//
ssize_t sys_user_read(int fd, char *bufva, uint64 count) {
  int i = 0;
  while (i < count)
  { // count can be greater than page size
    uint64 addr = (uint64)bufva + i;
    uint64 pa = lookup_pa((pagetable_t)current[read_tp()]->pagetable, addr);
    uint64 off = addr - ROUNDDOWN(addr, PGSIZE);
    uint64 len = count - i < PGSIZE - off ? count - i : PGSIZE - off;
    uint64 r = do_read(fd, (char *)pa + off, len);
    i += r;
    if (r < len)
    	return i;
  }
  return count;
}

//
// write file
//
ssize_t sys_user_write(int fd, char *bufva, uint64 count) {
  int i = 0;
  while (i < count)
  { // count can be greater than page size
    uint64 addr = (uint64)bufva + i;
    uint64 pa = lookup_pa((pagetable_t)current[read_tp()]->pagetable, addr);
    uint64 off = addr - ROUNDDOWN(addr, PGSIZE);
    uint64 len = count - i < PGSIZE - off ? count - i : PGSIZE - off;
    uint64 r = do_write(fd, (char *)pa + off, len);
    i += r;
    if (r < len)
    	return i;
  }
  return count;
}

//
// lseek file
//
ssize_t sys_user_lseek(int fd, int offset, int whence) {
  return do_lseek(fd, offset, whence);
}

//
// read vinode
//
ssize_t sys_user_stat(int fd, struct istat *istat) {
  struct istat * pistat = (struct istat *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), istat);
  return do_stat(fd, pistat);
}

//
// read disk inode
//
ssize_t sys_user_disk_stat(int fd, struct istat *istat) {
  struct istat * pistat = (struct istat *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), istat);
  return do_disk_stat(fd, pistat);
}

//
// close file
//
ssize_t sys_user_close(int fd) {
  return do_close(fd);
}


ssize_t sys_user_dup2(int oldfd, int newfd)
{
  return do_dup2(oldfd, newfd);
}

//
// lib call to opendir
//
ssize_t sys_user_opendir(char *pathva)
{
  char apath[MAX_PATH_LEN * 2];
  char *pathpa = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), pathva);
  relative_path_to_absolute_path(pathpa, apath);
  return do_opendir(apath);
}

//
// lib call to readdir
//
ssize_t sys_user_readdir(int fd, struct dir *vdir){
  struct dir * pdir = (struct dir *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), vdir);
  return do_readdir(fd, pdir);
}


//
// lib call to mkdir
//
ssize_t sys_user_mkdir(char *pathva)
{
  char apath[MAX_PATH_LEN * 2];
  char *pathpa = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), pathva);
  relative_path_to_absolute_path(pathpa, apath);
  return do_mkdir(apath);
}

//
// lib call to closedir
//
ssize_t sys_user_closedir(int fd){
  return do_closedir(fd);
}

//
// lib call to link
//
ssize_t sys_user_link(char * vfn1, char * vfn2){
  char * pfn1 = (char*)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)vfn1);
  char * pfn2 = (char*)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)vfn2);
  return do_link(pfn1, pfn2);
}

//
// lib call to unlink
//
ssize_t sys_user_unlink(char * vfn){
  char * pfn = (char*)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)vfn);
  return do_unlink(pfn);
}

//
// lib call to execute
//
ssize_t sys_user_exec(char *vcommand, char *vparam)
{
  char real_command[MAX_PATH_LEN * 2], real_param[MAX_PATH_LEN * 2];
  char *command = (char*)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)vcommand);
  char *param = (char*)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)vparam);
  relative_path_to_absolute_path(command, real_command);
  relative_path_to_absolute_path(param, real_param);
  return load_exec_program(real_command, real_param);
}

//
// lib call to wait
//
ssize_t sys_user_wait(int pid)
{
  sprint("into wait\n");
  if (pid == 0 || procs[pid].parent != current[read_tp()])
    return -1;
  current[read_tp()]->status = BLOCKED;
  current[read_tp()]->waitpid = pid;
  schedule();
  return pid;
}

int sys_user_sem(uint64 val)
{
  return sem(val);
}

void sys_user_sem_P(uint64 sem)
{
  P(sem);
}

void sys_user_sem_V(uint64 sem)
{
  V(sem);
}

ssize_t sys_user_printpa(uint64 va)
{
  uint64 pa = (uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)va);
  sprint("%lx\n", pa);
  return 0;
}

//
// lib call to print backtrace
//
ssize_t sys_print_backtrace(int64 dep)
{
  int i;
  const int func = 18;
  const uint64 m = 40; 
  uint64 cur_depth = 0;
  uint64 sp = (uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)(m + current[read_tp()]->trapframe->regs.sp));
  uint64 fp = (uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)(current[read_tp()]->trapframe->regs.s0));
  while (cur_depth < dep)
  {
    sp = fp + 8;
    uint64 ra = (uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)(*(uint64 *)sp));
    fp = ((uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)(*(uint64 *)fp))) - 16;
    cur_depth++;
    int string_table_index = -1;
    uint64 pre_ra = 0;
    for (i = 0; i < current[read_tp()]->symbol_num; i++)
    {
      elf_symbol *func_symbol = &(current[read_tp()]->symbols[i]);
      if (func_symbol->info != func)
        continue;
      uint64 pval = (uint64)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void*)(func_symbol->value));
      if (pval > pre_ra && pval < ra)
      {
        string_table_index = i;
        pre_ra = pval;
      }
    }
    elf_symbol *func_symbol = &(current[read_tp()]->symbols[string_table_index]);
    sprint("%s\n", &(current[read_tp()]->string_table[func_symbol->name]));
    if (strcmp("main", &(current[read_tp()]->string_table[func_symbol->name])) == 0)
      break;
  }
  return 0;
}

//
// lib call to read cwd
//
ssize_t sys_user_rcwd(char *vpath)
{
  char *path = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void *)vpath);
  return do_rcwd(path);
}

//
// lib call to change cwd
//
ssize_t sys_user_ccwd(const char *vpath)
{
  char apath[2 * MAX_PATH_LEN];
  char *path = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void *)vpath);
  relative_path_to_absolute_path(path, apath);
  return do_ccwd(apath);
}

ssize_t sys_user_scan(const char *buf)
{
  assert(current[read_tp()]);
  char *pa = (char *)user_va_to_pa((pagetable_t)(current[read_tp()]->pagetable), (void *)buf);
  sscan(pa);
  return 0;
}

// void rp_ap(char *relative_path, char *absolute_path)
void relative_path_to_absolute_path(char *relative_path, char *absolute_path)
{
  const char *rp = relative_path;
  char *ap = absolute_path;
  if (strlen(relative_path) == 0)
    return;
  else if (relative_path[0] == '/')
  {
    strcpy(absolute_path, relative_path);
    return;
  }
  if (strcmp("/", current[read_tp()]->pfiles->cwd->name))
    strcpy(absolute_path, current[read_tp()]->pfiles->cwd->name);
  while (*ap != '\0')
    ap++;
  while (*rp != '\0')
  {
    char temp[MAX_PATH_LEN * 2], *t = temp;
    while (*rp != '\0' && *rp != '/')
    {
      *t = *rp;
      rp++;
      t++;
    }
    *t = '\0';
    if (strcmp("..", temp) == 0)
    {
      ap--;
      while (*ap != '/')
        ap--;
    }
    else if (strcmp(".", temp))
    {
      *ap = '/';
      ap++;
      while (*t != '\0')
      {
        *ap = *t;
        t++;
        ap++;
      }
    }
    if (*rp != '\0')
      rp++;
  }
  *ap = '\0';
  if (strlen(absolute_path) == 0)
    strcpy(absolute_path, "/");
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char*)a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    // added @lab2_2
    case SYS_user_allocate_page:
      return sys_user_allocate_page(a1);
    case SYS_user_free_page:
      return sys_user_free_page(a1);
    case SYS_user_naive_allocate_page:
      return sys_user_naive_allocate_page();
    case SYS_user_naive_free_page:
      return sys_user_naive_free_page(a1);
    case SYS_user_fork:
      return sys_user_fork();
    case SYS_user_yield:
      return sys_user_yield();
    // added @lab4_1
    case SYS_user_open:
      return sys_user_open((char *)a1, a2);
    case SYS_user_read:
      return sys_user_read(a1, (char *)a2, a3);
    case SYS_user_write:
      return sys_user_write(a1, (char *)a2, a3);
    case SYS_user_lseek:
      return sys_user_lseek(a1, a2, a3);
    case SYS_user_stat:
      return sys_user_stat(a1, (struct istat *)a2);
    case SYS_user_disk_stat:
      return sys_user_disk_stat(a1, (struct istat *)a2);
    case SYS_user_close:
      return sys_user_close(a1);
    // added @lab4_2
    case SYS_user_opendir:
      return sys_user_opendir((char *)a1);
    case SYS_user_readdir:
      return sys_user_readdir(a1, (struct dir *)a2);
    case SYS_user_mkdir:
      return sys_user_mkdir((char *)a1);
    case SYS_user_closedir:
      return sys_user_closedir(a1);
    // added @lab4_3
    case SYS_user_link:
      return sys_user_link((char *)a1, (char *)a2);
    case SYS_user_execute:
      return sys_user_exec((char *)a1, (char *)a2);
    case SYS_user_wait:
      return sys_user_wait((uint64)a1);
    case SYS_user_scan:
      return sys_user_scan((const char*)a1);
    // added @lab1_challenge1
    case SYS_print_backtrace:
      return sys_print_backtrace(a1);
    case SYS_user_printpa:
      return sys_user_printpa(a1);
    case SYS_user_dup2:
      return sys_user_dup2((int)a1, (int)a2);
    // added @lab3_challenge1
    case SYS_user_sem:
      return sys_user_sem(a1);
    case SYS_user_sem_P:
      sys_user_sem_P(a1);
      return 0;
    case SYS_user_sem_V:
      sys_user_sem_V(a1);
      return 0;
    // added @lab4_challenge1
    case SYS_user_rcwd:
      return sys_user_rcwd((char *)a1);
    case SYS_user_ccwd:
      return sys_user_ccwd((const char *)a1);
    
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}
