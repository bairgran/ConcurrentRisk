#include "scoreboard.h"

Scoreboard::Scoreboard(QListView *view) : QObject(), listView(view) {  // Call QObject constructor
    model = new QStringListModel();  // No parent needed here
    listView->setModel(model);
}

void Scoreboard::updateScoreboard(const std::vector<Territory> &territories) {
    QStringList scoreList;

    for (const Territory &territory : territories) {
        // Convert adjacent territory IDs to a comma-separated string
        QStringList adjacentList;
        for (int adjacentId : territory.adjacent) {
            adjacentList.append(QString::number(adjacentId));
        }
        QString adjacentString = adjacentList.join(", ");

        // Format the score entry with all adjacent territories
        QString scoreEntry = QString("Territory %1: Owner: Player %2, Troops: %3, Adjacent: %4")
                                 .arg(territory.id)
                                 .arg(territory.owner)
                                 .arg(territory.troops)
                                 .arg(adjacentString);

        scoreList.append(scoreEntry);
    }

    model->setStringList(scoreList);
}
