#ifndef RCGRAPHICSSCENE_H
#define RCGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMimeData>

class QPainter;
class QRectF;
class QGraphicsScene;

/**
 * @brief The RCGraphicsScene class
 */
class RCGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit RCGraphicsScene(QObject * parent = 0);
    ~RCGraphicsScene();
    int mcount;
signals:

protected:
    //void paintEvent(QPaintEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void drawForeground(QPainter * painter, const QRectF & rect);
    /*
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    */
};

#endif // RCGRAPHICSSCENE_H
