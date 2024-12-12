#ifndef SERVERFORM_H
#define SERVERFORM_H

#include "gameserver.h"
#include "scoreboard.h"
#include <QNetworkInterface>
#include <QTcpServer>
#include <QWidget>

namespace Ui {
class ServerForm;
}

class ServerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServerForm(QWidget *parent = nullptr);
    ~ServerForm();
    Scoreboard *scoreboard;
public slots:

private:

    Ui::ServerForm *ui;
    QTcpServer *RiskServer;
    //QHostAddress ServerIP;
    int port;
    QMap<int, QTcpSocket *> PlayerMap;
    QList<QHostAddress> interfaces;
    QStringListModel *model;
    QStringList logList;

    GameServer *GAME;


    void updateStatusLight();
    void updateConsole();
    void determinePotentialInterfaceConnections();
    int connectionCounter=0;

    void setPort(int newPort);

private slots:
    bool serverListen();
    void serverStopListening();
    void onBtnStartClicked();
    void onBtnCloseClicked();
    void addNewConnection();
    void createGame();
    void updateLog(const QString &message);
    void updateScoreboard(std::vector<Territory> &territories);
signals:
    void serverDeletedSignal();
protected:
    //void closeEvent(QCloseEvent *event);
};

#endif // SERVERFORM_H
