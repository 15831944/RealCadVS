
#include "ncwin.h"

#include  <QtWidgets>;
#include  <QEvent>;
#include  <QtOpenGL/QtOpenGl>
#include  <QGraphicsView>;
#include  <QGraphicsScene>;

#include "RCView.h"
#include "RCCentralWidget.h"
#include "RCGraphicsScene.h"
#include "RCGraphicsView.h"

//#define DEVICEPIXELRATIO app->devicePixelRatio()
#define DEVICEPIXELRATIO (cadwindow!=0?(cadwindow->gethwnd()!=0?((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio():1.0):1.0)
//#define DEVICEPIXELRATIO 1.0

extern void file_open(int nocadfiles,RCCHAR *fname);
extern void open_dwg(RCCHAR *filename);
extern void open_dxf(RCCHAR *filename);
//extern void open_iges(RCCHAR *filename);

static int xlast=0,ylast=0;

// possible rubberband box function
// using XOR
void _DrawRBRect(int x1, int y1, int x2, int y2)
{
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glRecti(x1,y1, x2,y2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_COLOR_LOGIC_OP);
}


/**
 * @brief RCGraphicsView::drawForeground
 * @param painter
 * @param rect
 */
void RCGraphicsView::drawForeground(QPainter * painter, const QRectF & rect)
{
    //qDebug() << "in drawForeground";
    //QGraphicsView::drawForeground(painter,rect);

#if 0
    // this is the painter only version
    // find the view for the window we are drawing??
    if(((RCView*)parent())->getView3dWindow())
    {
        db.grid.draw(DM_NORMAL ,((RCView*)parent())->getView3dWindow(),painter);
        db.workplanes.draw(DM_NORMAL,((RCView*)parent())->getView3dWindow(),painter);
    }
#endif
#if 1
    //setForegroundBrush(QBrush(Qt::NoBrush));

    View3dWindow *window = ((RCView*)parent())->getView3dWindow();

    if(window == 0)
        return;


    //if(window->cursorisdrawing())
     //   return;

    //qDebug() << "view name " << QString(window->getview()->getname());

    //if (window->getsurfacetype() != View2dSurf )
    //    db.workplanes.draw(DM_NORMAL,((RCView*)parent())->getView3dWindow(),painter);

    //qDebug() << "paint engine type : " << painter->paintEngine()->type();

    if(backImage == 0 && painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    //if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        //qDebug() << "timer start of forground" << fTimer.elapsed();

        //qDebug() << "paint engine type : " << painter->paintEngine()->type();
        //painter->save();
        painter->beginNativePainting();

        View *view;
        Entity *e;
        Point p1,p2;
        Point eye,pmin,pmax;

        view = window->getview();

        //OpenOpenGl(((RCView*)parent())->view(),3,0,0,((RCView*)parent())->view()->width(),
        //                                             ((RCView*)parent())->view()->height());
        OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                   ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                   ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

#if 1
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
        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this aand set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 0 /* dot grid */)
            db.grid.drawGL(DM_NORMAL ,window);
        //else
        //    if (cadwindow != 0)
        //        cadwindow->updatetitle();

        if (window->getsurfacetype() != View2dSurf && window->getrepaintstyle() == Wireframe )
            db.workplanes.drawGL(DM_NORMAL,window);

        // this rubberbad call hangs with multiple views???
        //QPoint cpos = QCursor::pos();
        //cpos = ((RCView*)parent())->viewport()->mapFromGlobal(cpos);
        //rubberband.mousemoveevent(window,cpos.x(),cpos.y(),MK_LBUTTON);

#if 0
        // for gl rubber box cursor
        window->rubberbandGL();
#endif

        // if using xor uncomment this code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glFinish();

        //glPopMatrix();

        EndScene();

        painter->endNativePainting();
        //painter->restore();
        //qDebug() << "timer end of forground" << fTimer.elapsed();

    }
    else
    {
#if 0
        painter->beginNativePainting();

        View *view;
        Entity *e;
        Point p1,p2;
        Point eye,pmin,pmax;

        view = window->getview();

        OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                   ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                   ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());

        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // for gl rubber box cursor
        window->rubberbandGL();

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glFinish();

        EndScene();

        painter->endNativePainting();
#endif
        QGraphicsView::drawForeground(painter,rect);
    }
    // draw stuff that must be drawn after GL here i.e. painter stuff

    // draw the work plane icon if this is not a 2d view
    // this version uses the Qpainter

    //QGraphicsView::drawForeground(painter,rect);
#endif
}

void regenGLlists()
{
    // reset all visible views
    if(cadwindow->gethwnd())
    {
        if(((QtMainWindow*)cadwindow->gethwnd())->centerWidget)
            ((QtMainWindow*)cadwindow->gethwnd())->centerWidget->regenGLlists();
    }
}

void RCGraphicsView::regenGLlists()
{
    // reset the list flags so they will be regenerated
    // set the resizing flag to false
    gldlist1 = 0;
    gldlist2 = 0;
    gldlist3 = 0;
    gldlist4 = 0;
    gldlist5 = 0;
    gldlist6 = 0;
    gldlist7 = 0;
    gldlist8 = 0;
    resizing = false;
    //refreshTimer->stop();
}

void RCGraphicsView::resetGLlists()
{
    // reset the list flags so they will be regenerated
    // set the resizing flag to false
    gldlist1 = 0;
    gldlist2 = 0;
    gldlist3 = 0;
    gldlist4 = 0;
    gldlist5 = 0;
    gldlist6 = 0;
    gldlist7 = 0;
    gldlist8 = 0;
    resizing = 1;
    //refreshTimer->stop();
}
void RCGraphicsView::setTimer(int ms)
{
    if(ms == 0)
    {
        refreshTimer->stop();
        return;
    }
    refreshDelay = ms;
    refreshTimer->start(refreshDelay);
}

void RCGraphicsView::refresh()
{
    //qDebug() << "firing refresh slot";
    //qInfo() << "firing refresh slot";
    refreshTimer->stop();
    if(scene())
        scene()->setBackgroundBrush(QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                              int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                              int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));
}

#ifdef USING_GLDLISTS
#if 1 // dlist and background image
void RCGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    //qDebug() << "entering RCGraphicsView::drawBackground";

    if(cadwindow && cadwindow->getDrawingBatch() != 0)
        return;

    View3dWindow *window = ((RCView*)parent())->getView3dWindow();

    if(window == 0)
        return;

    //qDebug() << "in RCGraphicsView::drawBackground (window) : " << window;
    //qDebug() << "window->getscrolling()" << window->getscrolling();
    //qDebug() << "window->getcursoraction()" << window->getcursoraction();

    if(backImage == 0
        /*
       ||
       window->getRaytracing() == 0 ||
       backImage == 0 ||
       resizing ||
       window->getscrolling() ||
       state.dragging() ||
       (rubberband.drawing && state.getcommandid() != 121) || // print select window command
       window->getcursoraction() != 0 ||
       window->getrepaintstyle() != RayTraced
       */
     )
    {
        backImage = 0;
        painter->fillRect(rect,QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));
    }
    else
    {
        // if we use the rect width height we don't need to divide by the device pixel ratio here
        window->getScene()->setBackgroundBrush(QBrush(backImage->scaled(rect.width(),rect.height(),
                                               Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        /*
         * this is a alternative method
        if(DEVICEPIXELRATIO > 1)
        {
            // on a retina (HDPI) display the image needs to be scaled down
            window->getScene()->setBackgroundBrush(QBrush(backImage->scaled(backImage->width()/devicePixelRatio(),backImage->height()/devicePixelRatio(),
                                                                            Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                                                            Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        }
        else // both these functions work
            window->getScene()->setBackgroundBrush(QBrush(backImage->scaled(rect.width(),rect.height(),
                                                                        Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        */
    }

    //if( resizing )
    //    return;

    //QGraphicsView::drawBackground(painter,rect);

    if(backImage == 0 && painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    //if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        fRate = 0;
        refreshTimer->stop();
        fTimer.start();
        //qDebug() << "timer start" << fTimer.elapsed();
        //qDebug() << "painter is a GL engine";
        //qDebug() << "drawing with the GL engine";
        //qDebug() << "timer start of background" << fTimer.elapsed();
        int reduceDetail = v.getinteger("wn::reducedrawdetail");

        painter->save();
        painter->beginNativePainting();

        View *view;
        Entity *e=0;
        Point p1,p2;
        Point eye,pmin,pmax;
        EntityList dblist;
        int nlights=0;

        dblist = *db.geometry.getlist();

        view = window->getview();

        //qDebug() << "view name : " << QString(view->getname());

        //qDebug() << "app devicePixlRatio : " << app->devicePixelRatio();
        //qDebug() << "window devicePixlRatio : " << ((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio();

        //OpenOpenGl(((RCView*)parent())->view(),3,0,0,((RCView*)parent())->viewport()->width(),
        //                                             ((RCView*)parent())->viewport()->height());
        OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                   ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                   ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);
#if 1
        // KMJ: bunch of experimental and debug if defs here
#if 1
#if 0
        // only user this for rendering due to lighting??? not sure though
        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();
#endif
        // this is the legacy code for setting up the gl viewing
        // it uses home made matrices for the model view and the project view modes
        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#else
#if 0
        // only user this for rendering due to lighting??? not sure though
        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();
#endif
        // these use the OpenGL view settings to set up the viewing
        // I would prefer to uses these but the RC perspective does not quit match OpenGL
        // still working on it
        //RCSetView(view->geteye().x,view->geteye().y,view->geteye().z,
        //          view->getref().x,view->getref().y,view->getref().z,
        //          view->getup().x,view->getup().y,view->getup().z,
        //          view->getfront(),view->getback(),view->getperspective());
        //RCSetViewport(0,0,((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO,
        //              window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),
        //              view->getfront(),view->getback(),view->getperspective());
        RCSetViewing(0,0,((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO,
                     window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),
                     view->geteye(),view->getref(),view->getup(),
                     view->getfront(),view->getback(),view->getperspective());
#endif
#else
        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

        if (view->getperspective())
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
        else
          SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#endif
        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        setGLpixelMM(window->getpixelmodelmm());

        // these settings are for the grid and workplanes
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this as set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 1 /*line grid*/)
            db.grid.drawGL(DM_NORMAL ,window);
        //if (window->getsurfacetype() != View2dSurf )
        //    db.workplanes.drawGL(DM_NORMAL,window);


        //glPopMatrix();
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        EndScene();

        // switch off rendering and hidden line when selecting
        //qDebug() << "resizing    : " << resizing;
        //qDebug() << "cursorisdrawing    : " << window->cursorisdrawing();
        //qDebug() << "window->getscrolling(): " << window->getscrolling();
        //qDebug() << "state.dragging()      : " << state.dragging();
        //qDebug() << "state.getcommandid()      : " << state.getcommandid();
        //qDebug() << "rubberband.drawing    : " << rubberband.drawing;
        //qDebug() << "getcursoraction       : " << window->getcursoraction();
        //qDebug() << "db.getchanged()    : " << db.getchanged();
        //qDebug() << "selection length" << state.getselection().length();
        //qDebug() << "(rubberband.drawing && window->getcursoraction() == 0)" << (rubberband.drawing && window->getcursoraction() == 0);
        if( state.getselection().length() > 0 ||
           (rubberband.drawing &&
            state.getcommandid() != 2101 &&
            state.getcommandid() != 2102 &&
            state.getcommandid() != 121
            )
                /*&&
            (window->getrepaintstyle() == Hidden ||
             window->getrepaintstyle() == HiddenDashed ||
             window->getrepaintstyle() == ShadedHidden ||
             window->getrepaintstyle() == ShadedHiddenDashed ||
             window->getrepaintstyle() == ShadedHiddenTexture)*/)
            window->setrepaintstyle(Wireframe);

        if (window->getsurfacetype() == View3dSurf)
        {
            // ToDo: think about mixed shading modes??
            // Shading with OpenGL
            // only drawing shadable entities

            if(window->getrepaintstyle() == Shaded ||
               window->getrepaintstyle() == ShadedHidden ||
               window->getrepaintstyle() == ShadedHiddenDashed ||
               window->getrepaintstyle() == ShadedTexture ||
               window->getrepaintstyle() == ShadedHiddenTexture )
            {
                db.geometry.extents(window,&pmin,&pmax);
                //db.geometry.extents(0,&pmin,&pmax);

#if 1
#if 1
                if (view->getperspective())
                  eye = view->geteye();
                else
                  eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#else
                // this use the OpenGL view settings to set up the viewing
                // I would prefer to use this but the RC perspective does not quit match OpenGL
                // still working on it
                double modradius = (pmax - pmin).length() * 0.5;
                double front = 0.1;
                double back  = 1000.0;
                RCSetViewing(0,0,((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO,
                             window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),
                             view->geteye(),view->getref(),view->getup(),
                             front,back,view->getperspective());
#endif
#else
                if (view->getperspective())
                  eye = view->geteye();
                else
                  eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

                if (view->getperspective())
                  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
                else
                  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

                SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#endif
                QtBeginScene();
                BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

                glEnable(GL_LIGHTING);
                glEnable(GL_DEPTH_TEST);
                glClear( GL_DEPTH_BUFFER_BIT );

                //dblist = *db.geometry.getlist();
                Entity *e=0;
                for (dblist.end() ; (e = dblist.last()) != NULL ; )
                //for (dblist.start() ; (e = dblist.next()) != NULL ; )
                {
                  if (e->isa(light_entity) && e->isvisible(window))
                  {
                       double red,green,blue;
                       if (((Light *)e)->getoptions().test(1))
                         {  red = green = blue = 1.0;
                         }
                       else
                         {  red = db.colourtable[e->getcolour()].red/255.0;
                            green = db.colourtable[e->getcolour()].green/255.0;
                            blue = db.colourtable[e->getcolour()].blue/255.0;
                         }

                       SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                       nlights++;
                  }
                }

                if (nlights == 0)
                    SetLight(eye.x,eye.y,eye.z);
                /*
                    if(view->getperspective())
                        SetLight(view->geteye().x,view->geteye().y,view->geteye().z);
                    else
                    {
                        eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();
                        SetLight(eye.x,eye.y,eye.z);
                    }
                */

                if(resizing || window->getscrolling() || state.dragging() ||
                   (rubberband.drawing && window->getcursoraction() != 0))
                   // rubberband.drawing || window->getcursoraction() != 0)
                {
                    if(gldlist5 == 0)
                    {
                        if(glIsList(5) == GL_TRUE)
                            glDeleteLists(5,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(5,GL_COMPILE);
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "e type : " << e->type();
                            if (e->issurface() && e->isvisible(window) )
                            {
                                //qDebug() << "gl shading 3d window with no list";
                                if( e->type() == plane_entity        ||
                                    e->type() == ruledsurface_entity ||
                                    e->type() == surfaceofrev_entity ||
                                    e->type() == patch_entity  ||
                                    //e->type() == figure_entity ||
                                    e->type() == group_entity )
                                {
                                    //if(window->getoffscreenbitmap())
                                    //{
                                    //    e->drawGL(DM_NORMAL,NULL,window->getoffscreenbitmap());
                                    //}
                                    //else
                                        e->drawGL(DM_NORMAL,NULL,window);
                                 }
                            }
                            //if (program->getinterrupt())
                            //    break;
                        }
                        glEndList();
                        glCallList(5);
                        //resizing=false;
                        gldlist5 = 1;

                    }
                    else
                    {
                        glCallList(5);
                    }
                }
                else
                {
                    if(glIsList(5) == GL_TRUE)
                        glDeleteLists(5,1);
                    gldlist5 = 0;
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        //qDebug() << "e type : " << e->type();
                        if (e->issurface() && e->isvisible(window) )
                        {
                            //qDebug() << "gl shading 3d window with no list";
                            if( e->type() == plane_entity        ||
                                e->type() == ruledsurface_entity ||
                                e->type() == surfaceofrev_entity ||
                                e->type() == patch_entity  ||
                                //e->type() == figure_entity ||
                                e->type() == group_entity )
                            {
                                //if(window->getoffscreenbitmap())
                                //{
                                //    e->drawGL(DM_NORMAL,NULL,window->getoffscreenbitmap());
                                //}
                                //else
                                    e->drawGL(DM_NORMAL,NULL,window);
                             }
                        }
                        //if (program->getinterrupt())
                        //    break;
                    }
                }


                if( (window->getrepaintstyle() != Shaded && window->getrepaintstyle() != ShadedTexture))
                {
                if( resizing ||
                    (window->getscrolling() && window->getscrolling() !=2) ||
                    state.dragging() ||
                    rubberband.drawing ||
                    window->getcursoraction() != 0 )
                {
                    if(gldlist4 == 0)
                    {
                        //qDebug() << "gl hidden line drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(4) == GL_TRUE)
                            glDeleteLists(4,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(4,GL_COMPILE);

                        HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                        image.hideGL();

                        glEndList();
                        glCallList(4);
                        //resizing=false;
                        gldlist4 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing hidden line with list";
                        glCallList(4);
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing hidden line no list";
                    gldlist4 = 0;
                    if(glIsList(4) == GL_TRUE)
                        glDeleteLists(4,1);
                    HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                    image.hideGL();

                }
                }
                //EndScene();
                QtEndScene();
            }
            else if(window->getrepaintstyle() == Hidden ||
                    window->getrepaintstyle() == HiddenDashed )
            {
                if( resizing ||
                    (window->getscrolling() && window->getscrolling() !=2) ||
                    state.dragging() ||
                    rubberband.drawing ||
                    window->getcursoraction() != 0)
                {
                    if(gldlist4 == 0)
                    {
                        //qDebug() << "gl hidden line drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(4) == GL_TRUE)
                            glDeleteLists(4,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(4,GL_COMPILE);

                        HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                        image.hideGL();

                        glEndList();
                        glCallList(4);
                        //resizing=false;
                        gldlist4 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing hidden line with list";
                        glCallList(4);
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing hidden line no list";
                    gldlist4 = 0;
                    if(glIsList(4) == GL_TRUE)
                        glDeleteLists(4,1);
                    HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                    image.hideGL();

                }
            }
            else if(window->getrepaintstyle() == Wireframe ||
                    window->getrepaintstyle() == RayTraced)
            {
                //qDebug() << "starting draw";
                //qDebug() << "scrolling is :" << window->getscrolling();
                // drawing wireframe entites with OpenGL
                Entity *e=0;
                int ecount=1;
                //qDebug() << "resizing    : " << resizing;
                //qDebug() << "cursorisdrawing    : " << window->cursorisdrawing();
                //qDebug() << "window->getscrolling(): " << window->getscrolling();
                //qDebug() << "state.dragging()      : " << state.dragging();
                //qDebug() << "rubberband.drawing    : " << rubberband.drawing;
                //qDebug() << "getcursoraction       : " << window->getcursoraction();
                //qDebug() << "db.getchanged()    : " << db.getchanged();
                if(resizing || window->getscrolling() || state.dragging() ||
                   rubberband.drawing || window->getcursoraction() != 0)
                {
                    if(gldlist1 == 0)
                    {
                        //qDebug() << "gl drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(1) == GL_TRUE)
                            glDeleteLists(1,1);
                        //glNewList(1,GL_COMPILE_AND_EXECUTE);
                        glNewList(1,GL_COMPILE);
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "entity type : " << e->type();
                            //qDebug() << "entity qgi  : " << e->qgi;
                            if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                            //if (e->isvisible(window) /*&& !e->getselected()*/ /* && !state.getselection().inlist(e)*/)
                            {
                                if(e != app->mFlashEntity)
                                {
                                    if(window->getscrolling() && reduceDetail)
                                        e->drawGL(DM_SCROLLING,NULL,window);
                                    else
                                        e->drawGL(DM_NORMAL,NULL,window);
                                }
                            }
                            if (program->getinterrupt())
                                break;
                        }
                        glEndList();
                        if(glIsList(1) == GL_TRUE)
                            glCallList(1);
                        //resizing=false;
                        gldlist1 = 1;
                        //refreshTimer->start();
                        //setTimer(lastfRate);
                        //qDebug() << "frame rate" << fRate;
                        //qDebug() << "last frame rate" << lastfRate;
                    }
                    else
                    {
                        //qDebug() << "gl drawing 3d window with list";
                        if(glIsList(1) == GL_TRUE)
                            glCallList(1);
                        //refreshTimer->start();
                        //setTimer(lastfRate);
                        //qDebug() << "frame rate" << fRate;
                        //qDebug() << "last frame rate" << lastfRate;
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing 3d window with no list";
                    if(glIsList(1) == GL_TRUE)
                        glDeleteLists(1,1);
                    gldlist1 = 0;
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        //qDebug() << "entity type : " << e->type();
                        //qDebug() << "entity qgi  : " << e->qgi;
                        if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                        //if (e->isvisible(window) /*&& !e->getselected()*/ /* && !state.getselection().inlist(e)*/)
                        {
                            if(e != app->mFlashEntity)
                               e->drawGL(DM_NORMAL,NULL,window);
                            ecount++;
                        }
                        if (program->getinterrupt())
                            break;
                    }
                }

                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
                }
#if 1
                EntityList list = state.getselection();
                if(resizing || window->getscrolling() ||
                   rubberband.drawing || window->getcursoraction() != 0)
                {
                    if(gldlist6 == 0)
                    {
                        //qDebug() << "gl drawing making list";
                        if(glIsList(6) == GL_TRUE)
                            glDeleteLists(6,1);
                        glNewList(6,GL_COMPILE);
                        for (list.end() ; (e = list.last()) != 0 ; )
                        {
                            if (e != 0 && e->isvisible(window) )
                            {
                                //qDebug() << "view3dsurf drawing select list entity #" << secount;
                                e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                            }
                        }
                        glEndList();
                        glCallList(6);
                        gldlist6 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing selection with list";
                        glCallList(6);
                    }
                }
                else
                {
                    //qDebug() << "gl drawing selection with no list";
                    if(glIsList(6) == GL_TRUE)
                        glDeleteLists(6,1);
                    gldlist6 = 0;
                    for (list.end() ; (e = list.last()) != 0 ; )
                    {
                        if (e != 0 && e->isvisible(window) )
                        {
                            //qDebug() << "view3dsurf drawing select list entity : " << e->type();
                            e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                        }
                    }
                }
#endif
                //state.drawdraggerGL(DM_NORMAL,window);
                //glDisable(GL_COLOR_LOGIC_OP);
                //glPopAttrib();
                //glDrawBuffer(GL_FRONT);
            }
            EndScene();
        }
        // this is a reproduction of what View2dpaintevent does but using open gl
        // should this be done as a View2dWindow::paintevenGL kind of function??
        // and View3dWindow::paintevent also??
        else if (window->getsurfacetype() == View2dSurf)
        {
            // erase the scene boundary
            //((View2dWindow*)window)->drawboundary(0,0,0,0,DM_ERASE);

            // force the line weight display on for layout space
            int dbWeightdisplay = db.getlineweightdisplay() ;
            db.setlineweightdisplay(1);
            //qDebug() << "View2dSurf drawing";
            // draw the borders of the View2dWindow (the composed view)
            Point p1,p2;
            window->setupGL(1.0,0.0,DM_DASH,0,0,0,0);
            RCCOLORREF pcol = window->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            //GLfloat lwidth = window->curPen.widthF();
            glLineWidth(window->curGLWeight);

            p1.setxyz(0.0,0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,0.0,0.0);
            window->line3dGL(p1,p2);

            //window->setupGL(1.0,0.0,DM_GRID,0,0,0,0);
            View3dSubSurface *ss;
            // draw the sub window borders first
            int numsub=0;
            for (((View2dWindow*)window)->subsurfaces.start(),numsub=0 ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ;numsub++ )
                ss->drawborderGL(DM_DASH);
            // now draw the sub windows contents
            int dlc=0;
            for (((View2dWindow*)window)->subsurfaces.start(),dlc=0 ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ; dlc++ )
            {
                Point p1,p2,p3,p4,o,xaxis,yaxis;

                ss->setblackandwhite(window->getblackandwhite());
                ss->setdisplaylayertable(ss->getoneview()->getlayertable());

                ss->setscale( (ss->getumax() - ss->getumin()) / ss->getwidthmm());
                o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                p1 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p2 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p3 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
                p4 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

                // we have a pointer to the view not a copy
                // anything we change here must be restored later
                // save the transform flag because rubberbanding needs it
                //ss->setplotscale(1.0);
                int transflag = ss->gettransform2d();
                setGLpixelMM(ss->getpixelmodelmm());
                // clear the transform flag an reset the matrix for Gl drawing
                ss->cleartransform();
                ss->updatematrix();

                OpenOpenGl(((RCView*)parent())->view(),0,p1.x*DEVICEPIXELRATIO ,p1.y*DEVICEPIXELRATIO ,
                           ss->getwidth()*DEVICEPIXELRATIO,
                           ss->getheight()*DEVICEPIXELRATIO);

                View *view = ss->getview();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(ss->getumin(),ss->getvmin(),ss->getumax(),ss->getvmax());

                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

                BeginScene(BS_IGNOREDEPTH | BS_NOCLEAR/*BS_FRONTBUFFERONLY | BS_IGNOREDEPTH | BS_NOCLEAR*/);//  Allow the clear for now | BS_NOCLEAR);
                SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

                if(window->getrepaintstyle() == Wireframe ||
                   window->getrepaintstyle() == RayTraced)
                {
                    Entity *e=0;
                    if(resizing || window->getscrolling() || state.dragging() ||
                       rubberband.drawing || window->getcursoraction() != 0 )
                    {
                        if(gldlist2 < numsub)
                        {
                            //qDebug() << "gl drawing making sub list";
                            //qDebug() << "view number : " << ss->getview()->getviewno()+4;
                            if(glIsList(dlc+10) == GL_TRUE)
                                glDeleteLists(dlc+10,1);
                            glNewList(dlc+10,GL_COMPILE_AND_EXECUTE);
                            for (dblist.end() ; (e = dblist.last()) != 0 ; )
                            {
                                //qDebug() << "subview ent type : " << e->type();
                                if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                                {
                                    if(e != app->mFlashEntity)
                                    {
                                        if(window->getscrolling() && reduceDetail)
                                            e->drawGL(DM_SCROLLING,NULL,ss);
                                        else
                                            e->drawGL(DM_NORMAL,NULL,ss);
                                    }
                                }
                            }
                            glEndList();
                            gldlist2++;
                            //RCCheckGlError();
                        }
                        else
                        {
                            //qDebug() << "gl drawing sub window with list";
                            //fprintf(stderr,"gl drawing sub window with list\n");
                            glCallList(dlc+10);
                            //RCCheckGlError();
                        }
                    }
                    else
                    {
                        //qDebug() << "gl drawing sub window with no list";
                        //fprintf(stderr,"gl drawing sub window with no list\n");
                        if(glIsList(dlc+10) == GL_TRUE)
                            glDeleteLists(dlc+10,1);
                        gldlist2 = 0;
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "subview ent type : " << e->type();
                            if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                            {
                                //e->drawGL(DM_NORMAL,0,ss);
                                if(e != app->mFlashEntity)
                                    e->drawGL(DM_NORMAL,NULL,ss);
                            }
                        }
                    }
                }
                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,ss);
                }
                EntityList list = state.getselection();
                if(resizing || window->getscrolling() ||
                   rubberband.drawing || window->getcursoraction() != 0)
                {
                    if(gldlist7 < numsub)
                    {
                        //qDebug() << "gl drawing making list";
                        if(glIsList(dlc+20) == GL_TRUE)
                            glDeleteLists(dlc+20,1);
                        glNewList(dlc+20,GL_COMPILE);

                        for (list.end() ; (e = list.last()) != 0 ; )
                        {
                            if (e->isvisible(ss) )
                            {
                                //qDebug() << "subview drawing select list entity #" << secount;
                                e->drawGL(DM_SELECT,&state.glDraggerTform,ss);
                            }
                        }

                        glEndList();
                        glCallList(dlc+20);
                        gldlist7++;
                    }
                    else
                    {
                        //qDebug() << "gl drawing selection with list";
                        glCallList(dlc+20);
                    }
                }
                else
                {
                    if(glIsList(dlc+20) == GL_TRUE)
                        glDeleteLists(dlc+20,1);
                    gldlist7 = 0;
                    for (list.end() ; (e = list.last()) != 0 ; )
                    {
                        if (e->isvisible(ss) )
                        {
                            //qDebug() << "subview drawing select list entity #" << secount;
                            e->drawGL(DM_SELECT,&state.glDraggerTform,ss);
                        }
                    }
                }

                //state.drawdraggerGL(DM_NORMAL,ss);
                EndScene();
                //restore the transform flag for rubberbanding
                ss->settransform2d(transflag);
                ss->updatematrix();
            }
            OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                       ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                       ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);
            setGLpixelMM(window->getpixelmodelmm());

            Entity *e=0;
            if(resizing || window->getscrolling() || state.dragging() ||
               rubberband.drawing || window->getcursoraction() != 0)
            {
                if(gldlist3 == 0)
                {
                    //qDebug() << "gl drawing making list";
                    if(glIsList(3) == GL_TRUE)
                        glDeleteLists(3,1);
                    glNewList(3,GL_COMPILE_AND_EXECUTE);
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                        {
                            if(e != app->mFlashEntity)
                            {
                                if(window->getscrolling() && reduceDetail)
                                    e->drawGL(DM_SCROLLING,NULL,window);
                                else
                                    e->drawGL(DM_NORMAL,NULL,window);
                            }
                        }
                    }
                    glEndList();
                    gldlist3 = 1;
                    //RCCheckGlError();
                }
                else
                {
                    //qDebug() << "gl drawing 2d window with list";
                    //fprintf(stderr,"gl drawing 2d view with list\n");
                    glCallList(3);
                    //resizing = false;
                    //RCCheckGlError();
                }
            }
            else
            {
                //qDebug() << "gl drawing 2d view with no list";
                //fprintf(stderr,"gl drawing 2d view with no list\n");
                if(glIsList(3) == GL_TRUE)
                    glDeleteLists(3,1);
                gldlist3 = 0;
                for (dblist.end() ; (e = dblist.last()) != 0 ; )
                {
                    if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                    {
                        //qDebug() << "view2dsurf drawing select list entity #" << secount;
                        //e->drawGL(DM_NORMAL,NULL,window);
                        if(e != app->mFlashEntity)
                        {
                            if(window->getscrolling() && reduceDetail)
                                e->drawGL(DM_SCROLLING,NULL,window);
                            else
                                e->drawGL(DM_NORMAL,NULL,window);
                        }
                    }
                }
            }
            if(app->mFlashEntity)
            {
                int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
            }
            EntityList list = state.getselection();
            if(resizing || window->getscrolling() ||
                    rubberband.drawing || window->getcursoraction() != 0)
            {
                if(gldlist8 == 0)
                {
                    //qDebug() << "gl drawing making list";
                    if(glIsList(8) == GL_TRUE)
                        glDeleteLists(8,1);
                    glNewList(8,GL_COMPILE);
                    for (list.end() ; (e = list.last()) != 0 ; )
                    {
                        if (e->isvisible(window) )
                        {
                            //qDebug() << "view2dsurf drawing select list entity #" << secount;
                            e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                        }
                    }
                    glEndList();
                    glCallList(8);
                    gldlist8 = 1;
                }
                else
                {
                    //qDebug() << "gl drawing selection with list";
                    glCallList(8);
                }
            }
            else
            {
                if(glIsList(8) == GL_TRUE)
                    glDeleteLists(8,1);
                gldlist8 = 0;
                for (list.end() ; (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(window) )
                    {
                        //qDebug() << "view2dsurf drawing select list entity #" << secount;
                        e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                    }
                }
            }

            //state.drawdraggerGL(DM_NORMAL,window);
            db.setlineweightdisplay(dbWeightdisplay);
            EndScene();
        }


#if 0
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this aand set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 0 /* dot grid */)
            db.grid.drawGL(DM_NORMAL ,window);
        //else
        //    if (cadwindow != 0)
        //        cadwindow->updatetitle();

        if (window->getsurfacetype() != View2dSurf && window->getrepaintstyle() == Wireframe )
            db.workplanes.drawGL(DM_NORMAL,window);

        // this rubberbad call hangs with multiple views???
        //QPoint cpos = QCursor::pos();
        //cpos = ((RCView*)parent())->viewport()->mapFromGlobal(cpos);
        //rubberband.mousemoveevent(window,cpos.x(),cpos.y(),MK_LBUTTON);

        // if using xor uncomment this code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glFinish();

        //glPopMatrix();

        EndScene();
#endif
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);

        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        //EndScene();

        painter->endNativePainting();
        painter->restore();

        fRate = fTimer.elapsed();
        lastfRate = fRate;
        //qDebug() << "frame rate" << fRate;
        //qDebug() << "last frame rate" << lastfRate;
        if(resizing || window->getscrolling() || state.dragging() ||
           rubberband.drawing || window->getcursoraction() != 0)
            setTimer(lastfRate < refreshDelay ? refreshDelay : lastfRate);
        else
            setTimer(0);

    }
    else
    {
        //qDebug() << "not drawing with the GL engine";
        //qInfo() << "not drawing with the GL engine";
        QGraphicsView::drawBackground(painter,rect);
    }

    // QGraphicsView::drawBackground(painter,rect);
}
#endif
#if 0 // dlist method #1
// using gl display lists
void RCGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    //qDebug() << "entering RCGraphicsView::drawBackground";

    if(cadwindow && cadwindow->getDrawingBatch() != 0)
        return;

    View3dWindow *window = ((RCView*)parent())->getView3dWindow();

    if(window == 0)
        return;

    //if(backImage == 0)
        painter->fillRect(rect,QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));
    //else
    //    window->getScene()->setBackgroundBrush(QBrush(*backImage));

    //if( resizing )
    //    return;

    //if(backImage == 0 && painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        //qDebug() << "painter is a GL engine";
        //qDebug() << "drawing with the GL engine";
        painter->save();
        painter->beginNativePainting();

        View *view;
        Entity *e=0;
        Point p1,p2;
        Point eye,pmin,pmax;
        EntityList dblist;
        int nlights=0;

        dblist = *db.geometry.getlist();

        view = window->getview();

        //qDebug() << "view name : " << QString(view->getname());

        //qDebug() << "app devicePixlRatio : " << app->devicePixelRatio();
        //qDebug() << "window devicePixlRatio : " << ((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio();

        //OpenOpenGl(((RCView*)parent())->view(),3,0,0,((RCView*)parent())->viewport()->width(),
        //                                             ((RCView*)parent())->viewport()->height());
        OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                   ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                   ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);
#if 1
        /*
        if (view->getperspective())
          eye = view->geteye();
        else
          eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();
        */
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

        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#endif
        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        setGLpixelMM(window->getpixelmodelmm());

        // these settings are for the grid and workplanes
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this as set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 1 /*line grid*/)
            db.grid.drawGL(DM_NORMAL ,window);
        //if (window->getsurfacetype() != View2dSurf )
        //    db.workplanes.drawGL(DM_NORMAL,window);


        //glPopMatrix();
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        EndScene();

        // switch off rendering and hidden line when selecting
        //qDebug() << "resizing    : " << resizing;
        //qDebug() << "cursorisdrawing    : " << window->cursorisdrawing();
        //qDebug() << "window->getscrolling(): " << window->getscrolling();
        //qDebug() << "state.dragging()      : " << state.dragging();
        //qDebug() << "state.getcommandid()      : " << state.getcommandid();
        //qDebug() << "rubberband.drawing    : " << rubberband.drawing;
        //qDebug() << "getcursoraction       : " << window->getcursoraction();
        //qDebug() << "db.getchanged()    : " << db.getchanged();
        //qDebug() << "selection length" << state.getselection().length();
        //qDebug() << "(rubberband.drawing && window->getcursoraction() == 0)" << (rubberband.drawing && window->getcursoraction() == 0);
        if( state.getselection().length() > 0 ||
             (rubberband.drawing && state.getcommandid() != 2101 && state.getcommandid() != 2102)
                /*&&
            (window->getrepaintstyle() == Hidden ||
             window->getrepaintstyle() == HiddenDashed ||
             window->getrepaintstyle() == ShadedHidden ||
             window->getrepaintstyle() == ShadedHiddenDashed ||
             window->getrepaintstyle() == ShadedHiddenTexture)*/)
            window->setrepaintstyle(Wireframe);

        if (window->getsurfacetype() == View3dSurf)
        {
            // ToDo: think about mixed shading modes??
            // Shading with OpenGL
            // only drawing shadable entities

            if(window->getrepaintstyle() == Shaded ||
               window->getrepaintstyle() == ShadedHidden ||
               window->getrepaintstyle() == ShadedHiddenDashed ||
               window->getrepaintstyle() == ShadedTexture ||
               window->getrepaintstyle() == ShadedHiddenTexture )
            {
                db.geometry.extents(window,&pmin,&pmax);
                //db.geometry.extents(0,&pmin,&pmax);

#if 1
                if (view->getperspective())
                  eye = view->geteye();
                else
                  eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

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

                SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
#endif
                QtBeginScene();
                BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

                glEnable(GL_LIGHTING);
                glEnable(GL_DEPTH_TEST);
                glClear( GL_DEPTH_BUFFER_BIT );

                //dblist = *db.geometry.getlist();
                Entity *e=0;
                for (dblist.end() ; (e = dblist.last()) != NULL ; )
                //for (dblist.start() ; (e = dblist.next()) != NULL ; )
                {
                  if (e->isa(light_entity) && e->isvisible(window))
                  {
                       double red,green,blue;
                       if (((Light *)e)->getoptions().test(1))
                         {  red = green = blue = 1.0;
                         }
                       else
                         {  red = db.colourtable[e->getcolour()].red/255.0;
                            green = db.colourtable[e->getcolour()].green/255.0;
                            blue = db.colourtable[e->getcolour()].blue/255.0;
                         }

                       SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                       nlights++;
                  }
                }

                if (nlights == 0)
                    SetLight(eye.x,eye.y,eye.z);
                /*
                    if(view->getperspective())
                        SetLight(view->geteye().x,view->geteye().y,view->geteye().z);
                    else
                    {
                        eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();
                        SetLight(eye.x,eye.y,eye.z);
                    }
                */

                if(resizing || window->getscrolling() || state.dragging() ||
                   (rubberband.drawing && window->getcursoraction() != 0))
                   // rubberband.drawing || window->getcursoraction() != 0)
                {
                    if(gldlist5 == 0)
                    {
                        if(glIsList(5) == GL_TRUE)
                            glDeleteLists(5,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(5,GL_COMPILE);
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "e type : " << e->type();
                            if (e->issurface() && e->isvisible(window) )
                            {
                                //qDebug() << "gl shading 3d window with no list";
                                if( e->type() == plane_entity        ||
                                    e->type() == ruledsurface_entity ||
                                    e->type() == surfaceofrev_entity ||
                                    e->type() == patch_entity  ||
                                    e->type() == figure_entity ||
                                    e->type() == group_entity )
                                {
                                    //if(window->getoffscreenbitmap())
                                    //{
                                    //    e->drawGL(DM_NORMAL,NULL,window->getoffscreenbitmap());
                                    //}
                                    //else
                                        e->drawGL(DM_NORMAL,NULL,window);
                                 }
                            }
                            //if (program->getinterrupt())
                            //    break;
                        }
                        glEndList();
                        glCallList(5);
                        //resizing=false;
                        gldlist5 = 1;

                    }
                    else
                    {
                        glCallList(5);
                    }
                }
                else
                {
                    if(glIsList(5) == GL_TRUE)
                        glDeleteLists(5,1);
                    gldlist5 = 0;
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        //qDebug() << "e type : " << e->type();
                        if (e->issurface() && e->isvisible(window) )
                        {
                            //qDebug() << "gl shading 3d window with no list";
                            if( e->type() == plane_entity        ||
                                e->type() == ruledsurface_entity ||
                                e->type() == surfaceofrev_entity ||
                                e->type() == patch_entity  ||
                                e->type() == figure_entity ||
                                e->type() == group_entity )
                            {
                                //if(window->getoffscreenbitmap())
                                //{
                                //    e->drawGL(DM_NORMAL,NULL,window->getoffscreenbitmap());
                                //}
                                //else
                                    e->drawGL(DM_NORMAL,NULL,window);
                             }
                        }
                        //if (program->getinterrupt())
                        //    break;
                    }
                }


                if( (window->getrepaintstyle() != Shaded && window->getrepaintstyle() != ShadedTexture))
                {
                if( resizing ||
                    (window->getscrolling() && window->getscrolling() !=2) ||
                    state.dragging() ||
                    rubberband.drawing ||
                    window->getcursoraction() != 0 )
                {
                    if(gldlist4 == 0)
                    {
                        //qDebug() << "gl hidden line drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(4) == GL_TRUE)
                            glDeleteLists(4,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(4,GL_COMPILE);

                        HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                        image.hideGL();

                        glEndList();
                        glCallList(4);
                        //resizing=false;
                        gldlist4 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing hidden line with list";
                        glCallList(4);
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing hidden line no list";
                    gldlist4 = 0;
                    if(glIsList(4) == GL_TRUE)
                        glDeleteLists(4,1);
                    HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                    image.hideGL();

                }
                }
                //EndScene();
                QtEndScene();
            }
            else if(window->getrepaintstyle() == Hidden ||
                    window->getrepaintstyle() == HiddenDashed )
            {                
                if( resizing ||
                    (window->getscrolling() && window->getscrolling() !=2) ||
                    state.dragging() ||
                    rubberband.drawing ||
                    window->getcursoraction() != 0)
                {
                    if(gldlist4 == 0)
                    {
                        //qDebug() << "gl hidden line drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(4) == GL_TRUE)
                            glDeleteLists(4,1);
                        //glNewList(4,GL_COMPILE_AND_EXECUTE);
                        glNewList(4,GL_COMPILE);

                        HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                        image.hideGL();

                        glEndList();
                        glCallList(4);
                        //resizing=false;
                        gldlist4 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing hidden line with list";
                        glCallList(4);
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing hidden line no list";
                    gldlist4 = 0;
                    if(glIsList(4) == GL_TRUE)
                        glDeleteLists(4,1);
                    HideImage image(window,1+v.getinteger("hd::dashhiddenlines") * 2);
                    image.hideGL();

                }
            }
            else if(window->getrepaintstyle() == Wireframe)
            {
                //qDebug() << "starting draw";
                //qDebug() << "scrolling is :" << window->getscrolling();
                // drawing wireframe entites with OpenGL
                Entity *e=0;
                int ecount=1;
                //qDebug() << "resizing    : " << resizing;
                //qDebug() << "cursorisdrawing    : " << window->cursorisdrawing();
                //qDebug() << "window->getscrolling(): " << window->getscrolling();
                //qDebug() << "state.dragging()      : " << state.dragging();
                //qDebug() << "rubberband.drawing    : " << rubberband.drawing;
                //qDebug() << "getcursoraction       : " << window->getcursoraction();
                //qDebug() << "db.getchanged()    : " << db.getchanged();
                if(resizing || window->getscrolling() || state.dragging() ||
                   //(rubberband.drawing && window->getcursoraction() != 0))
                   rubberband.drawing || window->getcursoraction() != 0)
                //if(1)
                {
                    if(gldlist1 == 0)
                    {
                        //qDebug() << "gl drawing making list";
                        //gldlist = glGenLists(1);
                        //qDebug() << "gldlist" << gldlist;
                        if(glIsList(1) == GL_TRUE)
                            glDeleteLists(1,1);
                        //glNewList(1,GL_COMPILE_AND_EXECUTE);
                        glNewList(1,GL_COMPILE);
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "entity type : " << e->type();
                            //qDebug() << "entity qgi  : " << e->qgi;
                            //if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                            if (e->isvisible(window) /*&& !e->getselected()*/ /* && !state.getselection().inlist(e)*/)
                            {
                                if(e != app->mFlashEntity)
                                    if(window->getscrolling() /* && e->type() == plane_entity */)
                                        e->drawGL(DM_SCROLLING,NULL,window);
                                    else
                                        e->drawGL(DM_NORMAL,NULL,window);
                                    //else
                                    //    e->drawGL(DM_SCROLLING,NULL,window);
                            }
                            //if (program->getinterrupt())
                            //    break;
                        }
                        glEndList();
                        glCallList(1);
                        //resizing=false;
                        gldlist1 = 1;
                    }
                    else
                    {
                        //qDebug() << "gl drawing 3d window with list";
                        glCallList(1);
                        //resizing = false;
                        //fprintf(stderr,"gl drawing 3d window with list\n");
                    }
                }
                else
                {
                    //qDebug() << "gl drawing 3d window with no list";
                    if(glIsList(1) == GL_TRUE)
                        glDeleteLists(1,1);
                    gldlist1 = 0;
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        //qDebug() << "entity type : " << e->type();
                        //qDebug() << "entity qgi  : " << e->qgi;
                        //if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                        if (e->isvisible(window) /*&& !e->getselected()*/ /* && !state.getselection().inlist(e)*/)
                        {
                            if(e != app->mFlashEntity)
                               e->drawGL(DM_NORMAL,NULL,window);
                            ecount++;
                        }
                        //if (program->getinterrupt())
                        //    break;
                    }
                }

                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
                }
                EntityList list = state.getselection();
                int secount=0;
                //glEnable(GL_POINT_SMOOTH);
                //glDrawBuffer(GL_FRONT);
                //glPushAttrib(GL_ALL_ATTRIB_BITS);
                //glEnable(GL_COLOR_LOGIC_OP);
                //glLogicOp(GL_XOR);
                //glLogicOp(GL_INVERT);
                for (list.end() ; (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(window) )
                    {
                        //qDebug() << "view3dsurf drawing select list entity #" << secount;
                        e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                    }
                    secount++;
                }
                state.drawdraggerGL(DM_NORMAL,window);
                //glDisable(GL_COLOR_LOGIC_OP);
                //glPopAttrib();
                //glDrawBuffer(GL_FRONT);
            }
            /*
            if(app->mFlashEntity)
            {
                int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
            }
            EntityList list = state.getselection();
            int secount=0;
            for (list.end() ; (e = list.last()) != 0 ; )
            {
                if (e->isvisible(window) )
                {
                    //qDebug() << "view3dsurf drawing select list entity #" << secount;
                    e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                }
                secount++;
            }
            state.drawdraggerGL(DM_NORMAL,window);
            */
        }

        // this is a reproduction of what View2dpaintevent does but using open gl
        // should this be done as a View2dWindow::paintevenGL kind of function??
        // and View3dWindow::paintevent also??
        else if (window->getsurfacetype() == View2dSurf)
        {
            // force the line weight display on for layout space
            int dbWeightdisplay = db.getlineweightdisplay() ;
            db.setlineweightdisplay(1);
            //qDebug() << "View2dSurf drawing";
            // draw the borders of the View2dWindow (the composed view)
            Point p1,p2;
            window->setupGL(1.0,0.0,DM_DASH,0,0,0,0);
            RCCOLORREF pcol = window->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = window->curPen.widthF();
            glLineWidth(window->curGLWeight);

            p1.setxyz(0.0,0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,0.0,0.0);
            window->line3dGL(p1,p2);

            //window->setupGL(1.0,0.0,DM_GRID,0,0,0,0);
            View3dSubSurface *ss;
            // draw the sub window borders first
            int numsub=0;
            for (((View2dWindow*)window)->subsurfaces.start(),numsub=0 ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ;numsub++ )
                ss->drawborderGL(DM_DASH);
            // now draw the sub windows contents
            int dlc=0;
            for (((View2dWindow*)window)->subsurfaces.start(),dlc=0 ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ; dlc++ )
            {
                Point p1,p2,p3,p4,o,xaxis,yaxis;

                ss->setblackandwhite(window->getblackandwhite());
                ss->setdisplaylayertable(ss->getoneview()->getlayertable());

                ss->setscale( (ss->getumax() - ss->getumin()) / ss->getwidthmm());
                o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                p1 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p2 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p3 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
                p4 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

                // we have a pointer to the view not a copy
                // anything we change here must be restored later
                // save the transform flag because rubberbanding needs it
                //ss->setplotscale(1.0);
                int transflag = ss->gettransform2d();
                setGLpixelMM(ss->getpixelmodelmm());
                // clear the transform flag an reset the matrix for Gl drawing
                ss->cleartransform();
                ss->updatematrix();

                OpenOpenGl(((RCView*)parent())->view(),0,p1.x*DEVICEPIXELRATIO ,p1.y*DEVICEPIXELRATIO ,
                           ss->getwidth()*DEVICEPIXELRATIO,
                           ss->getheight()*DEVICEPIXELRATIO);

                View *view = ss->getview();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(ss->getumin(),ss->getvmin(),ss->getumax(),ss->getvmax());

                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

                BeginScene(BS_IGNOREDEPTH | BS_NOCLEAR/*BS_FRONTBUFFERONLY | BS_IGNOREDEPTH | BS_NOCLEAR*/);//  Allow the clear for now | BS_NOCLEAR);
                SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

                if(window->getrepaintstyle() == Wireframe)
                {
                    Entity *e=0;
                    if(resizing || window->getscrolling() || state.dragging() ||
                       rubberband.drawing || window->getcursoraction() != 0 )
                    {
                        if(gldlist2 < numsub)
                        {
                            //qDebug() << "gl drawing making sub list";
                            //qDebug() << "view number : " << ss->getview()->getviewno()+4;
                            if(glIsList(dlc+10) == GL_TRUE)
                                glDeleteLists(dlc+10,1);
                            glNewList(dlc+10,GL_COMPILE_AND_EXECUTE);
                            for (dblist.end() ; (e = dblist.last()) != 0 ; )
                            {
                                //qDebug() << "subview ent type : " << e->type();
                                if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                                {
                                    //e->drawGL(DM_NORMAL,0,ss);
                                    if(e != app->mFlashEntity)
                                        e->drawGL(DM_NORMAL,NULL,ss);
                                }
                            }
                            glEndList();
                            gldlist2++;
                            //RCCheckGlError();
                        }
                        else
                        {
                            //qDebug() << "gl drawing sub window with list";
                            //fprintf(stderr,"gl drawing sub window with list\n");
                            glCallList(dlc+10);
                            //RCCheckGlError();
                        }
                    }
                    else
                    {
                        //qDebug() << "gl drawing sub window with no list";
                        //fprintf(stderr,"gl drawing sub window with no list\n");
                        if(glIsList(dlc+10) == GL_TRUE)
                            glDeleteLists(dlc+10,1);
                        gldlist2 = 0;
                        for (dblist.end() ; (e = dblist.last()) != 0 ; )
                        {
                            //qDebug() << "subview ent type : " << e->type();
                            if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                            {
                                //e->drawGL(DM_NORMAL,0,ss);
                                if(e != app->mFlashEntity)
                                    e->drawGL(DM_NORMAL,NULL,ss);
                            }
                        }
                    }
                }
                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,ss);
                }
                EntityList list = state.getselection();
                for (list.end() ; (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(ss) )
                    {
                        //qDebug() << "subview drawing select list entity #" << secount;
                        e->drawGL(DM_SELECT,&state.glDraggerTform,ss);
                    }
                }
                state.drawdraggerGL(DM_NORMAL,ss);
                EndScene();
                //restore the transform flag for rubberbanding
                ss->settransform2d(transflag);
                ss->updatematrix();
            }
            OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                       ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                       ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);
            setGLpixelMM(window->getpixelmodelmm());

            Entity *e=0;
            if(resizing || window->getscrolling() || state.dragging() ||
               rubberband.drawing || window->getcursoraction() != 0)
            {
                if(gldlist3 == 0)
                {
                    //qDebug() << "gl drawing making list";
                    if(glIsList(3) == GL_TRUE)
                        glDeleteLists(3,1);
                    glNewList(3,GL_COMPILE_AND_EXECUTE);
                    for (dblist.end() ; (e = dblist.last()) != 0 ; )
                    {
                        if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                        {
                            //qDebug() << "view2dsurf drawing select list entity #" << secount;
                            //e->drawGL(DM_NORMAL,NULL,window);
                            if(e != app->mFlashEntity)
                                e->drawGL(DM_NORMAL,NULL,window);
                        }
                    }
                    glEndList();
                    gldlist3 = 1;
                    //RCCheckGlError();
                }
                else
                {
                    //qDebug() << "gl drawing 2d window with list";
                    //fprintf(stderr,"gl drawing 2d view with list\n");
                    glCallList(3);
                    //resizing = false;
                    //RCCheckGlError();
                }
            }
            else
            {
                //qDebug() << "gl drawing 2d view with no list";
                //fprintf(stderr,"gl drawing 2d view with no list\n");
                if(glIsList(3) == GL_TRUE)
                    glDeleteLists(3,1);
                gldlist3 = 0;
                for (dblist.end() ; (e = dblist.last()) != 0 ; )
                {
                    if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                    {
                        //qDebug() << "view2dsurf drawing select list entity #" << secount;
                        //e->drawGL(DM_NORMAL,NULL,window);
                        if(e != app->mFlashEntity)
                            e->drawGL(DM_NORMAL,NULL,window);
                    }
                }
            }
            if(app->mFlashEntity)
            {
                int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
            }
            EntityList list = state.getselection();
            for (list.end() ; (e = list.last()) != 0 ; )
            {
                if (e->isvisible(window) )
                {
                    //qDebug() << "view2dsurf drawing select list entity #" << secount;
                    e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                }
            }
            state.drawdraggerGL(DM_NORMAL,window);
            db.setlineweightdisplay(dbWeightdisplay);
            EndScene();
        }


#if 0
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this aand set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 0 /* dot grid */)
            db.grid.drawGL(DM_NORMAL ,window);
        //else
        //    if (cadwindow != 0)
        //        cadwindow->updatetitle();

        if (window->getsurfacetype() != View2dSurf && window->getrepaintstyle() == Wireframe )
            db.workplanes.drawGL(DM_NORMAL,window);

        // this rubberbad call hangs with multiple views???
        //QPoint cpos = QCursor::pos();
        //cpos = ((RCView*)parent())->viewport()->mapFromGlobal(cpos);
        //rubberband.mousemoveevent(window,cpos.x(),cpos.y(),MK_LBUTTON);

        // if using xor uncomment this code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glFinish();

        //glPopMatrix();

        EndScene();
#endif
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);

        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        //EndScene();

        painter->endNativePainting();
        painter->restore();
    }
    else
        //qDebug() << "not drawing with the GL engine";

// QGraphicsView::drawBackground(painter,rect);
}
#endif // dlist method #1
#else
// using gl direct mode
void RCGraphicsView::drawBackground(QPainter * painter, const QRectF & rect)
{
    //qDebug() << "entering RCGraphicsView::drawBackground";
#if 1
    //setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
    //                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
    //                                ::v.getreal("wn::backgroundcolour.blue")*255.0)));
    //QGraphicsView::drawBackground(painter,rect);
    //scene()->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
     //                                         ::v.getreal("wn::backgroundcolour.green")*255.0,
     //                                         ::v.getreal("wn::backgroundcolour.blue")*255.0)));
    //painter->fillRect(rect,backgroundBrush());

    if(cadwindow && cadwindow->getDrawingBatch() != 0)
        return;

    View3dWindow *window = ((RCView*)parent())->getView3dWindow();

    if(window == 0)
        return;

    //if(window->cursorisdrawing())
    //    return;

    painter->fillRect(rect,QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                         int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));

    if(painter->paintEngine()->type() == QPaintEngine::OpenGL2)
    {
        //qDebug() << "painter is a GL engine";
        painter->save();
        painter->beginNativePainting();

        View *view;
        Entity *e=0;
        Point p1,p2;
        Point eye,pmin,pmax;
        EntityList dblist;
        int nlights=0;

        dblist = *db.geometry.getlist();

        view = window->getview();

        //qDebug() << "view name : " << QString(view->getname());

        //qDebug() << "app devicePixlRatio : " << app->devicePixelRatio();
        //qDebug() << "window devicePixlRatio : " << ((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio();

        //OpenOpenGl(((RCView*)parent())->view(),3,0,0,((RCView*)parent())->viewport()->width(),
        //                                             ((RCView*)parent())->viewport()->height());
        OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                   ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                   ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

        SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
        SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
        BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

        setGLpixelMM(window->getpixelmodelmm());

        // these settings are for the grid and workplanes
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this as set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 1 /*line grid*/)
            db.grid.drawGL(DM_NORMAL ,window);
        //if (window->getsurfacetype() != View2dSurf )
        //    db.workplanes.drawGL(DM_NORMAL,window);


        //glPopMatrix();
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        EndScene();

        if (window->getsurfacetype() == View3dSurf)
        {                
            // ToDo: think about mixed shading modes??
            // Shading with OpenGL
            // only drawing shadable entities

            if(window->getrepaintstyle() == Shaded )
            {
                db.geometry.extents(window,&pmin,&pmax);
                //db.geometry.extents(0,&pmin,&pmax);

                if (view->getperspective())
                  eye = view->geteye();
                else
                  eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

                if (view->getperspective())
                  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
                else
                  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

                QtBeginScene();
                BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

                glEnable(GL_LIGHTING);
                glEnable(GL_DEPTH_TEST);
                glClear( GL_DEPTH_BUFFER_BIT );

                //dblist = *db.geometry.getlist();
                Entity *e=0;
                for (dblist.end() ; (e = dblist.last()) != NULL ; )
                //for (dblist.start() ; (e = dblist.next()) != NULL ; )
                {
                  if (e->isa(light_entity) && e->isvisible(window))
                  {
                       double red,green,blue;
                       if (((Light *)e)->getoptions().test(1))
                         {  red = green = blue = 1.0;
                         }
                       else
                         {  red = db.colourtable[e->getcolour()].red/255.0;
                            green = db.colourtable[e->getcolour()].green/255.0;
                            blue = db.colourtable[e->getcolour()].blue/255.0;
                         }

                       SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                       nlights++;
                  }
                }

                if (nlights == 0)
                    SetLight(eye.x,eye.y,eye.z);
                    //SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);


                //EntityList dblist;
                //dblist = *db.geometry.getlist();

                for (dblist.end() ; (e = dblist.last()) != 0 ; )
                {
                    //qDebug() << "e type : " << e->type();
                    if (e->issurface() && e->isvisible(window) )
                    {

                        if( e->type() == plane_entity        ||
                            e->type() == ruledsurface_entity ||
                            e->type() == surfaceofrev_entity ||
                            e->type() == patch_entity  ||
                            e->type() == figure_entity ||
                            e->type() == group_entity
                                )
                        {
                            if(window->getoffscreenbitmap())
                            {
                                e->drawGL(DM_NORMAL,NULL,window->getoffscreenbitmap());
                            }
                            else
                                e->drawGL(DM_NORMAL,NULL,window);
                         }

                    }
                    //if (program->getinterrupt())
                    //    break;
                }
                //EndScene();
                QtEndScene();
            }
            else if(window->getrepaintstyle() == Wireframe)
            {
                //qDebug() << "starting draw";
                //qDebug() << "scrolling is :" << window->getscrolling();
                // drawing wireframe entites with OpenGL
                //EntityList dblist;
                //dblist = *db.geometry.getlist();
                Entity *e=0;
                int ecount=1;
                    //qDebug() << "gl drawing 3d window with no list";
                    gldlist1 = 0;
                for (dblist.end() ; (e = dblist.last()) != 0 ; )
                {
                    //qDebug() << "entity type : " << e->type();
                    //qDebug() << "entity qgi  : " << e->qgi;
                    //if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                    if (e->isvisible(window) /*&& !e->getselected()*/ /* && !state.getselection().inlist(e)*/)
                    {
                        if(e != app->mFlashEntity)
                           e->drawGL(DM_NORMAL,NULL,window);
                        ecount++;
                    }
                    //if (program->getinterrupt())
                    //    break;
                }
                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
                }
                EntityList list = state.getselection();
                int secount=0;
                for (list.end() ; (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(window) )
                    {
                        //qDebug() << "view3dsurf drawing select list entity #" << secount;
                        e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                    }
                    secount++;
                }
                state.drawdraggerGL(DM_NORMAL,window);
            }
        }

        // this is a reproduction of what View2dpaintevent does but using open gl
        // should this be done as a View2dWindow::paintevenGL kind of function??
        // and View3dWindow::paintevent also??
        else if (window->getsurfacetype() == View2dSurf)
        {
            //qDebug() << "View2dSurf drawing";
            // draw the borders of the View2dWindow (the composed view)
            Point p1,p2;
            window->setupGL(1.0,0.0,DM_DASH,0,0,0,0);
            RCCOLORREF pcol = window->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = window->curPen.widthF();
            glLineWidth(window->curGLWeight);

            p1.setxyz(0.0,0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),0.0,0.0);
            p2.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(((View2dWindow*)window)->getview2d()->getpaperwidth(),((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            window->line3dGL(p1,p2);

            p1.setxyz(0.0,((View2dWindow*)window)->getview2d()->getpaperheight(),0.0);
            p2.setxyz(0.0,0.0,0.0);
            window->line3dGL(p1,p2);

            //window->setupGL(1.0,0.0,DM_GRID,0,0,0,0);
            View3dSubSurface *ss;
            // draw the sub window borders first
            for (((View2dWindow*)window)->subsurfaces.start() ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ; )
                ss->drawborderGL(DM_DASH);
            // now draw the sub windows contents
            int dlc=0;
            for (((View2dWindow*)window)->subsurfaces.start(),dlc=0 ; (ss = (View3dSubSurface *) ((View2dWindow*)window)->subsurfaces.next()) != NULL ; dlc++ )
            {
                Point p1,p2,p3,p4,o,xaxis,yaxis;

                ss->setblackandwhite(window->getblackandwhite());
                ss->setdisplaylayertable(ss->getoneview()->getlayertable());

                ss->setscale( (ss->getumax() - ss->getumin()) / ss->getwidthmm());
                o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                p1 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p2 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmin() / ss->getoneview()->getscale());
                p3 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumax() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());
                p4 = ss->getparent()->modeltoscreen(o + xaxis * ss->getoneview()->getumin() / ss->getoneview()->getscale() + yaxis * ss->getoneview()->getvmax() / ss->getoneview()->getscale());

                // we have a pointer to the view not a copy
                // anything we change here must be restored later
                // save the transform flag because rubberbanding needs it
                int transflag = ss->gettransform2d();
                setGLpixelMM(ss->getpixelmodelmm());
                // clear the transform flag an reset the matrix for Gl drawing
                ss->cleartransform();
                ss->updatematrix();
                OpenOpenGl(((RCView*)parent())->view(),0,p1.x*DEVICEPIXELRATIO ,p1.y*DEVICEPIXELRATIO ,
                           ss->getwidth()*DEVICEPIXELRATIO,
                           ss->getheight()*DEVICEPIXELRATIO);

                View *view = ss->getview();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(ss->getumin(),ss->getvmin(),ss->getumax(),ss->getvmax());

                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

                BeginScene(BS_IGNOREDEPTH | BS_NOCLEAR/*BS_FRONTBUFFERONLY | BS_IGNOREDEPTH | BS_NOCLEAR*/);//  Allow the clear for now | BS_NOCLEAR);
                SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

                Entity *e=0;
                //qDebug() << "gl drawing sub window with no list";
                //fprintf(stderr,"gl drawing sub window with no list\n");
                gldlist2 = 0;
                for (dblist.end() ; (e = dblist.last()) != 0 ; )
                {
                    //qDebug() << "subview ent type : " << e->type();
                    if (e->isvisible(ss) && (e->isa(group_entity) == 0 || e->getselected()))
                    {
                        //e->drawGL(DM_NORMAL,0,ss);
                        if(e != app->mFlashEntity)
                            e->drawGL(DM_NORMAL,NULL,ss);
                    }
                }

                if(app->mFlashEntity)
                {
                    int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                    app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,ss);
                }
                EntityList list = state.getselection();
                for (list.end() ; (e = list.last()) != 0 ; )
                {
                    if (e->isvisible(ss) )
                    {
                        //qDebug() << "subview drawing select list entity #" << secount;
                        e->drawGL(DM_SELECT,&state.glDraggerTform,ss);
                    }
                }
                state.drawdraggerGL(DM_NORMAL,ss);
                EndScene();
                //restore the transform flag for rubberbanding
                ss->settransform2d(transflag);
                ss->updatematrix();
            }
            OpenOpenGl(((RCView*)parent())->view(),3,0,0,
                       ((RCView*)parent())->viewport()->width()*DEVICEPIXELRATIO,
                       ((RCView*)parent())->viewport()->height()*DEVICEPIXELRATIO);

            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);
            setGLpixelMM(window->getpixelmodelmm());

            Entity *e=0;
            //qDebug() << "gl drawing 2d view with no list";
            //fprintf(stderr,"gl drawing 2d view with no list\n");
            gldlist3 = 0;
            for (dblist.end() ; (e = dblist.last()) != 0 ; )
            {
                if (e->isvisible(window) && (e->isa(group_entity) == 0 || e->getselected()))
                {
                    //qDebug() << "view2dsurf drawing select list entity #" << secount;
                    //e->drawGL(DM_NORMAL,NULL,window);
                    if(e != app->mFlashEntity)
                        e->drawGL(DM_NORMAL,NULL,window);
                }
            }
            if(app->mFlashEntity)
            {
                int tm = (QDateTime::currentMSecsSinceEpoch() / 350) % 3;
                app->mFlashEntity->drawGL(tm == 0 ? DM_FLASH1 : tm == 1 ? DM_FLASH2 : DM_FLASH3,NULL,window);
            }
            EntityList list = state.getselection();
            for (list.end() ; (e = list.last()) != 0 ; )
            {
                if (e->isvisible(window) )
                {
                    //qDebug() << "view2dsurf drawing select list entity #" << secount;
                    e->drawGL(DM_SELECT,&state.glDraggerTform,window);
                }
            }
            state.drawdraggerGL(DM_NORMAL,window);
            EndScene();
        }


#if 0
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glClear( GL_DEPTH_BUFFER_BIT );
        glFinish();

        // if we want to use xor uncomment this aand set the pen colour to black
        //glEnable(GL_BLEND);
        //glEnable(GL_COLOR_LOGIC_OP);
        //glLogicOp(GL_XOR);

        // drawing and color setting is done in this function
        if(db.grid.getdisplay() && db.grid.getStyle() == 0 /* dot grid */)
            db.grid.drawGL(DM_NORMAL ,window);
        //else
        //    if (cadwindow != 0)
        //        cadwindow->updatetitle();

        if (window->getsurfacetype() != View2dSurf && window->getrepaintstyle() == Wireframe )
            db.workplanes.drawGL(DM_NORMAL,window);

        // this rubberbad call hangs with multiple views???
        //QPoint cpos = QCursor::pos();
        //cpos = ((RCView*)parent())->viewport()->mapFromGlobal(cpos);
        //rubberband.mousemoveevent(window,cpos.x(),cpos.y(),MK_LBUTTON);

        // if using xor uncomment this code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glFinish();

        //glPopMatrix();

        EndScene();
#endif
        // if using xor uncomment is code
        //glDisable(GL_BLEND);
        //glDisable(GL_COLOR_LOGIC_OP);

        //glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        //glFinish();

        //EndScene();

        painter->endNativePainting();
        painter->restore();
    }
    //else
    //    qDebug() << "painter is NOT a GL engine";

#else
    QGraphicsView::drawBackground(painter,rect);
#endif

}
#endif

static int rcgvcount=0;
RCGraphicsView::RCGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    mcount = ++rcgvcount;
    qDebug() << "in RCGraphicsView CTOR1 : " << mcount;
    // enable drag and drop
    setAcceptDrops(false);
    gldlist1=0;
    gldlist2=0;
    gldlist3=0;
    gldlist4=0;
    gldlist5=0;
    gldlist6=0;
    gldlist7=0;
    gldlist8=0;
    resizing = false;
    backImage = 0;
    fRate = 0;
    lastfRate = 0;
    refreshDelay = v.getinteger("wn::screenrefreshdelay");
    reduceDetail = v.getinteger("wn::reducedrawdetail");

    refreshTimer = new QTimer(this);
    refreshTimer->setSingleShot(true);
    refreshTimer->setInterval(refreshDelay);
    QObject::connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()),(Qt::ConnectionType)(Qt::AutoConnection|Qt::UniqueConnection));
    refreshTimer->start();
}

RCGraphicsView::RCGraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene, parent)
{
    mcount= ++rcgvcount;
    qDebug() << "in RCGraphicsView CTOR2 : " << mcount;
    // enable drag and drop
    setAcceptDrops(false);
    gldlist1=0;
    gldlist2=0;
    gldlist3=0;
    gldlist4=0;
    gldlist5=0;
    gldlist6=0;
    gldlist7=0;
    gldlist8=0;
    resizing = false;
    backImage = 0;
    fRate = 0;
    lastfRate = 0;
    refreshDelay = v.getinteger("wn::screenrefreshdelay");
    reduceDetail = v.getinteger("wn::reducedrawdetail");

    refreshTimer = new QTimer(this);
    refreshTimer->setSingleShot(true);
    refreshTimer->setInterval(refreshDelay);
    QObject::connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()),(Qt::ConnectionType)(Qt::AutoConnection|Qt::UniqueConnection));
    refreshTimer->start();

}

RCGraphicsView::~RCGraphicsView()
{
    qDebug() << "in RCGraphicsView DTOR : " << mcount;
    if(scene())
        qDebug() << "scene count : " << ((RCGraphicsScene*)scene())->mcount;
    if(backImage != 0)
    {
        delete backImage;
        backImage = 0;
    }
    if(refreshTimer !=0 )
    {
        disconnect(refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()));
        refreshTimer->deleteLater();
    }

    for(int i=0; i<8; i++)
    {
        if(glIsList(i) == GL_TRUE)
            glDeleteLists(i,1);
    }
    gldlist1=0;
    gldlist2=0;
    gldlist3=0;
    gldlist4=0;
    gldlist5=0;
    gldlist6=0;
    gldlist7=0;
    gldlist8=0;

    // KMJ: ToDo fix this function
    //CloseOpenGl((HDC)this);
}

/*
void RCGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void RCGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void RCGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void RCGraphicsView::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    qDebug() << "in RCGraphicsView::dropEvent";

    // only looking for files at the moment
    if (mimeData->hasUrls())
    {
        //qDebug() << "a url was droped";

        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();
        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }
        if(pathList.length() > 0)
        {
            QString filepath = pathList.at(0);
            if(filepath.toLower().right(4) == ".cad")
            {
                ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
                if(filechanged(rs2.gets(),rs3.gets()))
                {
                    db.initialize();
                    file_open(0,filepath.data());
                    event->acceptProposedAction();
                }
            }
            else if(filepath.toLower().right(4) == ".dwg")
            {
                open_dwg(filepath.data());
                event->acceptProposedAction();
            }
            else if(filepath.toLower().right(4) == ".dxf")
            {
                open_dxf(filepath.data());
                event->acceptProposedAction();
            }
            //else if(filepath.toLower().right(4) == ".igs")
           // {
            //    open_iges(filepath.data());
            //}
        }
    }
#if 0
    else if (mimeData->hasImage())
    {
        //qDebug() << "image data was droped";
        //setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    }
    else if (mimeData->hasHtml())
    {
        qDebug() << "rich text data was droped";
        qDebug() << mimeData->html();
        //setText(mimeData->html());
        //setTextFormat(Qt::RichText);
    }
    else if (mimeData->hasText())
    {
        qDebug() << "plain text data was droped";
        qDebug() << mimeData->text();
        foreach(QString format, mimeData->formats())
        {
            qDebug() << format;
            if(format == "text/uri-list")
            {
                // assume a file was dropped if it is a .CAD file open it
                // only look for the first cad file
                QList<QUrl> urlList = mimeData->urls();
                for(int i=0; i <urlList.size(); i++)
                {
                    qDebug() << urlList[i].toString() + "\r";
                    QString uri = urlList[i].toString();
                    if(uri.contains("file://") && uri.toLower().right(4) == ".cad")
                    {
#ifdef _MAC
                        uri = uri.mid(7);
#else
                        uri = uri.mid(7);
                        if(uri.at(0) == "/")
                            uri = uri.mid(1);
#endif
                        ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
                        if(!filechanged(rs2.gets(),rs3.gets()))
                            break;
                        db.initialize();
                    }
                    file_open(0,uri.data());
                }
            }
        }
        //setText(mimeData->text());
        //setTextFormat(Qt::PlainText);
    }
    else if (mimeData->hasUrls())
    {
        qDebug() << "a url) was droped";
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
            qDebug() << text;
        }
        //setText(text);
    }
    else
    {
        qDebug() << "Cannot display data";
        //setText(tr("Cannot display data"));
    }
#endif

    //event->ignore();
    //event->acceptProposedAction();
}
*/
void RCGraphicsView::mousePressEvent(QMouseEvent *event)
{
    resizing = false;
    QGraphicsView::mousePressEvent(event);
}

void RCGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    resizing = false;
    QGraphicsView::mouseMoveEvent(event);
}

void RCGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    resizing = false;
    QGraphicsView::mousePressEvent(event);
}

void RCGraphicsView::wheelEvent(QWheelEvent *e)
{

    QGraphicsView::wheelEvent(e);
}



void RCGraphicsView::resizeEvent(QResizeEvent *event)
{
    //QGraphicsView::resizeEvent(event);
#if 0
    qDebug() << "RCGraphicsView::resizeEvent";
    qDebug() << "parent : " << (RCView*)parent();
    if(((RCView*)parent())->getView3dWindow() != 0)
    {
        if(((RCView*)parent())->getView3dWindow()->getview() != 0)
        {
            qDebug() << "View3d name      : " << QString(((RCView*)parent())->getView3dWindow()->getview()->getname());
            qDebug() << "View3D width     : " << ((RCView*)parent())->getView3dWindow()->getwidth();
            qDebug() << "View3D height    : " << ((RCView*)parent())->getView3dWindow()->getheight();
        }
    }
    qDebug() << "View3D vp width  : " << ((RCView*)parent())->viewport()->width();
    qDebug() << "View3D vp height : " << ((RCView*)parent())->viewport()->height();
    qDebug() << "this width       : " << width();
    qDebug() << "this height      : " << height();
    qDebug() << "event old width  : " << event->oldSize().width();
    qDebug() << "event old height : " << event->oldSize().height();
    qDebug() << "event new width  : " << event->size().width();
    qDebug() << "event new height : " << event->size().height();
#endif
#if 0
    if(cadwindow && ((QtMainWindow*)cadwindow->gethwnd())->isFullScreen())
        qDebug() << "main window is full screen";
    else
        qDebug() << "main window is NOT full screen";
    qDebug() << "graphics view - glwidget" << this->viewport()->objectName();
    qDebug() << "graphics view - glwidget" << this->viewport();
    qDebug() << "graphics view - scene" << this->scene();
    qDebug() << "graphics view - rect" << this->viewport()->rect();
    qDebug() << "graphics view - scenerect" << this->sceneRect();
    qDebug() << "graphics view - scene rect" << this->scene()->sceneRect();
#endif


    resizing = true;
    setSceneRect(QRect(QPoint(0,0),event->size()));
    if(scene())
        scene()->setSceneRect(0,0,50000,50000);
    if(qobject_cast<RCView*>(parent()) != 0)
    {
        if(((RCView*)parent())->getView3dWindow() != 0)
            ((RCView*)parent())->getView3dWindow()->sizeevent(event->size().width(),event->size().height());
    }
    //event->accept();


#if 0
    qDebug() << "graphics view - rect" << this->viewport()->rect();
    qDebug() << "graphics view - scenerect" << this->sceneRect();
    qDebug() << "graphics view - scene rect" << this->scene()->sceneRect();
#endif


    //QGraphicsView::resizeEvent(event);
    //emit wasResized(this);
}

void RCGraphicsView::enterEvent(QEvent *event)
{
    //qDebug() << "enterEvent";
    program->drawGraphicsCursor();
    //if(((RCView*)parent())->getView3dWindow() != 0)
    //    ((RCView*)parent())->getView3dWindow()->drawCursor();
}
