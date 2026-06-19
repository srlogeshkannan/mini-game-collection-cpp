# Mini Game Project — Tic Tac Toe + Snake (C++)

A console-based mini game collection in C++ demonstrating loops, arrays, and conditional logic.

## Games Included

### 🎮 Game 1: Tic Tac Toe
| Feature | Details |
|---|---|
| **Two modes** | Player vs Player or Player vs AI |
| **AI** | Minimax algorithm — unbeatable |
| **Score tracking** | X wins / O wins / Draws across rounds |
| **Replay** | Play again after each match |
| **Dynamic board** | Board redraws after every move with color |
| **Win detection** | Checks rows, columns, both diagonals |

### 🐍 Game 2: Snake
| Feature | Details |
|---|---|
| **3 speed levels** | Slow / Medium / Fast |
| **Dynamic growth** | Snake grows when eating food (*) |
| **Collision detection** | Walls and self-collision = game over |
| **Score system** | +10 per food, auto speed-up every 50 pts |
| **High score** | Tracked within session |
| **Replay** | Play again after game over |
| **Controls** | W/A/S/D to move, Q to quit |

## C++ Concepts Used

- **Arrays** — 2D board grid for Tic Tac Toe, character grid for Snake
- **Loops** — Game loops, board rendering, win detection
- **Conditional logic** — Move validation, collision checks, win/loss/draw detection
- **Vectors** — Snake body as dynamic array
- **Structs** — Point struct for coordinates
- **Classes** — TicTacToe and Snake encapsulate all game logic
- **Recursion** — Minimax AI for Tic Tac Toe

## Build & Run

```bash
# Compile
make
# or
g++ -std=c++17 -Wall -O2 -o mini_game mini_game.cpp

# Run
./mini_game        # Linux / macOS
.\mini_game.exe    # Windows
```

> **Note:** Snake requires a real terminal for keyboard input (W/A/S/D).  
> Use OnlineGDB or compile locally for the best experience.

## Controls

| Game | Key | Action |
|---|---|---|
| Tic Tac Toe | 1–9 | Place mark in that cell |
| Snake | W | Move Up |
| Snake | S | Move Down |
| Snake | A | Move Left |
| Snake | D | Move Right |
| Snake | Q | Quit game |

## File Structure

```
mini_game/
├── mini_game.cpp   # Full source — both games in one file
├── Makefile
└── README.md
```
