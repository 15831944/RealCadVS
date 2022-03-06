
#include "ncwin.h"

EdgeTable::EdgeTable(int s)
{  size = s;
   lists = new EdgeList[size];
}

EdgeTable::~EdgeTable()
{int i;
 Edge *edge;
  if (lists != NULL)
    {  for (i = 0 ; i < size ; i++)
         {  for (lists[i].start() ; (edge = lists[i].next()) != NULL ;)
              delete edge;
            lists[i].destroy();
         }
       delete [] lists;
    }
}

int Edge::nextline(int y)
{ if (y == ymax) return 0;
  x += dx;
  return 1;
}

Edge::Edge(int shorten,Point p1,Point p2,Shader *s)
{Point p;
 int swapped,y1,y2;

  p1.y -= 1.0E-5;  p2.y -= 1.0E-5;  //  Move edge down to touch the bottom edge
  shader = s;
  swapped = 0;  if (p1.y > p2.y) {  p = p1;  p1 = p2;  p2 = p;  swapped = 1;  }
  y1 = (int)ceil(p1.y);  y2 = (int)floor(p2.y);
  if (y1 > y2 || p1.y == p2.y)
    {  horizontal = 1;  return;
    }
  horizontal = 0;
  dx = (p2.x - p1.x) / (p2.y - p1.y);
  ymin = y1;  if (ymin < 0) ymin = 0;
  ymax = y2;
  if (shorten)
    {  if (swapped && floor(p2.y) == ceil(p2.y))
         ymax--;
       else if (! swapped && floor(p1.y) == ceil(p1.y))
         ymin++;
    }
  if (ymax < ymin)
    {  horizontal = 1;  return;
    }
  x = p1.x + dx * (ymin - p1.y);
  shader = s;

}

void ActiveEdgeList::nextline(int y)
{Edge *edge;
 GeneralList list;
  for (start() ; (edge = next()) != NULL ; )
    if (! edge->nextline(y)) list.add(edge);
  for (list.start() ; (edge = (Edge *) list.next()) != NULL ; )
    {  del(edge);  delete edge;
    }
  list.destroy();
  resort();
}

FillSegments::FillSegments(Surface *s)
{ surface = s;
  y = x1 = x2 = r = g = b = -1;
  hbrush = NULL;
}

FillSegments::~FillSegments()
{ if (hbrush != NULL) DeleteObject(hbrush);
}

void FillSegments::fill(int newy,double dnewx1,double dnewx2,double z1,double z2,int newr,int newg,int newb)
{RECT rect;
 int samecol,sameseg,newx1,newx2;
  newx1 = (int)dnewx1;  newx2 = (int)dnewx2;

  if (newx1 == x2 && dnewx2 - dnewx1 < 0.2 && z1 < z + 1.0E-8)
    return;

  z = z2;  //  Depth of the end of the segment
  sameseg = (newy == y) && (newx1 <= x2 + 1);
  samecol = (newr == r) && (newg == g) && (newb == b);

  if (! sameseg || ! samecol)
    {  if (hbrush != NULL)
         {double rx1,rx2,ry1,ry2;
            if (surface->gettransform2d())
              {  rx1 = x1;  ry1 = y;
                 surface->transform2d(&rx1,&ry1);
                 rx2 = x2;  ry2 = y;
                 surface->transform2d(&rx2,&ry2);
                 if (rx1 < rx2)
                   {  rect.left = (int)rx1;  rect.right = (int)rx2;  rect.right++;
                   }
                 else
                   {  rect.left = (int)rx2;  rect.right = (int)rx1;  rect.right++;
                   }
                 if (ry1 < ry2)
                   {  rect.top = (int)ry1;  rect.bottom = (int)ry2;  rect.bottom++;
                   }
                 else
                   {  rect.top = (int)ry2;  rect.bottom = (int)ry1;  rect.bottom++;
                   }
              }
            else
              {  rect.left = x1;  rect.right = x2 + 1;
                 rect.top = int(surface->getheight()) - y - 1;  rect.bottom = rect.top + 1;
              }
            FillRect(surface->gethdc(),&rect,hbrush);
            if (! samecol)
              {  DeleteObject(hbrush);  hbrush = NULL;
              }
         }
       y = newy;  x1 = newx1;  x2 = newx2;  r = newr;  g = newg;  b = newb;
    }
  else
    x2 = newx2;
  if (hbrush == NULL && r >= 0 && g >= 0 && b >= 0)
    hbrush = CreateSolidBrush(RGB(r,g,b));
}

void FillSegments::flush(void)
{  fill(-1,-1,-1,-1,-1,-1,-1,-1);
}

void ActiveEdgeList::fill(Surface *surface,int y)
{GeneralList inlist;
 Edge *lastedge,*edge;
 Shader *shader1,*shader2;
 FillSegments fillsegments(surface);
 ResourceString rs0(NCSHADEA);

  start();

  if ((lastedge = next()) == NULL) return;
  lastedge->shader->in = ! lastedge->shader->in;
  inlist.add(lastedge->shader);
  while ((edge = next()) != NULL)
    {  for (inlist.start(),shader1 = NULL ; (shader2 = (Shader *) inlist.next()) != NULL ; )
         if (shader1 == NULL || shader2->infrontof(shader1,(lastedge->x + edge->x) / 2.0,y))
           shader1 = shader2;
       if (shader1 != NULL)
         shader1->fill(&fillsegments,y,lastedge->x,edge->x,
                    (shader1->D + shader1->normal.x * lastedge->x + shader1->normal.y * y) /  shader1->normal.z,
                    (shader1->D + shader1->normal.x * edge->x + shader1->normal.y * y) /  shader1->normal.z);
       edge->shader->in = ! edge->shader->in;
       if (edge->shader->in)
         inlist.add(edge->shader);
       else
         inlist.del(edge->shader);
       lastedge = edge;
    }
  fillsegments.flush();
  if (inlist.length() > 0)
    {  cadwindow->MessageBox(rs0.gets(),"",MB_ICONSTOP);
    }
}

Shader::Shader(int red,int green,int blue,Point screenn,double screend,Point n,Point l,double ambient,double diffuse)
{double t;
  in = 0;
  t = ambient + fabs(n.dot(l)) * diffuse;
  r = int(t * red);
  g = int(t * green);
  b = int(t * blue);
  normal = screenn;
  D = screend;
if (fabs(normal.z) < 0.001)
  normal.z += 1.0e-100;
}

void Shader::fill(FillSegments *fillsegment,int y,double x1,double x2,double z1,double z2)
{ fillsegment->fill(y,x1,x2,z1,z2,r,g,b);
}

int Shader::infrontof(Shader *shader1,double x,double y)
{ if (fabs(normal.z) < db.getptoler() || fabs(shader1->normal.z) < db.getptoler())
    return 0;
  return ((shader1->D + shader1->normal.x * x + shader1->normal.y * y) /
             shader1->normal.z < (D + normal.x * x + normal.y * y) / normal.z);
}

int Polygon3d::addedges(SurfaceEntity *se,RenderImage *image)
{Shader *shader;
 int shorten,i,j,k;
 double y1,y2,y3,ylast;
 Point org,xa,ya,za,p1,p2,org1,xa1,ya1,za1;
 Edge *edge;

  clip(image->getsurface());
  org = image->getsurface()->modeltoscreen(origin);
  xa = (image->getsurface()->modeltoscreen(origin + xaxis * 1000.0) - org).normalize();
  ya = (image->getsurface()->modeltoscreen(origin + yaxis * 1000.0) - org).normalize();
  za = xa.cross(ya).normalize();
  if (za.length() < db.getptoler())
    return 1;
  if (fabs(za.z) < 0.001)
    return 1;
  if ((shader = new Shader(db.colourtable[se->getcolour()].red,db.colourtable[se->getcolour()].green,db.colourtable[se->getcolour()].blue,za,-za.dot(org),xaxis.cross(yaxis),
                           image->getsurface()->waxis,image->getambient(),image->getdiffuse())) == NULL) return 0;
  image->addshader(shader);
  for (i = 0 ; i < nloops ; i++)
    {  p1 = image->getsurface()->modeltoscreen(verts[i][0]);  y1 = p1.y;
       ylast = image->getsurface()->modeltoscreen(verts[i][nvert[i]-1]).y;
       for (k = nvert[i]-2 ; y1 == ylast && k > 0 ; k--)
         ylast = image->getsurface()->modeltoscreen(verts[i][k]).y;
       for (j = 1 ; j <= nvert[i] ; j++)
         {  p2 = image->getsurface()->modeltoscreen(verts[i][j % nvert[i]]);
            y2 = p2.y;
            for (k = 0,y3 = y2  ; y1 == y3 && k < nvert[i] ; k++)
              y3 = image->getsurface()->modeltoscreen(verts[i][(k + j + 1) % nvert[i]]).y;
            shorten = ylast < y1 && y1 < y3 ||
                      y3 < y1 && y1 < ylast;
            if ((edge = new Edge(shorten,p1,p2,shader)) == NULL) return 0;
            if (edge->horizontal == 1)
              delete edge;
            else
              {  if (! image->getet()->add(edge)) return 0;
              }
            if (y1 != y2) ylast = y1;
            p1 = p2;  y1 = y2;
         }
    }
  return 1;
}

int RenderImage::addedges(SurfaceEntity *se,int nvert,Point *point)
{Shader *shader;
 int shorten,i,y1,y2,ylast,status;
 Point org,xa,ya,za,p1,p2,org1,xa1,ya1,za1,origin,xaxis,yaxis;
 Edge *edge;

  //  Check that the polygon is entirely in the viewport
  for (i = 0 ; i < nvert ; i++)
    {  p1 = surface->modeltoview(point[i]);
       if (! surface->inside(p1))
         break;
    }

  //  If some of the polygon is out of the viewport, do the full processing
  if (i < nvert)
    {Polygon3d *poly;
      if (nvert == 3)
        poly = new Polygon3d(point[0],point[1],point[2]);
      else
        poly = new Polygon3d(point[0],point[1],point[2],point[3]);
      if (poly->getdefined())
        status = poly->addedges(se,this);
      else
        status = 1;
      delete poly;
      return status;
    }

  //  Do the simple processing
  origin = point[0];
  xaxis = (point[1] - point[0]).normalize();
  yaxis = xaxis.cross(point[2] - point[0]).cross(xaxis).normalize();

  org = surface->modeltoscreen(point[0]);
  xa = (surface->modeltoscreen(origin + xaxis * 1000.0) - org).normalize();
  ya = (surface->modeltoscreen(origin + yaxis * 1000.0) - org).normalize();
  za = xa.cross(ya).normalize();
  if (za.length() < db.getptoler())
    return 1;
  if (fabs(za.z) < 0.001)
    za.z += 1.0E-30;
  if ((shader = new Shader(db.colourtable[se->getcolour()].red,db.colourtable[se->getcolour()].green,db.colourtable[se->getcolour()].blue,za,-za.dot(org),xaxis.cross(yaxis),
                           surface->getwaxis(),getambient(),getdiffuse())) == NULL) return 0;
  addshader(shader);
  p1 = surface->modeltoscreen(point[nvert-1]);  y1 = int(p1.y);
  ylast = int(surface->modeltoscreen(point[nvert-2]).y);
  for (i = 0 ; i < nvert ; i++)
    {  p2 = surface->modeltoscreen(point[i]);  y2 = int(p2.y);
       shorten = ylast < y1 && y1 < y2 || y2 < y1 && y1 < ylast;
       if ((edge = new Edge(shorten,p1,p2,shader)) == NULL) return 0;
       if (edge->horizontal == 1)
         delete edge;
       else
         {  if (! getet()->add(edge)) return 0;
         }
       if (y1 != y2) ylast = y1;
       p1 = p2;  y1 = y2;
    }
  return 1;
}

static int shadecallback(SurfaceEntity *se,void *polygon,int nvert,void *pts,void *cbdata)
{RenderImage *ri = (RenderImage *)cbdata;
 int status;
  if (nvert == 0)
    {  if (! ((Polygon3d *)polygon)->getdefined()) return 1;
       status = ((Polygon3d *)polygon)->addedges(se,ri);
       delete ((Polygon3d *)polygon);
    }
  else
    status = ri->addedges(se,nvert,(Point *)pts);
  return status;
}

#ifdef USING_WIDGETS
void RenderImage::renderGL(void)
{
    Entity *e;

    if (v.getinteger("sh::openglshading") && SetLight != 0)
    {
       View *view;
       BitMask foptions(32);
       Point eye,p,pmin,pmax;
       int i,k,nbands,bandheight,bandwidth;
       //View3dOffScreenBitmap *bitmap;

        //db.clearScene(DM_ERASE,surface);
        //QtBeginScene();

        db.clearScene(DM_ERASE,surface);
        //QtBeginScene();
        for (db.geometry.end(); (e = db.geometry.last()) != NULL ; )
        {
          if (e->issurface() && e->isvisible(surface))
          {
              if (((SurfaceEntity *)e)->getntriangles() < 0)
                  ((SurfaceEntity *)e)->triangulate((surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler());

              e->drawGL(DM_NORMAL,0,surface);
          }

          //if (e->isa(bitmap_entity) && e->isvisible(surface) && ((BitMapE *)e)->getoptions().test(9))
          if (e->isa(bitmap_entity) && e->isvisible(surface))
               e->drawGL(DM_NORMAL,0,surface);
        }
        //QtEndScene();
    }
}
#endif

void RenderImage::render(void)
{
 ActiveEdgeList aelist;
 Entity *e;
 Edge *edge;
 Shader *shader;
 double error;
 BitMask options(32);
 int j,t,nlights,stopped,y,nhor,nver;
 RECT rect;
 ResourceString rs1(NCREVA+1);
 ResourceString rsnc0(NC);
 int ntriangles;
 DPoint *triangles;
 //Point *triangles;
 SurfaceProperties *sp;
 double ambient,diffuse;

#if USING_WIDGETS
 if (v.getinteger("sh::openglshading") && SetLight != 0)
 {
    View *view;
    BitMask foptions(32);
    Point eye,p,pmin,pmax;
    int i,k,nbands,bandheight,bandwidth;
    //View3dOffScreenBitmap *bitmap;

#if 1
     //db.clearScene(DM_ERASE,surface);
     //QtBeginScene();

#else
     db.clearScene(DM_ERASE,surface);
     //QtBeginScene();
     for (db.geometry.end(),stopped = 0 ; (e = db.geometry.last()) != NULL ; )
     {
       if (e->issurface() && e->isvisible(surface))
       {
           if (((SurfaceEntity *)e)->getntriangles() < 0)
               ((SurfaceEntity *)e)->triangulate((surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler());

           e->draw(DM_GL,0,surface);
           //e->draw(DM_NORMAL,0,surface);
       }

       if (e->isa(bitmap_entity) && e->isvisible(surface) && ((BitMapE *)e)->getoptions().test(9))
            e->draw(DM_NORMAL,0,surface);
     }
#endif
     //QtEndScene();
 }
#else
  if (v.getinteger("sh::openglshading") && SetLight != 0)
    {View *view;
     BitMask foptions(32);
     Point eye,p,pmin,pmax;
     int i,k,nbands,bandheight,bandwidth;
     View3dOffScreenBitmap *bitmap;

       //program->processpaintmessages();

       ambient = v.getreal("sh::ambient"); 
       diffuse = v.getreal("sh::diffuse");

       if (surface->gethwnd() == 0 && typeid(*surface) != typeid(class View3dOffScreenBitmap)) 
         {  //  Render to an offscreen image
            nbands = int((surface->getwidth() * surface->getheight() * 3 + 9999999) / 10000000);
            bandwidth = (int)surface->getwidth();
            bandheight = (int)(surface->getheight() / nbands);
            bitmap = new View3dOffScreenBitmap(cadwindow->getcurrentwindow(),bandwidth,bandheight);
         }
       else  
         {  bitmap = 0;
            nbands = 1;
            bandheight = (int)surface->getheight();
         } 

        for (i = 0 ; i < nbands ; i++)
          {  
             if (bitmap != 0) 
               {  OpenOpenGl(bitmap->gethdc(),2,0,0,bandwidth,bandheight);
                  bitmap->setbackground(RGB(255,255,255));
                  bitmap->setplotscale(surface->getplotscale());
               }
             else  
               OpenOpenGl(surface->gethdc(),0 + 2 * (surface->gethwnd() == 0),0,0,(int)surface->getwidth(),(int)surface->getheight());

             if (bitmap != 0)
               bitmap->zoom(surface->getumin(),surface->getvmin()+(surface->getvmax()-surface->getvmin())*(nbands-1-i)/nbands,
                            surface->getumax(),surface->getvmin()+(surface->getvmax()-surface->getvmin())*(nbands-i)/nbands);
             foptions.set(2);
             view = surface->getview();

             db.geometry.extents(surface,&pmin,&pmax);

             if (view->getperspective())
               eye = view->geteye();
             else
               eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

             if (view->getperspective()) 
               SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
             else
               SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

             SetViewport(surface->getumin(),surface->getvmin()+(surface->getvmax()-surface->getvmin())*(nbands-1-i)/nbands,surface->getumax(),surface->getvmin()+(surface->getvmax()-surface->getvmin())*(nbands-i)/nbands);

             SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

             BeginScene(BS_FRONTBUFFERONLY);

             nlights = 0;
             for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
               if (e->isa(light_entity) && e->isvisible(surface))
                 {double red,green,blue;
                    if (((Light *)e)->getoptions().test(1))
                      {  red = green = blue = 1.0;
                      }
                    else  
                      {  red = db.colourtable[e->getcolour()].red/255.0;
                         green = db.colourtable[e->getcolour()].green/255.0;
                         blue = db.colourtable[e->getcolour()].blue/255.0;
                      }
               
                    SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                    nlights++;
                 }

             if (nlights == 0)
               SetLight(eye.x,eye.y,eye.z);


             for (db.geometry.end(),stopped = 0 ; (e = db.geometry.last()) != NULL ; )
               if (e->issurface() && e->isvisible(surface))
                 {  if (((SurfaceEntity *)e)->getntriangles() < 0)
                      ((SurfaceEntity *)e)->triangulate((surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler());

                    ntriangles = ((SurfaceEntity *)e)->getntriangles();
                    triangles = ((SurfaceEntity *)e)->gettriangles();

                    for (k = 0,j = 0 ; k < ntriangles ; k++,j += 9)
                      {  if (triangles[j].z == RCDBL_MAX)
                           {  sp = (SurfaceProperties *) (int)triangles[j].x;
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
                 }

             for (db.geometry.end(),stopped = 0 ; (e = db.geometry.last()) != NULL ; )
               if (e->isa(bitmap_entity) && e->isvisible(surface) && ((BitMapE *)e)->getoptions().test(9))
                 e->draw(DM_NORMAL,0,bitmap == 0 ? surface : bitmap);

             EndScene();

             if (bitmap != 0)
               {int xoffset,yoffset;
                  xoffset = GetDeviceCaps(surface->gethdc(), PHYSICALOFFSETX);
                  yoffset = GetDeviceCaps(surface->gethdc(), PHYSICALOFFSETY);
                  BitBlt(surface->gethdc(),(int)surface->getoriginx()-xoffset,(int)(surface->getoriginy() + i*bandheight)-yoffset,(int)surface->getwidth(),bandheight,bitmap->gethdc(),0,0,SRCCOPY);
               }
          }
      
        delete bitmap;



       return;
    }

  if ((et = new EdgeTable(int(surface->getheight()+1.0))) == NULL)
    {  cadwindow->MessageBox(rs1.gets(),rsnc0.gets(),MB_ICONINFORMATION);
       return;
    }

  //program->processpaintmessages();
  program->setbusy(1);
  for (db.geometry.start(),stopped = 0 ; (e = db.geometry.next()) != NULL ; )
    if (e->issurface() && e->isvisible(surface))
      {SurfaceEntity *se = (SurfaceEntity *) e;
         error = (surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler();
         se->nfacets(error,&nhor,&nver);
         options.set(1);
         se->facet(options,error,nhor,nver,surface,shadecallback,this);
         if ((stopped = program->getinterrupt()) != 0) break;
      }

  if (! stopped)
    {  rect.left = 0;  rect.top = 0;  rect.right = int(surface->getwidth());  rect.bottom = int(surface->getheight());
       if (clear)
         {RCHBRUSH hbrush;
            hbrush = CreateSolidBrush(surface->getbackground());
            FillRect(surface->gethdc(),&rect,hbrush);
            DeleteObject(hbrush);
         }
       for (y = 0 ; y < surface->getheight() ; y++)
         {  aelist.merge((*et)[y]);
            aelist.fill(surface,y);
            aelist.nextline(y);
            if (program->getinterrupt()) break;
         }
    }

  for (aelist.start() ; (edge = aelist.next()) != NULL ;)
    delete edge;

  aelist.destroy();
  delete et;


  shaders.reverse();

  for (shaders.start() ; (shader = (Shader *) shaders.next()) != 0 ; )
    delete shader;

  shaders.destroy();

  program->setbusy(0);
#endif
}
