#include "mainwindow.h"
#include <QString>
#include <QStringListModel>
#include <QVector>
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug> // Include QDebug for debugging output
#include "serverform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->SetupPage);

    model = new QStringListModel(this);
    ui->listView->setModel(model);


    scoreboard = new Scoreboard(ui->listView_2); // Initialize the scoreboard with listView_2
    //connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::handleTurnAction);

    // Connect the button click to the slot that emits the signal
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_btnSendInput_clicked);

    connect(ui->btnJoin, &QPushButton::clicked, this, &MainWindow::connectToServer);
    // connections for debug purposes.
    connect(&gameSocket, &QAbstractSocket::stateChanged, this, &MainWindow::socketStateChange);
    connect(&gameSocket, &QAbstractSocket::errorOccurred, this, &MainWindow::socketError);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    updateSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//KD
void MainWindow::on_btnSendInput_clicked()
{
    // Emit the signal with the QLineEdit text
    emit userInputReceived(ui->lineEdit->text());
    ui->lineEdit->clear(); // Clear the input field after sending the text
}


// CLIENT
// Grant
// Signaled by QAbstractSocket::stateChanged. Debug info.
void MainWindow::socketStateChange(QAbstractSocket::SocketState newSocketState) {
    qDebug() << newSocketState;
}

// Grant
// Signaled by QAbstractSocket::errorOccurred. Debug info.
void MainWindow::socketError(QAbstractSocket::SocketError newSocketError) {
    qDebug() << newSocketError;
}

// Grant
// Creates Instance of server window. Server object is deleted upon closing the server window. Sends signal if deleted.
void MainWindow::on_btnOpenServerWindow_clicked()
{
    qDebug() << "Open Server Window Clicked";
    // Check if server has already been started.
    if (!serverStarted){
        //Server Window initialization
        ServerForm* serverWindow = new ServerForm(this);
        serverWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(serverWindow, &ServerForm::serverDeletedSignal, this, &MainWindow::serverDeleted);
        serverStarted=true;
    }
}

// Grant
// Signaled from destructor in server to indicate another server can be created if needed.
void MainWindow::serverDeleted() {
    serverStarted=false;
}

// TODO: rewrite this more eloquently.
// Grant
// resizes the window around whatever contents are currently being displayed in the stacked widget.
void MainWindow::updateSize() {

    // Frames use adjustSize(), widgets use resize().
    if (ui->comboBox->currentText() == "SetupPage") {
        ui->SetupPage->adjustSize();
        ui->stackedWidget->resize(ui->SetupPage->frameSize());
        ui->comboBox->move(ui->comboBox->x(), ui->stackedWidget->x()+ui->stackedWidget->height()+10);
        ui->centralwidget->adjustSize();
        this->resize(ui->centralwidget->frameSize().width(), ui->centralwidget->frameSize().height() + ui->statusbar->frameSize().height());
    }else {
        ui->GamePage->adjustSize();
        ui->stackedWidget->resize(ui->GamePage->frameSize());
        ui->comboBox->move(ui->comboBox->x(), ui->stackedWidget->x()+ui->stackedWidget->height()+10);
        ui->centralwidget->adjustSize();
        this->resize(ui->centralwidget->frameSize().width(), ui->centralwidget->frameSize().height() + ui->statusbar->frameSize().height());
    }
}

// Grant
// Updates size when stacked widget is changed via comboBox.
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    updateSize();
}

// Grant
// Pulls Ip and Port from ui and the clients socket attempts to establish a connection.
void MainWindow::connectToServer()
{
    // Error checking
    if (gameSocket.isOpen()) {
        // return without doing anything if we're already connected.
        if (ui->lnIPAddress->text() == IP && ui->spnPort->value() == Port) {
            return;
        }
        // IP and port info is old so close current socket.
        gameSocket.close();
    }

    // New socket time.
    IP = ui->lnIPAddress->text();
    Port = ui->spnPort->value();
    gameSocket.connectToHost(IP, Port);
}


//Buttons objects

void MainWindow::on_btnLaunchDefense_clicked()
{
    //placeholder
}


void MainWindow::on_btnEndFortification_clicked()
{
   //placeholder
}


void MainWindow::on_btnEndAttackPhase_clicked()
{
    //end attack placeholder
}


void MainWindow::on_btnSendReinforcements_clicked()
{
   //placeholder
}


void MainWindow::on_btnEndFortificationPhase_clicked()
{
    //End fortification phase
    //placeholder
}


void MainWindow::on_btnLaunchAttack_clicked()
{
    //placeholder
}

