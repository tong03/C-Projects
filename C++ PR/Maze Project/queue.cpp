/*
queue.cpp
*/

#include "queue.h"

//Constructor. maxlen must be as large as the total number
// of Locations that will ever be entered into this Queue.
Queue::Queue(int maxlen) {
    head = 0;
    tail = 0;
    // *** You complete **** CHECKPOINT 3
    // need storage!!
    contents = new Location[maxlen];
}

//Destructor. releases resources. C++ will call it automatically.
Queue::~Queue() {
    // *** You complete **** CHECKPOINT 3
    //head = 0;
    // delete allocated items inside of each element of contents array
    /*while(!is_empty()){
        Location temp = contents[head];
        head++;
        delete temp;
    }*/
    // delete the actual array
    delete [] contents;
}

//Insert a new Location at the end/back of our list
void Queue::add_to_back(Location loc) {

    // *** You complete **** CHECKPOINT 3
    /*Location* locp = new Location;
    locp->row = loc.row;
    locp->col = loc.col;*/
    contents[tail] = loc;
    tail++;
}

//Return and "remove" the oldest Location not already extracted
Location Queue::remove_from_front() {

    // *** You complete **** CHECKPOINT 3
        // create temp to hold contents[head] before post-incr head
        Location temp = contents[head++];
        return temp;

}

//Is this Queue empty? (did we extract everything added?)
//This is complete, you don't need to change it.
bool Queue::is_empty() {
    return head == tail;
}

