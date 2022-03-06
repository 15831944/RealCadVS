
#include "ncwin.h"

#define PAR_TOL 1.0E-10

Curve::Curve(int n1,int d,Point *p,double *k,double *w,const BitMask &o) :
  options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgih=0;
    usedByPlane=false;
#endif

  int i;
  n = short(n1);
  if (d > n1 - 1)
    d = n1 - 1;
  degree = short(d);
  if (degree == 0)
    degree = 1;
  if ((polygon = new Point[n]) == NULL)
    {  defined = 0;  return;
    }
  knot = tknot = tbasis = NULL;
  if (k != NULL && (knot = new double[n+degree+1]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  cweight = NULL;
  if (w != NULL && (cweight = new double[n]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++) polygon[i] = p[i];
  if (k != NULL)
    {  for (i = 0 ; i < n + degree + 1 ; i++) knot[i] = k[i];
    }
  else
    {  tempknots(&tknot);
       if (tknot == NULL)
         {  delete [] knot;  delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (w != NULL)
    {  for (i = 0 ; i < n ; i++) cweight[i] = w[i];
    }
  options = o;
  property_point_index = 0;
}

Curve::Curve(int n1,int d,Point *p,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgih=0;
    usedByPlane=false;
#endif

 int i,j,base;
 double l1,l2;
 Point *ptemp;
 Matrix a(n1);
  n = short(n1);
  if (d > n1 - 1)
    d = n1 - 1;
  degree = short(d);
  if (degree == 0)
    degree = 1;
  for (i = 1,l1 = 0.0 ; i < n1 ; i++)
    l1 += (p[i] - p[i-1]).length();
  if (l1 < db.getptoler())
    {  defined = 0;  return;
    }
  if ((polygon = new Point[n]) == 0)
    {  defined = 0;  return;
    }
  cweight = 0;
  knot = tknot = tbasis = 0;
  tempknots(&tknot);
  if (tknot == 0)
    {  delete[] polygon;  defined = 0;  return;
    }
  newbasis();
  if (tbasis == 0)
    {  delete[] tknot;  delete [] polygon;  defined = 0;  return;
    }

  for (i = 0,l2 = 0.0 ; i < n ; i++)
    {  rbasis(l2/l1,tknot,tbasis,&base);
       for (j = 0 ; j < n ; j++)
         a(i,j) = tbasis[j];
       if (i < n - 1)
         l2 += (p[i+1] - p[i]).length();
    }
  deletebasis();
  if ((ptemp = new Point[n]) == 0)
    {  delete[] tknot;  delete [] polygon;  delete [] tbasis;  defined = 0;  return;
    }
  memcpy(ptemp,p,n * sizeof(Point));

  if (! a.solve(polygon,ptemp))
    {  delete[] tknot;  delete [] polygon;  delete [] tbasis;  delete [] ptemp;  defined = 0;  return;
    }
  l1 = 0.0;
  for (i = 1 ; i < n1 ; i++)
    l1 += (p[i] - p[i-1]).length();

  l2 = 0.0;
  for (i = 1 ; i < n1 ; i++)
    l2 += (polygon[i] - polygon[i-1]).length();
  if (l2 > l1 * 10.0)
    {  delete[] tknot;  delete [] polygon;  delete [] tbasis;  delete [] ptemp;  defined = 0;  return;
    }
  delete [] ptemp;
  options = o;
  property_point_index = 0;
}

Curve::Curve(int n1,int d,int n2,Point *p,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgih=0;
    usedByPlane=false;
#endif

 int i,j,k,base;
 double l1,l2,*t;
 Point *ptemp;
 Matrix a(n1-2);
  n = short(n1);
  if (d > n1 - 1)
    d = n1 - 1;
  degree = short(d);
  if (degree == 0)
    degree = 1;
  for (i = 1,l1 = 0.0 ; i < n2 ; i++)
    l1 += (p[i] - p[i-1]).length();
  if (l1 < db.getptoler())
    {  defined = 0;  return;
    }
  if ((polygon = new Point[n]) == 0)
    {  defined = 0;  return;
    }
  t = new double[n2];
  t[0] = 0.0;
  for (i = 1,l2 = 0.0 ; i < n2 ; i++)
    {  l2 += (p[i] - p[i-1]).length();
       t[i] = l2 / l1;
    }
  cweight = 0;
  knot = tknot = tbasis = 0;
  tempknots(&tknot);
  if (tknot == 0)
    {  delete[] polygon; delete [] t; defined = 0;  return;
    }
  newbasis();
  if (tbasis == 0)
    {  delete[] tknot; delete [] polygon; delete [] t; defined = 0;  return;
    }
  for (i = 0 ; i < n-2 ; i++)
    {  for (j = 0 ; j < n-2 ; j++)
         a(i,j) = 0;
       polygon[i].setxyz(0.0,0.0,0.0);
    }

  for (k = 0 ; k < n2 - 2 ; k++)
    {  rbasis(t[k+1],tknot,tbasis,&base);
       for (i = 0 ; i < n-2 ; i++)
         {  for (j = 0 ; j < n-2 ; j++)
              a(i,j) += tbasis[i+1] * tbasis[j+1];
            polygon[i] += p[k+1] * tbasis[i+1] - p[0] * tbasis[0] * tbasis[i+1] - p[n2-1] * tbasis[n-1] * tbasis[i+1];
         }
    }

  deletebasis();
  if ((ptemp = new Point[n]) == 0)
    {  delete[] tknot;  delete [] polygon;  delete [] tbasis; delete [] t; defined = 0;  return;
    }
  if (! a.solve(ptemp,polygon))
    {  delete[] tknot;  delete [] polygon;  delete [] tbasis;  delete [] ptemp;  delete [] t; defined = 0;  return;
    }
  polygon[0] = p[0];
  for (i = 1 ; i < n - 1 ; i++)
    polygon[i] = ptemp[i-1];
  polygon[i] = p[n2-1];

  delete [] t;
  delete [] ptemp;
  options = o;
  property_point_index = 0;
}

void Curve::tempknots(double **tknot)
{int i;
  if ((*tknot = new double[n+degree+1]) == NULL) return;
  for (i = 0 ; i < degree + 1 ; i++)
    (*tknot)[i] = 0.0;
  for (i = degree + 1 ; i < n ; i++)
    (*tknot)[i] = double(i - degree) / double(n - degree);
  for (i = n ; i < n + degree + 1 ; i++)
    (*tknot)[i] = 1.0;
}

void Curve::param(Point *points,double **p)
{int i;
 double l1,l2;
  if ((*p = new double [n]) == 0) return;
  (*p)[0] = 0.0;
  for (i = 1,l1 = 0.0 ; i < n ; i++)
    l1 += (points[i] - points[i-1]).length();
  for (i = 1,l2 = 0.0 ; i < n ; i++)
    {  l2 += (points[i] - points[i-1]).length();
       (*p)[i] = l2 / l1;
    }
}


Curve::Curve(const EntityHeader &header,int n1,int d,Point *p,double *k,double *w,const BitMask &o) :
  LinearEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgih=0;
    usedByPlane=false;
#endif

  int i;
  if (d > n1 - 1)
    d = n1 - 1;
  n = short(n1);
  degree = short(d);
  if ((polygon = new Point[n]) == NULL)
    {  defined = 0;  return;
    }
  knot = tknot = tbasis = NULL;
  if (k != NULL && (knot = new double[n+degree+1]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  cweight = NULL;
  if (w != NULL && (cweight = new double[n]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++) polygon[i] = p[i];

  if (k != NULL)
    {  for (i = 0 ; i < n + degree + 1 ; i++) 
         knot[i] = k[i];
    }
  else
    {  tempknots(&tknot);
       if (tknot == NULL)
         {  delete [] knot;  delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (w != NULL)
    {  for (i = 0 ; i < n ; i++) cweight[i] = w[i];
    }
  options = o;
  property_point_index = 0;
}

Curve::~Curve()
{ if (defined)
    {  delete [] polygon;
       delete [] knot;
       delete [] tknot;
       delete [] cweight;
       delete [] tbasis;
    }
}

int Curve::nsegments(double error)
{int i,nsegs;
 double l;

  if ((degree == 0 || degree == 1) && n > 1)
    return nsegs = n - 1;


  for (i = 1,l = 0.0 ; i < n ; i++)
    l += fabs(polygon[i].x - polygon[i-1].x) +
         fabs(polygon[i].y - polygon[i-1].y) +
         fabs(polygon[i].z - polygon[i-1].z);
  nsegs = int(0.05 * l / error);
  if (cweight != 0)
     nsegs *= 2;
  if (nsegs > 0 && n > 2)
    nsegs = ((nsegs + n - 2 ) / (n-1)) * (n-1);
  if (nsegs < 4) nsegs = 4;
  if (nsegs > 3000) nsegs = 3000;
  if ((degree == 0 || degree == 1) && n > 1)
    nsegs = (n - 1) * (nsegs / (n-1) + 1);
  return nsegs;
}

void Curve::segment(int n,int include,Point *verts)
{int i,j;
 double t,dt;
 Point p3,p4,dir;
  if (include & LE_REVERSE)
    {  t = 1.0;  dt = -1.0 / double(n);
    }
  else
    {  t = 0.0;  dt =  1.0 / double(n);
    }
  j = 0;
  if (include & LE_INCLUDESTART)
    verts[j++] = position(t);
  if (! (include & LE_INCLUDEEND)) n--;
  for (i = 0,t += dt ; i < n ; i++,t += dt)
    verts[j++] = position(t);
}

void Curve::newbasis(void)
{
    delete [] tbasis;
    tbasis = new double[n + degree];
}

void Curve::deletebasis(void)
{ delete [] tbasis;  tbasis = NULL;
}

void Curve::addtodisplaylist(View3dSurface *vs,int child)
{

  Point pt1,pt2,pt3,pt4;
 int i,nseg;
 double t,dt;

#if 0
    QGraphicsItem *qline = 0;
    QList<QGraphicsItem *> glist;

    if(qgi)
     vs->getScene()->removeItem(qgi);
    delete qgi;
    qgi = 0;
#endif

  nseg = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,nseg * 34)) return;
    }
  t = 0.0;  dt = 1.0 / double(nseg);
  newbasis();
  pt1 = vs->modeltoview(position(t));
  for (i = 0,t = dt ; i < nseg ; i++,t += dt)
    {  pt2 = vs->modeltoview(position(t));
       pt3 = pt1;  pt4 = pt2;
       if (vs->clip3dfb(&pt3,&pt4))
         {  pt3 = vs->viewtoscreen(pt3);
            pt4 = vs->viewtoscreen(pt4);
            if (! vs->displaylist.moveto(pt3.x,pt3.y))
            { deletebasis();  return;  }
            if (! vs->displaylist.lineto(pt4.x,pt4.y))
            { deletebasis(); return;  }
#if 0
            // add the line to the scene as well
            QPointF qp1 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt3.x,pt3.y);
            QPointF qp2 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt4.x,pt4.y);
            if(vs->getScene())
                qline = vs->getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y());
            if(qline)
                glist.append(qline);
#endif
         }
       pt1 = pt2;
    }
  deletebasis();

#if 0
  if(glist.size() > 0)
      qgi = vs->getScene()->createItemGroup(glist);
  if(qgi)
  {
      if(getselected())
          qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
      else
          qgi->setZValue(zValue);
  }
#endif
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void Curve::drawGL(int drawmode,Transform *trans,View3dSurface *vs)
{
    Point pt1,pt2,pt3,pt4;
    int i,nseg;
    double t,dt;

    QGraphicsItem *qline = 0;
    QList<QGraphicsItem *> glist;

    //qDebug() << "Enterting Curve::draw() : qgi : " << qgi;
    // get rid of the scene version
    draw(DM_ERASE,trans,vs);

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
     vs->setupGL(linestylescale,0.0,drawmode,colour,style,weight,patternlinestyle);
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
     if (trans == NULL && patternlinestyle == 0 && ! gethandles() && ! DM_SELECT)
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

     nseg = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     t = 0.0;  dt = 1.0 / double(nseg);
     newbasis();
     //if (trans != NULL)
     //  pt1 = vs->modeltoview(trans->transform(position(t)));
     //else
     //  pt1 = vs->modeltoview(position(t));
     //if (trans != NULL)
     //  pt1 = trans->transform(position(t));
     //else
     //  pt1 = position(t);
     if (trans != NULL)
       pt1 = trans->transform(vs->modeltransform.transform(position(t)));
     else
       pt1 = vs->modeltransform.transform(position(t));
     //pt1 = vs->modeltransform.transform(pt1);
     for (i = 0,t = dt ; i < nseg ; i++,t += dt)
     {
          //if (trans != NULL)
          //  pt2 = vs->modeltoview(trans->transform(position(t)));
          //else
          //  pt2 = vs->modeltoview(position(t));
          //if (trans != NULL)
          //  pt2 = trans->transform(position(t));
          //else
          //  pt2 = position(t);
          if (trans != NULL)
            pt2 = trans->transform(vs->modeltransform.transform(position(t)));
          else
            pt2 = vs->modeltransform.transform(position(t));
          //pt2 = vs->modeltransform.transform(pt2);
          pt3 = pt1;  pt4 = pt2;
          //if (vs->clip3d(&pt3,&pt4))
          //{
               //pt3 = vs->viewtoscreen(pt3);
               //pt4 = vs->viewtoscreen(pt4);
               //vs->line(pt3.x,pt3.y,pt4.x,pt4.y);

          if(patternlinestyle !=0)
          {
              // drawing patternstyle with GL in screen space
              Point spt3 = vs->modeltoscreen(pt3);
              Point spt4 = vs->modeltoscreen(pt4);
              vs->line2dGL(spt3.x,spt3.y,spt4.x,spt4.y);
          }/*
          else if(vs->getlinestyle() !=0)
          {
              // drawing patternstyle with GL in screen space
              Point spt3 = vs->modeltoscreen(pt3);
              Point spt4 = vs->modeltoscreen(pt4);
              vs->line2dGL(spt3.x,spt3.y,spt4.x,spt4.y);
          }*/
          else
              vs->lineGL(pt3.x,pt3.y,pt3.z,pt4.x,pt4.y,pt4.z);
          //}
          pt1 = pt2;
     }
     deletebasis();
     if (patternlinestyle != 0)
         patternlinestyle->endGL(vs);

     if (gethandles())
     {
          vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_HANDLE_SELECT,colour,0,0,0);
          //vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
          RCCOLORREF pcol;
          pcol = vs->getcolour();
          //GLfloat fr=GetRValue(pcol)/255.0;
          //GLfloat fg=GetGValue(pcol)/255.0;
          //GLfloat fb=GetBValue(pcol)/255.0;
          //glColor3f(fr,fg,fb);
          glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
          GLfloat lwidth = vs->curPen.widthF();
          glLineWidth(lwidth<1.0?1.0:lwidth);

          Point pright = vs->getuaxis();
          Point pup = vs->getvaxis();
          double sc = vs->getpixelscale() * 6.0;

          for (i = 0 ; i < n ; i++)
          {
               //pt1 = vs->modeltoscreen(polygon[i]);
               pt1 = polygon[i];
               vs->cbox2dGL(pt1,pright,pup,sc,sc);
          }
     }

     //qDebug() << "Leaving Curve::draw() : qgi : " << qgi;
}
#endif

void Curve::draw(int drawmode,Transform *trans,View3dSurface *vs)
{
#ifdef USING_WIDGETS
    Point pt1,pt2,pt3,pt4;
    int i,nseg;
    double t,dt;

    QGraphicsItem *qline = 0;
    //QList<QGraphicsItem *> glist;

    //qDebug() << "Enterting Curve::draw() : qgi : " << qgi;

    if(drawmode != DM_PRINT)
    {
      if(qgi && qgi->scene() != vs->getScene())
           qgi->scene()->removeItem(qgi);
      else if(qgi && vs->getScene())
          vs->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      // handles
      for (i = 0 ; i < qgHandles.size() ; i++)
      {
          if(qgHandles[i] && qgHandles[i]->scene() != vs->getScene())
          {
              QGraphicsItem *handle = qgHandles[i];
              if(qgHandles[i]->scene())
                qgHandles[i]->scene()->removeItem(handle);
          }
          else if(qgHandles[i])
              vs->getScene()->removeItem(qgHandles[i]);
          //qDebug() << "qgHandles[" << i << "]" << qgHandles[i];
          delete qgHandles[i];
          qgHandles[i] = 0;
      }
      qgHandles.clear();

      if(qgih && qgih->scene() == vs->getScene())
          vs->getScene()->removeItem(qgih);
      delete qgih; qgih=0;
    }

      if(drawmode == DM_ERASE)
          return;

#if 0
  qgi = new QGraphicsItemGroup();
#else
  qgi = new QGraphicsPathItem();
  QPainterPath pPath;
#endif

     if (getselected() && drawmode == DM_NORMAL)
         drawmode = DM_SELECT;
     vs->setup(linestylescale,0.0,drawmode,colour,style,weight,patternlinestyle);

#if 0
     if (trans == NULL && patternlinestyle == 0 && ! gethandles() && ! DM_SELECT)
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

     nseg = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     t = 0.0;  dt = 1.0 / double(nseg);
     newbasis();
     if (trans != NULL)
       pt1 = vs->modeltoview(trans->transform(position(t)));
     else
       pt1 = vs->modeltoview(position(t));
     for (i = 0,t = dt ; i < nseg ; i++,t += dt)
       {  if (trans != NULL)
            pt2 = vs->modeltoview(trans->transform(position(t)));
          else
            pt2 = vs->modeltoview(position(t));
          pt3 = pt1;  pt4 = pt2;
          if (vs->clip3d(&pt3,&pt4))
            {  pt3 = vs->viewtoscreen(pt3);
               pt4 = vs->viewtoscreen(pt4);
#if 0
               qline = vs->line((QGraphicsItem*)0,pt3.x,pt3.y,pt4.x,pt4.y);
               if(qline)
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
#else
               vs->line(&pPath,pt3.x,pt3.y,pt4.x,pt4.y);
#endif
            }
          pt1 = pt2;
       }
     deletebasis();
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

     if (gethandles())
     {

          QGraphicsItem *qgout = 0,*qgh = 0;
          QList<QGraphicsItem *> hglist;

          for (i = 0 ; i < qgHandles.size() ; i++)
          {
              if(qgHandles[i] && qgHandles[i]->scene() != vs->getScene())
                  qgHandles[i]->scene()->removeItem(qgHandles[i]);
              else
                  vs->getScene()->removeItem(qgHandles[i]);
              delete qgHandles[i];
              qgHandles[i] = 0;
          }
          qgHandles.clear();

          vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

          for (i = 0 ; i < n ; i++)
          {
               pt1 = vs->modeltoscreen(polygon[i]);

               qgout = vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2); if( qgout != 0 ) hglist.append( qgout );
               qgout = vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2); if( qgout != 0 ) hglist.append( qgout );
               qgout = vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2); if( qgout != 0 ) hglist.append( qgout );
               qgout = vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2); if( qgout != 0 ) hglist.append( qgout );

               if(hglist.size() > 0)
               {
                   qgh=0;
                   qgh = vs->getScene()->createItemGroup(hglist);
                   if(qgh)
                   {
                       if(getselected())
                           qgh->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                       else
                           qgh->setZValue(zValue);
                       qgHandles.append(qgh);
                   }
                   hglist.clear();
               }
          }

     }
     else
     {
         for (i = 0 ; i < qgHandles.size() ; i++)
         {
             if(qgHandles[i] && qgHandles[i]->scene() != vs->getScene())
                 qgHandles[i]->scene()->removeItem(qgHandles[i]);
             else
                 vs->getScene()->removeItem(qgHandles[i]);
             delete qgHandles[i];
             qgHandles[i] = 0;
         }
         qgHandles.clear();
     }
     //qDebug() << "Leaving Curve::draw() : qgi : " << qgi;
#else
 Point pt1,pt2,pt3,pt4;
 int i,nseg;
 double t,dt;

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,patternlinestyle);
  if (trans == NULL && patternlinestyle == 0 && ! gethandles() && ! DM_SELECT)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  nseg = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  t = 0.0;  dt = 1.0 / double(nseg);
  newbasis();
  if (trans != NULL)
    pt1 = vs->modeltoview(trans->transform(position(t)));
  else
    pt1 = vs->modeltoview(position(t));
  for (i = 0,t = dt ; i < nseg ; i++,t += dt)
    {  if (trans != NULL)
         pt2 = vs->modeltoview(trans->transform(position(t)));
       else
         pt2 = vs->modeltoview(position(t));
       pt3 = pt1;  pt4 = pt2;
       if (vs->clip3d(&pt3,&pt4))
         {  pt3 = vs->viewtoscreen(pt3);
            pt4 = vs->viewtoscreen(pt4);
            vs->line(pt3.x,pt3.y,pt4.x,pt4.y);
         }
       pt1 = pt2;
    }
  deletebasis();
  if (patternlinestyle != 0)
    patternlinestyle->end();

  if (gethandles())
  {

       vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

       for (i = 0 ; i < n ; i++)
       {
            pt1 = vs->modeltoscreen(polygon[i]);
            vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
            vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
            vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
            vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);
       }
  }
#endif
}

int Curve::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1,d2;
 int i,handle;
  handle = 0;
  d1 = -1.0;
  for (i = 0 ; i < n ; i++)
    {  if ((d2 = vs->pick(polygon[i],distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
         {  handle = i + 1;  
            *pos = polygon[i];
            d1 = d2;
         }
    }
  return handle;
}

void Curve::movehandlepoint(View3dSurface *,int drawentity,int end,Point p)
{ if (GetLockedLayerTable()->test(layer)) return;  
  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  polygon[end-1] = p;
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

#if USING_WIDGETS
int Curve::hideGL(HideImage *image)
{Point pt1,pt2,pt3,pt4;
 int i,n;
 double t,dt;

 image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       image->hideGL(this,pt1,pt2);
       pt1 = pt2;
    }
  deletebasis();

  return 1;
}

int Curve::hide(HideImage *image)
{Point pt1,pt2,pt3,pt4;
 int i,n;
 double t,dt;

 if(qgi && qgi->scene() != image->getsurface()->getScene())
      qgi->scene()->removeItem(qgi);
 else if(qgi && image->getsurface()->getScene())
     image->getsurface()->getScene()->removeItem(qgi);
 delete qgi;
 qgi=0;

 // handles
 for (i = 0 ; i < qgHandles.size() ; i++)
 {
     if(qgHandles[i] && qgHandles[i]->scene() != image->getsurface()->getScene())
     {
         QGraphicsItem *handle = qgHandles[i];
         if(qgHandles[i]->scene())
           qgHandles[i]->scene()->removeItem(handle);
     }
     else if(qgHandles[i])
         image->getsurface()->getScene()->removeItem(qgHandles[i]);
     //qDebug() << "qgHandles[" << i << "]" << qgHandles[i];
     delete qgHandles[i];
     qgHandles[i] = 0;
 }
 qgHandles.clear();

 if(qgih && qgih->scene() == image->getsurface()->getScene())
     image->getsurface()->getScene()->removeItem(qgih);
 delete qgih; qgih=0;

 qgi = new QGraphicsItemGroup();

 image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       image->hide(this,pt1,pt2);
       pt1 = pt2;
    }
  deletebasis();

  if(qgi)
  {
      if(image->getsurface()->getScene())
          image->getsurface()->getScene()->addItem(qgi);
  }

  return 1;
}
#else
int Curve::hide(HideImage *image)
{Point pt1,pt2,pt3,pt4;
 int i,n;
 double t,dt;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       image->hide(this,pt1,pt2);
       pt1 = pt2;
    }
  deletebasis();
  return 1;
}
#endif

double Curve::pick(View3dSurface *vs,double x,double y,Point *pos)
{Point pt1,pt2;
 int i,n;
 double d,t,dt;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (pos == NULL && vs->displaylist.defined(this))
    return vs->displaylist.pick(this,x,y);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       if ((d = vs->pick(pt1,pt2,x,y,pos)) >= 0.0)
         {  deletebasis();  return d;
         }
       pt1 = pt2;
    }
  deletebasis();
  return -1.0;
}

int Curve::pickendpoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d1,d2;
 int i,end;
  end = 0;
  if ((d1 = vs->pick(polygon[0],distance,x,y)) >= 0.0)
    {  end = 1;  *pos = polygon[0];
    }
  if ((d2 = vs->pick(polygon[n-1],distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = polygon[n-1];
    }

  if (degree == 0 || degree == 1)
    {  for (i = 0 ; i < n ; i++)
         {  if ((d2 = vs->pick(polygon[i],distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
              {  end = 3 + i;  *pos = polygon[i];  d1 = d2;
              }
         }
    }

  return end;
}

void Curve::moveendpoint(int drawentity,int end,Point p)
{double t;
 Curve *curve;

  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  nearp(0,p,&t);
  if (t > PAR_TOL && t < 1.0 - PAR_TOL)
    {  if (end == 2)
         subdivide(t,NULL,&curve);
       else
         subdivide(t,&curve,NULL);
       delete curve;
    }
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

double Curve::gett(Point p)
{double t1;
  nearp(0,p,&t1);
  return t1;
}

void Curve::breakanddelete(int,Point p1,Point p2)
{double t1,t2,t;
 Point p;
 Curve *c1,*c2;
 Transform identity;

  if (GetLockedLayerTable()->test(layer)) return;  

  p1 = nearp(0,p1,&t1);
  p2 = nearp(0,p2,&t2);
  if (t1 > t2)
    {  t = t1;  t1 = t2;  t2 = t;
    }
  if (t2 <= 0.0 || t1 >= 1.0) return;
  if (t1 <= 0.0 && t2 >= 1.0) return;
  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  if (t1 <= PAR_TOL)
    {  subdivide(t2,&c1,NULL);  delete c1;
    }
  else if (t2 >= 1.0 - PAR_TOL)
    {  subdivide(t1,NULL,&c1);  delete c1;
    }
  else
    {  identity.identity();
       copye = NULL;
       c1 = (Curve *) clone(identity);
       subdivide(t1,NULL,&c2);  delete c2;
       c1->subdivide(t2,&c2,NULL);  delete c2;
       db.geometry.add(c1);
    }
  cadwindow->invalidatedisplaylist(this);
  draw(DM_NORMAL);
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int Curve::trace(View3dSurface *surface,Point end,Point uv1,Point uv2,double side,Point *newend,Point *uv3,Point *uv4,double *angle)
{double cosa,l1,l2;
 Point uv5,uv6,duv1,duv2;
 int i;
  for (i = 1 ; i < n ; i++)
    if ((polygon[i]-polygon[0]).length() > db.getptoler())
      break;
  if (i == n)
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

double Curve::pickhor(View3dSurface *vs,int x,int y,Point *start,Point *end,Point *uvp1,Point *uvp2,double *side)
{Point pt1,pt2,p1,p2;
 int i,n;
 double d,dmin,t,dt,s;
  if (start == 0 && vs->displaylist.defined(this))
    return vs->displaylist.pickhor(this,x,y);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);  dmin = -1.0;
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       if ((d = vs->pickhor(pt1,pt2,x,y,&p1,&s)) >= 0.0 && (dmin == -1.0 || d < dmin))
         {  dmin = d;
            p2 = p1;
            *side = s;
         }
       pt1 = pt2;
    }
  deletebasis();
  if (dmin >= 0.0 && start != 0)
    {  if ((position(0.0) - p2).length() < (position(1.0) - p2).length())
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

void Curve::insertpoint(int drawentity,int n1,int n2,Point p)
{Point *poly;
 double *cw,*k;
 int i;
  deletebasis();
  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);

  if ((poly = new Point[n+1]) == 0) return;
  if (cweight != 0 && (cw = new double[n+1]) == 0)
    {  delete [] poly;  return;
    }
  n++;
  tempknots(&k);
  n--;
  if (k == 0)
    {  if (cweight != 0) delete [] cw;  delete [] poly;  return;
    }

  if (n2 < n1) n1 = n2;
  n1++;
  for (i = 0 ; i < n1 ; i++)
    {  poly[i] = polygon[i];
       if (cweight != 0) cw[i] = cweight[i];
    }
  poly[n1] = p;
  if (cweight != 0) cw[n1] = 1.0;
  for (i = n1 ; i < n ; i++)
    {  poly[i+1] = polygon[i];
       if (cweight != 0) cw[i+1] = cweight[i];
    }
  delete [] polygon;
  polygon = poly;
  if (cweight != 0)
    {  delete [] cweight;
       cweight = cw;
    }
  if (knot != 0)
    {  delete [] knot;
       knot = k;
    }
  else
    {  delete [] tknot;
       tknot = k;
    }
  n++;
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

void Curve::movepoint(int drawentity,int index,Point p)
{Entity *e;
  db.saveundo(UD_MOVED,this);
  if (drawentity)
    {  draw(DM_ERASE);
       for (alist.start() ; (e = alist.next()) != 0 ; )
         e->draw(DM_ERASE);
    }
  cadwindow->invalidatedisplaylist(this);
  for (alist.start() ; (e = alist.next()) != 0 ; )
    cadwindow->invalidatedisplaylist(e);
  polygon[index] = p;
  if (drawentity)
    {  draw(DM_NORMAL);
       for (alist.start() ; (e = alist.next()) != 0 ; )
         e->draw(DM_NORMAL);
    }
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

LinearEntity *Curve::divide(Point p, Point *dp)
{double t;
 Curve *curve;

  if (GetLockedLayerTable()->test(layer)) return 0;  

  curve = 0;
  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,this);
  p = nearp(0,p,&t);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (t > PAR_TOL && t < 1.0 - PAR_TOL)
    {  subdivide(t,NULL,&curve);
       if (curve != 0)
         db.geometry.add(curve);
    }
  PointE point(p);
  //point.draw(DM_NORMAL);
  point.draw(DM_MARKER);
  draw(DM_NORMAL);
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
  return curve;
}

void Curve::subdivide(double t,Curve **c1,Curve **c2)
{Point *npolygon;
 double *nknot,*nweight,*tbasis,t1,t2,t3;
 int i,knotpos[2],knotmult[1];

  npolygon = NULL;  nweight = NULL;  nknot = NULL;  tbasis = NULL;
  if (! insertknots(1,&t,knotpos,knotmult,&npolygon,&nweight,&nknot,&tbasis))
    {  if (c1 != NULL) *c1 = NULL;  if (c2 != NULL) *c2 = NULL;
       return;
    }
  t1 = nknot[0];
  t2 = nknot[knotpos[0]];
  t3 = nknot[knotpos[1]-1];
  for (i = 0 ; i < knotpos[0] + degree + 1 ; i++)
    nknot[i] = (nknot[i] - t1) / (t2 - t1);
  if (c1 != NULL)
    *c1 = new Curve(*this,knotpos[0],degree,npolygon,nknot,nweight,options);
  else
    {  n = short(knotpos[0]);
       for (i = 0 ; i < n ; i++)
         {  polygon[i] = npolygon[i];
            if (cweight != NULL) cweight[i] = nweight[i];
         }
       if (knot == NULL)
         {  knot = tknot;  tknot = NULL;
         }
       for (i = 0 ; i < n + degree + 1 ; i++)
         knot[i] = nknot[i];
    }
  for (i = knotpos[0] ; i < knotpos[0] + degree + 1 ; i++)
    nknot[i] = 0.0;
  for (i = knotpos[0] + degree + 1 ; i < knotpos[1] ; i++)
    nknot[i] = (nknot[i] - t2) / (t3 - t2);
  if (c2 != NULL)
    *c2 = new Curve(*this,knotpos[1] - degree - 1 - knotpos[0],degree,npolygon+knotpos[0],nknot+knotpos[0],(nweight == NULL ? NULL : nweight+knotpos[0]),options);
  else
    {  n = short(knotpos[1] - degree - 1 - knotpos[0]);
       for (i = 0 ; i < n ; i++)
         {  polygon[i] = npolygon[i + knotpos[0]];
            if (cweight != NULL) cweight[i] = nweight[i + knotpos[0]];
         }
       if (knot == NULL)
         {  knot = tknot;  tknot = NULL;
         }
       for (i = 0 ; i < n + degree + 1 ; i++)
         knot[i] = nknot[i + knotpos[0]];
    }
  delete [] npolygon;  delete [] nknot;  delete [] nweight;  delete [] tbasis;
}

int Curve::insertknots(int nknots,double *newknots,int *knotpos,int *knotmult,
                       Point **npolygon,double **nweight,double **nknot,double **tbasis) const
{double *oknot,denom,d,e;
 int i,j,k,l,m,base;
  if (*npolygon == NULL)
    {  *npolygon = new Point[n + (degree + 1) * nknots];
    }
  if (knot != NULL) oknot = knot; else oknot = tknot;
  if (*nknot == NULL)
    {  *nknot = new double [n + (degree + 1) * (nknots + 1)];
    }
  if (cweight == NULL)
    *nweight = NULL;
  else if (*nweight == NULL)
    {  *nweight = new double[n + (degree + 1) * nknots];
    }
  if (*tbasis == NULL)
    {  *tbasis = new double[n + degree];
    }
  if (*npolygon == NULL || *nknot == NULL || (*nweight == NULL && cweight != NULL) || *tbasis == NULL || knotmult == NULL)
    {  delete [] *npolygon;  delete [] *nknot;  delete [] *nweight;  delete [] *tbasis;
       return 0;
    }
  for (l = 0 ; l < nknots ; l++)
    {  knotpos[l] = -1;
       knotmult[l] = 0;
    }
  for (i = 0,j = 0,l = 0 ; i < n + degree + 1 ; i++,j++)
    {  while (l < nknots && oknot[i] > newknots[l] + PAR_TOL)
         {  if (knotpos[l] < 0) knotpos[l] = j;
            for (; knotmult[l] < degree + 1 ; knotmult[l]++,j++) (*nknot)[j] = newknots[l];
            l++;
         }
       if (l < nknots && fabs(oknot[i] - newknots[l]) <= PAR_TOL)
         {  if (knotpos[l] < 0) knotpos[l] = j;
            (*nknot)[j] = newknots[l];
            knotmult[l]++;
         }
       else
         (*nknot)[j] = oknot[i];
    }
  knotpos[nknots] = j;
  m = j - degree - 1;
  for (j = 0 ; j < m ; j++)
    {  base = n + degree - 1;
       for (i = 0 ; i < n + degree ; i++)
         {  (*tbasis)[i] = oknot[i] <= (*nknot)[j] && (*nknot)[j] < oknot[i+1];
            if ((*tbasis)[i]) base = i;
         }
       for (k = 2 ; k <= degree + 1 ; k++)
         {  l = base;  if (base > n + degree - k) l = n + degree - k;
            for (i = base - k + 1 ; i <= l ; i++)
              {  denom = oknot[i+k-1] - oknot[i];
                 if (denom > 1.0E-10)
                   d = (*tbasis)[i] * ((*nknot)[j+k-1] - oknot[i]) / denom;
                 else
                   d = 0.0;
                 denom = oknot[i+k] - oknot[i+1];
                 if (denom > 1.0E-10)
                   e = (*tbasis)[i+1] * (oknot[i+k] - (*nknot)[j+k-1]) / denom;
                 else
                   e = 0.0;
                 (*tbasis)[i] = d + e;
              }
         }
       (*npolygon)[j].setxyz(0.0,0.0,0.0);
       if (*nweight != NULL) (*nweight)[j] = 0.0;
       l = base;  if (base > n - 1) l = n - 1;
       for (i = base - degree ; i <= l ; i++)
         {  (*npolygon)[j] += polygon[i] * (*tbasis)[i];
            if (*nweight != NULL) (*nweight)[j] += cweight[i] * (*tbasis)[i];
         }
    }
  return 1;
}

int Curve::posdir(double t,double **nknot,double **tbasis,Point *pos,Point *dir) const
{double *oknot,denom,d,e,w[2];
 int i,j,j1,j2,j3,k,l,knotpos,knotmult,base,inserted;
 Point p[2];
  if (knot != NULL) oknot = knot; else oknot = tknot;
  if (*nknot == NULL)
    {  *nknot = new double [n + (degree + 1) * 2];
    }
  if (*tbasis == NULL)
    {  *tbasis = new double[n + degree];
    }
  if (*nknot == NULL || *tbasis == NULL)
    {  delete [] *nknot;  delete [] *tbasis;
       return 0;
    }
  knotpos = -1;
  knotmult = 0;
  for (i = 0,j = 0,inserted = 0 ; i < n + degree + 1 ; i++,j++)
    {  if (! inserted && oknot[i] > t + PAR_TOL)
         {  if (knotpos < 0) knotpos = j;
            for (; knotmult < degree + 1 ; knotmult++,j++) (*nknot)[j] = t;
            inserted = 1;
         }
       if (! inserted && fabs(oknot[i] - t) <= PAR_TOL)
         {  if (knotpos < 0) knotpos = j;
            (*nknot)[j] = t;
            knotmult++;
         }
       else
         (*nknot)[j] = oknot[i];
    }
  if (t > 0.5)
    {  j1 = knotpos - 2;  j2 = knotpos - 1;
    }
  else
    {  j1 = knotpos;  j2 = knotpos + 1;
    }
  for (j = j1 ; j <= j2 ; j++)
    {  base = n + degree - 1;
       for (i = 0 ; i < n + degree ; i++)
         {  (*tbasis)[i] = oknot[i] <= (*nknot)[j] && (*nknot)[j] < oknot[i+1];
            if ((*tbasis)[i]) base = i;
         }
       for (k = 2 ; k <= degree + 1 ; k++)
         {  l = base;  if (base > n + degree - k) l = n + degree - k;
            for (i = base - k + 1 ; i <= l ; i++)
              {  denom = oknot[i+k-1] - oknot[i];
                 if (denom > 1.0E-10)
                   d = (*tbasis)[i] * ((*nknot)[j+k-1] - oknot[i]) / denom;
                 else
                   d = 0.0;
                 denom = oknot[i+k] - oknot[i+1];
                 if (denom > 1.0E-10)
                   e = (*tbasis)[i+1] * (oknot[i+k] - (*nknot)[j+k-1]) / denom;
                 else
                   e = 0.0;
                 (*tbasis)[i] = d + e;
              }
         }
       j3 = j - j1;
       p[j3].setxyz(0.0,0.0,0.0);
       if (cweight != NULL) w[j3] = 0.0;
       l = base;  if (base > n - 1) l = n - 1;
       for (i = base - degree ; i <= l ; i++)
         {  p[j3] += polygon[i] * (*tbasis)[i];
            if (cweight != NULL) w[j3] += cweight[i] * (*tbasis)[i];
         }
    }
  if (t > 0.5)
    {  *pos = p[1];  *dir = (p[1] - p[0]).normalize();
    }
  else
    {  *pos = p[0];  *dir = (p[1] - p[0]).normalize();
    }
  return 1;
}

void Curve::divide(int n, PointList *dplist)
{double t;
 int i;
 Curve *c1,*c2;
 Point p;

  if (GetLockedLayerTable()->test(layer)) return;  

  if (n < 2) return;
  draw(DM_ERASE);
  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,this);
  cadwindow->invalidatedisplaylist(this);
  c1 = this;
  for (i = 0 ; i < n - 1 ; i++)
    {  t = 1.0 / (n - i);
       p = c1->position(t);
       PointE point(p);
       //point.draw(DM_NORMAL);
       point.draw(DM_MARKER);
       c1->subdivide(t,NULL,&c2);
       if (i > 0)
         db.geometry.add(c1);
       c1 = c2;
    }
  db.geometry.add(c2);
  draw(DM_NORMAL);
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int Curve::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{Point pt1,pt2;
 int i,n;
 double t,dt;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    if (! vs->pick(position(t),inside,x1,y1,x2,y2))
      {  deletebasis();  return 0;
      }
  deletebasis();
  return 1;
}

int Curve::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Point pt1,pt2;
 int i,n,picked;
 double t,dt;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  dt = 1.0 / double(n);
  newbasis();
  pt1 = position(0.0);
  picked = inside != 2;
  for (i = 0,t = dt ; i < n ; i++,t += dt)
    {  pt2 = position(t);
       if (vs->pick(nvertices,pwvertices,pt1,pt2,inside,&picked))
         {  deletebasis();  return picked;
         }
       pt1 = pt2;
    }
  deletebasis();
  return picked;
}

double Curve::length(void) const
{Point pt1,pt2;
 int i,nseg;
 double l,t,dt;
  nseg = 200;
  t = 0.0;  dt = 1.0 / double(nseg);
  ((Curve *)this)->newbasis();
  pt1 = position(t);
  for (i = 0,t = dt,l = 0.0 ; i < nseg ; i++,t += dt)
    {  pt2 = position(t);
       l += (pt2 - pt1).length();
       pt1 = pt2;
    }
  ((Curve *)this)->deletebasis();
  return l;
}

void Curve::rbasis(double t,double *knot,double *basis,int *base) const
{int i,k,l,npluso;
 double d,e,sum;
  npluso = n + degree + 1;

  if (t < knot[0]) t = knot[0]; else if (t > knot[npluso-1]) t = knot[npluso-1];

  *base = n + degree - 1;
  for (i = 0 ; i < npluso - 1 ; i++)
    {  basis[i] = t >= knot[i] && t < knot[i+1];
       if (basis[i]) 
         *base = i;
    }
  if (*base < degree) *base = degree;

  for (k = 2 ; k <= degree + 1 ; k++)
    {  l = *base;  if (*base > n + degree - k) l = n + degree - k;
       for (i = *base - k + 1 ; i <= l ; i++)
         {  if (fabs(basis[i]) > PAR_TOL)
              d = (t - knot[i]) * basis[i] / (knot[i + k - 1] - knot[i]);
            else
              d = 0.0;
            if (fabs(basis[i+1]) > PAR_TOL)
              e = (knot[i+k] - t) * basis[i+1] / (knot[i+k] - knot[i+1]);
            else
              e = 0.0;
            basis[i] = d + e;
         }
    }
  if (fabs(t - knot[npluso - 1]) < PAR_TOL) basis[n-1] = 1.0;
  if (cweight != NULL)
    {  sum = 0.0;
       l = *base;  if (l > n - 1) l = n - 1;
       for (i = *base - degree ; i <= l ; i++)
         sum = sum + basis[i] * cweight[i];
       for (i = *base - degree ; i <= l ; i++)
         if (fabs(sum) < PAR_TOL)
           basis[i] = 0.0;
         else
           basis[i] = basis[i] * cweight[i] / sum;
    }
}

Point Curve::position(double t) const
{double *basis;
 int i,l,base;
 Point p(0.0,0.0,0.0);
  if (n == 2 && degree == 1)
    return polygon[0] + (polygon[1] - polygon[0]) * t;
  if (tbasis != NULL)
    basis = tbasis;
  else
    {  if ((basis = new double[n + degree]) == NULL) return p;
    }
  if (knot != NULL)
    rbasis(t,knot,basis,&base);
  else if (tknot != NULL)
    rbasis(t,tknot,basis,&base);
  else
    return p;
  l = base;  if (base > n - 1) l = n - 1;
  for (i = base - degree ; i <= l ; i++)
    p += polygon[i] * basis[i];

  if (tbasis == NULL) delete [] basis;

  return p;
}

Point Curve::positionbylength(double l,double *t) const
{const int nsegs = 1000;
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

Point Curve::direction(double t) const
{Point pos,dir(0.0,0.0,0.0);
 double *nknot,*tbasis;
  nknot = NULL;  tbasis = NULL;
  posdir(t,&nknot,&tbasis,&pos,&dir);
  delete [] nknot;  delete [] tbasis;
  return dir;
}

Point Curve::start(void)
{ return polygon[0];
}

Point Curve::end(void)
{ return polygon[n-1];
}

int Curve::npoints(void)
{ return n;
}

Point Curve::point(int i)
{ return polygon[i];
}

Point Curve::nearp(Point p)
{double t;
  return nearp(0,p,&t);
}


Point ExtendedCurvePosition(Curve *curve,Point pdir0,Point pdir1,double t)
{Point p;
  if (t < 0.0)
    p = curve->getpolygon()[0] - pdir0 * t;
  else if (t > 1.0)
    p = curve->getpolygon()[curve->getn()-1] + pdir1 * (t-1.0);
  else 
    p = curve->position(t);
  return p;
}
 

Point Curve::nearp(int normalonly,Point p,double *tmin)
{int nsegs,i,j;
 double t,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8,dmin,*nknot,*tbasis;
 Point p1,p2,pmin,pos,dir,pdir0,pdir1;

  newbasis();
  nknot = NULL;  tbasis = NULL;
  nsegs = 60;
  dt = 1.0 / nsegs;
  dmin = -1.0;

  p1 = position(dt);
  p2 = p1 - p;  
  d2 = p2.dot(p2);

  //  Use pdir0 and pdir1 to calculate the position beyond the start of the curve.
  pdir0 = (polygon[0] - polygon[1]).normalize();
  pdir0 *= (position(dt) - polygon[0]).length() / dt;

  pdir1 = (polygon[n-1] - polygon[n-2]).normalize();
  pdir1 *= (position(1.0-dt) - position(1.0)).length() / dt;

  for (i = 0,t = -dt ; i < nsegs + 3 ; i++,t += dt)
    {  p1 = ExtendedCurvePosition(this,pdir0,pdir1,t);
       p2 = p1 - p;  d3 = p2.dot(p2);
       if (normalonly)
         {  if (posdir(t,&nknot,&tbasis,&pos,&dir))
              {  p2.normalize();
                 if (fabs(p2.dot(dir)) < db.getatoler())
                  {  if (dmin < 0.0 || d3 < dmin)
                       {  dmin = d3;  pmin = p1;  *tmin = t;
                       }
                  }
              }
         }
       else
         {  if (d3 < dmin)
              {  dmin = d3;  pmin = p1;  *tmin = t;
              }
         }
       if (i > 1)
         if (d1 >= d2 && d2 <= d3)
           {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
              d4 = d1;  d5 = d2;  d6 = d3;
              for (j = 0 ; j < 50 ; j++)
                {  p1 = ExtendedCurvePosition(this,pdir0,pdir1,t1 + (t2 - t1) / 2.0);
                   p2 = p1 - p;  d7 = p2.dot(p2);
                   if (d4 >= d7 && d7 <= d5)
                     {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                        d6 = d5;  d5 = d7;
                     }
                   else
                     {  p1 = ExtendedCurvePosition(this,pdir0,pdir1,t3 - (t3 - t2) / 2.0);
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
              p1 = ExtendedCurvePosition(this,pdir0,pdir1,(t1+t3) / 2.0);
              p2 = p1 - p;
              d7 = p2.dot(p2);
              if (dmin < 0.0 || d7 < dmin)
                {  dmin = d7;  pmin = p1;  *tmin = (t1 + t3) / 2.0;
                }
           }
       d1 = d2;  d2 = d3;
    }
  deletebasis();
  delete [] nknot;  delete [] tbasis;
  if (dmin < 0.0) 
    {  //  Just choose the end point closest to the selection point
       if ((position(0.0) - p).length() < (position(1.0) - p).length())
         {  *tmin = 0.0;
            pmin = polygon[0];
         }
       else
         {  *tmin = 1.0;
            pmin = polygon[n-1];
         }
    }
  else if (*tmin < 0.0)
    {  *tmin = 0.0;
       pmin = polygon[0];
    } 
  else if (*tmin > 1.0)
    {  *tmin = 1.0;
       pmin = polygon[n-1];
    }

  return pmin;
}

int Curve::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int match1,match2,match3,match4;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  match1 = sp->match(polygon[0]);
  match2 = sp->match(polygon[1]);
  match3 = n-2 > 1 ? sp->match(polygon[n-2]) : 0;
  match4 = n-1 > 2 ? sp->match(polygon[n-1]) : 0;

  if (! match1 && ! match2 && ! match3 && ! match4) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0)
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus((status | 4) & ~8);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db == 0 ? 1 : db->geometry.add(c);
           }
      }
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  if (match1)
    polygon[0] = t.transform(polygon[0]);
  if (match2)
    polygon[1] = t.transform(polygon[1]);
  if (match3)
    polygon[n-2] = t.transform(polygon[n-2]);
  if (match4)
    polygon[n-1] = t.transform(polygon[n-1]);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

void Curve::transform(Transform &t,int drawentity)
{int i;
  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  for (i = 0 ; i < n ; i++)
    polygon[i] = t.transform(polygon[i]);
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}


int Curve::move(Database *db,Transform &t,int erase)
{Entity *e;
 int i;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ? 1 : db->geometry.add(clone(t));
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 1;
  for (i = 0 ; i < n ; i++)
    polygon[i] = t.transform(polygon[i]);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *Curve::clone(Transform &trans)
{Point *poly;
 int i;
  if (copye != NULL) return copye;
  if ((poly = new Point[n]) == NULL) return NULL;
  for (i = 0 ; i < n ; i++)
    poly[i] = trans.transform(polygon[i]);
  copye = new Curve(*this,n,degree,poly,knot,cweight,options);
  delete [] poly;
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int Curve::project(Database *db,Transform &t,int surface)
{BitMask options(32);
 int xmesh,ymesh;
  if ((status & 2) == 2) return 1;
  if (! db->geometry.add(new Line(*this,polygon[0],t.transform(polygon[0])))) return 0;
  if (! db->geometry.add(new Line(*this,polygon[n-1],t.transform(polygon[n-1])))) return 0;
  if (copye == NULL)
    {  if (! db->geometry.add(clone(t))) return 0;
    }
  if (surface)
    {  RuledSurface::getdefaults(&xmesh,&ymesh,&options);
       RuledSurface *surface = new RuledSurface(*this,(LinearEntity *)this,(LinearEntity *)copye,xmesh,ymesh,options);
       if (surface != NULL && surface->getdefined())
         {  db->geometry.add(surface);
            status |= 2;
         }
       else
         delete surface;
    }
  return 1;
}

void Curve::change(const BitMask &change,const EntityHeader &eh,int d,const BitMask &o)
{int i,olddegree,npluso;
 double *tknot1;
  db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4))
    {  db.saveundo(UD_MOVED,this);
       if (d + 1 > n) d = n - 1;
       olddegree = degree;
       degree = short(d);
       if (knot == NULL)
         {  tempknots(&tknot1);
            if (tknot1 != NULL)
              {  delete [] tknot;  tknot = tknot1;
              }
            else
              degree = short(olddegree);
         }
       else
         {  if (olddegree <= degree)
              npluso = n + degree + 1;
            else
              npluso = n + olddegree + 1;
            if ((tknot1 = new double [npluso]) != NULL)
              {  for (i = 0 ; i < degree + 1 ; i++)
                   tknot1[i] = 0.0;
                 for (i = degree + 1 ; i < n ; i++)
                   tknot1[i] = knot[i - degree + olddegree];
                 for (i = n ; i < n + degree + 1 ; i++)
                   tknot1[i] = 1.0;
                 delete [] knot;  knot = tknot1;
              }
            else
              degree = short(olddegree);
         }
       if (getposition() != 0 && db.MovedCallback != 0)
         db.MovedCallback(this);
    }
  if (change.test(5)) options = o;
  draw(DM_NORMAL);
}

int Curve::beginoffset(Point p,Point *zaxis,double *d,LinearEntity *enext,int *reverse,Point *startp)
{double d1,d2,d3,d4,t;
 Point p3,np,dir;
    p3 = nearp(0,p,&t);
    dir = direction(t);
    if (fabs(*d) < db.getptoler()) 
    {   InfinitePlane ip(*zaxis,p3);
        *d = (p3 - ip.nearp(p)).length();
    }

    if (dir.cross(*zaxis).dot(p - p3) > 0.0) 
        *zaxis = -*zaxis;
    if (enext == NULL)
        *reverse = 0;
    else
    {   d1 = (position(0.0) - enext->start()).length();
        d2 = (position(0.0) - enext->end()).length();
        if (d2 < d1) d1 = d2;
        d3 = (position(1.0) - enext->start()).length();
        d4 = (position(1.0) - enext->end()).length();
        if (d4 < d3) d3 = d4;
        if (fabs(d3) < db.getptoler() && fabs(d1) < db.getptoler() || d3 < d1)
            *reverse = 0;
        else
        {   *zaxis = -*zaxis;
            *reverse = 1;
        }
    }

    return offsetpoint(*zaxis,*d,*reverse,1,startp);
}


int Curve::offset(Point zaxis,double d,int,LinearEntity *enext,int *reverse,Point *startp,EntityList *olist)
{Point *fitpoint;
 int i,nseg;
 double t,dt;

  newbasis();
  nseg = n * 20;
  fitpoint = new Point[nseg+1];
  dt = 1.0 / nseg;
  for (i = 0,t = 0.0 ; i <= nseg ; i++,t += dt)
     if (*reverse)
       fitpoint[i] = position(t) + direction(t).cross(zaxis) * d;
     else
       fitpoint[i] = position(t) - direction(t).cross(zaxis) * d;
  deletebasis();

  olist->add(new Curve(n*4,3,nseg+1,fitpoint,options));

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

int Curve::offsetpoint(Point zaxis,double d,int reverse,int end,Point *p)
{Point yaxis;

    if (reverse)
        yaxis = zaxis.cross(-direction(2.0 - end)).normalize();
    else
        yaxis = zaxis.cross(direction(end - 1.0)).normalize();
    if (yaxis.length() < db.getptoler()) 
        return 0;
    yaxis *= d;
    *p = position(2.0 - end) + yaxis;

    return 1;
}

int Curve::save(CadFile *outfile)
{short type,x;
 int i;
 Byte terminators = 0;
  type = curve_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! outfile->write(&n)) return 0;
  if (! outfile->write(&degree)) return 0;
  if (! outfile->write(&terminators)) return 0;
  for (i = 0 ; i < n ; i++)
    if (! polygon[i].save(outfile)) return 0;
  if (knot == NULL)
    x = 0;
  else
    x = short(n + degree + 1);
  if (! outfile->write(&x)) return 0;
  for (i = 0 ; i < x ; i++)
    if (! outfile->write(&knot[i])) return 0;
  if (cweight == NULL)
    x = 0;
  else
    x = n;
  if (! outfile->write(&x)) return 0;
  for (i = 0 ; i < x ; i++)
    if (! outfile->write(&cweight[i])) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

static int ncurves = 0;

Entity *Curve::load(int,CadFile *infile)
{Curve *curve;
 EntityHeader *header;
 Point *polygon;
 double *cweight,*knot;
 short n,degree,x;
 BitMask options(32);
 int i;
 Byte terminators;

  if ((header = loadheader(infile)) == NULL) return NULL;
  if (! infile->read(&n)) return NULL;
  if (! infile->read(&degree)) return NULL;
  if (! infile->read(&terminators)) return 0;
  if ((polygon = new Point[n]) == NULL) return NULL;
  for (i = 0 ; i < n ; i++)
    if (! polygon[i].load(infile)) return NULL; 
  if (! infile->read(&x)) return NULL;
  if (x == 0)
    knot = NULL;
  else
    {  if ((knot = new double[x]) == NULL)
         {  delete [] polygon;  return NULL;
         }
    }
  for (i = 0 ; i < x ; i++)
    if (! infile->read(&knot[i])) return NULL;
  if (! infile->read(&x)) return NULL;
  if (x == 0)
    cweight = NULL;
  else
    {  if ((cweight = new double[x]) == NULL)
         {  delete [] polygon;  delete [] knot;  return NULL;
         }
    }
  for (i = 0 ; i < x ; i++)
    if (! infile->read(&cweight[i])) return NULL;
  if (! infile->read(&options)) return 0;
  curve = new Curve(*header,n,degree,polygon,knot,cweight,options);
  curve->atlist = header->getatlist();
  delete header;
  delete [] polygon;
  delete [] knot;
  delete [] cweight;
  return curve;
}

int Curve::save(UndoFile *outfile)
{short type;
  type = curve_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->blockwrite(polygon,sizeof(Point) * n)) return 0;
  if (knot != NULL)
    {  if (! outfile->blockwrite(knot,sizeof(double) * (n + degree + 1))) return 0;
    }
  if (cweight != NULL)
    {  if (! outfile->blockwrite(cweight,sizeof(double) * n)) return 0;
    }
  return 1;
}

Entity *Curve::load(UndoFile *infile)
{Curve *curve;
 char bm[sizeof(BitMask)];
  if ((curve = new Curve) != NULL)
    {AttributeList al;
       memcpy(bm,&curve->visible,sizeof(bm));
       if (! infile->blockread(curve,sizeof(*curve))) return NULL;
       memcpy(&curve->visible,bm,sizeof(bm));
       if (! infile->read(&curve->visible)) return 0;
       if (! al.load(infile)) return 0;   curve->atlist = al;
       curve->tknot = curve->tbasis = NULL;
       if ((curve->polygon = new Point[curve->n]) == NULL) return NULL;
       if (! infile->blockread(curve->polygon,sizeof(Point) * curve->n))
         {  delete curve;  return 0;
         }
       if (curve->knot != NULL)
         {  if ((curve->knot = new double[curve->n + curve->degree + 1]) == NULL)
              {  delete [] curve->polygon;  delete curve;  return 0;
              }
            if (! infile->blockread(curve->knot,sizeof(double) * (curve->n + curve->degree + 1))) return 0;
         }
       else
         {  curve->tempknots(&curve->tknot);
            if (curve->tknot == NULL)
              {  delete [] curve->polygon;  return NULL;
              }
         }
       if (curve->cweight != NULL)
         {  if ((curve->cweight = new double[curve->n]) == NULL)
              {  delete [] curve->knot;  delete [] curve->polygon;  delete curve;  return 0;
              }
            if (! infile->blockread(curve->cweight,sizeof(double) * curve->n)) return 0;
         }
    }
  return curve;
}

int Curve::saveattributes(CadFile *outfile)
{ if (! outfile->write(&degree)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

int Curve::loadattributes(CadFile *infile)
{ if (! infile->read(&degree)) return 0;
  if (! infile->read(&options)) return 0;
  return 1;
}

int Curve::savegeometry(CadFile *outfile)
{BitMask arrays(32);
  if (! outfile->write(&n)) return NULL;
  if (! outfile->write(&degree)) return NULL;
  arrays.set(0,knot != NULL);  arrays.set(1,cweight != NULL);
  if (! outfile->write(&arrays)) return NULL;
  if (! outfile->blockwrite(polygon,sizeof(Point) * n)) return 0;
  if (knot != NULL)
    {  if (! outfile->blockwrite(knot,sizeof(double) * (n + degree + 1))) return 0;
    }
  if (cweight != NULL)
    {  if (! outfile->blockwrite(cweight,sizeof(double) * n)) return 0;
    }
  return 1;
}

int Curve::savedxf(int blocksection,DXFOut *dxffile)
{double t,dt;
 int i,nseg;
 Point p;
  if (! blocksection)
    {  if (v.getreal("dx::curvelength") <= 0.0)
         {EntityList list;
          Entity *e;
            list = convert(0);
            for (list.start() ; (e = list.next()) != 0 ; )
              {  e->savedxf(blocksection,dxffile);
                 delete e;
              }
            list.destroy();  
         }
       else if (degree <= 1)
         {  if (! dxffile->writestring(0, (char*)"POLYLINE")) return 0;
            if (! dxffile->writeheader(this)) return 0;
            if (! dxffile->writestring(100, (char*)"AcDb3dPolyline")) return 0;
 
            if (! dxffile->writelong(66,1)) return 0;
            if (! dxffile->writereal(10,0.0)) return 0;
            if (! dxffile->writereal(20,0.0)) return 0;
            if (! dxffile->writereal(30,0.0)) return 0;
            if (! dxffile->writelong(70,8)) return 0;

            nseg = n-1;

            dt = 1.0 / double(nseg);
            newbasis();
            for (i = 0,t = 0.0 ; i <= nseg ; i++,t += dt)
              {  p = position(t);
                 if (! dxffile->writestring(0, (char*)"VERTEX")) return 0;
                 if (! dxffile->writehandle()) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
                 if (! dxffile->writelong(8,getlayer())) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbVertex")) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDb3dPolylineVertex")) return 0;
                 if (! dxffile->writereal(10,p.x * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(20,p.y * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(30,p.z * dxffile->getscale())) return 0;
              }
            deletebasis();
            if (! dxffile->writestring(0, (char*)"SEQEND")) return 0;
            if (! dxffile->writehandle()) return 0;
            if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
         } 
       else
         {  if (! dxffile->writestring(0, (char*)"SPLINE")) return 0;
            if (! dxffile->writeheader(this)) return 0;
            if (! dxffile->writestring(100, (char*)"AcDbSpline")) return 0;
 
            if (! dxffile->writelong(70,4)) return 0;
            if (! dxffile->writelong(71,degree)) return 0;
            if (! dxffile->writelong(72,n + degree + 1)) return 0;
            if (! dxffile->writelong(73,n)) return 0;
            if (! dxffile->writelong(74,0)) return 0;

            for (i = 0 ; i < n + degree + 1 ; i++)
              if (! dxffile->writereal(40,knot != 0 ? knot[i] : tknot[i])) return 0;

            for (i = 0 ; i < n ; i++)
              if (! dxffile->writereal(41,cweight != 0 ? cweight[i] : 1.0)) return 0;


            for (i = 0 ; i < n ; i++)
              {  if (! dxffile->writereal(10,polygon[i].x)) return 0;
                 if (! dxffile->writereal(20,polygon[i].y)) return 0;
                 if (! dxffile->writereal(30,polygon[i].z)) return 0;
              }

         } 
    }
  return 1;
}

int Curve::exportiges(FILE *outfile,int section,int,int *ndirectory,int *nparameter)
{int i;
  if (section == 0)
    {  if (fprintf(outfile,"     126%8d               0       0       0       0       0 0 0 0 0D%7d\n",(*nparameter)+1,++(*ndirectory)) < 0) return 0;
       if (fprintf(outfile,"     126       0%8d       2       0                               0D%7d\n",colour,++(*ndirectory)) < 0) return 0;
       (*nparameter) += 1 + n + degree + 1 + n + n + 1;
    }
  else 
    {  if (fprintf(outfile,"126,%3d,%2d,0,0,%d,0,                                              %7dP%7d\n",n-1,degree,cweight == 0,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       for (i = 0 ; i < n + degree + 1 ; i++)   
         if (knot != 0)
           {  if (fprintf(outfile,"%18.10e,                                              %7dP%7d\n",knot[i],*ndirectory + 1,++(*nparameter)) < 0) return 0;
           }
         else
           {  if (fprintf(outfile,"%18.10e,                                              %7dP%7d\n",tknot[i],*ndirectory + 1,++(*nparameter)) < 0) return 0;
           }

       for (i = 0 ; i < n ; i++)   
         if (fprintf(outfile,"%18.10e,                                              %7dP%7d\n",cweight != 0 ? cweight[i] : 1.0,*ndirectory + 1,++(*nparameter)) < 0) return 0;

       for (i = 0 ; i < n ; i++)   
         if (fprintf(outfile,"%18.10e,%18.10e,%18.10e,        %7dP%7d\n",polygon[i].x,polygon[i].y,polygon[i].z,*ndirectory + 1,++(*nparameter)) < 0) return 0;

       if (fprintf(outfile,"0.0,1.0;                                                         %7dP%7d\n",*ndirectory + 1,++(*nparameter)) < 0) return 0;

       (*ndirectory) += 2;

    }
  return 1;
}

int Curve::loadgeometry(CadFile *infile)
{BitMask arrays(32);
  if (cadwindow != 0)
    cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&n)) return NULL;
  if (! infile->read(&degree)) return NULL;
  if (! infile->read(&arrays)) return NULL;
  if (polygon == NULL) polygon = new Point[n];
  delete [] knot;  knot = NULL;
  if (arrays.test(0)) knot = new double[n + degree + 1];
  if (arrays.test(1) && cweight == NULL) cweight = new double[n];
  if(infile->getversion() == 2)
  {
      //if (! infile->blockread(polygon,(sizeof(Point)-sizeof(QGraphicsItem*)) * n))
      if (! infile->blockread(polygon,sizeof(Point) * n))
          return 0;
  }
  else
  {
      if (! infile->blockread(polygon,sizeof(Point) * n))
          return 0;
  }
  if (knot != NULL)
    {  if (! infile->blockread(knot,sizeof(double) * (n + degree + 1))) return 0;
    }
  else
    {  tempknots(&tknot);
       if (tknot == NULL)
         {  delete [] knot;  delete [] polygon;  delete [] cweight;
            defined = 0;
            return 0;
         }
    }
  if (cweight != NULL)
    {  if (! infile->blockread(cweight,sizeof(double) * n)) return 0;
    }
  defined = 1;
  return 1;
}


class CurveVerifyButtonControl : public ButtonDialogControl
  {public:
     CurveVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int CurveVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,layer,style;
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
 * @brief Curve::verify
 *
 * Entry point for the curve very command
 *
 *
 */
void Curve::verify(void)
{
 Dialog dialog("CurveVerify_Dialog");
 int c,l,s,i,n1;
 double w;
 Point cen,pmin,pmax;
 RCCHAR start[300],end[300],centre[300],smin[300],smax[300];

  cen.setxyz(0.0,0.0,0.0);
  n1 = 0;
  for (i = 0 ; i < n ; i++)
    {  if (i < n - 1 || (polygon[0] - polygon[n-1]).length() > db.getptoler())
        {  cen += polygon[i];
           n1++;
        }

       if (i == 0)
         pmin = pmax = polygon[i];
       else
         {  if (polygon[i].x < pmin.x) pmin.x = polygon[i].x;
            if (polygon[i].y < pmin.y) pmin.y = polygon[i].y;
            if (polygon[i].z < pmin.z) pmin.z = polygon[i].z;
            if (polygon[i].x > pmax.x) pmax.x = polygon[i].x;
            if (polygon[i].y > pmax.y) pmax.y = polygon[i].y;
            if (polygon[i].z > pmax.z) pmax.z = polygon[i].z;
         }
    }

  if (n1 > 0)
    cen /= n1;
 
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new CurveVerifyButtonControl(1000));
  dialog.add(new CurveVerifyButtonControl(1001));
  dialog.add(new CurveVerifyButtonControl(1002));
  dialog.add(new CurveVerifyButtonControl(1003));

  dialog.add(new IntegerDialogControl(104,&degree));
  dialog.add(new IntegerDialogControl(105,&n));
  dialog.add(new StringDialogControl(106,polygon[0].string(start),300));
  dialog.add(new StringDialogControl(107,polygon[n-1].string(end),300));
  dialog.add(new StringDialogControl(108,cen.string(centre),300));
  dialog.add(new StringDialogControl(109,pmin.string(smin),300));
  dialog.add(new StringDialogControl(110,pmax.string(smax),300));

  dialog.setposition(DPOK);
  if(dialog.process() == TRUE)
  {
      setcolour(c);
      setstyle(s);
      setlayer(l);
      setweight(db.lineweights.mmtoweight(w));
  }
}

void Curve::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{Point p3;
 int i,n;
 double t,dt;
  if (vs != NULL)
    n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  else
    n = 60;
  dt = 1.0 / double(n);
  newbasis();
  for (i = 0,t = 0.0 ; i <= n ; i++,t += dt)
    {  p3 = position(t);
       if (vs != NULL) p3 = vs->modeltouv(p3);
       if (i == 0)
         *pmin = *pmax = p3;
       else
         {  if (p3.x < pmin->x) pmin->x = p3.x;
            if (p3.y < pmin->y) pmin->y = p3.y;
            if (p3.z < pmin->z) pmin->z = p3.z;
            if (p3.x > pmax->x) pmax->x = p3.x;
            if (p3.y > pmax->y) pmax->y = p3.y;
            if (p3.z > pmax->z) pmax->z = p3.z;
         }
    }
  deletebasis();
}

int Curve::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(curve_entity)) return 0;
  switch (geometry)
    {  case Geometry :
#if 1
      {
      if((Entity*)this == &entity) return 1;
      Curve *curve = (Curve*)&entity;
      if(degree != curve->getdegree()) return 0;
      if(!(polygon[0] == curve->start())) return 0;
      if(!(polygon[n-1] == curve->end())) return 0;
      if(n != curve->npoints()) return 0;
      for(int i=0; i<n; i++)
      {
          if(!(polygon[i] == curve->point(i)))
              return 0;
      }
      for(int i=n-1; i>=0; i--)
      {
          if(!(polygon[i] == curve->point(i)))
              return 0;
      }
      return 1;
      }
#else
         return 0;
#endif
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int Curve::explode(void)
{EntityList list;
 Entity *e;
  list = convert(0);
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e != 0 && e->getdefined())
      db.geometry.add(e);
  return 1;
}


EntityList Curve::convert(int nlines)
{Point pt1,pt2;
 int nseg;
 double t1,t2,t,dt,explodetolerance;
 EntityList list;

  newbasis();
  explodetolerance = v.getreal("db::explodetoler");
  t1 = 0.0;  t2 = 1.0;
  while (t1 < 1.0)
    {  if (nlines > 1)
         t2 = t1 + 1.0 / nlines;
       Circle circle(1,position(t1),position((t1 + t2) / 2.0),position(t2));
       Line line(position(t1),position(t2));
       LinearEntity *le;
        if (nlines > 1 || circle.getradius() == 0.0 || fabs(circle.getradius() * (cos(circle.getsweepa()/2.0) - 1.0)) < explodetolerance)
          le = & line;
        else
          le = & circle;
        nseg = 20;
        dt = (t2 - t1) / double(nseg);
        for (t = t1 ; nlines <= 1 && t <= t2 ; t += dt)
          {  if ((le->nearp(position(t)) - position(t)).length() > explodetolerance)
               {  t2 = (t1 + t2) / 2.0;
                  break;
               }
          }
       if (nlines <= 1 && t <= t2)
         continue;
       if (le == &line)
         {Line *line = new Line(position(t1),position(t2));
            if (line != 0 && line->getdefined())
              list.add(line);
         }
       else
         {Circle *circle = new Circle(1,position(t1),position((t1 + t2) / 2.0),position(t2));
            if (circle != 0 && circle->getdefined())
              list.add(circle);
         }
       t1 = t2;  t2 = 1.0;
    }

  deletebasis();
  return list;
}

//#ifdef _USING_QTCHAR
//int Curve::process_property(int action,int id,void *data,RCCHAR *text)
//#else
int Curve::process_property(int action,int id,void *data,char *text)
//#endif
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
            sprintf((char*)text,"%d",getcolour());
        else if (id == 2)
            sprintf((char*)text,"%d",getlayer());
        else if (id == 3)
            sprintf((char*)text,"%d",getstyle());
        else if (id == 4)
            sprintf((char*)text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (id == 6)
            sprintf((char*)text,"%d",property_point_index);
        else if (id == 7)
            db.formatnumber((RCCHAR*)text,polygon[property_point_index].x,0);
        else if (id == 8)
            db.formatnumber((RCCHAR*)text,polygon[property_point_index].y,0);
        else if (id == 9)
            db.formatnumber((RCCHAR*)text,polygon[property_point_index].z,0);
        else if (id == 10)
            sprintf((char*)text,"%d",degree);
        else if (id == 11)
        {   sprintf((char*)text,"%d",n);
            return 1;
        }
        else if (id == 12)
        {   db.formatnumber((RCCHAR*)text,length(),0);
            return 1; 
        }
    }
    else if (action == PP_SETVALUE)
    {   if (id == 6)
        {   property_point_index = atoi((char*)text);
            if (property_point_index < 0)
                property_point_index = 0;
            if (property_point_index > n-1)
                property_point_index = n-1;
        }
        else if (id == 7)
        {   movepoint(1,property_point_index,Point(atof((char*)text),polygon[property_point_index].y,polygon[property_point_index].z));
        }
        else if (id == 8)
        {   movepoint(1,property_point_index,Point(polygon[property_point_index].x,atof((char*)text),polygon[property_point_index].z));
        }
        else if (id == 9)
        {   movepoint(1,property_point_index,Point(polygon[property_point_index].x,polygon[property_point_index].y,atof((char*)text)));
        }
        else if (id == 10)
        {   change_mask.set(4);
            change(change_mask,*this,atoi((char*)text),new_options);
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Curve");
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
                strcpy(text,"Point Index");
            else if (id == 7)
                strcpy(text,"Point X");
            else if (id == 8) 
                strcpy(text,"Point Y");
            else if (id == 9)
                strcpy(text,"Point Z");
            else if (id == 10)
                strcpy(text,"Degree");
            else if (id == 11)
                strcpy(text,"Point Count");
            else if (id == 12)
                strcpy(text,"Length");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 13; 
    
    return 0;
}

