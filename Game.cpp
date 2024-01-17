//
//  Game.cpp
//  Project3
//
//  Created by Teresa Lee on 5/24/23.
//

#include <stdio.h>
#include <iostream>

#include "Game.h"

using namespace std;

Game::Game(const Board& b, Player* south, Player* north) : board(b) {
    // Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
    southPlayer = south;
    northPlayer = north;
    // south player goes first
    currentPlayer = SOUTH;
}

void Game::display() const {
    // Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
    cout << "North Player: " << northPlayer->name() << endl;
    cout << "    ";
    for (int i = 1; i <= board.holes(); i++) {
        cout << board.beans(NORTH, i) << "   ";
    }
    // spacing is based on number of holes there are
    cout << endl << board.beans(NORTH, 0) << "   ";
    for (int i = 1; i <= board.holes(); i++) {
        cout << "    ";
    }
    cout << board.beans(SOUTH, 0) << endl << "    ";
    for (int i = 1; i <= board.holes(); i++) {
        cout << board.beans(SOUTH, i) << "   ";
    }
    cout << endl << "South Player: " << southPlayer->name() << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    // If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
    if (board.beansInPlay(NORTH) > 0 && board.beansInPlay(SOUTH) > 0) {
        over = false;
        return;
    }
    over = true;
    // if there are the same number of beans in the pots, then it is a tie game
    if (board.beans(NORTH, 0) == board.beans(SOUTH, 0)) {
        hasWinner = false;
    } else {
        hasWinner = true;
        // otherwise, set the winner accordingly
        if (board.beans(NORTH, 0) > board.beans(SOUTH, 0)) {
            winner = NORTH;
        } else {
            winner = SOUTH;
        }
    }
}

bool Game::move(Side s) {
    // Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    Side opponent;
    if (s == NORTH) {
        opponent = SOUTH;
    } else {
        opponent = NORTH;
    }
    // if there are no more moves, the game is over
    if (board.beansInPlay(s) == 0) {
        for (int i = 1; i <= board.holes(); i++) {
            board.moveToPot(opponent, i, opponent);
        }
        return false;
    }
    if (currentPlayer == NORTH) {
        cout << northPlayer->name() << "'s turn!" << endl;
    } else {
        cout << southPlayer->name() << "'s turn!" << endl;
    }
    int move = 0;
    if (s == NORTH) {
        move = northPlayer->chooseMove(board, NORTH);
    } else {
        move = southPlayer->chooseMove(board, SOUTH);
    }
    Side endSide;
    int endHole;
    // make a move at least once
    board.sow(s, move, endSide, endHole);
    cout << endl;
    display();
    cout << endl;
    // multiple turns
    while (endSide == s && endHole == 0) {
        // check to see if you can make moves. if you can't, move all remaining beans to the pot and the game is over
        if (board.beansInPlay(s) == 0) {
            for (int i = 1; i <= board.holes(); i++) {
                board.moveToPot(opponent, i, opponent);
            }
            return false;
        }
        cout << "Player gets another turn." << endl;
        if (s == NORTH) {
            move = northPlayer->chooseMove(board, NORTH);
        } else {
            move = southPlayer->chooseMove(board, SOUTH);
        }
        // this turn will continue until the player doesn't end on the pot on their own side
        board.sow(s, move, endSide, endHole);
        cout << endl;
        display();
        cout << endl;
    }
    // capture (there might be an issue if you go around the board more than once)
    if (endSide == s && beans(s, endHole) == 1 && beans(opponent, endHole) > 0) {
        if (s == NORTH) {
            cout << northPlayer->name() << " captured!" << endl << endl;
        } else {
            cout << southPlayer->name() << " captured!" << endl;

        }
        board.moveToPot(s, endHole, s);
        board.moveToPot(opponent, endHole, s);
        cout << endl;
        display();
        cout << endl;
    }
    currentPlayer = opponent;
    return true;
}

void Game::play() {
    // Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
    bool over = false;
    bool hasWinner = false;
    Side winner;
    cout << "Game begins!" << endl;
    cout << endl;
    display();
    cout << endl;
    // while the game is not over, call move to make moves
    while (over == false) {
        if (northPlayer->isInteractive() == false && southPlayer->isInteractive() == false) {
            cout << "Press enter to see the next move." << endl;
            cin.ignore();
        }
        if (board.beansInPlay(currentPlayer) > 0) {
            move(currentPlayer);
        } else
        {
            over = true;
        }
    }
    // depending on who has beans left, move them to the right pot
    if (board.beansInPlay(SOUTH) == 0) {
        for (int i = 1; i <= board.holes(); i++) {
            board.moveToPot(NORTH, i, NORTH);
        }
    } else {
        for (int i = 1; i <= board.holes(); i++) {
            board.moveToPot(SOUTH, i, SOUTH);
        }
    }
    status(over, hasWinner, winner);
    cout << "Game over. All remaining beans will be moved to its owner's pot." << endl;
    cout << endl;
    display();
    cout << endl;
    // cout the winner
    if (hasWinner == true) {
        if (winner == NORTH) {
            cout << northPlayer->name() << " won!" << endl << endl;
        } else {
            cout << southPlayer->name() << " won!" << endl << endl;

        }
    } else {
        cout << "Tie game!" << endl << endl;
    }
}

int Game::beans(Side s, int hole) const {
    // Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
    if (hole > board.holes() || hole < 0) {
        return -1;
    }
    return board.beans(s, hole);
}
