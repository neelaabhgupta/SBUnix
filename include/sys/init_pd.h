#ifndef _INIT_PD_H
#define _INIT_PD_H
#include<sys/defs.h>

//#define index_mask 0x00000000000001FF
#define pml4_index_mask  0x0000FF8000000000
#define pdp_index_mask   0x0000007FC0000000
#define pd_index_mask    0x000000003FE00000
#define pt_index_mask    0x00000000001FF000
#define page_entry_mask  0x0000000000000003
#define mapping_mask     0xFFFFFFFF80000000
#define identity_mapping_mask     0
#define offset_mask      0xFFFFFFFFFFFFF000
#define present_bit_mask 0x0000000000000001

struct FreeListNode
{
	uint64_t address;
	struct FreeListNode *next;
	int is_free;
	int page_usage_count;

};

uint64_t free_list_start;
uint64_t first_page_address;

typedef struct FreeListNode* freelistnode;
//uint64_t 

freelistnode head_free_list;
freelistnode end_free_list;
int paging_enabled;

void init_freelist(uint64_t physfree_address, uint64_t physend);
freelistnode get_free_page();
uint64_t init_paging(uint64_t physbase, uint64_t physend);
void updateCR3(uint64_t pml4);
void* my_memset(void *p, int j, int length);
uint64_t get_pml4_index(uint64_t vaddress);
uint64_t get_pdp_index(uint64_t vaddress);
uint64_t get_pd_index(uint64_t vaddress);
uint64_t get_pt_index(uint64_t vaddress);
void Traverse_PML4(uint64_t paddress, uint64_t *PML4_address, int mapping_flag, uint64_t page_entry_flag);
void Traverse_PDP(uint64_t paddress, uint64_t *pdp_address, int mapping_flag, uint64_t page_entry_flag);
void Traverse_PD(uint64_t paddress, uint64_t *pd_address, int mapping_flag, uint64_t page_entry_flag);
void Traverse_PT(uint64_t paddress, uint64_t *pt_address, int mapping_flag, uint64_t page_entry_flag);
uint64_t get_virtual_address(uint64_t paddress, int mapping_flag);
void set_page_flags(uint64_t page_address, uint64_t flag_index, uint64_t mapping_mask1);
uint64_t kmalloc(int size);
void after_paging();
void map_page(uint64_t page_address, uint64_t* pml4_address);
//extern void loadPageDirectory(uint64_t* address
//extern void enablePaging();
uint64_t memcopy(uint64_t start, uint64_t end);
void freepage(uint64_t page_address);
uint64_t get_physical_address(uint64_t vaddress, uint64_t* pml4_address);

#endif