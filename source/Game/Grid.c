
/****************************************Copyright (c)**************************************************                         
**
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			Grid.c
****************************************************/

/* Includes ------------------------------------------------------------------*/

#include "Grid.h" 
#include "../GLCD/AsciiLib.h"
#include "../GLCD/GLCD.h"
#include "img.c"
#include "stdio.h"
#include <stdbool.h>

/* Private variables ---------------------------------------------------------*/
volatile uint8_t squareSize, sideMargin, spaceBetween;
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

#define GRID_SIZE 13
#define BG_COLOR Blue2  			// colore di background
#define MAX_VALID_CELLS 5


extern uint8_t currentX,currentY,indexX,indexY;
extern GridCell grid[GRID_SIZE][GRID_SIZE];
extern uint8_t time_count;
extern uint8_t countWall_1, countWall_2, currentPlayer, playerX[2] ,playerY[2], targetRow[2];
extern uint16_t playerColors[2];


/*----- CREA SCACCHIERA -----*/
int Create_Grid(uint16_t Color)
{
    uint8_t i, j;
    uint16_t displayWidth = 240; // Larghezza display
    uint16_t sideMargin = 8; // Spazio ai lati della scacchiera
	
    // Dimensioni della scacchiera e numero di caselle
    uint8_t chessboardSize = 7;
	
    // Spazio tra le caselle
    uint8_t spaceBetween = 4;

    // Calcola la distanza dal bordo per centrare la griglia
    uint8_t totalSpaceBetween = (chessboardSize - 1) * spaceBetween;
    uint8_t totalSpaceAndMargin = 2 * sideMargin + totalSpaceBetween;
    squareSize = (displayWidth - totalSpaceAndMargin) / chessboardSize;

		sideMargin = sideMargin - spaceBetween;

    // Disegna i quadrati
    for (i = 0; i < chessboardSize; i++) {
        for (j = 0; j < chessboardSize; j++) {
            uint16_t x = 2*sideMargin + i * (squareSize + spaceBetween);
            uint16_t y = 2*sideMargin + j * (squareSize + spaceBetween);

            // Disegna i lati del quadrato
            LCD_DrawLine(x, y, x + squareSize, y, Color); // Linea superiore
            LCD_DrawLine(x, y, x, y + squareSize, Color); // Linea sinistra
            LCD_DrawLine(x + squareSize, y, x + squareSize, y + squareSize, Color); // Linea destra
            LCD_DrawLine(x, y + squareSize, x + squareSize, y + squareSize, Color); // Linea inferiore
        }
    }
    return squareSize;  // Dimensione delle caselle
}


/*---------- CREA CASELLA DEL TEMPO ------------------*/
void Create_Rectangle_Center(uint16_t Color, int Init)
{ 	
		uint8_t textWidth = 20;
		uint16_t textHeight = 1;
		uint16_t displayWidth = 240;
		uint16_t displayHeight = 320;
		uint8_t sideMargin2 = 6; 
		uint8_t spaceBetween2 = 2;
		
		uint16_t centerRX, centerRY, textX, textY; // Coordinate del centro del rettangolo e del testo
		
		uint8_t rectangleWidth = (displayWidth - 2 * sideMargin2 - 2*spaceBetween2)/3; // Base del rettangolo
		uint8_t rectangleHeight = 50; // Altezza del rettangolo 

		uint16_t x = displayWidth/2-rectangleWidth/2 - spaceBetween2;
		uint16_t y = (displayHeight - rectangleHeight) - 2*sideMargin2; 
		char stringa[10];  // Assumendo che il numero massimo di cifre possa essere 10

		// Init è 1 quando si deve inizializzare il rettangolo, altrimenti aggiorna solo il tempo rimanente
		if (Init == 1){ 
			// Disegna il rettangolo centrale
			LCD_DrawLine(x, y, x + rectangleWidth, y, Color); // Linea superiore
			LCD_DrawLine(x, y, x, y + rectangleHeight, Color); // Linea sinistra
			LCD_DrawLine(x + rectangleWidth, y, x + rectangleWidth, y + rectangleHeight, Color); // Linea destra
			LCD_DrawLine(x, y + rectangleHeight, x + rectangleWidth, y + rectangleHeight, Color); // Linea inferiore
	

			// Calcola il centro del rettangolo 
	    centerRX =  x+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2-2;
			
			// Calcola le coordinate x e y per il testo nel rettangolo
			textX = centerRX - textWidth / 2 -2;
			textY = centerRY - textHeight / 2;

			// Disegna il testo centrato
			sprintf(stringa, "%d s", 20);
			GUI_Text(textX, textY, (uint8_t *) stringa, Color, BG_COLOR);
			
		}
		else{ // Aggiorna il testo 
			centerRX =  x+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2-2;
			textX = centerRX - textWidth / 2 - 2;
			textY = centerRY - textHeight / 2;

			// Disegna il testo centrato
			sprintf(stringa, "%d s  ", time_count);
			GUI_Text(textX, textY, (uint8_t *) stringa, Color, BG_COLOR);
		}
		return;
}

/*---------- CREA CASELLA DEL N. DI MURI RIMANENTI AL PLAYER 1 ------------------*/
void Create_Rectangle_Left(uint8_t countWall,uint16_t Color, int Init)
{		char stringa[10];
		uint16_t displayWidth = 240;
		uint16_t displayHeight = 320;
		uint8_t sideMargin2 = 6; 
		uint8_t spaceBetween2 = 2;
		uint8_t textWidth = 40;
		uint8_t textHeight = 20;
		uint16_t centerRX, centerRY, textX, textY;
		
		uint8_t rectangleWidth = (displayWidth - 2 * sideMargin2 - 2*spaceBetween2)/3;
		uint8_t rectangleHeight = 50; 

		uint16_t x = sideMargin2 - spaceBetween2;
		uint16_t y = (displayHeight - rectangleHeight) - 2*sideMargin2; 

	  if (Init == 1){
			
			LCD_DrawLine(x, y, x + rectangleWidth, y, Color); // Linea superiore
			LCD_DrawLine(x, y, x, y + rectangleHeight, Color); // Linea sinistra
			LCD_DrawLine(x + rectangleWidth, y, x + rectangleWidth, y + rectangleHeight, Color); // Linea destra
			LCD_DrawLine(x, y + rectangleHeight, x + rectangleWidth, y + rectangleHeight, Color); // Linea inferiore

	    centerRX =  x/2+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2;
			
			// Calcola le coordinate x e y per centrare il testo nel rettangolo
			textX = centerRX - textWidth / 2;
			textY = centerRY - textHeight / 2;

			// Disegna il testo centrato
			GUI_Text(textX, textY, (uint8_t *) "P1 Wall", Color, BG_COLOR);
			textY = centerRY - textHeight/2 + 20;
			textX = centerRX - 2 / 2;
			sprintf(stringa, "%d", 8);
			GUI_Text(textX, textY, (uint8_t *) stringa, Color, BG_COLOR);
		}
		else{
			centerRX =  x/2+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2;
			textX = centerRX - textWidth / 2;
			textY = centerRY - textHeight / 2;

			if (countWall >= 1){ 
				// Disegna il testo centrato
				GUI_Text(textX, textY, (uint8_t *) "P1 Wall", Color, BG_COLOR);
				textY = centerRY - textHeight/2 + 20;
				textX = centerRX - 2 / 2;
				sprintf(stringa, "%d", countWall);
				GUI_Text(textX, textY, (uint8_t *) stringa, Color, BG_COLOR);
			}
			else{ // Se terminano i muri 
				GUI_Text(textX, textY, (uint8_t *) "No Wall", Color, BG_COLOR);
				textY = centerRY - textHeight/2 + 20;
				textX = centerRX - 2 / 2 -20;
				GUI_Text(textX, textY, (uint8_t *) "Move", Color, BG_COLOR);
			}
			
				
		}
		return;
}

/*---------- CREA CASELLA DEL N. DI MURI RIMANENTI AL PLAYER 2 ------------------*/
void Create_Rectangle_Right(uint8_t countWall,uint16_t Color, int Init)
{		
		char stringa[10];
		uint16_t displayWidth = 240;
		uint16_t displayHeight = 320;
		uint8_t sideMargin2 = 6; 
		uint8_t spaceBetween2 = 2;
		uint8_t textWidth = 40;
		uint8_t textHeight = 20;
		uint16_t centerRX, centerRY, textX, textY;
		
		uint8_t rectangleWidth = (displayWidth - 2 * sideMargin2 - 2*spaceBetween2)/3;
		uint8_t rectangleHeight = 50; 

		uint16_t x = (displayWidth - rectangleWidth)-sideMargin2-spaceBetween2;
		uint16_t y = (displayHeight - rectangleHeight) - 2*sideMargin2;
	
	  if (Init == 1){
			
			// Disegna il rettangolo centrale
			LCD_DrawLine(x, y, x + rectangleWidth, y, Color); // Linea superiore
			LCD_DrawLine(x, y, x, y + rectangleHeight, Color); // Linea sinistra
			LCD_DrawLine(x + rectangleWidth, y, x + rectangleWidth, y + rectangleHeight, Color); // Linea destra
			LCD_DrawLine(x, y + rectangleHeight, x + rectangleWidth, y + rectangleHeight, Color); // Linea inferiore


	    centerRX =  x+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2;
			
			// Calcola le coordinate x e y per centrare il testo nel rettangolo
			textX = centerRX - textWidth / 2;
		  textY = centerRY - textHeight / 2;

			// Disegna il testo centrato
			GUI_Text(textX, textY, (uint8_t *) "P2 Wall", Color, BG_COLOR);
			textY = centerRY - textHeight/2 + 20;
			textX = centerRX - 2 / 2;
			GUI_Text(textX, textY, (uint8_t *) "8", Color, BG_COLOR);
		}
		else{

			centerRX =  x+rectangleWidth/2;
			centerRY =  (y+ rectangleHeight/2) - sideMargin2;
			textY = centerRY - textHeight / 2;
			textX = centerRX - textWidth / 2;

			if (countWall >= 1){
				// Disegna il testo centrato
				GUI_Text(textX, textY, (uint8_t *) "P2 Wall", Color, BG_COLOR);
				textY = centerRY - textHeight/2 + 20;
				textX = centerRX - 2 / 2;
				sprintf(stringa, "%d", countWall);
				GUI_Text(textX, textY, (uint8_t *) stringa, Color, BG_COLOR);
			}
			else{
				GUI_Text(textX, textY, (uint8_t *) "No Wall", Color, BG_COLOR);
				textY = centerRY - textHeight/2 + 20;
				textX = centerRX - 2 / 2 - 20;
				GUI_Text(textX, textY, (uint8_t *) "Move", Color, BG_COLOR);
			}
		}
		return;	
}


/*----- Funzione per disegnare la pedina in una posizione specifica (centerX,centerY) ------*/
void DrawToken(uint8_t centerX, uint8_t centerY, uint16_t tokenColor, uint16_t backgroundColor){
    int i, j;
    int startX = centerX - IMG_WIDTH / 2;
    int startY = centerY - IMG_HEIGHT / 2;

    for (i = 0; i < IMG_HEIGHT; i++) {
        for (j = 0; j < IMG_WIDTH; j++) {
            int index = (i * IMG_WIDTH + j) * IMG_BYTES_PER_PIXEL;
						unsigned short pixelColor = (IMG_PIXEL_DATA[index] == 0xFF) ? backgroundColor : tokenColor;
            LCD_SetPoint(startX + j, startY + i, pixelColor);
        }
    }
}






/*---- Funzione per estrarre i centri delle caselle dalla griglia ----*/
void ExtractSquareCenters(uint8_t squareSize, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE]) {
  	uint8_t i, j;
    uint8_t chessboardSize = 7;  

    uint8_t index = 0;  // Indice per gli array di centri
		uint8_t sideMargin2 = 8;
	  uint8_t spaceBetween2 = 4;

    for (i = 0; i < chessboardSize; i++) {
			centersX[index] = sideMargin2 + (i * (squareSize + spaceBetween2)) + squareSize / 2;
			index=index+2; // Passa alla casella successiva (i muri sono nelle posizioni dispari)
		}
		index = 0;
		for (j = 0; j < chessboardSize; j++) {
			// Calcola il centro del quadrato corrente
			centersY[index] = sideMargin2 + (j * (squareSize + spaceBetween2)) + squareSize / 2;
			index=index+2; 
		}
		return;
	}

/*---- Funzione per muovere il token: incr/decr. gli indici in base alla direzione ----*/
void MoveTokenIndex(GridCell grid[][GRID_SIZE],uint8_t *indexX, uint8_t *indexY,Coordinate validMoves[MAX_VALID_CELLS],uint8_t numMoves, const char* direction)
{		uint8_t i;
		uint8_t newIndexX = *indexX;
		uint8_t newIndexY = *indexY;
    if (strcmp(direction, "down") == 0) {  
				if (grid[*indexX][*indexY+1].occupied == 0){   //  == 0 non c'è un muro 
					if (*indexY+2 < GRID_SIZE){										// non esce dai limiti della scacchiera 
						if (grid[*indexX][*indexY+2].hasToken == 1){	// == 1 c'è il token avversario 
							if (grid[*indexX][*indexY+3].occupied == 0)  // == 0 non c'è un muro alle spalle dell'avversario
								if (*indexY+4 < GRID_SIZE)
									newIndexY = newIndexY+4;                 // salto il token avversario
						}
						else{
									newIndexY = newIndexY+2;  // Muovi il token verso il basso
					}}}
    }     
		else if (strcmp(direction, "up") == 0) {
				if (grid[*indexX][*indexY-1].occupied == 0){
					if (*indexY-2 > -1){
						if (grid[*indexX][*indexY-2].hasToken == 1){
							if (grid[*indexX][*indexY-3].occupied == 0)
								if (*indexY-4 > -1)
									newIndexY = newIndexY-4;  
						}
						else{
								newIndexY = newIndexY-2;  
							}}}
				
    } else if (strcmp(direction, "right") == 0) {
				if (grid[*indexX+1][*indexY].occupied == 0){
					if (*indexX+2 < GRID_SIZE){
						if (grid[*indexX+2][*indexY].hasToken == 1){
							if (grid[*indexX+3][*indexY].occupied == 0)
								if (*indexX+4 < GRID_SIZE)
									newIndexX = newIndexX+4;  
						}
						else{
								newIndexX = newIndexX+2;  
							
							}}}
    } else if (strcmp(direction, "left") == 0) {
				if (grid[*indexX-1][*indexY].occupied == 0){
					if (*indexX-2 > -1){
						if (grid[*indexX-2][*indexY].hasToken == 1){
							if (grid[*indexX-3][*indexY].occupied == 0)
								if (*indexX-4 >-1)
									newIndexX = newIndexX-4;  
						}
						else{
								 newIndexX = newIndexX-2; 
									
}}}}
			for (i = 0; i < numMoves; i++) {     // check che la mossa calcolata sia effettivamente nelle mosse possibili (validMoves)
        if (validMoves[i].x == newIndexX && validMoves[i].y == newIndexY) {
           *indexX = newIndexX;
					 *indexY = newIndexY;
        }
    }
		return;
}

/*---- Funzione per muovere il token: disegna nella nuova posizione e cancella nella precedente ----*/						
void MoveToken(GridCell grid[][GRID_SIZE],uint8_t *currentX, uint8_t *currentY, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint8_t *indexX, uint8_t *indexY,Coordinate validMoves[MAX_VALID_CELLS],uint8_t numMoves,uint16_t color,uint16_t backgroundColor, const char* direction)
{ 
	grid[*indexX][*indexY].hasToken = 0; // La casella corrente non ha più il Token
	DrawToken(*currentX, *currentY, backgroundColor,backgroundColor); // Cancella il precedente 
  MoveTokenIndex(grid,indexX, indexY, validMoves, numMoves, direction); // Ottiene i nuovi indici
	*currentX = centersX[*indexX];   // Ottiene le nuove coordinate
	*currentY = centersY[*indexY];
	DrawToken(centersX[*indexX], centersY[*indexY], color,backgroundColor); // Disegna il token nella nuova posizione
	return;
}

/*---- Ridisegna il Token ----*/
void redrawToken(GridCell grid[][GRID_SIZE],uint8_t currentX, uint8_t currentY,uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint8_t previousX, uint8_t previousY, uint16_t color,uint16_t backgroundColor)
{
	DrawToken(currentX, currentY, backgroundColor,backgroundColor); 
	DrawToken(centersX[previousX], centersY[previousY], color, backgroundColor);
	return;
}

/*---- Inizializza i giocatori ----*/
void initializePlayers(uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE]) {
    // Inizializza il giocatore 1 (bianco)
    playerX[0] = 6;
    playerY[0] = 12;
    playerColors[0] = White;
		grid[playerX[0]][playerY[0]].hasToken = 1;
		targetRow[0] = 0;                            //Coordinata X del traguardo del giocatore

    // Inizializza il giocatore 2 (rosso(
    playerX[1] = 6;
    playerY[1] = 0;
    playerColors[1] = Red;
		grid[playerX[1]][playerY[1]].hasToken = 1;
		targetRow[1] = 12;
	
		// Disegna i giocatori
	  DrawToken(centersX[playerX[0]], centersY[playerY[0]],playerColors[0],BG_COLOR);
		DrawToken(centersX[playerX[1]],  centersY[playerY[1]], playerColors[1] ,BG_COLOR);
	  currentPlayer = 1;
		return;
}

/*---- Identifica le mosse possibili ----*/
void possibleMoves(GridCell grid[][GRID_SIZE],uint8_t x, uint8_t y,uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE],uint16_t color,uint8_t squareSize, Coordinate validMoves[MAX_VALID_CELLS], uint8_t* validCount){
	uint8_t i;
	int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}}; // Direzioni possibili
	*validCount = 0;
	validMoves[*validCount].x = x;
	validMoves[*validCount].y = y;
	(*validCount)++;
	for (i = 0; i < 4; i++) {
		int nextX = (int)x + directions[i][0];
		int nextY = (int)y + directions[i][1];

		// Verifica se la cella successiva è valida
 		if (nextX >= 0 && nextX < GRID_SIZE && nextY >= 0 && nextY < GRID_SIZE 
			&& grid[x + directions[i][0]/2][y + directions[i][1]/2].occupied == 0) { // Check che la casella successiva sia nei margini della griglia e che non ci sia un muro
				if (grid[nextX][nextY].hasToken == 1){ // Se c'è un token potrebbe saltarlo
 						if ( (nextX+directions[i][0]) >= 0 && (nextX+directions[i][0])< GRID_SIZE && nextY+directions[i][1] >= 0 && nextY+directions[i][1] < GRID_SIZE &&  grid[nextX + directions[i][0]/2][nextY + directions[i][1]/2].occupied == 0){
							validMoves[*validCount].x = nextX + directions[i][0];
							validMoves[*validCount].y = nextY + directions[i][1];
							(*validCount)++;
							highlightCell(nextX+directions[i][0], nextY+directions[i][1], centersX, centersY, squareSize, color);	// Evidenzia la cella valida
				}}
				else
				{
					validMoves[*validCount].x = nextX;
					validMoves[*validCount].y = nextY;
					(*validCount)++;
					highlightCell(nextX, nextY, centersX, centersY, squareSize, color);
				}
				}
			}
		return;
		}

/*---- Evidenzia i bordi della cella ----*/
void highlightCell(uint8_t x, uint8_t y,uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint8_t squareSize, uint16_t highlightColor) {
    // Calcola le coordinate pixel sulla griglia LCD in base alle coordinate della casella
    uint16_t pixelX = centersX[x]-(squareSize/2);  
    uint16_t pixelY = centersY[y]-(squareSize/2);

    // Disegna i lati del rettangolo attorno alla casella con il colore specificato
    LCD_DrawLine(pixelX, pixelY, pixelX + squareSize, pixelY, highlightColor); // Lato superiore
    LCD_DrawLine(pixelX, pixelY, pixelX, pixelY + squareSize, highlightColor); // Lato sinistro
    LCD_DrawLine(pixelX + squareSize, pixelY, pixelX + squareSize, pixelY + squareSize, highlightColor); // Lato destro
    LCD_DrawLine(pixelX, pixelY + squareSize, pixelX + squareSize, pixelY + squareSize, highlightColor); // Lato inferiore
		return;
}

/*---- Funzione ricorsiva per controllare se c'è un percorso disponibile dal token corrente al target ----*/
uint8_t isPathAvailable(GridCell grid[][GRID_SIZE], uint8_t visited[][GRID_SIZE], uint8_t playerX, uint8_t playerY, uint8_t targetRow) {
    uint8_t i;
		
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
		if (playerY == targetRow) {  
        return 1;
    }

    // Marca la cella corrente come visitata
    visited[playerX][playerY] = 1;

   

    // Itera attraverso le direzioni possibili
    for (i = 0; i < 4; ++i) {
        int nextX = playerX + directions[i][0];
        int nextY = playerY + directions[i][1];

        // Verifica se la cella successiva è valida
        if (nextX >= 0 && nextX < GRID_SIZE && nextY >= 0 && nextY < GRID_SIZE &&
            visited[nextX][nextY] == 0 && grid[nextX][nextY].occupied == 0) {
            // Chiamata ricorsiva per la cella successiva
            if (isPathAvailable(grid, visited, nextX, nextY, targetRow)) {
                return 1;
            }
        }
    }

    // Nessun percorso disponibile dalla cella corrente
    return 0;
}

// Funzione per verificare se c'è un percorso disponibile
uint8_t checkPathAvailable(GridCell grid[][GRID_SIZE], uint8_t playerX, uint8_t playerY, uint8_t targetRow) {
    // Creazione di una matrice per tenere traccia delle celle visitate
    uint8_t visited[GRID_SIZE][GRID_SIZE] = {{0}};

    return isPathAvailable(grid, visited, playerX, playerY, targetRow);
}


