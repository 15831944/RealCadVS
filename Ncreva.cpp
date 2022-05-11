
#include "ncwin.h"
#include "surfaceofrev_dialog.h"

SurfaceOfRev::SurfaceOfRev(Point org,Point ax,double a,double dz,LinearEntity *ent,int xm,int ym,const BitMask &o) : options(32)
{ origin = org;  axis = ax;  angle = a;  deltaz = dz;
  e = ent;  options = o;  xmesh = short(xm);  ymesh = short(ym);
}

SurfaceOfRev::SurfaceOfRev(const EntityHeader &header,Point org,Point ax,double a,double dz,LinearEntity *ent,int xm,int ym,const BitMask &o) :
  UVSurfaceEntity(header),options(32)
{ origin = org;  axis = ax;  angle = a;  deltaz = dz;
  e = ent;  options = o;  xmesh = short(xm);  ymesh = short(ym);
}

SurfaceOfRev::~SurfaceOfRev()
{ if (e->getposition() == NULL) delete e;
}

void SurfaceOfRev::dbinsert(Database *db,int draw)
{ if (db != NULL && e->getposition() == NULL) db->geometry.add(e,draw);
  e->adda(this);
  Entity::dbinsert(db,draw);
}

void SurfaceOfRev::dbdelete(void)
{ e->dela(this);
  Entity::dbdelete();
}

void SurfaceOfRev::dbsetassociated(void)
{ e->setstatus(1);
}

void SurfaceOfRev::dbtraverseassociated(EntityCallBack c,void *d)
{ c(e,d);
}

Point SurfaceOfRev::position(double u,double v) const
{Transform t;
  t.rotate(origin,axis,angle * v * M_PI / 180.0);
  return t.transform(e->position(u));
}

Point SurfaceOfRev::normal(double u,double v) const
{Transform t;
 Point p1,p2,du,av;
  p1 = e->position(u);
  p2 = p1 + e->direction(u);
  t.rotate(origin,axis,angle * v * M_PI / 180.0);
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  du = (p2 - p1);
  av = p1 - origin - axis * axis.dot(p1 - origin);
  if (av.length() < 1.0E-10 && u < 0.99)
    {  p1 = t.transform(e->position(u+0.001));
       av = p1 - origin - axis * axis.dot(p1 - origin);
    }
  if (av.length() < 1.0E-10 && u > 0.01)
    {  p1 = t.transform(e->position(u-0.001));
       av = p1 - origin - axis * axis.dot(p1 - origin);
    }
  return av.cross(axis).cross(du).normalize();
}

void SurfaceOfRev::nfacets(double error,int *nu,int *nv)
{int i;
 double r,maxr,cosa;
 Point p1,p2;
  *nu = e->nsegments(error);
  maxr = 0.0;
  for (i = 0 ; i <= *nu ; i++)
    {  p1 = e->position(double(i) / double(*nu));
       p2 = origin + axis * axis.dot(p1 - origin);
       r = (p2 - p1).length();
       if (i == 0 || r > maxr) maxr = r;
    }
  if (maxr < db.getptoler() || maxr < error * 2.0)
    *nv = 6;
  else
    {  cosa = (maxr - error) / maxr;
       if (cosa > 0.99999) cosa = 0.99999;
       *nv = int(M_PI / acos(cosa) * fabs(angle) / 360.0);
       if (*nv < 6) *nv = 6;
    }
  *nu = (*nu + xmesh - 1) / (xmesh-1);  *nu *= (xmesh-1);  if (*nu < 4) *nu = 4;
  *nv = (*nv + ymesh - 1) / (ymesh-1);  *nv *= (ymesh-1);  if (*nv < 4) *nv = 4;
}

int SurfaceOfRev::facet(const BitMask &foptions,double error,int nu,int nv,View3dSurface *,PolygonCallBack cb,void *data)
{Polygon3d *poly;
 int i,j;
 Transform tr;
 Triangle *triangle;
 UV uv1,uv2,uv3,uv4;
 Point ax1,p1,p2,p3,p4,p[9],*verts,*normals,du1,du2,du3,du4,dv1,dv2,av1,av2,av3,av4,n1,n2,n3,n4;
 double dz;

  if (nu == 0 || nv == 0) 
    nfacets(error,&nu,&nv);
  if ((verts = new Point[nu+1]) == NULL) return 0;
  if (foptions.test(0) || foptions.test(2))
    {  if ((normals = new Point[nu+1]) == 0)
         {  delete [] verts;  return 0;
         }
    }
  else
    normals = 0;
  for (i = 0 ; i <= nu ; i++)
    {  verts[i] = e->position(double(i)/double(nu));
       if (normals != 0)
         normals[i] = verts[i] + e->direction(double(i)/double(nu));
    }
  dz = deltaz / double(nv);
  ax1 = axis * dz;
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));
  for (j = 0 ; j < nv ; j++)
    {  p1 = verts[0];  p2 = tr.transform(verts[0]) + ax1;  verts[0] = p2;
       if (normals != 0)
         {  du1 = normals[0] - p1;
            normals[0] = tr.transform(normals[0]) + ax1;
            du2 = normals[0] - p2;
         }
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i];  p4 = tr.transform(verts[i]) + ax1;  verts[i] = p4;
            if (foptions.test(0) || foptions.test(2))
              {  du3 = normals[i] - p3;
                 normals[i] = tr.transform(normals[i]) + ax1;
                 du4 = normals[i] - p4;
                 av1 = p1 - origin - axis * axis.dot(p1 - origin);
                 av2 = p2 - origin - axis * axis.dot(p2 - origin);
                 av3 = p3 - origin - axis * axis.dot(p3 - origin);
                 av4 = p4 - origin - axis * axis.dot(p4 - origin);
                 if (av1.length() < 1.0E-5)
                   av1 = av3;
                 if (av3.length() < 1.0E-5)
                   av3 = av1;
                 if (av2.length() < 1.0E-5)
                   av2 = av4;
                 if (av4.length() < 1.0E-5)
                   av4 = av2;
                 n1 = av1.cross(axis).cross(du1).normalize();
                 n2 = av2.cross(axis).cross(du2).normalize();
                 n3 = av3.cross(axis).cross(du3).normalize();
                 n4 = av4.cross(axis).cross(du4).normalize();
                 uv1.u = double(i-1) / double(nu);
                 uv1.v = double(j) / double (nv);
                 uv2.u = double(i-1) / double(nu);
                 uv2.v = double(j+1) / double (nv);
                 uv3.u = double(i) / double(nu);
                 uv3.v = double(j) / double (nv);
                 uv4.u = double(i) / double(nu);
                 uv4.v = double(j+1) / double (nv);
                 if (foptions.test(2))
                   {  
                      p[2] = p1;
                      p[1] = p2;
                      p[0] = p3;
                      p[5] = n1;
                      p[4] = n2;
                      p[3] = n3;
                      p[8] = Point(uv1.u,uv1.v,0.0);
                      p[7] = Point(uv2.u,uv2.v,0.0);
                      p[6] = Point(uv3.u,uv3.v,0.0);
                      cb(this,0,3,p,data);

                      p[2] = p4;
                      p[1] = p3;
                      p[0] = p2;
                      p[5] = n4;
                      p[4] = n3;
                      p[3] = n2;
                      p[8] = Point(uv4.u,uv4.v,0.0);
                      p[7] = Point(uv3.u,uv3.v,0.0);
                      p[6] = Point(uv2.u,uv2.v,0.0);
                      cb(this,0,3,p,data);

                   }
                 else
                   {  if ((triangle = new Triangle(0,p1,p2,p3,n1,n2,n3,uv1,uv2,uv3)) != 0)
                        {  if (! triangle->getdefined())
                             delete triangle;
                           else
                             {  if (! cb(this,triangle,0,0,data)) return 0;
                             }
                        }
                      else
                        return 0;
                      if ((triangle = new Triangle(1,p4,p3,p2,n4,n3,n2,uv4,uv3,uv2)) != 0)
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
              {  poly = new Polygon3d(p1,p2,p3);
                 if (poly == NULL) return 0;
                 if (! poly->getdefined())
                   delete poly;
                 else
                   {  if (! cb((SurfaceEntity *)this,poly,0,0,data))
                        {  delete [] verts;  return 0;
                        }
                   }
                 poly = new Polygon3d(p4,p3,p2);
                 if (poly == NULL) return 0;
                 if (! poly->getdefined())
                   delete poly;
                 else
                   {  if (! cb((SurfaceEntity *)this,poly,0,0,data))
                        {  delete [] verts;  return 0;
                        }
                   }
              }
            p1 = p3;  p2 = p4;
            if (normals != 0)
              {  du1 = du3;  du2 = du4;
              }
         }
    }

  delete [] verts;
  delete [] normals;

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


void SurfaceOfRev::triangulate(double error, Transform *)
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



int SurfaceOfRev::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{int i,j,nu,nv,nx,ny;
 Transform tr;
 Point ax1,*verts;
  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
 if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,inside,x1,y1,x2,y2);
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) return 0;
  for (i = 0 ; i <= nu ; i++)
    {  verts[i] = e->position(double(i)/double(nu));
       if (! vs->pick(verts[i],inside,x1,y1,x2,y2)) return 0;
    }
  ax1 = axis * deltaz / double(nv);
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));
  for (j = 0 ; j <= nv ; j++)
    {  for (i = 0 ; i <= nu ; i++)
         {  verts[i] = tr.transform(verts[i]) + ax1;
            if ((i % nx) == 0 && ! vs->pick(verts[i],inside,x1,y1,x2,y2)) return 0;
         }
       if (((j+1) % ny) == 0)
         {  for (i = 1 ; i <= nu ; i++)
              if (! vs->pick(verts[i],inside,x1,y1,x2,y2)) return 0;
         }
    }
  delete [] verts;
  return 1;
}

int SurfaceOfRev::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{int i,j,nu,nv,nx,ny,picked;
 Transform tr;
 Point ax1,p1,*verts;
 double dmin;
  View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
 picked = inside != 2;
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) return 0;
  for (i = 0 ; i <= nu ; i++)
    verts[i] = e->position(double(i)/double(nu));
  ax1 = axis * deltaz / double(nv);
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));
  dmin = -1.0;
  for (j = 0 ; j < nv ; j++)
    {  for (i = 0 ; i <= nu ; i++)
         {  p1 = tr.transform(verts[i]) + ax1;
            if ((i % nx) == 0)
              {  if (vs->pick(nvertices,pwvertices,verts[i],p1,inside,&picked))
                   return picked;
              }
            verts[i] = p1;
         }
       if (((j+1) % ny) == 0)
         {  for (i = 1 ; i <= nu ; i++)
              {  if (vs->pick(nvertices,pwvertices,verts[i-1],verts[i],inside,&picked))
                   return picked;
              }
         }
    }
  delete [] verts;
  return picked;
}

double SurfaceOfRev::pick(View3dSurface *s,double x,double y,Point *pos)
{int i,j,nu,nv,nx,ny;
 Transform tr;
 Point ax1,p1,*verts;
 double d,dmin;
   View3dWindow *window;
  
    window = dynamic_cast<View3dWindow *>(s);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        s = window->getoffscreenbitmap();
 
    if (pos == NULL && s->displaylist.defined(this))
        return s->displaylist.pick(this,x,y);
    nfacets((s->getumax()-s->getumin()) / s->getwidth() * db.getdtoler(),&nu,&nv);
    nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
    ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
    if ((verts = new Point[nu+1]) == NULL) return -1.0;
    for (i = 0 ; i <= nu ; i++)
        verts[i] = e->position(double(i)/double(nu));
    ax1 = axis * deltaz / double(nv);
    tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));
    dmin = -1.0;
    for (j = 0 ; j < nv ; j++)
    {   for (i = 0 ; i <= nu ; i++)
        {   p1 = tr.transform(verts[i]) + ax1;
            if ((i % nx) == 0)
            {  if ((d = s->pick(verts[i],p1,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
                   dmin = d;
              }
            verts[i] = p1;
         }
       if (((j+1) % ny) == 0)
         {  for (i = 1 ; i <= nu ; i++)
              {  if ((d = s->pick(verts[i-1],verts[i],x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
                   dmin = d;
              }
         }
    }
  delete [] verts;
  return dmin;
}

void SurfaceOfRev::addtodisplaylist(View3dSurface *vs,int child)
{int i,j,nu,nv,nx,ny,drawmode,closedloop;
 Transform tr;
 Point ax1,p1,*verts,pt1,pt2;
 double dz;
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) return;
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,(xmesh + 4) * nv * 34 + (ymesh + 4) * nu * 34)) return;
    }
  for (i = 0 ; i <= nu ; i++)
    verts[i] = e->position(double(i)/double(nu));
  dz = deltaz / double(nv);
  ax1 = axis * dz;
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));

  drawmode = options.test(1) + options.test(2) * 2;

  if (drawmode != HLNormal)
    {Point p2,p3,p4,p5,p6;
     double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;

       //  Draw the vertical edges 

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x4 = vs->modeltoscreen(p4).x;
            y4 = vs->modeltoscreen(p4).y;

            for (j = 0 ; j < nv + closedloop ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   {  pt1 = vs->modeltoview(p3);
                      pt2 = vs->modeltoview(p4);
                      if (vs->clip3dfb(&pt1,&pt2))
                        {  pt1 = vs->viewtoscreen(pt1);
                           pt2 = vs->viewtoscreen(pt2);
                           if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                           if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                        }
                   }


                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                 y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;

              }
         }


       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       //  Draw the horizontal edges 
       for (i = 1 ; i < nu + closedloop  ; i++)
         {  if (i < nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[i+1];
              }
            else if (i == nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[1];
              }      
            x1 = vs->modeltoscreen(p1).x;
            y1 = vs->modeltoscreen(p1).y;
            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x5 = vs->modeltoscreen(p5).x;
            y5 = vs->modeltoscreen(p5).y;


            for (j = 0 ; j < nv ; j++)
              {  p2 = tr.transform(p1) + ax1;
                 p4 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p5) + ax1;

                 x2 = vs->modeltoscreen(p2).x;
                 y2 = vs->modeltoscreen(p2).y;
                 x4 = vs->modeltoscreen(p4).x;
                 y4 = vs->modeltoscreen(p4).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if ((((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   {  pt1 = vs->modeltoview(p3);
                      pt2 = vs->modeltoview(p4);
                      if (vs->clip3dfb(&pt1,&pt2))
                        {  pt1 = vs->viewtoscreen(pt1);
                           pt2 = vs->viewtoscreen(pt2);
                           if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                           if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                        }
                   }

                 p1 = p2;  p3 = p4;  p5 = p6;
                 x1 = x2;  x3 = x4;  x5 = x6;
                 y1 = y2;  y3 = y4;  y5 = y6;

              }
         }

       //  Draw the diagonal edges 
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x4 = vs->modeltoscreen(p4).x;
            y4 = vs->modeltoscreen(p4).y;

            for (j = 0 ; j < nv ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if ((((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                   {  pt1 = vs->modeltoview(p4);
                      pt2 = vs->modeltoview(p5);
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
         }


    }

  if (drawmode != HLEdgesOnly)
    {  

       if (drawmode == HLEdgesBorder)
         {  nx = nu;
            ny = nv;
         }

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       if (! closedloop)
         for (i = 1 ; i <= nu ; i++)
           {  pt1 = vs->modeltoview(verts[i-1]);  pt2 = vs->modeltoview(verts[i]);
              if (vs->clip3dfb(&pt1,&pt2))
                {  pt1 = vs->viewtoscreen(pt1);
                   pt2 = vs->viewtoscreen(pt2);
                   if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                   if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                }
           }

       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       for (j = 0 ; j < nv ; j++)
         {  for (i = 0 ; i <= nu ; i++)
              {  p1 = tr.transform(verts[i]) + ax1;
                 if ((i % nx) == 0 &&
                       (drawmode == HLNormal || drawmode == HLEdges || ! closedloop))
                   {  pt1 = vs->modeltoview(verts[i]);  pt2 = vs->modeltoview(p1);
                      if (vs->clip3dfb(&pt1,&pt2))
                        {  pt1 = vs->viewtoscreen(pt1);
                           pt2 = vs->viewtoscreen(pt2);
                           if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                           if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                        }
                   }
                 verts[i] = p1;
              }

            if (fabs(deltaz) > db.getptoler() || fabs(angle - 360.0) > db.getatoler())
             if (((j+1) % ny) == 0)
              {  for (i = 1 ; i <= nu ; i++)
                   {  pt1 = vs->modeltoview(verts[i-1]);  pt2 = vs->modeltoview(verts[i]);
                      if (vs->clip3dfb(&pt1,&pt2))
                        {  pt1 = vs->viewtoscreen(pt1);
                           pt2 = vs->viewtoscreen(pt2);
                           if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
                           if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
                        }
                   }
              } 
         }

    }
  delete [] verts;
  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void SurfaceOfRev::drawGL(int drawmode,Transform *transform,View3dSurface *vs)
{
  int i,j,closed,closedloop,nu,nv,nx,ny,edgemode;
  Transform tr;
  Point o,ax,ax1,p1,*verts;
  double dz,direction;

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
  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) return;
  for (i = 0 ; i <= nu ; i++)
  {
       verts[i] = e->position(double(i)/double(nu));

       //verts[i] = vs->modeltransform.transform(verts[i]);

       if (transform != NULL)
           verts[i] = transform->transform(verts[i]);
  }
  direction = 1.0;
  if (transform != NULL)
    {Point xax(1.0,0.0,0.0),yax(0.0,1.0,0.0),zax(0.0,0.0,1.0);
       o = transform->transform(origin);
       ax = transform->transform(axis+origin) - o;
       dz = deltaz * ax.length() / double(nv);
       ax.normalize();
       if (ax.length() < db.getptoler()) return;

       xax = transform->transform(xax+origin) - o;
       yax = transform->transform(yax+origin) - o;
       zax = transform->transform(zax+origin) - o;
       if (xax.cross(yax).dot(zax) < 0.0)
         direction = -1.0;

    }
  else
    {  o = origin;  ax = axis;  dz = deltaz / double(nv);
    }
  ax1 = ax * dz;
  tr.rotate(o,ax,angle * direction * M_PI / 180.0 / double(nv));

  edgemode = options.test(1) + options.test(2) * 2;

  if (edgemode != HLNormal)
    {Point p2,p3,p4,p5,p6;
     double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;

       //  Draw the vertical edges

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            //x3 = vs->modeltoscreen(p3).x;
            //y3 = vs->modeltoscreen(p3).y;
            //x4 = vs->modeltoscreen(p4).x;
            //y4 = vs->modeltoscreen(p4).y;
            x3 = p3.x;
            y3 = p3.y;
            x4 = p4.x;
            y4 = p4.y;

            for (j = 0 ; j < nv + closedloop ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;
                 //x5 = p5.x;
                 //y5 = p5.y;
                 //x6 = p6.x;
                 //y6 = p6.y;

                 if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   vs->line3dGL(vs->modeltransform.transform(p3),vs->modeltransform.transform(p4));

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                 y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;

              }
         }


       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       //  Draw the horizontal edges
       for (i = 1 ; i < nu + closedloop  ; i++)
         {  if (i < nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[i+1];
              }
            else if (i == nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[1];
              }
            //x1 = vs->modeltoscreen(p1).x;
            //y1 = vs->modeltoscreen(p1).y;
            //x3 = vs->modeltoscreen(p3).x;
            //y3 = vs->modeltoscreen(p3).y;
            //x5 = vs->modeltoscreen(p5).x;
            //y5 = vs->modeltoscreen(p5).y;

            x1 = p1.x;
            y1 = p1.y;
            x3 = p3.x;
            y3 = p3.y;
            x5 = p5.x;
            y5 = p5.y;


            for (j = 0 ; j < nv ; j++)
              {  p2 = tr.transform(p1) + ax1;
                 p4 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p5) + ax1;

                 //x2 = vs->modeltoscreen(p2).x;
                 //y2 = vs->modeltoscreen(p2).y;
                 //x4 = vs->modeltoscreen(p4).x;
                 //y4 = vs->modeltoscreen(p4).y;
                 //x6 = vs->modeltoscreen(p6).x;
                 //y6 = vs->modeltoscreen(p6).y;

                 x2 = p2.x;
                 y2 = p2.y;
                 x4 = p4.x;
                 y4 = p4.y;
                 x6 = p6.x;
                 y6 = p6.y;

                 if ((((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   vs->line3dGL(vs->modeltransform.transform(p3),vs->modeltransform.transform(p4));

                 p1 = p2;  p3 = p4;  p5 = p6;
                 x1 = x2;  x3 = x4;  x5 = x6;
                 y1 = y2;  y3 = y4;  y5 = y6;

              }
         }

       //  Draw the diagonal edges
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];

            //x3 = vs->modeltoscreen(p3).x;
            //y3 = vs->modeltoscreen(p3).y;
            //x4 = vs->modeltoscreen(p4).x;
            //y4 = vs->modeltoscreen(p4).y;

            x3 = p3.x;
            y3 = p3.y;
            x4 = p4.x;
            y4 = p4.y;


            for (j = 0 ; j < nv ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 //x5 = p5.x;
                 //y5 = p5.y;
                 //x6 = p6.x;
                 //y6 = p6.y;

                 if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                   vs->line3dGL(vs->modeltransform.transform(p4),vs->modeltransform.transform(p5));

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x3 = x5;  x4 = x6;
                 y1 = y3;  y3 = y5;  y4 = y6;

              }
         }
    }


    if (edgemode != HLEdgesOnly)
    {

       if (edgemode == HLEdgesBorder)
         {  nx = nu;
            ny = nv;
         }

       closed = (verts[0] - verts[nu]).length() < db.getptoler();
       for (j = 0 ; j < nv ; j++)
         {  for (i = 0 ; i <= nu ; i++)
              {  p1 = tr.transform(verts[i]) + ax1;
                 if ((i % nx) == 0 && (edgemode == HLNormal || edgemode == HLEdges || ! closedloop))
                    vs->line3dGL(vs->modeltransform.transform(verts[i]),vs->modeltransform.transform(p1));

                 verts[i] = p1;
              }
            if (((j+1) % ny) == 0)
            {
                 for (i = 1 ; i <= nu ; i++)
                 {
                    vs->line3dGL(vs->modeltransform.transform(verts[i-1]),vs->modeltransform.transform(verts[i]));
                 }
            }
         }
     }

  delete [] verts;
  }
}
#endif

void SurfaceOfRev::draw(int drawmode,Transform *transform,View3dSurface *vs)
{
  int i,j,closed,closedloop,nu,nv,nx,ny,edgemode;
  Transform tr;
  Point o,ax,ax1,p1,*verts;
  double dz,direction;

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

  nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) return;
  for (i = 0 ; i <= nu ; i++)
    {  verts[i] = e->position(double(i)/double(nu));
       if (transform != NULL)  verts[i] = transform->transform(verts[i]);
    }
  direction = 1.0;
  if (transform != NULL)
    {Point xax(1.0,0.0,0.0),yax(0.0,1.0,0.0),zax(0.0,0.0,1.0);
       o = transform->transform(origin);
       ax = transform->transform(axis+origin) - o;
       dz = deltaz * ax.length() / double(nv);
       ax.normalize();
       if (ax.length() < db.getptoler()) return;

       xax = transform->transform(xax+origin) - o;
       yax = transform->transform(yax+origin) - o;
       zax = transform->transform(zax+origin) - o;
       if (xax.cross(yax).dot(zax) < 0.0)
         direction = -1.0;

    }
  else
    {  o = origin;  ax = axis;  dz = deltaz / double(nv);
    }
  ax1 = ax * dz;
  tr.rotate(o,ax,angle * direction * M_PI / 180.0 / double(nv));

  edgemode = options.test(1) + options.test(2) * 2;


  if (edgemode != HLNormal)
    {Point p2,p3,p4,p5,p6;
     double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;

       //  Draw the vertical edges 

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x4 = vs->modeltoscreen(p4).x;
            y4 = vs->modeltoscreen(p4).y;

            for (j = 0 ; j < nv + closedloop ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   qgline = vs->line3d((QGraphicsItem*)0,p3,p4);

                 if(qgline)
                     qgsurface->addToGroup(qgline);

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                 y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;

              }
         }


       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       //  Draw the horizontal edges 
       for (i = 1 ; i < nu + closedloop  ; i++)
         {  if (i < nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[i+1];
              }
            else if (i == nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[1];
              }      
            x1 = vs->modeltoscreen(p1).x;
            y1 = vs->modeltoscreen(p1).y;
            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x5 = vs->modeltoscreen(p5).x;
            y5 = vs->modeltoscreen(p5).y;


            for (j = 0 ; j < nv ; j++)
              {  p2 = tr.transform(p1) + ax1;
                 p4 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p5) + ax1;

                 x2 = vs->modeltoscreen(p2).x;
                 y2 = vs->modeltoscreen(p2).y;
                 x4 = vs->modeltoscreen(p4).x;
                 y4 = vs->modeltoscreen(p4).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if ((((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x1 + (x3-x4)*y1 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   qgline = vs->line3d((QGraphicsItem*)0,p3,p4);

                 if(qgline)
                     qgsurface->addToGroup(qgline);

                 p1 = p2;  p3 = p4;  p5 = p6;
                 x1 = x2;  x3 = x4;  x5 = x6;
                 y1 = y2;  y3 = y4;  y5 = y6;

              }
         }

       //  Draw the diagonal edges 
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];

            x3 = vs->modeltoscreen(p3).x;
            y3 = vs->modeltoscreen(p3).y;
            x4 = vs->modeltoscreen(p4).x;
            y4 = vs->modeltoscreen(p4).y;


            for (j = 0 ; j < nv ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = vs->modeltoscreen(p5).x;
                 y5 = vs->modeltoscreen(p5).y;
                 x6 = vs->modeltoscreen(p6).x;
                 y6 = vs->modeltoscreen(p6).y;

                 if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                   qgline = vs->line3d((QGraphicsItem*)0,p4,p5);

                 if(qgline)
                     qgsurface->addToGroup(qgline);

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x3 = x5;  x4 = x6;
                 y1 = y3;  y3 = y5;  y4 = y6;

              }
         }


    }


  if (edgemode != HLEdgesOnly)
    {  

       if (edgemode == HLEdgesBorder)
         {  nx = nu;
            ny = nv;
         }

       closed = (verts[0] - verts[nu]).length() < db.getptoler();
       for (j = 0 ; j < nv ; j++)
         {  for (i = 0 ; i <= nu ; i++)
              {  p1 = tr.transform(verts[i]) + ax1;
                 if ((i % nx) == 0 && (edgemode == HLNormal || edgemode == HLEdges || ! closedloop))
                   qgline = vs->line3d((QGraphicsItem*)0,verts[i],p1);

                 if(qgline)
                     qgsurface->addToGroup(qgline);

                 verts[i] = p1;
              }
            if (((j+1) % ny) == 0)
            {
                 for (i = 1 ; i <= nu ; i++)
                 {
                   qgline = vs->line3d((QGraphicsItem*)0,verts[i-1],verts[i]);

                    if(qgline)
                        qgsurface->addToGroup(qgline);
                 }
            }
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

  delete [] verts;
}

#ifdef USING_WIDGETS
int SurfaceOfRev::hideGL(HideImage *image)
{int i,j,nu,nv,nx,ny,drawmode,closedloop;
 Transform tr;
 Point ax1,p1,p2,p3,p4,p5,p6,*verts;
 double dz,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;

  drawmode = options.test(1) + options.test(2) * 2;

  image->getsurface()->setupGL(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL)
    return 0;
  for (i = 0 ; i <= nu ; i++)
    verts[i] = e->position(double(i)/double(nu));
  dz = deltaz / double(nv);
  ax1 = axis * dz;
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));

  if (drawmode != HLNormal)
    {
       //  Draw the vertical edges

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x4 = image->getsurface()->modeltoscreen(p4).x;
            y4 = image->getsurface()->modeltoscreen(p4).y;

            for (j = 0 ; j < nv + closedloop ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = image->getsurface()->modeltoscreen(p5).x;
                 y5 = image->getsurface()->modeltoscreen(p5).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   image->hideGL(this,p3,p4);


                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                 y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;

              }
         }


       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       //  Draw the horizontal edges
       for (i = 1 ; i < nu + closedloop  ; i++)
         {  if (i < nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[i+1];
              }
            else if (i == nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[1];
              }
            x1 = image->getsurface()->modeltoscreen(p1).x;
            y1 = image->getsurface()->modeltoscreen(p1).y;
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x5 = image->getsurface()->modeltoscreen(p5).x;
            y5 = image->getsurface()->modeltoscreen(p5).y;


            for (j = 0 ; j < nv ; j++)
              {  p2 = tr.transform(p1) + ax1;
                 p4 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p5) + ax1;

                 x2 = image->getsurface()->modeltoscreen(p2).x;
                 y2 = image->getsurface()->modeltoscreen(p2).y;
                 x4 = image->getsurface()->modeltoscreen(p4).x;
                 y4 = image->getsurface()->modeltoscreen(p4).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if ((((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   image->hideGL(this,p3,p4);

                 p1 = p2;  p3 = p4;  p5 = p6;
                 x1 = x2;  x3 = x4;  x5 = x6;
                 y1 = y2;  y3 = y4;  y5 = y6;

              }
         }

       //  Draw the diagonal edges
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x4 = image->getsurface()->modeltoscreen(p4).x;
            y4 = image->getsurface()->modeltoscreen(p4).y;

            for (j = 0 ; j < nv ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = image->getsurface()->modeltoscreen(p5).x;
                 y5 = image->getsurface()->modeltoscreen(p5).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if ((((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                   image->hideGL(this,p4,p5);

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x3 = x5;  x4 = x6;
                 y1 = y3;  y3 = y5;  y4 = y6;

              }
         }


    }

  if (drawmode != HLEdgesOnly)
    {  //  Must generate the lines in continuous curves to give good dashed results.

       if (drawmode == HLEdgesBorder)
         {  nx = nu;
            ny = nv;
         }

       if (drawmode == HLNormal || drawmode == HLEdges ||
            (verts[0] - verts[nu]).length() > db.getptoler())
         for (i = 0 ; i <= nu ; i++)
           if ((i % nx) == 0)
             for (j = 0 ; j < nv ; j++)
               {  p1 = tr.transform(verts[i]) + ax1;
                  image->hideGL(this,verts[i],p1);
                  verts[i] = p1;
               }

       //  Must generate the lines in continuous curves to give good dashed results.
       for (i = 0 ; i <= nu ; i++)
         verts[i] = e->position(double(i)/double(nu));

       if (fabs(deltaz) > db.getptoler() || fabs(angle - 360.0) > db.getatoler())
         {  if (drawmode != HLEdgesOnly)
              {  for (i = 1 ; i <= nu ; i++)
                   image->hideGL(this,verts[i-1],verts[i]);
              }
            for (j = 0 ; j < nv ; j++)
              {  for (i = 0 ; i <= nu ; i++)
                   verts[i] = tr.transform(verts[i]) + ax1;
                 if (((j+1) % ny) == 0)
                   {  for (i = 1 ; i <= nu ; i++)
                        image->hideGL(this,verts[i-1],verts[i]);
                   }
              }
         }
    }

  delete [] verts;
  return 1;
}
#endif

int SurfaceOfRev::hide(HideImage *image)
{int i,j,nu,nv,nx,ny,drawmode,closedloop;
 Transform tr;
 Point ax1,p1,p2,p3,p4,p5,p6,*verts;
 double dz,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;

  drawmode = options.test(1) + options.test(2) * 2;

  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&nu,&nv);
  nx = nu / (xmesh - 1);  if (nx < 1) nx = 1;
  ny = nv / (ymesh - 1);  if (ny < 1) ny = 1;
  if ((verts = new Point[nu+1]) == NULL) 
    return 0;
  for (i = 0 ; i <= nu ; i++)
    verts[i] = e->position(double(i)/double(nu));
  dz = deltaz / double(nv);
  ax1 = axis * dz;
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));

  if (drawmode != HLNormal)
    {
       //  Draw the vertical edges 

       closedloop = fabs(deltaz) < db.getptoler() && fabs(angle - 360.0) < db.getatoler();

       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x4 = image->getsurface()->modeltoscreen(p4).x;
            y4 = image->getsurface()->modeltoscreen(p4).y;

            for (j = 0 ; j < nv + closedloop ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = image->getsurface()->modeltoscreen(p5).x;
                 y5 = image->getsurface()->modeltoscreen(p5).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   image->hide(this,p3,p4);


                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                 y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;

              }
         }


       closedloop = (verts[0] - verts[nu]).length() < db.getptoler();

       //  Draw the horizontal edges 
       for (i = 1 ; i < nu + closedloop  ; i++)
         {  if (i < nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[i+1];
              }
            else if (i == nu)
              {  p1 = verts[i-1];
                 p3 = verts[i];
                 p5 = verts[1];
              }      
            x1 = image->getsurface()->modeltoscreen(p1).x;
            y1 = image->getsurface()->modeltoscreen(p1).y;
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x5 = image->getsurface()->modeltoscreen(p5).x;
            y5 = image->getsurface()->modeltoscreen(p5).y;


            for (j = 0 ; j < nv ; j++)
              {  p2 = tr.transform(p1) + ax1;
                 p4 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p5) + ax1;

                 x2 = image->getsurface()->modeltoscreen(p2).x;
                 y2 = image->getsurface()->modeltoscreen(p2).y;
                 x4 = image->getsurface()->modeltoscreen(p4).x;
                 y4 = image->getsurface()->modeltoscreen(p4).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if ((((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                   image->hide(this,p3,p4);

                 p1 = p2;  p3 = p4;  p5 = p6;
                 x1 = x2;  x3 = x4;  x5 = x6;
                 y1 = y2;  y3 = y4;  y5 = y6;

              }
         }

       //  Draw the diagonal edges 
       for (i = 1 ; i <= nu ; i++)
         {  p3 = verts[i-1];
            p4 = verts[i];
            x3 = image->getsurface()->modeltoscreen(p3).x;
            y3 = image->getsurface()->modeltoscreen(p3).y;
            x4 = image->getsurface()->modeltoscreen(p4).x;
            y4 = image->getsurface()->modeltoscreen(p4).y;

            for (j = 0 ; j < nv ; j++)
              {  p5 = tr.transform(p3) + ax1;
                 p6 = tr.transform(p4) + ax1;

                 x5 = image->getsurface()->modeltoscreen(p5).x;
                 y5 = image->getsurface()->modeltoscreen(p5).y;
                 x6 = image->getsurface()->modeltoscreen(p6).x;
                 y6 = image->getsurface()->modeltoscreen(p6).y;

                 if ((((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                   image->hide(this,p4,p5);

                 p1 = p3;  p2 = p4;  p3 = p5;  p4 = p6;
                 x1 = x3;  x3 = x5;  x4 = x6;
                 y1 = y3;  y3 = y5;  y4 = y6;

              }
         }


    }

  if (drawmode != HLEdgesOnly)
    {  //  Must generate the lines in continuous curves to give good dashed results.

       if (drawmode == HLEdgesBorder)
         {  nx = nu;
            ny = nv;
         }

       if (drawmode == HLNormal || drawmode == HLEdges ||
            (verts[0] - verts[nu]).length() > db.getptoler())
         for (i = 0 ; i <= nu ; i++)
           if ((i % nx) == 0) 
             for (j = 0 ; j < nv ; j++)
               {  p1 = tr.transform(verts[i]) + ax1;
                  image->hide(this,verts[i],p1);
                  verts[i] = p1;
               }

       //  Must generate the lines in continuous curves to give good dashed results.
       for (i = 0 ; i <= nu ; i++)
         verts[i] = e->position(double(i)/double(nu));

       if (fabs(deltaz) > db.getptoler() || fabs(angle - 360.0) > db.getatoler())
         {  if (drawmode != HLEdgesOnly)
              {  for (i = 1 ; i <= nu ; i++)
                   image->hide(this,verts[i-1],verts[i]);
              }
            for (j = 0 ; j < nv ; j++)
              {  for (i = 0 ; i <= nu ; i++)
                   verts[i] = tr.transform(verts[i]) + ax1;
                 if (((j+1) % ny) == 0)
                   {  for (i = 1 ; i <= nu ; i++)
                        image->hide(this,verts[i-1],verts[i]);
                   }
              } 
         }
    }

  delete [] verts;
  return 1;
}

Point SurfaceOfRev::nearp(Point)
{Point tmp(0.0,0.0,0.0),normal;
  return tmp;
}

int SurfaceOfRev::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *ent,*c;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  e->stretchent(db,t,sp,erase);
  if (e->getcopye() == 0) return 1;
  for (alist.start() ; (ent = alist.next()) != NULL ; )
    if ((ent->getstatus() & 1) == 0 && ! e->isa(group_entity))
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus(getstatus() | 4);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db != 0 ? 1 : db->geometry.add(c);
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
  if (e->getcopye() != NULL) e = (LinearEntity *) e->getcopye();
  dbinsert(db);
  copye = this;
  return 1;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
}

int SurfaceOfRev::move(Database *db,Transform &t,int erase,int save)
{Entity *ae;
 Point o,ax,xax(1.0,0.0,0.0),yax(0.0,1.0,0.0),zax(0.0,0.0,1.0);
 double dz;

  if (copye != NULL) return 1;
  o = t.transform(origin);
  ax = t.transform(axis+origin) - o;
  dz = deltaz * ax.length();
  ax.normalize();
  if (ax.length() < ::db.getptoler()) return 0;
  for (alist.start() ; (ae = alist.next()) != NULL ; )
    if ((ae->getstatus() & 1) == 0)
      {  status &= ~1;  //  This entity will not be moved - remove it from the moved list
         return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
      }
  if (e->getcopye() == NULL) e->move(db,t,erase);
  if (e->getcopye() == NULL)
    {  draw(DM_NORMAL);  return 0;
    }
  status |= 1;
  if (db != 0)
    {  
	  if (save)
		db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  e = (LinearEntity *) e->getcopye();

  xax = t.transform(xax+origin) - o;
  yax = t.transform(yax+origin) - o;
  zax = t.transform(zax+origin) - o;
  if (xax.cross(yax).dot(zax) < 0.0)
    angle = -angle;

  origin = o;
  axis = ax;
  deltaz = dz;
  dbinsert(db);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

  return 1;
}

Entity *SurfaceOfRev::clone(Transform &t)
{Point o,ax,xax(1.0,0.0,0.0),yax(0.0,1.0,0.0),zax(0.0,0.0,1.0);
 double dz,reverse;
  if (copye != NULL) return copye;
  o = t.transform(origin);
  ax = t.transform(axis+origin) - o;

  xax = t.transform(xax+origin) - o;
  yax = t.transform(yax+origin) - o;
  zax = t.transform(zax+origin) - o;
  if (xax.cross(yax).dot(zax) < 0.0)
    reverse = -1.0;
  else
    reverse = 1.0;
  dz = deltaz * ax.length();
  ax.normalize();
  if (ax.length() < db.getptoler()) return NULL;
  if (e->getcopye() == NULL) e->clone(t);
  if (e->getcopye() == NULL) return NULL;
  copye = new SurfaceOfRev(*this,o,ax,angle * reverse,dz,(LinearEntity *) e->getcopye(),xmesh,ymesh,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

Entity *SurfaceOfRev::deepclone(Transform &t)
{Point o,ax;
 double dz;
  o = t.transform(origin);
  ax = t.transform(axis+origin) - o;
  dz = deltaz * ax.length();
  ax.normalize();
  if (ax.length() < db.getptoler()) return 0;
  e->setcopye(0); e->clone(t);
  if (e->getcopye() == 0) return 0;
  copye = new SurfaceOfRev(*this,o,ax,angle,dz,(LinearEntity *) e->getcopye(),xmesh,ymesh,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  return copye;
}

int SurfaceOfRev::project(Database *db,Transform &t,int)
{  return db->geometry.add(clone(t));
}

void SurfaceOfRev::change(const BitMask &change,const EntityHeader &header,Point org,Point ax,double a,double dz,int xm,int ym,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = header.getcolour();
  if (change.test(1)) layer = header.getlayer();
  if (change.test(2)) style = header.getstyle();
  if (change.test(3)) weight = header.getweight();
  if (change.test(4)) origin = org;
  if (change.test(5)) axis = ax;
  if (change.test(6)) angle = a;
  if (change.test(7)) deltaz = dz;
  if (change.test(8)) xmesh = short(xm);
  if (change.test(9)) ymesh = short(ym);
  if (change.test(10)) 
    {  options.set(1,o.test(1));
       options.set(2,o.test(2));
    }
  draw(DM_NORMAL);
}

/*
// defined in ncdialog.h
class SurfaceOfRevDisplayDialogControl : public DisplayDialogControl
  {public:
     SurfaceOfRevDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void SurfaceOfRevDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hWnd,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 double angle,deltaz,x1,y1,w,h;
 int xm,ym,edgemode;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);

 if(hWnd == 0)
     return;

 QGraphicsView *ctl = 0;
 if(dialog->gethdlg())
   ctl = (QGraphicsView*)((SurfaceOfRev_Dialog*)dialog->gethdlg())->getControl(109);
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

       //oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
       //IntersectClipRect(output.gethdc(),0,0,int(output.getwidth()),int(output.getheight()));
       //rect.left = 0;  rect.top = 0;
       //rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       //FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       //output.moveto(0,0);
       //output.lineto(rect.right-1,0);
       //output.lineto(rect.right-1,rect.bottom-1);
       //output.lineto(0,rect.bottom-1);
       //output.lineto(0,0);
       //SelectObject(output.gethdc(),oldhpen);
       //DeleteObject(hpen);

       if (dialog->currentvalue(104,&angle) &&
           dialog->currentvalue(105,&deltaz) &&
           dialog->currentvalue(106,&xm) &&
           dialog->currentvalue(107,&ym) &&
           dialog->currentvalue(108,&edgemode))
         {

            Point org,xaxis,yaxis,axis;
            BitMask options(32);
            options.set(1,edgemode % 2);  options.set(2,edgemode / 2);
            if (xm < 2) xm = 2;  if (ym < 2) ym = 2;
            x1 = output.getumin();
            y1 = output.getvmin();
            w = output.getumax() - x1;
            h = output.getvmax() - y1;
            org.setxyz(x1+w/2.0,y1+h/2.0,0.0);
            xaxis.setxyz(1.0,0.0,0.0);
            yaxis.setxyz(0.0,1.0,0.0);
            Circle *circle = new Circle(org,xaxis,yaxis,h/2*0.85,0.0,M_PI);
            if (circle != NULL)
            {
                 axis.setxyz(1.0,0.0,0.0);
                 SurfaceOfRev sr(org,axis,angle,deltaz,circle,xm,ym,options);
                 sr.draw(DM_NORMAL,NULL,&output);
            }
         }
    }
}

class SurfaceOfRevVerifyButtonControl : public ButtonDialogControl
  {public:
     SurfaceOfRevVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int SurfaceOfRevVerifyButtonControl::select(Dialog *dialog)
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


int SurfaceOfRev::setup(BitMask *change,double *angle,double *deltaz,int *xm,int *ym,BitMask *options,EntityHeader *inheader)
{
 int status,drawmode;
 //static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
 RCSTRING STRdrawmodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
 RCCHAR dm1[25],dm2[25],dm3[25],dm4[25];
 RCCHAR *drawmodel[] = { dm1,dm2,dm3,dm4 };
 Dialog dialog("SurfaceOfRev_Dialog"); 
 DialogControl *dc;
 ResourceString rs0(NCREVA);
 //
 int c,l,s,defColour,defLayer,defStyle,defxm,defym;
 double wt,defWeight;
 BitMask defoptions(32);

 for(int i=0; i<4; i++)
     strcpy(drawmodel[i],&STRdrawmodel[i]);

 EntityHeader *header=0;
 if(inheader == 0)
     header = new EntityHeader();
 else
     header = inheader;

 header->verify(&dialog,&c,&l,&s,&wt);

 int changecount = v.getinteger("op::changecount");

 defColour = db.header.getcolour();
 defLayer  = db.header.getlayer();
 defStyle  = db.header.getstyle();
 defWeight = db.lineweights.weighttomm(db.header.getweight());
 RuledSurface::getdefaults(&defxm,&defym,&defoptions);

 drawmode = options->test(1) + options->test(2) * 2;

 dialog.add(dc = new RealDialogControl(104,angle));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 dialog.add(dc = new RealDialogControl(105,deltaz));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(dc = new IntegerDialogControl(106,xm,2,2000));
 //dialog.add(new ScrollBarDialogControl(1102,dc));
 dialog.add(dc = new IntegerDialogControl(107,ym,2,2000));
 //dialog.add(new ScrollBarDialogControl(1103,dc));
 dialog.add(new ListDialogControl(108,4,drawmodel,&drawmode));
 dialog.add((DisplayDialogControl *) new SurfaceOfRevDisplayDialogControl(109));
 dialog.add(new ButtonDialogControl(110)); // surface properties
 dialog.add(new ButtonDialogControl(111)); // triangulate
 dialog.add(new ButtonDialogControl(112)); // clone (105->112)
 dialog.add(new ButtonDialogControl(113)); // clone all (106->113)

 if (change != NULL)
 {
     if(changecount == 1)
        dialog.title(_RCT("Surface of Revolution Verify"));
     else
      dialog.title(rs0.gets());

     v.setinteger("op::change",1);
     v.setbm("op::changemask",*change);

     // debug
     change->setall();
     change->clear(4);
     change->clear(5);
     //change->clearall();

     //dialog.add(new CheckBoxDialogControl(200,change,6));
     //dialog.add(new CheckBoxDialogControl(201,change,7));
     //dialog.add(new CheckBoxDialogControl(202,change,8));
     //dialog.add(new CheckBoxDialogControl(203,change,9));
     //dialog.add(new CheckBoxDialogControl(207,change,10));

     dialog.add(new SurfaceOfRevVerifyButtonControl(1000));
     dialog.add(new SurfaceOfRevVerifyButtonControl(1001));
     dialog.add(new SurfaceOfRevVerifyButtonControl(1002));
     dialog.add(new SurfaceOfRevVerifyButtonControl(1003));

 }
 status = dialog.process();
 if (*xm < 2) *xm = 2;  if (*ym < 2) *ym = 2;
 if (status == TRUE)
 {
     if(change != NULL)
     {

         for (int i = 0 ; i < 4 ; i++)
           change->set(i,1);

         header->setcolour(c);
         header->setstyle(s);
         header->setlayer(l);
         header->setweight(db.lineweights.mmtoweight(wt));

         options->set(1,drawmode % 2);  options->set(2,drawmode / 2);

     }
     else
     {
        options->set(1,drawmode % 2);  options->set(2,drawmode / 2);
     }
 }
 if(inheader == 0)
     delete header;
 return status;
}

void SurfaceOfRev::getdefaults(double *angle,double *deltaz,int *xm,int *ym,BitMask *options)
{ *angle = v.getreal("sr::angle");
  *deltaz = v.getreal("sr::deltaz");
  *options = *v.getbm("sr::options");
  *xm = v.getinteger("sr::xmesh");
  *ym = v.getinteger("sr::ymesh");
}

void SurfaceOfRev::setdefaults(double angle,double deltaz,int xm,int ym,const BitMask &options)
{ v.setreal("sr::angle",angle);
  v.setreal("sr::deltaz",deltaz);
  v.setbm("sr::options",options);
  v.setinteger("sr::xmesh",xm);
  v.setinteger("sr::ymesh",ym);
}

void SurfaceOfRev::setdefaults(void)
{ v.setreal("sr::angle",angle);
  v.setreal("sr::deltaz",deltaz);
  v.setbm("sr::options",options);
  v.setinteger("sr::xmesh",xmesh);
  v.setinteger("sr::ymesh",ymesh);
}

int SurfaceOfRev::save(CadFile *outfile)
{short type;
  type = surfaceofrev_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! outfile->write((const Entity **)&e)) return 0;
  if (! e->saveunknowngeometry(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! axis.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&deltaz)) return 0;
  if (! outfile->write(&xmesh)) return 0;
  if (! outfile->write(&ymesh)) return 0;
  return 1;
}

Entity *SurfaceOfRev::load(int reference,CadFile *infile)
{SurfaceOfRev *surfaceofrev;
 EntityHeader *header;
 Entity *e,*ejunk;
 BitMask options(32);
 double angle,deltaz;
 short xmesh,ymesh;
 Point origin,axis;
  if ((header = loadheader(infile)) == NULL) return NULL;
  if (! infile->read((Entity **)&e)) return 0;
  if (reference)
    {  if (! Entity::loadunknowngeometry(1,infile,&ejunk)) return 0;
    }
  else
    {  if (! Entity::loadunknowngeometry(0,infile,&e)) return 0;
    }
  if (! origin.load(infile)) return 0;
  if (! axis.load(infile)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&angle)) return 0;
  if (! infile->read(&deltaz)) return 0;
  if (! infile->read(&xmesh)) return 0;
  if (! infile->read(&ymesh)) return 0;
  surfaceofrev = new SurfaceOfRev(*header,origin,axis,angle,deltaz,(LinearEntity *)e,xmesh,ymesh,options);
  surfaceofrev->atlist = header->getatlist();
  delete header;
  return surfaceofrev;
}

int SurfaceOfRev::save(UndoFile *outfile)
{short type;
 long v = e->getid();
  type = surfaceofrev_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&v)) return 0;
  return 1;
}

Entity *SurfaceOfRev::load(UndoFile *infile)
{SurfaceOfRev *sr;
 char bm[sizeof(BitMask)];
  if ((sr = new SurfaceOfRev) != NULL)
    {AttributeList al;
       memcpy(bm,&sr->visible,sizeof(bm));
       if (! infile->blockread(sr,sizeof(*sr))) return 0;
       memcpy(&sr->visible,bm,sizeof(bm));
       if (! infile->read(&sr->visible)) return 0;
       if (! al.load(infile)) return 0;   sr->atlist = al;
       if (! infile->read((long *)&sr->e)) return 0;
    }
  return sr;
}

int SurfaceOfRev::saveattributes(CadFile *outfile)
{ if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&deltaz)) return 0;
  if (! outfile->write(&xmesh)) return 0;
  if (! outfile->write(&ymesh)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

int SurfaceOfRev::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&angle)) return 0;
  if (! infile->read(&deltaz)) return 0;
  if (! infile->read(&xmesh)) return 0;
  if (! infile->read(&ymesh)) return 0;
  if (! infile->read(&options)) return 0;
  return 1;
}

int SurfaceOfRev::savedxf(int blocksection,DXFOut *dxffile)
{int i,j,nu,nv;
 Transform tr;
 Point ax1,p1;
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
       ax1 = axis * (deltaz / double(nv - 1));
       tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv - 1));
       for (i = 0 ; i < nu ; i++)
         {  p1 = e->position(double(i)/double(nu - 1));
            for (j = 0 ; j < nv ; j++)
              {  if (! dxffile->writestring(0, (char*)"VERTEX")) return 0;
                 if (! dxffile->writehandle()) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
                 if (! dxffile->writelong(8,getlayer())) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbVertex")) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbPolygonMeshVertex")) return 0;
                 if (! dxffile->writereal(10,p1.x * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(20,p1.y * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(30,p1.z * dxffile->getscale())) return 0;
                 if (! dxffile->writereal(70,64)) return 0;
                 p1 = tr.transform(p1) + ax1;
              }
         }
       if (! dxffile->writestring(0, (char*)"SEQEND")) return 0;
       if (! dxffile->writehandle()) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
    }
  return 1;
}

int SurfaceOfRev::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int SurfaceOfRev::savegeometry(CadFile *outfile)
{long v = e->getid();
  if (! outfile->write(&angle)) return 0;
  if (! outfile->write(&deltaz)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! axis.save(outfile)) return 0;
  outfile->write(&v);
  return 1;
}

int SurfaceOfRev::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&angle)) return 0;
  if (! infile->read(&deltaz)) return 0;
  if (! origin.load(infile)) return 0;
  if (! axis.load(infile)) return 0;
  infile->read((long *)&e);
  return 1;
}

void SurfaceOfRev::fixupreferences(Entity ** index)
{ e = (LinearEntity *) index[long(e)];
  Entity::fixupreferences(index);
}

void SurfaceOfRev::verify(void)
{
#if 0
     // this function now just pops up the options dialog
     state.sendevent(new NewCommandEvent(411,0));
#else
     //static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
     RCSTRING STRedgemodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
     RCCHAR em1[25],em2[25],em3[25],em4[25];
     RCCHAR *edgemodel[] = { em1,em2,em3,em4 };
     Dialog dialog("Surfaceofrevverify_Dialog");
     int c,l,s,status,drawmode;
     double w;
     //RCCHAR drawmode[300];
     Entity *e;

     for(int i=0; i<4; i++)
         strcpy(edgemodel[i],&STRedgemodel[i]);

     drawmode = options.test(1) + options.test(2) * 2;
    //strcpy(drawmode,drawmodel[options.test(1) + options.test(2) * 2]);

    EntityHeader::verify(&dialog,&c,&l,&s,&w);
    dialog.add(new SurfaceOfRevVerifyButtonControl(1000));
    dialog.add(new SurfaceOfRevVerifyButtonControl(1001));
    dialog.add(new SurfaceOfRevVerifyButtonControl(1002));
    dialog.add(new SurfaceOfRevVerifyButtonControl(1003));

    dialog.add(new RealDialogControl(104,&angle));
    dialog.add(new RealDialogControl(105,&deltaz));
    dialog.add(new IntegerDialogControl(106,&xmesh));
    dialog.add(new IntegerDialogControl(107,&ymesh));
    //dialog.add(new ButtonDialogControl(108)); // status dialog
    dialog.add(new ButtonDialogControl(109)); // change surface properties
    //dialog.add(new StringDialogControl(110,drawmode,300));
    dialog.add(new ListDialogControl(110,4,edgemodel,&drawmode));
    dialog.add(new ButtonDialogControl(111)); // triangulate

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

        db.saveundo(UD_STARTBLOCK,NULL);

        options.set(1,drawmode % 2);
        options.set(2,drawmode / 2);

        db.saveundo(UD_ENDBLOCK,NULL);

        draw(DM_NORMAL);

    }
    if (status == 108) // to status dialog
        ::state.sendevent(new NewCommandEvent(411,0));
    else if (status == 111)// triangulate
    {
        state.destroyselection(1);
        if ((e = triangulatedclone(1)) != 0)
        {   db.saveundo(UD_STARTBLOCK,NULL);
            db.geometry.add(e);
            db.geometry.del(this);
            db.saveundo(UD_ENDBLOCK,NULL);
        }
    }  
    else if (status == 109)// change surface properties
    {
        SurfaceProperties sp;
        sp.verify(this,0);
    }
#endif
}

void SurfaceOfRev::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{int i,j,nu,nv;
 Transform tr;
 Point ax1,p1,*verts;
 double dz;
  pmin->setxyz(0.0,0.0,0.0);  *pmax = *pmin;
  nfacets(surface == 0 ? 1.0 : (surface->getumax()-surface->getumin()) / surface->getwidth() * db.getdtoler(),&nu,&nv);
  if ((verts = new Point[nu+1]) == NULL) return;
  for (i = 0 ; i <= nu ; i++)
    verts[i] = e->position(double(i)/double(nu));
  dz = deltaz / double(nv);
  ax1 = axis * dz;
  tr.rotate(origin,axis,angle * M_PI / 180.0 / double(nv));
  for (j = 0 ; j <= nv ; j++)
    {  for (i = 0 ; i <= nu ; i++)
         {  if (surface == NULL)
              p1 = verts[i];
            else
              p1 = surface->modeltouv(verts[i]);
            if (i == 0 && j == 0)
              *pmin = *pmax = p1;
            else
              {  if (p1.x < pmin->x) pmin->x = p1.x;
                 if (p1.y < pmin->y) pmin->y = p1.y;
                 if (p1.z < pmin->z) pmin->z = p1.z;
                 if (p1.x > pmax->x) pmax->x = p1.x;
                 if (p1.y > pmax->y) pmax->y = p1.y;
                 if (p1.z > pmax->z) pmax->z = p1.z;
              }
            verts[i] = tr.transform(verts[i]) + ax1;
         }
    }
  delete [] verts;
}

int SurfaceOfRev::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(surfaceofrev_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int SurfaceOfRev::explode(void)
{ return 0;
}

int SurfaceOfRev::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int i,xm,ym,result,cstate;
 double wt;
 static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };

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
        else if (id == 10)
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
            sprintf(text,"%.4lf",angle,0);
        else if (id == 7)
            db.formatnumber(text,deltaz,FormatForEditField);
        else if (id == 8)
            sprintf(text,"%d",xmesh);
        else if (id == 9)
            sprintf(text,"%d",ymesh);
        else if (id == 10)
            strcpy(text,drawmodel[options.test(1) + options.test(2) * 2]);
        else if (id == 11)
            db.formatnumber(text,0.0,0);
    }
    else if (action == PP_SETVALUE)
    {   if (id == 6)
        {   change_mask.set(6,1);
            change(change_mask,*this,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),atof(text),0.0,0,0,new_options);
        }
        else if (id == 7)
        {Expression e(text);
         double x;
            change_mask.set(7,1);
            if (e.evaluate(&x))
                change(change_mask,*this,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),0.0,x * db.getmainunitscale(),0,0,new_options);
        }
        else if (id == 8)
        {   change_mask.set(8,1);
            xm = atoi(text);
            if (xm < 2)
                xm = 2;
            change(change_mask,*this,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),0.0,0.0,xm,0,new_options);
        }
        else if (id == 9)
        {   change_mask.set(9,1);
            ym = atoi(text);
            if (ym < 2)
                ym = 2;
            change(change_mask,*this,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),0.0,0.0,0,ym,new_options);
        }
        else if (id == 10)
        {   change_mask.set(10,1);
            for (i = 0 ; i < 4 ; i++)
                if (strcmp(text,drawmodel[i]) == 0)
                {   new_options.set(1,i&1);
                    new_options.set(2,i&2);
                    change(change_mask,*this,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),0.0,0.0,0,0,new_options);
                }
        }

    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Surface Of Rev");
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
                strcpy(text,"Angle");
            else if (id == 7)
                strcpy(text,"Delta Z");
            else if (id == 8)
                strcpy(text,"Horizontal Mesh");
            else if (id == 9)
                strcpy(text,"Vertical Mesh");
            else if (id == 10)
                strcpy(text,"Drawing Mode");
            else if (id == 11)
                strcpy(text,"Surface Area");
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 11;
    
    return 0;
}

