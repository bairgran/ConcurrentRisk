#include "server.h"

Server::Server(QObject *parent)
    : QObject{parent}
{
    RISKServer = new QTcpServer(this);


    // if(RISKServer->listen(QHostAddress::Any, 55555)) {
    //     qDebug() << "Server listening";
    // }


}

bool Server::serverListen() {
    bool started = RISKServer->listen(QHostAddress::Any, port);
    if (started) {
        qDebug() << "Server listening";
    }else {
        qDebug() << "Server can't start listening";
    }
    return started;
}

void Server::serverStopListening() {
    RISKServer->close();
}

void Server::setPort(int newPort)
{
    port = newPort;
}

// void Server::initPlayerList() {

// }
