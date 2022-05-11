
#include "ncwin.h"
#include "lightverify_dialog.h"

//
//  The Light Entity class definition
//

Light::Light(Point org,Point d,double i,double s,double fd,double fe,double ca,double c,const BitMask &o) :
  options(32)
{ origin = org;
  direction = d;
  options = o;
  intensity = i;
  size = s;
  fadedistance = fd;
  fadeexponent = fe;
  coneangle = ca;
  concentration = c;
}

Light::Light(const EntityHeader &header,Point org,Point d,double i,double s,double fd,double fe,double ca,double c,const BitMask &o) :
  Entity(header),options(32)
{ origin = org;
  direction = d;
  options = o;
  intensity = i;
  size = s;
  fadedistance = fd;
  fadeexponent = fe;
  coneangle = ca;
  concentration = c;
}

double Light::pick(View3dSurface *v,double x,double y,Point *pnear)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
 double d,rsize;
 int i;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(v);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    v = window->getoffscreenbitmap();
 
  if (pnear == NULL && v->displaylist.defined(this))
    return v->displaylist.pick(this,x,y);
  xaxis = v->getuaxis();
  yaxis = v->getvaxis();
  rsize = 3.0 * ((v->getumax() - v->getumin()) / v->getwidthmm());
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       if ((d = v->pick(origin + v1 * 0.6,origin + v1,x,y,pnear)) >= 0.0)
         return d;
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       if ((d = v->pick(origin,p2,x,y,pnear)) >= 0.0)
         return d;
       if (i == 0)
         p3 = p2;
       else
         {  if ((d = v->pick(p1,p2,x,y,pnear)) >= 0.0)
              return d;
         }
       p1 = p2;
    }
  if ((d = v->pick(p1,p2,x,y,pnear)) >= 0.0)
    return d;
  return -1.0;
}

int Light::pick(View3dSurface *vs,int insidew,int x1,int y1,int x2,int y2)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p3;
 double rsize;
 int i;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,insidew,x1,y1,x2,y2);
  xaxis = vs->getuaxis();
  yaxis = vs->getvaxis();
  rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
  if (! vs->pick(origin,insidew,x1,y1,x2,y2)) return 0;
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       if (! vs->pick(origin + v1 * 0.6,insidew,x1,y1,x2,y2)) return 0;
       if (! vs->pick(origin + v1,insidew,x1,y1,x2,y2)) return 0;
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p1 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       if (! vs->pick(p1,insidew,x1,y1,x2,y2)) return 0;
    }
  return 1;
}

int Light::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
 double rsize;
 int i,picked;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  xaxis = vs->getuaxis();
  yaxis = vs->getvaxis();
  rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
  picked = inside != 2;
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       if (vs->pick(nvertices,pwvertices,origin + v1 * 0.6,origin + v1,inside,&picked))
         return picked;
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       if (vs->pick(nvertices,pwvertices,origin,p2,inside,&picked))
         return picked;
       if (i == 0)
         p3 = p2;
       else
         {  if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
              return picked;
         }
       p1 = p2;
    }
  if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
    return picked;
  return picked;
}

void Light::addtodisplaylist(View3dSurface *vs,int child)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3,pt1,pt2;
 double rsize;
 int i;
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,40 * 34)) return;
    }
  xaxis = vs->getuaxis();
  yaxis = vs->getvaxis();
  rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       pt1 = vs->modeltoview(origin + v1 * 0.6);
       pt2 = vs->modeltoview(origin + v1);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       pt1 = vs->modeltoview(origin);
       pt2 = vs->modeltoview(p2);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
       if (i == 0)
         p3 = p2;
       else
         {  pt1 = vs->modeltoview(p1);
            pt2 = vs->modeltoview(p2);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
              }
         }
       p1 = p2;
    }
  pt1 = vs->modeltoview(p1);
  pt2 = vs->modeltoview(p3);
  if (vs->clip3dfb(&pt1,&pt2))
    {  pt1 = vs->viewtoscreen(pt1);
       pt2 = vs->viewtoscreen(pt2);
       if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
       if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
    }
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void Light::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
    Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
    double rsize;
    int i;

    // get rid of the scene version
    draw(DM_ERASE,t,vs);

     if (getselected() && drawmode == DM_NORMAL)
         drawmode = DM_SELECT;

     vs->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
     RCCOLORREF pcol;
     pcol = vs->getcolour();
     SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
     GLfloat lwidth = vs->curPen.widthF();
     glLineWidth(vs->curGLWeight);

     xaxis = vs->getuaxis();
     yaxis = vs->getvaxis();
     rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
     for (i = 0 ; i < 8 ; i++)
     {
          v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
               yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
          v1 = v1.normalize() * rsize;
          if (t == 0)
            vs->line3dGL(origin + v1 * 0.6,origin + v1);
            //vs->line3d(origin + v1 * 0.6,origin + v1);
          else
            vs->line3dGL(t->transform(origin + v1 * 0.6),t->transform(origin + v1));
            //vs->line3d(t->transform(origin + v1 * 0.6),t->transform(origin + v1));
     }
     zaxis = (direction - origin).normalize();
     za.setxyz(1.0,0.0,0.0);
     v1 = za.cross(zaxis);
     za.setxyz(0.0,1.0,0.0);
     v2 = za.cross(zaxis);
     if (v1.length() > v2.length())
       xaxis = v1.normalize();
     else
       xaxis = v2.normalize();
     yaxis = xaxis.cross(zaxis);
     for (i = 0 ; i < 16 ; i++)
     {
          v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
          p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
          if (t == 0)
            vs->line3dGL(origin,p2);
            //vs->line3d(origin,p2);
          else
            vs->line3dGL(t->transform(origin),t->transform(p2));
            //vs->line3d(t->transform(origin),t->transform(p2));
          if (i == 0)
            p3 = p2;
          else
          {
               if (t == 0)
                 vs->line3dGL(p1,p2);
                 //vs->line3d(p1,p2);
               else
                 vs->line3dGL(t->transform(p1),t->transform(p2));
                 //vs->line3d(t->transform(p1),t->transform(p2));
          }
          p1 = p2;
     }
     if (t == 0)
       vs->line3dGL(p1,p3);
       //vs->line3d(p1,p3);
     else
       vs->line3dGL(t->transform(p1),t->transform(p3));
       //vs->line3d(t->transform(p1),t->transform(p3));

}

void Light::draw(int drawmode,Transform *t,View3dSurface *vs)
{
    Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
    double rsize;
    int i;
    QGraphicsItem *qline = 0;

    if(drawmode != DM_PRINT)
    {
        if(qgi && qgi->scene() != vs->getScene())
            qgi->scene()->removeItem(qgi);
        else if(qgi && vs->getScene())
            vs->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
    }

    if(drawmode == DM_ERASE)
        return;

    qgi = new QGraphicsItemGroup();

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
     vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

#if 0
     if (t == NULL)
       {  if (vs->displaylist.draw(this,drawmode)) return;
          if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
          if (vs->displaylist.draw(this,drawmode)) return;
       }
#endif

     xaxis = vs->getuaxis();
     yaxis = vs->getvaxis();
     rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
     for (i = 0 ; i < 8 ; i++)
     {
          v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
               yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
          v1 = v1.normalize() * rsize;
          if (t == 0)
            qline = vs->line3d((QGraphicsItem*)0,origin + v1 * 0.6,origin + v1);
          else
            qline = vs->line3d((QGraphicsItem*)0,t->transform(origin + v1 * 0.6),t->transform(origin + v1));
          if(qline)
              ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
     }
     zaxis = (direction - origin).normalize();
     za.setxyz(1.0,0.0,0.0);
     v1 = za.cross(zaxis);
     za.setxyz(0.0,1.0,0.0);
     v2 = za.cross(zaxis);
     if (v1.length() > v2.length())
       xaxis = v1.normalize();
     else
       xaxis = v2.normalize();
     yaxis = xaxis.cross(zaxis);
     for (i = 0 ; i < 16 ; i++)
     {
          v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
          p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
          if (t == 0)
            qline = vs->line3d((QGraphicsItem*)0,origin,p2);
          else
            qline = vs->line3d((QGraphicsItem*)0,t->transform(origin),t->transform(p2));
          if(qline)
              ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
          if (i == 0)
            p3 = p2;
          else
          {
               if (t == 0)
                 qline = vs->line3d((QGraphicsItem*)0,p1,p2);
               else
                 qline = vs->line3d((QGraphicsItem*)0,t->transform(p1),t->transform(p2));
               if(qline)
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
          }
          p1 = p2;
     }
     if (t == 0)
       qline = vs->line3d((QGraphicsItem*)0,p1,p3);
     else
       qline = vs->line3d((QGraphicsItem*)0,t->transform(p1),t->transform(p3));
     if(qline)
         ((QGraphicsItemGroup*)qgi)->addToGroup(qline);

     if(qgi)
     {
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
            qgi = 0;
         }
         else
            vs->getScene()->addItem(qgi);
     }
}

#else
void Light::draw(int drawmode,Transform *t,View3dSurface *vs)
{
 Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
 double rsize;
 int i;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  if (t == NULL)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  xaxis = vs->getuaxis();
  yaxis = vs->getvaxis();
  rsize = 3.0 * ((vs->getumax() - vs->getumin()) / vs->getwidthmm());
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       if (t == 0)
         vs->line3d(origin + v1 * 0.6,origin + v1);
       else
         vs->line3d(t->transform(origin + v1 * 0.6),t->transform(origin + v1));
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       if (t == 0)
         vs->line3d(origin,p2);
       else
         vs->line3d(t->transform(origin),t->transform(p2));
       if (i == 0)
         p3 = p2;
       else
         {  if (t == 0)
              vs->line3d(p1,p2);
            else
              vs->line3d(t->transform(p1),t->transform(p2));
         }
       p1 = p2;
    }
  if (t == 0)
    vs->line3d(p1,p3);
  else
    vs->line3d(t->transform(p1),t->transform(p3));
}
#endif

#ifdef USING_WIDGETS
int Light::hideGL(HideImage *image)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
 double rsize;
 int i;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  xaxis = image->getsurface()->getuaxis();
  yaxis = image->getsurface()->getvaxis();
  rsize = 3.0 * ((image->getsurface()->getumax() - image->getsurface()->getumin()) / image->getsurface()->getwidthmm());
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       image->hideGL(this,origin + v1 * 0.6,origin + v1);
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       image->hideGL(this,origin,p2);
       if (i == 0)
         p3 = p2;
       else
         image->hideGL(this,p1,p2);
       p1 = p2;
    }
  image->hideGL(this,p1,p3);
  return 1;
}
#endif

int Light::hide(HideImage *image)
{Point xaxis,yaxis,zaxis,za,v1,v2,p1,p2,p3;
 double rsize;
 int i;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  xaxis = image->getsurface()->getuaxis();
  yaxis = image->getsurface()->getvaxis();
  rsize = 3.0 * ((image->getsurface()->getumax() - image->getsurface()->getumin()) / image->getsurface()->getwidthmm());
  for (i = 0 ; i < 8 ; i++)
    {  v1 = xaxis * (i < 5 ? 2 - i : i - 6) +
            yaxis * (i < 3 ? i : (i < 7 ? 4 - i : i - 8));
       v1 = v1.normalize() * rsize;
       image->hide(this,origin + v1 * 0.6,origin + v1);
    }
  zaxis = (direction - origin).normalize();
  za.setxyz(1.0,0.0,0.0);
  v1 = za.cross(zaxis);
  za.setxyz(0.0,1.0,0.0);
  v2 = za.cross(zaxis);
  if (v1.length() > v2.length())
    xaxis = v1.normalize();
  else
    xaxis = v2.normalize();
  yaxis = xaxis.cross(zaxis);
  for (i = 0 ; i < 16 ; i++)
    {  v1 = (xaxis * (i < 9 ? 4 - i : i - 12) + yaxis * (i < 5 ? i : (i < 13 ? 8 - i : i - 16))).normalize();
       p2 = origin + zaxis * size + v1 * size * tan(coneangle / 180.0 * M_PI);
       image->hide(this,origin,p2);
       if (i == 0)
         p3 = p2;
       else
         image->hide(this,p1,p2);
       p1 = p2;
    }
  image->hide(this,p1,p3);
  return 1;
}

int Light::npoints(void)
{ return 2;
}

Point Light::point(int index)
{ if (index == 0)
    return origin;
  else
    return direction;
}

Point Light::nearp(Point)
{Line line(origin,direction);
  return line.nearp(origin);
}

int Light::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
{ if (! isvisible(0)) return 1;
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

int Light::move(Database *db,Transform &t,int erase,int save)
{Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (db != 0)
    { 
	  if (save)
		db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  origin = t.transform(origin);
  direction = t.transform(direction);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 0;
}

Entity *Light::clone(Transform &t)
{ if (copye != NULL) return copye;
  copye = new Light(*this,t.transform(origin),t.transform(direction),intensity,size,fadedistance,fadeexponent,
                     coneangle,concentration,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int Light::project(Database *db,Transform &t,int)
{  return db->geometry.add(clone(t));
}

void Light::change(const BitMask &change,const EntityHeader &eh,double i,double s,
                   double fd,double fe,double ca,double c,const BitMask &op)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4))
    intensity = i;
  if (change.test(5))
    options.set(0,op.test(0));
  if (change.test(6))
    options.set(1,op.test(1));
  if (change.test(7))
    size = s;
  if (change.test(8))
    options.set(2,op.test(2));
  if (change.test(9))
    fadedistance = fd;
  if (change.test(10))
    fadeexponent = fe;
  if (change.test(11))
    options.set(3,op.test(3));
  if (change.test(12))
    coneangle = ca;
  if (change.test(13))
    concentration = c;
  draw(DM_NORMAL);
}

class LightVerifyButtonControl : public ButtonDialogControl
  {public:
     LightVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int LightVerifyButtonControl::select(Dialog *dialog)
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

class LightCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
    LightCheckBoxDialogControl(int id,BitMask *o,int b) : CheckBoxDialogControl(id,o,b)  {}
    void changefocus(Dialog *,int updatescreen = 1);
    void load(Dialog *);
  };

void LightCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  changefocus(dialog);
}

void LightCheckBoxDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int i,distant;
 static int fade[22] = { 107,108,109,110,111,112,
                         113 };
  dialog->currentvalue(id,&distant);
  for (i = 0 ; i < 22 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),fade[i]),! distant);
}

class LightTriStateCheckBoxDialogControl : public TriStateCheckBoxDialogControl
  {public:
    LightTriStateCheckBoxDialogControl(int i,int *v,bool t=true,int d=0) : TriStateCheckBoxDialogControl(i,v,t,d)  {}
    void changefocus(Dialog *,int updatescreen = 1);
    void load(Dialog *);
  };

void LightTriStateCheckBoxDialogControl::load(Dialog *dialog)
{
  //CheckBoxDialogControl::load(dialog);
  TriStateCheckBoxDialogControl::load(dialog);
  changefocus(dialog);
}

void LightTriStateCheckBoxDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int i,distant;
 static int fade[22] = { 107,108,109,110,111,112,
                         113 };
  dialog->currentvalue(id,&distant);
  for (i = 0 ; i < 22 ; i++)
    EnableWindow(GetDlgItem(dialog->gethdlg(),fade[i]),! distant);
}

void LightVerifyDisplayControl::draw(Dialog *dialog, RCHDC hd)
{
    //if(hWnd == 0)
    //    return;

    if(suninfo ==0)
        return;

    ((LightVerify_Dialog*)dialog->gethdlg())->suninfo = suninfo;
    ((LightVerify_Dialog*)dialog->gethdlg())->draw();
}

int Light::setup(BitMask *change,double *i,double *s,double *fd,double *fe,double *ca,double *c,BitMask *o,EntityHeader *header, sunlightInfo *sundata)
{Dialog dialog("LightVerify_Dialog");
 DialogControl *dc;
 ResourceString rs8(NCRAY+8);
 int j,status,co,l,st,defColour,defLayer,defStyle;
 double w,defWeight;
 double defi,defs,deffd,deffe,defca,defc;
 BitMask defOptions(32),changeIn(32);
 int tristates[] = { 1,1,1,1 };

 int changecount = v.getinteger("op::changecount");

 EntityHeader entheader;
 if(header != 0)
     header->verify(&dialog,&co,&l,&st,&w);
 else
     entheader.verify(&dialog,&co,&l,&st,&w);

 defColour = db.header.getcolour();
 defLayer  = db.header.getlayer();
 defStyle  = db.header.getstyle();
 defWeight = db.lineweights.weighttomm(db.header.getweight());
 Light::getdefaults(&defi,&defs,&deffd,&deffe,&defca,&defc,&defOptions);


 // look at the change bits and see what value vary among any pre-slected entities
 if(change != NULL)
 {
     // verify mode

     // if test is true the values dont vary ie. the dialog will show the incoming values
     // otherwise false ie. the values vary so the dialog will show the *Varies* message
     // the entity values will not be modified if the dialog values are not changed
     //
     // here we are setting the controls to there default settings
     // text boxes and combo boxes will show *Varies*
     // check boxes will be in tri state mode
     if(!change->test(0))
         co = 0;
     if(!change->test(1))
         l = 0;
     if(!change->test(2))
         st = 0;
     if(!change->test(3))
         w = 0.10;

     // intensity
     if(!change->test(4))
         *i=0.0;
     // size
     if(!change->test(7))
         *s=0.0;
     // fadedistance
     if(!change->test(9))
         *fd=0.0;
     // fadeexponent
     if(!change->test(10))
        *fe=0.0;
     // coneangle
     if(!change->test(12))
         *ca=0.0;
     // concentration
     if(!change->test(13))
         *c=0.0;

     // set the options for the tristate check boxes
     // distant light
     if(change->test(5))
         o->test(0)==0 ? tristates[0] = 0 : tristates[0] = 2;
     // white light
     if(change->test(6))
         o->test(1)==0 ? tristates[1] = 0 : tristates[1] = 2;
     // enable fade
     if(change->test(8))
         o->test(2)==0 ? tristates[2] = 0 : tristates[2] = 2;
     // enable spot
     if(change->test(11))
         o->test(3)==0 ? tristates[3] = 0 : tristates[3] = 2;

 }
 else
 {
     // options mode
     *i = v.getreal("li::intensity");
     *s = v.getreal("li::size");
     *fd = v.getreal("li::fadedistance");
     *fe = v.getreal("li::fadeexponent");
     *ca = v.getreal("li::coneangle");
     *c = v.getreal("li::concentration");
     *o = *v.getbm("li::options");

     defOptions.test(5)==0  ? tristates[0] = 0 : tristates[0] = 1;
     defOptions.test(6)==0  ? tristates[1] = 0 : tristates[1] = 1;
     defOptions.test(8)==0  ? tristates[2] = 0 : tristates[2] = 1;
     defOptions.test(11)==0  ? tristates[3] = 0 : tristates[3] = 1;
 }

 dialog.add(dc = new RealDialogControl(104,i,0.1));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 //dialog.add(new LightCheckBoxDialogControl(105,o,0));
 //dialog.add(new CheckBoxDialogControl(106,o,1));
 dialog.add(new RealDialogControl(107,s,1.0,0.0,1.0E30,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1103,dc));
 //dialog.add(new CheckBoxDialogControl(108,o,2));
 dialog.add(new RealDialogControl(109,fd,1.0,0.0,1.0E30,ScaleByLength));
 //dialog.add(new ScrollBarDialogControl(1105,dc));
 dialog.add(new RealDialogControl(110,fe,1.0,0.0,1000.0));
 //dialog.add(new ScrollBarDialogControl(1106,dc));
 //dialog.add(new CheckBoxDialogControl(111,o,3));
 dialog.add(new RealDialogControl(112,ca,1.0,0.0,180.0));
 //dialog.add(new ScrollBarDialogControl(1108,dc));
 dialog.add(new RealDialogControl(113,c,1.0,0.0,1000.0));
 //dialog.add(new ScrollBarDialogControl(1109,dc));

 if(sundata != 0)
 {
     dialog.add((DisplayDialogControl*) new LightVerifyDisplayControl(2000,sundata));
 }

 // look at the change bits and see what value vary among any pre-slected entities
 if(change != NULL)
 {
     // if test is true the values dont vary ie. the dialog will show the incoming values
     // otherwise false ie. the values vary so the dialog will show the *Varies* message
     // the entity values will not be modified if the dialog values are not changed
     //
     // here we are setting the controls to there default settings
     // text boxes and combo boxes will show *Varies*
     // check boxes will be in tri state mode

     if(!change->test(0))
         co = 0;
     if(!change->test(1))
         l = 0;
     if(!change->test(2))
         st = 0;
     if(!change->test(3))
         w = 0.10;

     if(changecount != 0)
     {
         dialog.add(new LightVerifyButtonControl(1000));
         dialog.add(new LightVerifyButtonControl(1001));
         dialog.add(new LightVerifyButtonControl(1002));
         dialog.add(new LightVerifyButtonControl(1003));
     }

     if(changecount > 1 && !change->test(0)) // colour
     {
         ((IntegerDialogControl*)dialog.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(100))->setDefaultValue(defColour);
     }
     if(changecount > 1 && !change->test(1)) // layer
     {
         ((IntegerDialogControl*)dialog.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(101))->setDefaultValue(defLayer);
     }
     if(changecount > 1 && !change->test(2)) // style
     {
         ((IntegerDialogControl*)dialog.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(102))->setDefaultValue(defStyle);
     }
     if(changecount > 1 && !change->test(3)) // weight
     {
         ((RealDialogControl*)dialog.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(103))->setDefaultValue(defWeight);
     }

     // intensity
     if(changecount > 1 && !change->test(4))
     {
         ((RealDialogControl*)dialog.getcontrol(104))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(104))->setDefaultValue(defi);
     }
     // size
     if(changecount > 1 && !change->test(7))
     {
         ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defs);
     }
     // fadedistance
     if(changecount > 1 && !change->test(9))
     {
         ((RealDialogControl*)dialog.getcontrol(109))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(109))->setDefaultValue(deffd);
     }
     // fadeexponent
     if(changecount > 1 && !change->test(10))
     {
         ((RealDialogControl*)dialog.getcontrol(110))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(110))->setDefaultValue(deffe);
     }
     // coneangle
     if(changecount > 1 && !change->test(12))
     {
         ((RealDialogControl*)dialog.getcontrol(112))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(112))->setDefaultValue(defca);
     }
     // concentration
     if(changecount > 1 && !change->test(13))
     {
         ((RealDialogControl*)dialog.getcontrol(113))->setSpecialValueText(_RCT("*Varies*"));
         ((RealDialogControl*)dialog.getcontrol(113))->setDefaultValue(defc);
     }

     // make these into tristate check boxes
     dialog.add(new LightTriStateCheckBoxDialogControl(105,&tristates[0],changecount>=1^change->test(5),defOptions.test(5)));
     dialog.add(new TriStateCheckBoxDialogControl(106,&tristates[1],changecount>=1^change->test(6),defOptions.test(6)));
     dialog.add(new TriStateCheckBoxDialogControl(108,&tristates[2],changecount>=1^change->test(8),defOptions.test(8)));
     dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[3],changecount>=1^change->test(11),defOptions.test(11)));
 }
 else
 {
     // these can be two state check boxes
     dialog.add(new LightCheckBoxDialogControl(105,o,0));
     dialog.add(new CheckBoxDialogControl(106,o,1));
     dialog.add(new CheckBoxDialogControl(108,o,2));
     dialog.add(new CheckBoxDialogControl(111,o,3));
 }

 // set the change value for the dialog to read when it opens
 //dialog.title(rs2.gets());
 v.setinteger("op::change",0);
 v.setbm("op::changemask",defOptions);
 if (change != NULL)
 {
      // set the change value for the dialog to read when it opens
      v.setinteger("op::change",1);
      v.setbm("op::changemask",*change);
      if(changecount == 1)
         dialog.title(rs8.gets());
      else
        dialog.title(_RCT("Light Options"));
 }

 status = dialog.process();
 if (status == TRUE)
 {
     if (change != NULL)
     {
         changeIn = *change;
         change->clearall();

         if(changecount == 1)
         {
             for (int i = 0 ; i < 4 ; i++)
               change->set(i,1);

             header->setcolour(Byte(co));
             header->setstyle(Byte(st));
             header->setlayer(Byte(l));
             header->setweight(Byte(db.lineweights.mmtoweight(w)));
         }
         // colour
         if( changecount > 1 && !changeIn.test(0) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(100))->getSpecialValueText()) != 0 )
             change->clear(0);
         else
         {
             change->set(0);
             header->setcolour(Byte(co));
         }
         // layer
         if( changecount > 1 && !changeIn.test(1) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(1);
         else
         {
             change->set(1);
             header->setlayer(Byte(l));
         }
         // style
         if( changecount > 1 && !changeIn.test(2) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(2);
         else
         {
             change->set(2);
             header->setstyle(Byte(st));
         }
         // weight
         if( changecount > 1 && !changeIn.test(3) &&
             strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(3);
         else
         {
             change->set(3);
             header->setweight(Byte(db.lineweights.mmtoweight(w)));
         }
         // distant light
         if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(105))->isTriState())
             change->clear(5);
         else
         {
             change->set(5);
             o->set(0, tristates[0]);
         }
         // white light
         if(tristates[1] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(106))->isTriState())
             change->clear(6);
         else
         {
             change->set(6);
             o->set(1, tristates[1]);
         }
         // enable fade
         if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(108))->isTriState())
             change->clear(8);
         else
         {
             change->set(8);
             o->set(2, tristates[2]);
         }
         // enable spot
         if(tristates[3] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(111))->isTriState())
             change->clear(11);
         else
         {
             change->set(11);
             o->set(3, tristates[3]);
         }
         // intensity
         if( changecount > 1 && !changeIn.test(4) &&
             strlen(((RealDialogControl*)dialog.getcontrol(104))->getSpecialValueText()) != 0 ) // not changed
             change->clear(4);
         else
             change->set(4);
         // size
         if( changecount > 1 && !changeIn.test(7) &&
             strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0 ) // not changed
             change->clear(7);
         else
             change->set(7);
         // fade distance
         if( changecount > 1 && !changeIn.test(9) &&
             strlen(((RealDialogControl*)dialog.getcontrol(109))->getSpecialValueText()) != 0 ) // not changed
             change->clear(9);
         else
             change->set(9);
         // exponent
         if( changecount > 1 && !changeIn.test(10) &&
             strlen(((RealDialogControl*)dialog.getcontrol(110))->getSpecialValueText()) != 0 ) // not changed
             change->clear(10);
         else
             change->set(10);
         // cone angle
         if( changecount > 1 && !changeIn.test(12) &&
             strlen(((RealDialogControl*)dialog.getcontrol(112))->getSpecialValueText()) != 0 ) // not changed
             change->clear(12);
         else
             change->set(12);
         // concentration
         if( changecount > 1 && !changeIn.test(13) &&
             strlen(((RealDialogControl*)dialog.getcontrol(113))->getSpecialValueText()) != 0 ) // not changed
             change->clear(13);
         else
             change->set(13);
     }
     else
     {
         //o->set(0,tristates[0]);
         //o->set(1,tristates[1]);
         //o->set(2,tristates[2]);
         //o->set(3,tristates[3]);

        v.setreal("li::intensity",*i);
        v.setreal("li::size",*s);
        v.setreal("li::fadedistance",*fd);
        v.setreal("li::fadeexponent",*fe);
        v.setreal("li::concentration",*c);
        v.setreal("li::coneangle",*ca);
        v.setbm("li::options",*o);
     }
  }
  return status != FALSE;
}

void Light::getdefaults(double *i,double *s,double *fd,double *fe,double *ca,double *c,BitMask *o)
{ *i = v.getreal("li::intensity");
  *s = v.getreal("li::size");
  *fd = v.getreal("li::fadedistance");
  *fe = v.getreal("li::fadeexponent");
  *ca = v.getreal("li::coneangle");
  *c = v.getreal("li::concentration");
  *o = *v.getbm("li::options");
}

void Light::setdefaults(double i,double s,double fd,double fe,double ca,double c,const BitMask &o)
{ v.setreal("li::intensity",i);
  v.setreal("li::size",s);
  v.setreal("li::fadedistance",fd);
  v.setreal("li::fadeexponent",fe);
  v.setreal("li::coneangle",ca);
  v.setreal("li::concentration",c);
  v.setbm("li::options",o);
}

int Light::save(CadFile *outfile)
{short type;
  type = light_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! direction.save(outfile)) return 0;
  if (! outfile->write(&intensity)) return 0;
  if (! outfile->write(&size)) return 0;
  if (! outfile->write(&fadedistance)) return 0;
  if (! outfile->write(&fadeexponent)) return 0;
  if (! outfile->write(&coneangle)) return 0;
  if (! outfile->write(&concentration)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

Entity *Light::load(int,CadFile *infile)
{Light *light;
 EntityHeader *header;
 Point origin,direction;
 double intensity,size,fadedistance,fadeexponent,coneangle,concentration;
 BitMask options(32);
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! origin.load(infile)) return 0;
  if (! direction.load(infile)) return 0;
  if (! infile->read(&intensity)) return 0;
  if (! infile->read(&size)) return 0;
  if (! infile->read(&fadedistance)) return 0;
  if (! infile->read(&fadeexponent)) return 0;
  if (! infile->read(&coneangle)) return 0;
  if (! infile->read(&concentration)) return 0;
  if (! infile->read(&options)) return 0;
  light = new Light(*header,origin,direction,intensity,size,fadedistance,fadeexponent,coneangle,concentration,options);
  light->atlist = header->getatlist();
  delete header;
  return light;
}

int Light::save(UndoFile *outfile)
{short type;
  type = light_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

Entity *Light::load(UndoFile *infile)
{Light *light;
 char bm[sizeof(BitMask)];
  if ((light = new Light) != NULL)
    {AttributeList al;
       memcpy(bm,&light->visible,sizeof(bm));
       if (! infile->blockread(light,sizeof(*light))) return 0;
       memcpy(&light->visible,bm,sizeof(bm));
       if (! infile->read(&light->visible)) return 0;
       if (! al.load(infile)) return 0;   light->atlist = al;
       if (! infile->read(&light->options)) return 0;
    }
  return light;
}

int Light::saveattributes(CadFile *outfile)
{ if (! outfile->write(&intensity)) return 0;
  if (! outfile->write(&fadedistance)) return 0;
  if (! outfile->write(&fadeexponent)) return 0;
  if (! outfile->write(&coneangle)) return 0;
  if (! outfile->write(&concentration)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

int Light::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&intensity)) return 0;
  if (! infile->read(&fadedistance)) return 0;
  if (! infile->read(&fadeexponent)) return 0;
  if (! infile->read(&coneangle)) return 0;
  if (! infile->read(&concentration)) return 0;
  if (! infile->read(&options)) return 0;
  return 1;
}

int Light::savedxf(int,DXFOut *)
{  return 1;
}

int Light::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int Light::savegeometry(CadFile *outfile)
{ if (! origin.save(outfile)) return 0;
  if (! direction.save(outfile)) return 0;
  return 1;
}

int Light::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! origin.load(infile)) return 0;
  if (! direction.load(infile)) return 0;
  return 1;
}

void Light::verify(void)
{
#if 1
    int state;
    void *data;
    raytrace_command2(&state,NULL,&data);
#else
 Dialog dialog("Lightverify_Dialog");
 int c,l,s,state;
 double w,i,si,fd,fe,ca,co;
 BitMask o(32);
 void *data;

  i = intensity;
  si = size;
  fd = fadedistance;
  fe = fadeexponent;
  ca = coneangle;
  co = concentration;
  o = options;

  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new LightVerifyButtonControl(1000));
  dialog.add(new LightVerifyButtonControl(1001));
  dialog.add(new LightVerifyButtonControl(1002));
  dialog.add(new LightVerifyButtonControl(1003));

  dialog.add(new RealDialogControl(104,&i));
  dialog.add(new CheckBoxDialogControl(105,&o,0));
  dialog.add(new CheckBoxDialogControl(106,&o,1));
  dialog.add(new RealDialogControl(107,&si,ScaleByLength));
  dialog.add(new CheckBoxDialogControl(108,&o,2));
  dialog.add(new RealDialogControl(109,&fd,ScaleByLength));
  dialog.add(new RealDialogControl(110,&fe));
  dialog.add(new CheckBoxDialogControl(111,&o,3));
  dialog.add(new RealDialogControl(112,&ca));
  dialog.add(new RealDialogControl(113,&co));
  dialog.add(new ButtonDialogControl(114));
  //dialog.setposition(DPOK);
  if (dialog.process() == TRUE)
  {
      //raytrace_command2(&state,NULL,&data);
      v.setreal("li::intensity",i);
      v.setreal("li::size",s);
      v.setreal("li::fadedistance",fd);
      v.setreal("li::exponent",fe);
      v.setreal("li::concentration",c);
      v.setreal("li::coneangle",ca);
      v.setbm("li::options",o);
  }
#endif
}

void Light::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{Point p3;
  if (vs == NULL)
    {  *pmin = *pmax = origin;
       p3 = direction;
    }
  else
    {  *pmin = *pmax = vs->modeltouv(origin);
       p3 = vs->modeltouv(direction);
    }
  if (p3.x < pmin->x) pmin->x = p3.x;
  if (p3.y < pmin->y) pmin->y = p3.y;
  if (p3.z < pmin->z) pmin->z = p3.z;
  if (p3.x > pmax->x) pmax->x = p3.x;
  if (p3.y > pmax->y) pmax->y = p3.y;
  if (p3.z > pmax->z) pmax->z = p3.z;
}

int Light::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(figure_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

void Light::copyinfo(LightInfo *li)
{ li->origin = origin;
  li->direction = (direction - origin).normalize();
  li->distant = options.test(0);
  li->fade = options.test(2);
  li->cone = options.test(3);
  if (options.test(1))
    li->colour.red = li->colour.green = li->colour.blue = 1.0;
  else
    {  li->colour.red = db.colourtable[colour].red / 255.0;
       li->colour.green = db.colourtable[colour].green / 255.0;
       li->colour.blue = db.colourtable[colour].blue / 255.0;
    }
  li->colour.red *= intensity;
  li->colour.green *= intensity;
  li->colour.blue *= intensity;
  li->intensity = intensity;
  li->fadedistance = fadedistance;
  li->fadeexponent = fadeexponent;
  li->coneangle = cos(coneangle * M_PI / 180.0);
  li->concentration = concentration;
}

int Light::explode(void)
{ return 0;
}

