# NerdChess Engine
NerdChess is a WIP (work in progress) chess engine created by myself for educational purposes. According to plan, this engine should apply
basic algorithms to create a sensible chess game, such as a MiniMax algorithm, basic position evaluation, etc. Please note: this project
is currently a work-in-progress, and many features are still missing.

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
4. Engine will attempt to check enemy king while itself being in check
