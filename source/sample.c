/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            Quoridor game
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Silvia Polizzi
** Created date:            2024-12-23
** Version:                 v1.0
** Descriptions:            Quoridor game
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Silvia Polizzi
** Modified date:           14/01/2024
** Version:                 v2.0
** Descriptions:            Quoridor game
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "Game/Grid.h"
#include "timer/timer.h"
#include "Game/Wall.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "button_EXINT/button.h"

/* Defines ----------------------------------------------------------------*/
#define SIMULATOR 1s
#define GRID_SIZE 13  			// 13x13 griglia, 7 caselle, 6 spazi per i muri
#define BG_COLOR Blue2  			// colore di background
#define MAX_VALID_CELLS 5		// n. di caselle di spostamenti validi

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)

#endif

/* Global Variables -------------------------------------------------------*/
extern uint8_t  squareSize, spaceBetween, sideMargin;

/* centers: coordinate in pixel delle caselle sulla griglia */
uint8_t centersX[GRID_SIZE], centersY[GRID_SIZE];

/* current: coordinate correnti del token; index: indici correnti del token sulla griglia;
isHorizontal: stato del muro; target: obiettivo riga del giocatore corrente */
uint8_t currentX, currentY, indexX, indexY, isHorizontal, target;  

GridCell grid[GRID_SIZE][GRID_SIZE];			// griglia di gioco
Coordinate validMoves[MAX_VALID_CELLS];		// elenco di mosse valide 

uint8_t numMoves;						// n. di mosse possibili

/* Starting variables */
uint8_t countWall_1 = 8;   	// n. di muri iniziali per ogni giocatore
uint8_t countWall_2 = 8;
uint8_t currentPlayer = 1;  // Inizia con il giocatore 1
uint16_t playerColors[2] = {White, Red};  // Colori dei giocatori
uint8_t playerX[2] = {0};  // Coordinate X dei giocatori
uint8_t playerY[2] = {0};  // Coordinate Y dei giocatori
uint8_t targetRow[2] = {0}; //Coordinata X del traguardo dei giocatori

int main(void)
{
  /********************************INIZIALIZZAZIONE*************************************/
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	BUTTON_init();												/* BUTTON Initialization              */
	
	// Inizializza Schermo
	LCD_Clear(BG_COLOR);	
	squareSize = Create_Grid(Cyan);
	Create_Rectangle_Center(Black,1);
	Create_Rectangle_Left(8,playerColors[0],1);
	Create_Rectangle_Right(8,playerColors[1],1);
	
  // Estrai i centri e salvali nell'array
  ExtractSquareCenters(squareSize,centersX,centersY);
	
	// Inizializza i giocatori nelle posizioni corrette
	initializePlayers(centersX, centersY);
	// Il timer di 1 s decrementa il countdown su schermo 
	init_timer(0, 0x17D7840); 						    /* 1s * 25MHz = 25M */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
