# Albatross-8

Code to play a game of 6 x 6 Othello where player 1 (blue) is a human and player 2 is a computer algorithm. Player 1 can see their possible moves on the serial monitor and type a number corrsponding with a move to make it. Every few seconds, the Arduino checks to see if a move has been typed. If a move has been typed, it makes hte move then player 2 takes a turn. If not, the serial monitor re-displays the possible moves.

The most recent version of the code (Mod 4) is set to have player 1 be a human player who interacts with the serial monitor to make moves and player 2 be a computer algorithm making intelligent moves. This version also includes code to control an LCD display that shows the score, declares a winner, and displays taunting messages.

To be used on an Arduino with an 8x8 Adafruit Neopixel Matrix, along with the Arduino STL library, including Algorithm and Vector, and the library Adafruit_NeoPixel.h to control the matrix, along with an LCD display controlled with an I2C and the libraries Wire and liquidcrystal_I2C.
