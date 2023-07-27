#include "Player.h"
#include "Side.h"
#include "Board.h"
#include <iostream>
#include <random>
#include <limits>
#include <algorithm>
#include <stack>
#include <cassert>
using namespace std;

//    Create a Player with the indicated name.
Player::Player(string name) : m_name(name) {}

//    Return the name of the player.
string Player::name() const { return m_name; }

//    Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
bool Player::isInteractive() const { return false; }

//    Since this class is designed as a base class, it should have a virtual destructor.
Player::~Player() {}

//    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
int HumanPlayer::chooseMove(const Board& b, Side s) const {
    if (b.beansInPlay(s) == 0) return -1;
    int holeChoice, holeContents;
    cout << "Enter a number between 1 and " << b.holes() << " corresponding to a nonempty hole on " << name() << "'s side to pick up from: ";
    cin >> holeChoice;
    while ((holeContents = b.beans(s, holeChoice)) < 1) {
        // we have to make sure the choice corresponds to a hole with beans in it.
        // If it is an invalid hole, beans will return -1
        if (holeContents == -1)
            cout << "\nSorry, that was an invalid number. Please enter a number between 1 and " << b.holes() << ": ";
        else
            cout << "\nSorry, that hole is empty. Please enter a number between 1 and " << b.holes() << " corresponding to a nonempty hole on " << name() << "'s side: ";
        cin >> holeChoice;
    }
    cout << endl;
    return holeChoice;
}

//    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
int BadPlayer::chooseMove(const Board& b, Side s) const {
    if (b.beansInPlay(s) == 0) return -1;
    int holeChoice, holeContents;
    default_random_engine generator;
    generator.seed(static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count()));
    // we seed the pseudo-random generator to make it more random
    uniform_int_distribution<int> distribution(1,b.holes());
    auto dice = bind(distribution, generator);  // generates number in the range 1-b.holes() with equal possibilities
    do {
        holeContents = b.beans(s, holeChoice = dice());
    } while (holeContents < 1); // we must pick a hole with at least 1 bean!
    return holeChoice;
}

int SmartPlayer::minimax(const Board& b) const{
    int diff = b.beans(static_cast<Side>(1), 0) - b.beans(static_cast<Side>(0), 0); // SOUTH - NORTH
    
    if (!b.beansInPlay(static_cast<Side>(1)) && !b.beansInPlay(static_cast<Side>(0))) {
        if (diff > 0) {return PINF;}
        else if (diff < 0) {return NINF;}
        else {return 0;}
    } else if (b.beans(static_cast<Side>(1), 0) > b.totalBeans()/2) {
        return PINF;
    } else if (b.beans(static_cast<Side>(0), 0) > b.totalBeans()/2) {
        return NINF;
    } else {return diff;}
}



//    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
int SmartPlayer::chooseMove(const Board& b, Side s) const {
    if (b.beansInPlay(s) == 0) return -1;
    else {
        int bestHole, value;
        double timeLimit = 4800;  // 4.80 seconds; allow 200 ms for wrapping up
        JumpyTimer timer(1000); // calls elapsed only every 1000 calls
        // unsigned long result =
        chooseMove(s, b, bestHole, value, 0, 15, timeLimit, timer); // max depth is 15 levels deep, will switch branches after
        assert(timer.actualElapsed() < 5000); // we shouldn't have exceeded 5 seconds
//        cerr << timer.actualElapsed() << " ms" << endl;
//        cerr << result / 1000000.0  << " million calls" << endl;
        assert(bestHole != -1); // there must be a valid choice if there are beans on this side (see above if statement)
        assert(b.beans(s, bestHole) > 0); // the number of beans in the hole must be positive
        return bestHole;
    }
}

unsigned long SmartPlayer::chooseMove(Side s, Board b, int& bestHole, int& value, int depth, const int maxDepth, double timeLimit, JumpyTimer& timer) const {
    unsigned long callCount = 1;
    // base case
    if (b.beansInPlay(s) == 0 || depth == maxDepth || timeLimit <= 0) {
        bestHole = -1;
        value = minimax(b);
    } else {
        stack<int> bestMoveTracker;
        int endHole, h2, v2;
        Side endSide;
        // break at infinities
        for (int h = 1; h <= b.holes(); h++) {
            Board copy(b);
            if (copy.sow(s, h, endSide, endHole)) {
                // Allocate remaining time limit equally among remaining braches to explore
                double thisBranchTimeLimit = timeLimit / (b.holes() - h + 1);

                // Save branch start time so can compute the time the branch took
                double startTime = timer.elapsed();

                // FINISH MAKING THE MOVE H
                if (endHole == 0) {
                    do {
                        // changed bestHole to h1
                        callCount += chooseMove(s, copy, bestHole, v2, depth, maxDepth, thisBranchTimeLimit/b.holes(), timer);
                        // will only return the best hole for Side s since the other ones don't matter
                        
                        // Adjust remaining time
                        thisBranchTimeLimit -= (timer.elapsed() - startTime);
                    } while (copy.sow(s, bestHole, endSide, endHole) && endHole == 0);
                } // finishes the extra move(s).
                // now either bestHole == -1 or endHole != 0
                assert(endHole != 0 || bestHole == -1);
                if (endSide == s && copy.beans(endSide, endHole) == 1 && copy.beans(opponent(endSide), endHole) > 0) {
                    copy.moveToPot(endSide, endHole, s);
                    copy.moveToPot(opponent(endSide), endHole, s);
                } // finishes the captures
                
                // OPPONENT'S COUNTER-MOVE
                callCount += chooseMove(opponent(s), copy, h2, v2, depth + 1, maxDepth, thisBranchTimeLimit, timer);
                
                // Adjust remaining time
                timeLimit -= (timer.elapsed() - startTime);
                if (timeLimit <= 0) {timeLimit = 0;}
                
                if (bestMoveTracker.empty()) {
                    bestMoveTracker.push(h);
                    bestMoveTracker.push(v2);
                } else if (s == SOUTH && bestMoveTracker.top() < v2) {
                    bestMoveTracker.pop();
                    bestMoveTracker.pop();
                    bestMoveTracker.push(h);
                    bestMoveTracker.push(v2);
                } else if (s == NORTH && bestMoveTracker.top() > v2) {
                    bestMoveTracker.pop();
                    bestMoveTracker.pop();
                    bestMoveTracker.push(h);
                    bestMoveTracker.push(v2);
                }
                if (s == SOUTH && bestMoveTracker.top() == PINF) break;
                else if (s == NORTH && bestMoveTracker.top() == NINF) break;
            }
        }
        
        value = bestMoveTracker.top();
        bestMoveTracker.pop();
        bestHole = bestMoveTracker.top();
        bestMoveTracker.pop();
        assert(bestMoveTracker.empty()); // our stack should only have 2 values at a time
    }
    return callCount;
}

/*
unsigned long f(int n)
{
    double timeLimit = 4990;  // 4.99 seconds; allow 10 ms for wrapping up
    JumpyTimer timer(1000);
    unsigned long result = f_helper(n, timeLimit, timer);
    cout << timer.actualElapsed() << " ms" << endl;
    return result;
}

// n is max depth to go to
unsigned long f_helper(int n, double timeLimit, JumpyTimer& timer)
{
    unsigned long callCount = 1;
    if (n == 0)
        callCount += 0;  /// do a base case computation
    else if (timeLimit <= 0)
        callCount += 0;  //  do non-recursive heuristic computation
    else
    {
        for (int branchesLeftToExplore = 10; branchesLeftToExplore > 0; branchesLeftToExplore--)
        {
              // Allocate remaining time limit equally among remaining braches to explore
            double thisBranchTimeLimit = timeLimit / branchesLeftToExplore;

              // Save branch start time so can compute the time the branch took
            double startTime = timer.elapsed();
              // Explore this branch
            callCount += f_helper(n-1, thisBranchTimeLimit, timer);
              // Adjust remaining time
            timeLimit -= (timer.elapsed() - startTime);
            if (timeLimit <= 0)
                timeLimit = 0;
        }
    }
    return callCount;
}

*/
