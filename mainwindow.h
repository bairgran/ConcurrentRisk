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

    QTcpSocket gameSocket;
    QString IP;
    int Port;

    bool serverStarted=false;

    // access private members for test
    friend void testInitializeGame();
    friend void testAttack();
    friend void testReinforcement();
    friend void testFortification();
};



#endif // MAINWINDOW_H
