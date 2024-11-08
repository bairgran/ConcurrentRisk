#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void setPort(int newPort);

signals:

public slots:
    bool serverListen();
    void serverStopListening();
private:
    QTcpServer *RISKServer;
    int port;
    QMap<int, QTcpSocket *> PlayerMap;
};

#endif // SERVER_H
