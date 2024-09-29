
/****************************************Copyright (c)**************************************************                         
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			Wall.c
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "Grid.h"
#include "Wall.h"
#include "../GLCD/AsciiLib.h"
#include "../GLCD/GLCD.h"
#include "img.c"

/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8346A		10 /* 0x0046 */  
#define  HX8347D    11 /* 0x0047 */
#define  HX8347A    12 /* 0x0047 */	
#define  LGDP4535   13 /* 0x4535 */  
#define  SSD2119    14 /* 3.5 LCD 0x9919 */


// Definizione della nuova dimensione della griglia
#define GRID_SIZE 13

extern GridCell grid[GRID_SIZE][GRID_SIZE];

extern uint8_t squareSize, spaceBetween, sideMargin;

uint8_t indexWX, indexWY; // indici dei muri sulla griglia

// Funzione per verificare se un nuovo muro si sovrappone o interseca con quelli esistenti
uint8_t doesWallOverlap(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t isHorizontal) {
	  uint8_t i; 
    // Verifica se il nodo è già occupato o se è oltre i limiti della griglia
    if (centerX >= GRID_SIZE || centerY >= GRID_SIZE) {
        return 2;  // 2 vuol dire che è fuori griglia 
    }
		else if (grid[centerX][centerY].occupied == 1){
				return 1;
		}

    // Verifica se il nuovo muro sovrapporrà o intersecherà altri muri esistenti
    if (isHorizontal) {
        for ( i = 1; i < 4; i++) {
            if (centerX + i > GRID_SIZE)  
                return 2;
						else if (grid[centerX + i][centerY].occupied ){
							if (grid[centerX + i][centerY].isHorizontal == isHorizontal) 
								return 1;
							else {
								if (i == 1 && grid[centerX + i][centerY+1].occupied && grid[centerX + i][centerY-1].occupied)
									return 1;
						}
        }
    }
	}
	else {
        for (i = 1; i < 4; i++) {
             if (centerY + i > GRID_SIZE){
                return 2;
            }
						else if ( grid[centerX][centerY + i].occupied){
							if (grid[centerX][centerY + i].isHorizontal == isHorizontal) 
								return 1;
							else {
								if (i == 1 && grid[centerX + 1][centerY+i].occupied && grid[centerX - 1][centerY+i].occupied){
									return 1;
							}
							 						}
        }
    }
	}
    // Se non ci sono sovrapposizioni, restituisce 0
    return 0;
}

// Funzione per aggiungere un nuovo muro alla griglia
void addWall(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t squareSize, uint8_t isHorizontal) {
    // Verifica se il nuovo muro si sovrappone o interseca con quelli esistenti
  uint8_t i;  
  
        // Occupa le celle corrispondenti al nuovo muro
        for (i = 0; i < 4; i++) {
            if (isHorizontal == 1) {
                if (centerX + i < GRID_SIZE) {
										grid[centerX + i][centerY].occupied = 1;
										grid[centerX + i][centerY].isHorizontal = 1;
                }
            } else {
                if (centerY + i < GRID_SIZE) {
										grid[centerX][centerY + i].occupied = 1;
										grid[centerX][centerY + i].isHorizontal = 0; 
							}
            }
        }
 
}

// Funzione per rimuovere un nuovo muro alla griglia (occupied = 0)
void removeWall(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t squareSize, uint8_t isHorizontal) {
		uint8_t i;  
  
		// Cancella le celle corrispondenti al nuovo muro
		for (i = 0; i < 4; i++) {
				if (isHorizontal == 1) {
						if (centerX + i < GRID_SIZE) {
							grid[centerX + i][centerY].occupied = 0;
							grid[centerX + i][centerY].isHorizontal = 0;
					} else {
						if (centerY + i < GRID_SIZE) 
							grid[centerX][centerY + i].occupied = 0;
						
				}
		}
 }
return;
}

// Funzione per cancellare il muro precedente dallo schermo
void erasePreviousWall(uint8_t centerX, uint8_t centerY, int BG_COLOR, uint8_t wallType) {

    convertIdxPxl(&centerX, &centerY,wallType); // Ottiene le coordinate del muro in pixel

    if (wallType == 0)  /* verticale */
			{
					uint8_t x0 = centerX;
					uint8_t y0 = centerY;
					uint8_t x1 = centerX;
					uint8_t y1 = centerY + 2*squareSize + spaceBetween;
					LCD_DrawLine(x0, y0, x1, y1, BG_COLOR);        // Sovrappone il muro con uno col colore dello sfondo
					LCD_DrawLine(x0+1, y0, x1+1, y1, BG_COLOR);
					LCD_DrawLine(x0+2, y0, x1+2, y1, BG_COLOR);
			}
			else if (wallType == 1)   /* orizzontale */
			{ 
					uint8_t x0 = centerX; 
					uint8_t y0 = centerY;
					uint8_t x1 = centerX + 2*squareSize + spaceBetween;
					uint8_t y1 = centerY;
					LCD_DrawLine(x0, y0, x1, y1, BG_COLOR);
				  LCD_DrawLine(x0, y0+1, x1, y1+1, BG_COLOR);
					LCD_DrawLine(x0, y0+2, x1, y1+2, BG_COLOR);
			}
   return;
}


void DrawWall(GridCell grid[][GRID_SIZE],uint8_t centerX, uint8_t centerY, int color, uint8_t wallType)
{ 			
			uint8_t k;
			if (wallType == 0)  /* verticale */
			{
					uint8_t x0 = centerX;
					uint8_t y0 = centerY;
					uint8_t x1 = centerX;
					uint8_t y1 = centerY + 2*squareSize + spaceBetween;
					for (k=0; k < 3;k++){
						LCD_DrawLine(x0+k, y0, x1+k, y1, color);
			}}
			else if (wallType == 1)   /* orizzontale */
			{ 
					uint8_t x0 = centerX; 
					uint8_t y0 = centerY;
					uint8_t x1 = centerX + 2*squareSize + spaceBetween;
					uint8_t y1 = centerY;
					for (k=0; k < 3;k++){
						LCD_DrawLine(x0, y0+k, x1, y1+k, color);
			}}
}

/*--- Funzione per modificare l'indice del muro ---*/
void MoveWallIndex(uint8_t *indexX, uint8_t *indexY,uint8_t isHorizontal, const char* direction)
{   if (isHorizontal){
			if (strcmp(direction, "down") == 0) {
				if (*indexY + 2 < GRID_SIZE -1 )
					*indexY = *indexY + 2;  							
			}
			else if (strcmp(direction, "up") == 0) {
				if (*indexY - 2 > 0 )
					*indexY = *indexY - 2;  
			}					
			else if (strcmp(direction, "right") == 0){ 
				if (*indexX + 2 < GRID_SIZE -1 )
					*indexX = *indexX + 2;  
			}					
			else if (strcmp(direction, "left") == 0) {
				 if (*indexX - 2 >= 0 )
						*indexX = *indexX - 2; 
				 if (*indexX == 0xFF)                
						*indexX = 0;
				}
			}
    else{
			if (strcmp(direction, "down") == 0) {
				if (*indexY + 2 < GRID_SIZE -1 )
					*indexY = *indexY + 2;  							
			}
			  
			else if (strcmp(direction, "up") == 0) {
				if (*indexY - 2 >= 0 )
					*indexY = *indexY - 2;  
				if (*indexY == 0xFF)
					*indexY = 0;
								
			}else if (strcmp(direction, "right") == 0){ 
				if (*indexX + 2 < GRID_SIZE -1 )
					*indexX = *indexX + 2;  
			}		
				
			else if (strcmp(direction, "left") == 0) {
				 if (*indexX - 2 > 0 )
						*indexX = *indexX - 2; 
				}
				return;
		}
}

/*--- Converte l'indice della griglia in pixel ---*/
void convertIdxPxl(uint8_t *indexX,uint8_t *indexY,uint8_t isHorizontal)
{
	if (isHorizontal == 1)
	{

		*indexY =((*indexY-1)/2)*(squareSize+spaceBetween) + (squareSize) + sideMargin+1;
		if (*indexX == 0)
			 *indexX = ((*indexX-1)/2)*(squareSize+spaceBetween)+ sideMargin;
		else
			*indexX = ((*indexX-1)/2)*(squareSize+spaceBetween)+ (squareSize)+ sideMargin+spaceBetween;
		
	}
	else{

	  if (*indexY == 0)
			 *indexY = ((*indexY-1)/2)*(squareSize+spaceBetween)+ sideMargin;
		else
			*indexY =((*indexY-1)/2)*(squareSize+spaceBetween) + (squareSize) + sideMargin+spaceBetween;
		*indexX = ((*indexX-1)/2)*(squareSize+spaceBetween)+ (squareSize)+ sideMargin+1;
	}
}


/*--- Disegna il muro nella nuova posizione cancellando quellop precedente ---*/					
void buildWall(uint8_t indexWX, uint8_t indexWY,uint8_t prevIndexX, uint8_t prevIndexY,int BG_COLOR,int colorWall,uint8_t isHorizontal,char* direction)
{
			erasePreviousWall(prevIndexX, prevIndexY, BG_COLOR, isHorizontal);
			convertIdxPxl(&indexWX, &indexWY,isHorizontal);
			DrawWall(grid, indexWX, indexWY, colorWall, isHorizontal);
	}

/*--- Ridisegna i muri ---*/					

 void redrawWall(GridCell grid[][GRID_SIZE], int colorWall) {
    uint8_t i, j, x, y, k;

    for (i = 0; i < GRID_SIZE; i = i + 1) {
        for (j = 1; j <= GRID_SIZE; j = j + 1) {
            if (grid[i][j].occupied == 1) {
                x = i;
                y = j;

                convertIdxPxl(&x, &y, grid[i][j].isHorizontal);

                for (k = 0; k < 3; k++) {
                    // Ridisegna il muro
                    if (grid[i][j].isHorizontal == 1 && i % 2 == 0 && j % 2 == 1) {
                        LCD_DrawLine(x , y+k, x + squareSize, y+k, colorWall);
												// Check di un muro successivo per farli contigui
												if (i + 2 <= GRID_SIZE && grid[i+2][j].occupied == 1 && grid[i+2][j].isHorizontal == 1)
													LCD_DrawLine(x + squareSize, y+k, x + squareSize + spaceBetween, y+k, colorWall);
                    } else if (grid[i][j].isHorizontal == 0 && i % 2 == 1 && j % 2 == 0){
                        LCD_DrawLine(x+k, y, x+k, y+ squareSize, colorWall);
												if (j + 2 >= 0 && grid[i][j+2].occupied == 1 && grid[i][j+2].isHorizontal == 0)
													LCD_DrawLine(x+k, y + squareSize, x+k, y + squareSize + spaceBetween, colorWall);
                    }

                }
            }
        }
    }
}

 
/*--- Inizializza un muro ---*/
void initWall(int colorWall)
{
	uint8_t isHorizontal = 1;
	indexWX = 6;
	indexWY = 5;
	sideMargin = 8;
	spaceBetween = 4;
	convertIdxPxl(&indexWX, &indexWY,isHorizontal);
	DrawWall(grid, indexWX, indexWY, colorWall, isHorizontal);
	indexWX = 6;
	indexWY = 5;
}
