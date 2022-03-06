#include "ncwin.h"
#include  <QGraphicsView>;
#include  <QGraphicsScene>;
#include "rcgraphicspreview.h"

RCGraphicsPreview::RCGraphicsPreview(QWidget *parent)
    : QGraphicsView(parent)
{
    mview3dsurface = 0;
}

RCGraphicsPreview::RCGraphicsPreview(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    mview3dsurface = 0;
}

RCGraphicsPreview::~RCGraphicsPreview()
{

}

void RCGraphicsPreview::resizeEvent(QResizeEvent *event)
{
    setSceneRect(QRect(QPoint(0,0),event->size()));
    if(scene())
        scene()->setSceneRect(0,0,50000,50000);

    //if(mview3dsurface != 0)
    //    mview3dsurface->sizeevent(event->size().width(),event->size().height());
}

void RCGraphicsPreview::drawBackground(QPainter * painter, const QRectF & rect)
{
    View3dSurface *surface = (View3dSurface*)mview3dsurface;

    //if(window == 0)
     //   return;

    if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        painter->fillRect(rect,QBrush(QColor(255,255,255)));

    }
    else
    {
        QGraphicsView::drawBackground(painter,rect);

    }

}

void RCGraphicsPreview::drawForeground(QPainter * painter, const QRectF & rect)
{
    View3dSurface *surface = (View3dSurface*)mview3dsurface;

    //if(window == 0)
     //   return;

    if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        //painter->fillRect(rect,QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
        //                                 int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
        //                                 int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));

    }
    else
    {
        QGraphicsView::drawForeground(painter,rect);
    }
}
