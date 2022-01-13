/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/

#include "sched.h"

#include "defines.h"
#include "tss.h"
#include "screen.h"
#include "mapa.h"
#include "syscall.h"
#include "mmu.h"
#include "i386.h"


void sched_init(void)
{

	tareaIdleGenerica.selector = (GDT_IDX_TSS_IDLE << 3);
	tareaIdleGenerica.indice = 13;
	tareaIdleGenerica.activada = 0;

	tareaGenerica1.selector = (GDT_IDX_TSS_LEMMING_A_0 << 3);
	tareaGenerica1.indice = 15;
	tareaGenerica1.activada = 0;
	tareasA[0] = tareaGenerica1;

	tareaGenerica2.selector = (GDT_IDX_TSS_LEMMING_A_1 << 3);
	tareaGenerica2.indice = 16;
	
	tareaGenerica2.activada = 0;
	tareasA[1] = tareaGenerica2;

	tareaGenerica3.selector = (GDT_IDX_TSS_LEMMING_A_2 << 3);
	tareaGenerica3.indice = 17;
	tareaGenerica3.activada = 0;
	tareasA[2] = tareaGenerica3;

	tareaGenerica4.selector = (GDT_IDX_TSS_LEMMING_A_3 << 3);
	tareaGenerica4.indice = 18;
	tareaGenerica4.activada = 0;
	tareasA[3] = tareaGenerica4;

	tareaGenerica5.selector = (GDT_IDX_TSS_LEMMING_A_4 << 3);
	tareaGenerica5.indice = 19;
	tareaGenerica5.activada = 0;
	tareasA[4] = tareaGenerica5;

	/////////////////////////////////////////

	tareaGenerica6.selector = (GDT_IDX_TSS_LEMMING_B_0 << 3);
	tareaGenerica6.indice = 20;
	tareaGenerica6.activada = 0;
	tareasB[0] = tareaGenerica6;

	tareaGenerica7.selector = (GDT_IDX_TSS_LEMMING_B_1 << 3);
	tareaGenerica7.indice = 21;
	tareaGenerica7.activada = 0;
	tareasB[1] = tareaGenerica7;

	tareaGenerica8.selector = (GDT_IDX_TSS_LEMMING_B_2 << 3);
	tareaGenerica8.indice = 22;
	tareaGenerica8.activada = 0;
	tareasB[2] = tareaGenerica8;

	tareaGenerica9.selector = (GDT_IDX_TSS_LEMMING_B_3 << 3);
	tareaGenerica9.indice = 23;
	tareaGenerica9.activada = 0;
	tareasB[3] = tareaGenerica9;

	tareaGenerica10.selector = (GDT_IDX_TSS_LEMMING_B_4 << 3);
	tareaGenerica10.indice = 24;
	tareaGenerica10.activada = 0;
	tareasB[4] = tareaGenerica10;
	
	uint16_t ordenTareasA[5] = {6, 6, 6, 6, 6};
	uint16_t ordenTareasB[5] = {6, 6, 6, 6, 6};
	//tss_
	//	 struct tarea_t tareaIdle;
	scheduler.tareaIdle = tareaIdleGenerica;
	scheduler.LemmingsA = 0;
	scheduler.LemmingsB = 0;
	scheduler.indexA = 0;
	scheduler.indexB = 0;
	scheduler.ultimoPuebloA = true;
	scheduler.clocks = -1;
	for (int i = 0; i < 5; i++)
	{
		scheduler.puebloA[i] = tareasA[i];
		scheduler.puebloB[i] = tareasB[i];
		scheduler.ordenDesalojoA[i] = ordenTareasA[i];
		scheduler.ordenDesalojoB[i] = ordenTareasB[i];
	}
	//////////////////JUGADORES
	print("Amalin", 9, 42, 0x04 ); //poner color rojo
	print("Betarote", 62, 42, 0x01 ); //poner color azul

	// //tenemos 60-19 = 41
	// // => cada 4 bloques va un numero

		print("00", 21, 42, 0x04 ); //poner color rojo
		print("01", 25, 42, 0x04 ); //poner color rojo
		print("02", 29, 42, 0x04 ); //poner color rojo
		print("03", 33, 42, 0x04 ); //poner color rojo
		print("04", 37, 42, 0x04 ); //poner color azul

		print("00", 41, 42, 0x01 ); //poner color azul
		print("01", 45, 42, 0x01 ); //poner color azul
		print("02", 49, 42, 0x01 ); //poner color azul
		print("03", 53, 42, 0x01 ); //poner color azul
		print("04", 57, 42, 0x01 ); //poner color azul



}


int siguienteActiva(tarea_t tareas[5], int index)
{
	int indice = index;
	for (int i = 0; i < 5; i++)
	{
		indice = (indice + 1) % 5;
		if ((tareas[indice]).activada)
			return indice; // indice;
	}
	return 6;
}

void crearLemmingA()
{
	if(getMapPosition(39, 0)=='.'){
	for (size_t i = 0; i < 5; i++)
	{
		if (!((scheduler.puebloA)[i].activada))
		{

			tss_initTask(0, scheduler.puebloA[i].indice);
			scheduler.puebloA[i].activada = 1;
			int encontrada = 0;
			int h = 0;
			while (h < 5 && encontrada == 0)
			{
				if (scheduler.ordenDesalojoA[h] == 6)
				{
					scheduler.ordenDesalojoA[h] = i;
					encontrada = 1;
					modifyMapa(39, 0, 'A', 0x4F);

					puntajeA += 1;
					print_dec(puntajeA, 6,11,45, 0x4F);
					scheduler.LemmingsA = scheduler.LemmingsA + 1;
					posicionesAY[i] = 0;
					posicionesAX[i] = 39;
					return;
				}
				h = h + 1;
			}
		}
	}
	}
}

void crearLemmingB()
{
	if(getMapPosition(0, 79)=='.'){

	for (size_t i = 0; i < 5; i++)
	{
		if (!(scheduler.puebloB[i].activada))
		{
			tss_initTask(1, scheduler.puebloB[i].indice);
			scheduler.puebloB[i].activada = 1;
			int encontrada = 0;
			int h = 0;
			while (h < 5 && encontrada == 0)
			{
				if (scheduler.ordenDesalojoB[h] == 6)
				{
					scheduler.ordenDesalojoB[h] = i;
					modifyMapa(0, 79, 'B', 0x1F);
					puntajeB += 1;
					print_dec(puntajeB, 6, 64, 45,0x1F);
					scheduler.LemmingsB = scheduler.LemmingsB + 1;
					posicionesBY[i] = 79;
					posicionesBX[i] = 0;
					encontrada = 1;
					return;
					break;
				}
				h = h + 1;
			}
		}
	}
  }

}
/*

*/
void desalojarLemmingA()
{
	for (int i = 0; i < 5; i++)
	{
		if (scheduler.ordenDesalojoA[i] != 6)
		{
			modifyMapa(posicionesAX[scheduler.ordenDesalojoA[i]],posicionesAY[scheduler.ordenDesalojoA[i]], '.', 0x22);
			scheduler.puebloA[scheduler.ordenDesalojoA[i]].activada = 0;
			for (int h = i; h < 4; h++)
			{
				scheduler.ordenDesalojoA[h] = scheduler.ordenDesalojoA[h + 1];
			}
			scheduler.ordenDesalojoA[4] = 6;
			scheduler.LemmingsA = scheduler.LemmingsA - 1;
			
			
			return; //revisar
			break;

		}
	}
}

void desalojarLemmingB()
{
	for (int i = 0; i < 5; i++)
	{
		if (scheduler.ordenDesalojoB[i] != 6)
		{
			modifyMapa(posicionesBX[scheduler.ordenDesalojoB[i]],posicionesBY[scheduler.ordenDesalojoB[i]], '.', 0x22);

			scheduler.puebloB[scheduler.ordenDesalojoB[i]].activada = 0;
			for (int h = i; h < 4; h++)
			{
				scheduler.ordenDesalojoB[h] = scheduler.ordenDesalojoB[h + 1];
			}
			scheduler.ordenDesalojoB[4] = 6;
			scheduler.LemmingsB = scheduler.LemmingsB - 1;
			
			
			return; 
		}
	}
}
		
void printearVivos(){
	uint16_t  y = 43;
	uint16_t x;
	for (uint16_t i = 0; i < 5; i++)
	{
		
		x = 21+i*4;
		if (scheduler.puebloA[i].activada){
			printearReloj(x,y);
		}else{
			printearRelojQuieto(x,y);
		}
		x = 41+i*4;
		if (scheduler.puebloB[i].activada){
			printearReloj(x,y);
		}else{
			printearRelojQuieto(x,y);
		}
	}
	
}
void printearRelojQuieto(uint16_t x,uint16_t y){
	char caracter = '-';
	print_char(&caracter, x, y, 0x0F );
}

void printearReloj(uint16_t x,uint16_t y){


	print_char(&palitos[scheduler.clocks%4], x, y, 0x0F );

}

bool debug_impreso = false;




uint16_t sched_next_task(void)
{
	if(termino||debug_prendido){
	if (debug_prendido && !debug_impreso){
	screen_draw_box(1,10,40,60,' ', 0); //hacemos el cuadrado negro
	printRegs();
	debug_impreso = true;
	}
	return scheduler.tareaIdle.selector;
	}


	
	printearVivos();
	int indiceSig;
	scheduler.clocks = scheduler.clocks + 1;

	if (scheduler.clocks % 401 == 0)
	{

		if (scheduler.LemmingsA < 5)
		{
			crearLemmingA();
		}
		if (scheduler.LemmingsB < 5)
		{
			crearLemmingB();
		}
	}

	if (scheduler.clocks % 2001 == 0)
	{
		if (scheduler.LemmingsA == 5)
		{
			desalojarLemmingA();
			crearLemmingA();
		}
		if (scheduler.LemmingsB == 5)
		{

			desalojarLemmingB();
			crearLemmingB();
		}
	}
	if (scheduler.ultimoPuebloA)
	{ //estamos en pueblo A
		indiceSig = siguienteActiva(scheduler.puebloB, scheduler.indexB);
		if (indiceSig < 6)
		{
			scheduler.indexB = (indiceSig);
			scheduler.ultimoPuebloA = false;
			return scheduler.puebloB[indiceSig].selector;
		}
		else
		{
			indiceSig = siguienteActiva(scheduler.puebloA, scheduler.indexA);
			if (indiceSig < 6)
			{
				scheduler.ultimoPuebloA = true;
				scheduler.indexA = (indiceSig);

				return scheduler.puebloA[indiceSig].selector;
			}
			else
			{

				return scheduler.tareaIdle.selector;
			}
		}
	}
	else
	{ //estamos en pueblo B
		indiceSig = siguienteActiva(scheduler.puebloA, scheduler.indexA);
		if (indiceSig < 6)
		{
			scheduler.ultimoPuebloA = true;
			scheduler.indexA = (indiceSig);

			return scheduler.puebloA[indiceSig].selector;
		}
		else
		{

			indiceSig = siguienteActiva(scheduler.puebloB, scheduler.indexB);
			if (indiceSig < 6)
			{
				scheduler.indexB = (indiceSig);
				scheduler.ultimoPuebloA = false;

				return scheduler.puebloB[indiceSig].selector;
			}
			else
			{

				return scheduler.tareaIdle.selector;
			}
		}
	}

	return 0;
}

uint16_t IdleTask(void)
{
	
	return scheduler.tareaIdle.selector;
}

uint16_t moveLemming(uint16_t direccion)
{
	// //tarea_t lemming;
	uint16_t posicionX = 0;
	uint16_t posicionY = 0;
	uint16_t posicionaMoverseX = 0;
	uint16_t posicionaMoverseY = 0;

	if (scheduler.ultimoPuebloA == false)
	{
		posicionX = posicionesBX[scheduler.indexB];
		posicionY = posicionesBY[scheduler.indexB];
	}
	else
	{
		posicionX = posicionesAX[scheduler.indexA];
		posicionY = posicionesAY[scheduler.indexA];
	}
	char letradelMapa = 'j';

	if (direccion == 0)
	{
		letradelMapa = getMapPosition(posicionX - 1, posicionY);
		posicionaMoverseX = posicionX - 1;

		posicionaMoverseY = posicionY;
	}
	if (direccion == 1) //Der
	{
		letradelMapa = getMapPosition(posicionX, posicionY + 1);
		posicionaMoverseX = posicionX;

		posicionaMoverseY = posicionY + 1;
		if (scheduler.ultimoPuebloA && posicionY == 78)
		{
			//ganar(0);
			termino = true;
			print( "Gano Amalin",40,20,0xF0);
			
		}
	}
	if (direccion == 2)
	{
		letradelMapa = getMapPosition(posicionX + 1, posicionY);
		posicionaMoverseX = posicionX + 1;

		posicionaMoverseY = posicionY;
	}

	if (direccion == 3) //IZQ
	{
		letradelMapa = getMapPosition(posicionX, posicionY - 1);
		posicionaMoverseX = posicionX;

		posicionaMoverseY = posicionY - 1;
		if (!scheduler.ultimoPuebloA && posicionY == 1)
		{
			termino = true;
			print( "GanoBetarote",40,20,0xF0);
		}
	}

	if (letradelMapa == '.')
	{

		modifyMapa(posicionX, posicionY, '.', 0x22);
		char letradelPueblo;
		uint16_t color;
		if (scheduler.ultimoPuebloA)
		{
			letradelPueblo = 'A';
			color = 0x4F;
		}
		else
		{
			letradelPueblo = 'B';
			color = 0x1F;
		}
		modifyMapa(posicionaMoverseX, posicionaMoverseY, letradelPueblo, color);
		if (scheduler.ultimoPuebloA == false)
		{
			posicionesBX[scheduler.indexB] = posicionaMoverseX;
			posicionesBY[scheduler.indexB] = posicionaMoverseY;
		}
		else
		{
			posicionesAX[scheduler.indexA] = posicionaMoverseX;
			posicionesAY[scheduler.indexA] = posicionaMoverseY;
		}
		return 0;
	}

	if (letradelMapa == 'Z')
	{
		return 3;
	}
	if (letradelMapa == 'L')
	{
		return 2;
	}
	if (letradelMapa == 'P')
	{
		return 1;
	}
	if (letradelMapa == 'B' || letradelMapa == 'A')
	{
		return 4;
	}
	return 4;
}

void bridgeLemming(uint16_t direccion)
{

	uint16_t posicionX = 0;
	uint16_t posicionY = 0;
	uint16_t posicionaMoverseX = 0;
	uint16_t posicionaMoverseY = 0;

	if (scheduler.ultimoPuebloA == false)
	{
		posicionX = posicionesBX[scheduler.indexB];
		posicionY = posicionesBY[scheduler.indexB];
	}
	else
	{
		posicionX = posicionesAX[scheduler.indexA];
		posicionY = posicionesAY[scheduler.indexA];
	}
	char letradelMapa = 'j';
	if (direccion == 0)
	{
		letradelMapa = getMapPosition(posicionX - 1, posicionY);
		posicionaMoverseX = posicionX - 1;

		posicionaMoverseY = posicionY;
	}
	if (direccion == 1)
	{
		letradelMapa = getMapPosition(posicionX, posicionY + 1);
		posicionaMoverseX = posicionX;

		posicionaMoverseY = posicionY + 1;
	}
	if (direccion == 2)
	{
		letradelMapa = getMapPosition(posicionX + 1, posicionY);
		posicionaMoverseX = posicionX + 1;

		posicionaMoverseY = posicionY;
	}

	if (direccion == 3)
	{

		letradelMapa = getMapPosition(posicionX, posicionY - 1);
		posicionaMoverseX = posicionX;

		posicionaMoverseY = posicionY - 1;
	}
	if (letradelMapa == 'L')
	{

		modifyMapa(posicionaMoverseX, posicionaMoverseY, '.', 0x22);
	}
	modifyMapa(posicionX, posicionY, '.', 0x22);
	desalojarLemmingActual();
}

void desalojarLemmingActual()
{
	uint16_t x;
	uint16_t y;
	posLemmingActual(&x, &y);
	matarLemmingSiEsta(x, y);
}

void matarLemmingSiEsta(uint16_t x, uint16_t y)
{

	for (uint16_t i = 0; i < 5; i++)
	{
		if (posicionesAX[i] == x && posicionesAY[i] == y && scheduler.puebloA[i].activada)
		{
			desalojarLemming(i, 0);
			modifyMapa(x, y, '.', 0x22);
			return;
		}
	}
	for (uint16_t i = 0; i < 5; i++)
	{
		if (posicionesBX[i] == x && posicionesBY[i] == y && scheduler.puebloB[i].activada)
		{
			desalojarLemming(i, 1);
			modifyMapa(x, y, '.', 0x22);
			return;
		}
	}
}
void desalojarLemming(uint16_t Index, uint16_t pueblo)
{
	tarea_t *tareas;
	uint16_t *ordenDesalojo;
	if (pueblo == 0)
	{
		ordenDesalojo = scheduler.ordenDesalojoA;

		tareas = scheduler.puebloA;
		scheduler.LemmingsA -= 1;
	}
	if (pueblo == 1)
	{
		ordenDesalojo = scheduler.ordenDesalojoB;

		tareas = scheduler.puebloB;
		scheduler.LemmingsB -= 1;
	}
	tareas[Index].activada = false;

	for (int h = Index; h < 4; h++)
	{
		ordenDesalojo[h] = ordenDesalojo[h + 1];
	}
	ordenDesalojo[4] = 6;
}

void posLemmingActual(uint16_t *x, uint16_t *y)
{
	uint16_t *listaPosX;
	uint16_t *listaPosY;

	uint16_t Index;
	if (scheduler.ultimoPuebloA)
	{

		Index = scheduler.indexA;
		listaPosX = posicionesAX;
		listaPosY = posicionesAY;
	}
	else
	{

		Index = scheduler.indexB;
		listaPosX = posicionesBX;
		listaPosY = posicionesBY;
	}

	*x = listaPosX[Index];
	*y = listaPosY[Index];
}
void explodeLemming()
{
	uint16_t x;
	uint16_t y;
	posLemmingActual(&x, &y);

	desalojarLemmingActual();
	modifyMapa(x, y, '.', 0x22);
	matarLemmingSiEsta(x, y - 1);
	matarLemmingSiEsta(x + 1, y);
	matarLemmingSiEsta(x, y + 1);
	matarLemmingSiEsta(x - 1, y);

	if (getMapPosition(x, y - 1) == 'P')
	{
		modifyMapa(x, y - 1, '.', 0x22);
	}
	if (getMapPosition(x + 1, y) == 'P')
	{
		modifyMapa(x + 1, y, '.', 0x22);
	}
	if (getMapPosition(x, y + 1) == 'P')
	{
		modifyMapa(x, y + 1, '.', 0x22);
	}
	if (getMapPosition(x - 1, y) == 'P')
	{
		modifyMapa(x - 1, y, '.', 0x22);
	}
}

char getMapPosition(uint16_t x, uint16_t y)
{
	if (x > 39 || y > 79)
	{
		return 'Z';
	}

	return mapa_original[x][y];
}

void modifyMapa(uint16_t x, uint16_t y, char letra, uint16_t colour)
{
	mapa_original[x][y] = letra;
	print_char(&letra, y, x + 1, colour);
}

void printMapa()
{

	for (int h = 0; h < 40; h++)
	{
		for (int i = 0; i < 80; i++)
		{
			uint16_t color = 0x22;
			const char letra = mapa_original[h][i];
			if (letra == 'L')
			{
				color = 0x11;
			}
			else if (letra == 'P')
			{
				color = 0x46;
			}
			else if (letra == 'A')
			{
				color = 0x4F;
			}
			else if (letra == 'B')
			{
				color = 0x1F;
			}

			print_char(&letra, i, h + 1, color);
		}
	}
}


void debug_mode_switch(){
	if (!debug_prendido){
		debug_prendido = true;
		debug_impreso = false;
	}else{	
		printMapa(); 
		debug_prendido = false;
		debug_impreso = false;
	}
}


int32_t array_a[4096][2];
int32_t array_b[4096][2];

int siguienteA = 0;
int siguienteB = 0;

int chequearMapeoDireccion(int32_t dir){

  int next = 0;
  if (scheduler.ultimoPuebloA == 1) next = siguienteA;
  else next = siguienteB;

  for (int i = 0; i < next; i++){

    int mapped = 0;
	if(scheduler.ultimoPuebloA == 1) mapped =array_a[i][0];
	else mapped = array_b[i][0];
    int phy =0;
	if (scheduler.ultimoPuebloA == 1)  phy=  array_a [i][1];
	else phy = array_b[i][1];

    if (dir == mapped)
      return phy;
  }
  return -1;
}

void pageFault(){
  
  int32_t dir = rcr2();
  if (dir >= 0x400000 && dir <= 0x13FFFFF){
    int32_t cr3 = rcr3();
    int32_t phy = chequearMapeoDireccion(dir >> 12);
    if (phy != -1){
      mmu_map_page(cr3, dir, phy, 0x7);
    } else {
	 phy = mmu_next_free_user_page();
        mmu_map_page(cr3, dir, phy, 0x07);

        if (scheduler.ultimoPuebloA == 1){
          array_a[siguienteA][0] = dir >> 12;
          array_a[siguienteA][1] = phy;
          siguienteA++;
        } else {
          array_b[siguienteB][0] = dir >> 12;
          array_b[siguienteB][1] = phy;
          siguienteB++;
        }
    }
  } else {
	uint16_t x;
	uint16_t y;
	posLemmingActual(&x, &y);
	modifyMapa(x, y, '.', 0x22);
	desalojarLemmingActual();
  }
}
