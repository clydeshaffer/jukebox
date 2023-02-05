#ifndef BEHAVIORLISTVIEW_H
#define BEHAVIORLISTVIEW_H

#include <QListView>
#include <QObject>
#include <QWidget>
#include <QGroupBox>

class BehaviorListView : public QListView
{
    Q_OBJECT
public:
    BehaviorListView(QGroupBox*& parent);
    void contextMenuEvent(QContextMenuEvent *event) override;
    QPoint getContextMenuPos();

private slots:
    void deleteSelected();
};

#endif // BEHAVIORLISTVIEW_H
