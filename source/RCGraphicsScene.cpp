
#include <QtOpenGL/QtOpenGl>
#include <QGLFormat>
#include <QGLWidget>
#include <QGraphicsScene>


#include "qtdefs.h"
#include "RCOpenGL.h"
#include "RCGraphicsScene.h"

static int grcgscenecount=0;
RCGraphicsScene::RCGraphicsScene(QObject * parent) :
    QGraphicsScene(parent)
{
    mcount = ++grcgscenecount;
    qDebug() << "in RCGraphicsScene CTOR : " << mcount;
}

RCGraphicsScene::~RCGraphicsScene()
{
    qDebug() << "in RCGraphicsScene DTOR : " << mcount;
}

void RCGraphicsScene::drawForeground(QPainter * painter, const QRectF & rect)
{
    QGraphicsScene::drawForeground(painter,rect);
    //db.grid.draw(DM_NORMAL,cadwindow->getcurrentsurface(),painter);
}

void RCGraphicsScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    //QtEndScene();
    //QtBeginScene();
    QGraphicsScene::drawBackground(painter,rect);
    //db.grid.draw(DM_NORMAL,cadwindow->getcurrentsurface(),painter);
}
/*
void RCGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
}

void RCGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void RCGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
}

void RCGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dropEvent(event);
}
*/
