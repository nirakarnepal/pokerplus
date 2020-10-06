

#ifndef _POKERPLAYER
#define _POKERPLAYER

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
#include "poker_table.h"
#include "player.h"


using asio::ip::tcp;

//----------------------------------------------------------------------

class poker_player
: public player,
public std::enable_shared_from_this<poker_player>
{
public:
    poker_player(tcp::socket socket, poker_table& room);
    void start();
    void deliver(const chat_message& msg);
private:
    void do_read_header();
    void do_read_body();
    void do_write();
    
    std::vector<int> Deck;
    tcp::socket socket_;
    poker_table& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

#endif
