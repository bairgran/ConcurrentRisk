#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTcpSocket>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    void connectToServer(QString ip, int port);
signals:
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccurred(QAbstractSocket::SocketError);

private slots:
    void sockStateChanged(QAbstractSocket::SocketState sockState);

private:
    QTcpSocket controllerSocket;
    QString controllerIP;
    int controllerPort;
};

#endif // CONTROLLER_H
