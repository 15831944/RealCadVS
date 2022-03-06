#ifndef GRAPHICSRBITEM_H
#define GRAPHICSRBITEM_H

class QImage;
class QRect;
class QGraphicsLineItem;

class RBLineItemV1 : public QGraphicsLineItem
{
protected:
    QImage mlastBuffer;
    QRect mlastRect;
    bool mfirstTime;
    bool mOKToPaint;

    void init();

public:
    explicit RBLineItemV1(QGraphicsItem * parent = 0);
    explicit RBLineItemV1(const QLineF & line, QGraphicsItem * parent = 0);
    explicit RBLineItemV1(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0);

    void grabUnder();
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape (void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class RBLineItemV2 : public QGraphicsLineItem
{
protected:
    QImage mlastBuffer;
    QRect mlastRect;
    bool mOKToPaint;

    void init();

public:
    explicit RBLineItemV2(QGraphicsItem * parent = 0);
    explicit RBLineItemV2(const QLineF & line, QGraphicsItem * parent = 0);
    explicit RBLineItemV2(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape (void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void grabUnder();

    // Rubber band convenience methods
    void endRB();
    void moveTo(QPointF p);
    void lineTo(QPointF p);
    void movep1(QPointF p);
    void movep2(QPointF p);
    void move(QPointF p1, QPointF p2);
    void move(qreal dx, qreal dy);
    void startRB();

};


class RBLineItem : public QGraphicsLineItem
{
protected:
    int mpickPoint;
    void init();

public:
    explicit RBLineItem(QGraphicsItem * parent = 0);
    explicit RBLineItem(const QLineF & line, QGraphicsItem * parent = 0);
    explicit RBLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape (void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};
#endif // GRAPHICSRBITEM_H
