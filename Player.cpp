//
//  Player.cpp
//  Project3
//
//  Created by Teresa Lee on 5/24/23.
//

#include <stdio.h>
#include <iostream>
#include "Player.h"

using namespace std;

// PLAYER IMPLEMENTATION
Player::Player(string name) {
    // Create a Player with the indicated name.
    mName = name;
}

string Player::name() const {
    // Return the name of the player.
    return mName;
}

bool Player::isInteractive() const {
    // Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    return false;
}

Player::~Player() {
    // Since this class is designed as a base class, it should have a virtual destructor.
}


// HUMAN PLAYER IMPLEMENTATION
HumanPlayer::HumanPlayer(string name) : Player(name)
{}

bool HumanPlayer::isInteractive() const {
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const {
    int move = 0;
    // I used a do while loop so that the cout occurs at least once
    do {
        cout << "Enter a valid move: ";
        cin >> move;
    } while (move <= 0 || move > b.holes() || b.beans(s, move) == 0);
    return move;
}

HumanPlayer::~HumanPlayer()
{}


// BAD PLAYER IMPLEMENTATION
BadPlayer::BadPlayer(string name) : Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const {
    // BadPlayer will always choose the leftmost hole that has more than 0 beans
    for (int i = 1; i <= b.holes(); i++) {
        if (b.beans(s, i) > 0) {
            return i;
        }
    }
    return -1;
}

BadPlayer::~BadPlayer()
{}


// SMART PLAYER IMPLEMENTATION
SmartPlayer::SmartPlayer(string name) : Player(name)
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
    // I need a recursive chooseMove that keeps track of my values such as depth
    int bestHole = 0;
    int value = 0;
    int depth = 0;
    chooseMove(s, b, bestHole, value, depth);
    return bestHole;
}

int SmartPlayer::evaluate(const Board &b) const {
    // if there are 0 beans on one of the sides, return negative or positive infinity accordingly
    if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0)  {
        if (b.beans(SOUTH, 0) > b.beans(NORTH, 0)) {
            return 1000000;
        } else {
            return -1000000;
        }
    // else, return the difference between the south and north
    } else {
        return b.beans(SOUTH, 0) - b.beans(NORTH, 0);
    }
}

void SmartPlayer::makeMove(Board &board, Side s, int hole, int depth) const {
    Side endSide;
    int endHole;
    board.sow(s, hole, endSide, endHole);
    // multiple turns, mostly copied over from Game
    while (endSide == s && endHole == 0) {
        if (board.beansInPlay(s) == 0) {
            for (int i = 1; i <= board.holes(); i++) {
                board.moveToPot(opponent(s), i, opponent(s));
            }
            return;
        }
        int bestHole = -1;
        int value;
        chooseMove(s, board, bestHole, value, depth+1);
        if (bestHole == -1) {
            return;
        }
        board.sow(s, bestHole, endSide, endHole);
    }
    // capture (there might be an issue if you go around the board more than once), mostly copied over from Game
    if (endSide == s && board.beans(s, endHole) == 1 && board.beans(opponent(s), endHole) > 0) {
        board.moveToPot(s, endHole, s);
        board.moveToPot(opponent(s), endHole, s);
    }
    
}


void SmartPlayer::chooseMove(Side player, const Board &board, int &bestHole, int &value, int depth) const {
    // this function is based on the pseudocode provided
    if (board.beansInPlay(player) == 0) {
        bestHole = -1;
        value = evaluate(board);
        return;
    }
    if (depth > 6) {
        bestHole = -1;
        value = evaluate(board);
        return;
    }
    // initializing the values to the worst possible
    bestHole = -1;
    if (player == SOUTH) {
        value = -1000000;
    } else {
        value = 1000000;
    }
    // go through each hole, and call chooseMove on each child node until we've reached our depth
    for (int h = 1; h <= board.holes(); h++) {
        if (board.beans(player, h) == 0)
            continue;
        Board temp = board;
        // i make a temp board for the "make move" and "unmake move"
        makeMove(temp, player, h, depth);

        int opponentBestHole;
        int opponentValue;
        // we need to add 1 to our depth during our recursion
        chooseMove(opponent(player), temp, opponentBestHole, opponentValue, depth+1);
        int currentValue = evaluate(temp);
        // resetting the values if they were better than our previous ones
        if (player == SOUTH) {
            if (currentValue >= value) {
                bestHole = h;
                value = currentValue;
            }
        } else {
            if (currentValue <= value) {
                bestHole = h;
                value = currentValue;
            }
        }
    }

    return;

}

SmartPlayer::~SmartPlayer()
{}
