#ifndef __MAINWIN_H
#define __MAINWIN_H

#include <assert.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include "json.hpp"
#include <gtkmm.h>
#include <fstream>
#include <iostream>
#include "chat_client.h"

const std::string APP_TITLE{"POKER ++"};
const std::string APP_NAME{"edu.uta.cse1325.mass.v0_1_0"};
const std::string VERSION{"0.1.0"};
const std::string EXT = "mass";
const std::string COOKIE{"M⅍S1"};


class Mainwin : public Gtk::Window {
    public:
        Mainwin(chat_client*);
        virtual ~Mainwin();
    protected:
    void on_fold_click();
    void on_check_click();
    void on_bet_click();
    void on_call_click();
    void on_raise_click();
    void on_exchange_click();
    void on_ready_click();
    void updateButton(int);
    void grayOutButton();
    void on_HScale_value_changed();
    bool on_my_timeout();
        void updateReadyBoxes(int participant);
    void updateFoldAction(int participant);
    void updateCheckAction(int participant);
    void updateCallAction(int, int, int);
    void updateBetAction(int participant, int val);
    void updateRaiseAction(int participant, int val);
    void updateAllInAction(int participant, int put, int val);
    void updateExchangeAction(int participant);
    void showCards();
    void activateExchange(int);
    void startRound(int);
    void bustOut(int);
    void takeAni();
    void toGui(std::string, int, int, int, int);
    void theWinner(int);
    void updateCallRaiseButtons(int,int, int, int);
    void updateVals(int,int,int);
    void hidePlayers(int);
        bool reset=false;
    void displayWinner(int, int);
    Gtk::VBox cvbox;
    Gtk::HBox hbox;
    Gtk::HBox actionHbox,balanceHbox,publicCardHbox,indicatorHbox, potHbox,privateCardHbox,cardSelectionHbox,exchangeHbox,playHbox;
    Gtk::Grid m_Grid;
    Gtk::Frame Frame_Horizontal, Frame_Vertical;
    Gtk::CheckButton RB1, RB2, RB3, RB4, RB5;
    Gtk::Separator sep1, sep2,sep3, sep4;
    Gtk::Scale HScale;
    Gtk::Grid m_grid,small_card_grid1,small_card_grid2,small_card_grid3,small_card_grid4,small_card_grid5;

    void shiftIndicator(int);
    Gtk::Image *indicator =Gtk::manage(new Gtk::Image{"Icons/indicator.png"});
    friend class chat_client;

    private:
        Gtk::Label p1,p2,p3,p4,p5;                  // Display player name
        Gtk::Label action1, action2, action3, action4, action5;                   // Action message display
    Gtk::Label balance1, balance2, balance3, balance4, balance5;        //Player's current balance, Available amount
    Gtk::Label indicator1, indicator2, indicator3, indicator4, indicator5;   //Indicates whose turn.
    Gtk::Label potLabel;
    Gtk::Label image11, image12, image13, image14, image15;          //player1 hands
    Gtk::Label image21, image22, image23, image24, image25;    //player2 hands
    Gtk::Label image31, image32, image33, image34, image35;    //player3 hands
    Gtk::Label image41, image42, image43, image44, image45;    //player4 hands
    Gtk::Label image51, image52, image53, image54, image55;    //player5 hands

    Gtk::Label hand1, hand2, hand3, hand4, hand5;    //your hands, big picture
    
    Gtk::Button fold;//{"FOLD"};
    Gtk::Button check;//{"CHECK"};
    Gtk::Button bet;//{"BET $5"};
    Gtk::Button call;//{"CALL"};
    Gtk::Button raise;//{"RAISE $5"};
    Gtk::Button exchange{"EXCHANGE"};
    Gtk::Button ready{"READY"};
    
    Gtk::Image *bh1, *bh2, *bh3, *bh4, *bh5; //big cards
    Gtk::Image *h1, *h2, *h3, *h4, *h5;     //sml cards
          
    int TESTVAL;
    int TESTTURN = 0;
    bool readyBool = false;
    chat_client *c;
};
#endif

