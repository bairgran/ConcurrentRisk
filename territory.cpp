#include "territory.h"
#include <cmath>

Territory::Territory(int id, int owner, int troops)
    : id(id)
    , owner(owner)
    , troops(troops)
{
    // Adjacency list will be initialized in the game setup
}
void Territory::displayTerritoryGraph(QGraphicsScene *scene, const std::vector<Territory> &territories)
{
    // Define properties for nodes and edges
    int nodeSize = 40;
    int radius = 150; // Radius of the circular layout for nodes
    int centerX = 250; // X-coordinate for the center of the layout
    int centerY = 250; // Y-coordinate for the center of the layout

    // Calculate positions for nodes in a circular layout
    int numTerritories = territories.size();
    std::vector<QPointF> positions(numTerritories);

    for (int i = 0; i < numTerritories; ++i)
    {
        double angle = 2 * M_PI * i / numTerritories; // Angle for each node
        positions[i] = QPointF(centerX + radius * std::cos(angle),
                               centerY + radius * std::sin(angle));
    }

    // Add nodes (circles) to the scene
    for (const Territory &territory : territories)
    {
        QPointF pos = positions[territory.id];
        QGraphicsEllipseItem *node = scene->addEllipse(pos.x() - nodeSize / 2,
                                                       pos.y() - nodeSize / 2,
                                                       nodeSize, nodeSize,
                                                       QPen(Qt::black), QBrush(Qt::cyan));
        // Add text label for the node
        QGraphicsTextItem *label = scene->addText(QString::number(territory.id));
        label->setPos(pos.x() - nodeSize / 4, pos.y() - nodeSize / 4);
    }

    // Add directed edges (arrows) based on adjacency
    QPen edgePen(Qt::black, 2);

    for (const Territory &territory : territories)
    {
        QPointF fromPos = positions[territory.id];
        for (int adjId : territory.adjacent)
        {
            QPointF toPos = positions[adjId];

            // Draw the edge (line) between nodes
            QGraphicsLineItem *edge = scene->addLine(fromPos.x(), fromPos.y(),
                                                     toPos.x(), toPos.y(), edgePen);

            // Add an arrowhead to indicate direction
            double angle = std::atan2(toPos.y() - fromPos.y(), toPos.x() - fromPos.x());
            double arrowSize = 10;

            QPointF arrowP1 = toPos - QPointF(std::cos(angle + M_PI / 6) * arrowSize,
                                              std::sin(angle + M_PI / 6) * arrowSize);
            QPointF arrowP2 = toPos - QPointF(std::cos(angle - M_PI / 6) * arrowSize,
                                              std::sin(angle - M_PI / 6) * arrowSize);

            QPolygonF arrowHead;
            arrowHead << toPos << arrowP1 << arrowP2;
            scene->addPolygon(arrowHead, edgePen, QBrush(Qt::black));
        }
    }
}
