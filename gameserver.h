#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include "territory.h"
#include <QStringListModel>

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);

signals:
    void transmitData(const int player, const QString &message);
    void transmitScoreboard(std::vector<Territory> &territories);
    void transmitLogUpdate(const QString &message);

public slots:
    void handleUserInput(const QString &input); // Slot to handle input from MainWindow
    void initializeGame();



private slots:
    //void handleTurnAction();

private:

    //QStringListModel *model;
    //QStringList logList;

    void handleTurnAction(const QString &input); // Pass input directly


    void updateLog(const QString &message);
    void processReinforcement(const QString &input);
    void processAttack(const QString &input);
    void processFortify(const QString &input);
    void transferOwnership(int territoryId, int newOwner);

    // Game state variables
    int playerTurn;   // 1 or 2
    int currentPhase; // 0 = Reinforcement, 1 = Attack, 2 = Fortify
    std::vector<Territory> territories; // All territories in the game


};

#endif // GAMESERVER_H
