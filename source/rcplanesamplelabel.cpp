#include "rcplanesamplelabel.h"

RCPlaneSampleLabel::RCPlaneSampleLabel(QWidget *parent) : QLabel(parent)
{
    mDimHeight = 0.0;
}

void RCPlaneSampleLabel::setDimHeight(double h)
{
    mDimHeight = h;
}

void RCPlaneSampleLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(mDimHeight == 0.0)
        mDimHeight = rect().height() - 10.0;
    double voffset = (rect().height() - mDimHeight) / 2.0;

    QPointF topLeft(rect().topLeft());
    QPointF topRight(rect().topRight());
    QPointF bottomLeft(rect().bottomLeft());
    QPointF bottomRight(rect().bottomRight());
    QPointF topMid(topLeft + (topRight-topLeft) * 0.5);
    QPointF bottomMid(bottomLeft + (bottomRight-bottomLeft) * 0.5);

    // draw the horizontal lines
    painter.drawLine(topLeft.x()+5,topLeft.y()+voffset,topMid.x()+10,topRight.y()+voffset);
    painter.drawLine(bottomLeft.x()+5,bottomLeft.y()-voffset,bottomMid.x()+10,bottomRight.y()-voffset);

    // draw the vertical line
    painter.drawLine(topMid.x(),topMid.y()+voffset,bottomMid.x(),bottomMid.y()-voffset);

    // draw the arrows/dots
    QPen pen(QBrush(Qt::SolidPattern),4);
    painter.setPen(pen);
    painter.drawPoint(topMid.x(),topMid.y()+voffset);
    painter.drawPoint(bottomMid.x(),bottomMid.y()-voffset);
}
