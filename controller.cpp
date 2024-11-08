#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    // Create signal connections between objects and methods.
    // Connects  'connected' in controllerSocket (QTcpSocket) to connected in this (Controller).
    connect(&controllerSocket, &QTcpSocket::connected, this, &Controller::connected);
    // Connect other relevant signals
    connect(&controllerSocket, &QTcpSocket::disconnected, this, &Controller::disconnected);
    connect(&controllerSocket, &QTcpSocket::stateChanged, this, &Controller::sockStateChanged); // detour
    connect(&controllerSocket, &QTcpSocket::errorOccurred, this, &Controller::errorOccurred);

}

void Controller::connectToServer(QString ip, int port)
{
    if (controllerSocket.isOpen()) {
        // Are we already connected?
        if (ip == controllerIP && port == controllerPort) {
            return;
        }
        // If not, close current socket
        controllerSocket.close();
    }
    // Assign new values.
    controllerIP = ip;
    controllerPort = port;
    // Connect to Host.
    controllerSocket.connectToHost(controllerIP, controllerPort);
}


void Controller::sockStateChanged(QAbstractSocket::SocketState sockState)
{
    //Detour in state changed chain that closes socket every time we are in an unconnected state.
    if (sockState == QAbstractSocket::UnconnectedState) {
        controllerSocket.close();
    }
    emit stateChanged(sockState);
}
