/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../Game/Grid.h"
#include "../Game/Game.h"
#include "../RIT/RIT.h"
extern uint8_t time_count;
extern uint32_t move;
extern uint8_t currentPlayer;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler (void)
{
	uint8_t out_of_time;
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);

	// decrementa il conteggio dei secondi
	time_count--;
	
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	if (time_count < 1)  // è finito il tempo
	{
		// Mossa non valida, è scaduto il tempo:
		move = setPlayerId(move, currentPlayer -1);
		move = setActionType(move, 0);
		move = setVerticalHorizontal(move, 1);
		move = setYCoordinate(move,0); 
		move = setXCoordinate(move,0); 
		// Riavvia il timer
		time_count = 20;
		out_of_time = 1;
		changePlayer(out_of_time);
		out_of_time = 0;
	}
	// mostra i secondi a schermo
	Create_Rectangle_Center(Black, 0);
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
