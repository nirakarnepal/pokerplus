#include "poker_game.hpp"
using asio::ip::tcp;
#include "global_var.hpp"



poker_game::poker_game(asio::io_context& io_context,
                       const tcp::endpoint& endpoint)
: acceptor_(io_context, endpoint)
{
    do_accept();
}


void poker_game::do_accept()
{
    acceptor_.async_accept(
                           [this](std::error_code ec, tcp::socket socket)
                           {
        if (!ec)
        {
            std::make_shared<poker_player>(std::move(socket), room_)->start();
        }
        
        do_accept();
    });
}

