/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		Has been tested SSD1289、ILI9320、R61505U、SSD1298、ST7781、SPFD5408B、ILI9325、ILI9328、
**						HX8346A、HX8347A
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2012-3-10
** Version:				1.3
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __GRID_H 
#define __GRID_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

/* Private define ------------------------------------------------------------*/

/* LCD Interface */
#define PIN_EN		(1 << 19)
#define PIN_LE		(1 << 20)
#define PIN_DIR		(1 << 21)
#define PIN_CS      (1 << 22)
#define PIN_RS		(1 << 23)
#define PIN_WR		(1 << 24)
#define PIN_RD		(1 << 25)   

#define LCD_EN(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_EN) : (LPC_GPIO0->FIOCLR = PIN_EN));
#define LCD_LE(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_LE) : (LPC_GPIO0->FIOCLR = PIN_LE));
#define LCD_DIR(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DIR) : (LPC_GPIO0->FIOCLR = PIN_DIR));
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));

/* Private define ------------------------------------------------------------*/
#define DISP_ORIENTATION  0  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24位转换16位
* Input          : - red: R
*                  - green: G 
*				   - blue: B
* Output         : None
* Return         : RGB 颜色值
* Attention		 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

#define MAX_VALID_CELLS 5
#define GRID_SIZE 13

typedef struct {
    uint8_t occupied;
	  uint8_t hasToken;
		uint8_t isHorizontal;
} GridCell;

typedef struct {
    uint8_t x;
    uint8_t y;
} Coordinate;


/* Private function prototypes -----------------------------------------------*/

int Create_Grid(uint16_t Color);
void Create_Rectangle_Center(uint16_t Color,int Init);
void Create_Rectangle_Left(uint8_t countWall,uint16_t Color, int Init);
void Create_Rectangle_Right(uint8_t countWall,uint16_t Color, int Init);
void DrawToken(uint8_t Xpos, uint8_t Ypos, uint16_t tokenColor, uint16_t backgroundColor);
void ExtractSquareCenters(uint8_t squareSize, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE]);
void MoveTokenIndex(GridCell grid[][GRID_SIZE],uint8_t *indexX, uint8_t *indexY,Coordinate validMoves[],uint8_t numMoves, const char* direction);
void MoveToken(GridCell grid[][GRID_SIZE],uint8_t *currentX, uint8_t *currentY, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint8_t *indexX, uint8_t *indexY, Coordinate validMoves[MAX_VALID_CELLS],uint8_t numMoves,uint16_t color,uint16_t backgroundColor, const char* direction);
void initializePlayers(uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE]);
void redrawToken(GridCell grid[][GRID_SIZE],uint8_t currentX, uint8_t currentY,uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint8_t previousX, uint8_t previousY, uint16_t color,uint16_t backgroundColor);
void possibleMoves(GridCell grid[][GRID_SIZE], uint8_t x, uint8_t y, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE], uint16_t color,uint8_t squareSize, Coordinate validCells[MAX_VALID_CELLS], uint8_t* validCount);

void highlightCell(uint8_t x, uint8_t y, uint8_t centersX[GRID_SIZE], uint8_t centersY[GRID_SIZE],uint8_t squareSize, uint16_t highlightColor);

	
uint8_t isPathAvailable(GridCell grid[][GRID_SIZE], uint8_t visited[][GRID_SIZE], uint8_t playerX, uint8_t playerY, uint8_t targetRow);
uint8_t checkPathAvailable(GridCell grid[][GRID_SIZE], uint8_t playerX, uint8_t playerY, uint8_t targetRow);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
