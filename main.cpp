#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "territory.h"
#include "mainwindow.h"
#include "gameserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //GameServer gameServer;
    //QObject::connect(&w, &MainWindow::userInputReceived, &gameServer, &GameServer::handleUserInput);
    w.show();


    // Create a QGraphicsView and QGraphicsScene
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);

    // Create 6 territories
    std::vector<Territory> territories = {
        Territory(0, 1, 10),
        Territory(1, 2, 5),
        Territory(2, 1, 15),
        Territory(3, 2, 7),
        Territory(4, 1, 20),
        Territory(5, 2, 12),
    };

    // Define adjacency (connections) for each territory
    territories[0].adjacent = {1, 2};
    territories[1].adjacent = {3};
    territories[2].adjacent = {3, 4};
    territories[3].adjacent = {5};
    territories[4].adjacent = {5};
    territories[5].adjacent = {0};

    // Use the displayTerritoryGraph function to render the graph
    territories[0].displayTerritoryGraph(&scene, territories);

    // Show the view
    view.setRenderHint(QPainter::Antialiasing);
    view.resize(600, 600);
    view.show();


    return a.exec();
}
