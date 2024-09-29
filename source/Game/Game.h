#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define PLAYER_ID_MASK          0xFF000000
#define ACTION_TYPE_MASK        0x00F00000
#define VERTICAL_HORIZONTAL_MASK 0x000F0000
#define Y_COORDINATE_MASK       0x0000FF00  
#define X_COORDINATE_MASK       0x000000FF 

#define MOVE_ACTION             0x00000000
#define WALL_PLACEMENT_ACTION   0x00100000

#define VERTICAL                0x00000000
#define HORIZONTAL              0x00010000

uint32_t setPlayerId(uint32_t move, uint8_t playerId);
uint32_t setActionType(uint32_t move, uint8_t actionType);
uint32_t setVerticalHorizontal(uint32_t move, uint8_t verticalHorizontal);
uint32_t setYCoordinate(uint32_t move, uint16_t yCoordinate);
uint32_t setXCoordinate(uint32_t move, uint16_t xCoordinate);

#endif /* GAME_H */
