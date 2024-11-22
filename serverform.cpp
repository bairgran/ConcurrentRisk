#include "serverform.h"
#include "ui_serverform.h"
#include <QNetworkInterface>
#include <QNetworkReply>
#include <qstyle.h>

// ServerForm creates a QTcpServer and ui to manage said server. Server runs a game of Concurrent Risk.
// Child of MainWindow so will be deleted when MainWindow is closed.

ServerForm::ServerForm(QWidget *parent)
    : QWidget(parent, Qt::Window), ui(new Ui::ServerForm) {
    ui->setupUi(this);
    RiskServer = new QTcpServer(this);
    RiskServer->setMaxPendingConnections(6);
    determinePotentialInterfaceConnections();
    connect(RiskServer, &QTcpServer::pendingConnectionAvailable, this, &ServerForm::addNewConnection);
    connect(ui->btnStart, &QPushButton::clicked, this, &ServerForm::onBtnStartClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &ServerForm::onBtnCloseClicked);

    show();
}


ServerForm::~ServerForm()
{
    // Signals to main window that this window has been destroyed so another window can be created if necessary.
    emit serverDeletedSignal();
    delete ui;
}

// Grant
// Used to populate interfaces with all the possible IP addresses that the server will be hosting from,
// and weeds out any that arent IPv4 protocol.
void ServerForm::determinePotentialInterfaceConnections() {
    interfaces = QNetworkInterface::allAddresses();
    interfaces.removeIf([](QHostAddress x){return x.protocol()!=QAbstractSocket::IPv4Protocol;});
}

// Grant
// Starts listening on the first interface (IP) available. Returns true if listen was successful.
bool ServerForm::serverListen() {
    bool started = RiskServer->listen(interfaces[0], port);
    if (started) {
        qDebug() << "Server listening";
    }else {
        qDebug() << "Server can't start listening";
    }
    return started;
}

// Grant
// Stops server from listening. Does not sever the connection of the Socket connections that have already been created.
void ServerForm::serverStopListening() {
    RiskServer->close();
    if (RiskServer->isListening()) {
        qDebug() << "Server failed to close";
    }else {
        qDebug() << "Server stopped listening";
    }
}

// Grant
// Stores a new value into the port value that the server will eventually use to listen. Possibly useless.
void ServerForm::setPort(int newPort)
{
    port = newPort;
}

// Grant
// updates Port value and calls for serverListen(). Then calls for UI updates.
void ServerForm::onBtnStartClicked()
{
    setPort(ui->spnPort->value());
    serverListen();
    updateStatusLight();
    ui->btnStart->setEnabled(false);
    ui->btnClose->setEnabled(true);
    ui->spnPort->setEnabled(false);
    //ServerIP = RiskServer->serverAddress();
    updateConsole();
}

// Grant
// Call to update color of box on UI depending on if the server is listening or not.
void ServerForm::updateStatusLight() {
    if (RiskServer->isListening()) {
        ui->lblStatusLight->setProperty("ServerState", "1");
    }
    else {
        ui->lblStatusLight->setProperty("ServerState", "0");
    }
    style()->polish(ui->lblStatusLight);
}

// Grant
// calls for server to stop listening and for UI updates.
void ServerForm::onBtnCloseClicked()
{
    serverStopListening();
    updateStatusLight();
    ui->btnClose->setEnabled(false);
    ui->btnStart->setEnabled(true);
    ui->spnPort->setEnabled(true);
}

// Grant
// Called when a new connection is available on the server. Creates a socket for the connection and stores it in the PlayerMap.
void ServerForm::addNewConnection()
{
    PlayerMap.insert(connectionCounter, RiskServer->nextPendingConnection());
    qDebug() << "New Server Connection";
    connectionCounter++;
    qDebug() << PlayerMap;
}

// void ServerForm::closeEvent(QCloseEvent *event)
// {
//     qDebug() << event;

// }

// Grant
// updates ui with hosting information to give to players.
void ServerForm::updateConsole() {
    ui->lstConsole->addItem((QString) "Hosted at: " + RiskServer->serverAddress().toString());
    ui->lstConsole->addItem((QString) "Port: " + QString::number(RiskServer->serverPort()));
}
