#include<sys/init_pd.h>
#include<sys/kprintf.h>

//Citation  - https://stackoverflow.com/questions/10493411/what-is-bit-masking
//Citation - http://altdevblog.com/2011/07/24/memory-address-translation/
//Citation - http://wiki.osdev.org/Page_Tables

int count;

uint64_t free_list_start;
uint64_t first_page_address;

void* my_memset(void *p, int j, int length)
{
	unsigned char *vp = p;
	for(size_t i = 0; i < length; i++)
	{
		*(vp + i) = (uint8_t)j;
	}
	return p;
}

void init_freelist(uint64_t physfree_address, uint64_t physend_address)
{
	paging_enabled = 0;
	//head_free_list = physfree_address;
	//head_free_list = (freelistnode)(long)physfree_address;

	//kprintf("physfree in func = %x\n", physfree_address);
	uint64_t pagesize = 1<<12;

	uint64_t avail_phys_mem = physend_address - physfree_address;
	//kprintf("Available Physical Memory for us= %x\n", avail_phys_mem );
	uint64_t no_pages = avail_phys_mem>>12; //dividing into 4Kb pages
	//kprintf("Number of Pages = %d\n", no_pages);

	uint64_t size_of_free_list = no_pages - (avail_phys_mem/((uint64_t)sizeof(struct FreeListNode) + pagesize));
	//kprintf("Size of free list node in terms of pages = %d\n", size_of_free_list);

	my_memset((void*)(physfree_address + (size_of_free_list+1)*pagesize), 0, physend_address - (physfree_address + (size_of_free_list+1)*pagesize));
	head_free_list = (freelistnode)physfree_address;
	head_free_list->address = physfree_address + (size_of_free_list+1)*pagesize;
	head_free_list->next = NULL;
	head_free_list->page_usage_count = 1;
	//head_free_list->is_free = 1;
	//kprintf("Address of first free page = %p\n", head_free_list->address);

	freelistnode ptr = head_free_list;
	free_list_start = physfree_address;
	first_page_address = physfree_address + (size_of_free_list+1)*pagesize;
	

	//uint64_t temp = head_free_list->address;

	int i = 1;
	freelistnode tempptr = NULL;
	while((ptr->address + pagesize) <= physend_address)
	{
		tempptr = (freelistnode)((uint64_t)ptr + sizeof(struct FreeListNode));
		//freelistnode tempptr = (freelistnode)(ptr + );
		/*if(i <= 5)
		{
			kprintf("valueof ptr = %p      ", ptr);
			kprintf("valueof tempptr = %p\n", ptr);
		}*/
		tempptr->address = ptr->address + pagesize;
		tempptr->next = NULL;
		tempptr->page_usage_count = 1;
		//tempptr->is_free = 1;
		if(tempptr->address > physend_address)
			break;
		ptr->next = tempptr;
		ptr = tempptr;
		i++;
		//kprintf("Address of free list node = %x\n", tempptr);		
	}
	end_free_list = tempptr;
	//kprintf("Address stored in first freelist node = %x\n", first_page_address);
	//kprintf("size of node = %d\n", sizeof(struct FreeListNode));
	//kprintf("List Creation Done. I = %d\n", i);
	//kprintf("head_free_list->address = %x\n", head_free_list->address);
	/*int *a = (int*)head_free_list->address;
	*a = 15;
	a = (int*)(head_free_list->address + pagesize);
	*a = 20;
	a = (int*)(head_free_list->address + 2*pagesize);
	*a = 25;*/

	/*i = 0;
	ptr = head_free_list;
	while(ptr!=NULL)
	{
		ptr = ptr->next;
		i++;
	}
	kprintf("List Iteration Done. I = %d\n", i);*/
	//head_free_list = (freelistnode)(physfree_address + (size_of_free_list + size_of_free_list + 1));

	/*head_free_list = (freelistnode)physfree_address;
  	node->
  	  = 10;
  	node->next = node;
  	kprintf("Size of Node = %d\n", sizeof(*node));
  	kprintf("Value of Index = %d\n", node->index);
  	kprintf("Address of Index = %x\n", &(node->index));
 	kprintf("Address of next = %x\n", &(node->next));*/

}

freelistnode get_free_page()
{
	freelistnode temp; 
	if(!paging_enabled)
	{
		temp = head_free_list;
		//kprintf("In get_free_page. %p", head_free_list);
		my_memset((void *)temp->address, 0, 4*1024);
		head_free_list = (freelistnode)head_free_list->next;
	}
	else
	{
		temp = head_free_list;
		//kprintf("In get_free_page. %p", head_free_list);
		//Traverse_PML4(temp->address, (uint64_t*)pml4);
		//my_memset((void *)temp->address, 48, 4*1024);
		//temp->address = temp->address + mapping_mask;
		my_memset((void *)get_virtual_address(temp->address, 0), 0, 4*1024);
		head_free_list = (freelistnode)((uint64_t)head_free_list->next + mapping_mask);
	}
	
	return temp;
}

void freepage(uint64_t page_address)
{	
	//kprintf("in freepage\n");
	uint64_t page_num = (page_address - first_page_address)/4096;
	//kprintf("Page number = %x\n", page_num);
	uint64_t node_address = free_list_start + page_num*sizeof(struct FreeListNode);
	if(!paging_enabled)
	{
		freelistnode node = (freelistnode)node_address;
		//kprintf("Node page address = %x\n", node->address);
		//kprintf("page address =%x\n", page_address);
		node->next = NULL;
		node->page_usage_count = 1;
		end_free_list->next = node;
		end_free_list = node;
	}
	else
	{
		freelistnode node = (freelistnode)(node_address + mapping_mask);
		//kprintf("Node page address = %x\n", node->address);
		//kprintf("page address =%x\n", page_address);
		node->next = NULL;
		node->page_usage_count = 1;
		end_free_list->next = node;
		end_free_list = node;

	}		
	//kprintf("")
}

uint64_t get_pml4_index(uint64_t vaddress)
{
	return ((vaddress & pml4_index_mask)>>39); //Bits 47 through 39
}

uint64_t get_pdp_index(uint64_t vaddress)
{
	return ((vaddress & pdp_index_mask)>>30); //Bits 38 through 30
}

uint64_t get_pd_index(uint64_t vaddress)
{
	return ((vaddress & pd_index_mask)>>21); //Bits 29 through 21
}

uint64_t get_pt_index(uint64_t vaddress)
{
	return ((vaddress & pt_index_mask)>>12); //Bits 20 through 12
}

uint64_t get_virtual_address(uint64_t paddress, int mapping_flag)
{
	uint64_t vaddress;
	if(mapping_flag == 0)
	{
		vaddress = mapping_mask + paddress; //Getting Virtual Address from physical address 
	}																		//as mentioned in the linker_script.
	else
	{
		vaddress = identity_mapping_mask + paddress;
	}																		
	return vaddress;
	//return paddress;
}


//Traverse_PML4 -> Traverse_PDP -> Traverse_PD ->Traverse_PT

void Traverse_PML4(uint64_t paddress, uint64_t *PML4_address, int mapping_flag, uint64_t page_entry_flag)
{	
	uint64_t pml4_index = get_pml4_index(get_virtual_address(paddress, mapping_flag));
	if((PML4_address[pml4_index] & present_bit_mask) == 0) //checking the present bit(Bit 0) at this value(page is already memset). If not present we would have to add a pml4 entry for a new pdp
	{
		freelistnode pdp_page_node = get_free_page(); //get page for pdp
		PML4_address[pml4_index] = pdp_page_node->address | page_entry_mask; //address for pdp plus seeting the offset values
	}
	uint64_t *pdp_address;
	if(paging_enabled == 0)
	{
		pdp_address = (uint64_t*)(PML4_address[pml4_index] & offset_mask); //removing the offset values to get the pdp_address.
	}
	else
	{
		PML4_address[pml4_index] = PML4_address[pml4_index] | page_entry_flag;
		
		pdp_address = (uint64_t*)(get_virtual_address(PML4_address[pml4_index], mapping_flag) & offset_mask); //removing the offset values to get the pdp_address.
	}
	Traverse_PDP(paddress, pdp_address, mapping_flag, page_entry_flag);
}

void Traverse_PDP(uint64_t paddress, uint64_t *pdp_address, int mapping_flag, uint64_t page_entry_flag)
{
	uint64_t pdp_index =  get_pdp_index(get_virtual_address(paddress, mapping_flag));
	if((pdp_address[pdp_index] & present_bit_mask) == 0)
	{
		freelistnode pd_page_node = get_free_page();
		pdp_address[pdp_index] = pd_page_node->address | page_entry_mask;
	}
	uint64_t *pd_address;
	if(paging_enabled == 0)
	{
		pd_address = (uint64_t*)(pdp_address[pdp_index] & offset_mask);
	}
	else
	{
		pdp_address[pdp_index] = pdp_address[pdp_index] | page_entry_flag;
		
		pd_address = (uint64_t*)(get_virtual_address(pdp_address[pdp_index], mapping_flag) & offset_mask);
	}
	Traverse_PD(paddress, pd_address, mapping_flag, page_entry_flag);
}

void Traverse_PD(uint64_t paddress, uint64_t *pd_address, int mapping_flag, uint64_t page_entry_flag)
{
	uint64_t pd_index = get_pd_index(get_virtual_address(paddress, mapping_flag));
	if((pd_address[pd_index] & page_entry_mask) == 0)
	{
		freelistnode pt_page_node = get_free_page();
		pd_address[pd_index] = pt_page_node->address | page_entry_mask;
	}
	uint64_t *pt_address;
	if(paging_enabled == 0)
	{
		pt_address = (uint64_t*)(pd_address[pd_index] & offset_mask);
	}
	else
	{ 
		pd_address[pd_index] = pd_address[pd_index] | page_entry_flag;
		
		pt_address = (uint64_t*)(get_virtual_address(pd_address[pd_index], mapping_flag) & offset_mask);
	}
	Traverse_PT(paddress, pt_address, mapping_flag, page_entry_flag);
}

void Traverse_PT(uint64_t paddress, uint64_t *pt_address, int mapping_flag, uint64_t page_entry_flag)
{
	uint64_t pt_index = get_pt_index(get_virtual_address(paddress, mapping_flag));
	//if((pt_address[pt_index] & page_entry_mask) == 0)
	//{
		pt_address[pt_index] = paddress | (page_entry_mask | page_entry_flag);
	//}
}	

void updateCR3(uint64_t pml4)
{
	__asm__ volatile("movq %0, %%cr3;\n"
		:
		:"r"(pml4) //problem with "g"
		);
}

uint64_t init_paging(uint64_t physbase, uint64_t physend)
{
	//uint64_t phys_allocated_end = head_free_list->address - 4*1024;
	//count = 0;
	freelistnode pml4_node = get_free_page();
	uint64_t pml4_address = pml4_node->address;
	for(uint64_t i = physbase; i <=  physend; i = i + 4*1024)
	{
		Traverse_PML4(i, (uint64_t*)pml4_address, 0, 0);
	}
	/*for(uint64_t i = phys_allocated_end; i <=  physend; i = i + 4*1024)
	{
		Traverse_PML4(i, (uint64_t*)pml4_address, 1, 0);
	}*/
	//Traverse_PML4(pml4_address, (uint64_t*)pml4_address);

	/*paging_enabled = 1;	
	kprintf("before = %x\n", head_free_list);
	head_free_list = (freelistnode)(head_free_list + mapping_mask);
	//Traverse_PML4((uint64_t)head_free_list, (uint64_t*)pml4_address);
	kprintf("after = %x\n", head_free_list);*/
	//kprintf("The count is = %d\n", count);
	return pml4_address;
}

void after_paging()
{
	paging_enabled = 1;	
	//kprintf("before = %x\n", head_free_list);
	head_free_list = (freelistnode)((uint64_t)head_free_list + mapping_mask);
	end_free_list = (freelistnode)((uint64_t)end_free_list + mapping_mask);
	//Traverse_PML4((uint64_t)head_free_list, (uint64_t*)pml4_address);
	//kprintf("after = %x\n", head_free_list);
}

void map_page(uint64_t page_address, uint64_t* pml4_address)
{
	for(uint64_t i = page_address; i < (page_address + 4*1024); i++)
	{
		Traverse_PML4(i, pml4_address, 1, 0);
	}
}

void set_page_flags(uint64_t page_address, uint64_t flag_index, uint64_t mapping_mask1)
{
	uint64_t page_entry_flag = (uint64_t)1<<flag_index;
	//kprintf("Value of page_entry_flag = %x\n", page_entry_flag);
	page_address-= mapping_mask1;
	uint64_t pml4;
	__asm__ volatile("movq %%cr3, %0\n"
		:"=g"(pml4)
		:
		);
	if(mapping_mask1 == mapping_mask)
	{
		//kprintf("elif\n");
		Traverse_PML4(page_address, (uint64_t*)get_virtual_address(pml4, 0), 0, page_entry_flag);
	}
	else if(mapping_mask1 == identity_mapping_mask)
	{
		Traverse_PML4(page_address, (uint64_t*)get_virtual_address(pml4, 1), 1, page_entry_flag);
	}
	updateCR3(pml4);
}

uint64_t kmalloc(int size)
{
	//int size = sizeof(struct task_struct);
	freelistnode head = get_free_page();
	set_page_flags(get_virtual_address(head->address, 0) & 0xFFFFFFFFFFFFF000, 2, mapping_mask);
	//kprintf("Head page address %%p\n", head->address);
	//kprintf("kmalloc size = %d\n", size);
	size-= 4096;
	//kprintf("kmalloc size = %d\n", size);
	while(size > 0)
	{
		//freelistnode temp = get_free_page();

		freelistnode temp = get_free_page();
		set_page_flags(get_virtual_address(temp->address, 0) & 0xFFFFFFFFFFFFF000, 2, mapping_mask);
		//kprintf("Page address %%p\n", temp->address);
		size-= 4096;
		//kprintf("hello size = %d\n", size);
	}
	return get_virtual_address(head->address, 0);
}

uint64_t memcopy(uint64_t start, uint64_t end)
{
	uint64_t page= kmalloc((int)(end - start));
	for(uint64_t i = 0; i<=(end - start); i++)
	{
		*(uint64_t*)(page + i) = *(uint64_t*)(start + i);
	}
	return page;
}


uint64_t get_physical_address(uint64_t vaddress, uint64_t* pml4_address)
{
	uint64_t pml4_index = get_pml4_index(vaddress);
	//kprintf("pml4_index = %d\n", pml4_index);
	uint64_t* pdp_address = (uint64_t*)((pml4_address[pml4_index] & 0xFFFFFFFFFFFFF000)+ mapping_mask);
	uint64_t pdp_index = get_pdp_index(vaddress);
	//kprintf("pdp_index = %d\n", pdp_index);
	uint64_t* pd_address = (uint64_t*)((pdp_address[pdp_index] & 0xFFFFFFFFFFFFF000) + mapping_mask);
	uint64_t pd_index = get_pd_index(vaddress);
	//kprintf("pd_index = %d\n", pd_index);
	uint64_t* pt_address = (uint64_t*)((pd_address[pd_index] & 0xFFFFFFFFFFFFF000) + mapping_mask);
	uint64_t pt_index = get_pt_index(vaddress);
	//kprintf("pt_index = %d\n", pt_index);
	//kprintf("Physical Address Retrieval Done!\n");
	return (pt_address[pt_index]);
}