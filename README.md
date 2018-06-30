# Albatross-8

Code to play a game of 6 x 6 Othello where player 1 (blue) is a human and player 2 is a computer algorithm. Player 1 can interface with the board with the use of two buttons: toggle and select. When it is player 1's turn, a potential move is highlighted in green and pressing toggle will switch to show an alternate potential move. When select is pressed, the move currently displayed will be made and player 2 will have its turn.



Current version of code is set to have player 1 and player 2 be computer algorithms which select pseudo-random moves, therefore code for a player interface has not been added.

To be used on an Arduino with an 8 x 8 RGB LED matrix and driver shield attatched to it, along with the Arduino STL library, including <algorithm> and <vector>, and the library Colorduino to control the matrix. Although the library RGB Matrix is theoretically possible to use, it has not worked in practice.
