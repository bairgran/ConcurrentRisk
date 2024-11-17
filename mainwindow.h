#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "scoreboard.h" // Include the scoreboard header
#include "territory.h"
#include <vector>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateSize();

private slots:
    void handleTurnAction();

    void socketStateChange(QAbstractSocket::SocketState);
    void socketError(QAbstractSocket::SocketError);
    void on_btnOpenServerWindow_clicked();
    void on_comboBox_currentIndexChanged(int index);

    void connectToServer();
    void serverDeleted();
private:
    Scoreboard *scoreboard;
    Ui::MainWindow *ui;
    QStringListModel *model;
    QStringList logList;

    // Game state variables
    int playerTurn;   // 1 or 2
    int currentPhase; // 0 = Reinforcement, 1 = Attack, 2 = Fortify

    std::vector<Territory> territories; // All territories in the game

    void initializeGame();
    void updateLog(const QString &message);
    void processReinforcement(const QString &input);
    void processAttack(const QString &input);
    void processFortify(const QString &input);
    void transferOwnership(int territoryId, int newOwner);

    QTcpSocket gameSocket;
    QString IP;
    int Port;

    bool serverStarted=false;
};



#endif // MAINWINDOW_H
