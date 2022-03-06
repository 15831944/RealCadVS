
//
//  The implementation of the User Interface Classes
//

#include "ncwin.h"
#include <QWidget>
#if ! defined(_WIN32_WCE)
#include <time.h>
#endif

//#include "ijl.h"
#include "RCGraphicsItems.h"
#include "RCGraphicsScene.h"
#include "RCView.h"
#include "RCCentralWidget.h"

//
//  The Window manager class
//
#ifdef USING_WIDGETS
int View3dWindowList::add(View3dWindow *window)
{
    int ret = GeneralList::add(window);
    //window->qsidx = GeneralList::length()-1;
    return ret;
}
#endif
WindowManager::WindowManager()
{ lastwindow = NULL;
}

int WindowManager::add(Window *window)
{ return list.add(window);
}

void WindowManager::del(Window *window)
{ if (window == lastwindow) lastwindow = NULL;
  list.del(window);
}

Window *WindowManager::match(RCHWND hWnd)
{
#ifdef USING_WIDGETS
  Window *w;
  RCHWND thWnd=hWnd;

  //qDebug() << "match in hWnd : " << (QObject*)hWnd;
  //qDebug() << "match in hWnd : " << ((QObject*)hWnd)->objectName();

  if (lastwindow != NULL)
  {
      //if this is a GL widget we want the owner of its QGraphicsView
      if(thWnd && (((QObject*)thWnd)->objectName().contains("QGLWidget")))
          thWnd = ((QObject*)thWnd)->parent()->parent();
      else if(thWnd && (((QObject*)thWnd)->objectName().contains("RCGraphicsView")))
          thWnd = ((QObject*)thWnd)->parent();
      else if(thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_X")||
              thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_Y")||
              thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_Z"))
          thWnd = ((QObject*)thWnd)->parent();
      //else if(thWnd && ((QObject*)thWnd)->objectName().contains("X_CEDIT")||
      //        thWnd && ((QObject*)thWnd)->objectName().contains("Y_CEDIT")||
      //        thWnd && ((QObject*)thWnd)->objectName().contains("Z_CEDIT"))
      //    thWnd = ((QObject*)thWnd)->parent();
      if (lastwindow->gethwnd() == thWnd) return lastwindow;
      lastwindow->mousemoveevent(-32767,-32767,0);
      lastwindow = NULL;
  }
  for (list.start() ; (w = (Window *) list.next()) != NULL ; )
  {
      //qDebug() << "match w : " << ((QObject*)w->gethwnd())->objectName();

       //if this is a GL widget we want the owner of its QGraphicsView
       if(thWnd && ((QObject*)thWnd)->objectName() == "QGLWidget" )
           thWnd = ((QObject*)thWnd)->parent()->parent();
       else if(thWnd && (((QObject*)thWnd)->objectName().contains("RCGraphicsView")))
           thWnd = ((QObject*)thWnd)->parent();
       else if(thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_X")||
               thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_Y")||
               thWnd && ((QObject*)thWnd)->objectName().contains("QLabel_Z"))
            thWnd = ((QObject*)thWnd)->parent();
       //else if(thWnd && ((QObject*)thWnd)->objectName().contains("X_CEDIT")||
       //        thWnd && ((QObject*)thWnd)->objectName().contains("Y_CEDIT")||
       //        thWnd && ((QObject*)thWnd)->objectName().contains("Z_CEDIT"))
       //    thWnd = ((QObject*)thWnd)->parent();
       if (thWnd == w->gethwnd())
       {
           lastwindow = w;
           return lastwindow;
       }
      thWnd = hWnd;
  }
  //qDebug() << "wm match loop end [not found] >>>>>>>>>>";
  return NULL;
#else
    Window *w;
    qDebug() << "match in hWnd : " << (QObject*)hWnd;
    if (lastwindow != NULL)
    {
        if (lastwindow->gethwnd() == hWnd) return lastwindow;
        lastwindow->mousemoveevent(-32767,-32767,0);
        lastwindow = NULL;
    }
    //qDebug() << "list start =======";
    for (list.start() ; (w = (Window *) list.next()) != NULL ; )
    {
      //qDebug() << "match w : " << (QObject*)w->gethwnd();
      if ((QObject*)w->gethwnd() == (QObject*)hWnd)
      {
          qDebug() << "list matched : " << (QObject*)w->gethwnd();
          lastwindow = w;  return lastwindow;
      }
    }
    //qDebug() << "list end =======";
    return NULL;
#endif
}

void WindowManager::destroy(void)
{
  Window *w;
  for (list.start() ; (w = (Window *) list.next()) != NULL ; )
    if (w != cadwindow)
    {
#ifdef USING_WIDGETS
        //DestroyWindow(w->gethwnd());
        //w->sethwnd(0);
        delete w;
#else
        DestroyWindow(w->gethwnd());
#endif
    }
  list.destroy();
}

//
//  The 'Cursor' class
//

Cursor::Cursor(Window *window,int w,int h)
{
   parent = window;
   hDC = window->gethdc();
   s = 1;  a = 0;
   width = w;  height = h;
   drawn = 0;
   shape = 0;
   size = GetPrivateProfileInt("Cursor","CursorSize",3,home.getinifilename());
#ifdef USING_WIDGETS
   m_rubberband = 0;
   qgi=0;
   // change the cursors depending on the background color
   QColor background(::v.getreal("wn::backgroundcolour.red")*255.0,
                     ::v.getreal("wn::backgroundcolour.green")*255.0,
                     ::v.getreal("wn::backgroundcolour.blue")*255.0);
   int reverseCursor = 0; // normal cursors
   if( background.red() <= 127 &&
           background.green() <= 127 &&
           background.blue() <= 127  )
     reverseCursor = 1; // reversed cursors
   // use the gl view for the graphic cursor
   //QCursor qcursor(Qt::CrossCursor);
   //QCursor qcursor(QBitmap(":/images/crosshair_cur.bmp"),QBitmap(":/images/crosshair_msk.bmp"));
   RCCursor qcursor;
   if(reverseCursor)
       qcursor = RCCursor(":/images/crosshair_rev_cur.bmp",":/images/crosshair_rev_msk.bmp");
   else
       qcursor = RCCursor(":/images/crosshair_cur.bmp",":/images/crosshair_msk.bmp");
   if(parent->gethwnd())
   {
       QPoint cpos = ((QWidget*)parent->gethwnd())->rect().center();
       m_qw = ((QWidget*)parent->gethwnd())->childAt(cpos);
       if(m_qw)
           m_qw->setCursor((QCursor&)qcursor);
   }
#endif
}

void Cursor::erase(void)
{
    if(qgi)
        parent->getScene()->removeItem(qgi);
    delete qgi;
    qgi=0;
}

void Cursor::off(void)
{
    if (drawn == 1)
        draw(x,y);
    drawn = -1; // -1 == off
    if(qgi)
        parent->getScene()->removeItem(qgi);
    delete qgi;
    qgi=0;
}

void Cursor::on(void)
{
   if (drawn == 0 && x != -32767 && y != -32767)
   {
        draw(x,y);
        drawn = 1; // 1 == on
   }
   else
       drawn = 0;
}

void Cursor::style(int sty)
{
  if (drawn == 1)
    draw(x,y);
  s = sty;
  if (drawn != -1)
      drawn = 0;
}

void Cursor::setshape(int s)
{
  if (drawn == 1)
    draw(x,y);
  shape = s;
  if (drawn != -1)
      drawn = 0;
}

void Cursor::action(int act)
{
  if (drawn == 1)
    draw(x,y);
  a = act;
  if (drawn != -1)
      drawn = 0;
}

void Cursor::rubberband(int xp,int yp)
{
#if 0
     // using a qt rubberband
      if(m_rubberband == 0)
      {
          m_rubberband = new QRubberBand((QRubberBand::Shape)1,m_qw);
      }
#endif
  if (drawn == 1)
  {
#if 0
      m_rubberband->show();
#endif
      draw(x,y);
      //xr = xp;  yr = yp;
  }

  if (drawn != -1)
  {
#if 0
      // using a qt rubberband
      m_rubberband->hide();
#endif
      drawn = 0;
      xr = xp;  yr = yp;
  }
}

void Cursor::sizeevent(int w,int h)
{
  if (drawn == 1)
    draw(x,y);
  if (drawn != -1)
      drawn = 0;
   width = w;  height = h;
}

void Cursor::mousemoveevent(int xp,int yp,int)
{
  if (drawn == -1) return;
  if (xp == -32767 && yp == -32767)
  {
      if (drawn == 1)
      {
           draw(x,y);  x = xp;  y = yp;

           drawn = 0;
      }

  }
  else
  {

#if 1
      if(a == 2)
      {
          draw(xp,yp);
          x = xp;  y = yp;
          /*
           if (drawn == 1)
           {
               draw(xp,yp);
               x = xp;  y = yp;

               drawn = 0;
           }
           if( drawn != -1)
           {
               drawn = 1;
               x = xp;  y = yp;
           }
           */
      }
      else
      {

          if(qgi)
             parent->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;

          draw(x,y);
          x = xp;  y = yp;
      }
#else
      if (drawn == 1)
      {
          draw(x,y);
      }

      if(qgi)
         parent->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;


       x = xp;  y = yp;
       if( drawn == 0)
        draw(x,y);
       drawn = 1;
#endif
  }
}

void Cursor::drawCursor()
{
    SetCursor(0);

    if(m_qw)
    {
        //m_qw->unsetCursor();
        //qDebug() << "Widget for cursor is    :" << m_qw->objectName();
        //qDebug() << "Widget cursor before is :" << m_qw->cursor().shape();
        // qDebug() << "Widget cursor after is    :" << m_qw->cursor();
        if (program->gethcrosshairsnapcursor() == program->gethemptycursor())
          m_qw->setCursor(s == 2 ? (QCursor&)*program->gethcursor() : (QCursor&)*program->gethemptycursor());
          //SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());
        else if (program->gethdefaultcursor() != program->getharrowcursor() && program->gethdefaultcursor() != program->gethpositioncrosshaircursor())
          m_qw->setCursor((QCursor&)*program->gethdefaultcursor());
          //SetCursor(program->gethdefaultcursor());
        else if (s == 1 && shape == 1)
          m_qw->setCursor((QCursor&)*program->gethcrosshairsnapcursor());
          //SetCursor(program->gethcrosshairsnapcursor());
        else if (s == 1 && shape == 2)
          m_qw->setCursor((QCursor&)*program->gethcrosshairentitycursor());
          //SetCursor(program->gethcrosshairentitycursor());
        else if (s == 1 && shape == 3)
          m_qw->setCursor((QCursor&)*program->getherasecursor());
          //SetCursor(program->getherasecursor());
        else if (s == 1 && shape == 4)
          m_qw->setCursor((QCursor&)*program->gethcrosshairorigincursor());
          //SetCursor(program->gethcrosshairorigincursor());
        else if (s == 1 && shape == 5)
          m_qw->setCursor((QCursor&)*program->gethcrosshairintcursor());
          //SetCursor(program->gethcrosshairintcursor());
        else if (s == 1 && shape == 0)
        {
          m_qw->setCursor((QCursor&)*program->gethcrosshaircursor());
          //setCursor(*program->gethcrosshaircursor());
          //qDebug() << "getting crosshair cursor)";
          //qDebug() << m_qw->cursor().shape();
        }
        else
          m_qw->setCursor(s == 2 ? (QCursor&)*program->gethcursor() : (QCursor&)*program->gethemptycursor());
          //SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());
        //qDebug() << "Widget cursor after is    :" << m_qw->cursor().shape();
        //qDebug() << "Widget cursor after is    :" << m_qw->cursor();
        //m_qw->update();
    }
    else
        SetCursor(program->gethdefaultcursor());

}

void Cursor::draw(int xp,int yp)
{
#ifdef USING_WIDGETS
    int oldmode,offscreen;
    RCHPEN hPen,hOldpen;
    RCHDC oldhDC;

    if (drawn == -1) return;

    QList<QGraphicsItem *> glist;
    QGraphicsItem *gitem = 0;

    //QPoint gp = m_qw->mapToGlobal(QPoint(xp,yp));
    //m_qw->cursor().setPos(gp.x(),gp.y());

    /*
    if (typeid(*parent) == typeid(class View3dWindow) && ((View3dWindow *)parent)->getoffscreenbitmap() != 0)
      {  offscreen = 1;
         oldhDC = hDC;
         hDC = ((View3dWindow *)parent)->getoffscreenbitmap()->gethdc();
      }
    else
      offscreen = 0;
    */

    //hPen  = CreatePen(PS_SOLID,1, RGB(  0,   0, 255));
    //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
    //oldmode = SetROP2(hDC,R2_NOT);
    /*
    if (s == 1 || s >= 3)
    {

        if (program->gethcrosshairsnapcursor() == program->gethemptycursor())
           {  //  Only draw the cross hairs if the windows cursors are off
              View3dWindow* v =  ((RCView*)hDC)->getView3dWindow();
              if(v->qgich && v->qgich->scene() != v->getScene())
                  v->qgich->scene()->removeItem(v->qgich);
              else
                  v->getScene()->removeItem(v->qgich);
              v->qgich = new QGraphicsItemGroup;

              QGraphicsItem *qgi=0;

              hPen  = CreatePen(PS_SOLID,1, RGB(  0,   0, 255));
              hOldpen = (RCHPEN) SelectObject(v,hPen);

              //MoveToEx(hDC,xp,0,0);    LineTo(hDC,xp,height-1);
              v->moveto(xp,0); qgi = v->lineto(qgi,xp,height-1);
              if(qgi && v->qgich)
                   ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);

              //MoveToEx(hDC,0,height-yp,0);    LineTo(hDC,width-1,height-yp);
              qgi=0;
              v->moveto(0,height-yp); qgi = v->lineto(qgi,width-1,height-yp);
              if(qgi && v->qgich)
                   ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);

              if (shape == 1)
              {
                   v->moveto(xp-size,height-yp-size);
                   qgi=0;
                   qgi = v->lineto(qgi,xp-size,height-yp+size);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp+size,height-yp+size);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp+size,height-yp-size);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp-size,height-yp-size);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
              }
              else if (shape == 2)
              {
                   v->moveto(xp-size*2,height-yp);
                   qgi=0;
                   qgi = v->lineto(qgi,xp,height-yp+size*2);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp+size*2,height-yp);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp,height-yp-size*2);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
                   qgi=0;
                   qgi = v->lineto(qgi,xp-size*2,height-yp);
                   if(qgi && v->qgich)
                        ((QGraphicsItemGroup*)v->qgich)->addToGroup(qgi);
              }
              hOldpen = (RCHPEN) SelectObject(v,hOldpen);
              if(v->qgich && v->getScene())
                  v->getScene()->addItem(v->qgich);
           }
    }
    */


    drawCursor();
#if 0
    SetCursor(0);

    if(m_qw)
    {
        //qDebug() << "Widget for cursor is    :" << m_qw->objectName();
        //qDebug() << "Widget cursor before is :" << m_qw->cursor().shape();
        // qDebug() << "Widget cursor after is    :" << m_qw->cursor();
        if (program->gethcrosshairsnapcursor() == program->gethemptycursor())
          m_qw->setCursor(s == 2 ? (QCursor&)*program->gethcursor() : (QCursor&)*program->gethemptycursor());
          //SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());
        else if (program->gethdefaultcursor() != program->getharrowcursor() && program->gethdefaultcursor() != program->gethpositioncrosshaircursor())
          m_qw->setCursor((QCursor&)*program->gethdefaultcursor());
          //SetCursor(program->gethdefaultcursor());
        else if (s == 1 && shape == 1)
          m_qw->setCursor((QCursor&)*program->gethcrosshairsnapcursor());
          //SetCursor(program->gethcrosshairsnapcursor());
        else if (s == 1 && shape == 2)
          m_qw->setCursor((QCursor&)*program->gethcrosshairentitycursor());
          //SetCursor(program->gethcrosshairentitycursor());
        else if (s == 1 && shape == 3)
          m_qw->setCursor((QCursor&)*program->getherasecursor());
          //SetCursor(program->getherasecursor());
        else if (s == 1 && shape == 4)
          m_qw->setCursor((QCursor&)*program->gethcrosshairorigincursor());
          //SetCursor(program->gethcrosshairorigincursor());
        else if (s == 1 && shape == 5)
          m_qw->setCursor((QCursor&)*program->gethcrosshairintcursor());
          //SetCursor(program->gethcrosshairintcursor());
        else if (s == 1 && shape == 0)
        {
          m_qw->setCursor((QCursor&)*program->gethcrosshaircursor());
          //setCursor(*program->gethcrosshaircursor());
        }
        else
          m_qw->setCursor(s == 2 ? (QCursor&)*program->gethcursor() : (QCursor&)*program->gethemptycursor());
          //SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());
        //qDebug() << "Widget cursor after is    :" << m_qw->cursor().shape();
        //qDebug() << "Widget cursor after is    :" << m_qw->cursor();
        m_qw->update();
    }
    else
        SetCursor(program->gethdefaultcursor());
#endif

    if (a == 1)
    {
        //MoveToEx(hDC,xp,yp,0);  LineTo(hDC,xr,yr);
        if(parent->getScene())
        {
            if(qgi)
                qgi->scene()->removeItem(qgi);
            delete qgi;
            qgi=0;
            QColor background(::v.getreal("wn::backgroundcolour.red")*255.0,
                              ::v.getreal("wn::backgroundcolour.green")*255.0,
                              ::v.getreal("wn::backgroundcolour.blue")*255.0);
            QColor colour(background.red()  ==127?255:255-background.red(),
                          background.green()==127?255:255-background.green(),
                          background.blue() ==127?255:255-background.blue());
            QPen qpen(colour);
            QPointF qp1 = ((RCView*)parent->gethwnd())->view()->mapToScene(xp,yp);
            QPointF qp2 = ((RCView*)parent->gethwnd())->view()->mapToScene(xr,yr);


            if(((RCView*)parent->gethwnd())->view()->scene())
                gitem = ((RCView*)parent->gethwnd())->view()->scene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),qpen);
            if(gitem) glist.append(gitem);
            if(glist.size() > 0 )
               qgi = ((RCView*)parent->gethwnd())->view()->scene()->createItemGroup(glist);

            if(qgi)
            {
                qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                //qgi->update();
            }
        }
    }
    else if (a == 2 )
    {
#if 0
        // use a qt rubberband
        QPoint qp1(xp,yp);
        QPoint qp2(xr,yr);
        m_rubberband->setGeometry(QRect(qp1,qp2));
        m_rubberband->show();
#endif
#if 0
        // draw a gl rubberband
        glxp = xp;
        glyp = yp;
        parent->paintevent();
#endif
#if 1
        // draw a qscene rubberband
        if(qgi)
        {
            qDebug() << "enter draw cursor qgi " << qgi;
            qDebug() << "enter draw cursor " << qgi->childItems().length();
        }
        else
            qDebug() << "enter draw cursor ";
         if(parent->getScene())
         {
             qDebug() << "rubberband parent: " << parent;
             qDebug() << "rubberband parent scene: " << parent->getScene();
             //((RCView*)parent->gethwnd())->view()->scene()
             if(qgi)
             {
                 qDebug() << "remove cursor gitem ggi" << qgi;
                 qDebug() << "remove cursor gitem " << qgi->childItems().length();
                 parent->getScene()->removeItem(qgi);
                 parent->getScene()->clear();
             }
             delete qgi;
             qgi=0;
             QColor background(::v.getreal("wn::backgroundcolour.red")*255.0,
                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                               ::v.getreal("wn::backgroundcolour.blue")*255.0);
             QColor colour(background.red()  ==127?255:255-background.red(),
                           background.green()==127?255:255-background.green(),
                           background.blue() ==127?255:255-background.blue());
             QPen qpen(colour);
             QPointF qp1 = ((RCView*)parent->gethwnd())->view()->mapToScene(xp,yp);
             QPointF qp2 = ((RCView*)parent->gethwnd())->view()->mapToScene(xr,yr);
             //xr=xp;yr=yp;
             //x=xp;y=yp;

             //if(((RCView*)parent->gethwnd())->view()->scene())
             if(parent->getScene())
             {
                 parent->getScene()->clear();
                 glist.clear();
                 qgi = new QGraphicsItemGroup();
                 //qitem = parent->getScene()->addRect(QRectF(qp1,qp),qpen);
                 gitem = new QGraphicsLineItem(qp1.x(),qp1.y(),qp1.x(),qp2.y());
                 ((QGraphicsLineItem*)gitem)->setPen(qpen);
                 //gitem = parent->getScene()->addLine(qp1.x(),qp1.y(),qp1.x(),qp2.y(),qpen);
                 if(gitem)
                     //qgi->addToGroup(gitem);
                     glist.append(gitem);
                 //gitem = parent->getScene()->addLine(qp1.x(),qp2.y(),qp2.x(),qp2.y(),qpen);
                 gitem = new QGraphicsLineItem(qp1.x(),qp2.y(),qp2.x(),qp2.y());
                 ((QGraphicsLineItem*)gitem)->setPen(qpen);
                 if(gitem)
                     //qgi->addToGroup(gitem);
                    glist.append(gitem);
                 //gitem = parent->getScene()->addLine(qp2.x(),qp2.y(),qp2.x(),qp1.y(),qpen);
                 gitem = new QGraphicsLineItem(qp2.x(),qp2.y(),qp2.x(),qp1.y());
                 ((QGraphicsLineItem*)gitem)->setPen(qpen);
                 if(gitem)
                     //qgi->addToGroup(gitem);
                    glist.append(gitem);
                 //gitem = parent->getScene()->addLine(qp2.x(),qp1.y(),qp1.x(),qp1.y(),qpen);
                 gitem = new QGraphicsLineItem(qp2.x(),qp1.y(),qp1.x(),qp1.y());
                 ((QGraphicsLineItem*)gitem)->setPen(qpen);
                 if(gitem)
                     //qgi->addToGroup(gitem);
                    glist.append(gitem);
                 if(glist.size() > 0 )
                     qgi = parent->getScene()->createItemGroup(glist);
                 glist.clear();
                 if(qgi)
                 {
                     //parent->getScene()->addItem(qgi);
                     qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                 }
             }
         }
#endif
    }
    /*
    if (offscreen)
    {
        hDC = oldhDC;
         offscreen = 1;
         ((View3dWindow *)parent)->UpdateWindow();
    }
    */

#else
 int oldmode,offscreen;
 RCHPEN hPen,hOldpen;
 RCHDC oldhDC;

  if (drawn == -1) return;

  if (typeid(*parent) == typeid(class View3dWindow) && ((View3dWindow *)parent)->getoffscreenbitmap() != 0)
    {  offscreen = 1;
       oldhDC = hDC;
       hDC = ((View3dWindow *)parent)->getoffscreenbitmap()->gethdc();
    }
  else
    offscreen = 0;

  hPen  = CreatePen(PS_SOLID,1, RGB(  0,   0, 255));
  hOldpen = (RCHPEN) SelectObject(hDC,hPen);
  oldmode = SetROP2(hDC,R2_NOT);
  if (s == 1 || s >= 3)
    {  if (program->gethcrosshairsnapcursor() == program->gethemptycursor())
         {  //  Only draw the cross hairs if the windows cursors are off
            MoveToEx(hDC,xp,0,0);    LineTo(hDC,xp,height-1);
            MoveToEx(hDC,0,yp,0);    LineTo(hDC,width-1,yp);
            if (shape == 1)
              {  MoveToEx(hDC,xp-size,yp-size,0);
                 LineTo(hDC,xp-size,yp+size);
                 LineTo(hDC,xp+size,yp+size);
                 LineTo(hDC,xp+size,yp-size);
                 LineTo(hDC,xp-size,yp-size);
              }
            else if (shape == 2)
              {  MoveToEx(hDC,xp-size*2,yp,0);
                 LineTo(hDC,xp,yp+size*2);
                 LineTo(hDC,xp+size*2,yp);
                 LineTo(hDC,xp,yp-size*2);
                 LineTo(hDC,xp-size*2,yp);
              }
         }
    }

  if (program->gethcrosshairsnapcursor() == program->gethemptycursor())
    SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());
  else if (program->gethdefaultcursor() != program->getharrowcursor() && program->gethdefaultcursor() != program->gethpositioncrosshaircursor())
    SetCursor(program->gethdefaultcursor());
  else if (s == 1 && shape == 1)
    SetCursor(program->gethcrosshairsnapcursor());
  else if (s == 1 && shape == 2)
    SetCursor(program->gethcrosshairentitycursor());
  else if (s == 1 && shape == 3)
    SetCursor(program->getherasecursor());
  else if (s == 1 && shape == 4)
    SetCursor(program->gethcrosshairorigincursor());
  else if (s == 1 && shape == 5)
    SetCursor(program->gethcrosshairintcursor());
  else if (s == 1)
    SetCursor(program->gethcrosshaircursor());
  else
    SetCursor(s == 2 ? program->gethcursor() : program->gethemptycursor());

  if (a == 1)
  {
      MoveToEx(hDC,xp,yp,0);  LineTo(hDC,xr,yr);
  }
  else if (a == 2)
  {
       MoveToEx(hDC,xp,yp,0);  LineTo(hDC,xp,yr);  LineTo(hDC,xr,yr);
       LineTo(hDC,xr,yp);  LineTo(hDC,xp,yp);
  }
  SetROP2(hDC,oldmode);
  SelectObject(hDC,hOldpen);
  DeleteObject(hPen);


  if (offscreen)
    {  hDC = oldhDC;
       offscreen = 1;
       ((View3dWindow *)parent)->UpdateWindow();
    }
#endif
}
void Cursor::drawGL(View3dWindow *vs)
{
    if (drawn == -1)
        return;

    RCCOLORREF pcol;
    pcol = RGB(0,0,0);
    SetNoMaterial(0,0,0);

    if (a == 1)
    {
        ; // cross hair type
    }
    else if (a == 2)
    {
        View *view = vs->getview();
        // box type
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, vs->getwidth(), 0, vs->getheight(), -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //
        //Point mp1 = vs->screentomodel(Point(glxp,vs->getheight()-1-glyp,0));
        //Point mp2 = vs->screentomodel(Point(xr,vs->getheight()-1-yr,0));
        Point mp1 = Point(glxp,vs->getheight()-1-glyp,0);
        Point mp2 = Point(xr,vs->getheight()-1-yr,0);
        DrawLine(Point(mp1.x,mp1.y,0),Point(mp2.x,mp1.y,0),0);
        DrawLine(Point(mp2.x,mp1.y,0),Point(mp2.x,mp2.y,0),0);
        DrawLine(Point(mp2.x,mp2.y,0),Point(mp1.x,mp2.y,0),0);
        DrawLine(Point(mp1.x,mp2.y,0),Point(mp1.x,mp1.y,0),0);
    }
}

//
//  The Rubberband classes
//

RBCallbackInfo::RBCallbackInfo(int l,int i,RubberbandCallback cb,void *d, bool df)
{
    level = l;  id = i;  callback = cb;  data = d;  drawn = 0; drawable = df;
}

RBRawCallbackInfo::RBRawCallbackInfo(int l,int i,RubberbandRawCallback cb,void *d)
{
    level = l;  id = i;  callback = cb;  data = d;  drawn = 0;  x = -32767;  y = -32767;
}

void Rubberband::mousemoveevent(View3dSurface *surface,int x,int y,int)
{RBCallbackInfo *cbinfo;
 RBRawCallbackInfo *rawcbinfo;
 Point p;
 drawing = 0;
  for (rawlist.start() ; (rawcbinfo = rawlist.next()) != NULL ;)
    if (rawcbinfo->level == level || rawcbinfo->level == 0)
    {
         if (rawcbinfo->drawn)
         {
              rawcbinfo->callback(RB_ERASE,rawcbinfo->data,rawcbinfo->x,rawcbinfo->y,surface);
              rawcbinfo->drawn = 0;
              drawing = 0;
         }
         if (x != -32767 && y != -32767)
         {
              rawcbinfo->callback(RB_DRAW,rawcbinfo->data,x,y,surface);
              rawcbinfo->x = x;  rawcbinfo->y = y;
              rawcbinfo->drawn = 1;
              drawing = level > 0 ? 1 : 0;
         }
    }
  if (x != -32767 || y != -32767)
    if (! surface->coordinate3d(x,y,&p)) return;
  for (list.start() ; (cbinfo = list.next()) != NULL ;)
    if (cbinfo->level == level || cbinfo->level == 0)
    {
         if (cbinfo->drawn)
         {
              cbinfo->callback(RB_ERASE,cbinfo->data,&cbinfo->p,surface);
              cbinfo->drawn = 0;
              drawing = level > 0 ? 1 : 0;
         }
         if (x != -32767 && y != -32767)
         {
              cbinfo->callback(RB_DRAW,cbinfo->data,&p,surface);
              cbinfo->p = p;
              cbinfo->drawn = 1;
              drawing = level > 0 ? 1 : 0;
         }
    }
}

void Rubberband::input3devent(Point p,Point)
{
  RBCallbackInfo *cbinfo;
  for (list.start() ; (cbinfo = list.next()) != NULL ;)
    if (cbinfo->level == level || cbinfo->level == 0)
    {
         if (cbinfo->drawn)
         {
              cbinfo->callback(RB_ERASE,cbinfo->data,&cbinfo->p,NULL);
              cbinfo->drawn = 0;
         }
         cbinfo->callback(RB_DRAW,cbinfo->data,&p,NULL);
         cbinfo->p = p;
         cbinfo->drawn = 1;
    }
}

void Rubberband::timerevent(void)
{
  RBRawCallbackInfo *rawcbinfo;
  for (rawlist.start() ; (rawcbinfo = rawlist.next()) != NULL ;)
    if (rawcbinfo->level == level || rawcbinfo->level == 0)
      rawcbinfo->callback(RB_TIMER,rawcbinfo->data,rawcbinfo->x,rawcbinfo->y,NULL);
}

int Rubberband::add(RubberbandCallback callback,void *data)
{
    return list.add(new RBCallbackInfo(level,id,callback,data));
}

int Rubberband::add(RubberbandRawCallback callback,void *data)
{
    return rawlist.add(new RBRawCallbackInfo(level,id,callback,data));
}

void Rubberband::begin(int add)
{ if (! add) level++;
  id++;
}

void Rubberband::off(void)
{RBCallbackInfo *cbinfo;
 RBRawCallbackInfo *rawcbinfo;
  for (list.start() ; (cbinfo = list.next()) != NULL ;)
    {  if (cbinfo->drawn)
         {  cbinfo->callback(RB_ERASE,cbinfo->data,&cbinfo->p,NULL);
            cbinfo->drawn = 0;
            drawing = 0;
         }
    }
  for (rawlist.start() ; (rawcbinfo = rawlist.next()) != NULL ;)
    {  if (rawcbinfo->drawn)
         {  rawcbinfo->callback(RB_ERASE,rawcbinfo->data,rawcbinfo->x,rawcbinfo->y,NULL);
            rawcbinfo->drawn = 0;
            drawing = 0;
         }
    }
}

void Rubberband::end(int add)
{RBCallbackInfo *cbinfo;
 RBRawCallbackInfo *rawcbinfo;
  for (list.start() ; (cbinfo = list.next()) != NULL ; )
    {  if (cbinfo->id == id)
         {  if (cbinfo->drawn) cbinfo->callback(RB_ERASE,cbinfo->data,&cbinfo->p,NULL);
            list.del(cbinfo);
            delete cbinfo;
         }
       else
         break;
    }
  for (rawlist.start() ; (rawcbinfo = rawlist.next()) != NULL ; )
    {  if (rawcbinfo->id == id)
         {  if (rawcbinfo->drawn) rawcbinfo->callback(RB_ERASE,rawcbinfo->data,rawcbinfo->x,rawcbinfo->y,NULL);
            rawlist.del(rawcbinfo);
            delete rawcbinfo;
         }
       else
         break;
    }
  if (! add) level--;
  id--;
  drawing = 0;
}

Rubberband::~Rubberband()
{RBCallbackInfo *cbinfo;
 RBRawCallbackInfo *rawcbinfo;
  for (list.start() ; (cbinfo = list.next()) != NULL ; )
    delete cbinfo;
  for (rawlist.start() ; (rawcbinfo = rawlist.next()) != NULL ; )
    delete rawcbinfo;
  list.destroy();
  rawlist.destroy();
}

//
//  The surface class
//
Surface::Surface()
{
#ifdef USING_WIDGETS
    m_scene=0;
    qsidx=0;
    curWeight=1;
    curMMWeight=1.0;
    qgi=0;

    printxdotspermm = printydotspermm = 1;
#endif

  transform = 0;  lastx = lasty = 0;  linestyle = NULL;
  clipx1 = 0.0;  clipy1 = 0.0;  clipx2 = 1.0;  clipy2 = 1.0;
  colour = style = weight = drawmode = -32767;
  scale = plotscale = width = height = 1.0;
  originx = originy = 0.0;
  angle = 0.0;
  allocatedpens = 0;
  horiginalpen = 0;
  blackandwhite = 0;
  colourindex = NULL;
  background = RGB(255,255,255);
  colourref = fillcolourref = RGB(0,0,0);
  hWnd = NULL;  hDC = NULL;
  outfile = 0;
  fileformat = 0;
  patternlinestyle = 0;
  linestyle = 0;
  bandoffset = 0;
}

QGraphicsScene *Surface::getScene()
{
    //qDebug() << "(getScene() surface : " << this << " m_scene : " << m_scene;
    if(m_scene == 0)
    {
        //qDebug() << "getScene() is null";
        //pause();
        return (QGraphicsScene*)0;
    }
    return (QGraphicsScene*)m_scene;
}

int Surface::openhpgl(RCCHAR *s)
{
 RCOPENFILENAME ofn;
 RCCHAR directory[300]={0},filename[300]={0},filetitle[300]={0},filter[300]={0};
 RCCHAR defExt[300]={0},dlgTitle[300]={0};
 RCUINT  i;
  if (s == 0)
    {  _getcwd(directory, sizeof(directory));
       filename[0] = '\0';
       _tcscpy(filter,_RCT("HPGL Files (*.hpg *.HPG *.plt *.PLT)"));
       //for (i = 0; filter[i] != '\0'; i++)
       //  if (filter[i] == '|') filter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = (HWND)cadwindow->gethwnd();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile= filename;
       ofn.nMaxFile = sizeof(filename);
       ofn.lpstrFileTitle = filetitle;
       ofn.nMaxFileTitle = sizeof(filetitle);
       ofn.lpstrInitialDir = directory;
       strcpy(defExt,"hpg");
       ofn.lpstrDefExt = defExt;
       strcpy(dlgTitle,"Export HPGL file");
       ofn.lpstrTitle = dlgTitle;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       outfile = 0;
       if (! GetSaveFileName(&ofn)) return 0;
    }
#if ! defined(_WIN32_WCE)
  else
    strcpy(filename,s);
#endif
  fileformat = 0;
  return (outfile = fopen(filename,TEXT("w"))) != NULL;
}

void Surface::closehpgl(void)
{ if (outfile != 0)
    fclose(outfile);
  outfile = 0;
}

int Surface::openiges(RCCHAR *s)
{
 RCOPENFILENAME ofn;
 RCCHAR directory[300]={0},filename[300]={0},filetitle[300]={0},filter[300]={0};
 RCCHAR defExt[300]={0},dlgTitle[300]={0};
 RCUINT  i;
  if (s == 0)
    {  _getcwd(directory, sizeof(directory));
       filename[0] = '\0';
       _tcscpy(filter,TEXT("IGES Files (*.igs)"));
       //for (i = 0; filter[i] != '\0'; i++)
       //  if (filter[i] == '|') filter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = (HWND)cadwindow->gethwnd();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile= filename;
       ofn.nMaxFile = sizeof(filename);
       ofn.lpstrFileTitle = filetitle;
       ofn.nMaxFileTitle = sizeof(filetitle);
       ofn.lpstrInitialDir = directory;
       strcpy(defExt,"IGS");
       ofn.lpstrDefExt = defExt;
       strcpy(dlgTitle,"Export as IGES");
       ofn.lpstrTitle = dlgTitle;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       outfile = 0;
       if (! GetSaveFileName(&ofn)) return 0;
    }
#if ! defined(_WIN32_WCE)
  else
    strcpy(filename,s);
#endif
  fileformat = 1;
  if ((outfile = fopen(filename,TEXT("w"))) != 0)
    {time_t ltime;
     struct tm *tm;
     RCCHAR text[300];

#if ! defined(_WIN32_WCE)
       ltime = time(0L);
       tm = localtime(&ltime);
       strftime(text,300,_RCT("%y%m%d.%H%M%M"),tm);
//WCEFIX
#endif

       nstart = 1;
       nglobal = 2;

       if (fprintf(outfile,"                                                                        S      1\n") < 0) return 0;
       if (fprintf(outfile,"1H,,1H;,1H ,4HIGES,7HRealCAD,3H1.0,32,38,7,128,16,1H ,1.0,              G      1\n") < 0) return 0;
       if (fprintf(outfile,"2,2HMM,8,2.0,13H%s,0.001,2.0E+04,1H ,2HAA,6,0;               G      2\n",text) < 0) return 0;

       ndirectory = 0;
       nparameter = 0;

    }

  return outfile != 0;
}

void Surface::closeiges(void)
{ if (outfile != 0)
    {  fprintf(outfile,"S%7dG%7dD%7dP%7d                                        T      1\n",nstart,nglobal,ndirectory,nparameter);
       fclose(outfile);
    }
  outfile = 0;
}

void Surface::allocatepens(int npens)
{int i,indirect,error;
 PALETTEENTRY pe;
 DWORD c;
  if ((colourindex = new RCCOLORREF[npens]) != NULL)
    {  indirect = 1;  error = 0;
#if ! defined(_WIN32_WCE)
       for (i = 0 ; i < npens ; i++)
         if (Escape(hDC,GETCOLORTABLE,sizeof(int),(RCLPCSTR)&i,&c))
           { if (c > 20) indirect = 0;
           }
         else
           {  error = 1;  break;
           }
       for (i = 0 ; i < npens ; i++)
         {  if (error)
              colourindex[i] = RGB(db.colourtable[i].red,db.colourtable[i].green,db.colourtable[i].blue);
            else
              {  Escape(hDC,GETCOLORTABLE,sizeof(int),(RCLPCSTR)&i,&c);
                 if (indirect)
                   {  GetSystemPaletteEntries(cadwindow->gethdc(),i,1,&pe);
                      colourindex[i] = RGB(pe.peRed,pe.peGreen,pe.peBlue);
                   }
                 else
                   colourindex[i] = c;
              }
         }
       allocatedpens = npens;
#else
       allocatedpens = 0;
#endif
    }
}

void Surface::setfillcolour(RCCOLORREF cr)
{ fillcolourref = cr;
  colour = -1;
}

void Surface::setcolour(int c,int dm)
{RCHPEN hpen,hOldpen;
 RCCOLORREF colourref;
  if (colour == c || dm != DM_NORMAL)
    return;
  if (blackandwhite)
    {  if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
         colourref = RGB(255,255,255);
       else
         colourref = RGB(0,0,0);
    }
  else
    colourref = RGB(db.colourtable[c].red,db.colourtable[c].green,db.colourtable[c].blue);
  fillcolourref = colourref;
  hpen  = CreatePen(PS_SOLID,db.lineweights.weighttohdc(hDC,weight),colourref);
  curPen = (QPen)*hpen;
  delete hpen;
  SetTextColor(hDC,colourref);
  //hOldpen = (RCHPEN) SelectObject(hDC,hpen);
  //DeleteObject(hOldpen);
  colour = c;
}

void Surface::setweight(int w,int dm)
{RCHPEN hpen,hOldpen;
 RCCOLORREF colourref;
 int style;
  style = PS_SOLID;
  if (weight == w)
    return;
  if (blackandwhite)
    {  if (db.colourtable[colour].red == 255 && db.colourtable[colour].green == 255 && db.colourtable[colour].blue == 255)
         colourref = RGB(255,255,255);
       else
         colourref = RGB(0,0,0);
    }
  else if (dm == DM_SELECT)
    {  colourref = RGB(127,127,127);
       style = PS_DOT;
    }
  else if (dm == DM_ERASE)
    colourref = background;
  else if (dm == DM_NORMAL)
    colourref = RGB(db.colourtable[colour].red,db.colourtable[colour].green,db.colourtable[colour].blue);
  else
    return;
  fillcolourref = colourref;
  hpen  = CreatePen(style,db.lineweights.weighttohdc(hDC,w),colourref);
  curPen = (QPen)*hpen;
  delete hpen;
  //hOldpen = (RCHPEN) SelectObject(hDC,hpen);
  //DeleteObject(hOldpen);
  weight = w;
}

void Surface::setstyle(double linestylescale,double length,int s,int dm)
{ if (dm != DM_NORMAL)
    return;
  if ((linestyle = db.linestyles.indextostyle(s)) != 0)
    linestyle->start(scale / plotscale / linestylescale,length);
  style = s;
}

#ifdef USING_WIDGETS
void Surface::setupGL(double linestylescale,double length,int dm,int c,int s,int w,PatternLineStyle *pls)
{
     RCHPEN hPen=0,hOldpen=0;
     int penwidth;
     double mmwidth, glwidth;

     // set the default GL stipple
     GLint selectstipple = 2; // stipple factor
     GLint dashstipple = 4; // stipple factor
     GLushort stipplepattern = 0x5555; // dots ....

     // KMJ:
     /*
     qDebug() << "======Surface SetupGL==========";
     qDebug() << "length :" << length;
     qDebug() << "scale :" << scale;
     qDebug() << "plotscale :" << plotscale;
     qDebug() << "linestylescale :" << linestylescale;
     qDebug() << "scale / plotscale / linestylescale :" << scale / plotscale / linestylescale;
     qDebug() << "length / scale :" << length / scale;
     qDebug() << "length / scale / plotscale :" << length / scale / plotscale;
     qDebug() << "==============================";
     */

    if(dm == DM_SCROLLING || !db.getlineweightdisplay())
    {
        /*
        if(dm == DM_SELECT)
        {
            mmwidth = db.lineweights.weighttomm(5);
            penwidth = int((ydotspermm < 3 ? 3 : ydotspermm) * mmwidth + 0.5);
            curWeight = penwidth; // for Qt
            curGLWeight = (double)db.lineweights.weighttopx(5);
            curMMWeight = mmwidth;
        }
        else
        */
        {
            mmwidth = db.lineweights.weighttomm(1);
            penwidth = int((ydotspermm < 3 ? 3 : ydotspermm) * mmwidth + 0.5);
            curWeight = penwidth; // for Qt
            curGLWeight = (double)db.lineweights.weighttopx(1);
            curMMWeight = mmwidth;
        }
    }
    else
    {
            mmwidth = db.lineweights.weighttomm(w);
            penwidth = int((ydotspermm < 3 ? 3 : ydotspermm) * mmwidth + 0.5);
            curWeight = penwidth; // for Qt
            curGLWeight = (double)db.lineweights.weighttopx(w);
            curMMWeight = mmwidth;
    }

     // always disable first
     glDisable(GL_LINE_STIPPLE);

      if (dm == drawmode && c == colour && s == style && w == weight && pls == patternlinestyle)
        {
          if(dm != DM_SCROLLING) // no line style or patter when scrolling
          {
           if (linestyle != NULL)
             linestyle->startGL(plotscale / linestylescale ,length);
             //linestyle->startGL(plotscale / linestylescale ,length );
             //linestyle->startGL(scale / plotscale / linestylescale ,length / scale);
             //linestyle->startGL(scale / plotscale / linestylescale ,length );
           if (patternlinestyle != NULL)
             patternlinestyle->startGL(scale ,plotscale);
           return;
          }
        }
      if (allocatedpens > 0)
        {
           hPen  = CreatePen(PS_SOLID,0,colourindex[c % allocatedpens]);
           curPen = (QPen)*hPen;
           delete hPen;
           SetTextColor(hDC,colourindex[c % allocatedpens]);
           RCCOLORREF txcolref = colourindex[c % allocatedpens];
           curTextColour = QColor(GetRValue(txcolref),GetGValue(txcolref),GetBValue(txcolref));
           //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
           //DeleteObject(hOldpen);
           //return;
        }

      if (outfile != 0 && fileformat == 0)
        fprintf(outfile,"SP%d;",c);

      drawmode = dm;  colour = c;  style = s;  weight = w;
      patternlinestyle = pls;
      linestyle = 0;

      if(drawmode != DM_SCROLLING) // no line style or pattern when scrolling
      {
        if (patternlinestyle != 0)
          patternlinestyle->startGL(scale /*/ getpixelmodelmm()*/,plotscale);
        else
        {  // KMJ: need to create a select line style here for GL
          // use an existing one for now
           if (dm == DM_SELECT /*|| dm == DM_ERASE*/)
           {
               // using GL stipple use this if not available??
#if 0
               if(weight > 4)
               {
                 linestyle = db.linestyles.indextostyle(2);
                 linestyle->startGL(plotscale / linestylescale,length);
                 //linestyle->startGL(scale / plotscale / linestylescale,length);
                 //linestyle->start(scale / plotscale / linestylescale /* * getpixelmodelmm())*/,length / scale);
                 //linestyle->start(plotscale / linestylescale,length );
               }
#endif
           }
           else if (dm == DM_ERASE)
             linestyle = NULL;
           else if ((linestyle = db.linestyles.indextostyle(s)) != NULL)
             linestyle->startGL(plotscale / linestylescale, length );
             //linestyle->startGL(scale / plotscale / linestylescale, length);
             //linestyle->startGL(scale / plotscale / linestylescale, length /scale);
             //linestyle->startGL(plotscale / linestylescale, length);
        }
      }

      switch (drawmode)
        {
            case DM_SCROLLING:
              colourref = fillcolourref = RGB(db.colourtable[c].red,db.colourtable[c].green,db.colourtable[c].blue);
              curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
              hPen  = CreatePen(PS_SOLID,penwidth,colourref);
              curPen = (QPen)*hPen;
              delete hPen;
             break;
            case DM_NORMAL :
             /*
             if (blackandwhite)
               {
                  if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
                    colourref = fillcolourref = RGB(255,255,255);
                  else
                    colourref = fillcolourref = RGB(0,0,0);
               }
             */
             if (blackandwhite == 1) // back and white
             {
                  if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
                    colourref = fillcolourref = RGB(255,255,255);
                  else
                    colourref = fillcolourref = RGB(0,0,0);
             }
             else if(blackandwhite == 2) // greyscale
             {
                 if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
                   colourref = fillcolourref = RGB(255,255,255);
                 else
                 {
                     // using the Luminosity method
                     int grey = (db.colourtable[c].red*0.21+db.colourtable[c].green*0.72+db.colourtable[c].blue*0.07);
                     colourref = fillcolourref = RGB(grey,grey,grey);
                 }
             }
             else
    #ifdef USING_WIDGETS
                colourref = fillcolourref = RGB(db.colourtable[c].red,db.colourtable[c].green,db.colourtable[c].blue);

             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));;
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             curPen = (QPen)*hPen;
             delete hPen;
    #else
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetTextColor(hDC,colourref);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
           case DM_INVERT :
    #ifdef USING_WIDGETS
             //colourref = fillcolourref = RGB(0,0,0);
             //*
             colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                             GetGValue(background)==127?255:255-GetGValue(background),
                                             GetBValue(background)==127?255:255-GetBValue(background));
             //*/
             /*
             colourref = fillcolourref = RGB(GetRValue(background)<=155?255:127,
                                             GetGValue(background)<=155?255:127,
                                             GetBValue(background)<=155?255:127);
             */
             colour = 256;  //  Invalidate the colour
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             curPen = (QPen)*hPen;
             delete hPen;
    #else
             colourref = fillcolourref = RGB(255,255,255);
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetTextColor(hDC,colourref);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_NOT);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
           case DM_INVERTDASH :
    #ifdef USING_WIDGETS
             {
             //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
             colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                             GetGValue(background)==127?255:255-GetGValue(background),
                                             GetBValue(background)==127?255:255-GetBValue(background));
             //colourref = fillcolourref = RGB(0,0,0);
             colour = 256;  //  Invalidate the colour
             SetTextColor(hDC,colourref);
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             hPen  = CreatePen(PS_DASH,penwidth,colourref);
             // special dashed pattern
             QVector<qreal> dashes;
             qreal space = 4;
             dashes << 8 << space;
             hPen->setDashPattern(dashes);

             glEnable(GL_LINE_STIPPLE);
             glLineStipple(dashstipple,stipplepattern);

             //
             curPen = (QPen)*hPen;
             delete hPen;
             }
    #else
             colourref = fillcolourref = background;
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_DOT,0,colourref);
             SetTextColor(hDC,colourref);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_NOT);
             SetBkMode(hDC,TRANSPARENT);
    #endif
             break;
           case DM_DASH :
    #ifdef USING_WIDGETS
             {
             /*
             colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                             GetGValue(background)==127?255:255-GetGValue(background),
                                             GetBValue(background)==127?255:255-GetBValue(background));
             */
             colourref = fillcolourref = RGB(GetRValue(background)<=155?255:127,
                                             GetGValue(background)<=155?255:127,
                                             GetBValue(background)<=155?255:127);
             //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
             colour = 256;  //  Invalidate the colour
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             hPen  = CreatePen(PS_DASH,0,colourref);
             // special dashed pattern
             QVector<qreal> dashes;
             qreal space = 4;
             dashes << 8 << space;
             hPen->setDashPattern(dashes);

             glEnable(GL_LINE_STIPPLE);
             glLineStipple(dashstipple,stipplepattern);

             //
             curPen = (QPen)*hPen;
             delete hPen;
             }
    #else
             colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_DASH,0,colourref);
             SetTextColor(hDC,colourref);
             SetBkColor(hDC,background);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,TRANSPARENT);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
    #endif
             break;
           case DM_ERASE :
    #ifdef USING_WIDGETS
             colourref = fillcolourref = background;
             colour = 256;  //  Invalidate the colour
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             curPen = (QPen)*hPen;
             delete hPen;
    #else
             colourref = fillcolourref = background;
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetBkColor(hDC,colourref);
             SetTextColor(hDC,colourref);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
#endif
             break;
           case DM_WPOTHER :
            {
                  /*
                  colourref = fillcolourref = RGB(GetRValue(background)<=127?255:127,
                                                  GetGValue(background)<=127?255:127,
                                                  GetBValue(background)<=127?255:127);
                  */
                  /*
                  colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                                                  GetGValue(background)<=127?255:255-GetGValue(background),
                                                  GetBValue(background)<=127?255:255-GetBValue(background));
                  */

                  colourref = fillcolourref = RGB(GetRValue(background)<=155?255:255-GetRValue(background),
                                                  GetGValue(background)<=155?255:255-GetGValue(background),
                                                  GetBValue(background)<=155?255:255-GetBValue(background));

                  //colourref = fillcolourref = RGB(127,127,127);
                  //glColor3ub(backgroundr^r,backgroundg^g,backgroundb ^b);

                  colour = 256;  //  Invalidate the colour
                  curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
                  //hPen  = CreatePen(PS_DASH,penwidth,colourref);
                  // set dash style for the OpenGL selection display

                  glEnable(GL_LINE_STIPPLE);
                  glLineStipple(selectstipple,stipplepattern);

                  curPen = (QPen)*hPen;
                  delete hPen;
            }
            break;
           case DM_SELECT :
           case DM_TSELECT :
#ifdef USING_WIDGETS
            {
             /*
             colourref = fillcolourref = RGB(GetRValue(background)<=127?255:127,
                                             GetGValue(background)<=127?255:127,
                                             GetBValue(background)<=127?255:127);
             */
             /*
             colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                                             GetGValue(background)<=127?255:255-GetGValue(background),
                                             GetBValue(background)<=127?255:255-GetBValue(background));
             */
             /*
             colourref = fillcolourref = RGB(GetRValue(background)<=155?255:127,
                                             GetGValue(background)<=155?255:127,
                                             GetBValue(background)<=155?255:127);
             */
             selectcolourref = db.getSelectColourRef();
             if(selectcolourref == background)
                 colourref = fillcolourref = RGB(GetRValue(selectcolourref)^GetRValue(background),
                                                 GetGValue(selectcolourref)^GetGValue(background),
                                                 GetBValue(selectcolourref)^GetBValue(background));
             else
                 colourref = fillcolourref = RGB(GetRValue(selectcolourref),
                                                 GetGValue(selectcolourref),
                                                 GetBValue(selectcolourref));

             //colourref = fillcolourref = RGB(127,127,127);
             //glColor3ub(backgroundr^r,backgroundg^g,backgroundb ^b);

             colour = 256;  //  Invalidate the colour
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             //hPen  = CreatePen(PS_DASH,penwidth,colourref);
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             // set dash style for the OpenGL selection display

             //glEnable(GL_LINE_STIPPLE);
             //glLineStipple(selectstipple,stipplepattern);

             curPen = (QPen)*hPen;
             delete hPen;
             }
    #else
             colourref = fillcolourref = RGB(127,127,127);
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_DOT,penwidth,colourref);
             SetTextColor(hDC,colourref);
             SetBkColor(hDC,background);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
          case DM_HANDLE_SELECT :
            {
              /*
              colourref = fillcolourref = RGB(GetRValue(background)<=127?255:127,
                                            GetGValue(background)<=127?255:127,
                                            GetBValue(background)<=127?255:127);
              */
              /*
              colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                                            GetGValue(background)<=127?255:255-GetGValue(background),
                                            GetBValue(background)<=127?255:255-GetBValue(background));
              */
              //colourref = fillcolourref = RGB(127,127,127);
              if (blackandwhite)
              {
                   if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
                     colourref = fillcolourref = RGB(255,255,255);
                   else
                     colourref = fillcolourref = RGB(0,0,0);
              }
              else
                colourref = fillcolourref = RGB(db.colourtable[c].red,db.colourtable[c].green,db.colourtable[c].blue);

              colour = 256;  //  Invalidate the colour
              curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
              hPen  = CreatePen(PS_SOLID,penwidth,colourref);

              delete hPen;
             }
             break;
           case DM_FLASH1 :
    #ifdef USING_WIDGETS
              colourref = fillcolourref = RGB(255,0,0);
              colour = 256;  //  Invalidate the colour
              hPen  = CreatePen(PS_SOLID,penwidth,colourref);
              curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
              SetBkColor(hDC,background);
              //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
              //DeleteObject(hOldpen);
              //SetROP2(hDC, R2_COPYPEN);
              //SetBkMode(hDC,OPAQUE);
              curPen = (QPen)*hPen;
              delete hPen;
    #else
             colourref = fillcolourref = RGB(255,0,0);
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetTextColor(hDC,colourref);
             SetBkColor(hDC,background);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
           case DM_FLASH2 :
    #ifdef USING_WIDGETS
              colourref = fillcolourref = RGB(0,255,0);
              colour = 256;  //  Invalidate the colour
              hPen  = CreatePen(PS_SOLID,penwidth,colourref);
              //SetTextColor(hDC,colourref);
              curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
              SetBkColor(hDC,background);
              //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
              //DeleteObject(hOldpen);
              //SetROP2(hDC, R2_COPYPEN);
              //SetBkMode(hDC,OPAQUE);
              curPen = (QPen)*hPen;
              delete hPen;
    #else
             colourref = fillcolourref = RGB(0,255,0);
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetTextColor(hDC,colourref);
             SetBkColor(hDC,background);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
            case DM_FLASH3 :
    #ifdef USING_WIDGETS
              colourref = fillcolourref = RGB(0,0,255);
              colour = 256;  //  Invalidate the colour
              hPen  = CreatePen(PS_SOLID,penwidth,colourref);
              //SetTextColor(hDC,colourref);
              curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
              //SetBkColor(hDC,background);
              //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
              //DeleteObject(hOldpen);
              //SetROP2(hDC, R2_COPYPEN);
              //SetBkMode(hDC,OPAQUE);
              curPen = (QPen)*hPen;
              delete hPen;
    #else
             colourref = fillcolourref = RGB(0,0,255);
             colour = 256;  //  Invalidate the colour
             hPen  = CreatePen(PS_SOLID,penwidth,colourref);
             SetTextColor(hDC,colourref);
             SetBkColor(hDC,background);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
           case DM_GRID :
    #ifdef USING_WIDGETS
             //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
             //*
              //29/03/20 - LGM modified this to use the window background colour to set the grid colour
              double rr = ::v.getreal("wn::backgroundcolour.red");
              double gg = ::v.getreal("wn::backgroundcolour.green");
              double bb = ::v.getreal("wn::backgroundcolour.blue");
              if ((rr + gg + bb) < 1.5)
              {
                  colourref = fillcolourref = RGB(255,255,255);
              }
              else
              {
                  colourref = fillcolourref = RGB(0,0,0);
              }
             //colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
             //                                GetGValue(background)<=127?255:255-GetGValue(background),
             //                                GetBValue(background)<=127?255:255-GetBValue(background));
             if(db.grid.getCustColour() == 1)
             {
                 minorcolourref = RGB(::v.getreal("gd::minor.red")*255,
                                      ::v.getreal("gd::minor.green")*255,
                                      ::v.getreal("gd::minor.blue")*255);
                 majorcolourref = RGB(::v.getreal("gd::major.red")*255,
                                      ::v.getreal("gd::major.green")*255,
                                      ::v.getreal("gd::major.blue")*255);
             }
             else
             {
                 //minorcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                 //                     GetGValue(background)<=127?255:255-GetGValue(background),
                 //                     GetBValue(background)<=127?255:255-GetBValue(background));
                 majorcolourref = colourref;
                 // minor grid line colour
                 minorcolourref = RGB(GetRValue(majorcolourref)<127?255-GetRValue(majorcolourref)-64:GetRValue(majorcolourref)-255+64,
                                      GetGValue(majorcolourref)<127?255-GetGValue(majorcolourref)-64:GetGValue(majorcolourref)-255+64,
                                      GetBValue(majorcolourref)<127?255-GetBValue(majorcolourref)-64:GetBValue(majorcolourref)-255+64);
             }
             //*/
             /*
             int rlevel = GetRValue(background);
             int glevel = GetGValue(background);
             int blevel = GetBValue(background);
             int level = (rlevel+glevel+blevel)/3;
             if(level > 128 || (glevel > 175 && rlevel <= 125 && blevel <= 125) )
                colourref = fillcolourref = RGB(0,0,0);
             else
                colourref = fillcolourref = RGB(255,255,255);
             */
              // complementary colours
             // major grid line colour
             //majorcolourref = colourref;
             // minor grid line colour
             //minorcolourref = RGB(GetRValue(majorcolourref)<127?255-GetRValue(majorcolourref)-64:GetRValue(majorcolourref)-255+64,
             //                     GetGValue(majorcolourref)<127?255-GetGValue(majorcolourref)-64:GetGValue(majorcolourref)-255+64,
             //                     GetBValue(majorcolourref)<127?255-GetBValue(majorcolourref)-64:GetBValue(majorcolourref)-255+64);

             // always black and gray or white and gray
             /*
             int level = (GetRValue(background)+GetGValue(background)+GetBValue(background))/3;
             majorcolourref = level <= 127 ? RGB(255,255,255) : RGB(0,0,0);
             minorcolourref = RGB(192,192,192);
             */
             // always black and gray
             /*
             majorcolourref = RGB(73,79,105);
             minorcolourref = RGB(50,55,72);
             */

             colour = 256;  //  Invalidate the colour
             curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
             hPen = CreatePen(PS_SOLID,0,colourref);
             curPen = (QPen)*hPen;
             delete hPen;
    #else
             colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
             colour = 256;  //  Invalidate the colour
             hPen = CreatePen(PS_SOLID,0,colourref);
             SetTextColor(hDC,colourref);
             hOldpen = (RCHPEN) SelectObject(hDC,hPen);
             DeleteObject(hOldpen);
             SetROP2(hDC, R2_COPYPEN);
             SetBkMode(hDC,OPAQUE);
    #endif
             break;
        }
        curPen.setCapStyle(Qt::RoundCap);
}
#endif
void RCLineStyleToQt(int s,double scale,double plotscale,QPen &qPen)
{
    if(s == 0)
    {
        qPen.setStyle(Qt::SolidLine);
        return;
    }
    QVector<qreal> qdashes;
    double width = qPen.widthF();
    LineStyle *linestyle = db.linestyles.indextostyle(s);
    for(int i=0; i<linestyle->getnsegments(); i++)
    {
        double dash,space;
        linestyle->getsegment(i,&dash,&space);
        qdashes << (dash * scale * width / plotscale);
        qdashes << (space * scale * width / plotscale);
    }
    qPen.setDashPattern(qdashes);
}

void Surface::setup(double linestylescale,double length,int dm,int c,int s,int w,PatternLineStyle *pls)
{RCHPEN hPen,hOldpen;
 int penwidth;

 /*
  qDebug() << "======Surface Setup==========";
  qDebug() << "length :" << length;
  qDebug() << "scale :" << scale;
  qDebug() << "plotscale :" << plotscale;
  qDebug() << "linestylescale :" << linestylescale;
  qDebug() << "scale / plotscale / linestylescale :" << scale / plotscale / linestylescale;
  qDebug() << "length / scale :" << length / scale;
  qDebug() << "length / scale / plotscale :" << length / scale / plotscale;
  qDebug() << "==============================";
*/

  if (dm == drawmode && c == colour && s == style && w == weight && pls == patternlinestyle)
    {  if (linestyle != NULL)
         linestyle->start(scale / plotscale / linestylescale, length / scale );// r0
         //linestyle->start(plotscale / linestylescale,length);// r1
       if (patternlinestyle != NULL)
         patternlinestyle->start(scale,plotscale);
       return;
    }
  if (allocatedpens > 0)
    {
       hPen  = CreatePen(PS_SOLID,0,colourindex[c % allocatedpens]);
       curPen = (QPen)*hPen;
       delete hPen;
       SetTextColor(hDC,colourindex[c % allocatedpens]);
       RCCOLORREF txcolref = colourindex[c % allocatedpens];
       curTextColour = QColor(GetRValue(txcolref),GetGValue(txcolref),GetBValue(txcolref));
       //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
       //DeleteObject(hOldpen);
       //return;
    }

  if (outfile != 0 && fileformat == 0)
    fprintf(outfile,"SP%d;",c);

  drawmode = dm;  colour = c;  style = s;  weight = w;
  patternlinestyle = pls;
  linestyle = 0;
  if (patternlinestyle != 0)
    patternlinestyle->start(scale,plotscale);
  else
    {  if (dm == DM_SELECT || dm == DM_ERASE)
         linestyle = NULL;
       else if ((linestyle = db.linestyles.indextostyle(s)) != NULL)
         linestyle->start(scale / plotscale / linestylescale,length / scale);// r0
         //linestyle->start(plotscale / linestylescale,length);// r1
    }

  // get the pixel width from the mm line weight
  if( !db.getlineweightdisplay() )
      penwidth = db.lineweights.weighttopx(0);
  else if(dm == DM_INVERT) // rubberband mode match it with opengl widths
      penwidth = db.lineweights.weighttopx(w);
  else if(dm == DM_PRINT) // printing mode use dpmm
      //penwidth = printydotspermm * db.lineweights.weighttopxprint(w);
      penwidth = int(printydotspermm * db.lineweights.weighttomm(w) + 0.5);
      //penwidth = int(printydotspermm * db.lineweights.weighttopx(w));
  else
      penwidth = int((ydotspermm < 3 ? 3 : ydotspermm) * db.lineweights.weighttomm(w) + 0.5);
  // get the pixel width from the fixed pixel width
  curWeight = penwidth;

  //qDebug() << "setup : ydotspermm : " << ydotspermm;
  //qDebug() << "setup : penWidth : " << penwidth;

  switch (drawmode)
    {  case DM_NORMAL :
       case DM_PRINT :
         if (blackandwhite == 1) // back and white
         {
              if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
                colourref = fillcolourref = RGB(255,255,255);
              else
                colourref = fillcolourref = RGB(0,0,0);
         }
         else if(blackandwhite == 2) // greyscale
         {
             if (db.colourtable[c].red == 255 && db.colourtable[c].green == 255 && db.colourtable[c].blue == 255)
               colourref = fillcolourref = RGB(255,255,255);
             else
             {
                 // using the Luminosity method
                 int grey = (db.colourtable[c].red*0.21+db.colourtable[c].green*0.72+db.colourtable[c].blue*0.07);
                 colourref = fillcolourref = RGB(grey,grey,grey);
             }
         }
         else
#ifdef USING_WIDGETS
            colourref = fillcolourref = RGB(db.colourtable[c].red,db.colourtable[c].green,db.colourtable[c].blue);
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));;

#if MAJOR_VERSION > 7
         // Enhancement p1 for v8 reverse 100% white to black
         // 0 = no change; 1 = automatic; 2 = force reverse
         if(reverseblackwhite == 2 || reverseblackwhite == 1)
         {
             // change white to black
             if(GetRValue(colourref)==255 && GetGValue(colourref)==255 && GetBValue(colourref)==255)
                  colourref = fillcolourref = RGB(0,0,0);
         }
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));;
#endif
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         curPen = (QPen)*hPen;
         //if(drawmode == DM_PRINT && style != 0)
         //   RCLineStyleToQt(style,scale,plotscale,curPen);
         delete hPen;
#else
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetTextColor(hDC,colourref);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_INVERT :
#ifdef USING_WIDGETS
         //colourref = fillcolourref = RGB(0,0,0);
         //*
         colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                         GetGValue(background)==127?255:255-GetGValue(background),
                                         GetBValue(background)==127?255:255-GetBValue(background));
         //*/
         /*
         colourref = fillcolourref = RGB(GetRValue(background)<=155?255:127,
                                         GetGValue(background)<=155?255:127,
                                         GetBValue(background)<=155?255:127);
         */
         colour = 256;  //  Invalidate the colour
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         curPen = (QPen)*hPen;
         delete hPen;
#else
         colourref = fillcolourref = RGB(255,255,255);
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetTextColor(hDC,colourref);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_NOT);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_INVERTDASH :
#ifdef USING_WIDGETS
         {
         //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
         colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                         GetGValue(background)==127?255:255-GetGValue(background),
                                         GetBValue(background)==127?255:255-GetBValue(background));
         //colourref = fillcolourref = RGB(0,0,0);
         colour = 256;  //  Invalidate the colour
         SetTextColor(hDC,colourref);
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen  = CreatePen(PS_DASH,penwidth,colourref);
         // special dashed pattern
         QVector<qreal> dashes;
         qreal space = 4;
         dashes << 8 << space;
         hPen->setDashPattern(dashes);
         //
         curPen = (QPen)*hPen;
         delete hPen;
         }
#else
         colourref = fillcolourref = background;
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_DOT,0,colourref);
         SetTextColor(hDC,colourref);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_NOT);
         SetBkMode(hDC,TRANSPARENT);
#endif
         break;
       case DM_DASH :
#ifdef USING_WIDGETS
         {
         colourref = fillcolourref = RGB(GetRValue(background)==127?255:255-GetRValue(background),
                                         GetGValue(background)==127?255:255-GetGValue(background),
                                         GetBValue(background)==127?255:255-GetBValue(background));
         //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
         colour = 256;  //  Invalidate the colour
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen  = CreatePen(PS_DASH,0,colourref);
         // special dashed pattern
         QVector<qreal> dashes;
         qreal space = 4;
         dashes << 8 << space;
         hPen->setDashPattern(dashes);
         //
         curPen = (QPen)*hPen;
         delete hPen;
         }
#else
         colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_DASH,0,colourref);
         SetTextColor(hDC,colourref);
         SetBkColor(hDC,background);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,TRANSPARENT);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
#endif
         break;
       case DM_ERASE :
#ifdef USING_WIDGETS
         colourref = fillcolourref = background;
         colour = 256;  //  Invalidate the colour
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         curPen = (QPen)*hPen;
         delete hPen;
#else
         colourref = fillcolourref = background;
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetBkColor(hDC,colourref);
         SetTextColor(hDC,colourref);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_SELECT :
#ifdef USING_WIDGETS
        {
         /*
         colourref = fillcolourref = RGB(GetRValue(background)<=127?255:127,
                                         GetGValue(background)<=127?255:127,
                                         GetBValue(background)<=127?255:127);
         */
         /*
         colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                                         GetGValue(background)<=127?255:255-GetGValue(background),
                                         GetBValue(background)<=127?255:255-GetBValue(background));
         */
         colourref = fillcolourref = RGB(GetRValue(background)<=155?255:127,
                                         GetGValue(background)<=155?255:127,
                                         GetBValue(background)<=155?255:127);
         //colourref = fillcolourref = RGB(127,127,127);
         colour = 256;  //  Invalidate the colour
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen  = CreatePen(PS_DASH,penwidth,colourref);
         // special dashed pattern
         QVector<qreal> dashes;
         // for 7x7 pixel points
         qreal space = 4;
         dashes << 1 << space;
         hPen->setDashPattern(dashes);
         //
         curPen = (QPen)*hPen;
         delete hPen;
         }
#else
         colourref = fillcolourref = RGB(127,127,127);
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_DOT,penwidth,colourref);
         SetTextColor(hDC,colourref);
         SetBkColor(hDC,background);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_FLASH1 :
#ifdef USING_WIDGETS
          colourref = fillcolourref = RGB(255,0,0);
          colour = 256;  //  Invalidate the colour
          hPen  = CreatePen(PS_SOLID,penwidth,colourref);
          curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
          SetBkColor(hDC,background);
          //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
          //DeleteObject(hOldpen);
          //SetROP2(hDC, R2_COPYPEN);
          //SetBkMode(hDC,OPAQUE);
          curPen = (QPen)*hPen;
          delete hPen;
#else
         colourref = fillcolourref = RGB(255,0,0);
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetTextColor(hDC,colourref);
         SetBkColor(hDC,background);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_FLASH2 :
#ifdef USING_WIDGETS
          colourref = fillcolourref = RGB(0,255,0);
          colour = 256;  //  Invalidate the colour
          hPen  = CreatePen(PS_SOLID,penwidth,colourref);
          //SetTextColor(hDC,colourref);
          curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
          SetBkColor(hDC,background);
          //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
          //DeleteObject(hOldpen);
          //SetROP2(hDC, R2_COPYPEN);
          //SetBkMode(hDC,OPAQUE);
          curPen = (QPen)*hPen;
          delete hPen;
#else
         colourref = fillcolourref = RGB(0,255,0);
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetTextColor(hDC,colourref);
         SetBkColor(hDC,background);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
        case DM_FLASH3 :
#ifdef USING_WIDGETS
          colourref = fillcolourref = RGB(0,0,255);
          colour = 256;  //  Invalidate the colour
          hPen  = CreatePen(PS_SOLID,penwidth,colourref);
          //SetTextColor(hDC,colourref);
          curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
          //SetBkColor(hDC,background);
          //hOldpen = (RCHPEN) SelectObject(hDC,hPen);
          //DeleteObject(hOldpen);
          //SetROP2(hDC, R2_COPYPEN);
          //SetBkMode(hDC,OPAQUE);
          curPen = (QPen)*hPen;
          delete hPen;
#else
         colourref = fillcolourref = RGB(0,0,255);
         colour = 256;  //  Invalidate the colour
         hPen  = CreatePen(PS_SOLID,penwidth,colourref);
         SetTextColor(hDC,colourref);
         SetBkColor(hDC,background);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
       case DM_GRID :
#ifdef USING_WIDGETS
         //colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
         //*
         colourref = fillcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
                                         GetGValue(background)<=127?255:255-GetGValue(background),
                                         GetBValue(background)<=127?255:255-GetBValue(background));
         if(db.grid.getCustColour() == 1)
         {
             minorcolourref = RGB(::v.getreal("gd::minor.red")*255,
                                  ::v.getreal("gd::minor.green")*255,
                                  ::v.getreal("gd::minor.blue")*255);
             majorcolourref = RGB(::v.getreal("gd::major.red")*255,
                                  ::v.getreal("gd::major.green")*255,
                                  ::v.getreal("gd::major.blue")*255);
         }
         else
         {
             //minorcolourref = RGB(GetRValue(background)<=127?255:255-GetRValue(background),
             //                     GetGValue(background)<=127?255:255-GetGValue(background),
             //                     GetBValue(background)<=127?255:255-GetBValue(background));
             majorcolourref = colourref;
             // minor grid line colour
             minorcolourref = RGB(GetRValue(majorcolourref)<127?255-GetRValue(majorcolourref)-64:GetRValue(majorcolourref)-255+64,
                                  GetGValue(majorcolourref)<127?255-GetGValue(majorcolourref)-64:GetGValue(majorcolourref)-255+64,
                                  GetBValue(majorcolourref)<127?255-GetBValue(majorcolourref)-64:GetBValue(majorcolourref)-255+64);
         }
         //*/
         /*
         int rlevel = GetRValue(background);
         int glevel = GetGValue(background);
         int blevel = GetBValue(background);
         int level = (rlevel+glevel+blevel)/3;
         if(level > 128 || (glevel > 175 && rlevel <= 125 && blevel <= 125) )
            colourref = fillcolourref = RGB(0,0,0);
         else
            colourref = fillcolourref = RGB(255,255,255);
         */
          // complementary colours
         // major grid line colour
         //majorcolourref = colourref;
         // minor grid line colour
         //minorcolourref = RGB(GetRValue(majorcolourref)<127?255-GetRValue(majorcolourref)-64:GetRValue(majorcolourref)-255+64,
         //                     GetGValue(majorcolourref)<127?255-GetGValue(majorcolourref)-64:GetGValue(majorcolourref)-255+64,
         //                     GetBValue(majorcolourref)<127?255-GetBValue(majorcolourref)-64:GetBValue(majorcolourref)-255+64);

         // always black and gray or white and gray
         /*
         int level = (GetRValue(background)+GetGValue(background)+GetBValue(background))/3;
         majorcolourref = level <= 127 ? RGB(255,255,255) : RGB(0,0,0);
         minorcolourref = RGB(192,192,192);
         */
         // always black and gray
         /*
         majorcolourref = RGB(73,79,105);
         minorcolourref = RGB(50,55,72);
         */

         colour = 256;  //  Invalidate the colour
         curTextColour = QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref));
         hPen = CreatePen(PS_SOLID,0,colourref);
         curPen = (QPen)*hPen;
         delete hPen;
#else
         colourref = fillcolourref = RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background));
         colour = 256;  //  Invalidate the colour
         hPen = CreatePen(PS_SOLID,0,colourref);
         SetTextColor(hDC,colourref);
         hOldpen = (RCHPEN) SelectObject(hDC,hPen);
         DeleteObject(hOldpen);
         SetROP2(hDC, R2_COPYPEN);
         SetBkMode(hDC,OPAQUE);
#endif
         break;
    }
    curPen.setCapStyle(Qt::RoundCap);
}

#ifdef USING_WIDGETS
void Surface::lineGL(double x1,double y1,double z1,double x2,double y2,double z2,int alreadytransformed)
{
    double x3,y3,z3,x4,y4,z4;
    double sx3,sy3,sz3,sx4,sy4,sz4;
    int delta;

    //qDebug() << "entering Surface::line";

    //curPen.setColor(QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref)));
    //curPen.setWidth(curWeight);
    //curPen.setCapStyle(Qt::RoundCap);

    //QGraphicsItem *qline = 0;
    QGraphicsView *widget = 0;

    if(getWidget() && getWidget()->objectName() == "RCView")
        widget = (QGraphicsView*)((RCView*)getWidget())->view();
    else if(getWidget() && getWidget()->objectName() == "RCGraphicsView")
        widget = (QGraphicsView*)getWidget();

    if(widget == 0)
        if(cadwindow->getcurrentwindow())
            widget = (QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view();

    //qDebug() << "surface::line current widget : " << widget;
    //qDebug() << "surface::line current scene  : " << getScene();

    //qDebug() << "gethwnd()      : " << gethwnd();
    //qDebug() << "getWidget()    : " << widget;
    //qDebug() << "getScene()     : " << getScene();
    //qDebug() << "activeWindow() : " << getScene()->activeWindow();

    //if(gethwnd() == 0)
     //   return 0;

    //if(getScene() == 0)
    //    return 0;

    if(widget == 0)
        return ;

    //qDebug() << "view widget" << ((QWidget*)gethwnd())->objectName();
    /*
    QPainter painter;
    if(getScene() == 0 && gethwnd() && (((QWidget*)gethwnd())->objectName() == "QWidget"
        || ((QWidget*)gethwnd())->objectName() == "QGLWidget") )
    {
        QPen pen;
        painter.begin((QWidget*)gethwnd());
        painter.setPen(pen);
        painter.drawLine(0,0,((QWidget*)gethwnd())->width(),((QWidget*)gethwnd())->height());
     }
     */

    if (GetDeviceCaps(hDC,TECHNOLOGY) == DT_RASPRINTER)
      delta = 3;
    else
      delta = 1;
    delta = 0;



    if (alreadytransformed)
      {
             DrawLine(Point(x1,y1,z1),Point(x2,y2,z2),curGLWeight);
             //DrawLine(Point(x1,y1,z1),Point(x2,y2,z2),curMMWeight);
             //glBegin(GL_LINES);
             //glVertex3f(x1,y1,z1);
             //glVertex3f(x2,y2,z2);
             //glEnd();

         if (outfile != 0)
           {  if (fileformat == 0)
                {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long((height - 1.0 - y1)*40.0*scale/ydotspermm));
                   fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long((height - 1.0 - y2)*40.0*scale/ydotspermm));
                }
              else if (fileformat == 1)
                {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                   fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                   nparameter += 2;
                }
              else if (fileformat == 2)
                {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   ndirectory += 2;
                }
           }
      }
    else if (patternlinestyle != 0)
      {
        if (transform)
           {
              //x3 = a1 * x1 + b1 * y1 + c1;
              //y3 = a2 * x1 + b2 * y1 + c2;
              //x4 = a1 * x2 + b1 * y2 + c1;
              //y4 = a2 * x2 + b2 * y2 + c2;
              lastx = x4;  lasty = y4; lastz = z4;
              //if (clip2d(&x3,&y3,&x4,&y4))
              patternlinestyle->drawGL(this,x3,y3,z3,x4,y4,z4);
           }
         else
           {
              //x3 = x1;  y3 = height - 1.0 - y1;  z3 = 0; x4 = x2;  y4 = height - 1.0 - y2; z4 = 0;
              x3 = x1;  y3 = y1;  z3 = 0; x4 = x2;  y4 = y2; z4 =0;
              lastx = x4;  lasty = y4; lastz = z4;

              patternlinestyle->drawGL(this,x3,y3,z3,x4,y4,z4);
           }
      }
    else if (linestyle != NULL)
      {
        if (transform)
           {  //x3 = a1 * x1 + b1 * y1 + c1;
              //y3 = a2 * x1 + b2 * y1 + c2;
              //x4 = a1 * x2 + b1 * y2 + c1;
              //y4 = a2 * x2 + b2 * y2 + c2;
              //if (clip2d(&x3,&y3,&x4,&y4))
              linestyle->drawGL(this,x3,y3,z3,x4,y4,z4);
              lastx = x4;  lasty = y4; lastz = z4;
           }
         else
           {
              //x3= x1;  y3 = height - 1.0 - y1; z3=z1;  x4 = x2;  y4 = height - 1.0 - y2; z4=z2;
              x3= x1;  y3 = y1; z3=z1;  x4 = x2;  y4 = y2; z4=z2;
              linestyle->drawGL(this,x3,y3,z3,x4,y4,z4);
              lastx = x4;  lasty = y4; lastz = z4;
           }
      }
    else if (transform)
      {
         //x3 = a1 * x1 + b1 * y1 + c1;
         //y3 = a2 * x1 + b2 * y1 + c2;
         //x4 = a1 * x2 + b1 * y2 + c1;
         //y4 = a2 * x2 + b2 * y2 + c2;
         lastx = x4;  lasty = y4; lastz=z4;
         //if (clip2d(&x3,&y3,&x4,&y4))
         {

                  DrawLine(Point(x3,y3,z3),Point(x4,y4,z4),curGLWeight);
                  //DrawLine(Point(x3,y3,z3),Point(x4,y4,z4),curMMWeight);
                  //glBegin(GL_LINES);
                  //glVertex3f(x3,y3,z3);
                  //glVertex3f(x4,y4,z4);
                  //glEnd();

              if (outfile != 0)
              {
                  if (fileformat == 0)
                  {
                        fprintf(outfile,"PU%d,%d;",long(x3*40.0*scale/xdotspermm),long(y3*40.0*scale/ydotspermm));
                        fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long(y4*40.0*scale/ydotspermm));
                  }
                  else if (fileformat == 1)
                  {
                        fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                        fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                        nparameter += 2;
                  }
                  else if (fileformat == 2)
                  {
                        fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x3*scale/xdotspermm,y3*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                        fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x4*scale/xdotspermm,y4*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                        ndirectory += 2;
                  }
              }
           }
      }
    else
      {
             DrawLine(Point(x1,y1,z1),Point(x2,y2,z2),curGLWeight);
             //DrawLine(Point(x1,y1,z1),Point(x2,y2,z2),curMMWeight);
             //glBegin(GL_LINES);
             //glVertex3f(x1,y1,z1);
             //glVertex3f(x2,y2,z2);
             //glEnd();
         //}
         if (outfile != 0)
           {  if (fileformat == 0)
                {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long(y1*40.0*scale/ydotspermm));
                   fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long(y2*40.0*scale/ydotspermm));
                }
              else if (fileformat == 1)
                {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                   fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                   nparameter += 2;
                }
              else if (fileformat == 2)
                {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   ndirectory += 2;
                }
           }

      }

      return ;
}

void Surface::line(QPainter *painter, double x1,double y1,double x2,double y2,int alreadytransformed)
{
  double x3,y3,x4,y4;
  int delta;

  //qDebug() << "entering Surface::line";

  //curPen.setColor(QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref)));
  //curPen.setWidth(curWeight);
  //curPen.setCapStyle(Qt::RoundCap);

  QGraphicsItem *qline = 0;
  QGraphicsView *widget = 0;

  if(getWidget() && getWidget()->objectName() == "RCView")
      widget = (QGraphicsView*)((RCView*)getWidget())->view();
  else if(getWidget() && getWidget()->objectName() == "RCGraphicsView")
      widget = (QGraphicsView*)getWidget();

  if(widget == 0)
      widget = (QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view();
      //widget = (QGraphicsView*)cadwindow->getcurrentwindow()->getWidget();

  //qDebug() << "surface::line current widget : " << widget;
  //qDebug() << "surface::line current scene  : " << getScene();

  //qDebug() << "gethwnd()      : " << gethwnd();
  //qDebug() << "getWidget()    : " << widget;
  //qDebug() << "getScene()     : " << getScene();
  //qDebug() << "activeWindow() : " << getScene()->activeWindow();

  //if(gethwnd() == 0)
   //   return 0;

  if(getScene() == 0)
      return;

  if(widget == 0)
      return;

  //qDebug() << "view widget" << ((QWidget*)gethwnd())->objectName();

  if (GetDeviceCaps(hDC,TECHNOLOGY) == DT_RASPRINTER)
    delta = 3;
  else
    delta = 1;
  delta = 0;

  if (alreadytransformed)
    {
       QPointF qp1 = widget->mapToScene(x1,y1);
       QPointF qp2 = widget->mapToScene(x2,y2);

       //MoveToEx(hDC,int(x1),int(y1),0);
       if (int(x1) == int(x2) && int(y1) == int(y2))
       {
           //LineTo(hDC,int(x2)+delta,int(y2));
           //getScene()->addLine(int(x1),int(y1),int(x2)+delta,int(y2));
           if(getScene())
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
       }
       else
       {
           //LineTo(hDC,int(x2),int(y2));
           //getScene()->addLine(int(x1),int(y1),int(x2),int(y2));
           if(getScene())
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
       }
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long((height - 1.0 - y1)*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long((height - 1.0 - y2)*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }
    }
  else if (patternlinestyle != 0)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
              patternlinestyle->draw(painter,this,x3,y3,x4,y4);
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            patternlinestyle->draw(painter,this,x3,y3,x4,y4);
         }
    }
  else if (linestyle != NULL)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
              linestyle->draw(painter,this,x3,y3,x4,y4);
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            linestyle->draw(painter,this,x3,y3,x4,y4);
         }
    }
  else if (transform)
    {
       x3 = a1 * x1 + b1 * y1 + c1;
       y3 = a2 * x1 + b2 * y1 + c2;
       x4 = a1 * x2 + b1 * y2 + c1;
       y4 = a2 * x2 + b2 * y2 + c2;
       lastx = x4;  lasty = y4;
       if (clip2d(&x3,&y3,&x4,&y4))
       {
            //MoveToEx(hDC,int(x3),int(y3),0);
            QPointF qp3 = widget->mapToScene(x3,y3);
            QPointF qp4 = widget->mapToScene(x4,y4);

            if (int(x3) == int(x4) && int(y3) == int(y4))
            {
                //LineTo(hDC,int(x4)+delta,int(y4));
                //getScene()->addLine(int(x3),int(y3),int(x4)+delta,int(y4));
                //if(getScene())
                //    qline = getScene()->addLine(qp3.x(),qp3.y(),qp4.x(),qp4.y(),curPen);
                painter->drawPoint(x4,y4);
            }
            else
            {
                //LineTo(hDC,int(x4),int(y4));
                //getScene()->addLine(int(x3),int(y3),int(x4),int(y4));
                //if(getScene())
                //    qline = getScene()->addLine(qp3.x(),qp3.y(),qp4.x(),qp4.y(),curPen);
                painter->drawLine(x3,y3,x4,y4);
            }
            if (outfile != 0)
            {
                if (fileformat == 0)
                {
                      fprintf(outfile,"PU%d,%d;",long(x3*40.0*scale/xdotspermm),long(y3*40.0*scale/ydotspermm));
                      fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long(y4*40.0*scale/ydotspermm));
                }
                else if (fileformat == 1)
                {
                      fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                      fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                      nparameter += 2;
                }
                else if (fileformat == 2)
                {
                      fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x3*scale/xdotspermm,y3*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x4*scale/xdotspermm,y4*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      ndirectory += 2;
                }
            }
         }
    }
  else
    {
      // add the view to scene transform
      //qDebug() << "surface height in surface::line : " << height;
      //QPointF qp1 = widget->mapToScene(x1,height - 1.0 - y1);
      //QPointF qp2 = widget->mapToScene(x2,height - 1.0 - y2);
      QPointF qp1(x1,double(height) - 1.0 - y1);
      QPointF qp2(x2,double(height) - 1.0 - y2);

      //MoveToEx(hDC,int(x1),int(height - 1.0 - y1),0);
       if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
       {
            //getScene()->addLine(int(x1),int(height - 1.0 - y1),int(x2)+delta,int(height - 1.0 - y2));
               //qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
               //qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
           //painter->drawLine(int(qp1.x()),int(qp1.y()),int(qp2.x()),int(qp2.y()));
           painter->drawLine(qp1,qp2);
       }
       else
       {
           //getScene()->addLine(int(x1),int(height - 1.0 - y1),int(x2),int(height - 1.0 - y2));
               //qline = new QGraphicsLineItem(qp1.x(),qp1.y(),qp2.x(),qp2.y());
               //((QGraphicsLineItem*)qline)->setPen(curPen);
               //qline = getScene()->addLine((int)qp1.x()-0.5,(int)qp1.y()-0.5,(int)qp2.x()-0.5,(int)qp2.y()-0.5,curPen);
               //qline = getScene()->addLine(qp1.x()-0.5,qp1.y()-0.5,qp2.x()-0.5,qp2.y()-0.5,curPen);
               //qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
               //painter->drawLine(int(qp1.x()),int(qp1.y()),int(qp2.x()),int(qp2.y()));
               painter->drawLine(qp1,qp2);
       }
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long(y1*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long(y2*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }
    }

    return;
}

void Surface::line(QPainterPath *path, double x1,double y1,double x2,double y2,int alreadytransformed)
{
    double x3,y3,x4,y4;
    int delta;

    if (alreadytransformed)
    {
         QPointF qp1(x1,y1);
         QPointF qp2(x2,y2);

         //if(path)
         //    path->moveTo(qp1.x(),qp1.y());
         //MoveToEx(hDC,int(x1),int(y1),0);
         //if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
         if (int(x1) == int(x2) && int(y1) == int(y2))
         {
             //LineTo(hDC,int(x2)+delta,int(y2));
             //getScene()->addLine(int(x1),int(y1),int(x2)+delta,int(y2));
             if(path)
             {
                 path->moveTo(qp1.x(),qp1.y());
                 path->lineTo(qp2.x(),qp2.y());
             }
         }
         else
         {
             //LineTo(hDC,int(x2),int(y2));
             if(path)
             {
                 path->moveTo(qp1.x(),qp1.y());
                 path->lineTo(qp2.x(),qp2.y());
             }
         }
         if (outfile != 0)
           {  if (fileformat == 0)
                {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long((height - 1.0 - y1)*40.0*scale/ydotspermm));
                   fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long((height - 1.0 - y2)*40.0*scale/ydotspermm));
                }
              else if (fileformat == 1)
                {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                   fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                   nparameter += 2;
                }
              else if (fileformat == 2)
                {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   ndirectory += 2;
                }
           }
      }
    else if (patternlinestyle != 0)
      {
        if (transform)
           {  x3 = a1 * x1 + b1 * y1 + c1;
              y3 = a2 * x1 + b2 * y1 + c2;
              x4 = a1 * x2 + b1 * y2 + c1;
              y4 = a2 * x2 + b2 * y2 + c2;
              lastx = x4;  lasty = y4;
              if (clip2d(&x3,&y3,&x4,&y4))
                  patternlinestyle->draw(path,this,x3,y3,x4,y4);
                  //patternlinestyle->draw(this,x3,y3,x4,y4);
           }
         else
           {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
              lastx = x4;  lasty = y4;
              patternlinestyle->draw(path, this,x3,y3,x4,y4);
              //patternlinestyle->draw(this,x3,y3,x4,y4);
           }
      }
    else if (linestyle != NULL)
      {
        if (transform)
           {  x3 = a1 * x1 + b1 * y1 + c1;
              y3 = a2 * x1 + b2 * y1 + c2;
              x4 = a1 * x2 + b1 * y2 + c1;
              y4 = a2 * x2 + b2 * y2 + c2;
              lastx = x4;  lasty = y4;
              if (clip2d(&x3,&y3,&x4,&y4))
                linestyle->draw(path,this,x3,y3,x4,y4);
           }
         else
           {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
              lastx = x4;  lasty = y4;
              linestyle->draw(path,this,x3,y3,x4,y4);
           }
      }
    else if (transform)
      {
         x3 = a1 * x1 + b1 * y1 + c1;
         y3 = a2 * x1 + b2 * y1 + c2;
         x4 = a1 * x2 + b1 * y2 + c1;
         y4 = a2 * x2 + b2 * y2 + c2;
         lastx = x4;  lasty = y4;
         if (clip2d(&x3,&y3,&x4,&y4))
         {
              //MoveToEx(hDC,int(x3),int(y3),0);
              //QPointF qp3 = widget->mapToScene(x3,y3);
              //QPointF qp4 = widget->mapToScene(x4,y4);
              QPointF qp3(x3,y3);
              QPointF qp4(x4,y4);

              if (int(x3) == int(x4) && int(y3) == int(y4))
              {
                  if(path)
                  {
                      path->moveTo(qp3.x(),qp3.y());
                      path->lineTo(qp4.x(),qp4.y());
                  }
              }
              else
              {
                  if(path)
                  {
                      path->moveTo(qp3.x(),qp3.y());
                      path->lineTo(qp4.x(),qp4.y());
                  }
              }
              if (outfile != 0)
              {
                  if (fileformat == 0)
                  {
                        fprintf(outfile,"PU%d,%d;",long(x3*40.0*scale/xdotspermm),long(y3*40.0*scale/ydotspermm));
                        fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long(y4*40.0*scale/ydotspermm));
                  }
                  else if (fileformat == 1)
                  {
                        fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                        fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                        nparameter += 2;
                  }
                  else if (fileformat == 2)
                  {
                        fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x3*scale/xdotspermm,y3*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                        fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x4*scale/xdotspermm,y4*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                        ndirectory += 2;
                  }
              }
           }
      }
    else
      {
        // add the view to scene transform
        QPointF qp1(x1,double(height) - 1.0 - y1);
        QPointF qp2(x2,double(height) - 1.0 - y2);

        if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
         {
             if(path)
             {
                 path->moveTo(qp1.x(),qp1.y());
                 path->lineTo(qp2.x(),qp2.y());
             }
         }
         else
         {
             //LineTo(hDC,int(x2),int(height - 1.0 - y2));
             if(path)
             {
                 path->moveTo(qp1.x(),qp1.y());
                 path->lineTo(qp2.x(),qp2.y());
             }
         }
         if (outfile != 0)
           {  if (fileformat == 0)
                {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long(y1*40.0*scale/ydotspermm));
                   fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long(y2*40.0*scale/ydotspermm));
                }
              else if (fileformat == 1)
                {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                   fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                   nparameter += 2;
                }
              else if (fileformat == 2)
                {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                   ndirectory += 2;
                }
           }

      }
}

QGraphicsItem* Surface::line( double x1,double y1,double x2,double y2,int alreadytransformed)
{
    return line((QGraphicsItem*)0,x1,y1,x2,y2,alreadytransformed);
}

QGraphicsItem* Surface::line(QGraphicsItem *pgLine, double x1,double y1,double x2,double y2,int alreadytransformed)
{
  double x3,y3,x4,y4;
  int delta;

  //qDebug() << "entering Surface::line";

  //curPen.setColor(QColor(GetRValue(colourref),GetGValue(colourref),GetBValue(colourref)));
  //curPen.setWidth(curWeight);
  //curPen.setCapStyle(Qt::RoundCap);


  QGraphicsItem *qline = 0;
  QGraphicsView *widget = 0;

  //qDebug() << "scene  " << getScene()->objectName();
  //qDebug() << "widget " << getWidget();
  //if(getWidget())
  //    qDebug() << "widget object name " << getWidget()->objectName();

  if(getWidget() && getWidget()->objectName() == "RCView")
      widget = (QGraphicsView*)((RCView*)getWidget())->view();
  else if(getWidget() && getWidget()->objectName() == "RCGraphicsView")
      widget = (QGraphicsView*)getWidget();
  //else if(getScene()->objectName() != "PrinterScene")
  //    widget = (QGraphicsView*)getWidget();

  if(widget == 0)
  {
      //if(getScene()->objectName() != "PrinterScene")
      //    widget = (QGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view();
      widget = ((RCView*)cadwindow->getcurrentwindow()->getWidget())->view();
  }

  double dpr = 1.0;
  if(widget != 0)
      widget->devicePixelRatio();

  //qDebug() << "surface::line current widget : " << widget;
  //qDebug() << "surface::line current scene  : " << getScene();

  //qDebug() << "gethwnd()      : " << gethwnd();
  //qDebug() << "getWidget()    : " << widget;
  //qDebug() << "getScene()     : " << getScene();
  //qDebug() << "activeWindow() : " << getScene()->activeWindow();

  //if(gethwnd() == 0)
   //   return 0;

  if(getScene() == 0)
      return 0;

  if(widget == 0 && getScene()->objectName() != "PrinterScene")
      return 0;

  //qDebug() << "view widget" << ((QWidget*)gethwnd())->objectName();
  /*
  QPainter painter;
  if(getScene() == 0 && gethwnd() && (((QWidget*)gethwnd())->objectName() == "QWidget"
      || ((QWidget*)gethwnd())->objectName() == "QGLWidget") )
  {
      QPen pen;
      painter.begin((QWidget*)gethwnd());
      painter.setPen(pen);
      painter.drawLine(0,0,((QWidget*)gethwnd())->width(),((QWidget*)gethwnd())->height());
   }
   */

  if (GetDeviceCaps(hDC,TECHNOLOGY) == DT_RASPRINTER)
    delta = 3;
  else
    delta = 1;
  delta = 0;



  if (alreadytransformed)
    {
       //QPointF qp1 = widget->mapToScene(x1,y1);
       //QPointF qp2 = widget->mapToScene(x2,y2);
       QPointF qp1(x1,y1);
       QPointF qp2(x2,y2);

       if(pgLine)
           if(getScene())
               getScene()->removeItem(pgLine);
       delete pgLine;
       pgLine=0;
       //MoveToEx(hDC,int(x1),int(y1),0);
       //if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
       if (int(x1) == int(x2) && int(y1) == int(y2))
       {
           //LineTo(hDC,int(x2)+delta,int(y2));
           //getScene()->addLine(int(x1),int(y1),int(x2)+delta,int(y2));
           if(getScene())
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
       }
       else
       {
           //LineTo(hDC,int(x2),int(y2));
           //getScene()->addLine(int(x1),int(y1),int(x2),int(y2));
           if(getScene())
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
       }
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long((height - 1.0 - y1)*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long((height - 1.0 - y2)*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }
    }
  else if (patternlinestyle != 0)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
                qline = patternlinestyle->draw(qline,this,x3,y3,x4,y4);
                //patternlinestyle->draw(this,x3,y3,x4,y4);
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            qline = patternlinestyle->draw(qline, this,x3,y3,x4,y4);
            //patternlinestyle->draw(this,x3,y3,x4,y4);
         }
    }
  else if (linestyle != NULL)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
              qline = linestyle->draw(qline,this,x3,y3,x4,y4);
              //qline = linestyle->draw(qline,this,x3,y3,x4,y4);
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            qline = linestyle->draw(qline,this,x3,y3,x4,y4);
            //qline = linestyle->draw(qline,this,x3,y3,x4,y4);
         }
    }
  else if (transform)
    {
       x3 = a1 * x1 + b1 * y1 + c1;
       y3 = a2 * x1 + b2 * y1 + c2;
       x4 = a1 * x2 + b1 * y2 + c1;
       y4 = a2 * x2 + b2 * y2 + c2;
       lastx = x4;  lasty = y4;
       if (clip2d(&x3,&y3,&x4,&y4))
       {
            //MoveToEx(hDC,int(x3),int(y3),0);
            //QPointF qp3 = widget->mapToScene(x3,y3);
            //QPointF qp4 = widget->mapToScene(x4,y4);
            QPointF qp3(x3,y3);
            QPointF qp4(x4,y4);

            if (int(x3) == int(x4) && int(y3) == int(y4))
            {
                if(pgLine)
                    if(getScene())
                        getScene()->removeItem(pgLine);
                delete pgLine;
                pgLine = 0;
                //LineTo(hDC,int(x4)+delta,int(y4));
                //getScene()->addLine(int(x3),int(y3),int(x4)+delta,int(y4));
                if(getScene())
                    qline = getScene()->addLine(qp3.x(),qp3.y(),qp4.x(),qp4.y(),curPen);
            }
            else
            {
                if(pgLine)
                    if(getScene())
                        getScene()->removeItem(pgLine);
                delete pgLine;
                pgLine = 0;
                //LineTo(hDC,int(x4),int(y4));
                //getScene()->addLine(int(x3),int(y3),int(x4),int(y4));
                //qDebug() << "curPen : " << curPen;
                if(getScene())
                    qline = getScene()->addLine(qp3.x(),qp3.y(),qp4.x(),qp4.y(),curPen);
            }
            if (outfile != 0)
            {
                if (fileformat == 0)
                {
                      fprintf(outfile,"PU%d,%d;",long(x3*40.0*scale/xdotspermm),long(y3*40.0*scale/ydotspermm));
                      fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long(y4*40.0*scale/ydotspermm));
                }
                else if (fileformat == 1)
                {
                      fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                      fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                      nparameter += 2;
                }
                else if (fileformat == 2)
                {
                      fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x3*scale/xdotspermm,y3*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x4*scale/xdotspermm,y4*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      ndirectory += 2;
                }
            }
         }
    }
  else
    {
      // add the view to scene transform
      //qDebug() << "surface height in surface::line : " << height;
      //qDebug() << "surface width  in surface::line : " << width;
      //qDebug() << "surface widget : " << widget;
      //qDebug() << "surface hwnd : " << (QObject*)hWnd;
      //qDebug() << "surface hwnd height : " << ((RCView*)hDC)->height();
      //qDebug() << "surface hwnd width  : " << ((RCView*)hDC)->width();
      //qDebug() << "surface hwnd viewport height : " << ((RCView*)hDC)->viewport()->height();
      //qDebug() << "surface hwnd viewport width  : " << ((RCView*)hDC)->viewport()->width();
      //QPointF qp1 = widget->mapToScene(x1+0.5,height - 1.0 - y1+0.5);
      //QPointF qp2 = widget->mapToScene(x2+0.5,height - 1.0 - y2+0.5);
      //QPointF qp1(x1+0.5,double(((QGraphicsView*)widget)->viewport()->height()) - y1);
      //QPointF qp2(x2+0.5,double(((QGraphicsView*)widget)->viewport()->height()) - y2);
      QPointF qp1(x1,double(height) - 1.0 - y1);
      QPointF qp2(x2,double(height) - 1.0 - y2);
      //QPointF qp1(x1+0.5,double(((RCView*)hDC)->viewport()->height()) - y1);
      //QPointF qp2(x2+0.5,double(((RCView*)hDC)->viewport()->height()) - y2);

      //MoveToEx(hDC,int(x1),int(height - 1.0 - y1),0);
       if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
       {
           if(pgLine)
               if(getScene())
                    getScene()->removeItem(pgLine);
           delete pgLine;         //LineTo(hDC,int(x2)+delta,int(height - 1.0 - y2));
           pgLine=0;
         //getScene()->addLine(int(x1),int(height - 1.0 - y1),int(x2)+delta,int(height - 1.0 - y2));
           if(getScene())
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
               //qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
       }
       else
       {
           //LineTo(hDC,int(x2),int(height - 1.0 - y2));
           if(pgLine)
               if(getScene())
                   getScene()->removeItem(pgLine);
           delete pgLine;
           pgLine=0;
           //getScene()->addLine(int(x1),int(height - 1.0 - y1),int(x2),int(height - 1.0 - y2));
           if(getScene())
           {
               //qline = new QGraphicsLineItem(qp1.x(),qp1.y(),qp2.x(),qp2.y());
               //((QGraphicsLineItem*)qline)->setPen(curPen);
               //qline = getScene()->addLine((int)qp1.x()-0.5,(int)qp1.y()-0.5,(int)qp2.x()-0.5,(int)qp2.y()-0.5,curPen);
               //qline = getScene()->addLine(qp1.x()-0.5,qp1.y()-0.5,qp2.x()-0.5,qp2.y()-0.5,curPen);
               //qDebug() << "curPen.width : " << curPen.width();
               //qDebug() << "curPen.widthF : " << curPen.widthF();
               //qDebug() << "sceneRect : " << getScene()->sceneRect();
               qline = getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y(),curPen);
           }
       }
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long(y1*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long(y2*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }

    }

    return qline;
}
#else
void Surface::line(double x1,double y1,double x2,double y2,int alreadytransformed)
    double x3,y3,x4,y4;
 int delta;

  if (GetDeviceCaps(hDC,TECHNOLOGY) == DT_RASPRINTER)
    delta = 3;
  else
    delta = 1;

  if (alreadytransformed)
    {
      MoveToEx(hDC,int(x1),int(y1),0);
       if (int(x1) == int(x2) && int(y1) == int(y2))
         LineTo(hDC,int(x2)+delta,int(y2));
       else
         LineTo(hDC,int(x2),int(y2));
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long((height - 1.0 - y1)*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long((height - 1.0 - y2)*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }
    }
  else if (patternlinestyle != 0)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
              patternlinestyle->draw(this,x3,y3,x4,y4);
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            patternlinestyle->draw(this,x3,y3,x4,y4);
         }
    }
  else if (linestyle != NULL)
    {
      if (transform)
         {  x3 = a1 * x1 + b1 * y1 + c1;
            y3 = a2 * x1 + b2 * y1 + c2;
            x4 = a1 * x2 + b1 * y2 + c1;
            y4 = a2 * x2 + b2 * y2 + c2;
            lastx = x4;  lasty = y4;
            if (clip2d(&x3,&y3,&x4,&y4))
              linestyle->draw(this,int(x3),int(y3),int(x4),int(y4));
         }
       else
         {  x3 = x1;  y3 = height - 1.0 - y1;  x4 = x2;  y4 = height - 1.0 - y2;
            lastx = x4;  lasty = y4;
            linestyle->draw(this,int(x3),int(y3),int(x4),int(y4));
         }
    }
  else if (transform)
    {
      x3 = a1 * x1 + b1 * y1 + c1;
       y3 = a2 * x1 + b2 * y1 + c2;
       x4 = a1 * x2 + b1 * y2 + c1;
       y4 = a2 * x2 + b2 * y2 + c2;
       lastx = x4;  lasty = y4;
       if (clip2d(&x3,&y3,&x4,&y4))
         {  MoveToEx(hDC,int(x3),int(y3),0);
            if (int(x3) == int(x4) && int(y3) == int(y4))
              LineTo(hDC,int(x4)+delta,int(y4));
            else
              LineTo(hDC,int(x4),int(y4));
            if (outfile != 0)
              {  if (fileformat == 0)
                   {  fprintf(outfile,"PU%d,%d;",long(x3*40.0*scale/xdotspermm),long(y3*40.0*scale/ydotspermm));
                      fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long(y4*40.0*scale/ydotspermm));
                   }
                 else if (fileformat == 1)
                   {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                      fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                      nparameter += 2;
                   }
                 else if (fileformat == 2)
                   {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x3*scale/xdotspermm,y3*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x4*scale/xdotspermm,y4*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                      ndirectory += 2;
                   }
              }

         }
    }
  else
    {
      MoveToEx(hDC,int(x1),int(height - 1.0 - y1),0);
       if (int(x1) == int(x2) && int(height - 1.0 - y1) == int(height - 1.0 - y2))
         LineTo(hDC,int(x2)+delta,int(height - 1.0 - y2));
       else
         LineTo(hDC,int(x2),int(height - 1.0 - y2));
       if (outfile != 0)
         {  if (fileformat == 0)
              {  fprintf(outfile,"PU%d,%d;",long(x1*40.0*scale/xdotspermm),long(y1*40.0*scale/ydotspermm));
                 fprintf(outfile,"PD%d,%d;",long(x2*40.0*scale/xdotspermm),long(y2*40.0*scale/ydotspermm));
              }
            else if (fileformat == 1)
              {  fprintf(outfile,"     110%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
                 fprintf(outfile,"     110       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
                 nparameter += 2;
              }
            else if (fileformat == 2)
              {  fprintf(outfile,"110,%18.10e,%18.10e,%18.10e,    %7dP%7d\n",x1*scale/xdotspermm,y1*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",x2*scale/xdotspermm,y2*scale/ydotspermm,0.0,ndirectory + 1,++nparameter);
                 ndirectory += 2;
              }
         }

    }
}
#endif

void Surface::moveto(double x,double y)
{ if (transform)
    {  lastx = a1 * x + b1 * y + c1;
       lasty = a2 * x + b2 * y + c2;
    }
  else
    {  lastx = x;  lasty = height - 1.0 - y;
       if (outfile != 0 && fileformat == 0)
         fprintf(outfile,"PU%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
    }
}

void Surface::moveto(QPainterPath *path, double x,double y)
{
    if (transform)
    {  lastx = a1 * x + b1 * y + c1;
       lasty = a2 * x + b2 * y + c2;
    }
    else
    {  lastx = x;  lasty = height - 1.0 - y;
       if (outfile != 0 && fileformat == 0)
         fprintf(outfile,"PU%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
    }
    if(path)
        path->moveTo(lastx,lasty);
}

void Surface::textout(int alreadytransformed,double x,double y,RCCHAR *s,int l)
{ if (alreadytransformed)
    {  lastx = x;
       lasty = y;
    }
  else if (transform)
    {  lastx = a1 * x + b1 * y + c1;
       lasty = a2 * x + b2 * y + c2;
    }
  else
    {  lastx = x;  lasty = height - 1.0 - y;
    }
#if ! defined(_WIN32_WCE)
  if (outfile != 0)
    {OUTLINETEXTMETRIC tm;
       GetOutlineTextMetrics(hDC,sizeof(tm),&tm);
       if (fileformat == 0)
         {  fprintf(outfile,"SI%.0lf,%.0lf;DI%d,%d;PU%.0lf,%.0lf;LB%s%c",tm.otmTextMetrics.tmHeight*scale/25.4/xdotspermm,tm.otmTextMetrics.tmHeight*scale/25.4/ydotspermm,tm.otmsCharSlopeRise,tm.otmsCharSlopeRun,x*40*scale/xdotspermm,(height - 1 - y - tm.otmTextMetrics.tmHeight)*40*scale/ydotspermm,s,3);
         }
       else if (fileformat == 1)
         {  fprintf(outfile,"     212%8d               0       0       0       0       0 0 0 0 0D%7d\n",nparameter+1,++ndirectory);
            fprintf(outfile,"     212       0%8d       2       0                               0D%7d\n",colour,++ndirectory);
            if (strlen(s) <= 60)
              nparameter += 3;
            else
              nparameter += 3 + (strlen(s) - 60 + 63) / 64;
         }
       else if (fileformat == 2)
         {RCCHAR t[61];
          int base;
            fprintf(outfile,"212,1,%3d,%18.10e,%18.10e,1,0.0,%10.4f,%7dP%7d\n",strlen(s),strlen(s) * 0.6 * tm.otmTextMetrics.tmHeight*scale/xdotspermm,tm.otmTextMetrics.tmHeight*scale/xdotspermm,atan2((double)tm.otmsCharSlopeRise,(double)tm.otmsCharSlopeRun)-acos(-1.0)/2.0,ndirectory + 1,++nparameter);
            fprintf(outfile,"0,0,%18.10e,%18.10e,0.0,                   %7dP%7d\n",x*scale/xdotspermm,(height - 1 - y - tm.otmTextMetrics.tmHeight)*scale/ydotspermm,ndirectory + 1,++nparameter);
            strncpy(t,s,60);  t[60] = 0;
            fprintf(outfile,"%-3.3dH%-61s%7dP%7d\n",strlen(s),t,ndirectory + 1,++nparameter);
            nparameter += 3;
            base = 60;
            while (base < (signed)strlen(s))
              {  strncpy(t,s + base,64);  t[64] = 0;
                 if (base + 64 < (signed)strlen(s))
                   fprintf(outfile,"%-64s;%7dP%7d\n",t,ndirectory + 1,++nparameter);
                 else
                   fprintf(outfile,"%-65s%7dP%7d\n",t,ndirectory + 1,++nparameter);
                 base += 64;
              }
            ndirectory += 2;
         }
    }
//WCEFIX
#endif
  SetBkMode(gethdc(),TRANSPARENT);
  if (lastx > INT_MIN / 2 && lastx < INT_MAX / 2 && lasty > INT_MIN / 2 && lasty < INT_MAX / 2)
    {SIZE size;
       GetTextExtentPoint32W(gethdc(),(RCLPCTSTR)s,l,&size);
       //if (lastx + size.cx > 0.0 && lastx - size.cx < width &&
       //    lasty + size.cy > 0.0 && lasty - size.cy < height)
         TextOutW(gethdc(),int(lastx),int(lasty),(RCLPCTSTR)s,l);
         //ExtTextOutW(gethdc(),int(lastx),int(lasty),0,0,(LPCWSTR)s,l,NULL);
    }
  SetBkMode(gethdc(),OPAQUE);
}


void Surface::lineto(QPainter *painter,double x,double y)
{
    double x4,y4,x5,y5,lx,ly;

    QGraphicsView *widget = 0;

    if(getWidget() && getWidget()->objectName() == "RCView")
        widget = (QGraphicsView*)((RCView*)getWidget())->view();
    else if(getWidget() &&  getWidget()->objectName() == "RCGraphicsView")
        widget = (QGraphicsView*)getWidget();

    if(widget == 0)
        widget = ((RCView*)cadwindow->getcurrentwindow()->getWidget())->view();

    //qDebug() << "surface::lineto current widget : " << widget;
    //qDebug() << "surface::lineto current scene  : " << getScene();

    if (linestyle != NULL)
    {
        if (transform)
        {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              if (clip2d(&lastx,&lasty,&x4,&y4))
                  linestyle->draw(painter,this,lastx,lasty,x4,y4);
              lastx = x5;  lasty = y5;
         }
         else
         {
              x4 = x;  y4 = height-1-y;
              linestyle->draw(painter,this,lastx,lasty,x4,y4);
              lastx = x4;  lasty = y4;
         }
    }
    else
    {
         if (transform)
         {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              lx = lastx;  ly = lasty;
              if (clip2d(&lastx,&lasty,&x4,&y4))
              {
                   //MoveToEx(hDC,int(lastx),int(lasty),0);
                   //LineTo(hDC,int(x4),int(y4));
                   //QPointF qplast = widget->mapToScene(lastx,lasty);
                   //QPointF qp4    = widget->mapToScene(x4,y4);

                   painter->drawLine(QPointF(lastx,lasty),QPointF(x4,y4));
                   //painter->drawLine(lastx,lasty,x4,y4);

                   if (outfile != 0)
                   {
                       if ((long(lx*40.0) != long(lastx*40.0) || long(ly*40.0) != long(lasty*40.0)))
                           fprintf(outfile,"PU%d,%d;",long(lx*40.0*scale/xdotspermm),long((height - 1.0 - ly)*40.0*scale/ydotspermm));
                        fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long((height - 1.0 - y4)*40.0*scale/ydotspermm));
                   }
              }
              lastx = x5;  lasty = y5;
         }
         else
         {
              //MoveToEx(hDC,int(lastx),int(lasty),0);

              if (int(lastx) == int(x) && int(lasty) == int(height - 1.0 - y))
              {
                 //QPointF qplast = widget->mapToScene(lastx,lasty);
                 //LineTo(hDC,int(lastx)+1,int(lasty));

                 //QPointF qp(lastx,lasty);
                 painter->drawPoint(QPointF(lastx,lasty));
                 //painter->drawPoint(lastx,lasty);
              }
              else
              {
                 // QPointF qplast = widget->mapToScene(lastx,lasty);
                  //QPointF qp2 = widget->mapToScene(x,height - 1.0 - y);
                  //QPointF qplast(lastx,lasty);
                   //QPointF qp1(lastx,lasty);
                   //QPointF qp2(x,height - 1.0 - y);

                   //painter->drawLine(floor(lastx),floor(lasty),floor(x),floor(height - 1.0 - y));
                   //painter->drawLine(lastx,lasty,x,height - 1.0 - y);
                   //painter->drawLine(QPointF(lastx,lasty),QPointF(x,height - 1.0 - y));
                   //painter->drawLine(QLineF(QPointF(lastx,lasty),QPointF(x,height - 1.0 - y)).toLine());
                   painter->drawLine(QLineF(QPointF(lastx,lasty),QPointF(x,height - 1.0 - y)));

                   lastx = x;  lasty = height - 1.0 - y;
                   //lastx = floor(x);  lasty = floor(height - 1.0 - y);
                   //LineTo(hDC,int(lastx),int(lasty));
              }

              if (outfile != 0)
                fprintf(outfile,"PD%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
         }
    }
}

void Surface::linetoGL(double x,double y,double z)
{
    DrawLine(Point(lastx,lasty,lastz),Point(x,y,z),curMMWeight);
    //glBegin(GL_LINE);
    //glVertex3f((GLfloat)lastx,(GLfloat)lasty,(GLfloat)lastz);
    //glVertex3f((GLfloat)x,(GLfloat)y,(GLfloat)z);
    //glEnd();
    lastx = x;
    lasty = y;
    lastz = x;
}

void Surface::linetoGL(double x,double y)
{
    return linetoGL( x, y, 0.0);
}

void Surface::movetoGL(double x, double y, double z)
{
    lastx = x;
    lasty = y;
    lastz = z;
}

void Surface::movetoGL(double x, double y)
{
    movetoGL(x,y,0.0);
}

//void Surface::lineto(double x,double y)
QGraphicsItem *  Surface::lineto(double x,double y)
{
    return lineto((QGraphicsItem*)0, x, y);
}

void Surface::lineto(QPainterPath *path, double x, double y)
{
    double x4,y4,x5,y5,lx,ly;

    if (linestyle != NULL)
    {
        if (transform)
        {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              if (clip2d(&lastx,&lasty,&x4,&y4))
                  linestyle->draw(path,this,lastx,lasty,x4,y4);
              lastx = x5;  lasty = y5;
         }
         else
         {
              x4 = x;  y4 = height-1-y;
              linestyle->draw(path,this,lastx,lasty,x4,y4);
              lastx = x4;  lasty = y4;
         }
    }
    else
    {
         if (transform)
         {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              lx = lastx;  ly = lasty;
              if (clip2d(&lastx,&lasty,&x4,&y4))
              {
                   QPointF qplast(lastx,lasty);
                   QPointF qp4(x4,y4);

                   if(path)
                   {
                        path->lineTo(qp4);
                   }

                   if (outfile != 0)
                   {
                       if ((long(lx*40.0) != long(lastx*40.0) || long(ly*40.0) != long(lasty*40.0)))
                           fprintf(outfile,"PU%d,%d;",long(lx*40.0*scale/xdotspermm),long((height - 1.0 - ly)*40.0*scale/ydotspermm));
                        fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long((height - 1.0 - y4)*40.0*scale/ydotspermm));
                   }
              }
              lastx = x5;  lasty = y5;
         }
         else
         {
              QPointF qplast(lastx,lasty);
              QPointF qp2(x,height - 1.0 - y);

              path->lineTo(qp2);

              lastx = x;  lasty = height - 1.0 - y;

              if (outfile != 0)
                fprintf(outfile,"PD%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
         }
    }
}

QGraphicsItem * Surface::lineto(QGraphicsItem *qgin,double x,double y)
{
#ifdef USING_WIDGETS
    double x4,y4,x5,y5,lx,ly;

    QGraphicsItem *qgout = 0;
    QGraphicsView *widget = 0;

    if(getWidget() && getWidget()->objectName() == "RCView")
        widget = (QGraphicsView*)((RCView*)getWidget())->view();
    else if(getWidget() &&  getWidget()->objectName() == "RCGraphicsView")
        widget = (QGraphicsView*)getWidget();

    if(widget == 0)
        widget = ((RCView*)cadwindow->getcurrentwindow()->getWidget())->view();

    //qDebug() << "surface::lineto current widget : " << widget;
    //qDebug() << "surface::lineto current scene  : " << getScene();

    if (linestyle != NULL)
    {
        if (transform)
        {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              if (clip2d(&lastx,&lasty,&x4,&y4))
                  qgout = linestyle->draw(qgin,this,lastx,lasty,x4,y4);
              lastx = x5;  lasty = y5;
         }
         else
         {
              x4 = x;  y4 = height-1-y;
              qgout = linestyle->draw(qgin,this,lastx,lasty,x4,y4);
              lastx = x4;  lasty = y4;
         }
    }
    else
    {
         if (transform)
         {
              x4 = x5 = a1 * x + b1 * y + c1;
              y4 = y5 = a2 * x + b2 * y + c2;
              lx = lastx;  ly = lasty;
              if (clip2d(&lastx,&lasty,&x4,&y4))
              {
                   //MoveToEx(hDC,int(lastx),int(lasty),0);
                   //LineTo(hDC,int(x4),int(y4));
                   //QPointF qplast = widget->mapToScene(lastx,lasty);
                   //QPointF qp4    = widget->mapToScene(x4,y4);
                   QPointF qplast(lastx,lasty);
                   QPointF qp4(x4,y4);

                   if(getScene() != 0)
                   {
                        if(qgin)
                           getScene()->removeItem(qgin);
                        delete qgin;
                        qgin=0;
                        qgout = getScene()->addLine(qplast.x(),qplast.y(),qp4.x(),qp4.y(),curPen);
                       //getScene()->addLine(int(lastx),int(lasty),int(x4),int(y4));
                       //QGraphicsItem *ln = m_scene->addLine(int(lastx),int(lasty),int(x4),int(y4));
                       //ln->setScale(scale);
                   }
                   if (outfile != 0)
                   {
                       if ((long(lx*40.0) != long(lastx*40.0) || long(ly*40.0) != long(lasty*40.0)))
                           fprintf(outfile,"PU%d,%d;",long(lx*40.0*scale/xdotspermm),long((height - 1.0 - ly)*40.0*scale/ydotspermm));
                        fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long((height - 1.0 - y4)*40.0*scale/ydotspermm));
                   }
              }
              lastx = x5;  lasty = y5;
         }
         else
         {
              //MoveToEx(hDC,int(lastx),int(lasty),0);

             /*
              if (int(lastx) == int(x) && int(lasty) == int(height - 1.0 - y))
              {
                 QPointF qplast(lastx,lasty);
                 //QPointF qplast = widget->mapToScene(lastx,lasty);
                //LineTo(hDC,int(lastx)+1,int(lasty));

                     if(qgin)
                         if(getScene())
                            getScene()->removeItem(qgin);
                     delete qgin;
                     qgin=0;
                     if(getScene())
                        qgout = getScene()->addLine(qplast.x(),qplast.y(),qplast.x(),qplast.y(),curPen);

              //getScene()->addLine(int(lastx),int(lasty),int(lastx)+1,int(lasty));
                //QGraphicsItem *ln = m_scene->addLine(int(lastx),int(lasty),int(lastx)+1,int(lasty));
                //ln->setScale(scale);
              }
              else
              */
              {
                 // QPointF qplast = widget->mapToScene(lastx,lasty);
                  //QPointF qp2 = widget->mapToScene(x,height - 1.0 - y);
                  QPointF qplast(lastx,lasty);
                  QPointF qp2(x,height - 1.0 - y);

                       if(qgin)
                           if(getScene())
                                getScene()->removeItem(qgin);
                       delete qgin;
                       qgin=0;
                       if(getScene())
                            qgout = getScene()->addLine(qplast.x(),qplast.y(),qp2.x(),qp2.y(),curPen);

                   //QGraphicsItem *ln = m_scene->addLine(int(lastx),int(lasty),int(x),int(height - 1.0 - y));
                   lastx = x;  lasty = height - 1.0 - y;
                   //LineTo(hDC,int(lastx),int(lasty));
                   //ln->setScale(scale);
              }

              if (outfile != 0)
                fprintf(outfile,"PD%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
         }
    }
    return qgout;
#else
  double x4,y4,x5,y5,lx,ly;
  if (linestyle != NULL)
    {  if (transform)
         {  x4 = x5 = a1 * x + b1 * y + c1;
            y4 = y5 = a2 * x + b2 * y + c2;
            if (clip2d(&lastx,&lasty,&x4,&y4))
              linestyle->draw(this,int(lastx),int(lasty),int(x4),int(y4));
            lastx = x5;  lasty = y5;
         }
       else
         {  x4 = x;  y4 = height-1-y;
            linestyle->draw(this,lastx,lasty,x4,y4);
            lastx = x4;  lasty = y4;
         }
    }
  else
    {  if (transform)
         {  x4 = x5 = a1 * x + b1 * y + c1;
            y4 = y5 = a2 * x + b2 * y + c2;
            lx = lastx;  ly = lasty;
            if (clip2d(&lastx,&lasty,&x4,&y4))
              {  MoveToEx(hDC,int(lastx),int(lasty),0);
                 LineTo(hDC,int(x4),int(y4));
                 if (outfile != 0)
                   {  if ((long(lx*40.0) != long(lastx*40.0) || long(ly*40.0) != long(lasty*40.0)))
                        fprintf(outfile,"PU%d,%d;",long(lx*40.0*scale/xdotspermm),long((height - 1.0 - ly)*40.0*scale/ydotspermm));
                      fprintf(outfile,"PD%d,%d;",long(x4*40.0*scale/xdotspermm),long((height - 1.0 - y4)*40.0*scale/ydotspermm));
                   }
              }
            lastx = x5;  lasty = y5;
         }
       else
         {  MoveToEx(hDC,int(lastx),int(lasty),0);
            if (int(lastx) == int(x) && int(lasty) == int(height - 1.0 - y))
              LineTo(hDC,int(lastx)+1,int(lasty));
            else
              {  lastx = x;  lasty = height - 1.0 - y;
                 LineTo(hDC,int(lastx),int(lasty));
              }

            if (outfile != 0)
              fprintf(outfile,"PD%d,%d;",long(lastx*40.0*scale/xdotspermm),long(y*40.0*scale/ydotspermm));
         }
    }
#endif
}


#ifdef USING_WIDGETS
void Surface::fillGL(Point **verts, int *nvert, int nloops, int alreadytransformed)
{
    DrawPolygon(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),nloops,nvert,verts);
#if 0
    POINT *vertices;
    int *counts;
    int i,j,k,nv,base;

     nv = 0;
     for (i = 0 ; i < nloops ; i++)
       nv += nvert[i] + 1;
     if (nv > nloops)
     {
         vertices = new POINT[nv];
          counts = new int[nloops];
          for (i = 0,k = 0 ; i < nloops ; i++,k++)
            {  base = k;
               for (j = 0 ; j < nvert[i] ; j++,k++)
                 if (transform && ! alreadytransformed)
                   {
                      vertices[k].x = a1 * verts[i][j].x + b1 * verts[i][j].y + c1;
                      vertices[k].y = a2 * verts[i][j].x + b2 * verts[i][j].y + c2;
                   }
                 else
                   {
                      vertices[k].x = verts[i][j].x;
                      vertices[k].y = height - 1.0 - verts[i][j].y;
                   }
               vertices[k].x = vertices[base].x;
               vertices[k].y = vertices[base].y;
               counts[i] = nvert[i] + 1;
            }


          delete [] vertices;
          delete [] counts;
     }
#endif
}

/**
 * @brief Surface::cbox2dGL
 * @param cp
 * @param xaxis
 * @param yaxis
 * @param width
 * @param height
 *
 * draws an unfilled box in screen space of this using OpenGl commands
 * the box is centered on the given point snd aligned with the given x and y axis directions
 * uses the current drawing setup
 */
void Surface::cbox2dGL(Point cp, Point xaxis, Point yaxis, double width, double height)
{
    Point hpt0,hpt1,hpt2,hpt3;
    double w = width * 0.5;
    double h = height * 0.5;

    hpt0 = cp - (xaxis*w) - (yaxis*h);
    hpt1 = cp + (xaxis*w) - (yaxis*h);
    hpt2 = cp + (xaxis*w) + (yaxis*h);
    hpt3 = cp - (xaxis*w) + (yaxis*h);

    glBegin(GL_LINE_LOOP);
#if 1
    glVertex3d(hpt0.x,hpt0.y,hpt0.z);
    glVertex3d(hpt1.x,hpt1.y,hpt1.z);
    glVertex3d(hpt2.x,hpt2.y,hpt2.z);
    glVertex3d(hpt3.x,hpt3.y,hpt3.z);
#else
    glVertex3f(hpt0.x,hpt0.y,hpt0.z);
    glVertex3f(hpt1.x,hpt1.y,hpt1.z);
    glVertex3f(hpt2.x,hpt2.y,hpt2.z);
    glVertex3f(hpt3.x,hpt3.y,hpt3.z);
#endif
    glEnd();
}

/**
 * @brief Surface::cfillbox2dGL
 * @param cp
 * @param xaxis
 * @param yaxis
 * @param width
 * @param height
 *
 * draws an filled box in screen space of this using OpenGl commands
 * the box is centered on the given point snd aligned with the given x and y axis directions
 * uses the current drawing setup
 */
void Surface::cfillbox2dGL(Point cp, Point xaxis, Point yaxis, double width, double height)
{
    Point hpt0,hpt1,hpt2,hpt3;
    double w = width * 0.5;
    double h = height * 0.5;

    hpt0 = cp - (xaxis*w) - (yaxis*h);
    hpt1 = cp + (xaxis*w) - (yaxis*h);
    hpt2 = cp + (xaxis*w) + (yaxis*h);
    hpt3 = cp - (xaxis*w) + (yaxis*h);

    glBegin(GL_QUADS);
#if 1
    glVertex3d(hpt0.x,hpt0.y,hpt0.z);
    glVertex3d(hpt1.x,hpt1.y,hpt1.z);
    glVertex3d(hpt2.x,hpt2.y,hpt2.z);
    glVertex3d(hpt3.x,hpt3.y,hpt3.z);
#else
    glVertex3f(hpt0.x,hpt0.y,hpt0.z);
    glVertex3f(hpt1.x,hpt1.y,hpt1.z);
    glVertex3f(hpt2.x,hpt2.y,hpt2.z);
    glVertex3f(hpt3.x,hpt3.y,hpt3.z);
#endif
    glEnd();
}

/**
 * @brief Surface::cross2dGL
 * @param cp
 * @param xaxis
 * @param yaxis
 * @param width
 * @param height
 *
 * draws cross with 4 equal length arms in screen space of this using OpenGl commands
 * the cross is centered on the given point snd aligned with the given x and y axis directions
 * uses the current drawing setup
 */
void Surface::cross2dGL(Point cp, Point xaxis, Point yaxis, double width, double height)
{
    Point hpt0,hpt1,hpt2,hpt3;
    double w = width * 0.5;
    double h = height * 0.5;

    hpt0 = cp - (xaxis*w);
    hpt1 = cp + (xaxis*w);
    hpt2 = cp - (yaxis*h);
    hpt3 = cp + (yaxis*h);

    glBegin(GL_LINES);
#if 1
    glVertex3d(hpt0.x,hpt0.y,hpt0.z);
    glVertex3d(hpt1.x,hpt1.y,hpt1.z);
    glVertex3d(hpt2.x,hpt2.y,hpt2.z);
    glVertex3d(hpt3.x,hpt3.y,hpt3.z);
#else
    glVertex3f(hpt0.x,hpt0.y,hpt0.z);
    glVertex3f(hpt1.x,hpt1.y,hpt1.z);
    glVertex3f(hpt2.x,hpt2.y,hpt2.z);
    glVertex3f(hpt3.x,hpt3.y,hpt3.z);
#endif
    glEnd();
}

QGraphicsItem* Surface::fill(QGraphicsItem *qgItem, Point **verts, int *nvert, int nloops, int alreadytransformed, QBrush *brush)
{
 //POINT *vertices;
 Point *vertices;
 int *counts;
 int i,j,k,nv,base;
 //RCHBRUSH hbrush,oldhbrush;
 //RCHPEN hpen;
 QBrush hbrush, oldhbrush;
 QPen hpen;
 QPainterPath qpath;
 qpath.setFillRule(Qt::OddEvenFill);

  nv = 0;
  for (i = 0 ; i < nloops ; i++)
    nv += nvert[i] + 1;
  if (nv > nloops)
  {
       //vertices = new POINT[nv];
       vertices = new Point[nv];
       counts = new int[nloops];
       for (i = 0,k = 0 ; i < nloops ; i++,k++)
       {
            base = k;
            for (j = 0 ; j < nvert[i] ; j++,k++)
            {
              if (transform && ! alreadytransformed)
              {
                   vertices[k].x = a1 * verts[i][j].x + b1 * verts[i][j].y + c1;
                   vertices[k].y = a2 * verts[i][j].x + b2 * verts[i][j].y + c2;
              }
              else
              {
                   vertices[k].x = verts[i][j].x;
                   vertices[k].y = height - 1.0 - verts[i][j].y;
              }

              if(j == 0)
                  qpath.moveTo(vertices[k].x,vertices[k].y);
              else
                  qpath.lineTo(vertices[k].x,vertices[k].y);
            }
            vertices[k].x = vertices[base].x;
            vertices[k].y = vertices[base].y;
            counts[i] = nvert[i] + 1;
            //
            //QPolygonF qpoly;
            //for(j = 0; j<counts[i]; j++)
            //    qpoly << QPointF(vertices[j].x,vertices[j].y);
            //qpath.moveTo(QPointF(vertices[0].x,vertices[0].y));
            //qpath.addPolygon(qpoly);
            qpath.closeSubpath();
       }
       qpath.closeSubpath();

       hpen = QPen(Qt::NoPen);
       if(brush == 0)
           hbrush = QBrush(QColor(GetRValue(fillcolourref),GetGValue(fillcolourref),GetBValue(fillcolourref)));
       else
           hbrush = *brush;

       if(qgItem)
           if(getScene())
                getScene()->removeItem(qgItem);
       delete qgItem;
       qgItem = 0;

       if(getScene())
            qgItem = getScene()->addPath(qpath,hpen,hbrush);

       delete [] vertices;
       delete [] counts;
  }
  return qgItem;
}
#endif


#ifndef USING_WIDGETS
QGraphicsItem* Surface::fill(Point **verts,int *nvert,int nloops,int alreadytransformed)
{
 Point *vertices;
 int *counts;
 int i,j,k,nv,base;

 nv = 0;
 for (i = 0 ; i < nloops ; i++)
   nv += nvert[i] + 1;
 if (nv > nloops)
 {
      vertices = new Point[nv];
      counts = new int[nloops];
      for (i = 0,k = 0 ; i < nloops ; i++,k++)
      {
           base = k;
           for (j = 0 ; j < nvert[i] ; j++,k++)
             if (transform && ! alreadytransformed)
             {
                  vertices[k].x = int(a1 * verts[i][j].x + b1 * verts[i][j].y + c1);
                  vertices[k].y = int(a2 * verts[i][j].x + b2 * verts[i][j].y + c2);
             }
             else
             {
                  vertices[k].x = int(verts[i][j].x);
                  vertices[k].y = int(height - 1.0 - verts[i][j].y);
             }
           vertices[k].x = vertices[base].x;
           vertices[k].y = vertices[base].y;
           counts[i] = nvert[i] + 1;

      }


      QGraphicsPolygonItem *qpoly=0;
      for (i = 0,k = 0 ; i < nloops ; i++,k++)
      {
          base = k;
          QPolygonF poly;
          for (j = 0 ; j < nvert[i] ; j++,k++)
              poly << QPointF(vertices[k].x,vertices[k].y);
          qpoly = new QGraphicsPolygonItem(poly);
          qpoly->setBrush(QBrush(Qt::SolidPattern));
          getScene()->addItem(qpoly);
      }

      delete [] vertices;
      delete [] counts;

      return qpoly;
 }

#else
void Surface::fill(Point **verts,int *nvert,int nloops,int alreadytransformed)
{
 POINT *vertices;
 int *counts;
 int i,j,k,nv,base;

 RCHBRUSH hbrush,oldhbrush;
 RCHPEN hpen;
  nv = 0;
  for (i = 0 ; i < nloops ; i++)
    nv += nvert[i] + 1;
  if (nv > nloops)
    {  vertices = new POINT[nv];
       counts = new int[nloops];
       for (i = 0,k = 0 ; i < nloops ; i++,k++)
         {  base = k;
            for (j = 0 ; j < nvert[i] ; j++,k++)
              if (transform && ! alreadytransformed)
                {
                   vertices[k].x = a1 * verts[i][j].x + b1 * verts[i][j].y + c1;
                   vertices[k].y = a2 * verts[i][j].x + b2 * verts[i][j].y + c2;
                }
              else
                {
                   vertices[k].x = verts[i][j].x;
                   vertices[k].y = height - 1.0 - verts[i][j].y;
                }
            vertices[k].x = vertices[base].x;
            vertices[k].y = vertices[base].y;
            counts[i] = nvert[i] + 1;
         }

       hpen = (RCHPEN)SelectObject(hDC,GetStockObject(NULL_PEN));
       hbrush = CreateSolidBrush(fillcolourref);
       oldhbrush = (RCHBRUSH) SelectObject(hDC,hbrush);
       SetPolyFillMode(hDC,ALTERNATE);
       PolyPolygon(hDC,vertices,counts,nloops);
       SelectObject(hDC,oldhbrush);
       SelectObject(hDC,hpen);
       DeleteObject(hbrush);


       delete [] vertices;
       delete [] counts;
   }
#endif
}

#ifdef USING_WIDGETS
double Surface::getpixelmodelmm()
{
    return 1.0 / xdotspermm * scale;
}

void Surface::dotGL(double x, double y, double z)
{
    glBegin(GL_POINT);
#if 1
    glVertex3d((GLdouble)x,(GLdouble)y,(GLdouble)z);
#else
    glVertex3f((GLfloat)x,(GLfloat)y,(GLfloat)z);
#endif
    glEnd();
}

void Surface::dotGL(double x, double y)
{
    dotGL(x,y,0.0);
}

QGraphicsItem* Surface::dot(double x,double y)
{
    QGraphicsItem* qgi=0;
    return dot(qgi,x,y);
}

QGraphicsItem* Surface::dot(QGraphicsItem *pgItem,double x,double y)
{
    QGraphicsItem *qline1 = 0, *qline2 = 0;
    QList <QGraphicsItem *> glist;
    double x1,y1;
    RCCOLORREF c;
    if (drawmode == DM_ERASE)
    {
        c = background;
        curPen.setColor(QColor(GetRValue(c),GetGValue(c),GetBValue(c)));
        if(pgItem)
            getScene()->removeItem(pgItem);
        delete pgItem;
        return 0;
    }
    else
    {
        c = colourref;
        curPen.setColor(QColor(GetRValue(c),GetGValue(c),GetBValue(c)));
    }
    if (transform)
    {
        x1 = a1 * x + b1 * y + c1;
        y1 = a2 * x + b2 * y + c2;
        if (x1 >= clipx1 && x1 <= clipx2 && y1 >= clipy1 && y1 <= clipy2)
        {
            if(getScene())
            {
                if(pgItem)
                        getScene()->removeItem(pgItem);
                delete pgItem;

                qline1 = getScene()->addLine(x1,y1,x1,y1,curPen);
                if(qline1) glist.append(qline1);

                /*
                qline1 = getScene()->addLine(x1-1,y1,x1+1,y1,curPen);
                if(qline1) glist.append(qline1);

                qline2 = getScene()->addLine(x1,y1-1,x1,y1+1,curPen);
                if(qline2) glist.append(qline2);
                */

                if(glist.size() > 0)
                    pgItem = getScene()->createItemGroup(glist);
                //SetPixel(hDC,int(x1),int(y1),c);
            }
        }
    }
    else
    {
        if(getScene())
        {
            if(pgItem)
                getScene()->removeItem(pgItem);
            delete pgItem;

            qline1 = getScene()->addLine(x,height-1.0-y,x,height-1.0-y,curPen);
            if(qline1) glist.append(qline1);

            /*
            qline1 = getScene()->addLine(x-1,height-1.0-y,x+1,height-1.0-y,curPen);
            if(qline1) glist.append(qline1);

            qline2 = getScene()->addLine(x,height-1.0-y-1,x,height-1.0-y+1,curPen);
            if(qline2) glist.append(qline2);
            */

            if(glist.size() > 0)
                pgItem = getScene()->createItemGroup(glist);
            //SetPixel(hDC,int(x),int(height-1.0-y),c);
        }
    }
    return pgItem;
}
#else
void Surface::dot(double x,double y)

    double x1,y1;
    RCCOLORREF c;
    if (drawmode == DM_ERASE)
    c = background;
    else
    c = colourref;
    if (transform)
    {
        x1 = a1 * x + b1 * y + c1;
        y1 = a2 * x + b2 * y + c2;
        if (x1 >= clipx1 && x1 <= clipx2 && y1 >= clipy1 && y1 <= clipy2)
            SetPixel(hDC,int(x1),int(y1),c);
    }
    else
        SetPixel(hDC,int(x),int(height-1.0-y),c);
}
#endif

void Surface::resizeEvent(QResizeEvent *qevent)
{
    //qDebug() << "Entering : Surface::resizeEvent ";
    sizeevent(qevent->size().width(),qevent->size().height());
    //qDebug() << "Leaving : Surface::resizeEvent ";
}

void Surface::sizeevent(double w,double h)
{
  width = w;
  height = h;
  clipx1 = 0.0;  clipy1 = 0.0;  clipx2 = w;  clipy2 = h;

#ifdef USING_WIDGETS
  //if(getScene())
//     getScene()->setSceneRect(0.0,0.0,w,h);
#endif
}

void Surface::sendsizeevent(int w,int h)
{
  RECT rect1,rect2,rect3;
  int x,y,dx,dy;
  RCMSG msg;
  GetWindowRect(gethwnd(),&rect1);
  GetWindowRect(GetParent(gethwnd()),&rect2);
  GetClientRect(gethwnd(),&rect3);
  dx = (rect1.right - rect1.left) - (rect3.right - rect3.left);
  dy = (rect1.bottom - rect1.top) - (rect3.bottom - rect3.top);
  x = rect1.left - rect2.left;  y = rect1.top - rect2.top;
  MoveWindow(gethwnd(),x,y,w+dx,h+dy,TRUE);
  //while (! PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE))
    program->processmessages(1);
}

void Surface::paintEvent(QPaintEvent *event)
{
    //qDebug() << "Entering : Surface::paintEvent ";
    //qDebug() << "Leaving : Surface::paintEvent ";
}

void Surface::sethdc(RCHWND hwnd,RCHDC hdc)
{
  RCHPEN hpen;
  ResourceString rsnc0(NC);
  ResourceString rs0(NCWIN);
  hWnd = hwnd;
  hDC = hdc;
#ifndef USING_WIDGETS
  if (hDC == NULL)
  {
      if (hWnd == NULL)
         cadwindow->MessageBox(rs0.gets(),rsnc0.gets(),MB_ICONSTOP);
      else
      {
          hDC = GetDC(hWnd);  newhdc = 1;
      }
  }
#else
  if (hDC == NULL)
  {
      if (hWnd != NULL)
      {
          hDC = GetDC(hWnd);
          newhdc = 1;
      }
  }
  else
  {
     hDC = hdc; // this is the Qt scene
     newhdc = 0;
  }
#endif
  if (GetDeviceCaps(hDC,HORZSIZE) != 0)
    {
       xdotspermm = (int)(double(GetDeviceCaps(hDC,HORZRES)) / double(GetDeviceCaps(hDC,HORZSIZE)));
       ydotspermm = (int)(double(GetDeviceCaps(hDC,VERTRES)) / double(GetDeviceCaps(hDC,VERTSIZE)));
    }
  else
    {
       RCHDC hdc;
       hdc = GetDC(NULL);
       xdotspermm = (int)(double(GetDeviceCaps(hdc,HORZRES)) / double(GetDeviceCaps(hdc,HORZSIZE)));
       ydotspermm = (int)(double(GetDeviceCaps(hdc,VERTRES)) / double(GetDeviceCaps(hdc,VERTSIZE)));
       ReleaseDC(NULL,hdc);
    }
  hpen = CreatePen(PS_SOLID,1,RGB(db.colourtable[0].red,db.colourtable[0].green,db.colourtable[0].blue));
  horiginalpen = (RCHPEN) SelectObject(hDC,hpen);
}

void Surface::sethwnd(RCHWND hwnd)
{ hWnd = hwnd;
  hDC = NULL;
  newhdc = 0;
  xdotspermm = 1.0;
  ydotspermm = 1.0;
  horiginalpen = NULL;
}

void Surface::freehdc(void)
{ if (hDC != NULL)
    {  DeleteObject(SelectObject(hDC,horiginalpen));
       hDC = NULL;
    }
}

#ifdef USING_WIDGETS
int Surface::saveimage(int fileformat,const RCCHAR *fname,int quality)
{
    char *scanline;
    int i,j,k;
    QRgb pixel;
    FILE *fp;
    int numbytes;
    short w,h;
    QClipboard *clipboard;
    QImage image;
    QImageWriter iwriter;

    //QPixmap screenshot = ((QGraphicsView*)hWnd)->grab();
    //QPixmap screenshot(((QGraphicsView*)hWnd)->size());
    //((QWidget*)hWnd)->render(&screenshot);
    //QPixmap screenshot = QPixmap::grabWidget((QWidget*)cadwindow->getcurrentwindow()->gethwnd());
    //QPixmap scaled = screenshot.scaled(screenshot.width(),screenshot.height());
    QPixmap screenshot = QPixmap::grabWidget(((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view());


    switch (fileformat)
    {
        case 1 :
            // bmp
            qDebug() << "save image case 1 : bmp";
            screenshot.save(QString(fname));
            break;
        case 2 :
            // tga
            image = screenshot.toImage();
            numbytes = int(width * 3);
            if ((scanline = new char [int(numbytes)]) == NULL) return 0;
            if ((fp = fopen(fname,"wb")) == NULL) return 0;
            if (fwrite("\0\0\02\0\0\0\0\0\0\0\0\0",12,1,fp) != 1)
              {  fclose(fp);  delete [] scanline;  return 0;
              }
            w = (short) width;
            if (fwrite(&w,2,1,fp) != 1)
              {  fclose(fp);  delete [] scanline;  return 0;
              }
            h = (short) height;
            if (fwrite(&h,2,1,fp) != 1)
              {  fclose(fp);  delete [] scanline;  return 0;
              }
            if (fwrite("\030\040",2,1,fp) != 1)
              {  fclose(fp);  delete [] scanline;  return 0;
              }
            for (i = 0 ; i < int(height) ; i++)
              {  for (j = 0,k = 0 ; j < int(width) ; j++,k += 3)
                   {
                      pixel = image.pixel(j,i);
                      //pixel = GetPixel(hDC,j,i);
                      scanline[k] = qBlue(pixel);
                      scanline[k+1] = qGreen(pixel);
                      scanline[k+2] = qRed(pixel);
                   }
                 if (fwrite(scanline,int(width * 3),1,fp) != 1)
                   {  fclose(fp);  delete [] scanline;  return 0;
                   }
              }
            fclose(fp);
            delete [] scanline;
            qDebug() << "save image case 2 : tga";
            break;
        case 3 :
            // clipboard
            clipboard = QApplication::clipboard();
            if(clipboard)
            {
                clipboard->clear();
                clipboard->setPixmap(screenshot);
            }
            qDebug() << "save image case 3 : clipboard";
            break;
        case 4 :
            // jpg
            iwriter;
            iwriter.setFileName(QString(fname));
            iwriter.setFormat("jpg");
            iwriter.setQuality(20 + quality * 40);
            iwriter.write(screenshot.toImage());
            qDebug() << "save image case 4 : jpg";
            break;
    }
}
#else
int Surface::saveimage(int fileformat,const RCCHAR *fname,int quality)
{
 BITMAPFILEHEADER bmfh;
 BITMAPINFOHEADER bmih;
 BITMAPINFO bmi;
 RCHBITMAP hbitmap,hbitmapold;
 RCHDC hdc;
 char *scanline,*buffer,*jpegbuffer;
 int i,j,k;
 RCCOLORREF pixel;
 FILE *fp;
 long numwords,numbytes;
 short w,h;
 JPEG_CORE_PROPERTIES jcprops;

   switch (fileformat)
    {  case 1 :
         numwords = long((width * 3L + 3L) / 4L);
         numbytes = long(numwords * 4L * height);
         bmfh.bfType = 19778;                             // BM
         bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + numbytes;
         bmfh.bfReserved1 = 0;
         bmfh.bfReserved2 = 0;
         bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
         bmih.biSize = sizeof(BITMAPINFOHEADER);
         bmih.biWidth = int(width);
         bmih.biHeight = int(height);
         bmih.biPlanes = 1;
         bmih.biBitCount = 24;
         bmih.biCompression = BI_RGB;
         bmih.biSizeImage = 0;
         bmih.biXPelsPerMeter = 0;
         bmih.biYPelsPerMeter = 0;
         bmih.biClrUsed = 0;
         bmih.biClrImportant = 0;
         if ((fp = fopen(fname,"wb")) == NULL)
           return 0;
         if (fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,fp) != 1)
           {  fclose(fp);
              return 0;
           }
         if (fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,fp) != 1)
           {  fclose(fp);
              return 0;
           }

         numwords = long((width * 3L + 3L) / 4L);
         numbytes = long(numwords * 4L * height);
         buffer = new char [numbytes];
         bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         bmi.bmiHeader.biWidth = int(width);
         bmi.bmiHeader.biHeight = int(height);
         bmi.bmiHeader.biPlanes = 1;
         bmi.bmiHeader.biBitCount = 24;
         bmi.bmiHeader.biCompression = BI_RGB;
         bmi.bmiHeader.biSizeImage = numbytes;
         bmi.bmiHeader.biXPelsPerMeter = 0;
         bmi.bmiHeader.biYPelsPerMeter = 0;
         bmi.bmiHeader.biClrUsed = 0;
         bmi.bmiHeader.biClrImportant = 0;

         if ((hdc = CreateCompatibleDC(gethdc())) == NULL)
           return 0;
         if ((hbitmap = (RCHBITMAP) CreateCompatibleBitmap(gethdc(),(int)getwidth(),(int)getheight())) == NULL)
           {  DeleteDC(hdc);  return 0;
           }
         hbitmapold = (RCHBITMAP) SelectObject(hdc,hbitmap);
         BitBlt(hdc,0,0,int(getwidth()),int(getheight()),gethdc(),0,0,SRCCOPY);
         SelectObject(hdc,hbitmapold);
         GetDIBits(hdc,hbitmap,0,int(height),buffer,&bmi,DIB_RGB_COLORS);
         DeleteDC(hdc);
         DeleteObject(hbitmap);

         if (fwrite(buffer,numbytes,1,fp) != 1)
           {  fclose(fp);
              delete [] buffer;
              return 0;
           }

         fclose(fp);

         delete [] buffer;

         break;
       case 2 :
         numbytes = long(width * 3L);
         if ((scanline = new char [int(numbytes)]) == NULL) return 0;
         if ((fp = fopen(fname,"wb")) == NULL) return 0;
         if (fwrite("\0\0\02\0\0\0\0\0\0\0\0\0",12,1,fp) != 1)
           {  fclose(fp);  delete [] scanline;  return 0;
           }
         w = (short) width;
         if (fwrite(&w,2,1,fp) != 1)
           {  fclose(fp);  delete [] scanline;  return 0;
           }
         h = (short) height;
         if (fwrite(&h,2,1,fp) != 1)
           {  fclose(fp);  delete [] scanline;  return 0;
           }
         if (fwrite("\030\040",2,1,fp) != 1)
           {  fclose(fp);  delete [] scanline;  return 0;
           }
         for (i = 0 ; i < int(height) ; i++)
           {  for (j = 0,k = 0 ; j < width ; j++,k += 3)
                {  pixel = GetPixel(hDC,j,i);
                   scanline[k] = GetBValue(pixel);
                   scanline[k+1] = GetGValue(pixel);
                   scanline[k+2] = GetRValue(pixel);
                }
              if (fwrite(scanline,int(width * 3),1,fp) != 1)
                {  fclose(fp);  delete [] scanline;  return 0;
                }
           }
         fclose(fp);
         delete [] scanline;
         break;
       case 3 :
         numwords = long((width * 3L + 3L) / 4L);
         numbytes = long(numwords * 4L * height);
         bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         bmi.bmiHeader.biWidth = int(width);
         bmi.bmiHeader.biHeight = int(height);
         bmi.bmiHeader.biPlanes = 1;
         bmi.bmiHeader.biBitCount = 24;
         bmi.bmiHeader.biCompression = BI_RGB;
         bmi.bmiHeader.biSizeImage = numbytes;
         bmi.bmiHeader.biXPelsPerMeter = 0;
         bmi.bmiHeader.biYPelsPerMeter = 0;
         bmi.bmiHeader.biClrUsed = 0;
         bmi.bmiHeader.biClrImportant = 0;

         if ((hdc = CreateCompatibleDC(gethdc())) == NULL) return 0;
         if ((hbitmap = (RCHBITMAP) CreateCompatibleBitmap(gethdc(),(int)getwidth(),(int)getheight())) == NULL)
           {  DeleteDC(hdc);  return 0;
           }
         hbitmapold = (RCHBITMAP) SelectObject(hdc,hbitmap);
         BitBlt(hdc,0,0,int(getwidth()),int(getheight()),gethdc(),0,0,SRCCOPY);
         SelectObject(hdc,hbitmapold);
         DeleteDC(hdc);
         //  Now copy to the clipboard
         if (OpenClipboard(0))
           {  if (EmptyClipboard())
                SetClipboardData(CF_BITMAP,hbitmap);
              CloseClipboard();
           }
         break;
       case 4 :
         numwords = long((width * 3L + 3L) / 4L);
         numbytes = long(numwords * 4L * height);
         buffer = new char [numbytes];
         jpegbuffer = new char [numbytes];
         bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         bmi.bmiHeader.biWidth = int(width);
         bmi.bmiHeader.biHeight = int(height);
         bmi.bmiHeader.biPlanes = 1;
         bmi.bmiHeader.biBitCount = 24;
         bmi.bmiHeader.biCompression = BI_RGB;
         bmi.bmiHeader.biSizeImage = numbytes;
         bmi.bmiHeader.biXPelsPerMeter = 0;
         bmi.bmiHeader.biYPelsPerMeter = 0;
         bmi.bmiHeader.biClrUsed = 0;
         bmi.bmiHeader.biClrImportant = 0;

         if ((hdc = CreateCompatibleDC(gethdc())) == NULL)
           return 0;
         if ((hbitmap = (RCHBITMAP) CreateCompatibleBitmap(gethdc(),(int)getwidth(),(int)getheight())) == NULL)
           {  DeleteDC(hdc);  return 0;
           }
         hbitmapold = (RCHBITMAP) SelectObject(hdc,hbitmap);
         BitBlt(hdc,0,0,int(getwidth()),int(getheight()),gethdc(),0,0,SRCCOPY);
         SelectObject(hdc,hbitmapold);
         GetDIBits(hdc,hbitmap,0,int(height),buffer,&bmi,DIB_RGB_COLORS);
         DeleteDC(hdc);
         DeleteObject(hbitmap);

         if (ijlInit(&jcprops) == IJL_OK)
           {  // Set up information to write from the pixel buffer.
              jcprops.DIBWidth       = bmi.bmiHeader.biWidth;
              jcprops.DIBHeight      = -bmi.bmiHeader.biHeight; // Implies a bottom-up DIB.
              jcprops.DIBBytes       = (unsigned char *)buffer;
              jcprops.DIBPadBytes    = numwords * 4 - int(width) * 3L;
              jcprops.DIBChannels    = 3;
              jcprops.DIBColor       = IJL_BGR;

              jcprops.JPGWidth       = bmi.bmiHeader.biWidth;
              jcprops.JPGHeight      = bmi.bmiHeader.biHeight;
              jcprops.JPGFile        = NULL;
              jcprops.JPGBytes       = (unsigned char *)jpegbuffer;
              jcprops.JPGSizeBytes   = bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 3;
              jcprops.JPGChannels    = 3;
              jcprops.JPGColor       = IJL_YCBCR;
              jcprops.JPGSubsampling = IJL_411; // 4:1:1 subsampling.
              jcprops.jquality = 20 + quality * 40;

              // Write the actual JPEG image from the pixel buffer.
              if (ijlWrite(&jcprops,IJL_JBUFF_WRITEWHOLEIMAGE) == IJL_OK)
                {FILE *outfile;
                   if ((outfile = fopen(fname,"wb")) != 0)
                     {  fwrite(jpegbuffer,jcprops.JPGSizeBytes,1,outfile);
                        fclose(outfile);
                     }
                }
              ijlFree(&jcprops);
           }
         delete buffer;
         delete jpegbuffer;
         break;
    }
  return 1;
}
#endif

void Surface::settransform2d(double a3,double b3,double c3,double a4,double b4,double c4)
{double l,cosa;
  transform = 1;
  a1 = a3;  b1 = b3;  c1 = c3;
  a2 = a4;  b2 = b4;  c2 = c4;
  l = a1 * a1 + b1 * b1;
  if (l < db.getptoler())
    angle = 0.0;
  else
    {  cosa = a1 / l;
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       angle = acos(cosa) * 180.0 / M_PI;
       if (b1 > 0.0) angle = 360.0 - angle;
    }
}

void Surface::setrotate(void)
{ transform = 1;
  a1 =  0.0;  b1 = -1.0;  c1 = height - 1.0;
  a2 = -1.0;  b2 =  0.0;  c2 = width - 1.0;
  angle = 90.0;
}

void Surface::gettransform2d(double *a3,double *b3,double *c3,double *a4,double *b4,double *c4)
{ if (transform)
    {  *a3 = a1;  *b3 = b1;  *c3 = c1;
       *a4 = a2;  *b4 = b2;  *c4 = c2;
    }
  else
    {  *a3 = 1.0;  *b3 = 0.0;  *c3 = 0.0;
       *a4 = 0.0;  *b4 = 1.0;  *c4 = 0.0;
    }
}

double Surface::getrotation2d(void)
{ return angle;
}

void Surface::transform2d(double *x1,double *y1)
{
  double x2,y2;
  if (transform)
    {  x2 = a1 * *x1 + b1 * *y1 + c1;
       y2 = a2 * *x1 + b2 * *y1 + c2;
       *x1 = x2;  *y1 = y2;
    }
  else
    *y1 = height - 1 - *y1;
}

void Surface::untransform2d(double *x1,double *y1)
{
  double x2,y2;
  if (transform)
    {  x2 = a1 * *x1 + b1 * *y1 + c1;
       y2 = a2 * *x1 + b2 * *y1 + c2;
       *x1 = x2;  *y1 = y2;
    }
  else
    *y1 = (height - 1) - *y1;
}

void Surface::cleartransform(void)
{ transform = 0;  angle = 0.0;
}

void Surface::setclip(double x1,double y1,double x2,double y2)
{
    clipx1 = x1;  clipy1 = y1;  clipx2 = x2;  clipy2 = y2;
}

int Surface::clip2d(double *x1,double *y1,double *x2,double *y2)
{int    i,accept,outcode1,outcode2;
 double x,y;
  accept = 0;
  for (i = 0 ; i < 8 ; i++)
    {  outcode1 = ((*y1 > clipy2))      | ((*y1 < clipy1) << 1) |
                  ((*x1 > clipx2) << 2) | ((*x1 < clipx1) << 3);
       outcode2 = ((*y2 > clipy2))      | ((*y2 < clipy1) << 1) |
                  ((*x2 > clipx2) << 2) | ((*x2 < clipx1) << 3);
       if (outcode1 == 0 && outcode2 == 0)
         {  accept = 1;  break;
         }
       if (outcode1 & outcode2) break;
       if (outcode1 == 0)
         {   outcode1 = outcode2;
             x = *x1;  *x1 = *x2;  *x2 = x;
             y = *y1;  *y1 = *y2;  *y2 = y;
         }
       if (outcode1 & 1)
         {  *x1 = *x1 + (*x2 - *x1) * (clipy2 - *y1) / (*y2 - *y1);
            *y1 = clipy2;
         }
       else if (outcode1 & 2)
         {  *x1 = *x1 + (*x2 - *x1) * (clipy1 - *y1) / (*y2 - *y1);
            *y1 = clipy1;
         }
       else if (outcode1 & 4)
         {  *y1 = *y1 + (*y2 - *y1) * (clipx2 - *x1) / (*x2 - *x1);
            *x1 = clipx2;
         }
       else if (outcode1 & 8)
         {  *y1 = *y1 + (*y2 - * y1) * (clipx1 - *x1) / (*x2 - *x1);
            *x1 = clipx1;
         }
    }
  return accept;
}

Surface::~Surface()
{
#ifdef USING_WIDGETS
  //m_scene = 0;
    delete [] colourindex;
    if(horiginalpen !=0)
        delete horiginalpen;
#else
  RCHPEN holdpen;
  delete [] colourindex;
  if (hDC != NULL)
  {
       holdpen = (RCHPEN) SelectObject(hDC,horiginalpen);
       DeleteObject(holdpen);
       if (newhdc)
       {
            ReleaseDC(hWnd,hDC);
            if (SetLight != 0)
              ReleaseTextures(hDC);
       }
  }
#endif
}

//
//  The Window class
//
Window::Window(char *winclass,RCHWND parent,RCDWORD style,int x,int y,int w,int h,int show, RCPVOID params)
{
#ifdef _MAC
    Window(_RCT(winclass), parent, style, x, y, w, h, show, params);
#else
    this->Window::Window(_RCT(winclass), parent, style, x, y, w, h, show, params);
#endif
}

Window::Window(RCCHAR *winclass, RCHWND parent, RCDWORD style, int x, int y, int w, int h, int show, RCPVOID params)
{
  RCRECT rect;
  RCHWND hWnd;
  if (strcmp(winclass,"MenuWindow") == 0)
  {
      //RCRECT rect;
       GetWindowRect(parent,&rect);
       //x += rect.left;
       //y += rect.top;
       #if defined(WIN32)
         hWnd = RCCREATEWINDOWEX(WS_EX_TOOLWINDOW,_RCT("CadWindow"),_RCT(""),style,
                               x,y,w,h,parent,0,program->gethinst(),0);
         //hWnd = CreateWindowEx(WS_EX_TOOLWINDOW,TEXT("CadWindow"),TEXT(""),style,
         //                      x,y,w,h,parent,0,program->gethinst(),0);
       #else
         hWnd = CreateWindow(TEXT("CadWindow"),TEXT(""),style,
                             x,y,w,h,parent,0,program->gethinst(),NULL);
       #endif
  }
#if ! defined(_WIN32_WCE)
  else
  {
#ifdef DERIVE_FROM_WIDGETS
     hWnd = RCCREATEWINDOW(winclass,_RCT(""),style,
                          x,y,w,h,parent,NULL,program->gethinst(),((QWidget*)this));
#else
      hWnd = RCCREATEWINDOW(winclass,_RCT(""),style,
                           x,y,w,h,parent,NULL,program->gethinst(),params);
#endif
     //hWnd = CreateWindow(winclass,TEXT(""),style,
     //                    x,y,w,h,parent,NULL,program->gethinst(),NULL);
  }
//WCEFIX
#endif

  if (hWnd == NULL) return;

  //qDebug() << "Window hWnd : " << (QObject*)hWnd;

  wm.add(this);
  GetClientRect(hWnd,&rect);

  if (strcmp(winclass,"MenuWindow") == 0)
    {  if (rect.bottom == 0)
         {  MoveWindow(hWnd,x,y,w+(w-rect.right),50,FALSE);  //  Set size based on client rect
            GetClientRect(hWnd,&rect);
            MoveWindow(hWnd,x,y,w+(w-rect.right),h+(50-rect.bottom),FALSE);  //  Set size based on client rect
            GetClientRect(hWnd,&rect);
         }
       else
         {  MoveWindow(hWnd,x,y,w+(w-rect.right),h+(h-rect.bottom),FALSE);  //  Set size based on client rect
            GetClientRect(hWnd,&rect);
         }
    }

  Surface::sizeevent(rect.right,rect.bottom);

  int lparam = params != 0 ? *((int*)params) : 0;
  if((lparam & 1) == 1)
      show = SW_HIDE;

  if (strcmp(winclass,_RCT("EDIT")) != 0)
  {
      sethdc(hWnd,NULL);
      {
          ShowWindow(hWnd,show);
          BringWindowToTop(hWnd);
          InvalidateRect(hWnd,NULL,FALSE);
      }
  }
  else
  {
      sethwnd(hWnd);
      ShowWindow(hWnd,show);
  }
}

Window::~Window()
{
 Control *c;
 int i;
  wm.del(this);
  for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
    c->parent = 0;  //
  controllist.destroy();
}

void Window::title(const char *title)
{
    Window::title(CHAR2RCCHR(title));
}

void Window::title(const RCCHAR *title1,const RCCHAR *title2,const RCCHAR *title3)
{
    //if(cadwindow && cadwindow->subwindow->gethwnd())
    //    qDebug() << "cadwindow->subwindow->gethwnd() : " << (QObject*)cadwindow->subwindow->gethwnd();
    //if(gethwnd() &&  qobject_cast<QGraphicsView*>((QWidget*)gethwnd()) != 0)
    if(gethwnd() &&  qobject_cast<RCView*>((QWidget*)gethwnd()) != 0)
    {
        //qDebug() << "gethwnd()->parent() : " << (QObject*)((QWidget*)gethwnd())->parent();
        ((RCView*)(QWidget*)gethwnd())->setWindowTitle(QString(title1),QString(title2),QString(title3));
    }
    else
    {
        QString sTitle(title1);
        sTitle += QString(title2);
        sTitle += QString(title3);
        SetWindowText(gethwnd(),(RCLPCTSTR)sTitle.constData());
    }
}

void Window::title(const RCCHAR *title)
{
    //qDebug() << "Window::Title : gethwnd is a : " << (QWidget*)gethwnd();
    //if(cadwindow && cadwindow->subwindow->gethwnd())
    //    qDebug() << "cadwindow->subwindow->gethwnd() : " << (QObject*)cadwindow->subwindow->gethwnd();
    //if(gethwnd() &&  qobject_cast<QGraphicsView*>((QWidget*)gethwnd()) != 0)
    if(gethwnd() &&  qobject_cast<RCView*>((QWidget*)gethwnd()) != 0)
    {
        if(qobject_cast<QTabWidget*>((QObject*)cadwindow->subwindow->gethwnd()) != 0)
        {
            QTabWidget *qtbw = (QTabWidget*)cadwindow->subwindow->gethwnd();
            int tabid = qtbw->indexOf((QWidget*)gethwnd());
            //qDebug() << "title text : " << QString(title);
            //qtbw->setTabText(tabid,QString(title));
            qtbw->tabBar()->setTabToolTip(tabid,QString(title));
        }
        else if(qobject_cast<QMdiArea*>((QObject*)cadwindow->subwindow->gethwnd()) != 0)
        {
            //qDebug() << "gethwnd()->parent() : " << (QObject*)((QWidget*)gethwnd())->parent();
            ((QMdiSubWindow*)((QGraphicsView*)(QWidget*)gethwnd())->parent())->setWindowTitle(QString(title));
        }
        else if(qobject_cast<RCCentralWidget*>((QObject*)cadwindow->subwindow->gethwnd()) != 0)
        {
            //qDebug() << "gethwnd()->parent() : " << (QObject*)((QWidget*)gethwnd())->parent();
            ((RCView*)(QWidget*)gethwnd())->setWindowTitle(QString(title),"","");
        }
    }
    else
        SetWindowText(gethwnd(),(RCLPCTSTR)title);
}

int Window::addcontrol(Control *c,int draw)
{
    if (controllist.add(c))
    {  c->fixup(this);
       if (draw)
         c->draw(DM_NORMAL);
       return 1;
    }
  return 0;
}

Control *Window::getcontrol(int i)
{Control *c;

  for (controllist.start() ; (c = controllist.next()) != 0 ; i--)
    if (i == 0)
      return c;

  return 0;

}

void Window::delcontrol(Control *c,int draw)
{ controllist.del(c);
  if (draw)
    c->draw(DM_ERASE);
}

void Window::mousemoveevent(int x,int y,int buttons)
{Control *c;
 int i;

    /*
    qDebug() << "Window mousemovevent : " << ((QWidget*)gethwnd())->objectName();
    qDebug() << "Window mousemovevent buttons : " << buttons;
    qDebug() << "Window mousemoveevent x : " << x;
    qDebug() << "Window mousemoveevent y : " << y;
    if(getWidget()->objectName().toLower().contains("mainwindow"))
    {
        qDebug() << "Window widget : " << getWidget()->objectName();
        qDebug() << "Window central widget" << ((QMainWindow*)getWidget())->centralWidget();
        qDebug() << "Window cw active window" << ((RCCentralWidget*)((QMainWindow*)getWidget())->centralWidget())->activeWindow();
        qDebug() << "Window cw vp rect" << ((RCView*)((RCCentralWidget*)((QMainWindow*)getWidget())->centralWidget())->activeWindow())->view()->viewport()->rect();
        qDebug() << "Window cw vp scene rect" << ((RCView*)((RCCentralWidget*)((QMainWindow*)getWidget())->centralWidget())->activeWindow())->view()->sceneRect();
    }
    */

    if (x != -32767)
    {
        //RCCursor *cursor = new RCCursor(IDC_ARROW);
        //SetCursor(program->getharrowcursor()); LGM removed this as it was causing flicker in the X/Y/Z entry QLineEdit fields
        //delete cursor;
    }
    if(buttons == 1)
    {
        if(((QWidget*)gethwnd())->objectName().contains("X_CEdit"))
        {
            cadwindow->coordinateentry->setEntryFocus(_RCT('X'));
            cadwindow->coordinateentry->setEntryFocus(_RCT('Y'),false);
            cadwindow->coordinateentry->setEntryFocus(_RCT('Z'),false);
        }
        else if(((QWidget*)gethwnd())->objectName().contains("Y_CEdit"))
        {
            cadwindow->coordinateentry->setEntryFocus(_RCT('Y'));
            cadwindow->coordinateentry->setEntryFocus(_RCT('X'),false);
            cadwindow->coordinateentry->setEntryFocus(_RCT('Z'),false);
        }
        else if(((QWidget*)gethwnd())->objectName().contains("Z_CEdit"))
        {
            cadwindow->coordinateentry->setEntryFocus(_RCT('Z'));
            cadwindow->coordinateentry->setEntryFocus(_RCT('X'),false);
            cadwindow->coordinateentry->setEntryFocus(_RCT('Y'),false);
        }
        else
        {
            cadwindow->coordinateentry->setEntryFocus(_RCT('X'),false);
            cadwindow->coordinateentry->setEntryFocus(_RCT('Y'),false);
            cadwindow->coordinateentry->setEntryFocus(_RCT('Z'),false);
        }
    }
    else if(buttons)
    {
        cadwindow->coordinateentry->setEntryFocus(_RCT('X'),false);
        cadwindow->coordinateentry->setEntryFocus(_RCT('Y'),false);
        cadwindow->coordinateentry->setEntryFocus(_RCT('Z'),false);
    }
#if 0
    i = 0;
    while ((c = getcontrol(i++)) != 0)
        if (c->pick(x,y))
        {
            c->mousemoveevent(x,y,buttons);
        }
#endif
}

void Window::lbuttondownevent(int xp,int yp,int buttons)
{
    mousemoveevent(xp,yp,buttons);
}

void Window::lbuttonupevent(int xp,int yp,int buttons)
{ mousemoveevent(xp,yp,buttons);
}

void Window::paintEvent(QPaintEvent * event)
{
    //qDebug() << "Entering : Window::paintEvent ";
    paintevent();
    //qDebug() << "Leaving : Window::paintEvent ";
}

void Window::paintevent(void)
{
  //qDebug() << "Entering : Window::paintevent ";
  Control *c;
  for (controllist.start() ; (c = (Control *) controllist.next()) != NULL ; )
    c->draw(DM_NORMAL);
  //qDebug() << "Leaving : Window::paintevent ";
}

void Window::resizeEvent(QResizeEvent *qevent)
{
    //qDebug() << "Entering : Window::resizeEvent ";
    sizeevent(qevent->size().width(),qevent->size().height());
    //qDebug() << "Leaving : Window::resizeEvent ";
}

void Window::sizeevent(int w,int h)
{
  //qDebug() << "Entering : Window::sizeevent ";
  Control *c;
  Surface::sizeevent(w,h);
  for (controllist.start() ; (c = (Control *) controllist.next()) != NULL ; )
    c->fixup(this);

  //if(gethwnd())
   //   if(((QWidget*)gethwnd())->objectName() != "MainWindow")
    //    ((QWidget*)gethwnd())->resize(QSize(w,h));

  //qDebug() << "Leaving : Window::sizeevent ";
}

void Window::newmenu(HMENU hmenu)
{
#if ! defined(_WIN32_WCE)
  SetMenu(gethwnd(),hmenu);
//WCEFIX
#endif
}

//
//  The 'OpenList' class
//

OpenList::OpenList()
{ window = NULL;
  n = 0;
}

void OpenList::setwindow(Window *w)
{
 RCHMENU hmenu;
 int i;
 RCCHAR key[256],name[300],title[300];
  window = w;
#if ! defined(_WIN32_WCE)
  for (i = 0,n = 0 ; i < MAX_OPENLIST ; i++)
    {  sprintf(key,"op::file%d",i);
       GetPrivateProfileString(_RCT("NameList"),key,_RCT(""),name,256,home.getinifilename());
       GetPrivateProfileString(_RCT("TitleList"),key,_RCT(""),title,256,home.getinifilename());
       if (strlen(name) > 0 && strlen(title) > 0)
         {  strcpy(names[n],name);  strcpy(titles[n],title);
            n++;
         }
    }

#ifdef USING_WIDGETS
   // this is done in the QtMainWindow
  if(w && w->gethwnd())
      ((QtMainWindow*)w->gethwnd())->updateRecentFileActions();

#else
  if ((hmenu = GetSubMenu(GetMenu(window->gethwnd()),0)) != 0)
    {  for (i = 0 ; i < MAX_OPENLIST ; i++)
         RemoveMenu(hmenu,16,MF_BYPOSITION);
       for (i = n-1 ; i >= 0 ; i--)
         AppendMenu(hmenu,MF_STRING,108+i,(RCLPCTSTR)names[i]);
    }
#endif
#endif
}

void OpenList::opendatabase(int i)
{
#if defined(WIN32)
StorageFile sinfile;
#endif
 CadFile oinfile;
 CadFile *infile;
  if (i >= n || strlen(names[i]) == 0) return;
#if defined(WIN32) && ! defined(_WIN32_WCE)
 RCCHAR fname[256],*ws;
 const RCCHAR *s;
  s = names[i];  ws = fname;
  while ((*ws++ = *s++) != 0) {}
  if (StgIsStorageFile(fname) == S_OK)
    infile = & sinfile;
  else
#endif
    infile = & oinfile;
  if (infile->open(names[i],_RCT("rb")))
    {  db.clear();
       db.settitle(titles[i]);
       cadwindow->updatetitle();
       db.setname(names[i]);
       if (db.load(infile))
         program->fileopenapps();
       else
         db.initialize();
       infile->close();
       cadwindow->paintall();
    }
  else
    {RCCHAR message[300];
       sprintf(message,"The file '%s' could not be found.",names[i]);
       cadwindow->MessageBox(message,"Open recent file",MB_ICONINFORMATION);
    }
}

void OpenList::openeddatabase(const RCCHAR *name,const RCCHAR *title)
{
  int i;
  RCHMENU hmenu;
  RCCHAR key[256];
  for (i = 0 ; i < n ; i++)
    {  if (_stricmp(name,names[i]) == 0 && _stricmp(title,titles[i]) == 0)
         return;
    }
  if (i < MAX_OPENLIST)
    {  strcpy(names[i],name);  strcpy(titles[i],title);
       n = i + 1;
    }
  else
    {  for (i = 0 ; i < MAX_OPENLIST - 1 ; i++)
         {  strcpy(names[i],names[i+1]);  strcpy(titles[i],titles[i+1]);
         }
       strcpy(names[MAX_OPENLIST-1],name);  strcpy(titles[MAX_OPENLIST-1],title);
    }
#ifdef USING_WIDGETS
    if(window->gethwnd())
    {
        ((QtMainWindow*)window->gethwnd())->setCurrentFile(QString(name));
    }
#else
#if ! defined(_WIN32_WCE)
  if ((hmenu = GetSubMenu(GetMenu(window->gethwnd()),0)) != NULL)
  {
       for (i = 0 ; i < MAX_OPENLIST ; i++)
         RemoveMenu(hmenu,16,MF_BYPOSITION);
       for (i = n-1 ; i >= 0 ; i--)
         {
            AppendMenu(hmenu,MF_STRING,108+i,(RCLPCTSTR)names[i]);
            sprintf(key,"op::file%d",i);
            WritePrivateProfileString(_RCT("NameList"),key,names[i],home.getinifilename());
            WritePrivateProfileString(_RCT("TitleList"),key,titles[i],home.getinifilename());
         }
  }
#endif
#endif
}

//
//  DisplayList class information
//

DisplayList::DisplayList()
{ errorstatus = 0;
  countmode = 0;
  minmaxvalid = 0;
  errorstatus = 1;
  surface = 0;
}


void DisplayList::initialise(int initialsize,View3dSurface *s)
{long i;
  errorstatus = 0;
  countmode = 0;
  minmaxvalid = 0;
  surface = s;
  if (initialsize == 0 || v.getinteger("db::disablefastrepaint") != 0)
    errorstatus = 1;
  else
    {  tablesize = initialsize;
       if ((entitydisplaylist = new char * [tablesize]) == NULL)
         errorstatus = 2;
       else
         {  for (i = 0 ; i < tablesize ; i++)
              entitydisplaylist[i] = NULL;
         }
    }
}

void DisplayList::setcountmode(void)
{  countmode = 1;
}

void DisplayList::setsavemode(void)
{  countmode = 0;
}

DisplayList::~DisplayList()
{long i;
  if (errorstatus) return;
  for (i = 0 ; i < tablesize ; i++)
    delete [] entitydisplaylist[i];
  delete [] entitydisplaylist;
}

void DisplayList::clear()
{long i;
  if (errorstatus) return;
  for (i = 0 ; i < tablesize ; i++)
    {  delete [] entitydisplaylist[i];
       entitydisplaylist[i] = 0;
    }
}

int DisplayList::defined(Entity *e)
{long id = e->getid();
  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize) return 0;
  return entitydisplaylist[id] != NULL;
}

void DisplayList::del(Entity *e)
{long id = e->getid();
  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize) return;
  delete [] entitydisplaylist[id];
  entitydisplaylist[id] = 0;
#ifdef USING_WIDGETS
  if(e->qgi)
  {
      if(cadwindow && cadwindow->getcurrentsurface())
          cadwindow->getcurrentsurface()->getScene()->removeItem(e->qgi);
      delete e->qgi;
      e->qgi=0;
  }
#endif
}

void DisplayList::setorgtransform(double umin,double vmin,double umax,double vmax)
{int w,h;
  w = int(surface->getwidth());
  h = int(surface->getheight());
  scalex =  w / (umax - umin);
  offsetx = -umin * scalex;
  scaley =  h / (vmax - vmin);
  offsety = -vmin * scaley;
  sx = DL_SCALE;  ox = 0.0;  sy = DL_SCALE;  oy = 0.0;
}

void DisplayList::setdrawtransform(double *umin,double *vmin,double *umax,double *vmax)
{int w,h;
  if (scalex == 0.0 || scaley == 0.0)
    {  clear();
       setorgtransform(*umin,*vmin,*umax,*vmax);
    }
  w = int(surface->getwidth());
  h = int(surface->getheight());
  sx = (*umax - *umin) / w * scalex * DL_SCALE;
  ox = -w / (*umax - *umin) * (*umin + offsetx / scalex);
  sy = (*vmax - *vmin) / h * scaley * DL_SCALE;
  oy = -h / (*vmax - *vmin) * (*vmin + offsety / scaley);

  //qDebug() << "DisplayList::setdrawtransform sx : " << sx;
  //qDebug() << "DisplayList::setdrawtransform ox : " << ox;
  //qDebug() << "DisplayList::setdrawtransform sy : " << sy;
  //qDebug() << "DisplayList::setdrawtransform oy : " << oy;
}

int DisplayList::getextents(double *u1,double *v1,double *u2,double *v2)
{ if (minmaxvalid)
    {  *u1 = (minx / sx + ox) / surface->getwidth() * (surface->getumax() - surface->getumin()) + surface->getumin();
       *v1 = (miny / sy + oy) / surface->getheight() * (surface->getvmax() - surface->getvmin()) + surface->getvmin();
       *u2 = (maxx / sx + ox) / surface->getwidth() * (surface->getumax() - surface->getumin()) + surface->getumin();
       *v2 = (maxy / sy + oy) / surface->getheight() * (surface->getvmax() - surface->getvmin()) + surface->getvmin();
       return 1;
    }
  else
    return 0;
}

#ifdef USING_WIDGETS
int DisplayList::draw(Entity *e,int dm, QPainter *painter)
{double rw,rh,x1,y1,x2,y2,newx,newy;
 RCHFONT oldhfont,hfont;
 LOGFONTW lf;
 long id,size;
 int i,tnverts,filled,nloops,*nvert;
 Point origin,xaxis,yaxis,**verts;
 RCCOLORREF colour;
 RCHRGN clipregion;

 QGraphicsItem *qgi=0;

  id = e->getid();
  // this return causes problems with the Mac debugger???
  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize ||
           id >= 0 && (entitydisplaylist != 0 && entitydisplaylist[id] == NULL)) return 0;
  //if(errorstatus)return 0;
  //if(e->getposition() == (OneGeneral*)NULL) return 0;
  //if(id < 0) return 0;
  //if(id >= tablesize) return 0;
  //if(entitydisplaylist[id] == NULL) return 0;

  if(/*e->type() == circle_entity || e->type() == ellipse_entity || e->type() == curve_entity ||*/
     e->type() == plane_entity  || e->type() == ruledsurface_entity || e->type() == surfaceofrev_entity ||
     e->type() == patch_entity)
      return 0;

  dlposition = entitydisplaylist[id];
  hfont = oldhfont = 0;
  clipregion = 0;
  while (*dlposition != DL_end)
    {
       switch (*dlposition)
         {  case DL_colour:
              surface->setcolour(*((unsigned char *)(dlposition+1)),dm);
              dlposition += 2;
              break;
            case DL_style:
              if (dm == DM_NORMAL)
                surface->setstyle(*((float *)(dlposition+2)),0,*((unsigned char *)(dlposition+1)),dm);
              dlposition += 6;
              break;
            case DL_weight:
              surface->setweight(*((unsigned char *)(dlposition+1)),dm);
              dlposition += 2;
              break;
            case DL_moveto :
              lastx = *((double *)(dlposition + 1));
              lasty = *((double *)(dlposition + 9));
              dlposition += 17;
              break;
            case DL_pointat :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = newx / sx + ox-3;
              y1 = newy / sy + oy;
              x2 = newx / sx + ox+4;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   if(e->type() == figure_entity)
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                   //surface->lineto(x2,y2);
              }
              x1 = newx / sx + ox;
              y1 = newy / sy + oy-3;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy+4;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   if(e->type() == figure_entity)
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                   //surface->lineto(x2,y2);
              }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_lineto :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = lastx / sx + ox;
              y1 = lasty / sy + oy;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   //surface->lineto(x2,y2);
                   if(e->type() == line_entity)
                   {
                       //((Line*)e)->qgi = surface->lineto(((Line*)e)->qgi,x2,y2);
                       //qgi = e->getqgi(surface);
                       //qgi = surface->lineto(qgi,x2,y2);
                       //e->putqgi(surface,qgi);
                       //if(e->getqgi(surface))
                       //    e->getqgi(surface)->setZValue(QGMAXZVALUE+QGZVALINC);
                       surface->lineto(painter,x2,y2);
                   }
                   else if(e->type() == figure_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi)
                           e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == circle_entity)
                   {
                       //if(e->qgi)
                       //    ((QGraphicsItemGroup*)((Circle*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       //if(e->qgi)
                       //    e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                       surface->lineto(painter,x2,y2);
                   }
                   else if(e->type() == ellipse_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Circle*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == curve_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Curve*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == lineardimension_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Curve*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
              }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_selectfont :
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   hfont = 0;
                }
#ifdef _USING_QTCHAR
              memcpy(&lf,dlposition+9,sizeof(LOGFONTW));
#else
              memcpy(&lf,dlposition+9,sizeof(LOGFONT));
#endif
              rw = *((long *)(dlposition + 1)) / sx;
              rh = *((long *)(dlposition + 5)) / sy;
              if (rh > 2000.0) rh = 2000.0; else if (rh < -2000.0) rh = -2000.0;
              if (rw > 2000.0) rw = 2000.0; else if (rw < -2000.0) rw = -2000.0;
              lf.lfHeight = int(rh);
              lf.lfWidth = int(rw);
              if (lf.lfHeight > 0)
                {
#ifdef _USING_QTCHAR
                    if ((hfont = CreateFontIndirectW(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#else
                    if ((hfont = CreateFontIndirect(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#endif
                }
#ifdef _USING_QTCHAR
              dlposition += 9 + sizeof(LOGFONTW);
#else
              dlposition += 9 + sizeof(LOGFONT);
#endif
              break;
            case DL_textout :
#ifndef USING_WIDGETS
              if(e->type() == lineardimension_entity)
                  ((LinearDimension*)e)->text
                  e->draw(DM_NORMAL,0,surface);
              dlposition += 12 + *((short *)(dlposition+9)) * sizeof(RCCHAR) + sizeof(RCCHAR) ;
#else
              x1 = *((long *)(dlposition + 1)) / sx + ox;
              y1 = *((long *)(dlposition + 5)) / sy + oy;
              SetBkMode(surface->gethdc(),TRANSPARENT);
              if (lf.lfHeight > 0)
                surface->textout(0,x1,y1,(RCCHAR*)dlposition+11,*((short *)(dlposition+9)));
              SetBkMode(surface->gethdc(),OPAQUE);
              dlposition += 12 + *((short *)(dlposition+9)) * sizeof(RCCHAR) + sizeof(RCCHAR) ;
              //dlposition += 12 + *((short *)(dlposition+9));
#endif
              break;
            case DL_polygon3d :
              origin = *((Point *)(dlposition+1));
              xaxis = *((Point *)(dlposition+1+24));
              yaxis = *((Point *)(dlposition+1+48));
              filled = *((int *)(dlposition+1+72));
              colour = *((RCCOLORREF *)(dlposition+1+76));

              nloops = *((int *)(dlposition + 1 + 80));

              if (nloops > 0)
                {  nvert = new int[nloops];
                   verts = new Point *[nloops];
                   for (i = 0 ; i < nloops ; i++)
                     {  nvert[i] = *((int *)(dlposition+1+80+4+i*4));
                        verts[i] = new Point[nvert[i]];
                     }

                   for (i = 0,tnverts = 0 ; i < nloops ; tnverts += nvert[i],i++)
                     memcpy(verts[i],dlposition+1+80+4+nloops*4+tnverts*24,24 * nvert[i]);

                   Polygon3d polygon3d(origin,xaxis,yaxis,nloops,nvert,verts);

                   polygon3d.clip(surface);

                  if (clipregion != 0)
                     clipregion = CreateRectRgn(0,0,1,1);
                   if (GetClipRgn(surface->gethdc(),clipregion) <= 0)
                     {  DeleteObject(clipregion);
                        clipregion = 0;
                     }

                   if (polygon3d.getdefined() != 0)
                     {  if (filled)
                          polygon3d.fill(dm,0,surface);
                        else
                          polygon3d.setclipregion(0,surface);
                     }
                   else
                     IntersectClipRect(surface->gethdc(),-1,-1,0,0);

                }
              else
                SelectClipRgn(surface->gethdc(),0);

              size = 1 + 80 + 4 + nloops * 4;
              for (i = 0 ; i < nloops ; i++)
                size += *((int *)(dlposition+1+80+4+i*4)) * 24;

              dlposition += size;
              break;
            //case  DL_circleitem :
            //   qDebug() << "found a circle item";
            //   break;
            default:
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   return 1;
                }
         }
    }
  if (hfont != 0)
    {  SelectObject(surface->gethdc(),oldhfont);
       DeleteObject(hfont);
    }

  SelectClipRgn(surface->gethdc(),clipregion);
  DeleteObject(clipregion);

  return 1;
}
#endif

int DisplayList::draw(Entity *e,int dm)
{double rw,rh,x1,y1,x2,y2,newx,newy;
 RCHFONT oldhfont,hfont;
 LOGFONTW lf;
 long id,size;
 int i,tnverts,filled,nloops,*nvert;
 Point origin,xaxis,yaxis,**verts;
 RCCOLORREF colour;
 RCHRGN clipregion;

 QGraphicsItem *qgi=0;

  id = e->getid();
  // this return causes problems with the Mac debugger???
  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize ||
           id >= 0 && (entitydisplaylist != 0 && entitydisplaylist[id] == NULL)) return 0;
  //if(errorstatus)return 0;
  //if(e->getposition() == (OneGeneral*)NULL) return 0;
  //if(id < 0) return 0;
  //if(id >= tablesize) return 0;
  //if(entitydisplaylist[id] == NULL) return 0;

  if(/*e->type() == circle_entity || e->type() == ellipse_entity || e->type() == curve_entity ||*/
     e->type() == plane_entity  || e->type() == ruledsurface_entity || e->type() == surfaceofrev_entity ||
     e->type() == patch_entity)
      return 0;

  dlposition = entitydisplaylist[id];
  hfont = oldhfont = 0;
  clipregion = 0;
  while (*dlposition != DL_end)
    {
       switch (*dlposition)
         {  case DL_colour:
              surface->setcolour(*((unsigned char *)(dlposition+1)),dm);
              dlposition += 2;
              break;
            case DL_style:
              if (dm == DM_NORMAL)
                surface->setstyle(*((float *)(dlposition+2)),0,*((unsigned char *)(dlposition+1)),dm);
              dlposition += 6;
              break;
            case DL_weight:
              surface->setweight(*((unsigned char *)(dlposition+1)),dm);
              dlposition += 2;
              break;
            case DL_moveto :
              lastx = *((double *)(dlposition + 1));
              lasty = *((double *)(dlposition + 9));
              dlposition += 17;
              break;
            case DL_pointat :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = newx / sx + ox-3;
              y1 = newy / sy + oy;
              x2 = newx / sx + ox+4;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   if(e->type() == figure_entity)
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                   //surface->lineto(x2,y2);
              }
              x1 = newx / sx + ox;
              y1 = newy / sy + oy-3;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy+4;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   if(e->type() == figure_entity)
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                   //surface->lineto(x2,y2);
              }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_lineto :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = lastx / sx + ox;
              y1 = lasty / sy + oy;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
              {
                   surface->moveto(x1,y1);
                   //surface->lineto(x2,y2);
                   if(e->type() == line_entity)
                   {
                       //((Line*)e)->qgi = surface->lineto(((Line*)e)->qgi,x2,y2);
                       //qgi = e->getqgi(surface);
                       qgi = e->qgi;
                       qgi = surface->lineto(qgi,x2,y2);
                       //e->putqgi(surface,qgi);
                       e->qgi=qgi;
                       if(e->qgi)
                           e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                       //if(e->getqgi(surface))
                       //    e->getqgi(surface)->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == figure_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Figure*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi)
                           e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == circle_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Circle*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi)
                           e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                       //if(e->getqgi(surface))
                       //    ((QGraphicsItemGroup*)((Circle*)e)->getqgi(surface))->addToGroup(surface->lineto(0,x2,y2));
                       //if(e->getqgi(surface))
                       //    e->getqgi(surface)->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == ellipse_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Circle*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == curve_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Curve*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
                   else if(e->type() == lineardimension_entity)
                   {
                       if(e->qgi)
                           ((QGraphicsItemGroup*)((Curve*)e)->qgi)->addToGroup(surface->lineto((QGraphicsItem*)0,x2,y2));
                       if(e->qgi) e->qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                   }
              }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_selectfont :
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   hfont = 0;
                }
#ifdef _USING_QTCHAR
              memcpy(&lf,dlposition+9,sizeof(LOGFONTW));
#else
              memcpy(&lf,dlposition+9,sizeof(LOGFONT));
#endif
              rw = *((long *)(dlposition + 1)) / sx;
              rh = *((long *)(dlposition + 5)) / sy;
              if (rh > 2000.0) rh = 2000.0; else if (rh < -2000.0) rh = -2000.0;
              if (rw > 2000.0) rw = 2000.0; else if (rw < -2000.0) rw = -2000.0;
              lf.lfHeight = int(rh);
              lf.lfWidth = int(rw);
              if (lf.lfHeight > 0)
                {
#ifdef _USING_QTCHAR
                    if ((hfont = CreateFontIndirectW(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#else
                    if ((hfont = CreateFontIndirect(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#endif
                }
#ifdef _USING_QTCHAR
              dlposition += 9 + sizeof(LOGFONTW);
#else
              dlposition += 9 + sizeof(LOGFONT);
#endif
              break;
            case DL_textout :
#ifndef USING_WIDGETS
              if(e->type() == lineardimension_entity)
                  ((LinearDimension*)e)->text
                  e->draw(DM_NORMAL,0,surface);
              dlposition += 12 + *((short *)(dlposition+9)) * sizeof(RCCHAR) + sizeof(RCCHAR) ;
#else
              x1 = *((long *)(dlposition + 1)) / sx + ox;
              y1 = *((long *)(dlposition + 5)) / sy + oy;
              SetBkMode(surface->gethdc(),TRANSPARENT);
              if (lf.lfHeight > 0)
                surface->textout(0,x1,y1,(RCCHAR*)dlposition+11,*((short *)(dlposition+9)));
              SetBkMode(surface->gethdc(),OPAQUE);
              dlposition += 12 + *((short *)(dlposition+9)) * sizeof(RCCHAR) + sizeof(RCCHAR) ;
              //dlposition += 12 + *((short *)(dlposition+9));
#endif
              break;
            case DL_polygon3d :
              origin = *((Point *)(dlposition+1));
              xaxis = *((Point *)(dlposition+1+24));
              yaxis = *((Point *)(dlposition+1+48));
              filled = *((int *)(dlposition+1+72));
              colour = *((RCCOLORREF *)(dlposition+1+76));

              nloops = *((int *)(dlposition + 1 + 80));

              if (nloops > 0)
                {  nvert = new int[nloops];
                   verts = new Point *[nloops];
                   for (i = 0 ; i < nloops ; i++)
                     {  nvert[i] = *((int *)(dlposition+1+80+4+i*4));
                        verts[i] = new Point[nvert[i]];
                     }

                   for (i = 0,tnverts = 0 ; i < nloops ; tnverts += nvert[i],i++)
                     memcpy(verts[i],dlposition+1+80+4+nloops*4+tnverts*24,24 * nvert[i]);

                   Polygon3d polygon3d(origin,xaxis,yaxis,nloops,nvert,verts);

                   polygon3d.clip(surface);

                  if (clipregion != 0)
                     clipregion = CreateRectRgn(0,0,1,1);
                   if (GetClipRgn(surface->gethdc(),clipregion) <= 0)
                     {  DeleteObject(clipregion);
                        clipregion = 0;
                     }

                   if (polygon3d.getdefined() != 0)
                     {  if (filled)
                          polygon3d.fill(dm,0,surface);
                        else
                          polygon3d.setclipregion(0,surface);
                     }
                   else
                     IntersectClipRect(surface->gethdc(),-1,-1,0,0);

                }
              else
                SelectClipRgn(surface->gethdc(),0);

              size = 1 + 80 + 4 + nloops * 4;
              for (i = 0 ; i < nloops ; i++)
                size += *((int *)(dlposition+1+80+4+i*4)) * 24;

              dlposition += size;
              break;
            //case  DL_circleitem :
            //   qDebug() << "found a circle item";
            //   break;
            default:
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   return 1;
                }
         }
    }
  if (hfont != 0)
    {  SelectObject(surface->gethdc(),oldhfont);
       DeleteObject(hfont);
    }

  SelectClipRgn(surface->gethdc(),clipregion);
  DeleteObject(clipregion);

  return 1;
}

double DisplayList::pick(Entity *e,double x,double y)
{long id = e->getid();
 long size;
 double dir1,dir2,dir3,dir4,rw,rh,d,x1,y1,x2,y2,x3,y3,x4,y4,dx1,dy1,dx2,dy2,denom,t,toler,toler2,a,cosa,sina,newx,newy;
#ifdef _USING_QTCHAR
 LOGFONTW lf;
#else
 LOGFONT lf;
#endif
 RCHFONT hfont,oldhfont;
 int i,in,nloops,tnverts,*nvert,inpolygon,filled;
 Point origin,xaxis,yaxis,**verts;
 RCHRGN clipregion;

  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize) return 0;
  toler = db.getstoler();  toler2 = toler * toler;
  dlposition = entitydisplaylist[id];
  hfont = oldhfont = 0;
  inpolygon = 1;
  clipregion = 0;

  while (*dlposition != DL_end)
    {  switch (*dlposition)
         {  case DL_colour:
            case DL_weight:
              dlposition += 2;
              break;
            case DL_style:
              dlposition += 6;
              break;
            case DL_moveto :
              lastx = *((double *)(dlposition + 1));
              lasty = *((double *)(dlposition + 9));
              dlposition += 17;
              break;
            case DL_pointat :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = newx / sx + ox;
              y1 = newy / sy + oy;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
                {  dx1 = fabs(x1 - x);
                   dy1 = fabs(y1 - y);
                   if (dx1 < toler && dy1 < toler && inpolygon)
                     {   if (hfont != 0)
                          {  SelectObject(surface->gethdc(),oldhfont);
                              DeleteObject(hfont);
                           }
                         return sqrt(dx1*dx1+dy1*dy1);
                     }
                }
              dlposition += 17;
              break;
            case DL_lineto :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = lastx / sx + ox;
              y1 = lasty / sy + oy;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy;
              if (surface->clip2d(&x1,&y1,&x2,&y2))
                {  dx1 = x2 - x1;  dy1 = y2 - y1;
                   dx2 =  x - x1;  dy2 = y - y1;
                   denom = dx1*dx1 + dy1*dy1;
                   if (denom < 1.0)
                     {  dx1 = fabs((x1+x2)/2.0 - x);
                        dy1 = fabs((y1+y2)/2.0 - y);
                        if (dx1 < toler && dy1 < toler && inpolygon)
                          {   if (hfont != 0)
                                {  SelectObject(surface->gethdc(),oldhfont);
                                   DeleteObject(hfont);
                                }
                              return sqrt(dx1*dx1+dy1*dy1);
                          }
                     }
                   else
                     {  t = (dx1*dx2 + dy1*dy2) / denom;
                        if (t >= 0.0 && t <= 1.0)
                          {  d = dy1 * x - dx1 * y - dy1 * x1 + dx1 * y1;
                             d = d * d / denom;
                             if (d < toler2 && inpolygon)
                               {  if (hfont != 0)
                                    {  SelectObject(surface->gethdc(),oldhfont);
                                       DeleteObject(hfont);
                                    }
                                  return sqrt(d);
                               }
                          }
                     }
                }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_selectfont :
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   hfont = 0;
                }
#ifdef _USING_QTCHAR
              memcpy(&lf,dlposition+9,sizeof(LOGFONTW));
#else
              memcpy(&lf,dlposition+9,sizeof(LOGFONT));
#endif
              rw = *((long *)(dlposition + 1)) / sx;
              rh = *((long *)(dlposition + 5)) / sy;
              if (rh > 2000.0) rh = 2000.0; else if (rh < -2000.0) rh = -2000.0;
              if (rw > 2000.0) rw = 2000.0; else if (rw < -2000.0) rw = -2000.0;
              lf.lfHeight = int(rh);
              lf.lfWidth = int(rw);
              if (lf.lfHeight > 0)
                {
#ifdef _USING_QTCHAR
                    if ((hfont = CreateFontIndirectW(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#else
                    if ((hfont = CreateFontIndirect(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#endif
                }
#ifdef _USING_QTCHAR
              dlposition += 9 + sizeof(LOGFONTW);
#else
              dlposition += 9 + sizeof(LOGFONT);
#endif
              break;
            case DL_textout :
              x1 = *((long *)(dlposition + 1)) / sx + ox;
              y1 = *((long *)(dlposition + 5)) / sy + oy;
              if (lf.lfHeight == 0)
                rw *= *((short *)(dlposition+9)) * (rw == 0.0 ? rh : rw);
              else
                {
#if defined(WIN32)
                   SIZE size;
#ifdef _USING_QTCHAR
                   GetTextExtentPoint32W(surface->gethdc(),((RCLPCTSTR)(dlposition+11)),*((short *)(dlposition+9)),&size);
#else
                   GetTextExtentPoint(surface->gethdc(),dlposition+11,*((short *)(dlposition+9)),&size);
#endif
                   rw = size.cx;
                   rh = size.cy;
#else
                   long r;
                   //r = GetTextExtent(surface->gethdc(),dlposition+11,*((short *)(dlposition+9)));
                   rw = LOWORD(r);
                   rh = HIWORD(r);
#endif
                }
              a = lf.lfOrientation * M_PI / 1800.0;
              cosa = cos(a);
              sina = sin(a);
              x2 = x1 + cosa * rw;
              y2 = y1 + sina * rw;
              x3 = x2 + sina * rh;
              y3 = y2 - cosa * rh;
              x4 = x3 - cosa * rw;
              y4 = y3 - sina * rw;
              dir1 = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
              dir2 = (x3 - x2) * (y - y2) - (y3 - y2) * (x - x2);
              dir3 = (x4 - x3) * (y - y3) - (y4 - y3) * (x - x3);
              dir4 = (x1 - x4) * (y - y4) - (y1 - y4) * (x - x4);
              if (rw > 0 && rh > 0 && inpolygon &&
                    (dir1 <= 0.0 && dir2 <= 0.0 && dir3 <= 0.0 && dir4 <= 0.0 ||
                     dir1 >= 0.0 && dir2 >= 0.0 && dir3 >= 0.0 && dir4 >= 0.0))
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0.0;
                }
              dlposition += 12 + *((short *)(dlposition+9))*sizeof(RCCHAR)+sizeof(RCCHAR);
              //dlposition += 12 + *((short *)(dlposition+9));
              break;
            case DL_polygon3d :

#ifdef USING_WIDGETS
              if(e->qgi && e->type() == plane_entity && ((Plane*)e)->qgplane)
              {
                  filled = *((int *)(dlposition+1+72));
                  nloops = *((int *)(dlposition + 1 + 80));
                  if(nloops > 0)
                  {
                      x1 = x;  y1 = y;
                      surface->transform2d(&x1,&y1);
                      QPointF tp(x1,y1);
                      inpolygon = in = ((Plane*)e)->qgplane->contains(tp);
                      if (in && filled)
                           return 0.0;

                      if (filled)
                        inpolygon = 1;  //  If this polygon is a drawn polygon let other lines be selected.

                  }
                  else
                    inpolygon = 1;
              }
              else
              {
                  origin = *((Point *)(dlposition+1));
                  xaxis = *((Point *)(dlposition+1+24));
                  yaxis = *((Point *)(dlposition+1+48));
                  filled = *((int *)(dlposition+1+72));

                  nloops = *((int *)(dlposition + 1 + 80));

                  if (nloops > 0)
                    {  nvert = new int[nloops];
                       verts = new Point *[nloops];

                       for (i = 0 ; i < nloops ; i++)
                         {  nvert[i] = *((int *)(dlposition+1+80+4+i*4));
                            verts[i] = new Point[nvert[i]];
                         }

                       for (i = 0,tnverts = 0 ; i < nloops ; tnverts += nvert[i],i++)
                         memcpy(verts[i],dlposition+1+80+4+nloops*4+tnverts*24,24 * nvert[i]);

                       Polygon3d polygon3d(origin,xaxis,yaxis,nloops,nvert,verts);
                       polygon3d.clip(surface);
                       if (polygon3d.getdefined())
                       {
                            clipregion = CreateRectRgn(0,0,1,1);
                            if (GetClipRgn(surface->gethdc(),clipregion) <= 0)
                              {  DeleteObject(clipregion);
                                 clipregion = 0;
                              }
                            polygon3d.setclipregion(0,surface);
                            x1 = x;  y1 = y;
                            surface->transform2d(&x1,&y1);
                            inpolygon = in = PtVisible(surface->gethdc(),(int)x1,(int)y1) != 0;
                            SelectClipRgn(surface->gethdc(),clipregion);
                            DeleteObject(clipregion);
                            if (in && filled)
                              {  if (hfont != 0)
                                   {  SelectObject(surface->gethdc(),oldhfont);
                                      DeleteObject(hfont);
                                   }
                                 return 0.0;
                              }
                       }
                       if (filled)
                         inpolygon = 1;  //  If this polygon is a drawn polygon let other lines be selected.

                    }
                  else
                    inpolygon = 1;
              }
#else
              origin = *((Point *)(dlposition+1));
              xaxis = *((Point *)(dlposition+1+24));
              yaxis = *((Point *)(dlposition+1+48));
              filled = *((int *)(dlposition+1+72));

              nloops = *((int *)(dlposition + 1 + 80));

              if (nloops > 0)
                {  nvert = new int[nloops];
                   verts = new Point *[nloops];

                   for (i = 0 ; i < nloops ; i++)
                     {  nvert[i] = *((int *)(dlposition+1+80+4+i*4));
                        verts[i] = new Point[nvert[i]];
                     }

                   for (i = 0,tnverts = 0 ; i < nloops ; tnverts += nvert[i],i++)
                     memcpy(verts[i],dlposition+1+80+4+nloops*4+tnverts*24,24 * nvert[i]);

                   Polygon3d polygon3d(origin,xaxis,yaxis,nloops,nvert,verts);
                   polygon3d.clip(surface);
                   if (polygon3d.getdefined())
                   {
                        clipregion = CreateRectRgn(0,0,1,1);
                        if (GetClipRgn(surface->gethdc(),clipregion) <= 0)
                          {  DeleteObject(clipregion);
                             clipregion = 0;
                          }
                        polygon3d.setclipregion(0,surface);
                        x1 = x;  y1 = y;
                        surface->transform2d(&x1,&y1);
                        inpolygon = in = PtVisible(surface->gethdc(),(int)x1,(int)y1) != 0;
                        SelectClipRgn(surface->gethdc(),clipregion);
                        DeleteObject(clipregion);
                        if (in && filled)
                          {  if (hfont != 0)
                               {  SelectObject(surface->gethdc(),oldhfont);
                                  DeleteObject(hfont);
                               }
                             return 0.0;
                          }
                   }
                   if (filled)
                     inpolygon = 1;  //  If this polygon is a drawn polygon let other lines be selected.

                }
              else
                inpolygon = 1;
#endif

              size = 1 + 80 + 4 + nloops * 4;
              for (i = 0 ; i < nloops ; i++)
                size += *((int *)(dlposition+1+80+4+i*4)) * 24;

              dlposition += size;

              break;

            default:
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                }
              return -1.0;
         }
    }
  if (hfont != 0)
    {  SelectObject(surface->gethdc(),oldhfont);
       DeleteObject(hfont);
    }
  return -1.0;
}

double DisplayList::pickhor(Entity *e,int x,int y)
{long id = e->getid();
 static const double tolerance = 0.0001;
 long size;
 double newx,newy,d,dmin,x1,y1,x2,y2;
 int i,nloops;

  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize)
    return -1.0;
  dlposition = entitydisplaylist[id];
  dmin = -1.0;
  while (*dlposition != DL_end)
    {  switch (*dlposition)
         {  case DL_colour:
            case DL_weight:
              dlposition += 2;
              break;
            case DL_style:
              dlposition += 6;
              break;
            case DL_moveto :
              lastx = *((double *)(dlposition + 1));
              lasty = *((double *)(dlposition + 9));
              dlposition += 17;
              break;
            case DL_pointat :
              dlposition += 17;
              break;
            case DL_lineto :
              newx = *((double *)(dlposition + 1));
              newy = *((double *)(dlposition + 9));
              x1 = lastx / sx + ox;
              y1 = lasty / sy + oy;
              x2 = newx / sx + ox;
              y2 = newy / sy + oy;
              if (y1 < y2 - tolerance && y1 - tolerance <= y && y <= y2 + tolerance ||
                  y2 < y1 - tolerance && y2 - tolerance <= y && y <= y1 + tolerance)
                {  d = fabs(x1 + (y - y1) * (x2 - x1) / (y2 - y1) - x);
                   if (dmin == -1.0 || d < dmin)
                     dmin = d;
                }
              lastx = newx;
              lasty = newy;
              dlposition += 17;
              break;
            case DL_selectfont :
#ifdef _USING_QTCHR
              dlposition += 9 + sizeof(LOGFONTW);
#else
              dlposition += 9 + sizeof(LOGFONT);
#endif
              break;
            case DL_textout :
              dlposition += 12 + *((short *)(dlposition+9))*sizeof(RCCHAR)+sizeof(RCCHAR);
              //dlposition += 12 + *((short *)(dlposition+9));
              break;


            case DL_polygon3d :

              nloops = *((int *)(dlposition + 1 + 80));

              size = 1 + 80 + 4 + nloops * 4;
              for (i = 0 ; i < nloops ; i++)
                size += *((int *)(dlposition+1+80+4+i*4)) * 24;

              dlposition += size;

              break;

            default:
              return dmin;
         }
    }
  return dmin;
}

int DisplayList::pick(Entity *e,int insidew,int x1,int y1,int x2,int y2)
{long id = e->getid(),size;
 int atleastone;
#ifdef _USING_QTCHAR
 LOGFONTW lf;
#else
 LOGFONT lf;
#endif
 double rw,rh,x3,y3,a,cosa,sina;
 RCHFONT hfont,oldhfont;
 Point origin,xaxis,yaxis;
 int i,nloops;

  if (errorstatus || e->getposition() == NULL || id < 0 || id >= tablesize) return 0;
  dlposition = entitydisplaylist[id];
  atleastone = 0;
  hfont = oldhfont = 0;
  while (*dlposition != DL_end)
    {  switch (*dlposition)
         {  case DL_colour:
            case DL_weight:
              dlposition += 2;
              break;
            case DL_style:
              dlposition += 6;
              break;
            case DL_pointat :
            case DL_moveto :
              lastx = *((double *)(dlposition + 1)) / sx + ox;
              lasty = *((double *)(dlposition + 9)) / sy + oy;
              if ((x1 < lastx && lastx < x2 && y1 < lasty && lasty < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              atleastone = 1;
              dlposition += 17;
              break;
            case DL_lineto :
              lastx = *((double *)(dlposition + 1)) / sx + ox;
              lasty = *((double *)(dlposition + 9)) / sy + oy;
              if ((x1 < lastx && lastx < x2 && y1 < lasty && lasty < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              atleastone = 1;
              dlposition += 17;
              break;
            case DL_selectfont :
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   hfont = 0;
                }
#ifdef _USING_QTCHAR
              memcpy(&lf,dlposition+9,sizeof(LOGFONTW));
#else
              memcpy(&lf,dlposition+9,sizeof(LOGFONT));
#endif
              rw = *((long *)(dlposition + 1)) / sx;
              rh = *((long *)(dlposition + 5)) / sy;
              if (rh > 2000.0) rh = 2000.0; else if (rh < -2000.0) rh = -2000.0;
              if (rw > 2000.0) rw = 2000.0; else if (rw < -2000.0) rw = -2000.0;
              lf.lfHeight = int(rh);
              lf.lfWidth = int(rw);
              if (lf.lfHeight > 0)
                {
#ifdef _USING_QTCHAR
                    if ((hfont = CreateFontIndirectW(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#else
                    if ((hfont = CreateFontIndirect(&lf)) != 0)
                     oldhfont = (RCHFONT) SelectObject(surface->gethdc(),hfont);
#endif
                }
              atleastone = 1;
#ifdef _USING_QTCHAR
              dlposition += 9 + sizeof(LOGFONTW);
#else
              dlposition += 9 + sizeof(LOGFONT);
#endif
              break;
            case DL_textout :
              x3 = *((long *)(dlposition + 1)) / sx + ox;
              y3 = *((long *)(dlposition + 5)) / sy + oy;
              if (lf.lfHeight == 0)
                rw *= *((short *)(dlposition+9)) * (rw == 0.0 ? rh : rw);
              else
                {
#if defined(WIN32)
                   SIZE size;
#ifdef _USING_QTCHAR
                   GetTextExtentPoint32W(surface->gethdc(),(RCLPCTSTR)(dlposition+11),*((short *)(dlposition+9)),&size);
#else
                   GetTextExtentPoint(surface->gethdc(),dlposition+11,*((short *)(dlposition+9)),&size);
#endif
                   rw = size.cx;
                   rh = size.cy;
#else
                   long r;
                   //r = GetTextExtent(surface->gethdc(),dlposition+11,*((short *)(dlposition+9)));
                   rw = LOWORD(r);
                   rh = HIWORD(r);
#endif
                }
              a = lf.lfOrientation * M_PI / 1800.0;
              cosa = cos(a);
              sina = sin(a);
              if ((x1 < x3 && x3 < x2 && y1 < y3 && y3 < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              x3 += cosa * rw;
              y3 += sina * rw;
              if ((x1 < x3 && x3 < x2 && y1 < y3 && y3 < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              x3 += sina * rh;
              y3 -= cosa * rh;
              if ((x1 < x3 && x3 < x2 && y1 < y3 && y3 < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              x3 -= cosa * rw;
              y3 -= sina * rw;
              if ((x1 < x3 && x3 < x2 && y1 < y3 && y3 < y2) ^ insidew)
                {  if (hfont != 0)
                     {  SelectObject(surface->gethdc(),oldhfont);
                        DeleteObject(hfont);
                     }
                   return 0;
                }
              dlposition += 12 + *((short *)(dlposition+9))*sizeof(RCCHAR)+sizeof(RCCHAR);
              //dlposition += 12 + *((short *)(dlposition+9));
              atleastone = 1;
              break;

            case DL_polygon3d :

              nloops = *((int *)(dlposition + 1 + 80));

              size = 1 + 80 + 4 + nloops * 4;
              for (i = 0 ; i < nloops ; i++)
                size += *((int *)(dlposition+1+80+4+i*4)) * 24;

              dlposition += size;

              break;

            default:
              if (hfont != 0)
                {  SelectObject(surface->gethdc(),oldhfont);
                   DeleteObject(hfont);
                   return atleastone;
                }
         }
    }
  if (hfont != 0)
    {  SelectObject(surface->gethdc(),oldhfont);
       DeleteObject(hfont);
    }
  return atleastone;
}

int DisplayList::start(Entity *e,long size)
{long i,id = e->getid();
 char ** newtable;
  if (errorstatus || e->getposition() == NULL || id < 0) return 0;
  if (countmode)
    {  dlcount = 0;  return 1;
    }
  while (id >= tablesize)
    {  if ((newtable = new char * [tablesize + EXTEND_TABLE_SIZE]) != NULL)
         {  for (i = 0 ; i < tablesize ; i++)
              newtable[i] = entitydisplaylist[i];
            tablesize += EXTEND_TABLE_SIZE;
            for ( ; i < tablesize ; i++)
              newtable[i] = NULL;
            delete [] entitydisplaylist;
            entitydisplaylist = newtable;
         }
       else
         {  errorstatus = 3;
            return 0;
         }
    }
  dllength = size + 1;
  dlposition = entitydisplaylist[id] = new char[int(dllength)];

  if (dlposition == NULL)
    {  clear();
       errorstatus = 4;
       return 0;
    }
  dlcount = 0;
  return 1;
}

int DisplayList::pointat(double x,double y)
{ if (countmode)
    {  dlcount += 17;  return 1;
    }
  if (dlcount + 17 > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  x = (x - ox) * sx;
  y = (y - oy) * sy;
  if (x > LONG_MIN && x < LONG_MAX &&
      y > LONG_MIN && y < LONG_MAX)
    {  *dlposition = DL_pointat;
       *((double *)(dlposition+1)) = x;
       *((double *)(dlposition+9)) = y;
       if (minmaxvalid)
         {  if (minx > x) minx = x;
            if (maxx < x) maxx = x;
            if (miny > y) miny = y;
            if (maxy < y) maxy = y;
         }
       else
         {  minx = maxx = x;
            miny = maxy = y;
            minmaxvalid = 1;
         }
       dlposition += 17;
       dlcount += 17;
    }
  return 1;
}

int DisplayList::moveto(double x,double y)
{ lastdx = (x - ox) * sx;
  lastdy = (y - oy) * sy;
  if (minmaxvalid)
    {  if (minx > lastdx) minx = lastdx;
       if (maxx < lastdx) maxx = lastdx;
       if (miny > lastdy) miny = lastdy;
       if (maxy < lastdy) maxy = lastdy;
    }
  else
    {  minx = maxx = lastdx;
       miny = maxy = lastdy;
       minmaxvalid = 1;
    }
  return 1;
}

int DisplayList::lineto(double x,double y)
{double newdx,newdy;
    if (countmode)
    {   dlcount += 34;
        return 1;
    }
    if (dlcount + 34 > dllength)
    {   clear();  errorstatus = 5;
        return 0;
    }

  newdx = x = (x - ox) * sx;
  newdy = y = (y - oy) * sy;
  surface->setclip(LONG_MIN,LONG_MIN,LONG_MAX,LONG_MAX);
  if (surface->clip2d(&lastdx,&lastdy,&newdx,&newdy))
    {  *dlposition = DL_moveto;
       *((double *)(dlposition+1)) = lastdx;
       *((double *)(dlposition+9)) = lastdy;
       dlposition += 17;
       dlcount += 17;
       *dlposition = DL_lineto;
       *((double *)(dlposition+1)) = newdx;
       *((double *)(dlposition+9)) = newdy;
       if (minmaxvalid)
         {  if (minx > newdx) minx = newdx;
            if (maxx < newdx) maxx = newdx;
            if (miny > newdy) miny = newdy;
            if (maxy < newdy) maxy = newdy;
         }
       else
         {  minx = maxx = newdx;
            miny = maxy = newdy;
            minmaxvalid = 1;
         }
       dlposition += 17;
       dlcount += 17;
    }
  surface->setclip(0.0,0.0,surface->getwidth(),surface->getheight());
  lastdx = x;
  lastdy = y;

  return 1;
}

#ifdef _USING_QTCHAR
int DisplayList::selectfont(double width,double height,LOGFONTW *lf)
{double newx,newy;
  if (countmode)
    {  dlcount += 9 + sizeof(LOGFONTW);  return 1;
    }
  if (dlcount + 9 + (long)sizeof(LOGFONTW) > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  newx = width * sx;
  newy = height * sy;
  if (newx > LONG_MIN && newx < LONG_MAX && newy > LONG_MIN && newy < LONG_MAX)
    {  *dlposition = DL_selectfont;
       *((long *)(dlposition+1)) = long(newx);
       *((long *)(dlposition+5)) = long(newy);
       dlposition += 9;
       dlcount += 9;
       memcpy(dlposition,lf,sizeof(LOGFONTW));
       dlposition += sizeof(LOGFONTW);
       dlcount += sizeof(LOGFONTW);
    }
  return 1;
}
#else
int DisplayList::selectfont(double width,double height,LOGFONT *lf)
{double newx,newy;
  if (countmode)
    {  dlcount += 9 + sizeof(LOGFONT);  return 1;
    }
  if (dlcount + 9 + (long)sizeof(LOGFONT) > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  newx = width * sx;
  newy = height * sy;
  if (newx > LONG_MIN && newx < LONG_MAX && newy > LONG_MIN && newy < LONG_MAX)
    {  *dlposition = DL_selectfont;
       *((long *)(dlposition+1)) = long(newx);
       *((long *)(dlposition+5)) = long(newy);
       dlposition += 9;
       dlcount += 9;
       memcpy(dlposition,lf,sizeof(LOGFONT));
       dlposition += sizeof(LOGFONT);
       dlcount += sizeof(LOGFONT);
    }
  return 1;
}
#endif

int DisplayList::textout(double x,double y,RCCHAR *s,int l)
{double newx,newy;
  if (countmode)
    {  dlcount += 12 + l*sizeof(RCCHAR)+sizeof(RCCHAR);  return 1;
    }
  if (dlcount + 12 + l*sizeof(RCCHAR)+sizeof(RCCHAR) > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  newx = (x - ox) * sx;
  newy = (y - oy) * sy;
  if (newx > LONG_MIN && newx < LONG_MAX && newy > LONG_MIN && newy < LONG_MAX)
    {  *dlposition = DL_textout;
       *((long *)(dlposition+1)) = long(newx);
       *((long *)(dlposition+5)) = long(newy);
       *((short *)(dlposition+9)) = short(l);
       if (minmaxvalid)
         {  if (minx > newx) minx = newx;
            if (maxx < newx) maxx = newx;
            if (miny > newy) miny = newy;
            if (maxy < newy) maxy = newy;
         }
       else
         {  minx = maxx = newx;
            miny = maxy = newy;
            minmaxvalid = 1;
         }
       memcpy(dlposition+11,s,l*sizeof(RCCHAR)+sizeof(RCCHAR));
       memset(dlposition+11+l*sizeof(RCCHAR),0,sizeof(RCCHAR));
       //dlposition[11+l*sizeof(RCCHAR)+sizeof(RCCHAR)] = 0;
       dlposition += 12 + l*sizeof(RCCHAR)+sizeof(RCCHAR);
       dlcount += 12 + l*sizeof(RCCHAR)+sizeof(RCCHAR);
       //memcpy(dlposition+11,s,l+1);
       //dlposition[11+l] = 0;
       //dlposition += 12 + l;
       //dlcount += 12 + l;
    }
  return 1;
}

int DisplayList::end(void)
{ if (dlcount + 1 > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  *dlposition++ = DL_end;
  dlcount++;
  if (errorstatus) return 0;
  return 1;
}

int DisplayList::addpolygon3d(Point origin,Point xaxis,Point yaxis,int nloops,int *nvert,Point **verts,int filled,RCCOLORREF colour)
{int i,tnverts,size;

  size = 1 + 4 + nloops * 4 + 80;
  for (i = 0 ; i < nloops ; i++)
    size += 24 * nvert[i];

  if (countmode)
    {  dlcount += size;
       return 1;
    }

  if (dlcount + size > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  *dlposition = DL_polygon3d;

  *((Point *)(dlposition+1)) = origin;
  *((Point *)(dlposition+1+24)) = xaxis;
  *((Point *)(dlposition+1+48)) = yaxis;
  *((int *)(dlposition+1+72)) = filled;
  *((RCCOLORREF *)(dlposition+1+76)) = colour;

  *((int *)(dlposition+1+80)) = nloops;


  for (i = 0 ; i < nloops ; i++)
    *((int *)(dlposition+1+80+4+i*4)) = nvert[i];

  for (i = 0,tnverts = 0 ; i < nloops ; tnverts += nvert[i],i++)
    memcpy(dlposition+1+80+4+nloops*4+tnverts*24,verts[i],24 * nvert[i]);

  dlposition += size;
  dlcount += size;

  return 1;
}

int DisplayList::changecolour(int colour)
{ if (countmode)
    {  dlcount += 2;  return 1;
    }
  if (dlcount + 2 > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  *dlposition = DL_colour;
  *((RCCHAR *)(dlposition+1)) = RCCHAR(colour);
  dlposition += 2;
  dlcount += 2;
  return 1;
}

int DisplayList::changestyle(float linestylescale,int style)
{
    if (countmode)
    {  dlcount += 6;  return 1;
    }
  if (dlcount + 6 > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  *dlposition = DL_style;
  *((RCCHAR *)(dlposition+1)) = RCCHAR(style);
  memcpy(dlposition+2,&linestylescale,4);
  dlposition += 6;
  dlcount += 6;
  return 1;
}

int DisplayList::changeweight(int weight)
{ if (countmode)
    {  dlcount += 2;  return 1;
    }
  if (dlcount + 2 > dllength)
    {  clear();  errorstatus = 5;  return 0;
    }
  *dlposition = DL_weight;
  *((RCCHAR *)(dlposition+1)) = RCCHAR(weight);
  dlposition += 2;
  dlcount += 2;
  return 1;
}

#ifdef USING_WIDGETS
int DisplayList::addCircleItem(QGraphicsItem *qgi)
{
  if (countmode)
  {
      dlcount += sizeof(qgi);  return 1;
  }
  if (dlcount + sizeof(qgi) > dllength)
  {
      clear();  errorstatus = 5;  return 0;
  }
  *dlposition = DL_circleitem;
  //*((QGraphicsItem *)(dlposition+1)) = qgi;
  memcpy(dlposition+1,qgi,sizeof(qgi));
  dlposition += sizeof(qgi)+1;
  dlcount += sizeof(qgi)+1;
  return 1;
}
#endif

//
//  The CAD 'MainWindow' class
//

int MainWindow::iconheight = 64;
int MainWindow::coordheight = 32;
int MainWindow::promptheight = 22;
int MainWindow::displayheight = 102;
double MainWindow::fontsize = 8.0;
RCHFONT MainWindow::hsfont = 0;

#ifdef DERIVE_FROM_WIDGETS
MainWindow::MainWindow( QWidget *parent ) : Window(parent)
{
    QWidget::setParent(parent);
}
#endif

MainWindow::MainWindow(int x,int y,int w,int h,int show /*, QWidget *parent = 0 */) :
#ifdef USING_WIDGETS
  Window(CHAR2RCCHR("MainWindow"),NULL,(DWORD)WS_OVERLAPPEDWINDOW,x,y,w,h,show),
#else
  Window(CHAR2RCCHR("CadWindow"),NULL,(DWORD)WS_OVERLAPPEDWINDOW,x,y,w,h,show),
#endif
  options(32)
{
 RECT rect;
 RCCHAR string[300],text[300];
 TEXTMETRIC tm;
 RCHFONT hfont;
 SIZE size;
 hbitmapold=0;
 int xm,ym,wm,hm,fm,promptx,prompty,statuspromptx,statusprompty,showcoordinatedisplay;
 int showstatusprompt,showcoordinateentry,showprompt,showstyleentry;
 int statuspromptf,promptf,styleentryf,mainmenuf,maininterruptf,mainapplicationf,quickf,coordinatedisplayf,coordinateentryf;

  if (gethwnd() == NULL) return;

  QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  qDebug() << "MainWindow type : " << ((QWidget*)hWnd)->windowType();

  draw = 0;
  batch = 0;

  maincmenu = subcmenu = mainimenu = subimenu = quickmenu = mainamenu = subamenu = lastmainamenu = lastsubamenu = NULL;

  strcpy(applicationname,"");
  /*
  hsfont = CreateFont(int(-GetDeviceCaps(gethdc(),LOGPIXELSY)*fontsize/72.0),0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,_RCT("MS Sans Serif"));
  hfont = (RCHFONT)SelectObject(gethdc(),hsfont);
  GetTextMetrics(gethdc(),&tm);
  GetTextExtentPoint32(gethdc(),_RCT("X 1.2345678901234567"),20,&size);
  SelectObject(gethdc(),hfont);
  */
  // get the default font hieght

  QFont defFont;
  QFontInfo defInfo(defFont);
  QFontMetrics defMt(defFont);
  int fontSize = defInfo.pixelSize();
  QRect qcoordsize = defMt.boundingRect("X 1.2345678901234567");
  size.cx = qcoordsize.width();//related to coordinate display (not entry) in status bar
  size.cy = qcoordsize.height();

  iconheight = 64;
  //coordheight = tm.tmHeight * 3 / 2 + 6;
  coordheight = fontSize * 3 / 2 + 6;
  //promptheight = tm.tmHeight + 6;
  promptheight = fontSize + 6;
  displayheight = iconheight + coordheight + promptheight + 8;
  dragviewboundary = 1;
  // add the central widget to MainWindow
#ifdef USING_WIDGETS
#ifdef USING_TABBED_Windows
  GetClientRect(gethwnd(),&rect);
  subwindow = new SubWindow(_RCT("MdiArea"),gethwnd(),WS_CHILD | WS_CLIPCHILDREN,0,0,rect.right,rect.bottom);
#else
  GetClientRect(gethwnd(),&rect);
  subwindow = new SubWindow(_RCT("MdiArea"),gethwnd(),WS_CHILD | WS_CLIPCHILDREN,0,0,rect.right,rect.bottom);
#endif
#else
  GetClientRect(gethwnd(),&rect);
  SetBkMode(gethdc(),TRANSPARENT);
  subwindow = new SubWindow(_RCT("CadWindow"),gethwnd(),WS_CHILD | WS_CLIPCHILDREN,0,displayheight,rect.right,rect.bottom-displayheight-1);
#endif
  printpreviewwindow = 0;

  // dock status flags for toolbars? (I think)
  // these tool bars are allways made visible
  // coordinatedisplay)
  // coordinateentry
  // prompt
  // statusprompt
  // styleentry
  // otherwise a toolbar is visible if present in the [Layout] section of the INI file
  // the last number in the ini file entry for a tool bar is the dock status
  // xxxxf = 0 = docked
  // xxxxf = 1 = floating
  statuspromptf = promptf = styleentryf = mainmenuf = maininterruptf = mainapplicationf = quickf = coordinatedisplayf = coordinateentryf = 0;
  showstatusprompt = showcoordinateentry = showprompt = showstyleentry = 0;

  styleentryx = 12;
  styleentryy = -25;
  coordinateentryx = 590;
  coordinateentryy = MenuBorderOffset;
  coordinatedisplayx = coordinateentryx + size.cx * 3 + 30;
  coordinatedisplayy = iconheight + 2;

  //  Initialize the menu positions
  maincmenux = MenuBorderOffset+MenuHandleOffset+2;  maincmenuy = MenuBorderOffset + 2;
  subcmenux = MenuBorderOffset+MenuHandleOffset+2;  subcmenuy = MenuBorderOffset + 27;

  mainimenux = 312;  mainimenuy = MenuBorderOffset + 2;
  subimenux = 312;  subimenuy = MenuBorderOffset + 27;

  mainamenux = 590;  mainamenuy = MenuBorderOffset + 2;
  subamenux = 590;  subamenuy = MenuBorderOffset + 27;

  quickmenux = 630;  quickmenuy = -26;

  promptx = 590;  prompty = MenuBorderOffset + 31;
  statuspromptx = 764;  statusprompty = -25;

  //  #define SW_NORMAL           1
  //  #define SW_SHOWNORMAL       1
  //  #define SW_SHOW             5
  //  #define SW_MAXIMIZE         3
  //  #define SW_HIDE             0
  //  #define SW_SHOWMINIMIZED    2
  //  #define SM_CXMAXIMIZED      61
  //  #define SM_CYMAXIMIZED      62
  if (show == SW_MAXIMIZE)  //  Use the full window size to check the menu position is OK
  {
      w = GetSystemMetrics(SM_CXMAXIMIZED);
      h = GetSystemMetrics(SM_CYMAXIMIZED);
  }

  // statuspromptf = 0 = docked
  // statuspromptf = 1 = floating
  statuspromptf = 1;
  if(settings.contains("Layout/StatusPrompt"))
      showstatusprompt = 1;
  /*
  GetPrivateProfileString(_RCT("Layout"),_RCT("StatusPrompt"),_RCT(""),text,300,home.getinifilename());
  if (sscanf(text,_RCT("%d,%d,%d,%d,%d"),&xm,&ym,&wm,&hm,&statuspromptf) == 5)
    {  statuspromptx = xm;  statusprompty = ym;
       if (statuspromptf == 0 &&
             (xm < 0 || xm + size.cx > w ||
              ym < 0 || ym + 24 > h))
         {  statuspromptx = 50;
            statusprompty = 50;
            statuspromptf = 1;
         }
    }
    */

  promptf = 1;
  if(settings.contains("Layout/Prompt"))
      showprompt = 1;

  /*
  GetPrivateProfileString(_RCT("Layout"),_RCT("Prompt"),_RCT(""),text,300,home.getinifilename());
  if (sscanf(text,_RCT("%d,%d,%d,%d,%d"),&xm,&ym,&wm,&hm,&promptf) == 5)
    {  promptx = xm;  prompty = ym;
       if (promptf == 0 &&
             (xm < 0 || xm + size.cx * 3.0 > w ||
              ym < 0 || ym + 24 > h))
         {  promptx = 50;
            prompty = 90;
            promptf = 1;
         }
    }
    */

  styleentryf = 1;
  if(settings.contains("Layout/StyleEntry"))
      showstyleentry = 1;
  /*
  GetPrivateProfileString(_RCT("Layout"),_RCT("StyleEntry"),_RCT(""),text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&styleentryf) == 5)
    {  styleentryx = xm;  styleentryy = ym;
       if (styleentryf == 0 &&
             (xm < 0 || xm + wm > w ||
              ym < 0 || ym + 24 > h))
         {  styleentryx = 50;
            styleentryy = 90;
            styleentryf = 1;
         }
    }
    */

  /*
  GetPrivateProfileString("Layout","MainMenu","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mainmenuf) == 5)
    {  maincmenux = xm;  maincmenuy = ym;
       subcmenux = xm;  subcmenuy = ym + 25;
       if (mainmenuf == 0 &&
             (xm < 0 || xm + wm*24 > w ||
              ym < 0 || ym + 48 > h))
         {  maincmenux = 50;  maincmenuy = 120;
            subcmenux = 50;  subcmenuy = 120 + 25;
            mainmenuf = 1;
            WritePrivateProfileString("Layout","MainMenu","50,120,0,0,1",home.getinifilename());
         }
    }
    */
  /*
  GetPrivateProfileString("Layout","MainInterrupt","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&maininterruptf) == 5)
    {  mainimenux = xm;  mainimenuy = ym;
       subimenux = xm;  subimenuy = ym + 25;
       if (maininterruptf == 0 &&
             (xm < 0 || xm + wm*24 > w ||
              ym < 0 || ym + 48 > h))
         {  mainimenux = 50;  mainimenuy = 170;
            subimenux = 50;  subimenuy = 170 + 25;
            maininterruptf = 1;
            WritePrivateProfileString("Layout","MainInterrupt","50,170,0,0,1",home.getinifilename());
         }
    }
    */
  /*
  GetPrivateProfileString("Layout","MainApplication","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&fm) == 5)
    {  mainamenux = xm;  mainamenuy = ym;
       subamenux = xm;  subamenuy = ym + 25;
       if (fm == 0 &&
             (xm < 0 || xm + wm*24 > w ||
              ym < 0 || ym + 48 > h))
         {  fm = 1;
         }
       if (fm != 0)
         mainamenux = mainamenuy = -10000;
    }
    */
  /*
  GetPrivateProfileString("Layout","Quick","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&quickf) == 5)
    {  quickmenux = xm;  quickmenuy = ym;
       if (quickf == 0 &&
             (xm < 0 || xm + wm*24 > w ||
              ym < 0 || ym + 24 > h))
         {  quickmenux = 50;
            quickmenuy = 250;
            quickf = 1;
            WritePrivateProfileString("Layout","Quick","50,250,0,0,1",home.getinifilename());
         }
    }
    */

  // there is a setting in the ini file if the coordinate display is shown
  coordinatedisplayf = 1;
  showcoordinatedisplay = 0;
  if(settings.contains("Layout/CoordinateDisplay"))
      showcoordinatedisplay = 1;
  /*
  GetPrivateProfileString("Layout","CoordinateDisplay","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&coordinatedisplayf) == 5)
    {  coordinatedisplayx = xm;  coordinatedisplayy = ym;
       showcoordinatedisplay = 1;
       if (coordinatedisplayf == 0 &&
             (xm < 0 || xm + wm > w ||
              ym < 0 || ym + 24 > h))
         {  coordinatedisplayx = 50;
            coordinatedisplayy = 300;
            coordinatedisplayf = 1;
         }
    }
   */


  coordinateentryf = 1;
  showcoordinateentry = 0;
  if(settings.contains("Layout/CoordinateEntry"))
      showcoordinateentry = 1;
  /*
  GetPrivateProfileString("Layout","CoordinateEntry","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&coordinateentryf) == 5)
    {  coordinateentryx = xm;  coordinateentryy = ym;
       if (coordinateentryf == 0 &&
             (xm < 0 || xm + wm > w ||
              ym < 0 || ym + 24> h))
         {  coordinateentryx = 50;
            coordinateentryy = 350;
            coordinateentryf = 1;
         }
    }
    */

    QToolBar * promptToolbar = new QToolBar("PromptToolBar",(QtMainWindow*)hWnd);
    promptToolbar->setObjectName("PromptToolBar");
    promptToolbar->setWindowTitle(app->tr("Prompt"));
    promptToolbar->setMinimumHeight(coordheight+6);
    promptToolbar->setMaximumHeight(coordheight+6);
    promptToolbar->setMinimumWidth(500);
    //promptToolbar->setMaximumWidth(500);
    promptToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    promptToolbar->setFloatable(true);
    promptToolbar->setMovable(true);

    prompt = new Prompt(12,iconheight + coordheight + 8,440,promptToolbar);

    ((QLabel*)prompt->getQW())->setObjectName("PromptLabel");
    ((QLabel*)prompt->getQW())->setMinimumHeight(coordheight-6);
    ((QLabel*)prompt->getQW())->setMaximumHeight(coordheight-6);
    ((QLabel*)prompt->getQW())->setMinimumWidth(440);
    ((QLabel*)prompt->getQW())->setMaximumWidth(440);
    ((QLabel*)prompt->getQW())->setText("Prompt");
    ((QLabel*)prompt->getQW())->setTextFormat(Qt::AutoText);
    ((QLabel*)prompt->getQW())->setAutoFillBackground(true);
    ((QLabel*)prompt->getQW())->setBackgroundRole(QPalette::Background);
    QPalette PromptPalette = ((QLabel*)prompt->getQW())->palette();
    PromptPalette.setColor(QPalette::Window,QColor(255,255,255));
    PromptPalette.setColor(QPalette::WindowText,QColor(0,0,0));
    ((QLabel*)prompt->getQW())->setPalette(PromptPalette);

    promptToolbar->setMinimumWidth(((QLabel*)prompt->getQW())->width()+20);
    //promptToolbar->setMaximumWidth(((QLabel*)prompt->getQW())->width()+20);

    promptToolbar->addWidget((QLabel*)prompt->getQW());

    if(showprompt)
    {
        ((QtMainWindow*)hWnd)->addToolBar(Qt::TopToolBarArea,promptToolbar);
        promptToolbar->setParent((QtMainWindow*)hWnd);

        prompt->fixup(this);
        prompt->normalprompt(NCWIN+1);
        prompt->setcolour(RGB(0,255,0));
        //prompt->setbackgroundcolour(RGB(255,255,255));
        //prompt->move(this,promptx,prompty);

        addcontrol(prompt);
        QByteArray state = settings.value("Layout/Prompt","").toByteArray();
        prompt->getQW()->restoreGeometry(state);
        promptToolbar->show();
    }
    else
        promptToolbar->hide();

    // connect slot to handle this toolbars visibily changes
    promptToolbar->connect(promptToolbar,SIGNAL(visibilityChanged(bool)),(QtMainWindow*)hWnd,SLOT(onToolBarVisibilyChanged(bool)));

    // create a single prompt control with a default toolbar parent
    QToolBar * statusToolbar = new QToolBar("StatusToolBar",(QtMainWindow*)hWnd);
    statusToolbar->setObjectName("StatusToolBar");
    statusToolbar->setWindowTitle(app->tr("Status"));
    statusToolbar->setMinimumHeight(coordheight+6);
    statusToolbar->setMaximumHeight(coordheight+6);
    statusToolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    statusToolbar->setFloatable(true);
    statusToolbar->setMovable(true);

    statusprompt = new Prompt(12 + size.cx * 4 + 20,iconheight + coordheight + 8,size.cx,statusToolbar);
    ((QLabel*)statusprompt->getQW())->setObjectName("StatusLabel");
    ((QLabel*)statusprompt->getQW())->setMinimumHeight(coordheight-6);
    ((QLabel*)statusprompt->getQW())->setMaximumHeight(coordheight-6);
    ((QLabel*)statusprompt->getQW())->setMinimumWidth(size.cx);
    ((QLabel*)statusprompt->getQW())->setMaximumWidth(220);
    ((QLabel*)statusprompt->getQW())->setText("Status");
    ((QLabel*)statusprompt->getQW())->setTextFormat(Qt::AutoText);
    ((QLabel*)statusprompt->getQW())->setAutoFillBackground(true);
    ((QLabel*)statusprompt->getQW())->setBackgroundRole(QPalette::Background);
    QPalette StatusPalette = ((QLabel*)statusprompt->getQW())->palette();
    StatusPalette.setColor(QPalette::Window,QColor(255,255,255));
    StatusPalette.setColor(QPalette::WindowText,QColor(0,0,0));
    ((QLabel*)statusprompt->getQW())->setPalette(StatusPalette);

    statusToolbar->setMinimumWidth(((QLabel*)statusprompt->getQW())->width()+20);
    //statusToolbar->setMaximumWidth(((QLabel*)statusprompt->getQW())->width()+20);

    statusToolbar->addWidget((QLabel*)statusprompt->getQW());
    ((QtMainWindow*)hWnd)->addToolBar(Qt::TopToolBarArea,statusToolbar);
    //statusToolbar->setParent((QtMainWindow*)hWnd);

    if(showstatusprompt)
    {
        statusprompt->fixup(this);
        statusprompt->normalprompt(_RCT(""));
        statusprompt->setcolour(RGB(255,0,0));
        //statusprompt->setbackgroundcolour(RGB(255,255,255));
        //statusprompt->move(this,statuspromptx,statusprompty);
        addcontrol(statusprompt);
        QByteArray state = settings.value("Layout/StatusPrompt","").toByteArray();
        statusprompt->getQW()->restoreGeometry(state);
        statusToolbar->show();
    }
    else
        statusToolbar->hide();

    // connect slot to handle this toolbars visibily changes
    promptToolbar->connect(statusToolbar,SIGNAL(visibilityChanged(bool)),(QtMainWindow*)hWnd,SLOT(onToolBarVisibilyChanged(bool)));

    floatingmenu1 = floatingmenu2 = floatingmenu3 = floatingmenu4 = NULL;

    // KMJ: these things are added to the toolbar area in the order of creation below
    // if you want a different order change the code below
    coordinateentry=0;
    coordinateentry = new CoordinateEntry(this,coordinateentryx,coordinateentryy,size.cx * 3);
    coordinateentry->setcsystem(0);
    ((QToolBar*)coordinateentry->getQW())->setWindowTitle(app->tr("Coordinate Entry"));
    ((QToolBar*)coordinateentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    ((QToolBar*)coordinateentry->getQW())->setFloatable(true);
    ((QToolBar*)coordinateentry->getQW())->setMovable(true);
    ((QToolBar*)coordinateentry->getQW())->setMinimumHeight(coordheight+6);
    ((QToolBar*)coordinateentry->getQW())->setMaximumHeight(coordheight+6);
    ((QtMainWindow*)hWnd)->addToolBar(Qt::TopToolBarArea,(QToolBar*)coordinateentry->getQW());
    //coordinateentry->getQW()->setParent((QtMainWindow*)hWnd);

    if(showcoordinateentry)
    {
        addcontrol(coordinateentry);
        QByteArray state = settings.value("Layout/CoordinateEntry","").toByteArray();
        coordinateentry->getQW()->restoreGeometry(state);
        coordinateentry->getQW()->show();
    }
    else
        coordinateentry->getQW()->hide();

    // connect slot to handle this toolbars visibily changes
    coordinateentry->getQW()->connect(coordinateentry->getQW(),SIGNAL(visibilityChanged(bool)),(QtMainWindow*)hWnd,SLOT(onToolBarVisibilyChanged(bool)));

    coordinatedisplay=0;
    coordinatedisplay = new CoordinateDisplay(this,coordinatedisplayx,coordinatedisplayy,size.cx * 3);
    ((QToolBar*)coordinatedisplay->getQW())->setWindowTitle(app->tr("Coordinate Display"));
    ((QToolBar*)coordinatedisplay->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    ((QToolBar*)coordinatedisplay->getQW())->setFloatable(true);
    ((QToolBar*)coordinatedisplay->getQW())->setMovable(true);
    ((QToolBar*)coordinatedisplay->getQW())->setMinimumHeight(coordheight+6);
    ((QToolBar*)coordinatedisplay->getQW())->setMaximumHeight(coordheight+6);
    ((QtMainWindow*)hWnd)->addToolBar(Qt::TopToolBarArea,(QToolBar*)coordinatedisplay->getQW());
    //((QToolBar*)coordinatedisplay->getQW())->setParent((QtMainWindow*)hWnd);

    if (showcoordinatedisplay)
    {
        addcontrol(coordinatedisplay);
        QByteArray state = settings.value("Layout/CoordinateDisplay","").toByteArray();
        ((QToolBar*)coordinatedisplay->getQW())->restoreGeometry(state);
        ((QToolBar*)coordinatedisplay->getQW())->show();
    }
    else
        ((QToolBar*)coordinatedisplay->getQW())->hide();

    // connect slot to handle this toolbars visibily changes
    coordinatedisplay->getQW()->connect(coordinatedisplay->getQW(),SIGNAL(visibilityChanged(bool)),(QtMainWindow*)hWnd,SLOT(onToolBarVisibilyChanged(bool)));

    styleentry=0;
    styleentry = new StyleEntry(this,styleentryx,styleentryy,605);
    ((QToolBar*)styleentry->getQW())->setWindowTitle(app->tr("Style Bar"));
    ((QToolBar*)styleentry->getQW())->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    ((QToolBar*)styleentry->getQW())->setFloatable(true);
    ((QToolBar*)styleentry->getQW())->setMovable(true);
    //((QToolBar*)styleentry->getQW())->setMinimumHeight(coordheight+6);
    //((QToolBar*)styleentry->getQW())->setMaximumHeight(coordheight+6);

    if(showstyleentry)
    {
        ((QtMainWindow*)hWnd)->addToolBar(Qt::BottomToolBarArea,(QToolBar*)styleentry->getQW());
        ((QToolBar*)styleentry->getQW())->setParent((QtMainWindow*)hWnd);
        addcontrol(styleentry);

        QByteArray state = settings.value("Layout/StyleEntry","").toByteArray();
        ((QToolBar*)styleentry->getQW())->restoreGeometry(state);
        ((QToolBar*)styleentry->getQW())->show();

    }
    else
        ((QToolBar*)styleentry->getQW())->hide();

    // connect slot to handle this toolbars visibily changes
    styleentry->getQW()->connect(styleentry->getQW(),SIGNAL(visibilityChanged(bool)),(QtMainWindow*)hWnd,SLOT(onToolBarVisibilyChanged(bool)));

  viewtab=0;
  viewproperties=0;
  //viewtab = new ViewTab(this,16);
  //viewproperties = new ViewProperties(this,14);
  //addcontrol(viewtab);
  //addcontrol(viewproperties);

  // KMJ: using a Qt timer
  qtimer = new QTimer((QtMainWindow*)gethwnd());
  //((QtMainWindow*)gethwnd())->connect(qtimer,SIGNAL(timeout()),(QtMainWindow*)gethwnd(),SLOT(timerFunc()));
  qtimer->start(10);
  timer = (UINT)qtimer->timerId();

  //timer = SetTimer(gethwnd(),42,10,NULL);

  curview3d = NULL;
#if VERSION != TRICAD_STUDENT
  curcadbasic = NULL;
  curcommand = NULL;
#endif

  commandwindow = 0;
  layerwindow = 0;
  // set the main window title string
  ResourceString nc2(NCWIN+2);
  ResourceString ncname(NCWIN+21+(cad_version&15));
  sprintf(string,nc2.gets(),ncname.gets(),db.grid.getdisplay() ? _RCT("On") : _RCT("Off"),db.grid.getsnap() ? _RCT("On") : _RCT("Off"));
  title(string);

  cadwindow = this;

  // KMJ: offscreen bitmap not currently used with Qt
  // initialise these to null
  //  Allocate an offscreen bitmap to use when drawing inverse text.
  hoffscreenhdc = 0;
  hbitmapold = 0;
  /*
  RCHBITMAP hbitmap;
  hoffscreenhdc = CreateCompatibleDC(GetDC(0));
  hbitmap = CreateCompatibleBitmap(GetDC(0),GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
  hbitmapold = (RCHBITMAP) SelectObject(hoffscreenhdc,hbitmap);
 */
  //  Allocate an offscreen view for drawing in
  qDebug() << "MainWindow hWnd QWidget is a : " << (QWidget*)hWnd;
  View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
  View3dOutput output(hWnd,0,&top,1.0);
  offscreenbitmapused = 0;
  offscreenbitmapwidth = GetSystemMetrics(SM_CXSCREEN);
  offscreenbitmapheight = GetSystemMetrics(SM_CYSCREEN);
  offscreenbitmap = new View3dOffScreenBitmap(&output,offscreenbitmapwidth,offscreenbitmapheight);

  sizeevent(-1,-1);

  /*
#if ! defined(_WIN32_WCE)
  SetClipboardViewer(gethwnd());

  DragAcceptFiles(gethwnd(),TRUE);
#endif
  */
}


View3dOffScreenBitmap *MainWindow::GetOffScreenBitmap(View3dSurface *vs)
{
  if ( Surface::width < offscreenbitmapwidth && Surface::height < offscreenbitmapheight )
  {
      offscreenbitmap->ChangeView(vs);
      offscreenbitmapused = 1;
      return offscreenbitmap;
  }
  else
      return new View3dOffScreenBitmap(vs,(int)vs->getwidth(),(int)vs->getheight());
}

static Entity *default_entity = 0;

#ifdef _USING_QTCHAR
extern int RCentity_defaults(int action,int id,void *data,RCCHAR *text);
#endif
int entity_defaults(int action,int id,void *data,void *text)
{
    return RCentity_defaults(action,id, data, (RCCHAR*)text);
}

#ifdef _USING_QTCHAR

/*
int entity_defaults(int action,int id,void *data,void *text)
{
    return entity_defaults(action,id, data, (RCCHAR*)text);
}
*/
int RCentity_defaults(int action,int id,void *data,RCCHAR *text)
{int i,cstate,result;
#ifdef _USING_QTCHAR
RCSTRING RCentity_name_l[] = {  " ", "Plane", "Ruled Surface", "Surface of Revolution", "Patch", "Text", "Figure", "Bitmap", "Dimension", "Leader", "Wall", ""/*RCCHAR().Null*/  };
 RCCHAR e1[30],e2[30],e3[30],e4[30],e5[30],e6[30],e7[30],e8[30],e9[30],e10[30],e11[30],e12[30];
 RCCHAR *entity_name_l[12] = { e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12 };
 for( int ic=0;ic<12;ic++)
     RCSTRCPY(entity_name_l[ic],&RCentity_name_l[ic]);
 //printQChar(entity_name_l[1]);
#else
 RCCHAR *entity_name_l[] = {  " ", "Plane", "Ruled Surface", "Surface of Revolution", "Patch", "Text", "Figure", "Bitmap", "Dimension", "Leader", "Wall", 0  };
#endif

 static int default_entity_index = 0;

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Entity");
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            colour_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Colour");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getcolour());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            layer_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Layer");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getlayer());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            style_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Style");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getstyle());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            weight_command(&cstate,0,0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Weight");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(db.header.getweight()));

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Default");
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,_RCT(""));
            for (i = 0 ; *entity_name_l[i] != 0 ; i++)
            {   if (strlen(text) > 0)
                    strcat(text,_RCT("\t"));
                strcat(text,entity_name_l[i]);
            }
            return 2;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Entity");
        else if (action == PP_GETVALUE)
            strcpy(text,entity_name_l[default_entity_index]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; entity_name_l[i] != 0 ; i++)
                if (strcmp(text,entity_name_l[i]) == 0)
                {   delete default_entity;
                    default_entity_index = i;
                    if (default_entity_index == 1)
                    {EntityList list;
                     double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
                     BitMask options(32);
                     RCCHAR patternfilename[300];
                     RCCOLORREF gradientcolour;
                        Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                        default_entity = new Plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                    }
                    else if (default_entity_index == 2)
                    {Line *l1 = new Line;
                     Line *l2 = new Line;
                     int xmesh,ymesh;
                     BitMask options(32);
                        RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                        default_entity = new RuledSurface(l1,l2,xmesh,ymesh,options);
                    }
                    else if (default_entity_index == 3)
                    {Line *l = new Line;
                     int xmesh,ymesh;
                     double angle,deltaz;
                     BitMask options(32);
                        SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);
                        default_entity = new SurfaceOfRev(Point(1.0,0.0,0.0),Point(1.0,0.0,0.0),angle,deltaz,l,xmesh,ymesh,options);
                    }
                    else if (default_entity_index == 4)
                    {int udegree,vdegree,xmesh,ymesh;
                     BitMask options(32);
                     Point p[400];
                     EntityList tcurves;
                        for (i = 0 ; i < 400 ; i++)
                            p[i].setxyz(0.0,0.0,0.0);
                        Patch::getdefaults(&udegree,&vdegree,&xmesh,&ymesh,&options);
                        default_entity = new Patch(20,20,udegree,vdegree,xmesh,ymesh,p,0,0,0,tcurves,options);
                    }
                    else if (default_entity_index == 5)
                    {RCCHAR *f,*t;
                     double height,width,slant,angle;
                     BitMask options(32);
                     Byte halocolour;
                        Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
                        default_entity = new Text(f,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),width,height,slant,angle,halocolour,(RCCHAR*)"",(RCCHAR*)"",options);
                    }
                    else if (default_entity_index == 6)
                    {Point scale,rotation;
                     BitMask options(32);
                     Byte state1,state2;
                        Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
                        default_entity = new Figure(_RCT(""),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);
                    }
                    else if (default_entity_index == 7)
                    {double width,height,tr,tg,tb,trange,op;
                     BitMask options(32);
                        BitMapE::getdefaults(&width,&height,&tr,&tg,&tb,&trange,&op,&options);
                        default_entity = new BitMapE(_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),width,height,tr,tg,tb,trange,op,options);
                    }
                    else if (default_entity_index == 8)
                    {double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
                     int precision,tolprecision;
                     TerminatorInfo term1,term2;
                     TextInfo vinfo,tinfo,pinfo,sinfo;
                     SortedPointList l;
                     BitMask options(32);
                        LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                        default_entity = new LinearDimension(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                    }
                    else if (default_entity_index == 9)
                    {double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
                     int precision,tolprecision;
                     TerminatorInfo term1,term2;
                     TextInfo vinfo,tinfo,pinfo,sinfo;
                     SortedPointList l;
                     BitMask options(32);
                     BitMask style(32);
                        style.set(2);
                        textyoffset = 0.0;
                        Dimension::getleaderdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                        default_entity = new CircularDimension(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(10.0,10.0,0.0),Point(20.0,10.0,0.0),style,0,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                    }
                    else if (default_entity_index == 10)
                    {double width,height,ea,p,pd,ph,lhs,rhs,lha,rha,attributes[ArMaxWallAttributes];
                     int mc,rc;
                     short nattributes;
                     BitMask options1(32),options2(32);
                     RCCHAR name[300],strwm[300];
                        Wall::getdefaults(true,name,strwm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,&nattributes,attributes,&options1,&options2);
                        default_entity = new Wall(Point(0.0,0.0,0.0),Point(100.0,0.0,0.0),Point(0.0,0.0,1.0),name,strwm,width,height,ea,p,pd,ph,mc,rc,lhs,rhs,lha,rha,0.0,0.0,nattributes,attributes,options1,options2,false);
                    }
                    else
                        default_entity = 0;
                    return 1;
                }
        }
    }

    if (default_entity != 0)
    {   if (action == PP_GETNUMPROPERTIES)
            id -= default_entity->process_property(PP_GETNUMPROPERTIES,0,0,(RCCHAR*)0) - 5;
        else if (id >= 0)
        {
            result = default_entity->process_property(action,id+5,data,text);  //  + 5 to skip layer, colour style and weight
            if (action == PP_SETVALUE)
            {   default_entity->setdefaults();
            }
            return result;
        }
    }

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;

    return 0;
}
#endif

int entity_defaults(int action,int id,void *data,RCCHAR *text)
{int i,cstate,result;
 RCCHAR *entity_name_l[] = {  _RCT(" "), _RCT("Plane"), _RCT("Ruled Surface"),_RCT( "Surface of Revolution"), _RCT("Patch"), _RCT("Text"), _RCT("Figure"), _RCT("Bitmap"), _RCT("Dimension"), _RCT("Leader"), _RCT("Wall"), 0  };
 static int default_entity_index = 0;

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Entity");
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            colour_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Colour");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getcolour());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            layer_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Layer");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getlayer());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            style_command(&cstate,0,(void **)0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Style");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",db.header.getstyle());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   cstate = InitialState;
            weight_command(&cstate,0,0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Weight");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(db.header.getweight()));

    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Default");
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"");
            for (i = 0 ; entity_name_l[i] != 0 ; i++)
            {   if (strlen(text) > 0)
                    strcat(text,"\t");
                strcat(text,entity_name_l[i]);
            }
            return 2;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Entity");
        else if (action == PP_GETVALUE)
            strcpy(text,entity_name_l[default_entity_index]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; entity_name_l[i] != 0 ; i++)
                if (strcmp(text,entity_name_l[i]) == 0)
                {   delete default_entity;
                    default_entity_index = i;
                    if (default_entity_index == 1)
                    {EntityList list;
                     double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
                     BitMask options(32);
                     RCCHAR patternfilename[300];
                     RCCOLORREF gradientcolour;
                        Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                        default_entity = new Plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                    }
                    else if (default_entity_index == 2)
                    {Line *l1 = new Line;
                     Line *l2 = new Line;
                     int xmesh,ymesh;
                     BitMask options(32);
                        RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                        default_entity = new RuledSurface(l1,l2,xmesh,ymesh,options);
                    }
                    else if (default_entity_index == 3)
                    {Line *l = new Line;
                     int xmesh,ymesh;
                     double angle,deltaz;
                     BitMask options(32);
                        SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);
                        default_entity = new SurfaceOfRev(Point(1.0,0.0,0.0),Point(1.0,0.0,0.0),angle,deltaz,l,xmesh,ymesh,options);
                    }
                    else if (default_entity_index == 4)
                    {int udegree,vdegree,xmesh,ymesh;
                     BitMask options(32);
                     Point p[400];
                     EntityList tcurves;
                        for (i = 0 ; i < 400 ; i++)
                            p[i].setxyz(0.0,0.0,0.0);
                        Patch::getdefaults(&udegree,&vdegree,&xmesh,&ymesh,&options);
                        default_entity = new Patch(20,20,udegree,vdegree,xmesh,ymesh,p,0,0,0,tcurves,options);
                    }
                    else if (default_entity_index == 5)
                    {RCCHAR *f,*t;
                     double height,width,slant,angle;
                     BitMask options(32);
                     Byte halocolour;
                        Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
                        default_entity = new Text(f,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),width,height,slant,angle,halocolour,(RCCHAR*)"",(RCCHAR*)"",options);
                    }
                    else if (default_entity_index == 6)
                    {Point scale,rotation;
                     BitMask options(32);
                     Byte state1,state2;
                        Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
                        default_entity = new Figure(_RCT(""),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);
                    }
                    else if (default_entity_index == 7)
                    {double width,height,tr,tg,tb,trange,op;
                     BitMask options(32);
                        BitMapE::getdefaults(&width,&height,&tr,&tg,&tb,&trange,&op,&options);
                        default_entity = new BitMapE(_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),width,height,tr,tg,tb,trange,op,options);
                    }
                    else if (default_entity_index == 8)
                    {double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
                     int precision,tolprecision;
                     TerminatorInfo term1,term2;
                     TextInfo vinfo,tinfo,pinfo,sinfo;
                     SortedPointList l;
                     BitMask options(32);
                        LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                        default_entity = new LinearDimension(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                    }
                    else if (default_entity_index == 9)
                    {double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
                     int precision,tolprecision;
                     TerminatorInfo term1,term2;
                     TextInfo vinfo,tinfo,pinfo,sinfo;
                     SortedPointList l;
                     BitMask options(32);
                     BitMask style(32);
                        style.set(2);
                        textyoffset = 0.0;
                        Dimension::getleaderdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                        default_entity = new CircularDimension(Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(10.0,10.0,0.0),Point(20.0,10.0,0.0),style,0,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                    }
                    else if (default_entity_index == 10)
                    {double width,height,ea,p,pd,ph,lhs,rhs,lha,rha,attributes[ArMaxWallAttributes];
                     int mc,rc;
                     short nattributes;
                     BitMask options1(32),options2(32);
                     RCCHAR name[300],strwm[300];
                        Wall::getdefaults(true,name,strwm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,&nattributes,attributes,&options1,&options2);
                        default_entity = new Wall(Point(0.0,0.0,0.0),Point(100.0,0.0,0.0),Point(0.0,0.0,1.0),name,strwm,width,height,ea,p,pd,ph,mc,rc,lhs,rhs,lha,rha,0.0,0.0,nattributes,attributes,options1,options2,false);
                    }
                    else
                        default_entity = 0;
                    return 1;
                }
        }
    }

    if (default_entity != 0)
    {   if (action == PP_GETNUMPROPERTIES)
            id -= default_entity->process_property(PP_GETNUMPROPERTIES,0,0,(RCCHAR*)0) - 5;
        else if (id >= 0)
        {
            result = default_entity->process_property(action,id+5,data,text);  //  + 5 to skip layer, colour style and weight
            if (action == PP_SETVALUE)
            {   default_entity->setdefaults();
            }
            return result;
        }
    }

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;

    return 0;
}

#ifdef _USING_QTCHAR
int standard_entity_properties(int action,int id,void *data,void *text)
{
    //if( ((Entity *)data)->type() == text_entity )
        return ((Entity *)data)->process_property(action,id,data,(RCCHAR*)text);
    //else
    //	return ((Entity *)data)->process_property(action,id,data,(RCCHAR*)text);
}
#else
int standard_entity_properties(int action,int id,void *data,RCCHAR *text)
{
    return ((Entity *)data)->process_property(action,id,data,text);
}
#endif

#ifdef _USING_QTCHAR
int entity_properties(int action,int id,void *data,void *text)
{   return db.Properties((Entity *)data,action,id,data,(RCCHAR*)text);
}
#else
int entity_properties(int action,int id,void *data,RCCHAR *text)
{   return db.Properties((Entity *)data,action,id,data,text);
}
#endif

#ifdef _USING_QTCHAR
typedef int (*ProcessProperty)(int action,int id,void *data,void *text);
#else
typedef int (*ProcessProperty)(int action,int id,void *data,RCCHAR *text);
#endif

void MainWindow::update_properties(int default_group_state)
{
#if 0 // not using yet
 EntityList list;
 Entity *e;
 RCCHAR text[300];
 RCCHAR rctext[300];
 int i,j,n_properties,standard_properties,result;

     if(viewproperties == 0)
         return;

    if (1 || state.getcommandid() == 0 || state.getcommandid() >= 3000 && state.getcommandid() <= 3004)
    {   list = state.getselection();
        if (list.length() == 0 || list.length() > 1)
        {
            viewproperties->set_last_entity(default_entity);
            if (viewproperties->get_entity_changed())
            {   default_group_state = -1;
                viewproperties->clear_entity_changed();
            }
            else
                default_group_state = viewproperties->get_current_group_states();
            viewproperties->clear_properties();
            for (i = 0,j = 0 ; i < entity_defaults(PP_GETNUMPROPERTIES,0,0,(RCCHAR*)0) ; i++)
            {   result = entity_defaults(PP_GETLABEL,i,0,text);
                if (result == 1)
                {   viewproperties->add_property(text,default_group_state == -1 ? 0 : (default_group_state >> j) & 1);
                    j++;
                }
                else if (result == 2)
                {   viewproperties->add_property(text,default_group_state == -1 ? 1 : (default_group_state >> j) & 1);
                    j++;
                }
                else
                    viewproperties->add_property(text,i,entity_defaults,(void*)0);
            }

            viewproperties->draw(DM_NORMAL);
        }
        else
        {   list.start();
            e = list.next();
            viewproperties->set_last_entity(e);
            if (viewproperties->get_entity_changed())
            {   default_group_state = -1;
                viewproperties->clear_entity_changed();
            }
            else
                default_group_state = viewproperties->get_current_group_states();
            viewproperties->clear_properties();

            n_properties = db.Properties(e,PP_GETNUMPROPERTIES,0,0,0);
            standard_properties = 0;
            if (n_properties == INT_MAX)
            {
#ifdef _USING_QTCHAR
                n_properties = e->process_property(PP_GETNUMPROPERTIES,0,0,(void*)0);
                //if( e->type() == text_entity )
                //	n_properties = e->process_property(PP_GETNUMPROPERTIES,0,0,(RCCHAR*)0);
                //else
                //	n_properties = e->process_property(PP_GETNUMPROPERTIES,0,0,(RCCHAR*)0);
#else
                n_properties = e->process_property(PP_GETNUMPROPERTIES,0,0,0);
#endif
                standard_properties = 1;
            }
            for (i = 0,j = 0 ; i < n_properties ; i++)
            {
                if (standard_properties)
#ifdef _USING_QTCHAR
                    //if( e->type() == text_entity )
                        result = e->process_property(PP_GETLABEL,i,0,text);
                    //else
                    //	result = e->process_property(PP_GETLABEL,i,0,text);
#else
                        result = e->process_property(PP_GETLABEL,i,0,text);
#endif
                else
                    result = db.Properties(e,PP_GETLABEL,i,0,text);
                if (result == 1)
                {   viewproperties->add_property(text,default_group_state == -1 ? 0 : (default_group_state >> j) & 1);
                    j++;
                }
                else if (result == 2)
                {   viewproperties->add_property(text,default_group_state == -1 ? 1 : (default_group_state >> j) & 1);
                    j++;
                }
                else
                    viewproperties->add_property(text,i,standard_properties ? standard_entity_properties : entity_properties,e);

            }

            viewproperties->draw(DM_NORMAL);

        }

    }
    else
    {
    }
#endif
}

void MainWindow::ReleaseOffScreenBitmap(View3dOffScreenBitmap *osb)
{ if (offscreenbitmap != osb)
    delete osb;
  else
    offscreenbitmapused = 0;
}

void MainWindow::seticon(RCHICON hicon,RCCHAR *appname)
{
#ifdef USING_WIDGETS
    //((QMainWindow*)gethwnd())->setWindowIcon((const QIcon&)*((QIcon*)hicon));
    ((QMainWindow*)gethwnd())->setWindowIcon(hicon);
    strcpy(applicationname,appname);
    updatetitle();
#else
  SetClassLong(cadwindow->gethwnd(), GCL_HICON,(LONG)hicon);
  strcpy(applicationname,appname);
  updatetitle();
#endif
}

void MainWindow::movemenu(ButtonMenu *bm,int x,int y)
{
  if (bm == maincmenu && subcmenu != 0)
    {  maincmenux = x;  maincmenuy = y;
       subcmenux = x;   subcmenuy = y + 25;
       subcmenu->move(maincmenu->parent,x,y+25,-1);
    }
  else if (bm == subcmenu && maincmenu != 0)
    {  maincmenux = x;  maincmenuy = y - 25;
       subcmenux = x;   subcmenuy = y;
       maincmenu->move(subcmenu->parent,x,y-25,-1);
    }
  else if (bm == mainimenu && subimenu != 0)
    {  mainimenux = x;  mainimenuy = y;
       subimenux = x;  subimenuy = y + 25;
       subimenu->move(mainimenu->parent,x,y+25,-1);
    }
  else if (bm == subimenu && mainimenu != 0)
    {  mainimenux = x;  mainimenuy = y - 25;
       subimenux = x;  subimenuy = y;
       mainimenu->move(subimenu->parent,x,y-25,-1);
    }
  else if (bm == mainamenu && subamenu != 0)
    {  mainamenux = x;  mainamenuy = y;
       subamenux = x;  subamenuy = y + 25;
       subamenu->move(mainamenu->parent,x,y+25,-1);
    }
  else if (bm == subamenu && mainamenu != 0)
    {  mainamenux = x;  mainamenuy = y - 25;
       subamenux = x;  subamenuy = y;
       mainamenu->move(subamenu->parent,x,y-25,-1);
    }
  else if (bm == quickmenu)
    {  quickmenux = x;  quickmenuy = y;
    }

}

void MainWindow::movemenu(int menu)
{int menubar1,menubar2;
 menubar1 = !options.test(0) && !options.test(1) ||
            !options.test(2) && !options.test(3) ||
            !options.test(4) && !options.test(5);
 switch (menu)
    {  case BM_MAINCOMMAND :
         if (options.test(0))
           {  options.clear(0);  options.set(1);
              DestroyWindow(floatingmenu1->gethwnd());
              cadwindow->deletefloatingmenu(floatingmenu1);
           }
         else if (options.test(1))
           {  options.clear(0);  options.clear(1);
           }
         else
           {  options.set(0);  options.clear(1);
              if (maincmenu != NULL)  delcontrol(maincmenu);
              if (subcmenu != NULL)  delcontrol(subcmenu);
              floatingmenu1 = new MenuWindow(subwindow,int(getwidth())-60,0,48,264);
           }
         break;
       case BM_MAININTERRUPT :
         if (options.test(2))
           {  options.clear(2);  options.set(3);
              DestroyWindow(floatingmenu2->gethwnd());
              cadwindow->deletefloatingmenu(floatingmenu2);
           }
         else if (options.test(3))
           {  options.clear(2);  options.clear(3);
           }
         else
           {  options.set(2);  options.clear(3);
              if (mainimenu != NULL) delcontrol(mainimenu);
              if (subimenu != NULL) delcontrol(subimenu);
              floatingmenu2 = new MenuWindow(subwindow,int(getwidth())-120,0,48,192);
           }
         break;
       case BM_MAINAPPLICATION :
         if (options.test(6))
           {  options.clear(6);  options.set(7);
              DestroyWindow(floatingmenu4->gethwnd());
              cadwindow->deletefloatingmenu(floatingmenu4);
           }
         else if (options.test(7))
           {  options.clear(6);  options.clear(7);
           }
         else
           {  options.set(6);  options.clear(7);
              if (mainamenu != NULL) delcontrol(mainamenu);
              if (subamenu != NULL) delcontrol(subamenu);
              floatingmenu4 = new MenuWindow(subwindow,int(getwidth())-240,0,48,192);
           }
         break;
       case BM_QUICK :
         if (options.test(4))
           {  options.clear(4);  options.set(5);
              DestroyWindow(floatingmenu3->gethwnd());
              cadwindow->deletefloatingmenu(floatingmenu3);
           }
         else if (options.test(5))
           {  options.clear(4);  options.clear(5);
           }
         else
           {  options.set(4);  options.clear(5);
              if (quickmenu != NULL)  delcontrol(quickmenu);
              floatingmenu3 = new MenuWindow(subwindow,int(getwidth())-180,0,96,48);
           }
         break;
    }
  menubar2 = !options.test(0) && !options.test(1) ||
             !options.test(2) && !options.test(3) ||
             !options.test(4) && !options.test(5) ||
             !options.test(6) && !options.test(7);
  if (menubar1 != menubar2)
    {  sizeevent(int(getwidth()),int(getheight()));
    }
  switch (menu)
    {  case BM_MAINCOMMAND :
         if (! options.test(0) && ! options.test(1) ||
               options.test(0) && ! options.test(1))
           {  buttonmenu(BM_MAINCOMMAND,buttonmenus.match("Tool"));
              buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Line"));
           }
         break;
       case BM_MAININTERRUPT :
         if (! options.test(2) && ! options.test(3) ||
               options.test(2) && ! options.test(3))
           {
              //buttonmenu(BM_MAININTERRUPT,buttonmenus.match("Interrupt"));
              buttonmenu(BM_MAININTERRUPT,buttonmenus.match("Assistant"));
              buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Zoom"));
           }
         break;
       case BM_MAINAPPLICATION :
         if (! options.test(6) && ! options.test(7) ||
               options.test(6) && ! options.test(7))
           {  buttonmenu(BM_MAINAPPLICATION,lastmainamenu);
              buttonmenu(BM_SUBAPPLICATION,lastsubamenu);
           }
         break;
       case BM_QUICK :
         if (! options.test(4) && ! options.test(5) ||
               options.test(4) && ! options.test(5))
           {  buttonmenu(BM_QUICK,buttonmenus.match("Quick"));
           }
         break;
    }
  if (menubar1 != menubar2)
    {  InvalidateRect(gethwnd(),NULL,TRUE);
       InvalidateRect(subwindow->gethwnd(),NULL,TRUE);
    }
  updatemenu();
}

void MainWindow::timerevent(void)
{MenuWindow *menuwindow;
 Control *c;
 int i;
  for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
    c->timerevent();
  for (floatingmenulist.start() ; (menuwindow = (MenuWindow *)floatingmenulist.next()) != 0 ; )
    menuwindow->timerevent();
  if (printpreviewwindow != 0)
    buttonmenus.match("PrintPreview")->timerevent();
}

void MainWindow::updatemenu(void)
{ paintevent();
}

int MainWindow::getshiftstate(void)
{
    return ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000);
}

int MainWindow::getcontrolstate(void)
{ return ((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000);
}

void MainWindow::updatemenus(int c)
{
  if (maincmenu != 0)
    maincmenu->checkbutton(c);
  if (subcmenu != 0)
    subcmenu->checkbutton(c);
  if (mainimenu != 0)
    mainimenu->checkbutton(c);
  if (subimenu != 0)
    subimenu->checkbutton(c);
  if (mainamenu != 0)
    mainamenu->checkbutton(c);
  if (subamenu != 0)
    subamenu->checkbutton(c);
  if (quickmenu != 0)
    quickmenu->checkbutton(c);

  program->checkcustombutton(c);
}

void MainWindow::resetbuttonmenus()
{
    if (maincmenu != 0)
      ((RCToolBox*)maincmenu->getQW())->resetLayout(((RCToolBox*)maincmenu->getQW())->getLayoutType());
    if (mainimenu != 0)
      ((RCToolBox*)mainimenu->getQW())->resetLayout(((RCToolBox*)subimenu->getQW())->getLayoutType());
    if (mainamenu != 0)
      ((RCToolBox*)mainamenu->getQW())->resetLayout(((RCToolBox*)mainamenu->getQW())->getLayoutType());
    if (quickmenu != 0)
      ((RCToolBox*)quickmenu->getQW())->resetLayout(((RCToolBox*)quickmenu->getQW())->getLayoutType());

    program->resetcustommenus();

    if (custommenuactive.getbuttonmenu() != NULL)
        ((RCToolBox*)custommenuactive.getbuttonmenu()->getQW())->resetLayout(((RCToolBox*)custommenuactive.getbuttonmenu()->getQW())->getLayoutType());
}

void MainWindow::moveprompt(int toggle)
{
  if (toggle == -1)
    options.toggle(9);
  else if (toggle == 0)
    options.clear(9);
  else
    options.set(9);
  sizeevent(int(getwidth()),int(getheight()));
}

//extern char *llabel;//fg
extern int check_license(RCCHAR *label,RCCHAR *label1,RCCHAR *label2,int application,int showmessages);

void MainWindow::updatetitle(void)
{
 RCCHAR label[600],name[600],key[300],value[600],*s;
 int i;

 ResourceString rs15(NCWIN+15);
 ResourceString rs4(NCWIN+4);
 ResourceString rsname(NCWIN+21+(cad_version&15));

 //qDebug() << "Entering :  updatetitle";

#if 0
  if (strncmp(llabel,_RCT("REGISTRATION ID  :  XX-X00-000000XXXXXXX"),40) == 0 && program != 0 && ! program->getlicensed() ||
      strncmp(llabel,_RCT("REGISTRATION ID  :  XX-X00-000000XXXXXXX"),40) != 0 && ! check_license(_RCT("Main CAD Application"),_RCT("RealCADKey1"),_RCT("RealCADKey2"),0,1))
    strcpy(label,"Unlicensed - ");
  else
#endif
      strcpy(label,"");
  //if (cadwindow != 0)
  //  cadwindow->updatetitle();
  strcpy(name,rsname.gets());
  if (strlen(applicationname) > 0)
    strcpy(name,applicationname);
  //qDebug() << "layer : " << db.header.getlayer();
  //qDebug() << "name  : " << db.layernames.getlayername(db.header.getlayer());

  QString elidedTitle = elideTextMiddle(QString(db.gettitle()),50);

  if (db.workplanes.getcurrent() == NULL)
    sprintf(label+strlen(label),rs4.gets(),name,
            elidedTitle.data(),rs15.gets(),
            db.header.getcolour(),db.header.getlayer(),
            db.layernames.getlayername(db.header.getlayer())==0?_RCT(""):db.layernames.getlayername(db.header.getlayer())
            ,db.header.getstyle(),db.lineweights.weighttomm(db.header.getweight()),db.grid.getdisplay() ? _RCT("On") : _RCT("Off"),db.grid.getsnap() ? _RCT("On") : _RCT("Off"));
  else
      sprintf(label+strlen(label),rs4.gets(),name,
              elidedTitle.data(),db.workplanes.getcurrent()->getname(),
              db.header.getcolour(),db.header.getlayer(),
              db.layernames.getlayername(db.header.getlayer())==0?_RCT(""):db.layernames.getlayername(db.header.getlayer())
              ,db.header.getstyle(),db.lineweights.weighttomm(db.header.getweight()),db.grid.getdisplay() ? _RCT("On") : _RCT("Off"),db.grid.getsnap() ? _RCT("On") : _RCT("Off"));

  title(label);

  if (styleentry != 0)
    {

       styleentry->draw(DM_NORMAL);
      
       styleentry->setcolour(db.header.getcolour());
       styleentry->setlayer(db.header.getlayer());
       styleentry->setstyle(db.header.getstyle());
       styleentry->setweight(db.lineweights.weighttomm(db.header.getweight()));
	   int styleSetId=((QComboBox*)(styleentry->gettypehwnd()))->currentIndex();
       //  Try to match the current settings with a style.
        for (i = 1 ; ; i++)		
         { 
			//if (styleSetId >= 0) i = styleSetId;
			sprintf(key,"StyleSet%d",i);
			//sprintf(key, "StyleSet%d", styleSetId);
            strcpy(name,"");
            GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
            //qDebug() <<  "style set : key " << QString(key) << " value : " << QString(value);
			if (strlen(value) == 0) 
              break;
            s = strtok(value,_RCT(","));
            if( s != 0 )
                strcpy(name,s);

            //qDebug() <<  "name " << QString(name);

            s = strtok(0,_RCT(","));
            //qDebug() <<  "colour matched : " << QString(s);
            if (s != 0 && db.header.getcolour() != atoi(s))
				continue;   //  Doesn't match
            s = strtok(0,_RCT(","));
            //qDebug() <<  "layer matched : " << QString(s);
            if (s != 0 && db.header.getlayer() != atoi(s))
				continue; 
            s = strtok(0,_RCT(","));
            //qDebug() <<  "style matched : " << QString(s);
            if (s != 0 && db.header.getstyle() != atoi(s))
				continue; 
            s = strtok(0,_RCT(","));
			if (atof(s) == 0.0) break;
            //qDebug() << "weight matched : " << QString(s);
            if (s != 0 && fabs(db.lineweights.weighttomm(db.header.getweight())-atof(s)) > 0.001)
				continue; 
			
            break;
         }

       //qDebug() << "updatetitle : style name : " << QString(name);
	   
		styleentry->settype(name);

       styleentry->draw(DM_NORMAL);
    }

    //update_properties(-1);

}

MainWindow::~MainWindow()
{MenuWindow *menuwindow;
 RCHBITMAP hbitmap;

DebugTraceLine(__FILE__,__LINE__);

  program->fileexitapps();
DebugTraceLine(__FILE__,__LINE__);

  hbitmap = (RCHBITMAP) SelectObject(gethdc(),hbitmapold);
  DeleteObject(hbitmap);
  DeleteDC(hoffscreenhdc);

  // KMJ: not needed using Qt timer
  //KillTimer(gethwnd(),timer);
  buttonmenu(BM_MAINCOMMAND,NULL);
  buttonmenu(BM_SUBCOMMAND,NULL);
  buttonmenu(BM_MAININTERRUPT,NULL);
  buttonmenu(BM_SUBINTERRUPT,NULL);
  buttonmenu(BM_MAINAPPLICATION,NULL);
  buttonmenu(BM_SUBAPPLICATION,NULL);
  buttonmenu(BM_QUICK,NULL);
DebugTraceLine(__FILE__,__LINE__);
  for (floatingmenulist.start() ; (menuwindow = (MenuWindow *)floatingmenulist.next()) != 0 ; )
    delete menuwindow;
  delete subwindow;  subwindow = 0;
DebugTraceLine(__FILE__,__LINE__);
  delcontrol(prompt);
DebugTraceLine(__FILE__,__LINE__);
  delcontrol(statusprompt);
DebugTraceLine(__FILE__,__LINE__);
  delcontrol(coordinateentry);
DebugTraceLine(__FILE__,__LINE__);
  delcontrol(coordinatedisplay);
DebugTraceLine(__FILE__,__LINE__);
  delete prompt;  prompt = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete statusprompt;  statusprompt = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete coordinateentry;  coordinateentry = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete coordinatedisplay;  coordinatedisplay = 0;
DebugTraceLine(__FILE__,__LINE__);
  view3dlist.destroy();
DebugTraceLine(__FILE__,__LINE__);
#if VERSION != TRICAD_STUDENT
  cadbasiclist.destroy();
#endif
DebugTraceLine(__FILE__,__LINE__);
  floatingmenulist.destroy();
DebugTraceLine(__FILE__,__LINE__);
  delete floatingmenu1;  floatingmenu1 = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete floatingmenu2;  floatingmenu2 = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete floatingmenu3;  floatingmenu3 = 0;
DebugTraceLine(__FILE__,__LINE__);
  delete floatingmenu4;  floatingmenu4 = 0;
DebugTraceLine(__FILE__,__LINE__);
  if (hsfont != 0)
    DeleteObject(hsfont);
DebugTraceLine(__FILE__,__LINE__);
  wm.destroy();
DebugTraceLine(__FILE__,__LINE__);
  hsfont = 0;
DebugTraceLine(__FILE__,__LINE__);
  cadwindow = 0;
DebugTraceLine(__FILE__,__LINE__);
}

MenuWindow::MenuWindow(Window *window,int x,int y,int w,int h) :
  Window((char*)"MenuWindow",window->gethwnd(),(RCDWORD)(WS_POPUPWINDOW | WS_CAPTION),x,y,w,h,SW_SHOW)
{
    cadwindow->addfloatingmenuwindow(this);
}

void MenuWindow::nclbuttondblclk(int,int,int)
{ //  Resize the window from horizontal to vertical.


}

void MenuWindow::timerevent()
{Control *c;
 int i;
  for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
    c->timerevent();
}

MenuWindow::~MenuWindow()
{ if (cadwindow != 0)
    cadwindow->delfloatingmenuwindow(this);

  //  if this is an application menu, delete it from the menu list
  if (buttonmenus.match((ButtonMenu *)getcontrol(0)) != 0 && strncmp(buttonmenus.match((ButtonMenu *)getcontrol(0)),"ApplicationMenu::",17) == 0)
    buttonmenus.deletemenu((ButtonMenu *)getcontrol(0));

  if (buttonmenus.match((ButtonMenu *)getcontrol(0)) != 0 && _stricmp(buttonmenus.match((ButtonMenu *)getcontrol(0)),"Custom") == 0)
    v.setinteger("cm::active",0);

}

#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
VideoWindow::VideoWindow(Window *window,RCCHAR *filename,RCCHAR *t) :
  Window(_RCT("CadWindow"),window->gethwnd(),(DWORD)(DWORD)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS),CW_USEDEFAULT,CW_USEDEFAULT,200,200,SW_HIDE)
{
 RCMCI_DGV_OPEN_PARMS mciOpen;
 RCMCI_DGV_WINDOW_PARMS mciWindow;
 RCMCI_DGV_PUT_PARMS mciPut;
 RCMCI_DGV_RECT_PARMS mciRect;
 RCMCI_SET_PARMS mciSet;
 RECT rect;
  title(t);
  mciOpen.lpstrElementName = filename;
  if (mciSendCommand(0,MCI_OPEN,MCI_WAIT|MCI_OPEN_ELEMENT,(DWORD)&mciOpen))
    return;
  mciWindow.hWnd =  (RCHWND)gethwnd();
  mciWindow.nCmdShow = SW_SHOWNORMAL;
  mciWindow.lpstrText = 0;
  if (mciSendCommand(mciOpen.wDeviceID,MCI_WINDOW,MCI_WAIT|MCI_DGV_WINDOW_HWND,(DWORD)&mciWindow))
    return;
  if (mciSendCommand(mciOpen.wDeviceID,MCI_WHERE,MCI_WAIT|MCI_DGV_WHERE_SOURCE,(DWORD)&mciRect))
    return;
  GetClientRect(gethwnd(),&rect);
  SetWindowPos(gethwnd(),0,0,0,mciRect.rc.right+4+200-rect.right,mciRect.rc.bottom+30+200-rect.bottom,SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
  mciPut.rc.left = 2;
  mciPut.rc.right = mciRect.rc.right;
  mciPut.rc.top = 2;
  mciPut.rc.bottom = mciRect.rc.bottom;
  if (mciSendCommand(mciOpen.wDeviceID,MCI_PUT,MCI_WAIT|MCI_DGV_RECT|MCI_DGV_PUT_DESTINATION,(DWORD)&mciPut))
    return;
  if (mciSendCommand(mciOpen.wDeviceID,MCI_SET,MCI_SET_ON | MCI_DGV_SET_SEEK_EXACTLY,(DWORD)&mciSet))
    return;
  deviceid = mciOpen.wDeviceID;

  menu = new ButtonMenu(0,0,6,6,15);
  if (menu != 0)
    {  menu->add(new Button(-1,-1, (char*)"Animation9",20013));
       menu->add(new Button(-1,-1, (char*)"Animation11",20015));
       menu->add(new Button(-1,-1, (char*)"Animation6",20010));
       menu->add(new Button(-1,-1, (char*)"Animation7",20011));
       menu->add(new Button(-1,-1, (char*)"Animation10",20014));
       menu->add(new Button(-1,-1, (char*)"Animation8",20012));
       addcontrol(menu);
       menu->move(this,mciRect.rc.right/2-48,mciRect.rc.bottom+4,6);
    }
  SetBkColor(gethdc(),GetSysColor(COLOR_BTNSHADOW));
  SetTextAlign(gethdc(),TA_RIGHT	| TA_TOP);
  GetClientRect(gethwnd(),&rect);
  y = rect.bottom - 20;
}

void VideoWindow::paintevent(void)
{
 RECT rect;
 MCI_DGV_UPDATE_PARMS mciUpdate;
 RCHBRUSH hbrush;
  GetClientRect(gethwnd(),&rect);
  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);
  if (deviceid != 0)
    {  mciUpdate.hDC = (HDC)gethdc();
       mciSendCommand(deviceid,MCI_UPDATE,MCI_WAIT|MCI_DGV_UPDATE_HDC,(DWORD)&mciUpdate);
    }
  Window::paintevent();
  timerevent();
}

void VideoWindow::play(void)
{
  MCI_DGV_PLAY_PARMS mciPlay;
  mciSendCommand(deviceid,MCI_PLAY,0,(DWORD)&mciPlay);
  activevideowindow = this;
}

void VideoWindow::reverse(void)
{
  MCI_DGV_PLAY_PARMS mciPlay;
  mciSendCommand(deviceid,MCI_PLAY,MCI_DGV_PLAY_REVERSE,(DWORD)&mciPlay);
  activevideowindow = this;
}

void VideoWindow::stop(void)
{
 MCI_DGV_STOP_PARMS mciStop;
 MCI_SEEK_PARMS mciSeek;
  mciSendCommand(deviceid,MCI_STOP,0,(DWORD)&mciStop);
  mciSeek.dwTo = 0;
  mciSendCommand(deviceid,MCI_SEEK,MCI_SEEK_TO_START,(DWORD)&mciSeek);
  activevideowindow = 0;
  timerevent();
}

void VideoWindow::pause(void)
{
  MCI_DGV_PAUSE_PARMS mciPause;
  mciSendCommand(deviceid,MCI_PAUSE,0,(DWORD)&mciPause);
  activevideowindow = 0;
  timerevent();
}

void VideoWindow::stepforward(void)
{
 MCI_DGV_STATUS_PARMS mciStatus;
 MCI_DGV_STEP_PARMS mciStep;
  mciStep.dwFrames = 1;
  mciSendCommand(deviceid,MCI_STEP, MCI_DGV_STEP_FRAMES,(DWORD)&mciStep);
  mciStatus.dwItem = MCI_STATUS_MODE;
  do {  mciSendCommand(deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatus);
     } while (mciStatus.dwReturn != MCI_MODE_STOP && mciStatus.dwReturn != MCI_MODE_PAUSE);
  activevideowindow = 0;
  timerevent();
}

void VideoWindow::stepreverse(void)
{
 MCI_DGV_STATUS_PARMS mciStatus;
 MCI_DGV_STEP_PARMS mciStep;
  mciStatus.dwItem = MCI_STATUS_MODE;
  mciSendCommand(deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatus);

  mciStep.dwFrames = 1;
  mciSendCommand(deviceid,MCI_STEP, MCI_DGV_STEP_REVERSE,(DWORD)&mciStep);
  mciStatus.dwItem = MCI_STATUS_MODE;
  do {  mciSendCommand(deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatus);
     } while (mciStatus.dwReturn != MCI_MODE_STOP && mciStatus.dwReturn != MCI_MODE_PAUSE);
  activevideowindow = 0;
  timerevent();
}

void VideoWindow::timerevent(void)
{
 MCI_DGV_STATUS_PARMS mciStatus;
 RCCHAR line[20];
  mciStatus.dwItem = MCI_STATUS_POSITION;
  mciSendCommand(deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatus);
  sprintf(line,"%6ld",mciStatus.dwReturn+1);
  TextOut(gethdc(),35,y,(RCLPCTSTR)line,6);
  if (menu != 0) menu->timerevent();
}

VideoWindow::~VideoWindow()
{
  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
  activevideowindow = 0;
  delete menu;
}

CadBasicWindow::CadBasicWindow(Window *window,CadBasicProgram *cb,long) :
  Window((char*)"CadWindow",window->gethwnd(),(DWORD)(DWORD)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS),CW_USEDEFAULT,CW_USEDEFAULT,640,480,SW_NORMAL)
{ cbp = cb;
  menu = new ButtonMenu(0,0,7,7,15);
  if (menu != 0)
    {  menu->add(new Button(-1,-1, (char*)"Basic1",904));
       menu->add(new Button(-1,-1, (char*)"Basic2",905));
       menu->add(new Button(-1,-1, (char*)"Basic3",906));
       menu->add(new Button(-1,-1, (char*)"Basic4",907));
       menu->add(new Button(-1,-1, (char*)"Basic5",908));
       menu->add(new Button(-1,-1, (char*)"Basic6",909));
       menu->add(new Button(-1,-1, (char*)"Basic7",910));
       addcontrol(menu);
       menu->move(this,CN_CENTRED,-24,7);
    }

  cbew = new CadBasicEditorWindow(this,int(width),int(height-25),cb,0);

  title(cbp->gettitle());

  cadwindow->addcadbasicwindow(this);

  running = 0;

}

void CadBasicWindow::paintevent(void)
{RCHBRUSH hbrush;
 RECT rect;
  GetClientRect(gethwnd(),&rect);
  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  rect.top = rect.bottom - 25;
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);
  rect.bottom = rect.top + 1;
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
  Window::paintevent();
}

void CadBasicWindow::charevent(UINT c,DWORD buttons)
{ cbew->charevent(c,buttons);
}

void CadBasicWindow::sizeevent(int w,int h)
{ Window::sizeevent(w,h);
  SetWindowPos(cbew->gethwnd(),HWND_BOTTOM,0,0,w,h-25,SWP_DRAWFRAME);
}

void CadBasicWindow::run(void)
{long lineno;
  if (this == 0)
    return;
  if (cadwindow->getcadbasicwindow() != 0 &&
      cadwindow->getcadbasicwindow()->getrunning())
    { RCMessageBox(cadwindow->gethwnd(),_RCT("Another CAD Basic program is already running"),_RCT("Run CAD Bsaic"),MB_ICONINFORMATION);
       return;
    }
  cadwindow->selectwindow(this);
  lineno = cbp->getlineno();
  cbew->highlight(lineno);
  running = 1;
  for (;;)
    {  if (! cbp->run(0))
         {  cbew->highlight(lineno);
            cbew->highlight(cbp->getlineno());
            break;
         }
       cbew->highlight(lineno);
       lineno = cbp->getlineno();
       cbew->highlight(lineno);
       if (cbp->getbreakpoint(lineno))
         break;
       program->processmessages(0);
       if (state.getevent())
         break;
    }
  running = 0;
}

void CadBasicWindow::step(void)
{long lineno;
  cadwindow->selectwindow(this);
  lineno = cbp->getlineno();
  cbew->highlight(lineno);
  cbp->run(0);
  cbew->highlight(lineno);
  lineno = cbp->getlineno();
  cbew->highlight(lineno);
}

void CadBasicWindow::pause(void)
{
}

void CadBasicWindow::stop(void)
{long lineno;
  cadwindow->selectwindow(this);
  lineno = cbp->getlineno();
  cbew->highlight(lineno);
  cbp->run(1);
  cbew->highlight(lineno);
  cbew->highlight(cbp->getlineno());
}

CadBasicWindow::~CadBasicWindow()
{ delete cbew;
  cadwindow->delcadbasicwindow(this);
}

CadBasicEditorWindow::CadBasicEditorWindow(Window *window,int w,int h,CadBasicProgram *cb,long) :
  Window((char*)"CadWindow",window->gethwnd(),(DWORD)(WS_CHILD | WS_VSCROLL),0,0,w,h,SW_NORMAL)
{SIZE size;
  cbp = cb;
  SetScrollRange(gethwnd(),SB_VERT,0,1000,TRUE);
  hfont = CreateFont(18,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE | DEFAULT_PITCH,_RCT("Courier New"));
  holdfont = (RCHFONT) SelectObject(gethdc(),hfont);
  GetTextExtentPoint(gethdc(),_RCT("A"),1,&size);
  cheight = size.cy;
  cwidth = size.cx;
  selectiondrawn = 0;
  captured = 0;
  x1 = y1 = x2 = y2 = 0;
  ybase = 0;
}

CadBasicEditorWindow::~CadBasicEditorWindow()
{ if (hfont != 0 && gethdc() != 0)
    {  SelectObject(gethdc(),holdfont);
       DeleteObject(hfont);
    }
}

void CadBasicEditorWindow::paintevent(void)
{RECT rect;
 long i,nl;
 RCHBRUSH hbrush;
  GetClientRect(gethwnd(),&rect);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));

  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  rect.right = cwidth*2+1;
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);
  rect.left = cwidth*2;
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));

  nl = int(height / cheight);
  if (nl + ybase > cbp->getnlines())
    nl = cbp->getnlines() - ybase;

  SetTextColor(gethdc(),RGB(0,0,0));
  SetBkColor(gethdc(),RGB(255,255,255));
  for (i = 0 ; i < nl ; i++)
    {  TextOut(gethdc(),cwidth * 2 + 2,int(1 + i*cheight),(RCLPCTSTR)cbp->getline(i+ybase),strlen(cbp->getline(i+ybase)));
       highlight(i + ybase);
    }

  if (cbp->getlineno() >= ybase && cbp->getlineno() < height / cheight)
    highlight(cbp->getlineno());

  paintselection(0);

  Window::paintevent();
}

int CadBasicEditorWindow::deleteselection(void)
{long y,t;
 RCCHAR line[300];
  if (x1 != x2 || y1 != y2)
    {  if (y2 < y1)
         {  t = y1;  y1 = y2;  y2 = t;
         }
       else if (y1 == y2 && x2 < x1)
         {  t = x1;  x1 = x2;  x2 = t;
         }
       if (y1 == y2 && cbp->getline(y1) != 0)
         {  strcpy(line,cbp->getline(y1));
            if ((long)strlen(cbp->getline(y1)) > x2)
              {  cbp->getline(y1)[(int)x1] = 0;
                 strcat(cbp->getline(y1),line+(int)x2);
              }
            else
              cbp->getline(y1)[(int)x1] = 0;
         }
       else if (cbp->getline(y1) != 0)
         {  cbp->getline(y1)[(int)x1] = 0;
            if (cbp->getline(y2) != 0)
              strcat(cbp->getline(y1),cbp->getline(y2)+(int)x2);
            for (y = y1 + 1 ; y <= y2 ; y++)
              cbp->del(cbp->getline(y1+1));
         }
       y2 = y1;  x2 = x1;
       paintevent();
       return 1;
    }
  return 0;
}

void CadBasicEditorWindow::paste(void)
{HGLOBAL clipdata;
 RCCHAR *cd;
 long y;
 RCCHAR line[300],endline[300];
  if (OpenClipboard(cadwindow->gethwnd()))
    {  clipdata = GetClipboardData(CF_TEXT);
       if (clipdata != 0)
         {  cd = (RCCHAR *) GlobalLock(clipdata);
            if (cd != 0)
              {  y = y1;
                 while (cbp->getline(y) == 0)
                   cbp->add(y,_RCT(""));
                 while (x1 > (long)strlen(cbp->getline(y)))
                   strcat(cbp->getline(y)," ");
                 strcpy(line,cbp->getline(y));
                 strcpy(endline,cbp->getline(y)+x1);
                 line[x1] = 0;
                 while (*cd != 0)
                   {  if (*cd != '\n' && *cd != '\r')
                        {  line[strlen(line)+1] = 0;
                           line[strlen(line)] = *cd;
                        }
                      else if (*cd == '\n')
                        {  strcpy(cbp->getline(y),line);
                           strcpy(line,"");
                           y++;
                           cbp->add(y,_RCT(""));
                        }
                      cd++;
                   }
                 strcat(line,endline);
                 strcpy(cbp->getline(y),line);
                 GlobalUnlock(clipdata);
                 if (y == y1)
                   paintline(y1);
                 else
                   paintevent();
              }
         }
       CloseClipboard();
    }
}

void CadBasicEditorWindow::copyselection(void)
{HGLOBAL clipdata;
 RCCHAR *cd;
 long t,y;
 DWORD nbytes;
  if (x1 != x2 || y1 != y2)
    {  if (y2 < y1)
         {  t = y1;  y1 = y2;  y2 = t;
         }
       else if (y1 == y2 && x2 < x1)
         {  t = x1;  x1 = x2;  x2 = t;
         }
       if (y1 == y2 && cbp->getline(y1) != 0)
         {  if (x1 < (long)strlen(cbp->getline(y1)))
              {  if (x2 < (long)strlen(cbp->getline(y1)))
                   nbytes = x2 - x1;
                 else
                   nbytes = strlen(cbp->getline(y1)) - x1;
              }
            else
              nbytes = 0;
         }
       else
         {  if (cbp->getline(y1) != 0 && x1 < (long)strlen(cbp->getline(y1)))
              nbytes = strlen(cbp->getline(y1)) - x1 + 2;
            else
              nbytes = 2;
            if (cbp->getline(y2) != 0)
              {  if (x2 < (long)strlen(cbp->getline(y2)))
                   nbytes += x2;
                 else
                   nbytes += strlen(cbp->getline(y2));
              }
            for (y = y1 + 1 ; y < y2 ; y++)
              if (cbp->getline(y) != 0)
                nbytes += strlen(cbp->getline(y)) + 2;
              else
                nbytes += 2;
         }
       if (OpenClipboard(cadwindow->gethwnd()))
         {  clipdata = GlobalAlloc(0,nbytes+1);
            if (clipdata != 0)
              {  cd = (RCCHAR *) GlobalLock(clipdata);
                 if (cd != 0)
                   {  if (y1 == y2 && cbp->getline(y1) != 0)
                        {  if (x1 < (long)strlen(cbp->getline(y1)))
                             {  if (x2 < (long)strlen(cbp->getline(y1)))
                                  {  strncpy(cd,cbp->getline(y1)+x1,x2 - x1);
                                     cd[x2-x1] = 0;
                                  }
                                else
                                  strcpy(cd,cbp->getline(y1)+x1);
                             }
                           else
                             strcpy(cd,"");
                        }
                      else
                        {  if (cbp->getline(y1) != 0 && x1 < (long)strlen(cbp->getline(y1)))
                             strcpy(cd,cbp->getline(y1)+x1);
                           else
                             strcpy(cd,"");
                           strcat(cd,"\n\r");
                           for (y = y1 + 1 ; y < y2 ; y++)
                             {  if (cbp->getline(y) != 0)
                                  strcat(cd,cbp->getline(y));
                                strcat(cd,"\n\r");
                             }
                           if (cbp->getline(y2) != 0)
                             {  if (x2 >= (long)strlen(cbp->getline(y2)))
                                  strcat(cd,cbp->getline(y2));
                                else
                                  {  strncat(cd,cbp->getline(y2),x2);
                                     cd[nbytes] = 0;
                                  }
                             }
                        }
                   }
                 GlobalUnlock(clipdata);
                 SetClipboardData(CF_TEXT,clipdata);
              }
            CloseClipboard();
         }
    }
}

void CadBasicEditorWindow::charevent(UINT c,DWORD vk)
{RCCHAR line[300],*s;
  if (vk == VK_HOME)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1 = x2 = 0;  y1 = y2;
         }
       else
         x2 = 0;
       paintselection(0);
    }
  else if (vk == VK_END)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  if (cbp->getline(y2) == 0)
              x1 = x2 = 0;
            else
              x1 = x2 = strlen(cbp->getline(y2));
            y1 = y2;
         }
       else
         {  if (cbp->getline(y2) == 0)
              x2 = 0;
            else
              x2 = strlen(cbp->getline(y2));
         }
       paintselection(0);
    }
  else if (vk == VK_RIGHT)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1++;  x2 = x1;
         }
       else
         x2++;
       paintselection(0);
    }
  else if (vk == VK_LEFT)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  x1--;  if (x1 < 0) x1 = 0;  x2 = x1;
         }
       else
         {  x2--;  if (x2 < 0) x2 = 0;
         }
       paintselection(0);
    }
  else if (vk == VK_DOWN)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  y1++;   y2 = y1;  x1 = x2;
            if (y1 >= ybase + height / cheight)
              {  ybase = int(y1 - height / cheight + 1);
                 if (ybase < 0) ybase = 0;
                 if (ybase >= cbp->getnlines())
                   {  ybase = cbp->getnlines() - 1;
                      if (ybase < 0)
                        ybase = 0;
                      y1--;  y2--;
                   }
                 paintevent();
                 paintselection(1);
              }
         }
       else
         {  y2++;
            if (y2 >= ybase + height / cheight)
              {  ybase = int(y2 - height / cheight + 1);
                 if (ybase < 0) ybase = 0;
                 if (ybase >= cbp->getnlines())
                   {  ybase = cbp->getnlines() - 1;
                      y2--;
                   }
                 paintevent();
                 paintselection(1);
              }
         }
       paintselection(0);
    }
  else if (vk == VK_PRIOR)
    {  paintselection(1);
       if (ybase < height / cheight - 1)
         {  y1 -= ybase;  y2 = y1;  x2 = x1;
            ybase = 0;
         }
       else
         {  y1 -= int(height / cheight - 1);  y2 = y1;  x2 = x1;
            ybase -= int(height / cheight - 1);
         }
       paintevent();
    }
  else if (vk == VK_NEXT)
    {  paintselection(1);
       if (ybase < cbp->getnlines() - height / cheight - 1)
         {  y1 += int(height / cheight - 1);
            ybase += int(height / cheight - 1);
            y2 = y1;  x2 = x1;
         }
       paintevent();
    }
  else if (vk == VK_UP)
    {  paintselection(1);
       if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
         {  y1--;  if (y1 < 0) y1 = 0;  y2 = y1;
            if (y1 < ybase)
              {  ybase = y1;
                 paintevent();
                 paintselection(1);
              }
         }
       else
         {  y2--;  if (y2 < 0) y2 = 0;
            if (y2 < ybase)
              {  ybase = y2;
                 paintevent();
                 paintselection(1);
              }
         }
       paintselection(0);
    }
  else if (vk == VK_BACK)
    {  if (deleteselection())
         return;
       if (cbp->getline(y1) != 0)
         {  if (x1 > (long)strlen(cbp->getline(y1)))
              {  x1--;  x2--;
              }
            else if (x1 == 0)
              {  if (y1 > 0)
                   {  x1 = strlen(cbp->getline(y1-1));
                      strcat(cbp->getline(y1-1),cbp->getline(y1));
                      cbp->del(cbp->getline(y1));
                      x2 = x1;  y1--;  y2--;
                   }
              }
            else
              {  strcpy(line,cbp->getline(y1));
                 strcpy(cbp->getline(y1)+(int)x1-1,line+(int)x1);
                 x1--;  x2 = x1;
                 paintline(y1);
              }
            paintevent();
         }
       else
         {  if (x1 > 0 && x2 > 0)
              {  paintselection(1);
                 x1--;  x2--;
                 paintline(y1);
                 paintselection(0);
              }
         }

    }
  else if (vk == VK_INSERT)
    {  if ((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000)
         {  copyselection();
         }
       else if ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000)
         {  deleteselection();
            paste();
         }
    }
  else if (vk == VK_DELETE)
    {  copyselection();
       if (deleteselection())
         return;
       if (cbp->getline(y1) != 0)
         {  if (x1 >= (long)strlen(cbp->getline(y1)) && cbp->getline(y1+1) != 0)
              {  x1 = strlen(cbp->getline(y1));
                 strcat(cbp->getline(y1),cbp->getline(y1+1));
                 cbp->del(cbp->getline(y1+1));
                 x2 = x1;
                 paintevent();
              }
            else
              {  strcpy(line,cbp->getline(y1));
                 strcpy(cbp->getline(y1)+(int)x1,line+(int)x1+1);
                 paintline(y1);
              }
         }
       else
         {  line[0] = RCCHAR(c);  line[1] = 0;
            cbp->add(y1,line);
            x2 = 1;
            paintline(y1);
         }
    }
  else if (vk == VK_RETURN)
    {  deleteselection();
       if (cbp->getline(y1) != 0)
         {  if (x1 > (long)strlen(cbp->getline(y1)))
              {  x1 = x2 = 0;
                 y1++;  y2 = y1;
                 cbp->add(y1,_RCT(""));
              }
            else
              {  strcpy(line,cbp->getline(y1));
                 cbp->getline(y1)[(int)x1] = 0;
                 s = line + (int)x1;
                 x1 = x2 = 0;
                 y1++;  y2 = y1;
                 cbp->add(y1,s);
              }
            if (y1 >= ybase + height / cheight)
              {  ybase = int(y1 - height / cheight + 1);
                 if (ybase < 0) ybase = 0;
                 if (ybase >= cbp->getnlines())
                   {  ybase = cbp->getnlines() - 1;
                      if (ybase < 0)
                        ybase = 0;
                   }
              }
            paintevent();
         }
    }
  else if (c >= 32 && c <= 126)
    {  deleteselection();
       if (cbp->getline(y1) != 0)
         {  strcpy(line,cbp->getline(y1));
            while (x1 > (long)strlen(line))
              strcat(line," ");
            line[(int)x1] = (RCCHAR) c;
            line[(int)x1+1] = 0;
            if ((long)strlen(cbp->getline(y1)) > x1)
              strcat(line,cbp->getline(y1)+x1);
            x1++;  x2++;
            strcpy(cbp->getline(y1),line);
            paintline(y1);
         }
       else
         {  while (cbp->getline(y1) == 0)
              cbp->add(y1,_RCT(""));
            line[0] = 0;
            while (x1 > (long)strlen(line))
              strcat(line," ");
            line[(int)x1] = (RCCHAR) c;  line[(int)x1+1] = 0;
            strcpy(cbp->getline(y1),line);
            x2 = ++x1;
            paintline(y1);
         }
    }
  vscrollevent(1000,1000);
}

void CadBasicEditorWindow::paintselection(int clear)
{RECT rect;
 int x,nchars;
 long y,xt1,yt1,xt2,yt2;
 RCCHAR line[300];
  if (clear && ! selectiondrawn)
    return;

  if (clear)
    {  SetTextColor(gethdc(),RGB(0,0,0));
       SetBkColor(gethdc(),RGB(255,255,255));
    }
  else
    {  SetTextColor(gethdc(),GetSysColor(COLOR_HIGHLIGHTTEXT));
       SetBkColor(gethdc(),GetSysColor(COLOR_HIGHLIGHT));
    }
  if (y1 < y2 || y1 == y2 && x1 < x2)
    {  xt1 = x1;  xt2 = x2;  yt1 = y1;  yt2 = y2;
    }
  else
    {  xt1 = x2;  xt2 = x1;  yt1 = y2;  yt2 = y1;
    }

  if (clear)
    {  rect.left = cwidth * 2 + (int) x2 * cwidth + 1;  rect.right = cwidth * 2 + (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
       InvertRect(gethdc(),&rect);
    }
  SetBkMode(gethdc(),OPAQUE);

  for (y = yt1 ; y <= yt2 ; y++)
    {  if (y == yt1)
         {  x = cwidth * 2 + 2 + (int) xt1 * cwidth;
            if (cbp->getline(y) != 0 && (long)strlen(cbp->getline(y)) > xt1)
              sprintf(line,"%-299s",cbp->getline(y)+(int)xt1);
            else
              sprintf(line,"%-299s"," ");
            nchars = int(width / cwidth) - (int)xt1;
         }
       else
         {  if (cbp->getline(y) != 0 && (long)strlen(cbp->getline(y)) > xt1)
              sprintf(line,"%-299s",cbp->getline(y));
            else
              sprintf(line,"%-299s"," ");
            x = cwidth * 2 + 2;
            nchars = int(width / cwidth);
         }
       if (y == yt2)
         nchars = nchars - (int(width / cwidth) - (int) xt2);
       TextOut(gethdc(),x,int(2+(y-ybase)*cheight),(RCLPCTSTR)line,nchars);
    }
  if (! clear)
    {  rect.left = cwidth * 2 + (int) x2 * cwidth + 1;  rect.right = cwidth * 2 + (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
       InvertRect(gethdc(),&rect);
    }
  selectiondrawn = ! clear;
}

void CadBasicEditorWindow::paintline(long y)
{RECT rect;
  rect.left = (int)cwidth * 2 + 1;
  rect.right = (int)width;
  rect.top = (int)(y - ybase) * cheight + 1;
  rect.bottom = (int)(y - ybase) * cheight + cheight + 1;
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
  if (cbp->getline(y) == 0)
    return;
  SetTextColor(gethdc(),RGB(0,0,0));
  SetBkColor(gethdc(),RGB(255,255,255));
  SetBkMode(gethdc(),OPAQUE);
  TextOut(gethdc(),cwidth * 2 + 2,int(1+(y-ybase)*cheight),(RCLPCTSTR)cbp->getline(y),strlen(cbp->getline(y)));

  if (y == y2)
    {  rect.left = cwidth * 2 + (int) x2 * cwidth + 1;  rect.right = cwidth * 2 + (int) x2 * cwidth + 4;
       rect.top = (int) (y2 - ybase) * cheight + 1;  rect.bottom = (int) (y2 - ybase) * cheight + cheight + 1;
       InvertRect(gethdc(),&rect);
       selectiondrawn = 1;
    }

  highlight(y);

}

void CadBasicEditorWindow::highlight(long line)
{RCHBRUSH holdbrush,hbrush;
 RCHPEN holdpen;
 POINT points[7];
 RECT rect;
 int r;

  if (height / cheight + ybase <= line)
    {  if (line >= 2)
         ybase = line - 2;
       else
         ybase = line;
       paintevent();
    }
  else if (ybase > line)
    {  if (line >= 2)
         ybase = line - 2;
       else
         ybase = line;
       paintevent();
    }

  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
  rect.left = 1;  rect.right = cwidth*2-1;
  rect.top = 1 + (line - ybase) * cheight;  rect.bottom = rect.top + cheight;
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);

  if (cbp->getlineno() == line)
    {  holdpen = (RCHPEN) SelectObject(gethdc(),GetStockObject(BLACK_PEN));
       hbrush = CreateSolidBrush(RGB(255,0,0));
       holdbrush = (RCHBRUSH) SelectObject(gethdc(),hbrush);

       points[0].x = cwidth * 2 / 6;
       points[0].y = 1 + (line - ybase) * cheight + cheight / 3;
       points[1].x = cwidth * 2 / 6;
       points[1].y = 1 + (line - ybase) * cheight + cheight * 2 / 3;
       points[2].x = cwidth*2 / 2;
       points[2].y = 1 + (line - ybase) * cheight + cheight * 2 / 3;
       points[3].x = cwidth*2 / 2;
       points[3].y = 1 + (line - ybase) * cheight + cheight * 5 / 6;
       points[4].x = cwidth*2 * 5 / 6;
       points[4].y = 1 + (line - ybase) * cheight + cheight / 2;
       points[5].x = cwidth*2 / 2;
       points[5].y = 1 + (line - ybase) * cheight + cheight / 6;
       points[6].x = cwidth*2 / 2;
       points[6].y = 1 + (line - ybase) * cheight + cheight / 3;

       Polygon(gethdc(),points,7);

       SelectObject(gethdc(),holdpen);
       SelectObject(gethdc(),holdbrush);
       DeleteObject(holdbrush);
    }
  else if (cbp->getbreakpoint(line))
    {  holdpen = (RCHPEN) SelectObject(gethdc(),GetStockObject(BLACK_PEN));
       hbrush = CreateSolidBrush(RGB(0,255,0));
       holdbrush = (RCHBRUSH) SelectObject(gethdc(),hbrush);

       r = cwidth / 2;
       Ellipse(gethdc(),cwidth-r,1 + (line - ybase) * cheight + cheight / 2-r,
                        cwidth+r,1 + (line - ybase) * cheight + cheight / 2+r);

       SelectObject(gethdc(),holdpen);
       SelectObject(gethdc(),holdbrush);
       DeleteObject(holdbrush);
    }
}

void CadBasicEditorWindow::lbuttondownevent(int x,int y,int keys)
{long lineno;
  if (y > height - 24)
    Window::lbuttondownevent(x,y,keys);
  else if (x <= cwidth * 2)
    {  lineno = (y - 1) / cheight + ybase;
       if (cbp->getline(lineno) != 0)
         {  if (cbp->getbreakpoint(lineno) != 0)
              cbp->setbreakpoint(lineno,0);
            else
              cbp->setbreakpoint(lineno,1);
            highlight(lineno);
         }
    }
  else
    {  paintselection(1);
       if ((keys & MK_SHIFT) == MK_SHIFT)
         {  x2 = (x - 1) / cwidth - 2;
            y2 = (y - 1) / cheight + ybase;
         }
       else
         {  x1 = (x - 1) / cwidth - 2;  x2 = x1;
            y1 = (y - 1) / cheight + ybase;  y2 = y1;
         }
       paintselection(0);
       SetCapture(gethwnd());
       captured = 1;
    }
}

void CadBasicEditorWindow::setcurrentline(long l)
{ if (l < ybase || l >= ybase + height / cheight)
    {  x1 = x2 = 0;
       y1 = y2 = l;
       if (l < ybase)
         ybase = l;
       if (l >= ybase + height / cheight)
         ybase = int(l - height / cheight + 1);
       if (ybase < 0) ybase = 0;
       if (ybase >= cbp->getnlines())
         ybase = cbp->getnlines() - 1;
       paintevent();
    }
  else
    {  paintselection(1);
       x1 = x2 = 0;
       y1 = y2 = l;
       paintselection(0);
    }
}

void CadBasicEditorWindow::mousemoveevent(int x,int y,int b)
{ if (captured)
    {  paintselection(1);
       x2 = (x - 1) / cwidth - 2;  if (x2 < 0) x2 = 0;
       y2 = (y - 1) / cheight + ybase;
       if (y2 < ybase)
         {  ybase = y2;
            if (ybase < 0)
              ybase = 0;
            paintevent();
         }
       else if (y2 - ybase > (height + cheight - 1 ) / cheight)
         {  ybase = long(y2 - (height + cheight - 1 ) / cheight + 1);
            if (ybase > cbp->getnlines() - height / cheight)
              ybase = long(cbp->getnlines() - height / cheight);
            if (ybase < 0)
              ybase = 0;
            paintevent();
         }
       else
         paintselection(0);
    }
  else
    Window::mousemoveevent(x,y,b);
}

void CadBasicEditorWindow::lbuttonupevent(int x,int y,int b)
{ if (captured)
    {  paintselection(1);
       x2 = (x - 1) / cwidth - 2;  if (x2 < 0) x2 = 0;
       y2 = (y - 1) / cheight + ybase;
       paintselection(0);
       ReleaseCapture();
       captured = 0;
    }
  else
    Window::mousemoveevent(x,y,b);
}

void CadBasicEditorWindow::vscrollevent(int code,int pos)
{int start,end;
  GetScrollRange(gethwnd(),SB_VERT,&start,&end);
  switch (code)
    {case SB_BOTTOM : ybase = long(cbp->getnlines() - height / cheight);  break;
     case SB_LINEDOWN : ybase++;  break;
     case SB_LINEUP : ybase--;  break;
     case SB_PAGEDOWN : ybase += long(height / cheight - 1);  break;
     case SB_PAGEUP : ybase -= long(height / cheight - 1);  break;
     case SB_THUMBTRACK	:
     case SB_THUMBPOSITION	:  ybase = long(double(pos - start) / double(end - start) * (cbp->getnlines() - height / 18));  break;
     case SB_TOP : ybase = 0;  break;
    }
  if (ybase <= 0)
    ybase = 0;
  if (code != SB_THUMBTRACK)
    {  if (cbp->getnlines() - height / cheight > 0)
         SetScrollPos(gethwnd(),SB_VERT,long(start + double(end - start) * ybase / (cbp->getnlines() - height / 18)),TRUE);
       else
         SetScrollPos(gethwnd(),SB_VERT,0,TRUE);
    }
  if (code < 1000)
    paintevent();
}
#endif

void MainWindow::deletefloatingmenu(MenuWindow *menu)
{ if (menu != 0 && menu == floatingmenu1)
    {  options.clear(0);  options.set(1);  floatingmenu1 = NULL;
    }
  else if (menu != 0 && menu == floatingmenu2)
    {  options.clear(2);  options.set(3);  floatingmenu2 = NULL;
    }
  else if (menu != 0 && menu == floatingmenu3)
    {  options.clear(4);  options.set(5);  floatingmenu3 = NULL;
    }
  else if (menu != 0 && menu == floatingmenu4)
    {  options.clear(6);  options.set(7);  floatingmenu4 = NULL;
    }
  updatemenu();
}

void MenuWindow::paintevent(void)
{RECT rect;
 RCHBRUSH hbrush;
  //qDebug() << "Entering : MenuWindow::paintEvent ";

  rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
  hbrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);
  //Window::paintevent();
}

void SubWindow::paintevent(void)
{RECT rect;
 RCHBRUSH hbrush;
//qDebug() << "Entering : SubWindow::paintEvent ";

  rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
  hbrush = CreateSolidBrush(getbackground());
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);
  //Window::paintevent();
}

SubWindow::SubWindow(RCCHAR *name,RCHWND hwnd,RCDWORD style,int x,int y,int w,int h) :
        Window(name,hwnd,style,x,y,w,h,SW_SHOW)
{
    clickcommandid = v.getinteger("wn::clickcommandid");
}

void SubWindow::setclickcommandid(int id)
{ clickcommandid = id;
  v.setinteger("wn::clickcommandid",id);
}

void SubWindow::lbuttondownevent(int,int,int)
{ NewCommandEvent *nevent = new NewCommandEvent(clickcommandid,0);
  state.sendevent(nevent);
}


PrintPreviewWindow::PrintPreviewWindow(RCCHAR *name,RCHWND hwnd,RCDWORD style,int x,int y,int w,int h) :
        Window(name,hwnd,style,x,y,w,h,SW_SHOW)
{
  double xscale,yscale,xoffset,yoffset;

  buttonmenus.match("PrintPreview")->move(this,2,2,4);
  addcontrol(buttonmenus.match("PrintPreview"));
  view3dwindow = 0;
  view2dwindow = 0;

  xscale = (width - 50) / printer.getphyswidth();
  yscale = (height - 75) / printer.getphysheight() * v.getreal("vw::aspectadjustment");
  if (xscale < yscale)
    xscale = yscale / v.getreal("vw::aspectadjustment");
  xoffset = (width - xscale * printer.getphyswidth()) / 2.0;
  yoffset = (height - yscale * printer.getphysheight()) / 2.0;

  paperhwnd = CreateWindow(_RCT("CadSubWindow"),_RCT("PrintPreview"),WS_CHILD | WS_VISIBLE ,int(xoffset),int(yoffset),int(xscale * printer.getphyswidth()),int(yscale * printer.getphysheight()),
                            gethwnd(),0,program->gethinst(),0);
  paperhdc = GetDC(paperhwnd);
}

PrintPreviewWindow::~PrintPreviewWindow()
{ if (SetLight != 0)
    ReleaseTextures(paperhdc);
  ReleaseDC(paperhwnd,paperhdc);
  DestroyWindow(paperhwnd);
}

void PrintPreviewWindow::paintevent(void)
{RECT rect;
 double xscale,yscale,xoffset,yoffset,red,green,blue,um,vm;
 const int w = 3;
 int wwidth,wheight;

  rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
  FillRect(gethdc(),&rect,program->getbuttonfacebrush());

  if (v.getreal("vw::aspectadjustment") < 1.0)
    {  xscale = (width - 50) / printer.getphyswidth();
       yscale = (height - 75) / printer.getphysheight();
       if (yscale / v.getreal("vw::aspectadjustment") * printer.getphyswidth() > width - 50)
         {  yscale = xscale * v.getreal("vw::aspectadjustment");
         }
       else
         {  xscale = yscale / v.getreal("vw::aspectadjustment");
         }


    }
  else
    {  xscale = (width - 50) / printer.getphyswidth();
       yscale = (height - 75) / printer.getphysheight();

       if (xscale * v.getreal("vw::aspectadjustment") * printer.getphysheight() > height - 75)
         {  xscale = yscale / v.getreal("vw::aspectadjustment");
         }
       else
         {  yscale = xscale * v.getreal("vw::aspectadjustment");
         }

    }



  xoffset = (width - xscale * printer.getphyswidth()) / 2.0;
  yoffset = (height - yscale * printer.getphysheight()) / 2.0;

  rect.left = int(xoffset);  rect.top = int(yoffset);
  rect.right = int(xoffset + xscale * printer.getphyswidth());
  rect.bottom = int(yoffset + yscale * printer.getphysheight());
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));

  rect.left = int(xoffset - w);
  rect.top = int(yoffset - w);
  rect.right = int(xoffset + xscale * printer.getphyswidth() + w);
  rect.bottom = int(yoffset);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(LTGRAY_BRUSH));

  rect.left = int(xoffset - w);
  rect.top = int(yoffset - w);
  rect.right = int(xoffset);
  rect.bottom = int(yoffset + yscale * printer.getphysheight() + w);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(LTGRAY_BRUSH));

  rect.left = int(xoffset + xscale * printer.getphyswidth());  rect.top = int(yoffset - w);
  rect.right = int(xoffset + xscale * printer.getphyswidth() + w);  rect.bottom = int(yoffset + yscale * printer.getphysheight() + w);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(LTGRAY_BRUSH));

  rect.left = int(xoffset - 3);  rect.top = int(yoffset + yscale * printer.getphysheight());
  rect.right = int(xoffset + xscale * printer.getphyswidth() + 3);  rect.bottom = int(yoffset + yscale * printer.getphysheight() + w);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(LTGRAY_BRUSH));

  rect.left = int(xoffset + xscale * printer.getphyswidth() + w);  rect.top = int(yoffset);
  rect.right = int(xoffset + xscale * printer.getphyswidth() + w * 2);  rect.bottom = int(yoffset + yscale * printer.getphysheight() + w * 2);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));

  rect.left = int(xoffset);  rect.top = int(yoffset + yscale * printer.getphysheight() + w);
  rect.right = int(xoffset + xscale * printer.getphyswidth() + w*2);  rect.bottom = int(yoffset + yscale * printer.getphysheight() + w*2);
  FillRect(gethdc(),&rect,(RCHBRUSH)GetStockObject(BLACK_BRUSH));


  wwidth = int(xscale * printer.getphyswidth());
  wheight = int(yscale * printer.getphysheight());
  //  Adjust the area so it fits into the physical area
  um = (umax + umin) / 2.0;
  umin = um - wwidth * printscale / xscale / 2.0;
  umax = um + wwidth * printscale / xscale / 2.0;

  vm = (vmax + vmin) / 2.0;
  vmin = vm - wheight * printscale / yscale / 2.0;
  vmax = vm + wheight * printscale / yscale / 2.0;

  MoveWindow(paperhwnd,int(xoffset),int(yoffset),wwidth,wheight,0);
  rect.left = 0;
  rect.top = 0;
  rect.right = wwidth;
  rect.bottom = wheight;
  FillRect(paperhdc,&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));

  red = v.getreal("wn::backgroundcolour.red");
  green = v.getreal("wn::backgroundcolour.green");
  blue = v.getreal("wn::backgroundcolour.blue");
  v.setreal("wn::backgroundcolour.red",1.0);
  v.setreal("wn::backgroundcolour.green",1.0);
  v.setreal("wn::backgroundcolour.blue",1.0);

  if (view3dwindow != 0)
    {  View3dOutput output(paperhwnd,paperhdc,view3dwindow->getview(),printscale);
       output.Surface::sizeevent(wwidth,wheight);
       output.setplotscale(view3dwindow->getplotscale());
       output.setprinterhdc(printer.gethdc());
       output.zoom(umin,vmin,umax,vmax);
       output.paintevent();
    }
  else
    {  View *top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
       View2dOutput output(paperhwnd,paperhdc,view2dwindow->getview2d(),top);
       output.Surface::sizeevent((umax-umin)/printscale*xscale,(vmax-vmin)/printscale*yscale);
       output.setplotscale(1.0);
       output.setprinterhdc(printer.gethdc());
       output.zoom(umin,vmin,umax,vmax);
       output.paintevent();
    }


  v.setreal("wn::backgroundcolour.red",red);
  v.setreal("wn::backgroundcolour.green",green);
  v.setreal("wn::backgroundcolour.blue",blue);

  SelectClipRgn(gethdc(),0);
  SelectClipRgn(paperhdc,0);

  Window::paintevent();
}

void PrintPreviewWindow::print(void)
{int i,nbands,w,h,pw,ph,bandheight,drawwp;
 double red,green,blue,umin1,vmin1,umax1,vmax1,umax2,vmin2;

  red = v.getreal("wn::backgroundcolour.red");
  green = v.getreal("wn::backgroundcolour.green");
  blue = v.getreal("wn::backgroundcolour.blue");
  v.setreal("wn::backgroundcolour.red",1.0);
  v.setreal("wn::backgroundcolour.green",1.0);
  v.setreal("wn::backgroundcolour.blue",1.0);

  printer.firstpage();
  if (view3dwindow != 0)
    {  if (view3dwindow->requiresopengl())
         {  pw = GetDeviceCaps(printer.gethdc(),HORZRES);
            ph = GetDeviceCaps(printer.gethdc(),VERTRES);

            umax2 = umin + GetDeviceCaps(printer.gethdc(),HORZSIZE) * printscale;
            vmin2 = vmax - GetDeviceCaps(printer.gethdc(),VERTSIZE) * printscale;
            w = int((umax2-umin)/printscale*printer.getxdotspermm());
            h = int((vmax-vmin2)/printscale*printer.getydotspermm());

            umin1 = umin + (umax2 - umin) / w * GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX);
            umax1 = umin + (umax2 - umin) / w * (GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX) + pw);
            vmin1 = vmin2 + (vmax - vmin2) / h * GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY);
            vmax1 = vmin2 + (vmax - vmin2) / h * (GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY) + ph);

            nbands = (pw * ph * 3 + 9999999) / 10000000;

            bandheight = ph / nbands;

            View3dOffScreenBitmap bitmap(view3dwindow,pw,bandheight + 60);
            bitmap.setxdotspermm(printer.getxdotspermm());
            bitmap.setydotspermm(printer.getydotspermm());
            bitmap.setbackground(RGB(255,255,255));
            bitmap.setplotscale(view3dwindow->getplotscale());
            bitmap.setblackandwhite(printer.getblackandwhite());

            drawwp = v.getinteger("wp::display");
            v.setinteger("wp::display",0);

            for (i = 0 ; i < nbands ; i++)
              {  bitmap.zoom(umin1,vmin1 - (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-1-i)/nbands,
                             umax1,vmin1 + (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-i)/nbands);
                 bitmap.paintevent();
                 BitBlt(printer.gethdc(),0,i*bandheight,pw,bandheight,bitmap.gethdc(),0,30,SRCCOPY);

              }

            ReleaseTextures(bitmap.gethdc());


            v.setinteger("wp::display",drawwp);

         }
       else
         {  View3dOutput output(0,printer.gethdc(),view3dwindow->getview(),printscale);
            output.Surface::sizeevent((umax-umin)/printscale*printer.getxdotspermm(),(vmax-vmin)/printscale*printer.getydotspermm());
            output.setplotscale(view3dwindow->getplotscale());
            output.zoom(umin,vmin,umax,vmax);
            output.setblackandwhite(printer.getblackandwhite());
            output.settransform2d(1.0,0.0,-GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX),0.0,-1.0,output.getheight()-1-GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY));
            output.paintevent();
         }
    }
  else
    {  if (view2dwindow->requiresopengl())
         {
            pw = GetDeviceCaps(printer.gethdc(),HORZRES);
            ph = GetDeviceCaps(printer.gethdc(),VERTRES);

            w = int((umax-umin)/printscale*printer.getxdotspermm());
            h = int((vmax-vmin)/printscale*printer.getydotspermm());

            umin1 = umin + (umax - umin) / w * GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX);
            umax1 = umin + (umax - umin) / w * (GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX) + pw);
            vmin1 = vmin + (vmax - vmin) / h * GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY);
            vmax1 = vmin + (vmax - vmin) / h * (GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY) + ph);

            nbands = (pw * ph * 3 + 9999999) / 10000000;

            bandheight = ph / nbands;
            View2dOffScreenBitmap bitmap(view2dwindow,view2dwindow->getview2d(),pw,bandheight+60);
            bitmap.setxdotspermm(printer.getxdotspermm());
            bitmap.setydotspermm(printer.getydotspermm());
            bitmap.setbackground(RGB(255,255,255));
            bitmap.setplotscale(1.0);
            bitmap.setblackandwhite(printer.getblackandwhite());

            for (i = 0 ; i < nbands ; i++)
              {  bitmap.zoom(umin1,vmin1 - (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-1-i)/nbands,
                             umax1,vmin1 + (vmax1 - vmin1) / ph * 30 + (vmax1-vmin1)*(nbands-i)/nbands);
                 bitmap.setbandoffset(i * bandheight);
                 RECT rect;
                 rect.left = rect.top = 0;
                 rect.right = w;  rect.bottom = bandheight;
                 FillRect(bitmap.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
                 bitmap.paintevent();
                 BitBlt(printer.gethdc(),0,i*bandheight,pw,bandheight,bitmap.gethdc(),0,30,SRCCOPY);
              }

            ReleaseTextures(bitmap.gethdc());


         }
       else
         {  View *top = new View(view2dwindow->getview2d()->getname(),0,view2dwindow->getview()->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
            View2dOutput output(0,printer.gethdc(),view2dwindow->getview2d(),top);
            output.Surface::sizeevent((umax-umin)/printscale*printer.getxdotspermm(),(vmax-vmin)/printscale*printer.getydotspermm());
            output.setplotscale(1.0);
            output.zoom(umin,vmin,umax,vmax);
            output.setblackandwhite(printer.getblackandwhite());
            output.settransform2d(1.0,0.0,-GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETX),0.0,-1.0,output.getheight()-1-GetDeviceCaps(printer.gethdc(),PHYSICALOFFSETY));
            output.paintevent();
         }
    }
  printer.lastpage();

  v.setreal("wn::backgroundcolour.red",red);
  v.setreal("wn::backgroundcolour.green",green);
  v.setreal("wn::backgroundcolour.blue",blue);

}

void PrintPreviewWindow::lbuttondownevent(int xp,int yp,int buttons)
{ mousemoveevent(xp,yp,buttons);
}

void PrintPreviewWindow::setview3dwindow(View3dWindow *w)
{ view3dwindow = w;
  printscale = (w->getumax() - w->getumin()) / w->getwidthmm();
  umin = view3dwindow->getumin();
  vmin = view3dwindow->getvmin();
  umax = view3dwindow->getumax();
  vmax = view3dwindow->getvmax();
  printer.getdefault(umax - umin < vmax - vmin);
}

void PrintPreviewWindow::setzoom(double umin1,double vmin1,double umax1,double vmax1)
{ umin = umin1;  vmin = vmin1;  umax = umax1;  vmax = vmax1;
  printer.getdefault(umax - umin < vmax - vmin);
}

void PrintPreviewWindow::setview2dwindow(View2dWindow *w)
{ view2dwindow = w;
  printscale = 1;
  umin = 0.0;
  vmin = 0.0;
  umax = w->getview2d()->getpaperwidth();
  vmax = w->getview2d()->getpaperheight();
  printer.getdefault(umax - umin < vmax - vmin);
}

void Window::mbuttonupevent(int,int,int)
{int commandid;
  if ((commandid = state.getlastcommandid()) >= 0)
    {  NewCommandEvent *nevent = new NewCommandEvent(commandid,0,1);
       state.sendevent(nevent);
    }
}

#ifdef USING_WIDGETS
void MainWindow::buttonmenu(int m,ButtonMenu *menu, ButtonMenu *mainMenu)
{
    // KMJ : this is where a button menus are placed on the main window and drawn
    // we use a widget so that object does its own drawing
    // we need to create and position the button box on the main window

    ButtonMenu *oldmenu=0;
    Window *parent=0;
    int x,y,extraheight;
    Control *c=0;

     extraheight = 0;
     switch (m)
     {
         case BM_MAINCOMMAND :
            oldmenu = maincmenu;
            maincmenu = menu;
            parent = menu != 0 ? menu->parent : cadwindow;
            x = maincmenux;
            y = maincmenuy;
            if (menu != 0)
              menu->setshowbutton(27);

            // Qt toolbox stuff
            if(menu != 0)
            {
                // add the buttons that belong to this button menu to the tool box
                // the sId is 0 for this because it is the main command section of the tool box
                RCToolBox *rctb = new RCToolBox(RCToolBox::hLine,menu->getncolumns(),menu->getnusedbuttons(),0,(QtMainWindow*)this->gethwnd());
                rctb->setTitle("Entities");
                rctb->setRole("MainMenu");
                rctb->setObjectName("MainMenu");
                menu->setToolbox(rctb,0);
                // now add it to the main window
                ((QtMainWindow*)gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                // connect slot to handle this dockwidgets visibily changes
                //rctb->connect(rctb,SIGNAL(visibilityChanged(bool)),
                //              (QtMainWindow*)gethwnd(),
                //              SLOT(onToolBarVisibilyChanged(bool)));
            }

            break;
          case BM_SUBCOMMAND :
            oldmenu = subcmenu;
            subcmenu = menu;
            parent = maincmenu != 0 ? maincmenu->parent : cadwindow;
            x = subcmenux;
            y = subcmenuy;
            if (menu != 0)
              menu->setshowbutton(14);

            // Qt toolbar stuff
            if(menu != 0)
            {
                // this is a subcommand menu so the dock widget already exists
                // we need to use the main menu argument as the menu
                // the sId is 1 for this because it is the main command section of the tool box
                if(maincmenu->getQW() != 0)
                { // if the menus widget is 0 there is something wrong
                    // add the buttons that belong to this button menu to the tool box
                    //RCToolBox *rctb = new RCToolBox((QtMainWindow*)this->gethwnd());
                    menu->setToolbox((RCToolBox*)maincmenu->getQW(),1,maincmenu->getnusedbuttons());
                    //menu->debug();
                    // now add it to the main window
                    if( ! maincmenu->getQW()->isVisible() )
                        ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                    //menu->debug();
                }
            }

            break;
          case BM_MAININTERRUPT :
            oldmenu = mainimenu;
            mainimenu = menu;
            parent = menu != 0 ? menu->parent : cadwindow;
            x = mainimenux;
            y = mainimenuy;

            // Qt toolbar stuff
            if(menu != 0)
            {
                 RCToolBox *rctb = new RCToolBox(RCToolBox::hLine,menu->getncolumns(),menu->getnusedbuttons(),0,(QtMainWindow*)this->gethwnd());
                 rctb->setTitle("Assistant");
                 rctb->setRole("MainInterrupt");
                 rctb->setObjectName("MainInterrupt");
                 menu->setToolbox(rctb,0);
                // now add it to the main window
                ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                 // connect slot to handle this dockwidgets visibily changes
                 //rctb->connect(rctb,SIGNAL(visibilityChanged(bool)),
                 //              (QtMainWindow*)gethwnd(),
                 //              SLOT(onToolBarVisibilyChanged(bool)));
            }

            break;
          case BM_SUBINTERRUPT :
            oldmenu = subimenu;
            subimenu = menu;
            parent = mainimenu != 0 ? mainimenu->parent : cadwindow;
            x = subimenux;
            y = subimenuy;

            // Qt toolbar stuff
            if(menu != 0)
            {
                 if(mainimenu->getQW() != 0)
                { // if the menus widget is 0 there is something wrong
                    // add the buttons that belong to this button menu to the tool box
                    //RCToolBox *rctb = new RCToolBox((QtMainWindow*)this->gethwnd());
                    menu->setToolbox((RCToolBox*)mainimenu->getQW(),1,mainimenu->getncolumns());
                    // now add it to the main window
                    if( ! mainimenu->getQW()->isVisible() )
                        ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                }
            }

            break;
          case BM_MAINAPPLICATION :
            oldmenu = mainamenu;
            mainamenu = menu;
            parent = menu != 0 ? menu->parent : cadwindow;
            x = mainamenux;
            y = mainamenuy;
            extraheight = 25;
            if (menu != 0)
              menu->setshowbutton(27);

            // Qt toolbar stuff
            if(menu != 0)
            {
                RCToolBox *rctb = new RCToolBox(RCToolBox::hLine,menu->getncolumns(),menu->getnusedbuttons(),0,(QtMainWindow*)this->gethwnd());
                rctb->setTitle("");
                rctb->setRole("ApplicationMain");
                menu->setToolbox(rctb,0);
                // now add it to the main window
                ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
            }

            break;
          case BM_SUBAPPLICATION :
            oldmenu = subamenu;
            subamenu = menu;
            parent = mainamenu != 0 ? mainamenu->parent : cadwindow;
            x = subamenux;
            y = subamenuy;
            if (menu != 0)
              menu->setshowbutton(14);

            // Qt toolbar stuff
            if(menu != 0)
            {
                if(menu != 0)
                {
                     if(mainamenu->getQW() != 0)
                    { // if the menus widget is 0 there is something wrong
                        // add the buttons that belong to this button menu to the tool box
                        //RCToolBox *rctb = new RCToolBox((QtMainWindow*)this->gethwnd());
                        menu->setToolbox((RCToolBox*)mainamenu->getQW(),1,mainamenu->getnusedbuttons());
                        // now add it to the main window
                        if( ! mainamenu->getQW()->isVisible() )
                            ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                    }
                }
            }

            break;
          case BM_QUICK :
            oldmenu = quickmenu;
            quickmenu = menu;
            parent = oldmenu != 0 ? oldmenu->parent : cadwindow;
            x = quickmenux;
            y = quickmenuy;

            // Qt toolbar stuff
            if(menu != 0)
            {
                if(menu != 0)
                {
                    // add the buttons that belong to this button menu to the tool box
                    // the sId is 0 for this bescause it is the main command section of the tool box
                    //RCToolBox *rctb = new RCToolBox((QtMainWindow*)this->gethwnd());
                    RCToolBox *rctb = new RCToolBox(RCToolBox::hLine,menu->getncolumns(),menu->getnusedbuttons(),0,(QtMainWindow*)this->gethwnd());
                    rctb->setTitle("Quick");
                    rctb->setRole("Quick");
                    rctb->setObjectName("Quick");
                    menu->setToolbox(rctb,0);
                    // now add it to the main window
                    ((QtMainWindow*)this->gethwnd())->addDockWidget(Qt::TopDockWidgetArea,(QDockWidget*)menu->getQW());
                    // connect slot to handle this dockwidgets visibily changes
                    //rctb->connect(rctb,SIGNAL(visibilityChanged(bool)),
                    //              (QtMainWindow*)gethwnd(),
                    //              SLOT(onToolBarVisibilyChanged(bool)));
                }
            }

            break;
     }

     if (menu != 0 && (m == BM_MAINAPPLICATION || m == BM_SUBAPPLICATION))
       {  if (m == BM_MAINAPPLICATION && subamenu != 0 && subamenu->parent != 0)
            menu->move(subamenu->parent,x,y,-1);
          else if (m == BM_MAINAPPLICATION && oldmenu != 0 && oldmenu->parent != 0)
            menu->move(oldmenu->parent,x,y,-1);
          else if (m == BM_SUBAPPLICATION && mainamenu != 0 && mainamenu->parent != 0)
            menu->move(mainamenu->parent,x,y,-1);
          else if (m == BM_SUBAPPLICATION && oldmenu != 0 && oldmenu->parent != 0)
            menu->move(oldmenu->parent,x,y,-1);
          else
            {MenuWindow *mw;
             int i,y1,y2;

               y1 = y + menu->h + MenuBorderOffset;
               y2 = y - MenuBorderOffset;
               if (extraheight > 0)
                 y1 += extraheight;
               if (extraheight < 0)
                 y2 += extraheight;

               //  Make sure it doesn't go over an existing control.
               for (i = 0 ; (c = cadwindow->getcontrol(i)) != 0 ; i++)
                 if ((c != menu &&  //  Don't match itself
                       (c->x > x + menu->w || c->x + c->w < x || c->y > y + menu->h || c->y + c->h < y) &&    //  Don't match children controls
                       (c->x != x || c->y != y + extraheight)) &&
                       !(x + menu->w + MenuBorderOffset < c->x ||
                         x - MenuBorderOffset - MenuHandleOffset > c->x + c->w ||
                         y1 < c->y || y2 > c->y + c->h))
                   break;

               if (c == 0 && x != -10000)
                 {  //  Put it on the main window
                    menu->move(cadwindow,x,y,-1);
                 }
               else
                 {  //  Create a window for this menu.
                    mw = new MenuWindow(cadwindow->getsubwindow(),500,100,menu->w,menu->h + 25);
                    menu->parent = 0;
                    if (m == BM_MAINAPPLICATION)
                      menu->move(mw,0,0,-1);
                    else
                      menu->move(mw,0,25,-1);
                    mainamenux = 0;
                    mainamenuy = 0;
                    subamenux = 0;
                    subamenuy = 25;
                 }

            }
       }
     else if (menu != 0 && parent != 0)
       menu->move(parent,x,y,-1);
     else if (menu != 0 && menu != oldmenu && oldmenu != 0)
       menu->move(oldmenu->parent == 0 ? cadwindow : oldmenu->parent,x,y,-1);

     if (oldmenu != 0 && oldmenu->parent != 0 && oldmenu != menu)
       {  oldmenu->parent->delcontrol(oldmenu);
          oldmenu->parent = 0;
       }

     if (menu != 0)
       menu->draw(DM_NORMAL);
}
#else
void MainWindow::buttonmenu(int m,ButtonMenu *menu)
{
 ButtonMenu *oldmenu=0;
 Window *parent=0;
 int x,y,extraheight;
 Control *c=0;

  extraheight = 0;
  switch (m)
    {  case BM_MAINCOMMAND :
         oldmenu = maincmenu;
         maincmenu = menu;
         parent = menu != 0 ? menu->parent : cadwindow;
         x = maincmenux;
         y = maincmenuy;
         if (menu != 0)
           menu->setshowbutton(27);
         break;
       case BM_SUBCOMMAND :
         oldmenu = subcmenu;
         subcmenu = menu;
         parent = maincmenu != 0 ? maincmenu->parent : cadwindow;
         x = subcmenux;
         y = subcmenuy;
         if (menu != 0)
           menu->setshowbutton(14);
         break;
       case BM_MAININTERRUPT :
         oldmenu = mainimenu;
         mainimenu = menu;
         parent = menu != 0 ? menu->parent : cadwindow;
         x = mainimenux;
         y = mainimenuy;
         break;
       case BM_SUBINTERRUPT :
         oldmenu = subimenu;
         subimenu = menu;
         parent = mainimenu != 0 ? mainimenu->parent : cadwindow;
         x = subimenux;
         y = subimenuy;
         break;
       case BM_MAINAPPLICATION :
         oldmenu = mainamenu;
         mainamenu = menu;
         parent = menu != 0 ? menu->parent : cadwindow;
         x = mainamenux;
         y = mainamenuy;
         extraheight = 25;
         if (menu != 0)
           menu->setshowbutton(27);
         break;
       case BM_SUBAPPLICATION :
         oldmenu = subamenu;
         subamenu = menu;
         parent = mainamenu != 0 ? mainamenu->parent : cadwindow;
         x = subamenux;
         y = subamenuy;
         if (menu != 0)
           menu->setshowbutton(14);
         break;
       case BM_QUICK :
         oldmenu = quickmenu;
         quickmenu = menu;
         parent = oldmenu != 0 ? oldmenu->parent : cadwindow;
         x = quickmenux;
         y = quickmenuy;
         break;
    }


  if (menu != 0 && (m == BM_MAINAPPLICATION || m == BM_SUBAPPLICATION))
    {  if (m == BM_MAINAPPLICATION && subamenu != 0 && subamenu->parent != 0)
         menu->move(subamenu->parent,x,y,-1);
       else if (m == BM_MAINAPPLICATION && oldmenu != 0 && oldmenu->parent != 0)
         menu->move(oldmenu->parent,x,y,-1);
       else if (m == BM_SUBAPPLICATION && mainamenu != 0 && mainamenu->parent != 0)
         menu->move(mainamenu->parent,x,y,-1);
       else if (m == BM_SUBAPPLICATION && oldmenu != 0 && oldmenu->parent != 0)
         menu->move(oldmenu->parent,x,y,-1);
       else
         {MenuWindow *mw;
          int i,y1,y2;

            y1 = y + menu->h + MenuBorderOffset;
            y2 = y - MenuBorderOffset;
            if (extraheight > 0)
              y1 += extraheight;
            if (extraheight < 0)
              y2 += extraheight;

            //  Make sure it doesn't go over an existing control.
            for (i = 0 ; (c = cadwindow->getcontrol(i)) != 0 ; i++)
              if ((c != menu &&  //  Don't match itself
                    (c->x > x + menu->w || c->x + c->w < x || c->y > y + menu->h || c->y + c->h < y) &&    //  Don't match children controls
                    (c->x != x || c->y != y + extraheight)) &&
                    !(x + menu->w + MenuBorderOffset < c->x ||
                      x - MenuBorderOffset - MenuHandleOffset > c->x + c->w ||
                      y1 < c->y || y2 > c->y + c->h))
                break;

            if (c == 0 && x != -10000)
              {  //  Put it on the main window
                 menu->move(cadwindow,x,y,-1);
              }
            else
              {  //  Create a window for this menu.
                 mw = new MenuWindow(cadwindow->getsubwindow(),500,100,menu->w,menu->h + 25);
                 menu->parent = 0;
                 if (m == BM_MAINAPPLICATION)
                   menu->move(mw,0,0,-1);
                 else
                   menu->move(mw,0,25,-1);
                 mainamenux = 0;
                 mainamenuy = 0;
                 subamenux = 0;
                 subamenuy = 25;
              }

         }
    }
  else if (menu != 0 && parent != 0)
    menu->move(parent,x,y,-1);
  else if (menu != 0 && menu != oldmenu && oldmenu != 0)
    menu->move(oldmenu->parent == 0 ? cadwindow : oldmenu->parent,x,y,-1);

  if (oldmenu != 0 && oldmenu->parent != 0 && oldmenu != menu)
    {  oldmenu->parent->delcontrol(oldmenu);
       oldmenu->parent = 0;
    }

  if (menu != 0)
    menu->draw(DM_NORMAL);
}
#endif

void MainWindow::rubberband3d(Point p)
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->rubberband3d(p);
}

void MainWindow::rubberbanduv(Point p)
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->rubberbanduv(p);
}

void MainWindow::cursoroff()
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->cursoroff();
}

void MainWindow::cursoron()
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->cursoron();
}

void MainWindow::paintall(int resetdefaultmode,int offscreenpaint)
{
#ifdef USING_WIDGETS
  View3dWindow *v;
 RCCOLORREF cr;
 //WINDOWPLACEMENT wp;

    // KMJ: testing for gl display list use
    if(!state.dragging())
        regenGLlists();
    //

    cr = RGB(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
             ::v.getreal("wn::backgroundcolour.green")*255.0+0.5,
             ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5);

    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    {
        if ((offscreenpaint == 3 || offscreenpaint == 2) && v == curview3d)
            continue;

        // KMJ: don't repaint when raytracing a view
        if(v->getRaytracing() == 1)
            continue;

        if((RCView*)v->gethwnd() != 0)
            ((RCView*)v->gethwnd())->unFreezeBackImage();

        // KMJ : keep an eye on this
        // we want to force a scene repaint here
        // cleanup any view border graphics
        if (dynamic_cast<View2dWindow *>(v) != 0)
            ((View2dWindow*)v)->clearScene();

        v->clearScene();

        db.clearScene(DM_ERASE,v);

        if (resetdefaultmode)
            v->setrepaintstyle(Wireframe);
        v->setbackground(cr);
        v->setoffscreenpaint(offscreenpaint != 0);

        v->paintevent();
    }

    // KMJ : TODO : apply this to the tabbed widget??
    //viewtab->update_window_state();

#else
    View3dWindow *v;
 RCCOLORREF cr;
 WINDOWPLACEMENT wp;

    cr = RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
             ::v.getreal("wn::backgroundcolour.green")*255.0,
             ::v.getreal("wn::backgroundcolour.blue")*255.0);
    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    {   if ((offscreenpaint == 3 || offscreenpaint == 2) && v == curview3d)
            continue;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(v->gethwnd(),&wp);
        if (resetdefaultmode)
            v->setrepaintstyle(Wireframe);
        v->setbackground(cr);
        v->setoffscreenpaint(offscreenpaint != 0);
        if (::v.getinteger("wn::scrollbars") != 0)
        {   SetWindowLong(v->gethwnd(),GWL_STYLE,(LONG)((WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_HSCROLL   | WS_VSCROLL)&~WS_VISIBLE));
            SetScrollRange(v->gethwnd(),SB_HORZ,0,1000,TRUE);
            SetScrollRange(v->gethwnd(),SB_VERT,0,1000,TRUE);
        }
        else
            SetWindowLong(v->gethwnd(),GWL_STYLE,(LONG)(WS_CHILD | WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS));
        SetWindowPlacement(v->gethwnd(),&wp);  //  Reset the window placement - changing the above attributes clears these
        if (wp.showCmd == SW_SHOWMAXIMIZED)
            ShowWindow(v->gethwnd(),SW_SHOWMAXIMIZED);
    }

    viewtab->update_window_state();
#endif
}

void MainWindow::setdirtyall(Point pmin,Point pmax)
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->setdirty(pmin,pmax);
}

void MainWindow::cleardirtyall(void)
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->cleardirty();
}

class RCSplitterItem;

class RCSplitterItemList : public GeneralList
{
public:
     RCSplitterItem *next(void) {  return (RCSplitterItem *) GeneralList::next();  }
};

class RCSplitterItem
{
public:
    int mType; // 0 = splitter widget; 1 = view widget
    int mIndex; // widget index
public:
    RCSplitterItem()
    {
        mType = 0;
        mIndex = -1;
    }
    virtual int save(CadFile *outfile) {return 0;}
    virtual int load(CadFile *outfile) {return 0;}
    virtual void debug() {;}
};

class RCSplitterView : public RCSplitterItem
{
public:
    int mViewIndex;// the index of a view in the window3d list
public:
    RCSplitterView()
    {
        mViewIndex = -1;
    }
    int save(CadFile *outfile)
    {
        short x;
        x = short(mType);
        if(!outfile->write(&x)) return 0;
        x = short(mIndex);
        if(!outfile->write(&x)) return 0;
        x = short(mViewIndex);
        if(!outfile->write(&x)) return 0;
        return 1;
    }
    int load(CadFile *infile)
    {
        short x;
        if(!infile->read(&x)) return 0;
        mType = x;
        if(!infile->read(&x)) return 0;
        mIndex = x;
        if(!infile->read(&x)) return 0;
        mViewIndex = x;
        return 1;
    }
    void debug()
    {
        qDebug() << "mType : " << mType;
        qDebug() << "mIndex : " << mIndex;
        qDebug() << "mViewIndex : " << mViewIndex;
    }
};

class RCSplitter : public RCSplitterItem
{
public:
    int mOrientation; // 0 = horizontal; 1 = vertical
    RCSplitterItemList mChildList;
    QList<int> mSizes;
public:
    RCSplitter()
    {
        mOrientation = 0;
    }
    int save(CadFile *outfile)
    {
        short x;
        x = (short)mType;
        if(!outfile->write(&x)) return 0;
        x = short(mIndex);
        if(!outfile->write(&x)) return 0;
        x = short(mOrientation);
        if(!outfile->write(&x)) return 0;
        x = short(mChildList.length());
        if(!outfile->write(&x)) return 0;
        RCSplitterItem *sitem;
        for(mChildList.start(); (sitem = mChildList.next()) != NULL; )
        {
            // need to save the type marker before the child record
            // so we know which object type to load
            x = (short)sitem->mType;
            if(!outfile->write(&x)) return 0;
            if(!sitem->save(outfile))
                return 0;
        }
        x = mSizes.size();
        if(!outfile->write(&x)) return 0;
        for(int i=0; i<mSizes.size(); i++)
        {
            x = mSizes.at(i);
            if(!outfile->write(&x)) return 0;
        }
        return 1;
    }
    int load(CadFile *infile)
    {
        short x;
        if(!infile->read(&x)) return 0;
        mType = x;
        if(!infile->read(&x)) return 0;
        mIndex = x;
        if(!infile->read(&x)) return 0;
        mOrientation = x;
        if(!infile->read(&x)) return 0;
        int n = x;
        for(int i=0; i<n; i++)
        {
            //RCSplitterItem *sitem = new RCSplitterItem();
            if(!infile->read(&x)) return 0;
            if(x == 0)
            {
                RCSplitter *sitem = new RCSplitter();
                if(sitem == 0 || !sitem->load(infile))
                {
                    delete sitem;
                    return 0;
                }
                mChildList.add(sitem);
            }
            else if(x == 1)
            {
                RCSplitterView *sitem = new RCSplitterView();
                if(sitem == 0 || !sitem->load(infile))
                {
                    delete sitem;
                    return 0;
                }
                mChildList.add(sitem);
            }
        }
        mChildList.reverse();
        if(!infile->read(&x)) return 0;
        n = x;
        for(int i=0; i<n; i++)
        {
            if(!infile->read(&x)) return 0;
            mSizes.push_back((int)x);
        }
        return 1;
    }
    void debug()
    {
        qDebug() << "== Splitter Item Start ==";
        qDebug() << "mType : " << mType;
        qDebug() << "mIndex : " << mIndex;
        qDebug() << "mOrientation : " << mOrientation;
        qDebug() << "Child List start";
        RCSplitterItem *sitem;
        for (mChildList.start(); (sitem = mChildList.next()) != NULL; )
            sitem->debug();
        qDebug() << "Child List end";
        qDebug() << "sizes start";
        for(int i=0; i<mSizes.size(); i++)
            qDebug() << "size(" << i << ") : " << mSizes.at(i);
        qDebug() << "sizes end";
        qDebug() << "== Splitter Item End ==";
    }
};

void getSplitterData(const QSplitter *splitter, int index, RCSplitterItem **splitterItem)
{
    int i,j;
    View3dWindow *w;
    RCSplitter *thisSplitter = new RCSplitter();

    thisSplitter->mType = 0;
    thisSplitter->mIndex = index;
    thisSplitter->mOrientation = splitter->orientation() == Qt::Horizontal ? 0 : 1;
    *splitterItem = thisSplitter;
    thisSplitter->mChildList.start();
    for(i=0; i<splitter->count(); i++)
    {
        if(qobject_cast<RCView *>(splitter->widget(i)) != 0)
        {
            RCSplitterView *splitterView = new RCSplitterView();
            splitterView->mIndex = i;
            splitterView->mType = 1;
            for (cadwindow->GetView3dList()->start(),j=0 ; (w = cadwindow->GetView3dList()->next()) != NULL ;j++ )
            {
                if((RCView*)splitter->widget(i) == (RCView*)w->gethwnd())
                {
                    splitterView->mViewIndex = j;
                    break;
                }
            }
            thisSplitter->mChildList.add(splitterView);
        }
        else if(qobject_cast<QSplitter *>(splitter->widget(i)) != 0)
        {
            RCSplitter *childSplitter = 0;//new RCSplitter();
            getSplitterData((QSplitter*)splitter->widget(i),i,(RCSplitterItem**)&childSplitter);
            thisSplitter->mChildList.add(childSplitter);
        }
    }
    thisSplitter->mChildList.reverse();
    thisSplitter->mSizes = splitter->sizes();
}

void restoreSplitter(QSplitter **ppsplitter, RCSplitterItem *splitterItem)
{
    int j;
    View3dWindow *w;
    RCCentralWidget *cwidget = (RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget;
    QSplitter *splitter = *ppsplitter;

    if(splitter == 0)
        splitter = new QSplitter(((RCSplitter*)splitterItem)->mOrientation == 0 ? Qt::Horizontal : Qt::Vertical);
    if(splitter != 0)
    {
        qDebug() << "splitter orientation : " << splitter->orientation();
        splitter->setOrientation(((RCSplitter*)splitterItem)->mOrientation == 0 ? Qt::Horizontal : Qt::Vertical);
        qDebug() << "splitter orientation : " << splitter->orientation();
        RCSplitterItem *sitem=0;
        for(((RCSplitter*)splitterItem)->mChildList.start(); (sitem=((RCSplitter*)splitterItem)->mChildList.next()) != 0; )
        {
            if(sitem->mType == 1)
            {
                for (cadwindow->GetView3dList()->start(),j=0 ; (w = cadwindow->GetView3dList()->next()) != NULL ;j++ )
                {
                    if(j == ((RCSplitterView*)sitem)->mViewIndex)
                    {
                        RCView *thisView = (RCView*)w->gethwnd();
                        qDebug() << "view parent : " << thisView->parent();
                        cwidget->addView(splitter,thisView);
                        //cwidget->addView(splitter,thisView,0);
                        qDebug() << "view parent : " << thisView->parent();
                        break;
                    }
                }
            }
            else if(sitem->mType == 0)
            {
                QSplitter *newSplitter=0;
                restoreSplitter(&newSplitter,sitem);
                ((QSplitter*)splitter)->addWidget(newSplitter);
            }
        }
        qDebug() << "splitter count : " << ((QSplitter*)splitter)->count();
        qDebug() << "splitter sizes : " << ((RCSplitter*)splitterItem)->mSizes;
        splitter->setSizes(((RCSplitter*)splitterItem)->mSizes);
        splitter->setProperty("lastState",splitter->saveState());
        *ppsplitter = splitter;
    }
}

int MainWindow::save(CadFile *outfile)
{
  short type,n;
  int i,j;
  View3dWindowList list;
  View3dWindow *w,*tw;
#if 1
  RCCentralWidget *cwidget = (RCCentralWidget*)((QtMainWindow*)this->gethwnd())->centerWidget;
  QSplitter *rootSplitter = ((RCCentralWidget*)((QtMainWindow*)this->gethwnd())->centerWidget)->mSplitter;

  // get a list of the splitters currently displayed
  cwidget->purgeSplitters();
  int dcount=0;
  //cwidget->debugSplitter(rootSplitter,dcount);

  // build the splitter list from the top down
  // by convention view widgets are put in the list before splitter widgets
  // the root splitter is first in the list
  RCSplitter *sitem=0;
  //getSplitterData(rootSplitter, 0, splitterList);
  getSplitterData(rootSplitter, 0, (RCSplitterItem**)&sitem);

  qDebug() << "*** top item debug start ***";
  sitem->debug();
  qDebug() << "***  top item debug end  ***";

  // save this record type
  type = splitter_record;
  outfile->write(&type);

  // save the version number for this record
  // so we can have different versions
  short sversion = 1;
  if ( ! outfile->write(&sversion)) return 0;

  // save the splitter item data
  if( ! sitem->save(outfile)) return 0;

#if 0
  QList<QSplitter *> allSplitters = cwidget->findChildren<QSplitter *>();
  // make a list of the parent index of each splitter
  // the main (root) splitter has an index of -1 in this list
  QList<int> parents;
  for(int i=0; i<allSplitters.size(); i++)
  {
      QSplitter *thisSplitter = allSplitters.at(i);
      if(thisSplitter->count() > 0)
      {
        int index = allSplitters.indexOf((QSplitter*)thisSplitter->parentWidget());
         parents.push_back(index);
      }
  }
  // make a list of the sizes of each splitter
  QList< QList<int> *> allSizes;
  QList<int> orientations;
  for(int i=0; i<allSplitters.size(); i++)
  {
      QList<int> *sizes = new QList<int>;
       *sizes = allSplitters.at(i)->sizes();
      if(sizes->size() > 0)
      {
          allSizes.push_back(sizes);
          int orient = allSplitters.at(i)->orientation();
          orientations.push_back(orient);
      }
  }
  // find the view list index for all splitters
  QList<int> viewindex;
  for(int i=0; i<allSplitters.size(); i++)
  {
      for(int k=0; k<allSplitters.at(i)->count(); k++)
      {
          if(allSplitters.at(i)->count() > 0)
          {
              tw = ((RCView*)allSplitters.at(i)->widget(k))->getView3dWindow();
              for (view3dlist.start(),j=0 ; (w = view3dlist.next()) != 0 ;j++ )
              {
                  if (w == tw)
                  {
                      viewindex.push_back(j);
                      break;
                  }
              }
          }
      }
  }
  // save this record type
  type = splitter_record;
  outfile->write(&type);

  // save the splitter parent index list and their orientations
  n = short(parents.size());
  outfile->write(&n);
  for(int i=0; i<parents.size(); i++)
  {
      n = (short)parents.at(i);
      outfile->write(&n);
      n = (short)orientations.at(i);
      outfile->write(&n);
  }
  // save the sizes and orientation of each splitter in the same order
  n = short(allSizes.size());
  outfile->write(&n);
  for(int i=0; i<allSizes.size(); i++)
  {
      QList<int> *sizes = allSizes.at(i);
      n = (short)sizes->size();
      outfile->write(&n);
      for(int j=0; j<sizes->size(); j++)
      {
          n = (short)sizes->at(j);
          outfile->write(&n);
      }
  }
  // make the view list be in the same order as the windows
  n = short(view3dlist.length());
  list.start();
  for(int j=0; j<viewindex.size(); j++)
  {
      for (view3dlist.start(),i=0 ; (w = view3dlist.next()) != NULL ;i++ )
      {
          if(viewindex.at(j) == i)
          {
              list.add(w);
              break;
          }
      }
  }
#endif

  cwidget->debugView3dList();

  n = short(view3dlist.length());
  list = view3dlist.copy();
  //list.reverse();
  if (! outfile->write(&n)) return 0;
  for (list.start() ; (w = list.next()) != NULL ; )
    if (! w->save(outfile)) return 0;
  list.destroy();
  return 1;

#else


  type = windows_record;
  outfile->write(&type);
  n = short(view3dlist.length());
  list = view3dlist.copy();
  list.reverse();
  if (! outfile->write(&n)) return 0;
  for (list.start() ; (w = list.next()) != NULL ; )
    if (! w->save(outfile)) return 0;
  list.destroy();
  return 1;
#endif
}

int MainWindow::load(CadFile *infile,Database *)
{int i;
 short n,type;
 View3dWindow *w,*w0;

    // TO DO.
    if(infile->getversion() < 3)
    {
        // old version file needs to set up the splitters for old windows
    }

    if (! infile->read(&n))
        return 0;
    for (i = 0 ; i < n ; i++)
    {
        if (! infile->read(&type))
            return 0;
        if (type == View3dSurf)
        {
            if ((w = View3dWindow::load(infile)) == NULL)
                return 0;
        }
        else
        {
            if ((w = View2dWindow::load(infile)) == NULL)
                return 0;
        }
        if (cadwindow != 0 && cadwindow->view3dlist.add(w))
        {
            if (i == n-1)
                cadwindow->selectwindow(w);
        }
    }

    return 1;

}

int MainWindow::loadsplitter(CadFile *infile,Database *)
{
    int i;
    short n,type,x,y,sversion;
    View3dWindow *w,*w0;
    RCCentralWidget *cwidget = (RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget;
    QSplitter *rootSplitter = ((RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centerWidget)->mSplitter;

    if (! infile->read(&sversion))
        return NULL;
    if(sversion == 1)
    {
        RCSplitter *sitem = new RCSplitter();
        if(!sitem->load(infile)) return 0;

        qDebug() << "*** top item debug start ***";
        sitem->debug();
        qDebug() << "***  top item debug end  ***";

        cwidget->initialise(Qt::Horizontal);

        if (! infile->read(&n))
            return 0;
        for (i = 0 ; i < n ; i++)
        {
            if (! infile->read(&type))
                return 0;
            if (type == View3dSurf)
            {
                if ((w = View3dWindow::load(infile)) == NULL)
                    return 0;
            }
            else
            {
                if ((w = View2dWindow::load(infile)) == NULL)
                    return 0;
            }
            if (cadwindow != 0 && cadwindow->view3dlist.add(w))
            {
                //if (i == 0)
                //    w0 = w;
                if (i == n-1)
                    cadwindow->selectwindow(w);
            }
        }

        cwidget->debugView3dList();

        QSplitter *newSplitter=0;
        restoreSplitter(&rootSplitter, sitem);
        rootSplitter->restoreState(rootSplitter->property("lastState").toByteArray());
        //if(cwidget->isTiled())
        //    cwidget->tiled = 1;
        //else
        //    cwidget->tiled = 0;
        rootSplitter->show();
        //cwidget->isTiled();
        qDebug() << "current view : " << QString(w->getview()->getname());
        w = cadwindow->getcurrentwindow();

        if(cwidget->isTiled() == 1)
            cwidget->normalizeView((RCView*)w->gethwnd());
        else
            cwidget->maximizeView((RCView*)w->gethwnd());

        //rootSplitter->show();
        /*
        if(cwidget->tiled == 1)
        {
            cadwindow->view3dlist.start();
            w = cadwindow->view3dlist.next();
            cwidget->normalizeView((RCView*)w->gethwnd());
            //cadwindow->selectwindow(w);
        }
        */
        //cwidget->setActiveWindow((RCView*)w->gethwnd());
        cadwindow->selectwindow(0);
        qDebug() << "current view : " << QString(w->getview()->getname());
        cadwindow->selectwindow(w);

        int dcount=0;
        cwidget->debugSplitter(rootSplitter,dcount);
        qDebug() << "mSplitter count : " << cwidget->mSplitter->count();
        qDebug() << "mSplitter sizes : " << cwidget->mSplitter->sizes();
    }
    else
    {
        cwidget->initialise(Qt::Horizontal);

        if (! infile->read(&n))
            return 0;
        for (i = 0 ; i < n ; i++)
        {
            if (! infile->read(&type))
                return 0;
            if (type == View3dSurf)
            {
                if ((w = View3dWindow::load(infile)) == NULL)
                    return 0;
            }
            else
            {
                if ((w = View2dWindow::load(infile)) == NULL)
                    return 0;
            }
            if (cadwindow != 0 && cadwindow->view3dlist.add(w))
            {
                if (i == n-1)
                    cadwindow->selectwindow(w);
            }
        }
    }
#if 0
    // the Qt splitter data is in this version
    //read the splitter parent table size
    if (! infile->read(&n))
        return 0;
    // read the splitter table values and orientations
    QList<int> parents;
    QList<int> orientations;
    for(int i=0; i<n; i++)
    {
        if (! infile->read(&x))
            return 0;
        parents.push_back((int)x);
        if (! infile->read(&x))
            return 0;
        orientations.push_back((int)x);
    }
    // read the number of splitter size tables
    if (! infile->read(&n))
        return 0;
    // read the sizes of each splitter
    QList< QList<int> *> allSizes;
    for(int i=0; i<n; i++)
    {
        if (! infile->read(&x))
            return 0;
        QList<int> *sizes = new QList<int>;
        for(int j=0; j<x; j++)
        {
            if (! infile->read(&y))
                return 0;
            sizes->push_back((int)y);
        }
        allSizes.push_back(sizes);
    }

    // initialise with the root orientation
    cwidget->initialise((Qt::Orientation)orientations.at(0));

    if (! infile->read(&n))
        return 0;
    for (i = 0 ; i < n ; i++)
    {
        if (! infile->read(&type))
            return 0;
        if (type == View3dSurf)
        {
            if ((w = View3dWindow::load(infile)) == NULL)
                return 0;
        }
        else
        {
            if ((w = View2dWindow::load(infile)) == NULL)
                return 0;
        }
        if (cadwindow != 0 && cadwindow->view3dlist.add(w))
        {
            if (i == n-1)
                cadwindow->selectwindow(w);
        }
    }

    // get a list of the splitters currently displayed
    QList<QSplitter *> allSplitters = cwidget->findChildren<QSplitter *>();

    for(int i=0; i<parents.size(); i++)
    {
        QSplitter *thisSplitter=0;
        if(parents.at(i) == -1)
            thisSplitter = rootSplitter;
        else
            thisSplitter = allSplitters.at(parents.at(i));
        if(thisSplitter != 0)
        {
            QList<int> sizes = *allSizes.at(i);
            thisSplitter->setSizes(sizes);
            thisSplitter->setOrientation((Qt::Orientation)orientations.at(i));
        }
    }
#endif

    return 1;

}

void MainWindow::destroyall()
{
 GeneralList list;
 View3dWindow *v;
#if VERSION != TRICAD_STUDENT
 CadBasicWindow *cb;
#endif

 if(viewtab)
     viewtab->set_tiled(-1);  //  Don't update the window status while destroying the windows
  list = view3dlist.copy();
  RCCentralWidget *cw = (RCCentralWidget*)((QtMainWindow*)hWnd)->centerWidget;
  for (list.start() ;  (v = (View3dWindow *) list.next()) != NULL ; )
  {
#ifdef USING_WIDGETS
#ifdef USING_TABBED_WINDOWS
      int tabid = ((QTabWidget*)subwindow->gethwnd())->indexOf((QWidget*)v->gethwnd());
      ((QTabWidget*)subwindow->gethwnd())->removeTab(tabid);
      delete v;
#endif
#ifdef USING_MDIAREA
      //qDebug() << "v-gethWnd() : " << (QWidget*)v->gethwnd();
      //qDebug() << "v->gethWnd()->parent : " << ((QGraphicsView*)v->gethwnd())->parent();
      //((QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget())->removeSubWindow((QWidget*)((QGraphicsView*)v->gethwnd())->parent());
      ((QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->MdiArea)->removeSubWindow((QWidget*)((QGraphicsView*)v->gethwnd())->parent());
      delete v;
#endif

      cw->debugView3dList();
      cw->debug();
      if(cw !=0)
          cw->deleteView((RCView*)v->gethwnd());
      //delete v;
      cw->debugView3dList();
      cw->debug();

#else
      //DestroyWindow(v->gethwnd());
      //v->sethdc(0,0);
      //v->sethwnd(0);
      //v->setScene(0);
      DestroyWindow(v->gethwnd());
      delete v;
#endif
  }
  list.destroy();
  if(viewtab)
      viewtab->set_tiled(0);   //  Default to tiled setup

#if VERSION != TRICAD_STUDENT
  list = cadbasiclist.copy();
  for (list.start() ;  (cb = (CadBasicWindow *) list.next()) != NULL ; )
    DestroyWindow(cb->gethwnd());
  list.destroy();
#endif

}

void MainWindow::updatewindows(View *v)
{View3dWindow *window;
  for (view3dlist.start();  (window = (View3dWindow *) view3dlist.next()) != NULL ; )
    if (window->getview() == v)
      {  if (window->getoffscreenbitmap() != 0)
           {  window->getoffscreenbitmap()->displaylist.clear();
           }
         window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
      }
}

void MainWindow::updatewindows(View2d *view2d,int clear)
{View3dWindow *window;
  for (view3dlist.start();  (window = (View3dWindow *) view3dlist.next()) != NULL ; )
    if (((View2dWindow *)window)->getview2d() == view2d)
      {  if (window->getoffscreenbitmap() != 0)
           {  window->getoffscreenbitmap()->displaylist.clear();
           }
         ((View2dWindow *)window)->update(clear);
         window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
         InvalidateRect(window->gethwnd(),0,TRUE);
      }
}

void MainWindow::deletewindows(View2d *view2d)
{View3dWindow *window;
  for (view3dlist.start();  (window = (View3dWindow *) view3dlist.next()) != NULL ; )
    if (((View2dWindow *)window)->getview2d() == view2d)
      {  DestroyWindow(window->gethwnd());
         deletesubwindow(window);
         view3dlist.start();
      }
}

int MainWindow::viewused(int viewno)
{View3dWindow *window;
  for (view3dlist.start();  (window = (View3dWindow *) view3dlist.next()) != NULL ; )
    if (window->getview()->getviewno() == viewno) return 1;
  return 0;
}

void MainWindow::cursoraction(int act)
{View3dWindow *v;
  for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    v->cursoraction(act);
}

void MainWindow::cursorstyle(int sty)
{View3dWindow *v;
    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
        v->cursorstyle(sty);
}

void MainWindow::create3dsubwindow(View *v,int x,int y)
{
    // debug
    if(hWnd)
    {
        //qDebug() << "=========================";
        //qDebug() << "main window object   : " << ((QMainWindow*)hWnd)->objectName();
        //qDebug() << "main window geometry : " << ((QMainWindow*)hWnd)->geometry();
        //qDebug() << "main window Frame geometry : " << ((QMainWindow*)hWnd)->frameGeometry();
#ifdef USING_TABBED_WINDOWS
        qDebug() << "tab window object   : " << ((QMainWindow*)hWnd)->centralWidget()->objectName();
        qDebug() << "tab window geometry : " << ((QMainWindow*)hWnd)->centralWidget()->geometry();
        qDebug() << "tab window object   : " << (QWidget*)cadwindow->subwindow->gethwnd();
#endif
#ifdef USIN_QMDIAREA
        qDebug() << "mdiArea window object   : " << ((QtMainWindow*)hWnd)->MdiArea->objectName();
        qDebug() << "mdiArea window geometry : " << ((QtMainWindow*)hWnd)->MdiArea->geometry();
        qDebug() << "mdiArea window object   : " << (QWidget*)cadwindow->subwindow->gethwnd();
#endif
        //qDebug() << "subwindow object  : " << ((QWidget*)cadwindow->subwindow->gethwnd())->objectName();
        //qDebug() << "subwindow width   : " << cadwindow->subwindow->getwidth();
        //qDebug() << "subwindow height  : " << cadwindow->subwindow->getheight();
        //qDebug() << "=========================";
    }
    //create3dsubwindow(v,x,y,int(getwidth()) * 2 / 3,int(getheight()) * 2 / 3);
    //create3dsubwindow(v,x,y,int(getwidth()),int(getheight()));
#ifdef USING_TABBED_WINDOWS
    QTabWidget *qtw = (QTabWidget*)((QMainWindow*)hWnd)->centralWidget();
    create3dsubwindow(v,x,y,int(qtw->geometry().width()),int(qtw->geometry().height()));
#endif
#ifdef USING_QMDIAREA
    QMdiArea *qmda = (QMdiArea*)((QtMainWindow*)hWnd)->MdiArea;
    //QMdiArea *qmda = (QMdiArea*)((QMainWindow*)hWnd)->centralWidget();
    create3dsubwindow(v,x,y,int(qmda->geometry().width()),int(qmda->geometry().height()));
#endif
    RCCentralWidget *cw = (RCCentralWidget*)((QtMainWindow*)hWnd)->centerWidget;
    create3dsubwindow(v,x,y,int(cw->geometry().width()),int(cw->geometry().height()));
    //create3dsubwindow(v,x,y,int(cadwindow->subwindow->getwidth()),int(cadwindow->subwindow->getheight()));
}

void MainWindow::create3dsubwindow(View *v,int x,int y,int w,int h)
{
    View3dWindow *window = new View3dWindow(this,subwindow,v,x,y,w,h, RCSCENEVW | RCGLWINTYPE | RCV3DWINTYPE);
    //qDebug() << "View3dWindow object : " << ((QWidget*)window->gethwnd())->objectName();
    //qDebug() << "View3dWindow viewport object : " << ((RCView*)window->gethwnd())->viewport()->objectName();
    //qDebug() << "View3dWindow scene : " << (RCGraphicsScene*)window->getScene();
    //qDebug() << QString(v->getname());
    QString objname("3dSubwindow-");
    objname += QString(v->getname());
    window->getScene()->setObjectName(objname);
    if (view3dlist.add(window))
        selectwindow(window);
    ((QtMainWindow*)hWnd)->centerWidget->debugView3dList();

    sizeevent(-1,-1);
    window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
    // KMJ : TODO : think about if we need this long data on a 3d window
    SetWindowLong(window->gethwnd(),GWL_USERDATA,1);
    // KMJ : TODO : implement the window tabs
    //viewtab->update_window_state();
    //qDebug() << "=========================";
    RCCentralWidget *cw = (RCCentralWidget*)((QtMainWindow*)hWnd)->centerWidget;
    qDebug() << "center widget rect : "  << cw->rect();
    qDebug() << "RCView rect : " << ((RCView*)window->gethwnd())->rect();
    qDebug() << "graphic view rect : " << ((RCView*)window->gethwnd())->viewport()->rect();
    qDebug() << "graphicview scene rect : " << ((RCView*)window->gethwnd())->view()->sceneRect();
    qDebug() << "graphic view viewport : " << ((RCView*)window->gethwnd())->view()->viewport();
    qDebug() << "graphic view viewport rect : " << ((RCView*)window->gethwnd())->view()->viewport()->rect();
    qDebug() << "scene scene rect : " << ((RCGraphicsScene*)window->getScene())->sceneRect();
    qDebug() << "=========================";

}

void MainWindow::replace3dsubwindow(View3dWindow *currentWindow, View * newView)
{
    currentWindow->setView(newView);
    updatewindows(currentWindow->getview());
    //((RCView*)currentWindow->gethwnd())->adjustViewRect();
    //currentWindow->paintevent(true);
}

void MainWindow::freezeBackImage()
{
    if((RCView*)getcurrentwindow()->gethwnd() != 0)
        ((RCView*)getcurrentwindow()->gethwnd())->freezeBackImage();
}

void MainWindow::unFreezeBackImage()
{
    if((RCView*)getcurrentwindow()->gethwnd() != 0)
        ((RCView*)getcurrentwindow()->gethwnd())->unFreezeBackImage();
}

void MainWindow::create2dsubwindow(View2d *v)
{
    // debug
    if(hWnd)
    {
        qDebug() << "=========================";
        qDebug() << "main window object   : " << ((QMainWindow*)hWnd)->objectName();
        qDebug() << "main window geometry : " << ((QMainWindow*)hWnd)->geometry();
        qDebug() << "main window Frame geometry : " << ((QMainWindow*)hWnd)->frameGeometry();
#ifdef USING_TABBED_WINDOWS
        qDebug() << "tab window object   : " << ((QMainWindow*)hWnd)->centralWidget()->objectName();
        qDebug() << "tab window geometry : " << ((QMainWindow*)hWnd)->centralWidget()->geometry();
        qDebug() << "tab window object   : " << (QWidget*)cadwindow->subwindow->gethwnd();
#else
        qDebug() << "mdiArea window object   : " << ((QMainWindow*)hWnd)->centralWidget()->objectName();
        qDebug() << "mdiArea window geometry : " << ((QMainWindow*)hWnd)->centralWidget()->geometry();
        qDebug() << "mdiArea window object   : " << (QWidget*)cadwindow->subwindow->gethwnd();
#endif
        qDebug() << "subwindow object  : " << ((QWidget*)cadwindow->subwindow->gethwnd())->objectName();
        qDebug() << "subwindow width   : " << cadwindow->subwindow->getwidth();
        qDebug() << "subwindow height  : " << cadwindow->subwindow->getheight();
    }
    //create2dsubwindow(v,int(getwidth()) * 2 / 3,int(getheight()) * 2 / 3);
    //create2dsubwindow(v,int(getwidth()),int(getheight()));
#ifdef USING_TABBED_WINDOWS
    QTabWidget *qtw = (QTabWidget*)((QMainWindow*)hWnd)->centralWidget();
    create2dsubwindow(v,int(qtw->geometry().width()),int(qtw->geometry().height()));
#endif
#ifdef USING_QMDIAREA
    QMdiArea *qmda = (QMdiArea*)((QtMainWindow*)hWnd)->MdiArea;
    //QMdiArea *qmda = (QMdiArea*)((QMainWindow*)hWnd)->centralWidget();
    create2dsubwindow(v,int(qmda->geometry().width()),int(qmda->geometry().height()));
#else
    RCCentralWidget *cw = (RCCentralWidget*)((QtMainWindow*)hWnd)->centerWidget;
    create2dsubwindow(v,int(cw->geometry().width()),int(cw->geometry().height()));
#endif
}

void MainWindow::create2dsubwindow(View2d *v,int w,int h)
{
    View *top = new View(v->getname(),0,v->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1.0E6,1.0E6);
    View2dWindow *window = new View2dWindow(this,subwindow,v,top,w,h, RCSCENEVW | RCGLWINTYPE | RCV2DWINTYPE);
    qDebug() << "View3dWindow object : " << ((QWidget*)window->gethwnd())->objectName();
    qDebug() << "View3dWindow viewport object : " << ((RCView*)window->gethwnd())->view()->viewport()->objectName();
    //qDebug() << "View3dWindow viewport object : " << ((QGraphicsView*)window->gethwnd())->viewport()->objectName();
    qDebug() << "View3dWindow scene : " << (RCGraphicsScene*)window->getScene();
    qDebug() << QString(v->getname());
    QString objname("2dSubwindow-");
    objname += QString(v->getname());
    window->getScene()->setObjectName(objname);
    if (view3dlist.add(window))
        selectwindow(window);
    ((QtMainWindow*)hWnd)->centerWidget->debugView3dList();
    sizeevent(-1,-1);
    window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
    SetWindowLong(window->gethwnd(),GWL_USERDATA,1);
    // KMJ : TODO : implement the window tabs
    //viewtab->update_window_state();
    qDebug() << "=========================";
}

int MainWindow::issubwindow(View3dWindow *window)
{
    View3dWindow *v;
    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
        if (window == v) return 1;
    return 0;
}

void MainWindow::deletesubwindow(View3dWindow *window)
{
    qDebug() << "view3dlist.length : " << view3dlist.length();
    if (curview3d == window)
        selectwindow(NULL);
    view3dlist.del(window);
    qDebug() << "view3dlist.length : " << view3dlist.length();
    view3dlist.start();
    selectwindow(view3dlist.next());
    // KMJ : TODO : implement the window tabs
    //if(viewtab)
    //    viewtab->update_window_state();
    qDebug() << "view3dlist.length : " << view3dlist.length();
}

void MainWindow::cascade(void)
{
    View3dWindow *v;
 int i,n,w1,w2,sw,sh;
 View3dWindowList list;

    sw = int(subwindow->getwidth());  sh = int(subwindow->getheight());
    i = n = (floatingmenu1 != NULL) + (floatingmenu2 != NULL) + (floatingmenu3 != NULL)  + (floatingmenu4 != NULL);
    if (floatingmenu1 != NULL)
    {   SetWindowPos(floatingmenu1->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu2 != NULL)
    {   SetWindowPos(floatingmenu2->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu4 != NULL)
    {   SetWindowPos(floatingmenu4->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu3 != NULL)
        SetWindowPos(floatingmenu3->gethwnd(),HWND_TOP,sw - n * 50,0,49,48+GetSystemMetrics(SM_CYCAPTION),SWP_DRAWFRAME);
    sw = sw - i * 50;
    w1 = GetSystemMetrics(SM_CYCAPTION);
#if ! defined(_WIN32_WCE)
    w2 = GetSystemMetrics(SM_CXFRAME);
#else
    w2 = 0;
#endif
    if (viewtab->get_tiled() == 0)
    {//  If the windows are not tiled, then tile all the windows
        for (view3dlist.start() ; (v = (View3dWindow *) view3dlist.next()) != NULL ; )
            SetWindowLong(v->gethwnd(),GWL_USERDATA,1);

    }
    for (view3dlist.start(),n = 0 ; (v = (View3dWindow *) view3dlist.next()) != NULL ; )
        if (GetWindowLong(v->gethwnd(),GWL_USERDATA))
            n++;
    if (n == 0) return;
    if (n > 5) n = 5;
    while (sw - (n-1)*(w1+w2) < 100) n--;
    while (sh - (n-1)*(w1+w2) < 100) n++;
    if (n < 1) n = 1;
    list = view3dlist.copy();  list.reverse();
    for (list.start(),i = 0 ; (v = list.next()) != NULL ; )
        if (GetWindowLong(v->gethwnd(),GWL_USERDATA) != 0)
        {   ShowWindow(v->gethwnd(),SW_SHOWNORMAL);
            SetWindowPos(v->gethwnd(),HWND_TOP,(i % n)*(w1+w2),(i % n)*(w1+w2),sw-(n-1)*(w1+w2),sh-(n-1)*(w1+w2),SWP_DRAWFRAME);
            SetWindowLong(v->gethwnd(),GWL_USERDATA,1);
            i++;
        }
    list.destroy();

    viewtab->set_tiled(1);
    viewtab->draw(DM_NORMAL);

}

void MainWindow::tile(void)
{
    View3dWindow *v;
 int x,y,w,h,i,j,n,sw,sh,nrows,ncols,swapped;

    sw = int(subwindow->getwidth());
    sh = int(subwindow->getheight());
    i = n = (floatingmenu1 != NULL) + (floatingmenu2 != NULL) + (floatingmenu3 != NULL) + (floatingmenu4 != NULL);
    if (floatingmenu1 != NULL)
    {   SetWindowPos(floatingmenu1->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu2 != NULL)
    {   SetWindowPos(floatingmenu2->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu4 != NULL)
    {   SetWindowPos(floatingmenu4->gethwnd(),HWND_TOP,sw - n * 50,0,49,300,SWP_DRAWFRAME);
        n--;
    }
    if (floatingmenu3 != NULL)
        SetWindowPos(floatingmenu3->gethwnd(),HWND_TOP,sw - n * 50,0,49,48+GetSystemMetrics(SM_CYCAPTION),SWP_DRAWFRAME);
    sw = sw - i * 50;

    if (viewtab->get_tiled() == 0)
    {//  If the windows are not tiled, then tile all the windows
        for (view3dlist.start() ; (v = (View3dWindow *) view3dlist.next()) != NULL ; )
            SetWindowLong(v->gethwnd(),GWL_USERDATA,1);

    }

    for (view3dlist.start(),n = 0 ; (v = (View3dWindow *) view3dlist.next()) != NULL ; )
        if (GetWindowLong(v->gethwnd(),GWL_USERDATA))
            n++;
    if (n == 0) return;
    ncols = int(sqrt((double)n) + 0.5);
    view3dlist.start();
    x = 0;
    w = sw;
    h = sh;
    swapped = 0;
    if (w > h * 3 / 2)
    {   swapped = 1;  w = sh;
    }
    for (i = 0 ; i < ncols ; i++)
    {   nrows = n / (ncols - i);  if (nrows < 0) nrows = 1;
        y = 0;
        if (swapped)
            h = sw;
        else
            h = sh;
        for (j = 0 ; j < nrows ; j++)
        {   v = (View3dWindow *) view3dlist.next();
            while (v != 0 && GetWindowLong(v->gethwnd(),GWL_USERDATA) == 0)
                v = (View3dWindow *) view3dlist.next();
            if (v != 0)
            {   ShowWindow(v->gethwnd(),SW_SHOWNORMAL);
                if (swapped)
                    SetWindowPos(v->gethwnd(),HWND_TOP,y,x,h/(nrows-j),w/(ncols-i),SWP_DRAWFRAME);
                else
                    SetWindowPos(v->gethwnd(),HWND_TOP,x,y,w/(ncols-i),h/(nrows-j),SWP_DRAWFRAME);
            }
            y += h/(nrows-j);
            h -= h/(nrows-j);
        }
        x += w/(ncols-i);
        w -= w/(ncols-i);
        n -= nrows;
    }

    viewtab->set_tiled(1);
    viewtab->draw(DM_NORMAL);

}


void MainWindow::tab(void)
{
    if (GetWindow(subwindow->gethwnd(),GW_CHILD) != 0)
    {   if (viewtab->get_tiled())
        {   SetWindowPos(GetWindow(subwindow->gethwnd(),GW_CHILD),HWND_BOTTOM,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
            viewtab->draw(DM_NORMAL);
        }
        else
            viewtab->select(0);
    }
}
#ifdef USING_WIDGETS
void MainWindow::clearAllRubberBands()
{
    rubberband.drawing=0;
    View3dWindow *v;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        if (v->getsurfacetype() == View3dSurf)
        {
            if(v->qgirb && v->getScene())
                v->getScene()->removeItem(v->qgirb);
            delete v->qgirb;
            v->qgirb=0;
        }
        else if (v->getsurfacetype() == View2dSurf)
        {
            if(v->qgirb && v->getScene())
                v->getScene()->removeItem(v->qgirb);
            delete v->qgirb;
            v->qgirb=0;

            View3dSubSurface *ss;
            for (((View2dWindow*)v)->subsurfaces.start(); (ss = (View3dSubSurface *) ((View2dWindow*)v)->subsurfaces.next()) != NULL ; )
            {
                if(ss->qgirb && ss->getScene())
                    ss->getScene()->removeItem(ss->qgirb);
                delete ss->qgirb;
                ss->qgirb=0;
            }
        }
    }
}

void MainWindow::clearAllViews()
{
    View3dWindow *v;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        if (v->getsurfacetype() == View3dSurf)
        {
            db.clearScene(DM_ERASE,v);
        }
        else if (v->getsurfacetype() == View2dSurf)
        {
            db.clearScene(DM_ERASE,v);
        }
    }
}

void MainWindow::updateAllViews()
{
    View3dWindow *v;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        if (v->getsurfacetype() == View3dSurf)
        {
            v->UpdateWindow();
        }
        else if (v->getsurfacetype() == View2dSurf)
        {
            v->UpdateWindow();
        }
    }
}

void MainWindow::drawent(int drawmode,Transform *t,Entity *entity,QPainter *painter)
{
    View3dWindow *v;
  if (draw)
    return;
  int qsidx=0;
  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
  {
    if (v->getsurfacetype() == View3dSurf)
      { //v->qsidx = qsidx;
        if (entity->isvisible(v))
           {  if (v->getoffscreenbitmap() != 0)
                {
                   entity->draw(drawmode,t,v->getoffscreenbitmap());
                   if (batch == 0)
                     v->UpdateWindow();
                }
              else
                entity->draw(drawmode,t,v,painter);
           }
        qsidx++;
      }
    else if (v->getsurfacetype() == View2dSurf && entity->isvisible(NULL))
      ((View2dWindow *)v)->drawent(drawmode,t,entity);
  }
}

void MainWindow::drawentGL(int drawmode,Transform *t,Entity *entity )
{
    View3dWindow *v;
  if (draw)
    return;
  int qsidx=0;

  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
  {
    if (v->getsurfacetype() == View3dSurf)
      { //v->qsidx = qsidx;
        if (entity->isvisible(v))
           {
#if 0
            if (v->getoffscreenbitmap() != 0)
                {
                   entity->drawGL(drawmode,t,v->getoffscreenbitmap());
                   if (batch == 0)
                     v->UpdateWindow();
                }
              else
                entity->drawGL(drawmode,t,v);
#else
            entity->drawGL(drawmode,t,v);
#endif
           }
        qsidx++;
      }
    else if (v->getsurfacetype() == View2dSurf && entity->isvisible(NULL))
      ((View2dWindow *)v)->drawentGL(drawmode,t,entity);
  }
}
#endif
void MainWindow::drawent(int drawmode,Transform *t,Entity *entity)
{
  View3dWindow *v=0;
  if (draw)
    return;
  int cnt=0;

#if 1

  // KMJ: debug for speed up

  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
  {
#if 0
      if(v->getScene())
      {
          qDebug() << "scene : " << v->getScene();
          qDebug() << "scene tree depth : " << v->getScene()->bspTreeDepth();
      }
#endif

      //qDebug() << "view name : " << QString(((RCView*)v->gethwnd())->getView3dWindow()->getview()->getname());
      //qDebug() << "view is visible : " << ((RCView*)v->gethwnd())->isVisible();
    if (v->getsurfacetype() == View3dSurf && ((RCView*)v->gethwnd())->isVisible())
    {
        if (entity->isvisible(v))
        {
#if 0
              if (v->getoffscreenbitmap() != 0)
                {
                   entity->draw(drawmode,t,v->getoffscreenbitmap());
                   if (batch == 0)
                     v->UpdateWindow();
                }
              else
                entity->draw(drawmode,t,v);
#else
            //qDebug() << "drawing ent in view : " << QString(((RCView*)v->gethwnd())->getView3dWindow()->getview()->getname());
            //if (batch == 0)
                entity->draw(drawmode,t,v);
#endif
        }
    }
    else if (v->getsurfacetype() == View2dSurf && ((RCView*)v->gethwnd())->isVisible() )
    {
        if( entity->isvisible(0) )
        {
            //qDebug() << "drawing ent in view : " << QString(((RCView*)v->gethwnd())->getView3dWindow()->getview()->getname());
            ((View2dWindow *)v)->drawent(drawmode,t,entity);
        }
    }

      /*
    if(v->getScene())
    {
#if 0
        v->getScene()->setBackgroundBrush(QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                                        int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                                        int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));
#endif
    }
    */
  }

#endif
}

void MainWindow::startdrawingbatch()
{
    batch++;
    //qDebug() << "batch = " << batch;
}

void MainWindow::enddrawingbatch()
{
    View3dWindow *v;
  if (batch > 0)
    batch--;
  if (batch == 0)
    {  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
#ifdef NEW_DRAGGING
          //if (v->getsurfacetype() == View3dSurf)

          if(v->getRaytracing() == 0)
            v->UpdateWindow();
#else
         if (v->getsurfacetype() == View3dSurf)
           v->UpdateWindow();
#endif
    }
  //qDebug() << "batch = " << batch;
}


void MainWindow::invalidatedisplaylist(Entity *e)
{
    // KMJ not using display lists now
  /*
  View3dWindow *v;
  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    if (v->getoffscreenbitmap() != 0)
      v->getoffscreenbitmap()->displaylist.del(e);
  */
}

void MainWindow::drawcallback(int drawmode,void *data,DrawCallback cb)
{
 View3dWindow *v;
 GeneralList list;
  list = view3dlist;
  for (list.start();  (v = (View3dWindow *) list.next()) != NULL ; )
    cb(drawmode,data,(View3dSurface *)v);
}

// this is a Qt function
void MainWindow::paintEvent(QPaintEvent *event)
{
    // qDebug() << "Entering : MainWindow::paintEvent ";
     paintevent( );
     //qDebug() << "Leaving : MainWindow::paintEvent ";
}

void MainWindow::paintevent(void)
{
#ifdef USING_WIDGETS
  //qDebug() << "Entering : MainWindow::paintevent ";

  // KMJ : TODO : fix this
  Window::paintevent();

  //paintall();

  //if (prompt != 0)
  //  prompt->draw(DM_NORMAL);

  if (coordinatedisplay != 0)
    coordinatedisplay->draw(DM_NORMAL);
  if (coordinateentry != 0)
    coordinateentry->draw(DM_NORMAL);

  //qDebug() << "Leaving : MainWindow::paintevent ";
#else
    qDebug() << "Entering : MainWindow::paintevent ";
    RECT rect;

    rect.left = 0;  rect.top = 0;
    rect.right = int(getwidth());  rect.bottom = topmenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNFACE));
    rect.left = 0;  rect.top = topmenuareaheight-1;
    rect.right = int(getwidth());  rect.bottom = topmenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));

    rect.left = 0;  rect.top = int(getheight()) - bottommenuareaheight;
    rect.right = int(getwidth());  rect.bottom = int(getheight());
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNFACE));
    rect.left = 0;  rect.top = int(getheight()) - bottommenuareaheight;
    rect.right = int(getwidth());  rect.bottom = rect.top+1;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNHIGHLIGHT));

    rect.left = int(getwidth()-rightmenuareawidth);  rect.top = topmenuareaheight;
    rect.right = int(getwidth());  rect.bottom = int(getheight())-bottommenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNFACE));
    rect.left = int(getwidth()-rightmenuareawidth)-1;  rect.top = topmenuareaheight;
    rect.right = int(getwidth()-rightmenuareawidth);  rect.bottom = int(getheight())-bottommenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));

    rect.left = 0;  rect.top = topmenuareaheight;
    rect.right = leftmenuareawidth;  rect.bottom = int(getheight())-bottommenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNFACE));
    rect.left = leftmenuareawidth;  rect.top = topmenuareaheight;
    rect.right = leftmenuareawidth + 1;  rect.bottom = int(getheight())-bottommenuareaheight;
    FillRect(gethdc(),&rect,GetSysColorBrush(COLOR_BTNHIGHLIGHT));


    // KMJ : TODO : fix this
    Window::paintevent();

    if (prompt != 0)
      prompt->draw(DM_NORMAL);

    if (coordinatedisplay != 0)
      coordinatedisplay->draw(DM_NORMAL);
    if (coordinateentry != 0)
      coordinateentry->draw(DM_NORMAL);

#endif
}

// QT Widget function
void MainWindow::resizeEvent(QResizeEvent * qevent)
{
    //qDebug() << "Entering : MainWindow::resizeEvent ";
    //qDebug() << "MainWindow::resizeEvent setting : " << qevent->size().width() << qevent->size().height();
    sizeevent(qevent->size().width(),qevent->size().height());
    //qDebug() << "MainWindow::resizeEvent result : " << this->width << this->height;
    //qDebug() << "Leaving : MainWindow::resizeEvent ";
}

void MainWindow::sizeevent(int w,int h)
{
 //qDebug() << "Entering : MainWindow::sizeevent ";
 RECT rect;
 int commandwindowfixed,i,min_right,max_right,min_bottom,max_bottom;
 Control *c;
 POINT p;
 GeneralList list;

    if (w == -1)
        w = int(getwidth());
    if (h == -1)
        h = int(getheight());

    if (w == 0 || h == 0)
        return;
    Window::sizeevent(w,h);

    bottommenuareaheight = topmenuareaheight = 0;
    leftmenuareawidth = rightmenuareawidth = 0;

    min_right = -1;
    max_right = -1;
    min_bottom = -1;
    max_bottom = -1;

    /*
    //  Find the width of the docked controls on the right and the height of the docked controls on the bottom
    for (i = 0 ; (c = getcontrol(i)) != 0? ; i++)
        if (c != viewtab && c != viewproperties)
        {  if (c->w < c->h)
            {//  Docking to the sides.
                if (c->x > w / 2)
                {   if (min_right == -1 || c->x < min_right)
                        min_right = c->x;
                    if (max_right == -1 || c->x + c->w + MenuHandleOffset + 6 > max_right)
                        max_right = c->x + c->w + MenuHandleOffset + 6;
                }
            }
            else
            {//  Docking to the top/bottom
                if (c->y > h / 2)
                {   if (min_bottom == -1 || c->y < min_bottom)
                        min_bottom = c->y;
                    if (max_bottom == -1 || c->y + c->h > max_bottom + MenuBorderOffset + 2)
                        max_bottom = c->y + c->h + MenuBorderOffset + 2;
                }
            }
        }

    //  Now move the controls on the bottom and top to a good spot on the screen.
    for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
        list.add(c);

    for (list.start() ; (c = (Control *)list.next()) != 0 ; )
        if (c != viewtab && c != viewproperties)
        {  if (c->w < c->h)
            {//  Docking to the sides.
                if (c->x > w / 2)
                    c->move(w - max_right,0);
            }
            else
            {//  Docking to the top/bottom
                if (c->y > h / 2)
                    c->move(0,h - max_bottom);
            }
        }
    list.destroy();
    */

    //  Now actually allocate the space for the menus
    /*
    for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
        if (c != viewtab && c != viewproperties)
        {  if (c->w < c->h)
            {//  Docking to the sides.
                if (c->x > w / 2 && w - c->x + MenuHandleOffset + 6 >= rightmenuareawidth)
                    rightmenuareawidth = w - c->x + MenuHandleOffset;
                if (c->x + c->x < w / 2 && c->x + c->w + MenuHandleOffset >= leftmenuareawidth)
                    leftmenuareawidth = c->x + c->w + MenuHandleOffset;
            }
            else
            {//  Docking to the top/bottom
                if (c->y > h / 2 && h - c->y + MenuBorderOffset + 2 >= bottommenuareaheight)
                    bottommenuareaheight = h - c->y + MenuBorderOffset + 2;
                if (c->y + c->h < h / 2 && c->y + c->h + MenuBorderOffset + 2 >= topmenuareaheight)
                    topmenuareaheight = c->y + c->h + MenuBorderOffset + 2;
            }
        }
     */
    //  Add the properties control to the left or right of the screen
    /*
    if (viewproperties && viewproperties->get_dock_left())
    {   viewproperties->size(leftmenuareawidth,topmenuareaheight,viewproperties->get_properties_width(),h - topmenuareaheight - bottommenuareaheight);
        leftmenuareawidth += viewproperties->get_properties_width();
    }
    else
    {
        if(viewproperties){
        rightmenuareawidth += viewproperties->get_properties_width();
        viewproperties->size(w - rightmenuareawidth,topmenuareaheight,viewproperties->get_properties_width(),h - topmenuareaheight - bottommenuareaheight);
        }
    }
    //  Add the tab control to the bottom or top of the screen
    if (viewtab && viewtab->get_dock_top())
    {   topmenuareaheight += viewtab->get_tab_height(w - leftmenuareawidth - rightmenuareawidth);
        viewtab->size(leftmenuareawidth,topmenuareaheight-viewtab->get_tab_height(w - leftmenuareawidth - rightmenuareawidth),w - leftmenuareawidth - rightmenuareawidth,viewtab->get_tab_height(w - leftmenuareawidth - rightmenuareawidth));
    }
    else
    {
        if(viewtab){
        bottommenuareaheight += viewtab->get_tab_height(w - leftmenuareawidth - rightmenuareawidth);
        viewtab->size(leftmenuareawidth,h - bottommenuareaheight,w - leftmenuareawidth - rightmenuareawidth,viewtab->get_tab_height(w - leftmenuareawidth - rightmenuareawidth));
        }
    }
    */


    p.x = 0;  p.y = 0;

    if(subwindow)
        ClientToScreen(subwindow->gethwnd(),&p);
    else
        ClientToScreen(gethwnd(),&p);
    ScreenToClient(gethwnd(),&p);
    if(commandwindow)
        commandwindowfixed  = commandwindow != 0 && GetParent(commandwindow->gethwnd()) == gethwnd();
    else
        commandwindowfixed=0;

    // KMJ : We need to resize the graphic window
    // currently we are using a fixed tabbed window so need to resize it here
    // this function is called when the QtMainWindow is resized
    // NOTE: should put all the widget resizing inside the owners resize function not here
    View3dWindow *v;
    RCCOLORREF cr;

    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    {
        // resize the scene rect through the View3dWindow object
        // the veiwing will be correctly sized
        //v->sizeevent(w,h);

        //qDebug() <<  "v getwidth"  << v->getwidth();
        //qDebug() <<  "v getheight"  << v->getheight();
        //qDebug() <<  "viewport width"  << ((RCView*)v->gethwnd())->viewport()->width();
        //qDebug() <<  "viewport height"  << ((RCView*)v->gethwnd())->viewport()->height();
        //qDebug() <<  "QGraphicsView sceneRect width"  << ((RCView*)v->gethwnd())->view()->sceneRect().width();
        //qDebug() <<  "QGraphicsView sceneRect height"  << ((RCView*)v->gethwnd())->view()->sceneRect().height();
        //v->sizeevent(((QWidget*)v->gethwnd())->rect().width(),
        //             ((QWidget*)v->gethwnd())->rect().height());
        //v->sizeevent(((QGraphicsView*)v->gethwnd())->sceneRect().width(),
        //             ((QGraphicsView*)v->gethwnd())->sceneRect().height());
        //v->sizeevent(((RCView*)v->gethwnd())->viewport()->width(),
        //             ((RCView*)v->gethwnd())->viewport()->height());
        //qDebug() <<  "RCView View width"  << ((RCView*)v->gethwnd())->view()->width();
        //qDebug() <<  "RCView View height"  << ((RCView*)v->gethwnd())->view()->height();
        //v->sizeevent(((RCView*)v->gethwnd())->view()->width(),
        //             ((RCView*)v->gethwnd())->view()->height());
        v->sizeevent(((RCView*)v->gethwnd())->viewport()->width(),
                     ((RCView*)v->gethwnd())->viewport()->height());
        //qDebug() <<  "QGraphicsView sceneRect width"  << ((RCView*)v->gethwnd())->view()->sceneRect().width();
        //qDebug() <<  "QGraphicsView sceneRect height"  << ((RCView*)v->gethwnd())->view()->sceneRect().height();
        //qDebug() <<  "QGraphicsView scene Rect width"  << ((RCView*)v->gethwnd())->view()->scene()->sceneRect().width();
        //qDebug() <<  "QGraphicsView scene Rect height"  << ((RCView*)v->gethwnd())->view()->scene()->sceneRect().height();


        //QTabWidget *qtw = (QTabWidget*)((QMainWindow*)hWnd)->centralWidget();
        //v->sizeevent(qtw->geometry().width(),qtw->geometry().height());
        //qDebug() << "tab widget : " << ((QWidget*)qtw->widget(0))->objectName();
        //QGraphicsView *qgv = qtw->widget()
        //v->sizeevent(((QGraphicsView*)v->gethwnd())->viewport()->geometry().width(),
        //             ((QGraphicsView*)v->gethwnd())->viewport()->geometry().height());
        //v->sizeevent(((QGraphicsView*)v->gethwnd())->geometry().width(),
        //            ((QGraphicsView*)v->gethwnd())->geometry().height());
    }

    // update the top tab window
    // not really needed here just checking the sizes
#ifdef USING_TABBED_WINDOWS
    QTabWidget *qtab = (QTabWidget*)subwindow->gethwnd();
    if(qtab)
    {
        if(qtab->currentWidget())
        {
            QGraphicsView* gview = (QGraphicsView*)qtab->currentWidget();
            if(gview)
            {
                QGLWidget *glw = gview->findChild<QGLWidget *>("QGLWidget");


                /*
                qDebug() << " ";
                qDebug() << "tab widget name           : " << qtab->objectName();
                qDebug() << "tab widget rect           : " << qtab->rect();
                qDebug() << "tab widget size           : " << qtab->size();
                qDebug() << "tab widget geometry       : " << qtab->geometry();

                qDebug() << "before resize ";
                qDebug() << "Graphic widget name       : " << gview->objectName();
                qDebug() << "Graphic widget rect       : " << gview->rect();
                qDebug() << "Graphic widget size       : " << gview->size();
                qDebug() << "Graphic widget geometry   : " << gview->geometry();
                qDebug() << "Graphic widget scene rect : " << gview->sceneRect();

                qDebug() << "GL widget name            : " << glw->objectName();
                qDebug() << "GL widget rect            : " << glw->rect();
                qDebug() << "GL widget size            : " << glw->size();
                qDebug() << "GL widget geometry        : " << glw->geometry();
                */

                //gview->setGeometry(qtab->geometry());
                //gview->setSceneRect(qtab->rect());

                qDebug() << "after resize ";
                qDebug() << "Graphic widget name       : " << gview->objectName();
                qDebug() << "Graphic widget rect       : " << gview->rect();
                qDebug() << "Graphic widget size       : " << gview->size();
                qDebug() << "Graphic widget geometry   : " << gview->geometry();
                qDebug() << "Graphic widget scene rect : " << gview->sceneRect();
                qDebug() << "Graphic widge scene       : " << gview->scene();

                qDebug() << "GL widget name            : " << glw->objectName();
                qDebug() << "GL widget rect            : " << glw->rect();
                qDebug() << "GL widget size            : " << glw->size();
                qDebug() << "GL widget geometry        : " << glw->geometry();
            }
        }
    }
#else
#ifdef USING_QMDIAREA
    QMdiArea *qmda = (QMdiArea*)subwindow->gethwnd();
    if(qmda)
    {
        if(qmda->currentSubWindow())
        {
            qDebug() << "subwindow widget name           : " << qmda->currentSubWindow()->widget();
            QGraphicsView* gview = (QGraphicsView*)qmda->currentSubWindow()->widget();
            if(gview)
            {

                qDebug() << "subwindow viewport widget name : " << gview->viewport();
                //QGLWidget *glw = gview->findChild<QGLWidget *>("QGLWidget");
                QGLWidget *glw = (QGLWidget*)gview->viewport();


                /*
                qDebug() << " ";
                qDebug() << "subwindow widget name           : " << qmdw->objectName();
                qDebug() << "subwindow widget rect           : " << qmdw->rect();
                qDebug() << "subwindow widget size           : " << qmdw->size();
                qDebug() << "subwindow widget geometry       : " << qmdw->geometry();

                qDebug() << "before resize ";
                qDebug() << "Graphic widget name       : " << gview->objectName();
                qDebug() << "Graphic widget rect       : " << gview->rect();
                qDebug() << "Graphic widget size       : " << gview->size();
                qDebug() << "Graphic widget geometry   : " << gview->geometry();
                qDebug() << "Graphic widget scene rect : " << gview->sceneRect();

                qDebug() << "GL widget name            : " << glw->objectName();
                qDebug() << "GL widget rect            : " << glw->rect();
                qDebug() << "GL widget size            : " << glw->size();
                qDebug() << "GL widget geometry        : " << glw->geometry();
                */

                //gview->setGeometry(qtab->geometry());
                //gview->setSceneRect(qtab->rect());

                qDebug() << "after resize ";
                qDebug() << "Graphic widget name       : " << gview->objectName();
                qDebug() << "Graphic widget rect       : " << gview->rect();
                qDebug() << "Graphic widget size       : " << gview->size();
                qDebug() << "Graphic widget geometry   : " << gview->geometry();
                qDebug() << "Graphic widget scene rect : " << gview->sceneRect();
                qDebug() << "Graphic widge scene       : " << gview->scene();

                qDebug() << "GL widget name            : " << glw->objectName();
                qDebug() << "GL widget rect            : " << glw->rect();
                qDebug() << "GL widget size            : " << glw->size();
                qDebug() << "GL widget geometry        : " << glw->geometry();
            }
        }
    }
#endif
#endif
/*
 // debug
    for (view3dlist.start();  (v = (View3dWindow *) view3dlist.next()) != NULL ; )
    {
        qDebug() << "View3dWindow scene : " << v->getScene();
        if(v->getScene())
            qDebug() << "View3dWindow scene rect after : " << v->getScene()->sceneRect();
    }
*/
 /*
    if (commandwindowfixed)
        SetWindowPos(subwindow->gethwnd(),HWND_BOTTOM,leftmenuareawidth,topmenuareaheight,w-rightmenuareawidth-leftmenuareawidth-1,h - promptheight*3 - bottommenuareaheight - topmenuareaheight,SWP_DRAWFRAME);
    else
        SetWindowPos(subwindow->gethwnd(),HWND_BOTTOM,0,0,w,h,SWP_DRAWFRAME);
        //SetWindowPos(subwindow->gethwnd(),HWND_BOTTOM,leftmenuareawidth,topmenuareaheight,w-rightmenuareawidth-leftmenuareawidth-1,h - bottommenuareaheight - topmenuareaheight,SWP_DRAWFRAME);

    if (layerwindow != 0 && GetParent(layerwindow->gethwnd()) == gethwnd())
    {   GetWindowRect(layerwindow->gethwnd(),&rect);
        if (options.test(9))
            SetWindowPos(layerwindow->gethwnd(),0,0,displayheight-promptheight,rect.right-rect.left,h-(displayheight),SWP_NOZORDER);
        else
            SetWindowPos(layerwindow->gethwnd(),0,0,displayheight,rect.right-rect.left,h-(displayheight),SWP_NOZORDER);
    }

    if (commandwindow != 0 && GetParent(commandwindow->gethwnd()) == gethwnd())
    {   GetWindowRect(commandwindow->gethwnd(),&rect);
        SetWindowPos(commandwindow->gethwnd(),0,0,int(getheight()-promptheight*3),int(getwidth()/2),promptheight*3,SWP_NOZORDER);
    }

    if(viewtab && viewproperties){
    viewtab->update_window_state();
    viewproperties->draw(DM_NORMAL);
    }
*/
    //qDebug() << "Leaving : MainWindow::sizeevent ";

}

void MainWindow::charevent(UINT c,DWORD buttons)
{
    qDebug() << "MainWindow::charevent";
    qDebug() << "focus widget is : " << (QWidget*)GetFocus();
    if(coordinateentry == 0)
        return;

    if(coordinateentry->containsWidget((QWidget*)GetFocus()))
    {
        //qDebug() << "focus widget " << ((QObject*)GetFocus())->objectName();

        if( c == 'P' || c == 'p' ||
            c == 'X' || c == 'x' ||
            c == 'Y' || c == 'y' ||
            c == 'Z' || c == 'z' ||
            c == 'R' || c == 'r' ||
            c == 'T' || c == 't' ||
            c == 'D' || c == 'd' ||
            c == 'A' || c == 'a' ||
            c == 'B' || c == 'b' ||
            c == 'L' || c == 'l' ||
            c == 'I' || c == 'i' ||
            c == 'M' || c == 'm' ||
            c == 'F' || c == 'f' ||
            c == '\\' ||
            c == '\t' || c == '\r' || c == '\033' || c == ' ')
        {
            qDebug() << "GetKeyState(VK_MENU)" << GetKeyState(VK_MENU);
            qDebug() << "GetKeyState(VK_CONTROL)" << GetKeyState(VK_CONTROL);
            // ignore the alt and control keys
            if ( (GetKeyState(VK_MENU) != (RCSHORT)0x8000) && (GetKeyState(VK_CONTROL) != (RCSHORT)0x8000))
                coordinateentry->charevent(c,buttons);
        }
    }
    else
    {
        // ignore the alt and control keys
        if ( (GetKeyState(VK_MENU) != (RCSHORT)0x8000) && (GetKeyState(VK_CONTROL) != (RCSHORT)0x8000))
            coordinateentry->charevent(c,buttons);

        // because Qt5.2 does not handle single key shortcuts
        // handle the delete key on the Mac here
        //qDebug() << "not a coord entry key in";
        if(c == VK_DELETE || c == VK_BACK)
        {
            //qDebug() << "VK_DELETE or VK_BACK was pressed";
            ((QtMainWindow*)gethwnd())->on_action_Edit_Delete_triggered();
            return;
        }
    }

    // KMJ: this line kicks the qt event loop
    ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();

}

#if VERSION != TRICAD_STUDENT
void MainWindow::addcadbasicwindow(CadBasicWindow *cbw)
{ cadbasiclist.add(cbw);
  selectwindow(cbw);
}

void MainWindow::delcadbasicwindow(CadBasicWindow *cbw)
{ if (cbw == curcadbasic)
    selectwindow(0);
  cadbasiclist.del(cbw);
}
#endif

void MainWindow::addfloatingmenuwindow(MenuWindow *mw)
{ floatingmenulist.add(mw);
}

void MainWindow::showfloatingmenu(int show)
{MenuWindow *mw;
  for (floatingmenulist.start() ; (mw = (MenuWindow *)floatingmenulist.next()) != 0 ; )
    ShowWindow(mw->gethwnd(),show ? SW_NORMAL : SW_HIDE);
}

int MainWindow::IsVisible(Control *control)
{MenuWindow *mw;
 Control *c;
 int i;

  for (i = 0 ; (c = getcontrol(i)) != 0 ; i++)
    if (c == control)
      return 1;

  for (floatingmenulist.start() ; (mw = (MenuWindow *)floatingmenulist.next()) != 0 ; )
    {  for (i = 0 ; (c = mw->getcontrol(i)) != 0 ; i++)
         if (c == control)
           return 1;
    }

  return 0;

}

void MainWindow::delfloatingmenuwindow(MenuWindow *mw)
{ floatingmenulist.del(mw);
  if (mw == floatingmenu1)
    {  buttonmenu(BM_MAINCOMMAND,NULL);
       buttonmenu(BM_SUBCOMMAND,NULL);
       floatingmenu1 = 0;
       options.clear(0);  options.set(1);
    }
  if (mw == floatingmenu2)
    {  buttonmenu(BM_MAININTERRUPT,NULL);
       buttonmenu(BM_SUBINTERRUPT,NULL);
       floatingmenu2 = 0;
       options.clear(2);  options.set(3);
    }
  if (mw == floatingmenu4)
    {  buttonmenu(BM_MAINAPPLICATION,NULL);
       buttonmenu(BM_SUBAPPLICATION,NULL);
       floatingmenu4 = 0;
       options.clear(6);  options.set(7);
    }
  if (mw == floatingmenu3)
    {  buttonmenu(BM_QUICK,NULL);
       floatingmenu3 = 0;
       options.clear(4);  options.set(5);
    }
  updatemenu();
}

void MainWindow::selectwindow(Window *window)
{
 View3dWindow *v,*newvwindow;
 CadBasicWindow *c,*newcwindow;
 CommandWindow *newkcwindow;

  ((RCCentralWidget*)((QtMainWindow*)gethwnd())->centerWidget)->debugView3dList();

  if (curview3d != window && curcadbasic != window && curcommand != window || window == 0)
    {
       newvwindow = 0;
       newcwindow = 0;
       newkcwindow = 0;
       for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
         if (window == v)
           {  newvwindow = v;
              view3dlist.del(v);
              view3dlist.add(v);
              //v->qsidx = view3dlist.length()-1;
              view3dlist.reverse();
              break;
           }
       for (cadbasiclist.start();  (c = (CadBasicWindow *) cadbasiclist.next()) != NULL ; )
         if (window == c)
           {  newcwindow = c;
              break;
           }
       if (window == commandwindow)
         newkcwindow = commandwindow;
       if (newcwindow == window || newvwindow == window || newkcwindow == window)
         {
#ifdef USING_WIDGETS

#else
           if (curview3d != NULL) SendMessage(curview3d->gethwnd(),WM_NCACTIVATE,0,0);
           if (curcadbasic != NULL) SendMessage(curcadbasic->gethwnd(),WM_NCACTIVATE,0,0);
           if (curcommand != NULL) SendMessage(curcommand->gethwnd(),WM_NCACTIVATE,0,0);
           if (window != NULL) SendMessage(window->gethwnd(),WM_NCACTIVATE,1,0);
#endif
           if (curview3d != NULL) SendMessage(curview3d->gethwnd(),WM_NCACTIVATE,0,0);
           if (window != NULL) SendMessage(window->gethwnd(),WM_NCACTIVATE,1,0);
            curview3d = newvwindow;
            curcadbasic = newcwindow;
            curcommand = newkcwindow;
         }
    }

  ((RCCentralWidget*)((QtMainWindow*)gethwnd())->centerWidget)->debugView3dList();
}


void MainWindow::setcurrentwindow(View3dWindow *cw)
{
#if 1
    // new
    curview3d = cw;
    if (curview3d != 0)
        SendMessage(curview3d->gethwnd(),WM_NCACTIVATE,1,0);
#else
    // old
    curview3d = cw;
    if (curview3d != 0)
        SendMessage(curview3d->gethwnd(),WM_NCACTIVATE,1,0);
#endif
}

void MainWindow::setcurrentsurface(View3dSurface *vs)
{
    View3dWindow *v=0;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        if(vs == v)
            break;
    }
    setcurrentwindow(v);
}


void MainWindow::movetoback(Window *window)
{View3dWindow *v;

  //  Always deactivate the window
  SendMessage(window->gethwnd(),WM_NCACTIVATE,0,0);

  for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    if (window == v)
      {  view3dlist.del(v);
         view3dlist.reverse();
         view3dlist.add(v);
         view3dlist.reverse();
         view3dlist.start();
         curview3d = view3dlist.next();
         SendMessage(curview3d->gethwnd(),WM_NCACTIVATE,1,0);
         break;
      }

}

#ifdef USING_WIDGETS
void MainWindow::updatemousedirection(int direction)
{
    View3dWindow *v;

    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
        v->setwheeldirection(direction);
}

void MainWindow::setantialiasing(int onoff)
{
    View3dWindow *v;

    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
        ((RCView*)v->gethwnd())->view()->setRenderHint(QPainter::Antialiasing, (bool)onoff);;
}

void QtMainWindow::onToolBarVisibilyChanged(bool visibility)
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    qDebug() << "QtMainWindow::onToolBarVisibilyChanged";
    qDebug() << "sender : " << sender();

    if(visibility)
    {
        if(sender()->objectName() == "PromptToolBar")
        {
              qDebug() << "PromptToolBar set visible ";
              cadwindow->addcontrol(cadwindow->prompt);
              settings.sync();
              settings.setValue("Layout/Prompt",((QToolBar*)sender())->saveGeometry());
        }
        else if(sender()->objectName() == "StatusToolBar")
        {
              qDebug() << "StatusToolBar set visible ";
              cadwindow->addcontrol(cadwindow->statusprompt);
              settings.sync();
              settings.setValue("Layout/StatusPrompt",((QToolBar*)sender())->saveGeometry());
        }
        else if(sender()->objectName() == "RCCoordinateEntry")
        {
              qDebug() << "RCCoordinateEntry set visible ";
              cadwindow->addcontrol(cadwindow->coordinateentry);
              settings.sync();
              settings.setValue("Layout/CoordinateEntry",((QToolBar*)sender())->saveGeometry());
        }
        else if(sender()->objectName() == "RCStyleEntry")
        {
              qDebug() << "RCStyleEntry set visible ";
              cadwindow->addcontrol(cadwindow->styleentry);
              settings.sync();
              settings.setValue("Layout/StyleEntry",((QToolBar*)sender())->saveGeometry());
        }
        else if(sender()->objectName() == "MainMenu")
        {
              qDebug() << "MainMenu set visible ";
              ButtonMenu *bm = buttonmenus.match("Entities");
              cadwindow->addcontrol(bm);
              cadwindow->SetMainCADMenu(bm);
              cadwindow->updatemenu();
              settings.sync();
              ((RCToolBox*)sender())->saveState();
        }
        else if(sender()->objectName() == "MainInterrupt")
        {
              qDebug() << "MainInterrupt set visible ";
              ButtonMenu *bm = buttonmenus.match("Entities");
              cadwindow->addcontrol(bm);
              cadwindow->SetInterruptCADMenu(bm);
              cadwindow->updatemenu();
              settings.sync();
              ((RCToolBox*)sender())->saveState();
        }
        else if(sender()->objectName() == "Quick")
        {
              qDebug() << "MainInterrupt set visible ";
              ButtonMenu *bm = buttonmenus.match("Quick");
              cadwindow->addcontrol(bm);
              cadwindow->SetQuickCADMenu(bm);
              cadwindow->updatemenu();
              settings.sync();
              ((RCToolBox*)sender())->saveState();
        }
    }
    else if(!visibility && cadwindow && !((QtMainWindow*)cadwindow->gethwnd())->isClosing())
    {

        if(sender()->objectName() == "PromptToolBar")
        {
            qDebug() << "PromptToolBar set invisible ";
            cadwindow->delcontrol(cadwindow->prompt);
            settings.sync();
            settings.remove("Layout/Prompt");
        }
        else if(sender()->objectName() == "StatusToolBar")
        {
            qDebug() << "StatusToolBar set invisible ";
            cadwindow->delcontrol(cadwindow->statusprompt);
            settings.sync();
            settings.remove("Layout/StatusPrompt");
        }
        else if(sender()->objectName() == "RCCoordinateEntry")
        {
            qDebug() << "RCCoordinateEntry set invisible ";
            cadwindow->delcontrol(cadwindow->coordinateentry);
            settings.sync();
            settings.remove("Layout/CoordinateEntry");
        }
        else if(sender()->objectName() == "RCCoordinateDisplay")
        {
            qDebug() << "RCCoordinateEntry set invisible ";
            cadwindow->delcontrol(cadwindow->coordinatedisplay);
            settings.sync();
            settings.remove("Layout/CoordinateDisplay");
        }
        else if(sender()->objectName() == "RCStyleEntry")
        {
            qDebug() << "RCStyleEntry set invisible ";
            cadwindow->delcontrol(cadwindow->styleentry);
            settings.sync();
            settings.remove("Layout/StyleEntry");
        }
        else if(sender()->objectName() == "MainMenu")
        {
            qDebug() << "MainMenu set invisible ";
            ButtonMenu *bm = buttonmenus.match("Entities");
            cadwindow->buttonmenu(BM_MAINCOMMAND,0);
            cadwindow->buttonmenu(BM_SUBCOMMAND,0);
            cadwindow->delcontrol(bm);
            settings.sync();
            settings.remove("Layout/MainMenu");
        }
        else if(sender()->objectName() == "MainInterrupt")
        {
            qDebug() << "MainInterrupt set invisible ";
            ButtonMenu *bm = buttonmenus.match("Assistant");
            cadwindow->buttonmenu(BM_MAININTERRUPT,0);
            cadwindow->buttonmenu(BM_SUBINTERRUPT,0);
            cadwindow->delcontrol(bm);
            settings.sync();
            settings.remove("Layout/MainInterrupt");
        }
        else if(sender()->objectName() == "Quick")
        {
            qDebug() << "MainInterrupt set invisible ";
            ButtonMenu *bm = buttonmenus.match("Quick");
            cadwindow->buttonmenu(BM_QUICK,0);
            cadwindow->delcontrol(bm);
            settings.sync();
            settings.remove("Layout/Quick");
        }
    }
}

void MainWindow::showSplashScreen()
{
    if(gethwnd() != 0)
        ((QtMainWindow*)gethwnd())->showSplashScreen();
}

void MainWindow::hideSplashScreen()
{
    if(gethwnd() != 0)
        ((QtMainWindow*)gethwnd())->hideSplashScreen();
}

void MainWindow::deleteSpashScreen()
{
    if(gethwnd() != 0)
        ((QtMainWindow*)gethwnd())->deleteSplashScreen();
}

void MainWindow::setRefreshDelay(int delay)
{
    View3dWindow *v,*newvwindow;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        ((RCGraphicsView*)((RCView*)v->gethwnd())->view())->setRefreshDelay(delay);
    }
}

void MainWindow::setReduceDetail(int detail)
{
    View3dWindow *v,*newvwindow;
    for (view3dlist.start();  (v = view3dlist.next()) != NULL ; )
    {
        ((RCGraphicsView*)((RCView*)v->gethwnd())->view())->setReduceDetail(detail);
    }
}

bool MainWindow::isMinimized()
{
    // KMJ using new c++ static cast here
    if(static_cast<QMainWindow*>(gethwnd()) != Q_NULLPTR)
        return static_cast<QMainWindow*>(gethwnd())->isMinimized();

    return false;
}

#endif


