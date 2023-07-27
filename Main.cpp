#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>

using namespace std;

void doGameTests()
{
    Board z(6, 4);
    SmartPlayer x("Smallberg");
    BadPlayer v("Eggert");
    Game y(z, &x, &v);
    bool over1(true), hasWinner1(false);
    Side winner1(NORTH);
    y.status(over1, hasWinner1, winner1);
    assert(!over1 && !hasWinner1 && winner1 == NORTH);
    assert(y.beans(SOUTH, 7) == -1);
    
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
      //    Homer
      //   0  1  2
      // 0         0
      //   2  0  0
      //    Bart
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
    g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   0  1  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(NORTH);
      //   1  0  0
      // 0         3
      //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

    g.move(SOUTH);
      //   1  0  0
      // 0         3
      //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
    g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

    g.move(NORTH);
      //   0  0  0
      // 1         4
      //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
    g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
    g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
}

void doBoardTests()
{
    Board z(-1, -20);
    assert(z.holes() == 1);
    assert(z.beans(SOUTH, 1) == 0);
    assert(z.beansInPlay(NORTH) == 0);
    assert(z.totalBeans() == 0);
    Side test(NORTH);
    int meep(99);
    assert(!z.sow(SOUTH, 1, test, meep) && test == NORTH && meep == 99);
    
    Board b(3, 2);
    assert(b.holes() == 3);
    assert (b.totalBeans() == 12);
    assert(b.beans(SOUTH, POT) == 0);
    assert(b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11  &&  b.beans(SOUTH, 1) == 1  &&
                b.beans(SOUTH, 2) == 0  &&  b.beans(SOUTH, POT) == 2  &&
        b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    assert(b.sow(SOUTH, 3, es, eh));
    assert(es == NORTH);
    assert(eh == 3);
    assert(b.beans(SOUTH, 3) == 0);
    assert(b.beans(NORTH, 3) == 3);
    assert(b.beans(SOUTH, POT) == 3);
    assert(b.beansInPlay(SOUTH) == 1);
    assert(b.beansInPlay(NORTH) == 7);
}

void doPlayerTests()
{
//    SmartPlayer sp1("Professor");
//    Board z(8, 4);
//    bool side(false);
//    int i = 0;
//    do {
//        i++;
//        side = !side;
//        assert(sp1.chooseMove(z, static_cast<Side>(side)));
//    } while (i < 5);
    
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
}

int main()
{
    doPlayerTests();
    cout << "Passed all tests" << endl;
    doBoardTests();
    cout << "Passed all tests" << endl;
    doGameTests();
    cout << "Passed all tests" << endl;
    
    BadPlayer bp1("BadPlayer");
    BadPlayer bp2("Alexa");
    HumanPlayer hp1("Grog");
    HumanPlayer hp2("Eggert");
    SmartPlayer sp1("SmartPlayer");
    SmartPlayer sp2("ChatGPT");
    Board b(3, 2);
    Game g(b, &hp1, &bp2);
    g.play();
}

