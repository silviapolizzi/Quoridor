/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../Game/Grid.h"
#include "../Game/Wall.h"
#include "../timer/timer.h"
#include "../Game/Game.h"
#include "../button_EXINT/button.h"
#include "../joystick/joystick.h"
#include "../GLCD/AsciiLib.h"
#include "../GLCD/GLCD.h"
#include "stdio.h"
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define Yellow         0xFFE0
#define BG_COLOR Blue2
#define MAX_VALID_CELLS 5

volatile int down_0=0;
volatile int down_1=0;
volatile int down_2=0;

uint8_t mode;  /* 0 token; 1 wall */
extern uint8_t *indexXP1, *indexYP1, squareSize, indexX,indexY,isHorizontal;
extern uint8_t currentX,currentY,centersX[GRID_SIZE],centersY[GRID_SIZE],target;
extern uint8_t indexWX, indexWY;
extern uint8_t squareSize, spaceBetween, sideMargin;

extern uint8_t currentPlayer;  // Inizia con il giocatore 1
extern uint16_t playerColors[2];  // Colori dei giocatori
extern uint8_t playerX[2];  // Coordinate X dei giocatori
extern uint8_t playerY[2];  // Coordinate Y dei giocatori
extern uint8_t targetRow[2]; //Coordinata X del traguardo dei giocatori
int colorPlayer;
int colorWall = Yellow;
int colorWall_prov = Magenta;  // Colore muro provvisorio
uint32_t move;                 // Mossa in 32 bit

extern GridCell grid[GRID_SIZE][GRID_SIZE];
extern Coordinate validMoves[MAX_VALID_CELLS];
extern uint8_t numMoves;
extern uint8_t countWall_1,countWall_2;



void RIT_IRQHandler (void)
{					
	uint8_t previousWallX,previousWallY;
	static int start = 0;
	static int J_select=0;
	static int J_down = 0;
	static int J_left = 0;
	static int J_right = 0;
	static int J_up = 0;
	uint8_t new_indexX;
	uint8_t new_indexY;
	uint8_t out_of_time = 0;
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0 && start == 1){	
		/* Joytick J_Select pressed p1.25*/
		J_select++;		
		switch(J_select){
			case 1:
				// Confirm the move and change player
				
				if (indexY == target)  // il token ha raggiunto il target
						stopGame();
				else
					changePlayer(out_of_time);  // passa al giocatore successivo, se out_of_time == 0 -> il turno non è finito per mancanza di tempo
				break;
			default:
				break;
		}
	}
	else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && start == 1){
		J_down++;
		switch(J_down){
			case 1:
				if (mode == 0){
					MoveToken(grid,&currentX, &currentY, centersX, centersY, &indexX, &indexY, validMoves, numMoves, colorPlayer,BG_COLOR, "down");
				}
				
				else{
					  previousWallX = indexWX;
						previousWallY = indexWY;
					  MoveWallIndex(&indexWX, &indexWY,isHorizontal, "down");
						// Verifica la sovrapposizione
						if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 0){ // Non c'è sovrapposizione
								buildWall(indexWX,indexWY,previousWallX,previousWallY, BG_COLOR, colorWall_prov, isHorizontal,"down");//
								if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
									// se prima c'era un muro bisogna ridisegnarlo
									redrawWall(grid, colorWall);  
								}
								}else{ // c'è sovrapposizione
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){ 
										// nella posizione precedente c'era un muro, bisogna ridisegnarlo
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
										redrawWall(grid, colorWall);
								}else
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
									// Disegno il muro sulla nuova posizione
									new_indexX = indexWX;
									new_indexY = indexWY;
							    convertIdxPxl(&indexWX,&indexWY,isHorizontal);
									DrawWall(grid,indexWX,indexWY,colorWall_prov,isHorizontal);
									indexWX = new_indexX;
									indexWY = new_indexY;
						}
				
				break;}
			default:
				break;
		}
	}
else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && start == 1){
    J_left++;
    switch(J_left){
        case 1:
            if (mode == 0){
								MoveToken(grid,&currentX, &currentY, centersX, centersY, &indexX, &indexY, validMoves, numMoves, colorPlayer,BG_COLOR, "left");
						}
						else{

                previousWallX = indexWX;
                previousWallY = indexWY;
                MoveWallIndex(&indexWX, &indexWY, isHorizontal,"left");
                if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 0){
                    buildWall(indexWX, indexWY, previousWallX, previousWallY, BG_COLOR, colorWall_prov, isHorizontal, "left");
										if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
											redrawWall(grid, colorWall);
                }
								}else {
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
										redrawWall(grid, colorWall);
								}else
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
									new_indexX = indexWX;
									new_indexY = indexWY;
									convertIdxPxl(&indexWX,&indexWY,isHorizontal);
									DrawWall(grid,indexWX,indexWY,colorWall_prov,isHorizontal);
									indexWX = new_indexX;
									indexWY = new_indexY;
						}
            break;}
        default:
            break;
    }
}
else if((LPC_GPIO1->FIOPIN & (1<<28))  == 0 && start == 1){
    J_right++;
    switch(J_right){
        case 1:
            if (mode == 0){
								MoveToken(grid,&currentX, &currentY, centersX, centersY, &indexX, &indexY, validMoves, numMoves,colorPlayer,BG_COLOR, "right");
					}
						else{

                previousWallX = indexWX;
                previousWallY = indexWY;
                MoveWallIndex(&indexWX, &indexWY,isHorizontal, "right");
                if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 0){
                    buildWall(indexWX, indexWY, previousWallX, previousWallY, BG_COLOR, colorWall_prov, isHorizontal, "right");
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
										redrawWall(grid, colorWall);
                }
								}else{
									
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
										redrawWall(grid, colorWall);
									}else
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
									new_indexX = indexWX;
									new_indexY = indexWY;
									convertIdxPxl(&indexWX,&indexWY,isHorizontal);
									DrawWall(grid,indexWX,indexWY,colorWall_prov,isHorizontal);
									indexWX = new_indexX;
									indexWY = new_indexY;
						}
            break;}
        default:
            break;
    }
}
else if((LPC_GPIO1->FIOPIN & (1<<29) ) == 0 && start == 1){
    J_up++;
	
    switch(J_up){
        case 1:
            if (mode == 0){
								MoveToken(grid,&currentX, &currentY, centersX, centersY, &indexX, &indexY, validMoves, numMoves, colorPlayer,BG_COLOR, "up");
						}
						else{

                previousWallX = indexWX;
                previousWallY = indexWY;
								
                MoveWallIndex(&indexWX, &indexWY,isHorizontal, "up");
                if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 0){
                    buildWall(indexWX, indexWY, previousWallX, previousWallY, BG_COLOR, colorWall_prov, isHorizontal, "up");
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
										redrawWall(grid, colorWall);
                }
								}else{
									if (doesWallOverlap(grid, previousWallX, previousWallY, isHorizontal) == 1){
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
										redrawWall(grid, colorWall);
									}else
										erasePreviousWall(previousWallX,previousWallY,BG_COLOR,isHorizontal);
									new_indexX = indexWX;
									new_indexY = indexWY;
									convertIdxPxl(&indexWX,&indexWY,isHorizontal);
									DrawWall(grid,indexWX,indexWY,colorWall_prov,isHorizontal);
									indexWX = new_indexX;
									indexWY = new_indexY;
								}
            break;}
        default:
            break;
    }
}

	else {
			J_down=0;
			J_up = 0;
		  J_left = 0;
		  J_right = 0;
		  J_select = 0;
	} 
	
	/* button management */
	
	if(down_0!=0 && start == 0){ 
		down_0++;				
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* KEY0 pressed - START GAME*/
			switch(down_0){
				case 2:		
					// inizializzo le variabili col giocatore P1
          currentPlayer = 1;					
					indexX = playerX[currentPlayer-1];
					indexY = playerY[currentPlayer-1];
					colorPlayer = playerColors[currentPlayer-1];
					currentX = centersX[indexX];
					currentY = centersY[indexY];
			
					enable_timer(0);
				
					countWall_1 = 8;
					countWall_2 = 8;
				
					GAME_start();
					joystick_init();
				
					start = 1;
				  
					// evidenzia le mosse possibili
					possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Yellow, squareSize, validMoves, &numMoves);
					
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_1!=0){ 
		down_1++;				
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed - CHANGE MODE (PLAYER-WALL) */
			switch(down_1){
				case 2:				
					if (mode == 0){  // Player mode -> Wall mode
						if ((currentPlayer == 1 && countWall_1 > 0) || (currentPlayer == 2 && countWall_2 > 0)){ // se il giocatore ha ancora muri disponibili
							mode = 1;
							isHorizontal = 1;
							redrawToken(grid, currentX, currentY, centersX, centersY, playerX[currentPlayer-1], playerY[currentPlayer-1], colorPlayer, BG_COLOR);
							possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Cyan, squareSize, validMoves, &numMoves);
							
							// Inizializza il muro
							initWall(colorWall_prov);	
							NVIC_EnableIRQ(EINT2_IRQn);             
							
					} else  // Se il giocatore non ha più muri disponibili
 							GUI_Text(5, 240, (uint8_t *) "No walls, move the Token", colorPlayer, BG_COLOR);
						
					}
					else{    // Wall mode -> Player mode
						
						NVIC_DisableIRQ(EINT2_IRQn);
						mode = 0;
						erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
						possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Yellow, squareSize, validMoves, &numMoves);

						new_indexX = indexWY;
						new_indexY = indexWX;
						
						if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 1){
							redrawWall(grid, colorWall);
						}
							indexWX = new_indexX;
							indexWY = new_indexY;
						
							break;}
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_2!=0){ 
		down_2++;
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed - HORIZONTAL-VERTICAL*/			
			switch(down_2){
				case 2:
					if (mode == 1){
						if (isHorizontal == 0){
							erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
							// sistema gli indici x y
							if (indexWX == 0)
								new_indexX = indexWX-1;
							else
								new_indexX = indexWX-1;
							if (indexWY >= GRID_SIZE - 2 )
								new_indexY = indexWY-1;
							else
								new_indexY = indexWY+1;
							
							if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 1){
								redrawWall(grid,colorWall);
							}
							isHorizontal = 1;
							indexWX = new_indexX;
							indexWY = new_indexY;
							convertIdxPxl(&indexWX, &indexWY,isHorizontal);
							DrawWall(grid, indexWX, indexWY, colorWall_prov, isHorizontal);
							indexWX = new_indexX;
							indexWY = new_indexY;
						}
						else{
							erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
							if (indexWY == 0)
								new_indexY = indexWY+1;
							else
								new_indexY = indexWY-1;
							if (indexX >= GRID_SIZE - 2 )
								new_indexX = indexWX-1;
							else
								new_indexX = indexWX+1;
							if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 1){
								 convertIdxPxl(&indexWX,&indexWY,isHorizontal);
								 DrawWall(grid,indexWX,indexWY,colorWall,isHorizontal);}
							isHorizontal = 0;
							indexWX = new_indexX;
							indexWY = new_indexY;
							convertIdxPxl(&indexWX, &indexWY,isHorizontal);
							DrawWall(grid, indexWX, indexWY, colorWall_prov, isHorizontal);
							indexWX = new_indexX;
							indexWY = new_indexY;
							break;
					}}
				default:
					break;
			}
		}
		else {	/* button released */
			down_2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/* Funzione per passare al giocatore successivo */
void changePlayer(uint8_t out_of_time){
		GUI_Text(5, 240, (uint8_t *) "                           ", colorPlayer, BG_COLOR);
		if (mode == 0){  // Player Mode
					possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Cyan, squareSize,validMoves, &numMoves);

			
					// Conferma la posizione del giocatore corrente 
					// Salva la mossa
					if (out_of_time == 0) {
						move = setPlayerId(move, currentPlayer -1);
						move = setActionType(move, 0);
						move = setVerticalHorizontal(move, 0);
						// Salva la posizione del giocatore nella griglia
						grid[indexX][indexY].hasToken = 1;
						playerX[currentPlayer-1] = indexX;
						playerY[currentPlayer-1]= indexY;
					}
					else
						redrawToken(grid, currentX, currentY, centersX, centersY, playerX[currentPlayer-1], playerY[currentPlayer-1], colorPlayer, BG_COLOR);
					
					// Setting per il giocatore successivo
          currentPlayer = (currentPlayer == 1) ? 2 : 1;
					indexX = playerX[currentPlayer-1];
					indexY = playerY[currentPlayer-1];
					colorPlayer = playerColors[currentPlayer-1];
					// coordinate in pixel
					currentX = centersX[indexX];
					currentY = centersY[indexY];
					// salva le coordinate nella variabile mossa
					move = setYCoordinate(move,currentY); 
					move = setXCoordinate(move,currentX); 
					// set la riga di target del giocatore corrente
					target = targetRow[currentPlayer-1];
					// reset timer count
					disable_timer(0);
					init_timer(0, 0x17D7840); 
					enable_timer(0);
					possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Yellow,squareSize, validMoves, &numMoves);


				}
		else{
			if ((currentPlayer == 1 && countWall_1 > 0) || (currentPlayer == 2 && countWall_2 > 0)){
					if (doesWallOverlap(grid, indexWX, indexWY, isHorizontal) == 0 ){
						if(out_of_time == 0){
							addWall(grid, indexWX, indexWY, squareSize, isHorizontal ? 1 : 0);
							if (checkPathAvailable(grid, playerX[0], playerY[0], targetRow[0]) == 0 
								||  checkPathAvailable(grid, playerX[1], playerY[1], targetRow[1]) == 0){ // Non c'è percorso dispoinibile, cancella il muro
							 // Cancella il muro
								removeWall(grid,indexWX, indexWY, squareSize, isHorizontal);
								erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
								redrawWall(grid, colorWall);
								}
							else{
									convertIdxPxl(&indexWX, &indexWY,isHorizontal);
									DrawWall(grid, indexWX, indexWY, Yellow, isHorizontal);

									if (currentPlayer == 1){
										countWall_1--;
										Create_Rectangle_Left(countWall_1, colorPlayer, 0);

								 }else{
										countWall_2--;
										Create_Rectangle_Right(countWall_2, colorPlayer, 0);
										}
						
									// Salva la mossa
									move = setPlayerId(move, currentPlayer -1);
									move = setActionType(move, 1);
									move = setVerticalHorizontal(move, isHorizontal);
									convertIdxPxl(&indexWX,&indexWY,isHorizontal);
									move = setYCoordinate(move,indexWY+squareSize+spaceBetween/2); 
									move = setXCoordinate(move,indexWX+squareSize+spaceBetween/2); 
								
									
									currentPlayer = (currentPlayer == 1) ? 2 : 1;
									indexX = playerX[currentPlayer-1];
									indexY = playerY[currentPlayer-1];
									colorPlayer = playerColors[currentPlayer-1];
									currentX = centersX[indexX];
									currentY = centersY[indexY];
									target = targetRow[currentPlayer-1];
									mode = 0;
									disable_timer(0);
									init_timer(0, 0x17D7840); 
									enable_timer(0);
									possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Yellow, squareSize, validMoves, &numMoves);


							}
						}
						else{
							erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
							redrawWall(grid, colorWall); 
							mode = 0;
							currentPlayer = (currentPlayer == 1) ? 2 : 1;
							indexX = playerX[currentPlayer-1];
							indexY = playerY[currentPlayer-1];
							colorPlayer = playerColors[currentPlayer-1];
							currentX = centersX[indexX];
							currentY = centersY[indexY];
							target = targetRow[currentPlayer-1];
							disable_timer(0);
							init_timer(0, 0x17D7840); 
							enable_timer(0);
							possibleMoves(grid, playerX[currentPlayer-1], playerY[currentPlayer-1], centersX, centersY, Yellow, squareSize, validMoves, &numMoves);
						}							
							
					}						
					else{   // Overlap, cancella il muro
						erasePreviousWall(indexWX, indexWY, BG_COLOR, isHorizontal);
						redrawWall(grid, colorWall);  
					} 
				}
				
				}
		return;
}

// Uno dei giocatori ha vinto: ferma il gioco
void stopGame(){

	char stringa[20];
	uint16_t textX, textY;
	disable_timer(0);
	disable_RIT();
	LCD_Clear(BG_COLOR);
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	
	sprintf(stringa, "Player %d Wins", currentPlayer);
	textY = 150;
	textX = 50;
	GUI_Text(textX, textY, (uint8_t *) stringa, Black, BG_COLOR);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
