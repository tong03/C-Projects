#include "user.h"
#include <algorithm>
#include <iostream>
using namespace std;

// Add content ID to user viewing history
void User::addToHistory(CID_T contentID)
{
    history.push_back(contentID);
}

// Check if user has watched Content
bool User::haveWatched(CID_T contentID)
{
    auto my_iterator = std::find(history.begin(), history.end(), contentID);
    return my_iterator != history.end();
}

