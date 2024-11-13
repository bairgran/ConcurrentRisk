#ifndef SERVERFORM_H
#define SERVERFORM_H

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
    void setPort(int newPort);
    ~ServerForm();

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
public slots:
    //void showServerWindow();
    bool serverListen();
    void serverStopListening();

private slots:
    void on_btnStart_clicked();
    void on_btnClose_clicked();
};

#endif // SERVERFORM_H
