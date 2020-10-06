#ifndef __DEALER_H
#define __DEALER_H
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <iostream>     // std::cout
#include <cstring>
#include <array>
class dealer
{
public:
    dealer();
    void createDeck();
    std::vector<int> getDeck();
    int compareHands(int *, int *, int *, int *, int *);
    int organizeAndRank(int * hand, int * rankHand);
private:
    std::vector<int> Deck;
    int i;
};
#endif
