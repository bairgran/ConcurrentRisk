#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QListView>
#include <QObject>
#include <QStringListModel>
#include "territory.h"
#include <vector>

class Scoreboard : public QObject
{ // Inherit from QObject
    Q_OBJECT

public:
    Scoreboard(QListView *view);
public slots:
    void updateScoreboard(const std::vector<Territory> &territories);

private:
    QStringListModel *model;
    QListView *listView;
};

#endif // SCOREBOARD_H
