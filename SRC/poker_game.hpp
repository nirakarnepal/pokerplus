#ifndef poker_game_hpp
#define poker_game_hpp

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.hpp"
#include "player.h"
#include "poker_table.h"
#include "poker_player.hpp"

using asio::ip::tcp;

//----------------------------------------------------------------------

class poker_game {
public:
    poker_game(asio::io_context& io_context,
               const tcp::endpoint& endpoint);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    poker_table room_;
};



#endif /* poker_game_hpp */
