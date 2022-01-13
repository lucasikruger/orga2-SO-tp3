/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"

void sched_init();
uint16_t sched_next_task();

typedef struct tarea
{
  uint16_t selector;
  uint16_t indice;
  uint16_t activada;
} tarea_t;

struct sched
{
  tarea_t tareaIdle;
  int LemmingsA;
  int LemmingsB;
  int indexA;
  tarea_t puebloA[5];
  int indexB;
  tarea_t puebloB[5];
  bool ultimoPuebloA;
  bool enIdle;
  int clocks;
  uint16_t ordenDesalojoA[5];
  uint16_t ordenDesalojoB[5];
} sched_t;


struct sched scheduler;
uint16_t posicionesAY[5];
uint16_t posicionesAX[5];
uint16_t posicionesBY[5];
uint16_t posicionesBX[5];

uint16_t puntajeA;
uint16_t puntajeB;
bool termino = false;
tarea_t tareaIdleGenerica;
tarea_t tareaGenerica1;
tarea_t tareaGenerica2;
tarea_t tareaGenerica3;
tarea_t tareaGenerica4;
tarea_t tareaGenerica5;
tarea_t tareaGenerica6;
tarea_t tareaGenerica7;
tarea_t tareaGenerica8;
tarea_t tareaGenerica9;
tarea_t tareaGenerica10;

tarea_t tareasA[5];
tarea_t tareasB[5];
void desalojarLemmingActual();
void matarLemmingSiEsta(uint16_t y, uint16_t x);
void desalojarLemming(uint16_t Index, uint16_t pueblo);
void posLemmingActual(uint16_t *y, uint16_t *x);
void explodeLemming();

char getMapPosition(uint16_t x, uint16_t y);
void printMapa();
void modifyMapa(uint16_t x, uint16_t y, char letra, uint16_t colour);

void printearReloj(uint16_t x,uint16_t y);
void printearVivos();

void printearRelojQuieto(uint16_t x,uint16_t y);
void debug_mode_switch();

int chequearMapeoDireccion(int32_t dir);
void pageFault();

	char palitos[4] = {'|','/','-','\\'};
  bool debug_prendido = false;
#endif //  __SCHED_H__
