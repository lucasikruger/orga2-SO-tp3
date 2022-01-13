; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"

extern GDT_DESC
extern IDT_DESC

extern _isr32
extern _isr33
extern _isr88
extern _isr98
extern _isr108

extern mmu_init
extern mmu_init_kernel_dir
extern mmu_map_page
extern mmu_init_task_dir
extern tss_init
extern tss_initTaskIdle

extern next_clock
%define CS_RING_0 (8 << 3)

%define DS_RING_0 (10 << 3)

%define VM_RING_0 (12 << 3)

%define TSS_IDLE (13 << 3)

%define TSS_INIT (14 << 3)

%define ESP_INIT 0x25000
%define EBP_INIT 0x25000

extern screen_init
extern idt_init
extern pic_reset 
extern pic_enable
extern print_dec
extern screen_draw_box

extern print_libretas
extern sched_init
extern screenTest
extern createLemmingBetarote
extern createLemmingAmalin
extern printMapa


global start


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

; start_libreta_msg db     '341/18 - 153/18 - 799/19'
; start_libreta_len equ    $ - start_libreta_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0
    ;breakpoint
    ; xchg bx,bx

    ; Habilitar A20
    call A20_disable  
    call A20_check
    call A20_enable
    call A20_check
    
    
    ; Cargar la GDT

    lgdt [GDT_DESC]


    ; Setear el bit PE del registro CR0
    
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    ; Saltar a modo protegido
    jmp CS_RING_0:modo_protegido

BITS 32
modo_protegido:
    ; Establecer selectores de segmentos

    mov ax, DS_RING_0
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov fs, ax
    mov ax, VM_RING_0
    mov fs, ax
    ; Establecer la base de la pila
    mov ebp, EBP_INIT
    mov esp, ESP_INIT
    ; Imprimir mensaje de bienvenida
    print_text_pm start_pm_msg, start_pm_len, 0x07, 0 ,0
   


    xor edi,edi

; pintamos parte verde
imprimirBarra:
    
    mov dword [fs:edi], 0x00200020 ;b0010 0010 
    add edi, 4
    cmp edi, 160
    

    jl imprimirBarra
imprimirVerde:
    
    mov dword [fs:edi], 0x22202220 ;b0010 0010 
    add edi, 4
    cmp edi, 6560
    

    jl imprimirVerde
; pintamos parte negra
imprimirNegro:

    mov dword [fs:edi], 0x00200020


    add edi, 4
    cmp edi, 8000 
jl imprimirNegro

    
; pintamos rojo
xor edi,edi
mov esi, 7058 ; 80*2*44 + 9*2 = 7058

loopRojoV:
    xor ecx,ecx
    loopRojoH:
    
    mov dword [fs:esi],  0x44204420

    add esi, 2
    inc ecx
    cmp ecx, 9
    jl loopRojoH
    add esi, 142
inc edi
cmp edi, 3 
jl loopRojoV

;pintamos azul
xor edi,edi
mov esi, 7164 ; 80*2*44 + 61*2 = 7164
loopAzulV:
    xor ecx,ecx
    loopAzulH:
    
    mov dword [fs:esi],  0x11201120

    add esi, 2
    inc ecx
    cmp ecx, 9
    jl loopAzulH
    add esi, 142
inc edi
cmp edi, 3 
jl loopAzulV
  

    ; Inicializar el manejador de memoria
 
    call mmu_init
    ; Inicializar el directorio de paginas

    call mmu_init_kernel_dir
    ; Cargar directorio de paginas

    mov cr3, eax 


    ; Habilitar paginacion
    mov eax,cr0 


    or eax, 0x80000000 
    mov cr0, eax 


    call print_libretas
    ; print_dec(codigo,1,17,0,0x0C);

    ;print_text_rm start_libreta_msg, start_libreta_len, 0x02, 0, 28
; xchg bx,bx

;     push 3 ; 2
;     xchg bx,bx

;     push 0x00400000 ; Dire fisica
;     xchg bx,bx

;     push 0x0050E000 ; dire virtual
;     xchg bx,bx
;     mov eax, cr3
;     xchg bx,bx
;     push eax ; cr3

; xchg bx,bx

;     call mmu_map_page
;     add esp, 16

;     mov BYTE [0x0050E027], 0x1


    push 0x100000
    mov edi, cr3 
    call mmu_init_task_dir
    add esp, 4
    mov cr3, eax
        
    mov word [fs:0], 0x0d12


    mov cr3, edi


    ; Inicializar tss

    call tss_init

    ; Inicializar tss de la tarea Idle

    call tss_initTaskIdle


    ; Inicializar el scheduler

    call sched_init


    ; Inicializar la IDT
    call idt_init

    ; Cargar IDT
    lidt [IDT_DESC]

    ;xor eax,eax
    ;idiv eax

    ; Configurar controlador de interrupciones
        call pic_reset 
        call pic_enable

    ; Habilitar interrupciones
        sti

    ; Cargar tarea inicial

        mov ax,TSS_INIT
        ltr ax

    ; Saltar a la primera tarea: Idle
       ;call screenTest
    ;    xchg bx,bx
    
       call printMapa
       jmp TSS_IDLE:0

        ; xchg bx,bx
    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
