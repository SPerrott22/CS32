#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
Game::Game(const Board& b, Player* south, Player* north) : m_board(b), m_over(false), m_allRobots(false) {
    m_players[0] = north; // 0 corresponds to NORTH corresponds to false
    m_players[1] = south; // 1 corresponds to SOUTH corresponds to true
    if (!m_players[0]->isInteractive() && !m_players[1]->isInteractive()) m_allRobots = true;
}

//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
// Note to self: if you have extra time, use printf for the numbers so that if it is two digits, three digits it will work same.
void Game::display() const {
    cout << "   " << m_players[0]->name() << endl; // name of North
    for (int i = 1; i <= m_board.holes(); i++) {
      printf("%3d", m_board.beans(static_cast<Side>(0), i)); // print out the north holes, right aligned with 3 spaces allocated per hole
    }
    cout << endl;
    printf("%-2d", m_board.beans(static_cast<Side>(0), 0)); // print out the north pot, 2 spaces allocated left aligned
    cout << " ";
    for (int i = 1; i < m_board.holes(); i++) { // adequate spacing proportional to number of holes (3 spaces per)
      cout << "   ";
    }
    printf("%2d", m_board.beans(static_cast<Side>(1), 0)); // print out the south pot, 2 spaces right aligned
    cout << endl;
    for (int i = 1; i <= m_board.holes(); i++) {
      printf("%3d", m_board.beans(static_cast<Side>(1), i)); // print out the south holes, right aligned etc.
    }
    cout << endl << "   " << m_players[1]->name() << endl << endl; // name of South

//    cout << "\t\t\t\t\tNorth: " << m_players[0]->name() << endl;
////    cout << "\t\t\t\t";
////    for (int i = 1; i <= m_board.holes(); i++) {
////      cout << "\t" << i;
////    }
////  cout << endl;
//
//    cout << "\t\t\t\t\t";
//    for (int i = 0; i < m_board.holes() - 1; i++) {
//      cout << "----";
//    }
//    cout << "-" << endl << "\t\t\t\t";
//    for (int i = 1; i <= m_board.holes(); i++) {
//      cout << "\t" << m_board.beans(static_cast<Side>(0), i);
//    }
//    cout << endl << "North's pot\t\t" << m_board.beans(static_cast<Side>(0), 0) << " ";
//    for (int i = 0; i < m_board.holes(); i++) {
//      cout << "\t";
//    }
//    cout << "   " << m_board.beans(static_cast<Side>(1), 0) << "\t\tSouth's pot" << endl << "\t\t\t\t";
//    for (int i = 1; i <= m_board.holes(); i++) {
//      cout << "\t" << m_board.beans(static_cast<Side>(1), i);
//    }
//      cout << endl << "\t\t\t\t\t";
//    for (int i = 0; i < m_board.holes() - 1; i++) {
//      cout << "----";
//    }
//    cout << "-" << endl;
////    cout << "\t\t\t\t";
////    for (int i = 1; i <= m_board.holes(); i++) {
////      cout << "\t" << i;
////    }
////  cout << endl;
//    cout << "\t\t\t\t\tSouth: " << m_players[1]->name() << endl << endl;

}

//If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    // need to fix this also for boards altered by tampering and created with 0 beans
    if ((over = m_over) == true || (!m_board.beansInPlay(NORTH) && !m_board.beansInPlay(SOUTH))) { // if m_over or a board created with 0 initial beans
        over = true; // only matters in case of board created with 0 initial beans
        hasWinner = m_board.beans(static_cast<Side>(0), 0) != m_board.beans(static_cast<Side>(1), 0); // if the pots are unequal
        if (hasWinner) { // assign the side with most beans in pot
            winner = m_board.beans(static_cast<Side>(0), 0) > m_board.beans(static_cast<Side>(1), 0) ? static_cast<Side>(0) : static_cast<Side>(1);
        }
    }
}

//Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
bool Game::move(Side s) {
    Side endSide;
    int endHole;
    int chosenPot(m_players[s]->chooseMove(m_board, s)); // ask the player corresponding to side s to make a move. will be a valid pot or else -1
//    cout << chosenPot << endl;
    if (m_board.sow(s, chosenPot, endSide, endHole)) { // i.e. if it was a valid move
        cout << m_players[s]->name() << " chose " << chosenPot << endl;
        if (endHole == 0) { // if, after sowing, the last bean is placed in s's pot
            cout << endl;
            display();
            cout << m_players[s]->name() << " gets an extra move.\n\n";
            return move(s); // recursively go again!
        } else if (endSide == s && m_board.beans(endSide, endHole) == 1 && m_board.beans(opponent(endSide), endHole) > 0) {
            // if s ends on s's own side in a hole with previously no beans (thus it now only has 1) and the opposite hole has beans
            m_board.moveToPot(endSide, endHole, s);
            m_board.moveToPot(opponent(endSide), endHole, s);
            return true;
        } else {return true;}
    } else {
        assert(m_board.beansInPlay(s) == 0); // if sow has failed, that must mean that chooseMove return -1 which must mean that there were no options
        cout << m_players[s]->name() << " has no more beans to sow." << endl;
        cout << "Sweeping " << m_players[opponent(s)]->name() << "'s beans into their pot" << endl; // do the sweep for opponent!
        for (int i = 1; i <= m_board.holes(); i++) {
            assert(m_board.moveToPot(opponent(s), i, opponent(s))); // this should return true since i is a valid hole number
        }
        m_over = true;
        return false;
    }
}

//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
void Game::play() {
    bool side(false), over(false), hasWinner(false); // false is North and true is South
    Side winner;
    // call status at beginning to check if game is over.
    do {
        side = !side;
        cout << endl;
        display();
        if (m_allRobots) {
            cout << "Press ENTER to continue";
            cin.ignore(10000, '\n');
            cout << endl;
        }
    } while (move(static_cast<Side>(side))); // false is North and true is South
    status(over, hasWinner, winner);
    assert(over); // it should be over if move returned false
    cout << endl;
    display();
    if (hasWinner) {
        switch(winner) {
            case NORTH:
                cout << m_players[NORTH]->name() << " has won!" << endl; // NORTH == 0
                break;
            case SOUTH:
                cout << m_players[SOUTH]->name() << " has won!" << endl; // SOUTH == 1
                break;
            default:
                cout << "ERROR: Something has gone awry!" << endl;
                break;
        }
    } else {
        cout << "It is a tie!" << endl;
    }
}

//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
int Game::beans(Side s, int hole) const {
    return m_board.beans(s, hole);
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// YOU MAY MOVE TO ANOTHER FILE ALL THE CODE FROM HERE TO THE END OF FILE, BUT
// BE SURE TO MOVE *ALL* THE CODE; DON'T MODIFY OR REMOVE ANY #IFDEF, ETC.
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31/g32 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

//#ifdef _WIN32
//
//#include <windows.h>
//
//void Game::clearScreen()
//{
//    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//    GetConsoleScreenBufferInfo(hConsole, &csbi);
//    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
//    COORD upperLeft = { 0, 0 };
//    DWORD dwCharsWritten;
//    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
//                                                        &dwCharsWritten);
//    SetConsoleCursorPosition(hConsole, upperLeft);
//}
//
//#else  // not _WIN32
//
//#include <iostream>
//#include <cstring>
//#include <cstdlib>
//using namespace std;
//
//void Game::clearScreen()  // will just write a newline in an Xcode output window
//{
//    static const char* term = getenv("TERM");
//    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
//        cout << endl;
//    else
//    {
//        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
//        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
//    }
//}
//
//#endif
//void clearScreen() {
//    cout << endl;
//}

