#ifndef __PLAYER
#define __PLAYER

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"


using asio::ip::tcp;
//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------

class player
{
public:
    virtual ~player() {}
    virtual void deliver(const chat_message& msg) = 0;
    std::string uuid;
    //int turn;
    int playerNo;
    bool ready = false;
    int balance = 200;
    int put=0;
    int mod=0;
    int skipStatus = false;
    std::string actionTaken;
};
typedef std::shared_ptr<player> player_ptr;

#endif
