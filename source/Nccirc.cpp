
#include "ncwin.h"
#include "RCGraphicsItems.h"
#include "RCView.h"

Circle::Circle(int arc,Point p1,Point p2,Point p3)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    Point normal,p4,p5,d1,d2;
  d1 = p1 - p2;  d2 = p3 - p2;
  normal = d1.cross(d2);
  p4 = (p1 + p2) / 2.0;
  p5 = (p2 + p3) / 2.0;
  d1 = d1.cross(normal);
  d2 = d2.cross(normal);
  Line *line1 = new Line(p4,p4+d1);
  Line *line2 = new Line(p5,p5+d2);
  Intersect i(line1,line2);
  if (i.nintersections() == 1)
    {  origin = ((PointE *)i.intersection(0))->getp();
       xaxis = (p1 - origin);  xaxis.normalize();
       yaxis = xaxis.cross(normal);  yaxis.normalize();
       radius = (p1 - origin).length();
       starta = 0.0;
       if (arc) sweepa = angle(p3); else sweepa = M_PI * 2.0;
    }
  else
    {  origin.setxyz(0.0,0.0,0.0);
       xaxis.setxyz(1.0,0.0,0.0);
       yaxis.setxyz(0.0,1.0,0.0);
       radius = starta = sweepa = 0.0;
    }
}

Circle::Circle(Point o,Point c)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    origin = o;
   radius = (c - o).length();
   starta = 0.0;
   sweepa = 2.0 * M_PI;
   xaxis.x = 1.0;  xaxis.y = 0.0;  xaxis.z = 0.0;
   yaxis.x = 0.0;  yaxis.y = 1.0;  yaxis.z = 0.0;
}

Circle::Circle(Point o,double r)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

   origin = o;
   radius = r;
   starta = 0.0;
   sweepa = 2.0 * M_PI;
   xaxis.x = 1.0;  xaxis.y = 0.0;  xaxis.z = 0.0;
   yaxis.x = 0.0;  yaxis.y = 1.0;  yaxis.z = 0.0;
}

Circle::Circle(Point o,Point xa,Point ya,double r,double sta,double swa)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    origin = o;
   xaxis = xa;
   yaxis = ya;
   radius = r;
   starta = sta;
   sweepa = swa;
}

Circle::Circle(const EntityHeader &header,Point o,Point xa,Point ya,double r,double sta,double swa) : LinearEntity(header)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    origin = o;
   xaxis = xa;
   yaxis = ya;
   radius = r;
   starta = sta;
   sweepa = swa;
}

Circle::Circle(int arc,double r,Entity *e1,Point p1,Entity *e2,Point p2)
{
#ifdef USING_WIDGETS
  qgi = 0;
  qgi1=0;
  qgi2=0;
  usedByPlane=false;
#endif

   if (e1->isa(line_entity) && e2->isa(line_entity))
    {Line *l1 = (Line *) e1;
     Line *l2 = (Line *) e2;
     Point pint,p3,p4,dir1,dir2;
     double cosa,sina,tana,a;
       Intersect i(l1,l2);
       if (i.nintersections() == 1)
         {  pint = ((PointE *)i.intersection(0))->getp();
            dir1 = l1->direction(0.0);
            if ((p1 - pint).dot(dir1) < 0.0) dir1 = dir1 * -1.0;
            dir2 = l2->direction(0.0);
            if ((p2 - pint).dot(dir2) < 0.0) dir2 = dir2 * -1.0;
            if (dir1.length() > db.getptoler() && dir2.length() > db.getptoler())
              {  dir1.normalize();  dir2.normalize();
                 cosa = dir1.dot(dir2);
                 if (cosa < -1.0) cosa = -1.0;
                 if (cosa > 1.0) cosa = 1.0;
                 a = acos(cosa) / 2.0;
                 tana = tan(a);  sina = sin(a);
                 if (fabs(tana) > db.getatoler() && fabs(sina) > db.getatoler())
                   {  xaxis = dir1;
                      yaxis = ((dir1.cross(dir2)).cross(xaxis)).normalize();
                      origin = pint + ((dir1+dir2).normalize()) * (r / sina);
                      p3 = pint + dir1 * (r / tana);
                      p4 = pint + dir2 * (r / tana);
                      radius = r;
                      filletangles(arc,p3,p4);
                   }
                 else
                   defined = 0; 
              }
            else
              defined = 0;
         }
       else
         defined = 0;
    }                   
  else if (e1->isa(line_entity) && e2->isa(circle_entity) ||
           e1->isa(circle_entity) && e2->isa(line_entity))
    {Line *l;
     Circle *c;
     int i,found;
     double len,d,d1,d2;
     Point p3,p4,p5,p6,p7,p8,axis;
       if (e1->isa(line_entity))
         {  l = (Line *) e1;  c = (Circle *) e2;
         }
       else
         {  l = (Line *) e2;  c = (Circle *) e1;
            p3 = p1;  p1 = p2;  p2 = p3;
         }
       p3 = l->nearp(c->origin);
       d = (p3 - c->origin).length();
       xaxis = l->direction(0.0);
       yaxis = (c->xaxis.cross(c->yaxis)).cross(xaxis);
       if (yaxis.dot(p3 - c->origin) > 0.0) yaxis = yaxis * -1.0;
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  xaxis.normalize();  yaxis.normalize();
            for (i = 0,found = 0 ; i < 8 ; i++)
              {  if (i % 2)       d1 = c->radius + r;  else d1 = c->radius - r;
                 if ((i / 2) % 2) d2 = d + r; else d2 = d - r;
                 len = d1 * d1 - d2 * d2;
                 if (len > 0.0)
                   {  len = sqrt(len);  if (i / 4) len = -len;
                      p4 = p3 + xaxis * len;
                      if ((i / 2) % 2) p5 = p4 - yaxis * r;  else p5 = p4 + yaxis * r;
                      p6 = c->origin + (p5 - c->origin).normalize() * c->radius;
                      if (! found || (p4-p1).length() + (p6-p2).length() < (p7-p1).length() + (p8-p2).length())
                        {  found = 1;
                           origin = p5;  p7 = p4;  p8 = p6;
                        }
                   }
              }
            radius = r;
            if (found)
              filletangles(arc,p7,p8);
            else
              defined = 0;
         }
       else
         defined = 0;
    }
  else if (e1->isa(circle_entity) && e2->isa(circle_entity))
    {Circle *c1 = (Circle *) e1;
     Circle *c2 = (Circle *) e2;
     double d,d1,d2,a1,a2,cosa;
     Point p3,p4,p5,p6,p7;
     int i,found;
       d = (c2->origin - c1->origin).length();

       for (i = 0,found = 0 ; i < 8 ; i++)
         {  if (i % 2)   d1 = c1->radius - r;  else d1 = c1->radius + r;
            if ((i/2)%2) d2 = c2->radius - r;  else d2 = c2->radius + r;
            if (fabs(d1) < db.getptoler() || fabs(d2) < db.getptoler() || d < db.getptoler()) continue;
            cosa = (d*d + d1*d1 - d2*d2) / 2.0 / d / d1;
            if (cosa < -1.0 || cosa > 1.0) continue;
            a1 = c1->angle(c2->origin);
            if (i/4) a2 = a1 + acos(cosa); else a2 = a1 - acos(cosa);
            p3 = c1->origin + c1->xaxis * (cos(a2) * d1) + c1->yaxis * (sin(a2) * d1);
            p4 = p3 + (c1->origin - p3).normalize() * r;
            p5 = p3 + (c2->origin - p3).normalize() * r;
            if (! found || (p4-p1).length() + (p5-p2).length() < (p6-p1).length() + (p7-p2).length())
              {  found = 1;
                 origin = p3;  p6 = p4;  p7 = p5;
              }
            xaxis = c1->xaxis;  yaxis = c1->yaxis;
            radius = r;

         }
       if (found)
         filletangles(arc,p6,p7);
       else
         defined = 0;
    }
  else if (e1->isa(curve_entity))
    circle(arc,r,(Curve *)e1,p1,e2,p2);
  else if (e2->isa(curve_entity))
    circle(arc,r,(Curve *)e2,p2,e1,p1);
  else
    defined = 0;
}

Circle::Circle(int arc,double r,Point p1,Point p2,Point p3)
{
#ifdef USING_WIDGETS
   qgi = 0;
   qgi1=0;
   qgi2=0;
   usedByPlane=false;
#endif

    Point p;
 double l,d;
  d = (p2 - p1).length() / 2.0;
  l = r * r - d * d;
  if (l > 0.0)
    {  l = sqrt(l);
       Line line(p1,p2);
       xaxis = line.direction(0.0);
       yaxis = p3 - line.nearp(p3);
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  xaxis.normalize();  yaxis.normalize();
            origin = (p1 + p2) / 2.0 + yaxis * l;
            radius = r;
            filletangles(arc,p1,p2);
         }
       else
         defined = 0;
    }
  else
    defined = 0;
}

double Circle::angle(Point p)
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

int Circle::nsegments(double error)
{int n;
 double cosa;
  if (radius < db.getptoler() || radius < error * 2.0)
    return 6;
  cosa = (radius - error) / radius;
  if (cosa > 0.99999) cosa = 0.99999;
  n = int(fabs(sweepa) / acos(cosa));
  if (n < 6)
    n = 6;
  return n;
}

void Circle::segment(int n,int include,Point *verts)
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
  x1 = radius * cos(sta);
  y1 = radius * sin(sta);
  if (include & LE_INCLUDESTART)
    verts[j++] = origin + xaxis * x1 + yaxis * y1;
  da = swa / n;
  sina = sin(da);  cosa = cos(da);
  if (! (include & LE_INCLUDEEND)) n--; 
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       verts[j++] = origin + xaxis * x2 + yaxis * y2; 
       x1 = x2;  y1 = y2;
    }
}

void Circle::addtodisplaylist(View3dSurface *vs,int child)
{
#ifdef USING_WIDGETS
#if 1
    int    n,i;
    double x1,y1,x2,y2,da,sina,cosa;
    Point p1,p2,pt1,pt2;
     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     if (! child)
       {  vs->displaylist.setsavemode();
          if (! vs->displaylist.start(this,n * 34)) return;
       }
     x1 = cos(starta) * radius;
     y1 = sin(starta) * radius;
     pt1 = p1 = vs->modeltoview(origin + xaxis * x1 + yaxis * y1);
     da = sweepa / n;
     sina = sin(da);  cosa = cos(da);
     for (i = 0 ; i < n ; i++)
       {  x2 = x1 * cosa - y1 * sina;
          y2 = x1 * sina + y1 * cosa;
          pt2 = p2 = vs->modeltoview(origin + xaxis * x2 + yaxis * y2);
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

     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     if (! child)
     {
          vs->displaylist.setsavemode();
          if (! vs->displaylist.start(this,n * 34)) return;
     }
     x1 = cos(starta) * radius;
     y1 = sin(starta) * radius;
     pt1 = p1 = vs->modeltoview(origin + xaxis * x1 + yaxis * y1);
     da = sweepa / n;
     sina = sin(da);  cosa = cos(da);
     for (i = 0 ; i < n ; i++)
       {  x2 = x1 * cosa - y1 * sina;
          y2 = x1 * sina + y1 * cosa;
          pt2 = p2 = vs->modeltoview(origin + xaxis * x2 + yaxis * y2);
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
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,n * 34)) return;
    }
  x1 = cos(starta) * radius;
  y1 = sin(starta) * radius;
  pt1 = p1 = vs->modeltoview(origin + xaxis * x1 + yaxis * y1);
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       pt2 = p2 = vs->modeltoview(origin + xaxis * x2 + yaxis * y2); 
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
void Circle::draw(int drawmode,Transform *t,View3dSurface *vs, QPainter *painter)
{
    int    n,inside,i;
    double x1,y1,x2,y2,da,sina,cosa;
    Point o,xa,ya,p1,p2,pt1,pt2;

    //qDebug() << "Enterting Circle::draw() : qgi : " << qgi;
    // get rid of the scene version
    draw(DM_ERASE,t,vs);

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
     vs->setup(linestylescale,fabs(radius * sweepa),drawmode,colour,style,weight,patternlinestyle);
     painter->setPen(vs->curPen);

#if 1
   #if 1
     if (t == NULL && patternlinestyle == 0 && ! gethandles()  )
   #else
     if (t == NULL && patternlinestyle == 0 && ! gethandles() && drawmode  == DM_SELECT)
   #endif
     {
          if (vs->displaylist.draw(this,drawmode,painter))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
          if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
          if (vs->displaylist.draw(this,drawmode,painter))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
     }
#endif

     if (t == NULL)
       {  o = origin;  xa = xaxis * radius;  ya = yaxis * radius;
       }
     else
       {  o = t->transform(origin);
          xa = t->transform(origin+xaxis*radius) - o;
          ya = t->transform(origin+yaxis*radius) - o;
       }
     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     if (vs->inside(vs->modeltoview( xa + ya + o)) &&
         vs->inside(vs->modeltoview(-xa + ya + o)) &&
         vs->inside(vs->modeltoview( xa - ya + o)) &&
         vs->inside(vs->modeltoview(-xa - ya + o)))
       inside = 1;
     else
       inside = 0;
     if (inside && patternlinestyle == 0)
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = vs->modeltoscreen(o + xa * x1 + ya * y1);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          vs->moveto(pt1.x,pt1.y);
          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = vs->modeltoscreen(o + xa * x2 + ya * y2);

               vs->lineto(painter,pt2.x,pt2.y);

               x1 = x2;  y1 = y2;
            }
       }
     else
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = p1 = vs->modeltoview(o + xa * x1 + ya * y1);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = p2 = vs->modeltoview(o + xa * x2 + ya * y2);
               if (vs->clip3d(&pt1,&pt2))
                 {  pt1 = vs->viewtoscreen(pt1);
                    pt2 = vs->viewtoscreen(pt2);

                    vs->line(painter,pt1.x,pt1.y,pt2.x,pt2.y);
                 }
               pt1 = p1 = p2;  x1 = x2;  y1 = y2;
            }
       }

     if (patternlinestyle != 0)
       patternlinestyle->end();

     if (gethandles())
       {

          vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
          painter->setPen(vs->curPen);

          pt1 = vs->modeltoscreen(origin);
          pt2 = vs->modeltoscreen(origin+xaxis*radius);


          vs->line(painter,pt1.x-2,pt1.y-2,pt1.x+2,pt1.y-2);
          vs->line(painter,pt1.x+2,pt1.y-2,pt1.x+2,pt1.y+2);
          vs->line(painter,pt1.x+2,pt1.y+2,pt1.x-2,pt1.y+2);
          vs->line(painter,pt1.x-2,pt1.y+2,pt1.x-2,pt1.y-2);

          vs->line(painter,pt2.x-2,pt2.y-2,pt2.x+2,pt2.y-2);
          vs->line(painter,pt2.x+2,pt2.y-2,pt2.x+2,pt2.y+2);
          vs->line(painter,pt2.x+2,pt2.y+2,pt2.x-2,pt2.y+2);
          vs->line(painter,pt2.x-2,pt2.y+2,pt2.x-2,pt2.y-2);
     }
}

void Circle::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
     int    n,inside,i;
     double x1,y1,z1,x2,y2,z2,da,sina,cosa;
     Point o,xa,ya,p1,p2,pt1,pt2;

     //qDebug() << "Enterting Circle::draw() : qgi : " << qgi;
     //qDebug() << "vs sceneRect :" << vs->getScene()->sceneRect();
     //qDebug() << "vs->getumin : " << vs->getumin();
     //qDebug() << "vs->getvmin : " << vs->getvmin();
     //qDebug() << "vs->getumax : " << vs->getumax();
     //qDebug() << "vs->getumax : " << vs->getvmax();
     //qDebug() << "ulength : " << vs->getumax() - vs->getumin();
     //qDebug() << "vlength : " << vs->getvmax() - vs->getvmin();

     // get rid of the scene version
     draw(DM_ERASE,t,vs);

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

#if 0
     if(drawmode == DM_ERASE)
     {
         if(gldlist != 0)
             glDeleteLists(gldlist,1);
         gldlist = 0;
     }
     if(gldlist == 0 || drawmode == DM_SELECT)
     {
        glDeleteLists(gldlist,1);
        gldlist = glGenLists(1);
        qDebug() << "gldlist : " << gldlist;
        //gldlist = (GLuint)((unsigned long)this);
        glNewList(gldlist,GL_COMPILE_AND_EXECUTE);
     }
     else
     {
         glCallList(gldlist);
         return;
     }
#endif

     //if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
     vs->setupGL(linestylescale,fabs(radius * sweepa),drawmode,colour,style,weight,patternlinestyle);
     //painter->setPen(vs->curPen);
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
   #if 1
     if (t == NULL && patternlinestyle == 0 && ! gethandles()  )
   #else
     if (t == NULL && patternlinestyle == 0 && ! gethandles() && drawmode  == DM_SELECT)
   #endif
     {
          if (vs->displaylist.draw(this,drawmode,painter))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
          if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
          if (vs->displaylist.draw(this,drawmode,painter))
          {
              if(qgi)
                  vs->getScene()->addItem(qgi);
              return;
          }
     }
#endif

     o = origin;  xa = xaxis * radius;  ya = yaxis * radius;
     /* kmj : changed for gl dragging in figures
     if (t == NULL)
     {
         o = origin;  xa = xaxis * radius;  ya = yaxis * radius;
     }
     else
     {
          o = t->transform(origin);
          xa = t->transform(origin+xaxis*radius) - o;
          ya = t->transform(origin+yaxis*radius) - o;
     }
     */
     //qDebug() << "vs->getumax : " << vs->getumax();
     //qDebug() << "vs->getumin : " << vs->getumin();
     //qDebug() << "vs->getwidth : " << vs->getwidth();
     //qDebug() << "db.getdtoler : " << db.getdtoler();
     n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
     //qDebug() << "segments : " << n;
     /*
     if (vs->inside(vs->modeltoview( xa + ya + o)) &&
         vs->inside(vs->modeltoview(-xa + ya + o)) &&
         vs->inside(vs->modeltoview( xa - ya + o)) &&
         vs->inside(vs->modeltoview(-xa - ya + o)))
       inside = 1;
     else
       inside = 0;
     if (inside && patternlinestyle == 0)
       {  x1 = cos(starta);
          y1 = sin(starta);
          pt1 = vs->modeltoscreen(o + xa * x1 + ya * y1);
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          vs->moveto(pt1.x,pt1.y);
          for (i = 0 ; i < n ; i++)
            {  x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               pt2 = vs->modeltoscreen(o + xa * x2 + ya * y2);

               vs->lineto(painter,pt2.x,pt2.y);

               x1 = x2;  y1 = y2;
            }
       }
     else
       {
       */
          x1 = cos(starta);
          y1 = sin(starta);
          z1 = 0;
          //pt1 = p1 = vs->modeltoview(o + xa * x1 + ya * y1);
          //pt1 = p1 = vs->modeltransform.transform(o + xa * x1 + ya * y1);
          if(t == NULL)
          {
              //vs->modeltransform.dump("circle1");
              pt1 = p1 = vs->modeltransform.transform(o + xa * x1 + ya * y1);
          }
          else
          {
              //t->dump("circle2");
              pt1 = p1 = t->transform(vs->modeltransform.transform(o + xa * x1 + ya * y1));
          }
          da = sweepa / n;
          sina = sin(da);  cosa = cos(da);
          //qDebug() << "calling vs->lineGL from circle";
          for (i = 0 ; i < n ; i++)
          {
               x2 = x1 * cosa - y1 * sina;
               y2 = x1 * sina + y1 * cosa;
               z2 = 0;
               //pt2 = p2 = vs->modeltoview(o + xa * x2 + ya * y2);
               //pt2 = p2 = vs->modeltransform.transform(o + xa * x2 + ya * y2);
               if(t == 0)
               {
                   pt2 = p2 = vs->modeltransform.transform(o + xa * x2 + ya * y2);
               }
               else
               {
                   pt2 = p2 = t->transform(vs->modeltransform.transform(o + xa * x2 + ya * y2));
               }
               //if (vs->clip3d(&pt1,&pt2))
               //  {
                    //pt1 = vs->viewtoscreen(pt1);
                    //pt2 = vs->viewtoscreen(pt2);

               if(patternlinestyle !=0)
               {
                   // drawing patternstyle with GL in screen space
                   Point spt1 = vs->modeltoscreen(pt1);
                   Point spt2 = vs->modeltoscreen(pt2);
                   vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);
               }
               /*
               else if(vs->getlinestyle())
               {
                   Point spt1 = vs->modeltoscreen(pt1);
                   Point spt2 = vs->modeltoscreen(pt2);
                   vs->line2dGL(spt1.x,spt1.y,spt2.x,spt2.y);
               }*/
               else
               {
                   //qDebug() << "x: " << pt1.x << "y: " << pt1.y << "z: " << pt1.z;
                   vs->lineGL(pt1.x,pt1.y,pt1.z,pt2.x,pt2.y,pt2.z);
               }
               //  }
               pt1 = p1 = p2;  x1 = x2;  y1 = y2; z1 = z2;
            }
           //qDebug() << "end calling vs->lineGL from circle";
       //}

     if (patternlinestyle != 0)
       patternlinestyle->endGL(vs);

#if 0
     glEndList();
#endif

     if (gethandles())
     {

          vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_HANDLE_SELECT,colour,0,0,0);
          //vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
          //painter->setPen(vs->curPen);
          RCCOLORREF pcol;
          pcol = vs->getcolour();
          //GLfloat fr=GetRValue(pcol)/255.0;
          //GLfloat fg=GetGValue(pcol)/255.0;
          //GLfloat fb=GetBValue(pcol)/255.0;
          //glColor3f(fr,fg,fb);
          glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
          GLfloat lwidth = vs->curPen.widthF();
          glLineWidth(lwidth<1.0?1.0:lwidth);

          //pt1 = vs->modeltoscreen(origin);
          //pt2 = vs->modeltoscreen(origin+xaxis*radius);
          pt1 = origin;
          pt2 = origin+xaxis*radius;

          Point pright = vs->getuaxis();
          Point pup = vs->getvaxis();
          double sc = vs->getpixelscale() * 6.0;

          vs->cbox2dGL(pt1,pright,pup,sc,sc);
          vs->cbox2dGL(pt2,pright,pup,sc,sc);
     }
}
#endif

void Circle::draw(int drawmode,Transform *t,View3dSurface *vs)
{
#ifdef USING_WIDGETS
 int    n,inside,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point o,xa,ya,p1,p2,pt1,pt2;

 QGraphicsItem *qline = 0;

 //qDebug() << "Entering Circle::draw() : qgi : " << qgi;

  if(drawmode != DM_PRINT )
  {
      //if(vs->qgirb && vs->getScene())
      //    ((QGraphicsItemGroup*)vs->qgirb)->removeFromGroup(qgi);

      //if(qgi && qgi->scene() != vs->getScene())
      //    qgi->scene()->removeItem(qgi);
      if(qgi && qgi->scene() != vs->getScene())
          qgi->scene()->removeItem(qgi);
      else if(qgi && vs->getScene())
          vs->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      // handles
      if(qgi1 && qgi1->scene() != vs->getScene())
        qgi1->scene()->removeItem(qgi1);
      delete qgi1; qgi1=0;

      if(qgi2 && qgi2->scene() != vs->getScene())
          qgi2->scene()->removeItem(qgi2);
      delete qgi2; qgi2=0;

  }


  //if(drawmode == DM_ERASE || (drawmode == DM_INVERT && drawmode == DM_PRINT))
  if(drawmode == DM_ERASE)
      return;

#if 0
  // KMJ: testing for speed up
  if(getselected() || drawmode == DM_SELECT)
      return;

  qgi = new QGraphicsItemGroup();
#else
  qgi = new QGraphicsPathItem();
  QPainterPath pPath;
#endif

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,fabs(radius * sweepa),drawmode,colour,style,weight,patternlinestyle);

#if 0
  if (t == NULL && patternlinestyle == 0 && ! gethandles()  )
  //if (t == NULL && patternlinestyle == 0 && ! gethandles() && drawmode  == DM_SELECT)
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
    {  o = origin;  xa = xaxis * radius;  ya = yaxis * radius;
    }
  else
    {  o = t->transform(origin);
       xa = t->transform(origin+xaxis*radius) - o;
       ya = t->transform(origin+yaxis*radius) - o;
    }

  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  if(n < 1)
    n = 1;
  else if(n == 1)
      n = 2;
  if (vs->inside(vs->modeltoview( xa + ya + o)) &&
      vs->inside(vs->modeltoview(-xa + ya + o)) &&
      vs->inside(vs->modeltoview( xa - ya + o)) &&
      vs->inside(vs->modeltoview(-xa - ya + o)))
    inside = 1;
  else
    inside = 0;
  //inside = 1;

  if (inside && patternlinestyle == 0)
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = vs->modeltoscreen(o + xa * x1 + ya * y1);
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
            pt2 = vs->modeltoscreen(o + xa * x2 + ya * y2);

#if 0
            qline = vs->lineto((QGraphicsItem*)0,pt2.x,pt2.y);
            if(qline)
                ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
#else
            vs->lineto(&pPath,pt2.x,pt2.y);
#endif
            x1 = x2;  y1 = y2;
         }
    }
  else
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = p1 = vs->modeltoview(o + xa * x1 + ya * y1);
       da = sweepa / n;
       sina = sin(da);  cosa = cos(da);
       //qDebug() << "calling vs->line from circle";
       for (i = 0 ; i < n ; i++)
         {  x2 = x1 * cosa - y1 * sina;
            y2 = x1 * sina + y1 * cosa;
            pt2 = p2 = vs->modeltoview(o + xa * x2 + ya * y2);
            if (vs->clip3d(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
#if 0
                 qline = vs->line((QGraphicsItem*)0,pt1.x,pt1.y,pt2.x,pt2.y);
                 if(qline)
                     ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
#else
                 //qDebug() << "x: " << pt1.x << "y: " << pt1.y << "z: " << pt1.z;
                 vs->line(&pPath,pt1.x,pt1.y,pt2.x,pt2.y);
#endif
              }
            pt1 = p1 = p2;  x1 = x2;  y1 = y2;
         }
       //qDebug() << "end calling vs->line from circle";
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

  if (gethandles())
    {
       QGraphicsItem *qgout = 0;
       QList<QGraphicsItem *> glist;

       if(qgi1) vs->getScene()->removeItem(qgi1);
       delete qgi1; qgi1=0;

       vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
       pt1 = vs->modeltoscreen(origin);
       pt2 = vs->modeltoscreen(origin+xaxis*radius);

       qgout = vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2); if( qgout != 0 ) glist.append( qgout );

       if(glist.size() > 0)
          qgi1 = vs->getScene()->createItemGroup(glist);
       if(qgi1 && getselected())
           qgi1->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
       glist.clear();

       if(qgi2) vs->getScene()->removeItem(qgi2);
       delete qgi2; qgi2=0;

       qgout = vs->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2); if( qgout != 0 ) glist.append( qgout );
       qgout = vs->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2); if( qgout != 0 ) glist.append( qgout );

       if(glist.size() > 0)
          qgi2 = vs->getScene()->createItemGroup(glist);
       if(qgi2 && getselected())
           qgi2->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
       glist.clear();
  }
  else
  {
      if(qgi1) vs->getScene()->removeItem(qgi1);
      delete qgi1; qgi1=0;
      if(qgi2) vs->getScene()->removeItem(qgi2);
      delete qgi2; qgi2=0;
  }

  if(qgi)
  {
      if(getselected())
          qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
      else
          qgi->setZValue(zValue);

      if(qgi && dynamic_cast<View3dSubSurface*>(vs) != 0 && !usedByPlane)
      {
          if(((View3dSubSurface*)vs)->qgicp != 0 &&
             ((View3dSubSurface*)vs)->qgicp != qgi->parentItem() )
          {
              // KMJ : NOTE:
              // don't make qgi null here because it may be used by a plane
              // a parent entity may use the qgi to reparent it
              // otherwise it is in the subsurface group
              qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);
              //qgi=0;
          }
          else
              vs->getScene()->addItem(qgi);
      }
      else
          vs->getScene()->addItem(qgi);

      if(drawmode == DM_INVERT)
      {
          if(qgi)
          {
              if(vs->qgirb == 0)
              {
                  //qDebug() << "surface : " << vs->gethwnd();
                  vs->qgirb = new QGraphicsItemGroup();
                  vs->getScene()->addItem(vs->qgirb);
                  ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
              }
              else
                  ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
              qgi=0;
          }
      }
      //else
      //    vs->getScene()->addItem(qgi);
  }

  //qDebug() << "Leaving Circle::draw() : qgi : " << qgi;
#else
    int    n,inside,i;
 double x1,y1,x2,y2,da,sina,cosa; 
 Point o,xa,ya,p1,p2,pt1,pt2;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,fabs(radius * sweepa),drawmode,colour,style,weight,patternlinestyle);
  if (t == NULL && patternlinestyle == 0 && ! gethandles())
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  if (t == NULL)
    {  o = origin;  xa = xaxis * radius;  ya = yaxis * radius;
    }
  else
    {  o = t->transform(origin);
       xa = t->transform(origin+xaxis*radius) - o;
       ya = t->transform(origin+yaxis*radius) - o;
    }
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  if (vs->inside(vs->modeltoview( xa + ya + o)) &&
      vs->inside(vs->modeltoview(-xa + ya + o)) &&
      vs->inside(vs->modeltoview( xa - ya + o)) &&
      vs->inside(vs->modeltoview(-xa - ya + o)))
    inside = 1;
  else
    inside = 0;
  if (inside && patternlinestyle == 0)
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = vs->modeltoscreen(o + xa * x1 + ya * y1);
       da = sweepa / n;
       sina = sin(da);  cosa = cos(da);
       vs->moveto(pt1.x,pt1.y);
       for (i = 0 ; i < n ; i++)
         {  x2 = x1 * cosa - y1 * sina;
            y2 = x1 * sina + y1 * cosa;
            pt2 = vs->modeltoscreen(o + xa * x2 + ya * y2);
            vs->lineto(pt2.x,pt2.y);
            x1 = x2;  y1 = y2;
         }
    }
  else
    {  x1 = cos(starta);
       y1 = sin(starta);
       pt1 = p1 = vs->modeltoview(o + xa * x1 + ya * y1);
       da = sweepa / n;
       sina = sin(da);  cosa = cos(da);
       for (i = 0 ; i < n ; i++)
         {  x2 = x1 * cosa - y1 * sina;
            y2 = x1 * sina + y1 * cosa;
            pt2 = p2 = vs->modeltoview(o + xa * x2 + ya * y2);
            if (vs->clip3d(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 vs->line(pt1.x,pt1.y,pt2.x,pt2.y);
              }
            pt1 = p1 = p2;  x1 = x2;  y1 = y2;
         }
    }
  if (patternlinestyle != 0)
    patternlinestyle->end();
  if (gethandles())
    {  vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
       pt1 = vs->modeltoscreen(origin);
       pt2 = vs->modeltoscreen(origin+xaxis*radius);
       vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
       vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
       vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
       vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);

       vs->line(int(pt2.x)-2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)-2);
       vs->line(int(pt2.x)+2,int(pt2.y)-2,int(pt2.x)+2,int(pt2.y)+2);
       vs->line(int(pt2.x)+2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)+2);
       vs->line(int(pt2.x)-2,int(pt2.y)+2,int(pt2.x)-2,int(pt2.y)-2);
    }
#endif
}

#ifdef USING_WIDGETS
int Circle::hideGL(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 + yaxis * y2;
       image->hideGL(this,p1,p2);
       p1 = p2;  x1 = x2;  y1 = y2;
    }

  return 1;
}

int Circle::hide(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;


    if(image->drawmode != 1) // drawmode 1 = printing
    {
        if(qgi && qgi->scene() != image->getsurface()->getScene())
         qgi->scene()->removeItem(qgi);
        else if(qgi && image->getsurface()->getScene())
         image->getsurface()->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
    }
    // handles
    if(qgi1 && qgi1->scene() != image->getsurface()->getScene())
    qgi1->scene()->removeItem(qgi1);
    delete qgi1; qgi1=0;

    if(qgi2 && qgi2->scene() != image->getsurface()->getScene())
     qgi2->scene()->removeItem(qgi2);
    delete qgi2; qgi2=0;

    qgi = new QGraphicsItemGroup();

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 + yaxis * y2;
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
int Circle::hide(HideImage *image)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1,p2;

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  n1 = nsegments((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 + yaxis * y2; 
       image->hide(this,p1,p2);
       p1 = p2;  x1 = x2;  y1 = y2;
    } 
  return 1;
}
#endif

double Circle::pick(View3dSurface *vs,double x,double y,Point *pos)
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
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 + yaxis * y2; 
       if ((d = vs->pick(p1,p2,x,y,pos)) >= 0.0) return d;
       x1 = x2;  y1 = y2;  p1 = p2;
    }
  return -1.0;
}

int Circle::pickendpoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{Point p1,p2;
 double d1,d2;
 int end;

  if (! distance && sweepa > 1.99999 * M_PI) return 0;
  p1 = origin + xaxis * (radius * cos(starta)) + yaxis * (radius * sin(starta));
  p2 = origin + xaxis * (radius * cos(starta+sweepa)) + yaxis * (radius * sin(starta+sweepa));
  end = 0;
  if ((d1 = vs->pick(p1,distance,x,y)) >= 0.0)
    {  end = 1;  *pos = p1;
    }
  if ((d2 = vs->pick(p2,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  end = 2;  *pos = p2;
    }
  return end;
}

int Circle::pickoriginpoint(View3dSurface *vs,int snap,int x,int y,Point *pos)
{double d1;
 int end;

  end = 0;
  if ((d1 = vs->pick(origin,snap,x,y)) >= 0.0)
    {  end = 3;  *pos = origin;
    }

  return end;
}

int Circle::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{Point p1,p2;
 double d1,d2;
 int handle;
  p1 = origin;
  p2 = origin + xaxis * radius;
  handle = 0;
  if ((d1 = vs->pick(p1,distance,x,y)) >= 0.0)
    {  handle = 1;  *pos = p1;
    }
  if ((d2 = vs->pick(p2,distance,x,y)) >= 0.0 && (d1 < 0.0 || d2 < d1))
    {  handle = 2;  *pos = p2;
    }
  return handle;
}

void Circle::movehandlepoint(View3dSurface *,int drawentity,int handle,Point p)
{double r;
 Point zaxis,xa,ya,o;

  if (GetLockedLayerTable()->test(layer)) return;  

  zaxis = xaxis.cross(yaxis).normalize();
  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  //if (drawentity) cadwindow->clearAllRubberBands();
  cadwindow->invalidatedisplaylist(this);
  if (handle == 1)
    {  o = p;
       xa = ((origin + xaxis * radius) - p).normalize();
       ya = zaxis.cross(xa).normalize();
       r = (p - (origin + xaxis * radius)).length();
    }
  else
    {  o = origin;
       xa = xaxis;
       ya = yaxis;
       r = (p - origin).length();
    }
  if (xa.length() > db.getptoler() && ya.length() > db.getptoler() && r > db.getptoler())
    {  origin = o;
       xaxis = xa;
       yaxis = ya;
       radius = r;
    }

  if (drawentity) draw(DM_NORMAL);
  //if (drawentity) draw(DM_INVERT);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

void Circle::moveendpoint(int drawentity,int end,Point p)
{double a,a1,a2;

  if (GetLockedLayerTable()->test(layer)) return;  

  db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  a = angle(p);
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

double Circle::gett(Point p1)
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


void Circle::breakanddelete(int breakcircle,Point p1,Point p2)
{double a,a1,a2;
 Point p;
 Circle *circle;
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
  if (fabs(sweepa - 2.0 * M_PI) < db.getatoler() && breakcircle == 0)
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
  if (a1 <= starta && ! breakcircle)
    moveendpoint(1,1,p2);
  else if (a2 >= starta + sweepa && ! breakcircle)
    moveendpoint(1,2,p1);
  else
    {  identity.identity();
       copye = NULL;
       circle = (Circle *) clone(identity);
       moveendpoint(1,2,p1);
       circle->moveendpoint(1,1,p2);
       db.geometry.add(circle);
    }
  draw(DM_NORMAL);
  db.saveundo(UD_ENDBLOCK,NULL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

int Circle::trace(View3dSurface *surface,Point end,Point uv1,Point uv2,double side,Point *newend,Point *uv3,Point *uv4,double *angle)
{double cosa,l1,l2;
 Point uv5,uv6,duv1,duv2;
  if (radius * sweepa < db.getptoler())
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

double Circle::pickhor(View3dSurface *vs,int x,int y,Point *start,Point *end,Point *uvp1,Point *uvp2,double *side)
{int i,n;
 double d,dmin,x1,y1,x2,y2,da,sina,cosa,s1;
 Point p1,p2,p3,p4;
  if (start == 0 && vs->displaylist.defined(this))
    return vs->displaylist.pickhor(this,x,y);
  n = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  da = sweepa / n;
  sina = sin(da);  cosa = cos(da);  dmin = -1.0;
  for (i = 0 ; i < n ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p2 = origin + xaxis * x2 + yaxis * y2;
       if ((d = vs->pickhor(p1,p2,x,y,&p3,&s1)) >= 0.0 && (dmin == -1.0 || d < dmin))
         {  p4 = p3;  *side = s1;  dmin = d;
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

LinearEntity *Circle::divide(Point p, Point *dp)
{double a,a2;
 Circle *circle;

   if (GetLockedLayerTable()->test(layer)) return 0;  

  circle = 0;
  p = nearp(p);
  a = angle(p);
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
       circle = new Circle(*this,origin,xaxis,yaxis,radius,a,a2-a);
       db.geometry.add(circle);
       if (getposition() != 0 && db.MovedCallback != 0)
         db.MovedCallback(this);
    }
  return circle;
}

void Circle::divide(int n, PointList *dplist)
{int i;
 double a,da;

   if (GetLockedLayerTable()->test(layer)) return;  

  if (n > 0 && sweepa > db.getatoler())
    {  a = starta;  da = sweepa / (double) n;
       if ((start() - end()).length() < db.getptoler())
         {  PointE point(origin + xaxis * (cos(starta) * radius) + yaxis * (sin(starta) * radius));
            //point.draw(DM_NORMAL);
            point.draw(DM_MARKER);
         }
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
              {  Circle *circle = new Circle(*this,origin,xaxis,yaxis,radius,a,da);
                 db.geometry.add(circle);
              }
            a += da;
            if (i < n)
              {  PointE point(origin + xaxis * (cos(a) * radius) + yaxis * (sin(a) * radius));
                 //point.draw(DM_NORMAL);
                 point.draw(DM_MARKER);
              }
         }
 
    }
}

int Circle::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
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
  dx1 = radius * cos(starta);
  dy1 = radius * sin(starta);
  p1 = origin + xaxis * dx1 + yaxis * dy1;
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  for (i = 0 ; i < n1 ; i++)
    {  if (! vs->pick(p1,inside,x1,y1,x2,y2)) return 0;
       dx2 = dx1 * cosa - dy1 * sina;
       dy2 = dx1 * sina + dy1 * cosa;
       p1 = origin + xaxis * dx2 + yaxis * dy2; 
       dx1 = dx2;  dy1 = dy2;
    }
  if (! vs->pick(p1,inside,x1,y1,x2,y2)) return 0;
  return 1;
}

int Circle::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{int n1,i,picked;
 double dx1,dy1,dx2,dy2,da,sina,cosa;
 Point p1,p2;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  n1 = nsegments((vs->getumax() - vs->getumin()) / vs->getwidth() * db.getdtoler());
  dx1 = radius * cos(starta);
  dy1 = radius * sin(starta);
  p1 = origin + xaxis * dx1 + yaxis * dy1;
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  picked = inside != 2;
  for (i = 0 ; i < n1 ; i++)
    {  dx2 = dx1 * cosa - dy1 * sina;
       dy2 = dx1 * sina + dy1 * cosa;
       p2 = origin + xaxis * dx2 + yaxis * dy2; 
       if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
         return picked;
       p1 = p2; 
       dx1 = dx2;  dy1 = dy2;
    }

  return picked;
}

int Circle::type(void) const
{  return circle_entity;
}

int Circle::isa(int type) const
{  return circle_entity == type;
}

double Circle::length(void) const
{  return fabs(sweepa * radius);
}

Point Circle::start(void)
{ return origin + xaxis * (radius * cos(starta)) + yaxis * (radius * sin(starta));
}

Point Circle::end(void)
{ return origin + xaxis * (radius * cos(starta+sweepa)) + yaxis * (radius * sin(starta+sweepa));
}

int Circle::npoints(void)
{ return 3;
}

Point Circle::point(int i)
{ if (i == 0)
    return origin + xaxis * (radius * cos(starta)) + yaxis * (radius * sin(starta));
  else if (i == 1)
    return origin + xaxis * (radius * cos(starta+sweepa)) + yaxis * (radius * sin(starta+sweepa));
  else
    return origin;
}

Point Circle::position(double t) const
{double a;
  a = starta + sweepa * t;
  return origin + xaxis * (radius * cos(a)) + yaxis * (radius * sin(a));
}

Point Circle::positionbylength(double l,double *x) const
{double a,cl;
  cl = length();
  if (cl > 1.0E-10)
    *x = l / cl;
  else
    *x = 0.0;
  a = starta + sweepa * (*x);
  return origin + xaxis * (radius * cos(a)) + yaxis * (radius * sin(a));
}

Point Circle::direction(double t) const
{double a;
  a = starta + sweepa * t;
  return xaxis * sin(-a) + yaxis * cos(a);
}

Point Circle::nearp(Point p)
{Point tmp;
  tmp.x = xaxis.dot(p) - xaxis.dot(origin);
  tmp.y = yaxis.dot(p) - yaxis.dot(origin);
  tmp.z = 0.0;
  if (tmp.length() < db.getptoler())
    return origin + xaxis * radius;
  else
    {  tmp.normalize();  tmp *= radius;
       return origin + xaxis * tmp.x + yaxis * tmp.y;
    }
}

int Circle::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{int match1,match2,match3; 
 Point start1,end1,start2,end2;
 Entity *e,*c;

  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }

  start1 = start();
  end1 = end();
  match1 = sp->match(origin); 
  match2 = sp->match(start1);
  match3 = sp->match(end1);
  if (! match1 && ! match2 && !match3) return 1;
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
    origin = t.transform(origin);
  if (match2)
    start2 = t.transform(start1);
  else
    start2 = start1;
  if (match3)
    end2 = t.transform(end1);
  else
    end2 = end1;
  if (fabs((origin-start2).length() - radius) > fabs((origin-end2).length() - radius))
    radius = (origin-start2).length();
  else
    radius = (origin-end2).length();
  copye = this;

  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;
}

int Circle::move(Database *db,Transform &t,int erase)
{Point zero(0.0,0.0,0.0);
 Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db->geometry.add(clone(t));
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 1 + 32;
  origin = t.transform(origin);
  xaxis = t.transform(xaxis*radius) - t.transform(zero);
  yaxis = t.transform(yaxis*radius) - t.transform(zero);
  radius = (xaxis.length() + yaxis.length()) / 2.0;
  xaxis.normalize();  yaxis.normalize();
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

void Circle::transform(Transform &t,int drawentity)
{ db.saveundo(UD_MOVED,this);
  if (drawentity) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  origin = t.transform(origin);
  if (drawentity) draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}



Entity *Circle::clone(Transform &t)
{Point o,xa,ya,zero(0.0,0.0,0.0);
 double r;
  if (copye != NULL) return copye;
  o = t.transform(origin);
  xa = t.transform(xaxis*radius) - t.transform(zero);
  ya = t.transform(yaxis*radius) - t.transform(zero);
  r = (xa.length() + ya.length()) / 2.0;
  xa.normalize();  ya.normalize();
  copye = new Circle(*this,o,xa,ya,r,starta,sweepa);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
        return copye;
    }
  copye->cloneattributes(this);
  return copye;
}

int Circle::project(Database *db,Transform &t,int surface)
{Point p;
 BitMask options(32);
 int xmesh,ymesh;
  if ((status & 2) == 2) return 1;
  p = origin + xaxis * (radius * cos(starta)) + yaxis * (radius * sin(starta));
  if (! db->geometry.add(new Line(*this,p,t.transform(p)))) return 0;
  p = origin + xaxis * (radius * cos(starta+sweepa)) + yaxis * (radius * sin(starta+sweepa));
  if (! db->geometry.add(new Line(*this,p,t.transform(p)))) return 0;
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

int Circle::beginoffset(Point p,Point *zaxis,double *d,LinearEntity *enext,int *reverse,Point *startp)
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

int Circle::offset(Point zaxis,double d,int cornermode,LinearEntity *enext,int *reverse,Point *startp,EntityList *olist)
{Circle *arc;
 Point za;
  za = xaxis.cross(yaxis);
  if (*reverse) za = -za;
  if (enext == NULL || enext->isa(ellipse_entity) || enext->isa(curve_entity))
    {Point p3;
       if (! offsetpoint(zaxis,d,*reverse,2,&p3)) return 0;
       if ((arc = new Circle(0,origin,za,*startp,p3)) == NULL) return 0;
       if (arc->defined)
       {  olist->add(arc);
          arc->setcopye(this);
       }
       else
         delete arc;
       *startp = p3;
       *reverse = enext != 0 && (enext->position(0.0) - position(1.0-*reverse)).length() > (enext->position(1.0) - position(1.0-*reverse)).length();
    }
  else if (enext->isa(line_entity))
    {Point p3,p4,p5,xaxis2,yaxis1,yaxis2;
     double x1,y1,r;
     int rev;
     Line *lnext = (Line *) enext;
       if (*reverse)
         {  yaxis1 = (zaxis.cross(-direction(0.0))).normalize();
            p3 = position(0.0);
         }
       else
         {  yaxis1 = (zaxis.cross(direction(1.0))).normalize();
            p3 = position(1.0);
         }
       xaxis2 = (lnext->getp2() - lnext->getp1()).normalize();
       yaxis2 = (zaxis.cross(xaxis2)).normalize();
       if ((lnext->getp1() - p3).length() > (lnext->getp2() - p3).length())
         {  xaxis2 = -xaxis2;
            yaxis2 = -yaxis2;
            rev = 1;
         }
       else
         rev = 0;
       if (xaxis2.length() < db.getptoler() || yaxis1.length() < db.getptoler() ||
           yaxis2.length() < db.getptoler()) return 0;
       if (! offsetpoint(zaxis,d,*reverse,2,&p4)) return 0;
       if (! enext->offsetpoint(zaxis,d,rev,1,&p5)) return 0;
       r = (p4 - origin).length();
       y1 = fabs(yaxis2.dot(p5 - origin));
       if (r < y1 || cornermode && zaxis.dot(yaxis1.cross(yaxis2)) < 0.0)
         {  if ((arc = new Circle(0,origin,za,*startp,p4)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            if ((arc = new Circle(0,p3,-zaxis,p4,p5)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p5;
            *reverse = rev;
         }
       else
         {  x1 = r * r - y1 * y1;  if (x1 < 0.0) x1 = 0.0;
            x1 = sqrt(x1);
            if (yaxis2.dot(p5 - origin) < 0.0) y1 = -y1;
            if (xaxis2.dot(p3 - origin) < 0.0) x1 = -x1;
            p4 = origin + xaxis2 * x1 + yaxis2 * y1;
            if ((arc = new Circle(0,origin,za,*startp,p4)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p4;
            *reverse = rev;
         }
    }
  else if (enext->isa(circle_entity))
    {Point p3,p4,p5,yaxis1,yaxis2,xaxis3,yaxis3,dir1,dir2;
     double x1,y1,r1,r2,d1,cosa,a;
     int rev;
     Circle *cnext = (Circle *) enext;
       if (*reverse)
         {  yaxis1 = (zaxis.cross(-direction(0.0))).normalize();
            dir1 = direction(0.0);
            p3 = start();
         }
       else
         {  yaxis1 = (zaxis.cross(direction(1.0))).normalize();
            dir1 = -direction(1.0);
            p3 = end();
         }
       if ((cnext->start() - p3).length() > (cnext->end() - p3).length())
         {  yaxis2 = (zaxis.cross(-cnext->direction(1.0))).normalize();
            dir2 = -cnext->direction(1.0);
            rev = 1;
         }
       else
         {  yaxis2 = (zaxis.cross(cnext->direction(0.0))).normalize();
            dir2 = cnext->direction(0.0);
            rev = 0;
         }
       if (yaxis1.length() < db.getptoler() || yaxis2.length() < db.getptoler()) return 0;
       if (! offsetpoint(zaxis,d,*reverse,2,&p4)) return 0;
       if (! enext->offsetpoint(zaxis,d,rev,1,&p5)) return 0;
       r1 = (p4 - origin).length();
       r2 = (p5 - cnext->origin).length();
       d1 = (origin - cnext->origin).length();
       if (fabs(d1) < db.getptoler())
         {Point p3;
           if (! offsetpoint(zaxis,d,*reverse,2,&p4)) return 0;
           if ((arc = new Circle(0,origin,za,*startp,p4)) == NULL) return 0;
           if (arc->defined)
           {    olist->add(arc);
                arc->setcopye(this);
           } 
           else
             delete arc;
           *startp = p4;
           *reverse = rev;
           return 1;
         }
       if (fabs(r1) < db.getptoler())
         {  if ((arc = new Circle(1,p3,zaxis,p4,p5)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p5;
            *reverse = rev;
            return 1;
         }
       cosa = (r1 * r1 + d1 * d1 - r2 * r2) / 2.0 / r1 / d1;
       if (r1 + r2 < d1 || cosa < -1.0 || cosa > 1.0 ||
           cornermode && zaxis.dot(yaxis1.cross(yaxis2)) < 0.0)
         {  if ((arc = new Circle(0,origin,za,*startp,p4)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            if ((arc = new Circle(0,p3,-zaxis,p4,p5)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p5;
            *reverse = rev;
         }
       else
         {  a = acos(cosa);
            x1 = r1 * cos(a);  y1 = r1 * fabs(sin(a));
            xaxis3 = (cnext->origin - origin).normalize();
            yaxis3 = zaxis.cross(xaxis3).normalize();
            d1 = yaxis3.dot(p3 - origin);
            if (fabs(d1) < db.getptoler()) d1 = yaxis3.dot((p3+dir1+dir2) - origin);
            if (d1 < 0.0) y1 = -y1;
            p4 = origin + xaxis3 * x1 + yaxis3 * y1;
            if ((arc = new Circle(0,origin,za,*startp,p4)) == NULL) return 0;
            if (arc->defined)
            {  olist->add(arc);
               arc->setcopye(this);
            }
            else
              delete arc;
            *startp = p4;
            *reverse = rev;
         }
    }
  return 1;
}

int Circle::offsetpoint(Point zaxis,double d,int reverse,int end,Point *p)
{Point xaxis,yaxis;
 double t;
  if (reverse)
    {  t = 2.0 - end;
       if (end == 1)
         yaxis = zaxis.cross(-direction(t)).normalize();
       else
         yaxis = zaxis.cross(-direction(t)).normalize();
    }
  else
    {  t = end - 1.0;
       if (end == 1)
         yaxis = zaxis.cross(direction(t)).normalize();
       else
         yaxis = zaxis.cross(direction(t)).normalize();
    }
  if (yaxis.length() < db.getptoler()) return 0;
  yaxis *= d;
  if (end == 1)
    *p = position(t) + yaxis;
  else
    *p = position(t) + yaxis;
  return 1;
} 

int Circle::save(CadFile *outfile)
{short type;
 Byte terminators = 0;
  type = circle_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&radius)) return 0;
  if (! outfile->write(&starta)) return 0;
  if (! outfile->write(&sweepa)) return 0;
  if (! outfile->write(&terminators)) return 0;
  return 1;
}

Entity *Circle::load(int,CadFile *infile)
{Circle *circle;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 double radius,sweepa,starta;
 Byte terminators;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&radius)) return 0;
  if (! infile->read(&starta)) return 0;
  if (! infile->read(&sweepa)) return 0;
  if (! infile->read(&terminators)) return 0;
  circle = new Circle(*header,origin,xaxis,yaxis,radius,starta,sweepa);
  circle->atlist = header->getatlist();
  delete header;
  return circle;
}

int Circle::save(UndoFile *outfile)
{short type;
  type = circle_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  return 1;
}

Entity *Circle::load(UndoFile *infile)
{Circle *circle;
 Point p;
 char bm[sizeof(BitMask)];
  if ((circle = new Circle) != NULL)
    {AttributeList al;
       memcpy(bm,&circle->visible,sizeof(bm));
       if (! infile->blockread(circle,sizeof(*circle))) return 0;
       memcpy(&circle->visible,bm,sizeof(bm));
       if (! infile->read(&circle->visible)) return 0;
       if (! al.load(infile)) return 0;   circle->atlist = al;
    }
  return circle;
}

int Circle::saveattributes(CadFile *)
{ return 1;
}

int Circle::loadattributes(CadFile *)
{ return 1;
}

int Circle::savedxf(int blocksection,DXFOut *dxffile)
{Point zaxis,wx(1.0,0.0,0.0),wy(0.0,1.0,0.0),wz(0.0,0.0,1.0),ax,ay;
 double a,a1;
 int arc;
  if (! blocksection)
    {  zaxis = xaxis.cross(yaxis).normalize();
       if (fabs(zaxis.x) < 1.0/64.0 && fabs(zaxis.y) < 1.0/64.0)
         ax = wy.cross(zaxis).normalize();
       else
         ax = wz.cross(zaxis).normalize();
       ay = zaxis.cross(ax).normalize();
       a = angle(ax + origin);
       arc = sweepa < M_PI * 2.0 - db.getatoler();
       if (arc)
         {  if (! dxffile->writestring(0,(char*)"ARC")) return 0;
         }
       else
         {  if (! dxffile->writestring(0, (char*)"CIRCLE")) return 0;
         }
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbCircle")) return 0;
       if (! dxffile->writereal(10,ax.dot(origin) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(20,ay.dot(origin) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(30,zaxis.dot(origin) * dxffile->getscale())) return 0;
       if (! dxffile->writereal(40,radius * dxffile->getscale())) return 0;
       if (! dxffile->writereal(210,zaxis.x)) return 0;
       if (! dxffile->writereal(220,zaxis.y)) return 0;
       if (! dxffile->writereal(230,zaxis.z)) return 0;
       if (arc)
         {  if (! dxffile->writestring(100, (char*)"AcDbArc")) return 0;
            a1 = (starta - a) * 180.0 / M_PI;
            while (a1 < 0.0) a1 += 360.0;
            while (a1 > 360.0) a1 -= 360.0;
            if (! dxffile->writereal(50,a1)) return 0;
            a1 = (starta + sweepa - a) * 180.0 / M_PI;
            while (a1 < 0.0) a1 += 360.0;
            while (a1 > 360.0) a1 -= 360.0;
            if (! dxffile->writereal(51,a1)) return 0;
         }
    }
  return 1;
}


int Circle::exportiges(FILE *outfile,int section,int,int *ndirectory,int *nparameter)
{Point p1,p2;
 double x2,y2,x3,y3;
  if (section == 0)
    {  if (fprintf(outfile,"     100%8d               0       0       0       0       0 0 0 0 0D%7d\n",(*nparameter)+1,++(*ndirectory)) < 0) return 0;
       if (fprintf(outfile,"     100       0%8d       2       0                               0D%7d\n",colour,++(*ndirectory)) < 0) return 0;
       (*nparameter) += 2;
    }
  else 
    {  p1 = start();
       p2 = end();
      if (sweepa > 0.0 && xaxis.cross(yaxis).z > 0.0)
         {  x2 = p1.x;
            y2 = p1.y;
            x3 = p2.x;
            y3 = p2.y;
         }
       else
         {  x2 = p2.x;
            y2 = p2.y;
            x3 = p1.x;
            y3 = p1.y;
         }
       if (fprintf(outfile,"100,0,%18.10e,%18.10e,%18.10e,  %7dP%7d\n",origin.x,origin.y,x2,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       if (fprintf(outfile,"%18.10e,%18.10e,%18.10e;        %7dP%7d\n",y2,x3,y3,*ndirectory + 1,++(*nparameter)) < 0) return 0;
       (*ndirectory) += 2;
    }

   return 1;
}

int Circle::process_property(int action,int id,void *data,RCCHAR *text)
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
            db.formatnumber((RCCHAR*)text,origin.x,0);
        else if (id == 7)
            db.formatnumber((RCCHAR*)text,origin.y,0);
        else if (id == 8)
            db.formatnumber((RCCHAR*)text,origin.z,0);
        else if (id == 9)
            db.formatnumber((RCCHAR*)text,radius,0);
        else if (id == 10)
            db.formatnumber((RCCHAR*)text,starta * 180.0 / acos(-1.0),0);
        else if (id == 11)
            db.formatnumber((RCCHAR*)text,sweepa * 180.0 / acos(-1.0),0);
        else if (id == 12)
        {   db.formatnumber((RCCHAR*)text,length(),0);
            return 1;
        }
    }
    else if (action == PP_SETVALUE)
    {   if (id == 6)
        {   p1 = Point(atof((char*)text),origin.y,origin.z);
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 7)
        {   p1 = Point(origin.x,atof((char*)text),origin.z);
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 8)
        {   p1 = Point(origin.x,origin.y,atof((char*)text));
            t.translate(p1 - origin);
            copye = 0;
            move(&db,t,1);
            draw(DM_NORMAL);
        }
        else if (id == 9)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            radius = atof((char*)text);
            draw(DM_NORMAL);
        }
        else if (id == 10)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            starta = atof((char*)text) / 180.0 * acos(-1.0);
            draw(DM_NORMAL);
        }
        else if (id == 11)
        {   db.saveundo(UD_MOVED,this);
            draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(this);
            sweepa = atof((char*)text) / 180.0 * acos(-1.0);
            draw(DM_NORMAL);
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Circle");
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
                strcpy(text,"Radius");
            else if (id == 10)
                strcpy(text,"Start Angle");
            else if (id == 11)
                strcpy(text,"Sweep Angle");
            else if (id == 12)
                strcpy(text,"Length");
            return 0;
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 13; 
    
    return 0;
}

