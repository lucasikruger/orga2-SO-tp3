/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones de impresion por pantalla.
*/

#include "screen.h"
#include "i386.h"

void printScanCode(uint8_t scancode){
	if(!(scancode & 0x80)){
    print_hex(scancode, 2, 78, 0,
    C_FG_DARK_GREY | C_BG_LIGHT_GREY);
	}
}

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }
}

void print_char(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i = 0;
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
  }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}


regs_t registrosActuales;

void escanearRegistros(int exception, int32_t* esp3, int16_t ss, int16_t gs, int16_t fs, int16_t es, int16_t ds, int32_t edi, int32_t esi, int32_t ebp, int32_t esp, int32_t ebx, int32_t edx, int32_t ecx, int32_t eax, int32_t err, int32_t eip, int16_t cs, int32_t eflags)
{
  registrosActuales.eax = eax;
  registrosActuales.ebx = ebx;
  registrosActuales.ecx = ecx;
  registrosActuales.edx = edx;
  registrosActuales.esi = esi;
  registrosActuales.edi = edi;
  registrosActuales.ebp = ebp;
  registrosActuales.esp = esp;
  registrosActuales.eip = eip;
  registrosActuales.cs = cs;
  registrosActuales.ds = ds;
  registrosActuales.es = es;
  registrosActuales.fs = fs;
  registrosActuales.gs = gs;
  registrosActuales.ss = ss;
  registrosActuales.eflags = eflags;
  registrosActuales.cr0 = rcr0();
  registrosActuales.cr2 = rcr2();
  registrosActuales.cr3 = rcr3();
  registrosActuales.cr4 = rcr4();
  registrosActuales.err = err;
  registrosActuales.exception = exception;
  registrosActuales.esp3 = esp3;

}

void print_libretas(){
    print("341/18 - 153/18 - 799/19", 28, 0, 0x0F);
}

uint8_t encontramosErrorCode(uint32_t intr)
{
  if (intr == 8 || intr == 10 || intr == 11 || intr == 12 || intr == 13 || intr == 14 || intr == 17 || intr == 30)
    return 1;
  return 0;
}

void printRegs(){
	if (registrosActuales.exception != -1)
      {
        print("OCURRIO UNA EXCEPCION ", 20, 1, 0x0A);
        print_dec(registrosActuales.exception, 2, 30, 1, 0x0A);
      }
      else
        print("NO OCURRIO UNA EXCEPCION", 20, 1, 0x0A);

      print("EAX", 21, 5, 0x0F);
      print_hex(registrosActuales.eax, 8, 25, 5, 0x0A);
      print("EBX", 21, 7, 0x0F);
      print_hex(registrosActuales.ebx, 8, 25, 7, 0x0A);
      print("ECX", 21, 9, 0x0F);
      print_hex(registrosActuales.ecx, 8, 25, 9, 0x0A);
      print("EDX", 21, 11, 0x0F);
      print_hex(registrosActuales.edx, 8, 25, 11, 0x0A);
      print("ESI", 21, 13, 0x0F);
      print_hex(registrosActuales.esi, 8, 25, 13, 0x0A);
      print("EDI", 21, 15, 0x0F);
      print_hex(registrosActuales.edi, 8, 25, 15, 0x0A);
      print("EBP", 21, 17, 0x0F);
      print_hex(registrosActuales.ebp, 8, 25, 17, 0x0A);
      print("ESP", 21, 19, 0x0F);
      print_hex(registrosActuales.esp, 8, 25, 19, 0x0A);
      print("EIP", 21, 21, 0x0F);
      print_hex(registrosActuales.eip, 8, 25, 21, 0x0A);

      print("CS", 22, 23, 0x0F);
      print_hex(registrosActuales.cs, 8, 25, 23, 0x0A);
      print("DS", 22, 25, 0x0F);
      print_hex(registrosActuales.ds, 8, 25, 25, 0x0A);
      print("ES", 22, 27, 0x0F);
      print_hex(registrosActuales.es, 8, 25, 27, 0x0A);
      print("FS", 22, 29, 0x0F);
      print_hex(registrosActuales.fs, 8, 25, 29, 0x0A);
      print("GS", 22, 31, 0x0F);
      print_hex(registrosActuales.gs, 8, 25, 31, 0x0A);
      print("SS", 22, 33, 0x0F);
      print_hex(registrosActuales.ss, 8, 25, 33, 0x0A);

      print("EFLAGS", 21, 36, 0x0F);
      print_hex(registrosActuales.eflags, 8, 28, 36, 0x0A);

      print("CR0", 46, 6, 0x0F);
      print_hex(registrosActuales.cr0, 8, 50, 6, 0x0A);
      print("CR2", 46, 8, 0x0F);
      print_hex(registrosActuales.cr2, 8, 50, 8, 0x0A);
      print("CR3", 46, 10, 0x0F);
      print_hex(registrosActuales.cr3, 8, 50, 10, 0x0A);
      print("CR4", 46, 12, 0x0F);
      print_hex(registrosActuales.cr4, 8, 50, 12, 0x0A);
      print("ERRORCODE", 46, 14, 0x0F);
      print_hex(registrosActuales.err, 8, 50, 14, 0x0A);


      print("STACK LVL3", 40, 18, 0x0F);
      print_hex(0, 8, 40, 20, 0x0A);
      print_hex(0, 8, 40, 22, 0x0A);
      print_hex(0, 8, 40, 24, 0x0A);

  	int elemStackeados = (0x08003000 - (int) registrosActuales.esp3)/4;
  	for(int i = 0 ; i<3 && i<elemStackeados; i++){
		  print_hex(registrosActuales.esp3[i], 8, 40, 20+i*2, 0x0A);
  	}
	
}

