// comment after testing to avoid breaking other codes
// Jane

//#include "mainwindow.h"
//#include <cassert>
//#include <iostream>

// // initialize
// void testInitializeGame() {
//     MainWindow testWindow(nullptr); // mainwindow object
//     // in mainwindow.h add friend class
//     testWindow.initializeGame();

//    std::cout << "test Initialize Game test passed" << std::endl;
// }


// // attack
// void testAttack() {
//     MainWindow testWindow(nullptr);
//     testWindow.initializeGame();

//     // simulating attack
//     QString testing_input = "From:3 To:5 Dice:2";
//     testWindow.processAttack(testing_input);

//     // check verify attack
//     assert(testWindow.currentPhase == 1);
//     assert(testWindow.territories[5].owner != testWindow.territories[3].owner);

//     std::cout << "test Attack test passed" << std::endl;
// }


// // reinforcement
//void testReinforcement(){
//    MainWindow testWindow(nullptr);
//    testWindow.initializeGame();

//    QString input = "TerritoryID:5 Troops:3";
//    testWindow.processReinforcement(input);

//    assert(testWindow.currentPhase == 0);
//    assert(testWindow.territories[5].troopCount == 3);
//    std::cout << "reinforcement test passed" << std::endl;


// }

// // fortification
//void testFortification() {
//    MainWindow testWindow(nullptr);
//    testWindow.initializeGame();

//    QString input = "From:2 To:6 Troops:2";
//    testWindow.processFortify(input);

//    assert(testWindow.currentPhase == 2);
//    assert(testWindow.territories[2].troopCount == (initialCount2 - 2));
//    assert(testWindow.territories[6].troopCount == (initialCount6 + 2));

//    std::cout << "forticiation test passed" << std::endl;
//}

// // test
// int main() {
//     testInitializeGame();
//     testAttack();
//     testReinforcement
//     testFortification
//     return 0;
// }


