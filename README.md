# Skippity Game in C

## About
Skippity is a board game implemented in C. The game features both human vs. human and human vs. computer modes, where the computer uses a strategy to make the best possible move.

## Game Features
- Two-player mode (Human vs. Human)
- Single-player mode (Human vs. Computer)
- Save/Load functionality
- Undo moves
- Computer AI that seeks optimal moves

## How to Play

### Game Overview
Skippity is a strategic board game played on a square grid. The objective is to collect sets of different colored pieces (skippers) by making strategic jumps across the board.

### Setup
- The game is played on an N×N board (size selected at start)
- The board is filled with 5 types of colored pieces (A, B, C, D, E)
- The center of the board starts empty

### Rules
1. **Turn-Based Play**: Players take turns making moves.

2. **Moving Pieces**: 
   - Select a piece on the board
   - Jump over another piece (horizontally or vertically) to an empty space
   - You capture the piece you jump over
   - The jumped-over piece is added to your collection

3. **Chain Jumps**: 
   - If after a jump your piece can make another jump, you can continue jumping
   - Multiple jumps can be made in a single turn with the same piece

4. **Passing**: 
   - When no more jumps are possible with your current piece, you pass the turn to the next player

5. **End of Game**: 
   - The game ends when no more legal jumps can be made by any player
   - At this point, the scores are calculated

### Scoring
- **Sets**: Points are awarded based on how many complete sets of different color pieces you've collected
- **Tie-Breaker**: If players have the same number of sets, the total number of collected pieces is used as a tie-breaker

### Game Controls
- Select a piece: Enter the row and column coordinates
- Make a move: Enter the destination row and column coordinates
- After a move, you can choose to:
  - Make another move with the same piece (if possible)
  - Pass your turn
  - Undo your last move
  - Save and exit the game
  - Exit without saving

### Computer Opponent
When playing against the computer, the AI will try to make optimal moves by prioritizing pieces that allow it to collect sets efficiently.

## Implementation Details
The game is implemented in C with the following components:
- Dynamic board generation
- Move validation
- AI opponent (makes decisions based on board state)
- File I/O for save/load functionality

## Compilation
```bash
gcc Skippity.c -o skippity
./skippity