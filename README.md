# Albatross-8

Code to play a game of 6 x 6 Othello where player 1 (blue) is a human and player 2 is a computer algorithm. Player 1 can see their possible moves on the serial monitor and type a number corrsponding with a move to make it. Every few seconds, the Arduino checks to see if a move has been typed. If a move has been typed, it makes hte move then player 2 takes a turn. If not, the serial monitor re-displays the possible moves.

The most recent version of the code <Mod1> is set to have player 1 be a human player who interacts with the serial monitor to make moves and player 2 be a computer algorithm making intelligent moves. This version also includes code to control an LCD display.

To be used on an Arduino with an *8 x 8 RGB LED matrix and driver shield attatched to it, along with the Arduino STL library, including <algorithm> and <vector>, and the library <colorduino> to control the matrix. Although the library RGB Matrix is theoretically possible to use, it has not worked in practice. Also has an LCD display controlled with an I2C and the libraries <wire> and <liquidcrystal_I2C>.

* However, at this juncture, the code to control the RGB matrix has been temporarily replaced with code to print the gameboard on the serial monitor, pending a neopixel matrix to replace the RGM matrix. <colorduino> will be replaced with the neopixel library when this happens.
