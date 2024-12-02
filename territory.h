#ifndef TERRITORY_H
#define TERRITORY_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>

class Territory
{
public:
    int id;                    // Unchanging territory ID
    int owner;                 // Player 1 or Player 2
    int troops;                // Number of troops in the territory
    std::vector<int> adjacent; // Adjacency list (IDs of adjacent territories)

    Territory(int id, int owner, int troops);
    void displayTerritoryGraph(QGraphicsScene *scene, const std::vector<Territory> &territories);
};

#endif // TERRITORY_H
