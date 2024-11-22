#ifndef SERVERFORM_H
#define SERVERFORM_H

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
public slots:

private:
    Ui::ServerForm *ui;
    QTcpServer *RiskServer;
    //QHostAddress ServerIP;
    int port;
    QMap<int, QTcpSocket *> PlayerMap;
    QList<QHostAddress> interfaces;

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
signals:
    void serverDeletedSignal();
protected:
    //void closeEvent(QCloseEvent *event);
};

#endif // SERVERFORM_H
