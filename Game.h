#ifndef Game_h
#define Game_h
#include "Board.h"
#include "Player.h"
#include "Side.h"
class Game {
public:
    //Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
    Game(const Board& b, Player* south, Player* north);
    
    //Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
    void display() const;
    
    //If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
    void status(bool& over, bool& hasWinner, Side& winner) const;
    
    //Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    bool move(Side s);
    
    //Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
    void play();
    
    //Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
    int beans(Side s, int hole) const;
private:
    // prints out a new line
//    void clearScreen();
    // the board associated with this game
    Board m_board;
    // an array of size NSIDES (2 assuming we just have North and South) with pointers to the players corresponding to each side.
    Player* m_players[NSIDES];
    // is the game over, are both players both not interactive
    bool m_over, m_allRobots;
};
#endif /* Game_h */
