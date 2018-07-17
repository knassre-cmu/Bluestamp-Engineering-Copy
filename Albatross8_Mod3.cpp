#include <ArduinoSTL.h>
#include <algorithm>
#include <vector>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 6
#define NUMPIXELS 64

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int directions[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}; // 8 directions in cartesian

int gameboard[8][8] = { // 8 x 8 array that stores states of each LED
  {4,4,4,4,4,4,4,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,2,1,0,0,4},
  {4,0,0,1,2,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,0,0,0,0,0,0,4},
  {4,4,4,4,4,4,4,4}};
  
struct destination {
  int r; 
  int c;};

destination destiny;

int tempboard[8][8];
int turn = 1;
std::vector<int> novoid = {0};
int theta[2];

int NeoRow(int n){return (n - (n % 8)) / 8;}

int NeoCol(int n){return n % 8;}

int NeoAdr(int r, int c){return (r * 8) + c;}

void display_board(int board[8][8]){ // Updates LEDs to match gameboard or prints the gameboard on the serial monitor
  for(int B = 0; B < 64; B++){
    if (board[NeoRow(B)][NeoCol(B)] == 0){pixels.setPixelColor(B, pixels.Color(0,0,0));}
    if (board[NeoRow(B)][NeoCol(B)] == 1){pixels.setPixelColor(B, pixels.Color(0,10,00));}
    if (board[NeoRow(B)][NeoCol(B)] == 2){pixels.setPixelColor(B, pixels.Color(0,00,10));}}
  pixels.show();}

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
  /*display_board(gameboard);*/}

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

void next_board(int player, int row, int column, int board[8][8]){ // A version of the board where a certain move has been made
  for (int R = 0; R < 7; R++){
    for (int C = 0; C < 7; C++){
      tempboard[R][C] = board[R][C];}} // The variable that reflects the future board
  tempboard[row][column] = player;
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
  if (depth > 9){return false;} // Prevents infinate loops
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

int arct(int player, int board[8][8]){ // Evaluates how good a board is for a player
  int w_one = safe_discs(player,board);
  int w_two = wedge_discs(player,board);
  int w_three = frontier(player,board);
  int w_four = calculate_score(player,board);
  return (64 * w_one) + (16 * w_two) - w_three + w_four;}

bool arct_bool(int player, int board[8][8], int a, int b){
  next_board(player, (a - (a % 10)) / 10, a % 10, board);
  int AAA[8][8];
  for (int Y = 0; Y < 6; Y++){
    for(int X = 0; X < 6; X++){
      AAA[Y][X] = tempboard[Y][X];}}
      
  next_board(player, (b - (b % 10)) / 10, b % 10, board);
  
  return arct(player, AAA) > arct(player, tempboard);}

int arct_sort(int player, std::vector<int> moves, int board[8][8]){
  std::vector<int> trin = moves;
  std::sort(trin.begin(), trin.end(), [&](int a, int b) {return arct_bool(player, gameboard, a, b);});
  
  return trin.at(0);}

void setup() {
  Serial.begin(9600);
  delay(1000);
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  display_board(gameboard);
  lcd.begin(16,2);
  lcd.clear();
  delay(1000);
 }

void loop() {
  
  int serialData = Serial.read();

  if (valid_moves(1,gameboard).size() != 0 || valid_moves(2,gameboard).size() != 0){
    lcd.clear();
    lcd.print("Player: ");
    lcd.print(calculate_score(1, gameboard));
    lcd.setCursor(0,2);
    lcd.print("Computer: ");
    lcd.print(calculate_score(2, gameboard));
    
    for (int V = 0; V < 20; V++){Serial.println();}
    display_board(gameboard);
    Serial.println();
    Serial.println();
    
    if (turn == 2) {
      Serial.println("Computer's turn");
      Serial.println();
      Serial.println();
      delay(1000);
      std::vector<int> avail = valid_moves(2,gameboard);
      if (avail.size() == 0){turn = 1;}
      int wise = arct_sort(2,avail,gameboard);
      theta[0] = (wise - (wise % 10)) / 10;
      theta[1] = wise % 10;
      make_move(2,theta);
      turn = 1;}
    
    else{if (turn == 1) {
      std::vector<int> possible = valid_moves(1,gameboard);
      if (possible.size() == 0){turn = 2;}
      else {
        Serial.println("Possible Moves:");
        for (int U = 0; U < possible.size(); U++){
          int umm = possible.at(U);
          Serial.print("Type ");
          Serial.print(char(U + 97));
          Serial.print(" to move to (");
          Serial.print((umm - (umm % 10)) / 10);
          Serial.print(", ");
          Serial.print(umm % 10);
          Serial.println(")");;}
        delay(2000);
        if (serialData != -1 && serialData > 96 && serialData < possible.size() + 98){
            theta[0] = (possible.at(serialData-97) - (possible.at(serialData-97) % 10)) / 10;
            theta[1] = possible.at(serialData-97) % 10;
            make_move(1,theta);
            turn = 2;}}}}}
     
     else{
      if (calculate_score(1, gameboard) > calculate_score(2, gameboard)){
        char ending[50]  = {"Player won!         "};
        for (int V = 0; V < 16; V++){
          lcd.clear();
          lcd.print("Game Over");
          lcd.setCursor(0,2);
          for (int J = V; J < 16; J++){
            lcd.print(ending[J]);}
          for (int K = 0; K < V; K++){
            lcd.print(ending[K]);}
        delay(400);}}
        
      if (calculate_score(1, gameboard) < calculate_score(2, gameboard)){
        char ending[50] = {"Computer won!         "};
        for (int V = 0; V < 16; V++){
          lcd.clear();
          lcd.print("Game Over");
          lcd.setCursor(0,2);
          for (int J = V; J < 14; J++){
            lcd.print(ending[J]);}
          for (int K = 0; K < V; K++){
            lcd.print(ending[K]);}
        delay(400);}}
        
      if (calculate_score(1, gameboard) == calculate_score(2, gameboard)){
        char ending[50]  = {"It's a tie!         "};
        for (int V = 0; V < 16; V++){
          lcd.clear();
          lcd.print("Game Over");
          lcd.setCursor(0,2);
          for (int J = V; J < 16; J++){
            lcd.print(ending[J]);}
          for (int K = 0; K < V; K++){
            lcd.print(ending[K]);}
          delay(400);}}}}

            
