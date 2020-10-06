#include "chat_message.hpp"
#include "poker_player.hpp"
#include "dealer.h"
#include "json.hpp"
#include "poker_table.h"
#include "global_var.hpp"

using asio::ip::tcp;
int turn=0;
int start;
std::string action;
int pot = 0;
int bid = 0;
int raise_by = 0;
int bidChange = 0;
int totalPlayers;
bool skip1=false;
bool skip2=false;
bool skip3=false;
bool skip4=false;
bool skip5=false;
int allIn=0;

int busted=0;
int foldPlayers=0;
int skipPlayers=0;

bool lastPhase = false;
int proceedPlayer = 0;
int winner;
bool gameOver = false;
std::string phase = "b1phase";


poker_player::poker_player(tcp::socket socket, poker_table& room)
: socket_(std::move(socket)),
room_(room)
{
}

void poker_player::start()
{
    room_.join(shared_from_this());
    do_read_header();
}

void poker_player::deliver(const chat_message& msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        do_write();
    }
}


void poker_player::do_read_header()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.data(), chat_message::header_length),
                     [this, self](std::error_code ec, std::size_t /*length*/)
                     {
        if (!ec && read_msg_.decode_header())
        {
            // clear out the old buffer from the last read
            // a '\0' is a good value to make sure a std::string
            // is terminated
            for (unsigned int i=0;i<chat_message::max_body_length;i++)
            {
                read_msg_.body() [i] = '\0';
            }
            do_read_body();
        }
        else
        {
            room_.leave(shared_from_this());
        }
    });
}

void poker_player::do_read_body()
{
    auto self(shared_from_this());
    asio::async_read(socket_,
                     asio::buffer(read_msg_.body(), read_msg_.body_length()),
                     [this, self](std::error_code ec, std::size_t /*length*/)
                     {
        if (!ec)
        {
            if (turn==0)
            {
                nlohmann::json to_dealer = nlohmann::json::parse(std::string(read_msg_.body()));
                nlohmann::json from_player = nlohmann::json::parse(std::string(read_msg_.body()));
                room_.set_ready(to_dealer["ready"],shared_from_this()->playerNo);
                totalPlayers=room_.getSize();
                //send out ready status to all players
                if (shared_from_this()->ready==true)
                {
                    nlohmann::json to_player;
                    to_player["participant"]=shared_from_this()->playerNo;
                    to_player["ready"]=true;
                    to_player["pot"]=pot;
                    if(shared_from_this()->balance >= 5)
                    {
                        shared_from_this()->balance-=5;
                        pot+=5;
                        to_player["balance"]=shared_from_this()->balance;
                    }
                    to_player["size"] = totalPlayers;
                    std::string t=to_player.dump();
                    chat_message sending;
                    if (t.size() < chat_message::max_body_length)
                    {
                        memcpy( sending.body(), t.c_str(), t.size() );
                        sending.body_length(t.size());
                        sending.encode_header();
                        room_.deliver(sending);
                    }
                }
                
                if (room_.allReady()==false)
                {
                    std::cout<<"cant start game"<<std::endl;
                    nlohmann::json to_player1;
                    to_player1["start"]=false;
                    to_player1["pot"]=pot;
                    to_player1["balance"]=shared_from_this()->balance;
                    std::string t=to_player1.dump();
                    chat_message sending;
                    if (t.size() < chat_message::max_body_length)
                    {
                        memcpy( sending.body(), t.c_str(), t.size() );
                        sending.body_length(t.size());
                        sending.encode_header();
                        room_.deliver(sending);
                    }
                }
                else
                {
                    turn++;
                    nlohmann::json to_player2;
                    to_player2["start"]=true;
                    to_player2["participant"]=shared_from_this()->playerNo;
                    to_player2["turn"]=turn;
                    to_player2["bid"]=bid;
                    to_player2["pot"]=pot;
                    to_player2["balance"]=shared_from_this()->balance;
                    std::string t=to_player2.dump();
                    chat_message sending;
                    if (t.size() < chat_message::max_body_length)
                    {
                        memcpy( sending.body(), t.c_str(), t.size() );
                        sending.body_length(t.size());
                        sending.encode_header();
                        room_.deliver(sending);
                    }
                }
                
            }
            
            //if turn not 0
            else
            {
                nlohmann::json to_player;
                nlohmann::json from_player = nlohmann::json::parse(std::string(read_msg_.body()));
                if (from_player["autoTurn"].empty()==false)
                {
                    proceedPlayer = 0;
                    skipPlayers = 0;
                    foldPlayers=0;
                    shared_from_this()->balance+=pot;
                    action="win";
                    pot = 0;
                    bid = 0;
                    busted=0;
                    raise_by = 0;
                    turn=0;
                    Dealer.createDeck();
                    phase = "b1phase";
                    to_player["getAni"] = " ";
                    Deck = Dealer.getDeck();
                    for(int x = 0; x < playerNumber; x++)
                    {
                        for(int y = 0; y<5; y++)
                        {
                            hand[x][y] =Deck.back();
                            Deck.pop_back();
                            ehand[x][y] =Deck.back();
                            Deck.pop_back();
                        }
                        Dealer.organizeAndRank(hand[x],rankHand[x]);
                    }
                    
                    for(int x = 0; x < playerNumber; x++)
                    {
                        for(int y = 0; y<5; y++)
                        {
                            to_player["hand["+ std::to_string(x) +"]["+std::to_string(y)+"]"]=hand[x][y];
                            to_player["ehand["+ std::to_string(x) +"]["+std::to_string(y)+"]"]=ehand[x][y];
                        }
                    }
                    std::string t=to_player.dump();
                    chat_message sending;
                    if (t.size() < chat_message::max_body_length)
                    {
                        memcpy( sending.body(), t.c_str(), t.size() );
                        sending.body_length(t.size());
                        sending.encode_header();
                        room_.deliver(sending);
                    }
                    
                }
                if (from_player["ani"].empty()==false)
                {
                    std::cout<<"ANI FROM "+ std::to_string(shared_from_this()->playerNo)<<std::endl;
                    shared_from_this()->put=0;
                    int tempAni=from_player["ani"];
                    if(tempAni <= 0)
                    {
                        
                    }
                    else
                    {
                        shared_from_this()->balance-=tempAni;
                        pot+=tempAni;
                        std::cout<<"POT: "+ std::to_string(pot)<<std::endl;
                        if(shared_from_this()->playerNo==1)
                        {skip1=false;}
                        if(shared_from_this()->playerNo==2)
                        {skip2=false;}
                        if(shared_from_this()->playerNo==3)
                        {skip3=false;}
                        if(shared_from_this()->playerNo==4)
                        {skip4=false;}
                        if(shared_from_this()->playerNo==5)
                        {skip5=false;}turn = 0;
                    }
                    turn = 0;
                }
                if(from_player["action"].empty()==false && from_player["action"]=="bustedOut")
                {
                    shared_from_this()->put=0;
                    busted+=1;
                    std::cout<<"BUSTED: "+std::to_string(busted)<<std::endl;
                    if(shared_from_this()->playerNo==1)
                    {skip1=true;std::cout<<"SKIP1 SET"<<std::endl;}
                    if(shared_from_this()->playerNo==2)
                    {skip2=true;std::cout<<"SKIP2 SET"<<std::endl;}
                    if(shared_from_this()->playerNo==3)
                    {skip3=true;std::cout<<"SKIP3 SET"<<std::endl;}
                    if(shared_from_this()->playerNo==4)
                    {skip4=true;std::cout<<"SKIP4 SET"<<std::endl;}
                    if(shared_from_this()->playerNo==5)
                    {skip5=true;std::cout<<"SKIP5 SET"<<std::endl;}
                    turn = 0;
                    skipPlayers = busted;
                }
                turn++;
                if (turn>=(playerNumber+1))turn=1;
                if (skip1==true && turn==1)
                {proceedPlayer+=1;turn++;std::cout<<"SKIP1 USED"<<std::endl;}
                if (skip2==true && turn==2)
                {proceedPlayer+=1;turn++;std::cout<<"SKIP2 USED"<<std::endl;}
                if (skip3==true && turn==3)
                {proceedPlayer+=1;turn++;std::cout<<"SKIP3 USED"<<std::endl;}
                if (skip4==true && turn==4)
                {proceedPlayer+=1;turn++;std::cout<<"SKIP4 USED"<<std::endl;}
                if (skip5==true && turn==5)
                {proceedPlayer+=1;turn++;std::cout<<"SKIP5 USED"<<std::endl;}
                if (turn>=(playerNumber+1))turn=1;
                if(phase != "ephase")
                {
                    if (from_player["action"].empty()==false)
                    {
                        action = from_player["action"];
                        if(action == "bet")
                        {
                            proceedPlayer=1+skipPlayers;
                            bid = from_player["bid"];
                            shared_from_this()->put=bid;
                            shared_from_this()->mod=bid;
                            pot+=shared_from_this()->put;
                            std::cout << "pot = " <<pot<<std::endl;
                            shared_from_this()->balance-=shared_from_this()->put;
                            if(shared_from_this()->balance==0)allIn+=1;
                        }
                        if(action == "raised")
                        {
                            proceedPlayer=1+skipPlayers;
                            raise_by = from_player["raise_by"];
                            std::cout << "pot = " <<pot<<std::endl;
                            bid+=raise_by;
                            if(shared_from_this()->put == 0)
                            {
                                shared_from_this()->put+=bid;
                                shared_from_this()->balance-=bid;
                                pot+=bid;
                            }
                            else
                            {
                                shared_from_this()->balance-=bid-shared_from_this()->put;
                                pot+=bid-shared_from_this()->put;
                                shared_from_this()->put+=bid-shared_from_this()->put;
                            }
                            if(shared_from_this()->balance==0){allIn+=1;proceedPlayer=1+skipPlayers;
                                skipPlayers+=1;}
                        }
                        if(action == "called")
                        {
                            proceedPlayer+=1;
                            if(shared_from_this()->put == 0)
                            {
                                shared_from_this()->put+=bid;
                                shared_from_this()->balance-=bid;
                                shared_from_this()->mod=bid;
                                pot+=bid;
                            }
                            else
                            {
                                shared_from_this()->mod=bid-shared_from_this()->put;
                                shared_from_this()->balance-=bid-shared_from_this()->put;
                                pot+=bid-shared_from_this()->put;
                                shared_from_this()->put+=bid-shared_from_this()->put;
                            }
                            if(shared_from_this()->balance == 0){allIn+=1;proceedPlayer=1+skipPlayers;
                                skipPlayers+=1;}
                            std::cout << "bid = " <<bid<<std::endl;
                            std::cout << "pot = " <<pot<<std::endl;
                        }
                        if(action == "allin")
                        {
                            allIn+=1;
                            proceedPlayer=1+skipPlayers;
                            skipPlayers+=1;
                            if(from_player["raise_by"].empty() == false)
                            {
                                raise_by = from_player["raise_by"];
                                std::cout << "pot = " <<pot<<std::endl;
                                bid+=raise_by;
                                shared_from_this()->put+=bid;
                                pot+=bid;
                                shared_from_this()->balance-=bid;
                            }
                            if(from_player["bid"].empty() == false)
                            {
                                bid = from_player["bid"];
                                shared_from_this()->put=bid;
                                pot+=shared_from_this()->put;
                                std::cout << "pot = " <<pot<<std::endl;
                                shared_from_this()->balance-=shared_from_this()->put;
                            }
                            
                        }
                        if(action == "folded")
                        {
                            skipPlayers+=1;
                            proceedPlayer+=1;
                            foldPlayers+=1;
                            if (shared_from_this()->playerNo==1)
                            {rankHand[1][0]=0;skip1=true;}
                            if (shared_from_this()->playerNo==2)
                            {rankHand[2][0]=0;skip2=true;}
                            if (shared_from_this()->playerNo==3)
                            {rankHand[3][0]=0;skip3=true;}
                            if (shared_from_this()->playerNo==4)
                            {rankHand[4][0]=0;skip4=true;}
                            if (shared_from_this()->playerNo==5)
                            {rankHand[5][0]=0;skip5=true;}
                            shared_from_this()->skipStatus=true;
                        }
                        if(action == "checked") proceedPlayer+=1;
                        if (turn>=(playerNumber+1))turn=1;
                        
                    }
                }
                else{
                    if (from_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][0]"].empty() == false)
                        hand[shared_from_this()->playerNo][0] = from_player["hand["+ std::to_string(playerNo) +"][0]"];
                    if (from_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][1]"].empty() == false)
                        hand[shared_from_this()->playerNo][1] = from_player["hand["+ std::to_string(playerNo) +"][1]"];
                    if (from_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][2]"].empty() == false)
                        hand[shared_from_this()->playerNo][2] = from_player["hand["+ std::to_string(playerNo) +"][2]"];
                    if (from_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][3]"].empty() == false)
                        hand[shared_from_this()->playerNo][3] = from_player["hand["+ std::to_string(playerNo) +"][3]"];
                    if (from_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][4]"].empty() == false)
                        hand[shared_from_this()->playerNo][4] = from_player["hand["+ std::to_string(playerNo) +"][4]"];
                    if (from_player["gimmieCards"].empty() == false)
                    {
                        std::cout<<"Sending cards to player..."<<std::endl;
                        
                        Dealer.organizeAndRank(hand[shared_from_this()->playerNo],rankHand[shared_from_this()->playerNo]);
                        if(skip1 == true) rankHand[1][0]=0;
                        if(skip2 == true) rankHand[2][0]=0;
                        if(skip3 == true) rankHand[3][0]=0;
                        if(skip4 == true) rankHand[4][0]=0;
                        if(skip5 == true) rankHand[5][0]=0;
                        
                        to_player["turn"]=turn;
                        to_player["participant"]=shared_from_this()->playerNo;
                        to_player["balance"]=shared_from_this()->balance;
                        to_player["pot"]=pot;
                        to_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][0]"]=hand[shared_from_this()->playerNo][0];
                        to_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][1]"]=hand[shared_from_this()->playerNo][1];
                        to_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][2]"]=hand[shared_from_this()->playerNo][2];
                        to_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][3]"]=hand[shared_from_this()->playerNo][3];
                        to_player["hand["+ std::to_string(shared_from_this()->playerNo) +"][4]"]=hand[shared_from_this()->playerNo][4];
                        std::string t=to_player.dump();
                        chat_message sending;
                        if (t.size() < chat_message::max_body_length)
                        {
                            memcpy( sending.body(), t.c_str(), t.size() );
                            sending.body_length(t.size());
                            sending.encode_header();
                            room_.deliver(sending);
                        }
                    }
                    if (from_player["action"].empty()==false)
                    {
                        
                        to_player["turn"]=turn;
                        to_player["participant"]=shared_from_this()->playerNo;
                        to_player["balance"]=shared_from_this()->balance;
                        
                        shared_from_this()->put=0;
                        action = from_player["action"];
                        std::cout<<"RECEIVED EXCHANGE: " + action<<std::endl;
                        if(action == "exchanged")
                        {
                            proceedPlayer+=1;
                        }
                    }
                }
                nlohmann::json to_player2;
                
                std::cout<<"PROCEED: " + std::to_string(proceedPlayer)<<std::endl;
                //send turn to all players
                to_player2["action"]=action;
                to_player2["participant"]=shared_from_this()->playerNo;
                to_player2["raise_by"]=raise_by;
                to_player2["pot"]=pot;
                to_player2["bid"]=bid;
                to_player2["balance"]=shared_from_this()->balance;
                
                if(proceedPlayer >= playerNumber && phase == "ephase")
                {
                    std::cout<<"EPHASE OVER"<<std::endl;
                    phase = "b2phase";
                    to_player2["start"]=true;
                    proceedPlayer = 0;
         
                }
                if((proceedPlayer >= playerNumber && phase == "b2phase") || foldPlayers >= playerNumber-1 || (allIn >= playerNumber-foldPlayers && phase == "b2phase"))
                {
                    std::cout<<"B2PHASE OVER"<<std::endl;
                    std::cout << "FOLD PLAYERS: "+std::to_string(foldPlayers)<<std::endl;
                    int winner = Dealer.compareHands(rankHand[1], rankHand[2], rankHand[3], rankHand[4], rankHand[5]);
                    std::cout << "WINNER:  Player "+std::to_string(winner)<<std::endl;
                    to_player2["winner"]=winner;
                    skipPlayers = 0;
                    foldPlayers=0;
                    proceedPlayer=0;
                    skip1 = false;
                    skip2 = false;
                    skip3 = false;
                    skip4 = false;
                    skip5 = false;
                }
                if(proceedPlayer >= playerNumber && phase == "b1phase")
                {
                    std::cout<<"POT SETTLED"<<std::endl;
                    bid=0;
                    raise_by=0;
                    phase = "ephase";
                    to_player2["start"]=true;
                    proceedPlayer = 0;
                    turn =1;
                    if (skip1==true && turn==1)
                    {proceedPlayer+=1;turn++;}
                    if (skip2==true && turn==2)
                    {proceedPlayer+=1;turn++;}
                    if (skip3==true && turn==3)
                    {proceedPlayer+=1;turn++;}
                    if (skip4==true && turn==4)
                    {proceedPlayer+=1;turn++;}
                    if (skip5==true && turn==5)
                    {proceedPlayer+=1;turn++;}
                }
                if(busted>= playerNumber-1)
                {
                    to_player2["theWinner"]=" ";
                }
                to_player2["mod"]=shared_from_this()->mod;
                to_player2["turn"]=turn;
                to_player2["phase"]=phase;
                //if(phase == "ephase") turn=0;
                std::string t=to_player2.dump();
                chat_message sending;
                if (t.size() < chat_message::max_body_length)
                {
                    memcpy( sending.body(), t.c_str(), t.size() );
                    sending.body_length(t.size());
                    sending.encode_header();
                    room_.deliver(sending);
                }
                
            }
            do_read_header();
        }
        else
        {
            room_.leave(shared_from_this());
        }
    });
}

void poker_player::do_write()
{
    auto self(shared_from_this());
    asio::async_write(socket_,
                      asio::buffer(write_msgs_.front().data(),
                                   write_msgs_.front().length()),
                      [this, self](std::error_code ec, std::size_t /*length*/)
                      {
        if (!ec)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                do_write();
            }
        }
        else
        {
            room_.leave(shared_from_this());
        }
    });
}


