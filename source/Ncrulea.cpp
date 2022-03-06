
#include "ncwin.h"
#include "ruledsurface_dialog.h"

RuledSurface::RuledSurface(LinearEntity *ent1,LinearEntity *ent2,int xm,int ym,const BitMask &o) : options(32)
{ options = o;  xmesh = short(xm);  ymesh = short(ym);
  e1 = ent1;  e2 = ent2;
}

RuledSurface::RuledSurface(const EntityHeader &header,LinearEntity *ent1,LinearEntity *ent2,int xm,int ym,const BitMask &o) :
  UVSurfaceEntity(header),options(32)
{ options = o;  xmesh = short(xm);  ymesh = short(ym);
  e1 = ent1;  e2 = ent2;
}

void RuledSurface::dbinsert(Database *db,int draw)
{ if (db != NULL)
    {  if (e1->getposition() == NULL) db->geometry.add(e1,draw);
       if (e2->getposition() == NULL) db->geometry.add(e2,draw);
    }
  e1->adda(this);
  e2->adda(this);
  Entity::dbinsert(db,draw);

}

void RuledSurface::dbdelete(void)
{ e1->dela(this);
  e2->dela(this);
  Entity::dbdelete();
}

void RuledSurface::dbsetassociated(void)
{ e1->setstatus(1);
  e2->setstatus(1);
}

void RuledSurface::dbtraverseassociated(EntityCallBack c,void *d)
{ c(e1,d);
  c(e2,d);
}

RuledSurface::~RuledSurface()
{ if (e1->getposition() == NULL) delete e1;
  if (e2->getposition() == NULL) delete e2;
}

Point RuledSurface::position(double u,double v) const
{Point p1,p2;
  p1 = e1->position(u);  
  if (options.test(0))
    p2 = e2->position(1.0-u);
  else
    p2 = e2->position(u);
  return p1 + (p2 - p1) * v;
}

Point RuledSurface::normal(double u,double v) const
{Point p1,p2,du,dv,dv1,dv2;
  p1 = e1->position(u);
  p2 = e2->position(options.test(0) ? u = 1.0 - u : u);
  du = p2 - p1;
  dv1 = e1->direction(u);
  if (options.test(0))
    dv2 = -e2->direction(1.0 - u);
  else
    dv2 = e2->direction(u);
  dv = dv1 + (dv2 - dv1) * v;
  return du.cross(dv).normalize();
}

void RuledSurface::nfacets(double error,int *nu,int *nv)
{int n1,n2;
 double cosa,l1,l2;
  n1 = e1->nsegments(error);
  n2 = e2->nsegments(error);
  if (n1 < n2) n1 = n2;
  if (n1 == 1)
    {  l1 = e1->length();  l2 = e2->length();
       if (l1 < l2) l1 = l2;
       l1 /= 2.0;
       if (l1 < db.getptoler() || l1 < error * 2.0)
         *nu = 6;
       else
         {  cosa = (l1 - error) / l1;
            if (cosa > 0.99999) cosa = 0.99999;
            *nu = int(M_PI / acos(cosa));
            if (*nu < 6) *nu = 6;
         }
    }
  else
    *nu = n1;
  *nv = 2;
  *nu = (*nu + xmesh - 1) / (xmesh-1);  *nu *= (xmesh-1);
  if (*nu < 2) *nu = 2;
}

int RuledSurface::facet(const BitMask &foptions,double error,int nu,int nv,View3dSurface *,PolygonCallBack cb,void *data)
{Polygon3d *poly;
 Triangle *triangle;
 UV uv7,uv8,uv9,uv10;
 int i,j,y1;
 double len,t;
 Point p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,normal,n7,n8,n9,n10,du1,du2,du3,du4,du7,du8,du9,du10,dv1,dv2,p[9];
  if (nu == 0 || nv == 0)
    nfacets(error,&nu,&nv);
  p1 = e1->position(0.0);
  p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
  for (i = 1 ; i <= nu ; i++)
    {  t = (double) i / (double) nu;
       p3 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p4 = e2->position(t);
       normal = (p1 - p2).cross(p3 - p2);
       y1 = ymesh;
       if ((len = normal.length()) > db.getptoler())
         {  if (fabs(normal.dot(p4 - p1)) < 1.0E5 * db.getptoler() * len)
              y1 = 2;
         }
       p5 = p2 - p1;  p6 = p4 - p3;
       p7 = p1;  p8 = p3;
       for (j = 1 ; j < y1 ; j++)
         {  t = (double) j / (double) (y1 - 1);
            p9 = p1 + p5 * t;
            p10 = p3 + p6 * t;
            if (foptions.test(0) || foptions.test(2))
              {  uv7.u = double(i-1) / double(nu);
                 uv7.v = double(j-1) / double (y1 - 1);
                 uv8.u = double(i)   / double(nu);
                 uv8.v = double(j-1) / double (y1 - 1);
                 uv9.u = double(i-1) / double(nu);
                 uv9.v = double(j) / double (y1 - 1);
                 uv10.u = double(i) / double(nu);
                 uv10.v = double(j) / double (y1 - 1);
                 dv1 = p5;  dv2 = p6;
                 if (dv1.length() < 1.0E-10) dv1 = dv2;
                 if (dv2.length() < 1.0E-10) dv2 = dv1;
                 dv1.normalize();  dv2.normalize();
                 du1 = e1->direction(uv7.u);
                 du3 = e1->direction(uv8.u);
                 if (options.test(0))
                   {  du2 = -e2->direction(1.0 - uv7.u);
                      du4 = -e2->direction(1.0 - uv8.u);
                   }
                 else
                   {  du2 = e2->direction(uv7.u);
                      du4 = e2->direction(uv8.u);
                  }
                 du7 = du1 + (du2 - du1) * uv7.v;
                 du8 = du3 + (du4 - du3) * uv7.v;
                 du9 = du1 + (du2 - du1) * uv8.v;
                 du10 = du3 + (du4 - du3) * uv8.v;
                 n7 = du7.cross(dv1);
                 n8 = du8.cross(dv2);
                 n9 = du9.cross(dv1);
                 n10 = du10.cross(dv2);
                 if (foptions.test(2))
                   {  p[0] = p7;
                      p[1] = p8;
                      p[2] = p9;
                      p[3] = n7;
                      p[4] = n8;
                      p[5] = n9;
                      p[6] = Point(uv7.u,uv7.v,0.0);
                      p[7] = Point(uv8.u,uv8.v,0.0);
                      p[8] = Point(uv9.u,uv9.v,0.0);
                      if (! cb(this,0,3,p,data)) return 0;

                      p[0] = p10;
                      p[1] = p9;
                      p[2] = p8;
                      p[3] = n10;
                      p[4] = n9;
                      p[5] = n8;
                      p[6] = Point(uv10.u,uv10.v,0.0);
                      p[7] = Point(uv9.u,uv9.v,0.0);
                      p[8] = Point(uv8.u,uv8.v,0.0);
                      if (! cb(this,0,3,p,data)) return 0;

                   }        
                 else
                   {  if ((triangle = new Triangle(0,p7,p8,p9,n7,n8,n9,uv7,uv8,uv9)) != 0)
                        {  if (! triangle->getdefined())
                            delete triangle;
                           else
                             {  if (! cb(this,triangle,0,0,data)) return 0;
                             }
                        }
                      else
                        return 0;
                      if ((triangle = new Triangle(1,p10,p9,p8,n10,n9,n8,uv10,uv9,uv8)) != 0)
                        {  if (! triangle->getdefined())
                             delete triangle;
                           else
                             {  if (! cb(this,triangle,0,0,data)) return 0;
                             }
                        }
                      else
                        return 0;
                   }
              }
            else
              {  poly = new Polygon3d(p7,p8,p10);

//Line *l1,*l2,*l3;
//EntityList list;
//BitMask o(32);
//db.geometry.add(l1 = new Line(p7,p8));
//db.geometry.add(l2 = new Line(p8,p10));
//db.geometry.add(l3 = new Line(p10,p7));
//list.add(l1);  list.add(l2);  list.add(l3);
//db.geometry.add(new Plane(list,1000.0,1000.0,0.0,0.0,0.0,RGB(0,0,0),"",o));

                 if (poly == NULL) return 0;
                 if (! poly->getdefined())
                   delete poly;
                 else
                   {  if (! cb((SurfaceEntity *)this,poly,0,0,data)) return 0;
                   }
                 poly = new Polygon3d(p7,p10,p9);


//EntityList list1;
//db.geometry.add(l1 = new Line(p7,p10));
//db.geometry.add(l2 = new Line(p10,p9));
//db.geometry.add(l3 = new Line(p9,p7));
//list1.add(l1);  list1.add(l2);  list1.add(l3);
//db.geometry.add(new Plane(list1,1000.0,1000.0,0.0,0.0,0.0,RGB(0,0,0),"",o));



                 if (poly == NULL) return 0;
                 if (! poly->getdefined())
                   delete poly;
                 else
                   {  if (! cb((SurfaceEntity *)this,poly,0,0,data)) return 0;
                   }
              }
            p7 = p9;  p8 = p10;
         }
       p1 = p3;  p2 = p4;
    }
  return 1;
}

struct TriangulateData
{int pass;
 int ntriangles;
 Point *triangles;
};

static int triangulatecb(SurfaceEntity *se,void *polygon,int nvert,void *pts,void *cbdata)
{TriangulateData *td = (TriangulateData *)cbdata;
 int i;

  if (nvert == 3)
    {  if (td->pass != 0)
         {  for (i = 0 ; i < 9 ; i++)
              td->triangles[td->ntriangles * 9 + i + 2] = ((Point *)pts)[i];
         }
       td->ntriangles++;
    }

  return 1;
}

void RuledSurface:: triangulate(double error, Transform *)
{BitMask foptions(32);
 int i,nhor,nver;
 TriangulateData td;
 SurfaceProperties sp,*tsp;

  foptions.set(2,1);
  nfacets(error,&nhor,&nver);
  td.pass = 0;
  td.ntriangles = 0;
  td.triangles = 0;
  facet(foptions,error,nhor,nver,0,triangulatecb,&td);

  td.pass = 1;
  triangles = new Point[td.ntriangles * 9 + 2];
  td.triangles = triangles;
  if (sp.copyinfo(this,0,0,0) != 0)
    {  tsp = new SurfaceProperties;
       *tsp = sp;
    }
  else
    tsp = 0;

  //triangles[0].x = tsp != 0 ? (LONG_PTR)tsp->shader + 0.1 : 0.1;
  triangles[0].x = tsp != 0 ? (LONG_PTR)tsp + 0.1 : 0.1;
  triangles[0].y = tsp != 0 ? tsp->transparency : 0.0;
  triangles[0].z = RCDBL_MAX;
  triangles[1].x = db.colourtable[getcolour()].red;
  triangles[1].y = db.colourtable[getcolour()].green;
  triangles[1].z = db.colourtable[getcolour()].blue;
  td.ntriangles = 0;
  facet(foptions,error,nhor,nver,0,triangulatecb,&td);
  ntriangles = td.ntriangles;

  if (sp.tmi.texturetype == 1)
    {  for (i = 0 ; i < ntriangles ; i++)
         {  triangles[i * 9 + 8].x = sp.tmi.x1 + triangles[i * 9 + 8].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 8].y = sp.tmi.y1 + triangles[i * 9 + 8].y * (sp.tmi.y2 - sp.tmi.y1);
            triangles[i * 9 + 9].x = sp.tmi.x1 + triangles[i * 9 + 9].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 9].y = sp.tmi.y1 + triangles[i * 9 + 9].y * (sp.tmi.y2 - sp.tmi.y1);
            triangles[i * 9 + 10].x = sp.tmi.x1 + triangles[i * 9 + 10].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 10].y = sp.tmi.y1 + triangles[i * 9 + 10].y * (sp.tmi.y2 - sp.tmi.y1);
         }
    }


}

double RuledSurface::pick(View3dSurface *surface,double x,double y,Point *pos)
{double t,dmin,d;
 Point p1,p2,p3,p4,p5,p6;
 int i,j,nu,nv;
  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(surface);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    surface = window->getoffscreenbitmap();
 
 if (pos == NULL && surface->displaylist.defined(this))
    return surface->displaylist.pick(this,x,y);
  dmin = -1.0;
  for (i = 0 ; i < xmesh ; i++)
    {  t = (double) i / (double) (xmesh - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);
       if ((d = surface->pick(p1,p2,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
         dmin = d;
    }
  nfacets((surface->getumax()-surface->getumin()) / surface->getwidth() * db.getdtoler(),&nu,&nv);
  p1 = e1->position(0.0);
  p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
  for (i = 1 ; i <= nu ; i++)
    {  t = (double) i / (double) nu;
       p3 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p4 = e2->position(t);
       p5 = p2 - p1;  p6 = p4 - p3;
       for (j = 0 ; j < ymesh ; j++)
         {  t = (double) j / (double) (ymesh - 1);
            if ((d = surface->pick(p1 + p5 * t,p3 + p6 * t,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
              dmin = d;
         }
       p1 = p3;  p2 = p4;
    }
  return dmin;
}

int RuledSurface::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{View3dWindow *window;
 
    window = dynamic_cast<View3dWindow *>(vs);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        vs = window->getoffscreenbitmap();
 
    if (vs->displaylist.defined(this))
        return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
    if (! e1->pick(vs,inside,x1,y1,x2,y2)) return 0;
    if (! e2->pick(vs,inside,x1,y1,x2,y2)) return 0;

    return 1;
}

int RuledSurface::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{double t,dmin;
 Point p1,p2,p3,p4,p5,p6;
 int i,j,nu,nv,picked;
  dmin = -1.0;
  picked = inside != 2;
  for (i = 0 ; i < xmesh ; i++)
    {  t = (double) i / (double) (xmesh - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);
       if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
         return picked;
    }
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  p1 = e1->position(0.0);
  p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
  for (i = 1 ; i <= nu ; i++)
    {  t = (double) i / (double) nu;
       p3 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p4 = e2->position(t);
       p5 = p2 - p1;  p6 = p4 - p3;
       for (j = 0 ; j < ymesh ; j++)
         {  t = (double) j / (double) (ymesh - 1);
            if (vs->pick(nvertices,pwvertices,p1 + p5 * t,p3 + p6 * t,inside,&picked))
              return picked;
         }
       p1 = p3;  p2 = p4;
    }
  return picked;
}

void RuledSurface::addtodisplaylist(View3dSurface *vs,int child)
{int i,j,nu,nv,nedges,drawmode,xm,closedloop;
 double t,x1,x3,x4,x5,x6,y1,y3,y4,y5,y6;
 Point p1,p2,p3,p4,p5,p6,pt1,pt2;
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);

  drawmode = options.test(1) + options.test(2) * 2;
  //  Put edge lines onto the ruled surface
  x3 = y3 = 0.0;
  closedloop = drawmode != HLNormal &&
                 (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                 (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

  for (i = 0,nedges = 0 ; i <= nu + closedloop && drawmode > 0 ; i++)
    {  if (i <= nu)
         t = (double) i / (double) nu;
       else
         t =  1.0 / (double) nu;
       p5 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p6 = e2->position(t);
       x5 = vs->modeltoscreen(p5).x;
       y5 = vs->modeltoscreen(p5).y;
       x6 = vs->modeltoscreen(p6).x;
       y6 = vs->modeltoscreen(p6).y;
       nedges += i >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0);
       nedges += i >= 1 && (((y6-y3)*x5 + (x3-x6)*y5 + (y3-y6)*x3 + (x6-x3)*y3) * ((y6-y3)*x4 + (x3-x6)*y4 + (y3-y6)*x3 + (x6-x3)*y3) >= 0.0);
       p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
       x1 = x3;  x3 = x5;  x4 = x6;
       y1 = y3;  y3 = y5;  y4 = y6;
    }

  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,xmesh * 34 + nedges * 34 + ymesh * nu * 34)) return;
    }

  xm = drawmode == HLNormal || drawmode == HLEdges ? xmesh : 2;
  if (drawmode == HLEdgesOnly || 
      drawmode == HLEdgesBorder &&
       (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
       (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler())
    xm = 0;

  for (i = 0 ; i < xm ; i++)
    {  t = (double) i / (double) (xm - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);
       pt1 = vs->modeltoview(p1);
       pt2 = vs->modeltoview(p2);
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
    }

  //  Put edge lines onto the ruled surface
  closedloop = drawmode != HLNormal &&
                 (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                 (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

  for (i = 0 ; i <= nu + closedloop && drawmode > 0 ; i++)
    {  if (i <= nu)
         t = (double) i / (double) nu;
       else
         t = 1.0 / (double) nu;
       p5 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p6 = e2->position(t);
       x5 = vs->modeltoscreen(p5).x;
       y5 = vs->modeltoscreen(p5).y;
       x6 = vs->modeltoscreen(p6).x;
       y6 = vs->modeltoscreen(p6).y;
       if (i >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
         {  pt1 = vs->modeltoview(p3);
            pt2 = vs->modeltoview(p4);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
              }
         }
       if (i >= 1 && (((y6-y3)*x5 + (x3-x6)*y5 + (y3-y6)*x3 + (x6-x3)*y3) * ((y6-y3)*x4 + (x3-x6)*y4 + (y3-y6)*x3 + (x6-x3)*y3) >= 0.0))
         {  pt1 = vs->modeltoview(p3);
            pt2 = vs->modeltoview(p6);
            if (vs->clip3dfb(&pt1,&pt2))
              {  pt1 = vs->viewtoscreen(pt1);
                 pt2 = vs->viewtoscreen(pt2);
                 if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                 if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
              }
         }
       p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
       x1 = x3;  x3 = x5;  x4 = x6;
       y1 = y3;  y3 = y5;  y4 = y6;
    }

  if (drawmode != HLEdgesOnly)
    {  p1 = e1->position(0.0);
       p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
       for (i = 1 ; i <= nu ; i++)
         {  t = (double) i / (double) nu;
            p3 = e1->position(t);
            if (options.test(0)) t = 1.0 - t;
            p4 = e2->position(t);
            p5 = p2 - p1;  p6 = p4 - p3;
            for (j = 0 ; j < ymesh ; j++)
              {  t = (double) j / (double) (ymesh - 1);
                 pt1 = vs->modeltoview(p1 + p5 * t);
                 pt2 = vs->modeltoview(p3 + p6 * t);
                 if (vs->clip3dfb(&pt1,&pt2))
                   {  pt1 = vs->viewtoscreen(pt1);
                      pt2 = vs->viewtoscreen(pt2);
                      if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                      if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                   }

                 if (drawmode == HLEdgesBorder)
                   {  if (j == 0)
                        j = ymesh - 2;
                      else
                        j = ymesh;
                   }


              }
            p1 = p3;  p2 = p4;
        }
    }
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void RuledSurface::drawGL(int drawmode,Transform *transform,View3dSurface *vs)
{
 int i,j,nu,nv,edgemode,xm,closedloop;
 double t,x1,x3,x4,x5,x6,y1,y3,y4,y5,y6;
 Point p1,p2,p3,p4,p5,p6;

  draw(DM_ERASE,transform,vs);

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

  vs->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

  RCCOLORREF pcol = vs->getcolour();
  //GLfloat fr=GetRValue(pcol)/255.0;
  //GLfloat fg=GetGValue(pcol)/255.0;
  //GLfloat fb=GetBValue(pcol)/255.0;
  //glColor3f(fr,fg,fb);
  glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = vs->curPen.widthF();
  glLineWidth(lwidth<1.0?1.0:lwidth);

/*
  if (transform == NULL)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
*/

  if( vs->getrepaintstyle() == Shaded ||
      vs->getrepaintstyle() == ShadedHidden ||
      vs->getrepaintstyle() == ShadedHiddenDashed ||
      vs->getrepaintstyle() == ShadedTexture ||
      vs->getrepaintstyle() == ShadedHiddenTexture)
  {
      int k,t;
      SurfaceProperties *sp;

      if(isvisible(vs))
      {
          double ambient = ::v.getreal("sh::ambient");
          double diffuse = ::v.getreal("sh::diffuse");

          glEnable(GL_LIGHTING);
          glShadeModel(GL_SMOOTH);

          Transform mt;
          if(transform)
              mt = vs->modeltransform.apply(*transform);
          else
              mt = vs->modeltransform;

         if ( getntriangles() < 0)
         {
             triangulate((vs->getumax() - vs->getumin()) / vs->getwidth() * db.getdtoler());

             ntriangles = getntriangles();
             triangles = gettriangles();

             for (k = 0; k < ntriangles * 9 ; k+= 9)
             {
                 if (triangles[k].z == RCDBL_MAX)
                     k += 2;
                 triangles[k]   = mt.transform(triangles[k]);
                 triangles[k+1] = mt.transform(triangles[k+1]);
                 triangles[k+2] = mt.transform(triangles[k+2]);
             }
         }

         ntriangles = getntriangles();
         triangles = gettriangles();

         for (k = 0,j = 0 ; k < ntriangles ; k++,j += 9)
           {  if (triangles[j].z == RCDBL_MAX)
                {
                   sp=0;
                   if(vs->getrepaintstyle() == ShadedTexture ||
                      vs->getrepaintstyle() == ShadedHiddenTexture)
                        sp = (SurfaceProperties *) (LONG_PTR)triangles[j].x;
                   if (sp != 0 && sp->tmi.texturetype == 1)
                     {  SetMaterial(1.0,1.0,1.0,sp->ambient,sp->diffuse,sp->specular);
                        LoadTexture(sp->tmi.filename,1.0,1.0,1.0,0.004,1.0-triangles[j].y,&t);
                        if (t == -1)
                          {  SetTexture(0);
                             SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                          }
                        else
                          {  SetTexture(t);
                          }
                     }
                   else
                     {  SetTexture(0);
                        SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                     }
                   j += 2;
                }

              if (DrawTexturedTriangles != 0 && i+1 < ntriangles && triangles[j+9].z != RCDBL_MAX)
              {  DrawTexturedTriangles(ntriangles,triangles);
                 break;
              }
              else
                 DrawTexturedTriangle(triangles + j);
           }
         glFinish();
      }

      //EndScene();
  }
  else
  {
      edgemode = options.test(1) + options.test(2) * 2;
      xm = edgemode == HLNormal || edgemode == HLEdges ? xmesh : 2;
      if (edgemode == HLEdgesOnly ||
          edgemode == HLEdgesBorder &&
           (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
           (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler())
        xm = 0;

      for (i = 0 ; i < xm  ; i++)
      {
           t = (double) i / (double) (xm - 1.0);
           p1 = e1->position(t);
           if (options.test(0)) t = 1.0 - t;
           p2 = e2->position(t);

           p1 = vs->modeltransform.transform(p1);
           p2 = vs->modeltransform.transform(p2);

           if (transform == NULL)
             vs->line3dGL(p1,p2);
           else
             vs->line3dGL(transform->transform(p1),transform->transform(p2));
      }
      nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);

      //  Put edge lines onto the ruled surface
      x3 = y3 = 0.0;
      closedloop = edgemode != HLNormal &&
                     (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                     (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

      for (i = 0 ; i <= nu + closedloop && edgemode != HLNormal ; i++)
        {  if (i <= nu)
             t = (double) i / (double) nu;
           else
             t = 1.0 / (double) nu;

           p5 = e1->position(t);
           if (options.test(0)) t = 1.0 - t;
           p6 = e2->position(t);

           p5 = vs->modeltransform.transform(p5);
           p6 = vs->modeltransform.transform(p6);

           if (transform != 0)
             {  p5 = transform->transform(p5);
                p6 = transform->transform(p6);
             }
           x5 = vs->modeltoscreen(p5).x;
           y5 = vs->modeltoscreen(p5).y;
           x6 = vs->modeltoscreen(p6).x;
           y6 = vs->modeltoscreen(p6).y;
           //x5 = p5.x;
           //y5 = p5.y;
           //x6 = p6.x;
           //y6 = p6.y;
           if (i >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
             vs->line3dGL(p3,p4);
           if (i >= 1 && (((y6-y3)*x5 + (x3-x6)*y5 + (y3-y6)*x3 + (x6-x3)*y3) * ((y6-y3)*x4 + (x3-x6)*y4 + (y3-y6)*x3 + (x6-x3)*y3) >= 0.0))
             vs->line3dGL(p3,p6);

           p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
           x1 = x3;  x3 = x5;  x4 = x6;
           y1 = y3;  y3 = y5;  y4 = y6;
        }


      if (edgemode != HLEdgesOnly)
        {  p1 = e1->position(0.0);
           p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
           if (transform != NULL)
             {  p1 = transform->transform(p1);
                p2 = transform->transform(p2);
             }

           p1 = vs->modeltransform.transform(p1);
           p2 = vs->modeltransform.transform(p2);

           for (i = 1 ; i <= nu ; i++)
             {  t = (double) i / (double) nu;
                p3 = e1->position(t);
                if (options.test(0)) t = 1.0 - t;
                p4 = e2->position(t);

                p3 = vs->modeltransform.transform(p3);
                p4 = vs->modeltransform.transform(p4);

                if (transform != NULL)
                  {  p3 = transform->transform(p3);
                     p4 = transform->transform(p4);
                  }
                p5 = p2 - p1;  p6 = p4 - p3;
                for (j = 0 ; j < ymesh ; j++)
                  {  t = (double) j / (double) (ymesh - 1);
                     vs->line3dGL(p1 + p5 * t,p3 + p6 * t);
                     if (edgemode == HLEdgesBorder)
                       {  if (j == 0)
                            j = ymesh - 2;
                          else
                            j = ymesh;
                       }

                  }
                p1 = p3;  p2 = p4;
             }
         }
  }
}
#endif

void RuledSurface::draw(int drawmode,Transform *transform,View3dSurface *vs)
{
 int i,j,nu,nv,edgemode,xm,closedloop;
 double t,x1,x3,x4,x5,x6,y1,y3,y4,y5,y6;
 Point p1,p2,p3,p4,p5,p6;

  QGraphicsItemGroup *qgsurface = new QGraphicsItemGroup();
  QGraphicsItem *qgline=0;

  if(drawmode != DM_PRINT)
  {
      if(qgi && qgi->scene() == vs->getScene())
           vs->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;
  }

  if(drawmode == DM_ERASE)
      return;

  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

#if 0
  if (transform == NULL)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
#endif

  edgemode = options.test(1) + options.test(2) * 2;
  xm = edgemode == HLNormal || edgemode == HLEdges ? xmesh : 2;
  if (edgemode == HLEdgesOnly || 
      edgemode == HLEdgesBorder &&
       (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
       (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler())
    xm = 0;

  for (i = 0 ; i < xm  ; i++)
    {  t = (double) i / (double) (xm - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);

       if (transform == NULL)
         qgline = vs->line3d((QGraphicsItem*)0,p1,p2);
       else
         qgline = vs->line3d((QGraphicsItem*)0,transform->transform(p1),transform->transform(p2));

       if(qgline)
           qgsurface->addToGroup(qgline);
    }
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);

  //  Put edge lines onto the ruled surface
  x3 = y3 = 0.0;
  closedloop = edgemode != HLNormal &&
                 (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                 (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

  for (i = 0 ; i <= nu + closedloop && edgemode != HLNormal ; i++)
    {  if (i <= nu)
         t = (double) i / (double) nu;
       else
         t = 1.0 / (double) nu;

       p5 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p6 = e2->position(t);
       if (transform != 0)
         {  p5 = transform->transform(p5);
            p6 = transform->transform(p6);
         }
       x5 = vs->modeltoscreen(p5).x;
       y5 = vs->modeltoscreen(p5).y;
       x6 = vs->modeltoscreen(p6).x;
       y6 = vs->modeltoscreen(p6).y;
       if (i >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
         qgline = vs->line3d((QGraphicsItem*)0,p3,p4);
       if (i >= 1 && (((y6-y3)*x5 + (x3-x6)*y5 + (y3-y6)*x3 + (x6-x3)*y3) * ((y6-y3)*x4 + (x3-x6)*y4 + (y3-y6)*x3 + (x6-x3)*y3) >= 0.0))
         qgline = vs->line3d((QGraphicsItem*)0,p3,p6);

       if(qgline)
           qgsurface->addToGroup(qgline);

       p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
       x1 = x3;  x3 = x5;  x4 = x6;
       y1 = y3;  y3 = y5;  y4 = y6;
    }


  if (edgemode != HLEdgesOnly)
    {  p1 = e1->position(0.0);
       p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
       if (transform != NULL)
         {  p1 = transform->transform(p1);
            p2 = transform->transform(p2);
         }
       for (i = 1 ; i <= nu ; i++)
         {  t = (double) i / (double) nu;
            p3 = e1->position(t);
            if (options.test(0)) t = 1.0 - t;
            p4 = e2->position(t);
            if (transform != NULL)
              {  p3 = transform->transform(p3);
                 p4 = transform->transform(p4);
              }
            p5 = p2 - p1;  p6 = p4 - p3;
            for (j = 0 ; j < ymesh ; j++)
              {  t = (double) j / (double) (ymesh - 1);
                 qgline = vs->line3d((QGraphicsItem*)0,p1 + p5 * t,p3 + p6 * t);
                 if(qgline)
                     qgsurface->addToGroup(qgline);
                 if (edgemode == HLEdgesBorder)
                   {  if (j == 0)
                        j = ymesh - 2;
                      else
                        j = ymesh;
                   }

              }
            p1 = p3;  p2 = p4;
         }
     }

  if(qgsurface)
  {
      qgi = qgsurface;
      if(selected)
          qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
      else
          qgi->setZValue(zValue);

      if(qgi && dynamic_cast<View3dSubSurface*>(vs) != 0)
          if(((View3dSubSurface*)vs)->qgicp != 0 &&
            ((View3dSubSurface*)vs)->qgicp != qgi->parentItem())
             qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);

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
      else
        if(vs->getScene())
            vs->getScene()->addItem(qgi);
  }
}

#ifdef USING_WIDGETS
int RuledSurface::hideGL(HideImage *image)
{int i,j,nu,nv,ny,drawmode,xm,closedloop;
 double t,x1,x2,x3,x4,x5,x6,x7,x8,x9,y1,y2,y3,y4,y5,y6,y7,y8,y9,len;
 Point p1,p2,p3,p4,p5,p6,p1i,p2i,p3i,p4i,p5i,p6i,p7i,p8i,p9i,d1,d2,d3,normal;
  drawmode = options.test(1) + options.test(2) * 2;
  image->getsurface()->setupGL(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);

  RCCOLORREF pcol;
  pcol = image->getsurface()->getcolour();
  SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
  GLfloat lwidth = image->getsurface()->curPen.widthF();

  xm = drawmode == HLNormal || drawmode == HLEdges ? xmesh : 2;
  if (drawmode == HLEdgesOnly ||
      drawmode == HLEdgesBorder &&
       (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
       (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler())
    xm = 0;

  for (i = 0 ; i < xm ; i++)
    {  t = (double) i / (double) (xm - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);
       image->hideGL(this,p1,p2);
    }
  nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&nu,&nv);

  //  Put edge lines onto the ruled surface
  x3 = y3 = 0.0;
  closedloop = drawmode != HLNormal &&
                 (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                 (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

  for (i = 0 ; i <= nu + closedloop && drawmode > 0 ; i++)
    {  if (i <= nu)
         t = (double) i / (double) nu;
       else
         t = 1.0 / (double) nu;

       p5 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p6 = e2->position(t);

       if (i > 0)
         {  normal = (p1 - p2).cross(p3 - p2);
            ny = ymesh;
            if ((len = normal.length()) > db.getptoler())
              {  if (fabs(normal.dot(p4 - p1)) < 1.0E5 * db.getptoler() * len)
                   ny = 2;
              }

            x2 = 0.0;
            y2 = 0.0;
            x5 = 0.0;
            y5 = 0.0;
            x8 = 0.0;
            y8 = 0.0;

            d1 = p2 - p1;  d2 = p4 - p3;  d3 = p6 - p5;
            for (j = 0 ; j < ny ; j++)
              {  t = (double) j / (double) (ny - 1);
                 p3i = p1 + d1 * t;
                 p6i = p3 + d2 * t;
                 p9i = p5 + d3 * t;

                 x3 = image->getsurface()->modeltoscreen(p3i).x;
                 y3 = image->getsurface()->modeltoscreen(p3i).y;
                 x6 = image->getsurface()->modeltoscreen(p6i).x;
                 y6 = image->getsurface()->modeltoscreen(p6i).y;
                 x9 = image->getsurface()->modeltoscreen(p9i).x;
                 y9 = image->getsurface()->modeltoscreen(p9i).y;

                 if (j > 0 && i > 1 && (((y6-y5)*x9 + (x5-x6)*y9 + (y5-y6)*x5 + (x6-x5)*y5) * ((y6-y5)*x2 + (x5-x6)*y2 + (y5-y6)*x5 + (x6-x5)*y5) >= 0.0))
                   image->hideGL(this,p5i,p6i);

                 if (j > 1 && (((y8-y5)*x4 + (x5-x8)*y4 + (y5-y8)*x5 + (x8-x5)*y5) * ((y8-y5)*x9 + (x5-x8)*y9 + (y5-y8)*x5 + (x8-x5)*y5) >= 0.0))
                   image->hideGL(this,p5i,p8i);

                 if (j > 0 && (((y9-y5)*x6 + (x5-x9)*y6 + (y5-y9)*x5 + (x9-x5)*y5) * ((y9-y5)*x8 + (x5-x9)*y8 + (y5-y9)*x5 + (x9-x5)*y5) >= 0.0))
                   image->hideGL(this,p9i,p5i);

                 p1i = p2i;
                 p4i = p5i;
                 p7i = p8i;
                 p2i = p3i;
                 p5i = p6i;
                 p8i = p9i;

                 x1 = x2;
                 y1 = y2;
                 x4 = x5;
                 y4 = y5;
                 x7 = x8;
                 y7 = y8;

                 x2 = x3;
                 y2 = y3;
                 x5 = x6;
                 y5 = y6;
                 x8 = x9;
                 y8 = y9;

              }

         }

       p1 = p3;  p2 = p4;
       p3 = p5;  p4 = p6;

    }

  if (drawmode  != HLEdgesOnly)
    {  p1 = e1->position(0.0);
       p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
       for (i = 1 ; i <= nu ; i++)
         {  t = (double) i / (double) nu;
            p3 = e1->position(t);
            if (options.test(0)) t = 1.0 - t;
            p4 = e2->position(t);
            p5 = p2 - p1;  p6 = p4 - p3;
            for (j = 0 ; j < ymesh ; j++)
              {  t = (double) j / (double) (ymesh - 1);
                 image->hideGL(this,p1 + p5 * t,p3 + p6 * t);
                 if (drawmode == HLEdgesBorder)
                   {  if (j == 0)
                        j = ymesh - 2;
                      else
                        j = ymesh;
                   }
              }
            p1 = p3;  p2 = p4;
         }
    }

  return 1;
}
#endif

int RuledSurface::hide(HideImage *image)
{int i,j,nu,nv,ny,drawmode,xm,closedloop;
 double t,x1,x2,x3,x4,x5,x6,x7,x8,x9,y1,y2,y3,y4,y5,y6,y7,y8,y9,len;
 Point p1,p2,p3,p4,p5,p6,p1i,p2i,p3i,p4i,p5i,p6i,p7i,p8i,p9i,d1,d2,d3,normal;
  drawmode = options.test(1) + options.test(2) * 2;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  xm = drawmode == HLNormal || drawmode == HLEdges ? xmesh : 2;
  if (drawmode == HLEdgesOnly || 
      drawmode == HLEdgesBorder &&
       (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
       (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler())
    xm = 0;

  for (i = 0 ; i < xm ; i++)
    {  t = (double) i / (double) (xm - 1.0);
       p1 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p2 = e2->position(t);
       image->hide(this,p1,p2);
    }
  nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&nu,&nv);

  //  Put edge lines onto the ruled surface
  x3 = y3 = 0.0;
  closedloop = drawmode != HLNormal &&
                 (e1->position(0.0) - e1->position(1.0)).length() < db.getptoler() &&
                 (e2->position(0.0) - e2->position(1.0)).length() < db.getptoler();

  for (i = 0 ; i <= nu + closedloop && drawmode > 0 ; i++)
    {  if (i <= nu)
         t = (double) i / (double) nu;
       else
         t = 1.0 / (double) nu;

       p5 = e1->position(t);
       if (options.test(0)) t = 1.0 - t;
       p6 = e2->position(t);

       if (i > 0) 
         {  normal = (p1 - p2).cross(p3 - p2);
            ny = ymesh;
            if ((len = normal.length()) > db.getptoler())
              {  if (fabs(normal.dot(p4 - p1)) < 1.0E5 * db.getptoler() * len)
                   ny = 2;
              }

            x2 = 0.0;
            y2 = 0.0;
            x5 = 0.0;
            y5 = 0.0;
            x8 = 0.0;
            y8 = 0.0;

            d1 = p2 - p1;  d2 = p4 - p3;  d3 = p6 - p5;
            for (j = 0 ; j < ny ; j++)
              {  t = (double) j / (double) (ny - 1);
                 p3i = p1 + d1 * t;
                 p6i = p3 + d2 * t;
                 p9i = p5 + d3 * t;

                 x3 = image->getsurface()->modeltoscreen(p3i).x;
                 y3 = image->getsurface()->modeltoscreen(p3i).y;
                 x6 = image->getsurface()->modeltoscreen(p6i).x;
                 y6 = image->getsurface()->modeltoscreen(p6i).y;
                 x9 = image->getsurface()->modeltoscreen(p9i).x;
                 y9 = image->getsurface()->modeltoscreen(p9i).y;

                 if (j > 0 && i > 1 && (((y6-y5)*x9 + (x5-x6)*y9 + (y5-y6)*x5 + (x6-x5)*y5) * ((y6-y5)*x2 + (x5-x6)*y2 + (y5-y6)*x5 + (x6-x5)*y5) >= 0.0))
                   image->hide(this,p5i,p6i);

                 if (j > 1 && (((y8-y5)*x4 + (x5-x8)*y4 + (y5-y8)*x5 + (x8-x5)*y5) * ((y8-y5)*x9 + (x5-x8)*y9 + (y5-y8)*x5 + (x8-x5)*y5) >= 0.0))
                   image->hide(this,p5i,p8i);

                 if (j > 0 && (((y9-y5)*x6 + (x5-x9)*y6 + (y5-y9)*x5 + (x9-x5)*y5) * ((y9-y5)*x8 + (x5-x9)*y8 + (y5-y9)*x5 + (x9-x5)*y5) >= 0.0))
                   image->hide(this,p9i,p5i);

                 p1i = p2i;
                 p4i = p5i;
                 p7i = p8i;
                 p2i = p3i;
                 p5i = p6i;
                 p8i = p9i;

                 x1 = x2;
                 y1 = y2;
                 x4 = x5;
                 y4 = y5;
                 x7 = x8;
                 y7 = y8;

                 x2 = x3;
                 y2 = y3;
                 x5 = x6;
                 y5 = y6;
                 x8 = x9;
                 y8 = y9;

              }

         }

       p1 = p3;  p2 = p4;
       p3 = p5;  p4 = p6;

    }
  
  if (drawmode  != HLEdgesOnly)
    {  p1 = e1->position(0.0);
       p2 = (options.test(0) ? e2->position(1.0) : e2->position(0.0));
       for (i = 1 ; i <= nu ; i++)
         {  t = (double) i / (double) nu;
            p3 = e1->position(t);
            if (options.test(0)) t = 1.0 - t;
            p4 = e2->position(t);
            p5 = p2 - p1;  p6 = p4 - p3;
            for (j = 0 ; j < ymesh ; j++)
              {  t = (double) j / (double) (ymesh - 1);
                 image->hide(this,p1 + p5 * t,p3 + p6 * t);
                 if (drawmode == HLEdgesBorder)
                   {  if (j == 0)
                        j = ymesh - 2;
                      else
                        j = ymesh;
                   }
              }
            p1 = p3;  p2 = p4;
         }
    }

  return 1;
}

Point RuledSurface::nearp(Point)
{Point tmp(0.0,0.0,0.0),normal;
  return tmp;
}

int RuledSurface::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
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
  e1->stretchent(db,t,sp,erase);
  e2->stretchent(db,t,sp,erase);

  stretched = 0;
  if ((e1->getstatus() & ~ 8) != 0 || e1->getcopye() != 0 && (e1->getcopye()->getstatus() & ~8) != 0)
    stretched = 1;
  if ((e2->getstatus() & ~ 8) != 0 || e2->getcopye() != 0 && (e2->getcopye()->getstatus() & ~8) != 0)
    stretched = 1;
  if (! stretched)
    return 1;
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
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  status |= 2 | 16;
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  if (e1->getcopye() != NULL) e1 = (LinearEntity *) e1->getcopye();
  if (e2->getcopye() != NULL) e2 = (LinearEntity *) e2->getcopye();
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int RuledSurface::move(Database *db,Transform &t,int erase)
{Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {  status &= ~1;  //  This entity will not be moved - remove it from the moved list
         return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
      }   
  if (e1->getcopye() == NULL) e1->move(db,t,erase);
  if (e1->getcopye() == NULL)
    {  draw(DM_NORMAL);  return 0;
    }
  if (e2->getcopye() == NULL) e2->move(db,t,erase);
  if (e2->getcopye() == NULL)
    {  draw(DM_NORMAL);  return 0;
    }
  status |= 1;
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  e1 = (LinearEntity *) e1->getcopye();
  e2 = (LinearEntity *) e2->getcopye();
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *RuledSurface::clone(Transform &t)
{ if (copye != NULL) return copye;
  if (e1->getcopye() == NULL) e1->clone(t);
  if (e1->getcopye() == NULL) return NULL;
  if (e2->getcopye() == NULL) e2->clone(t);
  if (e2->getcopye() == NULL) return NULL;
  copye = new RuledSurface(*this,(LinearEntity *) e1->getcopye(),(LinearEntity *)e2->getcopye(),xmesh,ymesh,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

Entity *RuledSurface::deepclone(Transform &t)
{ if (copye != NULL) return copye;
  e1->setcopye(0); e1->clone(t);
  if (e1->getcopye() == 0) return 0;
  e2->setcopye(0); e2->clone(t);
  if (e2->getcopye() == 0) return 0;
  copye = new RuledSurface(*this,(LinearEntity *) e1->getcopye(),(LinearEntity *)e2->getcopye(),xmesh,ymesh,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  return copye;
}

int RuledSurface::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

void RuledSurface::change(const BitMask &change,const EntityHeader &eh,int xm,int ym,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4)) xmesh = short(xm);
  if (change.test(5)) ymesh = short(ym);
  if (change.test(6))
    {  options.set(1,o.test(1));
       options.set(2,o.test(2));
    }
  draw(DM_NORMAL);
}

/*
// defined in dialog.h
class RuledSurfaceDisplayDialogControl : public DisplayDialogControl
  {public:
     RuledSurfaceDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

#ifdef USING_WIDGETS
void RuledSurfaceDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
     ResourceString rs0(NCVIEWA);
     View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
     View3dOutput output(hWnd,hWnd,&top,1.0);
     RECT rect;
     RCHPEN hpen,oldhpen;
     int xm,ym,x1,y1,w,h,edgemode;
     EntityHeader header(0,0,0,0);
     Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
     BitMask options(32);

     if(hWnd == 0)
         return;

     QGraphicsView *ctl = 0;
     if(dialog->gethdlg())
       ctl = (QGraphicsView*)((RuledSurface_Dialog*)dialog->gethdlg())->getControl(107);
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

        //oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
        //rect.left = 0;  rect.top = 0;
        //rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
        //FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
        //MoveToEx(output.gethdc(),0,0,0);
        //LineTo(output.gethdc(),rect.right-1,0);
        //LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
        //LineTo(output.gethdc(),0,rect.bottom-1);
        //LineTo(output.gethdc(),0,0);
        //SelectObject(output.gethdc(),oldhpen);
        //DeleteObject(hpen);

        if (dialog->currentvalue(104,&xm) &&
            dialog->currentvalue(105,&ym) &&
            dialog->currentvalue(106,&edgemode))
        {

            Point p1,p2;
            BitMask options(32);
            options.set(1,edgemode % 2);  options.set(2,edgemode / 2);
            if (xm < 2) xm = 2;  if (ym < 2) ym = 2;
            x1 = (int)output.getumin();
            y1 = (int)output.getvmin();
            w = (int)(output.getumax() - x1);
            h = (int)(output.getvmax() - y1);
            p1.setxyz(x1+w*0.2,y1+h*0.1,0.0);   p2.setxyz(x1+w*0.9,y1+h*0.05,0.0);
            Line *line1 = new Line(p1,p2);
            p1.setxyz(x1+w*0.2,y1+h*0.9,0.0);  p2.setxyz(x1+w*0.95,y1+h*0.7,0.0);
            Line *line2 = new Line(p1,p2);
            if (line1 != NULL && line2 != NULL)
            {
                RuledSurface rs(line1,line2,xm,ym,options);
                rs.draw(DM_NORMAL,NULL,&output);
            }
        }
    }
}
#else
void RuledSurfaceDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 int xm,ym,x1,y1,w,h,edgemode;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);


  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
       //IntersectClipRect(output.hDC,0,0,output.width,output.height);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(100,&xm) &&
           dialog->currentvalue(101,&ym) && 
           dialog->currentvalue(102,&edgemode))
         {Point p1,p2;
          BitMask options(32);
            options.set(1,edgemode % 2);  options.set(2,edgemode / 2);
            if (xm < 2) xm = 2;  if (ym < 2) ym = 2;
            x1 = (int)output.getumin();
            y1 = (int)output.getvmin();
            w = (int)(output.getumax() - x1);
            h = (int)(output.getvmax() - y1);
            p1.setxyz(x1+w*0.2,y1+h*0.1,0.0);   p2.setxyz(x1+w*0.9,y1+h*0.05,0.0);
            Line *line1 = new Line(p1,p2);
            p1.setxyz(x1+w*0.2,y1+h*0.9,0.0);  p2.setxyz(x1+w*0.95,y1+h*0.7,0.0);
            Line *line2 = new Line(p1,p2);
            if (line1 != NULL && line2 != NULL)
              {  RuledSurface rs(line1,line2,xm,ym,options);
                 rs.draw(DM_NORMAL,NULL,&output);
              }
         }
    }
}
#endif

class RuledSurfaceVerifyButtonControl : public ButtonDialogControl
  {public:
     RuledSurfaceVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int RuledSurfaceVerifyButtonControl::select(Dialog *dialog)
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
    return 0;
}

int RuledSurface::setup(BitMask *change,int *xm,int *ym,BitMask *options,EntityHeader *inheader)
{
 int status,drawmode,dummydrawmode;
 Dialog dialog("RuledSurface_Dialog");
 //static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
 RCSTRING STRdrawmodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
 RCSTRING STRdrawmodelv[] = { "*Varies*", "Normal", "Edges","Edges Only","Edges and Border" };
 RCCHAR dm1[25],dm2[25],dm3[25],dm4[25];
 RCCHAR dmv1[25],dmv2[25],dmv3[25],dmv4[25],dmv5[25];
 RCCHAR *drawmodel[] = { dm1,dm2,dm3,dm4 };
 RCCHAR *drawmodelv[] = { dmv1,dmv2,dmv3,dmv4,dmv5 };
 DialogControl *dc;
 ResourceString rs0(NCRULEA),rsVaries(4107);
 //
 int i,c,l,s,defColour,defLayer,defStyle,defxm,defym;
 double wt,defWeight;
 BitMask defOptions(32),changeIn(32);

 for(i=0; i<4; i++)
     strcpy(drawmodel[i],&STRdrawmodel[i]);

 for(i=0; i<5; i++)
     strcpy(drawmodelv[i],&STRdrawmodelv[i]);

 EntityHeader entheader;
 if(inheader != 0)
     inheader->verify(&dialog,&c,&l,&s,&wt);
 else
     entheader.verify(&dialog,&c,&l,&s,&wt);

 int changecount = v.getinteger("op::changecount");

 drawmode = options->test(1) + options->test(2) * 2;

 defColour = db.header.getcolour();
 defLayer  = db.header.getlayer();
 defStyle  = db.header.getstyle();
 defWeight  = db.lineweights.weighttomm(db.header.getweight());
 RuledSurface::getdefaults(&defxm,&defym,&defOptions);

 dialog.add(dc = new IntegerDialogControl(104,xm,2,2000));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 dialog.add(dc = new IntegerDialogControl(105,ym,2,2000));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(new ListDialogControl(106,4,drawmodel,&drawmode));
 dialog.add((DisplayDialogControl *) new RuledSurfaceDisplayDialogControl(107));
 if(changecount == 1)
 {
     dialog.add(new ButtonDialogControl(108)); // surface properties
     dialog.add(new ButtonDialogControl(109)); // triangulate
 }
 dialog.add(new ButtonDialogControl(110)); // clone (104->110)
 dialog.add(new ButtonDialogControl(111)); // clone all (105->111)

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
         wt = 0.10;

      // x mesh
     if(!change->test(4))
         *xm = 2;

      // y mesh
     if(!change->test(5))
         *ym = 2;

     // draw mode
     if(!change->test(6))
         drawmode = defOptions.test(1) + defOptions.test(2) * 2;
 }
 else
 {
     //drawmode = options->test(1) + options->test(2) * 2;
 }

 if (change != NULL)
 {
     if(changecount == 1)
        dialog.title(_RCT("Ruled Surface Verify"));
     else
      dialog.title(rs0.gets());

     v.setinteger("op::change",1);
     v.setbm("op::changemask",*change);
     // debug
     //change->setall();
     //change->clearall();

      //dialog.add(new CheckBoxDialogControl(200,change,4));
      //dialog.add(new CheckBoxDialogControl(201,change,5));
      //dialog.add(new CheckBoxDialogControl(202,change,6));

     if(changecount != 0)
     {
         dialog.add(new RuledSurfaceVerifyButtonControl(1000));
         dialog.add(new RuledSurfaceVerifyButtonControl(1001));
         dialog.add(new RuledSurfaceVerifyButtonControl(1002));
         dialog.add(new RuledSurfaceVerifyButtonControl(1003));
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

     // x mesh
     if(changecount > 1 && !change->test(4))
     {
         ((IntegerDialogControl*)dialog.getcontrol(104))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(104))->setDefaultValue(defxm);
     }
     // y mesh
     if(changecount > 1 && !change->test(5))
     {
         ((IntegerDialogControl*)dialog.getcontrol(105))->setSpecialValueText(_RCT("*Varies*"));
         ((IntegerDialogControl*)dialog.getcontrol(105))->setDefaultValue(defym);
     }
     // draw mode
     if(changecount > 1 && !change->test(6))
         dialog.add(new ListDialogControl(1006,5,drawmodelv,&dummydrawmode));

 }
 else
 {
     v.setinteger("op::change",0);
     v.setbm("op::changemask",defOptions);

     c = defColour;
     l = defLayer;
     s = defStyle;
     wt = defWeight;
 }

 status = dialog.process();
 if (*xm < 2) *xm = 2;  if (*ym < 2) *ym = 2;
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

             inheader->setcolour(c);
             inheader->setstyle(s);
             inheader->setlayer(l);
             inheader->setweight(db.lineweights.mmtoweight(wt));
         }

         if( changecount > 1 && !changeIn.test(0) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(100))->getSpecialValueText()) != 0 )
             change->clear(0);
         else
         {
             change->set(0);
             inheader->setcolour(c);
         }
         if( changecount > 1 && !changeIn.test(1) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(101))->getSpecialValueText()) != 0 )
             change->clear(1);
         else
         {
             change->set(1);
             inheader->setlayer(l);
         }
         if( changecount > 1 && !changeIn.test(2) &&
             strlen(((IntegerDialogControl*)dialog.getcontrol(102))->getSpecialValueText()) != 0 )
             change->clear(2);
         else
         {
             change->set(2);
             inheader->setstyle(s);
         }
         if( changecount > 1 && !changeIn.test(3) &&
             strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
             change->clear(3);
         else
         {
             change->set(3);
             inheader->setweight(db.lineweights.mmtoweight(wt));
         }

         // x mesh
         if(changecount > 1 && !changeIn.test(4) &&
            strlen(((IntegerDialogControl*)dialog.getcontrol(104))->getSpecialValueText()) != 0)
             change->clear(4);
         else
             change->set(4);

         // y mesh
         if(changecount > 1 && !changeIn.test(5) &&
            strlen(((IntegerDialogControl*)dialog.getcontrol(105))->getSpecialValueText()) != 0)
             change->clear(5);
         else
             change->set(5);

         // draw mode

         if(changecount > 1 && !changeIn.test(6) && dummydrawmode == 0 )
             change->clear(6);
         else
         {
             change->set(6);
             options->set(1,drawmode % 2);  options->set(2,drawmode / 2);
         }

     }
     else
     {
         options->set(1,drawmode % 2);  options->set(2,drawmode / 2);
     }
 }

 return status;
}

void RuledSurface::getdefaults(int *xm,int *ym,BitMask *o)
{ *o = *v.getbm("rs::options");
  *xm = v.getinteger("rs::xmesh");
  *ym = v.getinteger("rs::ymesh");
}

void RuledSurface::setdefaults(int xm,int ym,const BitMask &o)
{ v.setbm("rs::options",o);
  v.setinteger("rs::xmesh",xm);
  v.setinteger("rs::ymesh",ym);
}

void RuledSurface::setdefaults(void)
{ v.setbm("rs::options",options);
  v.setinteger("rs::xmesh",xmesh);
  v.setinteger("rs::ymesh",ymesh);
}

int RuledSurface::save(CadFile *outfile)
{short type;
  type = ruledsurface_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! outfile->write((const Entity **)&e1)) return 0;
  if (! outfile->write((const Entity **)&e2)) return 0;
  if (! e1->saveunknowngeometry(outfile)) return 0;
  if (! e2->saveunknowngeometry(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&xmesh)) return 0;
  if (! outfile->write(&ymesh)) return 0;
  return 1;
}

Entity *RuledSurface::load(int reference,CadFile *infile)
{RuledSurface *ruledsurface;
 EntityHeader *header;
 Entity *e1,*e2,*ejunk;
 BitMask options(32);
 short xmesh,ymesh;
  if ((header = loadheader(infile)) == NULL) return NULL;
  if (! infile->read((Entity **)&e1)) return 0;
  if (! infile->read((Entity **)&e2)) return 0;
  if (reference)
    {  if (! Entity::loadunknowngeometry(1,infile,&ejunk)) return 0;
       if (! Entity::loadunknowngeometry(1,infile,&ejunk)) return 0;
    }
  else
    {  if (! Entity::loadunknowngeometry(0,infile,&e1)) return 0;
       if (! Entity::loadunknowngeometry(0,infile,&e2)) return 0;
    }
  if (! infile->read(&options)) return 0;
  if (! infile->read(&xmesh)) return 0;
  if (! infile->read(&ymesh)) return 0;
  ruledsurface = new RuledSurface(*header,(LinearEntity *) e1,(LinearEntity *)e2,xmesh,ymesh,options);
  ruledsurface->atlist = header->getatlist();
  delete header;
  return ruledsurface;
}

int RuledSurface::save(UndoFile *outfile)
{short type;
 long v1 = e1->getid();
 long v2 = e2->getid();
  type = ruledsurface_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&v1)) return 0;
  if (! outfile->write(&v2)) return 0;
  return 1;
}

Entity *RuledSurface::load(UndoFile *infile)
{RuledSurface *rs;
 char bm[sizeof(BitMask)];
  if ((rs = new RuledSurface) != NULL)
    {AttributeList al;
       memcpy(bm,&rs->visible,sizeof(bm));
       if (! infile->blockread(rs,sizeof(*rs))) return 0;
       memcpy(&rs->visible,bm,sizeof(bm));
       if (! infile->read(&rs->visible)) return 0;
       if (! al.load(infile)) return 0;   rs->atlist = al;
       if (! infile->read((long *)&rs->e1)) return 0;
       if (! infile->read((long *)&rs->e2)) return 0;
    }
  return rs;
}

int RuledSurface::saveattributes(CadFile *outfile)
{ outfile->write(&xmesh);
  outfile->write(&ymesh);
  outfile->write(&options);
  return 1;
}

int RuledSurface::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  infile->read(&xmesh);
  infile->read(&ymesh);
  infile->read(&options);
  return 1;
}

int RuledSurface::savedxf(int blocksection,DXFOut *dxffile)
{int nu,nv;
 Point p1,p2,p3;
 int i,j;
 double t;
  if (! blocksection)
    {  nu = 10;  nv = 10;
       if (! dxffile->writestring(0, (char*)"POLYLINE")) return 0;
       if (! dxffile->writeheader(this)) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbPolygonMesh")) return 0;
       if (! dxffile->writelong(66,1)) return 0;
       if (! dxffile->writereal(10,0.0)) return 0;
       if (! dxffile->writereal(20,0.0)) return 0;
       if (! dxffile->writereal(30,0.0)) return 0;
       if (! dxffile->writelong(70,16)) return 0;
       if (! dxffile->writelong(71,nu)) return 0;
       if (! dxffile->writelong(72,nv)) return 0;
       for (i = 0 ; i < nu ; i++)
         {  t = (double) i / (double) (nu - 1);
            p1 = e1->position(t);
            if (options.test(0)) t = 1.0 - t;
            p2 = e2->position(t);
            for (j = 0 ; j < nv ; j++)
              {  t = (double) j / (double) (nv - 1);
                 p3 = p1 + (p2 - p1) * t;
                 if (! dxffile->writestring(0, (char*)"VERTEX")) return 0;
                 if (! dxffile->writehandle()) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
                 if (! dxffile->writelong(8,getlayer())) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbVertex")) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbPolygonMeshVertex")) return 0;
                 if (! dxffile->writereal(10,p3.x * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(20,p3.y * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(30,p3.z * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(70,64)) return 0;
              }
         }
       if (! dxffile->writestring(0, (char*)"SEQEND")) return 0;
       if (! dxffile->writehandle()) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
    }
  return 1;
}

int RuledSurface::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int RuledSurface::savegeometry(CadFile *outfile)
{long v1 = e1->getid();
 long v2 = e2->getid();
  outfile->write(&v1);
  outfile->write(&v2);
  return 1;
}

int RuledSurface::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  infile->read((long *)&e1);
  infile->read((long *)&e2);
  return 1;
}

void RuledSurface::fixupreferences(Entity ** index)
{ e1 = (LinearEntity *) index[long(e1)];
  e2 = (LinearEntity *) index[long(e2)];
  Entity::fixupreferences(index);
}

/**
 * @brief RuledSurface::verify
 *
 * Entry point for the Ruled Surface Verify Dialog
 *
 * KMJ : working in qt version
 *
 */
void RuledSurface::verify(void)
{
#if 0
    // this function now just pops up the options dialog
    state.sendevent(new NewCommandEvent(410,0));
#else
     //static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
     RCSTRING STRedgemodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
     RCCHAR em1[25],em2[25],em3[25],em4[25];
     RCCHAR *edgemodel[] = { em1,em2,em3,em4 };
     Dialog dialog("Ruledsurfaceverify_Dialog");
     //RCCHAR drawmode[300];
     int c,l,s,status,edgemode;
     double w;
     Entity *e;

     for(int i=0; i<4; i++)
         strcpy(edgemodel[i],&STRedgemodel[i]);

     edgemode = options.test(1) + options.test(2) * 2;

    EntityHeader::verify(&dialog,&c,&l,&s,&w);
    dialog.add(new RuledSurfaceVerifyButtonControl(1000));
    dialog.add(new RuledSurfaceVerifyButtonControl(1001));
    dialog.add(new RuledSurfaceVerifyButtonControl(1002));
    dialog.add(new RuledSurfaceVerifyButtonControl(1003));

    dialog.add(new IntegerDialogControl(104,&xmesh));
    dialog.add(new IntegerDialogControl(105,&ymesh));
    //dialog.add(new StringDialogControl(106,drawmode,300));
    dialog.add(new ListDialogControl(106,4,edgemodel,&edgemode));
    dialog.add(new ButtonDialogControl(107)); // surface properties
    //dialog.add(new ButtonDialogControl(108)); // status
    dialog.add(new ButtonDialogControl(109)); // triangulate
    //strcpy(drawmode,drawmodel[options.test(1) + options.test(2) * 2]);

    dialog.setposition(DPOK);
    status = dialog.process();
    if(status == TRUE) // ok button
    {
        db.saveundo(UD_CHANGEATTRIBUTES,this);
        draw(DM_ERASE);
        cadwindow->invalidatedisplaylist(this);

        // OK button
        setcolour(c);
        setstyle(s);
        setlayer(l);
        setweight(db.lineweights.mmtoweight(w));

        db.saveundo(UD_STARTBLOCK,NULL);

        options.set(1,edgemode % 2);
        options.set(2,edgemode / 2);

        db.saveundo(UD_ENDBLOCK,NULL);

        draw(DM_NORMAL);

    }
    if (status == 108) // to setup dialog
        ::state.sendevent(new NewCommandEvent(410,0));
    else if (status == 109) // triangulate
    {
        state.destroyselection(1);
        if ((e = triangulatedclone(1)) != 0)
        {   db.saveundo(UD_STARTBLOCK,NULL);
            db.geometry.add(e);
            db.geometry.del(this);
            db.saveundo(UD_ENDBLOCK,NULL);
        }
    }  
    else if (status == 107)// change surface properties
    {
        SurfaceProperties sp;
        sp.verify(this,0);
    }
#endif
}

void RuledSurface::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{Point pmin1,pmax1;
  e1->extents(surface,pmin,pmax);
  e2->extents(surface,&pmin1,&pmax1);
  if (pmin1.x < pmin->x) pmin->x = pmin1.x;
  if (pmin1.y < pmin->y) pmin->y = pmin1.y;
  if (pmin1.z < pmin->z) pmin->z = pmin1.z;
  if (pmax1.x > pmax->x) pmax->x = pmax1.x;
  if (pmax1.y > pmax->y) pmax->y = pmax1.y;
  if (pmax1.z > pmax->z) pmax->z = pmax1.z;
}

int RuledSurface::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(plane_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int RuledSurface::explode(void)
{ return 0;
}

int RuledSurface::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int i,xm,ym,result,cstate;
 double wt;
 static const char *drawmodel[] = {  "Normal", "Edges", "Edges Only", "Edges and Border"  };



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
        else if (id == 8)
        {   strcpy(text,"Normal\tEdges\tEdges Only\tEdges and Border");
            return 2;
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
            sprintf(text,"%d",xmesh);
        else if (id == 7)
            sprintf(text,"%d",ymesh);
        else if (id == 8)
            strcpy(text,drawmodel[options.test(1) + options.test(2) * 2]);
        else if (id == 9)
        {   db.formatnumber(text,0.0,0);
            return 1;
        }
    }
    else if (action == PP_SETVALUE)
    {   if (id == 6)
        {   change_mask.set(4,1);
            xm = atoi(text);
            if (xm < 2)
                xm = 2;
            change(change_mask,*this,xm,0,new_options);
        }
        else if (id == 7)
        {   change_mask.set(5,1);
            ym = atoi(text);
            if (ym < 2)
                ym = 2;
            change(change_mask,*this,0,ym,new_options);
        }
        else if (id == 8)
        {   change_mask.set(6,1);
            for (i = 0 ; i < 4 ; i++)
                if (strcmp(text,drawmodel[i]) == 0)
                {   new_options.set(1,i&1);
                    new_options.set(2,i&2);
                    change(change_mask,*this,0,0,new_options);
                }
        }
    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Ruled Surface");
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
                strcpy(text,"Horizontal Mesh");
            else if (id == 7)
                strcpy(text,"Vertical Mesh");
            else if (id == 8)
                strcpy(text,"Drawing Mode");
            else if (id == 9)
                strcpy(text,"Area");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 9;
    
    return 0;
}

