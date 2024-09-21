/*
 * routines that scan and load a (host) Executable and Linkable Format (ELF) file
 * into the (emulated) memory.
 */

#include "elf.h"
#include "string.h"
#include "riscv.h"
#include "vmm.h"
#include "pmm.h"
#include "vfs.h"
#include "spike_interface/spike_utils.h"

typedef struct elf_info_t {
  struct file *f;
  process *p;
} elf_info;

//
// the implementation of allocater. allocates memory space for later segment loading.
// this allocater is heavily modified @lab2_1, where we do NOT work in bare mode.
//
static void *elf_alloc_mb(elf_ctx *ctx, uint64 elf_pa, uint64 elf_va, uint64 size) {
  elf_info *msg = (elf_info *)ctx->info;
  // we assume that size of proram segment is smaller than a page.
  kassert(size < PGSIZE);
  void *pa = alloc_page();
  if (pa == 0) panic("uvmalloc mem alloc falied\n");

  memset((void *)pa, 0, PGSIZE);
  // sprint("hartid: %d, va: %lx, pa: %lx\n", read_tp(), elf_va, pa);
  user_vm_map((pagetable_t)msg->p->pagetable, elf_va, PGSIZE, (uint64)pa,
         prot_to_type(PROT_WRITE | PROT_READ | PROT_EXEC, 1));

  return pa;
}

//
// actual file reading, using the vfs file interface.
//
static uint64 elf_fpread(elf_ctx *ctx, void *dest, uint64 nb, uint64 offset) {
  elf_info *msg = (elf_info *)ctx->info;
  vfs_lseek(msg->f, offset, SEEK_SET);
  return vfs_read(msg->f, dest, nb);
}

//
// init elf_ctx, a data structure that loads the elf.
//
elf_status elf_init(elf_ctx *ctx, void *info) {
  ctx->info = info;

  // load the elf header
  if (elf_fpread(ctx, &ctx->ehdr, sizeof(ctx->ehdr), 0) != sizeof(ctx->ehdr))
  	return EL_EIO;

  // check the signature (magic value) of the elf
  if (ctx->ehdr.magic != ELF_MAGIC)
  	return EL_NOTELF;

  return EL_OK;
}

void read_u128(uint64 *out, char **off)
{
  uint64 val = 0;
  int shift = 0;
  uint8 o;
  while (1)
  {
    o = *(uint8 *)(*off);
    (*off)++;
    val |= ((uint64)o & 0x7F) << shift;
    shift += 7;
    if ((o & 0x80) == 0)
      break;
  }
  if (out)
    *out = val;
}

void read_s128(int64 *out, char **off)
{
  int64 val = 0;
  int shift = 0;
  uint8 o;
  while (1)
  {
    o = *(uint8 *)(*off);
    (*off)++;
    val |= ((uint64_t)o & 0x7F) << shift;
    shift += 7;
    if ((o & 0x80) == 0)
      break;
  }
  if (shift < 64 && (o & 0x40))
    val |= -(1 << shift);
  if (out)
    *out = val;
}

void read_u64(uint64 *out, char **off)
{
  *out = 0;
  int max = 8;
  for (int i = 0; i < max; i++)
  {
    *out |= (uint64)(**off) << (i << 3);
    (*off)++;
  }
}

void read_u16(uint16 *out, char **off)
{
  *out = 0;
  int max = 2;
  for (int i = 0; i < max; i++)
  {
    *out |= (uint16)(**off) << (i << 3);
    (*off)++;
  }
}

//
// load the elf segments to memory regions.
//
elf_status elf_load(elf_ctx *ctx) {
  // elf_prog_header structure is defined in kernel/elf.h
  elf_prog_header ph_addr;
  elf_sect_header section_header_one, section_header_two, sh_addr;
  int string_length = 0, i, off;
  char segment_name[SEG_NAME_MAX_LEN];

  // traverse the elf program segment headers
  for (i = 0, off = ctx->ehdr.phoff; i < ctx->ehdr.phnum; i++, off += sizeof(ph_addr)) {
    // read segment headers
    if (elf_fpread(ctx, (void *)&ph_addr, sizeof(ph_addr), off) != sizeof(ph_addr))
      return EL_EIO;

    if (ph_addr.type != ELF_PROG_LOAD)
      continue;
    if (ph_addr.memsz < ph_addr.filesz)
      return EL_ERR;
    if (ph_addr.vaddr + ph_addr.memsz < ph_addr.vaddr)
      return EL_ERR;

    // allocate memory block before elf loading
    void *dest = elf_alloc_mb(ctx, ph_addr.vaddr, ph_addr.vaddr, ph_addr.memsz);

    // actual loading
    if (elf_fpread(ctx, dest, ph_addr.memsz, ph_addr.off) != ph_addr.memsz)
      return EL_EIO;

    // record the vm region in proc->mapped_info. added @lab3_1
    int j;
    for( j=0; j<PGSIZE/sizeof(mapped_region); j++ ) //seek the last mapped region
      if( (process*)(((elf_info*)(ctx->info))->p)->mapped_info[j].va == 0x0 )
        break;

    ((process*)(((elf_info*)(ctx->info))->p))->mapped_info[j].va = ph_addr.vaddr;
    ((process*)(((elf_info*)(ctx->info))->p))->mapped_info[j].npages = 1;

    // SEGMENT_READABLE, SEGMENT_EXECUTABLE, SEGMENT_WRITABLE are defined in kernel/elf.h
    if( ph_addr.flags == (SEGMENT_READABLE|SEGMENT_EXECUTABLE) ){
      ((process*)(((elf_info*)(ctx->info))->p))->mapped_info[j].seg_type = CODE_SEGMENT;
      // sprint( "CODE_SEGMENT added at mapped info offset:%d\n", j );
    }else if ( ph_addr.flags == (SEGMENT_READABLE|SEGMENT_WRITABLE) ){
      ((process*)(((elf_info*)(ctx->info))->p))->mapped_info[j].seg_type = DATA_SEGMENT;
      // sprint( "DATA_SEGMENT added at mapped info offset:%d\n", j );
    }else
      panic( "unknown program segment encountered, segment flag:%d.\n", ph_addr.flags );

    ((process*)(((elf_info*)(ctx->info))->p))->total_mapped_region ++;
  }

  if (elf_fpread(ctx, (void *)&section_header_one, sizeof(section_header_one),
                 ctx->ehdr.shoff + ctx->ehdr.shstrndx * sizeof(section_header_one)) != sizeof(section_header_one))
    return EL_EIO;

  for (i = 0, off = ctx->ehdr.shoff; i < ctx->ehdr.shnum; i++, off += sizeof(section_header_two))
  {
    if (elf_fpread(ctx, (void *)&section_header_two, sizeof(section_header_two), off) != sizeof(section_header_two))
      return EL_EIO;
    elf_fpread(ctx, (void *)segment_name, 20, section_header_one.offset + section_header_two.name);
  }

  for (i = 0, off = ctx->ehdr.shoff; i < ctx->ehdr.shnum; i++, off += sizeof(sh_addr))
  {
    if (elf_fpread(ctx, (void *)&sh_addr, sizeof(sh_addr), off) != sizeof(sh_addr))
      return EL_EIO;

    if (sh_addr.type == SYM_TABLE)
    {
      if (elf_fpread(ctx, &((process*)(((elf_info*)(ctx->info))->p))->symbols, sh_addr.size, sh_addr.offset) != sh_addr.size)
        return EL_EIO;
      ((process*)(((elf_info*)(ctx->info))->p))->symbol_num = sh_addr.size / 24;
    }
    else if (sh_addr.type == STR_TABLE)
    {
      if (elf_fpread(ctx, &((process*)(((elf_info*)(ctx->info))->p))->string_table + string_length, sh_addr.size, sh_addr.offset) != sh_addr.size)
        return EL_EIO;
      string_length += sh_addr.size;
    }
  }

  return EL_OK;
}

//
// load the elf of user application, by using the spike file interface.
//
void load_bincode_from_host_elf(process *p, char *prog_name) {
  sprint("hartid = %d, Application: %s\n", p->hart_id, prog_name);

  //elf loading. elf_ctx is defined in kernel/elf.h, used to track the loading process.
  elf_ctx elfloader;
  // elf_info is defined above, used to tie the elf file and its corresponding process.
  elf_info info;

  info.f = vfs_open(prog_name, O_RDONLY);
  info.p = p;

  // IS_ERR_VALUE is a macro defined in spike_interface/spike_htif.h
  if (IS_ERR_VALUE(info.f)) panic("Fail on openning the input application program.\n");

  // init elfloader context. elf_init() is defined above.
  if (elf_init(&elfloader, &info) != EL_OK)
    panic("fail to init elfloader.\n");


  // load elf. elf_load() is defined above.
  if (elf_load(&elfloader) != EL_OK) panic("Fail on loading elf.\n");


  // entry (virtual, also physical in lab1_x) address
  p->trapframe->epc = elfloader.ehdr.entry;

  // close the vfs file
  vfs_close( info.f );

  sprint("hartid = %d, Application program entry point (virtual address): 0x%lx\n", p->hart_id, p->trapframe->epc);
}
