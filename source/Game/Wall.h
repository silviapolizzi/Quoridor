
/*--------------File Info-------------------------------------------------------------------------------
** File name:			Wall.h

********************************************************************************************************/

#ifndef __WALL_H 
#define __WALL_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "Grid.h"
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


#define GRID_SIZE 13



//typedef struct {
//    uint8_t occupied;
//	  uint8_t hasToken;
//	  uint8_t isHorizontal;
//} GridCell;


// Matrice per la griglia
extern GridCell grid[GRID_SIZE][GRID_SIZE];



/* Private function prototypes -----------------------------------------------*/
uint8_t doesWallOverlap(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t isHorizontal);
void addWall(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t squareSize, uint8_t isHorizontal);
void erasePreviousWall(uint8_t centerX, uint8_t centerY,int BG_COLOR, uint8_t wallType);
void DrawWall(GridCell grid[][GRID_SIZE],uint8_t centerX, uint8_t centerY, int color, uint8_t wallType);
void MoveWallIndex(uint8_t *indexX, uint8_t *indexY, uint8_t isHorizontal,const char* direction);
void convertIdxPxl(uint8_t *indexX, uint8_t *indexY,uint8_t isHorizontal);
void buildWall(uint8_t indexWX, uint8_t indexWY,uint8_t previousWallX,uint8_t previousWallY,int BG_COLOR,int colorWall,uint8_t isHorizontal,char* direction);
void initWall(int colorWall);
void redrawWall(GridCell grid[][GRID_SIZE], int colorWall);
void removeWall(GridCell grid[][GRID_SIZE], uint8_t centerX, uint8_t centerY, uint8_t squareSize, uint8_t isHorizontal);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
