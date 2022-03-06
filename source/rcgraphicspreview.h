#ifndef RCGRAPHICSPREVIEW_H
#define RCGRAPHICSPREVIEW_H

#include <QWidget>
#include <QGraphicsView>

class View3dSurface;
class QGraphicsScene;

class RCGraphicsPreview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit RCGraphicsPreview(QWidget *parent = 0);
    explicit RCGraphicsPreview(QGraphicsScene *scene, QWidget *parent = 0);
    ~RCGraphicsPreview();

    View3dSurface * mview3dsurface;

protected:
    void resizeEvent(QResizeEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void drawForeground(QPainter * painter, const QRectF & rect);

};

#endif // RCGRAPHICSPREVIEW_H
