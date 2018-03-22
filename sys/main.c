#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include<sys/init_pd.h>
#include<sys/process_scheduling.h>
#include<sys/pic_interuppt.h>
#include<sys/pit_timer.h>
#include<sys/iofunc.h>
#include<sys/init_idt.h>


#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{

  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  //int i =0;
  uint64_t max_memory_length = 0;
  uint64_t physend = 0;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
      //i++;
     //kprintf("iteration i = %d\n", i);
      //kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
      if(smap->length > max_memory_length)
      {
        max_memory_length = (uint64_t)smap->length;
        physend = (uint64_t)(smap->base + smap->length);
      }
    }
  }
  //kprintf("physfree %p\n", (uint64_t)physfree);
  //kprintf("physbase %p\n", (uint64_t)physbase);
  //kprintf("physend %p\n", (uint64_t)physend);
  //kprintf("hello");
  //kprintf("Size of struct freelistnode = %d\n", sizeof(struct FreeListNode));
  /*kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
  uint64_t physend= 0x60C8000;
  init_freelist((uint64_t)physfree, physend);
  int *a;
  freelistnode node = get_free_page();
  a = (int*)node->address;
  kprintf("Value from page 1 = %d\n", *a);
  kprintf("Address of Page 1 = %p\n", node->address);
  node = get_free_page();
  a = (int*)node->address;
  kprintf("Value from page 2 = %d\n", *a);
  kprintf("Address of Page 2 = %p\n", node->address);
  node = get_free_page();
  a = (int*)node->address;
  kprintf("Value from page 3 = %d\n", *a);
  kprintf("Address of Page 3 = %p\n", node->address);
  node = get_free_page();
  a = (int*)node->address;
  kprintf("Value from page 4 = %d\n", *a);
  kprintf("Address of Page 4 = %p\n", node->address);*/
  //uint64_t pml4p1 = init_paging();
  //kprintf("Address of pml4 = %p\n", pml4p1);
  //__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4p1));
  //loadPageDirectory((uint64_t*)pml4p1);j
  //enablePaging();
  //kprintf("Paging complete", pml4p1);
  
  //physend = (uint64_t)physfree + 3*4096;
  init_freelist((uint64_t)physfree, physend);
  #if 0
  freelistnode node = get_free_page();
  node = get_free_page();
  node = get_free_page();
  node = get_free_page();
  //freepage(node->address);
  
  //kprintf("address of head_free_list starting = %p\n", head_free_list);
  //kprintf("address of page in first node starting= %x\n", head_free_list->address);
  node = get_free_page();
  #endif
  //kprintf("node_addr = %x\n", node->address);
  uint64_t pml4 = init_paging((uint64_t)physbase, physend);
  //pml4 = pml4;
  //kprintf("Address of pml4 = %x\n", pml4);
  Traverse_PML4(0xB8000, (uint64_t*)pml4, 0, 0); //mapping video memory to this as well
  //Traverse_PML4((uint64_t)head_free_list, (uint64_t*)pml4);
  after_paging();
  
  updateCR3(pml4);
  


  //paging_enabled = 1;
  //head_free_list = (freelistnode)(head_free_list + mapping_mask);
  //Traverse_PML4((uint64_t)head_free_list, (uint64_t*)pml4);

  //kprintf("address of head_free_list = %p\n", head_free_list);
  //kprintf("address of page in first node = %x\n", head_free_list->address);
  
  //node = get_free_page();
  //freepage(node->address);
  //kprintf("address of node = %p\n", node);
  //kprintf("Node address ....##... = %p\n", node->address);
  //uint64_t *addr = (uint64_t*)get_virtual_address(node->address, 0);
  //*(addr + 1024) = 64;
  //kprintf("Value = %d\n", *(addr + 1024));
  //kprintf("PML4address = %x\n", get_virtual_address(pml4, 0));
  //kprintf("PML4value = %x\n", *((uint64_t*)get_virtual_address(pml4, 0)));
  //Traverse_PML4(node->address, (uint64_t*)(get_virtual_address(pml4, 0)), 0, 0);
  //set_page_flags(get_virtual_address(pml4, 0), 2, mapping_mask);
  //init_task();
  //int s = 12;
  //kprintf("s = %d\n", s);

  init_gdt();
  load_idt_table();
  init_pic();
  init_pit();
  __asm__ volatile ("sti" 
    );


 /* taskptr a = init_task();
  taskptr b = init_task();
  taskptr c = init_task();
  taskptr d = init_task();
  init_process_count();
  init_process_list(a);
  Add_task_to_list(b);
  Add_task_to_list(c);
  Add_task_to_list(d);
  Traverse_Process_list();*/
  //Add_pr

  LoadFirstProcess();
  //switch_ring3();
  //get_physical_address(get_virtual_address(node->address, 0), (uint64_t*)get_virtual_address(pml4, 0));
  
  //kprintf("physbase %p\n", (uint64_t)physbase);
  //switch_ring3();
  /*init_gdt();
  load_idt_table();
  init_pic();
  init_pit();
  __asm__ volatile ("sti" 
    );*/
  #if 0
  uint64_t i = 0;
  while(i < 0x0000000ffffffff/6) i++;
  //tarfs_init();

  switch_ring3();
  #endif
  
  
  /*node = get_free_page();
  set_page_flags(node->address, 2);*/
  /*taskptr a = init_task(func_a);
  kprintf("A = %x\n", a->rsp);*/
  //map_page(node->address, (uint64_t*)pml4);
  //my_memset((void *)(node->address + mapping_mask), 48, 4*1024);
  //Traverse_PML4((uint64_t)&node, (uint64_t*)pml4);
  //node = (freelistnode)get_virtual_address((uint64_t)&node);
  //Traverse_PML4(node->address, (uint64_t*)pml4);
  //uint64_t address = node->address; 
  /*kprintf("node address = %p\n", &node);
  kprintf("hello\n");
  kprintf("Kernmem = %c\n", kernmem);
  kprintf("yoohoo\n");
  kprintf("ADDress pointed by the head free list = %p\n", head_free_list);
  kprintf("physfree %p\n", (uint64_t)physfree);
  kprintf("physbase %p\n", (uint64_t)physbase);*/
  
}


void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  //register char *temp1, *temp2;

  //for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  init_gdt();
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
  /*for(
    temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;*/
  while(1) __asm__ volatile ("hlt");
}
