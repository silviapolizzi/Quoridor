/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "game.h"


// Definizione delle costanti
#define PLAYER_ID_MASK          0xFF000000 // Maschera per i primi 8 bit (Player Id)
#define ACTION_TYPE_MASK        0x00F00000 // Maschera per i 4 bit successivi (Tipo di azione)
#define VERTICAL_HORIZONTAL_MASK 0x000F0000 // Maschera per i 4 bit successivi (Verticale/Orizzontale)
#define Y_COORDINATE_MASK       0x0000FF00   // Maschera per gli 8 bit (Coordinate Y)
#define X_COORDINATE_MASK       0x000000FF 

// Definizione delle costanti per il tipo di azione
#define MOVE_ACTION             0x00000000 // Azione di movimento
#define WALL_PLACEMENT_ACTION   0x00100000 // Azione di posizionamento del muro

// Definizione delle costanti per Verticale/Orizzontale
#define VERTICAL                0x00000000 // Muro verticale
#define HORIZONTAL              0x00010000 // Muro orizzontale


// Funzione per impostare il Player Id in una mossa
uint32_t setPlayerId(uint32_t move, uint8_t playerId) {
    // Pulisce i bit del Player Id e li imposta con il nuovo valore
    return (move & ~PLAYER_ID_MASK) | ((uint32_t)playerId << 24);
}

// Funzione per impostare il tipo di azione in una mossa
uint32_t setActionType(uint32_t move, uint8_t actionType) {
    // Pulisce i bit del Tipo di Azione e li imposta con il nuovo valore
    return (move & ~ACTION_TYPE_MASK) | ((uint32_t)actionType << 20);
}

// Funzione per impostare Verticale/Orizzontale in una mossa
uint32_t setVerticalHorizontal(uint32_t move, uint8_t verticalHorizontal) {
    // Pulisce i bit di Verticale/Orizzontale e li imposta con il nuovo valore
    return (move & ~VERTICAL_HORIZONTAL_MASK) | ((uint32_t)verticalHorizontal << 16);
}

// Funzione per impostare le coordinate Y in una mossa
uint32_t setYCoordinate(uint32_t move, uint16_t yCoordinate) {
    // Pulisce i bit delle Coordinate Y e li imposta con il nuovo valore
    return (move & ~Y_COORDINATE_MASK) | (uint32_t)yCoordinate << 8;
}

// Funzione per impostare le coordinate X in una mossa
uint32_t setXCoordinate(uint32_t move, uint16_t xCoordinate) {
    // Pulisce i bit delle Coordinate X e li imposta con il nuovo valore
    return (move & ~X_COORDINATE_MASK) | (uint32_t)xCoordinate;
}
