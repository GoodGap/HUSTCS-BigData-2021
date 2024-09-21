/*
 * Supervisor-mode startup codes
 */

#include "riscv.h"
#include "string.h"
#include "elf.h"
#include "process.h"
#include "pmm.h"
#include "vmm.h"
#include "sched.h"
#include "memlayout.h"
#include "sync_utils.h"
#include "spike_interface/spike_utils.h"
#include "util/types.h"
#include "vfs.h"
#include "rfs.h"
#include "ramdev.h"

//
// trap_sec_start points to the beginning of S-mode trap segment (i.e., the entry point of
// S-mode trap vector). added @lab2_1
//
extern char trap_sec_start[];

//
// turn on paging. added @lab2_1
//
void enable_paging()
{
  // write the pointer to kernel page (table) directory into the CSR of "satp".
  write_csr(satp, MAKE_SATP(g_kernel_pagetable));

  // refresh tlb to invalidate its content.
  flush_tlb();
}

typedef union
{
  uint64 buf[MAX_CMDLINE_ARGS];
  char *argv[MAX_CMDLINE_ARGS];
} arg_buf;

//
// returns the number (should be 1) of string(s) after PKE kernel in command line.
// and store the string(s) in arg_bug_msg.
//
static size_t parse_args(arg_buf *arg_bug_msg)
{
  // HTIFSYS_getmainvars frontend call reads command arguments to (input) *arg_bug_msg
  long r = frontend_syscall(HTIFSYS_getmainvars, (uint64)arg_bug_msg,
                            sizeof(*arg_bug_msg), 0, 0, 0, 0, 0);
  kassert(r == 0);

  size_t pk_argc = arg_bug_msg->buf[0];
  uint64 *pk_argv = &arg_bug_msg->buf[1];

  int arg = 1; // skip the PKE OS kernel string, leave behind only the application name
  for (size_t i = 0; arg + i < pk_argc; i++)
    arg_bug_msg->argv[i] = (char *)(uintptr_t)pk_argv[arg + i];

  // returns the number of strings after PKE kernel in command line
  return pk_argc - arg;
}

//
// load the elf, and construct a "process" (with only a trapframe).
// load_bincode_from_host_elf is defined in elf.c
//
int load_exec_program(char *command, char *param)
{
  reset_process(current[read_tp()]);
  load_bincode_from_host_elf(current[read_tp()], command);

  size_t *vsp = (size_t *)current[read_tp()]->trapframe->regs.sp;
  vsp -= 8;
  size_t *sp = (size_t *)user_va_to_pa(current[read_tp()]->pagetable, (void *)vsp);
  memcpy((char *)sp, param, 1 + strlen(param));

  vsp--;
  sp--;
  *sp = (size_t)(1 + vsp);

  current[read_tp()]->trapframe->regs.sp = (uint64)vsp;
  current[read_tp()]->trapframe->regs.a0 = (uint64)1;
  current[read_tp()]->trapframe->regs.a1 = (uint64)vsp;
  return -1;
}

int parse_cnt = 0;
arg_buf arg_bug_msg;
ssize_t argc = 0;
process *load_ori_program()
{
  process *proc;
  uint64 hart_id = read_tp();

  proc = alloc_process();
  sprint("hartid = %d, User application is loading.\n", hart_id);

  // retrieve command line arguements
  if (parse_cnt == 0)
  {
    parse_cnt = 1;
    argc = parse_args(&arg_bug_msg);
    if (!argc)
      panic("You need to specify the application program!\n");
  }

  load_bincode_from_host_elf(proc, arg_bug_msg.argv[proc->hart_id]);
  return proc;
}

//
// s_start: S-mode entry point of riscv-pke OS kernel.
//
int kernel_page_init_cnt = 0;
int s_start(void)
{
  // sprint("Enter supervisor mode...\n");
  // in the beginning, we use Bare mode (direct) memory mapping as in lab1.
  // but now, we are going to switch to the paging mode @lab2_1.
  // note, the code still works in Bare mode when calling pmm_init() and kern_vm_init().
  uint64 hart_id = read_tp();
  write_csr(satp, 0);
  if (hart_id == 0)
  // init phisical memory manager
  {
    pmm_init();

    // build the kernel page table
    kern_vm_init();

    // now, switch to paging mode by turning on paging (SV39)
    enable_paging();
    // the code now formally works in paging mode, meaning the page table is now in use.
    // sprint("kernel page table is on \n");

    // added @lab3_1
    init_proc_pool();

    // init file system, added @lab4_1
    fs_init();
  }
  sync_barrier(&kernel_page_init_cnt, NCPU);

  // sprint("Switch to user mode...\n");
  // the application code (elf) is first loaded into memory, and then put into execution
  // added @lab3_1
  // sprint("sstart hartid: %d\n", hart_id);
  vm_alloc_stage[hart_id] = 1;
  insert_to_ready_queue(load_ori_program());
  schedule();

  // we should never reach here.
  return 0;
}
