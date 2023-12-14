// NxN tic-tac-toes
#include <iostream>
#include <cstdlib>
using namespace std;

// The following 3 functions are helper functions to convert
// between 1D and 2D array indices.  The grid itself is stored
// as a 1D array.  However, for printing, checking who won,
// etc. it may be easier to write loops that generate separate
// row/column indices (i.e. treating the array like a 2D array).
// The functions below should be written using the hints in the
// writeup for performing the appropriate arithmetic conversion
// between 1D- and 2D-indices, and can then be called in any
// of the other functions when you find yourself wanting to
// convert one to the other.

/**
 * Helper function - Given the grid array and its dimension
 *    as well as a particular row (r) and column (c), this
 *    function performs the arithmetic to convert r and c
 *    to a 1D index and returns that character in the grid.
 *    For example, for dim = 3 and r = 2, c = 1, this function
 *    should compute the corresponding index: 7 and return
 *    grid[7].
 *
 * Use this function wherever you generate, row/column
 *    indices and want to access that element in the grid.
 *    Pass the row/column indices as r and c, respectively.
 *
 */
char getEntryAtRC(char grid[], int dim, int r, int c);

/**
 * Helper function - Given a 1D index returns the row
 * that corresponds to this 1D index.  Use this in
 * conjunction with idxToCol() anytime you have a 1D index
 * and want to convert to 2D indices.
 */
int idxToRow(int idx, int dim);

/**
 * Helper function - Given a 1D index returns the column
 * that corresponds to this 1D index.  Use this in
 * conjunction with idxToRow() anytime you have a 1D index
 * and want to convert to 2D indices.
 */
int idxToCol(int idx, int dim);


/** Should print the tic-tac-toe board in a nice grid
 *  format as shown below:
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O'
 *         (the letter oh), or '?'.
 */
void printTTT(char grid[], int dim);

/**
 * Should check if any player has won the game yet.
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *
 * Return value:
 *   Should return 1 if 'X' has won, 2 if 'O' has won, or 0 (zero)
 *   if the game is still undecided.
 *
 */
int checkForWinner(char grid[], int dim);


/**
 * Should check if there is no possible way any player can win.
 * More specifically, if there does not exist a row, column,
 * or diagonal that can still have 3 of a kind, then the game
 * will be a draw.
 *
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *
 * Return value:
 *   Return true if no player can win given the current
 *   game state, false otherwise.
 */
bool checkForDraw(char grid[], int dim);

/**
 * @brief Get the Ai Choice for the current player and update grid object
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *   dim: the dim(-ension) of the tic-tac-toe board
 *   player: current player => 'X' or 'O'
 * @return true If an error occurred or a choice was unable to be made
 * @return false If a play was successfully made
 */
bool getAiChoiceAndUpdateGrid(char grid[], int dim, char player);

/**
 * @brief Picks a random location for the current player and update grid
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *   dim: the dim(-ension) of the tic-tac-toe board
 *   player: current player => 'X' or 'O'
 * @return true If no viable location to be played
 * @return false If a play was successfully made
 */
bool getRandChoiceAndUpdateGrid(char grid[], int dim, char player);


/**********************************************************
 *  Write your implementations for each function prototyped
 *  above in the space below
 **********************************************************/
//get value of an array index given row, column
char getEntryAtRC(char grid[], int dim, int r, int c)
{
    return grid[(dim*r) + c];
}
//turn index to row position
int idxToRow(int idx, int dim)
{
    return (idx+1)/dim;
}
//turn index to column position
int idxToCol(int idx, int dim)
{
    return (idx%dim)+1;
}
//print the board
void printTTT(char grid[], int dim)
{
  char* temp = grid;
  for (int r=0;r<dim;r++){
    for(int c=0;c<dim;c++){
      cout<<" "<<getEntryAtRC(temp,dim,r,c);
      //check if not last index of the row
      if (c < dim-1){
        //add a '|' if not last index
        cout<<" |";
      }
      //only do space if is last index
      else{
        cout<<" ";
      }
    }
    cout<<endl;
    //add line separator + new line as long as not last row
    if (r<dim-1){
      //make dashes fit the size of dim
      int bound = 3*dim+(dim-1);
      for(int i=0;i<bound;i++){
        cout<<'-';
      }
      cout<<endl;
    }
  }
}

int checkForWinner(char grid[], int dim)
{
  int status = 0;
  //check each row
  for (int r=0;r<dim;r++){
    //reset count everytime going down new row
    int count_x = 0;
    int count_o = 0;
    for(int c=0;c<dim;c++){
      if (getEntryAtRC(grid,dim,r,c) != '?'){
        if (getEntryAtRC(grid,dim,r,c) == 'X'){
          count_x += 1;
          //set to X wins if all values are X
          if (count_x == dim){
            status = 1;
          }
        }
        else{ 
          count_o += 1;
          //set to O wins if all values are O
          if (count_o == dim){status = 2;}
        }
      }
    }
  }
  //check each column
  for (int c=0;c<dim;c++){
    //reset count when going to new column
    int count_x = 0;
    int count_o = 0;
    for(int r=0;r<dim;r++){
      if (getEntryAtRC(grid,dim,r,c) != '?'){
        if (getEntryAtRC(grid,dim,r,c) == 'X'){
          count_x += 1;
          //set to X wins if all values are X
          if (count_x == dim){status = 1;}
        }
        else{ 
          count_o += 1;
          //set to O wins if all values are O
          if (count_o == dim){status = 2;}
        }
      }
    }
  }
  //check diagonal 1 (top-left to bottom-right)
  int count_o = 0;
  int count_x = 0;
  int* ptr_o = &count_o;
  int* ptr_x = &count_x;
  for (int r=0;r<dim;r++){
    for (int c=0;c<dim;c++){
      if (r == c){
        char entry = getEntryAtRC(grid,dim,r,c);
        if (entry == 'X'){
          *ptr_x +=1;
          if (*ptr_x == dim){status = 1;}
        }
        else if (entry == 'O'){
          *ptr_o +=1;
          if (*ptr_o == dim){status = 2;}
        }
      }
    }
  }
  //check diagnal 2 (top-right to bottom-left)
  //reset the counters back to 0
  *ptr_x = 0;
  *ptr_o = 0;
  int r = 0;
  int c = dim-1;
  for (int i=0;i<dim;i++){
    char entry = getEntryAtRC(grid,dim,r,c);
    if (entry == 'X'){
      *ptr_x +=1;
      if (*ptr_x == dim){status = 1;}
    }
    else if (entry == 'O'){
      *ptr_o +=1;
      if (*ptr_o == dim){status = 2;}
    }
    r++;
    c--;
  }
  return status;
}

bool checkForDraw(char grid[], int dim)
{
  bool draw = false;
  //check each row
  for (int r=0;r<dim;r++){
    int count_x = 0;
    int count_o = 0;
    for (int c=0;c<dim;c++){
      if (getEntryAtRC(grid,dim,r,c) == 'X'){
        count_x += 1;
      }
      else if (getEntryAtRC(grid,dim,r,c) == 'O'){
        count_o += 1;
      }
    }
    //if both x and o count is more than 0, change draw = true for that row
    if (count_x > 0 && count_o > 0){
      draw = true;
    }
    else{
      draw = false;
      //can end check early if a row doesn't have both x and o
      break;}
  }
  //if draw is false after row check, end early
  if (draw == false){
    return draw;
  }
  //check each column
  for (int c=0;c<dim;c++){
    int count_x = 0;
    int count_o = 0;
    for (int r=0;r<dim;r++){
      if (getEntryAtRC(grid,dim,r,c) == 'X'){
        count_x += 1;
      }
      else if (getEntryAtRC(grid,dim,r,c) == 'O'){
        count_o += 1;
      }
    }
    //if both x and o is 1+, change draw = true for that col
    if (count_x > 0 && count_o > 0){
      draw = true;
    }
    else{
      draw = false;
      //end col check early if a column doesn't have both x and o yet
      break;
    }
  }
  //end check early if column doesn't have both x and o yet
  if (draw==false){
    return draw;
  }
  //check diagonal 1 (top-left to bottom-right)
  int count_o = 0;
  int count_x = 0;
  int* ptr_o = &count_o;
  int* ptr_x = &count_x;
  for (int r=0;r<dim;r++){
    for (int c=0;c<dim;c++){
      if (r == c){
        char entry = getEntryAtRC(grid,dim,r,c);
        if (entry == 'X'){*ptr_x +=1;}
        else if (entry == 'O'){*ptr_o +=1;}
      }
    }
  }
  //before next diagonal, check draw condition
  if (count_x > 0 && count_o > 0){draw = true;}
  else{draw = false;}
  //check diagonal 2 (top-right to bottom-left)
  //reset counters to 0
  *ptr_x = 0;
  *ptr_o = 0;
  int r = 0;
  int c = dim-1;
  for (int i=0;i<dim;i++){
    char entry = getEntryAtRC(grid,dim,r,c);
    if (entry == 'X'){*ptr_x +=1;}
    else if (entry == 'O'){*ptr_o +=1;}
    r++;
    c--;
  }
  //check draw condition
  if (count_x > 0 && count_o > 0){draw = true;}
  else{draw = false;}
  //return the final value of draw
  return draw;
}

bool getAiChoiceAndUpdateGrid(char grid[], int dim, char player)
{
  //check rows for win
  for (int r=0;r<dim;r++){
    //reset count everytime going down new row
    int count_x = 0;
    int count_o = 0;
    for(int c=0;c<dim;c++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //count up the x and o on a row
      if (entry == 'X'){
        count_x += 1;}
      else if (entry == 'O'){ 
        count_o += 1;}
    }
    //handles when x almost wins
    if (count_x == dim-1){
      for (int c=0;c<dim;c++){
        char entry = getEntryAtRC(grid,dim,r,c);
        //if current player is X, go for winning spot
        if (entry=='?' && player =='X'){
          grid[(dim*r) + c] = player;
          return false;}
        //if current player is O, block winning spot
        else if (entry=='?'&&player=='O'){
          grid[(dim*r) + c] = player;
          return false;}
      }
    }
    //handles when o almost wins
    else if (count_o == dim-1){
      for (int c=0;c<dim;c++){
        char entry = getEntryAtRC(grid,dim,r,c);
        //if current player is O, go for winning spot
        if (entry=='?' && player =='O'){
          grid[(dim*r) + c] = player;
          return false;}
        //if current player is X, block winning spot
        else if (entry=='?'&&player=='X'){
          grid[(dim*r) + c] = player;
          return false;}
      }        
    }
  }
  //check columns for win
  for (int c=0;c<dim;c++){
    //reset count when going to new column
    int count_x = 0;
    int count_o = 0;
    for(int r=0;r<dim;r++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //count up the x and o on a column
      if (entry == 'X'){
        count_x += 1;}
      else if (entry == 'O'){ 
        count_o += 1;}
    }
    //handles when x almost wins
    if (count_x == dim-1){
      for (int r=0;r<dim;r++){
        char entry = getEntryAtRC(grid,dim,r,c);
        //if current player is X, go for winning spot
        if (entry=='?' && player =='X'){
          grid[(dim*r) + c] = 'X';
          return false;}
        //if current player is O, block winning spot
        else if (entry=='?'&&player=='O'){
          grid[(dim*r) + c] = 'O';
          return false;}
      }
    }
    //handles when o almost wins
    else if (count_o == dim-1){
      for (int r=0;r<dim;r++){
        char entry = getEntryAtRC(grid,dim,r,c);
        //if current player is O, go for winning spot
        if (entry=='?' && player =='O'){
          grid[(dim*r) + c] = player;
          return false;}
        //if current player is X, block winning spot
        else if (entry=='?'&&player=='X'){
          grid[(dim*r) + c] = player;
          return false;}
      }        
    }
  }  
  //check diagonal1 for win
  int count_o = 0;
  int count_x = 0;
  //count up x and o spot of diagonal1
  for (int r=0;r<dim;r++){
    for (int c=0;c<dim;c++){
      if (r == c){
        char entry = getEntryAtRC(grid,dim,r,c);
        if (entry == 'X'){
          count_x +=1;}
        else if (entry == 'O'){
          count_o +=1;}
      }
    }
  }
  //handles when x almost wins
  if (count_x == dim-1){
    int r = 0;
    int c = 0;
    for (int i=0;i<dim;i++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //if current player is X, go for winning spot
      if (entry=='?' && player =='X'){
        grid[(dim*r) + c] = 'X';
        return false;}
      //if current player is O, block winning spot
      else if (entry=='?'&&player=='O'){
        grid[(dim*r) + c] = 'O';
        return false;}
      r++;
      c++;
    }
  }
  //handles when o almost wins
  else if (count_o == dim-1){
    int r = 0;
    int c = 0;
    for (int i=0;i<dim;i++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //if current player is O, go for winning spot
      if (entry=='?' && player =='O'){
        grid[(dim*r) + c] = player;
        return false;}
      //if current player is X, block winning spot
      else if (entry=='?'&&player=='X'){
        grid[(dim*r) + c] = player;
        return false;}
      r++;
      c++;
    }
  } 
  //check diagonal2 for win
  //reset counters
  count_x = 0;
  count_o = 0;
  int r = 0;
  int c = dim-1;
  //count up x and o of diagonal2
  for (int i=0;i<dim;i++){
    char entry = getEntryAtRC(grid,dim,r,c);
    if (entry == 'X'){
      count_x +=1;}
    else if (entry == 'O'){
      count_o +=1;}
    r++;
    c--;
  }
  //handles when x almost wins
  if (count_x == dim-1){
    int r = 0;
    int c = dim-1;
    for (int i=0;i<dim;i++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //if current player is X, go for winning spot
      if (entry=='?' && player =='X'){
        grid[(dim*r) + c] = 'X';
        return false;}
      //if current player is O, block winning spot
      else if (entry=='?'&&player=='O'){
        grid[(dim*r) + c] = 'O';
        return false;}
      r++;
      c--;
    }
  }
  //handles when o almost wins
  else if (count_o == dim-1){
    int r = 0;
    int c = dim-1;
    for (int i=0;i<dim;i++){
      char entry = getEntryAtRC(grid,dim,r,c);
      //if current player is O, go for winning spot
      if (entry=='?' && player =='O'){
        grid[(dim*r) + c] = player;
        return false;}
      //if current player is X, block winning spot
      else if (entry=='?'&&player=='X'){
        grid[(dim*r) + c] = player;
        return false;}
      r++;
      c--;
    }
  }
  //else, randomize an available spot
  getRandChoiceAndUpdateGrid(grid,dim,player);
  //if no viable spot
  return true;
}


// Complete...Do not alter
bool getRandChoiceAndUpdateGrid(char grid[], int dim, char player)
{
    int start = rand()%(dim*dim);
    // look for an open location to play based on random starting location.
    // If that location is occupied, move on sequentially until wrapping and
    // trying all locations
    for(int i=0; i < dim*dim; i++) {
        int loc = (start + i) % (dim*dim);
        if(grid[ loc ] == '?') {
            grid[ loc ] = player;
            return false;
        }
    }
    // No viable location
    return true;
}


/**********************************************************
 *  Complete the indicated parts of main(), below.
 **********************************************************/
int main()
{
    // This array holds the actual board/grid of X and Os. It is sized
    // for the biggest possible case (11x11), but you should only
    // use dim^2 entries (i.e. if dim=3, only use 9 entries: 0 to 8)
    char tttdata[121];

    // dim stands for dimension and is the side length of the
    // tic-tac-toe board i.e. dim x dim (3x3 or 5x5).
    int dim;
    int seed;
    // Get the dimension from the user
    cin >> dim >> seed;
    srand(seed);

    int dim_sq = dim*dim;

    for(int i=0; i < dim_sq; i++) {
        tttdata[i] = '?';
    }
    // Print one of these messages when the game is over
    // and before you quit
    const char xwins_msg[] = "X player wins!";
    const char owins_msg[] = "O player wins!";
    const char draw_msg[] =  "Draw...game over!";

    bool done = false;
    char player = 'X';
    // Show the initial starting board
    printTTT(tttdata, dim);
    while(!done) {

        //**********************************************************
        // Get the current player's input (i.e. the location they want to
        // choose to place their mark [X or O]) or choice of AI or Random
        // location and update the tttdata array.
        // Be sure to follow the requirements defined in the guide/writeup
        // for quitting immediately if the user input is out-of-bounds
        // (i.e. not in the range 0 to dim_sq-1 nor -1 nor -2) as well as
        // continuing to prompt for an input if the user chooses locations
        // that have already been chosen (already marked with an X or O).
        //**********************************************************

        // Add your code here for getting input
        //Start of new turn
        int choice;
        cout<<"Player "<<player<<" enter your square choice [0-"<<dim*dim-1<<"], -1 (AI), or -2 (Random):"<<endl;
        cin >> choice;
        if (choice <-2 || choice >dim*dim -1){break;}
        else if (choice == -1){
          getAiChoiceAndUpdateGrid(tttdata,dim,player);
          printTTT(tttdata,dim);
          int win = checkForWinner(tttdata,dim);
          bool draw = checkForDraw(tttdata,dim);
          if (win == 1){
            cout << xwins_msg << endl;
            break;}
          else if (win == 2){
            cout << owins_msg << endl;
            break;}
          if (draw == true){
            cout << draw_msg << endl;
            break;}
          //change player's turn
          if (player == 'X'){
            player = 'O';
          }   
          else{player='X';} 
        }
        else if (choice == -2){
          getRandChoiceAndUpdateGrid(tttdata,dim,player);
          printTTT(tttdata,dim);
          int win = checkForWinner(tttdata,dim);
          bool draw = checkForDraw(tttdata,dim);
          //check win and draw condition
          if (win == 1){
            cout << xwins_msg << endl;
            break;}
          else if (win == 2){
            cout << owins_msg << endl;
            break;}
          if (draw == true){
            cout << draw_msg << endl;
            break;}
          //change player's turn
          if (player == 'X'){
            player = 'O';
          }   
          else{player='X';}       
        }
        //if choice in valid dim range
        else{
          //loop runs as long as value of choice square is not '?'
          while (tttdata[choice]!='?'){
            cout<<"Player "<<player<<" enter your square choice [0-"<<dim*dim-1<<"], -1 (AI), or -2 (Random):"<<endl;
            cin >> choice;}
          tttdata[choice] = player;
        // Show the updated board if the user eventually chose a valid location
        // (i.e. you should have quit the loop and program by now without any
        //  other output message if the user chosen an out-of-bounds input).
          printTTT(tttdata, dim);
        //**********************************************************
        // Complete the body of the while loop to process the input that was just
        //  received to check for a winner or draw and update other status, as
        //  needed.
        //
        // To match our automated checkers' expected output, you must output
        // one of the messages defined above using *one* of the cout commands
        // (under the appropriate condition) below:
        //   cout << xwins_msg << endl;  OR
        //   cout << owins_msg << endl;  OR
        //   cout << draw_msg << endl;
        //
        // Note: Our automated checkers will examine a specific number of lines
        //  at the end of the program's output and expect to see the updated board
        //  and game status message.  You may certainly add some debug print
        //  statements during development but they will need to be removed to
        //  pass the automated checks.
        //**********************************************************
          int win = checkForWinner(tttdata,dim);
          bool draw = checkForDraw(tttdata,dim);
        //check win and draw condition
          if (win == 1){
            cout << xwins_msg << endl;
            break;  
          }
          else if (win == 2){
            cout << owins_msg << endl;
            break;
          }
          if (draw == true){
            cout << draw_msg << endl;
            break;
          }
          //switch player's turn
          if (player == 'X'){
            player = 'O';
          }
          else{player = 'X';}
        } 
    }  
    // end while loop
    return 0;
}