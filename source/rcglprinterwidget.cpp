#include "rcglprinterwidget.h"

void RCGLPrintWidget::initializeGL()
{
    qDebug() << "in initializeGL";

    if(mrcwindow == 0)
        return;

    View *view;
    view = mrcwindow->getview();

    //qDebug() << "view name : " << QString(view->getname());

    //qDebug() << "app devicePixlRatio : " << app->devicePixelRatio();
    //qDebug() << "window devicePixlRatio : " << ((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio();

    //OpenOpenGl(((RCView*)parent())->view(),3,0,0,((RCView*)parent())->viewport()->width(),
    //                                             ((RCView*)parent())->viewport()->height());
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    glEnable(GL_LIGHT0);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f,1.0f,1.0f,1.0f);

#if 1
    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(mrcwindow->getumin(),mrcwindow->getvmin(),mrcwindow->getumax(),mrcwindow->getvmax());
#else
    if (view->getperspective())
      eye = view->geteye();
    else
      eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

    if (view->getperspective())
      SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
    else
      SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());
#endif

    glClearDepth(1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);;
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glDepthFunc(GL_LEQUAL);
    //glDepthFunc(GL_GEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glShadeModel(GL_SMOOTH);

    glClearColor(1.0f,1.0f,1.0f,1.0f);

    glFinish();

}

void RCGLPrintWidget::resizeGL()
{
    qDebug() << "in resizeGL";
    if(mrcwindow == 0)
        return;

    Point eye,pmin,pmax;
    View *view = mrcwindow->getview();
    db.geometry.extents(mrcwindow,&pmin,&pmax);
    //db.geometry.extents(0,&pmin,&pmax);

#if 0
    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#else
    if (view->getperspective())
      eye = view->geteye();
    else
      eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

    if (view->getperspective())
      SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
    else
      SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());
#endif
}

void RCGLPrintWidget::paintGL()
{
    qDebug() << "in paintGL";
    View *view;
    Entity *e=0;
    Point p1,p2;
    Point eye,pmin,pmax;
    EntityList dblist;
    int nlights=0;

    if(mrcwindow == 0)
        return;

    dblist = *db.geometry.getlist();

    view = mrcwindow->getview();

    if(mrcwindow->getrepaintstyle() == Shaded ||
       mrcwindow->getrepaintstyle() == ShadedHidden ||
       mrcwindow->getrepaintstyle() == ShadedHiddenDashed ||
       mrcwindow->getrepaintstyle() == ShadedTexture ||
       mrcwindow->getrepaintstyle() == ShadedHiddenTexture )
    {
        db.geometry.extents(mrcwindow,&pmin,&pmax);
        //db.geometry.extents(0,&pmin,&pmax);

#if 0
        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#else
        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

        if (view->getperspective())
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
        else
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());
#endif
        QtBeginScene();
        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );

        //dblist = *db.geometry.getlist();
        Entity *e=0;
        for (dblist.end() ; (e = dblist.last()) != NULL ; )
        {
          if (e->isa(light_entity) && e->isvisible(mrcwindow))
          {
               double red,green,blue;
               if (((Light *)e)->getoptions().test(1))
               {
                  red = green = blue = 1.0;
               }
               else
               {
                    red = db.colourtable[e->getcolour()].red/255.0;
                    green = db.colourtable[e->getcolour()].green/255.0;
                    blue = db.colourtable[e->getcolour()].blue/255.0;
               }

               SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
               nlights++;
          }
        }

        if (nlights == 0)
            SetLight(eye.x,eye.y,eye.z);

        for (dblist.end() ; (e = dblist.last()) != 0 ; )
        {
            //qDebug() << "e type : " << e->type();
            if (e->issurface() && e->isvisible(mrcwindow) )
            {
                qDebug() << "gl shading 3d window with no list";
                if( e->type() == plane_entity        ||
                    e->type() == ruledsurface_entity ||
                    e->type() == surfaceofrev_entity ||
                    e->type() == patch_entity  ||
                    e->type() == figure_entity ||
                    e->type() == group_entity )
                {
                        e->drawGL(DM_NORMAL,NULL,mrcwindow);
                }
            }
        }
    }
}
