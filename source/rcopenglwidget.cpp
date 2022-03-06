
#include "ncwin.h"

#include "rcopenglwidget.h"

RCOpenGLWidget::RCOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mview3dsurface=0;
}

void RCOpenGLWidget::initializeGL()
{
    qDebug() << "RCOpenGLWidget::initializeGL()";
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
#if 0
    int x=0,y=0;

    if(mview3dsurface && mview3dsurface->gethdc())
    {
        OpenOpenGl(mview3dsurface->gethdc(),3,x,y,(int)mview3dsurface->getwidth(),(int)mview3dsurface->getheight());
        View *view = mview3dsurface->getview();

        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(mview3dsurface->getumin(),mview3dsurface->getvmin(),mview3dsurface->getumax(),mview3dsurface->getvmax());

        SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

        SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
    }
    else
        SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
#endif
}

void RCOpenGLWidget::resizeGL(int w, int h)
{
    qDebug() << "RCOpenGLWidget::resizeGL()";
#if 0
    if(mview3dsurface)
        SetViewport(mview3dsurface->getumin(),mview3dsurface->getvmin(),mview3dsurface->getumax(),mview3dsurface->getvmax());
#endif
    QOpenGLWidget::resizeGL(w,h);
}

void RCOpenGLWidget::paintGL()
{
    qDebug() << "RCOpenGLWidget::paintGL()";
    glClear(GL_COLOR_BUFFER_BIT);
#if 0
    Entity *e=0;
    EntityList dblist;

    BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);

    for (dblist.end() ; (e = dblist.last()) != 0 ; )
    {
        if (e->isvisible(0) && e->isa(group_entity) == 0)
        {
            if(e != app->mFlashEntity)
               e->drawGL(DM_NORMAL,NULL,0);
        }
    }

    EndScene();
#endif
    //QOpenGLWidget::paintGL();
}
