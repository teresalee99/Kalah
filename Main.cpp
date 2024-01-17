//
//  main.cpp
//  Project3
//
//  Created by Teresa Lee on 5/23/23.
//

#include <iostream>
#include "Board.h"
#include "Player.h"
#include "Game.h"

using namespace std;

int main() {
    Board test(5,5);

    HumanPlayer south("Teresa");
    SmartPlayer north("Ricky");

    Game game(test, &south, &north);
    game.play();



// previous tests for Board, won't work anymore since test is changed
//        cout << "Size: " << test.holes() << endl;
//        cout << "Number of beans test: " << test.beans(NORTH, 3) << endl;
//        cout << "One side beans test: " << test.beansInPlay(SOUTH) << endl;
//        cout << "Total beans test: " << test.totalBeans() << endl;
//        Side endSide = SOUTH;
//        int endHole = 0;
//        test.sow(SOUTH, 6, endSide, endHole);
//        cout << endSide << " " << endHole << endl;
//        test.sow(SOUTH, 2, endSide, endHole);
//        cout << endSide << " " << endHole << endl;
//        cout << "One side beans test: " << test.beansInPlay(SOUTH) << endl;
//        cout << "Total beans test: " << test.totalBeans() << endl;
//        test.setBeans(NORTH, 1, 15);
//        cout << "One side beans test: " << test.beansInPlay(NORTH) << endl;
//        cout << "Total beans test: " << test.totalBeans() << endl;

    return 0;
}

