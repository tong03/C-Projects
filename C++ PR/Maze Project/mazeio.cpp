/*
mazeio.cpp
*/

#include <iostream>
#include <fstream>
#include "mazeio.h"

using namespace std;

/*************************************************
 * read_maze:
 * Read the maze from the given filename into a
 *  2D dynamically  allocated array.
 *
 * Return the pointer to that array.
 * Return NULL (a special address) if there is a problem,
 * opening the file or reading in the integer sizes.
 *
 * The first argument is the filename of the maze input.
 *  You should use an ifstream to open and read from this
 *  file.
 *
 * We also pass in two pointers to integers. These are
 * output parameters (i.e. declared by the caller and
 * passed-by-reference for this function to fill in).
 * Fill the integers pointed to by these arguments
 * with the number of rows and columns
 * read (the first two input values).
 *
 *************************************************/
char** read_maze(char* filename, int* rows, int* cols)
{

    // *** You complete **** CHECKPOINT 1
  ifstream ifile(filename);
  //Check if opened successfully
  if (ifile.fail()){
    return NULL;
  }
  ifile >> *rows >> *cols;
  if (ifile.fail()){ //if cannot read two numbers
    return NULL;
  }
  int size = *rows * *cols;
  char** mymaze = new char*[size];
  
  //cout<<"Before loading the maze"<<endl;
  //read in the maze from file by each line
  for(int r=0;r<*rows;r++){
    //allocate an array of numcols per row
    char* temp = new char[*cols+1];
    ifile >> temp;
    //temp is pointer to numcols array, 
    //put start address in pointers of numrows array
    mymaze[r] = temp;
  }
  //cout<<"Loaded the maze"<<endl;
  //Close file
  ifile.close();
  return mymaze;
}

/*************************************************
 * Print the maze contents to the screen in the
 * same format as the input (rows and columns, then
 * the maze character grid).
 *************************************************/
void print_maze(char** maze, int rows, int cols)
{

  // *** You complete **** CHECKPOINT 1
  cout << rows << " " << cols << endl;
  for (int r=0;r<rows;r++){
    for (int c=0;c<cols;c++){
      cout << maze[r][c];
    }
    cout<<endl;
  }

}

