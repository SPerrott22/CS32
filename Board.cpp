#include "Board.h"
#include "Side.h"
#include <vector>
#include <cassert>

using namespace std;

// Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
Board::Board(int nHoles, int nInitialBeansPerHole) {
    if (nHoles < 1) m_nHolesPerSide = 1; // you can't make a board with 0 or fewer holes! Minimum will be 1
    else m_nHolesPerSide = nHoles;
    
    for (int j = 0; j < NSIDES; j++) {
        m_directions[j].push_back(0); // set the pots to 0
    }
    
    if (nInitialBeansPerHole < 0) nInitialBeansPerHole = 0; // you can't give initial beans a negative value, treat it as 0 minimum.
    
    for (int i = 0; i < m_nHolesPerSide; i++) {
        for (int j = 0; j < NSIDES; j++) {
            m_directions[j].push_back(nInitialBeansPerHole); // add the holes with nInitialBeansPerHole
        }
    }
}

// Return the number of holes on a side (not counting the pot).
int Board::holes() const {
    return m_nHolesPerSide;
}

//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const {
    if (hole < 0 || hole > m_nHolesPerSide) return -1; // hole number must be in 0..holes()
    else return m_directions[s][hole];
}

// Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::beansInPlay(Side s) const {
    int nBeansOnSide(0);
    for (int i = 1; i <= m_nHolesPerSide; i++) {
        nBeansOnSide += m_directions[s][i];
    }
    return nBeansOnSide;
}

//Return the total number of beans in the game, including any in the pots.
int Board::totalBeans() const {
    int nBeans(0);
    for (int j = 0; j < NSIDES; j++) {
        for (int i = 0; i <= m_nHolesPerSide; i++) {
            nBeans += m_directions[j][i];
        }
    }
    return nBeans;
}

// If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    int hand(beans(s, hole));
    if (hand <= 0) return false;
    
    m_directions[s][hole] = 0;
    
    endSide = s;
    endHole = hole;
    sow(hand, s, endSide, endHole);
    return true;
}

void Board::sow(int hand, Side playerSide, Side& endSide, int& endHole) {
    if (hand <= 0) {return;}
    else if (endHole == 0) { // that is we ended in a pot
        assert(endSide == playerSide); // we could've only reached our own pot
        if (endSide == 1) {endHole = m_nHolesPerSide;} // if we're South
        else {endHole++;} // if we're North
        endSide = opponent(endSide); // we switch sides in both cases
    } else if (endHole == m_nHolesPerSide && endSide == 1) { // if we're at (SOUTH, 6)
        if (playerSide == 1) {endHole = 0;} // go to pot if we're SOUTH
        else {endSide = opponent(endSide);} // skip pot if we're NORTH
    } else if (endHole == 1 && endSide == 0) { // if we're at (NORTH, 1)
        if (playerSide == 0) {endHole = 0;} // go to pot if we're NORTH
        else {endSide = opponent(endSide);} // skip pot if we're SOUTH
    } else if (endSide == 0) {
        endHole--; // decrement endHole if we're on NORTH's side
    } else {
        endHole++; // increment if we're on SOUTH's side
    }
//    if (hand == 1 && m_directions[endSide][endHole] > 0) {
//        hand += m_directions[endSide][endHole];
//        m_directions[endSide][endHole] = 0;
//    } else {
    m_directions[endSide][endHole]++; // add a bean
    sow(--hand, playerSide, endSide, endHole); // decrement the hand and call again
}

// If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::moveToPot(Side s, int hole, Side potOwner) {
    if (hole <= 0 || hole > m_nHolesPerSide) return false;
    else {
        m_directions[potOwner][0] += beans(s, hole);
        m_directions[s][hole] = 0;
        return true;
    }
}

// If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
bool Board::setBeans(Side s, int hole, int beans) {
    if (hole < 0 || hole > m_nHolesPerSide || beans < 0) {return false;}
    else {
        m_directions[s][hole] = beans;
        return true;
    }
}

