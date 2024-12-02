#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include "territory.h"
#include <QStringListModel>
#include "scoreboard.h" // Include the scoreboard header

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);

private slots:
    void handleTurnAction();

private:
    Scoreboard *scoreboard;
    QStringListModel *model;
    QStringList logList;

    void initializeGame();
    void updateLog(const QString &message);
    void processReinforcement(const QString &input);
    void processAttack(const QString &input);
    void processFortify(const QString &input);
    void transferOwnership(int territoryId, int newOwner);

    // Game state variables
    int playerTurn;   // 1 or 2
    int currentPhase; // 0 = Reinforcement, 1 = Attack, 2 = Fortify
    std::vector<Territory> territories; // All territories in the game

signals:
};

#endif // GAMESERVER_H
