#include "behaviorlistview.h"

#include <QMenu>
#include <QContextMenuEvent>

BehaviorListView::BehaviorListView(QGroupBox*& parent) : QListView(parent)
{

}

void BehaviorListView::contextMenuEvent(QContextMenuEvent *event)
{
    if(selectedIndexes().size() == 1) {
        QMenu menu(this);
        menu.addAction("Delete", this, SLOT(deleteSelected()));
        menu.exec(event->globalPos());
    }
}

void BehaviorListView::deleteSelected()
{
    if(selectedIndexes().size() == 1) {
        int ind = selectedIndexes().at(0).row();
        model()->removeRow(ind);
    }
}
