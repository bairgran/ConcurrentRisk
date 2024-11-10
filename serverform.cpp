#include "serverform.h"
#include "ui_serverform.h"
#include <QNetworkInterface>
#include <QNetworkReply>
#include <qstyle.h>


ServerForm::ServerForm(QWidget *parent)
    : QWidget(parent, Qt::Window), ui(new Ui::ServerForm) {
    ui->setupUi(this);
    RiskServer = new QTcpServer(this);
    determinePotentialInterfaceConnections();
}


ServerForm::~ServerForm()
{
    RiskServer->close();
    delete ui;
}

void ServerForm::determinePotentialInterfaceConnections() {
    QNetworkInterface *tempInterface = new QNetworkInterface();
    QList<QHostAddress> interfaceList = tempInterface->allAddresses();
    for (auto address : interfaceList) {
        if (address.protocol()==QAbstractSocket::IPv4Protocol) {
            interfaces.append(address);
        }

    }
    delete tempInterface;
}

void ServerForm::showServerWindow()
{

    show();
}

bool ServerForm::serverListen() {
    bool started = RiskServer->listen(interfaces[0], port);
    if (started) {
        qDebug() << "Server listening";
    }else {
        qDebug() << "Server can't start listening";
    }
    return started;
}

void ServerForm::serverStopListening() {
    RiskServer->close();
    if (RiskServer->isListening()) {
        qDebug() << "Server failed to close";
    }else {
        qDebug() << "Server Closed";
    }
}

void ServerForm::setPort(int newPort)
{
    port = newPort;
}

void ServerForm::on_btnStart_clicked()
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

void ServerForm::updateStatusLight() {
    if (RiskServer->isListening()) {
        ui->lblStatusLight->setProperty("ServerState", "1");
    }
    else {
        ui->lblStatusLight->setProperty("ServerState", "0");
    }
    style()->polish(ui->lblStatusLight);
}


void ServerForm::on_btnClose_clicked()
{
    serverStopListening();
    updateStatusLight();
    ui->btnClose->setEnabled(false);
    ui->btnStart->setEnabled(true);
    ui->spnPort->setEnabled(true);
    //ServerIP = QHostAddress::Null;
    ui->lstConsole->clear();
}

void ServerForm::updateConsole() {
    ui->lstConsole->addItem("Hosted at: " + RiskServer->serverAddress().toString());
    ui->lstConsole->addItem((QString) "Port: " + QString::number(RiskServer->serverPort()));
}
