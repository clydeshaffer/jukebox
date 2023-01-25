#include "sceneeditorview.h"
#include <QMenu>
#include <QGraphicsItem>

SceneEditorView::SceneEditorView(QWidget* parent) : QGraphicsView { parent }
{
    createAction = new QAction(tr("New entity here"), this);
    createAction->setStatusTip(tr("Create a new entity at the cursor position"));

    cloneAction = new QAction(tr("Clone"), this);
    cloneAction->setStatusTip(tr("Create a copy of this entity"));

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setStatusTip(tr("Remove this entity from the scene"));
}

void SceneEditorView::contextMenuEvent(QContextMenuEvent *event)
{
    lastRightClickedPos = event->pos();
    QGraphicsItem *sel = itemAt(event->pos());
    if(sel != nullptr && (sel->flags()) & QGraphicsItem::ItemIsSelectable) {
        scene()->clearSelection();
        sel->setSelected(true);

        QMenu menu(this);
        menu.addAction(cloneAction);
        menu.addAction(deleteAction);
        menu.exec(event->globalPos());
    } else {
        QMenu menu(this);
        menu.addAction(createAction);
        menu.exec(event->globalPos());
    }
}

QPoint SceneEditorView::getContextMenuPos()
{
    return lastRightClickedPos;
}
