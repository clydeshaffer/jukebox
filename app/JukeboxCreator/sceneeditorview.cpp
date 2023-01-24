#include "sceneeditorview.h"
#include <QMenu>
#include <QGraphicsItem>

SceneEditorView::SceneEditorView(QWidget* parent) : QGraphicsView { parent }
{

}

void SceneEditorView::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsItem *sel = itemAt(event->pos());
    if(sel != nullptr && (sel->flags()) & QGraphicsItem::ItemIsSelectable) {
        scene()->clearSelection();
        sel->setSelected(true);

        QMenu menu(this);
        QAction *cloneAction = menu.addAction("clone");
        QAction *deleteAction = menu.addAction("delete");
        menu.exec(event->globalPos());
    }
}
