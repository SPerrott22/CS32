#ifndef Player_h
#define Player_h

#include "Side.h"
#include "Board.h"
#include <string>

//    Player is an abstract base class that defines a common interface that all kinds of players (human and various computer players) must implement. It must support these public member functions:
class Player {

public:
    //    Create a Player with the indicated name.
    Player(std::string name);
    
    //    Return the name of the player.
    std::string name() const;
    
    //    Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual bool isInteractive() const;
    
    //    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const = 0;
    
    //    Since this class is designed as a base class, it should have a virtual destructor.
    virtual ~Player();
    
private:
    std::string m_name;
    
};

//    Each concrete class derived from Player will implement the chooseMove function in its own way. Of the classes listed here, only HumanPlayer::isInteractive should return true. (When testing, we may supply other kinds of interactive players.) Each of the three classes listed here must have a constructor taking a string representing the name of the player.

//A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid hole number), and returning that choice. We will never test for a situation where the user doesn't enter an integer when prompted for a hole number. (The techniques for dealing with the issue completely correctly are a distraction to this project, and involve either a function like stoi or strtol, or the type istringstream.)
class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    //    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const;
    
    //    Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual bool isInteractive() const;

};
//A BadPlayer is a computer player that chooses an arbitrary valid move and returns that choice. "Arbitrary" can be what you like: leftmost, nearest to pot, fewest beans, random, etc.. The point of this class is to have an easy-to-implement class that at least plays legally.
class BadPlayer : public Player {
public:
    BadPlayer(std::string name);
    //    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const;
};

//Here's your chance to shine. A SmartPlayer chooses a valid move and returns it. For any game played on a board of up to six holes per side, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than five seconds on the SEASnet Linux server lnxsrv07.seas.ucla.edu or lnxsrv09.seas.ucla.edu: (We'll give you a way of determining the time soon; until then, you can meet this requirement by limiting the depth of your game tree search or the number of game tree positions you explore to a limit you determine experimentally.) SmartPlayer::chooseMove will be worth about 15-20% of the points for this project.
class SmartPlayer: public Player {
public:
    SmartPlayer(std::string name);
    //    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual int chooseMove(const Board& b, Side s) const;
private:
    unsigned long chooseMove(Side s, Board b, int& bestHole, int& value, int depth, const int maxDepth, double timeLimit, JumpyTimer& timer) const;
    int minimax(const Board& b) const;
};

inline HumanPlayer::HumanPlayer(std::string name) : Player(name) {}
inline BadPlayer::BadPlayer(std::string name) : Player(name) {}
inline SmartPlayer::SmartPlayer(std::string name) : Player(name) {}
inline bool HumanPlayer::isInteractive() const {return true;}

#endif /* Player_h */
    

/*
 The Heuristic is a prediction, it isn't definitive
 since you aren't going to go through the entire tree
 to the leaves. No, you just guess and avoid stupid moves.
 
 */
