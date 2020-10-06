#ifndef __CLIENT_H
#define __CLIENT_H
#include <cstdlib>
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtkmm.h>
#include <assert.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include "json.hpp"
#include <string>

using asio::ip::tcp;
using namespace std;
typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(asio::io_context& io_context,
                const tcp::resolver::results_type& endpoints);
    void write(const chat_message& msg);
    void close();
    int turn=0;
    int playerNo=0;
    string uuid;
    bool start;
    int participant=0;
    int getCard1();
    int getCard2();
    int getCard3();
    int getCard4();
    int getCard5();
    int getHand(int card);
    int geteHand(int card);
    bool newRound=true;
    void giveMoney(int);
 
private:
    friend class Mainwin;
    void do_connect(const tcp::resolver::results_type& endpoints);
    void do_read_header();
    void do_read_body();
    void do_write();
    asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
    int card1, card2, card3, card4, card5;
    int hand[5];
    int ehand[5];
    int balance = 195;
    int size;
    int bid = 0;
    int mod = 0;
    bool isFold = false;
    int winner=0;
    int max =0;
    int put = 0;
    //int pot=0;
};
#endif


