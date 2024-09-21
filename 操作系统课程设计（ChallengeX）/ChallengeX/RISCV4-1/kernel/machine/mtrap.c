#include "kernel/riscv.h"
#include "kernel/process.h"
#include "kernel/vmm.h"
#include "spike_interface/spike_utils.h"
#include "util/string.h"

static void handle_instruction_access_fault() { panic("Instruction access fault!"); }

static void handle_load_access_fault() { panic("Load access fault!"); }

static void handle_store_access_fault() { panic("Store/AMO access fault!"); }

static void handle_illegal_instruction() { panic("Illegal instruction!"); }

static void handle_misaligned_load() { panic("Misaligned Load!"); }

static void handle_misaligned_store() { panic("Misaligned AMO!"); }

// added @lab1_3
static void handle_timer() {
  int cpuid = 0;
  // setup the timer fired at next time (TIMER_INTERVAL from now)
  *(uint64*)CLINT_MTIMECMP(cpuid) = *(uint64*)CLINT_MTIMECMP(cpuid) + TIMER_INTERVAL;

  // setup a soft interrupt in sip (S-mode Interrupt Pending) to be handled in S-mode
  write_csr(sip, SIP_SSIP);
}

struct stat errorstat;
char errpath[8192], errcode[32768];

static void print_errorline()
{
  int i = 0, j = 0, cnt = 0, code_ind;
  uint64 mepc = read_csr(mepc);
  for (i = 0; i < current[read_tp()]->line_ind; i++)
  {
    if (mepc < current[read_tp()]->line[i].addr)
    {
      addr_line *errorline = current[read_tp()]->line + i - 1;
      strcpy(errpath, current[read_tp()]->dir[current[read_tp()]->file[errorline->file].dir]);
      strcat(errpath, "/");
      strcat(errpath, current[read_tp()]->file[errorline->file].file);
      
      spike_file_t *file = spike_file_open(errpath, O_RDONLY, 0);
      spike_file_stat(file, &errorstat);
      spike_file_read(file, errcode, errorstat.st_size);
      
      while (j < errorstat.st_size)
      {
        for (code_ind = j; code_ind < errorstat.st_size && errcode[code_ind] != '\n'; code_ind++);
        if (cnt == errorline->line - 1)
        {
          errcode[code_ind] = '\0';
          sprint("Runtime error at %s: %d\n", errpath, errorline->line);
          sprint("%s\n", errcode + j);
          break;
        }
        else
        {
          j = code_ind + 1;
          cnt++;
        }
      }
      spike_file_close(file);
      break;
    }
  }
}

//
// handle_mtrap calls a handling function according to the type of a machine mode interrupt (trap).
//
void handle_mtrap() {
  uint64 mcause = read_csr(mcause);
  switch (mcause) {
    case CAUSE_MTIMER:
      handle_timer();
      break;
    case CAUSE_FETCH_ACCESS:
      print_errorline();
      handle_instruction_access_fault();
      break;
    case CAUSE_LOAD_ACCESS:
      print_errorline();
      handle_load_access_fault();
    case CAUSE_STORE_ACCESS:
      print_errorline();
      handle_store_access_fault();
      break;
    case CAUSE_ILLEGAL_INSTRUCTION:
      // TODO (lab1_2): call handle_illegal_instruction to implement illegal instruction
      // interception, and finish lab1_2.
      print_errorline();
      handle_illegal_instruction();
      break;
    case CAUSE_MISALIGNED_LOAD:
      print_errorline();
      handle_misaligned_load();
      break;
    case CAUSE_MISALIGNED_STORE:
      print_errorline();
      handle_misaligned_store();
      break;

    default:
      // sprint("machine trap(): unexpected mscause %p\n", mcause);
      // sprint("            mepc=%p mtval=%p\n", read_csr(mepc), read_csr(mtval));
      panic( "unexpected exception happened in M-mode.\n" );
      break;
  }
}
