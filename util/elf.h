#pragma once

#include <elf.h>
#include "util/util.h"




static inline bool elf_canread_prog_header(addr_t from, size_t fromsize){
  overflow_uint32_t fromend = overflow_uint32_t(uintptr_t(from)) + overflow_uint32_t(fromsize);
  if(!fromend.valid){
    return false;
  }

  if(sizeof(Elf32_Ehdr) > fromsize){
    return false;
  }

  const Elf32_Ehdr& header = *(Elf32_Ehdr*)(from);

  if(header.e_ident[EI_DATA] != ELFDATA2LSB){//TODO
    return false;
  }

  overflow_uint32_t    prog_offset{header.e_phoff};
  overflow_uint32_t    prog_num{header.e_phnum};
  overflow_uint32_t    prog_end = prog_offset + prog_num * overflow_uint32_t{sizeof(Elf32_Phdr)};
  if(!prog_end.valid){
    return false;
  }

  return true;
}


static inline bool elf_isvalid_prog_header(addr_t from, size_t fromsize){
  if(!elf_canread_prog_header(from,fromsize)){
    return false;
  }
  const Elf32_Ehdr& header      = *(Elf32_Ehdr*)(from);
  uint32_t prog_offset          = header.e_phoff;
  const Elf32_Phdr* prog_header = (Elf32_Phdr*)(from+prog_offset);
  uint32_t    prog_num          = header.e_phnum;


  for(int i=0;i<prog_num; i++){

  }


  return true;

}

static inline void elf_dump_header(const Elf32_Ehdr& header){

  hoh_debug("EI Magic byte 0: " << header.e_ident[EI_MAG0]);
  hoh_debug("EI Magic byte 1: " << header.e_ident[EI_MAG1]);
  hoh_debug("EI Magic byte 2: " << header.e_ident[EI_MAG2]);
  hoh_debug("EI Magic byte 3: " << header.e_ident[EI_MAG3]);
  hoh_debug("EI Class:        " << header.e_ident[EI_CLASS]);
  hoh_debug("EI Data:         " << header.e_ident[EI_DATA]);
  hoh_debug("EI Version:      " << header.e_ident[EI_VERSION]);
  hoh_debug("EI OS/ABI:       " << header.e_ident[EI_OSABI]);
  hoh_debug("EI ABI Version:  " << header.e_ident[EI_ABIVERSION]);
  hoh_debug("EI Padding:      " << header.e_ident[EI_PAD]);


  hoh_debug("ELF Type:        " << header.e_type);
  hoh_debug("ELF Machine:     " << header.e_machine);
  hoh_debug("ELF Version:     " << header.e_version);
  hoh_debug("ELF Entry:       " << header.e_entry);
  hoh_debug("ELF Phdr start:  " << header.e_phoff);
  hoh_debug("ELF Shdr start:  " << header.e_shoff);
  hoh_debug("ELF Flags:       " << header.e_flags);
  hoh_debug("ELF Ehdr size:   " << header.e_ehsize);
  hoh_debug("ELF Phdr size:   " << header.e_phentsize);
  hoh_debug("ELF Phdr num:    " << header.e_phnum);
  hoh_debug("ELF Shdr size:   " << header.e_shentsize);
  hoh_debug("ELF Shdr num:    " << header.e_shnum);
  hoh_debug("ELF ShdrStr Idx: " << header.e_shstrndx);

}


static inline uint32_t elf_get_start(const Elf32_Ehdr& header){
  return header.e_entry;
}




static inline void elf_dump_prog_header(const Elf32_Phdr* prog_header, uint32_t prog_num){

  for(int i=0;i<prog_num; i++){
    hoh_debug("PHdr:          " << i);
    hoh_debug("Phdr Type:     " << prog_header[i].p_type);
    hoh_debug("Phdr Offset:   " << prog_header[i].p_offset);
    hoh_debug("Phdr VirtAddr: " << prog_header[i].p_vaddr);
    hoh_debug("Phdr PhysAddr: " << prog_header[i].p_paddr);
    hoh_debug("Phdr FileSize: " << prog_header[i].p_filesz);
    hoh_debug("Phdr MemSize:  " << prog_header[i].p_memsz);
    hoh_debug("Phdr Flags:    " << prog_header[i].p_flags);
    hoh_debug("Phdr Align:    " << prog_header[i].p_align);
  }

}



static inline void elf_dump(addr_t pelf, size_t elf_size){

  hoh_assert(elf_canread_prog_header(pelf,elf_size),"XXX");

  const Elf32_Ehdr& header      = *(Elf32_Ehdr*)(pelf);

  elf_dump_header(header);

  uint32_t prog_offset          = header.e_phoff;
  const Elf32_Phdr* prog_header = (Elf32_Phdr*)(pelf+prog_offset);
  uint32_t    prog_num          = header.e_phnum;

  elf_dump_prog_header(prog_header, prog_num);

}


