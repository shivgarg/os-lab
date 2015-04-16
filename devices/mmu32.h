#pragma once
#include "util/util.h"
#include <algorithm>
//#include <utility>

class dev_pde32_t{
  public:
    enum { PAGE_SIZE = 4<<20 } ;
    enum { N=1024};
    uint32_t m_page[N] ALIGN(0x1000);
  public:
    dev_pde32_t(){
      hoh_assert(uintptr_t(m_page)%0x1000==0,"Alignment issue");
      for(int i=0;i<N;i++){
        unset(i);
      }
    }
  public:
    void reset(){
      asm volatile("movl %0,%%cr3\n"::"r"(m_page):"memory");
    }
  public:
    void map_identity(){
      map_large(0,0, 0x83, 1u << (32-22));
    }

    void map_large(addr_t va, addr_t pa, uint16_t flags, size_t num_pages){
      for(size_t i=0 ; i< num_pages; i++){
        hoh_assert((get_bits<21,0>(uintptr_t(va+i*PAGE_SIZE))==0),"XXX: " << va << ","<<i);
        map(va + i* PAGE_SIZE, pa + i* PAGE_SIZE, flags);
      }
    }

    void map(addr_t va, addr_t pa, uint16_t flags){
      hoh_assert((get_bits<21,0>(uintptr_t(va))==0),"XXX");
      set(get_bits<31,22>((uintptr_t)va), (uintptr_t)pa, flags);
    }

    void set(int i, uint32_t addr,uint16_t flags){
      hoh_assert((get_bits<11,0>(addr)==0),"XXX");
      hoh_assert(i>=0 && i<N,"XXX");
      m_page[i]=(get_bits<31,12>(addr)<<12) | flags;
    }
    void unset(int i){
      hoh_assert(i>=0 && i<N,"XXX");
      set(i,0,2);
    }
    uint32_t get(int i)const{
      hoh_assert(i>=0 && i<N,"XXX");
      return m_page[i];
    }
    uint32_t get_flags(int i)const{
      return get_bits<11,0>(get(i));
    }
    void swap(int i, int j){
      hoh_assert(i>=0 && i<N,"XXX");
      std::swap(m_page[i],m_page[j]);
    }
    uint32_t get_addr(int i)const{
      hoh_assert(i>=0 && i<N,"XXX");
      return get_bits<31,12>(get(i));
    }

};

