#include "poker_table.h"
#include "chat_message.hpp"
#include "dealer.h"
#include "json.hpp"
#include <sstream>
#include "global_var.hpp"



int playerNumber=0;
int hand[6][5];
int ehand[6][5];
int rankHand[6][6];

dealer Dealer;
poker_table::poker_table()
{
    Dealer.createDeck();
    Deck = Dealer.getDeck();
}
void poker_table::join(player_ptr participant)
{
    playerNumber++;
    std::cout<<"New Player has joined the room"<<std::endl;
    
    participant->playerNo = playerNumber;
    std::cout<< "***************" << participant->playerNo << "***************" << std::endl;
    
    
    
    boost::uuids::basic_random_generator<boost::mt19937> g;
    boost::uuids::uuid u=g();
    
    
    std::string playerID;
    std::stringstream ss;
    ss<<u;
    playerID=ss.str();
    participant->uuid = playerID;
    
    std::cout << "Player number " << participant->playerNo << "uuid is " << participant->uuid << std::endl;
    std::cout << "cards are ";
    for(int x = 0; x<5; x++)
    {
        hand[participant->playerNo][x] =Deck.back(); std::cout<<std::to_string(Deck.back())+" ";
        Deck.pop_back();
        ehand[participant->playerNo][x] =Deck.back();
        Deck.pop_back();
    }
    std::cout<<std::endl;
    
    Dealer.organizeAndRank(hand[participant->playerNo],rankHand[participant->playerNo]);
    
    participants_.insert(participant);
    
    //send client their uuid and turn
    nlohmann::json to_player;
    to_player["playerNo"] = participant->playerNo;
    to_player["uuid"] = participant->uuid;
    to_player["hand["+ std::to_string(participant->playerNo) +"][0]"]=hand[participant->playerNo][0];
    to_player["hand["+ std::to_string(participant->playerNo) +"][1]"]=hand[participant->playerNo][1];
    to_player["hand["+ std::to_string(participant->playerNo) +"][2]"]=hand[participant->playerNo][2];
    to_player["hand["+ std::to_string(participant->playerNo) +"][3]"]=hand[participant->playerNo][3];
    to_player["hand["+ std::to_string(participant->playerNo) +"][4]"]=hand[participant->playerNo][4];
    
    to_player["ehand["+ std::to_string(participant->playerNo) +"][0]"]=ehand[participant->playerNo][0];
    to_player["ehand["+ std::to_string(participant->playerNo) +"][1]"]=ehand[participant->playerNo][1];
    to_player["ehand["+ std::to_string(participant->playerNo) +"][2]"]=ehand[participant->playerNo][2];
    to_player["ehand["+ std::to_string(participant->playerNo) +"][3]"]=ehand[participant->playerNo][3];
    to_player["ehand["+ std::to_string(participant->playerNo) +"][4]"]=ehand[participant->playerNo][4];
    std::string t = to_player.dump();
    chat_message sending;
    if (t.size() < chat_message::max_body_length)
    {
        memcpy( sending.body(), t.c_str(), t.size() );
        sending.body_length(t.size());
        sending.encode_header();
        participant->deliver(sending);
    }
    
    
    
    participants_.insert(participant);
    for (auto msg: recent_msgs_)                    //CSE3310 (server)  previous chat messages are sent to a client
    {
        participant->deliver(msg);
    }
    
}

void poker_table::leave(player_ptr participant)
{
    participants_.erase(participant);
}

void poker_table::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();
    for (auto participant: participants_)       //CSE3310 (server)  messages are sent to all connected clients
    {
        participant->deliver(msg);
    }
}



void poker_table::deliver_to(const chat_message& msg,int deli_to)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();
    
    for (auto participant: participants_)       //CSE3310 (server)  messages are sent to all connected clients
    {
        if(participant->playerNo==deli_to)
            participant->deliver(msg);
    }
}
void poker_table::set_ready(bool status,int deli_to)
{
    for (auto participant: participants_)       //CSE3310 (server)  messages are sent to all connected clients
    {
        
        if(participant->playerNo==deli_to)
        {
            
            nlohmann::json to_player;
            participant->ready=status;
            std::cout<<"This is the set_ready function\nplayer: "<<deli_to<<" status: "<<status<<std::endl;
        }
    }
}
bool poker_table::allReady()        //checks if all the player in the servers are ready for the game
{
    bool all_ready = true;
    for (auto participant: participants_)       //set all_ready to false if all players aren't ready
    {
        
        if(participant->ready==false)
        {
            all_ready=false;
        }
    }
    return all_ready;
}
int poker_table::getSize()
{
    return participants_.size();
}
