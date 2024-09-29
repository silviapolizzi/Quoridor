# Quoridor

# Quoridor Game - Assembly Project

## Project Overview
This project implements the Quoridor board game on the LandTiger development board or an emulator, using Keil μVision. The game follows the official rules of Quoridor, where two players compete by moving tokens and placing barriers on a 7x7 grid, aiming to reach the opponent's perimeter line first.
Tokens are colored white for PLAYER1 and red for PLAYER2.


### Game Rules
- **Board**: A 7x7 grid, with grooved lines to allow walls (barriers) to be inserted.
- **Objective**: Move your token from your starting position to the opposite side of the board before your opponent.
- **Turns**: On each turn, a player can:
  1. Move their token horizontally or vertically.
  2. Place a wall to block their opponent's movement.
  
**Special Rules**:
- If the two tokens are facing each other, the player can jump over the opponent unless a barrier is behind them.
- It is prohibited to trap an opponent entirely using walls; there must always be a valid path to the goal.
- Each player has 20 seconds to complete their move, otherwise they lose their turn.

## Implementation Details

### Hardware and Software
- **Platform**: LandTiger development board or emulator.
- **Development Environment**: Keil μVision IDE.
  
The game is controlled using a **JOYSTICK** and buttons on the LandTiger board:
- **JOYSTICK** is used for token movement and wall positioning.
- **KEY1** switches between movement and wall placement modes.
- **KEY2** rotates walls during placement.
- **INT0** starts the game.

### Game Flow
1. **Starting the Game**: Press `INT0` to begin.
2. **Player Movement**:
   - Use the **JOYSTICK** to move the token one step in any direction.
   - Confirm the move by pressing **JOY SELECT**.
   - If PLAYER1 completes the move, it becomes PLAYER2’s turn, and vice versa.
   - The four possible movement squares around the player will be highlighted.
3. **Placing a Wall**:
   - Press **KEY1** to enter wall placement mode.
   - Use the **JOYSTICK** to position the wall between squares.
   - Press **KEY1** again to cancel or **JOY SELECT** to confirm.
   - Each player has 8 walls. Once all walls are placed, a warning will appear: “No walls available, move the token.”
   - Press **KEY2** to rotate the wall by 90 degrees before placement.

### Project Files
- **Grid.c**: Functions for drawing the interface, player tokens, and movement.
- **Wall.c**: Functions for placing walls.
- **Game.c**: Functions for saving moves in 32-bit format.
- **Grid.h**: Contains a 13x13 matrix as a global variable for tracking the positions of tokens and walls.

### Additional Features
- Players are given 20 seconds to make each move. If time expires, the player loses their turn.
- Global variables are used to manage the board's characteristics and the current player's state.

### Game Controls
- **JOYSTICK UP/DOWN/LEFT/RIGHT**: Move token or select wall position.
- **JOY SELECT**: Confirm the move or wall position.
- **KEY1**: Enter wall placement mode or cancel wall placement.
- **KEY2**: Rotate the wall 90 degrees during placement.
- **INT0**: Start the game.

## Project Setup
- Download and extract the project files to your working directory.
- Open the project in Keil μVision.
- If using the emulator, ensure that the settings are correctly configured for the LANDTIGER board.
- Build and run the project either on the emulator or the physical board.

