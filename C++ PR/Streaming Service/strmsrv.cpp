#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;


StreamService::StreamService()
{
  // initialize cUser pointer to NULL
  cUser_ = NULL;
}

// Deallocate the Contents and Users
StreamService::~StreamService()
{
  // iterate through content and delete each element
  for(size_t i=0;i<content_.size();i++){
    delete content_[i];
  }
  // iterate through users and delete each element
  for(size_t i=0;i<users_.size();i++){
    delete users_[i];
  }  
}

// Parse in Data from file
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// Log a user in
void StreamService::userLogin(const std::string& uname)
{
  // check if someone's already logged in
  if(cUser_ != NULL){
    throw runtime_error("userLogin() Already logged in!");
  }
  // check for valid user
  if(getUserIndexByName(uname) == -1){
    throw invalid_argument("userLogin() Invalid username!");
  }
  // change uname to index and set cUser to indexed User
  int idx = getUserIndexByName(uname);
  cUser_ = users_[idx];
}

// Log a user out
void StreamService::userLogout()
{
    if(cUser_ == NULL){
      return;
    }
    // set cUser to null
    cUser_ = NULL;
}

// Find content based on user input
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // "*" means all content
        if(partial == "*"){
          // loop through content_ to add everything
          for(size_t j=0;j<content_.size();j++){
            results.push_back(content_[j]->id());
          }
          // end right away after by returning
          return results;
        }
        // check to see if find() on partial returns anything
        if(content_[i]->name().find(partial) != string::npos){
            // add to results if it does
            results.push_back(content_[i]->id());
        }        
    }
    return results;
}

// Return a User's watched Content ID
std::vector<CID_T> StreamService::getUserHistory() const
{  
    // Checked if anyone logged in
    throwIfNoCurrentUser();
    return cUser_->history;
}

// Add a content ID into User's viewing history
void StreamService::watch(CID_T contentID)
{
    // check for noID
    throwIfNoCurrentUser();
    // check for valid content id
    if(! isValidContentID(contentID)){
        throw std::range_error("Watch: invalid contentID");
    }
    // check if content suitable
    if(cUser_->ratingLimit < content_[contentID]->rating()){
      throw RatingLimitError("watch() non-suitable rating");
    }
    // check if user has not watched content
    if(!cUser_->haveWatched(contentID)){
      // update User to watch this content
      cUser_->addToHistory(contentID);
      // update Content to include this user as viewed
      content_[contentID]->addViewer(cUser_->uname);
    }
    
}

// Leave a review for the Content ID
void StreamService::reviewShow(CID_T contentID, int numStars)
{
    // check noID
    throwIfNoCurrentUser();
    // check that it's a valid id
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
    // check valid review range
    if(numStars < 0 || numStars > 5){
        throw ReviewRangeError("Review is out of bounds!");
    }
    // add a review to the specific content
    content_[contentID]->review(numStars);
}

/* Look at shared commonalities to suggest similar content
     * For the given content, Cp, and current user, Uq, consider the set of 
     * Users, Uj, (Uj /= Uq) who also watched content Cp and find the single, 
     * content item, Ck, (Ck /= Cp) that was viewed the most times by users
     * in the set, Uj.*/
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // check noID
    throwIfNoCurrentUser();
    // check valid ID
    if(! isValidContentID(contentID)){
        throw std::range_error("Suggest: invalid contentID");
    }
    // get the viewer names from the content
    const vector<string>& viewU = content_[contentID]->getViewers();
    //  make int vector to tally similarity --> size = content size
    vector<CID_T> myTally(content_.size());
    // initialize tally vector to 0
    for(size_t i=0;i<myTally.size();i++){
      myTally[i] = 0;
    }
    // iterate over the viewers name vector of content
    for(size_t i=0; i<viewU.size();i++){
      // check as long as it not cUser
      if(viewU[i] != cUser_->uname){
        // get appropriate userID
        int userID = getUserIndexByName(viewU[i]);
        vector<CID_T>& Uhistory = users_[userID]->history;
        // loop through each user view history
        for(size_t j=0;j<Uhistory.size();j++){
          // if(viewedID != contentID) add to tally
          if(Uhistory[j] != contentID){
            // increment tally vector[viewedID] by 1;         
            myTally[Uhistory[j]] += 1;
          }
        }
      }
    }
    // set maxSimilar to 0
    int maxSim = 0;
    // set idx = -1 as default;
    CID_T idx = -1;
    // loop thru tally vector to find the most similar content
    for(size_t i=0;i<myTally.size();i++){
      // if vector value > maxSimilar & user have not watched it
      if(myTally[i] > maxSim && (cUser_->haveWatched(i) == false)){
        // set maxSimilar to new value
        maxSim = myTally[i];
        // set idx to current idx
        idx = i;        
      }
    }
    
    return idx;
}

// Show the basic content info
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Check for valid content ID
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
    ostream& ostr = cout;
    // Call the display abitlity of the appropriate content object
    content_[contentID]->display(ostr);
}

// Check for valid content ID
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Check if a user is logged in
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Return corresponding User idx given their uname
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
