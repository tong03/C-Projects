#include "component.h"
#include "cimage.h"
#include "bmplib.h"
#include <deque>
#include <iomanip>
#include <iostream>
#include <cmath>


using namespace std;

// Initialize starting image
CImage::CImage(const char* bmp_filename)
{
    //  Note: readRGBBMP dynamically allocates a 3D array
    //    (i.e. array of pointers to pointers (1 per row/height) where each
    //    point to an array of pointers (1 per col/width) where each
    //    point to an array of 3 unsigned char (uint8_t) pixels [R,G,B values])

    // Call readRGBBMP to initialize img_, h_, and w_;
    img_ = readRGBBMP(bmp_filename, h_, w_);


    // Check if img_ is invalid
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }

    // Set the background RGB color using the upper-left pixel
    for(int i=0; i < 3; i++) {
        bgColor_[i] = img_[0][0][i];
    }

    // RGB "distance" threshold to continue a BFS from neighboring pixels
    bfsBgrdThresh_ = 60;

    // Initialize the vector of vectors of labels to -1
    for (int r=0; r<h_;r++){
        // Inner vector of w_ entries for each row
        vector<int> myrow(w_);
        labels_.push_back(myrow);
        // initialize col of each row to -1
        for (int c=0;c<w_;c++){
            labels_[r][c] = -1;
        }
    }
}

// Deallocate the 3D array
CImage::~CImage()
{
  // Pass in data member pointing to 3D array
  deallocateImage(img_);
}

// Check if pixel is close to background value
bool CImage::isCloseToBground(uint8_t p1[3], double within) {
    // Computes "RGB" (3D Cartesian distance)
    double dist = sqrt( pow(p1[0]-bgColor_[0],2) +
                        pow(p1[1]-bgColor_[1],2) +
                        pow(p1[2]-bgColor_[2],2) );
    return dist <= within;
}

// Identify components in the image
size_t CImage::findComponents()
{
  // initialize component counter
  size_t label_c = 0;
  // iterate over the scan 2D array and find foreground
  for(int r=0; r < h_; r++) {
      for(int c = 0; c < w_; c++) {
        // RGB for each pixel
        uint8_t p1[3];
        for(int i=0; i < 3; i++) {
          // set RGB value to be that of curr pixel
          p1[i] = img_[r][c][i];
        }
        // run threshold check for foreground
        bool check = isCloseToBground(p1, bfsBgrdThresh_);
        // if not in threshold and label is -1, run BFS
        if(!check && labels_[r][c] == -1){
          Component item = bfsComponent(r,c,label_c);
          // add new component to component vector
          components_.push_back(item);
          // update component counter
          label_c++;
        }
      }
  }
  // return the number of components found
  return label_c;
}

// Complete - Do not alter
void CImage::printComponents() const
{
    cout << "Height and width of image: " << h_ << "," << w_ << endl;
    cout << setw(4) << "Ord" << setw(4) << "Lbl" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < components_.size(); i++) {
        const Component& c = components_[i];
        cout << setw(4) << i << setw(4) << c.label << setw(6) << c.ulNew.row << setw(6) << c.ulNew.col
             << setw(4) << c.height << setw(4) << c.width << endl;
    }

}


// Return component index
int CImage::getComponentIndex(int mylabel) const
{
  int idx = 0;
  // iterate over the component vector
  for (size_t i = 0; i<components_.size();i++){
    // if label matches my label, set idx to current index
    if(components_[i].label == mylabel){
      idx = i;
    }
  }
  return idx;
}


// Nearly complete - TO DO:
//   Add checks to ensure the new location still keeps
//   the entire component in the legal image boundaries
void CImage::translate(int mylabel, int nr, int nc)
{
    // Get the index of specified component
    int cid = getComponentIndex(mylabel);
    if(cid < 0) {
        return;
    }
    int h = components_[cid].height;
    int w = components_[cid].width;

    // ==========================================================
    // ADD CODE TO CHECK IF THE COMPONENT WILL STILL BE IN BOUNDS
    // IF NOT:  JUST RETURN.
    
    // check if coordinates in bound
    if(nr < 0 || nr > h_ || nc < 0 || nc > w_){
      return;
    }

    // if either dimensions exceed the bounds, return  
    else if(nr + h > h_ || nr + h <0 || nc + w > w_ || nc + w < 0){
      return;
    }
    // ==========================================================

    // If we reach here we assume the component will still be in bounds
    // so we update its location.
    Location nl(nr, nc);
    components_[cid].ulNew = nl;
}

// TO DO: Complete this function
void CImage::forward(int mylabel, int delta)
{
    int cid = getComponentIndex(mylabel);
    if(cid < 0 || delta <= 0) {
        return;
    }
    // Add your code here
    // set max size to component vector size
    int max_size = static_cast<int> (components_.size());
    // check if delta exceed upper bound, set to max
    int change = min(cid + delta, max_size-1);
    // save val at cid into a temp variable
    Component temp = components_[cid];
    // iterate from cid to before change
    for(int i=cid;i<change;i++){
      // set each idx value to one ahead --> shift array down to cid
      components_[i] = components_[i+1];
    }
    // set change id to have the old val at cid
    components_[change] = temp;
    //components_.insert(components_.begin() + change + 1,components_[cid]);
    //components_.erase(components_.begin() + cid);
}

// TO DO: Complete this function
void CImage::backward(int mylabel, int delta)
{
    int cid = getComponentIndex(mylabel);
    if(cid < 0 || delta <= 0) {
        return;
    }
    // Add your code here
    int min_size = 0;
    // check if delta exceed lower bound, set to min
    int change = max(cid - delta, min_size);
    Component temp = components_[cid];
    // iterate from cid to before change
    for(int i=cid;i>change;i--){
      // set each idx value to one behind --> shift array up to cid
      components_[i] = components_[i-1];
    }
    // set change id to have the old val at cid
    components_[change] = temp;
    //components_.insert(components_.begin() + change,components_[cid]);
    //components_.erase(components_.begin() + cid + 1);
}

// Save changes from original image
void CImage::save(const char* filename)
{
    // Create another image filled in with the background color
    uint8_t*** out = newImage(bgColor_);

    // Add your code here
    
    // Draw new component in correct order in component vecotr
    for(size_t i = 0; i<components_.size();i++){
      // New bounding box variables
      int n_row = components_[i].ulNew.row;
      int n_col = components_[i].ulNew.col;
      int nheight = components_[i].height;
      int nwidth = components_[i].width;
      // Old bounding box variables
      int o_row = components_[i].ulOrig.row;
      int o_col = components_[i].ulOrig.col;
      // Iterate through the bounding box dimensions
      for(int r=0;r<nheight;r++){
        for(int c=0;c<nwidth;c++){
          // see if label of OG grid matches with current label --> valid pixel
          // set coordinate in new image if it is
          if (labels_[r+o_row][c+o_col] == components_[i].label){
            out[r+n_row][c+n_col][0] = img_[r+o_row][c+o_col][0];
            out[r+n_row][c+n_col][1] = img_[r+o_row][c+o_col][1];
            out[r+n_row][c+n_col][2] = img_[r+o_row][c+o_col][2];
          }
        }
      }
    }

    writeRGBBMP(filename, out, h_, w_);
    // Add any other code you need after this

    // deallocate the newImage
    deallocateImage(out);
}

// Complete - Do not alter - Creates a blank image with the background color
uint8_t*** CImage::newImage(uint8_t bground[3]) const
{
    uint8_t*** img = new uint8_t**[h_];
    for(int r=0; r < h_; r++) {
        img[r] = new uint8_t*[w_];
        for(int c = 0; c < w_; c++) {
            img[r][c] = new uint8_t[3];
            img[r][c][0] = bground[0];
            img[r][c][1] = bground[1];
            img[r][c][2] = bground[2];
        }
    }
    return img;
}

// To be completed
void CImage::deallocateImage(uint8_t*** img) const
{
  // delete all the RGB array of each col
  for(int r = 0; r<h_; r++){
    for(int c = 0; c<w_; c++){
      delete [] img[r][c];
    }
  }
  // delete all the col_array contents of each row
  for(int r = 0; r<h_; r++){
    delete [] img[r];
  }
  // delete all the row_array contents of the 3D array
  delete [] img;
}

// bfs implementation to locate components
Component CImage::bfsComponent(int pr, int pc, int mylabel)
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};
    // initialize queue (which is a deque)
    deque<Location> myq;
    // initialize start Location
    Location start = Location(pr,pc);
    // mark start pixel as explored
    labels_[pr][pc] = mylabel;
    // add start into the queue
    myq.push_back(start);
    // initialize max Location
    Location maxl = start;
    //initialize min Location
    Location minl = start;
    // while queue isn't empty loop
    while(!myq.empty()){
        // extract item from front of q    
        Location curr = myq.front();
        myq.pop_front();
        // update bounding box information based on curr pixel
        if(curr.row > maxl.row){
          maxl.row = curr.row;
        }
        if(curr.col > maxl.col){
          maxl.col = curr.col;
        }
        if(curr.row < minl.row){
          minl.row = curr.row;
        }
        if(curr.col < minl.col){
          minl.col = curr.col;
        }
        // iterate over 8 directions from curr pixel
        for(int d=0;d<8;d++){
            // check that neighbor is in bound before going further
            if(curr.row+neighbor_row[d] > -1
             && curr.row+neighbor_row[d] < h_
             && curr.col+neighbor_col[d] > -1
              && curr.col+neighbor_col[d] < w_){
                // define neighbor row and col
                int nb_row = curr.row+neighbor_row[d];
                int nb_col = curr.col+neighbor_col[d];
                // determine foreground status of neighbor pixel
                uint8_t p1[3];
                for(int i=0; i < 3; i++) {
                    // set RGB value to be that of neighbor pixel
                    p1[i] = img_[nb_row][nb_col][i];
                }
                bool check = isCloseToBground(p1, bfsBgrdThresh_);   
                // if valid foreground and unvisited
                if(!check && labels_[nb_row][nb_col] == -1){
                    // label the neighbor as visited
                    labels_[nb_row][nb_col] = mylabel;
                    // create neighbor Location
                    Location nbor = Location(nb_row, nb_col);
                    // add neighbor to back of queue
                    myq.push_back(nbor);
                }
            }
        }
    }
    // bounding box info to create Component and return
    int h = maxl.row - minl.row + 1;
    int w = maxl.col - minl.col + 1;
    Component shape = Component(minl, h, w, mylabel);
    return shape;
}

// Complete - Debugging function to save a new image
void CImage::labelToRGB(const char* filename)
{
    //multiple ways to do this -- this is one way
    vector<uint8_t[3]> colors(components_.size());
    for(unsigned int i=0; i<components_.size(); i++) {
        colors[i][0] = rand() % 256;
        colors[i][1] = rand() % 256;
        colors[i][2] = rand() % 256;
    }

    for(int i=0; i<h_; i++) {
        for(int j=0; j<w_; j++) {
            int mylabel = labels_[i][j];
            if(mylabel >= 0) {
                img_[i][j][0] =  colors[mylabel][0];
                img_[i][j][1] =  colors[mylabel][1];
                img_[i][j][2] =  colors[mylabel][2];
            } else {
                img_[i][j][0] = 0;
                img_[i][j][1] = 0;
                img_[i][j][2] = 0;
            }
        }
    }
    writeRGBBMP(filename, img_, h_, w_);
}

// Complete - Do not alter
const Component& CImage::getComponent(size_t i) const
{
    if(i >= components_.size()) {
        throw std::out_of_range("Index to getComponent is out of range");
    }
    return components_[i];
}

// Complete - Do not alter
size_t CImage::numComponents() const
{
    return components_.size();
}

// Complete - Do not alter
void CImage::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < components_.size(); i++){
        Location ul = components_[i].ulOrig;
        int h = components_[i].height;
        int w = components_[i].width;
        for(int i = ul.row; i < ul.row + h; i++){
            for(int k = 0; k < 3; k++){
                img_[i][ul.col][k] = 255-bgColor_[k];
                img_[i][ul.col+w-1][k] = 255-bgColor_[k];

            }
            // cout << "bb " << i << " " << ul.col << " and " << i << " " << ul.col+w-1 << endl; 
        }
        for(int j = ul.col; j < ul.col + w ; j++){
            for(int k = 0; k < 3; k++){
                img_[ul.row][j][k] = 255-bgColor_[k];
                img_[ul.row+h-1][j][k] = 255-bgColor_[k];

            }
            // cout << "bb2 " << ul.row << " " << j << " and " << ul.row+h-1 << " " << j << endl; 
        }
    }
    writeRGBBMP(filename, img_, h_, w_);
}
