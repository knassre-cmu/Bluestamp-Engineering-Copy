/*
Uses an 8x8 RGB LED matrix and the arduino library Colorduino to display the gameboard. 
Has all basic functionality for playing a game of 6x6 Othello except for the player interface.
Both players are the computer making a pseudo-random move.
*/

#include <Colorduino.h>
#include <ArduinoSTL.h>
#include <algorithm>
#include <vector>

int directions[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}; 
// 8 directions in cartesian

int gameboard[8][8] = { // 8 x 8 array that stores states of each LED
  {4,4,4,4,4,4,4,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,2,1,0,0,4},
  {4,0,0,1,2,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,4,4,4,4,4,4,4}};

int v1[8][8] = { // Displays if blue has won
  {4,4,4,4,4,4,4,4},
  {4,1,1,1,1,1,1,4},
  {4,1,1,1,1,1,1,4},
  {4,1,1,1,1,1,1,4},
  {4,1,1,1,1,1,1,4},
  {4,1,1,1,1,0,1,4},
  {4,1,1,1,1,1,1,4},
  {4,4,4,4,4,4,4,4}};

int v2[8][8] = { // Displays if red has won
  {4,4,4,4,4,4,4,4},
  {4,2,2,2,2,2,2,4},
  {4,2,2,2,2,2,2,4},
  {4,2,2,2,2,2,2,4},
  {4,2,2,2,2,2,2,4},
  {4,2,2,2,2,2,2,4},
  {4,2,2,2,2,2,2,4},
  {4,4,4,4,4,4,4,4}};
  
struct destination {
  int r; 
  int c;};

destination destiny;

auto tempboard = gameboard;
int turn = 1;
int ticker = 4;
std::vector<int> novoid = {0};
int theta[2];

void display_board(int board[8][8]) { // Updates LEDs to match gameboard
  int x; int y;
  for(y = 0; y < 8; y++)
    for(x = 0; x < 8; x++) {{
        if(board[y][x] == 1){Colorduino.SetPixel(y, x, 0, 0, 200);}  // 1 = Blue
        if(board[y][x] == 2){Colorduino.SetPixel(y, x, 100, 0, 0);}  // 2 = Red
        if(board[y][x] == 3){Colorduino.SetPixel(y, x, 0, 100, 0);}  // 3 = Green
        if(board[y][x] == 0 ){Colorduino.SetPixel(y, x, 0, 0, 0);}}} // 0 = Black   
  Colorduino.FlipPage();}

bool reversi_recursi(int row, int column, int player, int delta, int board[8][8]) { // Finds sandwiches
  int M = row;
  int N = column;
  int warp = 0;
  int* direct = directions[delta]; // Direction that the function points in
  while (M <= 10){
    int cur = board[M][N];
    int adjx = M+direct[1];
    int adjy = N+direct[0];
    int adj = board[adjx][adjy]; // Value of the adjacent tile
    int adjcord[2] = {adjx,adjy};
    if (adj == 4 || adj == 0){ // If proof that this is not a sandwich, return dud
      return false;}
    if (adj == player){ // If proof that this is a sandwich, return end of sandwich
      if (warp == 0){
        return false;}
      else{
        return true;}}
    M += direct[1]; // If neither, keep pointing in direction DIRECT
    N += direct[0];
    warp += 1;}
    return false;}

bool valid_move(int player, int Y, int X, int board[8][8]){ // Determines if a move is a valid Othello move
  if (gameboard[Y][X] != 0){ // Checks if the tile is empty
    return false;}
  int temp = 0;
  for(int i = 0; i < 8; i++){ // Counts how many directions produce sandwiches
    if(reversi_recursi(Y,X,player,i,board)){
      temp += 1;}}
  if (temp > 0){
    return true;}
  return false;}

void make_move(int player, int cord[2]) { // Function that is called when a move is made
  if (valid_move(player,cord[0],cord[1],gameboard)){
    gameboard[cord[0]][cord[1]] = player; // Update the coordinate that is selected
    good_directions(player,cord[0],cord[1],gameboard);} // Flip any sandwiched pieces
  display_board(gameboard);
  delay(1000);}

void good_directions(int player,int row,int column,int board[8][8]){ // Points to each direction with a sandwich
   int D;
   for(D = 0; D < 8; D++){
    if (reversi_recursi(row,column,player,D,board)) { // Proof that this direction has a sandwich
      toBeFlipped(board,player,row,column,D);}}} 

void toBeFlipped(int board[8][8], int player, int a, int b, int direct){ // Flips all pieces in a sandwich
  int m = a;
  int n = b;
  int dirx = directions[direct][0];
  int diry = directions[direct][1];
  m += diry;
  n += dirx;
  while (gameboard[m][n] != player){ // Flip all pieces between start  (a,b) and the next tile of the same color
    gameboard[m][n] = player;
    m += diry; // Move to the next tile in direction D
    n += dirx;}}

int choice(std::vector<int> input){ // Chooses a random item from a array
  return input[random(100,1000) % leng(input)];}

int leng(std::vector<int> input){ // Returns the length of an array
  int sum = 0;
  std::for_each(input.begin(),input.end(),
  [&](int x){
  sum += 1;});
  return sum;}

void next_board(int player, int* cord, int board[8][8]){ // A version of the board where a certain move has been made
  tempboard = board; // The variable that reflects the future board
  int row = cord[0];
  int column = cord[1];
  tempboard[row][column] = player;
  int** temp;
  int D;
  for(D = 0; D < 8; D++){ // Repeats the functions of make_move but with tempboard
    if (reversi_recursi(row,column,player,D,board)) {
      int m = row;
      int n = column;
      int dirx = directions[D][0];
      int diry = directions[D][1];
        m += diry;
        n += dirx;
      while (gameboard[m][n] != player){
        m += diry;
        n += dirx;
        tempboard[m][n] = player;}}}}
        
int calculate_score(int player, int board[8][8]){ // Calculates how many pieces a player has
  int temp = 0;
  for(int R = 0; R < 8; R++){
    for(int C = 0; C < 8; C++){
      if (board[R][C] == player){
        temp += 1;}}}
  return temp;} 

std::vector<int> valid_moves(int player,int board[8][8]){ // Lists all possible moves for a player as intergers
  std::vector<int> beta;
  for(int R = 0; R < 8; R++){
    for(int C = 0; C < 8; C++){
      if (valid_move(player,R,C,board)){
        beta.push_back(10 * R + C);}}}
   return beta;}

bool contains(std::vector<int> input, int value){ // Checks if an element is in a vector
  for (int J = 0; J < leng(input); J++){
    if (input[J] == value){return true;}}
  return false;}

void pick_move(int player, int board[8][8]){ // Picks a "random" valid move
  int L = valid_moves(player,board).size();
  int target = random(0,L); // Picks a "random" number
  int out = valid_moves(player,board).at(target); // Uses the "random" number to pick a "random" move
  destiny.r = (out - (out % 10))/10; // Row of a random valid move
  destiny.c = out % 10;} // Column of a random valid move

int safe_discs(int player, int board[8][8]){ // Counts the number of safe discs a player has
  int s_score = 0;
  for(int y = 0; y < 8; y++){
    for(int x = 0; x < 8; x++) {
      if (safe(player,y,x,novoid,board,0)){
        s_score += 1;}}}
  return s_score;}

bool safe(int player, int R, int C, std::vector<int> visited, int board[8][8], int depth){ // Calculates if a disc is safe
  if (depth > 10){return false;} // Prevents infinate loops
  if (board[R][C] != player){return false;} // If not in player's control, not safe
  if ((R == 1 || R == 6) && (C == 1 || C == 6)){return true;} // Corners are always safe
  
  int current = 10 * R + C;
  if (contains(visited, current)){return true;} // Prevents checking pieces that have alraedy made recursive calls
  std::vector<int> updated = visited;
  updated.push_back(current);

  if (R == 1 || R == 6){return (safe(player, R, C+1, updated, board, depth + 1) or safe(player, R, C-1, updated, board, depth + 1));} // Is this row safe
  if (C == 1 || C == 6){return (safe(player, R+1, C, updated, board, depth + 1) or safe(player, R-1, C, updated, board, depth + 1));} // Is this column safe
  
  bool D1 = safe(player, R - 1, C - 1, updated, board, depth + 1); // Checks if the piece in direction ____ safe
  bool D2 = safe(player, R - 1, C + 0, updated, board, depth + 1);
  bool D3 = safe(player, R - 1, C + 1, updated, board, depth + 1);
  bool D4 = safe(player, R + 0, C + 1, updated, board, depth + 1);
  bool D5 = safe(player, R + 1, C + 1, updated, board, depth + 1);
  bool D6 = safe(player, R + 1, C + 0, updated, board, depth + 1);
  bool D7 = safe(player, R + 1, C - 1, updated, board, depth + 1);
  bool D8 = safe(player, R + 0, C - 1, updated, board, depth + 1);

  bool supercheck = // Checks to see if four adjacent pieces are also safe
     (D1 and D2 and D3 and D4) or (D5 and D2 and D3 and D4) 
  or (D5 and D6 and D3 and D4) or (D5 and D6 and D7 and D4) 
  or (D5 and D6 and D7 and D8) or (D1 and D6 and D7 and D8) 
  or (D1 and D2 and D8 and D8) or (D1 and D2 and D3 and D8);
  
  return supercheck;}

int wedge_discs(int player, int board[8][8]){ // Counts the number of wedges a player has
  int w_score = 0;
  int opposite;
  if (player == 1){opposite = 2;}
  else {opposite = 1;}
  
  for (int H = 2; H < 6; H++){
    if (gameboard[H][1] == player){
      if(gameboard[H + 1][1] == opposite && gameboard[H - 1][1] == opposite){ // Checks for a wedge on the left side
        w_score += 1;}}
    if (gameboard[H][6] == player){
      if(gameboard[H + 1][6] == opposite && gameboard[H - 1][6] == opposite){ // Checks for a wedge on the right side
        w_score += 1;}}
    if (gameboard[1][H] == player){
      if(gameboard[1][H + 1] == opposite && gameboard[1][H - 1] == opposite){ // Checks for a wedge on the top side
        w_score += 1;}}
    if (gameboard[6][H] == player){
      if(gameboard[6][H + 1] == opposite && gameboard[6][H - 1] == opposite){ // Checks for a wedge on the bottom side
        w_score += 1;}}}
    return w_score;}

int frontier(int player, int board[8][8]){ // Calculates teh frontier score of a player
  int f_score = 0;
  for(int y = 0; y < 8; y++){
    for(int x = 0; x < 8; x++) {
      if (board[y][x] == player){
        for (int D = 0; D < 8; D++){ // Counts the exposed sides from each piece that the player controls
          if (gameboard[y+directions[D][1]][x+directions[D][0]] == 0) {
            f_score += 1;}}}}}     
  return f_score;}

int arcturus(int player, int board[8][8]){ // Evaluates how good a board is for a player
  int w_one = safe_discs(player,board);
  int w_two = wedge_discs(player,board);
  int w_three = frontier(player,board);
  int w_four = calculate_score(player,board);
  return (64 * w_one) + (16 * w_two) - w_three + w_four;}

void setup() {
  delay(1000);
  Colorduino.Init();
  display_board(gameboard);
  delay(1000);
 }
 
void loop() {
  if (ticker < 32){
    pick_move(turn,gameboard);
    theta[0] = destiny.r;
    theta[1] = destiny.c;
    make_move(turn,theta);
    if (turn == 1) {turn++;}
    else {turn--;}
    ticker += 1;
    display_board(gameboard);}
  else {  
    delay(2000);
    if (calculate_score(1,gameboard) > calculate_score(2,gameboard)){display_board(v1);}
    else {display_board(v2);}}
  }
