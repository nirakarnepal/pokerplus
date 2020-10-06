
#ifndef _POKERTABLE
#define _POKERTABLE

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include "asio.hpp"
#include "chat_message.hpp"
#include "player.h"
#include "dealer.h"
#include <string>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

using asio::ip::tcp;

extern int playerNumber;
extern int hand[6][5];
extern int ehand[6][5];
extern int rankHand[6][6];

class poker_table {
public:
    poker_table();
    void join(player_ptr participant);
    void leave(player_ptr participant);
    void deliver(const chat_message& msg);
    void deliver_to(const chat_message& msg,int deli_to);
    void set_ready(bool status,int deli_to);
    bool allReady();                //checks if all the player in the servers are ready for the game
    int getSize();
private:
    std::vector<int> Deck;
    std::set<player_ptr> participants_;
    enum { max_recent_msgs = 100 };           //CSE3310 (server) maximum number of messages are stored
    chat_message_queue recent_msgs_;
};

#endif
