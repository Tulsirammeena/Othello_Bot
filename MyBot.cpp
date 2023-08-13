/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace Desdemona;
// int test1(OthelloBoard & temp_board , int height , int min , int max);
// int tester(OthelloBoard &temporary_board , int height , int min ,int  optimial_val);
// int othelloBoardEvaluator( OthelloBoard & board , Turn turn );
// Turn turn2;
// int move_num = 0;

#define INF 999999
class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread.
         */

        MyBot( Turn turn );

        virtual Move play( const OthelloBoard& board );
        virtual int  test1(const OthelloBoard& board, int height, int min, int max);
        virtual int tester(const OthelloBoard& board, int height, int min, int optimial_val);
        virtual int othelloBoardEvaluator(const OthelloBoard& board, Turn turn2);
        int move_num = 0;
        Turn turn2;

    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

Move MyBot::play( const OthelloBoard& board )
{
    move_num++;

    int height = 2;
    int optimum_value = -INF ;
    list<Move> moves = board.getValidMoves( turn );
    if(move_num == 1) turn2 = turn;
    int valid_moves= moves.size();

    list<Move>::iterator it = moves.begin();
    int new_value;
    Move our_move= *it;
    for(int i=1;i< valid_moves;it++,i++)
        {
            OthelloBoard temp_board= board;
            Turn trn = turn;
            temp_board.makeMove(trn , *it);
            new_value= test1( temp_board, height , -INF , INF );

            if( new_value > optimum_value)
            {
                optimum_value= new_value;
                our_move=*it;
            }

        }
    return our_move;
}
extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


int MyBot::test1(const OthelloBoard & temp_board , int height , int min , int max)
{
     Turn other_player= other(turn);
     list<Move> valid_moves= temp_board.getValidMoves(other_player);
     list<Move>::iterator it= valid_moves.begin();
     int optimial_val= max, new_value;
     for(int i=0;i<valid_moves.size();it++,i++)
     {
              OthelloBoard temporary_board= temp_board;
              Turn trn_b = other_player;
              temporary_board.makeMove(trn_b, *it);


              // if((height - 1) != 0)
               new_value= tester(temporary_board, height-1, min , optimial_val);
              //
              // else
               //new_value= othelloBoardEvaluator(temporary_board , turn2 );;


              if(new_value< optimial_val )
              {
                optimial_val= new_value;
               if( optimial_val<min)
               return min;
             }
     }

     return optimial_val;
}

int MyBot::tester(const OthelloBoard &temporary_board , int height , int min ,int  optimial_val)
{
  if(height == 0){
    OthelloBoard my_brd = temporary_board;
    return othelloBoardEvaluator(my_brd, turn2);
   }

          list<Move> moves = temporary_board.getValidMoves(turn);
          list<Move> ::iterator it= moves.begin();
          int alpha= min , new_value;
          for(int i=0;i<moves.size();it++,i++)
          {
            Turn trn_c = turn;
            OthelloBoard temp_board = temporary_board;
            temp_board.makeMove(trn_c , *it);
             new_value = test1(temp_board,height,alpha, optimial_val);
             if(new_value>alpha)
             {
                alpha=new_value;



             if(alpha> optimial_val)
                  return optimial_val;
          }
        }
          return alpha;

}

int MyBot::othelloBoardEvaluator(const OthelloBoard & board , Turn turn2 )
{
    Turn other_player= other(turn2);
    // int grid[8][8] = 	{ { 20 , -3 , 11 , 8  , 8  , 11  , -3 , 20 },
    // 				      { -3 , -7 , -4 , 1  , 1  , -4  , -7 , -3 },
    // 				      { 11 , -4 , 2  , 2  , 2  , 2   , -4 , 11 },
    // 				      { 8  , 1  , 2  , -3 , -3 , 2   , 1  , 8 },
    // 				      { 8  , 1  , 2  , -3 , -3 , 2   , 1  , 8 },
    // 			          { 11 , -4 , 2  , 2  , 2  , 2   , -4 , 11 },
    // 				      { -3 , -7 , -4 , 1  , 1  , -4  , -7 , -3 },
    // 			    	  { 20 , -3 , 11 , 8  , 8  , 11  , -3 , 20 } };
    int grid[8][8] = 	{ { 12,-3,2,2,2,2,-3,12},
    				      { -3,-4,-1,-1,-1,-1,-4,-3 },
    				      { 2,-1,1,0,0,1,-1,2 },
    				      {2,-1,0,1,1,0,-1,2},
    				      {2,-1,0,1,1,0,-1,2},
    			          { 2,-1,1,0,0,1,-1,2 },
    				      { -3,-4,-1,-1,-1,-1,-4,-3 },
    			    	  { 12,-3,2,2,2,2,-3,12 } };
    int sum_turn=0;
    int sum_other_player=0;

    for(int i=0;i<8;i++)
    {
     for(int j=0;j<8;j++)
     {
        if(board.get(i,j) == turn2 )
          sum_turn = sum_turn + grid[i][j];
        if(board.get(i,j)==other_player)
          sum_other_player = sum_other_player + grid[i][j];

     }}
     list<Move> move1 = board.getValidMoves(turn2);
     sum_turn = sum_turn + move1.size();
     list<Move> move2 = board.getValidMoves(other_player);
     sum_other_player= sum_other_player + move2.size();

    return sum_turn - sum_other_player;

}
