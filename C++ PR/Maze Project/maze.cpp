/*
maze.cpp
*/

#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// Add other prototypes here for any functions you wish to use


// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
    int rows, cols, result;
    char** mymaze=NULL;
    const char* invalid_char_message = "Error, invalid character.";
    const char* invalid_maze_message = "Invalid maze.";
    const char* no_path_message = "No path could be found!";

    if(argc < 2)
    {
        cout << "Please provide a maze input file" << endl;
        return 1;
    }

    mymaze = read_maze(argv[1], &rows, &cols);
    // For checkpoint 2 you should check the validity of the maze
    // You may do so anywhere you please and can abstract that
    // operation with a function or however you like.
    
    // Check for invalid maze early so no problem in maze_search()
    // Handles invalid file name
    if (mymaze == NULL){
      cout << invalid_maze_message << endl;
      return 0;
    }
    // Count up the S and F in the maze
    int s_count = 0;
    int f_count = 0;
    for (int r=0;r<rows;r++){
      for(int c=0;c<cols;c++){
            if(mymaze[r][c] == 'S'){
                s_count ++;
            }
            else if(mymaze[r][c] == 'F'){
                f_count ++;
            }        
      }
    }
    //check the s_count and f_count for invalid maze
    if(s_count != 1){
      cout << invalid_maze_message << endl;
      // delete the arrays in mymaze[]
      for (int r=0;r<rows;r++){
        delete [] mymaze[r];
      }
      // delete the arrays of mymaze
      delete [] mymaze;      
      return 0;
    }
    else if(f_count != 1){
      cout << invalid_maze_message << endl;
      // delete the arrays in mymaze[]
      for (int r=0;r<rows;r++){
        delete [] mymaze[r];
      }
      // delete the arrays of mymaze
      delete [] mymaze;
      return 0;
    }

    //================================
    // When working on Checkpoint 4, you will need to call maze_search
    // and output the appropriate message or, if successful, print
    // the maze.  But for Checkpoint 1, we print the maze, regardless.
    result = maze_search(mymaze,rows,cols);
    //check no path
    if(result == 0){
      cout << no_path_message << endl;
    }
    // check invalid character maze
    for (int r=0;r<rows;r++){
        for (int c=0;c<cols;c++){
            if(result == -1){
                cout << invalid_char_message << endl;
            }
        }
    }
    // print out filled maze if valid path found
    if(result == 1){
      print_maze(mymaze,rows,cols);
    }
    
    //================================
    // ADD CODE BELOW 
    // to delete all memory that read_maze allocated: CHECKPOINT 2

    // delete the arrays in mymaze[]
    for (int r=0;r<rows;r++){
        delete [] mymaze[r];
    }
    // delete the arrays of mymaze
    delete [] mymaze;
    return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 * NOTE: don't forget to deallocate memory in here too!
 *************************************************/
int maze_search(char** maze, int rows, int cols)
{
    // *** You complete **** CHECKPOINT 4
    // iterate through the maze
    for (int r=0;r<rows;r++){
        for (int c=0;c<cols;c++){
            //check for invalid character
            if(maze[r][c] != '.' && maze[r][c] != '#' && 
            maze[r][c] != 'S' && maze[r][c] != 'F'){
                return -1;
            }
        }
    }
    //create Queue
    Queue q(rows*cols);
    //create predecessor array
    Location** p_array = new Location*[rows*cols];
    //prefill p_array with default values
    for (int r=0;r<rows;r++){
      p_array[r] = new Location[cols];
    }
    for (int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            //p_array[r][c] = Location;
            p_array[r][c].row = -1;
            p_array[r][c].col = -1;
        }
    }
    //create explored array
    bool** e_array = new bool*[rows*cols];
    //prefill e_array with default values, 0 -> unexplored
    for (int r=0;r<rows;r++){
      e_array[r] = new bool[cols];
    }
    for (int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            e_array[r][c] = false;
        }
    }
    //find the start Location
    Location start;
    for (int r=0;r<rows;r++){
      for (int c=0;c<cols;c++){
        //if it is S symbol, set it to start coordinates
        if(maze[r][c] == 'S'){
          start.row = r;
          start.col = c;
        }
      }
    }
    //mark start location as explored
    e_array[start.row][start.col] = true;
    //add start location to queue
    q.add_to_back(start);
    //path tracker
    Location track;
    track.row = -1;
    track.col = -1;
    //BFS alogrithm
    while (!q.is_empty()){
      Location curr = q.remove_from_front();
      // if curr is 'F', set track to curr and leave loop
      if(maze[curr.row][curr.col] == 'F'){
          track.row = curr.row;
          track.col = curr.col;
          break;
        }
      //check north location
      if(curr.row-1 >=0 && (maze[curr.row-1][curr.col] == '.'
      || maze[curr.row-1][curr.col] == 'F') 
      && e_array[curr.row-1][curr.col] == false){
        //create Location only if fits requirements;
        Location north;
        north.row = curr.row-1;
        north.col = curr.col;
        //Mark neighbor explored
        e_array[north.row][north.col] = true;
        //Add neighbor to queue
        q.add_to_back(north);
        //Set predecessor of neighbor to curr
        p_array[north.row][north.col].row = curr.row;
        p_array[north.row][north.col].col = curr.col;
      }
      //check west location
      if((curr.col-1 >=0) && (maze[curr.row][curr.col-1] == '.'
      || maze[curr.row][curr.col-1] == 'F')
      && e_array[curr.row][curr.col-1] == false){
        //create Location only if fits requirements;
        Location west;
        west.row = curr.row;
        west.col = curr.col-1;
        //Mark neighbor explored
        e_array[west.row][west.col] = true;
        //Add neighbor to queue
        q.add_to_back(west);
        //Set predecessor of neighbor to curr
        p_array[west.row][west.col].row = curr.row;
        p_array[west.row][west.col].col = curr.col;
      }
      //check south location
      if((curr.row+1<rows) && (maze[curr.row+1][curr.col] == '.'
      || maze[curr.row+1][curr.col] == 'F')
      && e_array[curr.row+1][curr.col] == false){
        //create Location only if fits requirements;
        Location south;
        south.row = curr.row+1;
        south.col = curr.col;
        //Mark neighbor explored
        e_array[south.row][south.col] = true;
        //Add neighbor to queue
        q.add_to_back(south);
        //Set predecessor of neighbor to curr
        p_array[south.row][south.col].row = curr.row;
        p_array[south.row][south.col].col = curr.col;
      }
      //check east location
      if((curr.col+1<cols) && (maze[curr.row][curr.col+1] == '.'
      || maze[curr.row][curr.col+1] == 'F') 
      && e_array[curr.row][curr.col+1] == false){
        //create Location only if fits requirements;
        Location east;
        east.row = curr.row;
        east.col = curr.col+1;
        //Mark neighbor explored
        e_array[east.row][east.col] = true;
        //Add neighbor to queue
        q.add_to_back(east);
        //Set predecessor of neighbor to curr
        p_array[east.row][east.col].row = curr.row;
        p_array[east.row][east.col].col = curr.col;
      }
    }
    //if track has default value, return invalid path
    if(track.row == -1){
      //clear contents of r-rows in p_array
      for (int r=0;r<rows;r++){
          delete [] p_array[r];
      }    
      //clear p_array contents    
      delete [] p_array;
      //clear contents of r-rows in e_array
      for (int r=0;r<rows;r++){
          delete [] e_array[r];
      }
      //clear e_array contents    
      delete [] e_array;      
      return 0;
    }
    //Shift track variable to first immediate from F
    //since track coordinate is of 'F' currently
    track.row = p_array[track.row][track.col].row;
    track.col = p_array[track.row][track.col].col;
    //run loop until back to start location
    //which precedent is simply (-1,-1)
    while(p_array[track.row][track.col].row != -1 && 
    p_array[track.row][track.col].col != -1){
      //fill immediate with '*'
      maze[track.row][track.col] = '*';
      //trace back to precedent of current immediate
      int r_temp = track.row;
      int c_temp = track.col;
      //use temp variables so they won't change the coordinates of p_array
      track.row = p_array[r_temp][c_temp].row;
      track.col = p_array[r_temp][c_temp].col;
    }
    
    //clear contents of r-rows in p_array
    for (int r=0;r<rows;r++){
        delete [] p_array[r];
    }    
    //clear p_array contents    
    delete [] p_array;
    //clear contents of r-rows in e_array
    for (int r=0;r<rows;r++){
        delete [] e_array[r];
    }
    //clear e_array contents    
    delete [] e_array;
    return 1;
}
