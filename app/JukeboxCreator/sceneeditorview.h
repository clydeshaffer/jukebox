#ifndef SCENEEDITORVIEW_H
#define SCENEEDITORVIEW_H

#include <QGraphicsView>
#include <QContextMenuEvent>

class SceneEditorView : public QGraphicsView
{
public:
    SceneEditorView(QWidget* parent = nullptr);
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // SCENEEDITORVIEW_H
