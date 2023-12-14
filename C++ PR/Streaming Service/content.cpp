#include "content.h"
#include <iostream>
#include <iomanip>

using namespace std;


Content::Content(int id, std::string name, int nr, int ts, int rating)
{
  // initialize all the data
    id_ = id;
    name_ = name;
    numReviews_ = nr;
    totalStars_ = ts;
    rating_ = rating;
}

// Don't have to implement
Content::~Content()
{
  return;
}

// Default display of Content info
void Content::display(std::ostream& ostr) const
{
    ostr << "========" << endl;
    ostr << "ID: " << id_ << endl;
    ostr << "Name: " << name_ << endl;
    ostr << "Views: " << usersWhoViewed_.size() << endl;
    ostr << "Average review: " << setprecision(1) << fixed << getStarAverage() << endl;
    
    if(rating_ >= 0 && rating_ < 5) {
        const char** ratingMapping = this->ratingStrings();
        ostr << "Rating: " << ratingMapping[rating_] << endl;
    }
}

// Return content ID
int Content::id() const
{
    return id_;
}

// Return content name
std::string Content::name() const
{
    return name_;
}

// Return content rating
int Content::rating() const
{
    return rating_;
}

// Add a review
void Content::review(int numStars) 
{
    numReviews_++;
    totalStars_ += numStars;
}

// Add user to list of viewers for the content
void Content::addViewer(const std::string& username)
{
    usersWhoViewed_.push_back(username);
}

// Return list of unames that have watched the content
const std::vector<std::string>& Content::getViewers() const
{
    return usersWhoViewed_;
}

// Check if a user has seen the content
bool Content::hasViewed(const std::string& uname) const
{
    for(size_t m = 0; m < usersWhoViewed_.size(); m++){
        if(usersWhoViewed_[m] == uname){
            return true;
        }
    }
    return false;
}

// Rating for Movie objects
const char** Movie::ratingStrings() const
{
    // Global variable of movie rating mappings for 
    //  displaying Movie objects. We use "G" for rating 0 and 1.
    static const char* movierating[] = {"G", "G", "PG", "PG-13", "R"};
    return movierating;
}

// Rating for Series objects
const char** Series::ratingStrings() const
{
    // DEFAULT TO THE SERIES / TV Rating Mappings
    
    // Global variable of series rating mapping (0 = "TVY", 1 = "TVG", etc.)
    static const char* tvrating[] = {"TVY", "TVG", "TVPG", "TV14", "TVMA"};
    return tvrating;
}

// Return the Star Average
double Content::getStarAverage() const 
{
  // avoid division by 0 by returning 0
  if(numReviews_ == 0){
    return 0;
  }
  return totalStars_/double(numReviews_);

}

// Return the number of episodes in the series
int Series::numEpisodes() const
{
  return numEpisodes_;
}

// Movie constructor
Movie::Movie(int id, std::string name, int nr, int ts, int rating)
: Content(id, name, nr, ts, rating)
{

}

// Movie destructor
Movie::~Movie()
{
  return;
}

// Movie display()
// Don't need it use default

// Series constructor
Series::Series(int id, std::string name, int nr, int ts, int rating, int numEpisodes)
: Content(id, name, nr, ts, rating)
{
  numEpisodes_ = numEpisodes;
}

// Series destructor
Series::~Series()
{
  return;
}

// Series display() --> include number of episodes
void Series::display(
        std::ostream& ostr) const
{
  // call the default display first, then just add episodes
  Content::display(ostr);
  ostr << "Episodes: " << numEpisodes_ << endl;
}

