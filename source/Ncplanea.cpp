
#include "ncwin.h"
#include <QDir>
#include <QList>
#include "ncdialog.h"
#include "RCView.h"
#include "RCGraphicsItems.h"
#include "selecthatchpattern_dialog.h"
#include "bitmaptransparency_dialog.h"
#include "RCFigureBrowser.h"
#include "planeverify_dialog.h"
#include "ptin_poly.h"

extern void drawarrow(const Point lastp,const Point p, QGraphicsItemGroup *arrows);
extern double AngleBetween(const ::Point& v1,const ::Point& v2,const ::Point& vz);

QPixmap createTranspBackTexture(QSize size, QColor foreColour, QColor backColour,int style=0)
{
    // single colour style
    if(style == 1)
    {
        QPixmap pixmap(size.width(),size.height());
        QPainter painter(&pixmap);

        QBrush backBrush(backColour,Qt::SolidPattern);

        painter.begin(&pixmap);
        painter.setPen(backColour);
        painter.setBrush(backBrush);
        painter.drawRect(0,0,pixmap.width(),pixmap.height());
        painter.end();

        return pixmap;
    }
    // half and half style
    else if (style == 2)
    {
        QPixmap pixmap(size.width(),size.height());
        QPainter painter(&pixmap);

        QBrush foreBrush(foreColour,Qt::SolidPattern);
        QBrush backBrush(backColour,Qt::SolidPattern);

        painter.begin(&pixmap);
        painter.setPen(backColour);
        painter.setBrush(backBrush);
        painter.drawRect(0,0,pixmap.width()/2,pixmap.height());
        painter.setPen(foreColour);
        painter.setBrush(foreBrush);
        painter.drawRect(pixmap.width()/2,0,pixmap.width()/2,pixmap.height());
        painter.end();

        return pixmap;
    }
    // default
    // checkerboard style
    QPixmap pixmap(size.width()/2,size.height()/2);
    QPainter painter(&pixmap);

    QBrush foreBrush(foreColour,Qt::SolidPattern);
    QBrush backBrush(backColour,Qt::SolidPattern);

    painter.begin(&pixmap);
    painter.setPen(foreColour);
    painter.setBrush(foreBrush);
    painter.drawRect(0,0,pixmap.width()/2,pixmap.height()/2);
    painter.setPen(backColour);
    painter.setBrush(backBrush);
    painter.drawRect(pixmap.width()/2,0,pixmap.width()/2,pixmap.height()/2);
    painter.drawRect(0,pixmap.height()/2,pixmap.width()/2,pixmap.height()/2);
    painter.setPen(foreColour);
    painter.setBrush(foreBrush);
    painter.drawRect(pixmap.width()/2,pixmap.height()/2,pixmap.width()/2,pixmap.height()/2);
    painter.end();

    return pixmap;
}

// get the number of drawing segments in a given entity at the current settings
int getNumDrawSegments(Entity *ent,View3dSurface *vs )
{

    if(ent->type() == line_entity)
    {
        return 2;
    }
    else if(ent->type() == circle_entity)
    {
        int n = 0;
        n = ((Circle*)ent)->nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
        if(n < 1)
          n = 1;
        else if(n == 1)
            n = 2;
        return n;
    }
    else if(ent->type() == ellipse_entity)
    {
        int n = 0;
        n = ((EllipseE*)ent)->nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
        return n;
    }
    else if(ent->type() == curve_entity)
    {
        int n = 0;
        n = ((Curve*)ent)->nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
        return n;
    }
    return 0;
}

double toCurrentUnits(int currentUnits, int patternUnits)
{
    double currentFactor = 1.0;
    double patternFactor = 1.0;
    switch(currentUnits)
    {
    case 0 : // mm
        currentFactor = 1.0;
        break;
    case 1 : // m
        currentFactor = 1000.0;
        break;
    case 2 : // in
        currentFactor = 25.4;
        break;
    case 3 : // ft/in
        currentFactor = 12.0 * 25.4;
        break;
    case 4 : // "",' (ft,in)
        currentFactor = 12.0 * 25.4;
        break;
    case 5 : // cm
        currentFactor = 10.0;
        break;
    }
    switch(patternUnits)
    {
    case 0 : // mm
        patternFactor = 1.0;
        break;
    case 1 : // m
        patternFactor = 1000.0;
        break;
    case 2 : // in
        patternFactor = 25.4;
        break;
    case 3 : // ft/in
        patternFactor = 12.0 * 25.4;
        break;
    case 4 : // "",' (ft,in)
        patternFactor = 12.0 * 25.4;
        break;
    case 5 : // cm
        patternFactor = 10.0;
        break;
    }

    return patternFactor / currentFactor;
}

Plane::Plane(EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgplane=0;
    qgrenderplane=0;
#endif
    Point zaxis;
 double D;
  list = l;  options = o;  xspacing = xs;  yspacing = ys;  angle = a;  crossangle = ca;
  anchor.setxyz(0,0,0);
  transparency = trans;
  red = tr;
  green = tg;
  blue = tb;
  transparencyrange = trange;
  gradientcolour = gc;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    {  patternfilename = new RCCHAR[strlen(filename)+1];
       strcpy(patternfilename,filename);
       if (fabs(xspacing)< 1.0E-20) 
         xspacing = 1.0;
       if (fabs(yspacing)< 1.0E-20) 
         yspacing = 1.0;
    }
  else
    patternfilename = 0;

  Loops loops(1,list);
  if (! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
    defined = 0;
  drawnhighlighted = 0;

#ifdef USING_WIDGETS
  // cache the bitmap
  bitmape = 0;
  if (patternfilename != 0 && options.test(PlaneBitmapFilled))
  {
      Point xa,ya;
      Transform t1;
      if (options.test(1))
      {
           t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
           xa = t1.transform(xaxis);
           ya = t1.transform(yaxis);
      }
      else
      {
          xa = xaxis;
          ya = yaxis;
      }
      bitmape = new BitMapE(patternfilename,origin,
                            xa,ya,xspacing,yspacing,
                            red,green,blue,
                            transparencyrange,1.0-transparency,options);

  }
#endif

  if (defined && transparency != 0.0)
    db.setrequiresopengl(1);

}

Plane::Plane(const EntityHeader &header,EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o) :
  SurfaceEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgplane=0;
    qgrenderplane=0;
#endif

    Point zaxis;
 double D;
 
  list = l;  options = o;  xspacing = xs;  yspacing = ys;  angle = a;  crossangle = ca;
  anchor.setxyz(0,0,0);
  transparency = trans;
  red = tr;
  green = tg;
  blue = tb;
  transparencyrange = trange;
  gradientcolour = gc;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    {  patternfilename = new RCCHAR[strlen(filename)+1];
       strcpy(patternfilename,filename);
       if (fabs(xspacing)< 1.0E-20) 
         xspacing = 1.0;
       if (fabs(yspacing)< 1.0E-20) 
         yspacing = 1.0;
    }
  else
    patternfilename = 0;

  Loops loops(1,list);
  if (! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
    defined = 0;
  drawnhighlighted = 0;

#ifdef USING_WIDGETS
  // cache the bitmap
  bitmape = 0;
  if (patternfilename != 0 && options.test(PlaneBitmapFilled))
  {
      Point xa,ya;
      Transform t1;
      if (options.test(1))
      {
           t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
           xa = t1.transform(xaxis);
           ya = t1.transform(yaxis);
      }
      else
      {
          xa = xaxis;
          ya = yaxis;
      }
      bitmape = new BitMapE(patternfilename,origin,
                            xa,ya,xspacing,yspacing,
                            red,green,blue,
                            transparencyrange,1.0-transparency,options);

  }
#endif

  if (defined && transparency != 0.0)
    db.setrequiresopengl(1);

}

Plane::Plane(Point org,Point xa,Point ya,EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgplane=0;
    qgrenderplane=0;
#endif

    origin = org;  xaxis = xa;  yaxis = ya;
    anchor.setxyz(0,0,0);
  list = l;  options = o;
  xspacing = xs;  yspacing = ys;  angle = a;  crossangle = ca;
  transparency = trans;
  red = tr;
  green = tg;
  blue = tb;
  transparencyrange = trange;
  gradientcolour = gc;
  defined = 1;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    {  patternfilename = new RCCHAR[strlen(filename)+1];
       strcpy(patternfilename,filename);
       if (fabs(xspacing)< 1.0E-20) 
         xspacing = 1.0;
       if (fabs(yspacing)< 1.0E-20) 
         yspacing = 1.0;
    }
  else
    patternfilename = 0;

#ifdef USING_WIDGETS
  // cache the bitmap
  bitmape = 0;
  if (patternfilename != 0 && options.test(PlaneBitmapFilled))
  {
      Point xa,ya;
      Transform t1;
      if (options.test(1))
      {
           t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
           xa = t1.transform(xaxis);
           ya = t1.transform(yaxis);
      }
      else
      {
          xa = xaxis;
          ya = yaxis;
      }
      bitmape = new BitMapE(patternfilename,origin,
                            xa,ya,xspacing,yspacing,
                            red,green,blue,
                            transparencyrange,1.0-transparency,options);

  }
#endif

  if (defined && transparency != 0.0)
    db.setrequiresopengl(1);

}

Plane::Plane(const EntityHeader &header,Point org,Point xa,Point ya,EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o) :
  SurfaceEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgplane=0;
    qgrenderplane=0;
#endif

  origin = org;  xaxis = xa;  yaxis = ya;
  anchor.setxyz(0,0,0);
  list = l;  options = o;
  xspacing = xs;  yspacing = ys;  angle = a;  crossangle = ca;
  transparency = trans;
  red = tr;
  green = tg;
  blue = tb;
  transparencyrange = trange;
  gradientcolour = gc;
  defined = 1;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    {  patternfilename = new RCCHAR[strlen(filename)+1];
       strcpy(patternfilename,filename);
       if (fabs(xspacing)< 1.0E-20) 
         xspacing = 1.0;
       if (fabs(yspacing)< 1.0E-20) 
         yspacing = 1.0;
    }
  else
    patternfilename = 0;
  drawnhighlighted = 0;

#ifdef USING_WIDGETS
  // cache the bitmap
  bitmape = 0;
  if (patternfilename != 0 && options.test(PlaneBitmapFilled))
  {
      Point xa,ya;
      Transform t1;
      if (options.test(1))
      {
           t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
           xa = t1.transform(xaxis);
           ya = t1.transform(yaxis);
      }
      else
      {
          xa = xaxis;
          ya = yaxis;
      }
      bitmape = new BitMapE(patternfilename,origin,
                            xa,ya,xspacing,yspacing,
                            red,green,blue,
                            transparencyrange,1.0-transparency,options);

  }
#endif
  if (defined && transparency != 0.0)
    db.setrequiresopengl(1);

}

Plane::Plane(const EntityHeader &header,Point anc,Point org,Point xa,Point ya,EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
{
    Plane(header,org,xa,ya,l,xs,ys,a,ca,trans,tr,tg,tb,trange,gc,filename,o);
    anchor = anc;
}

Plane::Plane(Point anc,Point org,Point xa,Point ya,EntityList l,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
{
    Plane(org,xa,ya,l,xs,ys,a,ca,trans,tr,tg,tb,trange,gc,filename,o);
    anchor = anc;
}

void Plane::dbinsert(Database *db,int draw)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (db != NULL && e->getposition() == NULL) 
              db->geometry.add(e,draw);
            e->adda(this);
         }
    }

  Entity::dbinsert(db,draw);

}

void Plane::dbdelete(void)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         e->dela(this);
    }

  Entity::dbdelete();

}

void Plane::dbsetassociated(void)
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         e->setstatus(1);
    }
}

void Plane::dbtraverseassociated(EntityCallBack c,void *d)
{ for (list.start() ; ! list.atend() ; )
    c(list.next(),d);
}

Plane::~Plane()
{Entity *e;
  for (list.start() ; ! list.atend() ; )
    {  if ((e = list.next()) != NULL && e->getposition() == NULL)
         delete e;
    }
  delete [] triangles;
  triangles=0;
  ntriangles=-1;
  list.destroy();
  delete [] patternfilename;
  patternfilename = 0;

  delete bitmape;
  bitmape = 0;
  qgplane=0;
}

struct CBpickline_data
  {View3dSurface *surface;
   double dmin;
   double x,y;
   Point *pos;
  };

void pickline(Point p1,Point p2,void *data)
{CBpickline_data *pldata = (CBpickline_data *) data;
 double d;
  d = pldata->surface->pick(p1,p2,pldata->x,pldata->y,pldata->pos);
  if (d >= 0.0 && (pldata->dmin < -0.5 || d < pldata->dmin))
    pldata->dmin = d;
}

double Plane::pick(View3dSurface *surface,double x,double y,Point *pos)
{Entity *e;
 CBpickline_data pldata;
 int i,n;
 double a,len,d,dmin;
 Point p1,n1,dir1,o;
 Polygon3d polygon3d(origin,xaxis,yaxis,list,0,
                    (surface->getumax() - surface->getumin()) / surface->getwidth());
 View3dWindow *window;
  
    window = dynamic_cast<View3dWindow *>(surface);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        surface = window->getoffscreenbitmap();
 
    if (options.test(PlaneNoFill))
    {   dmin = -1.0;
        for (list.start() ;  ! list.atend() ; )
        {   if ((e = list.next()) != NULL)
            {   if ((d = e->pick(surface,x,y,pos)) >= 0.0 && (d < dmin || dmin == -1.0 && d >= 0.0))
                    dmin = d;
            } 
        }
        return dmin; 
    }
    else if (options.test(0))
        return polygon3d.pick(surface,x,y,pos);
    else if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled) || options.test(PlaneGradientFilled))
    {   if (pos == NULL && surface->displaylist.defined(this))
            return surface->displaylist.pick(this,x,y);
        else
            return polygon3d.pick(surface,x,y,pos);
    }
  else
    {  if (pos == NULL && surface->displaylist.defined(this))
         return surface->displaylist.pick(this,x,y);
       pldata.surface = surface;
       pldata.x = x;
       pldata.y = y;
       pldata.pos = pos;
       pldata.dmin = -1.0;
       if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
        for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return -1.0;
            n1 /= len;
            o = origin;
            if (options.test(PlaneAnchored))
              o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));
            p1 = o;
            n = 0;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,pickline,&pldata) >= 0)
              {  p1 += n1 * (i == 0 ? xspacing : yspacing);  n++;
              }
            p1 = o;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,pickline,&pldata) <= 0)
              {  p1 -= n1 * (i == 0 ? xspacing : yspacing);  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }
    }
  return pldata.dmin;
}

int Plane::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{Entity *e;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  if (! options.test(0) && vs->displaylist.defined(this) && vs->displaylist.pick(this,inside,x1,y1,x2,y2))
    return 1;
  for (list.start() ;  ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  if (! e->pick(vs,inside,x1,y1,x2,y2)) return 0;
         }
    }
  return 1;
}

int Plane::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Entity *e;
 int epicked,picked;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

  picked = inside != 2;

  for (list.start() ;  ! list.atend() ; )
    {  if ((e = list.next()) != NULL)
         {  epicked = e->pick(vs,nvertices,pwvertices,inside);
            if (inside == 2 && epicked)
              return 1;
            else if (inside != 2 && ! epicked)
              return 0;
         }
    }

  return picked;
}

struct CBdrawline_data
{
#ifdef USING_WIDGETS
   QGraphicsItem *qgi;
#endif
   View3dSurface *surface;
   Transform *transform;
   long count;
};

#ifdef USING_WIDGETS
static void drawlineGL(Point p1,Point p2,void *data)
{
    CBdrawline_data *dldata = (CBdrawline_data *) data;
    //if(dldata->surface != 0)
    //{
    //    p1 = dldata->surface->modeltransform.transform(p1);
    //    p2 = dldata->surface->modeltransform.transform(p2);
    //}
    dldata->surface->lineGL(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
}
#endif

static void drawline(Point p1,Point p2,void *data)
{
#ifdef USING_WIDGETS
    QGraphicsItem *qline=0;
    CBdrawline_data *dldata = (CBdrawline_data *) data;
    if(dldata->qgi)
    {
        qline = dldata->surface->line3d((QGraphicsItem*)0,p1,p2);
        if(qline)
            ((QGraphicsItemGroup*)dldata->qgi)->addToGroup(qline);
    }
    else
        dldata->surface->line3d(p1,p2);
#else
  CBdrawline_data *dldata = (CBdrawline_data *) data;
  dldata->surface->line3d(p1,p2);
#endif
}

int Plane::facet(const BitMask &foptions,double error,int nhor,int,View3dSurface *,PolygonCallBack cb,void *data)
{ if (foptions.test(0))
    {Face *face = new Face(origin,xaxis,yaxis,list,nhor,error);
      if (face == NULL || ! face->getdefined())
        {  delete face;  return 0;
        }
      else
        return cb((SurfaceEntity *)this,face,0,0,data);
    }
  else
    {Polygon3d *poly = new Polygon3d(origin,xaxis,yaxis,list,nhor,error);
      if (poly == NULL || ! poly->getdefined())
        {  delete poly;  return 0;
        }
      return cb((SurfaceEntity *)this,poly,0,0,data);
    }
  return 0;
}

//
//  Triangulate the plane
//
void Plane::triangulate(double error, Transform *t)
{Point *tr,n,uv;
 double umin,vmin,umax,vmax,u,v;
 int i,nhor,nver;
 SurfaceProperties sp,*tsp;

  nfacets(error,&nhor,&nver);
  Polygon3d poly(origin,xaxis,yaxis,list,nhor,error);
  if(t)
      poly.transform(t);
 
  n = poly.getxaxis().cross(poly.getyaxis());
  ntriangles = TriangulatePolygon(poly.getnloops(),poly.getnvert(),poly.getverts(),&tr);
  triangles = new Point[ntriangles*9+2];

  if (sp.copyinfo(this,0,0,0) != 0)
    {  tsp = new SurfaceProperties;
       *tsp = sp;
    }
  else
    tsp = 0;

  triangles[0].x = tsp != 0 ? (LONG_PTR)tsp + 0.1 : 0.1;
  triangles[0].y = tsp != 0 ? tsp->transparency : 0.0;
  triangles[0].z = RCDBL_MAX;
  triangles[1].x = db.colourtable[getcolour()].red;
  triangles[1].y = db.colourtable[getcolour()].green;
  triangles[1].z = db.colourtable[getcolour()].blue;
  
  umin = umax = poly.getxaxis().dot(tr[0] - poly.getorigin());
  vmin = vmax = poly.getyaxis().dot(tr[0] - poly.getorigin());

  for (i = 1 ; i < ntriangles * 3 ; i++)
    {  u = (poly.getxaxis().dot(tr[i] - poly.getorigin()));
       v = (poly.getyaxis().dot(tr[i] - poly.getorigin()));
       if (u < umin) umin = u; 
       if (v < vmin) vmin = v; 
       if (u > umax) umax = u; 
       if (v > vmax) vmax = v; 
    }

  for (i = 0 ; i < ntriangles ; i++)
    {  triangles[i * 9 + 2] = tr[i*3];
       triangles[i * 9 + 3] = tr[i*3+1];
       triangles[i * 9 + 4] = tr[i*3+2];
       triangles[i * 9 + 5] = n;
       triangles[i * 9 + 6] = n;
       triangles[i * 9 + 7] = n;

       uv.x = poly.getxaxis().dot(tr[i*3] - poly.getorigin());
       uv.y = poly.getyaxis().dot(tr[i*3] - poly.getorigin());

       if (sp.tmi.texturetype == 1)
         {  if (sp.tmi.x1 == sp.tmi.x2 && sp.tmi.y1 == sp.tmi.y2 && sp.tmi.x1 < 0 && sp.tmi.y1 < 0.0)
              {  triangles[i*9 + 8].x = sp.tmi.xaxis.x * uv.x + sp.tmi.xaxis.y * uv.y + sp.tmi.xaxis.z;
                 triangles[i*9 + 8].y = sp.tmi.yaxis.x * uv.x + sp.tmi.yaxis.y * uv.y + sp.tmi.yaxis.z;
              }
            else
              {  triangles[i*9 + 8].x = sp.tmi.x1 + (uv.x - umin) / (umax - umin) * (sp.tmi.x2 - sp.tmi.x1);
                 triangles[i*9 + 8].y = sp.tmi.y1 + (uv.y - vmin) / (vmax - vmin) * (sp.tmi.y2 - sp.tmi.y1);
              }
         }
       else
         {  triangles[i*9 + 8].x = (uv.x - umin) / (umax - umin) + umin;
            triangles[i*9 + 8].y = (uv.y - vmin) / (vmax - vmin) + vmin;
         }

       uv.x = poly.getxaxis().dot(tr[i*3+1] - poly.getorigin());
       uv.y = poly.getyaxis().dot(tr[i*3+1] - poly.getorigin());

       if (sp.tmi.texturetype == 1)
         {  if (sp.tmi.x1 == sp.tmi.x2 && sp.tmi.y1 == sp.tmi.y2 && sp.tmi.x1 < 0 && sp.tmi.y1 < 0.0)
              {  triangles[i*9 + 9].x = sp.tmi.xaxis.x * uv.x + sp.tmi.xaxis.y * uv.y + sp.tmi.xaxis.z;
                 triangles[i*9 + 9].y = sp.tmi.yaxis.x * uv.x + sp.tmi.yaxis.y * uv.y + sp.tmi.yaxis.z;
              }
            else
              {  triangles[i*9 + 9].x = sp.tmi.x1 + (uv.x - umin) / (umax - umin) * (sp.tmi.x2 - sp.tmi.x1);
                 triangles[i*9 + 9].y = sp.tmi.y1 + (uv.y - vmin) / (vmax - vmin) * (sp.tmi.y2 - sp.tmi.y1);
              }
         }
       else
         {  triangles[i*9 + 9].x = (uv.x - umin) / (umax - umin) + umin;
            triangles[i*9 + 9].y = (uv.y - vmin) / (vmax - vmin) + vmin;
         }

       uv.x = poly.getxaxis().dot(tr[i*3+2] - poly.getorigin());
       uv.y = poly.getyaxis().dot(tr[i*3+2] - poly.getorigin());

       if (sp.tmi.texturetype == 1)
         {  if (sp.tmi.x1 == sp.tmi.x2 && sp.tmi.y1 == sp.tmi.y2 && sp.tmi.x1 < 0 && sp.tmi.y1 < 0.0)
              {  triangles[i*9 + 10].x = sp.tmi.xaxis.x * uv.x + sp.tmi.xaxis.y * uv.y + sp.tmi.xaxis.z;
                 triangles[i*9 + 10].y = sp.tmi.yaxis.x * uv.x + sp.tmi.yaxis.y * uv.y + sp.tmi.yaxis.z;
              }
            else
              {  triangles[i*9 + 10].x = sp.tmi.x1 + (uv.x - umin) / (umax - umin) * (sp.tmi.x2 - sp.tmi.x1);
                 triangles[i*9 + 10].y = sp.tmi.y1 + (uv.y - vmin) / (vmax - vmin) * (sp.tmi.y2 - sp.tmi.y1);
              }
         }
       else
         {  triangles[i*9 + 10].x = (uv.x - umin) / (umax - umin);
            triangles[i*9 + 10].y = (uv.y - vmin) / (vmax - vmin);
         }


    }

}


static void countline(Point,Point,void *data)
{
  CBdrawline_data *dldata = (CBdrawline_data *) data;
  dldata->count += 34;
}

static void saveline(Point p1,Point p2,void *data)
{
#ifdef USING_WIDGETS
    QGraphicsLineItem *qline=0;
    CBdrawline_data *dldata = (CBdrawline_data *) data;
    p1 = dldata->surface->modeltoview(p1);
    p2 = dldata->surface->modeltoview(p2);
    if (dldata->surface->clip3dfb(&p1,&p2))
      {  p1 = dldata->surface->viewtoscreen(p1);
         p2 = dldata->surface->viewtoscreen(p2);
         if (! dldata->surface->displaylist.moveto(p1.x,p1.y)) return;
         if (! dldata->surface->displaylist.lineto(p2.x,p2.y)) return;
         QPointF qp1 = ((RCView*)dldata->surface->gethwnd())->view()->mapToScene(p1.x,p1.y);
         QPointF qp2 = ((RCView*)dldata->surface->gethwnd())->view()->mapToScene(p2.x,p2.y);
         if(dldata->surface->getScene())
            qline = dldata->surface->getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y());
         if(qline)
         {
             //qline->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
             ((QGraphicsItemGroup*)dldata->qgi)->addToGroup(qline);
         }
      }
#else
  CBdrawline_data *dldata = (CBdrawline_data *) data;
  p1 = dldata->surface->modeltoview(p1);
  p2 = dldata->surface->modeltoview(p2);
  if (dldata->surface->clip3dfb(&p1,&p2))
    {  p1 = dldata->surface->viewtoscreen(p1);
       p2 = dldata->surface->viewtoscreen(p2);
       if (! dldata->surface->displaylist.moveto(p1.x,p1.y)) return;
       if (! dldata->surface->displaylist.lineto(p2.x,p2.y)) return;
    }
#endif
}

void Plane::addtodisplaylist(View3dSurface *vs,int child)
{
#ifdef USING_WIDGETS
    CBdrawline_data dldata;
    HTCallback cb;
    int i,n;
    double a,len;
    Point p1,n1,n2,dir1,o,xa,ya;
    long size;
    Transform t1,t2,tview;

    QGraphicsItem *qgedges=0;

    if(qgi)
        vs->getScene()->removeItem(qgi);
    delete qgi;
    qgi = 0;
    qgplane = 0;


     if (options.test(1))
       {  t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
          xa = t1.transform(xaxis);
          ya = t1.transform(yaxis);
       }
     else
       {  xa = xaxis;
          ya = yaxis;
       }

     Polygon3d polygon3d(origin,xa,ya,list,0,
                         (vs->getumax() - vs->getumin()) / vs->getwidth());

     if (options.test(PlaneDisplayEdges))
        qgedges = polygon3d.addedgestodisplaylist(vs);

     if (options.test(PlaneNoFill))
       {
       }
     else if (options.test(PlaneFilled))
       {int c;
          polygon3d.transform(&vs->modeltransform);
          for (c = child ; c < 2 ; c++)
            {  if (! child)
                 {  if (c == 0)
                      vs->displaylist.setcountmode();
                    else
                      vs->displaylist.setsavemode();
                    if (! vs->displaylist.start(this,size)) return;
                 }
               polygon3d.addtodisplaylist(vs,1,0);
               size = vs->displaylist.getcount();
            }
          if (! child && ! vs->displaylist.end()) return;
       }
     else if (options.test(PlanePatterned))
       {
          /*
          CadFile infile;
          int i,j,imin,imax,jmin,jmax,c;

          RCGraphicsPlaneItem * qplane = new RCGraphicsPlaneItem();
          QPainterPath qclippath;

          if(qgi)
              qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));

          size = 0;
          for (c = child ; c < 2 ; c++)
            {  if (! child)
                 {  if (c == 0)
                      vs->displaylist.setcountmode();
                    else
                      vs->displaylist.setsavemode();
                    if (! vs->displaylist.start(this,size)) return;
                 }

               polygon3d.addtodisplaylist(vs,0,0);

               qclippath = polygon3d.addpathtodisplaylist(vs);

               if(qplane)
               {
                   qplane->qpath = qclippath;
                   qplane->qpattern = new QGraphicsItemGroup(qplane);
                   if(qplane->qpattern)
                       qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
               }

               if (infile.open(patternfilename,"rb"))
                 {  if (infile.readheader())
                      {Entity *e;
                       Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,p1,p2,pmin1,pmax1,pmin2,pmax2;

                       //  Set the clipping region
                       polygon3d.setextents();

                       //  Calculate the extents of the pattern from the point entities
                       while ((e = infile.nextentity(NULL)) != NULL)
                         {  if (e->isa(point_entity))
                              {  p = ((PointE *)e)->getp();
                                 if (p.x < pmin.x) pmin.x = p.x;
                                 if (p.y < pmin.y) pmin.y = p.y;
                                 if (p.z < pmin.z) pmin.z = p.z;
                                 if (p.x > pmax.x) pmax.x = p.x;
                                 if (p.y > pmax.y) pmax.y = p.y;
                                 if (p.z > pmax.z) pmax.z = p.z;
                              }
                            delete e;
                         }

                       if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                         {  pmin.setxyz(-10.0,-10.0,-10.0);
                            pmax.setxyz(10.0,10.0,10.0);
                         }

                       pmin1 = polygon3d.getpmin();
                       pmax1 = polygon3d.getpmax();

                       pmin2.x = pmax2.y = (pmin1 - origin).dot(xa);
                       pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
                       for (i = 0 ; i < 7 ; i++)
                         {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                            p2.x = (p1 - origin).dot(xa);
                            p2.y = (p1 - origin).dot(ya);
                            if (pmin2.x > p2.x) pmin2.x = p2.x;
                            if (pmin2.y > p2.y) pmin2.y = p2.y;
                            if (pmax2.x < p2.x) pmax2.x = p2.x;
                            if (pmax2.y < p2.y) pmax2.y = p2.y;
                         }

                       t1.tocoordinatesystem(xa,ya,xa.cross(ya));
                       t2.translate(origin);
                       t1.apply(t2);

                       imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
                       jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
                       imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
                       jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);


                       for (i = imin ; i <= imax ; i++)
                         for (j = jmin ; j <= jmax ; j++)
                          {  infile.position(0);
                             infile.readheader();
                             vs->gettransform(&tview);
                             t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                             t2.translate(origin + xa * i * xspacing * (pmax.x - pmin.x) + ya * j * (pmax.y - pmin.y) * yspacing);
                             t1.apply(t2);
                             vs->applytransform(t1);
                             while ((e = infile.nextentity(NULL)) != NULL)
                               {  e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                                  e->setvisible(*getvisible());
                                  e->qgi=0;
                                  if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                                    e->addtodisplaylist(vs,1);
#if 1
                                  if(e->qgi)
                                  {
                                      if(qclippath.elementCount() > 0)
                                      {
                                          ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(e->qgi);
                                          e->qgi=0;
                                      }
                                  }
                                  if(e->qgi)
                                  {
                                      if(vs->getScene())
                                          vs->getScene()->removeItem(e->qgi);
                                      delete e->qgi;
                                      e->qgi=0;
                                  }
#else
                                  if(e->qgi)
                                      if(vs->getScene())
                                          vs->getScene()->removeItem(e->qgi);
                                  delete e->qgi;
                                  e->qgi=0;
#endif
                                  delete e;
                               }

                             vs->settransform(tview);

                          }
                      }

                 }

               vs->displaylist.addpolygon3d(origin,xaxis,yaxis,0,0,0,0,0);

               size = vs->displaylist.getcount();

               infile.close();
            }

          if(qgi)
          {
              if(qplane)
              {
                  ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
                  qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                  qgplane = qplane;
              }
              if(vs->getScene())
                  vs->getScene()->addItem(qgi);
          }
          if (! child && ! vs->displaylist.end()) return;
          */
       }
     else
       {
          dldata.surface = vs;
          dldata.count = 0;
          dldata.qgi = new QGraphicsItemGroup();

          cb = countline;
          if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
            {  for (i = 0 + child * 2 ; i < 4 ; i++)
                 {  if (! child)
                      {  if (i == 0)
                           {  vs->displaylist.setcountmode();
                              if (! vs->displaylist.start(this,dldata.count)) return;
                           }
                         else if (i == 2)
                           {  vs->displaylist.setsavemode();
                              cb = saveline;
                              if (! vs->displaylist.start(this,dldata.count)) return;
                           }
                      }
                    else
                      cb = saveline;

                    a = ((i % 2) == 0 ? angle : angle + crossangle);
                    o = origin;
                    dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
                    n1 = dir1.cross(xaxis.cross(yaxis));
                    if ((len = n1.length()) < db.getptoler()) return;
                    n1 /= len;
                    if ((i % 2) == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;
                    if (options.test(PlaneAnchored))
                      o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));
                    p1 = o;
                    n = 0;
                    while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,cb,&dldata) >= 0)
                    {
                        p1 += n2;  n++;
                    }
                    p1 = o - n2;
                    while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,cb,&dldata) <= 0)
                    {
                        p1 -= n2;  n++;
                    }
                    if ((i % 2) == 0 && fabs(crossangle) < db.getatoler()) i++;
                 }
               if (! child && ! vs->displaylist.end()) return;
            }

          //qgi = dldata.qgi;
          if(dldata.qgi)
          {
              qgi = dldata.qgi;

              if(qgedges)
                  ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);

              //qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));

              if(dldata.surface->getScene())
                  dldata.surface->getScene()->addItem(qgi);
          }
     }
#else
 CBdrawline_data dldata;
 HTCallback cb;
 int i,n;
 double a,len;
 Point p1,n1,n2,dir1,o,xa,ya;
 long size;
 Transform t1,t2,tview;

  if (options.test(1))
    {  t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       xa = t1.transform(xaxis);
       ya = t1.transform(yaxis);
    }
  else
    {  xa = xaxis;
       ya = yaxis;
    }

  Polygon3d polygon3d(origin,xa,ya,list,0,
                      (vs->getumax() - vs->getumin()) / vs->getwidth());

  if (options.test(PlaneDisplayEdges))
     polygon3d.addedgestodisplaylist(vs);

  if (options.test(PlaneNoFill))
    {
    }
  else if (options.test(PlaneFilled))
    {int c;
       polygon3d.transform(&vs->modeltransform);
       for (c = child ; c < 2 ; c++)
         {  if (! child)
              {  if (c == 0)
                   vs->displaylist.setcountmode();
                 else
                   vs->displaylist.setsavemode();
                 if (! vs->displaylist.start(this,size)) return;
              }
            polygon3d.addtodisplaylist(vs,1,0);
            size = vs->displaylist.getcount();
         }
       if (! child && ! vs->displaylist.end()) return;
    }
  else if (options.test(PlanePatterned))
    {CadFile infile;
     int i,j,imin,imax,jmin,jmax,c;
       size = 0;
       for (c = child ; c < 2 ; c++)
         {  if (! child)
              {  if (c == 0)
                   vs->displaylist.setcountmode();
                 else
                   vs->displaylist.setsavemode();
                 if (! vs->displaylist.start(this,size)) return;
              }

            polygon3d.addtodisplaylist(vs,0,0);

            if (infile.open(patternfilename,"rb"))
              {  if (infile.readheader())
                   {Entity *e;
                    Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,p1,p2,pmin1,pmax1,pmin2,pmax2;

                    //  Set the clipping region
                    polygon3d.setextents();

                    //  Calculate the extents of the pattern from the point entities
                    while ((e = infile.nextentity(NULL)) != NULL)
                      {  if (e->isa(point_entity))
                           {  p = ((PointE *)e)->getp();
                              if (p.x < pmin.x) pmin.x = p.x;
                              if (p.y < pmin.y) pmin.y = p.y;
                              if (p.z < pmin.z) pmin.z = p.z;
                              if (p.x > pmax.x) pmax.x = p.x;
                              if (p.y > pmax.y) pmax.y = p.y;
                              if (p.z > pmax.z) pmax.z = p.z;
                           }
                         delete e;
                      }

                    if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                      {  pmin.setxyz(-10.0,-10.0,-10.0);
                         pmax.setxyz(10.0,10.0,10.0);
                      }

                    pmin1 = polygon3d.getpmin();
                    pmax1 = polygon3d.getpmax();

                    pmin2.x = pmax2.y = (pmin1 - origin).dot(xa);
                    pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
                    for (i = 0 ; i < 7 ; i++)
                      {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                         p2.x = (p1 - origin).dot(xa);
                         p2.y = (p1 - origin).dot(ya);
                         if (pmin2.x > p2.x) pmin2.x = p2.x;
                         if (pmin2.y > p2.y) pmin2.y = p2.y;
                         if (pmax2.x < p2.x) pmax2.x = p2.x;
                         if (pmax2.y < p2.y) pmax2.y = p2.y;
                      }

                    t1.tocoordinatesystem(xa,ya,xa.cross(ya));
                    t2.translate(origin);
                    t1.apply(t2);

                    imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
                    jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
                    imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
                    jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);


                    for (i = imin ; i <= imax ; i++)
                      for (j = jmin ; j <= jmax ; j++)
                       {  infile.position(0);
                          infile.readheader();
                          vs->gettransform(&tview);
                          t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                          t2.translate(origin + xa * i * xspacing * (pmax.x - pmin.x) + ya * j * (pmax.y - pmin.y) * yspacing);
                          t1.apply(t2);
                          vs->applytransform(t1);
                          while ((e = infile.nextentity(NULL)) != NULL)
                            {  e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                               e->setvisible(*getvisible());
                               if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                                 e->addtodisplaylist(vs,1);
                               delete e;
                            }

                          vs->settransform(tview);

                       }
                   }

              }

            vs->displaylist.addpolygon3d(origin,xaxis,yaxis,0,0,0,0,0);

            size = vs->displaylist.getcount();

            infile.close();
         }
       if (! child && ! vs->displaylist.end()) return;
    }
  else
    {  dldata.surface = vs;
       dldata.count = 0;
       cb = countline;
       if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
         {  for (i = 0 + child * 2 ; i < 4 ; i++)
              {  if (! child)
                   {  if (i == 0)
                        {  vs->displaylist.setcountmode();
                           if (! vs->displaylist.start(this,dldata.count)) return;
                        }
                      else if (i == 2)
                        {  vs->displaylist.setsavemode();
                           cb = saveline;
                           if (! vs->displaylist.start(this,dldata.count)) return;
                        }
                   }
                 else
                   cb = saveline;

                 a = ((i % 2) == 0 ? angle : angle + crossangle);
                 o = origin;
                 dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
                 n1 = dir1.cross(xaxis.cross(yaxis));
                 if ((len = n1.length()) < db.getptoler()) return;
                 n1 /= len;
                 if ((i % 2) == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;
                 if (options.test(PlaneAnchored))
                   o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));
                 p1 = o;
                 n = 0;
                 while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,cb,&dldata) >= 0)
                   {  p1 += n2;  n++;
                   }
                 p1 = o - n2;
                 while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,cb,&dldata) <= 0)
                   {  p1 -= n2;  n++;
                   }
                 if ((i % 2) == 0 && fabs(crossangle) < db.getatoler()) i++;
              }
            if (! child && ! vs->displaylist.end()) return;
         } 
    }
#endif
}

#ifdef USING_WIDGETS
// this is not called for OpenGl
static QGraphicsItem * FillGradientPolygon(Polygon3d *polygon3d,View3dSurface *surface,int drawmode,Transform *transform,RCCOLORREF colour,double transparency)
{
    if (drawmode != DM_ERASE)
        surface->setfillcolour(colour);
    if(transparency != 0.0)
    {
        QBrush brush(QColor(GetRValue(colour),
                            GetGValue(colour),
                            GetBValue(colour)
                            ,(1.0-transparency)*255));
        return polygon3d->fill(drawmode,transform,surface,&brush);
    }
    else
        return polygon3d->fill(drawmode,transform,surface,(QBrush*)0);
}
#else
static void FillGradientPolygon(Polygon3d *polygon3d,View3dSurface *surface,int drawmode,Transform *transform,RCCOLORREF colour,double transparency)
{
    if (OpenOpenGl != 0 && transparency != 0.0 && drawmode == DM_NORMAL)
    {
        SetMaterialEx(GetRValue(colour)/255.0,GetGValue(colour)/255.0,GetBValue(colour)/255.0,1.0,0.0,0.0,transparency);
        polygon3d->transform(&surface->modeltransform);
        DrawPolygon(polygon3d->getorigin(),polygon3d->getxaxis(),polygon3d->getyaxis(),polygon3d->getnloops(),polygon3d->getnvert(),polygon3d->getverts());
        Flush();
    }
    else
    {
        if (drawmode != DM_ERASE)
            surface->setfillcolour(colour);
        polygon3d->fill(drawmode,transform,surface);
    } 
}
#endif

#ifdef USING_WIDGETS

void Plane::drawGL(int drawmode,Transform *transform,View3dSurface *surface)
{
 CBdrawline_data dldata;
 CadFile infile;
 int i,j,halfway,imin,jmin,imax,jmax,n,r1,g1,b1,r2,g2,b2,gradientfillstyle;
 double a,len;
 Point p1,p2,p3,p4,n1,n2,dir1,o,xa,ya,pmin1,pmax1,pmin2,pmax2;
 Transform t1,t2,tview;
 RCHRGN clipregion;

 //qDebug() << "entering Plane::drawGL";

 // get rid of the scene version
 draw(DM_ERASE,transform,surface);

  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled) || options.test(PlaneGradientFilled))
    {
       t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
      /*
       if(options.test(PlaneAnchored))
           t1.rotate(origin-anchor,xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       else
        t1.rotate(origin,xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       */
       xa = t1.transform(xaxis);
       ya = t1.transform(yaxis);
    }
  else
    {  xa = xaxis;
       ya = yaxis;
    }

  if (getselected() && drawmode == DM_NORMAL)
      drawmode = DM_SELECT;
  surface->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

  RCCOLORREF pcol;
  pcol = surface->getcolour();
  GLfloat fr=GetRValue(pcol)/255.0;
  GLfloat fg=GetGValue(pcol)/255.0;
  GLfloat fb=GetBValue(pcol)/255.0;
  glColor3f(fr,fg,fb);
  //glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = surface->curPen.widthF();
  //glLineWidth(lwidth<1.0?1.0:lwidth);
  glLineWidth(surface->curGLWeight);

#if 0
  if (drawmode != DM_GL && ! options.test(PlaneDisplayEdges) && ! options.test(PlaneBitmapFilled) &&  ! options.test(PlaneGradientFilled) && ! options.test(PlaneNoFill) && transparency == 0.0 && transform == NULL)
    {  if (surface->displaylist.draw(this,drawmode))
         return;
       if (! surface->displaylist.geterrorstatus())
         addtodisplaylist(surface,0);
       if (surface->displaylist.draw(this,drawmode))
         return;
    }
#endif

  // always using OpenGL
  db.setrequiresopengl(1);

  Polygon3d polygon3d(origin,xa,ya,list,0,(surface->getumax() - surface->getumin()) / surface->getwidth());
  dldata.surface = surface;
  dldata.transform = transform;
  dldata.qgi=0;

  polygon3d.setextents();

  polygon3d.transform(&surface->modeltransform);

  if ( options.test(PlaneDisplayEdges) || drawnhighlighted || drawmode == DM_SCROLLING || (drawmode == DM_SELECT && options.test(PlaneNoFill)) )
  {
      if(surface->getrepaintstyle() == Wireframe)
        SetNoMaterial(fr,fg,fb);
      polygon3d.drawGL(drawmode,transform,surface);
  }

  if(drawmode == DM_SCROLLING)
      return;

  //  Planes without fill, and no edges are drawn with edges when selected
  //  The next time they are drawn, they are also drawn with edges to remove the highlighting.
  //if (drawnhighlighted == 0)
  //  drawnhighlighted = drawmode == DM_SELECT && options.test(PlaneNoFill);
  //else if (drawmode == DM_NORMAL)
  //  drawnhighlighted = 0;

  //SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

  //qDebug() << "plane::drawGl : surface render style : " << surface->getrepaintstyle();
  //qDebug() << "plane::drawGl : drawmode : " << drawmode;

  if( (surface->getrepaintstyle() == Shaded ||
       surface->getrepaintstyle() == ShadedHidden ||
       surface->getrepaintstyle() == ShadedHiddenDashed ||
       surface->getrepaintstyle() == ShadedTexture ||
       surface->getrepaintstyle() == ShadedHiddenTexture) &&
       drawmode != DM_SELECT)
  //if( surface->getrepaintstyle() == Shaded && drawmode == DM_GL)
  {
      // KMJ: this only does plane rendering no bitmaps or gradation
      int nlights = 0;
      View *view;
      Entity *e;
      EntityList dblist;
      int k,t;
      SurfaceProperties *sp;
      Point eye,pmin,pmax;


      if(isvisible(surface))
      {
         double ambient = v.getreal("sh::ambient");
         double diffuse = v.getreal("sh::diffuse");

         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);

         Transform mt;
         if(transform)
             mt = surface->modeltransform.apply(*transform);
         else
             mt = surface->modeltransform;

         if ( getntriangles() < 1)
            triangulate((surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler(),&mt);

         ntriangles = getntriangles();
         triangles = gettriangles();

         for (k = 0,j = 0 ; k < ntriangles ; k++,j += 9)
         {
             if (triangles[j].z == RCDBL_MAX)
             {
                   sp = 0;
                   if(surface->getrepaintstyle() == ShadedTexture ||
                      surface->getrepaintstyle() == ShadedHiddenTexture)
                       sp = (SurfaceProperties *) (LONG_PTR)triangles[j].x;
                   if (sp != 0 && sp->tmi.texturetype == 1)// texture set using material ambient and diffuse
                   {
                        SetMaterial(1.0,1.0,1.0,ambient,diffuse,sp->specular);
                        //SetMaterialEx(1.0,1.0,1.0,ambient,diffuse,sp->specular,triangles[j].y);
                        if(drawmode == DM_PRINT)
                            LoadTexturePrint(sp->tmi.filename,1.0,1.0,1.0,0.0,1.0-triangles[j].y,&t);
                        else
                            LoadTexture(sp->tmi.filename,1.0,1.0,1.0,0.0,1.0-triangles[j].y,&t);

                        if (t == -1)
                          {  SetTexture(0);
                             SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,sp->specular,triangles[j].y);
                          }
                        else
                          {
                             SetTexture(t);
                          }
                   }
                   else if(sp != 0 && sp->tmi.texturetype == 0) // no texture material set using material ambient and diffuse
                   {
                        //SetMaterial(fr,fg,fb,sp->ambient,sp->diffuse,sp->specular);
                        SetTexture(0);
                        SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                   }
                   else // no material set using default ambient and diffuse
                   {
                        //SetMaterial(fr,fg,fb,sp->ambient,sp->diffuse,sp->specular);
                        SetTexture(0);
                        SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                   }
                   j += 2;
              }

              if (DrawTexturedTriangles != 0 && j+1 < ntriangles && triangles[j+9].z != RCDBL_MAX)
              {
                  DrawTexturedTriangles(ntriangles,triangles);
                  break;
              }
              else
                  DrawTexturedTriangle(triangles + j);
         }
         glFinish();
         //qDebug() << "in plane gl shade :";
      }

      //EndScene();

#if 0
      // this is for rendering uses the RCGraphicsPlaneItem
      db.setrequiresopengl(1);

      // put the OpenGL shading code in here

      qgrenderplane = new QGraphicsItemGroup();
      //qgi = new QGraphicsItemGroup();
      RCGraphicsPlaneItem * qplane=0;
      Transform transf = surface->modeltransform;
      qplane = new RCGraphicsPlaneItem(this,transf,surface,false);
      QPainterPath qclipPath;
      qclipPath.setFillRule(Qt::OddEvenFill);

      if(qplane)
      {
          qclipPath = polygon3d.setclipregion(transform,surface);
          qplane->qpath = qclipPath;
          qplane->rcdrawmode = drawmode;
          qgplane = qplane;
      }
#endif
  }
  else if (options.test(PlaneNoFill))
  {
      return;// nothing to do here yet
  }
  else if (options.test(PlaneFilled))
  {
       //qclipPath = polygon3d.setclipregion(transform,surface);


       if( OpenOpenGl == 0 || transparency == 0.0 || drawmode != DM_NORMAL || transform != 0)
       {
           if(drawmode == DM_SELECT)
               SetNoMaterial(fr,fg,fb,0.5);
           else
               SetNoMaterial(fr,fg,fb);
           polygon3d.fillGL(drawmode,transform,surface);
       }
       else
       {

           //OpenOpenGl(((RCView*)surface->gethdc())->view(),3,0,0,widget->width(),widget->height());

           //view = cadwindow->getcurrentsurface()->getview();
           //View *view = surface->getview();
           //SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
           //SetViewport(rcsurface->getumin(),rcsurface->getvmin(),rcsurface->getumax(),rcsurface->getvmax());
           //SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
           //QtBeginScene();
           //BeginScene(BS_NOCLEAR/*BS_FRONTBUFFERONLY | BS_IGNOREDEPTH */);

           SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

           double rc = db.colourtable[getcolour()].red/255.0;
           double gc = db.colourtable[getcolour()].green/255.0;
           double bc = db.colourtable[getcolour()].blue/255.0;

           if(drawmode == DM_NORMAL)
               SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,(1.0-gettransparency()));
           else if(drawmode == DM_SELECT)
               SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,0.5);

           //polygon3d.transform(&rcsurface->modeltransform);
           if(transform)
              polygon3d.transform(transform);
           //rcsurface->modeltransform.dump("Gl transform");
           DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
       }
  }
  else if (options.test(PlaneBitmapFilled))
  {
      if ( polygon3d.getdefined() &&
           getpatternfilename() &&
           strlen(getpatternfilename()) > 0)
      {
          int texture;
          Point p[9];
          Point p3,p4;

          Point origina = origin;
          if(options.test(PlaneAnchored))
             origina = anchor - origin;

           //polygon3d.setextents();
          if(transform)
            polygon3d.transform(transform);

           pmin1 = polygon3d.getpmin();
           pmax1 = polygon3d.getpmax();

           pmin2.x = pmax2.x = (pmin1 - origina).dot(xa);
           pmin2.y = pmax2.y = (pmin1 - origina).dot(ya);
           for (i = 0 ; i < 7 ; i++)
             {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                p2.x = (p1 - origina).dot(xa);
                p2.y = (p1 - origina).dot(ya);
                if (pmin2.x > p2.x) pmin2.x = p2.x;
                if (pmin2.y > p2.y) pmin2.y = p2.y;
                if (pmax2.x < p2.x) pmax2.x = p2.x;
                if (pmax2.y < p2.y) pmax2.y = p2.y;
             }

           imin = (int)floor(pmin2.x /  getxspacing());
           jmin = (int)floor(pmin2.y /  getyspacing());
           imax = (int)ceil(pmax2.x /  getxspacing());
           jmax = (int)ceil(pmax2.y /  getyspacing());


           //draw the stencil mask

           glEnable(GL_STENCIL_TEST);
           glStencilMask(0x00);
           glClearStencil(0);
           glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
           glDepthMask(GL_FALSE);

           glStencilFunc(GL_NEVER,1,0xff);
           glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
           glStencilMask(0xff);
           glClear(GL_STENCIL_BUFFER_BIT);

           DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());

           glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
           glDepthMask(GL_TRUE);
           glStencilFunc(GL_EQUAL,1,0xFF);
           glStencilMask(0x00);

           SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

           BitMask bo(32);
           if ( gettransparency() != 0.0 ||  gettransparencyrange() != 0.0)
             bo.set(9,1);
           if(bitmape == 0)
           {
               // if the bitmap is null
               // make a new bitmap otherwise use the existing pointer
               if(bitmape == 0)
               {
                  bitmape = new BitMapE(getpatternfilename(), getorigin(),
                  //bitmape = new BitMapE(getpatternfilename(), origina,
                                      xa,ya, getxspacing(), getyspacing(),
                                      getred(), getgreen(), getblue(),
                                      gettransparencyrange(),1.0-gettransparency(),bo);
               }
           }
#if 0
           // we are using the bitmap draw function so don't need this here
           int hor = bitmape->getoptions().test(2) * 2 + bitmape->getoptions().test(1);
           int ver = bitmape->getoptions().test(4) * 2 + bitmape->getoptions().test(3);

           if (drawmode == DM_NORMAL)
           {
               SetMaterial(1.0,1.0,1.0,1.0,1.0,0.0);
               LoadMemoryTexture(bitmape->getfilename(),(char *)bitmape->getpbuffer(),
                             bitmape->getbmi()->bmiHeader.biWidth,bitmape->getbmi()->bmiHeader.biHeight,
                             bitmape->getred(),bitmape->getgreen(),bitmape->getblue(),
                             bitmape->gettransparencyrange(),bitmape->getopacity(),&texture);
               SetTexture(texture);
           }
           else if (drawmode == DM_SELECT )
           {
               SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
               LoadMemoryTexture(bitmape->getfilename(),(char *)bitmape->getpbuffer(),
                             bitmape->getbmi()->bmiHeader.biWidth,bitmape->getbmi()->bmiHeader.biHeight,
                             bitmape->getred(),bitmape->getgreen(),bitmape->getblue(),
                             bitmape->gettransparencyrange(),bitmape->getopacity(),&texture);
               SetTexture(texture);
           }
           else if (drawmode == DM_INVERT )
           {
               SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
               LoadMemoryTexture(bitmape->getfilename(),(char *)bitmape->getpbuffer(),
                             bitmape->getbmi()->bmiHeader.biWidth,bitmape->getbmi()->bmiHeader.biHeight,
                             bitmape->getred(),bitmape->getgreen(),bitmape->getblue(),
                             bitmape->gettransparencyrange(),bitmape->getopacity(),&texture);
               SetTexture(texture);
           }
           else
           {
               SetMaterial(v.getreal("wn::backgroundcolour.red"),v.getreal("wn::backgroundcolour.green"),v.getreal("wn::backgroundcolour.blue"),1.0,1.0,0.0);
               SetTexture(0);
           }
#endif
           // draw the bitmap
           Point po = surface->modeltoscreen(Point(0.0,0.0,0.0));
           Point pb = surface->modeltoscreen(Point(getxspacing(),getyspacing(),0.0));
           int dx = pb.x - po.x;
           int dy = pb.y - po.y;
           int maxXlines = 50000;
           int maxYlines = 50000;
           if( ((imax-imin) < maxXlines && (jmax-jmin) < maxYlines) &&  min(dx,dy) > 10)
           {
               int interrupt=0;
               //program->resetinterrupt();
               for (i = imin ; i <= imax ; i++)
               {
                 for (j = jmin ; j <= jmax ; j++)
                   {
                      p1 = origina + xa * i * getxspacing() + ya * j * getyspacing();
                      //p1 = getorigin() + xa *  i * getxspacing() + ya * j * getyspacing();
                      //p2 = rcplane->getorigin() + xa * (i+1) * rcplane->getxspacing() + ya * (j+1) * rcplane->getyspacing();
                      bitmape->setorigin(p1);
                      bitmape->setxaxis(xa);
                      bitmape->setyaxis(ya);
                      bitmape->setoptions(bo);
                      if (bitmape->getdefined())
                      {
                          //qDebug() << "drawing bitmap plane with GL";
                          bitmape->drawGL(drawmode,transform,surface);
                          if((interrupt=program->getinterrupt()))
                              break;
    #if 0
                          // we are using the bitmap draw function so don't need this here
                          p1 = surface->modeltransform.transform(bitmape->getorigin());

                          p1 = p1 - bitmape->getxaxis() * (hor*bitmape->getwidth()/2.0) - bitmape->getyaxis() * ver*bitmape->getheight()/2.0;
                          p2 = p1 + bitmape->getxaxis() * bitmape->getwidth();
                          p3 = p2 + bitmape->getyaxis() * bitmape->getheight();
                          p4 = p3 - bitmape->getxaxis() * bitmape->getwidth();


                          //p1 = cadwindow->getcurrentsurface()->modeltransform.transform(p1);
                          //p2 = cadwindow->getcurrentsurface()->modeltransform.transform(p2);
                          //p3 = cadwindow->getcurrentsurface()->modeltransform.transform(p3);
                          //p4 = cadwindow->getcurrentsurface()->modeltransform.transform(p4);

                          p1 = surface->modeltransform.transform(p1);
                          p2 = surface->modeltransform.transform(p2);
                          p3 = surface->modeltransform.transform(p3);
                          p4 = surface->modeltransform.transform(p4);

                          p[0] = p1;
                          p[1] = p2;
                          p[2] = p3;
                          p[3] = bitmape->getxaxis().cross(bitmape->getyaxis());
                          p[4] = p[3];
                          p[5] = p[3];
                          p[6] = Point(0.001,0.001,0.0);
                          p[7] = Point(0.999,0.001,0.0);
                          p[8] = Point(0.999,0.999,0.0);
                          DrawTexturedTriangle(p);

                          p[0] = p1;
                          p[1] = p3;
                          p[2] = p4;
                          p[3] = bitmape->getxaxis().cross(bitmape->getyaxis());
                          p[4] = p[3];
                          p[5] = p[3];
                          p[6] = Point(0.001,0.001,0.0);
                          p[7] = Point(0.999,0.999,0.0);
                          p[8] = Point(0.001,0.999,0.0);
                          DrawTexturedTriangle(p);

                          Flush();
    #endif
                      }
                      //if(interrupt == 1)
                      //    break;
                   }
               }
           }
           else
           {
               // too many small images or pixel coverage is too small
               // use a transparent plane fill
               SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

               // filled non shaded transparent plane
               //double rc = db.colourtable[getcolour()].red/255.0;
               //double gc = db.colourtable[getcolour()].green/255.0;
               //double bc = db.colourtable[getcolour()].blue/255.0;
               //SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,0.25);
               SetNoMaterial(fr,fg,fb,0.25);

               //polygon3d.transform(&rcsurface->modeltransform);
               //if(transform)
               //   polygon3d.transform(transform);
               //rcsurface->modeltransform.dump("Gl transform");
               DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
           }

           glStencilMask(0xFF);
           glClear(GL_STENCIL_BUFFER_BIT);
           glDisable(GL_STENCIL_TEST);
        }
  }
  else if (options.test(PlaneGradientFilled))
  {
      if(polygon3d.getdefined())
      {
          if(transform)
            polygon3d.transform(transform);
          pmin1 = polygon3d.getpmin();
          pmax1 = polygon3d.getpmax();

          pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
          pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
          for (i = 0 ; i < 7 ; i++)
            {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
               p2.x = (p1 - origin).dot(xa);
               p2.y = (p1 - origin).dot(ya);
               if (pmin2.x > p2.x) pmin2.x = p2.x;
               if (pmin2.y > p2.y) pmin2.y = p2.y;
               if (pmax2.x < p2.x) pmax2.x = p2.x;
               if (pmax2.y < p2.y) pmax2.y = p2.y;
            }
          pmin2.z = pmax2.z = 0.0;

          imin = (int)floor(pmin2.x /  xspacing);
          jmin = (int)floor(pmin2.y /  yspacing);
          imax = (int)ceil(pmax2.x /  xspacing);
          jmax = (int)ceil(pmax2.y /  yspacing);

          if (imin >= imax)
            imin = imax - 1;
          if (jmin >= jmax)
            jmin = jmax - 1;

          // this is the planes colour
          //r1 = db.colourtable[colour].red;
          //g1 = db.colourtable[colour].green;
          //b1 = db.colourtable[colour].blue;
          // might be selected so use the current colour
          r1 = GetRValue(surface->getcolour());
          g1 = GetGValue(surface->getcolour());
          b1 = GetBValue(surface->getcolour());

          r2 = GetRValue(gradientcolour);
          g2 = GetGValue(gradientcolour);
          b2 = GetBValue(gradientcolour);

          gradientfillstyle = options.test(PlaneGradientStyle1) * 8 + options.test(PlaneGradientStyle2) * 4 +  options.test(PlaneGradientStyle3) * 2 +  options.test(PlaneGradientStyle4);


          //draw the stencil mask
          Point po = surface->modeltoscreen(Point(0.0,0.0,0.0));
          Point pb = surface->modeltoscreen(Point(xspacing,yspacing,0.0));
          int dx = pb.x - po.x;
          int dy = pb.y - po.y;

          switch(gradientfillstyle)
          {
          case 0:
          case 1:
          case 4:
          case 5:
          case 8:
          case 9:
              dy = dx;
              break;
          case 2:
          case 3:
          case 6:
          case 7:
              dx = dy;
              break;
          }

          int maxXlines = 10000;
          int maxYlines = 10000;

          if((imax-imin) < maxXlines && (jmax-jmin) < maxYlines /*&& min(dx,dy) > 1*/ )
          {
          glEnable(GL_STENCIL_TEST);
          glStencilMask(0x00);
          glClearStencil(0);
          glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
          glDepthMask(GL_FALSE);

          glStencilFunc(GL_NEVER,1,0xff);
          glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
          glStencilMask(0xff);
          glClear(GL_STENCIL_BUFFER_BIT);

          DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
          //polygon3d.drawGL(drawmode,transform,surface);

          glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
          glDepthMask(GL_TRUE);
          glStencilFunc(GL_EQUAL,1,0xFF);
          glStencilMask(0x00);

          SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);
          //SetNoMaterial(fr,fg,fb);

          if(gradientfillstyle == 0)
          {
              for (i = imin ; i <= imax ; i++)
              {
                   p1 = getorigin() + xa *  i * getxspacing() + ya * jmin * getyspacing();
                   p2 = getorigin() + xa * (i+1) * getxspacing() + ya * jmin * getyspacing();
                   p3 = getorigin() + xa * (i+1) * getxspacing() + ya * jmax * getyspacing();
                   p4 = getorigin() + xa * i * getxspacing() + ya * jmax * getyspacing();
                   Polygon3d polygon3d(p1,p2,p3,p4);

                   double grc = (r1 + ((r2 - r1)*(i-imin))/(imax - imin)) / 255.0;
                   double ggc = (g1 + ((g2 - g1)*(i-imin))/(imax - imin)) / 255.0;
                   double gbc = (b1 + ((b2 - b1)*(i-imin))/(imax - imin)) / 255.0;

                   SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,(1.0-gettransparency()));
                   polygon3d.transform(&surface->modeltransform);
                   if(transform)
                       polygon3d.transform(transform);
                   DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                   Flush();
              }
          }
          else if (gradientfillstyle == 1)
            {

              for (i = imin ; i <= imax ; i++)
                 {  p1 = getorigin() + xa *  i * getxspacing() + ya * jmin * getyspacing();
                    p2 = getorigin() + xa * (i+1) * getxspacing() + ya * jmin * getyspacing();
                    p3 = getorigin() + xa * (i+1) * getxspacing() + ya * jmax * getyspacing();
                    p4 = getorigin() + xa * i * getxspacing() + ya * jmax * getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);

                    //RGB(r2 + ((r1 - r2)*(i-imin))/(imax - imin),g2 + ((g1 - g2)*(i-imin))/(imax - imin),b2 + ((b1 - b2)*(i-imin))/(imax - imin));
                    double grc = (r2 + ((r1 - r2)*(i-imin))/(imax - imin)) / 255.0;
                    double ggc = (g2 + ((g1 - g2)*(i-imin))/(imax - imin)) / 255.0;
                    double gbc = (b2 + ((b1 - b2)*(i-imin))/(imax - imin)) / 255.0;

                    SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,(1.0-gettransparency()));
                    polygon3d.transform(&surface->modeltransform);
                    if(transform)
                        polygon3d.transform(transform);
                    DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                    Flush();
                 }
            }
          else if (gradientfillstyle == 2)
            {

              for (j = jmin ; j <= jmax ; j++)
                 {  p1 =  getorigin() + xa * imin *  getxspacing() + ya * j *  getyspacing();
                    p2 =  getorigin() + xa * imax *  getxspacing() + ya * j *  getyspacing();
                    p3 =  getorigin() + xa * imax *  getxspacing() + ya * (j+1) *  getyspacing();
                    p4 =  getorigin() + xa * imin *  getxspacing() + ya * (j+1) *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);

                    //RGB(r2 + ((r1 - r2)*(j-jmin))/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin))/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin))/(jmax - jmin))
                    double grc = (r2 + ((r1 - r2)*(j-jmin))/(jmax - jmin)) / 255.0;
                    double ggc = (g2 + ((g1 - g2)*(j-jmin))/(jmax - jmin)) / 255.0;
                    double gbc = (b2 + ((b1 - b2)*(j-jmin))/(jmax - jmin)) / 255.0;

                    SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                    polygon3d.transform(&surface->modeltransform);
                    if(transform)
                        polygon3d.transform(transform);
                    DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                    Flush();
              }
            }
          else if (gradientfillstyle == 3)
            {  for (j = jmin ; j <= jmax ; j++)
                 {  p1 =  getorigin() + xa * imin *  getxspacing() + ya * j *  getyspacing();
                    p2 =  getorigin() + xa * imax *  getxspacing() + ya * j *  getyspacing();
                    p3 =  getorigin() + xa * imax *  getxspacing() + ya * (j+1) *  getyspacing();
                    p4 =  getorigin() + xa * imin *  getxspacing() + ya * (j+1) *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);

                    //RGB(r1 + ((r2 - r1)*(j-jmin))/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin))/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin))/(jmax - jmin));
                    double grc = (r1 + ((r2 - r1)*(j-jmin))/(jmax - jmin)) / 255.0;
                    double ggc = (g1 + ((g2 - g1)*(j-jmin))/(jmax - jmin)) / 255.0;
                    double gbc = (b1 + ((b2 - b1)*(j-jmin))/(jmax - jmin)) / 255.0;

                    SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                    polygon3d.transform(&surface->modeltransform);
                    if(transform)
                        polygon3d.transform(transform);
                    DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                    Flush();
                }
            }
          else if (gradientfillstyle == 4)
            {  for (i = imin ; i <= imax ; i++)
                 {  p1 =  getorigin() + xa *  i *  getxspacing() + ya * jmin *  getyspacing();
                    p2 =  getorigin() + xa * (i+1) *  getxspacing() + ya * jmin *  getyspacing();
                    p3 =  getorigin() + xa * (i+1) *  getxspacing() + ya * jmax *  getyspacing();
                    p4 =  getorigin() + xa * i *  getxspacing() + ya * jmax *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);



                    halfway = (imin + imax) / 2;
                    if (i < halfway)
                    {
                        //RGB(r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin),g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin),b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin));
                        double grc = (r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin)) / 255.0;
                        double ggc = (g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin)) / 255.0;
                        double gbc = (b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin)) / 255.0;

                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin),g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin),b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin)),transparency);
                    }
                    else
                    {
                        //RGB(r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin),g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin),b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin));

                        double grc = (r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin)) / 255.0;
                        double ggc = (g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin)) / 255.0;
                        double gbc = (b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin)) / 255.0;

                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin),g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin),b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin)),transparency);
                    }

                    Flush();
                 }
            }
          else if (gradientfillstyle == 5)
            {  for (i = imin ; i <= imax ; i++)
                 {  p1 =  getorigin()  + xa *  i *  getxspacing() + ya * jmin *  getyspacing();
                    p2 =  getorigin()  + xa * (i+1) *  getxspacing() + ya * jmin *  getyspacing();
                    p3 =  getorigin()  + xa * (i+1) *  getxspacing() + ya * jmax *  getyspacing();
                    p4 =  getorigin()  + xa * i *  getxspacing() + ya * jmax *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);

                    halfway = (imin + imax) / 2;
                    if (i < halfway)
                    {
                        //RGB(r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin),g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin),b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin));
                        double grc = (r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin)) / 255.0;
                        double ggc = (g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin)) / 255.0;
                        double gbc = (b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin)) / 255.0;
                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin),g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin),b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin)),transparency);
                    }
                    else
                    {
                        //RGB(r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin),g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin),b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin));
                       double grc = (r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                       double ggc = (g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                       double gbc = (b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin)) / 255.0;
                       SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,(1.0-gettransparency()));
                       polygon3d.transform(&surface->modeltransform);
                       if(transform)
                            polygon3d.transform(transform);
                       DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                       //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin),g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin),b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin)),transparency);
                    }
                 }
            }
          else if (gradientfillstyle == 6)
            {  for (j = jmin ; j <= jmax ; j++)
                 {  p1 =  getorigin() + xa * imin *  getxspacing() + ya * j *  getyspacing();
                    p2 =  getorigin() + xa * imax *  getxspacing() + ya * j *  getyspacing();
                    p3 =  getorigin() + xa * imax *  getxspacing() + ya * (j+1) *  getyspacing();
                    p4 =  getorigin() + xa * imin *  getxspacing() + ya * (j+1) *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);

                    halfway = (jmin + jmax) / 2;
                    if (j < halfway)
                    {
                        //RGB(r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin));
                        double grc = (r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        double ggc = (g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        double gbc = (b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin)),transparency);
                    }
                    else
                    {
                        //RGB(r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin));
                        double grc = (r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        double ggc = (g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        double gbc = (b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin)),transparency);
                    }
                 }
            }
          else if (gradientfillstyle == 7)
            {  for (j = jmin ; j <= jmax ; j++)
                 {  p1 =  getorigin() + xa * imin *  getxspacing() + ya * j *  getyspacing();
                    p2 =  getorigin() + xa * imax *  getxspacing() + ya * j *  getyspacing();
                    p3 =  getorigin() + xa * imax *  getxspacing() + ya * (j+1) *  getyspacing();
                    p4 =  getorigin() + xa * imin *  getxspacing() + ya * (j+1) *  getyspacing();
                    Polygon3d polygon3d(p1,p2,p3,p4);
                    halfway = (jmin + jmax) / 2;

                    if (j < halfway)
                    {
                        //RGB(r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin));
                        double grc = (r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        double ggc = (g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        double gbc = (b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin)) / 255.0;
                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, gettransparency());
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin)),transparency);
                    }
                    else
                    {
                       // RGB(r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin));
                        double grc = (r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        double ggc = (g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        double gbc = (b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin)) / 255.0;
                        SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0,(1.0-gettransparency()));
                        polygon3d.transform(&surface->modeltransform);
                        if(transform)
                            polygon3d.transform(transform);
                        DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                        //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin)),transparency);
                    }
                }
            }
          else if (gradientfillstyle == 8 || gradientfillstyle == 9)
            {double radius,radius1,radius2;

               radius = (pmax2 - pmin2).length() / 2.0;
               radius1 = 0.0;
               radius2 =  getxspacing();
               i = 0;
               imax = int(radius /  getxspacing() + 0.5);
               if (imax < 1) imax = 1;
               while (radius1 < radius)
                 {
                    p1 =  getorigin() + xa * (pmin2.x + pmax2.x) / 2.0 + ya * (pmin2.y + pmax2.y) / 2.0;
                    Circle circle1(p1,xa,ya,radius1,0.0,acos(-1.0)*2.0);
                    Circle circle2(p1,xa,ya,radius2,0.0,acos(-1.0)*2.0);
                    EntityList list;
                    if (radius1 > 0.0)
                    {
                         list.add(&circle1);
                         list.add(0);
                    }
                    list.add(&circle2);
                    //Polygon3d polygon3d(p1,xa,ya,list,0,(cadwindow->getcurrentsurface()->getumax() - cadwindow->getcurrentsurface()->getumin()) / cadwindow->getcurrentsurface()->getwidth());
                    Polygon3d polygon3d(p1,xa,ya,list,0,(surface->getumax() - surface->getumin()) / surface->getwidth());
                    if (gradientfillstyle == 8)
                    {
                         //RGB(r2 + ((r1 - r2)*i)/imax,g2 + ((g1 - g2)*i)/imax,b2 + ((b1 - b2)*i/imax));
                         double grc = (r2 + ((r1 - r2)*i)/imax) / 255.0;
                         double ggc = (g2 + ((g1 - g2)*i)/imax) / 255.0;
                         double gbc = (b2 + ((b1 - b2)*i/imax)) / 255.0;
                         SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                         polygon3d.transform(&surface->modeltransform);
                         if(transform)
                            polygon3d.transform(transform);
                         DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                         //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*i)/imax,g2 + ((g1 - g2)*i)/imax,b2 + ((b1 - b2)*i/imax)),transparency);
                    }
                    else
                    {
                         //RGB(r2 + ((r1 - r2)*(imax-i))/imax,g2 + ((g1 - g2)*(imax-i))/imax,b2 + ((b1 - b2)*(imax-i)/imax))
                         double grc = (r2 + ((r1 - r2)*(imax-i))/imax) / 255.0;
                         double ggc = (g2 + ((g1 - g2)*(imax-i))/imax) / 255.0;
                         double gbc = (b2 + ((b1 - b2)*(imax-i)/imax)) / 255.0;
                         SetMaterialEx(grc,ggc,gbc,1.0,0.0,0.0, (1.0-gettransparency()));
                         polygon3d.transform(&surface->modeltransform);
                         if(transform)
                            polygon3d.transform( transform);
                         DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                         //qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(imax-i))/imax,g2 + ((g1 - g2)*(imax-i))/imax,b2 + ((b1 - b2)*(imax-i)/imax)),transparency);
                    }
                    radius1 +=  getxspacing();
                    radius2 +=  getxspacing();
                    i++;
                 }
            }

          glStencilMask(0xFF);
          glClear(GL_STENCIL_BUFFER_BIT);
          glDisable(GL_STENCIL_TEST);
          }

      }
  }
  else if (options.test(PlanePatterned))
  {
       //Transform transf;
       if(patternfilename && infile.open(patternfilename,"rb"))
       {
           qDebug() << "patternfilename" << QString(patternfilename);

         if (infile.readheader())
         {
            Entity *e;
            Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,p1,p2,pmin1,pmax1,pmin2,pmax2;

            EntityList entityCache;

            int numLines = 0;
            entityCache.start();
            while ((e = infile.nextentity(NULL)) != NULL)
            {
                entityCache.add(e);
                numLines += getNumDrawSegments(e,surface);
            }

            infile.close();

            //  Set the clipping region
            //polygon3d.clip(surface);

              if (polygon3d.getdefined())
              {
                 //polygon3d.setclipregion(transform,surface);
                //qDebug() << "drawing pattern plane GL";
                if(transform)
                  polygon3d.transform(transform);

                //draw the stencil mask
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0x00);
                glClearStencil(0);
                glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                glDepthMask(GL_FALSE);

                glStencilFunc(GL_NEVER,1,0xff);
                glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
                glStencilMask(0xff);
                glClear(GL_STENCIL_BUFFER_BIT);

                DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                //polygon3d.drawGL(drawmode,transform,surface);

                glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
                glDepthMask(GL_TRUE);
                glStencilFunc(GL_EQUAL,1,0xFF);
                glStencilMask(0x00);

                SetNoMaterial(fr,fg,fb);

                 //  Calculate the extents of the pattern from the point entities
                 entityCache.start();
                 while((e = entityCache.next()) != NULL )
                 //while ((e = infile.nextentity(NULL)) != NULL)
                   {  if (e->isa(point_entity))
                        {
                           p = ((PointE *)e)->getp();
                           if (p.x < pmin.x) pmin.x = p.x;
                           if (p.y < pmin.y) pmin.y = p.y;
                           if (p.z < pmin.z) pmin.z = p.z;
                           if (p.x > pmax.x) pmax.x = p.x;
                           if (p.y > pmax.y) pmax.y = p.y;
                           if (p.z > pmax.z) pmax.z = p.z;
                        }
                      //delete e;
                   }

                 if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                   {  pmin.setxyz(-10.0,-10.0,-10.0);
                      pmax.setxyz(10.0,10.0,10.0);
                   }

                 pmin1 = polygon3d.getpmin();
                 pmax1 = polygon3d.getpmax();

                 // anchored origin?
                 Point origina = origin;
                 if(options.test(PlaneAnchored))
                     origina = anchor - origin;

                 pmin2.x = pmax2.x = (pmin1 - origina).dot(xa);
                 pmin2.y = pmax2.y = (pmin1 - origina).dot(ya);

                 for (i = 0 ; i < 7 ; i++)
                 {
                      p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                      p2.x = (p1 - origina).dot(xa);
                      p2.y = (p1 - origina).dot(ya);
                      if (pmin2.x > p2.x) pmin2.x = p2.x;
                      if (pmin2.y > p2.y) pmin2.y = p2.y;
                      if (pmax2.x < p2.x) pmax2.x = p2.x;
                      if (pmax2.y < p2.y) pmax2.y = p2.y;
                 }

                 //t1 = t1.identity();
                 //t1.tocoordinatesystem(xa,ya,xa.cross(ya));
                 //t2.translate(origina);
                 //t1.apply(t2);

                 imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
                 jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
                 imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
                 jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);

                 //Point po = surface->modeltoscreen(Point(0.0,0.0,0.0));
                 Point po = surface->modeltoscreen(pmin);
                 Point pb = surface->modeltoscreen(Point(pmin.x+(pmax.x - pmin.x) * xspacing,pmin.y+(pmax.y - pmin.y) * yspacing,0.0));
                 int dx = pb.x - po.x;
                 int dy = pb.y - po.y;
                 int maxXlines = 50000;
                 int maxYlines = 50000;
                 if( (numLines * (imax-imin) < maxXlines) && (numLines * (jmax-jmin) < maxYlines) && (min(dx,dy) > 10))
                 {
                     Transform lastmodeltransform;
                     for (i = imin ; i <= imax ; i++)
                     {
                       for (j = jmin ; j <= jmax ; j++)
                       {
                            //infile.position(0);
                            //infile.readheader();
                            surface->gettransform(&tview);
                            lastmodeltransform = surface->modeltransform;

                            if (transform != NULL)
                              surface->applytransform(*transform);

                            t1 = t1.identity();
                            t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                            t2.translate(origina + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing);
                            t1.apply(t2);
                            //surface->applytransform(t1);
                            surface->modeltransform = surface->modeltransform * t1;

                            entityCache.start();
                            while((e = entityCache.next()) != NULL )
                            //while ((e = infile.nextentity(NULL)) != NULL)
                            {
                                 e->qgi=0;
                                 e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                                 e->setvisible(*getvisible());
                                 if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                                 {
                                     //qDebug() << "drawing pattern entity GL";
                                     double tdtol = db.getdtoler();
                                     //db.setdtoler(tdtol*2.0);
                                     e->drawGL(drawmode,transform,surface);
                                     db.setdtoler(tdtol);
                                 }
                            }
                            surface->modeltransform = lastmodeltransform;
                            surface->settransform(tview);
                       }
                     }
                 }
                 else
                 {
                     // too many small patters or pixel converage is too small
                     // use a transparent plane fill
                     SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

                     // filled non shaded transparent plane
                     //double rc = db.colourtable[getcolour()].red/255.0;
                     //double gc = db.colourtable[getcolour()].green/255.0;
                     //double bc = db.colourtable[getcolour()].blue/255.0;
                     //SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,0.25);
                     SetNoMaterial(fr,fg,fb,0.25);

                     //polygon3d.transform(&rcsurface->modeltransform);
                     //if(transform)
                     //   polygon3d.transform(transform);
                     //rcsurface->modeltransform.dump("Gl transform");
                     DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
                 }

                 glStencilMask(0xFF);
                 glClear(GL_STENCIL_BUFFER_BIT);
                 glDisable(GL_STENCIL_TEST);

              }

         }
         //infile.close();
       }

  }
  else if (! options.test(PlaneNoFill))
  {
      SetNoMaterial(fr,fg,fb);

       if (transform != 0)
         polygon3d.transform(transform);
       //polygon3d.transform(&surface->modeltransform);

       // limit hatching to not draw if the space between is less than the 2 line widths in pixels
       // in either direction
       Point po = surface->modeltoscreen(Point(0.0,0.0,0.0));
       Point pb = surface->modeltoscreen(Point(xspacing,yspacing,0.0));
       int dx = pb.x-po.x;
       int dy = pb.y-po.y;
       int cw = db.getlineweightdisplay() ? db.lineweights.weighttopx(weight) : db.lineweights.weighttopx(0);

       //qDebug() << "dx :  " << dx << " w :" << cw;
       //qDebug() << "dy :  " << dx << " w :" << cw;

       if(dx > cw && dy > cw)
       //if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10) // this is the orignal hatching limiter
       {
        for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            o = origin;
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return;
            n1 /= len;
            if (i == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;

            /*
            // kmj : changed for gl dragging in a figure
            o  = surface->modeltransform.transform(origin);
            n1 = surface->modeltransform.transform(n1+origin) - o;
            n2 = surface->modeltransform.transform(n2+origin) - o ;
            dir1 = surface->modeltransform.transform(dir1+origin) - o;

            if (transform != NULL)
            {
                 o = transform->transform(origin);
                 n1 = transform->transform(n1+origin) - o;
                 n2 = transform->transform(n2+origin) - o;
                 dir1 = transform->transform(dir1+origin) - o;
            }
            */
            if (transform == NULL)
            {
                o  = surface->modeltransform.transform(origin);
                n1 = surface->modeltransform.transform(n1+origin) - o;
                n2 = surface->modeltransform.transform(n2+origin) - o ;
                dir1 = surface->modeltransform.transform(dir1+origin) - o;
            }
            else
            {
                o  = transform->transform(surface->modeltransform.transform(origin));
                n1 = transform->transform(surface->modeltransform.transform(n1+origin)) - o;
                n2 = transform->transform(surface->modeltransform.transform(n2+origin)) - o ;
                dir1 = transform->transform(surface->modeltransform.transform(dir1+origin)) - o;
            }


            if (options.test(PlaneAnchored))
              o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));
            n = 0;
            p1 = o;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,drawlineGL,&dldata) >= 0)
              {  p1 += n2;  n++;
              }
            p1 = o - n2;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,drawlineGL,&dldata) <= 0)
              {  p1 -= n2;  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }
       }
       else
       {
           // too many small patters or pixel converage is too small
           // use a transparent plane fill
           SetLight(surface->getview()->geteye().x*1000000.0,surface->getview()->geteye().y*1000000.0,surface->getview()->geteye().z*1000000.0);

           // filled non shaded transparent plane
           double rc = db.colourtable[getcolour()].red/255.0;
           double gc = db.colourtable[getcolour()].green/255.0;
           double bc = db.colourtable[getcolour()].blue/255.0;

           SetMaterialEx(rc,gc,bc,1.0,0.0,0.0,0.25);
           //polygon3d.transform(&rcsurface->modeltransform);
           if(transform)
              polygon3d.transform(transform);
           //rcsurface->modeltransform.dump("Gl transform");
           DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
       }
    }
}
#endif

void drawEntitiesToBitmap(EntityHeader &h,EntityList *entityList, QImage &image)
{
    Entity *e;
    double umax,vmax,umin,vmin;
    QRectF qrect(image.rect());
    RECT rect;

    // set up the view to draw onto the scene
    View top(_RCT("top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
    View3dSurface surface(&top,-1.0,-1.0,1.0,1.0,0,0);
    rect.top    = image.rect().top();
    rect.bottom = image.rect().bottom();
    rect.left   = image.rect().left();
    rect.right  = image.rect().right();
    surface.setxdotspermm(4.0);
    surface.setydotspermm(4.0);
    surface.sizeevent(rect.right,rect.bottom);
    umin = -surface.getwidthmm() * 0.1 * 1.0;
    umax =  surface.getwidthmm() * 0.9 * 1.0;
    vmin = -surface.getheightmm() * 0.1 * 1.0;
    vmax =  surface.getheightmm() * 0.9 * 1.0;
    surface.setscale((umax - umin) / surface.getwidth() * surface.getxdotspermm());
    surface.updatematrix();

    // need to scale the pattern extents to fit in to the view
    // zoom the view to fit the pattern

    Point pmin,pmax,pmin1,pmax1;
    int c = 0;
    entityList->start();
    while ((e = entityList->next()) != NULL)
      {  if (e->isa(point_entity))
           {  if (c == 0)
                e->extents(&surface,&pmin,&pmax);
              else
                {  e->extents(&surface,&pmin1,&pmax1);
                   if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                   if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                   if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                   if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                   if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                   if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                }
              c++;
           }
      }
    if (pmax.x <= pmin.x || pmax.y <= pmin.y)
      {  pmin.setxyz(-10.0,-10.0,-10.0);
         pmax.setxyz(10.0,10.0,10.0);
      }

    surface.setuvminmax(pmin.x,pmin.y,pmax.x,pmax.y);
    surface.updatematrix();

    // set up the scene to work with the view
    QGraphicsScene scene;
    surface.setScene(&scene);
    surface.setScene(&scene);
    scene.setSceneRect(QRectF());
    scene.clear();

    scene.setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                          ::v.getreal("wn::backgroundcolour.green")*255.0,
                                          ::v.getreal("wn::backgroundcolour.blue")*255.0)));

    surface.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                           ::v.getreal("wn::backgroundcolour.green")*255.0,
                           ::v.getreal("wn::backgroundcolour.blue")*255.0));

    surface.sizeevent(qrect.width(),qrect.height());
    scene.setSceneRect(qrect);
    scene.update(qrect);

    //qDebug() << "scene rect : " << scene.sceneRect();

    entityList->start();
    while((e = entityList->next()) != NULL )
    {
         //qDebug() << "entity is a : " << e->type();
         e->qgi=0;
         e->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
         //e->setvisible(*getvisible());
         if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
         {
             //qDebug() << "drawing plane pattern entity";
             double tdtol=db.getdtoler();
             db.setdtoler(tdtol*4.0);
             e->usedByPlane = true;
             e->draw(DM_NORMAL,NULL,&surface);
             db.setdtoler(tdtol);
         }
    }

    //qDebug() << "scene rect : " << scene.sceneRect();

    // render the scene to the image
    QPainter painter(&image);
    scene.render(&painter);

    // debug
    image.save("pattern.png","PNG");
    //
}

void drawEntitiesToBitmap(EntityHeader &h,EntityList *entityList,Point origin,int imin,int imax,int jmin,int jmax,int xspacing,int yspacing,Point xa,Point ya,QImage &image)
{
    Entity *e;
    double umax,vmax,umin,vmin;
    QRectF qrect(image.rect());
    RECT rect;

    // set up the view to draw onto the scene
    View top(_RCT("top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
    View3dSurface surface(&top,-1.0,-1.0,1.0,1.0,0,0);
    rect.top    = image.rect().top();
    rect.bottom = image.rect().bottom();
    rect.left   = image.rect().left();
    rect.right  = image.rect().right();
    surface.setxdotspermm(4.0);
    surface.setydotspermm(4.0);
    surface.sizeevent(rect.right,rect.bottom);
    umin = -surface.getwidthmm() * 0.1 * 1.0;
    umax =  surface.getwidthmm() * 0.9 * 1.0;
    vmin = -surface.getheightmm() * 0.1 * 1.0;
    vmax =  surface.getheightmm() * 0.9 * 1.0;
    surface.setscale((umax - umin) / surface.getwidth() * surface.getxdotspermm());
    surface.updatematrix();

    // need to scale the pattern extents to fit in to the view
    // zoom the view to fit the pattern

    Point pmin,pmax,pmin1,pmax1;
    int c = 0;
    entityList->start();
    while ((e = entityList->next()) != NULL)
      {  if (e->isa(point_entity))
           {  if (c == 0)
                e->extents(&surface,&pmin,&pmax);
              else
                {  e->extents(&surface,&pmin1,&pmax1);
                   if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                   if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                   if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                   if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                   if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                   if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                }
              c++;
           }
      }
    if (pmax.x <= pmin.x || pmax.y <= pmin.y)
      {  pmin.setxyz(-10.0,-10.0,-10.0);
         pmax.setxyz(10.0,10.0,10.0);
      }

    //surface.setuvminmax(pmin.x,pmin.y,pmax.x,pmax.y);
    surface.setuvminmax(0,0,qrect.width(),qrect.height());
    surface.updatematrix();


    // set up the scene to work with the view
    QGraphicsScene scene;
    surface.setScene(&scene);
    surface.setScene(&scene);
    scene.setSceneRect(QRectF());
    scene.clear();

    scene.setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                          ::v.getreal("wn::backgroundcolour.green")*255.0,
                                          ::v.getreal("wn::backgroundcolour.blue")*255.0)));

    surface.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                           ::v.getreal("wn::backgroundcolour.green")*255.0,
                           ::v.getreal("wn::backgroundcolour.blue")*255.0));

    surface.sizeevent(qrect.width(),qrect.height());
    scene.setSceneRect(qrect);
    scene.update(qrect);

    //qDebug() << "scene rect : " << scene.sceneRect();
#if 0
    entityList->start();
    while((e = entityList->next()) != NULL )
    {
         //qDebug() << "entity is a : " << e->type();
         e->qgi=0;
         e->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
         //e->setvisible(*getvisible());
         if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
         {
             //qDebug() << "drawing plane pattern entity";
             double tdtol=db.getdtoler();
             db.setdtoler(tdtol*4.0);
             e->usedByPlane = true;
             e->draw(DM_NORMAL,NULL,&surface);
             db.setdtoler(tdtol);
         }
    }
#else

    int i,j;
    Transform t1,t2,t3,t4,tview;

    t1.identity();t2.identity();t3.identity();
    //t1.tocoordinatesystem(xa,ya,xa.cross(ya));
    //t2.translate(origin);
    //t1.tocoordinatesystem(Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,0.0,0.0).cross(Point(0.0,1.0,0.0)));
    //t2.translate(Point(0.0,0.0,0.0));
    //t1.apply(t2);

    for (i = 0 ; i <= abs(imax-imin) ; i++)
      for (j = 0 ; j <= abs(jmax-jmin) ; j++)
      {
           //t1.identity();
           //t2.identity();
           //infile.position(0);
           //infile.readheader();
           surface.gettransform(&tview);
           //if (transform != NULL)
           //  surface->applytransform(*transform);
           t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
           //t1.translate(-origin);
           //t2.tocoordinatesystem(xa,ya,xa.cross(ya));
           //t3.scale(xspacing,yspacing,1.0);
           //Point transp = origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing;
           //t1.tocoordinatesystem(Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,0.0,0.0).cross(Point(0.0,1.0,0.0)));
           Point transp = origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing;
           t2.translate(transp);
           //t4.translate(origin);
           t1.apply(t2);
           //t1.apply(t3);
           //t1.apply(t4);
           surface.applytransform(t1);
           entityList->start();
           while((e = entityList->next()) != NULL )
           {
                //qDebug() << "entity is a : " << e->type();
                e->qgi=0;
                e->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                //e->setvisible(*getvisible());
                if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                {
                    //qDebug() << "drawing plane pattern entity";
                    double tdtol=db.getdtoler();
                    db.setdtoler(tdtol*4.0);
                    e->usedByPlane = true;
                    e->draw(DM_NORMAL,NULL,&surface);
                    db.setdtoler(tdtol);
                }
           }
           surface.settransform(tview);
      }
#endif
    //qDebug() << "scene rect : " << scene.sceneRect();

    // render the scene to the image
    QPainter painter(&image);
    scene.render(&painter);

    // debug
    image.save("pattern.png","PNG");
    //
}

void Plane::draw(int drawmode,Transform *transform,View3dSurface *surface)
{CBdrawline_data dldata;
 CadFile infile;
 int i,j,halfway,imin,jmin,imax,jmax,n,r1,g1,b1,r2,g2,b2,gradientfillstyle;
 double a,len;
 Point p1,p2,p3,p4,n1,n2,dir1,o,xa,ya,pmin1,pmax1,pmin2,pmax2;
 Transform t1,t2,tview;
 RCHRGN clipregion;

  QGraphicsItem * qgedges=0;

  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() == surface->getScene())
        surface->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;
      if(qgrenderplane && qgrenderplane->scene() == surface->getScene())
          surface->getScene()->removeItem(qgrenderplane);
      delete qgrenderplane;
      qgrenderplane=0;
      qgplane=0;
  }

  //if(drawmode == DM_ERASE || (drawmode != DM_INVERT && drawmode != DM_PRINT))
  if(drawmode == DM_ERASE)
      return;

  //if(getselected())
  //    return;

  // bounding box check against the view
  Point bmin,bmax;
  extents(0,&bmin,&bmax);
  bmin = surface->modeltoscreen(bmin);
  bmax = surface->modeltoscreen(bmax);
  if(bmax.x < 0 || bmax.y < 0 || bmin.x > surface->getwidth() || bmin.y > surface->getheight())
      return;
  // end of bounding box test

  //qDebug() << "entering Plane::draw";

  // apply the angle to patterns and bitmap fills??
  if (options.test(1) || options.test(4)|| options.test(PlaneGradientFilled))
    {  t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       xa = t1.transform(xaxis);
       ya = t1.transform(yaxis);
    }
  else
    {  xa = xaxis;
       ya = yaxis;
    }

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  surface->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

 /*
  if (drawmode != DM_GL && ! options.test(PlaneDisplayEdges) && ! options.test(PlaneBitmapFilled) &&  ! options.test(PlaneGradientFilled) && ! options.test(PlaneNoFill) && transparency == 0.0 && transform == NULL)
    {  if (surface->displaylist.draw(this,drawmode))
         return;
       if (! surface->displaylist.geterrorstatus())
         addtodisplaylist(surface,0);
       if (surface->displaylist.draw(this,drawmode))
         return;
    }
*/

  Polygon3d polygon3d(origin,xa,ya,list,0,(surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler());
  dldata.surface = surface;
  dldata.transform = transform;
  dldata.qgi=0;

  if (options.test(PlaneDisplayEdges) || drawnhighlighted || drawmode == DM_INVERT || drawmode == DM_SELECT && options.test(PlaneNoFill))
      qgedges = polygon3d.draw(drawmode,transform,surface);

  //  Planes without fill, and no edges are drawn with edges when selected
  //  The next time they are drawn, they are also drawn with edges to remove the highlighting.
  if (drawnhighlighted == 0)
    drawnhighlighted = drawmode == DM_SELECT && options.test(PlaneNoFill);
  else if (drawmode == DM_NORMAL)
    drawnhighlighted = 0;

  if(cadwindow->getcurrentsurface() != 0 && cadwindow->getcurrentsurface()->getrepaintstyle() == Shaded /*&& drawmode == DM_GL*/)
  {
      db.setrequiresopengl(1);

      //if(qgrenderplane)
      //    surface->getScene()->removeItem(qgrenderplane);
      //delete qgrenderplane;
      //qgrenderplane=0;
      //if(qgi)
      //    surface->getScene()->removeItem(qgi);
      //delete qgi;
      //qgi=0;
      //qgplane=0;
      //if(drawmode == DM_ERASE)
      //    return;

      qgrenderplane = new QGraphicsItemGroup();
      //qgi = new QGraphicsItemGroup();
      RCGraphicsPlaneItem * qplane=0;
      Transform transf = surface->modeltransform;
      qplane = new RCGraphicsPlaneItem(this,transf,surface,false);
      QPainterPath qclipPath;
      qclipPath.setFillRule(Qt::OddEvenFill);

      if(qplane)
      {
          qclipPath = polygon3d.setclipregion(transform,surface);
          qplane->qpath = qclipPath;
          qplane->rcdrawmode = drawmode;
          qgplane = qplane;
      }

      if(qplane)
      {
          //if(qplane)
          //    ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
          //if(surface->getScene())
          //    surface->getScene()->addItem(qgi);
          if(qplane)
              ((QGraphicsItemGroup*)qgrenderplane)->addToGroup(qplane);
          if(surface->getScene())
              surface->getScene()->addItem(qgrenderplane);
      }
  }
  else if (options.test(PlaneNoFill))
  {
      if(drawmode != DM_PRINT)
      {
          if(qgi && qgi->scene() == dldata.surface->getScene())
              dldata.surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }

      if(drawmode == DM_ERASE)
          return;

      qgi = new QGraphicsItemGroup();

      if(qgi)
      {
          if(qgedges)
              ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
          if(surface->getScene())
              surface->getScene()->addItem(qgi);
          if(selected)
              qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
          //else
          //    qgi->setZValue(zValue);
      }
  }
  else if (options.test(PlaneFilled))
  {
      RCGraphicsPlaneItem * qplane=0;
      QGraphicsItem *qpoly=0;
      QPainterPath qclipPath;


      if(drawmode != DM_PRINT)
      {
          if(qgi && qgi->scene() == surface->getScene())
              surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }

      if(drawmode == DM_ERASE)
          return;

       qgi = new QGraphicsItemGroup();

       //qclipPath = polygon3d.setclipregion(transform,surface);

       //if( OpenOpenGl == 0 || transparency == 0.0 || drawmode != DM_NORMAL || transform != 0)
       //if(1)
       {
           if (transform != NULL)
               qpoly = polygon3d.draw(drawmode,transform,surface);
           else
           {
               QBrush brush(QColor(GetRValue(surface->getcolour()),
                                   GetGValue(surface->getcolour()),
                                   GetBValue(surface->getcolour())
                                   ,(1.0-transparency)*255));
                                   // ,transparency*255));
               qpoly = polygon3d.fill(drawmode,transform,surface,&brush);
           }
       }
       /*
       else
       {
           qclipPath = polygon3d.setclipregion(transform,surface);

           db.setrequiresopengl(1);

           Transform transf = surface->modeltransform;
           qplane = new RCGraphicsPlaneItem(this,transf,surface,false);

           if(qplane)
           {
               qplane->qpath = qclipPath;
               qplane->qpattern = new QGraphicsItemGroup(qplane);
               if(qplane->qpattern)
               {
                   qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
                }
               qgplane = qplane;
           }

       }
       */


       if(qgi)
       {
           if(qplane)
               ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
           else if(qpoly)
               ((QGraphicsItemGroup*)qgi)->addToGroup(qpoly);
           if(qgedges)
               ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
           if(selected)
               qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
           //else
           //    qgi->setZValue(zValue);
           if(surface->getScene())
               surface->getScene()->addItem(qgi);
       }
  }
  else if (options.test(PlaneBitmapFilled))
  {  //  Set the clipping region

      int texture;
      RCGraphicsPlaneItem * qplane = 0; //new RCGraphicsPlaneItem();
      QPainterPath qclipPath;
      QGraphicsPathItem *qclip=0;

      if(drawmode != DM_PRINT && drawmode != DM_PREVIEW)
      {
          if(qgi && qgi->scene() == surface->getScene())
              surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }

      if(drawmode == DM_ERASE)
          return;

      qgi = new QGraphicsItemGroup();

       polygon3d.clip(surface);

       // don't draw the bitmaps when rubberbanding
       if(drawmode != DM_INVERT)
       {
           if (polygon3d.getdefined())
           {
                polygon3d.fixLoopDirections();

                qclipPath = polygon3d.setclipregion(transform,surface);

                /*
                // KMJ not currently using this
                if (OpenOpenGl != 0 && (transparency != 0.0 || transparencyrange != 0.0)
                        && (drawmode != DM_INVERT && drawmode != DM_PRINT))
                {  //  Make sure that opengl is active on this window
                     db.setrequiresopengl(1);

                     //if(drawmode != DM_INVERT)
                     {
                         // this is drawn with OpenGL
                         Transform transf = surface->modeltransform;
                         qplane = new RCGraphicsPlaneItem(this,transf,surface,false);

                         if(qplane)
                         {
                             qplane->rcbitmap = bitmape;
                             qplane->rcdrawmode = drawmode;
                             qplane->qpath = qclipPath;
                             //qplane->qpattern = new QGraphicsItemGroup(qplane);
                             //if(qplane->qpattern)
                             //    qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
                         }
                     }
                }
                else
                */
                {
                    // this is drawn with the 2D scene painter
                    /*
                    Transform transf;
                    qplane = new RCGraphicsPlaneItem(this,transf);

                    if(qplane)
                    {
                        qplane->qpath = qclipPath;
                        qplane->qpattern = new QGraphicsItemGroup(qplane);
                        if(qplane->qpattern)
                            qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
                    }
                    */

                    // KMJ: Note we are using a transparent consmetic line for the clip path
                    // using a Qt:NoPen does not work with planes after mirroring
                    // ToDo: find the root of the mirroring problem and fix it
                    qclip = new QGraphicsPathItem(qclipPath);
                    qclip->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
                    qclip->setPen(QPen(Qt::NoBrush,1,Qt::NoPen));
                    qclip->setBrush(QBrush(Qt::NoBrush));

#if 0
                    BitMask bo(32);
                    if (transparency != 0.0 || transparencyrange != 0.0)
                      bo.set(9,1);
                    BitMapE bitmap(patternfilename,origin,xa,ya,xspacing,yspacing,red,green,blue,transparencyrange,1.0-transparency,bo);

                    polygon3d.setextents();

                    pmin1 = polygon3d.getpmin();
                    pmax1 = polygon3d.getpmax();

                    pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
                    pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
                    for (i = 0 ; i < 7 ; i++)
                      {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                         p2.x = (p1 - origin).dot(xa);
                         p2.y = (p1 - origin).dot(ya);
                         if (pmin2.x > p2.x) pmin2.x = p2.x;
                         if (pmin2.y > p2.y) pmin2.y = p2.y;
                         if (pmax2.x < p2.x) pmax2.x = p2.x;
                         if (pmax2.y < p2.y) pmax2.y = p2.y;
                      }

                    imin = (int)floor(pmin2.x /  xspacing);
                    jmin = (int)floor(pmin2.y /  yspacing);
                    imax = (int)ceil(pmax2.x /  xspacing);
                    jmax = (int)ceil(pmax2.y /  yspacing);


                    for (i = imin ; i <= imax ; i++)
                      for (j = jmin ; j <= jmax ; j++)
                        {  p1 = origin + xa *  i * xspacing + ya * j * yspacing;
                           p2 = origin + xa * (i+1) * xspacing + ya * (j+1) * yspacing;
                           bitmap.setorigin(p1);
                           if (bitmap.getdefined())
                             bitmap.draw(drawmode,transform,surface);
                           if(bitmap.qgi)
                           {
                               if(qclipPath.elementCount() > 0)
                               {
                                   bitmap.qgi->setFlag(QGraphicsItem::ItemIsSelectable,false);
                                   ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(bitmap.qgi);
                                   bitmap.qgi=0;
                               }
                           }
                        }
#else
                    BitMask bo(32);
                    if(bitmape == 0)
                        bitmape = new BitMapE(patternfilename,origin,xa,ya,xspacing,yspacing,red,green,blue,transparencyrange,1.0-transparency,bo);
                    BitMapE *bitmap = bitmape;
                    bitmap->usedByPlane=true;
                    bo = bitmap->getoptions();
                    if (transparency != 0.0 || transparencyrange != 0.0)
                      bo.set(9,1);
                    bitmap->setoptions(bo);
                    if(bitmap != 0)
                    {
                        polygon3d.setextents();

                        pmin1 = polygon3d.getpmin();
                        pmax1 = polygon3d.getpmax();

                        pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
                        pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
                        for (i = 0 ; i < 7 ; i++)
                          {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                             p2.x = (p1 - origin).dot(xa);
                             p2.y = (p1 - origin).dot(ya);
                             if (pmin2.x > p2.x) pmin2.x = p2.x;
                             if (pmin2.y > p2.y) pmin2.y = p2.y;
                             if (pmax2.x < p2.x) pmax2.x = p2.x;
                             if (pmax2.y < p2.y) pmax2.y = p2.y;
                          }

                        imin = (int)floor(pmin2.x /  xspacing);
                        jmin = (int)floor(pmin2.y /  yspacing);
                        imax = (int)ceil(pmax2.x /  xspacing);
                        jmax = (int)ceil(pmax2.y /  yspacing);

                        for (i = imin ; i <= imax ; i++)
                        {
                          for (j = jmin ; j <= jmax ; j++)
                          {
                               p1 = origin + xa *  i * xspacing + ya * j * yspacing;
                               p2 = origin + xa * (i+1) * xspacing + ya * (j+1) * yspacing;
                               bitmap->setorigin(p1);
                               bitmap->setxaxis(xa);// added 2015/05/25 compsed print??
                               bitmap->setyaxis(ya);//  added 2015/05/25 composed print ???
                               //qDebug() << "drawing bitmap in plane";
                               //qDebug() << "p1 x : " << p1.x;
                               //qDebug() << "p1 y : " << p1.y;
                               //qDebug() << "bm width step  :" << bitmap->getwidth();
                               //qDebug() << "bm height step :" << bitmap->getheight();
                               if (bitmap->getdefined())
                                 bitmap->draw(drawmode,transform,surface);
                               if(bitmap->qgi)
                               {
                                   if(qclipPath.elementCount() > 0)
                                   {
                                       bitmap->qgi->setFlag(QGraphicsItem::ItemIsSelectable,false);
                                       //((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(bitmap->qgi);
                                       if(qclip)
                                           if(bitmap->qgi)
                                               bitmap->qgi->setParentItem(qclip);
                                       //qDebug() << "bit map is clipped : " << bitmap->qgi->isClipped();
                                       //qDebug() << "bitmap is obscured by clipper  : " << bitmap->qgi->isObscuredBy(qclip);
                                       //qDebug() << "bitmap is contained by clipper : " << bitmap->qgi->collidesWithItem(qclip,Qt::ContainsItemShape);
                                       //qDebug() << "bitmap intersects with clipper : " << bitmap->qgi->collidesWithItem(qclip,Qt::IntersectsItemShape);
                                       bitmap->qgi=0;
                                   }
                               }
                            }
                        }
                    }
#endif
                }
             }
       }

       if(qgi)
       {
           if(surface->getScene())
               surface->getScene()->addItem(qgi);
           if(qclip)
           {
               //(qclip)->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qclip);
           }
           else
           {
               if(qplane)
               {
                   //qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
                   qgplane = qplane;
               }
           }
           if(qgedges)
           {
               //qgedges->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
           }
           if(selected)
               qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
          // else
           //    qgi->setZValue(zValue);
       }
  }
  else if (options.test(PlaneGradientFilled))
  {
      //  Set the clipping region
      int texture;
      RCGraphicsPlaneItem * qplane = 0; //new RCGraphicsPlaneItem(this);
      QPainterPath qclipPath;
      QGraphicsItem *qpoly=0,*qfill=0;
      QGraphicsPathItem *qclip=0;

      if(drawmode != DM_PRINT)
      {
          if(qgi && qgi->scene() == dldata.surface->getScene())
              dldata.surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }

      if(drawmode == DM_ERASE)
          return;

      qgi = new QGraphicsItemGroup();

       // removed this to fix problem when
       // clipping against composed views
       //polygon3d.clip(surface);

       if (polygon3d.getdefined())
       {
            qclipPath = polygon3d.setclipregion(transform,surface);
     
            polygon3d.setextents();

            pmin1 = polygon3d.getpmin();
            pmax1 = polygon3d.getpmax();

            pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
            pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
            for (i = 0 ; i < 7 ; i++)
              {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                 p2.x = (p1 - origin).dot(xa);
                 p2.y = (p1 - origin).dot(ya);
                 if (pmin2.x > p2.x) pmin2.x = p2.x;
                 if (pmin2.y > p2.y) pmin2.y = p2.y;
                 if (pmax2.x < p2.x) pmax2.x = p2.x;
                 if (pmax2.y < p2.y) pmax2.y = p2.y;
              }
            pmin2.z = pmax2.z = 0.0;
             
            imin = (int)floor(pmin2.x /  xspacing);
            jmin = (int)floor(pmin2.y /  yspacing);
            imax = (int)ceil(pmax2.x /  xspacing);
            jmax = (int)ceil(pmax2.y /  yspacing);

            if (imin >= imax)
              imin = imax - 1;
            if (jmin >= jmax)
              jmin = jmax - 1;

            /*
            if (OpenOpenGl != 0 && transparency != 0.0 && drawmode == DM_NORMAL && transform == 0)
            {  //  Make sure that opengl is active on this window
                 db.setrequiresopengl(1);

                 // this will draw using OpenGL
                 Transform transf = surface->modeltransform;
                 qplane = new RCGraphicsPlaneItem(this,transf,surface,false);

                 if(qplane)
                 {
                     qplane->rcdrawmode = drawmode;
                     qplane->qpath = qclipPath;
                 }
            }
            else
            */
            {
                // this will draw using the 2D scene painting
                /*
                Transform transf;
                qplane = new RCGraphicsPlaneItem(this,transf);

                if(qplane)
                {
                    qplane->qpath = qclipPath;
                    qplane->qpattern = new QGraphicsItemGroup(qplane);
                    if(qplane->qpattern)
                    {
                        qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
                    }
                }
                */

                qclip = new QGraphicsPathItem(qclipPath);
                qclip->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
                //qclip->setPen(Qt::NoPen);
                qclip->setPen(QPen(Qt::NoBrush,1,Qt::NoPen));
                qclip->setBrush(Qt::NoBrush);
                //qclip->setBoundingRegionGranularity(1);

                RCCOLORREF thisgradientcolour;
                if (surface->getblackandwhite() == 1) // back and white
                {
                     if (GetRValue(gradientcolour) == 255 &&
                         GetGValue(gradientcolour) == 255 &&
                         GetBValue(gradientcolour) == 255)
                         thisgradientcolour = RGB(255,255,255);
                     else
                       thisgradientcolour = RGB(0,0,0);
                }
                else if(surface->getblackandwhite() == 2) // greyscale
                {
                    if (GetRValue(gradientcolour) == 255 &&
                        GetGValue(gradientcolour) == 255 &&
                        GetBValue(gradientcolour) == 255)
                      thisgradientcolour =  RGB(255,255,255);
                    else
                    {
                        // using the Luminosity method
                        int grey = (GetRValue(gradientcolour)*0.21+GetGValue(gradientcolour)*0.72+GetBValue(gradientcolour)*0.07);
                        thisgradientcolour = RGB(grey,grey,grey);
                    }
                }
                else
                    thisgradientcolour = gradientcolour;

                r1 = GetRValue(surface->getcolour());
                g1 = GetGValue(surface->getcolour());
                b1 = GetBValue(surface->getcolour());

                r2 = GetRValue(thisgradientcolour);
                g2 = GetGValue(thisgradientcolour);
                b2 = GetBValue(thisgradientcolour);


                gradientfillstyle = options.test(PlaneGradientStyle1) * 8 + options.test(PlaneGradientStyle2) * 4 +  options.test(PlaneGradientStyle3) * 2 +  options.test(PlaneGradientStyle4);

                if (gradientfillstyle == 0)
                  {
                     for (i = imin ; i <= imax ; i++)
                       {  p1 = origin + xa * i * xspacing + ya * jmin * yspacing;
                          p2 = origin + xa * (i+1) * xspacing + ya * jmin * yspacing;
                          p3 = origin + xa * (i+1) * xspacing + ya * jmax * yspacing;
                          p4 = origin + xa * i * xspacing + ya * jmax * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-imin))/(imax - imin),g1 + ((g2 - g1)*(i-imin))/(imax - imin),b1 + ((b2 - b1)*(i-imin))/(imax - imin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);

                       }
                  }
                else if (gradientfillstyle == 1)
                  {

                    for (i = imin ; i <= imax ; i++)
                       {  p1 = origin + xa *  i * xspacing + ya * jmin * yspacing;
                          p2 = origin + xa * (i+1) * xspacing + ya * jmin * yspacing;
                          p3 = origin + xa * (i+1) * xspacing + ya * jmax * yspacing;
                          p4 = origin + xa * i * xspacing + ya * jmax * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-imin))/(imax - imin),g2 + ((g1 - g2)*(i-imin))/(imax - imin),b2 + ((b1 - b2)*(i-imin))/(imax - imin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 2)
                  {

                    for (j = jmin ; j <= jmax ; j++)
                       {  p1 = origin + xa * imin * xspacing + ya * j * yspacing;
                          p2 = origin + xa * imax * xspacing + ya * j * yspacing;
                          p3 = origin + xa * imax * xspacing + ya * (j+1) * yspacing;
                          p4 = origin + xa * imin * xspacing + ya * (j+1) * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-jmin))/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin))/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin))/(jmax - jmin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 3)
                  {  for (j = jmin ; j <= jmax ; j++)
                       {  p1 = origin + xa * imin * xspacing + ya * j * yspacing;
                          p2 = origin + xa * imax * xspacing + ya * j * yspacing;
                          p3 = origin + xa * imax * xspacing + ya * (j+1) * yspacing;
                          p4 = origin + xa * imin * xspacing + ya * (j+1) * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-jmin))/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin))/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin))/(jmax - jmin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 4)
                  {  for (i = imin ; i <= imax ; i++)
                       {  p1 = origin + xa *  i * xspacing + ya * jmin * yspacing;
                          p2 = origin + xa * (i+1) * xspacing + ya * jmin * yspacing;
                          p3 = origin + xa * (i+1) * xspacing + ya * jmax * yspacing;
                          p4 = origin + xa * i * xspacing + ya * jmax * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          halfway = (imin + imax) / 2;
                          if (i < halfway)
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-imin)*2)/(imax - imin),g1 + ((g2 - g1)*(i-imin)*2)/(imax - imin),b1 + ((b2 - b1)*(i-imin)*2)/(imax - imin)),transparency);
                          else
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-halfway)*2)/(imax - imin),g2 + ((g1 - g2)*(i-halfway)*2)/(imax - imin),b2 + ((b1 - b2)*(i-halfway)*2)/(imax - imin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                    }
                  }
                else if (gradientfillstyle == 5)
                  {  for (i = imin ; i <= imax ; i++)
                       {  p1 = origin + xa *  i * xspacing + ya * jmin * yspacing;
                          p2 = origin + xa * (i+1) * xspacing + ya * jmin * yspacing;
                          p3 = origin + xa * (i+1) * xspacing + ya * jmax * yspacing;
                          p4 = origin + xa * i * xspacing + ya * jmax * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          halfway = (imin + imax) / 2;
                          if (i < halfway)
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(i-imin)*2)/(imax - imin),g2 + ((g1 - g2)*(i-imin)*2)/(imax - imin),b2 + ((b1 - b2)*(i-imin)*2)/(imax - imin)),transparency);
                          else
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(i-halfway)*2)/(imax - imin),g1 + ((g2 - g1)*(i-halfway)*2)/(imax - imin),b1 + ((b2 - b1)*(i-halfway)*2)/(imax - imin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 6)
                  {  for (j = jmin ; j <= jmax ; j++)
                       {  p1 = origin + xa * imin * xspacing + ya * j * yspacing;
                          p2 = origin + xa * imax * xspacing + ya * j * yspacing;
                          p3 = origin + xa * imax * xspacing + ya * (j+1) * yspacing;
                          p4 = origin + xa * imin * xspacing + ya * (j+1) * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          halfway = (jmin + jmax) / 2;
                          if (j < halfway)
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-jmin)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-jmin)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-jmin)*2)/(jmax - jmin)),transparency);
                          else
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-halfway)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-halfway)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-halfway)*2)/(jmax - jmin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 7)
                  {
                    for (j = jmin ; j <= jmax ; j++)
                       {  p1 = origin + xa * imin * xspacing + ya * j * yspacing;
                          p2 = origin + xa * imax * xspacing + ya * j * yspacing;
                          p3 = origin + xa * imax * xspacing + ya * (j+1) * yspacing;
                          p4 = origin + xa * imin * xspacing + ya * (j+1) * yspacing;
                          Polygon3d polygon3d(p1,p2,p3,p4);
                          halfway = (jmin + jmax) / 2;
                          if (j < halfway)
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(j-jmin)*2)/(jmax - jmin),g2 + ((g1 - g2)*(j-jmin)*2)/(jmax - jmin),b2 + ((b1 - b2)*(j-jmin)*2)/(jmax - jmin)),transparency);
                          else
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r1 + ((r2 - r1)*(j-halfway)*2)/(jmax - jmin),g1 + ((g2 - g1)*(j-halfway)*2)/(jmax - jmin),b1 + ((b2 - b1)*(j-halfway)*2)/(jmax - jmin)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);
                       }
                  }
                else if (gradientfillstyle == 8 || gradientfillstyle == 9)
                  {double radius,radius1,radius2;

                     radius = (pmax2 - pmin2).length() / 2.0;
                     radius1 = 0.0;
                     radius2 = xspacing;
                     i = 0;
                     imax = int(radius / xspacing + 0.5);
                     if (imax < 1) imax = 1;
                     while (radius1 < radius)
                       {  p1 = origin + xa * (pmin2.x + pmax2.x) / 2.0 + ya * (pmin2.y + pmax2.y) / 2.0;
                          Circle circle1(p1,xa,ya,radius1,0.0,acos(-1.0)*2.0);
                          Circle circle2(p1,xa,ya,radius2,0.0,acos(-1.0)*2.0);
                          EntityList list;
                          if (radius1 > 0.0)
                            {  list.add(&circle1);
                               list.add(0);
                            }
                          list.add(&circle2);
                          Polygon3d polygon3d(p1,xa,ya,list,0,(surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler());
                          if (gradientfillstyle == 8)
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*i)/imax,g2 + ((g1 - g2)*i)/imax,b2 + ((b1 - b2)*i/imax)),transparency);
                          else
                            qfill = FillGradientPolygon(&polygon3d,surface,drawmode,transform,RGB(r2 + ((r1 - r2)*(imax-i))/imax,g2 + ((g1 - g2)*(imax-i))/imax,b2 + ((b1 - b2)*(imax-i)/imax)),transparency);

                          //if(qfill)
                          //    if(qplane->qpattern)
                          //        ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(qfill);
                          if(qclip)
                              if(qfill)
                                qfill->setParentItem(qclip);

                          radius1 += xspacing;
                          radius2 += xspacing;
                          i++;
                       }
                  }
             }

       }

       if(qgi)
       {
           if(qclip)
           {
               //qclip->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qclip);
           }
           else
           {
               if(qplane)
               {
                   //qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
                   qgplane = qplane;
               }
               if(qfill)
               {
                   //qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
               }
           }
           if(qgedges)
           {
                //qgedges->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
           }
           if(selected)
               qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
           //else
           //    qgi->setZValue(zValue);
           if(surface->getScene())
               surface->getScene()->addItem(qgi);
       }
  }
  else if (options.test(PlanePatterned))
  {
      Transform transf;
      RCGraphicsPlaneItem * qplane = 0;//new RCGraphicsPlaneItem(this,transf);
      QPainterPath qclipPath;
      QGraphicsPathItem *qclip=0;

      if(drawmode != DM_PRINT)
      {
          if(qgi && qgi->scene() == dldata.surface->getScene())
              dldata.surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }

      if(drawmode == DM_ERASE)
          return;

      qgi = new QGraphicsItemGroup();

      polygon3d.clip(surface);

      // don't draw the pattern when rubberbanding
      //if(drawmode != DM_INVERT)
      if(drawmode != DM_INVERT)
      {
           if (patternfilename && infile.open(patternfilename,"rb"))
           {
             if (infile.readheader())
             {
                Entity *e;
                Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,p1,p2,pmin1,pmax1,pmin2,pmax2;

                EntityList entityCache;

                int numLines=0;
                entityCache.start();
                while ((e = infile.nextentity(NULL)) != NULL)
                {
                    entityCache.add(e);
                    numLines += getNumDrawSegments(e,surface);
                }

                // get the model units of the pattern file
                int patternUnits=0;
                Values patternValues(1);
                infile.nextentity(NULL,&patternValues);
                patternUnits = patternValues.getinteger("db::units");

                infile.close();

                //qDebug() << "drawing pattern plane";
                //  Set the clipping region
                //polygon3d.clip(surface);

                if (polygon3d.getdefined())
                  {

                    // debug - fix clippath directions???
                    /*
                    polyContainsTest();
                    std::vector<Point> polygon;
                    polygon.push_back(Point(0,0,0));
                    polygon.push_back(Point(100,0,0));
                    polygon.push_back(Point(100,100,0));
                    polygon.push_back(Point(0,100,0));
                    polyContainsTest(polygon,Point(50,50,0));

                    polygon3d.findLoopContainments();

                    polygon3d.fixLoopDirections();
                    qDebug() << "*** loop containments ***";

                    for(int L=0; L<polygon3d.getnloops(); L++)
                    {
                        qDebug() << "loop : " << L << " = " << polygon3d.loopContainments[L];
                    }
                    qDebug() <<"**************************";
                    */
                    // debug

                     polygon3d.fixLoopDirections();

                     qclipPath = polygon3d.setclipregion(transform,surface);
                     //qclipPath.setFillRule(Qt::OddEvenFill);

                     /*
                     if(qplane)
                     {
                         qplane->qpath = qclipPath;
                         qplane->qpattern = new QGraphicsItemGroup(qplane);
                         if(qplane->qpattern)
                             qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
                     }
                     */

                     // put the pattern into a bitmap and draw that instead of a pattern
                     // create a an image
                     // create a scene the same size as the image
                     // render the scene to the bitmap
                     // use the bitmap in the plane
#if 0
                     /*
                     // convert the pattern to its bitmap equvalent method - not good bad resolution too much memory used
                     EntityHeader bmheader(getcolour(),getlayer(),getstyle(),getweight());
                     QImage pattBm(512,512,QImage::Format_RGB32);
                     pattBm.fill(QColor(255,255,255));
                     drawEntitiesToBitmap(bmheader,&entityCache,pattBm);
                     pattBm.save("temp.jpg","JPG");
                     */

                     qclip = new QGraphicsPathItem(qclipPath);
                     qclip->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
                     //qclip->setPen(QPen(QColor(255,0,0)));
                     qclip->setPen(Qt::NoPen);
                     qclip->setBrush(Qt::NoBrush);

                     polygon3d.setextents();

                     // one big bitmap to cover the whole plane

                     //  Calculate the extents of the pattern from the point entities
                     entityCache.start();
                     while((e = entityCache.next()) != NULL )
                       {  if (e->isa(point_entity))
                            {  p = ((PointE *)e)->getp();
                               if (p.x < pmin.x) pmin.x = p.x;
                               if (p.y < pmin.y) pmin.y = p.y;
                               if (p.z < pmin.z) pmin.z = p.z;
                               if (p.x > pmax.x) pmax.x = p.x;
                               if (p.y > pmax.y) pmax.y = p.y;
                               if (p.z > pmax.z) pmax.z = p.z;
                            }
                       }

                     if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                       {  pmin.setxyz(-10.0,-10.0,-10.0);
                          pmax.setxyz(10.0,10.0,10.0);
                       }

                     pmin1 = polygon3d.getpmin();
                     pmax1 = polygon3d.getpmax();

                     pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
                     pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
                     for (i = 0 ; i < 7 ; i++)
                       {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                          p2.x = (p1 - origin).dot(xa);
                          p2.y = (p1 - origin).dot(ya);
                          if (pmin2.x > p2.x) pmin2.x = p2.x;
                          if (pmin2.y > p2.y) pmin2.y = p2.y;
                          if (pmax2.x < p2.x) pmax2.x = p2.x;
                          if (pmax2.y < p2.y) pmax2.y = p2.y;
                       }

                     t1.tocoordinatesystem(xa,ya,xa.cross(ya));
                     t2.translate(origin);
                     t1.apply(t2);

                     imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
                     jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
                     imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
                     jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);

                     Point testpts[4] = { Point(pmin.x,pmin.y,0.0),Point(pmax.x,pmin.y,0.0),Point(pmax.x,pmax.y,0.0),Point(pmin.x,pmax.y,0.0) };
                     // get the bounding box for all the translated pattern repeats
                     Point bmtmin(FLT_MAX,FLT_MAX,FLT_MAX),bmtmax(-FLT_MAX,-FLT_MAX,-FLT_MAX);
                     for (i = imin ; i <= imax ; i++)
                     {
                       for (j = jmin ; j <= jmax ; j++)
                       {
                           Transform t1,t2;
                           t1.identity();t2.identity();
                           t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                           t2.translate(origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing);
                           t1.apply(t2);
                           for(int itp=0; itp < 4; itp++)
                           {
                                Point tp = t1.transform(testpts[itp]);
                                if (tp.x < bmtmin.x) bmtmin.x = tp.x;
                                if (tp.y < bmtmin.y) bmtmin.y = tp.y;
                                if (tp.x > bmtmax.x) bmtmax.x = tp.x;
                                if (tp.y > bmtmax.y) bmtmax.y = tp.y;
                           }
                       }
                     }

                     // debug - draw the image box
                     /*
                     Point bbp1(bmtmin.x,bmtmin.y,0.0),bbp2(bmtmax.x,bmtmin.y,0.0),bbp3(bmtmax.x,bmtmax.y,0.0),bbp4(bmtmin.x,bmtmax.y,0.0);
                     Line line1(bbp1,bbp2);line1.draw(drawmode,transform,surface);
                     Line line2(bbp2,bbp3);line2.draw(drawmode,transform,surface);
                     Line line3(bbp3,bbp4);line3.draw(drawmode,transform,surface);
                     Line line4(bbp4,bbp1);line4.draw(drawmode,transform,surface);
                     */
                     // debug

                     // draw the patterns onto the image
                     int bmw = bmtmax.x - bmtmin.x;
                     int bmh = bmtmax.y - bmtmin.y;
                     Point ptorigin(((bmtmax.x - bmtmin.x) / 2.0) - ((pmax.x-pmin.x) / 2.0),0.0,0.0);
                     if(xa.y == 0.0)
                         ptorigin = Point(0.0,0.0,0.0);
                     EntityHeader bmheader(getcolour(),getlayer(),getstyle(),getweight());
                     QImage pattBm(bmw,bmh,QImage::Format_RGB32);
                     pattBm.fill(QColor(255,255,255));
                     drawEntitiesToBitmap(bmheader,&entityCache,ptorigin,imin,imax,jmin,jmax,xspacing,yspacing,xa,ya,pattBm);
                     pattBm.save("temp.png","PNG");

                     if(bitmape !=0)
                         delete bitmape;

                     Point bmorigin(0.0,0.0,0.0);
                     bmorigin.x = bmtmin.x;
                     bmorigin.y = bmtmin.y;
                     bmorigin.z = bmtmin.z + (bmtmax.z - bmtmin.z) * 0.5;

                     BitMask bo(32);
                     bo.set(9,1);
                     BitMapE *bitmape = new BitMapE(_RCT("temp.png"),bmorigin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),bmw,bmh,1.0,1.0,1.0,0.5,1.0,bo);
                     bitmape->usedByPlane=true;

                     if (bitmape->getdefined())
                        bitmape->draw(drawmode,transform,surface);
                     if(bitmape->qgi)
                     {
                         if(qclipPath.elementCount() > 0)
                         {
                             bitmape->qgi->setFlag(QGraphicsItem::ItemIsSelectable,false);
                             if(qclip)
                                 if(bitmape->qgi)
                                     bitmape->qgi->setParentItem(qclip);
                             bitmape->qgi=0;
                         }
                     }
#else

                     qclip = new QGraphicsPathItem(qclipPath);
                     qclip->setFlag(QGraphicsItem::ItemClipsChildrenToShape );
                     qclip->setPen(QPen(Qt::NoBrush,1,Qt::NoPen));
                     qclip->setBrush(QBrush(Qt::NoBrush));
                     // for debug
                     //qclip->setPen(QPen(QBrush(QColor(0,255,0),Qt::SolidPattern),5));
                     //

                     // debug - draw clip start points
                     //polygon3d.draw(drawmode,transform,surface);
                     // debug

                     polygon3d.setextents();

                     //  Calculate the extents of the pattern from the point entities
                     entityCache.start();
                     while((e = entityCache.next()) != NULL )
                     //while ((e = infile.nextentity(NULL)) != NULL)
                       {  if (e->isa(point_entity))
                            {  p = ((PointE *)e)->getp();
                               if (p.x < pmin.x) pmin.x = p.x;
                               if (p.y < pmin.y) pmin.y = p.y;
                               if (p.z < pmin.z) pmin.z = p.z;
                               if (p.x > pmax.x) pmax.x = p.x;
                               if (p.y > pmax.y) pmax.y = p.y;
                               if (p.z > pmax.z) pmax.z = p.z;
                            }
                          //delete e;
                       }

                     if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                       {  pmin.setxyz(-10.0,-10.0,-10.0);
                          pmax.setxyz(10.0,10.0,10.0);
                       }

                     pmin1 = polygon3d.getpmin();
                     pmax1 = polygon3d.getpmax();

                     pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
                     pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);

                     for (i = 0 ; i < 7 ; i++)
                       {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                          p2.x = (p1 - origin).dot(xa);
                          p2.y = (p1 - origin).dot(ya);
                          if (pmin2.x > p2.x) pmin2.x = p2.x;
                          if (pmin2.y > p2.y) pmin2.y = p2.y;
                          if (pmax2.x < p2.x) pmax2.x = p2.x;
                          if (pmax2.y < p2.y) pmax2.y = p2.y;
                       }

                     //int len = entityCache.length();
                     //int area = (pmax2.x - pmin2.x) * (pmax2.y - pmin2.y) ;
                     //int density = area / len;
                     t1 = t1.identity();
                     t1.tocoordinatesystem(xa,ya,xa.cross(ya));
                     t2.translate(origin);
                     t1.apply(t2);

                     imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
                     jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
                     imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
                     jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);

                     // debug test the box transformations
                     /*
                     Transform cview;
                     for (i = imin ; i <= imax ; i++)
                     {
                       for (j = jmin ; j <= jmax ; j++)
                       {
                           surface->gettransform(&cview);
                           if (transform != NULL)
                             surface->applytransform(*transform);
                           Point testpts[4] = { Point(pmin.x,pmin.y,0.0),Point(pmax.x,pmin.y,0.0),Point(pmax.x,pmax.y,0.0),Point(pmin.x,pmax.y,0.0) };
                           Transform t1,t2;
                           t1.identity();t2.identity();
                           t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                           t2.translate(origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing);
                           t1.apply(t2);
                           for(int itp=0; itp < 4; itp++)
                           {
                                Point tp = t1.transform(testpts[itp]);
                                //if (transform == 0)
                                //  tp = surface->modeltoscreen(tp);
                                //else
                                //  tp = surface->modeltoscreen(transform->transform(tp));
                                //surface->transform2d(&tp.x,&tp.y);
                                //surface->screentomodel(tp);
                                testpts[itp] = tp;
                           }
                           Point bbp1(testpts[0].x,testpts[0].y,0.0),bbp2(testpts[1].x,testpts[1].y,0.0),bbp3(testpts[2].x,testpts[2].y,0.0),bbp4(testpts[3].x,testpts[3].y,0.0);
                           Line line1(bbp1,bbp2);line1.draw(drawmode,0,surface);
                           Line line2(bbp2,bbp3);line2.draw(drawmode,0,surface);
                           Line line3(bbp3,bbp4);line3.draw(drawmode,0,surface);
                           Line line4(bbp4,bbp1);line4.draw(drawmode,0,surface);
                           surface->settransform(cview);
                       }
                     }
                     */
                     // debug
                     qDebug() << "width : " << min(xspacing,yspacing) * surface->getscale();
                     qDebug() << "width : " << db.lineweights.weighttomm(getweight()) * 3;

                     // pending tests to stop hanging when too many pattern to display
                     //if((numLines * (imax-imin)*(jmax-jmin) < 1000000) ||
                     //   (min(xspacing,yspacing) * surface->getscale() > db.lineweights.weighttomm(getweight()) * 3) )
                     //if(Finished)
                     {

                     //program->resetinterrupt();
                     //app->processEvents();

                     //Finished=0;
                     for (i = imin ; i <= imax ; i++)
                     {
                       for (j = jmin ; j <= jmax ; j++)
                       {
                            //app->processEvents();
                            //infile.position(0);
                            //infile.readheader();
                            surface->gettransform(&tview);
                            if (transform != NULL)
                              surface->applytransform(*transform);
                            t1 = t1.identity();
                            t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                            t2.translate(origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing);
                            t1.apply(t2);
                            surface->applytransform(t1);

                            // get the bounding points of this patterns region
                            // translate it to the screen
                            // test to see if it is outside the clip path

                            Point pbox[] { Point(pmin.x,pmin.y,0.0),Point(pmax.x,pmin.y,0.0),Point(pmax.x,pmax.y,0.0),Point(pmin.x,pmax.y,0.0) };
                            int ibp=0,inpts=0;
                            for(ibp=0; ibp < 4; ibp++)
                            {
                                Point tp = pbox[ibp];

                                if (transform == 0)
                                  tp = surface->modeltoview(tp);
                                else
                                  tp = surface->modeltoview(transform->transform(tp));
                                tp = surface->viewtoscreen(tp);
                                surface->transform2d(&tp.x,&tp.y);

                                pbox[ibp] = tp;

                                QPointF qtp(tp.x,tp.y);
                                if(qclipPath.contains(qtp))
                                   inpts++;
                            }
                            if(inpts == 0)
                            {
                                // check for intersections
                                QPolygonF qtpp;
                                qtpp << QPointF(pbox[0].x,pbox[0].y) << QPointF(pbox[1].x,pbox[1].y) << QPointF(pbox[2].x,pbox[2].y) << QPointF(pbox[3].x,pbox[3].y) << QPointF(pbox[0].x,pbox[0].y);
                                QPainterPath qtpb; qtpb.addPolygon(qtpp);
                                if(!qclipPath.intersects(qtpb))
                                {
                                    surface->settransform(tview);
                                    continue;
                                }
                            }

                            // debug - draw the box
                            /*
                            Point bbp1(pbox[0].x,pbox[0].y,0.0),bbp2(pbox[1].x,pbox[1].y,0.0),bbp3(pbox[2].x,pbox[2].y,0.0),bbp4(pbox[3].x,pbox[3].y,0.0);
                            Line line1(bbp1,bbp2);line1.draw(drawmode,0,surface);
                            Line line2(bbp2,bbp3);line2.draw(drawmode,0,surface);
                            Line line3(bbp3,bbp4);line3.draw(drawmode,0,surface);
                            Line line4(bbp4,bbp1);line4.draw(drawmode,0,surface);
                            */
                            // debug

                            // debug - draw the path
                            /*
                            QPolygonF dbpoly = qclipPath.toFillPolygon(QMatrix());
                            QGraphicsPolygonItem *dbgpoly = surface->getScene()->addPolygon(dbpoly,QPen(QBrush(QColor(255,0,0),Qt::SolidPattern),4,Qt::SolidLine),QBrush(QColor(255,0,0),Qt::CrossPattern));
                            */
                            // debug

                            entityCache.start();
                            //int step = 0;
                            //int cnt=0;
                            while((e = entityCache.next()) != NULL )
                            {
                                //if((step == 0) || (cnt % step) == 0)
                                {
                                 //qDebug() << "entity is a : " << e->type();
                                 e->qgi=0;
                                 e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                                 e->setvisible(*getvisible());
                                 if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                                 {
                                     //qDebug() << "drawing plane pattern entity";
                                     double tdtol=db.getdtoler();
                                     db.setdtoler(tdtol*4.0);
                                     e->usedByPlane = true;
                                     e->draw(drawmode,NULL,surface);
                                     db.setdtoler(tdtol);
                                     if(e->qgi)
                                     {
                                         if(qclipPath.elementCount() > 0)
                                         {
                                             if(qclip)
                                             {
                                                 if(e->qgi)
                                                 {
                                                     //surface->getScene()->removeItem(e->qgi);
                                                     e->qgi->setParentItem(qclip);
                                                 }
                                             }
                                            e->qgi=0;
                                         }

                                     }
                                 }

                                 if(e->qgi)
                                 {
                                     if(surface->getScene())
                                        surface->getScene()->removeItem(e->qgi);
                                     delete e->qgi;
                                     e->qgi=0;
                                 }

                                 //delete e;
                                }
                                 //cnt++;
                            }
                            surface->settransform(tview);
                       }
                     }                     
                     }
#endif
                  }
             }
             //infile.close();
           }
      }

       if(qgi)
       {
           if(qclip)
           {
               //qclip->setPen(Qt::NoPen);
               //qclip->setBrush(Qt::NoBrush);
               //qclip->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qclip);
           }
           /*
           else
           {
               if(qplane)
               {
                   //qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                   ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
                   qgplane = qplane;
               }
           }
           */
           if(qgedges)
           {
               //qgedges->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
           }
           if(selected)
               qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
           //else
           //    qgi->setZValue(zValue);

           if(surface->getScene())
               surface->getScene()->addItem(qgi);
       }
  }
  else if (! options.test(PlaneNoFill))
  {

      if(drawmode != DM_PRINT)
      {
          if(qgi && qgi->scene() == surface->getScene())
              surface->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
      }
      if(drawmode == DM_ERASE)
          return;
      qgi = new QGraphicsItemGroup();

      if(qgi)
      {
          if(qgedges)
              ((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
          if(dldata.qgi)
          {
              ((QGraphicsItemGroup*)qgi)->addToGroup(dldata.qgi);
              dldata.qgi = qgi;
          }
          else
              dldata.qgi = qgi;
      }

       if (transform != 0)
         polygon3d.transform(transform);
       if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
        for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            o = origin;
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return;
            n1 /= len;
            if (i == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;
            if (transform != NULL)
              {  o = transform->transform(origin);
                 n1 = transform->transform(n1+origin) - o;
                 n2 = transform->transform(n2+origin) - o;
                 dir1 = transform->transform(dir1+origin) - o;
              }
            if (options.test(PlaneAnchored))
              o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));
            n = 0;
            p1 = o;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,drawline,&dldata) >= 0)
              {  p1 += n2;  n++;
              }
            p1 = o - n2;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,drawline,&dldata) <= 0)
              {  p1 -= n2;  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }
       if(qgi)
       {
           if(dldata.qgi != qgi)
               ((QGraphicsItemGroup*)qgi)->addToGroup(dldata.qgi);
           qgi = dldata.qgi;
           if(surface->getScene())
               surface->getScene()->addItem(qgi);

           if(selected)
               qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
           if(drawmode == DM_PRINT)
               qgi->setZValue(0);
       }
    }

#ifdef NEW_RUBBERB
  if(qgi)
  {
      if(qgi && dynamic_cast<View3dSubSurface*>(surface) != 0)
          if(((View3dSubSurface*)surface)->qgicp != 0 &&
             ((View3dSubSurface*)surface)->qgicp != qgi->parentItem())
              qgi->setParentItem(((View3dSubSurface*)surface)->qgicp);

      if(drawmode == DM_INVERT)
      {
          if(surface->qgirb == 0)
          {
              //qDebug() << "surface : " << surface->gethwnd();
              surface->qgirb = new QGraphicsItemGroup();
              surface->getScene()->addItem(surface->qgirb);
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);
          }
          else
              ((QGraphicsItemGroup*)surface->qgirb)->addToGroup(qgi);

          qgi=0;
      }
  }
#endif
  if(qgi && drawmode == DM_PRINT)
      qgi->setZValue(zValue);
  //if(qgi)
  //  qDebug() << "Plane ZValue : " << qgi->zValue();

}

static void hideline(Point p1,Point p2,void *data)
{HideImage *image = (HideImage *) ((void **) data)[0];
  image->hide((Entity *)((void **)data)[1],p1,p2);
}

int Plane::hide(HideImage *image)
{int i,j,imin,imax,jmin,jmax,n;
 double a,len;
 Point p1,n1,n2,dir1,o,xa,ya;
 void *data[2];
 Transform t1,t2,tview;
 RCHRGN clipregion;

  //qDebug() << "in plane hide :";
  if (options.test(PlaneFilled) || options.test(PlaneBitmapFilled) || options.test(PlaneNoFill) || options.test(PlaneGradientFilled))
    return 1;  //  Nothing to hide.


  if (options.test(1))
    {  t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       xa = t1.transform(xaxis);
       ya = t1.transform(yaxis);
    }
  else
    {  xa = xaxis;
       ya = yaxis;
    }

  Polygon3d polygon3d(origin,xa,ya,list,0,
                    (image->getsurface()->getumax() - image->getsurface()->getumin()) / image->getsurface()->getwidth());


  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);

  if (options.test(PlanePatterned))
    {
      CadFile infile;
       if (! infile.open(patternfilename,"rb"))
         return 1;

       Transform transf;
       RCGraphicsPlaneItem * qplane = new RCGraphicsPlaneItem(this,transf);
       QPainterPath qclipPath;

       if(image->drawmode != 1)// drawmode 1 = printing
       {
           if(qgi)
               image->getsurface()->getScene()->removeItem(qgi);
           delete qgi;
           qgi=0;
           qgplane=0;
       }
       qgi = new QGraphicsItemGroup();

       if (infile.readheader())
         {Entity *e;
          Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,pmin1,pmax1,pmin2,pmax2,p1,p2;

            t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
            xa = t1.transform(xaxis);
            ya = t1.transform(yaxis);

            //  Set the clipping region
            polygon3d.clip(image->getsurface());

            //clipregion = CreateRectRgn(0,0,1,1);
            //if (GetClipRgn(image->getsurface()->gethdc(),clipregion) <= 0)
            //{  DeleteObject(clipregion);
            //     clipregion = 0;
            //}

            qclipPath = polygon3d.setclipregion(0,image->getsurface());

            if(qplane)
            {
                qplane->qpath = qclipPath;
                qplane->qpattern = new QGraphicsItemGroup(qplane);
                if(qplane->qpattern)
                    qplane->qpattern->setFlag(QGraphicsItem::ItemStacksBehindParent);
            }

            polygon3d.setextents();

            //  Calculate the extents of the pattern from the point entities
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(point_entity))
                   {  p = ((PointE *)e)->getp();
                      if (p.x < pmin.x) pmin.x = p.x;
                      if (p.y < pmin.y) pmin.y = p.y;
                      if (p.z < pmin.z) pmin.z = p.z;
                      if (p.x > pmax.x) pmax.x = p.x;
                      if (p.y > pmax.y) pmax.y = p.y;
                      if (p.z > pmax.z) pmax.z = p.z;
                   }
                 delete e;
              }

            if (pmax.x <= pmin.x || pmax.y <= pmin.y)
              {  pmin.setxyz(-10.0,-10.0,-10.0);
                 pmax.setxyz(10.0,10.0,10.0);
              }

            pmin1 = polygon3d.getpmin();
            pmax1 = polygon3d.getpmax();

            pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
            pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
            for (i = 0 ; i < 7 ; i++)
              {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                 p2.x = (p1 - origin).dot(xa);
                 p2.y = (p1 - origin).dot(ya);
                 if (pmin2.x > p2.x) pmin2.x = p2.x;
                 if (pmin2.y > p2.y) pmin2.y = p2.y;
                 if (pmax2.x < p2.x) pmax2.x = p2.x;
                 if (pmax2.y < p2.y) pmax2.y = p2.y;
              }

            t1.tocoordinatesystem(xa,ya,xa.cross(ya));
            t2.translate(origin);
            t1.apply(t2);

            imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
            jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
            imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
            jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);

            for (i = imin ; i <= imax ; i++)
              for (j = jmin ; j <= jmax ; j++)
                {  infile.position(0);
                   infile.readheader();
                   image->getsurface()->gettransform(&tview);
                   t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                   t2.translate(origin + xa * i * xspacing * (pmax.x - pmin.x) + ya * j * yspacing * (pmax.y - pmin.y));
                   t1.apply(t2);
                   image->getsurface()->applytransform(t1);
                   while ((e = infile.nextentity(NULL)) != NULL)
                   {
                        e->qgi=0;
                        e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                        e->setvisible(*getvisible());
                        if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                        {
                          e->hide(image);
                          if(e->qgi)
                          {
                              if(qclipPath.elementCount() > 0)
                              {
                                  ((QGraphicsItemGroup*)qplane->qpattern)->addToGroup(e->qgi);
                                  e->qgi=0;
                              }
                          }
                        }
                        if(e->qgi)
                        {
                            if(image->getsurface()->getScene())
                                image->getsurface()->getScene()->removeItem(e->qgi);
                            delete e->qgi;
                            e->qgi=0;
                        }
                        delete e;
                   }
                   image->getsurface()->settransform(tview);
                }
             //SelectClipRgn(image->getsurface()->gethdc(),clipregion);
             //DeleteObject(clipregion);
         }
       infile.close();

       if(qgi)
       {
           if(qplane)
           {
               //qplane->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               ((QGraphicsItemGroup*)qgi)->addToGroup(qplane);
               qgplane = qplane;
           }
           //if(qgedges)
           //{
               //qgedges->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
               //((QGraphicsItemGroup*)qgi)->addToGroup(qgedges);
           //}
           //if(selected)
           //    qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
           //else
           //    qgi->setZValue(zValue);
           if(image->getsurface()->getScene())
              image->getsurface()->getScene()->addItem(qgi);
       }

    }
  else
    {

      if(image->drawmode != 1) // printing
      {
          if(qgi)
              image->getsurface()->getScene()->removeItem(qgi);
          delete qgi;
          qgi=0;
          qgplane=0;
      }
      qgi = new QGraphicsItemGroup();

       data[0] = image;
       data[1] = this;
       if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
        for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            o = origin;
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return 1;
            n1 /= len;
            if (i == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;

            if (options.test(PlaneAnchored))
              o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));

            n = 0;
            p1 = o;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,hideline,data) >= 0)
              {  p1 += n2;  n++;
              }
            p1 = o - n2;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,hideline,data) <= 0)
              {  p1 -= n2;  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }

       if(qgi)
       {
           if(image->getsurface()->getScene())
              image->getsurface()->getScene()->addItem(qgi);
       }
    }
  return 1;
}

static void hidelineGL(Point p1,Point p2,void *data)
{HideImage *image = (HideImage *) ((void **) data)[0];
  image->hideGL((Entity *)((void **)data)[1],p1,p2);
}

int Plane::hideGL(HideImage *image)
{int i,j,imin,imax,jmin,jmax,n;
 double a,len;
 Point p1,n1,n2,dir1,o,xa,ya;
 void *data[2];
 Transform t1,t2,tview;
 RCHRGN clipregion;

  //qDebug() << "in plane hideGL :";
  if (options.test(PlaneFilled) || options.test(PlaneBitmapFilled) || options.test(PlaneNoFill) || options.test(PlaneGradientFilled))
    return 1;  //  Nothing to hide.


  if (options.test(1))
    {  t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
       xa = t1.transform(xaxis);
       ya = t1.transform(yaxis);
    }
  else
    {  xa = xaxis;
       ya = yaxis;
    }

  Polygon3d polygon3d(origin,xa,ya,list,0,
                    (image->getsurface()->getumax() - image->getsurface()->getumin()) / image->getsurface()->getwidth());

  polygon3d.setextents();

  polygon3d.transform(&image->getsurface()->modeltransform);

  image->getsurface()->setupGL(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);

  RCCOLORREF pcol;
  pcol = image->getsurface()->getcolour();
  GLfloat fr=GetRValue(pcol)/255.0;
  GLfloat fg=GetGValue(pcol)/255.0;
  GLfloat fb=GetBValue(pcol)/255.0;
  glColor3f(fr,fg,fb);
  GLfloat lwidth = image->getsurface()->curPen.widthF();
  glLineWidth(image->getsurface()->curGLWeight);

  if (options.test(PlanePatterned))
    {
      CadFile infile;
       if (! infile.open(patternfilename,"rb"))
         return 1;

       if (infile.readheader())
         {Entity *e;
          Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38),p,pmin1,pmax1,pmin2,pmax2,p1,p2;

            t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
            xa = t1.transform(xaxis);
            ya = t1.transform(yaxis);


            //if(image->getsurface())
            //  polygon3d.transform(image->getsurface()->gettransform());
            //  Set the clipping region
            //polygon3d.clip(image->getsurface());

            //draw the stencil mask
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0x00);
            glClearStencil(0);
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glDepthMask(GL_FALSE);

            glStencilFunc(GL_NEVER,1,0xff);
            glStencilOp(GL_REPLACE,GL_KEEP,GL_KEEP);
            glStencilMask(0xff);
            glClear(GL_STENCIL_BUFFER_BIT);

            DrawPolygon(polygon3d.getorigin(),polygon3d.getxaxis(),polygon3d.getyaxis(),polygon3d.getnloops(),polygon3d.getnvert(),polygon3d.getverts());
            //polygon3d.drawGL(drawmode,transform,surface);

            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glDepthMask(GL_TRUE);
            glStencilFunc(GL_EQUAL,1,0xFF);
            glStencilMask(0x00);

            SetNoMaterial(fr,fg,fb);


            //  Calculate the extents of the pattern from the point entities
            while ((e = infile.nextentity(NULL)) != NULL)
              {  if (e->isa(point_entity))
                   {  p = ((PointE *)e)->getp();
                      if (p.x < pmin.x) pmin.x = p.x;
                      if (p.y < pmin.y) pmin.y = p.y;
                      if (p.z < pmin.z) pmin.z = p.z;
                      if (p.x > pmax.x) pmax.x = p.x;
                      if (p.y > pmax.y) pmax.y = p.y;
                      if (p.z > pmax.z) pmax.z = p.z;
                   }
                 delete e;
              }

            if (pmax.x <= pmin.x || pmax.y <= pmin.y)
              {  pmin.setxyz(-10.0,-10.0,-10.0);
                 pmax.setxyz(10.0,10.0,10.0);
              }

            pmin1 = polygon3d.getpmin();
            pmax1 = polygon3d.getpmax();

            pmin2.x = pmax2.x = (pmin1 - origin).dot(xa);
            pmin2.y = pmax2.y = (pmin1 - origin).dot(ya);
            for (i = 0 ; i < 7 ; i++)
              {  p1.setxyz((i /4)%2 ? pmin1.x : pmax1.x,(i/2)%2 ? pmin1.y : pmax1.y,i%2 ? pmin1.z : pmax1.z);
                 p2.x = (p1 - origin).dot(xa);
                 p2.y = (p1 - origin).dot(ya);
                 if (pmin2.x > p2.x) pmin2.x = p2.x;
                 if (pmin2.y > p2.y) pmin2.y = p2.y;
                 if (pmax2.x < p2.x) pmax2.x = p2.x;
                 if (pmax2.y < p2.y) pmax2.y = p2.y;
              }

            t1.tocoordinatesystem(xa,ya,xa.cross(ya));
            t2.translate(origin);
            t1.apply(t2);

            imin = (int)floor(pmin2.x / (pmax.x - pmin.x) / xspacing);
            jmin = (int)floor(pmin2.y / (pmax.y - pmin.y) / yspacing);
            imax = (int)ceil(pmax2.x / (pmax.x - pmin.x) / xspacing);
            jmax = (int)ceil(pmax2.y / (pmax.y - pmin.y) / yspacing);

            for (i = imin ; i <= imax ; i++)
            {
              for (j = jmin ; j <= jmax ; j++)
              {
                   infile.position(0);
                   infile.readheader();
                   image->getsurface()->gettransform(&tview);
#if 1
                   t1.tocoordinatesystem(xa * xspacing,ya * yspacing,xa.cross(ya));
                   t2.translate(origin + xa * i * xspacing * (pmax.x - pmin.x) + ya * j * yspacing * (pmax.y - pmin.y));
                   t1.apply(t2);
                   image->getsurface()->applytransform(t1);
#else
                   // TODO KMJ: fix this to work better with figures
                   Point pato = origin;
                   Point patxa = xa;
                   Point patya = ya;
                   pato  = image->getsurface()->modeltransform.transform(origin);
                   patxa = image->getsurface()->modeltransform.transform(xa+origin)-pato;
                   patya = image->getsurface()->modeltransform.transform(ya+origin)-pato;
                   patxa.normalize();
                   patya.normalize();

                   //t1.tocoordinatesystem(patxa * xspacing,patya * yspacing,patxa.cross(patya));
                   //t2.translate(pato + patxa * i * (pmax.x - pmin.x) * xspacing + patya * j * (pmax.y - pmin.y) * yspacing);
                   //t1.apply(t2);
                   t1.tocoordinatesystem(origin * xspacing,xa * yspacing,xa.cross(ya));
                   t2.translate(origin + xa * i * (pmax.x - pmin.x) * xspacing + ya * j * (pmax.y - pmin.y) * yspacing);
                   t1.apply(t2);
                   image->getsurface()->applytransform(t1);
#endif
                   while ((e = infile.nextentity(NULL)) != NULL)
                   {
                        e->qgi=0;
                        e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                        e->setvisible(*getvisible());
                        if ((! e->isa(text_entity) || ((Text *)e)->getlabel() == 0) && ! e->isa(point_entity))
                        {
                          e->hideGL(image);
                        }
                        delete e;
                   }
                   image->getsurface()->settransform(tview);
              }
            }
            //glDisable(GL_SCISSOR_TEST);
            glStencilMask(0xFF);
            glClear(GL_STENCIL_BUFFER_BIT);
            glDisable(GL_STENCIL_TEST);
         }
       infile.close();

    }
  else
    {
       data[0] = image;
       data[1] = this;
       if (xspacing >= 1.0E-10 && yspacing >= 1.0E-10)
        for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            o = origin;
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return 1;
            n1 /= len;
            if (i == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;

            if (options.test(PlaneAnchored))
              o += n1 * ((i == 0 ? xspacing : yspacing) - fmod(n1.dot(o - anchor),i == 0 ? xspacing : yspacing));

            n = 0;
            p1 = o;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,hidelineGL,data) >= 0)
              {  p1 += n2;  n++;
              }
            p1 = o - n2;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,hidelineGL,data) <= 0)
              {  p1 -= n2;  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }
    }
  return 1;
}

int Plane::npoints(void)
{  return 0;
}

double Plane::getarea(int nsegs,double error)
{Polygon3d polygon3d(origin,xaxis,yaxis,list,nsegs,error);
  return fabs(polygon3d.getarea());
}

Point Plane::point(int)
{Point tmp(0.0,0.0,0.0);  return tmp;
}

Point Plane::nearp(Point p)
{Point tmp(0.0,0.0,0.0),normal;
 double t,denom;
  normal = xaxis.cross(yaxis);  normal.normalize();
  denom = normal.dot(normal);
  if (denom > db.getptoler())
    {  t = -(normal.dot(p) + -normal.dot(origin)) / denom;
       tmp = normal * t + p;
    }
  return tmp;
}

int Plane::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 EntityList newlist;
 Point o,xax,yax,zax;
 double d;
 int stretched;

  if (! isvisible(0)) return 1;

  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  for (list.start(),stretched = 0 ; ! list.atend(); )
    {  if ((e = list.next()) != 0)
         {  e->stretchent(db,t,sp,erase);
            if ((e->getstatus() & ~ 8) != 0 ||
                e->getcopye() != 0 && (e->getcopye()->getstatus() & ~8) != 0)
              stretched = 1;
         }
    }
  if (! stretched) return 1;
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0 && ! e->isa(group_entity))
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus(getstatus() | 4);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db == 0 ? 1 : db->geometry.add(c);
           }
      }
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  if (e->getcopye() == NULL) 
              newlist.add(e);
            else
              newlist.add(e->getcopye());
         }
    }
  newlist.reverse();
  Loops loops(1,newlist);
  if (! loops.planar(&o,&xax,&yax,&zax,&d))
    {  newlist.destroy();
       return 1;
    }
  status |= 2 | 16;
  origin = o;
  xaxis = xax;
  yaxis = yax;
  dbdelete();
  list.destroy();
  list = newlist;
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int Plane::move(Database *db,Transform &t,int erase)
{Entity *e;
 EntityList newlist;
 Point a,o,xax,yax,zax;
 double d;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {  status &= ~1;  //  This entity will not be moved - remove it from the moved list
         return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
      } 
   
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  if (e->getcopye() == NULL) e->move(db,t,erase);
            if (e->getcopye() == NULL)
              {  newlist.destroy();  draw(DM_NORMAL);  return 0;
              }
            newlist.add(e->getcopye());
         }
    }
  newlist.reverse();
  Loops loops(1,newlist);
  if (! loops.planar(&o,&xax,&yax,&zax,&d))
    {  newlist.destroy();  
       return 0;
    }
  else
    {
       a = t.transform(anchor);
       o = t.transform(origin);
       xax = t.transform(xaxis+origin) - o;  xax.normalize();
       yax = t.transform(yaxis+origin) - o;  yax.normalize();
       zax = xax.cross(yax);
       yax = zax.cross(xax);
       if (xax.length() < ::db.getptoler() || yax.length() < ::db.getptoler())
         {  newlist.destroy();  
            return 0;
         }
    }
  status |= 1;
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  xaxis = xax;  
  yaxis = yax;  
  origin = o;
  //anchor = a; // KMJ added for v8 setting anchors for patterns and bitmaps
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  list.destroy();
  list = newlist;
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *Plane::clone(Transform &t)
{EntityList newlist;
 Entity *e;
 Point org,xa,ya,za;
 double D;
  if (copye != NULL) return copye;
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  if (e->getcopye() == NULL)
              {  if (e->clone(t) == NULL) return 0;
              }
            newlist.add(e->getcopye());
         }
    }
  newlist.reverse();
  Loops loops(1,newlist);
  if (! loops.planar(&org,&xa,&ya,&za,&D))
    copye = NULL;
  else
    {
      // KMJ: added
       Point a,o,xax,yax,zax;
       a = t.transform(anchor);
       o = t.transform(origin);
       xax = t.transform(xaxis+origin) - o;  xax.normalize();
       yax = t.transform(yaxis+origin) - o;  yax.normalize();
       zax = xax.cross(yax);
       yax = zax.cross(xax);
       if (xax.length() < ::db.getptoler() || yax.length() < ::db.getptoler())
       {
           newlist.destroy();
           return 0;
       }
       xa = xax;
       ya = yax;
       org = o;
       //
       //copye = new Plane(*this,anchor,org,xa,ya,newlist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       copye = new Plane(*this,org,xa,ya,newlist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       if (copye == NULL || ! copye->getdefined())
         {  delete copye;
            copye = NULL;
         }
      //else
      //     ((Plane*)copye)->SetAnchorPoint(a);
    }
  copye->cloneattributes(this);
  return copye;
}

Entity *Plane::deepclone(Transform &t)
{EntityList newlist;
 Entity *e;
 Point anc,org,xa,ya,za;
 double D;
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  e->setcopye(0);
            newlist.add(e->clone(t));
         }
    }
  newlist.reverse();
  Loops loops(1,newlist);
  if (! loops.planar(&org,&xa,&ya,&za,&D))
    copye = NULL;
  else
    {
      // KMJ: added
       Point a,o,xax,yax,zax;
       a = t.transform(anchor);
       o = t.transform(origin);
       xax = t.transform(xaxis+origin) - o;  xax.normalize();
       yax = t.transform(yaxis+origin) - o;  yax.normalize();
       zax = xax.cross(yax);
       yax = zax.cross(xax);
       if (xax.length() < ::db.getptoler() || yax.length() < ::db.getptoler())
       {
           newlist.destroy();
           return 0;
       }
       xa = xax;
       ya = yax;
       org = o;
       //
       //copye = new Plane(*this,anchor,org,xa,ya,newlist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       copye = new Plane(*this,org,xa,ya,newlist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       if (copye == NULL || ! copye->getdefined())
       {
            delete copye;
            copye = NULL;
       }
       //else
       //    ((Plane*)copye)->SetAnchorPoint(anchor);
    }
  return copye;
}

int Plane::project(Database *db,Transform &t,int surface)
{EntityList newlist;
 Entity *e;
 Point org,xa,ya,za;
 double D;
  if (copye != NULL) return 1;
  for (list.start() ; ! list.atend(); )
    {  if ((e = list.next()) == NULL)
         newlist.add(e);
       else
         {  if (e->getcopye() == NULL) e->project(db,t,surface);
            if (e->getcopye() == NULL) return 0;
            newlist.add(e->getcopye());
         }
    }
  newlist.reverse();
  Loops loops(1,newlist);
  if (! loops.planar(&org,&xa,&ya,&za,&D))
    {  copye = NULL;
       return 0;
    }
  else
    {  copye = new Plane(*this,org,xa,ya,newlist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
       if (copye->getdefined())
         return db->geometry.add(copye);
       else
         {  delete copye;
            copye = NULL;
            return 0;
         }
    }
}

void Plane::change(const BitMask &change,const EntityHeader &header,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  //draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);

  if (change.test(0)) colour = header.getcolour();
  if (change.test(1)) layer = header.getlayer();
  if (change.test(2)) style = header.getstyle();
  if (change.test(3)) weight = header.getweight();

  if (change.test(4)) 
    {  options.set(PlaneNoFill,o.test(PlaneNoFill));
       options.set(PlanePatterned,o.test(PlanePatterned));
       options.set(PlaneBitmapFilled,o.test(PlaneBitmapFilled));
       options.set(PlaneFilled,o.test(PlaneFilled));
       options.set(PlaneGradientFilled,o.test(PlaneGradientFilled));
    }  

  if (change.test(5)) 
    options.set(PlaneDisplayEdges,o.test(PlaneDisplayEdges));

  if (change.test(6))
    {  delete [] patternfilename;
       patternfilename = 0;
       if ((options.test(PlanePatterned) || options.test(PlaneBitmapFilled)) && filename != 0 && strlen(filename) > 0)
         {  patternfilename = new RCCHAR[strlen(filename)+1];
            strcpy(patternfilename,filename);
         }
    }

  if (change.test(7)) 
    {  options.set(PlaneGradientStyle1,o.test(PlaneGradientStyle1));
       options.set(PlaneGradientStyle2,o.test(PlaneGradientStyle2));
       options.set(PlaneGradientStyle3,o.test(PlaneGradientStyle3));
       options.set(PlaneGradientStyle4,o.test(PlaneGradientStyle4));
    }

  if (change.test(8))
    gradientcolour = gc;

  if (change.test(9)) xspacing = xs;
  if (change.test(10)) yspacing = ys;
  if (change.test(11)) angle = a;
  if (change.test(12)) crossangle = ca;
  if (change.test(13)) transparency = trans;

  if (change.test(14)) 
    {  red = tr;
       green = tg;
       blue = tb;
    }
  if (change.test(15)) 
    transparencyrange = trange;

  if (patternfilename == 0 && (options.test(PlanePatterned) || options.test(PlaneBitmapFilled)))
    {  patternfilename = new RCCHAR[1];
       strcpy(patternfilename,"");
    }

  if (defined && transparency != 0.0 || transparencyrange != 0.0)
    db.setrequiresopengl(1);

#ifdef USING_WIDGETS
  if((options.test(PlanePatterned) || options.test(PlaneBitmapFilled)) && strlen(patternfilename) > 0)
  {
      delete bitmape;
      bitmape = 0;
      //delete [] patternfilename;
      //patternfilename = new RCCHAR[strlen(filename)+1];
      //strcpy(patternfilename,filename);
      if (patternfilename != 0 && options.test(PlaneBitmapFilled))
      {
          Point xa,ya;
          Transform t1;
          if (options.test(1))
          {
               t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
               xa = t1.transform(xaxis);
               ya = t1.transform(yaxis);
          }
          else
          {
              xa = xaxis;
              ya = yaxis;
          }
          bitmape = new BitMapE(patternfilename,origin,
                                xa,ya,xspacing,yspacing,
                                red,green,blue,
                                transparencyrange,1.0-transparency,options);

          if(bitmape != 0)
          {
              if(qgplane)
                qgplane->rcbitmap = bitmape;
          }
      }
      else if (patternfilename != 0 && options.test(PlanePatterned))
      {
          patternEnts.start();
          patternEnts.destroy();
      }
  }
  else
  {
      delete bitmape;
      bitmape = 0;
      //delete [] patternfilename;
      //patternfilename = 0;
      if(qgplane)
        qgplane->rcbitmap = 0;
  }
#endif

  //draw(DM_NORMAL);
}


//
//  Recalculate the equation of the plane - use after edges have been added or modifed
//
void Plane::ReCalculatePlane(void)
{Point zaxis;
 double D;
  Loops loops(1,list);
  if (! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
    defined = 0;
}


void Plane::AlignPlane(Point p1,Point p2)
{Point zaxis;
 Point xa,ya;
  cadwindow->invalidatedisplaylist(this);
  xa = (p2 - p1).normalize();
  zaxis = xaxis.cross(yaxis);
  xa = ((zaxis.cross(xa)).cross(zaxis)).normalize();
  ya = zaxis.cross(xa);

  if (xa.cross(ya).length() > 1.0e-10)
    {  xaxis = xa;
       yaxis = ya;
    }
}


//
//  Inserts an edge l, at the position p.
//    The adjacent edges are not modified - they will need to be modified elsewhere.
//    The plane equation should also be recalculated after this routine is used.
//
void Plane::AddEdge(Point p,Line *l)
{Entity *line1,*line2,*e;
 EntityList newlist;

  db.saveundo(UD_MOVED,this);
 
  //  Get the entity at the end of the plane list
  for (list.start() ; ! list.atend(); )
    line1 = list.next();

  for (list.start() ; ! list.atend(); )
    {  line2 = list.next();
       if (line1 == 0 || line2 == 0 || ! line1->isa(line_entity) || ! line2->isa(line_entity))
         continue;

       if (((((Line *)line1)->getp1() - p).length() < db.getptoler() || (((Line *)line1)->getp2() - p).length() < db.getptoler()) &&
           ((((Line *)line2)->getp1() - p).length() < db.getptoler() || (((Line *)line2)->getp2() - p).length() < db.getptoler()))
         {  //  Found the location for the new edge - create a new list with the new edge at this position.
            for (list.start() ; ! list.atend(); )
              {  e = list.next();
                 if (e == line2)
                   newlist.add(l);
                 newlist.add(e);
              }
            newlist.reverse();

            //  Delete the associated entities, and reassociated with the new list
            dbdelete();
            list.destroy();
            list = newlist;
            dbinsert(&db);
            break;

         } 

       line1 = line2;

    }
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

}

//
//  Inserts an edge l, at the position p.
//    The adjacent edges are not modified - they will need to be modified elsewhere.
//    The plane equation should also be recalculated after this routine is used.
//
void Plane::ReplaceEdges(Line *line,Point p)
{Entity *edge1,*edge2,*e;
 EntityList newlist;

  db.saveundo(UD_MOVED,this);
 
  //  Get the entity at the end of the plane list
  for (list.start() ; ! list.atend(); )
    edge1 = list.next();

  for (list.start() ; ! list.atend(); )
    {  edge2 = list.next();
       if (edge1 == 0 || edge2 == 0)
         continue;

       if ( ( (((LinearEntity *)edge1)->start() - line->start()).length() < db.getptoler() && (((LinearEntity *)edge1)->end() - p).length() < db.getptoler()  || 
              (((LinearEntity *)edge1)->end() - line->start()).length() < db.getptoler() && (((LinearEntity *)edge1)->start() - p).length() < db.getptoler()  ||
              (((LinearEntity *)edge1)->start() - line->end()).length() < db.getptoler() && (((LinearEntity *)edge1)->end() - p).length() < db.getptoler()  || 
              (((LinearEntity *)edge1)->end() - line->end()).length() < db.getptoler() && (((LinearEntity *)edge1)->start() - p).length() < db.getptoler()  ) &&
            ( (((LinearEntity *)edge2)->start() - line->end()).length() < db.getptoler() && (((LinearEntity *)edge2)->end() - p).length() < db.getptoler()   || 
              (((LinearEntity *)edge2)->end() - line->end()).length() < db.getptoler() && (((LinearEntity *)edge2)->start() - p).length() < db.getptoler()   ||
              (((LinearEntity *)edge2)->start() - line->start()).length() < db.getptoler() && (((LinearEntity *)edge2)->end() - p).length() < db.getptoler()   || 
              (((LinearEntity *)edge2)->end() - line->start()).length() < db.getptoler() && (((LinearEntity *)edge2)->start() - p).length() < db.getptoler()  ))
         {  
            //  Found the location for the new edge - create a new list with the new edge at this position.
            for (list.start() ; ! list.atend(); )
              {  e = list.next();
                 if (e == edge2)
                   newlist.add(line);
                 if (e != edge1 && e != edge2)
                   newlist.add(e);
              }
            newlist.reverse();

            //  Delete the associated entities, and reassociated with the new list
            dbdelete();
            list.destroy();
            list = newlist;
            dbinsert(&db);
            break;

         } 

       edge1 = edge2;

    }
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

}


//
//  Inserts an edge l, at the position p.
//    The adjacent edges are not modified - they will need to be modified elsewhere.
//    The plane equation should also be recalculated after this routine is used.
//
Entity *Plane::AddVertex(Line *line1,Line *line2,Entity **newedges)
{Entity *edge1,*edge2,*e;
 EntityList newlist;
 Point p1;

  db.saveundo(UD_MOVED,this);
 
  //  Get the entity at the end of the plane list
  edge1 = 0;
  for (list.start() ; ! list.atend(); )
    {  edge2 = list.next();
       if (edge2 != 0)
         edge1 = edge2;
    }

  if (edge1 == 0)
    return 0;

  for (list.start() ; ! list.atend(); )
    {  
       edge2 = list.next();
       if (edge2 == 0)
         continue;

       if ( (((LinearEntity *)edge2)->start() - line1->getp1()).length() < db.getptoler() &&
            (((LinearEntity *)edge2)->end() - line2->getp2()).length() < db.getptoler() ||
            (((LinearEntity *)edge2)->end() - line1->getp1()).length() < db.getptoler() &&
            (((LinearEntity *)edge2)->start() - line2->getp2()).length() < db.getptoler() )
         {  
            //  Found the location for the new edge - create a new list with the new edge at this position.
            for (list.start() ; ! list.atend(); )
              {  e = list.next();
                 if (e == edge2)
                   {  

                       if ( (((LinearEntity *)edge2)->start() - ((LinearEntity *)edge1)->start()).length() < db.getptoler() ||
                            (((LinearEntity *)edge2)->start() - ((LinearEntity *)edge1)->end()).length() < db.getptoler() )
                         p1 = ((LinearEntity *)edge2)->start();
                       else   
                         p1 = ((LinearEntity *)edge2)->end();

                      if ((p1 - line1->getp1()).length() < db.getptoler() )
                        {  newlist.add(line1);
                           newlist.add(line2);
                        }
                      else  
                        {  newlist.add(line2);
                           newlist.add(line1);
                        }

                   }
                 else
                   newlist.add(e);
              }
            newlist.reverse();

            //  Delete the associated entities, and reassociated with the new list
            dbdelete();
            list.destroy();
            list = newlist;
            dbinsert(&db);
            break;

         } 

       edge1 = edge2;

    }
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

  return edge2;

}



//
//  Delets the edge l from the plane
//    The adjacent edges are not modified - they will need to be modified elsewhere.
//    The plane equation should also be recalculated after this routine is used.
//
void Plane::DeleteEdge(Line *l)
{EntityList newlist;
 Entity *e;

  db.saveundo(UD_MOVED,this);

  for (list.start() ; ! list.atend(); )
    {  e = list.next();
       if (e != l)
         newlist.add(e);
    }
  newlist.reverse();

  //  Delete the associated entities, and reassociated with the new list
  dbdelete();
  list.destroy();
  list = newlist;
  dbinsert(&db);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

}

//  
//  Cuts the plane into two.  Plane can only cut lines at the moment.
//
void Plane::Cut(Point o,Point n,EntityList **list1,EntityList **list2)
{
}

void Plane::Offset(double offset,double height,EntityList **list1)
{Loops loops(0);
 Point origin,xaxis,yaxis,zaxis;
 Entity *e;
 Transform t;
 double D;
 EntityList l;

  //  Offset the plane.  If offset > 0.0, area should increase, otherwise it should decrease
  l = list.copy();
  loops.create(&l);
  *list1 = new EntityList;
  loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D);
  Polygon3d poly1(origin,xaxis,yaxis,list,10,0.0);
  loops.offset(1,0,Point(0.0,0.0,0.0),offset,*list1);
  Polygon3d poly2(origin,xaxis,yaxis,**list1,10,0.0);
  if (fabs(poly1.getarea()) > fabs(poly2.getarea()) && offset > 0.0 || 
      fabs(poly1.getarea()) < fabs(poly2.getarea()) && offset < 0.0)
    {  for ((*list1)->start() ; (e = (*list1)->next()) != 0 ; )
         db.geometry.del(e);
       loops.offset(1,0,Point(0.0,0.0,0.0),-offset,*list1);
    } 

  //  Move the offset up by the given height.
  t.translate(xaxis.cross(yaxis).normalize() * height);
  for ((*list1)->start() ; (e = (*list1)->next()) != 0 ; )
    e->move(&db,t,0);

}

/*
// defined in dialog.h
class PlaneDisplayDialogControl : public DisplayDialogControl
  {public:
     PlaneDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

#ifdef USING_WIDGETS
void PlaneDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA),rsVaries(4107);;
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 View top2(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 RECT rect;
 RCHPEN hpen,oldhpen;
 Point p1,p2,pmin,pmax,pmin1,pmax1,pmin3,pmax3;
 double w,w2,h,h2,xspacing=0,xspacing2,yspacing=2,yspacing2,angle,crossangle,transparency,red,green,blue,transparencyrange,scale,r,g,b;
 RCCHAR patternfilename[300],text[300];
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),org2(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32),defoptions(32);
 EntityList list,list2;
 int c,fillstyle,gradientfillstyle;
 CadFile infile;
 Entity *e;
 RCCOLORREF gradientcolour;
 double defxspacing,defyspacing,defangle,defcrossangle,deftransparency,deftr,deftg,deftb,deftrange;
 RCCOLORREF defgc;
 RCCHAR defpatternfilename[300];
 int tristates[] = { 1 };
 int backstyle=0;

 int cl,l,s;
 double wt;
 cl=db.header.getcolour();
 l=(int)db.header.getlayer();
 s=(int)db.header.getstyle();
 wt=db.lineweights.weighttomm(db.header.getweight());

  if(hWnd == 0)
      return;

  QGraphicsView *ctl = 0;
  if(dialog->gethdlg())
    ctl = (QGraphicsView*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(219);
  QGraphicsScene *scene = 0;
  if(ctl)
      scene = ctl->scene();

  QGraphicsView *ctl2 = 0;
  if(dialog->gethdlg())
    ctl2 = (QGraphicsView*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(220);
  QGraphicsScene *scene2 = 0;
  if(ctl2)
      scene2 = ctl2->scene();

  if(ctl && scene && ctl2 && scene2)
  //if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
  {
        View3dOutput output(ctl,ctl,&top,1.0);
        output.setScene(scene);
        scene->setSceneRect(QRectF());
        scene->clear();
        QRect qrect = ctl->rect();

        scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                              ::v.getreal("wn::backgroundcolour.green")*255.0,
                                              ::v.getreal("wn::backgroundcolour.blue")*255.0)));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                               ::v.getreal("wn::backgroundcolour.blue")*255.0));

        output.sizeevent(qrect.width(),qrect.height());
        scene->update();
        app->processEvents();

        View3dOutput output2(ctl2,ctl2,&top2,1.0);
        output2.setScene(scene2);
        scene2->setSceneRect(ctl2->rect());
        scene2->clear();
        QRect qrect2 = ctl2->rect();
        //qDebug() << "scene rect : " << scene2->sceneRect();
        QColor backColour(QColor(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0+.05,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5));
        scene2->setBackgroundBrush(QBrush(backColour));

        output2.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                  ::v.getreal("wn::backgroundcolour.green")*255.0,
                                  ::v.getreal("wn::backgroundcolour.blue")*255.0));

        output2.sizeevent(qrect2.width(),qrect2.height());
        scene2->update();
        app->processEvents();

        Plane::getdefaults(&defxspacing,&defyspacing,&defangle,&defcrossangle,&deftransparency,&deftr,&deftg,&deftb,&deftrange,&defgc,defpatternfilename,&defoptions);

        gradientfillstyle = defoptions.test(PlaneGradientStyle1) * 8 + defoptions.test(PlaneGradientStyle2) * 4 +  defoptions.test(PlaneGradientStyle3) * 2 +  defoptions.test(PlaneGradientStyle4);

        options.clearall();
       if (
           dialog->currentvalue(100,&cl ) &&
           dialog->currentvalue(101,&l ) &&
           dialog->currentvalue(102,&s ) &&
           dialog->currentvalue(103,&wt ) &&
           dialog->currentvalue(114,&fillstyle) &&
           //dialog->currentvalue(120,&options) &&
           dialog->currentvalue(108,patternfilename,300) &&
           dialog->currentvalue(120,&tristates[0]) &&
           dialog->currentvalue(121,&gradientfillstyle) &&
           //dialog->currentvalue(122,&r) &&
           //dialog->currentvalue(123,&g) &&
           //dialog->currentvalue(124,&b) &&
           ((GradientPlaneButtonDialogControl*)dialog->getcontrol(1004))->currentvalue(&r,&g,&b) &&
           dialog->currentvalue(104,&xspacing) &&
           dialog->currentvalue(105,&yspacing) &&
           dialog->currentvalue(106,&angle) &&
           dialog->currentvalue(107,&crossangle) &&
           dialog->currentvalue(125,&transparency) &&
           //dialog->currentvalue(127,&red) &&
           //dialog->currentvalue(128,&green) &&
           //dialog->currentvalue(129,&blue) &&
           //((BitMapEPlaneButtonDialogControl*)dialog->getcontrol(1007))->currentvalue(&red,&green,&blue) &&
           ((BitMapETransparencyRangeDialogControl*)dialog->getcontrol(1005))->currentvalue(&transparencyrange,&red,&green,&blue) &&
           dialog->currentvalue(126,&transparencyrange))
           dialog->currentvalue(1009,&backstyle);
         {
           //double xscaling,yscaling;
           //dialog->currentvalue(104,&xscaling);
           //dialog->currentvalue(105,&yscaling);

           if(((QCheckBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(120))->isTristate())
                tristates[0] == 1 ? options.set(PlaneDisplayEdges,defoptions.test(PlaneDisplayEdges)) : options.set(PlaneDisplayEdges,tristates[0]);
           else
               options.set(PlaneDisplayEdges,tristates[0]);

           if( !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(114))->isChecked() &&
               !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(115))->isChecked() &&
               !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(116))->isChecked() &&
               !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(117))->isChecked() &&
               !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(118))->isChecked() &&
               !((QRadioButton*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(119))->isChecked() )
           {
               fillstyle = 1; // hatching
               if (defoptions.test(PlaneNoFill))
                 fillstyle = 0;
               else if (defoptions.test(PlanePatterned))
                 fillstyle = 2;
               else if (defoptions.test(PlaneFilled))
                 fillstyle = 3;
               else if (defoptions.test(PlaneBitmapFilled))
                 fillstyle = 4;
               else if (defoptions.test(PlaneGradientFilled))
                 fillstyle = 5;
           }

           //if(((QLineEdit*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(108))->text().compare(QString(rsVaries.gets())) == 0 )
           if(strcmp(patternfilename,rsVaries.gets()) == 0)
               strcpy(patternfilename,defpatternfilename);

           //qDebug() << "combobox " << (QComboBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(1021);
           //qDebug() << "combobox visible " << ((QComboBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(1021))->isVisible();

           if(((QComboBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(1021))->isVisible())
               gradientfillstyle = defoptions.test(PlaneGradientStyle1) * 8 + defoptions.test(PlaneGradientStyle2) * 4 +  defoptions.test(PlaneGradientStyle3) * 2 +  defoptions.test(PlaneGradientStyle4);

           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(122))->specialValueText().isEmpty() &&
           //   r == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(122))->minimum() )
           //    r = GetRValue(defgc);

           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(123))->specialValueText().isEmpty() &&
           //   g == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(123))->minimum() )
           //    g = GetGValue(defgc);

           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(124))->specialValueText().isEmpty() &&
           //   b == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(124))->minimum() )
           //    b = GetBValue(defgc);

           if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(104))->specialValueText().isEmpty() &&
              xspacing == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(104))->minimum() )
               xspacing = defxspacing;

           if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(105))->specialValueText().isEmpty() &&
              yspacing == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(105))->minimum() )
               yspacing = defyspacing;

           if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
              angle == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
               angle = defangle;

           if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(107))->specialValueText().isEmpty() &&
              crossangle == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(107))->minimum() )
               crossangle = defcrossangle;

           qDebug() << "trans sp text : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(125))->specialValueText();
           qDebug() << "trans min     : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(125))->minimum();
           //transparency = 0.0;  //  No need to show transparency
           if(((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(125))->specialValueText() == "*Varies*" &&
              transparency == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(125))->minimum() )
               transparency = deftransparency;

           qDebug() << "trans% sp text : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(134))->specialValueText();
           qDebug() << "trans% min     : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(134))->minimum();
           if(((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(134))->specialValueText() == "*Varies*" &&
              transparency == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(134))->minimum() )
               transparency = deftransparency;

           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(127))->specialValueText().isEmpty() &&
           //   red == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(127))->minimum() )
           //    red = deftr;
           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(128))->specialValueText().isEmpty() &&
           //   green == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(128))->minimum() )
           //    green = deftg;
           //if(!((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(129))->specialValueText().isEmpty() &&
           //   blue == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(129))->minimum() )
           //    blue = deftb;

           //transparencyrange = 0.0;  //  No need to show transparency
           qDebug() << "trange sp text : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(126))->specialValueText();
           qDebug() << "trange min     : " << ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(126))->minimum();
           if(((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(126))->specialValueText() == "*Varies*" &&
              transparencyrange == ((QDoubleSpinBox*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(126))->minimum() )
               transparencyrange = deftrange;

            options.set(PlaneNoFill,fillstyle == 0);
            options.set(PlanePatterned,fillstyle == 2);
            options.set(PlaneFilled,fillstyle == 3);
            options.set(PlaneBitmapFilled,fillstyle == 4);
            options.set(PlaneGradientFilled,fillstyle == 5);

            options.set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
            options.set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
            options.set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
            options.set(PlaneGradientStyle4,(gradientfillstyle&1)==1);

            // enable disable controls depending on the current fill style
            // pattern/bitmap fill controls
            EnableWindow(GetDlgItem(dialog->gethdlg(),108),fillstyle == 2 || fillstyle == 4);// pattern/bitmap filename
            EnableWindow(GetDlgItem(dialog->gethdlg(),1022),fillstyle == 2 || fillstyle == 4);// pattern/bitmap file browse button
            EnableWindow(GetDlgItem(dialog->gethdlg(),110),fillstyle == 2 || fillstyle == 4);// pattern/bitmap image browser button

            // gradient fill controls
            EnableWindow(GetDlgItem(dialog->gethdlg(),121),fillstyle == 5);// gradient fill combo box
            //EnableWindow(GetDlgItem(dialog->gethdlg(),122),fillstyle == 5);
            //EnableWindow(GetDlgItem(dialog->gethdlg(),123),fillstyle == 5);
            //EnableWindow(GetDlgItem(dialog->gethdlg(),124),fillstyle == 5);
            EnableWindow(GetDlgItem(dialog->gethdlg(),1004),fillstyle == 5);// gradient fill colour button

            // sizing controls
            EnableWindow(GetDlgItem(dialog->gethdlg(),104),fillstyle == 1 || fillstyle == 2 || fillstyle == 4 || (fillstyle == 5 && (gradientfillstyle == 0 || gradientfillstyle == 1 || gradientfillstyle == 4 || gradientfillstyle == 5 || gradientfillstyle == 8 || gradientfillstyle == 9)));// xspacing
            EnableWindow(GetDlgItem(dialog->gethdlg(),105),fillstyle == 1 || fillstyle == 2 || fillstyle == 4 || (fillstyle == 5 && (gradientfillstyle == 2 || gradientfillstyle == 3 || gradientfillstyle == 6 || gradientfillstyle == 7)));// yspacing
            EnableWindow(GetDlgItem(dialog->gethdlg(),106),fillstyle == 1 || fillstyle == 2 || fillstyle == 4 || fillstyle == 5 );// angle
            EnableWindow(GetDlgItem(dialog->gethdlg(),107),fillstyle == 1);// cross hatch angle

            // transparent fill controls
            EnableWindow(GetDlgItem(dialog->gethdlg(),125),fillstyle == 3 || fillstyle == 4 || fillstyle == 5);// transparency decimal value spin box
            EnableWindow(GetDlgItem(dialog->gethdlg(),134),fillstyle == 3 || fillstyle == 4 || fillstyle == 5);// transparency % value spin box
            //EnableWindow(GetDlgItem(dialog->gethdlg(),1006),fillstyle == 3 || fillstyle == 4 || fillstyle == 5);// transparency dialog button (not used)
            ((QSlider*)((PlaneVerify_Dialog*)dialog->gethdlg())->getControl(303))->setEnabled((fillstyle == 3 || fillstyle == 4 || fillstyle == 5));// transparency slider
            //EnableWindow(GetDlgItem(dialog->gethdlg(),127),fillstyle == 4 );
            //EnableWindow(GetDlgItem(dialog->gethdlg(),128),fillstyle == 4 );
            //EnableWindow(GetDlgItem(dialog->gethdlg(),129),fillstyle == 4 );

            // bitmap transparent color and transparency range controls
            EnableWindow(GetDlgItem(dialog->gethdlg(),1005),fillstyle == 4 );// transparency colour and range dialog button
            EnableWindow(GetDlgItem(dialog->gethdlg(),1007),fillstyle == 4 );// transparenct colour dialog button
            EnableWindow(GetDlgItem(dialog->gethdlg(),126),fillstyle == 4 );// transparency value spin box

            EnableWindow(GetDlgItem(dialog->gethdlg(),1009), (transparency > 0.0 || transparencyrange > 0.0));
            EnableWindow(GetDlgItem(dialog->gethdlg(),1010), (transparency > 0.0 || transparencyrange > 0.0));
            EnableWindow(GetDlgItem(dialog->gethdlg(),1011), (transparency > 0.0 || transparencyrange > 0.0));

            gradientcolour = RGB(int(r*255.0),int(g*255.0),int(b*255.0));

            if (xspacing != 0.0 && xspacing < db.getptoler()) xspacing = 1.0;
            if (yspacing != 0.0 && yspacing < db.getptoler()) yspacing = 1.0;

            w = (output.getumax() - output.getumin());
            h = (output.getvmax() - output.getvmin());

            w2 = (output2.getumax() - output2.getumin());
            h2 = (output2.getvmax() - output2.getvmin());

            // init the scale display
            //SetDlgItemText(dialog->gethdlg(),302,"Scale 1:1");
            //dialog->SetDlgItemText(302,"Scale 1:1");

            // clear the current pattern Height
            dialog->SetDlgItemText(133, (char*)"");

            bool drawPreview = true;
            int numLines = 0;

            if (fillstyle == 2) // pattern fill
            {  dialog->SetDlgItemText(300, (char*)"Horizontal scale");
                 dialog->SetDlgItemText(301, (char*)"Vertical scale");

                 if(QString(patternfilename).isEmpty())
                    strcpy(patternfilename,v.getstring("pl::patternfilename"));
                 else
                    v.getstring("pl::patternfilename",patternfilename);

                 dialog->SetDlgItemText(108,patternfilename);

                 if (strlen(patternfilename) > 0 && infile.open(patternfilename,"rb"))
                 {
                      if (infile.readheader())
                      {
                          Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38);
                          Point pmin2(1.0E38,1.0E38,1.0E38),pmax2(-1.0E38,-10.E38,-1.0E38);

                           int numLines=0;
                           c = 0;
                           while ((e = infile.nextentity(NULL)) != NULL)
                           {
                               if (!e->isa(point_entity))
                                   numLines += getNumDrawSegments(e,(View3dSurface*)&output);

                               if (e->isa(point_entity))
                               {
                                    if (c == 0)
                                       e->extents(&output,&pmin,&pmax);
                                    else
                                    {
                                        e->extents(&output,&pmin1,&pmax1);
                                        if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                                        if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                                        if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                                        if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                                        if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                                        if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                                    }
                                    if (c == 0)
                                       e->extents(&output2,&pmin2,&pmax2);
                                    else
                                    {
                                        e->extents(&output2,&pmin3,&pmax3);
                                        if (pmin3.x < pmin2.x) pmin2.x = pmin3.x;
                                        if (pmin3.y < pmin2.y) pmin2.y = pmin3.y;
                                        if (pmin3.z < pmin2.z) pmin2.z = pmin3.z;
                                        if (pmax3.x > pmax2.x) pmax2.x = pmax3.x;
                                        if (pmax3.y > pmax2.y) pmax2.y = pmax3.y;
                                        if (pmax3.z > pmax2.z) pmax2.z = pmax3.z;
                                    }
                                    c++;
                               }
                               delete e;
                           }

                           // get the model units of the pattern file
                           int patternUnits=0;
                           Values patternValues(1);
                           infile.nextentity(NULL,&patternValues);
                           patternUnits = patternValues.getinteger("db::units");

                           infile.close();

                           if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                             {  pmin.setxyz(-10.0,-10.0,-10.0);
                                pmax.setxyz(10.0,10.0,10.0);
                             }
                           if (pmax2.x <= pmin2.x || pmax2.y <= pmin2.y)
                             {  pmin2.setxyz(-10.0,-10.0,-10.0);
                                pmax2.setxyz(10.0,10.0,10.0);
                             }

                           /*
                            * if the width of the pattern is greater than half the width of the window
                            * then reduce the width of the pattern and increase the scale
                            * else if width of the pattern is less than a half of the width of the window
                            * then increase the width of the pattern and reduce the scale
                            */
                           qDebug() << "width  :" << w;
                           qDebug() << "height :" << h;
                           qDebug() << "xspacing :" << xspacing;
                           qDebug() << "yspacing :" << yspacing;
                           qDebug() << "xspacing * (pmax.x - pmin.x) :" << xspacing * (pmax.x - pmin.x);
                           qDebug() << "yspacing * (pmax.y - pmin.y) :" << yspacing * (pmax.y - pmin.y);

                           scale = 1.0;
                           // display the height of the current pattern
                           //sprintf(s,"X%ls,Y%ls,Z%ls",db.formatnumber(text1,x,0),db.formatnumber(text2,y,0),db.formatnumber(text3,z));

                           double patternHeight = (pmax.y - pmin.y);// * toCurrentUnits(v.getinteger("db::units"),patternUnits);
                           RCCHAR text[300];
                           QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                           dialog->SetDlgItemText(133,strHeight.data());
#if 0
                           // pre v7 version
                           while (xspacing * (pmax.x - pmin.x) > w * 0.8 / 2.0)
                           {
                                xspacing /= 3.0;
                                yspacing /= 3.0;
                                scale *= 3.0;
                           }
#endif
#if 0
                           // v7 version
                           if(xspacing * (pmax.x - pmin.x) > h / 2.0)
                           {
                               while (xspacing * (pmax.x - pmin.x) > h / 2.0)
                               {
                                    xspacing /= 3.0;
                                    yspacing /= 3.0;
                                    scale *= 3.0;
                               }
                           }
                           else if(xspacing * (pmax.x - pmin.x) < h / 2.0)
                           {
                               while (xspacing * (pmax.x - pmin.x) < h / 2.0)
                               {
                                    xspacing *= 3.0;
                                    yspacing *= 3.0;
                                    scale /= 3.0;
                               }
                           }
                           sprintf(text,"Scale 1:%.7f",scale);
                           SetDlgItemText(dialog->gethdlg(),302,text);
                           double previewHeight = h * scale;
                           ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,previewHeight);
#endif
#if 1 // v8 version
                           ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                           xspacing /= scale / toCurrentUnits(v.getinteger("db::units"),patternUnits);
                           yspacing /= scale / toCurrentUnits(v.getinteger("db::units"),patternUnits);
                           ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                           qDebug() << "# in w : " << ((w * 0.8 /scale) / ((pmax2.x - pmin2.x) / scale * xspacing));
                           qDebug() << "# in h : " << ((h * 0.8 /scale) / ((pmax2.y - pmin2.y) / scale * yspacing));
                           qDebug() << "#w * #h : " << (((w * 0.8 /scale) / ((pmax2.x - pmin2.x) / scale * xspacing)) * ((h * 0.8 / scale) / ((pmax2.y - pmin2.y) / scale  * yspacing)));
                           qDebug() << "total #w lines : " << numLines * ((w * 0.8 / scale) / ((pmax2.x - pmin2.x) / scale * xspacing));
                           qDebug() << "total #h lines : " << numLines * ((h * 0.8 / scale) / ((pmax2.y - pmin2.y) / scale * yspacing));

                           Point po = output.modeltoscreen(Point(0.0,0.0,0.0));
                           Point pb = output.modeltoscreen(Point((pmax.x - pmin.x) * xspacing,(pmax.y - pmin.y) * yspacing,0.0));
                           int dx = pb.x - po.x;
                           int dy = pb.y - po.y;
                           int maxXlines = 50000;
                           int maxYlines = 50000;

                           if(min(dx,dy) < 7 )
                           //if(numLines * ((w * 0.8 / scale) / ((pmax2.x - pmin2.x) / scale * xspacing)) > maxXlines ||
                           //   numLines * ((h * 0.8 / scale) / ((pmax2.y - pmin2.y) / scale * yspacing)) > maxYlines ||
                           //   min(dx,dy) < 5)
                           //if(numLines * (((w * 0.8 / scale) / ((pmax2.x - pmin2.x) / scale * xspacing)) * ((h * 0.8 / scale) / ((pmax2.y - pmin2.y) / scale * yspacing))) > 100000)
                               drawPreview = false;
#endif
                           // sample preview
                           xspacing2 = w2 * 0.8 / (pmax2.x - pmin2.x);
                           yspacing2 = xspacing2;
                      }
                   }

            }
            else if (fillstyle == 4) // bitmap fill
            {
                 if(QString(patternfilename).isEmpty())
                    strcpy(patternfilename,v.getstring("pl::bitmapfilename"));
                 else
                    v.getstring("pl::bitmapfilename",patternfilename);

                 dialog->SetDlgItemText(108,patternfilename);

                 dialog->SetDlgItemText(300, (char*)"Bitmap width");
                 dialog->SetDlgItemText(301, (char*)"Bitmap height");

                 double patternHeight = yspacing;
                 RCCHAR text[300];
                 QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                 //QString strHeight = QString("%1").arg(patternHeight);
                 dialog->SetDlgItemText(133,strHeight.data());
#if 0
                 // v7 version
                 scale = 1.0;
                 double previewHeight = h * scale;

                 if(xspacing * 2 > w / 2.0)
                 {
                     while (xspacing * 2 > w / 2.0)
                     {
                          xspacing /= 2.0;
                          yspacing /= 2.0;
                          scale *= 2.0;
                     }
                     sprintf(text,"Scale 1:%.0lf",scale);
                     SetDlgItemText(dialog->gethdlg(),302,text);
                     previewHeight = h * scale;
                 }
                 else
                 {
                     while (yspacing < h * 0.8 / 2.0)
                     {
                          xspacing *= 2.0;
                          yspacing *= 2.0;
                          scale /= 2.0;
                     }
                     sprintf(text,"Scale 1:%.6lf",scale);
                     SetDlgItemText(dialog->gethdlg(),302,text);
                     previewHeight = h * 0.8 * scale;
                 }
#else
                 // v8 version
                 ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                 xspacing /= scale;
                 yspacing /= scale;
                 ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                 qDebug() << "# in w : " << w * 0.8 / xspacing;
                 qDebug() << "# in h : " << h * 0.8 / yspacing;

                 Point io = output.modeltoscreen(Point(0,0,0));
                 Point iw = output.modeltoscreen(Point(xspacing,yspacing,0));
                 double dx = iw.x-io.x;
                 double dy = iw.y-io.y;
                 if(min(dx,dy) <= 4)
                 //if( w * 0.8 / xspacing > 50 || h * 0.8 / yspacing > 20)
                 //if(xspacing < 5 || yspacing < 5)
                     drawPreview = false;

#endif
                 // sample preview
                 xspacing2 = w2 * 0.8;
                 yspacing2 = xspacing2;

            }
            else if(fillstyle == 1) // hatching
            {
                 dialog->SetDlgItemText(300, (char*)"Spacing");
                 dialog->SetDlgItemText(301, (char*)"Cross hatch spacing");

#if 0
                 // v7 version
                 scale = 1.0;
                 while (xspacing > w / 2.0)
                   {  xspacing /= 5.0;
                      yspacing /= 5.0;
                      scale *= 5.0;
                   }
                 sprintf(text,"Scale 1:%.0lf",scale);
                 SetDlgItemText(dialog->gethdlg(),302,text);
#else
                 double patternHeight = xspacing * 1.4;
                 if(yspacing > xspacing)
                     patternHeight = xspacing * (yspacing / xspacing) * 1.4;

                 RCCHAR text[300];
                 QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                 //QString strHeight = QString("%1").arg(patternHeight);
                 dialog->SetDlgItemText(133,strHeight.data());

                 // sample preview
                 xspacing2 = w2 * 0.7 * (xspacing / max(xspacing,yspacing));
                 yspacing2 = h2 * 0.7 * (yspacing / max(xspacing,yspacing));
                 org2 = Point(1.4,1.4,0.0);

                 // dynamic preview
                 ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                 xspacing /= scale;
                 yspacing /= scale;
                 ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                 // limit hatching to not draw if the space between is less than the 2 line widths in pixels
                 // in either direction
                 Point po = output.modeltoscreen(Point(0.0,0.0,0.0));
                 Point pb = output.modeltoscreen(Point(xspacing,yspacing,0.0));
                 int dx = pb.x-po.x;
                 int dy = pb.y-po.y;
                 int iwt = db.lineweights.mmtoweight(wt);
                 int cw = db.getlineweightdisplay() ? db.lineweights.weighttopx(iwt) : db.lineweights.weighttopx(0);

                 //qDebug() << "dx :  " << dx << " w :" << cw;
                 //qDebug() << "dy :  " << dx << " w :" << cw;

                 if(dx <= cw || dy <= cw)
                     drawPreview = false;

#endif
            }
            else if(fillstyle == 3) // solid plane filled
            {
                 dialog->SetDlgItemText(300, (char*)"Spacing");
                 dialog->SetDlgItemText(301, (char*)"Cross hatch spacing");

                 double patternHeight = output.getheightmm() * 0.8;
                 RCCHAR text[300];
                 QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                 //QString strHeight = QString("%1").arg(patternHeight);
                 dialog->SetDlgItemText(133,strHeight.data());

                 // dynamic preview
                 ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                 xspacing *= 1.0/scale;
                 yspacing *= 1.0/scale;
                 ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                 // sample preview
                 xspacing2 = w2 * 0.8;
                 yspacing2 = xspacing2;
            }
            else if(fillstyle == 5) // gradient filled
            {
                 dialog->SetDlgItemText(300, (char*)"Spacing");
                 dialog->SetDlgItemText(301, (char*)"Cross hatch spacing");

                 double patternHeight = xspacing * 10.0;
                 RCCHAR text[300];
                 QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                 //QString strHeight = QString("%1").arg(patternHeight);
                 dialog->SetDlgItemText(133,strHeight.data());

                 // dynamic preview
                 ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                 xspacing *= 1.0/scale;
                 yspacing *= 1.0/scale;
                 ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                 // sample preview
                 xspacing2 = w2 * 0.8 / 10.0;
                 yspacing2 = xspacing2;
            }
            else if(fillstyle == 0) // no plane fill
            {
                 dialog->SetDlgItemText(300, (char*)"Spacing");
                 dialog->SetDlgItemText(301, (char*)"Cross hatch spacing");

                 double patternHeight = output.getheightmm() * 0.8;
                 RCCHAR text[300];
                 QString strHeight = QString("%1").arg(QString(db.formatnumber(text,patternHeight,FormatNumberAlwaysShowUnits)));
                 //QString strHeight = QString("%1").arg(patternHeight);
                 dialog->SetDlgItemText(133,strHeight.data());

                 // dynamic preview
                 ((RealDialogControl*)dialog->getcontrol(130))->currentvalue(dialog,&scale);
                 xspacing *= 1.0/scale;
                 yspacing *= 1.0/scale;
                 ((RealDialogControl*)dialog->getcontrol(130))->change(dialog,scale);

                 // sample preview
                 xspacing2 = w2 * 0.8;
                 yspacing2 = xspacing2;
            }

            // sample preview all types are now displayed
            //if(fillstyle == 2 || fillstyle == 3 || fillstyle == 4 || fillstyle == 5)
            {
                double factor=0.8;
                p1.setxyz(0.0,0.0,0.0);  p2.setxyz(w2*factor,0.0,0.0);
                Line *line1 = new Line(p1,p2);
                p1.setxyz(w2*factor,0.0,0.0);  p2.setxyz(w2*factor,w2*factor,0.0);
                Line *line2 = new Line(p1,p2);
                p1.setxyz(w2*factor,w2*factor,0.0);  p2.setxyz(0.0,w2*factor,0.0);
                Line *line3 = new Line(p1,p2);
                p1.setxyz(0.0,w2*factor,0.0);  p2.setxyz(0.0,0.0,0.0);
                Line *line4 = new Line(p1,p2);
                if (line1 != NULL && line2 != NULL && line3 != NULL && line4 != NULL)
                {
                    list2.add(line1);  list2.add(line2);  list2.add(line3);  list2.add(line4);

                     // apply the header values to the preview
                     EntityHeader header(cl,l,0,db.lineweights.mmtoweight(wt));
                     Plane plane2(header,org2,xaxis,yaxis,list2,xspacing2,yspacing2,angle,crossangle,0.0,red,green,blue,0.0,gradientcolour,patternfilename,options);
                     if(options.test(PlaneNoFill) && !options.test(PlaneDisplayEdges))
                     {
                        line1->draw(DM_NORMAL,NULL,&output2);
                        line2->draw(DM_NORMAL,NULL,&output2);
                        line3->draw(DM_NORMAL,NULL,&output2);
                        line4->draw(DM_NORMAL,NULL,&output2);
                     }
                     plane2.draw(DM_NORMAL,NULL,&output2);
                }
            }

            // dynamic preview
            if(v.getinteger("pl::showpreview") == 1 && drawPreview)
            {
                // setup the transparency background for plane fiell, bitmap fill and gradation fill
                //if( drawPreview && (transparency > 0.0 || (fillstyle == PlaneBitmapFilled && transparencyrange > 0.0)))
                if( (drawPreview && (transparencyrange > 0.0 && fillstyle == 4)) || (transparency > 0.0 && (fillstyle == 3 || fillstyle == 4 || fillstyle == 5)))
                {
                    QColor foreColour(255,255,255);
                    QSize bsize;
                    double sfactor = ctl->height();
                    QPixmap backpix;
                    switch(backstyle)
                    {
                    case 0:
                    default: // checker board
                        if(backColour == foreColour)
                            backColour = QColor(190,190,190);
                        bsize = QSize(100,100);
                        backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                        sfactor = ctl->height() / 2.9;
                        scene->setBackgroundBrush(QBrush(backpix.scaledToHeight(sfactor)));
                        break;
                    case 1: // single colour
                        bsize = QSize(ctl->width(),ctl->height());
                        backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                        sfactor = ctl->height() / 2.9;
                        scene->setBackgroundBrush(backpix);
                    break;
                    case 2: // half and half
                        bsize = QSize(ctl->width(),ctl->height());
                        if(backColour == foreColour)
                            backColour = QColor(0,0,0);
                        backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                        sfactor = ctl->height() / 2.9;
                        scene->setBackgroundBrush(backpix);
                    break;
                    };
                }
                double factor=0.8;
                p1.setxyz(0.0,0.0,0.0);  p2.setxyz(w*factor,0.0,0.0);
                Line *line1 = new Line(p1,p2);
                p1.setxyz(w*factor,0.0,0.0);  p2.setxyz(w*factor,h*factor,0.0);
                Line *line2 = new Line(p1,p2);
                p1.setxyz(w*factor,h*factor,0.0);  p2.setxyz(0.0,h*factor,0.0);
                Line *line3 = new Line(p1,p2);
                p1.setxyz(0.0,h*factor,0.0);  p2.setxyz(0.0,0.0,0.0);
                Line *line4 = new Line(p1,p2);
                if (line1 != NULL && line2 != NULL && line3 != NULL && line4 != NULL)
                {
                    list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);

                     // apply the header values to the preview
                     EntityHeader header(cl,l,0,db.lineweights.mmtoweight(wt));
                     Plane plane(header,org,xaxis,yaxis,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                     if(options.test(PlaneNoFill) && !options.test(PlaneDisplayEdges))
                     {
                        line1->draw(DM_NORMAL,NULL,&output);
                        line2->draw(DM_NORMAL,NULL,&output);
                        line3->draw(DM_NORMAL,NULL,&output);
                        line4->draw(DM_NORMAL,NULL,&output);
                     }
                     //qDebug() << "px mm " << output.getpixelmodelmm();
                     //qDebug() << "mm " << min(xspacing,yspacing) / output.getydotspermm() * output.getscale();
                     plane.draw(DM_NORMAL,NULL,&output);
                }
            }
            else
            {
                // force a white background
                scene->setBackgroundBrush(QBrush(QColor(255.0,255.0,255.0)));
                //output.setbackground(RGB(255.0,255.0,255.0));
                // show the text messages
                QGraphicsTextItem *gtext = scene->addText("");
                gtext->setTextWidth(ctl->width());
                if(v.getinteger("pl::showpreview") == 0)
                     gtext->setHtml("<html><body><div align=\"center\"><h1 align=\"center\">Preview is Disabled</h1><div></body></html>");
                else if(fillstyle == 4)
                     gtext->setHtml("<html><body><div align=\"center\"><h1 align=\"center\">Cannot display Preview</h1><h3 align=\"center\">width or height are too small</h3><div></body></html>");
                else if(fillstyle == 2)
                     gtext->setHtml("<html><body><div align=\"center\"><h1 align=\"center\">Cannot display Preview</h1><h3 align=\"center\">horizontal or vertical scale are too small</h3><div></body></html>");
                else if(fillstyle == 1)
                     gtext->setHtml("<html><body><div align=\"center\"><h1 align=\"center\">Cannot display Preview</h1><h3 align=\"center\">spacing values are too small</h3><div></body></html>");
            }
       }

    }
}
#else
void PlaneDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E10,1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 Point p1,p2,pmin,pmax,pmin1,pmax1;
 double w,h,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,scale,r,g,b;
 RCCHAR patternfilename[300],text[300];
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);
 EntityList list;
 int c,fillstyle,gradientfillstyle;
 CadFile infile;
 Entity *e;
 RCCOLORREF gradientcolour;

#if MENUS == FLEXICADMENUS
  ShowWindow(GetDlgItem(dialog->gethdlg(),105),SW_HIDE);
  ShowWindow(GetDlgItem(dialog->gethdlg(),106),SW_HIDE);
  ShowWindow(GetDlgItem(dialog->gethdlg(),206),SW_HIDE);
  ShowWindow(GetDlgItem(dialog->gethdlg(),107),SW_HIDE);
#endif

  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
       IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       output.moveto(0,0);
       output.lineto(rect.right-1,0);
       output.lineto(rect.right-1,rect.bottom-1);
       output.lineto(0,rect.bottom-1);
       output.lineto(0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(100,&fillstyle) &&
           dialog->currentvalue(106,&options) &&
           dialog->currentvalue(107,patternfilename,300) &&
           dialog->currentvalue(110,&gradientfillstyle) &&
           dialog->currentvalue(111,&r) &&
           dialog->currentvalue(112,&g) &&
           dialog->currentvalue(113,&b) &&
           dialog->currentvalue(115,&xspacing) &&
           dialog->currentvalue(116,&yspacing) &&
           dialog->currentvalue(117,&angle) &&
           dialog->currentvalue(118,&crossangle) &&
           dialog->currentvalue(121,&transparency) &&
           dialog->currentvalue(124,&red) &&
           dialog->currentvalue(125,&green) &&
           dialog->currentvalue(126,&blue) &&
           dialog->currentvalue(128,&transparencyrange))
         {  transparency = 0.0;  //  No need to show transparency
            options.set(PlaneNoFill,fillstyle == 0);
            options.set(PlanePatterned,fillstyle == 2);
            options.set(PlaneFilled,fillstyle == 3);
            options.set(PlaneBitmapFilled,fillstyle == 4);
            options.set(PlaneGradientFilled,fillstyle == 5);

            options.set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
            options.set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
            options.set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
            options.set(PlaneGradientStyle4,(gradientfillstyle&1)==1);

            EnableWindow(GetDlgItem(dialog->gethdlg(),107),fillstyle == 2 || fillstyle == 4);
            EnableWindow(GetDlgItem(dialog->gethdlg(),108),fillstyle == 2 || fillstyle == 4);
            EnableWindow(GetDlgItem(dialog->gethdlg(),109),fillstyle == 2 || fillstyle == 4);

            EnableWindow(GetDlgItem(dialog->gethdlg(),110),fillstyle == 5);
            EnableWindow(GetDlgItem(dialog->gethdlg(),111),fillstyle == 5);
            EnableWindow(GetDlgItem(dialog->gethdlg(),112),fillstyle == 5);
            EnableWindow(GetDlgItem(dialog->gethdlg(),113),fillstyle == 5);
            EnableWindow(GetDlgItem(dialog->gethdlg(),114),fillstyle == 5);

            EnableWindow(GetDlgItem(dialog->gethdlg(),115),fillstyle == 1 || fillstyle == 2 || fillstyle == 4 || fillstyle == 5 && (gradientfillstyle == 0 || gradientfillstyle == 1 || gradientfillstyle == 4 || gradientfillstyle == 5 || gradientfillstyle == 8 || gradientfillstyle == 9));
            EnableWindow(GetDlgItem(dialog->gethdlg(),116),fillstyle == 1 || fillstyle == 2 || fillstyle == 4 || fillstyle == 5 && (gradientfillstyle == 2 || gradientfillstyle == 3 || gradientfillstyle == 6 || gradientfillstyle == 7));
            EnableWindow(GetDlgItem(dialog->gethdlg(),117),fillstyle == 1 || fillstyle == 2);
            EnableWindow(GetDlgItem(dialog->gethdlg(),118),fillstyle == 1);

            gradientcolour = RGB(int(r*255.0),int(g*255.0),int(b*255.0));

            if (xspacing != 0.0 && xspacing < db.getptoler()) xspacing = 1.0;
            if (yspacing != 0.0 && yspacing < db.getptoler()) yspacing = 1.0;
            w = (output.getumax() - output.getumin());
            h = (output.getvmax() - output.getvmin());

            SetDlgItemText(dialog->gethdlg(),302,"Scale 1:1");

            if (fillstyle == 2)
              {  dialog->SetDlgItemText(300,"Horizontal scale");
                 dialog->SetDlgItemText(301,"Vertical scale");

                 if (strlen(patternfilename) > 0 && infile.open(patternfilename,"rb"))
                   {  if (infile.readheader())
                        {Point pmin(1.0E38,1.0E38,1.0E38),pmax(-1.0E38,-10.E38,-1.0E38);

                           c = 0;
                           while ((e = infile.nextentity(NULL)) != NULL)
                             {  if (e->isa(point_entity))
                                  {  if (c == 0)
                                       e->extents(&output,&pmin,&pmax);
                                     else
                                       {  e->extents(&output,&pmin1,&pmax1);
                                          if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                                          if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                                          if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                                          if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                                          if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                                          if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                                       }
                                     c++;
                                  }
                                delete e;
                             }
                           if (pmax.x <= pmin.x || pmax.y <= pmin.y)
                             {  pmin.setxyz(-10.0,-10.0,-10.0);
                                pmax.setxyz(10.0,10.0,10.0);
                             }

                           scale = 1.0;  
                           while (xspacing * (pmax.x - pmin.x) > w / 2.0)
                             {  xspacing /= 3.0;             
                                yspacing /= 3.0;             
                                scale *= 3.0;
                             }
                           sprintf(text,"Scale 1:%.0lf",scale);
                           SetDlgItemText(dialog->gethdlg(),302,text); 

                        }
                      infile.close();

                   }

              }
            else if (fillstyle == 4)
              {  
                 dialog->SetDlgItemText(300,"Bitmap width");
                 dialog->SetDlgItemText(301,"Bitmap height");

                 scale = 1.0;
                 while (xspacing * 2 > w / 2.0)
                   {  xspacing /= 3.0;
                      yspacing /= 3.0;
                      scale *= 3.0;
                   }

                 sprintf(text,"Scale 1:%.0lf",scale);
                 SetDlgItemText(dialog->gethdlg(),302,text);

              }
            else
              {  dialog->SetDlgItemText(300,"Spacing");
                 dialog->SetDlgItemText(301,"Cross hatch spacing");
                 scale = 1.0;  
                 while (xspacing > w / 2.0)
                   {  xspacing /= 5.0;             
                      yspacing /= 5.0;             
                      scale *= 5.0;
                   }
                 sprintf(text,"Scale 1:%.0lf",scale);
                 SetDlgItemText(dialog->gethdlg(),,text);

              }

            p1.setxyz(0.0,0.0,0.0);  p2.setxyz(w*0.8,0.0,0.0);
            Line *line1 = new Line(p1,p2);
            p1.setxyz(w*0.8,0.0,0.0);  p2.setxyz(w*0.8,h*0.8,0.0);
            Line *line2 = new Line(p1,p2);
            p1.setxyz(w*0.8,h*0.8,0.0);  p2.setxyz(0.0,h*0.8,0.0);
            Line *line3 = new Line(p1,p2);
            p1.setxyz(0.0,h*0.8,0.0);  p2.setxyz(0.0,0.0,0.0);
            Line *line4 = new Line(p1,p2);
            if (line1 != NULL && line2 != NULL && line3 != NULL && line4 != NULL)
              {  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                 Plane plane(org,xaxis,yaxis,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 line1->draw(DM_NORMAL,NULL,&output);
                 line2->draw(DM_NORMAL,NULL,&output);
                 line3->draw(DM_NORMAL,NULL,&output);
                 line4->draw(DM_NORMAL,NULL,&output);
                 plane.draw(DM_NORMAL,NULL,&output);
              }
         }
    }
}
#endif

class PSButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     PSButtonDialogControl(int id) : ButtonDialogControl(id)  {  }
     int select(Dialog *);
  };

// preview reset scale button control
class PRButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     PRButtonDialogControl(int id) : ButtonDialogControl(id)  {  }
     int select(Dialog *);
  };

int PRButtonDialogControl::select(Dialog *dlg)
{

    // reset the scale
    if (id == 304)
    {
        double scale = cadwindow->getcurrentsurface()->getscale();
        ((RealDialogControl*)dlg->getcontrol(130))->change(dlg,scale);
    }
    else if(id == 305)
    {

    }
    return 0;
}

#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD)
static UINT APIENTRY PreviewHookProc(HWND hdlg,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{static RCCHAR lastfilename[300],filename[300];
 CadFile infile;
 Entity *e;
 Point pmin,pmax,pmin1,pmax1;
 long c;
 double du,dv,aspect;
#if ! defined(_WIN32_WCE)
  if (uiMsg == 1125)
    return 0;

  // KMJ fix this
  //CommDlg_OpenSave_GetFilePath(GetParent(hdlg),filename,300);

  if (uiMsg == WM_INITDIALOG)
    {  CheckDlgButton(hdlg,2001,1);
       strcpy(lastfilename,filename);
    }
  else if (strcmp(filename,lastfilename) != 0)
    {  InvalidateRect(GetDlgItem(hdlg,2000),0,TRUE);
       strcpy(lastfilename,filename);
    }

  if (uiMsg == WM_DRAWITEM)
    {DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT *) lParam;
     ResourceString rs0(NCVIEWA);
     View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
     RECT rect;
     RCHPEN hpen,oldhpen;
     EntityHeader header(0,0,0,0);
     Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
     BitMask options(32);
     HDC hdc;
       if (uiMsg == WM_DRAWITEM)
         hdc = dis->hDC;
       else
         hdc = 0;
       View3dOutput output(GetDlgItem(hdlg,2000),hdc,&top,1.0);
       if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
         {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
            IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
            rect.left = 0;  rect.top = 0;
            rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
            FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
            MoveToEx(output.gethdc(),0,0,0);
            LineTo(output.gethdc(),rect.right-1,0);
            LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
            LineTo(output.gethdc(),0,rect.bottom-1);
            LineTo(output.gethdc(),0,0);
            if (IsDlgButtonChecked(hdlg,2001) == BST_CHECKED && strlen(filename) > 0 && infile.open(filename,"rb"))
              {  SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
                 if (infile.readheader())
                   {  c = 0;
                      while ((e = infile.nextentity(NULL)) != NULL)
                        {  if (e->getvisible()->test(0))
                             {  if (c == 0)
                                  e->extents(&output,&pmin,&pmax);
                                else
                                  {  e->extents(&output,&pmin1,&pmax1);
                                     if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                                     if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                                     if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                                     if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                                     if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                                     if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                                  }
                                 c++;
                             }
                           delete e;
                        }
                      du = fabs(pmax.x - pmin.x) / 2.0;  dv = fabs(pmax.y - pmin.y) / 2.0;
                      if (du > db.getptoler() || dv > db.getptoler())
                        {  aspect = output.getwidthmm() / output.getheightmm();
                           if (du / dv > aspect) dv = du / aspect;
                           if (du / dv < aspect) du = dv * aspect;
                           pmin.x = (pmin.x + pmax.x) / 2.0 - du;
                           pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
                           pmax.x = (pmin.x + pmax.x) / 2.0 + du;
                           pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
                           output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);
                        }
                   }
                 infile.close();
                 if (infile.open(lastfilename,"rb"))
                   {  if (infile.readheader())
                        {  while ((e = infile.nextentity(NULL)) != NULL)
                             {  if (e->getvisible()->test(0))
                                  e->draw(DM_NORMAL,NULL,&output);
                                delete e;
                             }
                        }
                      infile.close();
                   }
              }
            else
              {  MoveToEx(output.gethdc(),0,rect.bottom-1,0);
                 LineTo(output.gethdc(),rect.right-1,0);
                 MoveToEx(output.gethdc(),0,0,0);
                 LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
                 SelectObject(output.gethdc(),oldhpen);
                 DeleteObject(hpen);
              }
         }
        return 1;
    }
#endif
  return 0;
}

static LPWORD lpwAlign ( LPWORD lpIn)
{ULONG ul;
  ul = (ULONG) lpIn;
  ul +=3;
  ul >>=2;
  ul <<=2;
  return (LPWORD) ul;
}

static int nCopyAnsiToWideChar (LPWORD lpWCStr, RCCHAR* lpAnsiIn)
{int cchAnsi = strlen(lpAnsiIn);
    return 0;
  //return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, lpAnsiIn, cchAnsi, (wchar_t *)lpWCStr, cchAnsi) + 1;
}
#endif


class HPListDialogControl : public ListDialogControl
{
   private:
    RCCHAR *type;
    int mfillstyle;
   public:
     HPListDialogControl(int id,int fillstyle) :
         ListDialogControl(id,0,(RCCHAR**)0,(RCCHAR*)0,300)  { mfillstyle = fillstyle; }
     void load(Dialog *);
};

class HPScrollBarDialogControl : public ScrollBarDialogControl
  {private:
     int *base,n;
   public:
     HPScrollBarDialogControl(int i,int *b) : ScrollBarDialogControl(i,0) {  base = b; }
     void vscrollevent(Dialog *,int,int,int);
     void setrange(Dialog *,int);
     void load(Dialog *dialog);
  };

/*
// defined in ncdialog.h
class HPDisplayDialogControl : public DisplayDialogControl
  {private:
     int *base;
     int fillstyle;
     RCCHAR lastdirectory[300];
   public:
     HPDisplayDialogControl(int id,int fs,int *bs) : DisplayDialogControl(id) {  fillstyle = fs;  base = bs;  strcpy(lastdirectory,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void HPDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hWnd,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation,pmin,pmax;
 BitMask options(32);
 RCCHAR directory[300],filename[300];
 double s;
 long handle;
 struct RC_finddata_t fd;
 int i;

     if(hWnd == 0)
        return;

     QGraphicsView *ctl = 0;
     if(dialog->gethdlg())
         ctl = (QGraphicsView*)this->getQW();
     //ctl = (QGraphicsView*)((SelectHatchPattern_Dialog*)dialog->gethdlg())->getControl(id);
     QGraphicsScene *scene = 0;
     if(ctl)
         scene = ctl->scene();

    if(ctl && scene)
    //if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {
        output.setScene(scene);
        scene->setSceneRect(QRectF());
        scene->clear();
        QRect qrect = ctl->rect();

        scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                              ::v.getreal("wn::backgroundcolour.green")*255.0,
                                              ::v.getreal("wn::backgroundcolour.blue")*255.0)));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                               ::v.getreal("wn::backgroundcolour.green")*255.0,
                               ::v.getreal("wn::backgroundcolour.blue")*255.0));

        output.sizeevent(qrect.width(),qrect.height());
        scene->update();
#if 0
       oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       output.moveto(0,0);
       output.lineto(rect.right-1,0);
       output.lineto(rect.right-1,rect.bottom-1);
       output.lineto(0,rect.bottom-1);
       output.lineto(0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       IntersectClipRect(output.gethdc(),1,1,int(output.getwidth())-1,int(output.getheight())-1);
#endif

       GetDlgItemText(dialog->gethdlg(),114,directory,300);
       if (_stricmp(lastdirectory,directory) != 0)
         {  strcat(directory,"/*.*");
            handle = _findfirst(directory,&fd);
            i = 0;
            while (handle != -1)
              {  if (strlen(fd.name) > 4 && 
                      ((fillstyle == 4 && (_stricmp(fd.name+strlen(fd.name)-4,".bmp") == 0 || _stricmp(fd.name+strlen(fd.name)-4,".jpg") == 0) ||
                       (fillstyle == 2 && _stricmp(fd.name+strlen(fd.name)-4,".cad") == 0))))
                   {  i++;
                   }

                 if (_findnext(handle,&fd) != 0)
                   {  _findclose(handle);
                      handle = -1;
                   } 
              }
            ((HPScrollBarDialogControl *)dialog->getcontrol(118))->setrange(dialog,i);
            strcpy(lastdirectory,directory);

         }

       //  Update the filename titles
       GetDlgItemText(dialog->gethdlg(),114,directory,300);
       strcat(directory,"/*.*");
       handle = _findfirst(directory,&fd);
       i = id - 100 + *base;
       while (handle != -1)
         {  if (strlen(fd.name) > 4 && 
                 ((fillstyle == 4 && (_stricmp(fd.name+strlen(fd.name)-4,".bmp") == 0 || _stricmp(fd.name+strlen(fd.name)-4,".jpg") == 0) ||
                  (fillstyle == 2 && _stricmp(fd.name+strlen(fd.name)-4,".cad") == 0))))
              {  GetDlgItemText(dialog->gethdlg(),114,directory,300);
                 strcpy(filename,directory);
                 strcat(filename,"/");
                 strcat(filename,fd.name);
                 if (i == 0)
                   break;
                 i--; 
              }

            if (_findnext(handle,&fd) != 0)
              {  _findclose(handle);
                 handle = -1;
              } 

         }

       if (handle != -1)
         {  _findclose(handle);

            if (strchr(fd.name,'.') != 0)
              strchr(fd.name,'.')[0] = 0;

            SetDlgItemText(dialog->gethdlg(),id + 100,fd.name);
            SetDlgItemText(dialog->gethdlg(),id + 200,filename);


            Figure figure1(filename,_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
            figure1.extents(0,&pmin,&pmax);

            s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
            if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
              s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
            if (s < 0.00001)
              s = 1.0;

            s *= 1.1;

            if (fillstyle == 2)
              {  org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
                 Figure figure2(filename,_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                 figure2.draw(DM_NORMAL,NULL,&output);
              }
            else
              {BitMask bo(32);            
                 BitMapE bitmap(filename,Point(output.getumin(),output.getvmin(),0.0),
                                 Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),
                                 output.getumax()-output.getumin(),output.getvmax()-output.getvmin(),0.0,0.0,0.0,0.0,1.0,bo);
                  
                 bitmap.draw(DM_NORMAL,NULL,&output);
              }
         }
       else
         SetDlgItemText(dialog->gethdlg(),id + 100, (char*)"-");
         
    }
}

void HPListDialogControl::load(Dialog *dialog)
{RCCHAR key1[256],*s1;
 int i;

  removeallitems(dialog);
  if (mfillstyle == 2)
  {
      for (i = 0 ; i < 10 ; i++)
      {
           sprintf(key1,"pl::hatchpatterndirectory%d",i+1);
           if (strlen(s1 = v.getstring(key1)) != 0)
             SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)v.getstring(key1)));
           if (i == 0)
             dialog->SetDlgItemText(id,v.getstring(key1));
      }
  }
  else if (mfillstyle == 4)
  {
      for (i = 0 ; i < 10 ; i++)
      {
           sprintf(key1,"pl::bitmapfilldirectory%d",i+1);
           if (strlen(s1 = v.getstring(key1)) != 0)
             SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)v.getstring(key1)));
           if (i == 0)
             dialog->SetDlgItemText(id,v.getstring(key1));
      }
  }
}

class HPButtonDialogControl : public ButtonDialogControl
  {private:
     //RCCHAR _filename[300];
     RCCHAR *filename;
     int *base;
     int fillstyle;
   public:
     HPButtonDialogControl(int id,int fs,int *bs,RCCHAR *fn) : ButtonDialogControl(id)  {   base = bs;  fillstyle = fs;  /*strcpy(_filename,fn);*/ filename = fn;  }
     int select(Dialog *);
  };

void HPScrollBarDialogControl::vscrollevent(Dialog *dialog,int scrollid,int scrollcode,int pos)
{DisplayDialogControl *ddc;
 int start,end;
 GeneralList list;

  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {
       case SB_TOP : *base = 0; break;
       case SB_BOTTOM : *base = n; break;//n - 12;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : *base += 4;  break;
       case SB_LINEUP : *base -= 4;  break;
       case SB_PAGEDOWN : *base += 12;break; //12 / 2;
       case SB_PAGEUP : *base -= 12; break;//12 / 2;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
       default:
         *base = pos;
         break;
    }
  if (*base > n) *base = n;
  if (*base < 0) *base = 0;
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,*base,TRUE);

  list = dialog->getdisplaylist();
  for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
    ddc->draw(dialog,0);

}

void HPScrollBarDialogControl::load(Dialog *dialog)
{ n = 20;
  SetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,0,12,TRUE);
}

void HPScrollBarDialogControl::setrange(Dialog *dialog,int n1)
{ //n = (n1/4)*4-8;
  n = n1 - 12;
  SetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,0,n,TRUE);
}


int HPButtonDialogControl::select(Dialog *dialog)
{OPENFILENAME ofn;
 RCCHAR dirname[300],sfilename[300],filetitle[300],filter[300],key1[300],key2[300],text[300],directory[300];
 RCCHAR caption[300],defextension[10];
 int i,handle;
 struct RC_finddata_t fd;

   if (id < 112)
     {  GetDlgItemText(dialog->gethdlg(),id+200,filename,300);
        return id;
     }
   else if (id == 112 )
     {  //if (*base > 0)
        //  (*base) -= 12;
        //if (*base < 0)
        //  *base = 0;
        ((SelectHatchPattern_Dialog*)dialog->gethdlg())->trigger_118(QAbstractSlider::SliderPageStepSub);
        //((SelectHatchPattern_Dialog*)dialog->gethdlg())->draw();
        //InvalidateRect(dialog->gethdlg(),0,TRUE);
        return 0;
     }
   else if (id == 113)
     {  GetDlgItemText(dialog->gethdlg(),211,text,300);
        //if (strcmp(text,"-") != 0)
          //(*base) += 12;
        if (strcmp(text,"-") != 0)
            ((SelectHatchPattern_Dialog*)dialog->gethdlg())->trigger_118(QAbstractSlider::SliderPageStepAdd);
        //((SelectHatchPattern_Dialog*)dialog->gethdlg())->draw();
        //InvalidateRect(dialog->gethdlg(),0,TRUE);
        return 0;
     }
   else if (id == 116)
     {  //*base = 0;
        ((SelectHatchPattern_Dialog*)dialog->gethdlg())->trigger_118(QAbstractSlider::SliderToMinimum);
        //((SelectHatchPattern_Dialog*)dialog->gethdlg())->draw();
        //InvalidateRect(dialog->gethdlg(),0,TRUE);
        return 0;
     }
   else if (id == 117)
     {  GetDlgItemText(dialog->gethdlg(),114,directory,300);
        strcat(directory,"/*.*");
        handle = _findfirst(directory,&fd);
        *base = -12;
        while (handle != -1)
          {  if (strlen(fd.name) > 4 && 
                  ((fillstyle == 4 && (_stricmp(fd.name+strlen(fd.name)-4,".bmp") == 0 || _stricmp(fd.name+strlen(fd.name)-4,".jpg") == 0) ||
                   (fillstyle == 2 && _stricmp(fd.name+strlen(fd.name)-4,".cad") == 0))))
               (*base)++; 

            qDebug() << "select : fd.name " << QString(fd.name);
             if (_findnext(handle,&fd) != 0)
               {  _findclose(handle);
                  handle = -1;
               } 

          }

        if (*base < 0)
          *base = 0;
 
        ((SelectHatchPattern_Dialog*)dialog->gethdlg())->trigger_118(QAbstractSlider::SliderToMaximum);
        ((SelectHatchPattern_Dialog*)dialog->gethdlg())->draw();
        //InvalidateRect(dialog->gethdlg(),0,TRUE);
        return 0;
     }

  //_getcwd(dirname, sizeof(dirname));
  strcpy(dirname,home.getpublicfilename(""));
  sfilename[0] = '\0';

  if (fillstyle == 2)
  {
      strcpy(dirname,v.getstring("db::hatch-patterns-path"));
      if(strlen(dirname) < 1)
          strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
      else
      {
          // check if it exists
          // if not use the default
          if(!QDir(QString(dirname)).exists())
              strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
      }
      strcpy(filter,"CAD Pattern files :(*.cad)");
      strcpy(defextension,"CAD");
      strcpy(caption,"Select a pattern file directory");
  }
  else if (fillstyle == 4)
  {
      strcpy(dirname,v.getstring("db::textures-path"));
      if(strlen(dirname) < 1)
          strcpy(dirname,home.getpublicfilename("Textures"));
      else
      {
          // check if it exists
          // if not use the default
          if(!QDir(QString(dirname)).exists())
              strcpy(dirname,home.getpublicfilename("Textures"));
      }
      strcpy(filter,"Image files (*.bmp *.jpg *.jpeg);;Bitmap files (*.bmp);;JPEG files (*.jpg *.jpeg)");
      strcpy(defextension,"BMP");
      strcpy(caption,"Select a bitmap file directory");
  }


  //for (i = 0; filter[i] != '\0'; i++)
  //  if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lpstrDefExt = defextension;
  ofn.lpstrTitle = caption;
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= sfilename;
  ofn.nMaxFile = sizeof(sfilename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;
  ofn.lpfnHook = 0;
  ofn.Flags = QFileDialog::ShowDirsOnly;
  //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

  if (GetOpenFileName(&ofn))
    {
         if (strrchr(ofn.lpstrFile,'/') != 0)
         {
             // we only get directories so don't need this
              //strrchr(ofn.lpstrFile,'/')[0] = 0;
            if (fillstyle == 2)
            {
                for (i = 1 ; i <= 10 ; i++)
                  {  sprintf(key1,"pl::hatchpatterndirectory%d",i);
                     if (_stricmp(v.getstring(key1),ofn.lpstrFile) == 0)
                       break;
                  }

                if (i > 10)
                  {  //  Add a new directory to the list of recent strings
                     for (i = 10 ; i > 1 ; i--)
                       {  sprintf(key1,"pl::hatchpatterndirectory%d",i);
                          sprintf(key2,"pl::hatchpatterndirectory%d",i-1);
                          v.setstring(key1,v.getstring(key2));
                       }
                     v.setstring("pl::hatchpatterndirectory1",ofn.lpstrFile);
                  }
                else
                  {  //  Swap the selected value and the start of the list
                     sprintf(key1,"pl::hatchpatterndirectory%d",i);
                     strcpy(text,v.getstring(key1));
                     v.setstring(key1,v.getstring("pl::hatchpatterndirectory1"));
                     v.setstring("pl::hatchpatterndirectory1",text);
                  }
            }
            else if (fillstyle == 4)
            {
                for (i = 1 ; i <= 10 ; i++)
                  {  sprintf(key1,"pl::bitmapfilldirectory%d",i);
                     if (_stricmp(v.getstring(key1),ofn.lpstrFile) == 0)
                       break;
                  }

                if (i > 10)
                  {  //  Add a new directory to the list of recent strings
                     for (i = 10 ; i > 1 ; i--)
                       {  sprintf(key1,"pl::bitmapfilldirectory%d",i);
                          sprintf(key2,"pl::bitmapfilldirectory%d",i-1);
                          v.setstring(key1,v.getstring(key2));
                       }
                     v.setstring("pl::bitmapfilldirectory1",ofn.lpstrFile);
                  }
                else
                  {  //  Swap the selected value and the start of the list
                     sprintf(key1,"pl::bitmapfilldirectory%d",i);
                     strcpy(text,v.getstring(key1));
                     v.setstring(key1,v.getstring("ar::bitmapfilldirectory1"));
                     v.setstring("pl::bitmapfilldirectory1",text);
                  }
            }

            ((HPListDialogControl *)dialog->getcontrol(114))->load(dialog);
            ((SelectHatchPattern_Dialog*)dialog->gethdlg())->draw();

         }

    }

  return 0;
}



#ifdef USING_WIDGETS
int PSButtonDialogControl::select(Dialog *dialog)
{
 RCOPENFILENAME ofn;
 RCCHAR dirname[300],filename[300],filetitle[300],filter[300];
 int i,base,fillstyle;
 WORD  *p, *dlgtemplate;
 int   nchar;
 DWORD lStyle;

   dialog->currentvalue(114,&fillstyle);
   // use this path if all else  fails
   strcpy(dirname,home.getpublicfilename(""));

  filename[0] = '\0';

   if (id == 110)
   {
       // this is the code for the select hatch pattern dialog
       // now using the new figure browser dialog
#if 1
       QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

       RCCHAR key1[256],*s1;
       int i;
       strcpy(filename,"");
       RCFigureBrowser hpdialog(0);

       if (fillstyle == 2)
       {
           QString lastDir = settings.value("FigureBrowser/PatternFill_Filepath",QString(dirname)).toString();
           if(lastDir.length() < 1)
               strcpy(dirname,v.getstring("db::hatch-patterns-path"));
           else
               strcpy(dirname,lastDir.data());
           if(strlen(dirname) < 1)
               strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
           else
           {
               // check if it exists
               // if not use the default
               if(!QDir(QString(dirname)).exists())
                   strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
           }
           QStringList dirs;
           for (i = 0 ; i < 10 ; i++)
           {
                sprintf(key1,"pl::hatchpatterndirectory%d",i+1);
                if (strlen(s1 = v.getstring(key1)) != 0)
                    dirs.append(QString(v.getstring(key1)));
           }
           hpdialog.SetFilter(_RCT("*.cad"));
           hpdialog.SetRecentPaths(dirs);
           hpdialog.SetRootDir(dirname);
           hpdialog.SetPrefix(_RCT("PatternFill"));
           hpdialog.SetTitle(_RCT("Hatch Patterns"));
           hpdialog.ShowSettingsGroup(RCFigureBrowser::none);
           hpdialog.OnInitDialog();
       }
       else if (fillstyle == 4)
       {
           QString lastDir = settings.value("FigureBrowser/BitmapFill_Filepath",QString(dirname)).toString();
           if(lastDir.length() < 1)
               strcpy(dirname,v.getstring("db::textures-path"));
           else
               strcpy(dirname,lastDir.data());
           if(strlen(dirname) < 1)
               strcpy(dirname,home.getpublicfilename("Textures"));
           else
           {
               // check if it exists
               // if not use the default
               if(!QDir(QString(dirname)).exists())
                   strcpy(dirname,home.getpublicfilename("Textures"));
           }
           QStringList dirs;
           for (i = 0 ; i < 10 ; i++)
           {
                sprintf(key1,"pl::bitmapfilldirectory%d",i+1);
                if (strlen(s1 = v.getstring(key1)) != 0)
                    dirs.append(QString(v.getstring(key1)));
           }
           hpdialog.SetFilter(_RCT("*.bmp;*.jpg;*.jpeg;*.jpg;*.jpeg"));
           hpdialog.SetRecentPaths(dirs);
           hpdialog.SetRootDir(dirname);
           hpdialog.SetPrefix(_RCT("BitmapFill"));
           hpdialog.SetTitle(_RCT("Bitmap Images"));
           hpdialog.ShowSettingsGroup(RCFigureBrowser::none);
           hpdialog.OnInitDialog();
       }


       QString selFilename;
       if(hpdialog.exec())
       {
           selFilename = QString(hpdialog.GetFileName());
           strcpy(filename,QDir::fromNativeSeparators(selFilename).data());
           ((StringDialogControl *)dialog->getcontrol(108))->change(dialog,filename);

           if (fillstyle == 2)
           {
               QStringList dirs;
               dirs = hpdialog.GetRecentPaths();
               for (i = 0 ; i < dirs.count() ; i++)
               {
                   QString path = dirs.at(i);
                   QString key = QString("pl::hatchpatterndirectory%1").arg(i+1);
                   v.setstring(key.toLatin1().data(),path.data());
               }
               settings.setValue("FigureBrowser/PatternFill_Filepath",QString(hpdialog.GetCurrentPath()));
               v.setstring("pl::patternfilename",filename);
           }
           else if (fillstyle == 4)
           {
               QStringList dirs;
               dirs = hpdialog.GetRecentPaths();
               for (i = 0 ; i < dirs.count() ; i++)
               {
                   QString path = dirs.at(i);
                   QString key = QString("pl::bitmapfilldirectory%1").arg(i+1);
                   v.setstring(key.toLatin1().data(),path.data());
               }
               settings.setValue("FigureBrowser/BitmapFill_Filepath",QString(hpdialog.GetCurrentPath()));
               v.setstring("pl::bitmapfilename",filename);
           }
       }
       return 0;
#else
       Dialog hpdialog("SelectHatchPattern_dialog");
       base = 0;
       strcpy(filename,"");

       if (fillstyle == 3)
           hpdialog.title("Hatch Patterns");
       else if (fillstyle == 4)
           hpdialog.title("Bitmap Images");

       for (i = 100 ; i < 112 ; i++)
       {
            hpdialog.add(new HPDisplayDialogControl(i,fillstyle,&base));
            hpdialog.add(new HPButtonDialogControl(i,fillstyle,&base,filename));
       }
       hpdialog.add(new HPButtonDialogControl(112,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(113,fillstyle,&base,filename));
       hpdialog.add(new HPListDialogControl(114,fillstyle));
       hpdialog.add(new HPButtonDialogControl(115,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(116,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(117,fillstyle,&base,filename));
       hpdialog.add(new HPScrollBarDialogControl(118,&base));

       if (hpdialog.process() != FALSE && strlen(filename) > 0)
       {
            ((StringDialogControl *)dialog->getcontrol(108))->change(dialog,filename);
            //if (dialog->getcontrol(207) != 0)
            //  ((CheckBoxDialogControl *)dialog->getcontrol(207))->change(dialog,1);
            ((RadioButtonDialogControl *)dialog->getcontrol(116))->change(dialog,fillstyle);
            ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,1.0);
            ((RealDialogControl *)dialog->getcontrol(105))->change(dialog,1.0);
            ((DisplayDialogControl *)dialog->getcontrol(219))->draw(dialog,0);
       }
       //qDebug() << "filename : " << QString(filename);
       return 0;
#endif
    }

  // look in the default folder
  //_getcwd(dirname, sizeof(dirname));
   strcpy(dirname,home.getpublicfilename(""));

  filename[0] = '\0';

  if (fillstyle == 4)
  {
       strcpy(dirname,v.getstring("db::textures-path"));
       if(strlen(dirname) < 1)
           strcpy(dirname,home.getpublicfilename("Textures"));
       else
       {
           // check if it exists
           // if not use the default
           if(!QDir(QString(dirname)).exists())
               strcpy(dirname,home.getpublicfilename("Textures"));
       }

       _tcscpy(filter,_RCT("Image files (*.bmp *.jpg *.jpeg);;Bitmap files (*.bmp);;JPEG files (*.jpg *.jpeg)"));
       ofn.lpstrDefExt = _RCT("BMP");
       ofn.lpstrTitle = _RCT("Select the bitmap file");
  }
  else
  {
      strcpy(dirname,v.getstring("db::hatch-patterns-path"));
      if(strlen(dirname) < 1)
          strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
      else
      {
          // check if it exists
          // if not use the default
          if(!QDir(QString(dirname)).exists())
              strcpy(dirname,home.getpublicfilename("Hatch-Patterns"));
      }

      //_tcscpy(filter,_RCT("CAD Pattern files *.cad|*.cad"));
      _tcscpy(filter,_RCT("CAD Pattern files (*.cad)"));
       ofn.lpstrDefExt = _RCT("cad");
       ofn.lpstrTitle = _RCT("Select the pattern file");
  }

  memset(&ofn, 0, sizeof(RCOPENFILENAME));

  ofn.lStructSize = sizeof(RCOPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;

  // this is stuff to display the preview at the bottom of the open file dialog
#if 0
  if ( LOBYTE(LOWORD(GetVersion())) > 3)
    {  ofn.lpfnHook = (RCLPOFNHOOKPROC)PreviewHookProc;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
    }
  else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    }

  /* allocate some memory to play with  */
  dlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

  /* start to fill in the dlgtemplate information.  addressing by WORDs */
  lStyle = WS_CHILD | WS_VISIBLE | DS_3DLOOK | DS_CONTROL | WS_CLIPSIBLINGS | DS_SETFONT;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 2;          // NumberOfItems
  *p++ = 0;          // x
  *p++ = 0;          // y
  *p++ = 180;        // cx
  *p++ = 80;         // cy
  *p++ = 0;          // Menu
  *p++ = 0;          // Class

  /* copy the title of the dialog */
  nchar = nCopyAnsiToWideChar (p, TEXT("Preview Template"));
  p += nchar;

 /* Font information because of DS_SETFONT */
  *p++ = 8;     // point size
  nchar = nCopyAnsiToWideChar (p, TEXT("MS Sans Serif"));  // Face name
  p += nchar;

  /* make sure the first item starts on a DWORD boundary */
  p = lpwAlign (p);

  /* now start with the first item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 5;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2000;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("OK"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  /* now the next item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | WS_TABSTOP;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 110;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2001;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("Show preview"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  ofn.hInstance = (RCHINSTANCE) dlgtemplate;
//  LocalFree (LocalHandle (pdlgtemplate));
#endif

  if (GetOpenFileName(&ofn))
  {
       ((StringDialogControl *)dialog->getcontrol(108))->change(dialog,ofn.lpstrFile);
       ((DisplayDialogControl *)dialog->getcontrol(219))->draw(dialog,0);
      if (fillstyle == 4)// bitmap
      {
          if (dialog->getcontrol(118) != 0)
              ((CheckBoxDialogControl *)dialog->getcontrol(118))->change(dialog,1);
          v.setstring("pl::bitmapfilename",filename);
      }
      else// pattern
      {
          if (dialog->getcontrol(116) != 0)
              ((CheckBoxDialogControl *)dialog->getcontrol(116))->change(dialog,1);
          v.setstring("pl::patternfilename",filename);
      }
  }
  return 0;
}
#else
int PSButtonDialogControl::select(Dialog *dialog)
{
 OPENFILENAME ofn;
 RCCHAR dirname[300],filename[300],filetitle[300],filter[300];
 int i,base,fillstyle;
 WORD  *p, *dlgtemplate;
 int   nchar;
 DWORD lStyle;
 Dialog hpdialog("SelectHatchPattern_dialog");

  dialog->currentvalue(100,&fillstyle);

  if (id == 114)
    {Dialog cdialog("Selectcolour_dialog");
     DialogControl *dc;
     double red,green,blue;
       if (dialog->currentvalue(111,&red) &&
           dialog->currentvalue(112,&green) &&
           dialog->currentvalue(113,&blue))
         {  cdialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
            cdialog.add(new ScrollBarDialogControl(1101,dc));
            cdialog.add(new RTColourScrollBarDialogControl(104,dc));
            cdialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
            cdialog.add(new ScrollBarDialogControl(1102,dc));
            cdialog.add(new RTColourScrollBarDialogControl(105,dc));
            cdialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
            cdialog.add(new ScrollBarDialogControl(1103,dc));
            cdialog.add(new RTColourScrollBarDialogControl(106,dc));
            cdialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,dialog));
            if (cdialog.process(dialog->gethdlg()) == TRUE)
              {  ((RealDialogControl *) dialog->getcontrol(111))->change(dialog,red);
                 ((RealDialogControl *) dialog->getcontrol(112))->change(dialog,green);
                 ((RealDialogControl *) dialog->getcontrol(113))->change(dialog,blue);

                 if (dialog->getcontrol(211) != 0)
                   ((CheckBoxDialogControl *)dialog->getcontrol(211))->change(dialog,1);

                 ((DisplayDialogControl *)dialog->getcontrol(119))->draw(dialog,0);
              }
         }
       return 0;
     }
   else if (id == 109)
    {  base = 0;
       strcpy(filename,"");
       for (i = 100 ; i < 112 ; i++)
         {  hpdialog.add(new HPDisplayDialogControl(i,fillstyle,&base));
            hpdialog.add(new HPButtonDialogControl(i,fillstyle,&base,filename));
         }
       hpdialog.add(new HPButtonDialogControl(112,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(113,fillstyle,&base,filename));
       hpdialog.add(new HPListDialogControl(114));
       hpdialog.add(new HPButtonDialogControl(115,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(116,fillstyle,&base,filename));
       hpdialog.add(new HPButtonDialogControl(117,fillstyle,&base,filename));
       hpdialog.add(new HPScrollBarDialogControl(118,&base));

       if (hpdialog.process() != FALSE && strlen(filename) > 0)
         {  ((StringDialogControl *)dialog->getcontrol(107))->change(dialog,filename);
            if (dialog->getcontrol(207) != 0)
              ((CheckBoxDialogControl *)dialog->getcontrol(207))->change(dialog,1);
            ((RadioButtonDialogControl *)dialog->getcontrol(102))->change(dialog,fillstyle);
            ((RealDialogControl *)dialog->getcontrol(115))->change(dialog,1.0);
            ((RealDialogControl *)dialog->getcontrol(116))->change(dialog,1.0);
            ((DisplayDialogControl *)dialog->getcontrol(119))->draw(dialog,0);
         }

       return 0;
    }

  _getcwd(dirname, sizeof(dirname));
  filename[0] = '\0';
 
  if (fillstyle == 4)
    {  _tcscpy(filter,_RCT("Bitmap files *.bmp|*.bmp|JPEG files *.jpg|*.jpg|"));
       ofn.lpstrDefExt = _RCT("BMP");
       ofn.lpstrTitle = _RCT("Select the bitmap file");
    }
  else
    {  _tcscpy(filter,_RCT("CAD Pattern files *.cad|*.cad"));
       ofn.lpstrDefExt = _RCT("CAD");
       ofn.lpstrTitle = _RCT("Select the pattern file");
    }

  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;
  #if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
  if ( LOBYTE(LOWORD(GetVersion())) > 3)
    {  ofn.lpfnHook = (RCLPOFNHOOKPROC)PreviewHookProc;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ENABLETEMPLATEHANDLE | OFN_ENABLEHOOK;
    }
  else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
    }

  /* allocate some memory to play with  */
  dlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);

  /* start to fill in the dlgtemplate information.  addressing by WORDs */
  lStyle = WS_CHILD | WS_VISIBLE | DS_3DLOOK | DS_CONTROL | WS_CLIPSIBLINGS | DS_SETFONT;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 2;          // NumberOfItems
  *p++ = 0;          // x
  *p++ = 0;          // y
  *p++ = 180;        // cx
  *p++ = 80;         // cy
  *p++ = 0;          // Menu
  *p++ = 0;          // Class

  /* copy the title of the dialog */
  nchar = nCopyAnsiToWideChar (p, TEXT("Preview Template"));
  p += nchar;

 /* Font information because of DS_SETFONT */
  *p++ = 8;     // point size
  nchar = nCopyAnsiToWideChar (p, TEXT("MS Sans Serif"));  // Face name
  p += nchar;

  /* make sure the first item starts on a DWORD boundary */
  p = lpwAlign (p);

  /* now start with the first item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 5;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2000;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("OK"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  /* now the next item */
  lStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | WS_TABSTOP;
  *p++ = LOWORD (lStyle);
  *p++ = HIWORD (lStyle);
  *p++ = 0;          // LOWORD (lExtendedStyle)
  *p++ = 0;          // HIWORD (lExtendedStyle)
  *p++ = 110;         // x
  *p++ = 5;          // y
  *p++ = 100;        // cx
  *p++ = 70;        // cy
  *p++ = 2001;       // ID

 /* fill in class i.d., this time by name */
  nchar = nCopyAnsiToWideChar (p, TEXT("BUTTON"));
  p += nchar;

 /* copy the text of the first item */
  nchar = nCopyAnsiToWideChar (p, TEXT("Show preview"));
  p += nchar;

  *p++ = 0;  // advance pointer over nExtraStuff WORD

  ofn.hInstance = (RCHINSTANCE) dlgtemplate;
//  LocalFree (LocalHandle (pdlgtemplate));

#else
    {  ofn.lpfnHook = 0;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    }
#endif

  if (GetOpenFileName(&ofn))
    {  ((StringDialogControl *)dialog->getcontrol(id-1))->change(dialog,ofn.lpstrFile);
       ((DisplayDialogControl *)dialog->getcontrol(119))->draw(dialog,0);
       if (dialog->getcontrol(207) != 0)
         ((CheckBoxDialogControl *)dialog->getcontrol(207))->change(dialog,1);
    }
  return 0;
}
#endif

int TransparentPlaneButtonDialogControl::currentvalue(double *value)
{
    double tr;
    tr = (ptransparency != 0) ? *ptransparency : 0.0;
    *value   = tr;
    return 1;
}

int TransparentPlaneButtonDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("BitmapTransparency_Dialog");
    DialogControl *dc;
    int i,index;
    double tred,tgreen,tblue,trange;
    int lastIndex;
    double lasttRed,lasttGreen,lasttBlue,lasttRange;
    double hue,lightness,saturation;

    index = -1;
    Colour c;
    c.set(tred,tgreen,tblue);

    // ToDo: implement the transparency dialog
    /*
    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Gradient Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = *ptransparency;

    if(dialog.process() == TRUE)
    {
        *ptransparency = red;
    }
    else
    {
        *ptransparency = lastRed;
    }
    db.setnumericdisplaytolerance(n);
    */

    return 0;
}

void BitMapETransparencyRangeDisplayControl::draw(Dialog *dialog, RCHDC hDC)
{
    ResourceString rs0(NCVIEWA);
    //View3dOutput output(hWnd,hDC,&top,1.0);
    double trange,tred,tgreen,tblue;
    int cl,l,s;
    double wt;
    RCCHAR bitmap[600] = {0};
    int backstyle=0,originalShapePreview=0,originalShapePicker=0;

    cl=db.header.getcolour();
    l=(int)db.header.getlayer();
    s=(int)db.header.getstyle();
    wt=db.lineweights.weighttomm(db.header.getweight());

    if(hWnd == 0)
        return;

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
      ctl = (QGraphicsView*)((BitmapTransparency_Dialog*)dialog->gethdlg())->getControl(100);
    QGraphicsScene *scene = 0;
    if(ctl)
        scene = ctl->scene();

    if(ctl && scene)
    {
        View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1E6,1E6);
        View3dOutput output(ctl,ctl,&top,1.0);
        qDebug() << "ctl->rect : " << ctl->rect();
        output.setScene(scene);
        ctl->setSceneRect(ctl->rect());
        //scene->setSceneRect(ctl->rect());
        //scene->setSceneRect(QRectF());
        scene->clear();
        QRectF qrect = ctl->sceneRect();
        qDebug() << "scene->sceneRect : " << scene->sceneRect();
        qDebug() << "qrect : " << qrect;

        // using a checkered background for the result preview
        QColor backColour(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0));
        //scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
        //                                        ::v.getreal("wn::backgroundcolour.green")*255.0,
        //                                        ::v.getreal("wn::backgroundcolour.blue")*255.0)));
        scene->setBackgroundBrush(QBrush(backColour));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0));
        //output.setbackground(RGB(255.0,0.0,0.0));
#if 0
        QColor foreColour(255,255,255);
        QColor backColour = QColor(190,190,190);
        QPixmap backpix = createTranspBackTexture(QSize(100,100),foreColour,backColour);
        double sfactor = ctl->height() / 2.9;
        scene->setBackgroundBrush(QBrush(backpix.scaledToHeight(sfactor)));
#endif
        output.sizeevent(qrect.width(),qrect.height());
        //output.zoom(0.0,0.0,qrect.width(),qrect.height());
        ctl->update();
        //app->processEvents();

        if(dialog->currentvalue(102,&trange) &&
           dialog->currentvalue(1011,&backstyle) &&
           dialog->currentvalue(104,&originalShapePreview) &&
           dialog->currentvalue(105,&originalShapePicker) &&
          ((ColourSampleControl*)dialog->getcontrol(101))->getColour(&tred,&tgreen,&tblue) &&
           strlen(bitmapPath) > 0)
        {
            QColor foreColour(255,255,255);
            QSize bsize;
            double sfactor = ctl->height();
            QPixmap backpix;
            switch(backstyle)
            {
            case 0:
            default: // checker board
                if(backColour == foreColour)
                    backColour = QColor(190,190,190);
                bsize = QSize(100,100);
                backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                sfactor = ctl->height() / 2.9;
                scene->setBackgroundBrush(QBrush(backpix.scaledToHeight(sfactor)));
                break;
            case 1: // single colour
                bsize = QSize(ctl->width(),ctl->height());
                backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                sfactor = ctl->height() / 2.9;
                scene->setBackgroundBrush(backpix);
            break;
            case 2: // half and half
                bsize = QSize(ctl->width(),ctl->height());
                if(backColour == foreColour)
                    backColour = QColor(0,0,0);
                backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                sfactor = ctl->height() / 2.9;
                scene->setBackgroundBrush(backpix);
            break;
            };

            strcpy(bitmap,bitmapPath);
#if 1
            Point p1,origin,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
            double bmwidth = output.getumax() - output.getumin();
            double bmheight = output.getvmax() - output.getvmin();

            if(originalShapePreview)
            {
                if(*width > 0)
                    bmwidth = *width;
                if(*height > 0)
                    bmheight = *height;
                // get the preview viewport size
                origin.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
                xaxis.setxyz(output.getumax() - output.getumin(),0.0,0.0);
                yaxis.setxyz(0.0,output.getvmax() - output.getvmin(),0.0);

                // fit the bitmap rectangle to the preview viewport
                // use that for the bitmap height and width
                if(bmheight >= bmwidth)
                {
                    double w = bmwidth * yaxis.y / bmheight;
                    double h = bmheight * yaxis.y / bmheight;
                    if(w > xaxis.x)
                    {
                        yaxis.y = h * xaxis.x / w;
                    }
                    else
                    {
                        yaxis.y = h;
                        xaxis.x = w;
                    }
                }
                else
                {
                    double w = bmwidth * xaxis.x / bmwidth;
                    double h = bmheight * xaxis.x / bmwidth;
                    if(h > yaxis.y)
                    {
                        xaxis.x = w * yaxis.y / h;
                    }
                    else
                    {
                        yaxis.y = h;
                        xaxis.x = w;
                    }
                }
                xaxis.x *= 0.9;
                yaxis.y *= 0.9;
                // get the base point for the bitmap
                p1 = origin - xaxis / 2.0 - yaxis / 2.0;
            }
            else
            {
                origin.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
                xaxis.x = double(bmwidth) * 0.9;
                yaxis.y = double(bmheight) * 0.9;

                // get the base point for the bitmap
                p1 = origin - xaxis / 2.0 - yaxis / 2.0 ;
            }


            BitMask bo(32);
             if(trange != 0.0)
                 bo.set(9,1);
             BitMapE bitmap(bitmapPath,p1,Point(1,0,0),Point(0,1,0),xaxis.x,yaxis.y,tred,tgreen,tblue,trange,1.0,bo);
             bitmap.draw(DM_NORMAL,NULL,&output);
             //PointE point(Point(0.0,0.0,0.0));
             //point.draw(DM_NORMAL,NULL,&output);
             ctl->update();
#endif
#if 0
            Point p1,p2,origin(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
            EntityList list;
            double factor = 0.8;
            p1.setxyz(0.0,0.0,0.0);  p2.setxyz(w*factor,0.0,0.0);
            Line *line1 = new Line(p1,p2);
            p1.setxyz(w*factor,0.0,0.0);  p2.setxyz(w*factor,h*factor,0.0);
            Line *line2 = new Line(p1,p2);
            p1.setxyz(w*factor,h*factor,0.0);  p2.setxyz(0.0,h*factor,0.0);
            Line *line3 = new Line(p1,p2);
            p1.setxyz(0.0,h*factor,0.0);  p2.setxyz(0.0,0.0,0.0);
            Line *line4 = new Line(p1,p2);
            if (line1 != NULL && line2 != NULL && line3 != NULL && line4 != NULL)
            {
                list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);

                 // apply the header values to the preview
                 BitMask options(32);
                 //options.clearall();
                 //options.set(PlaneFilled,1);
                 options.set(PlaneBitmapFilled,1);
                 //options.set(PlaneNoFill,1);
                 //options.set(PlaneDisplayEdges,1);
                 RCCOLORREF gradcolour(RGB(0,0,0));
                 EntityHeader header(cl,l,0,db.lineweights.mmtoweight(wt));
                 Plane plane(header,origin,xaxis,yaxis,list,xspacing,yspacing,0.0,0.0,0.0,tred,tgreen,tblue,trange,gradcolour,bitmap,options);
                 if(options.test(PlaneNoFill) && !options.test(PlaneDisplayEdges))
                 {
                    line1->draw(DM_NORMAL,NULL,&output);
                    line2->draw(DM_NORMAL,NULL,&output);
                    line3->draw(DM_NORMAL,NULL,&output);
                    line4->draw(DM_NORMAL,NULL,&output);
                 }
                 //QGraphicsItem *qgi = 0;
                 plane.draw(DM_NORMAL,NULL,&output);
                 //qgi = plane.qgi;
                 //if(qgi != 0)
                 //    ctl->fitInView(qgi,Qt::KeepAspectRatio);
                 ctl->update();
                 app->processEvents();
            }
#endif
        }
    }
}

BitMapETransparencyRangeDialogControl::BitMapETransparencyRangeDialogControl(int id, int tid, int cid, int bid, double *range, double *red, double *green, double *blue, int wid, int hid)
    : ButtonDialogControl(id)
{
    tsampleId = tid;
    csampleId = cid;
    bitmapId  = bid;
    ptrans    = range;
    ptred     = red;
    ptgreen   = green;
    ptblue    = blue;
    widthId   = wid;
    heightId  = hid;
}

int BitMapETransparencyRangeDialogControl::currentvalue(double *range, double *red, double *green, double *blue)
{
    double t,r,g,b;
    t = (ptrans != 0) ? *ptrans : 0.0;
    r = (ptred   != 0) ? *ptred   : 0.0;
    g = (ptgreen != 0) ? *ptgreen : 0.0;
    b = (ptblue  != 0) ? *ptblue  : 0.0;
    *range = t;
    *red = r;
    *green = g;
    *blue = b;
    return 1;
}

// this is used by the plane and bitmaps setup functions
// if the bitmappath is empty a stand in bitmap is used by the display control
int BitMapETransparencyRangeDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("BitmapTransparency_Dialog");
    DialogControl *dc;
    int i,originalShapePreview=0,originalShapePicker=0;
    double tred,tgreen,tblue,trange,twidth,theight;
    double lasttRed,lasttGreen,lasttBlue,lasttRange;
    double hue,lightness,saturation;
    RCCHAR bitmapPath[600];

    if((StringDialogControl*)parentdialog->getcontrol(bitmapId) != 0)
    {
        if(QString(((StringDialogControl*)parentdialog->getcontrol(bitmapId))->getSpecialValueText()) == "*Varies*")
            strcpy(bitmapPath,((StringDialogControl*)parentdialog->getcontrol(bitmapId))->getDefaultValue());
        else
            ((StringDialogControl*)parentdialog->getcontrol(bitmapId))->currentvalue(parentdialog,bitmapPath,600);
    }
    // get the latest value from the transparent range control on the parent dialog
    if(QString(((RealDialogControl*)parentdialog->getcontrol(tsampleId))->getSpecialValueText()) == "*Varies*")
        *ptrans = ((RealDialogControl*)parentdialog->getcontrol(tsampleId))->getDefaultValue();
    else
        ((RealDialogControl*)parentdialog->getcontrol(tsampleId))->currentvalue(parentdialog,ptrans);
    trange = *ptrans;

    if(QString(((RealDialogControl*)parentdialog->getcontrol(widthId))->getSpecialValueText()) == "*Varies*")
        *ptrans = ((RealDialogControl*)parentdialog->getcontrol(widthId))->getDefaultValue();
    else
        ((RealDialogControl*)parentdialog->getcontrol(widthId))->currentvalue(parentdialog,&twidth);

    if(QString(((RealDialogControl*)parentdialog->getcontrol(heightId))->getSpecialValueText()) == "*Varies*")
        *ptrans = ((RealDialogControl*)parentdialog->getcontrol(heightId))->getDefaultValue();
    else
        ((RealDialogControl*)parentdialog->getcontrol(heightId))->currentvalue(parentdialog,&theight);

    // the colour values are already up to date but we could get the latest values here if needed
    tred   = *ptred;
    tgreen = *ptgreen;
    tblue  = *ptblue;
    Colour c;
    c.set(*ptred,*ptgreen,*ptblue);
    int backstyle = v.getinteger("bi::backstyle"); // default preview background style

    dialog.title(app->tr("Define Bitmap Transparent Colour").data());
    dialog.add(dc = new RealDialogControl(102,&trange,0.01,0.0,1.0));
    //dialog.add(new TransparencyScrollBarDialogControl(103,dc));
    dialog.add(new ColourSampleControl(101,&dialog,&c));
    dialog.add(new BitMapETransparencyRangeDisplayControl(100, 101, 102, bitmapPath, &twidth, &theight));
    // preview background style
    dialog.add(new RadioButtonDialogControl(1011,1011,1013,&backstyle));
    dialog.add(new RadioButtonDialogControl(1012,1011,1012,&backstyle));
    dialog.add(new RadioButtonDialogControl(1013,1011,1012,&backstyle));
    dialog.add(new CheckBoxDialogControl(104,&originalShapePreview));
    dialog.add(new CheckBoxDialogControl(105,&originalShapePicker));


    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lasttRed   = *ptred;
    lasttGreen = *ptgreen;
    lasttBlue  = *ptblue;
    lasttRange = *ptrans;

    if(dialog.process() == TRUE)
    {
        *ptrans = trange;
        ((RealDialogControl*)parentdialog->getcontrol(tsampleId))->change(parentdialog,*ptrans);
        ((RealDialogControl*)parentdialog->getcontrol(tsampleId))->setSpecialValueText(_RCT(""));

        c.get(ptred,ptgreen,ptblue);
        ((ColourSampleControl*)parentdialog->getcontrol(csampleId))->change(Colour(*ptred,*ptgreen,*ptblue));
        ((ColourSampleControl*)parentdialog->getcontrol(csampleId))->setSpecialValueText(_RCT(""));
        ((ColourSampleControl*)parentdialog->getcontrol(csampleId))->draw(parentdialog,parentdialog->gethdlg());
        setSpecialValueText(_RCT(""));
    }
    else
    {
        *ptrans = lasttRange;
        *ptred   = lasttRed;
        *ptgreen = lasttGreen;
        *ptblue  = lasttBlue;
        ((RealDialogControl*)parentdialog->getcontrol(tsampleId))->change(parentdialog,*ptrans);
        ((ColourSampleControl*)parentdialog->getcontrol(csampleId))->change(Colour(*ptred,*ptgreen,*ptblue));
        ((ColourSampleControl*)parentdialog->getcontrol(csampleId))->draw(parentdialog,parentdialog->gethdlg());
    }
    db.setnumericdisplaytolerance(n);

    return 0;
}

/*
class GradientPlaneButtonDialogControl : public ButtonDialogControl
{
protected:
    int sampleid; // the id of the colour sample graphic
    double *pr, *pg, *pb; // pointers to returned colour values
public:
     GradientPlaneButtonDialogControl(int id, int bid, double *red, double *green, double *blue)
         : ButtonDialogControl(id), sampleid(bid), pr(red), pg(green), pb(blue) {}
     int select(Dialog *);
     int currentvalue(double *red, double *green, double *blue)
     { red = pr; green = pg; blue = pb; return 1; }
};
*/
int GradientPlaneButtonDialogControl::currentvalue(double *red, double *green, double *blue)
{
    double r,g,b;
    r = (pr != 0) ? *pr : 0.0;
    g = (pg != 0) ? *pg : 0.0;
    b = (pb != 0) ? *pb : 0.0;
    *red   = r;
    *green = g;
    *blue  = b;
    return 1;
}

int GradientPlaneButtonDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    index = -1;
    red   = *pr;
    green = *pg;
    blue  = *pb;

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Gradient Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = *pr;
    lastGreen = *pg;
    lastBlue  = *pb;

    if(dialog.process() == TRUE)
    {
        *pr = red;
        *pg = green;
        *pb = blue;
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->change(Colour(*pr,*pg,*pb));
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->setSpecialValueText(_RCT(""));
        setSpecialValueText(_RCT(""));
    }
    else
    {
        *pr = lastRed;
        *pg = lastGreen;
        *pb = lastBlue;
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->change(Colour(*pr,*pg,*pb));
    }

    db.setnumericdisplaytolerance(n);

    return 0;
}

#ifdef USING_WIDGETS
/*
class BitMapEPlaneButtonDialogControl : public ButtonDialogControl
{
protected:
    int sampleid; // the id of the colour sample graphic
    double *tr, *tg, *tb; // pointers to returned colour values
public:
     BitMapEPlaneButtonDialogControl(int id, int bid, double *tred=0, double *tgreen=0, double *tblue=0)
         : ButtonDialogControl(id), sampleid(bid), tr(tred), tg(tgreen), tb(tblue) {}
     int select(Dialog *);
     int currentvalue(double *red, double *green, double *blue)
     { red = tr; green = tg; blue = tb; return 1; }
};
*/

int BitMapEPlaneButtonDialogControl::currentvalue(double *red, double *green, double *blue)
{
    double r,g,b;
    r = (tr != 0) ? *tr : 0.0;
    g = (tg != 0) ? *tg : 0.0;
    b = (tb != 0) ? *tb : 0.0;
    *red   = r;
    *green = g;
    *blue  = b;
    return 1;
}

int BitMapEPlaneButtonDialogControl::select(Dialog *parentdialog)
{
    // puts up the a color selector dialog
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    index = -1;
    red   = *tr;
    green = *tg;
    blue  = *tb;

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Transparent Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = *tr;
    lastGreen = *tg;
    lastBlue  = *tb;

    if(dialog.process() == TRUE)
    {
        *tr = red;
        *tg = green;
        *tb = blue;
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->change(Colour(*tr,*tg,*tb));
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->setSpecialValueText(_RCT(""));
        setSpecialValueText(_RCT(""));
    }
    else
    {
        *tr = lastRed;
        *tg = lastGreen;
        *tb = lastBlue;
        ((ColourSampleControl*)parentdialog->getcontrol(sampleid))->change(Colour(*tr,*tg,*tb));
    }

    db.setnumericdisplaytolerance(n);

    return 0;
}
#else
int BitMapEPlaneButtonDialogControl::select(Dialog *parentdialog)
{Dialog dialog("Selectcolour_Dialog");
 DialogControl *dc;
 double red,green,blue;
  if (parentdialog->currentvalue(124,&red) &&
      parentdialog->currentvalue(125,&green) &&
      parentdialog->currentvalue(126,&blue))
    {  dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new RTColourScrollBarDialogControl(104,dc));
       dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(new RTColourScrollBarDialogControl(105,dc));
       dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(new RTColourScrollBarDialogControl(106,dc));
       dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,parentdialog));
       if (dialog.process(parentdialog->gethdlg()) == TRUE)
         {  ((RealDialogControl *) parentdialog->getcontrol(124))->change(parentdialog,red);
            ((RealDialogControl *) parentdialog->getcontrol(125))->change(parentdialog,green);
            ((RealDialogControl *) parentdialog->getcontrol(126))->change(parentdialog,blue);

            if (parentdialog->getcontrol(224))
                ((CheckBoxDialogControl *) parentdialog->getcontrol(224))->change(parentdialog,1);

         }
    }
  return 0;
}
#endif

class PlaneVerifyButtonControl : public ButtonDialogControl
  {public:
     PlaneVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };

int PlaneVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,gcolour,tcolour,layer,style,iweight;
    double weight,gred,ggreen,gblue,tred,tgree,tblue;

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
    else if(id == 1004)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((RealDialogControl *)dialog->getcontrol(122))->currentvalue(dialog,&gred);
        ((RealDialogControl *)dialog->getcontrol(123))->currentvalue(dialog,&ggreen);
        ((RealDialogControl *)dialog->getcontrol(124))->currentvalue(dialog,&gblue);
        general_property_command(&state,0,(void **)&gcolour);
        gred   = db.colourtable[gcolour].red / 255.0;
        ggreen = db.colourtable[gcolour].green / 255.0;
        gblue  = db.colourtable[gcolour].blue / 255.0;
        ((RealDialogControl *)dialog->getcontrol(122))->change(dialog,gred);
        ((RealDialogControl *)dialog->getcontrol(123))->change(dialog,ggreen);
        ((RealDialogControl *)dialog->getcontrol(124))->change(dialog,gblue);
    }
    else if(id == 1005)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        //((RealDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&tcolour);
        general_property_command(&state,0,(void **)&colour);
        //((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
    }
    return 0;
}

#ifdef USING_WIDGETS
int Plane::setup(BitMask *change,double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gradientcolour,RCCHAR *patternfilename,BitMask *options,EntityHeader *header, BitMask *dgc)
{
 int status,fillstyle,gradientfillstyle,backstyle;
 Dialog dialog("PlaneVerify_Dialog");
 DialogControl *dc;
 ResourceString rs0(NCPLANEA);
 //RCCHAR *GradientFilll[] = {  _RCT("Left to Right"),  _RCT("Right to Left"),  _RCT("Top to Bottom"),  _RCT("Bottom to Top"),  _RCT("Left to Centre to Right"), _RCT("Right to Centre to Left"),  _RCT("Top to Centre to Bottom"),  _RCT("Bottom to Centre to Top"), _RCT("Outside to Inside"), _RCT("Inside To Outside")  };
 RCSTRING STRGradientFilll[] = { "Left to Right","Right to Left","Top to Bottom","Bottom to Top","Left to Centre to Right","Right to Centre to Left","Top to Centre to Bottom","Bottom to Centre to Top","Outside to Inside","Inside To Outside"  };
 RCSTRING STRGradientFilllv[] = { "*Varies*", "Left to Right","Right to Left","Top to Bottom","Bottom to Top","Left to Centre to Right","Right to Centre to Left","Top to Centre to Bottom","Bottom to Centre to Top","Outside to Inside","Inside To Outside"  };
 RCCHAR gfv[25], gf1[25],gf2[25],gf3[25],gf4[25],gf5[25],gf6[25],gf7[25],gf8[25],gf9[25],gf10[25];
 RCCHAR *GradientFilll[] = { gf1,gf2,gf3,gf4,gf5,gf6,gf7,gf8,gf9,gf10 };
 RCCHAR *GradientFilllv[] = { gfv,gf1,gf2,gf3,gf4,gf5,gf6,gf7,gf8,gf9,gf10 };
 RCCHAR sampleHeight[100];
 double red,green,blue;
 double transparencyPcnt = *transparency * 100.0; // transparency as percenatage spinbox
 double previewScale = cadwindow->getcurrentsurface()->getscale();
 int showPreview = v.getinteger("pl::showpreview");

 //
 int c,l,s;
 double wt,defWeight,defWidth,defHeight,deftRange,deftRed,deftGreen,deftBlue,defgred,defggreen,defgblue,defOpacity;
 int defColour,defLayer,defStyle,dummygradstyle=0,deffillstyle;
 double defxspacing,defyspacing,defangle,defcrossangle,deftransparency,deftr,deftg,deftb,deftrange;
 RCCOLORREF defgc;
 RCCHAR defpatternfilename[300],nullString[300];
 BitMask defoptions(32),difgc(3),changeIn(32);
 int tristates[] = { 1 };
 ResourceString rsVaries(4107);
 bool fillstyleVaries=false;
 backstyle = v.getinteger("pl::backstyle"); // default preview background style

 // normal gradation types list 10 items
 for(int i=0; i<10; i++)
     strcpy(GradientFilll[i],&STRGradientFilll[i]);

 // *Varies* gradation types list 11 items
 for(int i=0; i<11; i++)
     strcpy(GradientFilllv[i],&STRGradientFilllv[i]);

  //EntityHeader::verify(&dialog,&c,&l,&s,&w);
  EntityHeader entheader;
  if(header != 0)
      header->verify(&dialog,&c,&l,&s,&wt);
  else
      entheader.verify(&dialog,&c,&l,&s,&wt);

  int changecount = v.getinteger("op::changecount");

  defColour = db.header.getcolour();
  defLayer  = db.header.getlayer();
  defStyle  = db.header.getstyle();
  defWeight = db.lineweights.weighttomm(db.header.getweight());
  Plane::getdefaults(&defxspacing,&defyspacing,&defangle,&defcrossangle,&deftransparency,&deftr,&deftg,&deftb,&deftrange,&defgc,defpatternfilename,&defoptions);
  Colour deftc(deftg,deftg,deftb);

  // get the default values
  memset(nullString,0,sizeof(RCCHAR)*300);
  if(changecount == 1)
      strcpy(defpatternfilename,patternfilename);
  else if(defpatternfilename == 0)
      strcpy(defpatternfilename,nullString);
  else
      strcpy(defpatternfilename,nullString);

  defgred   = GetRValue(defgc) / 255.0;
  defggreen = GetGValue(defgc) / 255.0;
  defgblue  = GetBValue(defgc) / 255.0;
  Colour defgcc(defgred,defggreen,defgblue);

  if(dgc != 0)
      difgc = *dgc;
  else
      difgc.setall();

  fillstyleVaries=false;
  fillstyle = 1;
  if (options->test(PlaneNoFill))
    fillstyle = 0;
  else if (options->test(PlanePatterned))
    fillstyle = 2;
  else if (options->test(PlaneFilled))
    fillstyle = 3;
  else if (options->test(PlaneBitmapFilled))
    fillstyle = 4;
  else if (options->test(PlaneGradientFilled))
    fillstyle = 5;

  gradientfillstyle = options->test(PlaneGradientStyle1) * 8 + options->test(PlaneGradientStyle2) * 4 +  options->test(PlaneGradientStyle3) * 2 +  options->test(PlaneGradientStyle4);

  red   = GetRValue(*gradientcolour) / 255.0;
  green = GetGValue(*gradientcolour) / 255.0;
  blue  = GetBValue(*gradientcolour) / 255.0;


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
          c = 0;
      if(!change->test(1))
          l = 0;
      if(!change->test(2))
          s = 0;
      if(!change->test(3))
          wt = 0.0;

      if(!change->test(4)) // fill style
      {
          fillstyle = PlaneUnknownState; // these are radio buttons if they vary none are checked
          fillstyleVaries = true;
      }
      if(change->test(5)) // display edges
          options->test(PlaneDisplayEdges) == 0 ? tristates[0] = 0 : tristates[0] = 2;
      if(!change->test(6)) // pattern file name
      {
          ;//strcpy(patternfilename,"");
      }
      if(!change->test(7)) // gradient style
      {
          gradientfillstyle = defoptions.test(PlaneGradientStyle1) * 8 + defoptions.test(PlaneGradientStyle2) * 4 +  defoptions.test(PlaneGradientStyle3) * 2 +  defoptions.test(PlaneGradientStyle4);
          dummygradstyle=0;
      }
      else
      {
          dummygradstyle=0;
      }
      if(!change->test(8)) // gradient colour
      {
          red   = 0.0;
          green = 0.0;
          blue  = 0.0;
      }
      if(!change->test(9)) // x spacing
          *xspacing = 0.0;
      if(!change->test(10)) // y spacing
          *yspacing = 0.0;
      if(!change->test(11)) // angle
          *angle = -360.0;
      if(!change->test(12)) // cross angle
          *crossangle = -360.0;
      if(!change->test(13)) // transparency
      {
          *transparency = 0.0;
          transparencyPcnt = 0.0;
      }
      if(!change->test(14)) // transparency colour
      {
          *tr = 0.0;
          *tg = 0.0;
          *tb = 0.0;
      }
      if(!change->test(15)) // transparency range
          *trange = 0.0;
  }
  else
  {
      fillstyle = 1;
      if (options->test(PlaneNoFill))
        fillstyle = 0;
      else if (options->test(PlanePatterned))
        fillstyle = 2;
      else if (options->test(PlaneFilled))
        fillstyle = 3;
      else if (options->test(PlaneBitmapFilled))
        fillstyle = 4;
      else if (options->test(PlaneGradientFilled))
        fillstyle = 5;

      gradientfillstyle = options->test(PlaneGradientStyle1) * 8 + options->test(PlaneGradientStyle2) * 4 +  options->test(PlaneGradientStyle3) * 2 +  options->test(PlaneGradientStyle4);

      red   = GetRValue(*gradientcolour) / 255.0;
      green = GetGValue(*gradientcolour) / 255.0;
      blue  = GetBValue(*gradientcolour) / 255.0;
  }

  dialog.add(new RadioButtonDialogControl(114,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(115,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(116,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(117,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(118,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(119,114,119,&fillstyle));
  dialog.add(new StringDialogControl(108,patternfilename,300));
  dialog.add(new PSButtonDialogControl(1022));
  dialog.add(new PSButtonDialogControl(110));
  dialog.add(new ListDialogControl(121,10,GradientFilll,&gradientfillstyle));

  //dialog.add(new RealDialogControl(122,&red));
  //dialog.add(new RealDialogControl(123,&green));
  //dialog.add(new RealDialogControl(124,&blue));
  dialog.add(new GradientPlaneButtonDialogControl(1004,204,&red,&green,&blue));
  Colour gc(red,green,blue);
  dialog.add(new ColourSampleControl(204,&dialog,&gc));

  dialog.add(new RealDialogControl(104,xspacing,1.0,0.00001,1000000.0,ScaleByLength));
  dialog.add(new RealDialogControl(105,yspacing,1.0,0.00001,1000000.0,ScaleByLength));
  dialog.add(new RealDialogControl(106,angle));
  dialog.add(new RealDialogControl(107,crossangle));
  dialog.add(new RealDialogControl(125,transparency,0.1,0.0,1.0,DontScale));
  dialog.add(dc = new RealDialogControl(134,&transparencyPcnt,1.0,0.0,100.0,DontScale));
  //dialog.add(new TransparencyScrollBarDialogControl(303,dc,0,100));
  //dialog.add(new TransparentPlaneButtonDialogControl(1006,&transparencyPcnt));

  //dialog.add(new RealDialogControl(127,tr,0.01,0.0,1.0,DontScale));
  //dialog.add(new RealDialogControl(128,tg,0.01,0.0,1.0,DontScale));
  //dialog.add(new RealDialogControl(129,tb,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(126,trange,0.01,0.0,1.0,DontScale));
  dialog.add(new BitMapEPlaneButtonDialogControl(1007,205,tr,tg,tb));
  Colour tc(*tr,*tg,*tb);
  dialog.add(new ColourSampleControl(205,&dialog,&tc));
  dialog.add(new BitMapETransparencyRangeDialogControl(1005,126,205,108,trange,tr,tg,tb,104,105));

  dialog.add(new StringDialogControl(133,sampleHeight,100));
  dialog.add(new RealDialogControl(130,&previewScale,0.100,0.001,1000000.0,DontScale));
  dialog.add((DisplayDialogControl *) new PlaneDisplayDialogControl(219));
  dialog.add(new PRButtonDialogControl(304));
  dialog.add(new CheckBoxDialogControl(305,&showPreview));

  // preview background style
  dialog.add(new RadioButtonDialogControl(1009,1009,1011,&backstyle));
  dialog.add(new RadioButtonDialogControl(1010,1009,1011,&backstyle));
  dialog.add(new RadioButtonDialogControl(1011,1009,1011,&backstyle));

  dialog.add(new ButtonDialogControl(111)); // change surface properties
  dialog.add(new ButtonDialogControl(112)); // align
  dialog.add(new ButtonDialogControl(113)); // set anchor
  dialog.add(new ButtonDialogControl(131)); // clone (used in setup options)
  dialog.add(new ButtonDialogControl(132)); // clone all (used in setup options)

  if (change != NULL)
  {
       if(changecount == 1)
          dialog.title(_RCT("Plane Verify"));
       else
          dialog.title(rs0.gets());

       v.setinteger("op::change",1);
       v.setbm("op::changemask",*change);
       //change->clearall();

       dialog.add(new PlaneVerifyButtonControl(1000));
       dialog.add(new PlaneVerifyButtonControl(1001));
       dialog.add(new PlaneVerifyButtonControl(1002));
       dialog.add(new PlaneVerifyButtonControl(1003));

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

       // if single plane should not be tristate
       dialog.add(new TriStateCheckBoxDialogControl(120,&tristates[0],((changecount>=1)^change->test(5)),defoptions.test(PlaneDisplayEdges)));

       // pattern/bitmap file name
       if(changecount > 1 && !change->test(6))
       {
           if(patternfilename != 0)
           {
               strcpy(patternfilename,_RCT("*Varies*"));
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
           else
           {
               //dialog.add(new StringDialogControl(108,nullString,300));
               strcpy(patternfilename,nullString);
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
       }
       else if(changecount > 1 && change->test(6))
       {
           if(patternfilename != 0)
           {
               //dialog.add(new StringDialogControl(108,patternfilename,300));
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
           else
           {
               //dialog.add(new StringDialogControl(108,nullString,300));
               strcpy(patternfilename,nullString);
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
       }
       else if(changecount == 1 && change->test(6))
       {
           if(patternfilename != 0)
           {
               //dialog.add(new StringDialogControl(108,patternfilename,300));
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
           else
           {
               //dialog.add(new StringDialogControl(108,nullString,300));
               strcpy(patternfilename,nullString);
               ((StringDialogControl*)dialog.getcontrol(108))->load(&dialog);
           }
       }
       if(changecount > 1 && !change->test(7))
           dialog.add(new ListDialogControl(1021,11,GradientFilllv,&dummygradstyle));

       if(changecount > 1 && !change->test(8))
       {
           // gradient red          
           if(!difgc.test(0) || !difgc.test(1) || !difgc.test(2))
           {
               ((GradientPlaneButtonDialogControl*)dialog.getcontrol(1004))->setSpecialValueText(_RCT("*Varies*"));
               ((GradientPlaneButtonDialogControl*)dialog.getcontrol(1004))->setDefaultValue(defgred,defgred,deftBlue);

               ((ColourSampleControl*)dialog.getcontrol(204))->setSpecialValueText(_RCT("*Varies*"));
               ((ColourSampleControl*)dialog.getcontrol(204))->setDefaultValue(defgcc);

               //((RealDialogControl*)dialog.getcontrol(122))->setSpecialValueText(_RCT("*Varies*"));
               //((RealDialogControl*)dialog.getcontrol(122))->setDefaultValue(defgred);
           }
           // gadient green
           //if(!difgc.test(1))
           //{
               //((RealDialogControl*)dialog.getcontrol(123))->setSpecialValueText(_RCT("*Varies*"));
               //((RealDialogControl*)dialog.getcontrol(123))->setDefaultValue(defggreen);
           //}
           // gradient blue
           //if(!difgc.test(2))
           //{
               //((RealDialogControl*)dialog.getcontrol(124))->setSpecialValueText(_RCT("*Varies*"));
               //((RealDialogControl*)dialog.getcontrol(124))->setDefaultValue(defgblue);
           //}
       }
       // x spacing
       if(changecount > 1 && !change->test(9))
       {
           ((RealDialogControl*)dialog.getcontrol(104))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(104))->setDefaultValue(defxspacing);
       }
       // y spacing
       if(changecount > 1 && !change->test(10))
       {
           ((RealDialogControl*)dialog.getcontrol(105))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(105))->setDefaultValue(defyspacing);
       }
       // angle
       if(changecount > 1 && !change->test(11))
       {
           ((RealDialogControl*)dialog.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(106))->setDefaultValue(defangle);
       }
       // cross angle
       if(changecount > 1 && !change->test(12))
       {
           ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defcrossangle);
       }
       // transparency
       if(changecount > 1 && !change->test(13))
       {
           ((RealDialogControl*)dialog.getcontrol(125))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(125))->setDefaultValue(deftransparency);
           //
           ((RealDialogControl*)dialog.getcontrol(134))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(134))->setDefaultValue((deftransparency * 100.0));
       }
       // bitmap transparency colour
       if(changecount > 1 && !change->test(14))
       {
           ((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->setSpecialValueText(_RCT("*Varies*"));
           ((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->setDefaultValue(deftr,deftg,deftb);

           ((ColourSampleControl*)dialog.getcontrol(205))->setSpecialValueText(_RCT("*Varies*"));
           ((ColourSampleControl*)dialog.getcontrol(205))->setDefaultValue(deftc);

           //((RealDialogControl*)dialog.getcontrol(127))->setSpecialValueText(_RCT("*Varies*"));
           //((RealDialogControl*)dialog.getcontrol(127))->setDefaultValue(deftr);

           //((RealDialogControl*)dialog.getcontrol(128))->setSpecialValueText(_RCT("*Varies*"));
           //((RealDialogControl*)dialog.getcontrol(128))->setDefaultValue(deftg);

           //((RealDialogControl*)dialog.getcontrol(129))->setSpecialValueText(_RCT("*Varies*"));
           //((RealDialogControl*)dialog.getcontrol(129))->setDefaultValue(deftb);
       }
       // bitmap transparency range
       if(changecount > 1 && !change->test(15))
       {
           ((RealDialogControl*)dialog.getcontrol(126))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(126))->setDefaultValue(deftrange);
       }
       //dialog.add(new CheckBoxDialogControl(200,change,4));
       //dialog.add(new CheckBoxDialogControl(206,change,5));
       //dialog.add(new CheckBoxDialogControl(207,change,6));
       //dialog.add(new CheckBoxDialogControl(210,change,7));
       //dialog.add(new CheckBoxDialogControl(211,change,8));
       //dialog.add(new CheckBoxDialogControl(215,change,9));
       //dialog.add(new CheckBoxDialogControl(216,change,10));
       //dialog.add(new CheckBoxDialogControl(217,change,11));
       //dialog.add(new CheckBoxDialogControl(218,change,12));
       //dialog.add(new CheckBoxDialogControl(221,change,13));


       //dialog.add(new CheckBoxDialogControl(224,change,14));
       //dialog.add(new CheckBoxDialogControl(228,change,15));
  }
  else
  {
      v.setinteger("op::change",0);
      v.setbm("op::changemask",defoptions);

      c = defColour;
      l = defLayer;
      s = defStyle;
      wt = defWeight;

      dialog.adddisable(111);
      dialog.adddisable(112);

      // pattern file path
      if(patternfilename != 0)
        dialog.add(new StringDialogControl(108,patternfilename,300));
      else
        dialog.add(new StringDialogControl(108,nullString,300));

      tristates[0]= options->test(PlaneDisplayEdges);
      dialog.add(new TriStateCheckBoxDialogControl(120,&tristates[0],false,options->test(PlaneDisplayEdges)));
  }

  if (options->test(1))
  {
       *xspacing *= db.getmainunitscale();
       *yspacing *= db.getmainunitscale();
  }

  status = dialog.process();
  if (status == TRUE)
  {
      if(change != NULL)
      {
          // reset the outgoing change values
          changeIn = *change;
          change->clearall();

          if(changecount == 1)
          {
              for (int i = 0 ; i < 4 ; i++)
                change->set(i,1);

              header->setcolour(c);
              header->setstyle(s);
              header->setlayer(l);
              header->setweight(db.lineweights.mmtoweight(wt));
          }

          if( changecount > 1 && !changeIn.test(0) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(100))->getSpecialValueText()) != 0 )
              change->clear(0);
          else
          {
              change->set(0);
              header->setcolour(c);
          }
          if( changecount > 1 && !changeIn.test(1) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(101))->getSpecialValueText()) != 0 )
              change->clear(1);
          else
          {
              change->set(1);
              header->setlayer(l);
          }
          if( changecount > 1 && !changeIn.test(2) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(102))->getSpecialValueText()) != 0 )
              change->clear(2);
          else
          {
              change->set(2);
              header->setstyle(s);
          }
          if( changecount > 1 && !changeIn.test(3) &&
              strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
              change->clear(3);
          else
          {
              change->set(3);
              header->setweight(db.lineweights.mmtoweight(wt));
          }
          // fill type
          if(changecount > 1 && !changeIn.test(4) && v.getinteger("op::lastplanefillstyle") == PlaneUnknownState)
              change->clear(4); // these are radio buttons if they vary none are checked
          else
          {
              change->set(4);
              options->set(PlaneNoFill,fillstyle == 0);
              options->set(PlanePatterned,fillstyle == 2);
              options->set(PlaneFilled,fillstyle == 3);
              options->set(PlaneBitmapFilled,fillstyle == 4);
              options->set(PlaneGradientFilled,fillstyle == 5);
          }
          // display edges
          if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(120))->isTriState())
              change->clear(5);
          else
          {
              change->set(5);
              options->set(PlaneDisplayEdges, tristates[0]);
          }
          // pattern/bitmap file name
          if(changecount > 1 && !changeIn.test(6) && QString(patternfilename) == QString("*Varies*"))
              change->clear(6);
          else if((options->test(PlanePatterned) || options->test(PlaneBitmapFilled)) && !QString(patternfilename).isEmpty())
          {
              change->set(6);
          }
          // gradation fill style
          if(changecount > 1 && !changeIn.test(7) && dummygradstyle == 0 )
              change->clear(7);
          else
          {
              change->set(7);
              options->set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
              options->set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
              options->set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
              options->set(PlaneGradientStyle4,(gradientfillstyle&1)==1);
          }
          // gradient colours
          if(changecount > 1 && !changeIn.test(8) &&
             strlen(((GradientPlaneButtonDialogControl*)dialog.getcontrol(1004))->getSpecialValueText()) != 0)
             //strlen(((RealDialogControl*)dialog.getcontrol(122))->getSpecialValueText()) != 0 ||
             //strlen(((RealDialogControl*)dialog.getcontrol(123))->getSpecialValueText()) != 0 ||
             //strlen(((RealDialogControl*)dialog.getcontrol(124))->getSpecialValueText()) != 0 )
              change->clear(8);
          else
          {
             change->set(8);
             if(strlen(((GradientPlaneButtonDialogControl*)dialog.getcontrol(1004))->getSpecialValueText()) != 0 )
             {
                 red = defgred;
                 green = defggreen;
                 blue = defgblue;
             }
             /*
             if(strlen(((RealDialogControl*)dialog.getcontrol(122))->getSpecialValueText()) != 0 )
                 red = defgred;
             else if(strlen(((RealDialogControl*)dialog.getcontrol(123))->getSpecialValueText()) != 0 )
                 green = defggreen;
             else if(strlen(((RealDialogControl*)dialog.getcontrol(124))->getSpecialValueText()) != 0 )
                 blue = defgblue;
             */
             *gradientcolour = RGB(int(red*255.0),int(green*255.0),int(blue*255.0));
          }
          // x spacing
          if(changecount > 1 && !changeIn.test(9) &&
             strlen(((RealDialogControl*)dialog.getcontrol(104))->getSpecialValueText()) != 0)
              change->clear(9);
          else
              change->set(9);
          // y spacing
          if(changecount > 1 && !changeIn.test(10) &&
             strlen(((RealDialogControl*)dialog.getcontrol(105))->getSpecialValueText()) != 0)
              change->clear(10);
          else
              change->set(10);
          // angle
          if(changecount > 1 && !changeIn.test(11) &&
             strlen(((RealDialogControl*)dialog.getcontrol(106))->getSpecialValueText()) != 0)
              change->clear(11);
          else
              change->set(11);
          // cross angle
          if(changecount > 1 && !changeIn.test(12) &&
             strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0)
              change->clear(12);
          else
              change->set(12);
          // transparency
          if(changecount > 1 && !changeIn.test(13) &&
             strlen(((RealDialogControl*)dialog.getcontrol(125))->getSpecialValueText()) != 0)
              change->clear(13);
          else
              change->set(13);
          // bitmap transparency colour
          if(changecount > 1 && !changeIn.test(14) &&
              strlen(((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->getSpecialValueText()) != 0)
             //strlen(((RealDialogControl*)dialog.getcontrol(127))->getSpecialValueText()) != 0 ||
             //strlen(((RealDialogControl*)dialog.getcontrol(128))->getSpecialValueText()) != 0 ||
             //strlen(((RealDialogControl*)dialog.getcontrol(129))->getSpecialValueText()) != 0 )
              change->clear(14);
          else
          {
              change->set(14);
              if(strlen(((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->getSpecialValueText()) != 0)
              {
                  *tr = deftr;
                  *tg = deftg;
                  *tb = deftb;
              }
              /*
              if(strlen(((RealDialogControl*)dialog.getcontrol(127))->getSpecialValueText()) != 0 )
                  *tr = deftr;
              else if(strlen(((RealDialogControl*)dialog.getcontrol(128))->getSpecialValueText()) != 0 )
                  *tg = deftg;
              else if(strlen(((RealDialogControl*)dialog.getcontrol(129))->getSpecialValueText()) != 0 )
                  *tb = deftb;
              */
          }
          // transparency range
          if(changecount > 1 && !changeIn.test(15) &&
             strlen(((RealDialogControl*)dialog.getcontrol(126))->getSpecialValueText()) != 0)
              change->clear(15);
          else
              change->set(15);

          v.setinteger("pl::showpreview",showPreview);
      }
      else
      {
           options->set(PlaneDisplayEdges, tristates[0]);

           options->set(PlaneNoFill,fillstyle == 0);
           options->set(PlanePatterned,fillstyle == 2);
           options->set(PlaneFilled,fillstyle == 3);
           options->set(PlaneBitmapFilled,fillstyle == 4);
           options->set(PlaneGradientFilled,fillstyle == 5);

           options->set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
           options->set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
           options->set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
           options->set(PlaneGradientStyle4,(gradientfillstyle&1)==1);

           *gradientcolour = RGB(int(red*255.0),int(green*255.0),int(blue*255.0));
      }

      v.setinteger("pl::backstyle",backstyle);
  }

  if (*xspacing != 0.0 && *xspacing < db.getptoler()) *xspacing = 1.0;
  if (*yspacing != 0.0 && *yspacing < db.getptoler()) *yspacing = 1.0;
  if (options->test(1))
    {  *xspacing /= db.getmainunitscale();
       *yspacing /= db.getmainunitscale();
    }

  return status;
}
#else
int Plane::setup(BitMask *change,double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gradientcolour,RCCHAR *patternfilename,BitMask *options)
{int status,fillstyle,gradientfillstyle;
 Dialog dialog("Plane_Dialog");
 DialogControl *dc;
 ResourceString rs0(NCPLANEA);
 RCCHAR *GradientFilll[] = {  _RCT("Left to Right"),  _RCT("Right to Left"),  _RCT("Top to Bottom"),  _RCT("Bottom to Top"),  _RCT("Left to Centre to Right"), _RCT("Right to Centre to Left"),  _RCT("Top to Centre to Bottom"),  _RCT("Bottom to Centre to Top"), _RCT("Outside to Inside"), _RCT("Inside To Outside")  };
 double red,green,blue;
  
  fillstyle = 1;
  if (options->test(PlaneNoFill))
    fillstyle = 0;
  else if (options->test(PlanePatterned))
    fillstyle = 2;
  else if (options->test(PlaneFilled))
    fillstyle = 3;
  else if (options->test(PlaneBitmapFilled))
    fillstyle = 4;
  else if (options->test(PlaneGradientFilled))
    fillstyle = 5;

  gradientfillstyle = options->test(PlaneGradientStyle1) * 8 + options->test(PlaneGradientStyle2) * 4 +  options->test(PlaneGradientStyle3) * 2 +  options->test(PlaneGradientStyle4); 

  red = GetRValue(*gradientcolour) / 255.0;
  green = GetGValue(*gradientcolour) / 255.0;
  blue = GetBValue(*gradientcolour) / 255.0;

  dialog.add(new RadioButtonDialogControl(100,100,105,&fillstyle));
  dialog.add(new RadioButtonDialogControl(101,100,105,&fillstyle));
  dialog.add(new RadioButtonDialogControl(102,100,105,&fillstyle));
  dialog.add(new RadioButtonDialogControl(103,100,105,&fillstyle));
  dialog.add(new RadioButtonDialogControl(104,100,105,&fillstyle));
  dialog.add(new RadioButtonDialogControl(105,100,105,&fillstyle));
  dialog.add(new CheckBoxDialogControl(106,options,PlaneDisplayEdges));
  dialog.add(new StringDialogControl(107,patternfilename,300));
  dialog.add(new PSButtonDialogControl(108));
  dialog.add(new PSButtonDialogControl(109));
  dialog.add(new ListDialogControl(110,10,GradientFilll,&gradientfillstyle));

  dialog.add(dc = new RealDialogControl(111,&red));
  dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(dc = new RealDialogControl(112,&green));
  dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(dc = new RealDialogControl(113,&blue));
  dialog.add(new ScrollBarDialogControl(1113,dc));
  dialog.add(new PSButtonDialogControl(114));

  dialog.add(dc = new RealDialogControl(115,xspacing,1.0,0.001,1000000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1115,dc));
  dialog.add(dc = new RealDialogControl(116,yspacing,1.0,0.001,1000000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1116,dc));
  dialog.add(dc = new RealDialogControl(117,angle));
  dialog.add(new ScrollBarDialogControl(1117,dc));
  dialog.add(dc = new RealDialogControl(118,crossangle));
  dialog.add(new ScrollBarDialogControl(1118,dc));
  dialog.add(dc = new RealDialogControl(121,transparency,0.1,0.0,1.0,DontScale));
  dialog.add(new ScrollBarDialogControl(1121,dc));

  dialog.add(dc = new RealDialogControl(124,tr,0.01,0.0,1.0,DontScale));
  dialog.add(new ScrollBarDialogControl(1124,dc));
  dialog.add(dc = new RealDialogControl(125,tg,0.01,0.0,1.0,DontScale));
  dialog.add(new ScrollBarDialogControl(1125,dc));
  dialog.add(dc = new RealDialogControl(126,tb,0.01,0.0,1.0,DontScale));
  dialog.add(new ScrollBarDialogControl(1126,dc));
  dialog.add(new BitMapEPlaneButtonDialogControl(127));
  dialog.add(dc = new RealDialogControl(128,trange,0.01,0.0,1.0,DontScale));
  dialog.add(new ScrollBarDialogControl(1128,dc));

  dialog.add((DisplayDialogControl *) new PlaneDisplayDialogControl(219));
  dialog.add(new ButtonDialogControl(120)); // clone
  dialog.add(new ButtonDialogControl(122)); // clone all
  dialog.add(new ButtonDialogControl(123)); // align

#if MENUS == FLEXICADMENUS
  dialog.adddisable(105);
  dialog.adddisable(106);
  dialog.adddisable(107);
  options->clear(1);
#endif

  if (change != NULL)
    {  dialog.title(rs0.gets());
       change->clearall();
       dialog.add(new CheckBoxDialogControl(200,change,4));
       dialog.add(new CheckBoxDialogControl(206,change,5));
       dialog.add(new CheckBoxDialogControl(207,change,6));
       dialog.add(new CheckBoxDialogControl(210,change,7));
       dialog.add(new CheckBoxDialogControl(211,change,8));
       dialog.add(new CheckBoxDialogControl(215,change,9));
       dialog.add(new CheckBoxDialogControl(216,change,10));
       dialog.add(new CheckBoxDialogControl(217,change,11));
       dialog.add(new CheckBoxDialogControl(218,change,12));
       dialog.add(new CheckBoxDialogControl(221,change,13));


       dialog.add(new CheckBoxDialogControl(224,change,14));
       dialog.add(new CheckBoxDialogControl(228,change,15));

    }
  else
    dialog.adddisable(123);

  if (options->test(1))
    {  *xspacing *= db.getmainunitscale();
       *yspacing *= db.getmainunitscale();
    }
  status = dialog.process();
  if (status == TRUE)
    {  options->set(PlaneNoFill,fillstyle == 0);
       options->set(PlanePatterned,fillstyle == 2);
       options->set(PlaneFilled,fillstyle == 3);
       options->set(PlaneBitmapFilled,fillstyle == 4);
       options->set(PlaneGradientFilled,fillstyle == 5);

       options->set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
       options->set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
       options->set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
       options->set(PlaneGradientStyle4,(gradientfillstyle&1)==1);

       *gradientcolour = RGB(int(red*255.0),int(green*255.0),int(blue*255.0));

    }

  if (*xspacing != 0.0 && *xspacing < db.getptoler()) *xspacing = 1.0;
  if (*yspacing != 0.0 && *yspacing < db.getptoler()) *yspacing = 1.0;
  if (options->test(1))
    {  *xspacing /= db.getmainunitscale();
       *yspacing /= db.getmainunitscale();
    }
  return status;
}
#endif

void Plane::getdefaults(double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,RCCHAR *patternfilename,BitMask *options)
{ *xspacing = v.getreal("pl::xspacing");
  *yspacing = v.getreal("pl::yspacing");
  *angle = v.getreal("pl::angle");
  *crossangle = v.getreal("pl::crossangle");
  *options = *v.getbm("pl::options");
  if(options->test(PlanePatterned))
     strcpy(patternfilename,v.getstring("pl::patternfilename"));
  else if(options->test(PlaneBitmapFilled))
     strcpy(patternfilename,v.getstring("pl::bitmapfilename"));
  *gc = v.getlong("pl::gradientcolour");
  *transparency = v.getreal("pl::transparency");
  *tr= v.getreal("pl::red");
  *tg = v.getreal("pl::green");
  *tb = v.getreal("pl::blue");
  *trange = v.getreal("pl::transparencyrange");
}

void Plane::setdefaults(double xspacing,double yspacing,double angle,double crossangle,double transparency,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *patternfilename,const BitMask &options)
{
  v.setreal("pl::xspacing",xspacing);
  v.setreal("pl::yspacing",yspacing);
  v.setreal("pl::angle",angle);
  v.setreal("pl::crossangle",crossangle);
  if (patternfilename != 0)
  {
    if(options.test(PlanePatterned))
        v.setstring("pl::patternfilename",patternfilename);
    else if(options.test(PlaneBitmapFilled))
        v.setstring("pl::bitmapfilename",patternfilename);
  }
  else
  {
    if(options.test(PlanePatterned))
       v.setstring("pl::patternfilename","");
    else if(options.test(PlaneBitmapFilled))
       v.setstring("pl::bitmapfilename","");
  }
  v.setbm("pl::options",options);
  v.setlong("pl::gradientcolour",gc);
  v.setreal("pl::transparency",transparency);
  v.setreal("pl::red",tr);
  v.setreal("pl::green",tg);
  v.setreal("pl::blue",tb);
  v.setreal("pl::transparencyrange",trange);

}

void Plane::setdefaults(void)
{
  v.setreal("pl::xspacing",xspacing);
  v.setreal("pl::yspacing",yspacing);
  v.setreal("pl::xspacing",xspacing);
  v.setreal("pl::yspacing",yspacing);
  v.setreal("pl::angle",angle);
  v.setreal("pl::crossangle",crossangle);
  if (patternfilename != 0)
  {
    if(options.test(PlanePatterned))
        v.setstring("pl::patternfilename",patternfilename);
    else if(options.test(PlaneBitmapFilled))
        v.setstring("pl::bitmapfilename",patternfilename);
  }
  else
  {
    if(options.test(PlanePatterned))
       v.setstring("pl::patternfilename","");
    else if(options.test(PlaneBitmapFilled))
       v.setstring("pl::bitmapfilename","");
  }
  v.setbm("pl::options",options);
  v.setlong("pl::gradientcolour",gradientcolour);
  v.setreal("pl::transparency",transparency);
  v.setreal("pl::red",red);
  v.setreal("pl::green",green);
  v.setreal("pl::blue",blue);
  v.setreal("pl::transparencyrange",transparencyrange);

}

int Plane::save(CadFile *outfile)
{short type;
  RCCHAR exedir[600],lib1[600],libcmp[600];
  type = plane_entity;

  if (transparency != 0.0)
    options.set(PlaneHasTransparency);
  if (red != 0.0 || green != 0.0 || blue != 0.0 || transparencyrange != 0.0)
    options.set(PlaneHasTransparentColour);

  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! list.savegeometry(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&xspacing)) return 0;
  if (! outfile->write(&yspacing)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&crossangle)) return 0;
  if (! origin.save(outfile)) return NULL;
  if (! xaxis.save(outfile)) return NULL;
  if (! yaxis.save(outfile)) return NULL;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
  {
      // v8
    strcpy(exedir,home.getpublicfilename(""));  _strupr(exedir);
    strcpy(lib1,patternfilename);
    strcpy(libcmp,QDir::toNativeSeparators(QString(patternfilename)));  _strupr(libcmp);
    if (strncmp(exedir,libcmp,strlen(exedir)) == 0)
    {
      strcpy(lib1,"::CADHD::");  strcat(lib1,patternfilename+strlen(exedir));
    }
    if (! outfile->write(lib1)) return NULL;
      // v8
    //if (! outfile->write(patternfilename)) return NULL;
  }
  if (options.test(PlaneAnchored))
    if (! anchor.save(outfile)) return NULL;
  if (options.test(PlaneGradientFilled))
    if (! outfile->write((long *)&gradientcolour)) return NULL;
  if (options.test(PlaneHasTransparency))
    if (! outfile->write(&transparency)) return NULL;
  if (options.test(PlaneHasTransparentColour))
    {  if (! outfile->write(&red)) return 0;
       if (! outfile->write(&green)) return 0;
       if (! outfile->write(&blue)) return 0;
       if (! outfile->write(&transparencyrange)) return 0;
    }

  return 1;
}

int count = 0;

Entity *Plane::load(int references,CadFile *infile)
{Plane *plane;
 EntityHeader *header;
 EntityList list,junklist;
 BitMask options(32);
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 RCCHAR *patternfilename,lib1[600];
 Point origin,xaxis,yaxis,anchor;
 RCCOLORREF gradientcolour;

  if ((header = loadheader(infile)) == NULL) return NULL;
  if (references)
    {  if (! list.load(infile)) return NULL;
       if (! list.loadgeometry(1,infile)) return NULL;
    }
  else
    {  if (! junklist.load(infile)) return NULL;  junklist.destroy();
       if (! list.loadgeometry(0,infile)) return NULL;
    }
  if (! infile->read(&options)) return NULL;
  if (! infile->read(&xspacing)) return NULL;
  if (! infile->read(&yspacing)) return NULL;
  if (! infile->read(&angle)) return NULL;
  if (! infile->read(&crossangle)) return NULL;
  if (! origin.load(infile)) return NULL;
  if (! xaxis.load(infile)) return NULL;
  if (! yaxis.load(infile)) return NULL;
  patternfilename = 0;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
  {
      if(infile->getversion() == 2)
      {
          char *cpatternname;
          if (! infile->read(&cpatternname))
              return NULL;
          delete [] patternfilename;
          patternfilename = new RCCHAR[strlen(cpatternname)+1];
          strcpy(patternfilename,cpatternname);
          delete cpatternname;
      }
      else
      {
          if (! infile->read(&patternfilename))
              return NULL;
          // v8
          QString qsfilename(patternfilename);
          qsfilename.replace("\\","/");
          qsfilename = QDir::toNativeSeparators(qsfilename);
          strcpy(patternfilename,qsfilename.data());
          if (strncmp(patternfilename,"::CADHD::",9) == 0)
          {
            strcpy(lib1,home.getpublicfilename(patternfilename+9));
            delete [] patternfilename;
            patternfilename = new RCCHAR[strlen(lib1)+1];
            strcpy(patternfilename,lib1);
          }
          // v8
      }
       if (fabs(xspacing)< 1.0E-20) 
         xspacing = 1.0;
       if (fabs(yspacing)< 1.0E-20) 
         yspacing = 1.0;
  }
  if (options.test(PlaneAnchored))
    if (! anchor.load(infile)) return NULL;


  if (options.test(PlaneGradientFilled))
    if (! infile->read((long *)&gradientcolour)) return NULL;

  if (options.test(PlaneHasTransparency))
    {  if (! infile->read(&transparency)) return NULL;
    }
  else
    transparency = 0.0;

  if (options.test(PlaneHasTransparentColour) && GetPrivateProfileInt("Configuration","IgnorePlaneHasTransparentColour",0,home.getinifilename()) == 0)
    {  if (! infile->read(&red)) return 0;
       if (! infile->read(&green)) return 0;
       if (! infile->read(&blue)) return 0;
       if (! infile->read(&transparencyrange)) return 0;
    }
  else
    red = green = blue = transparencyrange = 0.0;

  plane = new Plane(*header,origin,xaxis,yaxis,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
  plane->atlist = header->getatlist();
  if (options.test(PlaneAnchored))
    plane->anchor = anchor;
  delete header;
  delete [] patternfilename;
  patternfilename=0;

  return plane;
}

int Plane::save(UndoFile *outfile)
{
  short type;
  type = plane_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! list.save(outfile)) return 0;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
  {
    if (! outfile->write(patternfilename))
    {
        patternfilename=0;
        bitmape=0;
        return NULL;
    }
    else
    {
        patternfilename=0;
        bitmape=0;
    }
  }

  return 1;
}

Entity *Plane::load(UndoFile *infile)
{Plane *plane;
 EntityList list;
 RCCHAR bm[sizeof(BitMask)];
  if ((plane = new Plane) != NULL)
    {AttributeList al;
       memcpy(bm,&plane->visible,sizeof(bm));
       if (! infile->blockread(plane,sizeof(*plane))) return 0;
       memcpy(&plane->visible,bm,sizeof(bm));
       if (! infile->read(&plane->visible)) return 0;
       if (! al.load(infile)) return 0;   plane->atlist = al;
       if (! infile->read(&plane->options)) return 0;
       if (! list.load(infile)) return 0;
       plane->list = list;
       if (plane->options.test(PlanePatterned) || plane->options.test(PlaneBitmapFilled))
       {
           if (! infile->read(&plane->patternfilename))
           {
               plane->patternfilename=0;
               plane->bitmape=0;
               return NULL;
           }
       }
       else
       {
           plane->patternfilename=0;
           plane->bitmape=0;
       }
    }
  return plane;
}

int Plane::saveattributes(CadFile *outfile)
{ if (! saveheaderu(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&xspacing)) return 0;
  if (! outfile->write(&yspacing)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&crossangle)) return 0;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    if (! outfile->write(patternfilename)) return NULL;
  return 1;
}

int Plane::loadattributes(CadFile *infile)
{ if (! loadheaderu(infile)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&xspacing)) return 0;
  if (! infile->read(&yspacing)) return 0;
  if (! infile->read(&angle)) return 0;
  if (! infile->read(&crossangle)) return 0;
  if (options.test(PlanePatterned) || options.test(PlaneBitmapFilled))
    {  delete patternfilename;
       patternfilename=0;
       delete bitmape;
       bitmape=0;
       if (! infile->read(&patternfilename)) return NULL;
    }
  return 1;
}

struct CBdxfline_data
  {Plane *plane;
   DXFOut *dxffile;
   int status;
  };

int Plane::savedxf(int blocksection,DXFOut *dxffile)
{int i,j;
 Point p1,n1,n2,dir1,o,pmin,pmax,zaxis,Ax,Ay;
 CBdxfline_data cbd;
 double error;

  extents(0,&pmin,&pmax);
  error = (pmax - pmin).length() / 1000.0;
  if (error < 0.001)
    error = 0.001;
  Polygon3d polygon3d(origin,xaxis,yaxis,list,0,error);
  zaxis = xaxis.cross(yaxis);
  if (fabs(zaxis.x) < 1.0/64.0 && fabs(zaxis.y) < 1.0/64.0)
    Ax = Point(0.0,1.0,0.0).cross(zaxis);
  else
    Ax = Point(0.0,0.0,1.0).cross(zaxis);
  Ay = zaxis.cross(Ax);
  Ax = Ax.normalize();
  Ay = Ay.normalize();
  

  cbd.dxffile = dxffile;
  cbd.status = 1;
  cbd.plane = this;
  if (! blocksection)
    {  

       if (! dxffile->writestring(0, (char*)"HATCH")) return 0;
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbHatch")) return 0;
       if (! dxffile->writereal(10,0.0)) return 0;
       if (! dxffile->writereal(20,0.0)) return 0;
       if (! dxffile->writereal(30,zaxis.dot(polygon3d.getverts()[0][0]))) return 0;
       if (! dxffile->writereal(210,zaxis.x)) return 0;
       if (! dxffile->writereal(220,zaxis.y)) return 0;
       if (! dxffile->writereal(230,zaxis.z)) return 0;
       if (options.test(PlaneFilled))
         {  if (! dxffile->writestring(2, (char*)"SOLID")) return 0;
         } 
       else
         {  if (! dxffile->writestring(2, (char*)"*")) return 0;
         } 
       if (! dxffile->writelong(70,options.test(PlaneFilled))) return 0;
       if (! dxffile->writelong(71,0)) return 0;
       if (! dxffile->writelong(91,polygon3d.getnloops())) return 0;
       for (i = 0 ; i < polygon3d.getnloops() ; i++)
         {   
            if (! dxffile->writelong(92,0)) return 0;
            if (! dxffile->writelong(93,polygon3d.getnvert()[i])) return 0;
  
            for (j = 0 ; j < polygon3d.getnvert()[i] ; j++)
              {  if (! dxffile->writelong(72,1)) return 0;
                 p1 = Point(Ax.dot(polygon3d.getverts()[i][j]),Ay.dot(polygon3d.getverts()[i][j]),zaxis.dot(polygon3d.getverts()[i][j]));
                 if (! dxffile->writereal(10,p1.x)) return 0;
                 if (! dxffile->writereal(20,p1.y)) return 0;
                 p1 = Point(Ax.dot(polygon3d.getverts()[i][(j+1)%polygon3d.getnvert()[i]]),Ay.dot(polygon3d.getverts()[i][(j+1)%polygon3d.getnvert()[i]]),zaxis.dot(polygon3d.getverts()[i][(j+1)%polygon3d.getnvert()[i]]));
                 if (! dxffile->writereal(11,p1.x)) return 0;
                 if (! dxffile->writereal(21,p1.y)) return 0;
              } 
            if (! dxffile->writelong(97,0)) return 0;

         } 

       if (! dxffile->writelong(75,0)) return 0;
       if (! dxffile->writelong(76,0)) return 0;

       if (options.test(PlaneFilled))
         {  if (! dxffile->writelong(98,1)) return 0;
            if (! dxffile->writereal(10,0.0)) return 0;
            if (! dxffile->writereal(20,0.0)) return 0;
         }
       else
         {  if (! dxffile->writereal(52,0)) return 0;
            if (! dxffile->writereal(41,1.0)) return 0;
            if (! dxffile->writelong(77,0)) return 0;
            if (! dxffile->writelong(78,1 + (crossangle != 0.0))) return 0;

            if (! dxffile->writereal(53,angle)) return 0;

            if (! dxffile->writereal(43,0.001)) return 0;
            if (! dxffile->writereal(44,-0.001)) return 0;
            if (! dxffile->writereal(45,cos(angle * M_PI / 180.0 + M_PI/2.0) * xspacing)) return 0;
            if (! dxffile->writereal(46,sin(angle * M_PI / 180.0 + M_PI/2.0) * xspacing)) return 0;
            if (! dxffile->writelong(79,0)) return 0;
    
            if (crossangle != 0.0)
              {  if (! dxffile->writereal(53,crossangle)) return 0;
                 if (! dxffile->writereal(43,-0.001)) return 0;
                 if (! dxffile->writereal(44,+0.001)) return 0;
                 if (! dxffile->writereal(45,cos(crossangle * M_PI / 180.0 + M_PI/2.0) * yspacing)) return 0;
                 if (! dxffile->writereal(46,sin(crossangle * M_PI / 180.0 + M_PI/2.0) * yspacing)) return 0;
                 if (! dxffile->writelong(79,0)) return 0;
              }
            if (! dxffile->writelong(98,0)) return 0;
         }


    }
  return cbd.status;
}

int Plane::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}



int Plane::savegeometry(CadFile *outfile)
{ if (! origin.save(outfile)) return 0;
  if (! anchor.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! outfile->write(&options)) return NULL;
  return 1;
}

int Plane::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! origin.load(infile)) return 0;
  if (! anchor.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  list.destroy();
  if (! list.load(infile)) return 0;
  if (! infile->read(&options)) return NULL;
  return 1;
}

void Plane::fixupreferences(Entity ** index)
{Point o,xa,ya,zaxis;
 double D;
  list.fixupreferences(index);
  Loops loops(1,list);
  if (! loops.planar(&o,&xa,&ya,&zaxis,&D))
    defined = 0;
  else
    {//  If the plane equation is different than before, then update the plane equation.
     //  Otherwaise, leave the plane equation alone to avoid rotating textures.
       if (fabs(zaxis.dot(xaxis.cross(yaxis))) < 0.999 || zaxis.dot(o - origin) > db.getptoler())
         {  origin = o;  xaxis = xa;  yaxis = ya;
         }
    }
  Entity::fixupreferences(index);
}

void Plane::SetAnchorPoint(Point p,int dr)
{
  if (dr)
    draw(DM_ERASE);
  db.saveundo(UD_MOVED,this);
  cadwindow->invalidatedisplaylist(this);
  anchor = p;
  options.set(PlaneAnchored,1);
  if (dr)
    draw(DM_NORMAL);
  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);
}

extern BitMask * GetLockedLayerTable(void);


void Plane::verify(void)
{
#ifdef USING_WIDGETS
    // this function now just pops up the plane options dialog
    state.sendevent(new NewCommandEvent(409,0));
#else
 //   Dialog dialog("PLANEVERIFY_DIALOG");
 Dialog dialog("PlaneVerify_Dialog");
 int c,l,s,status,fillstyle,gradientfillstyle;
 double an,can,xs,ys,w,tr;
 BitMask o(32);
 RCCHAR filename[300],label1[300],label2[300];
 RCSTRING STRGradientFilll[] = { "Left to Right","Right to Left","Top to Bottom","Bottom to Top","Left to Centre to Right","Right to Centre to Left","Top to Centre to Bottom","Bottom to Centre to Top","Outside to Inside","Inside To Outside"  };
 RCCHAR gf1[25],gf2[25],gf3[25],gf4[25],gf5[25],gf6[25],gf7[25],gf8[25],gf9[25],gf10[25];
 RCCHAR *GradientFilll[] = { gf1,gf2,gf3,gf4,gf5,gf6,gf7,gf8,gf9,gf10 };
 double gred,ggreen,gblue;

 for(int i=0; i<10; i++)
     strcpy(GradientFilll[i],&STRGradientFilll[i]);

  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new PlaneVerifyButtonControl(1000));
  dialog.add(new PlaneVerifyButtonControl(1001));
  dialog.add(new PlaneVerifyButtonControl(1002));
  dialog.add(new PlaneVerifyButtonControl(1003));

  xs = xspacing;   dialog.add(new RealDialogControl(104,&xspacing,1.0,0.001,1000000.0,ScaleByLength));
  ys = yspacing;   dialog.add(new RealDialogControl(105,&yspacing,1.0,0.001,1000000.0,ScaleByLength));
  tr = transparency;  dialog.add(new RealDialogControl(125,&transparency,0.1,0.0,1.0,DontScale));
  an = angle;  dialog.add(new RealDialogControl(106,&angle));
  can = crossangle;  dialog.add(new RealDialogControl(107,&crossangle));
  //o = options;  dialog.add(new CheckBoxDialogControl(108,&o,0));
  //o = options;  dialog.add(new CheckBoxDialogControl(109,&o,1));
  if (patternfilename != 0)
    strcpy(filename,patternfilename);
  else
  {
    patternfilename = new RCCHAR[5];
    strcpy(patternfilename,"");
    strcpy(filename,"");
  }
  fillstyle = 1;
  if (options.test(PlaneNoFill))
    fillstyle = 0;
  else if (options.test(PlanePatterned))
    fillstyle = 2;
  else if (options.test(PlaneFilled))
    fillstyle = 3;
  else if (options.test(PlaneBitmapFilled))
    fillstyle = 4;
  else if (options.test(PlaneGradientFilled))
    fillstyle = 5;

  gradientfillstyle = options.test(PlaneGradientStyle1) * 8 + options.test(PlaneGradientStyle2) * 4 +  options.test(PlaneGradientStyle3) * 2 +  options.test(PlaneGradientStyle4); 

  gred = GetRValue(gradientcolour) / 255.0;
  ggreen = GetGValue(gradientcolour) / 255.0;
  gblue = GetBValue(gradientcolour) / 255.0;

  strcpy(label1,"Spacing");
  strcpy(label2,"Cross hatch spacing");

  if (fillstyle == 0 || fillstyle == 2)
  {
       strcpy(label1,"Horizontal scale");
       strcpy(label2,"Vertical scale");
       xs *= db.getmainunitscale();
       ys *= db.getmainunitscale();
  }
  else if (fillstyle == 4)
  {
       strcpy(label1,"Bitmap width");
       strcpy(label2,"Bitmap height");
  }
  else
  {
       strcpy(label1,"Spacing");
       strcpy(label2,"Cross hatch spacing");
  }

  dialog.add(new StringDialogControl(204,label1,300));
  dialog.add(new StringDialogControl(205,label2,300));
  dialog.add(new StringDialogControl(108,filename,300));
  dialog.add(new PSButtonDialogControl(109));
  dialog.add(new PSButtonDialogControl(110));

  dialog.add(new RadioButtonDialogControl(114,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(115,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(116,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(117,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(118,114,119,&fillstyle));
  dialog.add(new RadioButtonDialogControl(119,114,119,&fillstyle));
  dialog.add(new CheckBoxDialogControl(120,&options,PlaneDisplayEdges));
  dialog.add(new ListDialogControl(121,10,GradientFilll,&gradientfillstyle));
  dialog.add(new RealDialogControl(122,&gred,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(123,&ggreen,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(124,&gblue,0.01,0.0,1.0,DontScale));
  dialog.add(new GradientPlaneButtonDialogControl(1004,122));

  dialog.add(new RealDialogControl(127,&red,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(128,&green,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(129,&blue,0.01,0.0,1.0,DontScale));
  dialog.add(new RealDialogControl(126,&transparencyrange,0.01,0.0,1.0,DontScale));
  dialog.add(new GradientPlaneButtonDialogControl(1005,127));

  dialog.add((DisplayDialogControl *) new PlaneDisplayDialogControl(219));
  //if (GetLockedLayerTable() == 0 || GetLockedLayerTable()->test(getlayer()) == 0)
  //{
       dialog.add(new ButtonDialogControl(111)); // change surface properties
       dialog.add(new ButtonDialogControl(112)); // align
       dialog.add(new ButtonDialogControl(113)); // set anchor
       dialog.add(new ButtonDialogControl(131)); // clone (used in setup options)
       dialog.add(new ButtonDialogControl(132)); // clone all (used in setup options)
  //}

  dialog.setposition(DPOK);
  status = dialog.process();
  if(status == TRUE)
  {
      db.saveundo(UD_CHANGEATTRIBUTES,this);
      draw(DM_ERASE);
      cadwindow->invalidatedisplaylist(this);

      // OK button
      setcolour(c);
      setstyle(s);
      setlayer(l);
      setweight(db.lineweights.mmtoweight(w));

      gradientcolour = RGB(int(gred*255.0),int(ggreen*255.0),int(gblue*255.0));

      options.set(PlaneNoFill,fillstyle == 0);
      if((options.test(PlanePatterned) || options.test(PlaneBitmapFilled)) && fillstyle == 0)
      {
          this->qgplane = 0;
      }
      options.set(PlanePatterned,fillstyle == 2);
      options.set(PlaneFilled,fillstyle == 3);
      options.set(PlaneBitmapFilled,fillstyle == 4);
      options.set(PlaneGradientFilled,fillstyle == 5);

      options.set(PlaneGradientStyle1,(gradientfillstyle&8)==8);
      options.set(PlaneGradientStyle2,(gradientfillstyle&4)==4);
      options.set(PlaneGradientStyle3,(gradientfillstyle&2)==2);
      options.set(PlaneGradientStyle4,(gradientfillstyle&1)==1);

      if((options.test(PlanePatterned) || options.test(PlaneBitmapFilled)) && strlen(filename) > 0)
      {
          delete bitmape;
          bitmape = 0;
          delete [] patternfilename;
          patternfilename = new RCCHAR[strlen(filename)+1];
          strcpy(patternfilename,filename);
          if (patternfilename != 0 && options.test(PlaneBitmapFilled))
          {
              Point xa,ya;
              Transform t1;
              if (options.test(1))
              {
                   t1.rotate(Point(0.0,0.0,0.0),xaxis.cross(yaxis),angle * acos(-1.0) / 180.0);
                   xa = t1.transform(xaxis);
                   ya = t1.transform(yaxis);
              }
              else
              {
                  xa = xaxis;
                  ya = yaxis;
              }
              bitmape = new BitMapE(patternfilename,origin,
                                    xa,ya,xspacing,yspacing,
                                    red,green,blue,
                                    transparencyrange,1.0-transparency,options);

              if(bitmape != 0)
              {
                  if(qgplane)
                    qgplane->rcbitmap = bitmape;
              }
          }
      }
      else
      {
          delete bitmape;
          bitmape = 0;
          delete [] patternfilename;
          patternfilename = 0;
          if(qgplane)
            qgplane->rcbitmap = 0;
      }

      draw(DM_NORMAL);

  }
  else if (status == 112) // change (not used)
    ::state.sendevent(new NewCommandEvent(409,0));
  else if (status == 113) // set anchor
    ::state.sendevent(new NewCommandEvent(1511,0));
  else if (status == 111)
  {
      // change properties
      SurfaceProperties sp;
      sp.verify(this,0);
  }
#endif
}

static void explodeline(Point p1,Point p2,void *data)
{Entity *e = (Entity *) data;
 Entity *line;
  if ((line = new Line(*e,p1,p2)) != 0)
    db.geometry.add(line);
}

int Plane::explode(void)
{int i,n;
 double a,len;
 Point p1,n1,n2,dir1,o;
  if (cadwindow->getcurrentwindow() == 0)
    return 0;
  Polygon3d polygon3d(origin,xaxis,yaxis,list,0,
                    (cadwindow->getcurrentwindow()->getumax() - cadwindow->getcurrentwindow()->getumin()) / cadwindow->getcurrentwindow()->getwidth());
  if (options.test(0))
    return 0;
  else
    {  for (i = 0 ; i < 2 ; i ++)
         {  a = (i == 0 ? angle : angle + crossangle);
            o = origin;
            dir1 = xaxis * cos(a * M_PI / 180.0) + yaxis * sin(a * M_PI / 180.0);
            n1 = dir1.cross(xaxis.cross(yaxis));
            if ((len = n1.length()) < db.getptoler()) return 0;
            n1 /= len;
            if (i == 0) n2 = n1 * xspacing; else n2 = n1 * yspacing;
            p1 = o;
            n = 0;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,explodeline,this) >= 0)
              {  p1 += n2;  n++;
              }
            p1 = o - n2;
            while (n < 10000 && polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,explodeline,this) <= 0)
              {  p1 -= n2;  n++;
              }
            if (fabs(crossangle) < db.getatoler()) break;
         }
    }
   return 1;
}

int Plane::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(plane_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

void Plane::extents(View3dSurface *surface,Point *pmin,Point *pmax)
/*
{Entity *e;
 Point pmin1,pmax1;
  pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
  list.start();
  // KMJ TODO: the first item in the list might be NULL
  // fix that bug
  //if ((e = list.next()) == NULL)
  //    return;
  // work around for that bug
  int len = list.length();
  if(len < 1)
      return;
  for (int i=0 ; i < len; i++)
  {
      e = list.next();
      if(e != NULL)
          break;
  }
  e->extents(surface,pmin,pmax);
  for ( ; (e = list.next()) != NULL ; )
    {  e->extents(surface,&pmin1,&pmax1);
       if (pmin1.x < pmin->x) pmin->x = pmin1.x;
       if (pmin1.y < pmin->y) pmin->y = pmin1.y;
       if (pmin1.z < pmin->z) pmin->z = pmin1.z;
       if (pmax1.x > pmax->x) pmax->x = pmax1.x;
       if (pmax1.y > pmax->y) pmax->y = pmax1.y;
       if (pmax1.z > pmax->z) pmax->z = pmax1.z;
    }
*/
{Entity *e = NULL;
#if 1
 Point pmin1(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax1(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
 *pmin = pmin1; *pmax = pmax1;
#else
Point pmin1,pmax1;
 pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
#endif
 list.start();
 // KMJ TODO: the first item in the list might be NULL
 // fix that bug
 //if ((e = list.next()) == NULL)
 //    return;
 // work around for that bug
 // get the extents of all the ents ingoring the null ents (loop markers)
#if 1
 int len = list.length();
 for (int i=0; i<len; i++ )
 {
      e = list.next();
      if(e == NULL)
         continue;
      e->extents(surface,&pmin1,&pmax1);
      if (pmin1.x < pmin->x) pmin->x = pmin1.x;
      if (pmin1.y < pmin->y) pmin->y = pmin1.y;
      if (pmin1.z < pmin->z) pmin->z = pmin1.z;
      if (pmax1.x > pmax->x) pmax->x = pmax1.x;
      if (pmax1.y > pmax->y) pmax->y = pmax1.y;
      if (pmax1.z > pmax->z) pmax->z = pmax1.z;
 }
#else
 // this is the old method not tolerant of bad loops
 int len = list.length();
 if(len < 1)
     return;
 for (int i=0 ; i < len; i++)
 {
     e = list.next();
     if(e != NULL)
         break;
 }
 e->extents(surface,pmin,pmax);
 for ( ; (e = list.next()) != NULL ; )
   {  e->extents(surface,&pmin1,&pmax1);
      if (pmin1.x < pmin->x) pmin->x = pmin1.x;
      if (pmin1.y < pmin->y) pmin->y = pmin1.y;
      if (pmin1.z < pmin->z) pmin->z = pmin1.z;
      if (pmax1.x > pmax->x) pmax->x = pmax1.x;
      if (pmax1.y > pmax->y) pmax->y = pmax1.y;
      if (pmax1.z > pmax->z) pmax->z = pmax1.z;
   }
#endif
}

int Plane::LiesOn(Point p)
{ return fabs((xaxis.cross(yaxis)).dot(p - origin)) < db.getptoler();
}

Polygon3d::Polygon3d(Point o,Point xa,Point ya,EntityList list,int nsegs,double error)
{
    loopDirections=0;
    loopContainments=0;

 LinearEntity *e,*enext;
 int i,j,n,reverse;
 Point endp,za;
 double d1,d2,d3,d4;

  defined = 0;
  origin = o;  xaxis = xa;  yaxis = ya;
  for (list.start(),nloops = 1 ; ! list.atend() ; )
    {  e = (LinearEntity *)list.next();
       if (e == NULL) nloops++;
    }
  normals = 0;
  uvs = 0;
  if ((nvert = new int[nloops]) == NULL) return;
  if ((verts = new Point *[nloops]) == NULL)
    {  delete [] nvert;
       return;
    }
  for (list.start(),i = 0,nvert[0] = 0 ; ! list.atend() ; )
    {  e = (LinearEntity *)list.next();
       if (e == NULL)
         {  if (nvert[i] > 2)
              i++;
            nvert[i] = 0;
         }
       else
         {  if (nsegs == 0)
              nvert[i] += e->nsegments(error);
            else
              nvert[i] += nsegs;
         }
    }
  if (nvert[i] > 2) i++;  //  Add the last loop to the count
  nloops = i;

  for (i = 0 ; i < nloops ; i++)
    if (nvert[i] > 0 && (verts[i] = new Point[nvert[i]]) == NULL)
      {  for (j = 0 ; j < i ; j++) delete [] verts[j];
         delete [] verts;  delete [] nvert;
      }

  for (list.start(),i = j = 0 ; nloops > 0 && ! list.atend() ; )
    {
       reverse = 0;
       if ((e = (LinearEntity *)list.next()) != NULL)
         {  if (j == 0)
              {  if ((enext = (LinearEntity *)list.peek()) != NULL)
                   {  d1 = (e->start() - enext->start()).length();
                      d2 = (e->start() - enext->end()).length();
                      d3 = (e->end() - enext->start()).length();
                      d4 = (e->end() - enext->end()).length();
                      if (d1 > d2) d1 = d2;
                      if (d3 > d4) d3 = d4;
                      if (d3 < d1)
                        endp = e->end();
                      else
                        {
                          endp = e->start();   reverse = LE_REVERSE;
                        }
                   }
                 else
                   endp = e->end();
              }
            else
              {  d1 = (endp - e->start()).length();
                 d2 = (endp - e->end()).length();
                 if (d2 > d1)
                   endp = e->end();
                 else
                   {
                     endp = e->start();  reverse = LE_REVERSE;
                   }
              }
         }
       if (e == NULL)
         {  if (j > 2) i++;   // Discard invalid loops
            j = 0;
            if (i == nloops)
              break;
         }
       else
         {  if (nsegs == 0)
              {  n = e->nsegments(error);
                 if (n > nvert[i])
                   n = e->nsegments(error);
              }
            else
              n = nsegs;
            e->segment(n,LE_INCLUDEEND | reverse,verts[i]+j);
            j += n;
         }
    }

  za = xaxis.cross(yaxis);
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      verts[i][j] -= za * (za.dot(verts[i][j] - origin));

  // KMJ : debug :
  // fix the loop directions to alternate the direction
  // 1. set all the loop to be the same direction??
  // 2. find the cointainment count for the loop (how many times it is contained within other loops)
  // 3. change the direction of the loop depending on the containment number
  // 4. outer loop is not changed; inner loops odd numbers are flipped
  /*
  int ssense = getLoopSense(0);
  for (i = 1 ; i < nloops ; i++)
  {
      getLoopSense(i);
  }
  */

#if 0
  qDebug() << "polygon3d nloops : " << nloops;
  for (i = 0 ; i < nloops ; i++)
  {
    qDebug() << "polygon3d nvert[" << i << "] : " << nvert[i];
    for (j = 0 ; j < nvert[i] ; j++)
    {
        qDebug() << "polygon3d verts[" << i << "][" << j << "] : " << verts[i][j].x << "," << verts[i][j].y << "," << verts[i][j].y;
    }
  }
  // KMJ : end debug
#endif
  defined = 1;
}

Polygon3d::Polygon3d(Point p1,Point p2,Point p3)
{
  loopDirections=0;
  loopContainments=0;

  double len;
  nloops = 0;
  normals = 0;
  uvs = 0;
  defined = 0;
  origin = p1;
  xaxis = p2 - p1;
  yaxis = (xaxis.cross(p3-p1)).cross(xaxis);
  if ((len = xaxis.length()) < db.getptoler()) return;
  xaxis /= len;
  if ((len = yaxis.length()) < db.getptoler()) return;
  yaxis /= len;
  if ((nvert = new int[1]) == NULL) return;
  if ((verts = new Point *[1]) == NULL)
    {  delete [] nvert;  return;
    }
  if ((verts[0] = new Point[3]) == NULL)
    {  delete [] verts;  delete [] nvert;  return;
    }
  nloops = 1;
  nvert[0] = 3;
  verts[0][0] = p1;  verts[0][1] = p2;  verts[0][2] = p3;
  defined = 1;
}

Polygon3d::Polygon3d(Point p1,Point p2,Point p3,Point p4)
{
  loopDirections=0;
  loopContainments=0;

  double len;
  nloops = 0;
  normals = 0;
  uvs = 0;
  defined = 0;
  origin = p1;
  xaxis = p2 - p1;
  yaxis = (xaxis.cross(p3-p1)).cross(xaxis);
  if ((len = xaxis.length()) < db.getptoler()) return;
  xaxis /= len;
  if ((len = yaxis.length()) < db.getptoler()) return;
  yaxis /= len;
  if ((nvert = new int[1]) == NULL) return;
  if ((verts = new Point *[1]) == NULL)
    {  delete [] nvert;  return;
    }
  if ((verts[0] = new Point[4]) == NULL)
    {  delete [] verts;  delete [] nvert;  return;
    }
  nloops = 1;
  nvert[0] = 4;
  verts[0][0] = p1;  verts[0][1] = p2;  verts[0][2] = p3;  verts[0][3] = p4;
  defined = 1;
}

Polygon3d::Polygon3d(Point o,Point xa,Point ya,int nl,int *nv,Point **v)
{
  loopDirections=0;
  loopContainments=0;

  origin = o;
  xaxis = xa;
  yaxis = ya;
  nloops = nl;
  nvert = nv;
  verts = v;
}

#ifdef USING_WIDGETS
/**
 * @brief Polygon3d::getLoopSense
 * @param lid = loop id
 * @return 0=CCW; 1=CW
 *
 * Finds the direction sense of the given loop
 * calculates the average normal vectors between the edges
 * returns -1 if the sense is CCW or 1 if it is CW
 * returns 0 if the loop id is illegal
 */
int Polygon3d::getLoopSense(int lid)
{
    // get the normal vectors between consecutive edges
    // compare with the z vector of the plane

    Point p1,p2,p3,nv;

    if(lid < 0 || lid > nloops)
        return 0;

    for (int j = 0 ; j < nvert[lid]-1 ; j++)
    {
        p1 = verts[lid][j];
        p2 = verts[lid][j+1];
        if(j < nvert[lid]-2)
            p3 = verts[lid][j+2];
        else
            p3 = verts[lid][0];

        // CW points gives normal z = 1
        // CCW points gives normal z = -1
        nv += (p2-p1).cross(p3-p2).normalize();
    }
    return (nv.z / nvert[lid]) > 0.0 ? 1 : -1;
}

/**
 * @brief Polygon3d::setLoopDirection
 * @param lid = loop id
 */
void Polygon3d::reverseLoopDirection(int lid)
{
    if(lid < 0 || lid > nloops)
        return;

    Point *nverts;
    nverts = new Point[nvert[lid]];

    for (int j = 0 ; j < nvert[lid]; j++)
        nverts[j] = verts[lid][nvert[lid]-j-1];

    for (int j = 0 ; j < nvert[lid]; j++)
        verts[lid][j] = nverts[j];
}

/**
 * @brief Polygon3d::fixLoopDirections
 *
 * fixes the loop vertex directions to be suitable for clippig with a painter path
 * i.e. the loop directions alternate between islands and holes
 *
 */
void Polygon3d::fixLoopDirections()
{
    // get the first loop sense
    int sense = getLoopSense(0);
    findLoopContainments();
    int cont = loopContainments[0];

    // make all outer loops (islands) the same as the first loop
    // make all holes the opposite of the first loop

    for(int i=1; i < nloops; i++)
    {
        if(/*loopContainments[i] == cont &&*/ (i % 2 != 0) && getLoopSense(i) == sense)
            reverseLoopDirection(i);
        //else if(loopContainments[i] != cont && getLoopSense(i) == sense)
        //    reverseLoopDirection(i);
    }
}

/**
 * @brief Polygon3d::findLoopDirections
 *
 * find the directions of loops compared to the first loop in a plane
 *
 * method:
 * If there is more than one loop in the plane
 * set the direction flag of all loops to 1 (same direction)
 * Check to see if each loop is contained within or intersects any other loop
 * if a loop is contained or intersects any other loop set its direction flag to -1 (reverse direction)
 *
 */
void Polygon3d::findLoopDirections()
{
    findLoopContainment(0);
}

/**
 * @brief Polygon3d::findLoopDirection
 * @param lid
 * @return the loop direction for this loop lid
 *
 * finds the loop direction for the loop given lid
 * this will check the direction of the given loop agains all the other loops in the plane
 * set the loop direction for the loop in the loopDirections list and return it
 *
 * KMJ : this code is not finished. Don't use it yet...
 */
int Polygon3d::findLoopDirection(int lid)
{
    // check each point in the loop against all the loops in the plane

    // method:
    //
    // initialise a counter to 0
    // set up the vertex structures
    // for each loop in the plane
    //    for each vertex in the given loop
    //      check if it is contained by the given loop
    //      if contained set the contained flag increment the counter
    //      if it intersects a loop set the flag to increment the counter
    //      end for
    // end for
    // if the counter is an even number or zero set the containment flag to 1
    // else if the counter is an odd number set the conrinment flag to -1
    //

    int i,j,count;
    Point p;

    if(lid < 0 || lid > nloops)
        return 0;

    if(nloops == 1)
        return 1;

    if(loopDirections == 0)
    {
        loopDirections = new int[nloops];
        for (i = 0 ; i < nloops ; i++)
            loopDirections[i]=0;
    }
    else
        loopDirections[lid]=0;

    count=0;
    for (i = 0 ; i < nloops ; i++)
    {
        // don't check against the input loop
        if(i == lid)
            continue;

        // move the loop points to a vector for now
        // TODO: change the contains function to use verts
        std::vector<Point> polygon;
        for (j = 0 ; j < nvert[i] ; j++)
            polygon.push_back(verts[i][j]);

        // check each point in the input loop with the
        // current loop polygon
        int thiscount=0;
        for (j = 0 ; j < nvert[lid] ; j++)
            if( polygonContains(polygon,verts[lid][j]) )
                thiscount++;
        if(thiscount == 0) // totaly outside
            ;// count = 0; do nothing
        else if(thiscount == nvert[i]) // totaly inside
            count++;
        else if(thiscount < nvert[i]) // partial (intersection)
            count++;
    }

    // 0 count is external
    // even count is an island
    // uneven count is a hole
    // negative is intersecting
    if(lid == 0)
        loopDirections[lid] = 1;
    else if( count % 2 == 0 )
        loopDirections[lid] = 1;
    else
        loopDirections[lid] = -1;

    return loopDirections[lid];
}


/**
 * @brief Polygon3d::getLoopDirection
 * @param lid
 * @return the stored loop direction flag for loop lid
 *
 * If the loop flag is set to 0 (unknown direction) the function will find the direction and set it
 * NOTE: this may be slow so don't reset the flag unnecessarily
 *
 */
int Polygon3d::getLoopDirection(int lid)
{
    if(lid < 0 || lid > nloops)
        return 0;

    if(loopDirections && loopDirections[lid])
        return loopDirections[lid];
    else
        return findLoopDirection(lid);
}
#endif

int Polygon3d::hatchline(int threed,Point n1,double D1,Point p1,Point dir2,HTCallback cb,void *data)
{int i,j,k,nplus,nminus,nzero;
 double u1,u2,u3,t1,t2;
 Point p,p3,p4,p5,dir1;
 SortedDoubleList *list,l;

  if (cb == NULL)
    list = (SortedDoubleList *) data;
  else
    list = & l;   
  nplus = nminus = nzero = 0;
  for (i = 0 ; i < nloops ; i++)
    {  p3 = verts[i][0];
       u1 = n1.dot(p3) + D1;  nplus += u1 > 0.0;  nminus += u1 < 0.0;  nzero += u1 == 0.0;
       for (j = 1 ; j <= nvert[i] ; j++)
         {  p4 = verts[i][j % nvert[i]];
            u2 = n1.dot(p4) + D1; nplus += u2 > 0.0;  nminus += u2 < 0.0;  nzero += u2 == 0.0;
            if (u1 <= 0.0 && u2 >= 0.0 || u1 >= 0.0 && u2 <= 0.0)
              {  if (u1 == 0.0)
                   { // Do nothing if u1 == 0.0
                   }
                 else //if (fabs(u2 - u1) > db.getptoler())
                   {  for (k = j + 1,u3 = u2 ; u3 == 0.0 ; k++)
                        u3 = n1.dot(verts[i][k % nvert[i]]) + D1;
                      if (u1 < 0.0 && u3 > 0.0 || u1 > 0.0 && u3 < 0.0)
                        {  p = p3 - (p4 - p3) * (u1 / (u2 - u1));
                           dir1 = p - p1;
                           if (threed)
                             {  if      (fabs(dir2.x) >= fabs(dir2.y) && fabs(dir2.x) >= fabs(dir2.z))
                                  list->add(dir1.x / dir2.x);
                                else if (fabs(dir2.y) >= fabs(dir2.x) && fabs(dir2.y) >= fabs(dir2.z))
                                  list->add(dir1.y / dir2.y);
                                else
                                  list->add(dir1.z / dir2.z);
                             }
                           else
                             {  if      (fabs(dir2.x) >= fabs(dir2.y))
                                  list->add(dir1.x / dir2.x);
                                else
                                  list->add(dir1.y / dir2.y);
                             }
                        }
                   }
              }
            p3 = p4;  u1 = u2;
         }
    }
  if (cb != NULL)
    {  for (list->start() ; ! list->atend() ; )
         {  t1 = list->next();  if (list->atend()) break;
            t2 = list->next();
            cb(p1 + dir2 * t1,p1 + dir2 * t2,data);
         }
       list->destroy();
    }
  if (nminus + nzero == 0) return 1;
  else if (nplus + nzero == 0) return -1;
  else return 0;
}

void Polygon3d::findLoopContainments()
{
    // check each point in the loop against all the loops in the plane
    // method:
    //
    // initialise a counter to 0
    // set up the vertex structures
    // for each loop in the plane
    //    for each vertex in the given loop
    //      check if it is contained by the given loop
    //      if contained set the contained flag increment the counter
    //      if it intersects a loop set the flag to increment the counter
    //      end for
    // end for
    // if the counter is an even number or zero set the containment flag to 1
    // else if the counter is an odd number set the continment flag to -1
    //

    int i,j,k,count,thiscount;
    Point p;

    if(loopContainments == 0)
    {
        loopContainments = new int[nloops];
        for (i = 0 ; i < nloops ; i++)
            loopContainments[i]=0;
    }

    // trivial case of a single loop
    if(nloops == 1)
    {
        loopContainments[0] = 1;
        return;
    }

    // pass 1 get the basic containments
    for (k = 0 ; k < nloops ; k++)
    {
        count=0;
        thiscount=0;
        for (i = 0 ; i < nloops ; i++)
        {
            // don't check against the input loop
            if(i == k)
                continue;

            // move the loop points to a vector for now
            // TODO: change the contains function to use verts
            std::vector<Point> polygon;
            for (j = 0 ; j < nvert[i] ; j++)
                polygon.push_back(verts[i][j]);

            // check each point in the input loop with the
            // current loop polygon
            thiscount=0;
            for (j = 0 ; j < nvert[k] ; j++)
            {
                if( polygonContains(polygon,verts[k][j]) )
                    thiscount++;
                else
                    thiscount--;
            }
            qDebug() << "thiscount : " << thiscount;

            if(thiscount == 0) // totally outside
                count+=1;
            else if(thiscount >= 0 /*nvert[k]*/) // totally contains
                count+=1;
            else if(thiscount < 0 /*nvert[k]*/) // totally contained
                count-=1;
        }

        if( count > 0 )
            loopContainments[k] = 1; // contains island
        else
            loopContainments[k] = -1; // contained hole

    }

    /*
    // pass 2 adjust for islands in holes
    count=0;
    for (k = 0 ; k < nloops ; k++)
    {
        for (i = 0 ; i < nloops ; i++)
        {
            // don't check against the input loop
            if(i == k)
                continue;

            // move the loop points to a vector for now
            // TODO: change the contains function to use verts
            std::vector<Point> polygon;
            for (j = 0 ; j < nvert[i] ; j++)
                polygon.push_back(verts[i][j]);

            // check each point in the input loop with the
            // current loop polygon
            for (j = 0 ; j < nvert[k] ; j++)
                if( polygonContains(polygon,verts[k][j]) && loopContainments[i] == -1)
                    loopContainments[k] = 1;
                else if( polygonContains(polygon,verts[k][j]) && loopContainments[i] == 1)
                    loopContainments[k] = -1;
        }
    }
    */

    return;
}

int Polygon3d::findLoopContainment(int lid)
{
    // check each point in the loop against all the loops in the plane

    // method:
    //
    // initialise a counter to 0
    // set up the vertex structures
    // for each loop in the plane
    //    for each vertex in the given loop
    //      check if it is contained by the given loop
    //      if contained set the contained flag increment the counter
    //      if it intersects a loop set the flag to increment the counter
    //      end for
    // end for
    // if the counter is an even number or zero set the containment flag to 1
    // else if the counter is an odd number set the continment flag to -1
    //

    int i,j,count;
    Point p;

    if(lid < 0 || lid > nloops)
        return 0;


    if(loopContainments == 0)
    {
        loopContainments = new int[nloops];
        for (i = 0 ; i < nloops ; i++)
            loopContainments[i]=0;
    }
    else
        loopContainments[lid]=0;

    if(nloops == 1)
    {
        loopContainments[lid] = 1;
        return loopContainments[lid];
    }

    count=0;
    for (i = 0 ; i < nloops ; i++)
    {
        // don't check against the input loop
        if(i == lid)
            continue;

        // move the loop points to a vector for now
        // TODO: change the contains function to use verts
        std::vector<Point> polygon;
        for (j = 0 ; j < nvert[i] ; j++)
            polygon.push_back(verts[i][j]);

        // check each point in the input loop with the
        // current loop polygon
        int thiscount=0;
        for (j = 0 ; j < nvert[lid] ; j++)
            if( polygonContains(polygon,verts[lid][j]))
                thiscount++;
        if(thiscount == 0) // totally outside
            count=-1;
        else if(thiscount == nvert[i]) // totally inside
            count=1;
        else if(thiscount < nvert[i]) // partial (intersection)
            count=1;
    }

    if( count > 0 )
        loopContainments[lid] = -1;
    else
        loopContainments[lid] = 1;

    return loopContainments[lid];
}

int Polygon3d::getLoopContainment(int lid)
{
    if(lid < 0 || lid > nloops)
        return 0;

    if(loopContainments && loopContainments[lid])
        return loopContainments[lid];
    else
        return findLoopContainment(lid);
}

void Polygon3d::modeltoview(View3dSurface *surface)
{int i,j,k,c;
 Point xa,ya,za,o;
 CalculatePlane cp;
 double d,dmin,D1;
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      verts[i][j] = surface->modeltoview(verts[i][j]);
  cp.initialize();
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      cp.add(verts[i][j]);
  for (k = 0,defined = 0,dmin = -1.0 ; k < 3 ; k++)
    {  if (cp.planar(k,&o,&xa,&ya,&za,&D1))
         {  for (i = 0,d = 0.0,c = 0 ; i < nloops ; i++)
              for (j = 0 ; j < nvert[i] ; j++,c++)
                 d += cp.check(verts[i][j]);
            d /= double(c);
            if (d < db.getptoler() && (! defined || d < dmin))
              {  defined = 1;  dmin = d;
                 origin = o;  xaxis = xa;  yaxis = ya;
              }
         }
    }
}

void Polygon3d::modeltoscreen(View3dSurface *surface)
{int i,j,k,c,defined;
 Point xa,ya,za,o;
 CalculatePlane cp;
 double d,dmin,D1;

  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      verts[i][j] = surface->modeltoscreen(verts[i][j]);
  cp.initialize();
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      cp.add(verts[i][j]);
  for (k = 0,defined = 0,dmin = -1.0 ; k < 3 ; k++)
    {  if (cp.planar(k,&o,&xa,&ya,&za,&D1))
         {  for (i = 0,d = 0.0,c = 0 ; i < nloops ; i++)
              for (j = 0 ; j < nvert[i] ; j++,c++)
                 d += cp.check(verts[i][j]);
            d /= double(c);
            if (d < db.getptoler() && (! defined || d < dmin))
              {  defined = 1;  dmin = d;
                 origin = o;  xaxis = xa;  yaxis = ya;
              }
         }
    }
}

double Polygon3d::pick(View3dSurface *surface,double x,double y,Point *pos)
{int i,j,k,ntoleft;
 double d,x1,y1,x2,y2,x3,y3;
 Point p1,p2;
  //  Is it near an edge?
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      if (surface != 0 && (d = surface->pick(verts[i][j],verts[i][(j+1)%nvert[i]],x,y,pos)) >= 0.0)
        return d;
  //  Is it inside the polygon ?
  if (surface != 0)
  { clip(surface);
    for (i = 0 ; i < nloops ; i++)
        for (j = 0 ; j < nvert[i] ; j++)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
  }
  ntoleft = 0;
  for (i = 0 ; i < nloops ; i++)
    {  x1 = verts[i][nvert[i]-1].x;  y1 = verts[i][nvert[i]-1].y;
       for (j = 0 ; j < nvert[i] ; j++)
         {  x2 = verts[i][j].x;
            y2 = verts[i][j].y;
            if (y1 <= y && y2 >= y || y1 >= y && y2 <= y)
              {  if (y1 == y)
                   { // Do nothing if y1 == y
                   }
                 else
                   {  for (k = j + 1,y3 = y2 ; y3 == y ; k++)
                        y3 = verts[i][k % nvert[i]].y;
                      if (y1 < y && y3 > y || y1 > y && y3 < y)
                        {  x3 = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                           ntoleft += x3 < x;
                        }
                   }
              }
            x1 = x2;  y1 = y2;
         }
    }
  if ((ntoleft % 2) == 0)
    return -1.0;
  else
    return db.getstoler() * 0.999999;
}

#ifdef USING_WIDGETS
void Polygon3d::fillGL(int,Transform *transform,View3dSurface *surface)
{
    if(transform)
        this->transform(transform);
    DrawPolygon(getorigin(),getxaxis(),getyaxis(),getnloops(),getnvert(),getverts());

#if 0
  int i,j;

  clip(surface);

  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      if (transform == 0)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
      else
        verts[i][j] = surface->modeltoscreen(transform->transform(verts[i][j]));

  return surface->fill(verts,nvert,nloops);
#endif
}

void Polygon3d::fill(int, Transform *transform, View3dSurface *surface)
{
  int i,j;

  clip(surface);

  //Point pmin,pmax;
  QBrush brush = Qt::SolidPattern;
  //QLinearGradient linearGrad;

  pmax = getpmin();
  pmin = getpmax();

  for (i = 0 ; i < nloops ; i++)
  {
    for (j = 0 ; j < nvert[i] ; j++)
    {
      if (transform == 0)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
      else
        verts[i][j] = surface->modeltoscreen(transform->transform(verts[i][j]));
    }
  }


  surface->fill(0,verts,nvert,nloops);

}

QGraphicsItem * Polygon3d::fill(int, Transform *transform, View3dSurface *surface, QBrush *brush)
{
  int i,j;

  clip(surface);

  for (i = 0 ; i < nloops ; i++)
  {
    for (j = 0 ; j < nvert[i] ; j++)
    {
      if (transform == 0)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
      else
        verts[i][j] = surface->modeltoscreen(transform->transform(verts[i][j]));
    }
  }

  return surface->fill(0,verts,nvert,nloops,0,brush);

}

QGraphicsItem * Polygon3d::fill(int, Transform *transform, View3dSurface *surface, int fillstyle)
{
  int i,j;

  clip(surface);

  //Point pmin,pmax;
  QBrush brush = Qt::SolidPattern;
  //QLinearGradient linearGrad;

  //pmax = getpmin();
  //pmin = getpmax();

  for (i = 0 ; i < nloops ; i++)
  {
    for (j = 0 ; j < nvert[i] ; j++)
    {
      if (transform == 0)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
      else
        verts[i][j] = surface->modeltoscreen(transform->transform(verts[i][j]));

    //if(pmin.x > verts[i][j].x) pmin.x = verts[i][j].x;
    //if(pmin.y > verts[i][j].y) pmin.y = verts[i][j].y;
    //if(pmax.x < verts[i][j].x) pmax.x = verts[i][j].x;
    //if(pmax.y < verts[i][j].y) pmax.y = verts[i][j].y;
    }
  }

  /*
  switch(fillstyle)
  {
  case 0:
      //linearGrad = QLinearGradient(QPointF(pmin.x, pmin.y), QPointF(pmax.x, pmax.y));
      linearGrad = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
      linearGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
      linearGrad.setColorAt(0, Qt::black);
      linearGrad.setColorAt(1, Qt::white);
      brush =  QBrush(linearGrad);
      break;
  default :
      break;
  }
  */

  return surface->fill(0,verts,nvert,nloops,0,&brush);

}
#else
void Polygon3d::fill(int,Transform *transform,View3dSurface *surface)
{
  int i,j;

  clip(surface);

  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      if (transform == 0)
        verts[i][j] = surface->modeltoscreen(verts[i][j]);
      else
        verts[i][j] = surface->modeltoscreen(transform->transform(verts[i][j]));

  return surface->fill(verts,nvert,nloops);

}
#endif

#ifdef USING_WIDGETS
void Polygon3d::drawGL(int,Transform *transform,View3dSurface *surface)
{
 int i,j;
 Point p1,p2,p3,p4;

  for (i = 0 ; i < nloops ; i++)
  {
       //if (transform == 0)
       //  p3 = surface->modeltoview(verts[i][0]);
       //else
       //  p3 = surface->modeltoview(transform->transform(verts[i][0]));
       if (transform == 0)
         p3 = verts[i][0];
       else
         p3 = transform->transform(verts[i][0]);

       for (j = 0 ; j < nvert[i] ; j++)
       {
            //if (transform == 0)
            //  p4 = surface->modeltoview(verts[i][(j+1)%nvert[i]]);
            //else
            //  p4 = surface->modeltoview(transform->transform(verts[i][(j+1)%nvert[i]]));
            if (transform == 0)
              p4 = verts[i][(j+1)%nvert[i]];
            else
              p4 = transform->transform(verts[i][(j+1)%nvert[i]]);

            p1 = p3;  p2 = p4;
            //if (surface->clip3d(&p1,&p2))
            //{
                 //p1 = surface->viewtoscreen(p1);
                 //p2 = surface->viewtoscreen(p2);
                 //surface->line(int(p1.x),int(p1.y),int(p2.x),int(p2.y));

                 surface->lineGL(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
            //}
            p3 = p4;
        }
    }
}
#endif

#ifdef USING_WIDGETS
QGraphicsItem * Polygon3d::draw(int, Transform *transform, View3dSurface *surface, bool invisible)
#else
void Polygon3d::draw(int,Transform *transform,View3dSurface *surface)
#endif
{int i,j;
 Point p1,p2,p3,p4;

#ifdef USING_WIDGETS
 QGraphicsItem *qgi = new QGraphicsItemGroup();
 //qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
 QGraphicsItem *qline;
#endif

  for (i = 0 ; i < nloops ; i++)
  {
       if (transform == 0)
         p3 = surface->modeltoview(verts[i][0]);
       else
         p3 = surface->modeltoview(transform->transform(verts[i][0]));
       for (j = 0 ; j < nvert[i] ; j++)
       {
            if (transform == 0)
              p4 = surface->modeltoview(verts[i][(j+1)%nvert[i]]);
            else
              p4 = surface->modeltoview(transform->transform(verts[i][(j+1)%nvert[i]]));
            p1 = p3;  p2 = p4;
            if (surface->clip3d(&p1,&p2))
            {
                 p1 = surface->viewtoscreen(p1);
                 p2 = surface->viewtoscreen(p2);
#ifdef USING_WIDGETS
                 if(invisible)
                 {
                     QPen oldPen = surface->curPen;
                     surface->curPen = Qt::NoPen;
                     qline = surface->line(p1.x,p1.y,p2.x,p2.y);
                     surface->curPen = oldPen;
                 }
                 else
                    qline = surface->line(p1.x,p1.y,p2.x,p2.y);
                 if(qline)
                 {
                     //qline->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                     ((QGraphicsItemGroup*)qgi)->addToGroup(qline);
                     // debug - show start point
                     //QPen oldPen = surface->curPen;
                     //surface->curPen = QPen(QBrush(QColor(255,0,0),Qt::SolidPattern),5,Qt::SolidLine);
                     //drawarrow(p1,p2,(QGraphicsItemGroup*)qgi);
                     //surface->curPen = oldPen;
                     // debug
                 }
#else
                 surface->line(int(p1.x),int(p1.y),int(p2.x),int(p2.y));
#endif
            }
            p3 = p4;
        }
    }
#ifdef USING_WIDGETS
  return qgi;
#endif
}

#ifdef USING_WIDGETS
QGraphicsItem * Polygon3d::addedgestodisplaylist(View3dSurface *vs)
{int i,j;
 Point p1,p2,pt1,pt2;

  QGraphicsLineItem *qgline=0;
  QGraphicsItemGroup *qgrp=0;

  for (i = 0 ; i < nloops ; i++)
    {  for (j = 0 ; j < nvert[i] ; j++)
         {  p1 = verts[i][j];  p2 = verts[i][(j+1)%nvert[i]];
            pt1 = vs->modeltoview(p1);
            pt2 = vs->modeltoview(p2);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return qgrp;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return qgrp;
                 // add the line to the scene as well
                 //QPointF qp1 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt1.x,vs->getheight() - 1.0 - pt1.y);
                 //QPointF qp2 = ((QGraphicsView*)vs->gethwnd())->mapToScene(pt2.x,vs->getheight() - 1.0 - pt2.y);
                 QPointF qp1 = ((RCView*)vs->gethwnd())->view()->mapToScene(pt1.x,pt1.y);
                 QPointF qp2 = ((RCView*)vs->gethwnd())->view()->mapToScene(pt2.x,pt2.y);
                 if(vs->getScene())
                     //qgline = vs->getScene()->addLine(pt1.x,pt1.y,pt2.x,pt2.y);
                     qgline = vs->getScene()->addLine(qp1.x(),qp1.y(),qp2.x(),qp2.y());
                 if(qgline)
                 {
                     if(qgrp == 0)
                         qgrp = new QGraphicsItemGroup();
                     if(qgrp)
                        qgrp->addToGroup(qgline);
                 }
              }
         }
    }

  //if(qgrp)
  //    qgrp->setZValue(QGDRAGZVALUE(QGMAXZVALUE));

  return qgrp;
}

QPainterPath Polygon3d::addpathtodisplaylist(View3dSurface *vs)
{int i,j;
 Point p1,p2,pt1,pt2;

  QPainterPath qppath;
  qppath.setFillRule(Qt::OddEvenFill);

  for (i = 0 ; i < nloops ; i++)
    {  for (j = 0 ; j < nvert[i] ; j++)
         {  p1 = verts[i][j];  p2 = verts[i][(j+1)%nvert[i]];
            pt1 = vs->modeltoview(p1);
            pt2 = vs->modeltoview(p2);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return qppath;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return qppath;
                 // add the line to the scene as well
                 QPointF qp1 = ((RCView*)vs->gethwnd())->view()->mapToScene(pt1.x,pt1.y);
                 QPointF qp2 = ((RCView*)vs->gethwnd())->view()->mapToScene(pt2.x,pt2.y);
                 if(j == 0)
                     qppath.moveTo(qp1);
                 else
                     qppath.lineTo(qp1);
              }
         }
         qppath.closeSubpath();
    }
    qppath.closeSubpath();

  return qppath;
}
#else
void Polygon3d::addedgestodisplaylist(View3dSurface *vs)
{int i,j;
 Point p1,p2,pt1,pt2;
  for (i = 0 ; i < nloops ; i++)
    {  for (j = 0 ; j < nvert[i] ; j++)
         {  p1 = verts[i][j];  p2 = verts[i][(j+1)%nvert[i]];
            pt1 = vs->modeltoview(p1);
            pt2 = vs->modeltoview(p2);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
              }
         }
    }
}
#endif

void Polygon3d::setextents(void)
{int i,j;
  if (nloops == 0 || nvert[0] == 0)
    {  pmin.setxyz(0.0,0.0,0.0);  pmax.setxyz(0.0,0.0,0.0);  return;
    }
  pmin = pmax = verts[0][0];
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      {  if (pmin.x > verts[i][j].x) pmin.x = verts[i][j].x;
         if (pmin.y > verts[i][j].y) pmin.y = verts[i][j].y;
         if (pmin.z > verts[i][j].z) pmin.z = verts[i][j].z;
         if (pmax.x < verts[i][j].x) pmax.x = verts[i][j].x;
         if (pmax.y < verts[i][j].y) pmax.y = verts[i][j].y;
         if (pmax.z < verts[i][j].z) pmax.z = verts[i][j].z;
      }
  pmin.x -= db.getptoler();
  pmin.y -= db.getptoler();
  pmin.z -= db.getptoler();
  pmax.x += db.getptoler();
  pmax.y += db.getptoler();
  pmax.z += db.getptoler();
}

int Polygon3d::checkextents(Point p1,Point p2,int checkz)
{Point min,max;
  if (p1.x < p2.x)
    {  min.x = p1.x;  max.x = p2.x;
    }
  else
    {  min.x = p2.x;  max.x = p1.x;
    }
  if (p1.y < p2.y)
    {  min.y = p1.y;  max.y = p2.y;
    }
  else
    {  min.y = p2.y;  max.y = p1.y;
    }
  if (p1.z < p2.z)
    {  min.z = p1.z;  max.z = p2.z;
    }
  else
    {  min.z = p2.z;  max.z = p1.z;
    }

  return (max.x < pmin.x || min.x > pmax.x ||
          max.y < pmin.y || min.y > pmax.y ||
          max.z < pmin.z || checkz && min.z > pmax.z);
}

int Polygon3d::clip(Point n,double D)
{int i,j,k,nnew,crosses;
 double u1,u2,u3;
 Point p,p3,p4,p5,dir1,*newvert;
  for (i = 0 ; i < nloops ; i++)
    {  if (nvert[i] == 0) continue;
       nnew = 0;  crosses = 0;  newvert = verts[i];
       p3 = verts[i][nvert[i]-1];
       u1 = n.dot(p3) + D;
       for (j = 0 ; j < nvert[i] ; j++)
         {  p4 = verts[i][j];
            u2 = n.dot(p4) + D;
            if (u1 <= 0.0 && u2 >= 0.0 || u1 >= 0.0 && u2 <= 0.0)
              {  if (u1 == 0.0)
                   { // Do nothing if u1 == 0.0
                   }
                 else //if (fabs(u2 - u1) > db.getptoler())
                   {  for (k = j + 1,u3 = u2 ; u3 == 0.0 ; k++)
                        u3 = n.dot(verts[i][k % nvert[i]]) + D;
                      if (u1 < 0.0 && u3 > 0.0 || u1 > 0.0 && u3 < 0.0)
                        {  p = p3 - (p4 - p3) * (u1 / (u2 - u1));
                           if (! crosses)
                             {  crosses = 1;
                                if ((newvert = new Point[nvert[i] * 2]) == NULL) return 0;
                                memcpy(newvert,verts[i],sizeof(Point) * nnew);
                             }
                           newvert[nnew++] = p;
                        }
                   }
              }
            p3 = p4;  u1 = u2;
            if (u1 > 0.0)
              {  if (crosses)
                   newvert[nnew] = p3;
                 nnew++;
              }
         }
       if (nnew == 0)
         {  nloops--;  
            delete [] verts[i];
            if (nloops == 0)
              {  delete [] nvert;
                 delete [] verts;
                 nvert = 0;
                 verts = 0;
              }
            for (j = i ; j < nloops ; j++)
              {  verts[j] = verts[j+1];
                 nvert[j] = nvert[j+1];
              }
            i--;
         }
       else
         {  if (nnew > nvert[i])
              {  delete [] verts[i];
                 if ((verts[i] = new Point[nnew]) == NULL)
                   {  delete [] newvert;  return 0;
                   }
              }
            if (crosses)
              {  memcpy(verts[i],newvert,sizeof(Point) * nnew);
                 nvert[i] = nnew;
                 delete [] newvert;
              }
         }
    }
  return 1;
}

int Polygon3d::clip(View3dSurface *surface)
{ if (surface->getclipplanesvalid() == 0)
    {  defined = nloops > 0;
       return 1;  //  Don't clip planes inside figures - view matrix has had the figure matrix added to it
    }
  if (! clip(surface->clip.n1,surface->clip.d1)) return 0;
  if (! clip(surface->clip.n2,surface->clip.d2)) return 0;
  if (! clip(surface->clip.n3,surface->clip.d3)) return 0;
  if (! clip(surface->clip.n4,surface->clip.d4)) return 0;
  if (! clip(surface->clip.n5,surface->clip.d5)) return 0;
  if (! clip(surface->clip.n6,surface->clip.d6)) return 0;
  defined = nloops > 0;
  return 1;
}

#ifdef USING_WIDGETS
void drawarrow(const Point lastp,const Point p, QGraphicsItemGroup *arrows)
{
    Point leftp,rightp,xaxis(1.0,0.0,0.0),dir;
    double angle = 0.0, alpha = 45.0 / 180.0 * 3.141459265, length;

    dir = lastp - p;
    length = dir.length() / 10.0;
    dir.normalize();
    angle = dir.dot(xaxis);

    ::Point vx = ::Point(1.0,0.0,0.0);
    ::Point va = lastp - p;
    va = va.normalize();
    double shaftAngle = AngleBetween(va,vx,::Point(0.0,0.0,-1.0));
    double leftAngle = shaftAngle - atan(length/length*0.25);
    double rightAngle = shaftAngle + atan(length/length*0.25);


    leftp.x = p.x + length * cos(leftAngle);
    leftp.y = p.y + length * sin(leftAngle);

    rightp.x = p.x + length * cos(rightAngle);
    rightp.y = p.y + length * sin(rightAngle);

    QGraphicsLineItem *line1 = new QGraphicsLineItem(p.x,p.y,leftp.x,leftp.y);
    QGraphicsLineItem *line2 = new QGraphicsLineItem(p.x,p.y,rightp.x,rightp.y);
    arrows->addToGroup(line1);
    arrows->addToGroup(line2);

    //qppath.moveTo(p.x,p.y);
    //qppath.lineTo(leftp.x,leftp.y);
    //qppath.moveTo(p.x,p.y);
    //qppath.lineTo(rightp.x,rightp.y);
}

void drawoutindicator(const Point lastp,const Point p, QGraphicsItemGroup *indicators)
{
    Point leftp,rightp,zaxis(0.0,0.0,1.0),dir,out;
    double angle = 0.0, alpha = 45.0 / 180.0 * 3.141459265, length;

    dir = lastp - p;
    length = dir.length() / 10.0;
    dir.normalize();
    out = dir.cross(zaxis);
    out.normalize();

    rightp = p + out * length;

    QGraphicsLineItem *line1 = new QGraphicsLineItem(p.x,p.y,rightp.x,rightp.y);
    indicators->addToGroup(line1);

}

//QGraphicsItem * Polygon3d::setclipregion(Transform *transform,View3dSurface *surface)
QPainterPath Polygon3d::setclipregion(Transform *transform,View3dSurface *surface)
{
    Point p;
    int i,j;

    Point lastp;
    //QGraphicsPathItem *qgpath = new QGraphicsPathItem();
    //qgpath->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    //qgpath->setFlag(QGraphicsItem::ItemClipsToShape);

    QPainterPath qppath,qallpath;
    qppath.setFillRule(Qt::OddEvenFill);
    //qppath.setFillRule(Qt::WindingFill);

    QGraphicsItemGroup *arrows = new QGraphicsItemGroup();

    int firstdir = getLoopDirection(0);
    //BeginPath(surface->gethdc());
    for (i = 0 ; i < nloops ; i++)
    {
         for (j = 0 ; j < nvert[i] ; j++)
         {
             if(surface != 0)
             {
               if (transform == 0)
                 p = surface->modeltoscreen(verts[i][j]);
               else
                 p = surface->modeltoscreen(transform->transform(verts[i][j]));
               surface->transform2d(&p.x,&p.y);
             }

               if (j == 0)
               {
                   qppath.moveTo(p.x,p.y);
                   //MoveToEx(surface->gethdc(),(int)p.x,(int)p.y,0);
                   lastp = p;
                   //if(j==0)
                   // drawoutindicator(lastp,p,arrows);
               }
               else
               {
                   qppath.lineTo(p.x,p.y);
                   //LineTo(surface->gethdc(),(int)p.x,(int)p.y);
                   //drawarrow(lastp,p,arrows);
                   //drawoutindicator(lastp,p,arrows);
                   lastp = p;
               }
         }
         qppath.closeSubpath();
    }
    qppath.closeSubpath();
    //EndPath(surface->gethdc());

    //qgpath->setPath(qppath);

    surface->getScene()->addItem(arrows);

    //SelectClipPath(surface->gethdc(),RGN_COPY);
    //if (surface->gethwnd() == 0)
    //  IntersectClipRect(surface->gethdc(),(int)surface->originx,(int)surface->originy,(int)(surface->originx+surface->width),(int)(surface->originy+surface->height));

    //return qgpath;
    return qppath;
}
#else
void Polygon3d::setclipregion(Transform *transform,View3dSurface *surface)
{
 Point p;
 int i,j;

  QGraphicsPathItem *qgpath = new QGraphicsPathItem();
  QPainterPath *qppath = new QPainterPath;

  BeginPath(surface->gethdc());
  for (i = 0 ; i < nloops ; i++)
    {  for (j = 0 ; j < nvert[i] ; j++)
         {  if (transform == 0)
              p = surface->modeltoscreen(verts[i][j]);
            else
              p = surface->modeltoscreen(transform->transform(verts[i][j]));
            surface->transform2d(&p.x,&p.y);
            if (j == 0)
              MoveToEx(surface->gethdc(),(int)p.x,(int)p.y,0);
            else
              LineTo(surface->gethdc(),(int)p.x,(int)p.y);
         }
       CloseFigure(surface->gethdc());
    }
  EndPath(surface->gethdc());

  SelectClipPath(surface->gethdc(),RGN_COPY);
  if (surface->gethwnd() == 0)
    IntersectClipRect(surface->gethdc(),(int)surface->originx,(int)surface->originy,(int)(surface->originx+surface->width),(int)(surface->originy+surface->height));

}
#endif

int Polygon3d::hide(View3dSurface *,Point p1,Point p2,Segments *segs)
{double d1,d2,d3,toler;
 Point zaxis,p3,p4,dir,n;
 Segments segs2;

  toler = 0.00001;

  zaxis = xaxis.cross(yaxis);
  d1 = zaxis.dot(p1 - origin);
  d2 = zaxis.dot(p2 - origin);
  d3 = zaxis.z;
  if (d1 * d3 <= 0.0 && d2 * d3 <= 0.0 || fabs(d1) < toler && fabs(d2) < toler)
    return 2;
  else
    {  n.x =  (p2 - p1).y;
       n.y = -(p2 - p1).x;
       n.z =  0.0;
       if (n.length() < toler)
         {  if (d1 * d3 < 0.0)
              {  segs->add(0.0);  segs->add(1.0);
              }
         }
       else
         {  n.normalize();
            p3 = p1;  p3.z = 0.0;
            p4 = p2;  p4.z = 0.0;
            hatchline(0,n,-n.dot(p1),p3,p4 - p3,NULL,segs);
            segs->add(-MAXDOUBLE);  segs->add(MAXDOUBLE);
         }
       if (d1 * d3 < 0.0 && d2 * d3 > 0.0)
         {  segs2.add(0.0);  segs2.add(-d1 / (d2 - d1));
            (*segs) |= segs2;  segs2.destroy();
         }
       else if (d1 * d3 > 0.0 && d2 * d3 < 0.0)
         {  segs2.add(-d1 / (d2 - d1));  segs2.add(1.0);
            (*segs) |= segs2;  segs2.destroy();
         }
    }
  return 1;
}

void Polygon3d::transform(Transform *t)
{int i,j,k,c;
 Point xa,ya,za,o;
 CalculatePlane cp;
 double d,dmin,D1;
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
        verts[i][j] = t->transform(verts[i][j]);
  cp.initialize();
  for (i = 0 ; i < nloops ; i++)
    for (j = 0 ; j < nvert[i] ; j++)
      cp.add(verts[i][j]);
  for (k = 0,defined = 0,dmin = -1.0 ; k < 3 ; k++)
    {  if (cp.planar(k,&o,&xa,&ya,&za,&D1))
         {  for (i = 0,d = 0.0,c = 0 ; i < nloops ; i++)
              for (j = 0 ; j < nvert[i] ; j++,c++)
                 d += cp.check(verts[i][j]);
            d /= double(c);
            if (d < db.getptoler() && (! defined || d < dmin))
              {  defined = 1;  dmin = d;
                 origin = o;  xaxis = xa;  yaxis = ya;
              }
         }
    }
}

double Polygon3d::getarea(void)
{

    // KMJ : debug :
    qDebug() << "polygon3d nloops : " << nloops;
    for (int i = 0 ; i < nloops ; i++)
    {
      qDebug() << "polygon3d nvert[" << i << "] : " << nvert[i];
      for (int j = 0 ; j < nvert[i] ; j++)
      {
          qDebug() << "polygon3d verts[" << i << "][" << j << "] : " << verts[i][j].x << "," << verts[i][j].y << "," << verts[i][j].y;
      }
    }
    // KMJ : end debug

#if 1
    // new version of the area algorithm
    // i don't know why but, the original one below assumes that the first loop is the outer loop
    // which is wrong
    // in the entity the outer loop entities are first in the list and inner loop entities are after that.
    // in polygon3d this is reversed the inner loops are first and the outer loop is last in the list
    // this mease the resulting area was not correct when there is more that one inner loop
    // I assume this was done for use with opengl?? but not sure

    // to fix it i keep the main part of the algorithm but put the resulting areas into a list
    // I then reverse the list and assume the first on is now the outer loop area etc.

    std::list<double> areaList;

    delete [] loopContainments;
    findLoopContainments();

    int i,j;
    Point p1,p2,p3;
    double a,area=0.0,sign;
     for (i = 0 ; i < nloops ; i++)
       {  if (nvert[i] == 0) continue;
          p1 = verts[i][nvert[i]-1] - origin;
          p3.x = xaxis.dot(p1);
          p3.y = yaxis.dot(p1);
          for (j = 0,a = 0.0 ; j < nvert[i] ; j++)
            {  p1 = verts[i][j] - origin;
               p2.x = xaxis.dot(p1);
               p2.y = yaxis.dot(p1);
               a += 0.5 * (p3.x - p2.x) * (p3.y - p2.y) + p2.x * (p3.y - p2.y);
               p3 = p2;
            }
          // sum the areas
          area += fabs(a) * ((loopContainments[i] < 0) ? -1.0 : 1.0);
          //area += fabs(a) * (a < 0.0 ? -1.0 : 1.0);
          // put the areas in the areas list
          //areaList.push_back(a);
          /*
          if (i == 0)
            {  area = fabs(a);  sign = (a < 0.0 ? -1.0 : 1.0);
            }
          else
            area -= fabs(a);
          */
       }
     return area;
     // reverse the areas list
#if 0
     areaList.reverse();
     // sum the areas taking loop containment into account
     std::list<double>::iterator it;
     for (it=areaList.begin(); it!=areaList.end(); ++it)
     {
         a = *it;
         if (it == areaList.begin())
         {
             area = fabs(a);  sign = (a < 0.0 ? -1.0 : 1.0);
         }
         else
           area -= fabs(a);
     }
     return area * sign;
#endif

#else
 int i,j;
 Point p1,p2,p3;
 double a,area,sign;
  for (i = 0 ; i < nloops ; i++)
    {  if (nvert[i] == 0) continue;
       p1 = verts[i][nvert[i]-1] - origin;
       p3.x = xaxis.dot(p1);
       p3.y = yaxis.dot(p1);
       for (j = 0,a = 0.0 ; j < nvert[i] ; j++)
         {  p1 = verts[i][j] - origin;
            p2.x = xaxis.dot(p1);
            p2.y = yaxis.dot(p1);
            a += 0.5 * (p3.x - p2.x) * (p3.y - p2.y) + p2.x * (p3.y - p2.y);
            p3 = p2;
         }
       if (i == 0)
         {  area = fabs(a);  sign = (a < 0.0 ? -1.0 : 1.0);
         }
       else
         area -= fabs(a);
    }
  return area * sign;
#endif
}

Polygon3d::~Polygon3d()
{
  int i;
  if (defined)
  {
      for (i = 0 ; i < nloops ; i++)
         delete [] verts[i];
      delete [] verts;
      delete [] nvert;
#ifdef USING_WIDGETS
      delete [] loopDirections;
      delete [] loopContainments;
#endif
  }
}

int Polygon3d:: addtodisplaylist(View3dSurface *surface,int filled,RCCOLORREF colour)
{ return surface->displaylist.addpolygon3d(origin,xaxis,yaxis,nloops,nvert,verts,filled,colour);
}

int Plane::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int i,result,cstate,fill_style,gradient_fill_style;
 double x,wt;
 RCCHAR *fill_style_l[] = {  _RCT("None"), _RCT("Hatched"), _RCT("Patterned"), _RCT("Filled"), _RCT("Bitmap Filled"), _RCT("Gradient Filled")  };
 RCCHAR *gradient_fill_style_l[] = {  _RCT("Left to Right"),  _RCT("Right to Left"),  _RCT("Top to Bottom"),  _RCT("Bottom to Top"),  _RCT("Left to Centre to Right"), _RCT("Right to Centre to Left"),  _RCT("Top to Centre to Bottom"),  _RCT("Bottom to Centre to Top"), _RCT("Outside to Inside"), _RCT("Inside To Outside")  };

    fill_style = 1;
    if (options.test(PlaneNoFill))
        fill_style = 0;
    else if (options.test(PlanePatterned))
        fill_style = 2;
    else if (options.test(PlaneFilled))
        fill_style = 3;
    else if (options.test(PlaneBitmapFilled))
        fill_style = 4;
    else if (options.test(PlaneGradientFilled))
        fill_style = 5;

    gradient_fill_style = options.test(PlaneGradientStyle1) * 8 + options.test(PlaneGradientStyle2) * 4 +  options.test(PlaneGradientStyle3) * 2 +  options.test(PlaneGradientStyle4); 

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Entity");
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Colour");
        else if (action == PP_CLICK)
        {   result = getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != getcolour())
                Entity::change(change_mask,result,0,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Layer");
        else if (action == PP_CLICK)
        {   result = getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != getlayer())
                Entity::change(change_mask,0,result,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Style");
        else if (action == PP_CLICK)
        {   result = getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != getstyle())
                Entity::change(change_mask,0,0,result,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(getweight()))
                Entity::change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Plane");
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tHatched\tPatterned\tFilled\tBitmap Filled\tGradient Filled");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Fill Style");
        else if (action == PP_GETVALUE)
            strcpy(text,fill_style_l[fill_style]);
        else if (action == PP_SETVALUE)
        {   change_mask.set(4,1);
            for (i = 0 ; i < 6 ; i++)
                if (strcmp(text,fill_style_l[i]) == 0)
                {   new_options.set(PlaneNoFill,i == 0);
                    new_options.set(PlanePatterned,i == 2);
                    new_options.set(PlaneFilled,i == 3);
                    new_options.set(PlaneBitmapFilled,i == 4);
                    new_options.set(PlaneGradientFilled,i == 5);
                    //change(change_mask,*this,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
                }
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {  strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Display Edges");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(PlaneDisplayEdges) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(5,1);
            new_options.set(PlaneDisplayEdges,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
        }
    }
    else if (fill_style == 1)
    {   if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,xspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(9,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Cross Hatch Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,yspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(10,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",angle);
            else if (action == PP_SETVALUE)
            {   change_mask.set(11,1);
                change(change_mask,*this,0.0,0.0,atof(text),0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Cross Hatch Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",crossangle);
            else if (action == PP_SETVALUE)
            {   change_mask.set(12,1); 
                change(change_mask,*this,0.0,0.0,0.0,atof(text),0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    }    
    else if (fill_style == 2)
    {   if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Hor Scale");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",xspacing);
            else if (action == PP_SETVALUE)
            {   change_mask.set(9,1);
                change(change_mask,*this,atof(text),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Ver Scale");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",yspacing);
            else if (action == PP_SETVALUE)
            {   change_mask.set(10,1);
                change(change_mask,*this,0.0,atof(text),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Angle");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",angle);
            else if (action == PP_SETVALUE)
            {   change_mask.set(11,1);
                change(change_mask,*this,0.0,0.0,atof(text),0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
            {Dialog hpdialog("SelectHatchPattern_dialog");
             int i,base;
             RCCHAR filename[300];

                base = 0;
                strcpy(filename,"");
                for (i = 100 ; i < 112 ; i++)
                {   hpdialog.add(new HPDisplayDialogControl(i,fill_style,&base));
                    hpdialog.add(new HPButtonDialogControl(i,fill_style,&base,filename));
                }
                hpdialog.add(new HPButtonDialogControl(112,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(113,fill_style,&base,filename));
                hpdialog.add(new HPListDialogControl(114,fill_style));
                hpdialog.add(new HPButtonDialogControl(115,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(116,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(117,fill_style,&base,filename));
                hpdialog.add(new HPScrollBarDialogControl(118,&base));

                if (hpdialog.process() != FALSE && strlen(filename) > 0)
                {   change_mask.set(6,1);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,filename,new_options);
                }

                return 1;

            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Pattern");
            else if (action == PP_GETVALUE) 
            {   if (patternfilename == 0)
                    strcpy(text,"");
                else if (strrchr(patternfilename,'\\') != 0)
                    strcpy(text,strrchr(patternfilename,'\\')+1);
                else
                    strcpy(text,patternfilename);
                    
                if (strchr(text,'.') != 0)
                    *strchr(text,'.') = 0;

            }
        }
        else if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    }    
    else if (fill_style == 3)
    {   if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Transparency");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",transparency);
            else if (action == PP_SETVALUE)
            {   change_mask.set(13,1);
                //void Plane::change(const BitMask &change,const EntityHeader &header,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
                x = atof(text);
                if (x < 0.0)
                    x = 0.0;
                if (x > 1.0)
                    x = 1.0;
                change(change_mask,*this,0.0,0.0,0.0,0.0,x,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    }    
    else if (fill_style == 4)
    {   if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Bitmap Width");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,xspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(9,1);
                if (e.evaluate(&x))
                   change(change_mask,*this,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Bitmap Height");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,yspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(10,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0) 
        {   if (action == PP_CLICK)
            {Dialog dialog("Selectcolour_Dialog");
             DialogControl *dc;
             double red,green,blue;
                red = GetRValue(gradientcolour) / 255.0;
                green =  GetGValue(gradientcolour) / 255.0;
                blue = GetBValue(gradientcolour) / 255.0;
                dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1101,dc));
                dialog.add(new RTColourScrollBarDialogControl(104,dc));
                dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1102,dc));
                dialog.add(new RTColourScrollBarDialogControl(105,dc));
                dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1103,dc));
                dialog.add(new RTColourScrollBarDialogControl(106,dc));
                dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,0));
                if (dialog.process() == TRUE)
                {   change_mask.set(14); 
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,red,green,blue,0.0,0,0,new_options);
                }
                return 1; 
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Transparent Colour");
            else if (action == PP_GETVALUE) 
            {   sprintf(text,"R%.2lf G%.2lf B%.2lf",red,green,blue);
            }

        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Transparency Range");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",transparencyrange);
            else if (action == PP_SETVALUE)
            {   change_mask.set(15,1);
                change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,atof(text),0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Transparency");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",transparency);
            else if (action == PP_SETVALUE)
            {   change_mask.set(13,1);
                //void Plane::change(const BitMask &change,const EntityHeader &header,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
                x = atof(text);
                if (x < 0.0)
                    x = 0.0;
                if (x > 1.0)
                    x = 1.0;
                change(change_mask,*this,0.0,0.0,0.0,0.0,x,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
            {Dialog hpdialog("SelectHatchPattern_dialog");
             int i,base;
             RCCHAR filename[300];

                base = 0;
                strcpy(filename,"");
                for (i = 100 ; i < 112 ; i++)
                {   hpdialog.add(new HPDisplayDialogControl(i,fill_style,&base));
                    hpdialog.add(new HPButtonDialogControl(i,fill_style,&base,filename));
                }
                hpdialog.add(new HPButtonDialogControl(112,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(113,fill_style,&base,filename));
                hpdialog.add(new HPListDialogControl(114,fill_style));
                hpdialog.add(new HPButtonDialogControl(115,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(116,fill_style,&base,filename));
                hpdialog.add(new HPButtonDialogControl(117,fill_style,&base,filename));
                hpdialog.add(new HPScrollBarDialogControl(118,&base));

                if (hpdialog.process() != FALSE && strlen(filename) > 0)
                {   change_mask.set(6,1);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,filename,new_options);
                }

                return 1;

            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Bitmap");
            else if (action == PP_GETVALUE) 
            {   if (patternfilename == 0)
                    strcpy(text,"");
                else if (strrchr(patternfilename,'\\') != 0)
                    strcpy(text,strrchr(patternfilename,'\\')+1);
                else
                    strcpy(text,patternfilename);
            }
        }
        else if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    } 
    else if (fill_style == 5)
    {   if (id-- == 0)
        {   if (action == PP_CLICK)
            {   strcpy(text,"Left to Right\tRight to Left\tTop to Bottom\tBottom to Top\tLeft to Centre to Right\tRight to Centre to Left\tTop to Centre to Bottom\tBottom to Centre to Top\tOutside to Inside\tInside To Outside");
                return 2;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Gradient Fill Style");
            else if (action == PP_GETVALUE)
                strcpy(text,gradient_fill_style_l[gradient_fill_style]);
            else if (action == PP_SETVALUE)
            {   for (i = 0 ; i < 10 ; i++) 
                    if (strcmp(text,gradient_fill_style_l[i]) == 0)
                    {   change_mask.set(7,1);
                        new_options.set(PlaneGradientStyle1,(i&8)==8);
                        new_options.set(PlaneGradientStyle2,(i&4)==4);
                        new_options.set(PlaneGradientStyle3,(i&2)==2);
                        new_options.set(PlaneGradientStyle4,(i&1)==1);
                        change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
                    }
            }
        }
        else if (id-- == 0) 
        {   if (action == PP_CLICK)
            {Dialog dialog("Selectcolour_Dialog");
             DialogControl *dc;
             double red,green,blue;
                red = GetRValue(gradientcolour) / 255.0;
                green =  GetGValue(gradientcolour) / 255.0;
                blue = GetBValue(gradientcolour) / 255.0;
                dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1101,dc));
                dialog.add(new RTColourScrollBarDialogControl(104,dc));
                dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1102,dc));
                dialog.add(new RTColourScrollBarDialogControl(105,dc));
                dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
                dialog.add(new ScrollBarDialogControl(1103,dc));
                dialog.add(new RTColourScrollBarDialogControl(106,dc));
                dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,0));
                if (dialog.process() == TRUE)
                {   change_mask.set(8);
                    change(change_mask,*this,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(int(red*255.0),int(green*255.0),int(blue*255.0)),0,new_options);
                }
                return 1;
            }
            else if (action == PP_GETLABEL) 
                strcpy(text,"Gradient Fill Colour");
            else if (action == PP_GETVALUE) 
            {   sprintf(text,"R%.2lf G%.2lf B%.2lf",GetRValue(gradientcolour)/255.0,GetGValue(gradientcolour)/255.0,GetBValue(gradientcolour)/255.0);
            }

        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Hor Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,xspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(9,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Ver Spacing");
            else if (action == PP_GETVALUE)
                db.formatnumber(text,yspacing,FormatForEditField);
            else if (action == PP_SETVALUE)
            {Expression e(text);
             double x;
                change_mask.set(10,1);
                if (e.evaluate(&x))
                    change(change_mask,*this,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (id-- == 0)
        {   if (action == PP_CLICK)
                return 1;
            else if (action == PP_GETLABEL) 
                strcpy(text,"Transparency");
            else if (action == PP_GETVALUE)
                sprintf(text,"%.3lf",transparency);
            else if (action == PP_SETVALUE)
            {   change_mask.set(13,1);
                //void Plane::change(const BitMask &change,const EntityHeader &header,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,RCCHAR *filename,const BitMask &o)
                x = atof(text);
                if (x < 0.0)
                    x = 0.0;
                if (x > 1.0)
                    x = 1.0;
                change(change_mask,*this,0.0,0.0,0.0,0.0,x,0.0,0.0,0.0,0.0,0,0,new_options);
            }
        }
        else if (action == PP_GETNUMPROPERTIES)
            return 10000 - id;
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}




