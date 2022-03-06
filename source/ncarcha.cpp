
#include "ncwin.h"


//     Wall(const EntityHeader&, Point,Point,Point,RCCHAR *,RCCHAR *strWM,double,double,double,double,double,double,int,int,double,double,double,double,double,double,short,double *,const BitMask&,const BitMask&,bool onInsert=false);
//     Wall(                     Point,Point,Point,RCCHAR *,RCCHAR *strWM,double,double,double,double,double,double,int,int,double,double,double,double,double,double,short,double *,const BitMask&,const BitMask&,bool onInsert=false);


Wall::Wall(Point p11,Point p21,Point za,RCCHAR *n,RCCHAR *,double w,double h,double e,double p,double pd,double ph,int mc,int rc,double lhs,double rhs,double lha,double rha,double a1,double a2,short nattributes_,double *attributes_,const BitMask &o1,const BitMask &o2,bool) :
  options(32),options2(32)
{ p1 = p11;  p2 = p21;  zaxis = za;  width = w;  height = h;  angle1 = a1;  angle2 = a2;  options = o1;
  eaves = e;
  pitch = p;
  peakdistance = pd;
  peakheight = ph;
  lhatchspacing = lhs;
  rhatchspacing = rhs;
  lhatchangle = lha;
  rhatchangle = rha;
  middlecolour = short(mc);
  rightcolour = short(rc);
  if ((name = new RCCHAR[strlen(n)+1]) == 0)
    {  defined = 0;
       return;
    }
  nattributes = nattributes_;
  attributes = new double[nattributes];
  memcpy(attributes,attributes_,nattributes*sizeof(double));
  strcpy(name,n);
  options.set(ArWallAttributes);
  options.set(ArWallFrameExtendStart,1);
  options.set(ArWallFrameExtendEnd,1);
  defined = 1;
}

Wall::Wall(const EntityHeader &header,Point p11,Point p21,Point za,RCCHAR *n,RCCHAR *,double w,double h,double e,double p,double pd,double ph,int mc,int rc,double lhs,double rhs,double lha,double rha,double a1,double a2,short nattributes_,double *attribute_,const BitMask &o1,const BitMask &o2,bool) :
  SurfaceEntity(header),options(32),options2(32)
{ p1 = p11;  p2 = p21;  zaxis = za;  width = w;  height = h;  angle1 = a1;  angle2 = a2;  options = o1;
  eaves = e;
  pitch = p;
  peakdistance = pd;
  peakheight = ph;
  lhatchspacing = lhs;
  rhatchspacing = rhs;
  lhatchangle = lha;
  rhatchangle = rha;
  middlecolour = short(mc);
  rightcolour = short(rc);
  if ((name = new RCCHAR[strlen(n)+1]) == 0)
    {  defined = 0;
       return;
    }
  nattributes = nattributes_;
  attributes = new double[nattributes];
  memcpy(attributes,attribute_,nattributes*sizeof(double));
  strcpy(name,n);
  options.set(ArWallAttributes);
  options.set(ArWallFrameExtendStart,1);
  options.set(ArWallFrameExtendEnd,1);
  defined = 1;
}

Wall::~Wall()
{Entity *e;
  for (openinglist.start() ; (e = openinglist.next()) != 0 ; )
    {  e->dela(this);
       if (e->getalist().empty())
         db.geometry.del(e,cadwindow != 0);
    }
  openinglist.destroy();
  delete name;
}

void Wall::dbinsert(Database *db,int draw)
{Entity *e;
  for (openinglist.start() ; ! openinglist.atend() ; )
    {  if ((e = openinglist.next()) != NULL)
         {  if (db != NULL && e->getposition() == NULL) db->geometry.add(e,draw);
            e->adda(this);
         }
    }
  Entity::dbinsert(db,draw);

}

void Wall::dbdelete(void)
{Entity *e;
  for (openinglist.start() ; ! openinglist.atend() ; )
    {  if ((e = openinglist.next()) != NULL)
         e->dela(this);
    }
  Entity::dbdelete();
}

void Wall::dbsetassociated(void)
{Entity *e;
  for (openinglist.start() ; ! openinglist.atend() ; )
    {  if ((e = openinglist.next()) != NULL)
         e->setstatus(1);
    }
}

void Wall::dbtraverseassociated(EntityCallBack c,void *d)
{Entity *e;
  for (openinglist.start() ; (e = openinglist.next()) != 0 ; )
    c(e,d);
}

double Wall::pick(View3dSurface *v,double x,double y,Point *pnear)
{Point xaxis,yaxis,p3,p4,p5;
 double d,dmin,totalwidth;
 int j;
 View3dWindow *window;

  window = dynamic_cast<View3dWindow *>(v);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    v = window->getoffscreenbitmap();
 
  if (pnear == 0 && v->displaylist.defined(this))
    return v->displaylist.pick(this,x,y);

  dmin = -1.0;
  //  Operate in 2D only when picking using pnear.
  xaxis = (p2 - p1).normalize();
  yaxis = (p2 - p1).cross(zaxis).normalize();
  totalwidth = gettotalwidth();
  for (j = 0 ; j < 4 ; j++)
    if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0 || j == 0 || j == 3)
      {  if (j == 0)
           {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
              p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
           }
         else if (j == 1)
           {  p3 = p1 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(angle1));
              p4 = p2 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(-angle2));
           }
         else if (j == 2)
           {  p3 = p1 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(angle1));
              p4 = p2 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(-angle2));
           }
         else
           {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
              p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
           }

         Line line(*this,p3,p4);
         d = line.pick(v,x,y,&p5);

         if (d >= 0.0 && (dmin < 0.0  || d < dmin))
           {  dmin = d;  
              if (pnear != 0)
                *pnear = p5;  
           }  
      }
  return dmin;
}

int Wall::pick(View3dSurface *vs,int insidew,int x1,int y1,int x2,int y2)
{View3dWindow *window;
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
  if (vs->displaylist.defined(this))
    return vs->displaylist.pick(this,insidew,x1,y1,x2,y2);
  //  Require display list to be operating when picking walls...
  return 0;
}

int Wall::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Point xaxis,yaxis,p3,p4,p5,p6,p7,p8,za,xax1,xax2;
 int i,j,picked;
 double totalwidth;
 View3dWindow *window;

  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  picked = inside != 2;
  if (vs->getwaxis().z > 0.99999) //  TOP View
    {  xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize();
       totalwidth = gettotalwidth();
       for (j = 0 ; j < 4 ; j++)
         if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0 || j == 0 || j == 3)   
           {  if (j == 0)
                {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                }
              else if (j == 1)
                {  p3 = p1 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(angle1));
                   p4 = p2 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(-angle2));
                }
              else if (j == 2)
                {  p3 = p1 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(angle1));
                   p4 = p2 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(-angle2));
                }
              else
                {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                }
              
              if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
                return picked;              
         }
    }
  else
    {  xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize();
       totalwidth = gettotalwidth();
       for (i = 0 ; i < 2 ; i++)
         {  if (i == 0 && ArWallHeight2 < nattributes && attributes[ArWallCavity] != 0.0)
              za = zaxis * attributes[ArWallHeight2];
            else 
              za = zaxis * height;
            if (i == 0)
              {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
              }
            else
              {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
              }
            if (vs->pick(nvertices,pwvertices,p1+yaxis+xax1,p2+yaxis+xax2,inside,&picked))
              return picked;
            if (vs->pick(nvertices,pwvertices,p2+yaxis+xax2,p2+yaxis+za+xax2,inside,&picked))
              return picked;
            if (fabs(peakheight) > 1.0E-5)
              {  p3 = (p2 - p1) * peakdistance / 100.0 + zaxis * peakheight;
                 if (vs->pick(nvertices,pwvertices,p2+yaxis+xax2+za,p1+yaxis+p3+za,inside,&picked))
                   return picked;
                 if (vs->pick(nvertices,pwvertices,p1+yaxis+p3+za,p1+xax1+yaxis+za,inside,&picked))
                   return picked;
              }
            else
              {  if (vs->pick(nvertices,pwvertices,p2+yaxis+xax2+za,p1+xax1+yaxis+za,inside,&picked))
                   return picked;
              }
            if (vs->pick(nvertices,pwvertices,p1+yaxis+xax1+za,p1+xax1+yaxis,inside,&picked))
              return picked;

         }
    }

   return picked;

}

int Wall::pickpoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{double d,dmin,totalwidth;
 int i,j,end;
 Point p3,p4,p5,p6,xaxis,yaxis,dir;
 SortedDoubleList sdl,esdl;
 RCCHAR *leaf;
 Figure *figure;
 EntityList clist;
 Entity *e;

  totalwidth = gettotalwidth();
  xaxis = (p2 - p1).normalize();
  yaxis = (p2 - p1).cross(zaxis).normalize();
  end = 0;

  dmin = 1.0E30;
  i = 0;
  for (j = 0 ; j < 2 ; j++)
    {  if (j == 0)
         {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
            p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
            sdl = esdl;
            for (break2dlist1.start() ; ! break2dlist1.atend() ; )
              sdl.add(break2dlist1.next());
            leaf = _RCT("1");
         }
       else
         {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
            p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
            sdl = esdl;
            for (break2dlist4.start() ; ! break2dlist4.atend() ; )
              sdl.add(break2dlist4.next());
            leaf = _RCT("4");
         }

       dir = (p4 - p3).normalize();
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(0,leaf,&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  if (e->isa(line_entity))
                     {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                        p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                        sdl.add(dir.dot(p5 - p3));
                        sdl.add(dir.dot(p6 - p3));
                     }
                   delete e;
                }
              clist.destroy();
           }

       i++;
       if ((d = vs->pick(p3,distance,x,y)) >= 0.0 && d < dmin)
         {  end = i;  *pos = p3;  dmin = d;
         }
       i++;
       if ((d = vs->pick(p4,distance,x,y)) >= 0.0 && d < dmin)
         {  end = i;  *pos = p4;  dmin = d;
         }
       
       for (sdl.start() ; ! sdl.atend() ; )
         {  i++;
            p5 = p3 + dir * sdl.next();
            if ((d = vs->pick(p5,distance,x,y)) >= 0.0 && d < dmin)
              {  end = i;  *pos = p5;  dmin = d;
              }  
         }

       sdl.destroy();

    }   

  return end;
}

int Wall::add(Figure *figure)
{int status;
  db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status = openinglist.add(figure);
  figure->adda(this);
  draw(DM_NORMAL);
  return status;
}

void Wall::del(Figure *figure)
{ db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  openinglist.del(figure);
  figure->dela(this);
  draw(DM_NORMAL);
}

void Wall::addtodisplaylist(View3dSurface *vs,int child)
{int i,j,k;
 long size;
 Point xaxis,xax1,xax2,yaxis,za,p3,p4,p5,p6,dir;
 EntityList list,emptylist;
 BitMask o(32);
 SortedDoubleList sdl,esdl;
 Figure *figure;
 Entity *e;
 Line *line[5];
 EntityList clist;
 RCCHAR *leaf;
 double totalwidth,walllength,t1;
  if (! child && getposition() == 0) return;
  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  walllength = (p2 - p1).length();
  for (i = child,size = 0 ; i < 2 ; i++)
    {  if (! child)
         {  if (i == 0)
              vs->displaylist.setcountmode();
            else
              vs->displaylist.setsavemode();
            if (! vs->displaylist.start(this,size)) 
                return;
         }
       if (vs->getwaxis().z > 0.99999) //  TOP View
         {  xaxis = (p2 - p1).normalize();
            yaxis = (p2 - p1).cross(zaxis).normalize();
            for (j = 0 ; j < 4 ; j++)
              if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0 || j == 0 || j == 3)
                {  if (j == 0)
                     {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                        p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                        sdl = esdl;
                        for (break2dlist1.start() ; ! break2dlist1.atend() ; )
                          sdl.add(break2dlist1.next());
                        leaf = _RCT("1");
                     }
                   else if (j == 1)
                     {  p3 = p1 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(angle1));
                        p4 = p2 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(-angle2));
                        sdl = esdl;
                        for (break2dlist2.start() ; ! break2dlist2.atend() ; )
                          sdl.add(break2dlist2.next());
                        leaf = _RCT("2");
                     }
                   else if (j == 2)
                     {  p3 = p1 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(angle1));
                        p4 = p2 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(-angle2));
                        sdl = esdl;
                        for (break2dlist3.start() ; ! break2dlist3.atend() ; )
                          sdl.add(break2dlist3.next());
                        leaf = _RCT("3");
                     }
                   else
                     {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                        p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                        sdl = esdl;
                        for (break2dlist4.start() ; ! break2dlist4.atend() ; )
                          sdl.add(break2dlist4.next());
                        leaf = _RCT("4");
                     }

                   dir = (p4 - p3).normalize();
                   for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
                     if (figure->cutout(0,leaf,&clist))
                       {  for (clist.start() ; (e = clist.next()) != 0 ; )
                            {  if (e->isa(line_entity))
                                 {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                                    p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                                    sdl.add(fabs((p5 - p3).dot(dir)));
                                    sdl.add(fabs((p6 - p3).dot(dir)));
                                 }
                               delete e;
                            }
                          clist.destroy();
                       }
                   if (j == 1 && rightcolour < 256)
                     vs->displaylist.changecolour(rightcolour);
                   p5 = p3;
                   for (sdl.start() ; ! sdl.atend() ; )
                     {  t1 = sdl.next();
                        if (t1 < 1.0 || t1 >= walllength - 1.0)
                          {  sdl.next();  continue;  //  Discard breaks not actually on the wall
                          }  
                        p6 = p3 + dir * t1;
                        Line line1(*this,p5,p6);
                        line1.addtodisplaylist(vs,1);
                        p5 = p3 + dir * sdl.next();
                     }
                   Line line1(*this,p5,p4);
                   line1.addtodisplaylist(vs,1);
                   sdl.destroy();
                   if (j == 1 && rightcolour < 256)
                     vs->displaylist.changecolour(getcolour());
                }
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(0,_RCT("5"),&clist) && ! clist.empty())
                {  if (middlecolour < 256)
                     vs->displaylist.changecolour(middlecolour);
                   for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  e->addtodisplaylist(vs,1);
                        delete e;
                     }
                   clist.destroy();
                   if (middlecolour < 256)
                     vs->displaylist.changecolour(getcolour());
                }

            if (options.test(1) || options.test(2))
              {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                 Line line1(*this,p3,p4);
                 line1.addtodisplaylist(vs,1);
              }

            if (options.test(1) || options.test(3))
              {  p3 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                 p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                 Line line2(*this,p3,p4);
                 line2.addtodisplaylist(vs,1);
              }


         }
       else
         {  xaxis = (p2 - p1).normalize();
            for (j = 0 ; j < 2 ; j++)
              {  if (j == 0 && ArWallHeight2 < nattributes && attributes[ArWallCavity] != 0.0)
                   za = zaxis * attributes[ArWallHeight2];
                 else 
                   za = zaxis * height;
                 if (j == 0)
                   {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                      xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
                      xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
                      leaf = _RCT("1");
                   }
                 else
                   {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                      xax1 = xaxis*(totalwidth/2.0*tan(angle1));
                      xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
                      leaf = _RCT("4");
                   }
                 list = emptylist;
                 for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
                   if (figure->cutout(1,leaf,&clist))
                     {  if (list.length() > 0)
                          list.add(0);
                        for (clist.start() ; ! clist.atend() ; )
                          list.add(clist.next());
                        clist.destroy();
                     }
                 if (list.length() > 0)
                   list.add(0);
                 line[4] = new Line(*this,p1+yaxis+xax1,p2+yaxis+xax2);
                 line[3] = new Line(*this,p2+yaxis+xax2,p2+yaxis+za+xax2);
                 if (fabs(peakheight) > 1.0E-5)
                   {  p3 = (p2 - p1) * peakdistance / 100.0 + zaxis * peakheight;
                      line[2] = new Line(*this,p2+yaxis+xax2+za,p1+yaxis+p3+za);
                      line[1] = new Line(*this,p1+yaxis+p3+za,p1+xax1+yaxis+za);
                   }
                 else
                   {  line[2] = new Line(*this,p2+yaxis+xax2+za,p1+xax1+yaxis+za);
                      line[1] = 0;
                   }
                 line[0] = new Line(*this,p1+yaxis+xax1+za,p1+xax1+yaxis);
                 for (k = 0 ; k < 5 ; k++)
                   if (line[k] != 0)
                     {  line[k]->addtodisplaylist(vs,1);
                        list.add(line[k]);
                     }
                 Plane plane(*this,p1+yaxis,xaxis,zaxis,list,j == 0 ? lhatchspacing : rhatchspacing,200000.0,0.0,j == 0 ? lhatchangle : rhatchangle,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 if (j == 1 && rightcolour < 256)
                   vs->displaylist.changecolour(rightcolour);
                 plane.addtodisplaylist(vs,1);
                 if (j == 1 && rightcolour < 256)
                   vs->displaylist.changecolour(getcolour());
              }
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(1,_RCT("5"),&clist) && ! clist.empty())
                {  if (middlecolour < 256)
                     vs->displaylist.changecolour(middlecolour);
                   for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  e->addtodisplaylist(vs,1);
                        delete e;
                     }
                   clist.destroy();
                   if (middlecolour < 256)
                     vs->displaylist.changecolour(getcolour());
                }
            if (options.test(1) || options.test(2) || options.test(3))
              {int ystart,yend;
                 yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 ystart = options.test(1) || options.test(2) ? 0 : 1;
                 yend = options.test(1) || options.test(3) ? 1 : 0;
                 for (j = ystart ; j <= yend ; j++)
                   {  if (j == 0)
                        {  p3 = p1 - yaxis - xaxis*(totalwidth/2.0*tan(angle1));
                           p4 = p1 + yaxis + xaxis*(totalwidth/2.0*tan(angle1));
                        }
                      else
                        {  p3 = p2 - yaxis - xaxis*(totalwidth/2.0*tan(-angle2));
                           p4 = p2 + yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
                        }
                      Line *line1 = new Line(*this,p3,p4);
                      line1->addtodisplaylist(vs,1);
                      Line *line2 = new Line(*this,p4,p4+za);
                      line2->addtodisplaylist(vs,1);
                      Line *line3 = new Line(*this,p4+za,p3+za);
                      line3->addtodisplaylist(vs,1);
                      Line *line4 = new Line(*this,p3+za,p3);
                      line4->addtodisplaylist(vs,1);
                      list = emptylist;
                      list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                      Plane plane(*this,p3,yaxis,zaxis,list,200000.0,200000.0,0.0,00.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                      plane.addtodisplaylist(vs,1);
                   }
              }
         }
       size = vs->displaylist.getcount();
    }
  if (! child && ! vs->displaylist.end()) 
        return;
}

double Wall::getarea(int,double)
{ return 0.0;
}


double Wall::gettotalwidth(void) const
{ if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    return width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    return width;
}


void Wall::draw(int drawmode,Transform *t,View3dSurface *vs)
{Transform tview;
 Point xax1,xax2,xaxis,yaxis,za,p3,p4,p5,p6,dir;
 int i,j;
 EntityList list,emptylist,clist;
 BitMask o(32);
 SortedDoubleList sdl,esdl;
 Figure *figure;
 Entity *e;
 Line *line[5];
 RCCHAR *leaf;
 double totalwidth,walllength,t1;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  if (t == 0)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }
  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  walllength = (p2 - p1).length();
  if (vs->getwaxis().z > 0.99999) //  TOP View
    {  if (t != 0)
         {  vs->gettransform(&tview);
            vs->applytransform(*t);
         }
       xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize();
       for (j = 0 ; j < 4 ; j++)
         if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0 || j == 0 || j == 3)   
           {  if (j == 0)
                {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist1.start() ; ! break2dlist1.atend() ; )
                     sdl.add(break2dlist1.next());
                   leaf = _RCT("1");
                }
              else if (j == 1)
                {  p3 = p1 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(angle1));
                   p4 = p2 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist2.start() ; ! break2dlist2.atend() ; )
                     sdl.add(break2dlist2.next());
                   leaf = _RCT("2");
                }
              else if (j == 2)
                {  p3 = p1 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(angle1));
                   p4 = p2 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist3.start() ; ! break2dlist3.atend() ; )
                     sdl.add(break2dlist3.next());
                   leaf = _RCT("3");
                }
              else
                {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist4.start() ; ! break2dlist4.atend() ; )
                     sdl.add(break2dlist4.next());
                   leaf = _RCT("4");
                }

            dir = (p4 - p3).normalize();
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(0,leaf,&clist))
                {  for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  if (e->isa(line_entity))
                          {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                             p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                             sdl.add(fabs((p5 - p3).dot(dir)));
                             sdl.add(fabs((p6 - p3).dot(dir)));
                          }
                        delete e;
                     }
                   clist.destroy();
                }
            p5 = p3;
            for (sdl.start() ; ! sdl.atend() ; )
              {  t1 = sdl.next();
                 if (t1 < 1.0 || t1 >= walllength - 1.0)
                   {  sdl.next();  continue;  //  Discard breaks not actually on the wall
                   } 
                 p6 = p3 + dir * t1;
                 Line line1(*this,p5,p6);
                 if (j == 1 && rightcolour < 256)
                   line1.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
                 line1.draw(drawmode,0,vs);
                 p5 = p3 + dir * sdl.next();
              }
            Line line1(*this,p5,p4);
            if (j == 1 && rightcolour < 256)
              line1.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
            line1.draw(drawmode,0,vs);
            sdl.destroy();
         }
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(0,_RCT("5"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  e->EntityHeader::change(middlecolour < 256 ? middlecolour : getcolour(),getlayer(),getstyle(),getweight());
                   e->draw(drawmode,0,vs);
                   delete e;
                }
              clist.destroy();
           }

       if (options.test(1) || options.test(2))
         {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
            p4 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
            Line line1(*this,p3,p4);
            line1.draw(drawmode,0,vs);
         }

       if (options.test(1) || options.test(3))
         {  p3 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
            p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
            Line line2(*this,p3,p4);
            line2.draw(drawmode,0,vs);
         }

       if (t != 0)
         vs->settransform(tview);
    }
  else
    {  if (t != 0)
         {  vs->gettransform(&tview);
            vs->applytransform(*t);
         }



       xaxis = (p2 - p1).normalize();
       for (i = 0 ; i < 2 ; i++)
         {  if (i == 0 && ArWallHeight2 < nattributes && attributes[ArWallCavity] != 0.0)
              za = zaxis * attributes[ArWallHeight2];
            else 
              za = zaxis * height;
            if (i == 0)
              {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
                 leaf = _RCT("1");
              }
            else
              {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
                 leaf = _RCT("4");
              }
            list = emptylist;
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(1,leaf,&clist))
                {  if (list.length() > 0)
                     list.add(0);
                   for (clist.start() ; ! clist.atend() ; )
                     list.add(clist.next());
                   clist.destroy();
                }
            if (list.length() > 0)
              list.add(0);
            line[4] = new Line(*this,p1+yaxis+xax1,p2+yaxis+xax2);
            line[3] = new Line(*this,p2+yaxis+xax2,p2+yaxis+za+xax2);
            if (fabs(peakheight) > 1.0E-5)
              {  p3 = (p2 - p1) * peakdistance / 100.0 + zaxis * peakheight;
                 line[2] = new Line(*this,p2+yaxis+xax2+za,p1+yaxis+p3+za);
                 line[1] = new Line(*this,p1+yaxis+p3+za,p1+xax1+yaxis+za);
              }
            else
              {  line[2] = new Line(*this,p2+yaxis+xax2+za,p1+xax1+yaxis+za);
                 line[1] = 0;
              }
            line[0] = new Line(*this,p1+yaxis+xax1+za,p1+xax1+yaxis);
            for (j = 0 ; j  < 5 ; j ++)
              if (line[j] != 0)
               {  line[j]->draw(drawmode,0,vs);
                  list.add(line[j]);
               }
            Plane plane(*this,p1+yaxis,xaxis,zaxis,list,j == 0 ? lhatchspacing : rhatchspacing,200000.0,j == 0 ? lhatchangle : rhatchangle,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (j == 1 && rightcolour < 256)
              plane.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
            plane.draw(drawmode,0,vs);
         }
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(1,_RCT("5"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  e->EntityHeader::change(middlecolour < 256 ? middlecolour : getcolour(),getlayer(),getstyle(),getweight());
                   e->draw(drawmode,0,vs);
                   delete e;
                }
              clist.destroy();
           }
       if (options.test(1) || options.test(2) || options.test(3))
         {int ystart,yend;
            yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
            ystart = options.test(1) || options.test(2) ? 0 : 1;
            yend = options.test(1) || options.test(3) ? 1 : 0;
            for (j = ystart ; j <= yend ; j++)
              {  if (j == 0)
                   {  p3 = p1 - yaxis - xaxis*(totalwidth/2.0*tan(angle1));
                      p4 = p1 + yaxis + xaxis*(totalwidth/2.0*tan(angle1));
                   }
                 else
                   {  p3 = p2 - yaxis - xaxis*(totalwidth/2.0*tan(-angle2));
                      p4 = p2 + yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
                   }
                 Line *line1 = new Line(*this,p3,p4);
                 line1->draw(drawmode,0,vs);
                 Line *line2 = new Line(*this,p4,p4+za);
                 line2->draw(drawmode,0,vs);
                 Line *line3 = new Line(*this,p4+za,p3+za);
                 line3->draw(drawmode,0,vs);
                 Line *line4 = new Line(*this,p3+za,p3);
                 line4->draw(drawmode,0,vs);
                 list = emptylist;
                 list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                 Plane plane(*this,p3,yaxis,zaxis,list,200000.0,200000.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 plane.draw(drawmode,0,vs);
              }
         }
       if (t != 0)
         vs->settransform(tview);
    }
}

Plane *Wall::getleaf(RCCHAR *leaf)
{Transform tview;
 Point xax1,xax2,xaxis,yaxis,za,p3,p4,p5,p6,dir;
 EntityList list,emptylist,clist;
 BitMask o(32);
 Figure *figure;
 double totalwidth;

  za = zaxis * height;
  xaxis = (p2 - p1).normalize();
  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  if (*leaf == '1')
    {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
       xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
       xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
    }
  else
    {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
       xax1 = xaxis*(totalwidth/2.0*tan(angle1));
       xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
    }
  Line *line1 = new Line(*this,p1+yaxis+xax1,p2+yaxis+xax2);
  Line *line2 = new Line(*this,p2+yaxis+xax2,p2+yaxis+za+xax2);
  Line *line3 = new Line(*this,p2+yaxis+xax2+za,p1+xax1+yaxis+za);
  Line *line4 = new Line(*this,p1+yaxis+xax1+za,p1+xax1+yaxis);
  list = emptylist;
  for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
    if (figure->cutout(1,leaf,&clist))
      {  if (list.length() > 0)
           list.add(0);
         for (clist.start() ; ! clist.atend() ; )
           list.add(clist.next());
         clist.destroy();
      }
  if (list.length() > 0)
    list.add(0);
  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
  Plane *plane = new Plane(*this,p1+yaxis,xaxis,zaxis,list,400.0,20000.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
  return plane;
}

int Wall::hide(HideImage *image)
{int i,j;
 Point xaxis,xax1,xax2,yaxis,za,p3,p4,p5,p6,dir;
 EntityList list,emptylist;
 BitMask o(32);
 SortedDoubleList sdl,esdl;
 Figure *figure;
 Entity *e;
 Line *line[5];
 EntityList clist;
 RCCHAR *leaf;
 double totalwidth;

  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  if (image->getsurface()->getwaxis().z > 0.99999) //  TOP View
    {  xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize() * gettotalwidth() / 2.0;
       for (j = 0 ; j < 2 ; j++)
         {  if (j == 0)
              {  p3 = p1 + yaxis + xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p2 + yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
                 sdl = esdl;
                 for (break2dlist1.start() ; ! break2dlist1.atend() ; )
                   sdl.add(break2dlist1.next());
                 leaf = _RCT("1");
              }
            else
              {  p3 = p1 - yaxis + xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p2 - yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
                 sdl = esdl;
                 for (break2dlist4.start() ; ! break2dlist4.atend() ; )
                   sdl.add(break2dlist4.next());
                 leaf = _RCT("4");
              }
            dir = (p4 - p3).normalize();
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(0,leaf,&clist))
                {  for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  if (e->isa(line_entity))
                          {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                             p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                             sdl.add(fabs((p5 - p3).dot(dir)));
                             sdl.add(fabs((p6 - p3).dot(dir)));
                          }
                        delete e;
                     }
                   clist.destroy();
                }
            p5 = p3;
            for (sdl.start() ; ! sdl.atend() ; )
              {  p6 = p3 + dir * sdl.next();
                 Line line1(*this,p5,p6);
                 line1.hide(image);
                 p5 = p3 + dir * sdl.next();
              }
            Line line1(*this,p5,p4);
            line1.hide(image);
            sdl.destroy();
         }
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(0,_RCT("5"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                   e->hide(image);
                   delete e;
                }
              clist.destroy();
           }
       if (options.test(1))
         {  yaxis = (p2 - p1).cross(zaxis).normalize() * width / 2.0;
            p3 = p1 - yaxis - xaxis*(width/2.0*tan(angle1));
            p4 = p1 + yaxis + xaxis*(width/2.0*tan(angle1));
            Line line1(*this,p3,p4);
            line1.hide(image);
            p3 = p2 - yaxis - xaxis*(width/2.0*tan(-angle2));
            p4 = p2 + yaxis + xaxis*(width/2.0*tan(-angle2));
            Line line2(*this,p3,p4);
            line2.hide(image);
         }
    }
  else
    {  xaxis = (p2 - p1).normalize();
       for (j = 0 ; j < 2 ; j++)
         {  if (j == 0 && ArWallHeight2 < nattributes && attributes[ArWallCavity] != 0.0)
              za = zaxis * attributes[ArWallHeight2];
            else 
              za = zaxis * height;
            if (j == 0)
              {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
                 leaf = _RCT("1");
              }
            else
              {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
                 xax1 = xaxis*(totalwidth/2.0*tan(angle1));
                 xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
                 leaf = _RCT("4");
              }
            list = emptylist;
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(1,leaf,&clist))
                {  if (list.length() > 0)
                     list.add(0);
                   for (clist.start() ; ! clist.atend() ; )
                     list.add(clist.next());
                   clist.destroy();
                }
            if (list.length() > 0)
              list.add(0);
            line[4] = new Line(*this,p1+yaxis+xax1,p2+yaxis+xax2);
            line[3] = new Line(*this,p2+yaxis+xax2,p2+yaxis+za+xax2);
            if (fabs(peakheight) > 1.0E-5)
              {  p3 = (p2 - p1) * peakdistance / 100.0 + zaxis * peakheight;
                 line[2] = new Line(*this,p2+yaxis+xax2+za,p1+yaxis+p3+za);
                 line[1] = new Line(*this,p1+yaxis+p3+za,p1+xax1+yaxis+za);
              }
            else
              {  line[2] = new Line(*this,p2+yaxis+xax2+za,p1+xax1+yaxis+za);
                 line[1] = 0;
              }
            line[0] = new Line(*this,p1+yaxis+xax1+za,p1+xax1+yaxis);
            for (i = 0 ; i  < 5 ; i++)
              if (line[i] != 0)
               {  line[i]->hide(image);
                  list.add(line[i]);
               }
            Plane plane(*this,p1+yaxis,xaxis,zaxis,list,j == 0 ? lhatchspacing : rhatchspacing,200000.0,j == 0 ? lhatchangle : rhatchangle,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (j == 1 && rightcolour < 256)
              plane.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
            plane.hide(image);
         }
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(1,_RCT("5"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  e->EntityHeader::change(getcolour(),getlayer(),getstyle(),getweight());
                   e->hide(image);
                   delete e;
                }
              clist.destroy();
           }
       if (options.test(1))
         {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
            for (j = 0 ; j < 2 ; j++)
              {  if (j == 0)
                   {  p3 = p1 - yaxis - xaxis*(totalwidth/2.0*tan(angle1));
                      p4 = p1 + yaxis + xaxis*(totalwidth/2.0*tan(angle1));
                   }
                 else
                   {  p3 = p2 - yaxis - xaxis*(totalwidth/2.0*tan(-angle2));
                      p4 = p2 + yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
                   }
                 Line *line1 = new Line(*this,p3,p4);
                 line1->hide(image);
                 Line *line2 = new Line(*this,p4,p4+za);
                 line2->hide(image);
                 Line *line3 = new Line(*this,p4+za,p3+za);
                 line3->hide(image);
                 Line *line4 = new Line(*this,p3+za,p3);
                 line4->hide(image);
                 list = emptylist;
                 list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                 Plane plane(*this,p3,yaxis,zaxis,list,20000.0,20000.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 plane.hide(image);
              }
         }
    }
  return 1;
}

void Wall::nfacets(double,int *nu,int *nv)
{  *nu = *nv = 0;
}

struct WallCallBackInfo
  {PolygonCallBack cb;
   void *data;
  };

static int wallcallback(SurfaceEntity *se,void *polygon,int,void *,void *cbdata)
{WallCallBackInfo *fcbi = (WallCallBackInfo *) cbdata;
  return fcbi->cb(se,(Polygon3d *)polygon,0,0,fcbi->data);
}

int Wall::facet(const BitMask &foptions,double error,int nu,int nv,View3dSurface *vs,PolygonCallBack cb,void *data)
{Point za,xaxis,xax1,xax2,yaxis,ya,p3,p4;
 WallCallBackInfo fcbi;
 int i,j;
 EntityList list,emptylist,clist;
 BitMask o(32);
 Figure *figure;
 RCCHAR *leaf;
 Entity *e;
 Line *line[5];
 double totalwidth;

  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  fcbi.cb = cb;  fcbi.data = data;
  za = zaxis * height;
  xaxis = (p2 - p1).normalize();
  for (i = 0 ; i < 2 ; i++)
    {  if (i == 0 && ArWallHeight2 < nattributes && attributes[ArWallCavity] != 0.0)
         za = zaxis * attributes[ArWallHeight2];
       else 
         za = zaxis * height;
       if (i == 0)
         {  yaxis = -(p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
            xax1 = -xaxis*(totalwidth/2.0*tan(angle1));
            xax2 = -xaxis*(totalwidth/2.0*tan(-angle2));
            leaf = _RCT("1");
         }
       else
         {  yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
            xax1 = xaxis*(totalwidth/2.0*tan(angle1));
            xax2 = xaxis*(totalwidth/2.0*tan(-angle2));
            leaf = _RCT("4");
         }
       list = emptylist;
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(1,leaf,&clist))
           {  if (list.length() > 0)
                list.add(0);
              for (clist.start() ; ! clist.atend() ; )
                list.add(clist.next());
              clist.destroy();
           } 
       if (list.length() > 0)
         list.add(0);
       line[4] = new Line(*this,p1+yaxis+xax1,p2+yaxis+xax2);
       line[3] = new Line(*this,p2+yaxis+xax2,p2+yaxis+za+xax2);
       if (fabs(peakheight) > 1.0E-5)
         {  p3 = (p2 - p1) * peakdistance / 100.0 + zaxis * peakheight;
            line[2] = new Line(*this,p2+yaxis+xax2+za,p1+yaxis+p3+za);
            line[1] = new Line(*this,p1+yaxis+p3+za,p1+xax1+yaxis+za);
         }
       else
         {  line[2] = new Line(*this,p2+yaxis+xax2+za,p1+xax1+yaxis+za);
            line[1] = 0;
         }
       line[0] = new Line(*this,p1+yaxis+xax1+za,p1+xax1+yaxis);
       for (j = 0 ; j  < 5 ; j ++)
         if (line[j] != 0)
           list.add(line[j]);
       Plane plane(*this,p1+yaxis,xaxis,zaxis,list,1000.0,1000.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
       if (i == 1 && ! options.test(1) && rightcolour < 256)
         plane.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
       if (! plane.facet(foptions,error,nu,nv,vs,wallcallback,&fcbi)) return 0;
    }
  for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
    if (figure->cutout(1,_RCT("5"),&clist))
      {  for (clist.start() ; (e = clist.next()) != 0 ; )
           {  if (e->issurface())
                {  e->EntityHeader::change(middlecolour < 256 ? middlecolour : getcolour(),getlayer(),getstyle(),getweight());
                   if (! ((SurfaceEntity *)e)->facet(foptions,error,nu,nv,vs,wallcallback,&fcbi)) return 0;
                }
              delete e;
           }
         clist.destroy();
      }
  if (options.test(1) || options.test(2) || options.test(3))
    {int ystart,yend;
       ya = xaxis.cross(zaxis).normalize();
       yaxis = ya * totalwidth / 2.0;
       ystart = options.test(1) || options.test(2) ? 0 : 1;
                 yend = options.test(1) || options.test(3) ? 1 : 0;
                 for (j = ystart ; j <= yend ; j++)
       for (j = ystart ; j <= yend ; j++)
         {  if (j == 0)
              {  p3 = p1 - yaxis - xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p1 + yaxis + xaxis*(totalwidth/2.0*tan(angle1));
              }
            else
              {  p3 = p2 - yaxis - xaxis*(totalwidth/2.0*tan(-angle2));
                 p4 = p2 + yaxis + xaxis*(totalwidth/2.0*tan(-angle2));
              }
            Line *line1 = new Line(*this,p3,p4);
            Line *line2 = new Line(*this,p4,p4+za);
            Line *line3 = new Line(*this,p4+za,p3+za);
            Line *line4 = new Line(*this,p3+za,p3);
            list = emptylist;
            list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
            Plane plane(*this,p3,ya,zaxis,list,200.0,200.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (! plane.facet(foptions,error,nu,nv,vs,wallcallback,&fcbi)) return 0;
         }
    }
  return ! program->getinterrupt();
}

void Wall::triangulate(double, Transform *)
{
}

int Wall::npoints(void)
{double totalwidth;
 int np,j,end;
 Point p3,p4,p5,p6,xaxis,yaxis,dir;
 SortedDoubleList sdl,esdl;
 Figure *figure;
 EntityList clist;
 Entity *e;

  totalwidth = gettotalwidth();
  xaxis = (p2 - p1).normalize();
  yaxis = (p2 - p1).cross(zaxis).normalize();
  end = 0;

  np = 6 + 4;  //  At least 10 points to start off with
  for (j = 0 ; j < 2 ; j++)
    {  if (j == 0)
         {  for (break2dlist1.start() ; ! break2dlist1.atend() ; )
              {  break2dlist1.next();  np++;
              } 
         }
       else
         {  for (break2dlist4.start() ; ! break2dlist4.atend() ; )
              {  break2dlist4.next();  np++;
              }
         }

       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(0,j == 0 ? _RCT("1") : _RCT("4"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  if (e->isa(line_entity))
                     np += 2;
                   delete e;
                }
              clist.destroy();
           }
    }

  return np;

}

Point Wall::point(int index)
{Point xaxis,yaxis;
 double totalwidth;

  totalwidth = gettotalwidth();

  if (index == 0)
    return p1;
  else if (index == 1)
    return p2;
  else if (index < 6)
    {  xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize() * totalwidth / 2.0;
       if (index == 2)
         return p1+yaxis+xaxis*(totalwidth/2.0*tan(angle1));
       else if (index == 3)
         return p2+yaxis+xaxis*(totalwidth/2.0*tan(-angle2));
       else if (index == 4)
         return p1-yaxis-xaxis*(totalwidth/2.0*tan(angle1));
       else
         return p2-yaxis-xaxis*(totalwidth/2.0*tan(-angle2));
    }
  else
    {double dmin,totalwidth;
     int i,j,end;
     Point p3,p4,p5,p6,xaxis,yaxis,dir;
     SortedDoubleList sdl,esdl;
     RCCHAR *leaf;
     Figure *figure;
     EntityList clist;
     Entity *e;

       totalwidth = gettotalwidth();
       xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize();
       end = 0;

       i = 5;
       for (j = 0 ; j < 2 ; j++)
         {  if (j == 0)
              {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                 sdl = esdl;
                 for (break2dlist1.start() ; ! break2dlist1.atend() ; )
                   sdl.add(break2dlist1.next());
                 leaf = _RCT("1");
              }
            else
              {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                 p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                 sdl = esdl;
                 for (break2dlist4.start() ; ! break2dlist4.atend() ; )
                   sdl.add(break2dlist4.next());
                 leaf = _RCT("4");
              }

            dir = (p4 - p3).normalize();
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(0,leaf,&clist))
                {  for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  if (e->isa(line_entity))
                          {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                             p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                             sdl.add(fabs((p5 - p3).dot(dir)));
                             sdl.add(fabs((p6 - p3).dot(dir)));
                          }
                        delete e;
                     }
                   clist.destroy();
                }

            dmin = 1.0E30;
            i++;
            if (i == index)
              return p3;
            i++;
            if (i == index)
              return p4;
       
            for (sdl.start() ; ! sdl.atend() ; )
              {  i++;
                 p5 = p3 + dir * sdl.next();
                 if (i == index)
                   return p5;
              }

            sdl.destroy();
         }
    }
  return Point(0.0,0.0,0.0);
}

//#ifdef _USING_QTCHAR
//int Wall::process_property(int action,int id,void *data,RCCHAR *text)
//#else
int Wall::process_property(int action,int id,void *data,RCCHAR *text)
//#endif
{BitMask change_mask(32),new_options(32);
 int result,cstate;
 double wt;

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Entity");
            return 1;
        }
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Colour");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());

    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Layer");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Style");
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
    }

    if (id-- == 0)
    {   if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else if (action == PP_GETLABEL)
            strcpy(text,"Weight"); 
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
    }

    if (id-- == 0)
    {   if (action == PP_GETLABEL)
        {   strcpy(text,"Wall");
            return 1;
        }
    }

    if (id-- == 0) 
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Width");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,width,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
                change_mask.set(5,1);
            //change(change_mask,*this,RCCHAR *n,double w,double h,double e,double p,double pd,double ph,int mc,int rc,double lhs,double rhs,double lha,double rha,short nattributes_,double *attributes_,const BitMask &op)
            if (e.evaluate(&x))
                change(change_mask,*this,0,0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,new_options,new_options);
       }   
    } 

    if (id-- == 0) 
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Height");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,height,FormatForEditField); 
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
                change_mask.set(6,1);
            //change(change_mask,*this,RCCHAR *n,double w,double h,double e,double p,double pd,double ph,int mc,int rc,double lhs,double rhs,double lha,double rha,short nattributes_,double *attributes_,const BitMask &op)
            if (e.evaluate(&x))
                change(change_mask,*this,0,0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,new_options,new_options);
        }   
    } 

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}

