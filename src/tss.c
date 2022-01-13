/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/

#define CS_RING_0 (8 << 3)
#define CS_RING_3 (9 << 3)
#define DS_RING_0 (10 << 3)
#define DS_RING_3 (11 << 3)

#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"

tss_t tss_initial = {0};
tss_t tss_idle = {0};

tss_t tssLemmingsA[5];
tss_t tssLemmingsB[5];

// tss_t tareasPuebloA[] = {tss_lemmingA_0, tss_lemmingA_1, tss_lemmingA_2, tss_lemmingA_3, tss_lemmingA_4};


// struct tarea

void tss_init(void) {

  gdt[GDT_IDX_TSS_IDLE].base_15_0 = (uint32_t) (&tss_idle);
  gdt[GDT_IDX_TSS_IDLE].base_23_16 = (uint32_t) (&tss_idle) >> 16;
  gdt[GDT_IDX_TSS_IDLE].base_31_24 = (uint32_t) (&tss_idle) >> 24;

  gdt[GDT_IDX_TSS_INIT].base_15_0 = (uint32_t) (&tss_initial);
  gdt[GDT_IDX_TSS_INIT].base_23_16 = (uint32_t) (&tss_initial) >> 16;
  gdt[GDT_IDX_TSS_INIT].base_31_24 = (uint32_t) (&tss_initial) >> 24;



for (uint32_t i = 15; i <= 19; i++ ){
    gdt[i].base_15_0 = (uint32_t) (&tssLemmingsA[i-15]);
    gdt[i].base_23_16 = (uint32_t) (&tssLemmingsA[i-15]) >> 16;
    gdt[i].base_31_24 = (uint32_t) (&tssLemmingsA[i-15]) >> 24;

}

for (uint32_t i = 20; i <= 24; i++ ){
    gdt[i].base_15_0 = (uint32_t) (&tssLemmingsB[i-20]);
    gdt[i].base_23_16 = (uint32_t) (&tssLemmingsB[i-20]) >> 16;
    gdt[i].base_31_24 = (uint32_t) (&tssLemmingsB[i-20]) >> 24;

}

}

void tss_initTaskIdle(void) {
	tss_idle.eip = 0x0001C000;
	tss_idle.cr3 = 0x00025000;
	tss_idle.esp = 0x00025000;
	tss_idle.ebp = 0x00025000;
	tss_idle.iomap = 0x68;
	tss_idle.cs = CS_RING_0;
	tss_idle.ds = DS_RING_0;
	tss_idle.es = DS_RING_0;
	tss_idle.fs = DS_RING_0;
	tss_idle.gs = DS_RING_0;
	tss_idle.ss = DS_RING_0;
	tss_idle.eflags = 0x202; //interrupciones activas
}


void tss_initTask(int pueblo, int indexTSS) {
	
    if (pueblo == 0){ // si es del pueblo A

  tss_t tss = tssLemmingsA[indexTSS-15];
	tss.eip = 0x08000000;
	tss.cr3 = mmu_init_task_dir(0x18000);
	tss.esp =  0x08003000;
	tss.ebp =  0x08003000;

  tss.esp0 =  mmu_next_free_kernel_page() + 0x1000 -1  ;
  tss.ss0 = DS_RING_0;
	tss.cs = CS_RING_3 | 3;
	tss.ds = DS_RING_3 | 3;
	tss.es = DS_RING_3 | 3;
	tss.fs = DS_RING_3 | 3;
	tss.gs = DS_RING_3 | 3;
	tss.ss = DS_RING_3 | 3;
	tss.iomap = 0x68;


	tss.eflags = 0x202; //interrupciones activas
  tssLemmingsA[indexTSS-15] = tss;



    } else{ // si no es del pueblo A => es del pueblo B
  	tss_t tss = tssLemmingsB[indexTSS-20];

  	tss.eip = 0x08000000;
	tss.cr3 = mmu_init_task_dir(0x1A000);
	tss.esp = 0x08003000;
	tss.ebp = 0x08003000;
  	tss.esp0 =  mmu_next_free_kernel_page() + 0x1000 - 1;
 	tss.ss0 = DS_RING_0;
	tss.cs = CS_RING_3 | 3;
	tss.ds = DS_RING_3 | 3;
	tss.es = DS_RING_3 | 3;
	tss.fs = DS_RING_3 | 3;
	tss.gs = DS_RING_3 | 3;
	tss.ss = DS_RING_3 | 3;
	tss.iomap = 0x68;
	tss.eflags = 0x202; //interrupciones activas
  tssLemmingsB[indexTSS-20] = tss;

    }

}