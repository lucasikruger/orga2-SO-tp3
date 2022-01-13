/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

//typedef uint32_t paddr_t;
//typedef uint32_t vaddr_t;

// 
#define PAGE_SIZE 4096

#define MMU_FLAG_PRESENT 1
#define MMU_FLAG_READWRITE 1
#define MMU_FLAG_SUPERVISOR 0

uint32_t proxima_pagina_libre;
uint32_t proxima_pagina_libre_user;

void mmu_init(void) {

  proxima_pagina_libre = 0x100000;
  proxima_pagina_libre_user = 0x400000;
}

paddr_t mmu_next_free_kernel_page(void) {
  uint32_t new_page = proxima_pagina_libre;
  proxima_pagina_libre += PAGE_SIZE;
  return new_page;
}

paddr_t mmu_next_free_user_page(void) {
  uint32_t new_page = proxima_pagina_libre_user;
  proxima_pagina_libre_user += PAGE_SIZE;
  return new_page;
}


paddr_t mmu_init_kernel_dir(void) {
  page_directory_entry *pd = (page_directory_entry *) KERNEL_PAGE_DIR;
	page_table_entry *pt_0 = (page_table_entry *)KERNEL_PAGE_TABLE_0;

	//inicializamos en 0
	for (int i = 0 ; i<1024 ; i++){
    pd[i] = (page_directory_entry){0};
    pt_0[i] = (page_table_entry){0};
	}
	pd[0].present = MMU_FLAG_PRESENT;
	pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
	pd[0].read_write = MMU_FLAG_READWRITE;
	pd[0].page_table_base = ((uint32_t) pt_0>>12);
 

	for (int i = 0 ; i<1024  ; i++){
		pt_0[i].present = MMU_FLAG_PRESENT;
		pt_0[i].user_supervisor = MMU_FLAG_SUPERVISOR;
		pt_0[i].read_write = MMU_FLAG_READWRITE;
		pt_0[i].physical_address_base = i;
	}
	return (uint32_t) pd;
}


paddr_t mmu_init_task_dir(paddr_t phy_start){
  	page_directory_entry* new_directory = (page_directory_entry*) mmu_next_free_kernel_page();
	// paddr_t a = phy_start;
	// a +=1;
	// for(int i = 0; i < 1024; ++i) {
	// 	new_directory[i] = (page_directory_entry){0};
	// }

	for(int add = 0; add < 0x400000; add += PAGE_SIZE) {
		mmu_map_page((uint32_t)new_directory,add,add,3);
	}


	//paddr_t cr3 = rcr3();

	mmu_map_page((uint32_t)new_directory,  0x08000000, phy_start, 5); //lectura

	mmu_map_page((uint32_t)new_directory, 0x08001000, phy_start + PAGE_SIZE, 5);
	
	mmu_map_page((uint32_t)new_directory, 0x08002000, mmu_next_free_user_page(), 7);  //pila
	return (uint32_t) new_directory;
}


void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs) {
	page_directory_entry *directory = (page_directory_entry*) (cr3 & 0xFFFFF000); //direccion donde se encuentra el directorio
	uint32_t directory_index = virt >> 22; //el indice del directorio son los 10 bits mas significativos de la dir virtual
	uint32_t table_index = (virt >> 12) & 0x3FF; //el indice de la tabla son los bits 12 a 21
	page_table_entry *table;
	
	if(directory[directory_index].present == 0) {
		

		//directory[directory_index].page_table_base = (mmu_next_free_kernel_page() >> 12); //setea la tabla
		page_table_entry* table = (page_table_entry*)mmu_next_free_kernel_page(); //(directory[directory_index].page_table_base << 12);
		for (int i = 0 ; i<1024 ; i++){
			table[i] = (page_table_entry){0};
		}

		directory[directory_index].page_table_base = (uint32_t)table>>12;
	}

	directory[directory_index].present |= 0x1;
	directory[directory_index].read_write |= ((attrs>>1) & 1); //MMU_FLAG_READWRITE;
	directory[directory_index].user_supervisor |= ((attrs>>2) & 1);

	table = (page_table_entry*) (directory[directory_index].page_table_base << 12);
	
 	//hay que setear los campos de la entrada de la tabla
	table[table_index].present |= MMU_FLAG_PRESENT;
	table[table_index].read_write |= ((attrs>>1) & 1);
	table[table_index].user_supervisor |= ((attrs>>2) & 1);
	table[table_index].page_write_through = 0;
	table[table_index].page_cache_disable = 0;
	table[table_index].accessed = 0;
	table[table_index].dirty = 0;
	table[table_index].x = 0;
	table[table_index].global = 0;
	table[table_index].available = 0;
	table[table_index].physical_address_base = phy >> 12; //la tabla tiene que tener los 20 bits mas significativos de la dir de la pagina

	tlbflush();
}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt) {
	
	page_directory_entry *directory = (page_directory_entry*) (cr3 & 0xFFFFF000);
    uint32_t directory_index = virt >> 22;

  	if (directory[directory_index].present == 1){
	  	uint32_t table_index = (virt >> 12) & 0x3FF;
  		page_table_entry *PT = (page_table_entry*) (directory[directory_index].page_table_base<<12);
		PT[table_index].present = 0;

		tlbflush();		  
	}
	return 0;
}

