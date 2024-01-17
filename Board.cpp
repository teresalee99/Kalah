//
//  Board.cpp
//  Project3
//
//  Created by Teresa Lee on 5/23/23.
//

#include <iostream>
#include <stdio.h>
#include "Board.h"

using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole) {
    // Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
    // if a board is constructed with invalid values, set them to the default value
    if (nHoles <= 0) {
        nHoles = 1;
    }
    if (nInitialBeansPerHole < 0) {
        nInitialBeansPerHole = 0;
    }
    
    // setting number of beans in hole#0 (pot) to 0
    north.push_back(0);
    south.push_back(0);
    // setting number of beans to rest of the holes
    for (int i = 0; i < nHoles; i++) {
        north.push_back(nInitialBeansPerHole);
        south.push_back(nInitialBeansPerHole);
    }
    
}

int Board::holes() const {
    // Return the number of holes on a side (not counting the pot).
    return (int)(north.size())-1;
}

int Board::beans(Side s, int hole) const {
    // Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
    // if hole is greater than the amount of holes or less than 0, it is invalid
    if (hole > holes() || hole < 0) {
        return -1;
    }
    if (s == NORTH) {
        return north[hole];
    } else {
        return south[hole];
    }
}

int Board::beansInPlay(Side s) const {
    // Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    int count = 0;
    // if else to check what side it is
    if (s == NORTH) {
        for (int i = 1; i <= holes(); i++) {
            count += north[i];
        }
    } else {
        for (int i = 1; i <= holes(); i++) {
            count += south[i];
        }
    }
    return count;
}

int Board::totalBeans() const {
    // Return the total number of beans in the game, including any in the pots.
    int count = 0;
    for (int i = 0; i <= holes(); i++) {
        count += north[i];
    }
    for (int i = 0; i <= holes(); i++) {
        count += south[i];
    }
    return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    // If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
    int numOfBeans = 0;
    Side ownSide = s;
    // setting number of beans if valid parameters are passed
    if (s == NORTH) {
        if (hole <= 0 || hole > north.size() || north[hole] == 0) {
            return false;
        }
        numOfBeans = north[hole];
        north[hole] = 0;
    } else if (s == SOUTH) {
        if (hole <= 0 || hole > south.size() || south[hole] == 0) {
            return false;
        }
        numOfBeans = south[hole];
        south[hole] = 0;
    }
    int prevHole = hole;
    while (numOfBeans > 0) {
        if (s == SOUTH) {
            // if the previous hole is the greatest hole (ie if there are 6 holes and the previous hole was hole#6), then we need to add a bean to the pot and change the values of prevHole and numOfBeans accordingly
            if (prevHole == holes()) {
                if (ownSide == SOUTH) {
                    south[0] += 1;
                    prevHole = 0;
                    numOfBeans -= 1;
                } else {
                    prevHole = 0;
                }
            // if the previous hole was the pot, we need to switch sides
            } else if (prevHole == 0) {
                north[holes()] += 1;
                s = NORTH;
                prevHole = holes();
                numOfBeans -= 1;
            // in all other cases, go down the holes on the north side
            } else {
                south[prevHole+1] += 1;
                prevHole += 1;
                numOfBeans -= 1;
            }
        }
        else if (s == NORTH) {
            // if the previous hole is the smallest hole (ie if there are 6 holes and the previous hole was hole#1), then we need to add a bean to the pot and change the values of prevHole and numOfBeans accordingly
            if (prevHole == 1) {
                if (ownSide == NORTH) {
                    north[0] += 1;
                    prevHole = 0;
                    numOfBeans -= 1;
                } else {
                    prevHole = 0;
                }
            // if the previous hole was the pot, we need to switch sides
            } else if (prevHole == 0) {
                south[1] += 1;
                s = SOUTH;
                prevHole = 1;
                numOfBeans -= 1;
            // in all other cases, go up the holes on the south side
            } else {
                north[prevHole - 1] += 1;
                prevHole -= 1;
                numOfBeans -= 1;
            }
        }
        // set values accordingly after we reach 0 beans left
        if (numOfBeans == 0) {
            endSide = s;
            endHole = prevHole;
        }
    }
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {
    // If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
    if (hole > holes() || hole <= 0) {
        return false;
    }
    int numOfBeans = 0;
    // check to see which side we're on to update numOfBeans and make the number of beans in that hole 0
    if (s == NORTH) {
        numOfBeans = north[hole];
        north[hole] = 0;
    } else {
        numOfBeans = south[hole];
        south[hole] = 0;
    }
    // add the amount of beans to the correct pot
    if (potOwner == NORTH) {
        north[0] += numOfBeans;
    } else {
        south[0] += numOfBeans;
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans) {
    // If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
    if (hole > holes() || hole < 0 || beans < 0) {
        return false;
    }
    if (s == NORTH) {
        north[hole] = beans;
    } else {
        south[hole] = beans;
    }
    return true;
}
