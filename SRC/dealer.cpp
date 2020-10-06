#include "dealer.h"

dealer::dealer()
{
}
int dealer::compareHands(int hand1[], int hand2[], int hand3[], int hand4[], int hand5[])
{
    int winner =0;
    int hands[5][6];
    for(int x=0;x<6;x++)
    {
        hands[0][x]= hand1[x];
        hands[1][x]= hand2[x];
        hands[2][x]= hand3[x];
        hands[3][x]= hand4[x];
        hands[4][x]= hand5[x];
    }
    for(int x=1;x<5;x++)
    {
        for(int y=0;y<6;y++)
        {
            if(hands[winner][y]>hands[x][y])
            {
            break;
            }
            else if(hands[winner][y]<hands[x][y])
            {
            winner = x;
            break;
            }
        }
    }
    return winner+1;
}
void dealer::createDeck()
{
//this function creates a Deck and shuffled it up

   int i;
    //push cards on cards vector
    //first 2 digits are value, last is face
    //1 is clubs
    //2 is diamonds
    //3 is hearts
    //4 is spades
    //2 for 2, 3 for 3...,11 for Jack, 12 for Queen, 13 for King, 14 for Ace
for(i = 2; i<=14; i++)
{
    for(int y=1;y<=4;y++)
    {
    Deck.push_back(i * 10 + y);
    }
}
  
  std::srand ( unsigned ( std::time(0) ) );
  // using built-in random generator:
  std::random_shuffle ( Deck.begin(), Deck.end() );
  // using myrandom:
  std::random_shuffle ( Deck.begin(), Deck.end(), [&](int i) {return std::rand()%i;});
}
int dealer::organizeAndRank(int * hand, int * rankHand)
{

    int suitHand[5];
    int valueHand[5];
    int lowHand[] = {14, 5, 4, 3, 2};
    bool pass1 = true;
    bool pass2 = true;
    int temp;
    std::sort(hand, hand + 5, std::greater<int>());
    for(int x=0;x<6;x++)//reseting the rankHand
    {rankHand[x]=0;}
    for(int x=0;x<5;x++)
    {
        valueHand[x] = hand[x] / 10;
        suitHand[x] = hand[x] % 10;
    }
    for (int x=0;x<4;x++)
    {
        if(suitHand[x]!=suitHand[x+1]){pass1 = false;}//not a flush -> set false to something other than 10, 9, 6
    }
    if (pass1)//is a flush
    {
        if(valueHand == lowHand)//use ace as low card for straight flush
        {
        rankHand[0]=9;
        rankHand[1]=5;
        //std::cout << "low straight flush" << std::endl;
        return 0;
        }
        pass1 = true;
        for(int x=0; x<4; x++)//straight flush?
        {
            if(valueHand[x]!=valueHand[x+1]+1){pass1 = false;}
        }
        if(pass1)//if straight flush or royal flush
        {
            if(valueHand[0]==14)//if royal flush
            {
            rankHand[0]=10;
            //std::cout << "royal flush" << std::endl;
            return 0;
            }
            rankHand[0]=9;
            for(int x=0;x<5;x++)
            {
                rankHand[0]=9;
                rankHand[1]=valueHand[0];
                //std::cout << "straight flush" << std::endl;
                return 0;
            }

        }
        else//at this point it is a flush but not a straight flush or royal flush
        {
            rankHand[0]=6;
            for(int x=1;x<6;x++){rankHand[x]=valueHand[x-1];}//fill rest of handRank
            //std::cout << "flush" << std::endl;
            return 0;
        }
    }
    pass1 = true;
    int num1 = 0;
    int num2 = 0;
    int freq1 = 1;
    int freq2 = 1;
    for(int x=0;x<5;x++)
    {
        if(freq1==1){num1 = valueHand[x];}
        else if(freq2==1 && valueHand[x] != num1){num2 = valueHand[x];}
        for(int y=x+1;y<5;y++)
        {
            if(valueHand[y]==num1 && pass1){num1=valueHand[y]; freq1++;}//first "kind" is found and is ready to be checked for 3 of a kind
            else if(valueHand[y]==num2 && pass2){num2=valueHand[y]; freq2++;}//second "kind" is found and is ready to be checked for 3 of a kind
        }
        if(freq1 != 1){pass1 = false;}//stop incrementing for kinds if found
        if(freq2 != 1){pass2 = false;}

    }
    if(freq1 == 4)//4 of a kind
    {
        if(valueHand[0]!=num1)
        {
        temp = hand[4];
        hand[4]=hand[0];
        hand[0]=temp;
        }
        rankHand[0]=8;
        rankHand[1]=num1;
        rankHand[2]=valueHand[4];
        //std::cout << "4 of a kind" << std::endl;
        return 0;
    }
    if(freq1 == 3 || freq2 == 3)//at least one 3 of a kind
    {
        if(freq1 == 2 || freq2 == 2)//full house
        {
            if(valueHand[2] == num2 && valueHand[4] == num2)//check middle (three of a kind) and see if it's at the front. ie check for swap
            {
            temp = hand[0];
            hand[0] = hand[4];
            hand[4] = temp;
            temp = hand[1];
            hand[1] = hand[3];
            hand[3] = temp;
            temp = valueHand[0];
            valueHand[0] = valueHand[4];
            valueHand[4] = temp;
            temp = valueHand[1];
            valueHand[1] = valueHand[3];
            valueHand[3] = temp;
            }
            rankHand[0] = 7;
            rankHand[1] = num1;
            rankHand[2] = num2;
            //std::cout << "full house" << std::endl;
            return 0;
        }
        if(valueHand[0]==num1)//only one three of a kind
        {}
        else if(valueHand[1]==num1)
        {
            temp=hand[0];
            hand[0]=hand[3];
            hand[3]=temp;
            temp=valueHand[0];
            valueHand[0]=valueHand[3];
            valueHand[3]=temp;
        }
        else
        {
            temp=hand[0];
            hand[0]=hand[3];
            hand[3]=temp;
            temp=hand[1];
            hand[1]=hand[4];
            hand[4]=temp;
            temp=valueHand[0];
            valueHand[0]=valueHand[3];
            valueHand[3]=temp;
            temp=valueHand[1];
            valueHand[1]=valueHand[4];
            valueHand[4]=temp;
        }
        rankHand[0]=4;
        rankHand[1]=num1;
        rankHand[2]=valueHand[3];
        rankHand[3]=valueHand[4];
        //std::cout << "three of a kind" << std::endl;
        return 0;
        
    }
    if(freq1 == 2 || freq2 == 2)//at least one pair
    {
        if(freq1 == 2 && freq2 == 2)//two pair
        {
            for(int x=0;x<5;x++)
            {
                if(valueHand[x]!=num1 && valueHand[x]!=num2)
                {
                    if(x==0)
                    {
                        temp=hand[0];
                        hand[0]=hand[2];
                        hand[2]=temp;
                        temp=hand[4];
                        hand[4]=hand[2];
                        hand[2]=temp;
                        temp=valueHand[0];
                        valueHand[0]=valueHand[2];
                        valueHand[2]=temp;
                        temp=valueHand[4];
                        valueHand[4]=valueHand[2];
                        valueHand[2]=temp;
                    }
                    else if(x==2)
                    {
                        temp=hand[4];
                        hand[4]=hand[2];
                        hand[2]=temp;
                        temp=valueHand[4];
                        valueHand[4]=valueHand[2];
                        valueHand[2]=temp;
                    }
                    rankHand[0]=3;
                    rankHand[1]=num1;
                    rankHand[2]=num2;
                    rankHand[3]=valueHand[4];
                    //std::cout << "two pair" << std::endl;
                    return 0;
                }
            }
        }
        if(valueHand[0]==num1)//only one pair
        {}
        else if(valueHand[1]==num1)
        {
            temp=hand[2];
            hand[2]=hand[0];
            hand[0]=temp;
            temp=valueHand[2];
            valueHand[2]=valueHand[0];
            valueHand[0]=temp;
        }
        else if(valueHand[2]==num1)
        {
            temp=hand[2];
            hand[2]=hand[0];
            hand[0]=temp;
            temp=hand[3];
            hand[3]=hand[1];
            hand[1]=temp;
            temp=valueHand[2];
            valueHand[2]=valueHand[0];
            valueHand[0]=temp;
            temp=valueHand[3];
            valueHand[3]=valueHand[1];
            valueHand[1]=temp;
        }
        else
        {
            temp=hand[3];
            hand[3]=hand[0];
            hand[0]=temp;
            temp=hand[4];
            hand[4]=hand[1];
            hand[1]=temp;
            temp=hand[3];
            hand[3]=hand[2];
            hand[2]=temp;
            temp=hand[3];
            hand[3]=hand[4];
            hand[4]=temp;
            temp=valueHand[3];
            valueHand[3]=valueHand[0];
            valueHand[0]=temp;
            temp=valueHand[4];
            valueHand[4]=valueHand[1];
            valueHand[1]=temp;
        }
        rankHand[0]=2;
        rankHand[1]=num1;
        rankHand[2]=valueHand[2];
        rankHand[3]=valueHand[3];
        rankHand[4]=valueHand[4];
        //std::cout << "one pair" << std::endl;
        return 0;
    }
    if(valueHand==lowHand)//test for straight with ace low card
    {
        rankHand[0]=5;
        rankHand[1]=5;
        //std::cout << "straight" << std::endl;
        return 0;
    }
    pass1 = true;
    for(int x=0;x<4;x++)//test for straight
    {
        if(valueHand[x]!=valueHand[x+1]+1){pass1=false;}
    }
    if(!pass1)//high card or "junk"
    {
        rankHand[0]=1;
        for(int x=0;x<5;x++)
        {
        rankHand[x+1]=valueHand[x];
        }
        //std::cout << "high card" << std::endl;
        return 0;
    }
    else//straight
    {
        rankHand[0]=5;
        rankHand[1]=valueHand[0];
        //std::cout << "straight" << std::endl;
        return 0;
    }
}
std::vector<int> dealer::getDeck()
{
   return Deck;
}
