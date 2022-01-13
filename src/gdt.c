/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"

gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */

    
    [GDT_IDX_NULL_DESC] =
        {
            .limit_15_0 = 0x0000,
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .limit_19_16 = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            .base_31_24 = 0x00,
        },


    /* Segmento de Codigo de nivel 0*/
    [GDT_IDX_CS_RING_0] =
        {
            .limit_19_16 = 0x03,
            .limit_15_0 = 0x30FF,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0xA,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        },

      /* Segmento de Codigo de nivel 3*/
      [GDT_IDX_CS_RING_3] =
        {
            .limit_19_16 = 0x03,
            .limit_15_0 = 0x30FF,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0xA,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        },

        /* Segmento de Datos de nivel 0*/
        [GDT_IDX_DS_RING_0] =
        {
            .limit_19_16 = 0x03,
            .limit_15_0 = 0x30FF,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x2,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        },

        /* Segmento de Datos de nivel 3*/
        [GDT_IDX_DS_RING_3] =
        {
            .limit_19_16 = 0x03,
            .limit_15_0 = 0x30FF,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x2,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        },

        /* Segmento de Video de nivel 0*/
        [GDT_IDX_VM_RING_0] =
        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x1F3F,

            .base_31_24 = 0x00,
            .base_23_16 = 0x0B,
            .base_15_0 = 0x8000,

            .type = 0x2,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
        },
        
        [GDT_IDX_TSS_IDLE] =
        
        {   
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },

        [GDT_IDX_TSS_INIT] =
        
        {   
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },

        [GDT_IDX_TSS_LEMMING_A_0] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },
        [GDT_IDX_TSS_LEMMING_A_1] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },
        [GDT_IDX_TSS_LEMMING_A_2] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },
        [GDT_IDX_TSS_LEMMING_A_3] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },
        [GDT_IDX_TSS_LEMMING_A_4] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },

        [GDT_IDX_TSS_LEMMING_B_0] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },

        [GDT_IDX_TSS_LEMMING_B_1] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },

        [GDT_IDX_TSS_LEMMING_B_2] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },

        [GDT_IDX_TSS_LEMMING_B_3] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        },

        [GDT_IDX_TSS_LEMMING_B_4] = 

        {
            .limit_19_16 = 0x00,
            .limit_15_0 = 0x0067,

            .base_31_24 = 0x00,
            .base_23_16 = 0x00,
            .base_15_0 = 0x0000,

            .type = 0x9, //1001
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
            
        }




};

gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};
