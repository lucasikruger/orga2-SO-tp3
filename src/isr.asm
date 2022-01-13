; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"
extern print_exception
extern printScanCode
global next_clock

BITS 32

sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF

;; PIC
extern pic_finish1

;; Sched
extern sched_next_task
extern sched_next_Task
extern IdleTask
extern moveLemming
extern explodeLemming
extern bridgeLemming
extern desalojarLemmingActual

extern encontramosErrorCode
extern escanearRegistros

extern pageFault 
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;


%macro ISR 1
global _isr%1

_isr%1:
    pushad
    mov eax, %1
    push eax
    call encontramosErrorCode
    add esp, 4
    cmp al, 1
    je .hayErrorCodeEnLaPila
    popad
    sub esp, 4
    mov dword [esp], 0xDEAD                   
    pushad
    .hayErrorCodeEnLaPila:
    push ds
    push es
    push fs
    push gs
    push ss
    mov eax, [esp + 68]
    push eax
    mov eax, %1
    push eax
    call escanearRegistros
    add esp, 28
    mov eax, %1
    push eax
    call print_exception
    add esp, 4
    call desalojarLemmingActual
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    popad
    add esp, 4
    iret

%endmacro

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
; ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

global _isr14
_isr14:
        pushad
        call pageFault 
        popad
        add esp, 4
        iret
;; Rutina de atención del RELOJ


global _isr32
_isr32:
	pushad
        ;Avisar al pic que se recibio la interrupcion
        call pic_finish1
        ; Imprimir el reloj del sistema
        call next_clock
        
        call sched_next_task
        mov cx, ax
        str ax; guardo el tr en ax
        cmp ax, cx
        je .fin
        mov [sched_task_selector], cx
        jmp far [sched_task_offset]

.fin:
        popad
        iret
;; -------------------------------------------------------------------------- ;;
;; Rutina de atención del TECLADO

global _isr33


extern debug_mode_switch
_isr33:
	pushad
	in al, 0x60
        cmp al, 0x15     
        jne .printScan
        call debug_mode_switch
        jmp .fin
.printScan:
	push eax
	call printScanCode
	add esp, 4
.fin:
	call pic_finish1
	popad
	iret


;-----------------------------------------------------------------------------

resultado: dd 0x0
global _isr88
_isr88: 
    pushad
    push eax
    call moveLemming
    add esp,4
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    
    mov [resultado], eax
    popad
    mov eax, [resultado]
    
    iret



global _isr98
_isr98:
        pushad
        call explodeLemming
        mov word [sched_task_selector], 0x68
        jmp far [sched_task_offset]
	popad
	iret  
            

        
global _isr108
_isr108: 
    pushad
    push eax
    call bridgeLemming
    add esp,4
    mov word [sched_task_selector], 0x68
    jmp far [sched_task_offset]
    popad
    iret  
;; -------------------------------------------------------------------------- ;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret


;       
;
;
;
