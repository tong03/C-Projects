#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// Parse in Data in a customized format
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();
    
    size_t nContent;
    // check that integer n is readable
    if(!(is >> nContent)){
      throw ParserError(error_msg_1);
    }
   
    // loop through number of contents
    for(size_t i=0;i<nContent;i++){
    
      // <cid> <ctype>
      int cid, ctype;
      // try to parse in
      if(!(is >> cid >> ctype)){
        throw ParserError(error_msg_2);
      }
      // take care of white space
      is >> ws;
      // content name
      string name;
      // try to parse in
      if(!getline(is, name)){
        throw ParserError(error_msg_2);
      }
      // num reviews + total stars + rating
      int nr, ts, rating;
      // try to parse in
      if(!(is >> ws >> nr)){
        throw ParserError(error_msg_2);
      }
      // take care of white space
      is >> ws;
      // try to parse in
      if(!(is >> ws >> ts)){
        throw ParserError(error_msg_2);
      }
      // try to parse in
      if(!(is >> ws >> rating)){
        throw ParserError(error_msg_2);
      }

      // (if series) num episodes initialize to 0 anyways
      int numEp = 0;
      if(ctype == 1){
        is >> ws;
        // try to parse in
        if(!(is >> ws >> numEp)){
          throw ParserError(error_msg_2);
        }
      }
      // allocate new content with appropriate ctype
      Content* newc = NULL;
      if(ctype == 0){
        newc = new Movie(cid, name, nr, ts, rating);
      }
      else {
        newc = new Series(cid, name, nr, ts, rating, numEp);
      }
      
      // add viewers - no need to check
      string line;
      // first time is a dummy to get rid of previous \n
      getline(is,line);
      // obtaining the actual info on the viewers line
      getline(is, line);
      // add the line of viewers to stringstream
      stringstream ss(line);
      
      string temp;
      // input each user from stringstream till fail
      while (ss >> temp){
        // add each viewer to the content
        newc->addViewer(temp);
      }
      // add new content to content vector
      content.push_back(newc);
    }

   // check if can read in any usernames    
    string uname;
    int limit;
    // keep looping until end of file trying to read in uname
    while(is >> uname){
        // check if valid ratinglimit read
        if(!(is >> ws >> limit)){
          throw ParserError(error_msg_3);
        }
        // create new user
        User* u = new User(uname, limit);
        // add viewing history
        string line;
        // first time is a dummy to get rid of previous \n
        getline(is, line);
        // get the viewed content of users
        getline(is, line);
        // only do something if line isn't emtpy
        if(!(line.empty())){
          stringstream ss2(line);
          int viewID;
          // iterate through stringstream to add viewID
          while(ss2 >> viewID){
            u->addToHistory(viewID);
          }
        }
        // add new user to users vector
        users.push_back(u);
    }
}