#include "mainwindow.h"
#include <QString>
#include <QStringListModel>
#include <QVector>
#include "territory.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug> // Include QDebug for debugging output
#include <QMessageBox> // for victory

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    ui->listView->setModel(model);

    scoreboard = new Scoreboard(ui->listView_2);  // Initialize the scoreboard with listView_2
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::handleTurnAction);

    initializeGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkForVictory()
{
    int owner = territories[0].owner; // Take the owner of the first territory as a reference
    for (const Territory &territory : territories) {
        if (territory.owner != owner) {
            return; // If any territory is owned by a different player, no victory
        }
    }

    // If all territories are owned by the same player
    updateLog(QString("Player %1 has won the game!").arg(owner));
    QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(owner));

    // Close the application after the player acknowledges the message box
    QApplication::quit();
}





void MainWindow::initializeGame()
{
    playerTurn = 1;

    // Initialize territories with unique IDs, owners, and troops
    territories.push_back(Territory(1, 1, 8)); // Territory 1: Player 1 with 10 troops
    territories.push_back(Territory(2, 1, 15)); // Territory 2: Player 1 with 10 troops
    territories.push_back(Territory(3, 1, 8));  // Territory 3: Player 2 with 2 troops
    territories.push_back(Territory(4, 1, 8));  // Territory 4: Player 2 with 2 troops
    territories.push_back(Territory(5, 1, 8));  // Territory 5: Player 2 with 2 troops
    territories.push_back(Territory(6, 2, 1));  // Territory 6: Player 2 with 2 troops

    // Set adjacency for each territory
    territories[0].adjacent = {2, 3};         // Territory 1 is connected to 2, 3
    territories[1].adjacent = {1, 3, 6};      // Territory 2 is connected to 1, 3, 6
    territories[2].adjacent = {1, 2, 4, 5};   // Territory 3 is connected to 1, 2, 4, 5
    territories[3].adjacent = {3, 5, 6};      // Territory 4 is connected to 3, 5, 6
    territories[4].adjacent = {3, 4};      // Territory 5 is connected to 3, 4
    territories[5].adjacent = {2, 4};      // Territory 6 is connected to 2, 4

    currentPhase = 0;

    updateLog("Game initialized. It's Player 1's turn. Reinforce an owned territory (enter the number).");
    scoreboard->updateScoreboard(territories); // Update scoreboard after game initialization
}

void MainWindow::updateLog(const QString &message)
{
    logList.append(message);
    model->setStringList(logList);
}

void MainWindow::handleTurnAction()
{
    QString input = ui->lineEdit->text();
    ui->lineEdit->clear();

    switch (currentPhase) {
    case 0:
        processReinforcement(input);
        break;
    case 1:
        processAttack(input);
        break;
    case 2:
        processFortify(input);
        break;
    }
}

void MainWindow::processReinforcement(const QString &input)
{
    // Reset for new reinforcement phase
    static int currentReinforcement = 0; // Track remaining reinforcements
    static int selectedTerritoryId = -1; // Track selected territory

    // Calculate the number of reinforcements
    if (currentReinforcement == 0) { // Only calculate reinforcements if it's a fresh phase
        int territoryCount = 0;
        for (const Territory &territory : territories) {
            if (territory.owner == playerTurn) {
                territoryCount++;
            }
        }

        currentReinforcement = territoryCount / 3;                // 1 troop for every 3 territories
        currentReinforcement = std::max(currentReinforcement, 3); // Minimum of 3 reinforcements
    }

    if (selectedTerritoryId == -1) {
        // First ask for the territory to reinforce
        selectedTerritoryId = input.toInt();
        bool validTerritory = false;

        // Check if the selected territory is valid and owned by the player
        for (const Territory &territory : territories) {
            if (territory.id == selectedTerritoryId && territory.owner == playerTurn) {
                validTerritory = true;
                break;
            }
        }

        if (validTerritory) {
            updateLog(
                QString("You have %1 troops to place. Select how many to reinforce Territory %2.")
                    .arg(currentReinforcement)
                    .arg(selectedTerritoryId));
            scoreboard->updateScoreboard(territories); // Update scoreboard
            return;                                    // Wait for next input for troop allocation
        } else {
            updateLog("Invalid territory ID. Please select a valid territory owned by you.");
            scoreboard->updateScoreboard(territories); // Update scoreboard
            selectedTerritoryId = -1;                  // Reset for next input
            return;
        }
    } else {
        // Now process the number of troops to place
        int troopsToPlace = input.toInt();

        if (troopsToPlace > currentReinforcement) {
            updateLog("You cannot place more troops than you have left.");
            scoreboard->updateScoreboard(territories); // Update scoreboard
        } else if (troopsToPlace < 1) {
            updateLog("You must place at least 1 troop.");
            scoreboard->updateScoreboard(territories); // Update scoreboard
        } else {
            // Reinforce the selected territory
            for (Territory &territory : territories) {
                if (territory.id == selectedTerritoryId) {
                    territory.troops += troopsToPlace;     // Add troops to the selected territory
                    currentReinforcement -= troopsToPlace; // Subtract from total reinforcements
                    updateLog(QString("Player %1 reinforced Territory %2 with %3 troops.")
                                  .arg(playerTurn)
                                  .arg(selectedTerritoryId)
                                  .arg(troopsToPlace));
                    scoreboard->updateScoreboard(territories); // Update scoreboard

                    // Check if there are still troops to place
                    if (currentReinforcement > 0) {
                        selectedTerritoryId = -1; // Reset for next territory selection
                        updateLog(
                            QString("You have %1 troops left. Select a territory to reinforce.")
                                .arg(currentReinforcement));
                        scoreboard->updateScoreboard(territories); // Update scoreboard
                        return;                                    // Wait for next input
                    }
                }
            }
        }
    }

    // If no troops left to place, end the reinforcement phase
    updateLog("You have placed all your reinforcements.");
    scoreboard->updateScoreboard(territories); // Final scoreboard update
    currentPhase = 1;                          // Move to Attack phase
    selectedTerritoryId = -1;                  // Reset for the next phase
    updateLog(QString("It's Player %1's turn to attack. Select the territory to attack from.")
                  .arg(playerTurn)); // Prompt for attack
}

void MainWindow::processAttack(const QString &input)
{
    static int attackingFrom = -1;   // Store the attacking territory ID
    static bool isContinuing = true; // Track if the player wants to continue attacking

    if (attackingFrom == -1 && isContinuing) {
        // First ask for the territory you are attacking from
        attackingFrom = input.toInt();
        bool validTerritory = false;

        // Check if the attacking territory is valid and owned by the player
        for (const Territory &territory : territories) {
            if (territory.id == attackingFrom && territory.owner == playerTurn) {
                validTerritory = true;
                break;
            }
        }

        if (validTerritory) {
            updateLog(QString("Select the territory to attack from Territory %1.").arg(attackingFrom));
            scoreboard->updateScoreboard(territories); // Update scoreboard
            return; // Exit to wait for the next input
        } else {
            updateLog("Invalid attacking territory. Please select a valid territory owned by you.");
            attackingFrom = -1; // Reset for next input
            return;
        }
    } else if (isContinuing) {
        // Process the target territory
        int targetTerritoryId = input.toInt();
        bool isAdjacent = false;

        // Check if the target territory is adjacent
        for (const int &adjId : territories[attackingFrom - 1].adjacent) {
            if (adjId == targetTerritoryId) {
                isAdjacent = true;
                break;
            }
        }

        if (!isAdjacent) {
            updateLog("The target territory is not adjacent to the attacking territory.");
            return;
        }

        // Proceed with the attack logic
        for (Territory &defenderTerritory : territories) {
            if (defenderTerritory.id == targetTerritoryId) {
                if (defenderTerritory.owner != playerTurn) {
                    Territory &attackerTerritory = territories[attackingFrom - 1];

                    if (attackerTerritory.troops > 1) { // Check troop availability
                        // Perform dice rolls and resolve combat
                        int attackerDice = std::min(attackerTerritory.troops - 1, 3);
                        int defenderDice = std::min(defenderTerritory.troops, 2);

                        QVector<int> attackerRolls(attackerDice);
                        QVector<int> defenderRolls(defenderDice);

                        for (int i = 0; i < attackerDice; ++i) {
                            attackerRolls[i] = rand() % 6 + 1; // Roll a die (1-6)
                        }
                        for (int i = 0; i < defenderDice; ++i) {
                            defenderRolls[i] = rand() % 6 + 1; // Roll a die (1-6)
                        }

                        std::sort(attackerRolls.begin(), attackerRolls.end(), std::greater<int>());
                        std::sort(defenderRolls.begin(), defenderRolls.end(), std::greater<int>());

                        for (int i = 0; i < std::min(attackerDice, defenderDice); ++i) {
                            if (attackerRolls[i] > defenderRolls[i]) {
                                defenderTerritory.troops--;
                            } else {
                                attackerTerritory.troops--;
                            }
                        }

                        if (defenderTerritory.troops <= 0) {
                            transferOwnership(targetTerritoryId, playerTurn);
                        }

                        updateLog(QString("Attack resolved between Territory %1 and Territory %2.")
                                      .arg(attackingFrom)
                                      .arg(targetTerritoryId));
                        scoreboard->updateScoreboard(territories);
                        return;
                    } else {
                        updateLog("Not enough troops to attack.");
                        attackingFrom = -1; // Reset for the next attack
                        return;
                    }
                } else {
                    updateLog("You cannot attack your own territory.");
                    attackingFrom = -1;
                    return;
                }
            }
        }

        updateLog("Invalid target territory. Please try again.");
    } else {
        updateLog("Invalid input. Please enter a valid territory ID.");
    }
}

void MainWindow::processFortify(const QString &input)
{
    static int fromTerritory = -1;       // Territory to move troops from
    static int toTerritory = -1;         // Territory to move troops to
    static int phaseStep = 0;            // Step in the fortification process
    static bool continueFortifying = true; // Whether the player wants to keep fortifying

    if (!continueFortifying) {
        if (input.trimmed().toLower() == "y") {
            continueFortifying = true;
            phaseStep = 0;
            updateLog("Enter the territory ID to move troops from:");
            return;
        } else if (input.trimmed().toLower() == "n") {
            updateLog(QString("Player %1 has ended the fortification phase.").arg(playerTurn));
            playerTurn = 3 - playerTurn; // Switch turn
            currentPhase = 0;
            updateLog(QString("Now it's Player %1's turn. Reinforce a territory.").arg(playerTurn));
            return;
        } else {
            updateLog("Invalid input. Enter 'y' to continue or 'n' to end fortification.");
            return;
        }
    }

    switch (phaseStep) {
    case 0: { // From territory
        fromTerritory = input.toInt();
        bool validTerritory = false;

        for (const Territory &territory : territories) {
            if (territory.id == fromTerritory && territory.owner == playerTurn && territory.troops > 1) {
                validTerritory = true;
                break;
            }
        }

        if (validTerritory) {
            updateLog(QString("You selected Territory %1. Enter the territory ID to move troops to:")
                          .arg(fromTerritory));
            phaseStep = 1;
        } else {
            updateLog("Invalid source territory. Please select a valid territory owned by you with more than 1 troop.");
        }
        break;
    }
    case 1: { // To territory
        toTerritory = input.toInt();
        bool isAdjacent = false;

        // Check if the territories are adjacent
        for (const int &adjId : territories[fromTerritory - 1].adjacent) {
            if (adjId == toTerritory) {
                isAdjacent = true;
                break;
            }
        }

        if (!isAdjacent) {
            updateLog("The target territory is not adjacent to the source territory.");
            return;
        }

        bool validTerritory = false;
        for (const Territory &territory : territories) {
            if (territory.id == toTerritory && territory.owner == playerTurn) {
                validTerritory = true;
                break;
            }
        }

        if (validTerritory) {
            updateLog(QString("You selected Territory %1. Enter the number of troops to move:").arg(toTerritory));
            phaseStep = 2;
        } else {
            updateLog("Invalid target territory. Please select a valid adjacent territory owned by you.");
        }
        break;
    }
    case 2: { // Number of troops
        int troopsToMove = input.toInt();
        bool validMove = false;

        for (Territory &source : territories) {
            if (source.id == fromTerritory && source.troops > troopsToMove) {
                for (Territory &destination : territories) {
                    if (destination.id == toTerritory) {
                        source.troops -= troopsToMove;
                        destination.troops += troopsToMove;
                        updateLog(QString("Moved %1 troops from Territory %2 to Territory %3.")
                                      .arg(troopsToMove)
                                      .arg(fromTerritory)
                                      .arg(toTerritory));
                        validMove = true;
                        break;
                    }
                }
            }
            if (validMove) break;
        }

        if (!validMove) {
            updateLog("Invalid troop movement. Ensure you have enough troops and the territories are valid.");
        } else {
            scoreboard->updateScoreboard(territories);
            phaseStep = 0;
            continueFortifying = false;
            updateLog("Do you want to keep fortifying? (y/n)");
        }
        break;
    }
    }
}



void MainWindow::transferOwnership(int territoryId, int newOwner)
{
    for (Territory &territory : territories) {
        if (territory.id == territoryId) {
            territory.owner = newOwner;
            updateLog(
                QString("Territory %1 is now owned by Player %2.").arg(territoryId).arg(newOwner));
            scoreboard->updateScoreboard(territories); // Update scoreboard after game initialization
            break;
        }
    }
    scoreboard->updateScoreboard(territories); // Update scoreboard after game initialization
     checkForVictory();
}
