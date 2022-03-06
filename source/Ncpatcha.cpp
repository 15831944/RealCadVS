
#include "ncwin.h"
#include "patch_dialog.h"
#include "RCView.h"
#include "RCGraphicsItems.h"
//
//
//  Implementation of the 'Patch' class
//

Patch::Patch(int n1,int n2,int d1,int d2,int xm,int ym,Point *p,double *k1,double *k2,
             double *w,EntityList tcurves,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgHLines=0;
#endif

    int i;
  un = short(n1);  vn = short(n2);
  xmesh = short(xm);  ymesh = short(ym);
  udegree = short(d1);  vdegree = short(d2);
  if (un < udegree + 1) udegree = short(un - 1);
  if (vn < vdegree + 1) vdegree = short(vn - 1);
  if ((polygon = new Point[n1 * n2]) == NULL)
    {  defined = 0;  return;
    }
  uknot = utknot = utbasis = utdbasis = vtdbasis = 0;
  if (k1 != NULL && (uknot = new double[n1+d1+1]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  vknot = vtknot = vtbasis = NULL;
  if (k2 != NULL && (vknot = new double[n2+d2+1]) == NULL)
    {  delete [] polygon;  delete [] uknot;  defined = 0;  return;
    }
  cweight = NULL;
  if (w != NULL && (cweight = new double[n1 * n2]) == NULL)
    {  delete [] polygon;  delete [] uknot;  delete [] vknot;  defined = 0;  return;
    }
  for (i = 0 ; i < n1 * n2 ; i++) polygon[i] = p[i];
  if (k1 != NULL)
    {  for (i = 0 ; i < n1 + d1 + 1 ; i++) uknot[i] = k1[i];
    }
  else
    {  tempknots(1,&utknot);
       if (utknot == NULL)
         {  delete [] uknot;  delete[] vknot;  delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (k2 != NULL)
    {  for (i = 0 ; i < n2 + d2 + 1 ; i++) vknot[i] = k2[i];
    }
  else
    {  tempknots(0,&vtknot);
       if (vtknot == NULL)
         {  delete [] uknot;  delete [] vknot;
            delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (w != NULL)
    {  for (i = 0 ; i < n1 * n2 ; i++) cweight[i] = w[i];
    }

  trimcurves = tcurves.copy();
  ubasis = vbasis = -1;
  options = o;
  triangles = 0;
  ntriangles = 0;
  nvertices = 0;
  colour_map = 0;
  property_u_index = 0;
  property_v_index = 0;

}

Patch::Patch(const EntityHeader &header,int n1,int n2,int d1,int d2,int xm,int ym,Point *p,double *k1,double *k2,
             double *w,EntityList tcurves,const BitMask &o) : UVSurfaceEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgHLines=0;
#endif

    int i;
  un = short(n1);  vn = short(n2);
  udegree = short(d1);  vdegree = short(d2);
  xmesh = short(xm);  ymesh = short(ym);
  if (un < udegree + 1) udegree = short(un - 1);
  if (vn < vdegree + 1) vdegree = short(vn - 1);
  if ((polygon = new Point[n1 * n2]) == NULL)
    {  defined = 0;  return;
    }
  uknot = utknot = utbasis = utdbasis = vtdbasis = 0;
  if (k1 != NULL && (uknot = new double[n1+d1+1]) == NULL)
    {  delete [] polygon;  defined = 0;  return;
    }
  vknot = vtknot = vtbasis = NULL;
  if (k1 != NULL && (vknot = new double[n2+d2+1]) == NULL)
    {  delete [] polygon;  delete [] uknot;  defined = 0;  return;
    }
  cweight = NULL;
  if (w != NULL && (cweight = new double[n1 * n2]) == NULL)
    {  delete [] polygon;  delete [] uknot;  delete [] vknot;  defined = 0;  return;
    }
  for (i = 0 ; i < n1 * n2 ; i++) polygon[i] = p[i];
  if (k1 != NULL)
    {  for (i = 0 ; i < n1 + d1 + 1 ; i++) uknot[i] = k1[i];
    }
  else
    {  tempknots(1,&utknot);
       if (utknot == NULL)
         {  delete [] uknot;  delete[] vknot;  delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (k2 != NULL)
    {  for (i = 0 ; i < n2 + d2 + 1 ; i++) vknot[i] = k2[i];
    }
  else
    {  tempknots(0,&vtknot);
       if (vtknot == NULL)
         {  delete [] uknot;  delete [] vknot;  delete [] utknot;
            delete [] polygon;  delete [] cweight;
            defined = 0;
            return;
         }
    }
  if (w != NULL)
    {  for (i = 0 ; i < n1 * n2 ; i++) cweight[i] = w[i];
    }

  trimcurves = tcurves;
  triangles = 0;
  ntriangles = 0;
  nvertices = 0;
  ubasis = vbasis = -1;
  options = o;
  colour_map = 0;
  property_u_index = 0;
  property_v_index = 0;

}

Patch::Patch(long nv,Point *p,long ni,long *t,const BitMask &o) : options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgHLines=0;
#endif

    options = o;
    options.set(PatchTriangulated);

    nvertices = nv;
    if ((polygon = new Point[nv]) != 0)
        memcpy(polygon,p,sizeof(Point) * nv);
    else
    {  defined = 0;
       return;
    }
    
    nindex = ni;
    if ((index = new long[ni]) != 0)
        memcpy(index,t,sizeof(long) * ni);
    else
    {  defined = 0;
       return;
    }

    //polygon=0;
    //index=0;
    colour_map = 0;
    property_u_index = 0;
    property_v_index = 0;
        
}

Patch::Patch(const EntityHeader &header,long nv,Point *p,long ni,long *i,const BitMask &o)  : UVSurfaceEntity(header),options(32)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgHLines=0;
#endif

    options = o;
    options.set(PatchTriangulated);

    nvertices = nv;
    if ((polygon = new Point[nv]) != 0)
        memcpy(polygon,p,sizeof(Point) * nv);
    else
    {  defined = 0;
       return;
    }
    
    nindex = ni;
    if ((index = new long[ni]) != 0)
        memcpy(index,i,sizeof(long) * ni);
    else
    {  defined = 0;
       return;
    }

    colour_map = 0;
    property_u_index = 0;
    property_v_index = 0;
        
}

Patch::~Patch()
{   if (defined)
    {   if (options.test(PatchTriangulated))
        {   delete [] polygon;
            delete [] index;
            delete [] colour_map;
        }
        else
        {   delete [] polygon;
            delete [] uknot;
            delete [] utknot;
            delete [] vknot;
            delete [] vtknot;
            delete [] cweight;
            delete [] utbasis;
            delete [] vtbasis;
            delete [] colour_map;
        }
    }
}

void Patch::set_colour_map(int index,long c)
{   if (colour_map == 0)
        colour_map = new long[npoints()];
    colour_map[index] = c;
}

void Patch::clear_colour_map(void)
{   delete [] colour_map;
    colour_map = 0;
}

void Patch::tempknots(int u,double **tknot)
{int i;
  if (u != 0)
    {  if ((*tknot = new double[un+udegree+1]) == NULL) return;
       for (i = 0 ; i < udegree + 1 ; i++)
         (*tknot)[i] = 0.0;
       for (i = udegree + 1 ; i < un ; i++)
         (*tknot)[i] = double(i - udegree) / double(un - udegree);
       for (i = un ; i < un + udegree + 1 ; i++)
         (*tknot)[i] = 1.0;
    }
  else
    {  if ((*tknot = new double[vn+vdegree+1]) == NULL) return;
       for (i = 0 ; i < vdegree + 1 ; i++)
         (*tknot)[i] = 0.0;
       for (i = vdegree + 1 ; i < vn ; i++)
         (*tknot)[i] = double(i - vdegree) / double(vn - vdegree);
       for (i = vn ; i < vn + vdegree + 1 ; i++)
         (*tknot)[i] = 1.0;
    }
}


// 
//  Add to display list callback
//
static void add_edge_display_list(View3dSurface *vs,Point p1,Point p2,void *)
{   p1 = vs->modeltoview(p1);
    p2 = vs->modeltoview(p2);
    if (vs->clip3dfb(&p1,&p2))
    {   p1 = vs->viewtoscreen(p1);
        p2 = vs->viewtoscreen(p2);
        if (! vs->displaylist.moveto(p1.x,p1.y))
            return;  
        if (! vs->displaylist.lineto(p2.x,p2.y))
            return;
    }
}

// 
//  Draw callback
//
#ifdef USING_WIDGETS
static void draw_edgeGL(View3dSurface *vs,Point p1,Point p2,void *)
{
    QGraphicsItem *qgedge=0;

    //p1 = vs->modeltoview(p1);
    //p2 = vs->modeltoview(p2);
    //if (vs->clip3d(&p1,&p2))
    {
        //p1 = vs->viewtoscreen(p1);
        //p2 = vs->viewtoscreen(p2);
        p1 = p1;
        p2 = p2;

        vs->lineGL(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
    }
}

static void draw_edge(View3dSurface *vs,Point p1,Point p2,void *cbdata)
{
    QGraphicsItem *qgedge=0;

    p1 = vs->modeltoview(p1);
    p2 = vs->modeltoview(p2);
    if (vs->clip3d(&p1,&p2))
    {   p1 = vs->viewtoscreen(p1);
        p2 = vs->viewtoscreen(p2);

        if(cbdata == 0)
            vs->line(p1.x,p1.y,p2.x,p2.y);
        else
        {
            qgedge=0;
            qgedge = vs->line((QGraphicsItem*)0,p1.x,p1.y,p2.x,p2.y);
            if(qgedge)
                ((QGraphicsItemGroup*)cbdata)->addToGroup(qgedge);
        }
    }
}
#else
static void draw_edge(View3dSurface *vs,Point p1,Point p2,void *)
{   p1 = vs->modeltoview(p1);
    p2 = vs->modeltoview(p2);
    if (vs->clip3d(&p1,&p2))
    {   p1 = vs->viewtoscreen(p1);
        p2 = vs->viewtoscreen(p2);
        vs->line(int(p1.x),int(p1.y),int(p2.x),int(p2.y));
    }
}
#endif

// 
//  Hide callback
//
struct HideData
{HideImage *image;
 Entity *entity;
};

static void hide_edge(View3dSurface *vs,Point p1,Point p2,void *image)
{   ((struct HideData *)image)->image->hide(((struct HideData *)image)->entity,p1,p2);
}

static void hide_edgeGL(View3dSurface *vs,Point p1,Point p2,void *image)
{   ((struct HideData *)image)->image->hideGL(((struct HideData *)image)->entity,p1,p2);
}

// 
//  Pick callback
//
struct PickData
{Point *pos,*pwvertices;
 int inside,nvertices;
 double x,y,d;
 int x1,y1,x2,y2;
};

static void pick_edge(View3dSurface *vs,Point p1,Point p2,void *pickdata)
{double d;
    if ((d = vs->pick(p1,p2,((struct PickData *)pickdata)->x,((struct PickData *)pickdata)->y,((struct PickData *)pickdata)->pos)) >= 0.0)
        ((struct PickData *)pickdata)->d = d;
}

static void pick_window_edge(View3dSurface *vs,Point p1,Point p2,void *pickdata)
{   if (! vs->pick(p1,((struct PickData *)pickdata)->inside,((struct PickData *)pickdata)->x1,((struct PickData *)pickdata)->y1,((struct PickData *)pickdata)->x2,((struct PickData *)pickdata)->y2))
        ((struct PickData *)pickdata)->d = 0.0;
}


static void pick_polygon_edge(View3dSurface *vs,Point p1,Point p2,void *pickdata)
{int picked;
    if (vs->pick(((struct PickData *)pickdata)->nvertices,((struct PickData *)pickdata)->pwvertices,p1,p2,((struct PickData *)pickdata)->inside,&picked))
        ((struct PickData *)pickdata)->d = picked;
}


void Patch::generate_triangulated_patch_edges(View3dSurface *vs,Transform *t,PatchCallBack pcb,void *cbdata)
{
    int i;

    if (t != 0)
    {Point p1,p2,p3;
        for (i = 0 ; i < nindex ; i += 3)
        {   
            p1 = t->transform(polygon[index[i]]);
            p2 = t->transform(polygon[index[i+1]]);
            p3 = t->transform(polygon[index[i+2]]);

            pcb(vs,p1,p2,cbdata);
            pcb(vs,p2,p3,cbdata);
            pcb(vs,p3,p1,cbdata);
        }
    }
    else
    {   for (i = 0 ; i < nindex ; i += 3)
        {   pcb(vs,polygon[index[i]],polygon[index[i+1]],cbdata);
            pcb(vs,polygon[index[i+1]],polygon[index[i+2]],cbdata);
            pcb(vs,polygon[index[i+2]],polygon[index[i]],cbdata);
        }
    }


}

void Patch::delete_triangulated_patch_vertex(int n,int keep,int *delete_index)
{int i,j;


    db.saveundo(UD_MOVED,this);
    draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(this);

    for (i = 0 ; i < nindex ; i += 3)
    {   for (j = 0 ; j < n ; j++)
        {   if (index[i] == delete_index[j] || index[i+1] == delete_index[j] || index[i+2] == delete_index[j])
            {   index[i] |= 0x80000000;
                index[i+1] |= 0x80000000;
                index[i+2] |= 0x80000000;
            }
        }
    }

    for (i = 0,j = 0 ; j < nindex ; )
        if (!keep && (index[j]&0x80000000) == 0 || keep && (index[j]&0x80000000) != 0)
            index[i++] = index[j++]&0x7FFFFFF;
        else
            j++;

    nindex -= (j - i);

    draw(DM_NORMAL);

    if (getposition() != 0 && db.MovedCallback != 0)
        db.MovedCallback(this);

}


void Patch::addtodisplaylist(View3dSurface *vs,int child)
{Point pt1,pt2,pt3,pt4,*row1,*row2,*row3;
 int i,j,useg,vseg,xm,ym,edgemode;
 double u,v;


    if (options.test(PatchTriangulated))
    {   
        if (! child)
        {   vs->displaylist.setsavemode();
            if (! vs->displaylist.start(this,nindex * 34)) 
                return;
        }

        generate_triangulated_patch_edges(vs,0,add_edge_display_list,0);

        if (! child && ! vs->displaylist.end()) 
            return;

    }
    else
    {   nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);
        if (! child)
        {   vs->displaylist.setsavemode();
            if (! vs->displaylist.start(this,((xmesh + 3) * vseg + (ymesh + 3) * useg) * 34)) 
                return;
        }
        newbasis();

        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return;
        }

        edgemode = options.test(1) + options.test(2) * 2;

        row1 = row2 = row3 = 0;

        if (edgemode != HLNormal)
        {   if (useg > vseg)
            {   row1 = new Point[useg+1];
                row2 = new Point[useg+1];
                row3 = new Point[useg+1];
            }
            else
            {   row1 = new Point[vseg+1];
                row2 = new Point[vseg+1];
                row3 = new Point[vseg+1];
            }

            for (i = 0 ; i <= useg ; i++)
            {   double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
                u = double(i)/double(useg);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                for (j = 0 ; j <= vseg ; j++)
                    row3[j] = position(u,double(j) / double(vseg));


                if (i >= 1)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x5 = vs->modeltoscreen(row2[j]).x;
                        y5 = vs->modeltoscreen(row2[j]).y;
                        x6 = vs->modeltoscreen(row3[j]).x;
                        y6 = vs->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                        {   pt3 = vs->modeltoview(row2[j]);
                            pt4 = vs->modeltoview(row3[j-1]);
                            if (vs->clip3dfb(&pt3,&pt4))
                            {   pt3 = vs->viewtoscreen(pt3);
                                pt4 = vs->viewtoscreen(pt4);
                                if (! vs->displaylist.moveto(pt3.x,pt3.y))
                                {   deletebasis();  ubasis = -1;  
                                    return;  
                                }
                                if (! vs->displaylist.lineto(pt4.x,pt4.y))
                                {   deletebasis();  ubasis = -1;  
                                    return;
                                }
                            }
                        }

                        if (j >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   pt3 = vs->modeltoview(row2[j-1]);
                            pt4 = vs->modeltoview(row3[j-1]);
                            if (vs->clip3dfb(&pt3,&pt4))
                            {   pt3 = vs->viewtoscreen(pt3);
                                pt4 = vs->viewtoscreen(pt4);
                                if (! vs->displaylist.moveto(pt3.x,pt3.y))
                                {   deletebasis();  ubasis = -1;  
                                    return;  
                                }
                                if (! vs->displaylist.lineto(pt4.x,pt4.y))
                                {   deletebasis();  ubasis = -1;  return;  
                                }
                            }
                        }

                        x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                        y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;
                   }
                } 
               

                if (i >= 2)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x2 = vs->modeltoscreen(row1[j]).x;
                        y2 = vs->modeltoscreen(row1[j]).y;
                        x4 = vs->modeltoscreen(row2[j]).x;
                        y4 = vs->modeltoscreen(row2[j]).y;
                        x6 = vs->modeltoscreen(row3[j]).x;
                        y6 = vs->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   pt3 = vs->modeltoview(row2[j]);
                            pt4 = vs->modeltoview(row2[j-1]);
                            if (vs->clip3dfb(&pt3,&pt4))
                            {   pt3 = vs->viewtoscreen(pt3);
                                pt4 = vs->viewtoscreen(pt4);
                                if (! vs->displaylist.moveto(pt3.x,pt3.y))
                                {  deletebasis();  ubasis = -1;  return;  
                                }
                                if (! vs->displaylist.lineto(pt4.x,pt4.y))
                                {  deletebasis();  ubasis = -1;  return;  
                                }
                            }
                        }
                        x1 = x2;  x3 = x4;  x5 = x6;
                        y1 = y2;  y3 = y4;  y5 = y6;
                    }
                } 


                for (j = 0 ; j <= vseg ; j++)
                {   row1[j] = row2[j];
                    row2[j] = row3[j];
                }
            }
        }

        if (edgemode != HLEdgesOnly)
        {   if (edgemode == HLEdgesBorder)
            {   xm = 2;
                ym = 2;
            }
            else
            {   xm = xmesh;
                ym = ymesh;
            }

            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                pt1 = vs->modeltoview(position(u,0.0));
                for (j = 1 ; j <= vseg ; j++)
                {   pt2 = vs->modeltoview(position(u,double(j) / double(vseg)));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3dfb(&pt3,&pt4))
                    {   pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);
                        if (! vs->displaylist.moveto(pt3.x,pt3.y))
                        {   deletebasis();  ubasis = -1;  
                            return;  
                        }
                        if (! vs->displaylist.lineto(pt4.x,pt4.y))
                        {   deletebasis();  ubasis = -1;  
                            return;  
                        }
                    }
                    pt1 = pt2;
                }
            }
    
            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                pt1 = vs->modeltoview(position(0.0,v));
                for (i = 1 ; i <= useg ; i++)
                {   pt2 = vs->modeltoview(position(double(i) / double(useg),v));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3dfb(&pt3,&pt4))
                    {   pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);
                        if (! vs->displaylist.moveto(pt3.x,pt3.y))
                        {   deletebasis();  vbasis = -1;  
                            return;  
                        }
                        if (! vs->displaylist.lineto(pt4.x,pt4.y))
                        {   deletebasis();  vbasis = -1;  
                            return; 
                        }
                    }
                    pt1 = pt2;
                }
            }
            vbasis = -1;
        }
        if (! child && ! vs->displaylist.end()) 
            return;
        deletebasis();

        delete [] row1,row2,row3;

    }

}

void Patch::newbasis(void)
{ utbasis = new double[un + udegree + 1];
  vtbasis = new double[vn + vdegree + 1];
}

void Patch::deletebasis(void)
{ delete [] utbasis;  utbasis = 0;
  delete [] vtbasis;  vtbasis = 0;
}

void Patch::newdbasis(void)
{ utdbasis = new double[un + udegree];
  vtdbasis = new double[vn + vdegree];
}

void Patch::deletedbasis(void)
{ delete [] utdbasis;  utdbasis = 0;
  delete [] vtdbasis;  vtdbasis = 0;
}

void Patch::rdbasis(int degree,int n,double *knot,double t,double *basis,double *dbasis,int *base) const
{int i,k,l,npluso;
 double b1,b2,f1,f2,f3,f4;
  npluso = n + degree + 1;
  *base = n + degree - 1;
  for (i = 0 ; i < npluso - 1 ; i++)
    {  basis[i] = t >= knot[i] && t < knot[i+1];
       if (basis[i]) *base = i;
       dbasis[i] = 0.0;
    }
  if (fabs(t - knot[npluso - 1]) < db.getptoler())
    {  basis[n-1] = 1.0;
       basis[n] = 0.0;
    }
  for (k = 2 ; k <= degree + 1 ; k++)
    {  l = *base;  if (*base > n + degree - k) l = n + degree - k;
       for (i = *base - k + 1 ; i <= l ; i++)
         {  if (fabs(basis[i]) > db.getptoler())
              b1 = (t - knot[i]) * basis[i] / (knot[i + k - 1] - knot[i]);
            else
              b1 = 0.0;
            if (fabs(basis[i+1]) > db.getptoler())
              b2 = (knot[i+k] - t) * basis[i+1] / (knot[i+k] - knot[i+1]);
            else
              b2 = 0.0;
            if (fabs(basis[i]) > db.getptoler())
              f1 = basis[i] / (knot[i+k-1] - knot[i]);
            else
              f1 = 0.0;
            if (fabs(basis[i+1]) > db.getptoler())
              f2 = -basis[i+1] / (knot[i+k] - knot[i+1]);
            else
              f2 = 0.0;
            if (fabs(dbasis[i]) > db.getptoler())
              f3 = (t - knot[i])*dbasis[i] / (knot[i+k-1] - knot[i]);
            else
              f3 = 0.0;
            if (fabs(dbasis[i+1]) > db.getptoler())
              f4 = (knot[i+k] - t)*dbasis[i+1] / (knot[i+k] - knot[i+1]);
            else
              f4 = 0.0;
            basis[i] = b1 + b2;
            dbasis[i] = f1 + f2 + f3 + f4;
         }
    }
}

void Patch::rbasis(int degree,int n,double *knot,double t,double *basis,int *base) const
{int i,k,l,npluso;
 double d,e;
  npluso = n + degree + 1;
  *base = n + degree - 1;
  for (i = 0 ; i < npluso - 1 ; i++)
    {  basis[i] = t >= knot[i] && t < knot[i+1];
       if (basis[i]) *base = i;
    }
  for (k = 2 ; k <= degree + 1 ; k++)
    {  l = *base;  if (*base > n + degree - k) l = n + degree - k;
       for (i = *base - k + 1 ; i <= l ; i++)
         {  if (fabs(basis[i]) > db.getptoler())
              d = (t - knot[i]) * basis[i] / (knot[i + k - 1] - knot[i]);
            else
              d = 0.0;
            if (fabs(basis[i+1]) > db.getptoler())
              e = (knot[i+k] - t) * basis[i+1] / (knot[i+k] - knot[i+1]);
            else
              e = 0.0;
            basis[i] = d + e;
         }
    }
  if (fabs(t - knot[npluso - 1]) < db.getptoler()) basis[n-1] = 1.0;
}

#ifdef USING_WIDGETS
void Patch::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
    Point pt1,pt2,pt3,pt4,*row1,*row2,*row3;
    int i,j,useg,vseg,xm,ym,edgemode;
    double u,v;

    draw(DM_ERASE,t,vs);

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
    if (t == 0)
    {
        if (vs->displaylist.draw(this,drawmode)) return;
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
        int nlights = 0;
        Entity *e;
        int k,tx;
        SurfaceProperties *sp;


        if(isvisible(vs))
        {
            double ambient = ::v.getreal("sh::ambient");
            double diffuse = ::v.getreal("sh::diffuse");

            glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

            Transform mt;
            if(t)
                mt = vs->modeltransform.apply(*t);
            else
                mt = vs->modeltransform;

           if ( getntriangles() <= 0)
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
                          LoadTexture(sp->tmi.filename,1.0,1.0,1.0,0.004,1.0-triangles[j].y,&tx);
                          if (tx == -1)
                            {  SetTexture(0);
                               SetMaterialEx(triangles[j+1].x/255.0,triangles[j+1].y/255.0,triangles[j+1].z/255.0,ambient,diffuse,1.0,triangles[j].y);
                            }
                          else
                            {  SetTexture(tx);
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
#if 0
        QPointF qp1,qp2;
        QPainterPath qclipPath;
        qclipPath.setFillRule(Qt::OddEvenFill);

        db.setrequiresopengl(1);

        // this will draw using OpenGL with no clipping
        qpatch = new RCGraphicsPatchItem(this,vs);

        if(qpatch)
        {
            qpatch->rcdrawmode = drawmode;

            newbasis();
            if (utbasis == 0 || vtbasis == 0)
            {
                deletebasis();
                return;
            }

            nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);

            xm = 2;
            ym = 2;

            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(u,0.0)));
                else
                    pt1 = vs->modeltoview(position(u,0.0));
                for (j = 1 ; j <= vseg ; j++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(u,double(j) / double(vseg))));
                    else
                        pt2 = vs->modeltoview(position(u,double(j) / double(vseg)));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {
                        pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);

                        qp1 = ((RCView*)vs->getWidget())->view()->mapToScene(pt3.x,vs->getheight()-pt3.y);
                        qp2 = ((RCView*)vs->getWidget())->view()->mapToScene(pt4.x,vs->getheight()-pt4.y);

                        //qclipPath.moveTo(pt3.x,vs->getheight()-pt3.y);
                        //qclipPath.lineTo(pt4.x,vs->getheight()-pt4.y);
                        qclipPath.moveTo(qp1.x(),qp1.y());
                        qclipPath.lineTo(qp2.x(),qp2.y());
                    }
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(0.0,v)));
                else
                    pt1 = vs->modeltoview(position(0.0,v));
                for (i = 1 ; i <= useg ; i++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(double(i) / double(useg),v)));
                    else
                        pt2 = vs->modeltoview(position(double(i) / double(useg),v));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {
                        pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);

                        qp1 = ((RCView*)vs->getWidget())->view()->mapToScene(pt3.x,vs->getheight()-pt3.y);
                        qp2 = ((RCView*)vs->getWidget())->view()->mapToScene(pt4.x,vs->getheight()-pt4.y);

                        //qclipPath.moveTo(pt3.x,vs->getheight()-pt3.y);
                        //qclipPath.lineTo(pt4.x,vs->getheight()-pt4.y);
                        qclipPath.moveTo(qp1.x(),qp1.y());
                        qclipPath.lineTo(qp2.x(),qp2.y());
                    }
                    pt1 = pt2;
                }
            }

            vbasis = -1;
            deletebasis();

            //delete [] row1,row2,row3;

            qpatch->qpath = qclipPath;
        }
#endif
    }
    else if (options.test(PatchTriangulated))
    {
        generate_triangulated_patch_edges(vs,t,draw_edgeGL,0);
    }
    else
    {
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return;
        }
        nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);

        // debug
        //options.clear(1);
        //options.clear(2);
        //
        edgemode = options.test(1) + options.test(2) * 2;

        row1 = row2 = row3 = 0;

        if (edgemode != HLNormal)
        {   if (useg > vseg)
            {   row1 = new Point[useg+1];
                row2 = new Point[useg+1];
                row3 = new Point[useg+1];
            }
            else
            {   row1 = new Point[vseg+1];
                row2 = new Point[vseg+1];
                row3 = new Point[vseg+1];
            }

            for (i = 0 ; i <= useg ; i++)
            {double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
                u = double(i)/double(useg);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                for (j = 0 ; j <= vseg ; j++)
                    row3[j] = position(u,double(j) / double(vseg));

                if (i >= 1)
                {   for (j = 0 ; j <= vseg ; j++)
                    {
                        //x5 = vs->modeltoscreen(row2[j]).x;
                        //y5 = vs->modeltoscreen(row2[j]).y;
                        //x6 = vs->modeltoscreen(row3[j]).x;
                        //y6 = vs->modeltoscreen(row3[j]).y;

                        x5 = row2[j].x;
                        y5 = row2[j].y;
                        x6 = row3[j].x;
                        y6 = row3[j].y;

                        if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                        {   if (t != 0)
                                vs->line3d(vs->modeltransform.transform(t->transform(row2[j])),vs->modeltransform.transform(t->transform(row3[j-1])));
                            else
                                vs->line3d(vs->modeltransform.transform(row2[j]),vs->modeltransform.transform(row3[j-1]));
                        }

                        if (j >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   if (t != 0)
                                vs->line3dGL(vs->modeltransform.transform(t->transform(row2[j-1])),vs->modeltransform.transform(t->transform(row3[j-1])));
                            else
                                vs->line3dGL(vs->modeltransform.transform(row2[j-1]),vs->modeltransform.transform(row3[j-1]));
                        }

                        x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                        y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;
                    }
                }


                if (i >= 2)
                {   for (j = 0 ; j <= vseg ; j++)
                    {
                        //x2 = vs->modeltoscreen(row1[j]).x;
                        //y2 = vs->modeltoscreen(row1[j]).y;
                        //x4 = vs->modeltoscreen(row2[j]).x;
                        //y4 = vs->modeltoscreen(row2[j]).y;
                        //x6 = vs->modeltoscreen(row3[j]).x;
                        //y6 = vs->modeltoscreen(row3[j]).y;

                        x2 = row1[j].x;
                        y2 = row1[j].y;
                        x4 = row2[j].x;
                        y4 = row2[j].y;
                        x6 = row3[j].x;
                        y6 = row3[j].y;

                        if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   if (t != 0)
                                vs->line3dGL(vs->modeltransform.transform(t->transform(row2[j])),vs->modeltransform.transform(t->transform(row2[j-1])));
                            else
                                vs->line3dGL(vs->modeltransform.transform(row2[j]),vs->modeltransform.transform(row2[j-1]));
                        }
                        x1 = x2;  x3 = x4;  x5 = x6;
                        y1 = y2;  y3 = y4;  y5 = y6;
                    }
                }


                for (j = 0 ; j <= vseg ; j++)
                {   row1[j] = row2[j];
                    row2[j] = row3[j];
                }

            }
        }

        if (edgemode != HLEdgesOnly)
        {   if (edgemode == HLEdgesBorder)
            {   xm = 2;
                ym = 2;
            }
            else
            {   xm = xmesh;
                ym = ymesh;
            }

            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                if (t != 0)
                    pt1 = t->transform(position(u,0.0));
                    //pt1 = vs->modeltoview(t->transform(position(u,0.0)));
                else
                    pt1 = position(u,0.0);
                    //pt1 = vs->modeltoview(position(u,0.0));
                for (j = 1 ; j <= vseg ; j++)
                {   if (t != 0)
                        pt2 = t->transform(position(u,double(j) / double(vseg)));
                        //pt2 = vs->modeltoview(t->transform(position(u,double(j) / double(vseg))));
                    else
                        pt2 = position(u,double(j) / double(vseg));
                        //pt2 = vs->modeltoview(position(u,double(j) / double(vseg)));
                    pt3 = pt1;  pt4 = pt2;
                    //if (vs->clip3d(&pt3,&pt4))
                    {
                        //pt3 = vs->viewtoscreen(pt3);
                        //pt4 = vs->viewtoscreen(pt4);
                        pt3 = vs->modeltransform.transform(pt3);
                        pt4 = vs->modeltransform.transform(pt4);

                        vs->lineGL(pt3.x,pt3.y,pt3.z,pt4.x,pt4.y,pt4.z);
                    }
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                if (t != 0)
                    pt1 = t->transform(position(0.0,v));
                    //pt1 = vs->modeltoview(t->transform(position(0.0,v)));
                else
                    pt1 = position(0.0,v);
                    //pt1 = vs->modeltoview(position(0.0,v));
                for (i = 1 ; i <= useg ; i++)
                {
                    if (t != 0)
                        pt2 = t->transform(position(double(i) / double(useg),v));
                        //pt2 = vs->modeltoview(t->transform(position(double(i) / double(useg),v)));
                    else
                        pt2 = position(double(i) / double(useg),v);
                        //pt2 = vs->modeltoview(position(double(i) / double(useg),v));
                    pt3 = pt1;  pt4 = pt2;
                    //if (vs->clip3d(&pt3,&pt4))
                    {
                        //pt3 = vs->viewtoscreen(pt3);
                        //pt4 = vs->viewtoscreen(pt4);
                        pt3 = vs->modeltransform.transform(pt3);
                        pt4 = vs->modeltransform.transform(pt4);

                        vs->lineGL(pt3.x,pt3.y,pt3.z,pt4.x,pt4.y,pt4.z);

                    }
                    pt1 = pt2;
                }
            }
        }
        vbasis = -1;
        deletebasis();

        delete [] row1,row2,row3;
    }
}
#endif

void Patch::draw(int drawmode,Transform *t,View3dSurface *vs)
{
    Point pt1,pt2,pt3,pt4,*row1,*row2,*row3;
    int i,j,useg,vseg,xm,ym,edgemode;
    double u,v;

    RCGraphicsPatchItem *qpatch=0;
    QGraphicsItemGroup *qgsurface = 0;
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

    qgsurface = new QGraphicsItemGroup();

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

    vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

#if 0
    if (t == 0)
    {
        if (vs->displaylist.draw(this,drawmode)) return;
        if (! vs->displaylist.geterrorstatus())
            addtodisplaylist(vs,0);
        if (vs->displaylist.draw(this,drawmode)) return;
    }
#endif

    //if(0)
    //if (::v.getinteger("sh::openglshading") && SetLight != 0)
    if ( vs->getrepaintstyle() == Shaded && OpenOpenGl != 0 && drawmode == DM_GL && t == 0)
    {
        QPointF qp1,qp2;
        QPainterPath qclipPath;
        qclipPath.setFillRule(Qt::OddEvenFill);

        db.setrequiresopengl(1);

        // this will draw using OpenGL with no clipping
        qpatch = new RCGraphicsPatchItem(this,vs);

        if(qpatch)
        {
            qpatch->rcdrawmode = drawmode;

            newbasis();
            if (utbasis == 0 || vtbasis == 0)
            {
                deletebasis();
                delete qgsurface;
                delete qpatch;
                return;
            }

            nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);

            xm = 2;
            ym = 2;

            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(u,0.0)));
                else
                    pt1 = vs->modeltoview(position(u,0.0));
                for (j = 1 ; j <= vseg ; j++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(u,double(j) / double(vseg))));
                    else
                        pt2 = vs->modeltoview(position(u,double(j) / double(vseg)));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {
                        pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);

                        qp1 = ((RCView*)vs->getWidget())->view()->mapToScene(pt3.x,vs->getheight()-pt3.y);
                        qp2 = ((RCView*)vs->getWidget())->view()->mapToScene(pt4.x,vs->getheight()-pt4.y);

                        //qclipPath.moveTo(pt3.x,vs->getheight()-pt3.y);
                        //qclipPath.lineTo(pt4.x,vs->getheight()-pt4.y);
                        qclipPath.moveTo(qp1.x(),qp1.y());
                        qclipPath.lineTo(qp2.x(),qp2.y());
                    }
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(0.0,v)));
                else
                    pt1 = vs->modeltoview(position(0.0,v));
                for (i = 1 ; i <= useg ; i++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(double(i) / double(useg),v)));
                    else
                        pt2 = vs->modeltoview(position(double(i) / double(useg),v));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {
                        pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);

                        qp1 = ((RCView*)vs->getWidget())->view()->mapToScene(pt3.x,vs->getheight()-pt3.y);
                        qp2 = ((RCView*)vs->getWidget())->view()->mapToScene(pt4.x,vs->getheight()-pt4.y);

                        //qclipPath.moveTo(pt3.x,vs->getheight()-pt3.y);
                        //qclipPath.lineTo(pt4.x,vs->getheight()-pt4.y);
                        qclipPath.moveTo(qp1.x(),qp1.y());
                        qclipPath.lineTo(qp2.x(),qp2.y());
                    }
                    pt1 = pt2;
                }
            }

            vbasis = -1;
            deletebasis();

            //delete [] row1,row2,row3;

            qpatch->qpath = qclipPath;
        }
    }
    else if (options.test(PatchTriangulated))
    {
        //qgsurface = new QGraphicsItemGroup();
        if(vs->getScene())
            vs->getScene()->addItem(qgsurface);

        generate_triangulated_patch_edges(vs,t,draw_edge,qgsurface);
    }
    else
    {
        //qgsurface = new QGraphicsItemGroup();
        if(vs->getScene())
            vs->getScene()->addItem(qgsurface);

        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return;
        }
        nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);

        // debug
        //options.clear(1);
        //options.clear(2);
        //
        edgemode = options.test(1) + options.test(2) * 2;

        row1 = row2 = row3 = 0;

        if (edgemode != HLNormal)
        {   if (useg > vseg)
            {   row1 = new Point[useg+1];
                row2 = new Point[useg+1];
                row3 = new Point[useg+1];
            }
            else
            {   row1 = new Point[vseg+1];
                row2 = new Point[vseg+1];
                row3 = new Point[vseg+1];
            }

            for (i = 0 ; i <= useg ; i++)
            {double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
                u = double(i)/double(useg);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                for (j = 0 ; j <= vseg ; j++)
                    row3[j] = position(u,double(j) / double(vseg));

                if (i >= 1)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x5 = vs->modeltoscreen(row2[j]).x;
                        y5 = vs->modeltoscreen(row2[j]).y;
                        x6 = vs->modeltoscreen(row3[j]).x;
                        y6 = vs->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                        {   if (t != 0)
                                qgline = vs->line3d((QGraphicsItem*)0,t->transform(row2[j]),t->transform(row3[j-1]));
                            else
                                qgline = vs->line3d((QGraphicsItem*)0,row2[j],row3[j-1]);

                            if(qgline)
                                qgsurface->addToGroup(qgline);
                        }

                        if (j >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   if (t != 0)
                                qgline = vs->line3d((QGraphicsItem*)0,t->transform(row2[j-1]),t->transform(row3[j-1]));
                            else
                                qgline = vs->line3d((QGraphicsItem*)0,row2[j-1],row3[j-1]);

                            if(qgline)
                                qgsurface->addToGroup(qgline);
                        }

                        x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                        y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;
                    }
                } 
               

                if (i >= 2)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x2 = vs->modeltoscreen(row1[j]).x;
                        y2 = vs->modeltoscreen(row1[j]).y;
                        x4 = vs->modeltoscreen(row2[j]).x;
                        y4 = vs->modeltoscreen(row2[j]).y;
                        x6 = vs->modeltoscreen(row3[j]).x;
                        y6 = vs->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                        {   if (t != 0)
                                qgline = vs->line3d((QGraphicsItem*)0,t->transform(row2[j]),t->transform(row2[j-1]));
                            else
                                qgline = vs->line3d((QGraphicsItem*)0,row2[j],row2[j-1]);

                            if(qgline)
                                qgsurface->addToGroup(qgline);
                        }
                        x1 = x2;  x3 = x4;  x5 = x6;
                        y1 = y2;  y3 = y4;  y5 = y6;
                    }
                } 


                for (j = 0 ; j <= vseg ; j++)
                {   row1[j] = row2[j];
                    row2[j] = row3[j];
                }
            
            }
        }

        if (edgemode != HLEdgesOnly)
        {   if (edgemode == HLEdgesBorder)
            {   xm = 2;
                ym = 2;
            }
            else
            {   xm = xmesh;
                ym = ymesh;
            }

            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(u,0.0)));
                else
                    pt1 = vs->modeltoview(position(u,0.0));
                for (j = 1 ; j <= vseg ; j++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(u,double(j) / double(vseg))));
                    else
                        pt2 = vs->modeltoview(position(u,double(j) / double(vseg)));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {   pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);
                        qgline = vs->line((QGraphicsItem*)0,pt3.x,pt3.y,pt4.x,pt4.y);

                        if(qgline)
                            qgsurface->addToGroup(qgline);
                    }
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                if (t != 0)
                    pt1 = vs->modeltoview(t->transform(position(0.0,v)));
                else
                    pt1 = vs->modeltoview(position(0.0,v));
                for (i = 1 ; i <= useg ; i++)
                {   if (t != 0)
                        pt2 = vs->modeltoview(t->transform(position(double(i) / double(useg),v)));
                    else
                        pt2 = vs->modeltoview(position(double(i) / double(useg),v));
                    pt3 = pt1;  pt4 = pt2;
                    if (vs->clip3d(&pt3,&pt4))
                    {   pt3 = vs->viewtoscreen(pt3);
                        pt4 = vs->viewtoscreen(pt4);
                        qgline = vs->line((QGraphicsItem*)0,pt3.x,pt3.y,pt4.x,pt4.y);

                        if(qgline)
                            qgsurface->addToGroup(qgline);
                    }
                    pt1 = pt2;
                }
            }
        }
        vbasis = -1;
        deletebasis();

        delete [] row1,row2,row3;
    }

    if(qgsurface)
    {
        if(qpatch)
        {
            qgsurface->addToGroup(qpatch);
            if(vs->getScene())
                vs->getScene()->addItem(qgsurface);
        }

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

    }
}

void Patch::nfacets(double error,int *useg,int *vseg)
{int i,j,i1,i2;
 double l,lmax;

    if (options.test(PatchTriangulated))
    {   *useg = *vseg = 2;
    }
    else
    {   lmax = 0.0;
        for (j = 0 ; j < vn ; j++)
        {   for (i = 1,l = 0.0 ; i < un ; i++)
            {   i1 = j * un + i;  i2 = i1 - 1;
                l += fabs(polygon[i1].x - polygon[i2].x) +
                    fabs(polygon[i1].y - polygon[i2].y) +
                    fabs(polygon[i1].z - polygon[i2].z);
            }
            if (l > lmax)
                lmax = l;
        }
        *useg = int(0.05 * lmax / error);
        if (*useg > 120) *useg = 120;
        if (*useg < xmesh) *useg = xmesh;

        lmax = 0.0;
        for (i = 0 ; i < un ; i++)
        {   for (j = 1,l = 0.0 ; j < vn ; j++)
            {   i1 = j * un + i;  i2 = i1 - un;
                l += fabs(polygon[i1].x - polygon[i2].x) +
                    fabs(polygon[i1].y - polygon[i2].y) +
                    fabs(polygon[i1].z - polygon[i2].z);
            }
            if (l > lmax)
                lmax = l;
        }
        *vseg = int(0.05 * lmax / error);
        if (*vseg > 120) *vseg = 120;
        if (*vseg < ymesh) *vseg = ymesh;

        *useg = ((*useg + xmesh - 2) / (xmesh-1)) * (xmesh-1);
        *vseg = ((*vseg + ymesh - 2) / (ymesh-1)) * (ymesh-1);
    }
}

int Patch::facet(const BitMask &foptions,double error,int useg,int vseg,View3dSurface *,PolygonCallBack cb,void *data)
{Polygon3d *poly;
 Triangle *triangle;
 UV uv1,uv2,uv3,uv4;
 int i,j,status,triangles;
 double u,v,du,dv;
 Point p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,n1,n2,n3,n9,n10,du1,du2,du3,du4,du7,du8,du9,du10,dv1,dv2,*ubuffer,*nbuffer,p[9];
  status = 0;

    if (useg == 0 || vseg == 0)
        nfacets(error,&useg,&vseg);

    triangles = foptions.test(0) || foptions.test(2);

    if (options.test(PatchTriangulated))
    {   for (i = 0 ; i < nindex ; i += 3)
        {   if (triangles)
            {   uv1.u = 0.0;  uv1.v = 0.0;
                uv2.u = 0.0;  uv2.v = 0.0;
                uv3.u = 0.0;  uv3.v = 0.0;
                n1 = n2 = n3 = ((polygon[index[i+1]] - polygon[index[i]]).cross(polygon[index[i+2]] - polygon[index[i]])).normalize();
                if (foptions.test(2))
                {   p[0] = polygon[index[i]];
                    p[1] = polygon[index[i+1]];
                    p[2] = polygon[index[i+2]];
                    p[3] = n1;
                    p[4] = n2;
                    p[5] = n3;
                    p[6] = Point(uv1.u,uv1.v,0.0);
                    p[7] = Point(uv2.u,uv2.v,0.0);
                    p[8] = Point(uv3.u,uv3.v,0.0);

                    if (! cb(this,0,3,p,data)) 
                      return 0;

                }
                else
                {   if ((triangle = new Triangle(0,polygon[index[i]],polygon[index[i+1]],polygon[index[i+2]],n1,n2,n3,uv1,uv2,uv3)) != 0)
                    {   if (! triangle->getdefined())
                            delete triangle;
                        else
                        {   if (! cb(this,triangle,0,0,data)) 
                                return 0;
                        }
                    }
                    else
                        goto error;
                }
            }
            else
            {   poly = new Polygon3d(polygon[index[i]],polygon[index[i+1]],polygon[index[i+2]]);
                if (poly == NULL) 
                    goto error;
                if (! poly->getdefined())
                    delete poly;
                else
                {   if (! cb((SurfaceEntity *)this,poly,0,0,data)) 
                        goto error;
                }
            }
        }
        return 1;
    }
    else
    {

        newbasis();
        if (triangles)
            newdbasis();
        if (utbasis == 0 || vtbasis == 0 || triangles && (utdbasis == 0 || vtdbasis == 0))
        {   deletebasis();
            if (triangles)
                deletedbasis();
            return 0;
        }
        if ((ubuffer = new Point[vseg+1]) == 0)
        {   deletebasis();
            if (triangles)
                deletedbasis();
            return 0;
        }
        if (triangles && (nbuffer = new Point[vseg+1]) == 0)
        {   deletebasis();
            if (triangles)
                deletedbasis();
            delete [] ubuffer;
                return 0;
        }
        du = 1.0 / double(useg);
        dv = 1.0 / double(vseg);
        for (i = 0 ; i <= useg ; i++)
        {   u = double(i)/double(useg);  if (u >= 1.0 - 1E-14) u = 1.0 - 1E-14;
            if (triangles)
                rdbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,utdbasis,&ubasis);
            else
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
            for (j = 0 ; j <= vseg ; j++)
            {   v = double(j) / double(vseg);  if (v >= 1.0 - 1E-14) v = 1.0 - 1E-14;
                p2 = position(u,v);
                if (triangles)
                    n2 = normal(u,v);
                if (j > 0)
                {   if (i > 0)
                    {   if (triangles)
                        {   uv1.u = u;       uv1.v = v - dv;
                            uv2.u = u;       uv2.v = v;
                            uv3.u = u - du;  uv3.v = v - dv;
                            uv4.u = u - du;  uv4.v = v;
                            if (foptions.test(2))
                            {   p[0] = ubuffer[j-1];
                                p[1] = p1;
                                p[2] = ubuffer[j];
                                p[3] = nbuffer[j-1];
                                p[4] = n1;
                                p[5] = nbuffer[j];
                                p[6] = Point(uv3.u,uv3.v,0.0);
                                p[7] = Point(uv1.u,uv1.v,0.0);
                                p[8] = Point(uv4.u,uv4.v,0.0);

                                if (! cb(this,0,3,p,data)) 
                                    return 0;

                                p[0] = p2;
                                p[1] = ubuffer[j];
                                p[2] = p1;
                                p[3] = n2;
                                p[4] = nbuffer[j];
                                p[5] = n1;
                                p[6] = Point(uv2.u,uv2.v,0.0);
                                p[7] = Point(uv4.u,uv4.v,0.0);
                                p[8] = Point(uv1.u,uv1.v,0.0);

                                if (! cb(this,0,3,p,data)) 
                                    return 0;

                            }
                            else
                            {   if ((triangle = new Triangle(0,ubuffer[j-1],p1,ubuffer[j],nbuffer[j-1],n1,nbuffer[j],uv3,uv1,uv4)) != 0)
                                {   if (! triangle->getdefined())
                                        delete triangle;
                                    else
                                    {   if (! cb(this,triangle,0,0,data)) 
                                            return 0;
                                    }
                                }
                                else
                                    goto error;
                                if ((triangle = new Triangle(1,p2,ubuffer[j],p1,n2,nbuffer[j],n1,uv2,uv4,uv1)) != 0)
                                {   if (! triangle->getdefined())
                                        delete triangle;
                                    else
                                    {   if (! cb(this,triangle,0,0,data)) 
                                            return 0;
                                    }
                                }
                                else
                                    goto error;
                            }
                        }
                        else
                        {   poly = new Polygon3d(ubuffer[j-1],p1,ubuffer[j]);
                            if (poly == NULL) 
                                goto error;
                            if (! poly->getdefined())
                                delete poly;
                            else
                            {   if (! cb((SurfaceEntity *)this,poly,0,0,data)) 
                                    goto error;
                            }
                            poly = new Polygon3d(p2,ubuffer[j],p1);
                            if (poly == NULL) 
                                goto error;
                            if (! poly->getdefined())
                                delete poly;
                            else
                            {   if (! cb((SurfaceEntity *)this,poly,0,0,data)) 
                                    goto error;
                            }
                        }
                    }
                    ubuffer[j-1] = p1;
                    if (triangles)
                        nbuffer[j-1] = n1;
                }
                p1 = p2;
                if (triangles)
                    n1 = n2;
            }
            ubuffer[vseg] = p1;
            if (triangles)
                nbuffer[vseg] = n1;
        }
        status = 1;
error:
        ubasis = -1;
        deletebasis();
        delete [] ubuffer;
        if (triangles)
        {   deletedbasis();
            delete [] nbuffer;
        }
        return status;
    }
    
}

struct TriangulateData
{int pass;
 int ntriangles;
 int coloured;
 Point *triangles;
 Transform *t;
};

static int triangulatecb(SurfaceEntity *se,void *polygon,int nvert,void *pts,void *cbdata)
{TriangulateData *td = (TriangulateData *)cbdata;
 int i;

  if (nvert == 3)
    {  if (td->pass != 0)
         {  if (td->coloured)
              {  for (i = 0 ; i < 9 ; i++)
                   td->triangles[td->ntriangles * 11 + i + 2] = ((Point *)pts)[i];
                  //td->triangles[td->ntriangles * 11 + i + 2] = td->t==0 ? ((Point *)pts)[i] : td->t->transform(((Point *)pts)[i]);
              }
            else
              {  for (i = 0 ; i < 9 ; i++)
                   td->triangles[td->ntriangles * 9 + i + 2] = ((Point *)pts)[i];
                  //td->triangles[td->ntriangles * 9 + i + 2] = td->t==0 ? ((Point *)pts)[i] : td->t->transform(((Point *)pts)[i]);
              }
         }
       td->ntriangles++;
    }

  return 1;
}



void Patch::triangulate(double error, Transform *transform)
{BitMask foptions(32);
 int i,nhor,nver;
 TriangulateData td;
 SurfaceProperties sp,*tsp;

    foptions.set(2,1);

    nfacets(error,&nhor,&nver);
    td.pass = 0;
    td.ntriangles = 0;
    td.triangles = 0;
    //td.t = transform;
    facet(foptions,error,nhor,nver,0,triangulatecb,&td);

    td.pass = 1;
    if (colour_map != 0 && index != 0)
    {   triangles = new Point[td.ntriangles * 11];
        td.coloured = 1;
    }
    else
    {   triangles = new Point[td.ntriangles * 9 + 2];
        td.coloured = 0;
    }
    td.triangles = triangles;
    if (sp.copyinfo(this,0,0,0) != 0)
    {   tsp = new SurfaceProperties;
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

    if (td.coloured)
    {   for (i = 0 ; i < td.ntriangles ; i++)
        {
            //triangles[i*11].x = tsp != 0 ? (int)tsp->shader + 0.1 : 0.1;
            triangles[i*11].x = tsp != 0 ? (LONG_PTR)tsp + 0.1 : 0.1;
            triangles[i*11].y = tsp != 0 ? tsp->transparency : 0.0;
            triangles[i*11].z = RCDBL_MAX;
            triangles[i*11+1].x = GetRValue(colour_map[index[i*3]]);
            triangles[i*11+1].y = GetGValue(colour_map[index[i*3]]);
            triangles[i*11+1].z = GetBValue(colour_map[index[i*3]]);
        }
    }
    else if (sp.tmi.texturetype == 1)
    {   for (i = 0 ; i < ntriangles ; i++)
        {   triangles[i * 9 + 8].x = sp.tmi.x1 + triangles[i * 9 + 8].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 8].y = sp.tmi.y1 + triangles[i * 9 + 8].y * (sp.tmi.y2 - sp.tmi.y1);
            triangles[i * 9 + 9].x = sp.tmi.x1 + triangles[i * 9 + 9].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 9].y = sp.tmi.y1 + triangles[i * 9 + 9].y * (sp.tmi.y2 - sp.tmi.y1);
            triangles[i * 9 + 10].x = sp.tmi.x1 + triangles[i * 9 + 10].x * (sp.tmi.x2 - sp.tmi.x1);
            triangles[i * 9 + 10].y = sp.tmi.y1 + triangles[i * 9 + 10].y * (sp.tmi.y2 - sp.tmi.y1);
        }
    }

}

#ifdef USING_WIDGETS
int Patch::hideGL(HideImage *image)
{Point pt1,pt2,pt3,pt4,*row1,*row2,*row3;
 int i,j,useg,vseg,edgemode,xm,ym;
 double u,v;


    if (options.test(PatchTriangulated))
    {struct HideData hidedata;
        hidedata.image = image;
        hidedata.entity = this;
        generate_triangulated_patch_edges(image->getsurface(),0,hide_edgeGL,&hidedata);
    }
    else
    {   nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&useg,&vseg);
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return -1;
        }

        edgemode = options.test(1) + options.test(2) * 2;

        row1 = row2 = row3 = 0;

        if (edgemode != HLNormal)
        {   if (useg > vseg)
            {   row1 = new Point[useg+1];
                row2 = new Point[useg+1];
                row3 = new Point[useg+1];
            }
            else
            {   row1 = new Point[vseg+1];
                row2 = new Point[vseg+1];
                row3 = new Point[vseg+1];
            }

            for (i = 0 ; i <= useg ; i++)
            {double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
                u = double(i)/double(useg);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                for (j = 0 ; j <= vseg ; j++)
                    row3[j] = position(u,double(j) / double(vseg));

                if (i >= 1)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x5 = image->getsurface()->modeltoscreen(row2[j]).x;
                        y5 = image->getsurface()->modeltoscreen(row2[j]).y;
                        x6 = image->getsurface()->modeltoscreen(row3[j]).x;
                        y6 = image->getsurface()->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                            image->hideGL(this,row2[j],row3[j-1]);

                        if (j >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                            image->hideGL(this,row2[j-1],row3[j-1]);

                        x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                        y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;
                    }
                }


                if (i >= 2)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x2 = image->getsurface()->modeltoscreen(row1[j]).x;
                        y2 = image->getsurface()->modeltoscreen(row1[j]).y;
                        x4 = image->getsurface()->modeltoscreen(row2[j]).x;
                        y4 = image->getsurface()->modeltoscreen(row2[j]).y;
                        x6 = image->getsurface()->modeltoscreen(row3[j]).x;
                        y6 = image->getsurface()->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                            image->hideGL(this,row2[j],row2[j-1]);
                        x1 = x2;  x3 = x4;  x5 = x6;
                        y1 = y2;  y3 = y4;  y5 = y6;
                    }
                }

                for (j = 0 ; j <= vseg ; j++)
                {   row1[j] = row2[j];
                    row2[j] = row3[j];
                }
            }
        }

        if (edgemode != HLEdgesOnly)
        {   if (edgemode == HLEdgesBorder)
            {   xm = 2;
                ym = 2;
            }
            else
            {   xm = xmesh;
                ym = ymesh;
            }
            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                pt1 = position(u,0.0);
                for (j = 1 ; j <= vseg ; j++)
                {   pt2 = position(u,double(j) / double(vseg));
                    image->hideGL(this,pt1,pt2);
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                pt1 = position(0.0,v);
                for (i = 1 ; i <= useg ; i++)
                {   pt2 = position(double(i) / double(useg),v);
                    image->hideGL(this,pt1,pt2);
                    pt1 = pt2;
                }
            }
        }
        vbasis = -1;
        deletebasis();

        delete [] row1,row2,row3;
    }

  return 1;
}
#endif

int Patch::hide(HideImage *image)
{Point pt1,pt2,pt3,pt4,*row1,*row2,*row3;
 int i,j,useg,vseg,edgemode,xm,ym;
 double u,v;

    QGraphicsItemGroup *qghidden = 0;

    image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);

    if((image->getClear() & 1) == 1)
    {
        if(qgi)
            if(image->getsurface()->getScene())
                image->getsurface()->getScene()->removeItem(qgHLines);
        delete qgi;
        qgi=0;

        qgi = qghidden = new QGraphicsItemGroup();
    }

    if(image->getsurface()->getScene())
        image->getsurface()->getScene()->addItem(qgi);

    if (options.test(PatchTriangulated))
    {struct HideData hidedata;
        hidedata.image = image;
        hidedata.entity = this;
        generate_triangulated_patch_edges(image->getsurface(),0,hide_edge,&hidedata);
    }
    else
    {   nfacets((image->getsurface()->getumax()-image->getsurface()->getumin()) / image->getsurface()->getwidth() * db.getdtoler(),&useg,&vseg);
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return -1;
        }

        edgemode = options.test(1) + options.test(2) * 2;

        row1 = row2 = row3 = 0;

        if (edgemode != HLNormal)
        {   if (useg > vseg)
            {   row1 = new Point[useg+1];
                row2 = new Point[useg+1];
                row3 = new Point[useg+1];
            }
            else
            {   row1 = new Point[vseg+1];
                row2 = new Point[vseg+1];
                row3 = new Point[vseg+1];
            }

            for (i = 0 ; i <= useg ; i++)
            {double x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
                u = double(i)/double(useg);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                for (j = 0 ; j <= vseg ; j++)
                    row3[j] = position(u,double(j) / double(vseg));

                if (i >= 1)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x5 = image->getsurface()->modeltoscreen(row2[j]).x;
                        y5 = image->getsurface()->modeltoscreen(row2[j]).y;
                        x6 = image->getsurface()->modeltoscreen(row3[j]).x;
                        y6 = image->getsurface()->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y5-y4)*x3 + (x4-x5)*y3 + (y4-y5)*x4 + (x5-x4)*y4) * ((y5-y4)*x6 + (x4-x5)*y6 + (y4-y5)*x4 + (x5-x4)*y4) >= 0.0))
                            image->hide(this,row2[j],row3[j-1]);

                        if (j >= 2 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                            image->hide(this,row2[j-1],row3[j-1]);

                        x1 = x3;  x2 = x4;  x3 = x5;  x4 = x6;
                        y1 = y3;  y2 = y4;  y3 = y5;  y4 = y6;
                    }
                } 
               

                if (i >= 2)
                {   for (j = 0 ; j <= vseg ; j++)
                    {   x2 = image->getsurface()->modeltoscreen(row1[j]).x;
                        y2 = image->getsurface()->modeltoscreen(row1[j]).y;
                        x4 = image->getsurface()->modeltoscreen(row2[j]).x;
                        y4 = image->getsurface()->modeltoscreen(row2[j]).y;
                        x6 = image->getsurface()->modeltoscreen(row3[j]).x;
                        y6 = image->getsurface()->modeltoscreen(row3[j]).y;
                        if (j >= 1 && (((y4-y3)*x5 + (x3-x4)*y5 + (y3-y4)*x4 + (x4-x3)*y4) * ((y4-y3)*x2 + (x3-x4)*y2 + (y3-y4)*x4 + (x4-x3)*y4) >= 0.0))
                            image->hide(this,row2[j],row2[j-1]);
                        x1 = x2;  x3 = x4;  x5 = x6;
                        y1 = y2;  y3 = y4;  y5 = y6;
                    }
                } 

                for (j = 0 ; j <= vseg ; j++)
                {   row1[j] = row2[j];
                    row2[j] = row3[j];
                }
            }
        }

        if (edgemode != HLEdgesOnly)
        {   if (edgemode == HLEdgesBorder)
            {   xm = 2;
                ym = 2;
            }
            else
            {   xm = xmesh;
                ym = ymesh;
            }
            for (i = 0 ; i < xm ; i++)
            {   u = double(i)/double(xm-1);
                rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
                pt1 = position(u,0.0);
                for (j = 1 ; j <= vseg ; j++)
                {   pt2 = position(u,double(j) / double(vseg));
                    image->hide(this,pt1,pt2);
                    pt1 = pt2;
                }
            }

            ubasis = -1;
            for (j = 0 ; j < ym ; j++)
            {   v = double(j)/double(ym-1);
                rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
                pt1 = position(0.0,v);
                for (i = 1 ; i <= useg ; i++)
                {   pt2 = position(double(i) / double(useg),v);
                    image->hide(this,pt1,pt2);
                    pt1 = pt2;
                }
            } 
        }
        vbasis = -1;
        deletebasis();

        delete [] row1,row2,row3;
    }

  return 1;
}

double Patch::pick(View3dSurface *vs,double x,double y,Point *pos)
{Point pt1,pt2,pt3,pt4;
 int i,j,useg,vseg;
 double u,v,d;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (pos == 0 && vs->displaylist.defined(this))
    return vs->displaylist.pick(this,x,y);
 
    if (options.test(PatchTriangulated))
    {struct PickData pickdata;
        pickdata.x = x;
        pickdata.y = y;
        pickdata.pos = pos;
        pickdata.d = -1.0;
        generate_triangulated_patch_edges(vs,0,pick_edge,&pickdata);
        return pickdata.d;
    }
    else
    {   nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return -1;
        }

        for (i = 0 ; i < xmesh ; i++)
        {   u = double(i)/double(xmesh-1);
            rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
            pt1 = position(u,0.0);
            for (j = 1 ; j <= vseg ; j++)
            {   pt2 = position(u,double(j) / double(vseg));
                if ((d = vs->pick(pt1,pt2,x,y,pos)) >= 0.0)
                {   deletebasis();  ubasis = -1; 
                    return d;
                }
                pt1 = pt2;
            }
        }

        ubasis = -1;
        for (j = 0 ; j < ymesh ; j++)
        {   v = double(j)/double(ymesh-1);
            rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
            pt1 = position(0.0,v);
            for (i = 1 ; i <= useg ; i++)
            {   pt2 = position(double(i) / double(useg),v);
                if ((d = vs->pick(pt1,pt2,x,y,pos)) >= 0.0)
                {   deletebasis();  vbasis = -1;  
                    return d;
                }
                pt1 = pt2;
            }
        }
        vbasis = -1;
        deletebasis();
    }
  return -1.0;
}

int Patch::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{int i,j,useg,vseg;
 double u,v;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

    if (vs->displaylist.defined(this))
        return vs->displaylist.pick(this,inside,x1,y1,x2,y2);

    if (options.test(PatchTriangulated))
    {struct PickData pickdata;
        pickdata.x1 = x1;
        pickdata.y1 = y1;
        pickdata.x2 = x2;
        pickdata.y2 = y2;
        pickdata.inside = inside;
        pickdata.d = 1.0;
        generate_triangulated_patch_edges(vs,0,pick_window_edge,&pickdata);
        return (int)pickdata.d;
    }
    else
    {   nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return -1;
        }
        for (i = 0 ; i < xmesh ; i++)
        {   u = double(i)/double(xmesh-1);
            rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
            for (j = 0 ; j <= vseg ; j++)
            if (! vs->pick(position(u,double(j) / double(vseg)),inside,x1,y1,x2,y2))
            {   deletebasis();  ubasis = -1;  return 0;
            }
        }

        ubasis = -1;
        for (j = 0 ; j < ymesh ; j++)
        {   v = double(j)/double(ymesh-1);
            rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
            for (i = 0 ; i <= useg ; i++)
            if (! vs->pick(position(double(i) / double(useg),v),inside,x1,y1,x2,y2))
            {   deletebasis();  vbasis = -1;  
                return 0;
            }
        }
        vbasis = -1;
        deletebasis();
    }

    return 1;

}

int Patch::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Point pt1,pt2,pt3,pt4;
 int i,j,useg,vseg,picked;
 double u,v;
  picked = inside != 2;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 

    if (options.test(PatchTriangulated))
    {struct PickData pickdata;
        pickdata.nvertices = nvertices;
        pickdata.pwvertices = pwvertices;
        pickdata.inside = inside;
        pickdata.d = 1.0;
        generate_triangulated_patch_edges(vs,0,pick_polygon_edge,&pickdata);
        return (int)pickdata.d;
    }
    else
    {   
        nfacets((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler(),&useg,&vseg);
        newbasis();
        if (utbasis == 0 || vtbasis == 0)
        {   deletebasis();
            return -1;
        }
        for (i = 0 ; i < xmesh ; i++)
        {   u = double(i)/double(xmesh-1);
            rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
            pt1 = position(u,0.0);
            for (j = 1 ; j <= vseg ; j++)
            {   pt2 = position(u,double(j) / double(vseg));
                if (vs->pick(nvertices,pwvertices,pt1,pt2,inside,&picked))
                {   deletebasis();  ubasis = -1;  
                    return picked;
                }
                pt1 = pt2;
            }
        }
        ubasis = -1;
        for (j = 0 ; j < ymesh ; j++)
        {   v = double(j)/double(ymesh-1);
            rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vtbasis,&vbasis);
            pt1 = position(0.0,v);
            for (i = 1 ; i <= useg ; i++)
            {   pt2 = position(double(i) / double(useg),v);
                if (vs->pick(nvertices,pwvertices,pt1,pt2,inside,&picked))
                {   deletebasis();  vbasis = -1;  
                    return picked;
                }
                pt1 = pt2;
            }
        }
        vbasis = -1;
        deletebasis();
    }

    return picked;

}

Point Patch::position(double u,double v) const
{double sum,x,*ubas,*vbas;
 int i,j,j1,ul,vl,ubase,vbase;
 Point p(0.0,0.0,0.0);

    if (options.test(PatchTriangulated))
        return p;

    if (u < 0.0) 
        u = 0.0; 
    else if (u > 1.0) 
        u = 1.0;
    if (v < 0.0) 
        v = 0.0; 
    else if (v > 1.0) 
        v = 1.0;
    if (ubasis == -1)
    {   if (utbasis != 0)
            ubas = utbasis;
        else
        {   if ((ubas = new double[un + udegree]) == 0) 
                return p;
        }
        rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,ubas,&ubase);
    }
    else
    {   ubase = ubasis;
        ubas = utbasis;
    }
    if (vbasis == -1)
    {   if (vtbasis != 0)
            vbas = vtbasis;
        else
        {   if ((vbas = new double[vn + vdegree]) == 0) 
                return p;
        }
        rbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vbas,&vbase);
    }
    else
    {  vbase = vbasis;
       vbas = vtbasis;
    }

    ul = ubase;  
    if (ubase > un - 1) 
        ul = un - 1;
    vl = vbase;  
    if (vbase > vn - 1) 
        vl = vn - 1;

    sum = 0.0;
    for (i = ubase - udegree ; i <= ul ; i++)
        for (j = vbase - vdegree ; j <= vl ; j++)
            if (cweight != 0)
                sum += cweight[j * un + i] * ubas[i] * vbas[j];
            else
                sum += ubas[i] * vbas[j];
    for (i = ubase - udegree ; i <= ul ; i++)
        for (j = vbase - vdegree ; j <= vl ; j++)
        {   j1 = j * un + i;
            if (cweight != 0)
                x = cweight[j1] * ubas[i] * vbas[j];
            else
                x = ubas[i] * vbas[j];
            p.x += polygon[j1].x * x;
            p.y += polygon[j1].y * x;
            p.z += polygon[j1].z * x;
        }
    p /= sum;

    if (utbasis == 0) 
        delete [] ubas;
    if (vtbasis == 0) 
        delete [] vbas;

    return p;
}

Point Patch::normal(double u,double v) const
{double sum1,sum2,sum3,x1,x2,x3,*ubas,*vbas,*udbas,*vdbas;
 int i,j,j1,ul,vl,ubase,vbase;
 Point p(0.0,0.0,0.0),snu(0.0,0.0,0.0),snv(0.0,0.0,0.0),du,dv;

    if (options.test(PatchTriangulated))
        return Point(0.0,0.0,1.0);


    if (u < 0.0) 
        u = 0.0; 
    else if (u > 1.0 - 1.0E-14) 
        u = 1.0 - 1.0E-14;
    if (v < 0.0) 
        v = 0.0; 
    else if (v > 1.0 - 1.0E-14) 
        v = 1.0 - 1.0E-14;

    if (ubasis == -1)
    {   if (utbasis != 0)
        {   ubas = utbasis;
            udbas = utdbasis;
        }
        else
        {   if ((ubas = new double[un + udegree]) == 0) 
                return p;
            if ((udbas = new double[un + udegree]) == 0) 
                return p;
        }
        rdbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,ubas,udbas,&ubase);
    }
    else
    {   ubase = ubasis;
        ubas = utbasis;
        udbas = utdbasis;
    }

    if (vbasis == -1)
    {   if (vtbasis != 0)
        {   vbas = vtbasis;
            vdbas = vtdbasis;
        }
        else
        {   if ((vbas = new double[vn + vdegree]) == 0) 
                return p;
            if ((vdbas = new double[vn + vdegree]) == 0) 
                return p;
        }
        rdbasis(vdegree,vn,(vknot != 0 ? vknot : vtknot),v,vbas,vdbas,&vbase);
    }
    else
    {   vbase = vbasis;
        vbas = vtbasis;
        vdbas = vtdbasis;
    }

    ul = ubase;  
    if (ubase > un - 1) 
        ul = un - 1;
    vl = vbase;  
    if (vbase > vn - 1) 
        vl = vn - 1;
    sum1 = sum2 = sum3 = 0.0;
    for (i = ubase - udegree ; i <= ul ; i++)
        for (j = vbase - vdegree ; j <= vl ; j++)
        {   j1 = j * un + i;
            if (cweight != 0)
            {   x1 = cweight[j1] * ubas[i] * vbas[j];
                x2 = cweight[j1] * udbas[i] * vbas[j];
                x3 = cweight[j1] * ubas[i] * vdbas[j];
            }
            else
            {   x1 = ubas[i] * vbas[j];
                x2 = udbas[i] * vbas[j];
                x3 = ubas[i] * vdbas[j];
            }
            sum1 += x1;
            sum2 += x2;
            sum3 += x3;
            p.x += polygon[j1].x * x1;
            p.y += polygon[j1].y * x1;
            p.z += polygon[j1].z * x1;
            snu.x += polygon[j1].x * x2;
            snu.y += polygon[j1].y * x2;
            snu.z += polygon[j1].z * x2;
            snv.x += polygon[j1].x * x3;
            snv.y += polygon[j1].y * x3;
            snv.z += polygon[j1].z * x3;
        }

    du.x = (snu.x - p.x * sum2 / sum1) / sum1;
    du.y = (snu.y - p.y * sum2 / sum1) / sum1;
    du.z = (snu.z - p.z * sum2 / sum1) / sum1;
    dv.x = (snv.x - p.x * sum3 / sum1) / sum1;
    dv.y = (snv.y - p.y * sum3 / sum1) / sum1;
    dv.z = (snv.z - p.z * sum3 / sum1) / sum1;
    p = du.cross(dv).normalize();

    if (utbasis == 0) 
        delete [] ubas;
    if (vtbasis == 0) 
        delete [] vbas;
    if (utdbasis == 0) 
        delete [] udbas;
    if (vtdbasis == 0) 
        delete [] vdbas;

    return p;
}

int Patch::npoints(void)
{   if (options.test(PatchTriangulated))
        return nvertices;
    else
        return un * vn;
}

Point Patch::point(int i)
{ return polygon[i];
}

Point Patch::nearp(Point p)
{int i,n;
 double dmin,d;
 Point pmin;

    n = npoints();
    dmin = (p - polygon[0]).length();
    pmin = polygon[0];
    for (i = 1 ; i < n ; i++)
        if ((d = (p - polygon[i]).length()) < dmin)
        {   dmin = d;
            pmin = polygon[i];
        }

    return pmin;

}

void Patch::movepoints(int drawentity,int n,int *indicies,Transform *t)
{int i;
    db.saveundo(UD_MOVED,this);
    if (drawentity) 
        draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(this);
    for (i = 0 ; i < n ; i++)
        polygon[indicies[i]] = t->transform(polygon[indicies[i]]);
    if (drawentity) 
        draw(DM_NORMAL);
    if (getposition() != 0 && db.MovedCallback != 0)
        db.MovedCallback(this);
}

int Patch::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
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

int Patch::move(Database *db,Transform &t,int erase)
{Entity *e;
 int i,n;
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
  n = npoints();
  for (i = 0 ; i < n ; i++)
    polygon[i] = t.transform(polygon[i]);
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *Patch::clone(Transform &t)
{Point *newpoints;
 int i,n;

    if (copye != NULL) return copye;
    n = npoints();

    if ((newpoints = new Point[n]) == 0)
        return 0;
    for (i = 0 ; i < n ; i++)
        newpoints[i] = t.transform(polygon[i]);

    if (options.test(PatchTriangulated))
        copye = new Patch(*this,nvertices,newpoints,nindex,index,options);
    else
        copye = new Patch(*this,un,vn,udegree,vdegree,xmesh,ymesh,newpoints,uknot,vknot,
                            cweight,trimcurves,options);
    delete [] newpoints;
    copye->cloneattributes(this);

    return copye;
}

int Patch::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

int Patch::save(CadFile *outfile)
{short type,x;
 int i;
    type = patch_entity;
    if (! outfile->write(&type)) 
        return 0;
    if (! saveheader(outfile)) 
        return 0;

    if (options.test(PatchTriangulated))
    {   x = (short)0x8000;
        if (! outfile->write(&x)) 
            return 0;
        if (! outfile->write(&nvertices)) 
            return 0;
        for (i = 0 ; i < nvertices ; i++)
            if (! polygon[i].save(outfile)) 
                return 0;
        if (! outfile->write(&nindex)) 
            return 0;
        for (i = 0 ; i < nindex ; i++)
            if (! outfile->write(& index[i]))
                return 0;
        if (! outfile->write(&options)) 
            return 0;
    }
    else
    {   if (! outfile->write(&un)) 
            return 0;
        if (! outfile->write(&vn)) 
            return 0;
        if (! outfile->write(&udegree)) 
            return 0;
        if (! outfile->write(&vdegree)) 
            return 0;
        if (! outfile->write(&xmesh)) 
            return 0;
        if (! outfile->write(&ymesh)) 
            return 0;
        for (i = 0 ; i < un * vn ; i++)
            if (! polygon[i].save(outfile)) 
                return 0;
        x = short(uknot == 0 ? 0 : un + udegree + 1);
        if (! outfile->write(&x)) return 0;
        for (i = 0 ; i < x ; i++)
            if (! outfile->write(&uknot[i])) 
                return 0;
        x = short(vknot == 0 ? 0 : vn + vdegree + 1);
        if (! outfile->write(&x)) return 0;
        for (i = 0 ; i < x ; i++)
            if (! outfile->write(&vknot[i])) 
                return 0;
        x = short(cweight == 0 ? 0 : un * vn);
        if (! outfile->write(&x)) 
            return 0;
        for (i = 0 ; i < x ; i++)
            if (! outfile->write(&cweight[i])) 
                return 0;
        x = 0;
        if (! outfile->write(&x)) 
            return 0;  //  Number of trim curves
        if (! outfile->write(&options)) 
            return 0;
    }
  return 1;
}

Entity *Patch::load(int,CadFile *infile)
{Patch *patch;
 EntityHeader *header;
 Point *polygon;
 double *cweight,*uknot,*vknot;
 short un,vn,udegree,vdegree,xmesh,ymesh,x;
 BitMask options(32);
 EntityList tcurves;
 int i;
 long nvertices,nindex,*index;

    if ((header = loadheader(infile)) == 0) 
        return 0;

    if (! infile->read(&un)) 
        return 0;
    if (un == (short)0x8000)
    {   if (! infile->read(&nvertices)) 
            return 0;
        if ((polygon = new Point[nvertices]) == 0) 
            return 0;
        for (i = 0 ; i < nvertices ; i++)
            if (! polygon[i].load(infile)) 
                return 0;
        if (! infile->read(&nindex)) 
            return 0;
        if ((index = new long[nindex]) == 0)
            return 0;
        for (i = 0 ; i < nindex ; i++)
            if (! infile->read(&index[i]))
                return 0;
        if (! infile->read(&options)) 
            return 0;

        patch = new Patch(*header,nvertices,polygon,nindex,index,options);
        patch->atlist = header->getatlist();
        delete header;
        delete [] polygon;
        delete [] index;

    }
    else
    {   if (! infile->read(&vn)) return 0;
        if (! infile->read(&udegree)) return 0;
        if (! infile->read(&vdegree)) return 0;
        if (! infile->read(&xmesh)) return 0;
        if (! infile->read(&ymesh)) return 0;
        if ((polygon = new Point[un * vn]) == 0) return 0;
        for (i = 0 ; i < un * vn ; i++)
            if (! polygon[i].load(infile)) return 0;
        if (! infile->read(&x)) return 0;
        if (x == 0)
            uknot = 0;
        else
        {   if ((uknot = new double[x]) == 0)
            {  delete [] polygon;  
               return 0;
            }
        }
        for (i = 0 ; i < x ; i++)
            if (! infile->read(&uknot[i])) 
                return 0;
        if (! infile->read(&x)) return 0;
        if (x == 0)
            vknot = 0;
        else
        {   if ((vknot = new double[x]) == 0)
            {   delete [] polygon;  delete [] vknot;  
                return 0;
            }
        }
        for (i = 0 ; i < x ; i++)
            if (! infile->read(&vknot[i])) 
                return 0;
        if (! infile->read(&x)) 
            return 0;
        if (x == 0)
            cweight = 0;
        else
        {   if ((cweight = new double[x]) == NULL)
            {   delete [] polygon;  delete [] uknot;  delete [] vknot;
                return 0;
            }
        }
        for (i = 0 ; i < x ; i++)
        if (! infile->read(&cweight[i])) 
            return 0;
        if (! infile->read(&x)) 
            return 0;  //  Number of trim curves
        if (! infile->read(&options)) 
            return 0;
        patch = new Patch(*header,un,vn,udegree,vdegree,xmesh,ymesh,polygon,uknot,vknot,
                            cweight,tcurves,options);
        patch->atlist = header->getatlist();
        delete header;
        delete [] polygon;
        delete [] uknot;
        delete [] vknot;
        delete [] cweight;
    }
  return patch;
}

int Patch::save(UndoFile *outfile)
{short type;
    type = patch_entity;
    if (! outfile->write(&type))
        return 0;
    if (! outfile->blockwrite(this,sizeof(*this))) 
        return 0;
    if (! outfile->write(&visible))
        return 0;
    if (! atlist.save(outfile)) 
        return 0;
    if (! outfile->blockwrite(polygon,sizeof(Point) * npoints())) 
        return 0;

    if (options.test(PatchTriangulated))
    {   if (! outfile->blockwrite(index,sizeof(long) * nindex)) 
            return 0;
    }
    else
    {   if (uknot != NULL)
        {   if (! outfile->blockwrite(uknot,sizeof(double) * (un + udegree + 1))) 
                return 0;
        }
        if (vknot != NULL)
        {   if (! outfile->blockwrite(vknot,sizeof(double) * (vn + vdegree + 1))) 
                return 0;
        }
        if (cweight != NULL)
        {   if (! outfile->blockwrite(cweight,sizeof(double) * un * vn)) 
                return 0;
        }
    }

  return 1;
}

Entity *Patch::load(UndoFile *infile)
{Patch *patch;
 char bm[sizeof(BitMask)];
  if ((patch = new Patch) != NULL)
    {AttributeList al;
       memcpy(bm,&patch->visible,sizeof(bm));
       if (! infile->blockread(patch,sizeof(*patch))) return NULL;
       memcpy(&patch->visible,bm,sizeof(bm));
       if (! infile->read(&patch->visible)) return 0;
       if (! al.load(infile)) return 0;   patch->atlist = al;
       patch->utknot = patch->utbasis = 0;
       patch->vtknot = patch->vtbasis = 0;
       if ((patch->polygon = new Point[patch->npoints()]) == 0) return 0;
       if (! infile->blockread(patch->polygon,sizeof(Point) * patch->npoints()))
         {  delete patch;  return 0;
         }

        if (patch->options.test(PatchTriangulated))
        {   patch->index = new long[patch->nindex];
            if (! infile->blockread(patch->index,sizeof(long) * patch->nindex)) 
                return 0;
        }
        else
        {   if (patch->uknot != NULL)
            {   if ((patch->uknot = new double[patch->un + patch->udegree + 1]) == 0)
                {   delete [] patch->polygon;  delete patch;  
                    return 0;
                }
                if (! infile->blockread(patch->uknot,sizeof(double) * (patch->un + patch->udegree + 1))) 
                    return 0;
            }
            else
            {   patch->tempknots(0,&patch->utknot);
                if (patch->utknot == NULL)
                {   delete [] patch->polygon;  
                    return 0;
                }
            }
            if (patch->vknot != NULL)
            {   if ((patch->vknot = new double[patch->vn + patch->vdegree + 1]) == 0)
                {   delete [] patch->polygon;  delete [] patch->uknot;  delete [] patch->utknot;  delete patch;  
                    return 0;
                }
                if (! infile->blockread(patch->vknot,sizeof(double) * (patch->vn + patch->vdegree + 1))) 
                    return 0;
            }
            else
            {   patch->tempknots(1,&patch->vtknot);
                if (patch->vtknot == NULL)
                {   delete [] patch->polygon;  delete [] patch->uknot;  delete [] patch->utknot;  delete patch;  
                    return 0;
                }
            }
            if (patch->cweight != NULL)
            {   if ((patch->cweight = new double[patch->vn * patch->un]) == NULL)
                {   delete [] patch->uknot;  delete [] patch->vknot;  delete [] patch->polygon;  delete patch;  
                    return 0;
                }
                if (! infile->blockread(patch->cweight,sizeof(double) * patch->un * patch->vn)) 
                    return 0;
            }
        }
    }

    return patch;

}

int Patch::saveattributes(CadFile *outfile)
{ if (! outfile->write(&udegree)) return 0;
  if (! outfile->write(&vdegree)) return 0;
  if (! outfile->write(&xmesh)) return 0;
  if (! outfile->write(&ymesh)) return 0;
  if (! outfile->write(&options)) return 0;
  return 1;
}

int Patch::loadattributes(CadFile *infile)
{ if (! infile->read(&udegree)) return 0;
  if (! infile->read(&vdegree)) return 0;
  if (! infile->read(&xmesh)) return 0;
  if (! infile->read(&ymesh)) return 0;
  if (! infile->read(&options)) return 0;
  return 1;
}

int Patch::savedxf(int blocksection,DXFOut *dxffile)
{int i,j,useg,vseg,status;
 double u,v;
 Point p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,n1,n2,n9,n10,du1,du2,du3,du4,du7,du8,du9,du10,dv1,dv2;

  status = 1;
  if (! blocksection)
    {  status = 0;
       useg = 19;  vseg = 19;
       newbasis();
       if (utbasis == 0 || vtbasis == 0 )
         {  deletebasis();
            return 0;
         }
       if (! dxffile->writestring(0, (char*)"POLYLINE")) goto error;
       if (! dxffile->writeheader(this)) goto error;
       if (! dxffile->writestring(100, (char*)"AcDbPolygonMesh")) return 0;
       if (! dxffile->writelong(66,1)) goto error;
       if (! dxffile->writereal(10,0.0)) goto error;
       if (! dxffile->writereal(20,0.0)) goto error;
       if (! dxffile->writereal(30,0.0)) goto error;
       if (! dxffile->writelong(70,16)) goto error;
       if (! dxffile->writelong(71,useg+1)) goto error;
       if (! dxffile->writelong(72,vseg+1)) goto error;
       for (i = 0 ; i <= useg ; i++)
         {  u = double(i)/double(useg);  if (u >= 1.0 - 1E-14) u = 1.0 - 1E-14;
            rbasis(udegree,un,(uknot != 0 ? uknot : utknot),u,utbasis,&ubasis);
            for (j = 0 ; j <= vseg ; j++)
              {  v = double(j) / double(vseg);  if (v >= 1.0 - 1E-14) v = 1.0 - 1E-14;
                 p2 = position(u,v);
                 if (! dxffile->writestring(0, (char*)"VERTEX")) goto error;
                 if (! dxffile->writehandle()) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
                 if (! dxffile->writelong(8,getlayer())) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbVertex")) return 0;
                 if (! dxffile->writestring(100, (char*)"AcDbPolygonMeshVertex")) return 0;
                 if (! dxffile->writereal(10,p2.x  * dxffile->getscale())) goto error;
                 if (! dxffile->writereal(20,p2.y  * dxffile->getscale())) goto error;
                 if (! dxffile->writereal(30,p2.z  * dxffile->getscale())) goto error;
                 if (! dxffile->writereal(70,64)) goto error;
              }
         }
       if (! dxffile->writestring(0, (char*)"SEQEND")) goto error;
       if (! dxffile->writehandle()) return 0;
       if (! dxffile->writestring(100, (char*)"AcDbEntity")) return 0;
       status = 1;
    error:
       ubasis = -1;
       deletebasis();
    }
  return status;
}

int Patch::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}


int Patch::savegeometry(CadFile *outfile)
{BitMask arrays(32);
  if (! outfile->write(&un)) return 0;
  if (! outfile->write(&vn)) return 0;
  if (! outfile->write(&udegree)) return 0;
  if (! outfile->write(&vdegree)) return 0;
  if (! outfile->write(&nvertices)) return 0;
  if (! outfile->write(&nindex)) return 0;
  arrays.set(0,! options.test(PatchTriangulated) && uknot != NULL);
  arrays.set(1,! options.test(PatchTriangulated) && vknot != NULL);
  arrays.set(2,! options.test(PatchTriangulated) && cweight != NULL);
  arrays.set(3,options.test(PatchTriangulated));
  if (! outfile->write(&arrays)) return NULL;
  if (! outfile->blockwrite(polygon,sizeof(Point) * npoints())) return 0;
  if (! options.test(PatchTriangulated) && uknot != NULL)
    {  if (! outfile->blockwrite(uknot,sizeof(double) * (un + udegree + 1))) return 0;
    }
  if (! options.test(PatchTriangulated) && vknot != NULL)
    {  if (! outfile->blockwrite(vknot,sizeof(double) * (vn + vdegree + 1))) return 0;
    }
  if (! options.test(PatchTriangulated) && cweight != NULL)
    {  if (! outfile->blockwrite(cweight,sizeof(double) * un * vn)) return 0;
    }
  if (options.test(PatchTriangulated))
    {  if (! outfile->blockwrite(index,sizeof(long) * nindex)) return 0;
    }
  return 1;
}

int Patch::loadgeometry(CadFile *infile)
{BitMask arrays(32);
  cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&un)) return NULL;
  if (! infile->read(&vn)) return NULL;
  if (! infile->read(&udegree)) return NULL;
  if (! infile->read(&vdegree)) return NULL;
  if (! infile->read(&nvertices)) return 0;
  if (! infile->read(&nindex)) return 0;
  if (! infile->read(&arrays)) return NULL;
  if (polygon == 0) polygon = new Point[npoints()];
  if (! options.test(PatchTriangulated))
  { delete [] uknot;  uknot = 0;
  }
  if (arrays.test(0)) uknot = new double[un + udegree + 1];
  if (arrays.test(1)) vknot = new double[vn + vdegree + 1];
  if (arrays.test(2) && cweight == 0) cweight = new double[un * vn];
  if (! infile->blockread(polygon,sizeof(Point) * npoints())) return 0;
  if (! options.test(PatchTriangulated))
  { if (uknot != NULL)
    {  if (! infile->blockread(uknot,sizeof(double) * (un + udegree + 1))) return 0;
    }
    else
    {  tempknots(0,&utknot);
       if (utknot == 0)
         {  delete [] uknot;  delete [] utknot;
            delete [] polygon;  delete [] cweight;
            defined = 0;
            return 0;
         }
    }
  if (vknot != NULL)
    {  if (! infile->blockread(vknot,sizeof(double) * (vn + vdegree + 1))) return 0;
    }
  else
    {  tempknots(1,&vtknot);
       if (vtknot == NULL)
         {  delete [] uknot;  delete [] vknot;
            delete [] utknot;  delete [] vtknot;
            delete [] polygon;  delete [] cweight;
            defined = 0;
            return 0;
         }
    }
  if (cweight != NULL)
    {  if (! infile->blockread(cweight,sizeof(double) * un * vn)) return 0;
    }
  }
  else
    {  if (! infile->blockread(index,sizeof(long) * nindex)) return 0;
    }
  defined = 1;
  return 1;
}

void Patch::change(const BitMask &change,const EntityHeader &eh,int ud,int vd,int xm,int ym,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4))
    {  udegree = short(ud);
       if (udegree > un - 1) udegree = un - 1;
       delete [] uknot;  delete [] utknot;  uknot = 0;
       tempknots(1,&utknot);
    }
  if (change.test(5))
    {  vdegree = short(vd);  
       if (vdegree > vn - 1) vdegree = vn - 1;
       delete [] vknot;  delete [] vtknot;  vknot = 0;
       tempknots(0,&vtknot);
    }
  if (change.test(6)) xmesh = short(xm);
  if (change.test(7)) ymesh = short(ym);
  if (change.test(8)) 
    {  options.set(1,o.test(1));
       options.set(2,o.test(2));
    }

  draw(DM_NORMAL);
}

/*
// defined in ncdialog.h
class PatchDisplayDialogControl : public DisplayDialogControl
  {public:
     PatchDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

void PatchDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
    ResourceString rs0(NCVIEWA);
    View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hWnd,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    int xm,ym,ud,vd,edgemode;
    EntityList elist;
    double x1,y1,w,h;
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
    BitMask options(32);

    if(hWnd == 0)
     return;

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
    ctl = (QGraphicsView*)((Patch_Dialog*)dialog->gethdlg())->getControl(109);
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

       if (dialog->currentvalue(104,&ud) &&
           dialog->currentvalue(105,&vd) &&
           dialog->currentvalue(106,&ym) &&
           dialog->currentvalue(107,&xm) &&
           dialog->currentvalue(108,&edgemode))
       {
          Point polygon[121];
          BitMask options(32);
          int i,j;
            options.set(1,edgemode % 2);  options.set(2,edgemode / 2);
            x1 = output.getumin();
            y1 = output.getvmin();
            w = output.getumax() - x1;
            h = output.getvmax() - y1;
            for (i = 0 ; i < 11 ; i++)
              for (j = 0 ; j < 11 ; j++)
                polygon[i*11 + j].setxyz(x1+w*0.1+w*0.8 * j / 10.0,y1+h*0.1+h*0.8 * i / 10.0,0.0);
            Patch patch(11,11,ud,vd,xm,ym,polygon,0,0,0,elist,options);
            if (patch.getdefined())
              patch.draw(DM_NORMAL,NULL,&output);
       }
    }
}

class PatchVerifyButtonControl : public ButtonDialogControl
  {public:
     PatchVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int PatchVerifyButtonControl::select(Dialog *dialog)
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

int Patch::setup(BitMask *change,int *xd,int *yd,int *xm,int *ym,BitMask *options,EntityHeader *inheader)
{
 int status,edgemode;
 //static RCCHAR *edgemodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
 RCSTRING STRedgemodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
 RCCHAR dm1[25],dm2[25],dm3[25],dm4[25];
 RCCHAR *edgemodel[] = { dm1,dm2,dm3,dm4 };
 Dialog dialog("Patch_Dialog");
 DialogControl *dc;
 ResourceString rs6(NCPATCH+6);
 //
 int c,l,s,defColour,defLayer,defStyle,defxm,defym;
 double wt,defWeight;
 BitMask defoptions(32);

 for(int i=0; i<4; i++)
     strcpy(edgemodel[i],&STRedgemodel[i]);

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

 edgemode = options->test(1) + options->test(2) * 2;

 dialog.add(dc = new IntegerDialogControl(104,xd,2,10));
 //dialog.add(new ScrollBarDialogControl(1100,dc));
 dialog.add(dc = new IntegerDialogControl(105,yd,2,10));
 //dialog.add(new ScrollBarDialogControl(1101,dc));
 dialog.add(dc = new IntegerDialogControl(106,ym,2,1000));
 //dialog.add(new ScrollBarDialogControl(1102,dc));
 dialog.add(dc = new IntegerDialogControl(107,xm,2,1000));
 //dialog.add(new ScrollBarDialogControl(1103,dc));
 dialog.add(new ListDialogControl(108,4,edgemodel,&edgemode));
 dialog.add((DisplayDialogControl *) new PatchDisplayDialogControl(109));
 dialog.add(new ButtonDialogControl(110)); // surface properties
 dialog.add(new ButtonDialogControl(111)); // compare
 dialog.add(new ButtonDialogControl(112)); // triangulate
 dialog.add(new ButtonDialogControl(113)); // clone (105->113)
 dialog.add(new ButtonDialogControl(114)); // clone all (106->114)

 if (change != NULL)
 {
     if(changecount == 1)
        dialog.title(_RCT("Patch Verify"));
     else
         dialog.title(rs6.gets());
     // debug
     change->setall();
      //change->clearall();

      //dialog.add(new CheckBoxDialogControl(200,change,4));
      //dialog.add(new CheckBoxDialogControl(201,change,5));
      //dialog.add(new CheckBoxDialogControl(202,change,7));
      //dialog.add(new CheckBoxDialogControl(203,change,6));
      //dialog.add(new CheckBoxDialogControl(207,change,8));

      dialog.add(new PatchVerifyButtonControl(1000));
      dialog.add(new PatchVerifyButtonControl(1001));
      dialog.add(new PatchVerifyButtonControl(1002));
      dialog.add(new PatchVerifyButtonControl(1003));
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

         options->set(1,edgemode % 2);  options->set(2,edgemode / 2);

     }
     else
     {
         options->set(1,edgemode % 2);  options->set(2,edgemode / 2);
     }
 }
 if(inheader == 0)
     delete header;
 return status;
}

void Patch::getdefaults(int *ud,int *vd,int *xm,int *ym,BitMask *o)
{ *o = *v.getbm("sp::options");
  *xm = v.getinteger("sp::xmesh");
  *ym = v.getinteger("sp::ymesh");
  *ud = v.getinteger("sp::udegree");
  *vd = v.getinteger("sp::vdegree");
}

void Patch::setdefaults(int ud,int vd,int xm,int ym,const BitMask &o)
{ v.setbm("sp::options",o);
  v.setinteger("sp::xmesh",xm);
  v.setinteger("sp::ymesh",ym);
  v.setinteger("sp::udegree",ud);
  v.setinteger("sp::vdegree",vd);
}

void Patch::setdefaults(void)
{ v.setbm("sp::options",options);
  v.setinteger("sp::xmesh",xmesh);
  v.setinteger("sp::ymesh",ymesh);
  v.setinteger("sp::udegree",udegree);
  v.setinteger("sp::vdegree",vdegree);
}

/**
 * @brief Patch::verify
 *
 * Entry point for the Patch Verify Dialog
 *
 */
void Patch::verify(void)
{
    //static RCCHAR *edgemodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
    RCSTRING STRedgemodel[] = { "Normal", "Edges","Edges Only","Edges and Border" };
    RCCHAR em1[25],em2[25],em3[25],em4[25];
    RCCHAR *edgemodel[] = { em1,em2,em3,em4 };
    Dialog dialog("PatchVerify_Dialog");
    int c,l,s,status,edgemode;
    double w;
    //RCCHAR edgemode[300];
    Entity *e;

    for(int i=0; i<4; i++)
        strcpy(edgemodel[i],&STRedgemodel[i]);

    edgemode = options.test(1) + options.test(2) * 2;

    EntityHeader::verify(&dialog,&c,&l,&s,&w);
    dialog.add(new PatchVerifyButtonControl(1000));
    dialog.add(new PatchVerifyButtonControl(1001));
    dialog.add(new PatchVerifyButtonControl(1002));
    dialog.add(new PatchVerifyButtonControl(1003));

    dialog.add(new IntegerDialogControl(104,&udegree));
    dialog.add(new IntegerDialogControl(105,&vdegree));
    dialog.add(new IntegerDialogControl(106,&ymesh));
    dialog.add(new IntegerDialogControl(107,&xmesh));
    //dialog.add(new StringDialogControl(110,edgemode,300));
    dialog.add(new ListDialogControl(110,4,edgemodel,&edgemode));
    //dialog.add(new ButtonDialogControl(108));
    dialog.add(new ButtonDialogControl(109));
    dialog.add(new ButtonDialogControl(111));
    dialog.add(new ButtonDialogControl(112));
    //strcpy(edgemode,edgemodel[options.test(1) + options.test(2) * 2]);

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

        options.set(1,edgemode % 2);
        options.set(2,edgemode / 2);

        if (udegree > un - 1) udegree = un - 1;
        delete [] uknot;  delete [] utknot;  uknot = 0;
        tempknots(1,&utknot);

        if (vdegree > vn - 1) vdegree = vn - 1;
        delete [] vknot;  delete [] vtknot;  vknot = 0;
        tempknots(0,&vtknot);

        db.saveundo(UD_ENDBLOCK,NULL);

        draw(DM_NORMAL);
    }
    else if (status == 108) // setup (not used)
        ::state.sendevent(new NewCommandEvent(419,0));
    else if (status == 112) // triangulate
    {
        state.destroyselection(1);
        if ((e = triangulatedclone(1)) != 0)
        {   db.saveundo(UD_STARTBLOCK,NULL);
            db.geometry.add(e);
            db.geometry.del(this);
            db.saveundo(UD_ENDBLOCK,NULL);
        }
    }  
    else if (status == 109) // change suface properties
    {
        SurfaceProperties sp;
        sp.verify(this,0);
    }
    else if (status == 111) // compare
        ::state.sendevent(new NewCommandEvent(426,0));
  
}

void Patch::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{Point p3;
 int i,n;

    n = npoints();
    for (i = 0 ; i < n ; i++)
    {   p3 = polygon[i];
        if (vs != NULL) p3 = vs->modeltouv(p3);
        if (i == 0)
            *pmin = *pmax = p3;
        else
        {   if (p3.x < pmin->x) pmin->x = p3.x;
            if (p3.y < pmin->y) pmin->y = p3.y;
            if (p3.z < pmin->z) pmin->z = p3.z;
            if (p3.x > pmax->x) pmax->x = p3.x;
            if (p3.y > pmax->y) pmax->y = p3.y;
            if (p3.z > pmax->z) pmax->z = p3.z;
        }
    }

}

int Patch::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(patch_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int Patch::explode(void)
{ return 0;
}

int Patch::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int i,xm,ym,result,cstate,index;
 double wt;
 static RCCHAR *drawmodel[] = {  _RCT("Normal"), _RCT("Edges"), _RCT("Edges Only"), _RCT("Edges and Border")  };
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
        else if (! options.test(PatchTriangulated) && id == 18) 
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
        else if (options.test(PatchTriangulated))
        {   if (id == 6)
            {   strcpy(text,"Trangulated");
                return 1;
            }
            else if (id == 7)
                sprintf(text,"%d",property_u_index);
            else if (id == 8)
                db.formatnumber(text,polygon[property_u_index].x,FormatForEditField);
            else if (id == 9)
                db.formatnumber(text,polygon[property_u_index].y,FormatForEditField);
            else if (id == 10)
                db.formatnumber(text,polygon[property_u_index].z,FormatForEditField);
            else if (id == 11)
                sprintf(text,"%d",nvertices);
        }
        else
        {   if (id == 6)
            {   strcpy(text,"NURBS");
                return 1;
            }
            else if (id == 7)
                sprintf(text,"%d",property_u_index);
            else if (id == 8)
                sprintf(text,"%d",property_v_index);
            else if (id == 9)
                db.formatnumber(text,polygon[property_v_index * un + property_u_index].x,FormatForEditField);
            else if (id == 10)
                db.formatnumber(text,polygon[property_v_index * un + property_u_index].y,FormatForEditField);
            else if (id == 11)
                db.formatnumber(text,polygon[property_v_index * un + property_u_index].z,FormatForEditField);
            else if (id == 12)
                sprintf(text,"%d",udegree);
            else if (id == 13)
                sprintf(text,"%d",vdegree);
            else if (id == 14)
            {   sprintf(text,"%d",un);
                return 1;
            }
            else if (id == 15)
            {   sprintf(text,"%d",vn);
                return 1;
            }
            else if (id == 16)
                sprintf(text,"%d",xmesh);
            else if (id == 17)
                sprintf(text,"%d",ymesh);
            else if (id == 18)
                strcpy(text,drawmodel[options.test(1) + options.test(2) * 2]);
        }
    }
    else if (action == PP_SETVALUE)
    {   if (options.test(PatchTriangulated))
        {Expression e(text);
         double x;
            if (id == 7)
            {   property_u_index = atoi(text);
                if (property_u_index < 0)
                    property_u_index = 0;
                if (property_u_index > nvertices-1)
                    property_u_index = nvertices-1;
            }
            else if (id == 8 && e.evaluate(&x))
            {   t.translate(Point(x * db.getmainunitscale(),polygon[property_u_index].y,polygon[property_u_index].z) - polygon[property_u_index]);
                index = property_u_index;
                movepoints(1,1,&index,&t);
            }
            else if (id == 9 && e.evaluate(&x))
            {   t.translate(Point(polygon[property_u_index].x,x * db.getmainunitscale(),polygon[property_u_index].z) - polygon[property_u_index]);
                index = property_u_index;
                movepoints(1,1,&index,&t);
            }
            else if (id == 10 && e.evaluate(&x))
            {   t.translate(Point(polygon[property_u_index].x,polygon[property_u_index].y,x * db.getmainunitscale()) - polygon[property_u_index]);
                index = property_u_index;
                movepoints(1,1,&index,&t);
            }

        }
        else
        {Expression e(text);
         double x;
            if (id == 7)
            {   property_u_index = atoi(text);
                if (property_u_index < 0)
                    property_u_index = 0;
                if (property_u_index > un-1)
                    property_u_index = un-1;
            }
            else if (id == 8)
            {   property_v_index = atoi(text);
                if (property_v_index < 0)
                    property_v_index = 0;
                if (property_v_index > vn-1)
                    property_v_index = vn-1;
            }
            else if (id == 9 && e.evaluate(&x))
            {   t.translate(Point(x * db.getmainunitscale(),polygon[property_v_index * un + property_u_index].y,polygon[property_v_index * un + property_u_index].z) - polygon[property_v_index * un + property_u_index]);
                index = property_v_index * un + property_u_index;
                movepoints(1,1,&index,&t);
            }
            else if (id == 10 && e.evaluate(&x))
            {   t.translate(Point(polygon[property_v_index * un + property_u_index].x,x * db.getmainunitscale(),polygon[property_v_index * un + property_u_index].z) - polygon[property_v_index * un + property_u_index]);
                index = property_v_index * un + property_u_index;
                movepoints(1,1,&index,&t);
            }
            else if (id == 11 && e.evaluate(&x))
            {   t.translate(Point(polygon[property_v_index * un + property_u_index].x,polygon[property_v_index * un + property_u_index].y,x * db.getmainunitscale()) - polygon[property_v_index * un + property_u_index]);
                index = property_v_index * un + property_u_index;
                movepoints(1,1,&index,&t);
            }
            else if (id == 12)
            {   change_mask.set(4,1);
                change(change_mask,*this,atoi(text),0,0,0,new_options);
            }
            else if (id == 13)
            {   change_mask.set(5,1);
                change(change_mask,*this,0,atoi(text),0,0,new_options);
            }
            else if (id == 16)
            {   change_mask.set(6,1);
                xm = atoi(text);
                if (xm < 2)
                    xm = 2;
                change(change_mask,*this,0,0,xm,0,new_options);
            }
            else if (id == 17)
            {   change_mask.set(7,1);
                ym = atoi(text);
                if (ym < 2)
                    ym = 2;
                change(change_mask,*this,0,0,0,ym,new_options);
            }
            else if (id == 18)
            {   change_mask.set(8,1);
                for (i = 0 ; i < 4 ; i++)
                    if (strcmp(text,drawmodel[i]) == 0)
                    {   new_options.set(1,i&1);
                        new_options.set(2,i&2);
                        change(change_mask,*this,0,0,0,0,new_options);
                    }
            }
        }

    }
    else if (action == PP_GETLABEL)
    {   if (id == 0)
        {   strcpy(text,"Entity");
            return 1;
        }
        else if (id == 5)
        {   strcpy(text,"Patch");
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
                strcpy(text,"Patch Type"); 
            else if (options.test(PatchTriangulated))
            {   if (id == 7)
                    strcpy(text,"Point Index");
                else if (id == 8)
                    strcpy(text,"Point X");
                else if (id == 9) 
                    strcpy(text,"Point Y");
                else if (id == 10)
                    strcpy(text,"Point Z");
                else if (id == 11)
                    strcpy(text,"Point Count");
            }
            else
            {   if (id == 7)
                    strcpy(text,"U Point Index");
                else if (id == 8)
                    strcpy(text,"V Point Index");
                else if (id == 9)
                    strcpy(text,"Point X");
                else if (id == 10) 
                    strcpy(text,"Point Y");
                else if (id == 11)
                    strcpy(text,"Point Z");
                else if (id == 12)
                    strcpy(text,"U Degree");
                else if (id == 13)
                    strcpy(text,"V Degree");
                else if (id == 14)
                    strcpy(text,"U Point Count");
                else if (id == 15)
                    strcpy(text,"V Point Count"); 
                else if (id == 16)
                    strcpy(text,"Horizontal Mesh");
                else if (id == 17)
                    strcpy(text,"Vertical Mesh");
                else if (id == 18)
                    strcpy(text,"Drawing Mode");
            }
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return options.test(PatchTriangulated) ? 12 : 19;
    
    return 0;
}



