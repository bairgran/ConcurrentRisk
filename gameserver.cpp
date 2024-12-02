#include "gameserver.h"

GameServer::GameServer(QObject *parent)
    : QObject{parent}
{
    //initializeGame();
}

void GameServer::initializeGame()
{
    playerTurn = 1;

    // Initialize territories with unique IDs, owners, and troops
    territories.push_back(Territory(1, 1, 2)); // Territory 1: Player 1 with 10 troops
    territories.push_back(Territory(2, 1, 2)); // Territory 2: Player 1 with 10 troops
    territories.push_back(Territory(3, 1, 2));  // Territory 3: Player 2 with 2 troops
    territories.push_back(Territory(4, 2, 2));  // Territory 4: Player 2 with 2 troops
    territories.push_back(Territory(5, 2, 2));  // Territory 5: Player 2 with 2 troops
    territories.push_back(Territory(6, 2, 2));  // Territory 6: Player 2 with 2 troops

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

void GameServer::updateLog(const QString &message)
{
    logList.append(message);
    model->setStringList(logList);
}

void GameServer::handleTurnAction()
{
    // QString input = ui->lineEdit->text();
    // ui->lineEdit->clear();
    QString input = ""; // FIX ME.

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

void GameServer::processReinforcement(const QString &input)
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

void GameServer::processAttack(const QString &input)
{
    static int attackingFrom = -1;   // Store the attacking territory ID
    static bool isContinuing = true; // Track if the player wants to continue attacking

    if (attackingFrom == -1 && isContinuing) {
        // First ask for the territory you are attacking from
        attackingFrom = input.toInt();
        bool validTerritory = false;
        isContinuing = true;

        // Check if the attacking territory is valid and owned by the player
        for (const Territory &territory : territories) {
            if (territory.id == attackingFrom && territory.owner == playerTurn) {
                validTerritory = true;
                break;
            }
        }

        if (validTerritory) {
            // Prompt user for the territory they are attacking
            updateLog(
                QString("Select the territory to attack from Territory %1.").arg(attackingFrom));
            scoreboard->updateScoreboard(territories); // Update scoreboard
            return;                                    // Exit to wait for the next input
        } else {
            updateLog("Invalid attacking territory. Please select a valid territory owned by you.");
            attackingFrom = -1; // Reset for next input
            return;
        }
    } else if (isContinuing) {
        // Now process the attack to the selected territory
        int targetTerritoryId = input.toInt();
        for (Territory &defenderTerritory : territories) {
            if (defenderTerritory.id == targetTerritoryId) {
                // Check if the defender is not the same as the attacker
                if (defenderTerritory.owner != playerTurn) {
                    Territory &attackerTerritory = territories[attackingFrom];

                    // Ensure there are enough troops to attack
                    if (attackerTerritory.troops > 1) { // Assuming at least 1 troop must stay behind
                        // Determine the number of dice to roll
                        int attackerDice = std::min(attackerTerritory.troops - 1, 3); // Max 3 dice
                        int defenderDice = std::min(defenderTerritory.troops, 2);     // Max 2 dice

                        // Roll the dice
                        QVector<int> attackerRolls(attackerDice);
                        QVector<int> defenderRolls(defenderDice);
                        for (int i = 0; i < attackerDice; ++i) {
                            attackerRolls[i] = rand() % 6 + 1; // Roll a die (1-6)
                        }
                        for (int i = 0; i < defenderDice; ++i) {
                            defenderRolls[i] = rand() % 6 + 1; // Roll a die (1-6)
                        }

                        // Sort the rolls in descending order
                        std::sort(attackerRolls.begin(), attackerRolls.end(), std::greater<int>());
                        std::sort(defenderRolls.begin(), defenderRolls.end(), std::greater<int>());

                        // Compare the highest rolls first
                        for (int i = 0; i < std::min(attackerDice, defenderDice); ++i) {
                            if (attackerRolls[i] > defenderRolls[i]) {
                                defenderTerritory.troops--; // Defender loses a troop
                            } else {
                                attackerTerritory.troops--; // Attacker loses a troop
                            }
                        }

                        // Convert rolls to QStrings for logging
                        QList<QString> attackerRollsStr;
                        QList<QString> defenderRollsStr;

                        for (int roll : attackerRolls) {
                            attackerRollsStr.append(QString::number(roll));
                        }
                        for (int roll : defenderRolls) {
                            defenderRollsStr.append(QString::number(roll));
                        }

                        // Update log messages
                        updateLog(QString("Player %1 attacks Territory %2 from Territory %3!")
                                      .arg(playerTurn)
                                      .arg(targetTerritoryId)
                                      .arg(attackingFrom));
                        updateLog(QString("Attacker rolled: %1").arg(attackerRollsStr.join(", ")));
                        updateLog(QString("Defender rolled: %1").arg(defenderRollsStr.join(", ")));

                        // Check if defender's troops have reached 0
                        if (defenderTerritory.troops <= 0) {
                            transferOwnership(targetTerritoryId, playerTurn);
                        }

                        scoreboard->updateScoreboard(territories); // Update scoreboard

                        // Check if attacking player has troops left to continue attacking
                        if (attackerTerritory.troops <= 1) {
                            updateLog("You have no troops left to continue attacking.");
                            currentPhase = 2; // Move to Fortify Phase
                        } else {
                            updateLog("Do you want to continue attacking? (y/n)");
                            isContinuing = false; // Set to false to wait for yes/no input
                        }

                        return;
                    } else {
                        updateLog("You do not have enough troops to attack.");
                        attackingFrom = -1; // Reset for next input
                        return;
                    }
                } else {
                    updateLog("You cannot attack your own territory.");
                    attackingFrom = -1; // Reset for next input
                    return;
                }
            }
        }
        updateLog("Invalid territory ID. Enter a valid ID for the target territory.");
    } else {
        // Process 'y' or 'n' input for continuing attack
        if (input.trimmed().toLower() == "y") {
            // Reset for the next attack selection
            attackingFrom = -1;
            isContinuing = true; // Continue attacking
            updateLog(
                "Select the territory to attack from."); // Prompt to select new attacking territory
            return;
        } else if (input.trimmed().toLower() == "n") {
            // End the attack phase
            updateLog("You have chosen to end your attacks.");
            currentPhase = 2;   // Move to Fortify Phase
            attackingFrom = -1; // Reset for next phase
            updateLog(QString("Player %1, it's your turn to fortify. You can move troops between "
                              "your territories.")
                          .arg(playerTurn));
            updateLog("Enter the territory to reinforce to");
            updateLog(QString("Now it's Player %1's turn to fortify.")
                          .arg(playerTurn)); // Inform player they are in the fortification phase
            return;
        } else {
            updateLog(
                "Invalid input. Please enter 'y' to continue attacking or 'n' to end your turn.");
            return; // Wait for valid input
        }
    }
}

void GameServer::processFortify(const QString &input)
{
    static int fromTerritory = -1;       // Territory to move troops from
    static int toTerritory = -1;         // Territory to move troops to
    static int phaseStep = 0;            // Step in the fortification process
    static bool continueFortifying = true; // Whether the player wants to keep fortifying

    // If the player has chosen not to continue fortifying
    if (!continueFortifying) {
        if (input.trimmed().toLower() == "y") {
            continueFortifying = true;
            phaseStep = 0; // Reset for the next action
            updateLog("Enter the territory ID to move troops from:");
            return;
        } else if (input.trimmed().toLower() == "n") {
            updateLog(QString("Player %1 has ended the fortification phase.").arg(playerTurn));
            playerTurn = 3 - playerTurn; // Switch turn
            currentPhase = 0;            // Move back to Reinforcement phase
            updateLog(QString("Now it's Player %1's turn. Reinforce a territory.").arg(playerTurn));
            return;
        } else {
            updateLog("Invalid input. Enter 'y' to continue or 'n' to end fortification.");
            return;
        }
    }

    // Sequential prompts for the fortification phase
    switch (phaseStep) {
    case 0: // Prompt for the territory to move troops from
        fromTerritory = input.toInt();
        for (const Territory &territory : territories) {
            if (territory.id == fromTerritory && territory.owner == playerTurn && territory.troops > 1) {
                updateLog(QString("You selected Territory %1. Enter the territory ID to move troops to:").arg(fromTerritory));
                phaseStep = 1; // Move to the next step
                return;
            }
        }
        updateLog("Invalid territory. Please select a valid territory owned by you with more than 1 troop.");
        break;

    case 1: // Prompt for the territory to move troops to
        toTerritory = input.toInt();
        for (const Territory &territory : territories) {
            if (territory.id == toTerritory && territory.owner == playerTurn) {
                updateLog(QString("You selected Territory %1. Enter the number of troops to move:").arg(toTerritory));
                phaseStep = 2; // Move to the next step
                return;
            }
        }
        updateLog("Invalid territory. Please select a valid territory owned by you.");
        break;

    case 2: // Prompt for the number of troops to move
        int troopsToMove = input.toInt();
        for (Territory &source : territories) {
            if (source.id == fromTerritory && source.owner == playerTurn && source.troops > troopsToMove) {
                for (Territory &destination : territories) {
                    if (destination.id == toTerritory && destination.owner == playerTurn) {
                        source.troops -= troopsToMove;
                        destination.troops += troopsToMove;
                        updateLog(QString("Player %1 moved %2 troops from Territory %3 to Territory %4.")
                                      .arg(playerTurn)
                                      .arg(troopsToMove)
                                      .arg(fromTerritory)
                                      .arg(toTerritory));
                        scoreboard->updateScoreboard(territories); // Update the scoreboard
                        phaseStep = 0; // Reset for the next fortification action
                        continueFortifying = false; // Prompt the player to continue or end
                        updateLog("Do you want to keep fortifying? (y/n)");
                        return;
                    }
                }
            }
        }
        updateLog("Invalid troop movement. Ensure you have enough troops and both territories are valid.");
        break;
    }

    // Refresh the scoreboard after invalid input
    scoreboard->updateScoreboard(territories);
    return;
}


void GameServer::transferOwnership(int territoryId, int newOwner)
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
}
