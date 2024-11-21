// Jane

#include "mainwindow.h"
#include <cassert>
#include <iostream>

// initialize
void testInitializeGame() {
    MainWindow testWindow(nullptr); // mainwindow object
    // in mainwindow.h add friend class
    testWindow.initializeGame();

   std::cout << "test Initialize Game test passed" << std::endl;
}


// attack
void testAttack() {
    MainWindow testWindow(nullptr);
    testWindow.initializeGame();

    // simulating attack
    QString testing_input = "From:3 To:5 Dice:2";
    testWindow.processAttack(testing_input);

    // check verify attack
    assert(testWindow.currentPhase == 1);
    assert(testWindow.territories[5].owner != testWindow.territories[3].owner);

    std::cout << "test Attack test passed" << std::endl;
}

// reinforcement
void testReinforcement(){
    MainWindow testWindow(nullptr);
    testWindow.initializeGame();

}

// fortification
void testFortification() {
    MainWindow testWindow(nullptr);
    testWindow.initializeGame();
}

// test // doesn't work
// will try testing later using Qt Test
int main() {
    testInitializeGame();
    testAttack();
    return 0;
}


