# NerdChess Engine
NerdChess is a WIP (work in progress) chess engine created by myself for educational purposes. According to plan, this engine should apply
basic algorithms to create a sensible chess game, such as a MiniMax algorithm, basic position evaluation, etc. Please note: this project
is currently a work-in-progress, and many features are still missing.
## How to use
1. Download the latest build on your machine and execute it.
2. You will see a tiny grid with letters pop up, that is the board. Different letters represent different pieces: p = pawn, N = knight, B = bishop, R = rook, Q = queen, K = king.
3. You will see a small yellow square, that is the cursor. Move the cursor with your arrow keys and select pieces/squares by pressing SPACE.
4. Select the piece you want to move and press SPACE, then select the **SQUARE** you want to move to and press SPACE
5. The engine will then make their own move, when it has moved a piece, the cursor will reappear.\
**ATTENTION!** As of right now, the latest build will only accept input on devices running the **Windows** OS. This will most likely be changed in the future.
## Roadmap
Here is what I plan on adding and what I am currently attempting to implement:
1. Bitboard implementation - **DONE**
2. Moving/capturing pieces - *ALMOST DONE*
3. Position evaluation - *WIP*
4. Minimax algorithm - **DONE**
5. Add opening book - *WIP*
7. Add support for importing/exporting PGN and FEN (optional)
8. Add interactive GUI - *WIP*
## Known bugs:
1. Crash while selecting two empty squares with the cursor
2. En pessant will occur even if a pawn has not advanced 2 squares
3. Kings can move outside of the board
