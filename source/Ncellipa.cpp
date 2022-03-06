//
// Implementation of the 'EllipseE' class
//

#include "ncwin.h"

EllipseE::EllipseE(Point o,Point xa,Point ya,double maj,double min,double sta,double swa)
{
  usedByPlane=false;
  origin = o;
  xaxis = xa;
  yaxis = ya;
  major = maj;
  minor = min;
  starta = sta;
  sweepa = swa;
}

EllipseE::EllipseE(const EntityHeader &header,Point o,Point xa,Point ya,double maj,double min,double sta,double swa) : LinearEntity(header)
{
  usedByPlane=false;
  origin = o;
  xaxis = xa;
  yaxis = ya;
  major = maj;
  minor = min;
  starta = sta;
  sweepa = swa;
}

EllipseE::EllipseE(Point o,Point maj,Point c)
{
  usedByPlane=false;
  Point zaxis;
  double x,y,d;
  origin = o;
  starta = 0.0;
  sweepa = 2.0 * M_PI;
  xaxis = maj - o;   major = xaxis.length();
  yaxis = c - o;
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler())
    {  defined = 0;  return;
    }
  x = xaxis.dot(c - o);
  y = yaxis.dot(c - o);
  d = major * major - x * x;
  if (d > 0.0)
    minor = major * y / sqrt(d);
  else
    minor = 0.0;
  if (fabs(major) < db.getptoler() || fabs(minor) < db.getptoler())
    defined = 0;
}

double EllipseE::togeometric(double pangle)
{double gangle,denom,cosa,sina,sing;
  cosa = cos(pangle);  sina = sin(pangle);
  denom = sqrt(major * major * cosa * cosa + minor * minor * sina * sina);   
  if (fabs(denom) < 1.0E-10) return 0.0;
  sing = minor * sina / denom;
  if (sing < -1.0) sing = -1.0; else if (sing > 1.0) sing = 1.0;
  gangle = asin(sing);
  if (cos(gangle) * cos(pangle) < 0.0) gangle = M_PI - gangle;
  while (gangle < 0.0) gangle += M_PI * 2.0;
  return gangle;
}

double EllipseE::toparametric(double gangle)
{double pangle,denom,cosa,sina,sinp;
  cosa = cos(gangle);  sina = sin(gangle);
  denom = major * major * sina * sina + minor * minor * cosa * cosa;
  if (denom < 0.0) return 0.0;
  denom = sqrt(denom);  
  if (fabs(denom) < 1.0E-10) return 0.0;
  sinp = major * sina / denom;
  if (sinp < -1.0) sinp = -1.0; else if (sinp > 1.0) sinp = 1.0;
  pangle = asin(sinp);
  if (cos(gangle) * cos(pangle) < 0.0) pangle = M_PI - pangle;
  while (pangle < 0.0) pangle += M_PI * 2.0;
  return pangle;
}

double EllipseE::angle(Point p)
{double l,a,cosa,sina;
   cosa = p.dot(xaxis) - origin.dot(xaxis);
   sina = p.dot(yaxis) - origin.dot(yaxis);
   l = sqrt(cosa*cosa + sina*sina);
   if (l < db.getptoler())
     return 0.0;
   else
     {  cosa /= l;  if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
        a = acos(cosa);
        if (sina > 0.0) return a; else return M_PI * 2.0 - a;
     }
}

int EllipseE::nsegments(double error)
{int n;
 double cosa,r;
  r = major > minor ? major : minor;
  if (r < db.getptoler() || r < error * 2.0) return 6;
  cosa = (r - error) / r;
  if (cosa > 0.99999) cosa = 0.99999;
  n = int(sweepa / acos(cosa));
  if (n < 6) n = 6;
  return n;
}

void EllipseE::segment(int n,int include,Point *verts)
{int i,j;
 double x1,y1,x2,y2,sta,swa,da,sina,cosa;
  if (include & LE_REVERSE)
    {  sta = starta + sweepa;
       swa = -sweepa;
    }
  else
    {  sta = starta;
       swa = sweepa;
    }
  j = 0;
  x1 = cos(sta);
  y1 = sin(sta);
  if (include & LE_INCLUDESTART)
    verts[j++] = origin + xaxis * x1 * major + yaxis * y1 * minor;
  da = swa / n;
  sina = sin(da);  cosa = cos(da);
  if (! (include & LE_INCLUDEEND)) n--; 
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       verts[j++] = origin + xaxis * x2 * major + yaxis * y2 * minor; 
       x1 = x2;  y1 = y2;
    }
}

void EllipseE::addtodisplaylist(View3dSurface *vs,int child)
{
#ifdef USING_WIDGETS
#if 1 // new drawing
    int    n,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2,pt1,pt2;
  n = nsegments((vs->getumax() - vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,n * 34)) return;
    }
  x1 = cos(starta);
  y1 = sin(starta);
  pt1 = p1 = vs->modeltoview(origin + xaxis * x1 * major + yaxis * y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       pt2 = p2 = vs->modeltoview(origin + xaxis * x2 * major + yaxis * y2 * minor);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
       pt1 = p1 = p2;  x1 = x2;  y1 = y2;
    }
  if (! child && ! vs->displaylist.end()) return;
#else
 int    n,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2,pt1,pt2;


  QGraphicsItem *qline = 0;
  QList<QGraphicsItem *> glist;

  if(qgi)
      vs->getScene()->removeItem(qgi);
  delete qgi;
  qgi = 0;

  n = nsegments((vs->getumax() - vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (! child)
  {
      vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,n * 34)) return;
  }
  x1 = cos(starta);
  y1 = sin(starta);
  pt1 = p1 = vs->modeltoview(origin + xaxis * x1 * major + yaxis * y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       pt2 = p2 = vs->modeltoview(origin + xaxis * x2 * major + yaxis * y2 * minor);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
            // add the line to the scene as well
            QPointF qp1 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt1.x,pt1.y);
            QPointF qp2 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt2.x,pt2.y);
            if(vs->getScene())
                qline = vs->getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y());
            if(qline)
                glist.append(qline);
         }
       pt1 = p1 = p2;  x1 = x2;  y1 = y2;
    }

  if(glist.size() > 0)
      qgi = vs->getScene()->createItemGroup(glist);
  if(qgi)
  {
      if(getselected())
          qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
      else
          qgi->setZValue(zValue);
  }
  if (! child && ! vs->displaylist.end()) return;
#endif
#else
    int    n,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2,pt1,pt2;
  n = nsegments((vs->getumax() - vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,n * 34)) return;
    }
  x1 = cos(starta);
  y1 = sin(starta);
  pt1 = p1 = vs->modeltoview(origin + xaxis * x1 * major + yaxis * y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       pt2 = p2 = vs->modeltoview(origin + xaxis * x2 * major + yaxis * y2 * minor);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
       pt1 = p1 = p2;  x1 = x2;  y1 = y2;
    }
  if (! child && ! vs->displaylist.end()) return;
#endif
}

#ifdef USING_WIDGETS
void EllipseE::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
    int    n,inside,i;
    double x1,y1,z1,x2,y2,z2,da,sina,cosa;
    Point o,xa,ya,p1,p2,pt1,pt2;

    QGraphicsItem *qline = 0;
    //QList<QGraphicsItem *> glist;

    //qDebug() << "Enterting EllipseE::draw() : qgi : " << qgi;
    draw(DM_ERASE,t,vs);

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
    vs->setupGL(linestylescale,fabs(sweepa * (major + minor) / 2.0),drawmode,colour,style,weight,patternlinestyle);

    RCCOLORREF pcol;
    pcol = vs->getcolour();
    //GLfloat fr=GetRValue(pcol)/255.0;
    //GLfloat fg=GetGValue(pcol)/255.0;
    //GLfloat fb=GetBValue(pcol)/255.0;
    //glColor3f(fr,fg,fb);
    //glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    GLfloat lwidth = vs->curPen.widthF();
    //glLineWidth(lwidth<1.0?1.0:lwidth);
    glLineWidth(vs->curGLWeight);

#if 0
     if (t == NULL && ! DM_SELECT)
     {
          if (vs->displaylist.draw(this,drawmode))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
          if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
          if (vs->displaylist.draw(this,drawmode))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
     }
#endif

     o = origin;  xa = xaxis;  ya = yaxis;
     /*
     // kmj : changed for gl dragging in figures
     if (t == NULL)
     {
         o = origin;  xa = xaxis;  ya = yaxis;
     }
     else
     {
          o = t->transform(origin);
          xa = t->transform(origin+xaxis) - o;
          ya = t->transform(origin+yaxis) - o;
     }
     */
     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     /*
     if (vs->inside(vs->modeltoview( xa * major + ya * minor + o)) &&
         vs->inside(vs->modeltoview(-xa * major + ya * minor + o)) &&
         vs->inside(vs->modeltoview( xa * major - ya * minor + o)) &&
         vs->inside(vs->modeltoview(-xa * major - ya * minor + o)))
       inside = 1;
     else
       inside = 0;
     if (inside)
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = vs->modeltoscreen(o + xa * x1 * major + ya * y1 * minor);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          vs->moveto(pt1.x,pt1.y);
          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = vs->modeltoscreen(o + xa * x2 * major + ya * y2 * minor);

               qline = vs->lineto((QGraphicsItem*)0,pt2.x,pt2.y);
               if(qline)
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
                   //glist.append(qline);

               x1 = x2;  y1 = y2;
            }
       }
     else
       {
     */
          x1 = cos(starta);
          y1 = sin(starta);
          z1 = 0;
          //pt1 = p1 = vs->modeltoview(o + xa * x1 * major + ya * y1 * minor);
          //pt1 = p1 = vs->modeltransform.transform(o + xa * x1 * major + ya * y1 * minor);
          if (t == NULL)
          {
              pt1 = p1 = vs->modeltransform.transform(o + xa * x1 * major + ya * y1 * minor);
          }
          else
          {
              pt1 = p1 = t->transform(vs->modeltransform.transform(o + xa * x1 * major + ya * y1 * minor));
          }
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          for (i = 0 ; i < n ; i++)
          {
               x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               z2 = 0;
               //pt2 = p2 = vs->modeltoview(o + xa * x2 * major + ya * y2 * minor);
               //pt2 = p2 = vs->modeltransform.transform(o + xa * x2 * major + ya * y2 * minor);
               if (t == NULL)
               {
                   pt2 = p2 = vs->modeltransform.transform(o + xa * x2 * major + ya * y2 * minor);
               }
               else
               {
                   pt2 = p2 = t->transform(vs->modeltransform.transform(o + xa * x2 * major + ya * y2 * minor));
               }
               //if (vs->clip3d(&pt1,&pt2))
               //{
                    //pt1 = vs->viewtoscreen(pt1);
                    //pt2 = vs->viewtoscreen(pt2);
                    //vs->line(pt1.x,pt1.y,pt2.x,pt2.y);
               if (patternlinestyle != 0)
               {
                   // drawing patternstyle with GL in screen space
                   Point spt1 = vs->modeltoscreen(pt1);
                   Point spt2 = vs->modeltoscreen(pt2);
                   vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);

                   // this one draws patternstyle line with GL in model space
                   //vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);

               }
               /*
               else if(vs->getlinestyle())
               {
                   Point spt1 = vs->modeltoscreen(pt1);
                   Point spt2 = vs->modeltoscreen(pt2);
                   vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);
               }*/
               else
                    vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
               //}
               pt1 = p1 = p2;  x1 = x2;  y1 = y2; z1 = z2;
          }

          if(patternlinestyle != 0)
              patternlinestyle->endGL(vs);
     //}
}
#endif

void EllipseE::draw(int drawmode,Transform *t,View3dSurface *vs)
{
#ifdef USING_WIDGETS
    int    n,inside,i;
    double x1,y1,x2,y2,da,sina,cosa;
    Point o,xa,ya,p1,p2,pt1,pt2;

    QGraphicsItem *qline = 0;
    //QList<QGraphicsItem *> glist;

    //qDebug() << "Enterting EllipseE::draw() : qgi : " << qgi;

    if(drawmode != DM_PRINT)
    {
        if(qgi && qgi->scene() != vs->getScene())
            qgi->scene()->removeItem(qgi);
        else if(qgi && vs->getScene())
            vs->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
    }

    if( drawmode == DM_ERASE )
        return;

#if 0
  qgi = new QGraphicsItemGroup();
#else
  qgi = new QGraphicsPathItem();
  QPainterPath pPath;
#endif

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
     vs->setup(linestylescale,0.0,drawmode,colour,style,weight,patternlinestyle);

#if 0
     if (t == NULL && ! DM_SELECT)
     {
          if (vs->displaylist.draw(this,drawmode))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
          if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
          if (vs->displaylist.draw(this,drawmode))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
     }
#endif

     if (t == NULL)
       {  o = origin;  xa = xaxis;  ya = yaxis;
       }
     else
       {  o = t->transform(origin);
          xa = t->transform(origin+xaxis) - o;
          ya = t->transform(origin+yaxis) - o;
       }
     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     if (vs->inside(vs->modeltoview( xa * major + ya * minor + o)) &&
         vs->inside(vs->modeltoview(-xa * major + ya * minor + o)) &&
         vs->inside(vs->modeltoview( xa * major - ya * minor + o)) &&
         vs->inside(vs->modeltoview(-xa * major - ya * minor + o)))
       inside = 1;
     else
       inside = 0;
     if (inside && patternlinestyle == 0)
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = vs->modeltoscreen(o + xa * x1 * major + ya * y1 * minor);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);

#if 0
       vs->moveto(pt1.x,pt1.y);
#else
       vs->moveto(&pPath,pt1.x,pt1.y);
#endif

          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = vs->modeltoscreen(o + xa * x2 * major + ya * y2 * minor);

#if 0
               qline = vs->lineto((QGraphicsItem*)0,pt2.x,pt2.y);
               if(qline)
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
                   //glist.append(qline);
#else
               vs->lineto(&pPath,pt2.x,pt2.y);
#endif

               x1 = x2;  y1 = y2;
            }
       }
     else
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = p1 = vs->modeltoview(o + xa * x1 * major + ya * y1 * minor);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = p2 = vs->modeltoview(o + xa * x2 * major + ya * y2 * minor);
               if (vs->clip3d(&pt1,&pt2))
                 {  pt1 = vs->viewtoscreen(pt1);
                    pt2 = vs->viewtoscreen(pt2);
#if 0
                    qline = vs->line((QGraphicsItem*)0,pt1.x,pt1.y,pt2.x,pt2.y);
                    if(qline)
                        ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
                        //glist.append(qline);
#else
                   vs->line(&pPath,pt1.x,pt1.y,pt2.x,pt2.y);
#endif
                 }
               pt1 = p1 = p2;  x1 = x2;  y1 = y2;
            }
     }

     if (patternlinestyle != 0)
     {
#if 0
         qline = patternlinestyle->end((QGraphicsItem*)0);
         if(qline)
             ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
#else
         patternlinestyle->end(&pPath);
#endif
     }

#if 1
  ((QGraphicsPathItem*)qgi)->setPath(pPath);
  ((QGraphicsPathItem*)qgi)->setPen(vs->curPen);
#endif

     if(qgi)
     {
         if(getselected())
             qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
         else
             qgi->setZValue(zValue);

         if(qgi && dynamic_cast<View3dSubSurface*>(vs) != 0  && !usedByPlane)
         {
             if(((View3dSubSurface*)vs)->qgicp != 0 &&
                ((View3dSubSurface*)vs)->qgicp != qgi->parentItem())
             {
                 qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);
             }
             else
                 vs->getScene()->addItem(qgi);
         }
         else
             vs->getScene()->addItem(qgi);

         if(drawmode == DM_INVERT)
         {
             if(vs->qgirb == 0)
             {
                 vs->qgirb = new QGraphicsItemGroup();
                 vs->getScene()->addItem(vs->qgirb);
                 ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
             }
             else
                 ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);

             qgi=0;
         }
         //else
         //    vs->getScene()->addItem(qgi);
     }
     //qDebug() << "Leaving EllipsE::draw() : qgi : " << qgi;
#else
 int    n,inside,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point o,xa,ya,p1,p2,pt1,pt2;

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  if (t == NULL)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  if (t == NULL)
    {  o = origin;  xa = xaxis;  ya = yaxis;
    }
  else
    {  o = t->transform(origin);
       xa = t->transform(origin+xaxis) - o;
       ya = t->transform(origin+yaxis) - o;
    }
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (vs->inside(vs->modeltoview( xa * major + ya * minor + o)) &&
      vs->inside(vs->modeltoview(-xa * major + ya * minor + o)) &&
      vs->inside(vs->modeltoview( xa * major - ya * minor + o)) &&
      vs->inside(vs->modeltoview(-xa * major - ya * minor + o)))
    inside = 1;
  else
    inside = 0;
  if (inside)
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = vs->modeltoscreen(o + xa * x1 * major + ya * y1 * minor);
       da = sweepa / n;
       sina = sin(da);  cosa = cos(da);
       vs->moveto(pt1.x,pt1.y);
       for (i = 0 ; i < n ; i++)
         {  x2 = x1 * cosa - y1 * sina;
            y2 = x1 * sina + y1 * cosa;
            pt2 = vs->modeltoscreen(o + xa * x2 * major + ya * y2 * minor);
            vs->lineto(pt2.x,pt2.y);
            x1 = x2;  y1 = y2;
         }
    }
  else
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = p1 = vs->modeltoview(o + xa * x1 * major + ya * y1 * minor);
       da = sweepa / n;
       sina = sin(da);  cosa = cos(da);
       for (i = 0 ; i < n ; i++)
         {  x2 = x1 * cosa - y1 * sina;
            y2 = x1 * sina + y1 * cosa;
            pt2 = p2 = vs->modeltoview(o + xa * x2 * major + ya * y2 * minor);
            if (vs->clip3d(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 vs->line(pt1.x,pt1.y,pt2.x,pt2.y);
              }
            pt1 = p1 = p2;  x1 = x2;  y1 = y2;
         }
    }
#endif
}

#ifdef USING_WIDGETS
int EllipseE::hideGL(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * x1 * major + yaxis * y1 * minor;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 * major + yaxis * y2 * minor;
       image->hideGL(this,p1,p2);
       p1 = p2;  x1 = x2;  y1 = y2;
    }

  return 1;
}

int EllipseE::hide(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;

 if(qgi && qgi->scene() != image->getsurface()->getScene())
     qgi->scene()->removeItem(qgi);
 else if(qgi && image->getsurface()->getScene())
     image->getsurface()->getScene()->removeItem(qgi);
 delete qgi;
 qgi=0;

 qgi = new QGraphicsItemGroup();

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * x1 * major + yaxis * y1 * minor;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 * major + yaxis * y2 * minor;
       image->hide(this,p1,p2);
       p1 = p2;  x1 = x2;  y1 = y2;
    }

  if(qgi)
  {
      if(image->getsurface()->getScene())
          image->getsurface()->getScene()->addItem(qgi);
  }

  return 1;
}
#else
int EllipseE::hide(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * x1 * major + yaxis * y1 * minor;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 * major + yaxis * y2 * minor; 
       image->hide(this,p1,p2);
       p1 = p2;  x1 = x2;  y1 = y2;
    }
  return 1;
}
#endif

double EllipseE::pick(View3dSurface *vs,double x,double y,Point *pos)
{int i,n;
 double d,x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (pos == NULL && vs->displaylist.defined(this))
    return vs->displaylist.pick(this,x,y);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * (x1 * major) + yaxis * (y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * (x2 * major) + yaxis * (y2 * minor); 
       if ((d = vs->pick(p1,p2,x,y,pos)) >= 0.0) return d;
       x1 = x2;  y1 = y2;  p1 = p2;
    }
  return -1.0;
}

int EllipseE::pickendpoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{Point p1,p2;
 double d1,d2;
 int end;
  if (! distance && sweepa > 1.99999 * M_PI) return 0;
  p1 = origin + xaxis * (major * cos(starta)) + yaxis * (minor * sin(starta));
  p2 = origin + xaxis * (major * cos(starta+sweepa)) + yaxis * (minor * sin(starta+sweepa));
  end = 0;
  if ((d1 = vs->pick(p1,distance,x,y)) >= 0.0)
    {  end = 1;  *pos = p1;
    }
  if ((d2 = vs->pick(p2,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = p2;
    }
  return end;
}

void EllipseE::moveendpoint(int drawentity,int end,Point p)
{double a,a1,a2;

  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  a = angle(p);
  a = toparametric(a);
  a1 = starta;
  a2 = starta + sweepa;
  if (end == 1)
    a1 = a;
  else
    a2 = a;
  while (a2 > a1) a2 -= 2.0 * M_PI;
  while (a2 < a1) a2 += 2.0 * M_PI;
  starta = a1;
  sweepa = a2 - a1;
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

double EllipseE::gett(Point p1)
{double a1;
  a1 = angle(p1);
  while (a1 > starta) a1 -= 2.0 * M_PI;
  while (a1 < starta) a1 += 2.0 * M_PI;
  if (a1 > starta + sweepa && fabs(a1 - 2.0 * M_PI - starta) < fabs(a1 - starta - sweepa))
    a1 -= 2.0 * M_PI; 
  if (sweepa == 0.0)
    return 0.0;
  else
    return (a1 - starta) / sweepa;
}


void EllipseE::breakanddelete(int,Point p1,Point p2)
{double a,a1,a2;
 Point p;
 EllipseE *ellipse;
 Transform identity;

  if (GetLockedLayerTable()->test(layer)) return;  

  a1 = angle(p1);
  while (a1 > starta) a1 -= 2.0 * M_PI;
  while (a1 < starta) a1 += 2.0 * M_PI;
  if (a1 > starta + sweepa && fabs(a1 - 2.0 * M_PI - starta) < fabs(a1 - starta - sweepa))
    a1 -= 2.0 * M_PI; 
  a2 = angle(p2);
  while (a2 > starta) a2 -= 2.0 * M_PI;
  while (a2 < starta) a2 += 2.0 * M_PI;
  if (a2 > starta + sweepa && fabs(a2 - 2.0 * M_PI - starta) < fabs(a2 - starta - sweepa))
    a2 -= 2.0 * M_PI; 
  if (a1 > a2)
    {  a = a1;  a1 = a2;  a2 = a;
       p = p1;  p1 = p2;  p2 = p;
    }
  if (a2 <= starta || a1 >= starta + sweepa) return;
  if (a1 <= starta && a2 >= starta + sweepa) return;
  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,this);
  if (fabs(sweepa - 2.0 * M_PI) < db.getatoler())
    {  if (fabs(a2 - a1) < M_PI)
         {  moveendpoint(1,2,p1);
            moveendpoint(1,1,p2);
         }
       else
         {  moveendpoint(1,1,p1);
            moveendpoint(1,2,p2);
         }
       db.saveundo(UD_ENDBLOCK,NULL);
       return;
    }
  if (a1 <= starta)
    moveendpoint(1,1,p2);
  else if (a2 >= starta + sweepa)
    moveendpoint(1,2,p1);
  else
    {  identity.identity();
       copye = NULL;
       ellipse = (EllipseE *) clone(identity);
       moveendpoint(1,2,p1);
       ellipse->moveendpoint(1,1,p2);
       db.geometry.add(ellipse);
    }
  draw(DM_NORMAL);
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int EllipseE::trace(View3dSurface *surface,Point end,Point uv1,Point uv2,double side,Point *newend,Point *uv3,Point *uv4,double *angle)
{double cosa,l1,l2;
 Point uv5,uv6,duv1,duv2;
  if ((major + minor) * sweepa / 2.0 < db.getptoler())
    return 0;
  if ((position(0.0) - end).length() < db.getptoler())
    {  *newend = position(1.0);
       *uv3 = surface->modeltouv(position(0.99));
       *uv4 = surface->modeltouv(position(1.0));
       uv5 = surface->modeltouv(position(0.0));
       uv6 = surface->modeltouv(position(0.01));
    }
  else if ((position(1.0) - end).length() < db.getptoler())
    {  *newend = position(0.0);
       *uv3 = surface->modeltouv(position(0.01));
       *uv4 = surface->modeltouv(position(0.0));
       uv5 = surface->modeltouv(position(1.0));
       uv6 = surface->modeltouv(position(0.99));
    }
  else
    return 0;
  duv1 = uv1 - uv2;  duv1.z = 0.0;
  duv2 = uv6 - uv5;  duv2.z = 0.0;
  l1 = duv1.length();
  l2 = duv2.length();
  if (l1 > db.getptoler() && l2 > db.getptoler())
    {  cosa = duv1.dot(duv2) / l1 / l2;
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       *angle = acos(cosa);
       if (side * duv1.cross(duv2).z > 0.0)
         *angle = 2.0 * M_PI - *angle;
    }
  else
    *angle = 0.0;
  return 1;
}

double EllipseE::pickhor(View3dSurface *vs,int x,int y,Point *start,Point *end,Point *uvp1,Point *uvp2,double *side)
{int i,n;
 double d,dmin,s,x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2,p3,p4;
  if (start == 0 && vs->displaylist.defined(this))
    return vs->displaylist.pickhor(this,x,y);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * (x1 * major) + yaxis * (y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);  dmin = -1.0;
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * (x2 * major) + yaxis * (y2 * minor);
       if ((d = vs->pickhor(p1,p2,x,y,&p3,&s)) >= 0.0 && (dmin == -1.0 || d < dmin))
         {  dmin = d;
            p4 = p3;
            *side = s;
         }
       x1 = x2;  y1 = y2;  p1 = p2;
    }
  if (dmin >= 0.0 && start != 0)
    {  if ((position(0.0) - p4).length() < (position(1.0) - p4).length())
         {  *start = position(1.0);
            *end = position(0.0);
            *uvp1 = vs->modeltouv(position(0.01));
            *uvp2 = vs->modeltouv(position(0.0));
            *side = -*side;
         }
       else
         {  *start = position(0.0);
            *end = position(1.0);
            *uvp1 = vs->modeltouv(position(0.99));
            *uvp2 = vs->modeltouv(position(1.0));
         }
    }
  return dmin;
}

LinearEntity *EllipseE::divide(Point p, Point *dp)
{double a,a2;
 EllipseE *ellipse;

  if (GetLockedLayerTable()->test(layer)) return 0;  

  p = nearp(p);
  a = angle(p);
  a = toparametric(a);
  if (a < starta) a += 2.0 * M_PI;
  if (starta < a && a < starta + sweepa)
    {  PointE point(p);
       //point.draw(DM_NORMAL);
       point.draw(DM_MARKER);
       a2 = starta + sweepa;
       db.saveundo(UD_MOVED,this);
       draw(DM_ERASE);
       cadwindow->invalidatedisplaylist(this);
       sweepa = a - starta;
       draw(DM_NORMAL);
       ellipse = new EllipseE(*this,origin,xaxis,yaxis,major,minor,a,a2-a);
       db.geometry.add(ellipse);
       if (getposition() != 0 && db.MovedCallback != 0)
         db.MovedCallback(this);
    }
  return ellipse;
}

void EllipseE::divide(int n, PointList *dplist)
{int i;
 double a,da;

 if (GetLockedLayerTable()->test(layer)) return;  

  if (n > 0 && sweepa > db.getatoler())
    {  a = starta;  da = sweepa / (double) n;
       for (i = 1 ; i <= n ; i++)
         {Point p;
            if (i == 1)
              {  db.saveundo(UD_MOVED,this);
                 draw(DM_ERASE);
                 cadwindow->invalidatedisplaylist(this);
                 sweepa = da;
                 draw(DM_NORMAL);
                 if (getposition() != 0 && db.MovedCallback != 0)
                   db.MovedCallback(this);
              }
            else
              {  EllipseE *ellipse = new EllipseE(*this,origin,xaxis,yaxis,major,minor,a,da);
                 db.geometry.add(ellipse);
              }
            a += da;
            if (i < n)
              {  PointE point(origin + xaxis * (cos(a) * major) + yaxis * (sin(a) * minor));
                 //point.draw(DM_NORMAL);
                 point.draw(DM_MARKER);
              }
         }
    }
}

int EllipseE::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{int    n1,i;
 double dx1,dy1,dx2,dy2,da,sina,cosa;
 Point p1,p2;
  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
 if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
  n1 = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  dx1 = cos(starta);
  dy1 = sin(starta);
  p1 = origin + xaxis * (dx1 * major) + yaxis * (dy1 * minor);
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n1 ; i++)
    {  if (! vs->pick(p1,inside,x1,y1,x2,y2)) return 0;
       dx2 = dx1 * cosa - dy1 * sina;
       dy2 = dx1 * sina + dy1 * cosa;
       p1 = origin + xaxis * (dx2 * major) + yaxis * (dy2 * minor); 
       dx1 = dx2;  dy1 = dy2;
    }
  if (! vs->pick(p1,inside,x1,y1,x2,y2)) return 0;
  return 1;
}

int EllipseE::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{int i,n,picked;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;
  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
 n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * (x1 * major) + yaxis * (y1 * minor);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  picked = inside != 2;
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * (x2 * major) + yaxis * (y2 * minor); 
       if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
         return picked;
       x1 = x2;  y1 = y2;  p1 = p2;
    }
  return picked;
}

int EllipseE::type(void) const
{  return ellipse_entity;
}

int EllipseE::isa(int type) const
{  return ellipse_entity == type;
}

double EllipseE::length(void) const
{  return fabs(sweepa * (major + minor) / 2.0);
}

Point EllipseE::start(void)
{ return origin + xaxis * (major * cos(starta)) + yaxis * (minor * sin(starta));
}

Point EllipseE::end(void)
{ return origin + xaxis * (major * cos(starta+sweepa)) + yaxis * (minor * sin(starta+sweepa));
}

int EllipseE::npoints(void)
{ return 3;
}

Point EllipseE::point(int i)
{ if (i == 0)
    return origin + xaxis * (major * cos(starta)) + yaxis * (minor * sin(starta));
  else if (i == 1)
    return origin + xaxis * (major * cos(starta+sweepa)) + yaxis * (minor * sin(starta+sweepa));
  else
    return origin;
}

Point EllipseE::position(double t) const
{double a;
  a = starta + sweepa * t;
  return origin + xaxis * (major * cos(a)) + yaxis * (minor * sin(a));
}

Point EllipseE::positionbylength(double l,double *t) const
{const int nsegs = 200;
 double cl,sl;
 Point p1,p2;
 int i;

  cl = 0.0;
  p1 = position(0.0);

  for (i = 1,*t = 1.0 / nsegs ; i <= nsegs ; i++,(*t) += 1.0 / nsegs)
    {  p2 = position(*t);
       sl = (p2 - p1).length(); 
       if (cl + sl > l)
         {  *t = *t - 1.0 / nsegs + (1.0 / nsegs) * (l - cl) / sl;
            return position(*t);
         }
       cl += sl;
       p1 = p2;
    }

  *t = 1.0;    
  return position(*t);

}

Point EllipseE::direction(double t) const
{double a;
  a = starta + sweepa * t;
  return (xaxis * major * sin(-a) + yaxis * minor * cos(a)).normalize();
}

Point EllipseE::nearp(Point p)
{int nsegs,i,j;
 double t,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8,dmin;
 Point p1,p2,pmin,pos,dir;

  nsegs = 60;
  dmin = -1.0;
  t = 0.0;  dt = 1.0 / double(nsegs);
  p1 = position(0.0);
  p2 = p1 - p;  d1 = p2.dot(p2);

  dmin = d1;  pmin = p1;  
  p1 = position(dt);
  p2 = p1 - p;  d2 = p2.dot(p2);
  if (d2 < dmin)
    {  dmin = d2;  pmin = p1;
    }
  for (i = 0,t = 2.0 * dt ; i < nsegs - 1 ; i++,t += dt)
    {  p1 = position(t);
       p2 = p1 - p;  d3 = p2.dot(p2);
       if (d3 < dmin)
         {  dmin = d3;  pmin = p1;
         }
       if (d1 >= d2 && d2 <= d3)
         {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
            d4 = d1;  d5 = d2;  d6 = d3;
            for (j = 0 ; j < 50 ; j++)
              {  p1 = position(t1 + (t2 - t1) / 2.0);
                 p2 = p1 - p;  d7 = p2.dot(p2);
                 if (d4 >= d7 && d7 <= d5)
                   {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                      d6 = d5;  d5 = d7;
                   }
                 else
                   {  p1 = position(t3 - (t3 - t2) / 2.0);
                      p2 = p1 - p;  d8 = p2.dot(p2);
                      if (d5 >= d8 && d8 <= d6)
                        {  t1 = t2;  t2 = t3 - (t3 - t2) / 2.0;
                           d4 = d5;  d5 = d8;
                        }
                      else
                        {  t1 = t1 + (t2 - t1) / 2.0;  t3 = t3 - (t3 - t2) / 2.0;
                           d4 = d7;  d6 = d8;
                        }
                   }
              }
            p1 = position((t1+t3) / 2.0);
            p2 = p1 - p;
            d7 = p2.dot(p2);
            if (dmin < 0.0 || d7 < dmin)
              {  dmin = d7;  pmin = p1;
              }
         }
       d1 = d2;  d2 = d3;
    }
  return pmin;
}

int EllipseE::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
{ 
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  return 1;
}

int EllipseE::move(Database *db,Transform &t,int erase)
{Point zero(0.0,0.0,0.0);
 Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ?  clone(t) != 0 : db->geometry.add(clone(t));
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 1;
  origin = t.transform(origin);
  xaxis = t.transform(xaxis*major) - t.transform(zero);
  yaxis = t.transform(yaxis*minor) - t.transform(zero);
  major = xaxis.length();
  minor = yaxis.length();
  xaxis.normalize();  yaxis.normalize();
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

void EllipseE::transform(Transform &t,int drawentity)
{ db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  origin = t.transform(origin);
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}


Entity *EllipseE::clone(Transform &t)
{Point o,xa,ya,zero(0.0,0.0,0.0);
 double maj,min;
  if (copye != NULL) return copye;
  o = t.transform(origin);
  xa = t.transform(xaxis*major) - t.transform(zero);
  ya = t.transform(yaxis*minor) - t.transform(zero);
  maj = xa.length();
  min = ya.length();
  xa.normalize();  ya.normalize();
  copye = new EllipseE(*this,o,xa,ya,maj,min,starta,sweepa);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int EllipseE::project(Database *db,Transform &t,int surface)
{Point p;
 BitMask options(32);
 int xmesh,ymesh;
  if ((status & 2) == 2) return 1;
  p = origin + xaxis * (major * cos(starta)) + yaxis * (minor * sin(starta));
  if (! db->geometry.add(new Line(p,t.transform(p)))) return 0;
  p = origin + xaxis * (major * cos(starta+sweepa)) + yaxis * (minor * sin(starta+sweepa));
  if (! db->geometry.add(new Line(p,t.transform(p)))) return 0;
  if (copye == NULL)
    {  if (! db->geometry.add(clone(t))) return 0;
    }
  if (surface)
    {  RuledSurface::getdefaults(&xmesh,&ymesh,&options);
       RuledSurface *surface = new RuledSurface((LinearEntity *)this,(LinearEntity *)copye,xmesh,ymesh,options);
       if (surface != NULL && surface->getdefined())
         {  db->geometry.add(surface);
            status |= 2;
         }
       else
         delete surface;
    }
  return 1;
}

int EllipseE::beginoffset(Point p,Point *zaxis,double *d,LinearEntity *enext,int *reverse,Point *startp)
{double d1,d2,d3,d4;
 Point p3,za;
  p3 = nearp(p);
  if (fabs(*d) < db.getptoler()) 
    {  InfinitePlane ip(*zaxis,p3);
       *d = (p3 - ip.nearp(p)).length();
    }
  za = xaxis.cross(yaxis);
  if (zaxis->dot((za.cross(p3 - origin)).cross(p3 - p)) > 0.0) *zaxis = -*zaxis;
  if (enext == NULL)
    *reverse = 0;
  else
    {  d1 = (position(0.0) - enext->start()).length();
       d2 = (position(0.0) - enext->end()).length();
       if (d2 < d1) d1 = d2;
       d3 = (position(1.0) - enext->start()).length();
       d4 = (position(1.0) - enext->end()).length();
       if (d4 < d3) d3 = d4;
       if (fabs(d3) < db.getptoler() && fabs(d1) < db.getptoler() || d3 < d1)
         *reverse = 0;
       else
         {  *zaxis = -*zaxis;
            *reverse = 1;
         }
    }
  return offsetpoint(*zaxis,*d,*reverse,1,startp);
}

int EllipseE::offset(Point zaxis,double d,int,LinearEntity *enext,int *reverse,Point *startp,EntityList *olist)
{Point *fitpoint;
 int i,nseg;
 double t,dt;
 BitMask options(32);
 
  nseg = int(sweepa * 20);
  if (nseg < 10) nseg = 10;
  fitpoint = new Point[nseg+1];
  dt = 1.0 / nseg;
  for (i = 0,t = 0.0 ; i <= nseg ; i++,t += dt)
     if (*reverse)
       fitpoint[i] = position(t) + direction(t).cross(zaxis) * d;
     else
       fitpoint[i] = position(t) - direction(t).cross(zaxis) * d;

  olist->add(new Curve(nseg/4,3,nseg+1,fitpoint,options));

  if (enext != 0 && (enext->position(0.0) - position(1.0-*reverse)).length() > (enext->position(1.0) - position(1.0-*reverse)).length())
    *reverse = 1;
  else
    *reverse = 0;
  if (enext != 0)
    {  if ((enext->position(0.0) - position(1.0-*reverse)).length() > (enext->position(1.0) - position(1.0-*reverse)).length())
         *reverse = 1;
       else
         *reverse = 0;
       enext->offsetpoint(zaxis,d,*reverse,1,startp);
    }
  return 1;
}

int EllipseE::offsetpoint(Point zaxis,double d,int reverse,int end,Point *p)
{Point yaxis;
  if (reverse)
    yaxis = zaxis.cross(-direction(2.0 - end)).normalize();
  else
    yaxis = zaxis.cross(direction(end - 1.0)).normalize();
  if (yaxis.length() < db.getptoler()) return 0;
  yaxis *= d;
  *p = position(2.0 - end) + yaxis;
  return 1;
}

int EllipseE::save(CadFile *outfile)
{short type;
 Byte terminators = 0;
  type = ellipse_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&major)) return 0;
  if (! outfile->write(&minor)) return 0;
  if (! outfile->write(&starta)) return 0;
  if (! outfile->write(&sweepa)) return 0;
  if (! outfile->write(&terminators)) return 0;
  return 1;
}

Entity *EllipseE::load(int,CadFile *infile)
{EllipseE *ellipse;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 double major,minor,sweepa,starta;
 Byte terminators;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&major)) return 0;
  if (! infile->read(&minor)) return 0;
  if (! infile->read(&starta)) return 0;
  if (! infile->read(&sweepa)) return 0;
  if (! infile->read(&terminators)) return 0;
  ellipse = new EllipseE(*header,origin,xaxis,yaxis,major,minor,starta,sweepa);
  ellipse->atlist = header->getatlist();
  delete header;
  return ellipse;
}

int EllipseE::save(UndoFile *outfile)
{short type;
  type = ellipse_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  return 1;
}

Entity *EllipseE::load(UndoFile *infile)
{EllipseE *ellipse;
 Point p;
 char bm[sizeof(BitMask)];
  if ((ellipse = new EllipseE) != NULL)
    {AttributeList al;
       memcpy(bm,&ellipse->visible,sizeof(bm));
       if (! infile->blockread(ellipse,sizeof(*ellipse))) return 0;
       memcpy(&ellipse->visible,bm,sizeof(bm));
       if (! infile->read(&ellipse->visible)) return 0;
       if (! al.load(infile)) return 0;   ellipse->atlist = al;
    }
  return ellipse;
}

int EllipseE::saveattributes(CadFile *)
{ return 1;
}

int EllipseE::loadattributes(CadFile *)
{ return 1;
}

int EllipseE::savedxf(int blocksection,DXFOut *dxffile)
{Point zaxis,xa,ya,wx(1.0,0.0,0.0),wy(0.0,1.0,0.0),wz(0.0,0.0,1.0),ax,ay;
 double a,a1,a2,sta,swa,ratio,maj;
    if (! blocksection)
    {  

        if (minor < major)
        {   xa = xaxis;
            ya = yaxis;
            sta = starta;
            swa = sweepa;
            ratio = minor / major;
            maj = major;
        }
        else
        {   xa = yaxis;
            ya = -xaxis;
            sta = starta - acos(-1.0) / 2.0;
            swa = sweepa;
            ratio = major / minor;
            maj = minor;
        }
        
       zaxis = xa.cross(ya).normalize();
       if (fabs(zaxis.x) < 1.0/64.0 && fabs(zaxis.y) < 1.0/64.0)
         ax = wy.cross(zaxis).normalize();
       else
         ax = wz.cross(zaxis).normalize();
       ay = zaxis.cross(ax).normalize();
       a = angle(ax + origin);
       if (! dxffile->writestring(0, (char*)"ELLIPSE")) return 0;
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbEllipse")) return 0;
       if (! dxffile->writereal(10,origin.x * dxffile->getscale())) return 0;
       if (! dxffile->writereal(20,origin.y * dxffile->getscale())) return 0;
       if (! dxffile->writereal(30,origin.z * dxffile->getscale())) return 0;
       if (! dxffile->writereal(11,xa.x * maj * dxffile->getscale())) return 0;
       if (! dxffile->writereal(21,xa.y * maj * dxffile->getscale())) return 0;
       if (! dxffile->writereal(31,xa.z * maj * dxffile->getscale())) return 0;
       if (! dxffile->writereal(40,ratio)) return 0;
       a1 = sta;
       while (a1 < 0.0) a1 += M_PI * 2.0;
       while (a1 > M_PI * 2.0) a1 -= M_PI * 2.0;
       a2 = (sta + swa);
       while (a2 < 0.0) a2 += M_PI * 2.0;
       while (a2 > M_PI * 2.0) a2 -= M_PI * 2.0;
       if (fabs(a1-a2) < 0.00001) 
         {  a1 = 0.0;
            a2 = M_PI * 2.0;
         }
       if (! dxffile->writereal(41,a1)) return 0;
       if (! dxffile->writereal(42,a2)) return 0;
       if (! dxffile->writereal(210,zaxis.x)) return 0;
       if (! dxffile->writereal(220,zaxis.y)) return 0;
       if (! dxffile->writereal(230,zaxis.z)) return 0;
    }
  return 1;
}

int EllipseE::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}


int EllipseE::savegeometry(CadFile *outfile)
{ if (! outfile->write(&major)) return 0;
  if (! outfile->write(&minor)) return 0;
  if (! outfile->write(&starta)) return 0;
  if (! outfile->write(&sweepa)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  return 1;
}

int EllipseE::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&major)) return 0;
  if (! infile->read(&minor)) return 0;
  if (! infile->read(&starta)) return 0;
  if (! infile->read(&sweepa)) return 0;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  return 1;
}

class EllipseVerifyButtonControl : public ButtonDialogControl
  {public:
     EllipseVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int EllipseVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,halocolour,layer,style,iweight;
    double weight;

    state = 1001;
    if (id == 1000)
    { // colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&colour);
        general_property_command(&state,0,(void **)&colour);
        ((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
    }
    else if(id == 1001)
    { // layer
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    return 0;
}

/**
 * @brief EllipseE::verify
 *
 * Entry point for the Ellipse Verify Command
 *
 * KMJ : Working in the Qt version
 *
 */
void EllipseE::verify(void)
{
 Dialog dialog("EllipseVerify_Dialog");
 Workplane *wp;
 int c,l,s;
 double length,sta,swa,maj,min,w;
 Point xa,ya,o,zero(0.0,0.0,0.0);
 RCCHAR sorigin[300],sxaxis[300],syaxis[300];

  if ((wp = db.workplanes.getcurrent()) != NULL)
    {  o = wp->modeltoworkplane(origin);
       xa = wp->modeltoworkplane(xaxis) - wp->modeltoworkplane(zero);
       ya = wp->modeltoworkplane(yaxis) - wp->modeltoworkplane(zero);
    }
  else
    {  o = origin;
       xa = xaxis;
       ya = yaxis;
    }
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new EllipseVerifyButtonControl(1000));
  dialog.add(new EllipseVerifyButtonControl(1001));
  dialog.add(new EllipseVerifyButtonControl(1002));
  dialog.add(new EllipseVerifyButtonControl(1003));

  dialog.add(new StringDialogControl(104,o.string(sorigin),128));
  dialog.add(new StringDialogControl(105,xa.string(sxaxis),128));
  dialog.add(new StringDialogControl(106,ya.string(syaxis),128));
  maj = major;  dialog.add(new RealDialogControl(107,&maj,ScaleByLength));
  min = minor;  dialog.add(new RealDialogControl(108,&min,ScaleByLength));
  sta = starta * 180.0 / M_PI;  dialog.add(new RealDialogControl(109,&sta));
  swa = sweepa * 180.0 / M_PI;  dialog.add(new RealDialogControl(110,&swa));
  length = sweepa * major;   dialog.add(new RealDialogControl(111,&length,ScaleByLength));
  dialog.setposition(DPOK);
  if(dialog.process() == TRUE)
  {
      setcolour(c);
      setstyle(s);
      setlayer(l);
      setweight(db.lineweights.mmtoweight(w));
  }
}

void EllipseE::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1;
  if (vs == NULL)
    n1 = 20;
  else
    n1 = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = cos(starta);
  y1 = sin(starta);
  p1 = origin + xaxis * (x1 * major) + yaxis * (y1 * minor);
  if (vs == NULL)
    *pmin = *pmax = p1;
  else
    *pmin = *pmax = vs->modeltouv(p1);
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p1 = origin + xaxis * (x2 * major) + yaxis * (y2 * minor); 
       if (vs != NULL)
         p1 = vs->modeltouv(p1);
       if (p1.x < pmin->x) pmin->x = p1.x;
       if (p1.y < pmin->y) pmin->y = p1.y;
       if (p1.z < pmin->z) pmin->z = p1.z;
       if (p1.x > pmax->x) pmax->x = p1.x;
       if (p1.y > pmax->y) pmax->y = p1.y;
       if (p1.z > pmax->z) pmax->z = p1.z;
       x1 = x2;  y1 = y2;
    }
}

int EllipseE::issame(int geometry,const Entity &entity) const
{Point zaxis1,zaxis2;
 EllipseE *ellipse;
  if (! entity.isa(ellipse_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         ellipse = (EllipseE *)&entity;
         zaxis1 = ellipse->xaxis.cross(ellipse->yaxis).normalize();
         zaxis2 = xaxis.cross(yaxis).normalize();
         if (fabs(zaxis1.dot(zaxis2)) < 0.999999) return 0;
         if ((ellipse->origin - origin).length() > db.getptoler()) return 0;
         if (fabs(ellipse->major - major) > db.getptoler()) return 0;
         if (fabs(ellipse->minor - minor) > db.getptoler()) return 0;
         if (zaxis1.dot(zaxis2) > 0.0 &&
              (ellipse->position(0.0) - position(0.0)).length() < db.getptoler() &&
              (ellipse->position(1.0) - position(1.0)).length() < db.getptoler()) return 1;
         if (zaxis1.dot(zaxis2) < 0.0 &&
              (ellipse->position(1.0) - position(0.0)).length() < db.getptoler() &&
              (ellipse->position(0.0) - position(1.0)).length() < db.getptoler()) return -1;
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int EllipseE::explode(void)
{ return 0;
}

int EllipseE::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int result,cstate;
 double wt;
 Point p1;
 Transform t;

    if (action == PP_CLICK)
    {   if (id == 1)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (id == 2)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (id == 3)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (id == 4)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else
            return 1;
    }
    else if (action == PP_GETVALUE)
    {   if (id == 1)
            sprintf(text,"%d",getcolour());
        else if (id == 2)
            sprintf(text,"%d",getlayer());
        else if (id == 3)
            sprintf(text,"%d",getstyle());
        else if (id == 4)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (id == 6)
            db.formatnumber(text,origin.x,0);
        else if (id == 7)
            db.formatnumber(text,origin.y,0);
        else if (id == 8)
            db.formatnumber(text,origin.z,0);
        else if (id == 9)
            db.formatnumber(text,major,0);
        else if (id == 10)
            db.formatnumber(text,minor,0);
        else if (id == 11)
            db.formatnumber(text,starta * 180.0 / acos(-1.0),0);
        else if (id == 12)
            db.formatnumber(text,sweepa * 180.0 / acos(-1.0),0);
        else if (id == 13)
        {   db.formatnumber(text,length(),0);
            return 1;
        }
    }
    else if (action == PP_SETVALUE)
    {   if (id == 6)
        {   p1 = Point(atof(text),origin.y,origin.z);
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 7)
        {   p1 = Point(origin.x,atof(text),origin.z);
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 8)
        {   p1 = Point(origin.x,origin.y,atof(text));
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 9)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            major = atof(text);
            draw(DM_NORMAL);
        }
        else if (id == 10)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            minor = atof(text);
            draw(DM_NORMAL);
        }
        else if (id == 11)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            starta = atof(text) / 180.0 * acos(-1.0);
            draw(DM_NORMAL);
        }
        else if (id == 12)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            sweepa = atof(text) / 180.0 * acos(-1.0);
            draw(DM_NORMAL);
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Ellipse");
            return 1;
        }
        else 
        {   if (id == 1)
                strcpy(text,"Colour");
            else if (id == 2)
                strcpy(text,"Layer");
            else if (id == 3)
                strcpy(text,"Style");
            else if (id == 4)
                strcpy(text,"Weight");
            else if (id == 6)
                strcpy(text,"Origin X");
            else if (id == 7)
                strcpy(text,"Origin Y");
            else if (id == 8)
                strcpy(text,"Origin Z");
            else if (id == 9)
                strcpy(text,"Major Length");
            else if (id == 10)
                strcpy(text,"Minor Length");
            else if (id == 11)
                strcpy(text,"Start Angle");
            else if (id == 12)
                strcpy(text,"Sweep Angle");
            else if (id == 13)
                strcpy(text,"Length");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 14; 
    
    return 0;
}


