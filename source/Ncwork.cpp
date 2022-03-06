
#include "ncwin.h"

Workplane::Workplane(RCCHAR *n,double ox,double oy,double oz,double xax,double xay,double xaz,double yax,double yay,double yaz,double v)
{
#ifdef USING_WIDGETS
    qgi = 0;
#endif

  strcpy(name,n);
  origin.setxyz(ox,oy,oz);
  xaxis.setxyz(xax,xay,xaz);   xaxis.normalize();
  yaxis.setxyz(yax,yay,yaz);   yaxis.normalize();
  zaxis = xaxis.cross(yaxis);
  defined = (xaxis.cross(yaxis)).length() > db.getptoler();
  visible = v;
}

Workplane::Workplane(RCCHAR *n,int workplane,double v)
{
#ifdef USING_WIDGETS
    qgi = 0;
#endif

  strcpy(name,n);
  standard(workplane);
  visible = v;
}

void Workplane::standard(int workplane)
{
    switch (workplane)
    {  case 0 :
         origin.setxyz( 0.0, 0.0, 0.0);
         xaxis.setxyz ( 1.0, 0.0, 0.0);
         yaxis.setxyz ( 0.0, 1.0, 0.0);
         break;
       case 1 :
         origin.setxyz( 0.0, 0.0, 0.0);
         xaxis.setxyz (-1.0, 0.0, 0.0);
         yaxis.setxyz ( 0.0, 1.0, 0.0);
         break;
       case 2 :
         origin.setxyz( 0.0, 0.0, 0.0);
         xaxis.setxyz ( 1.0, 0.0, 0.0);
         yaxis.setxyz ( 0.0, 0.0, 1.0);
         break;
       case 3 :
         origin.setxyz(  0.0, 0.0, 0.0);
         xaxis.setxyz ( -1.0, 0.0, 0.0);
         yaxis.setxyz (  0.0, 0.0, 1.0);
         break;
       case 4 :
         origin.setxyz(  0.0, 0.0, 0.0);
         xaxis.setxyz (  0.0,-1.0, 0.0);
         yaxis.setxyz (  0.0, 0.0, 1.0);
         break;
       case 5 :
         origin.setxyz( 0.0, 0.0, 0.0);
         xaxis.setxyz ( 0.0, 1.0, 0.0);
         yaxis.setxyz ( 0.0, 0.0, 1.0);
         break;
       case 6 :
         origin.setxyz( 0.0, 0.0, 0.0);
         xaxis.setxyz ( 1.0,-1.0, 0.0);
         yaxis.setxyz (-1.0,-1.0, 2.0);
    }
  xaxis.normalize();
  yaxis.normalize();
  zaxis = xaxis.cross(yaxis);
  defined = 1;
}

Workplane::Workplane(RCCHAR *n,Point org,Point xa,Point ya,double v)
{
#ifdef USING_WIDGETS
    qgi = 0;
#endif

  strcpy(name,n);
  origin = org;
  xaxis = xa;  xaxis.normalize();
  yaxis = ya;  yaxis.normalize();
  zaxis = xaxis.cross(yaxis);
  defined = (xaxis.cross(yaxis)).length() > db.getptoler();
  visible = v;
}

int Workplane::change(Point org,Point xa,Point ya)
{
    Point za;
  xa.normalize();  ya.normalize();
  za = xa.cross(ya);
  if (za.length() < 0.99999) return 0;
  origin = org;
  xaxis = xa;
  yaxis = ya;
  zaxis = xaxis.cross(yaxis);
  defined = 1;
  return 1;
}

Workplane *Workplane::load(CadFile *infile)
{
 char *cname;
 RCCHAR *name;
 Point origin,xaxis,yaxis;
 double visible;
 Workplane *wp;
 if( infile->getversion() == 2)
 {
     if (! infile->read(&cname)) return NULL;
     name = new RCCHAR[strlen(cname)+1];
     strcpy(name,cname);
     delete [] cname;
 }
 else
 {
     if (! infile->read(&name))
         return NULL;
 }

  if (! origin.load(infile)) return NULL;
  if (! xaxis.load(infile)) return NULL;
  if (! yaxis.load(infile)) return NULL;
  if (! infile->read(&visible)) return NULL;
  wp = new Workplane(name,origin,xaxis,yaxis,visible);
  delete [] name;
  return wp;

}

int Workplane::save(CadFile *outfile)
{
    if (! outfile->write(name)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&visible)) return 0;
  return 1;
}

int Workplane::load(UndoFile *infile)
{
  RCCHAR *s;
  if (! infile->read(&s)) return 0;  strcpy(name,s);  delete s;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&visible)) return 0;
  return 1;
}

int Workplane::save(UndoFile *outfile)
{
    if (! outfile->write(name)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&visible)) return 0;
  return 1;
}

Point Workplane::workplanetomodel(Point p)
{
    return xaxis * p.x + yaxis * p.y + zaxis * p.z + origin;
}

Point Workplane::modeltoworkplane(Point p)
{
    Point tmp(xaxis.dot(p)-xaxis.dot(origin),
           yaxis.dot(p)-yaxis.dot(origin),
           zaxis.dot(p)-zaxis.dot(origin));
  return tmp;
}

#ifdef USING_WIDGETS
void drawworkplane(int drawmode,void *workplane,View3dSurface *window)
{
    // don't need to do anything here if using QPainter
    if (workplane != NULL && window->getsurfacetype() != View2dSurf)
      ((Workplane *)workplane)->draw(drawmode,window);
}
#else
void drawworkplane(int drawmode,void *workplane,View3dSurface *window)
{
    if (workplane != NULL && window->getsurfacetype() != View2dSurf)
      ((Workplane *)workplane)->draw(drawmode,window);
}
#endif

#ifdef USING_WIDGETS
void Workplanes::setcurrent(Workplane *w)
{
    Workplane *wp;

    if (current != w)
    {
        current = w;

        if (cadwindow != 0)
            cadwindow->updatetitle();
        state.setposition(state.getposition());
    }
}
#else
void Workplanes::setcurrent(Workplane *w)
{
    Workplane *wp;

  if (current == w)
    return;

  for (list.start() ; (wp = list.next()) != NULL ; )
    if (! wp->getvisible() && cadwindow != 0)
      cadwindow->drawcallback(DM_ERASE,wp,drawworkplane);
  if (current != NULL && cadwindow != 0)
    {  cadwindow->drawcallback(DM_ERASE,current,drawworkplane);
       if (db.grid.getplane() == onworkplane && db.grid.display)
         cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
    }
  current = w;
  if (current != NULL && cadwindow != 0)
    {  cadwindow->drawcallback(DM_NORMAL,current,drawworkplane);
       if (db.grid.getplane() == onworkplane && db.grid.display)
         cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
    }
  for (list.start() ; (wp = list.next()) != NULL ; )
    if (wp->getvisible() && cadwindow != 0)
      cadwindow->drawcallback(wp == current ? DM_NORMAL : DM_SELECT,wp,drawworkplane);

  if (cadwindow != 0)
    cadwindow->updatetitle();
  state.setposition(state.getposition());
}
#endif

#ifdef USING_WIDGETS
void Workplane::draw(int drawmode, View3dSurface *surface )
{
  double s;
  Point xax,yax,zax;
  View3dWindow *window;

  //qDebug() <<  surface->getScene();

  if(qgi && qgi->scene() == surface->getScene())
      surface->getScene()->removeItem(qgi);
  delete qgi;
  qgi=0;

  if (v.getinteger("wp::display") == 0)
    return;

  if(drawmode == DM_ERASE)
      return;

  QGraphicsItem *qgout = 0, *qgin=0; //new QGraphicsItemGroup();
  QList<QGraphicsItem *> glist;

  if (drawmode == DM_NORMAL) drawmode = DM_GRID;

/*
  window = dynamic_cast<View3dWindow *> (surface);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    surface = window->getoffscreenbitmap();
*/

  //s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#ifdef _MAC
  //s = 0.17 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#else
  //s = 0.3 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#endif
  xax = xaxis * s;
  yax = yaxis * s;
  zax = xaxis.cross(yaxis) * s;

  surface->setup(1.0,0.0,drawmode,0,0,0,0);
  qgout = surface->line3d(qgin,origin+xax* 10.0+yax*10.0,origin+xax*100.0+yax*10.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*100.0+yax*10.0,origin+xax*100.0+yax*20.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*100.0+yax*20.0,origin+xax*120.0+yax* 0.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*120.0+yax* 0.0,origin+xax*100.0-yax*20.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*100.0-yax*20.0,origin+xax*100.0-yax*10.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*100.0-yax*10.0,origin-xax* 10.0-yax*10.0); if( qgout != 0 ) glist.append( qgout );

  qgout = surface->line3d(qgin,origin+yax* 10.0+xax*10.0,origin+yax*100.0+xax*10.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*100.0+xax*10.0,origin+yax*100.0+xax*20.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*100.0+xax*20.0,origin+yax*120.0+xax* 0.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*120.0+xax* 0.0,origin+yax*100.0-xax*20.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*100.0-xax*20.0,origin+yax*100.0-xax*10.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*100.0-xax*10.0,origin-yax* 10.0-xax*10.0); if( qgout != 0 ) glist.append( qgout );

  if (v.getinteger("wp::zdisplay") != 0)
  {
       qgout = surface->line3d(qgin,origin+yax* 10.0+xax*10.0,origin+yax*10.0+xax*10.0+zax*100.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin+yax* 10.0+xax*10.0+zax*100.0,origin+yax*20.0+xax*20.0+zax*100.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin+yax* 20.0+xax*20.0+zax*100.0,origin+yax*0.0+xax* 0.0+zax*120.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin+yax* 0.0 +xax* 0.0+zax*120.0,origin-yax*20.0-xax*20.0+zax*100.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin-yax*20.0-xax*20.0+zax*100.0,origin-yax*10.0-xax*10.0+zax*100.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin-yax*10.0-xax*10.0+zax*100.0,origin-yax* 10.0-xax*10.0); if( qgout != 0 ) glist.append( qgout );

       surface->setup(1.0,0.0,drawmode,0,0,2,0);
 
       qgout = surface->line3d(qgin,origin-xax*5.0-yax*5.5+zax*105.0,origin+xax*5.0+yax*5.0+zax*105.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin+xax*5.0+yax*5.5+zax*105.0,origin-xax*5.0-yax*5.0+zax*95.0); if( qgout != 0 ) glist.append( qgout );
       qgout = surface->line3d(qgin,origin-xax*5.0-yax*5.5+zax*95.0,origin+xax*5.0+yax*5.0+zax*95.0); if( qgout != 0 ) glist.append( qgout );
  }

  surface->setup(1.0,0.0,drawmode,0,0,2,0);
  qgout = surface->line3d(qgin,origin+yax* 92.0,origin+yax*100.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*108.0+xax*4.0,origin+yax*100.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+yax*108.0-xax*4.0,origin+yax*100.0); if( qgout != 0 ) glist.append( qgout );

  qgout = surface->line3d(qgin,origin+xax*96.0-yax*7.0,origin+xax*100.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*96.0+yax*7.0,origin+xax*100.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*104.0-yax*7.0,origin+xax*100.0); if( qgout != 0 ) glist.append( qgout );
  qgout = surface->line3d(qgin,origin+xax*104.0+yax*7.0,origin+xax*100.0); if( qgout != 0 ) glist.append( qgout );

  if(glist.size() > 0)
     qgi = surface->getScene()->createItemGroup(glist);
  if(qgi)
  {
      qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));

      if(drawmode == DM_INVERT)
      {
          if(surface->qgirb == 0)
          {
              //qDebug() << "surface : " << vs->gethwnd();
              surface->qgirb = new QGraphicsItemGroup();
              surface->getScene()->addItem(surface->qgirb);
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);
          }
          else
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);
          qgi=0;
      }
  }
}
#else
void Workplane::draw(int drawmode,View3dSurface *surface)
{
  double s;
  Point xax,yax,zax;
  View3dWindow *window;
  if (v.getinteger("wp::display") == 0)
    return;

  if (drawmode == DM_NORMAL) drawmode = DM_GRID;

  window = dynamic_cast<View3dWindow *> (surface);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    surface = window->getoffscreenbitmap();

  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  xax = xaxis * s;
  yax = yaxis * s;
  zax = xaxis.cross(yaxis) * s;

  surface->setup(1.0,0.0,drawmode,0,0,0,0);
  surface->line3d(origin+xax* 10.0+yax*10.0,origin+xax*100.0+yax*10.0);
  surface->line3d(origin+xax*100.0+yax*10.0,origin+xax*100.0+yax*20.0);
  surface->line3d(origin+xax*100.0+yax*20.0,origin+xax*120.0+yax* 0.0);
  surface->line3d(origin+xax*120.0+yax* 0.0,origin+xax*100.0-yax*20.0);
  surface->line3d(origin+xax*100.0-yax*20.0,origin+xax*100.0-yax*10.0);
  surface->line3d(origin+xax*100.0-yax*10.0,origin-xax* 10.0-yax*10.0);

  surface->line3d(origin+yax* 10.0+xax*10.0,origin+yax*100.0+xax*10.0);
  surface->line3d(origin+yax*100.0+xax*10.0,origin+yax*100.0+xax*20.0);
  surface->line3d(origin+yax*100.0+xax*20.0,origin+yax*120.0+xax* 0.0);
  surface->line3d(origin+yax*120.0+xax* 0.0,origin+yax*100.0-xax*20.0);
  surface->line3d(origin+yax*100.0-xax*20.0,origin+yax*100.0-xax*10.0);
  surface->line3d(origin+yax*100.0-xax*10.0,origin-yax* 10.0-xax*10.0);

  if (v.getinteger("wp::zdisplay") != 0)
    {  surface->line3d(origin+yax* 10.0+xax*10.0,origin+yax*10.0+xax*10.0+zax*100.0);
       surface->line3d(origin+yax* 10.0+xax*10.0+zax*100.0,origin+yax*20.0+xax*20.0+zax*100.0);
       surface->line3d(origin+yax* 20.0+xax*20.0+zax*100.0,origin+yax*0.0+xax* 0.0+zax*120.0);
       surface->line3d(origin+yax* 0.0 +xax* 0.0+zax*120.0,origin-yax*20.0-xax*20.0+zax*100.0);
       surface->line3d(origin-yax*20.0-xax*20.0+zax*100.0,origin-yax*10.0-xax*10.0+zax*100.0);
       surface->line3d(origin-yax*10.0-xax*10.0+zax*100.0,origin-yax* 10.0-xax*10.0);

       surface->setup(1.0,0.0,drawmode,0,0,2,0);

       surface->line3d(origin-xax*5.0-yax*5.5+zax*105.0,origin+xax*5.0+yax*5.0+zax*105.0);
       surface->line3d(origin+xax*5.0+yax*5.5+zax*105.0,origin-xax*5.0-yax*5.0+zax*95.0);
       surface->line3d(origin-xax*5.0-yax*5.5+zax*95.0,origin+xax*5.0+yax*5.0+zax*95.0);
    }

  surface->setup(1.0,0.0,drawmode,0,0,2,0);
  surface->line3d(origin+yax* 92.0,origin+yax*100.0);
  surface->line3d(origin+yax*108.0+xax*4.0,origin+yax*100.0);
  surface->line3d(origin+yax*108.0-xax*4.0,origin+yax*100.0);

  surface->line3d(origin+xax*96.0-yax*7.0,origin+xax*100.0);
  surface->line3d(origin+xax*96.0+yax*7.0,origin+xax*100.0);
  surface->line3d(origin+xax*104.0-yax*7.0,origin+xax*100.0);
  surface->line3d(origin+xax*104.0+yax*7.0,origin+xax*100.0);

  // KMJ : TODO : fix this update to not crash
  //if (window != 0)
  //  window->UpdateWindow();

}
#endif

#ifdef USING_WIDGETS
#if 1
void Workplane::drawGL(int drawmode,View3dSurface *surface)
{
  double s;
  Point xax,yax,zax;
  View3dWindow *window;
  if (v.getinteger("wp::display") == 0)
    return;

  //if(painter == 0) return;

  //if(drawmode == DM_ERASE)
  //    return;

  if (drawmode == DM_NORMAL) drawmode = DM_GRID;


  window = dynamic_cast<View3dWindow *> (surface);

  if(window == 0)
      return;

  //if (window != 0 && window->getoffscreenbitmap() != 0)
  //  surface = window->getoffscreenbitmap();

  //s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#ifdef _MAC
  //s = 0.17 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#else
  //s = 0.3 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#endif

  xax = xaxis * s;
  yax = yaxis * s;
  zax = xaxis.cross(yaxis) * s;

  surface->setupGL(1.0,0.0,drawmode,0,0,0,0);

  RCCOLORREF pcol;
  if (surface != 0)
      pcol = surface->getcolour();
  GLfloat fr=GetRValue(pcol)/255.0;
  GLfloat fg=GetGValue(pcol)/255.0;
  GLfloat fb=GetBValue(pcol)/255.0;
  glColor3f(fr,fg,fb);
  //painter->setPen(surface->curPen);
  GLfloat lwidth = surface->curPen.widthF();
  glLineWidth(lwidth<1.0?1.0:lwidth);


  surface->line3dGL(origin+xax* 10.0+yax*10.0,origin+xax*100.0+yax*10.0);
  surface->line3dGL(origin+xax*100.0+yax*10.0,origin+xax*100.0+yax*20.0);
  surface->line3dGL(origin+xax*100.0+yax*20.0,origin+xax*120.0+yax* 0.0);
  surface->line3dGL(origin+xax*120.0+yax* 0.0,origin+xax*100.0-yax*20.0);
  surface->line3dGL(origin+xax*100.0-yax*20.0,origin+xax*100.0-yax*10.0);
  surface->line3dGL(origin+xax*100.0-yax*10.0,origin-xax* 10.0-yax*10.0);

  surface->line3dGL(origin+yax* 10.0+xax*10.0,origin+yax*100.0+xax*10.0);
  surface->line3dGL(origin+yax*100.0+xax*10.0,origin+yax*100.0+xax*20.0);
  surface->line3dGL(origin+yax*100.0+xax*20.0,origin+yax*120.0+xax* 0.0);
  surface->line3dGL(origin+yax*120.0+xax* 0.0,origin+yax*100.0-xax*20.0);
  surface->line3dGL(origin+yax*100.0-xax*20.0,origin+yax*100.0-xax*10.0);
  surface->line3dGL(origin+yax*100.0-xax*10.0,origin-yax* 10.0-xax*10.0);

  if (v.getinteger("wp::zdisplay") != 0)
  {
       surface->line3dGL(origin+yax* 10.0+xax*10.0,origin+yax*10.0+xax*10.0+zax*100.0);
       surface->line3dGL(origin+yax* 10.0+xax*10.0+zax*100.0,origin+yax*20.0+xax*20.0+zax*100.0);
       surface->line3dGL(origin+yax* 20.0+xax*20.0+zax*100.0,origin+yax*0.0+xax* 0.0+zax*120.0);
       surface->line3dGL(origin+yax* 0.0 +xax* 0.0+zax*120.0,origin-yax*20.0-xax*20.0+zax*100.0);
       surface->line3dGL(origin-yax*20.0-xax*20.0+zax*100.0,origin-yax*10.0-xax*10.0+zax*100.0);
       surface->line3dGL(origin-yax*10.0-xax*10.0+zax*100.0,origin-yax* 10.0-xax*10.0);

       surface->setup(1.0,0.0,drawmode,0,0,2,0);
       if (surface != 0)
           pcol = surface->getcolour();
       fr=GetRValue(pcol)/255.0;
       fg=GetGValue(pcol)/255.0;
       fb=GetBValue(pcol)/255.0;
       glColor3f(fr,fg,fb);
       //painter->setPen(surface->curPen);

       surface->line3dGL(origin-xax*5.0-yax*5.5+zax*105.0,origin+xax*5.0+yax*5.0+zax*105.0);
       surface->line3dGL(origin+xax*5.0+yax*5.5+zax*105.0,origin-xax*5.0-yax*5.0+zax*95.0);
       surface->line3dGL(origin-xax*5.0-yax*5.5+zax*95.0,origin+xax*5.0+yax*5.0+zax*95.0);
  }

  surface->setupGL(1.0,0.0,drawmode,0,0,2,0);

  if (surface != 0)
      pcol = surface->getcolour();
  fr=GetRValue(pcol)/255.0;
  fg=GetGValue(pcol)/255.0;
  fb=GetBValue(pcol)/255.0;
  glColor3f(fr,fg,fb);
  //painter->setPen(surface->curPen);
  lwidth = surface->curPen.widthF();
  glLineWidth(lwidth<1.0?1.0:lwidth);


  //painter->setPen(surface->curPen);
  surface->line3dGL(origin+yax* 92.0,origin+yax*100.0);
  surface->line3dGL(origin+yax*108.0+xax*4.0,origin+yax*100.0);
  surface->line3dGL(origin+yax*108.0-xax*4.0,origin+yax*100.0);

  surface->line3dGL(origin+xax*96.0-yax*7.0,origin+xax*100.0);
  surface->line3dGL(origin+xax*96.0+yax*7.0,origin+xax*100.0);
  surface->line3dGL(origin+xax*104.0-yax*7.0,origin+xax*100.0);
  surface->line3dGL(origin+xax*104.0+yax*7.0,origin+xax*100.0);

}
#endif

void Workplane::draw(int drawmode,View3dSurface *surface, QPainter *painter)
{
  double s;
  Point xax,yax,zax;
  View3dWindow *window;
  if (v.getinteger("wp::display") == 0)
    return;

  if(painter == 0) return;

  //if(drawmode == DM_ERASE)
  //    return;

  if (drawmode == DM_NORMAL) drawmode = DM_GRID;

  //window = dynamic_cast<View3dWindow *> (surface);
  //if (window != 0 && window->getoffscreenbitmap() != 0)
  //  surface = window->getoffscreenbitmap();

  //s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#ifdef _MAC
  //s = 0.17 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#else
  //s = 0.3 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#endif
  xax = xaxis * s;
  yax = yaxis * s;
  zax = xaxis.cross(yaxis) * s;

  surface->setup(1.0,0.0,drawmode,0,0,0,0);

  painter->setPen(surface->curPen);

  surface->line3d(painter,origin+xax* 10.0+yax*10.0,origin+xax*100.0+yax*10.0);
  surface->line3d(painter,origin+xax*100.0+yax*10.0,origin+xax*100.0+yax*20.0);
  surface->line3d(painter,origin+xax*100.0+yax*20.0,origin+xax*120.0+yax* 0.0);
  surface->line3d(painter,origin+xax*120.0+yax* 0.0,origin+xax*100.0-yax*20.0);
  surface->line3d(painter,origin+xax*100.0-yax*20.0,origin+xax*100.0-yax*10.0);
  surface->line3d(painter,origin+xax*100.0-yax*10.0,origin-xax* 10.0-yax*10.0);

  surface->line3d(painter,origin+yax* 10.0+xax*10.0,origin+yax*100.0+xax*10.0);
  surface->line3d(painter,origin+yax*100.0+xax*10.0,origin+yax*100.0+xax*20.0);
  surface->line3d(painter,origin+yax*100.0+xax*20.0,origin+yax*120.0+xax* 0.0);
  surface->line3d(painter,origin+yax*120.0+xax* 0.0,origin+yax*100.0-xax*20.0);
  surface->line3d(painter,origin+yax*100.0-xax*20.0,origin+yax*100.0-xax*10.0);
  surface->line3d(painter,origin+yax*100.0-xax*10.0,origin-yax* 10.0-xax*10.0);

  if (v.getinteger("wp::zdisplay") != 0)
  {
       surface->line3d(painter,origin+yax* 10.0+xax*10.0,origin+yax*10.0+xax*10.0+zax*100.0);
       surface->line3d(painter,origin+yax* 10.0+xax*10.0+zax*100.0,origin+yax*20.0+xax*20.0+zax*100.0);
       surface->line3d(painter,origin+yax* 20.0+xax*20.0+zax*100.0,origin+yax*0.0+xax* 0.0+zax*120.0);
       surface->line3d(painter,origin+yax* 0.0 +xax* 0.0+zax*120.0,origin-yax*20.0-xax*20.0+zax*100.0);
       surface->line3d(painter,origin-yax*20.0-xax*20.0+zax*100.0,origin-yax*10.0-xax*10.0+zax*100.0);
       surface->line3d(painter,origin-yax*10.0-xax*10.0+zax*100.0,origin-yax* 10.0-xax*10.0);

       surface->setup(1.0,0.0,drawmode,0,0,2,0);
       painter->setPen(surface->curPen);

       surface->line3d(painter,origin-xax*5.0-yax*5.5+zax*105.0,origin+xax*5.0+yax*5.0+zax*105.0);
       surface->line3d(painter,origin+xax*5.0+yax*5.5+zax*105.0,origin-xax*5.0-yax*5.0+zax*95.0);
       surface->line3d(painter,origin-xax*5.0-yax*5.5+zax*95.0,origin+xax*5.0+yax*5.0+zax*95.0);
  }

  surface->setup(1.0,0.0,drawmode,0,0,2,0);
  painter->setPen(surface->curPen);

  surface->line3d(painter,origin+yax* 92.0,origin+yax*100.0);
  surface->line3d(painter,origin+yax*108.0+xax*4.0,origin+yax*100.0);
  surface->line3d(painter,origin+yax*108.0-xax*4.0,origin+yax*100.0);

  surface->line3d(painter,origin+xax*96.0-yax*7.0,origin+xax*100.0);
  surface->line3d(painter,origin+xax*96.0+yax*7.0,origin+xax*100.0);
  surface->line3d(painter,origin+xax*104.0-yax*7.0,origin+xax*100.0);
  surface->line3d(painter,origin+xax*104.0+yax*7.0,origin+xax*100.0);

}
#endif

int Workplane::pick(int x,int y,View3dSurface *surface)
{double s;
 Point xax,yax;

  if (v.getinteger("wp::display") == 0)
    return 0;

  //s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#ifdef _MAC
  //s = 0.17 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#else
  //s = 0.3 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
  s = 0.2 * (surface->getumax() - surface->getumin()) / surface->getwidthmm();
#endif
  xax = xaxis * s;
  yax = yaxis * s;

  if (surface->pick(origin+xax* 10.0+yax*10.0,origin+xax*100.0+yax*10.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*100.0+yax*10.0,origin+xax*100.0+yax*20.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*100.0+yax*20.0,origin+xax*120.0+yax* 0.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*120.0+yax* 0.0,origin+xax*100.0-yax*20.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*100.0-yax*20.0,origin+xax*100.0-yax*10.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*100.0-yax*10.0,origin-xax* 10.0-yax*10.0,x,y,0) >= 0.0) return 1;

  if (surface->pick(origin+yax* 10.0+xax*10.0,origin+yax*100.0+xax*10.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*100.0+xax*10.0,origin+yax*100.0+xax*20.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*100.0+xax*20.0,origin+yax*120.0+xax* 0.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*120.0+xax* 0.0,origin+yax*100.0-xax*20.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*100.0-xax*20.0,origin+yax*100.0-xax*10.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*100.0-xax*10.0,origin-yax* 10.0-xax*10.0,x,y,0) >= 0.0) return 1;

  if (surface->pick(origin+yax* 92.0,origin+yax*100.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*108.0+xax*4.0,origin+yax*100.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+yax*108.0-xax*4.0,origin+yax*100.0,x,y,0) >= 0.0) return 1;

  if (surface->pick(origin+xax*96.0-yax*7.0,origin+xax*100.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*96.0+yax*7.0,origin+xax*100.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*104.0-yax*7.0,origin+xax*100.0,x,y,0) >= 0.0) return 1;
  if (surface->pick(origin+xax*104.0+yax*7.0,origin+xax*100.0,x,y,0) >= 0.0) return 1;

  return 0;

}

Workplane *Workplanes::match(const char *name)
{
    return match(CHAR2RCCHR(name));
}

Workplane *Workplanes::match(const RCCHAR *name)
{Workplane *w;
  
  if (strcmp(name,"RC::Dynamic") == 0)
    return & dynamic;

  for (list.start() ; (w = list.next()) != NULL ; )
    if (strcmp(w->getname(),name) == 0) 
      return w;

  return NULL;
}

Workplane *Workplanes::match(int index)
{Workplane *w;

  if (index == -1)
    return & dynamic;

  for (list.start() ; (w = list.next()) != NULL ; )
    if (index == 0)
      return w;
    else
       index--;

  return NULL;
}

Workplane *Workplanes::match(Workplane *workplane)
{Workplane *w;

  if (workplane == & dynamic)
    return &dynamic;

  for (list.start() ; (w = list.next()) != NULL ; )
    if (workplane == w) 
      return w;

  return NULL;
}

void Workplanes::names(RCCHAR ***namelist,int *n,int *index)
{Workplane *w;
  for (list.start(),*n = 0 ; (w = list.next()) != NULL ; (*n)++);
  if ((*namelist = new RCCHAR *[*n+20]) != NULL)  //  Extra 20 bytes for Display On/Display Off Message
    {  *index = *n - 1;
       for (list.start(),*n = 0 ; (w = list.next()) != NULL ; (*n)++)
         {  (*namelist)[*n] = w->getname();
            if (current == w && index != 0) *index = *n;
         }
    }
}

void Workplanes::clear(void)
{Workplane *w;
  setcurrent(NULL);
  for (list.start() ; (w = list.next()) != NULL ; ) delete w;
  list.destroy();
}

void Workplanes::initialize(void)
{ResourceString rs0(NCWORK);
 ResourceString rs1(NCWORK+1);
 ResourceString rs2(NCWORK+2);
 ResourceString rs3(NCWORK+3);
 ResourceString rs4(NCWORK+4);
 ResourceString rs5(NCWORK+5);
 ResourceString rs6(NCWORK+6);
  clear();
  add(new Workplane(rs0.gets(),0,0.0));
  add(new Workplane(rs1.gets(),1,0.0));
  add(new Workplane(rs2.gets(),2,0.0));
  add(new Workplane(rs3.gets(),3,0.0));
  add(new Workplane(rs4.gets(),4,0.0));
  add(new Workplane(rs5.gets(),5,0.0));
  add(new Workplane(rs6.gets(),6,0.0));
  setcurrent(db.workplanes.match(rs0.gets()));
}

int Workplanes::load(CadFile *infile,Database *database)
{Workplane *w,*cw;
 short nworkplanes;
 int i;
 char *cname;
 RCCHAR *name;
  database->workplanes.setcurrent(NULL);
  if (! infile->read(&nworkplanes)) return 0;
  w = NULL;
  for (i = 0 ; i < nworkplanes ; i++)
    {  if ((w = Workplane::load(infile)) == NULL) return 0;
       database->workplanes.add(w);
    }
  if( infile->getversion() == 2 )
  {
      if (! infile->read(&cname)) return 0;
      name = new RCCHAR[strlen(cname)+1];
      strcpy(name,cname);
      delete [] cname;
  }
  else
  {
      if (! infile->read(&name))
          return 0;
  }

  if ((cw = database->workplanes.match(name)) != NULL)
    database->workplanes.setcurrent(cw);
  else
    database->workplanes.setcurrent(w);
  database->workplanes.list.reverse();
  delete [] name;
  return 1;
}

int Workplanes::save(CadFile *outfile)
{Workplane *w;
 short type,nworkplanes;
  type = workplane_record;
  if (! outfile->write(&type)) return 0;
  for (list.start(),nworkplanes = 0 ; list.next() != NULL ; nworkplanes++);
  if (! outfile->write(&nworkplanes)) return 0;
  for (list.start() ; (w = list.next()) != NULL ; )
    if (! w->save(outfile)) return 0;
  if (db.workplanes.getcurrent() == NULL)
    {  if (! outfile->write("")) return 0;
    }
  else
    {  if (! outfile->write(current->getname())) return 0;
    }
  return 1;
}

#ifdef USING_WIDGETS
void Workplanes::drawGL(int drawmode,View3dSurface *surface)
{
  Workplane *w;
  if (drawmode == DM_NORMAL)
    {  for (list.start() ; (w = list.next()) != NULL ; )
         if (w->getvisible())
           w->drawGL(DM_DASH,surface);
    }
  if (current != 0)
    current->drawGL(drawmode,surface);
}

void Workplanes::draw(int drawmode,View3dSurface *surface,  QPainter *painter)
{
  Workplane *w;
  if (drawmode == DM_NORMAL)
    {  for (list.start() ; (w = list.next()) != NULL ; )
         if (w->getvisible())
           w->draw(DM_SELECT,surface,painter);
    }
  if (current != 0)
    current->draw(drawmode,surface,painter);
}

void Workplanes::clearScene(int drawmode,View3dSurface *surface)
{
  Workplane *w;
  for (list.start() ; (w = list.next()) != NULL ; )
      if (w->getvisible())
          w->draw(drawmode,surface);
  if (current != 0)
    current->draw(drawmode,surface);
}

void Workplanes::clearScene(int drawmode,RCView3dPrinter *surface)
{
  Workplane *w;
  for (list.start() ; (w = list.next()) != NULL ; )
      if (w->getvisible())
          w->draw(drawmode,surface);
  if (current != 0)
    current->draw(drawmode,surface);
}

#endif

void Workplanes::draw(int drawmode,View3dSurface *surface)
{
  Workplane *w;
  if (drawmode == DM_NORMAL)
    {  for (list.start() ; (w = list.next()) != NULL ; )
         if (w->getvisible())
           w->draw(DM_SELECT,surface);
    }
  if (current != 0)
    current->draw(drawmode,surface);
}


void Workplanes::pick(int verify,int x,int y,View3dSurface *surface)
{Workplane *w;
  for (list.start() ; (w = list.next()) != NULL ; )
    if ((w->getvisible() || w == current) && w->pick(x,y,surface))
      {  if (verify)
           {  NewCommandEvent *nevent = new NewCommandEvent(402,0);
              state.sendevent(nevent);
              break;
           }
         else
           setcurrent(w);
      }
}

class WPButtonDialogControl : public ButtonDialogControl
{private:
   int n;
   RCCHAR **namelist;
 public:
   WPButtonDialogControl(int id,int n1,RCCHAR **nl) : ButtonDialogControl(id) {  n = n1;  namelist = nl;  }
   int select(Dialog *);
};

int WPButtonDialogControl::select(Dialog *dialog)
{int i,j;
  if (dialog->currentvalue(100,&i))
    {  if (id == 101)
         {  if (strstr(namelist[i]," - Display Off") != 0 && strstr(namelist[i]," - Display Off") - namelist[i] == (signed)strlen(namelist[i]) - 14)
              {  j = SendDlgItemMessage(dialog->gethdlg(),100,CB_FINDSTRINGEXACT,-1,(LPARAM)namelist[i]);
                 strcpy(strstr(namelist[i]," - Display Off")," - Display On");
                 SendDlgItemMessage(dialog->gethdlg(),100,CB_DELETESTRING,j,0);
                 ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,namelist[i]);
                 SendDlgItemMessage(dialog->gethdlg(),100,CB_SELECTSTRING,-1,(LPARAM)namelist[i]);
              }
         }
       else if (id == 102)
         {  if (strstr(namelist[i]," - Display On") != 0 && strstr(namelist[i]," - Display On") - namelist[i] == (signed)strlen(namelist[i]) - 13)
              {  j = SendDlgItemMessage(dialog->gethdlg(),100,CB_FINDSTRINGEXACT,-1,(LPARAM)namelist[i]);
                 strcpy(strstr(namelist[i]," - Display On")," - Display Off");
                 SendDlgItemMessage(dialog->gethdlg(),100,CB_DELETESTRING,j,0);
                 ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,namelist[i]);
                 SendDlgItemMessage(dialog->gethdlg(),100,CB_SELECTSTRING,-1,(LPARAM)namelist[i]);
              }
         }
    }
  return 0;
}

void workplane_command1(int *cstate,HighLevelEvent *,void **)
{
 Workplane *w=0;
 //Dialog dialog("SelectWorkplane_dialog");
 Dialog dialog("SelectWorkplane_Dialog");
 int i,on,index,n,display,lastdisplay,zdisplay,lastzdisplay;
 RCCHAR **namelist,buffer[300];
 ResourceString rs7(NCWORK+7);
  index = 0;
  db.workplanes.names(&namelist,&n,&index);
  if (namelist != NULL)
    {  lastdisplay = display = v.getinteger("wp::display");
       lastzdisplay = zdisplay = v.getinteger("wp::zdisplay");
       for (i = 0 ; i < n ; i++)
         if ((w = db.workplanes.match(namelist[i])) != 0)
           strcat(namelist[i]+strlen(namelist[i]),w->getvisible() == 0 ? " - Display Off" : " - Display On");
       dialog.add(new ListDialogControl(100,n,namelist,&index));
       dialog.add(new WPButtonDialogControl(101,n,namelist));
       dialog.add(new WPButtonDialogControl(102,n,namelist));
       dialog.add(new CheckBoxDialogControl(103,&display));
       dialog.add(new CheckBoxDialogControl(104,&zdisplay));
       if (dialog.process() == TRUE)
         {
            v.setinteger("wp::display",display);
            v.setinteger("wp::zdisplay",zdisplay);
            for (i = 0 ; i < n ; i++)
            {
                strcpy(buffer,namelist[i]);
                 if (strstr(namelist[i]," - Display On") != 0 && strstr(namelist[i]," - Display On") - namelist[i] == (signed)strlen(namelist[i]) - 13)
                 {
                     on = 1;
                      *strstr(namelist[i]," - Display On") = 0;
                 }
                 else if (strstr(namelist[i]," - Display Off") != 0 && strstr(namelist[i]," - Display Off") - namelist[i] == (signed)strlen(namelist[i]) - 14)
                 {
                     on = 0;
                      *strstr(namelist[i]," - Display Off") = 0;
                 }
                 if ((w = db.workplanes.match(namelist[i])) != 0)
                   w->setvisible(on);
            }
            if ((w = db.workplanes.match(namelist[index])) != NULL)
              db.workplanes.setcurrent(w);
         }
       else
         {
           for (i = 0 ; i < n ; i++)
           {
                 strcpy(buffer,namelist[i]);
                 if (strstr(namelist[i]," - Display On") != 0 && strstr(namelist[i]," - Display On") - namelist[i] == (signed)strlen(namelist[i]) - 13)
                   *strstr(namelist[i]," - Display On") = 0;
                 else if (strstr(namelist[i]," - Display Off") != 0 && strstr(namelist[i]," - Display Off") - namelist[i] == (signed)strlen(namelist[i]) - 14)
                   *strstr(namelist[i]," - Display Off") = 0;
           }
            db.workplanes.setcurrent(db.workplanes.getcurrent());
         }
       delete namelist;

       if (lastdisplay != display || lastzdisplay != zdisplay)
         cadwindow->paintall();

    }
  *cstate = ExitState;
}

void workplane_command2(int *cstate,HighLevelEvent *,void **)
{Workplane *oldw,*neww;
 Dialog *dialog;
 int n,index;
 RCCHAR **namelist,string[256];
 ResourceString rs8(NCWORK+8),rs9(NCWORK+9),rs10(NCWORK+10),rs11(NCWORK+11);
 
  if ((dialog = new Dialog(_RCT("CreateWorkplane_Dialog"))) == NULL)
    state.fail(Memory,rs8.gets());
  else
    {  index = 0;
       db.workplanes.names(&namelist,&n,&index);
       if (namelist != NULL)
         {
            string[0] = 0;
            dialog->add(new StringDialogControl(100,string,256));
            dialog->add(new ListDialogControl(101,n,namelist,&index));
            if (dialog->process() == TRUE)
              {  if (db.workplanes.match(string) != NULL)
                   cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
                 else if ((oldw = db.workplanes.match(namelist[index])) != NULL)
                   {  if ((neww = new Workplane(string,oldw->getorigin(),oldw->getxaxis(),oldw->getyaxis(),0.0)) == NULL)
                        state.fail(Memory,rs11.gets());
                      else
                        {  db.workplanes.add(neww);
                           db.workplanes.setcurrent(neww);
                        }
                   }
              }
            delete namelist;
         }
        delete dialog;
    }
  *cstate = ExitState;
}

void RBWorkplane3(int dmode,void *data,Point *p,View3dSurface *)
{
    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
  Workplane *w = (Workplane *) data;
  w->change(*p,w->getxaxis(),w->getyaxis());
  cadwindow->drawcallback(DM_INVERT,w,drawworkplane);
}

void workplane_command3(int *cstate,HighLevelEvent *event,void **)
{
  static Workplane *w;
  if (db.workplanes.getcurrent() == NULL) return;
  switch (*cstate)
    {
        case InitialState :
         w = new Workplane(_RCT(""),db.workplanes.getcurrent()->getorigin(),db.workplanes.getcurrent()->getxaxis(),db.workplanes.getcurrent()->getyaxis(),0.0);
         *cstate = 1;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {
              Workplane *w;
              cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
              if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
              w = db.workplanes.getcurrent();
              db.saveundo(UD_CHANGEWORKPLANE,w);
              w->change(((Coordinate3dEvent *) event)->getp(),w->getxaxis(),w->getyaxis());
              // using opengl do draw so erase the rubberband
              //cadwindow->drawcallback(DM_NORMAL,w,drawworkplane);
              cadwindow->drawcallback(DM_ERASE,w,drawworkplane);
              if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
              state.setposition(state.getposition());
           }
         else if (event->isa(Abort) || event->isa(Exit)) 
         {
             if(w)
                cadwindow->drawcallback(DM_ERASE,w,drawworkplane);
             *cstate = ExitState;
         }
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBWorkplane3,w);
       cadwindow->prompt->normalprompt(NCWORK+12);
    }
  else if (*cstate == ExitState)
    {  delete w;
       w = 0;  

    }
}


struct RBWorkplane4_data
  {
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
#endif
    int state;
    Workplane w;
  };
   
void RBWorkplane4(int dmode,void *data,Point *p,View3dSurface *)
{
 RBWorkplane4_data *rbdata = (RBWorkplane4_data *) data;
 Workplane w(_RCT(""),rbdata->w.getorigin(),rbdata->w.getxaxis(),rbdata->w.getyaxis(),0.0);
 Point xaxis,yaxis,zaxis;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  if (rbdata->state == 1)
    w.change(*p,w.getxaxis(),w.getyaxis());
  else if (rbdata->state == 2)
    {  xaxis = *p - w.getorigin();  xaxis.normalize();
       zaxis = w.getxaxis().cross(w.getyaxis());
       yaxis = zaxis.cross(xaxis);
       if (yaxis.length() < db.getatoler())
         {  zaxis = w.getxaxis();
            yaxis = zaxis.cross(xaxis);
         }
       yaxis.normalize();
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         w.change(w.getorigin(),xaxis,yaxis);
    }
  else if (rbdata->state == 3)
    {  yaxis = *p - w.getorigin();  yaxis.normalize();
       zaxis = w.getxaxis().cross(yaxis);
       yaxis = zaxis.cross(w.getxaxis());  yaxis.normalize();
       if (yaxis.length() > db.getptoler())
         w.change(w.getorigin(),w.getxaxis(),yaxis);
    }

  //w.qgi = rbdata->qgi;

  cadwindow->drawcallback(DM_INVERT,&w,drawworkplane);

  //rbdata->qgi = w.qgi;
}

void workplane_command4(int *cstate,HighLevelEvent *event,void **data)
{
  RBWorkplane4_data *rbdata = (RBWorkplane4_data *) *data;
  if (db.workplanes.getcurrent() == NULL) return;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBWorkplane4_data; rbdata->qgi=0;  *data = rbdata;
         rbdata->w = *db.workplanes.getcurrent();
         *cstate = 1;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->w.change(((Coordinate3dEvent *) event)->getp(),rbdata->w.getxaxis(),rbdata->w.getyaxis());
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {Point xaxis,yaxis,zaxis;
              xaxis = ((Coordinate3dEvent *) event)->getp() - rbdata->w.getorigin();  xaxis.normalize();
              zaxis = rbdata->w.getxaxis().cross(rbdata->w.getyaxis());
              yaxis = zaxis.cross(xaxis);
              if (yaxis.length() < db.getatoler())
                {  zaxis = rbdata->w.getxaxis();
                   yaxis = zaxis.cross(xaxis);
                }
              yaxis.normalize();
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                rbdata->w.change(rbdata->w.getorigin(),xaxis,yaxis);
              *cstate = 3;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {Point xaxis,yaxis,zaxis;
              yaxis = ((Coordinate3dEvent *) event)->getp() - rbdata->w.getorigin();  yaxis.normalize();
              zaxis = rbdata->w.getxaxis().cross(yaxis);
              yaxis = zaxis.cross(rbdata->w.getxaxis());  yaxis.normalize();
              if (yaxis.length() > db.getptoler())
                {  cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
                   if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                     cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
                   db.saveundo(UD_CHANGEWORKPLANE,db.workplanes.getcurrent());
                   db.workplanes.getcurrent()->change(rbdata->w.getorigin(),rbdata->w.getxaxis(),yaxis);
                   // using opengl do draw so erase the rubberband
                   //cadwindow->drawcallback(DM_NORMAL,db.workplanes.getcurrent(),drawworkplane);
                   cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
                   //if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                   //  cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
                   state.setposition(state.getposition());
                }
              *cstate = ExitState;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate >= 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBWorkplane4,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCWORK+13);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCWORK+14);
  else if (*cstate == 3)
    cadwindow->prompt->normalprompt(NCWORK+15);
  else if (*cstate == ExitState)
    delete rbdata;
}

void workplane_command5(int *cstate,HighLevelEvent *,void **)
{Dialog *dialog=0;
 int index;
 ResourceString rs0(NCWORK+0),rs1(NCWORK+1),rs2(NCWORK+2),rs3(NCWORK+3);
 ResourceString rs4(NCWORK+4),rs5(NCWORK+5),rs6(NCWORK+6),rs16(NCWORK+16);
 RCCHAR *namelist[] = { rs0.gets(),rs1.gets(),rs2.gets(),rs3.gets(),rs4.gets(),rs5.gets(),rs6.gets()  };
 
  if ((dialog = new Dialog(_RCT("ResetWorkplane_Dialog"))) == NULL)
    state.fail(Memory,rs16.gets());
  else   
    {  index = 0;
       dialog->add(new ListDialogControl(100,7,namelist,&index));
       if (dialog->process() == TRUE)
         {
            cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
            if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
              cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
            db.saveundo(UD_CHANGEWORKPLANE,db.workplanes.getcurrent());
            db.workplanes.getcurrent()->standard(index);
            // using opengl do draw so erase the rubberband
            //cadwindow->drawcallback(DM_NORMAL,db.workplanes.getcurrent(),drawworkplane);
            cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
            if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
              cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
            state.setposition(state.getposition());
         }
    }
    if(dialog !=0)
        delete dialog;
  *cstate = ExitState;
}

void workplane_command6(int *cstate,HighLevelEvent *,void **)
{
 Workplane *w;
 Dialog *dialog=0;
 int n,index;
 RCCHAR **namelist,mess[256];
 ResourceString rs17(NCWORK+17),rs18(NCWORK+18),rs19(NCWORK+19);
 ResourceString rs20(NCWORK+20),rs21(NCWORK+21);


  if ((dialog = new Dialog(_RCT("DeleteWorkplane_Dialog"))) == NULL)
    state.fail(Memory,rs17.gets());
  else   
    {  index = 0;
       db.workplanes.names(&namelist,&n,&index);
       if (namelist != NULL)
         {  dialog->add(new ListDialogControl(100,n,namelist,&index));
            if (dialog->process() == TRUE)
              {  if ((w = db.workplanes.match(namelist[index])) != NULL)
                   {  if (w == db.workplanes.getcurrent())
                        cadwindow->MessageBox(rs18.gets(),rs19.gets(),MB_ICONINFORMATION);
                      else
                        {  sprintf(mess,rs20.gets(),namelist[index]);
                           if (cadwindow->MessageBox(mess,rs21.gets(),MB_YESNO) == IDYES)
                             {
                               db.workplanes.del(w);
                               delete w;
                             }
                        }
                   }
              }
            delete [] namelist;
         }
    }
    if(dialog != 0 )
        delete dialog;
  *cstate = ExitState;
}


void workplane_command7(int *cstate,HighLevelEvent *event,void **data)
{Entity *e;

  if (db.workplanes.getcurrent() == NULL) return;
  switch (*cstate)
    {  case InitialState :
         if (db.workplanes.getcurrent() != 0) 
           *cstate = 1;
         else
           *cstate = ExitState; 
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {Point origin,xaxis,yaxis,zaxis;
              state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              if (e->isa(line_entity))
                {  if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     origin = ((Line *)e)->getp1();
                   else
                     origin = ((Line *)e)->getp2();
                   zaxis = ((Line *)e)->direction(0.0);
                   if (fabs(zaxis.x) >= fabs(zaxis.y) && fabs(zaxis.x) >= fabs(zaxis.z))
                     {  if (zaxis.x < 0.0)
                          zaxis = -zaxis;
                        yaxis.setxyz(0.0,0.0,1.0);
                        xaxis = yaxis.cross(zaxis); 
                        yaxis = zaxis.cross(xaxis);
                        //xaxis.z < 0.0) 
                     }
                   else if (fabs(zaxis.y) > fabs(zaxis.x) && fabs(zaxis.y) > fabs(zaxis.z))
                     {  if (zaxis.y < 0.0)
                          zaxis = -zaxis;
                        yaxis.setxyz(0.0,0.0,1.0);
                        xaxis = yaxis.cross(zaxis); 
                        yaxis = zaxis.cross(xaxis);
                     }
                   else
                     {  if (zaxis.z < 0.0)
                          zaxis = -zaxis;
                        xaxis.setxyz(1.0,0.0,0.0);
                        yaxis = zaxis.cross(xaxis);
                        xaxis = yaxis.cross(zaxis);
                     }
                   xaxis.normalize(); 
                   yaxis.normalize(); 
                }
              else if (((EntitySelectedEvent *) event)->getentity()->isa(circle_entity))
                {  origin = ((Circle *)e)->getorigin();
                   xaxis = ((Circle *)e)->getxaxis();
                   yaxis = ((Circle *)e)->getyaxis();
                }
              else if (((EntitySelectedEvent *) event)->getentity()->isa(plane_entity))
                {  origin = ((Plane *)e)->getorigin();
                   xaxis = ((Plane *)e)->getxaxis();
                   yaxis = ((Plane *)e)->getyaxis();
                }

              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
                   if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                     cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
                   db.saveundo(UD_CHANGEWORKPLANE,db.workplanes.getcurrent());
                   db.workplanes.getcurrent()->change(origin,xaxis,yaxis);
                   // using opengl do draw so erase the rubberband
                   //cadwindow->drawcallback(DM_NORMAL,db.workplanes.getcurrent(),drawworkplane);
                   cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
                   if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                     cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
                }
                
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }

  //
  //  About to enter state actions
  //
  if (*cstate >= 1)
    {  cadwindow->prompt->normalprompt(NCWORK+22);
       state.selmask.entity.clearandset(line_entity,circle_entity,plane_entity,end_list);
    }
    
}

