//
//  Player.h
//  Project3
//
//  Created by Teresa Lee on 5/24/23.
//

#ifndef Player_h
#define Player_h

#include "Board.h"

#include <string>

using namespace std;

class Player {
public:
    Player(std::string name);
    // Create a Player with the indicated name.
    std::string name() const;
    // Return the name of the player.
    virtual bool isInteractive() const;
    // Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    // Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();
    // Since this class is designed as a base class, it should have a virtual destructor.
private:
    string mName;
};

class BadPlayer : public Player {
public:
    BadPlayer(string name);
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~BadPlayer();
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~HumanPlayer();
};

class SmartPlayer : public Player {
public:
    SmartPlayer(string name);
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~SmartPlayer();
private:
    int evaluate(const Board &b) const;
    void makeMove(Board &board, Side s, int hole, int depth) const;
    void chooseMove(Side player, const Board &board, int &bestHole, int &value, int depth) const;
};

#endif /* Player_h */
