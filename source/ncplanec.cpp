
#include "ncwin.h"

void CalculatePlane::initialize(void)
{ A1.null();  A2.null();  A3.null();
  B1.null();  B2.null();  B3.null();
  npoints = 0;
}

void CalculatePlane::add(Point p)
{ if (npoints == 0) origin = p;
  p -= origin;
  A1(0,0) += p.x * p.x;
  A1(0,1) += p.x * p.y;
  A1(0,2) += p.x;
  A1(1,0) += p.y * p.x;
  A1(1,1) += p.y * p.y;
  A1(1,2) += p.y;
  A1(2,0) += p.x;
  A1(2,1) += p.y;
  A1(2,2) += 1.0;
  B1(0)   -= p.z * p.x;
  B1(1)   -= p.z * p.y;
  B1(2)   -= p.z;
  A2(0,0) += p.x * p.x;
  A2(0,1) += p.x * p.z;
  A2(0,2) += p.x;
  A2(1,0) += p.z * p.x;
  A2(1,1) += p.z * p.z;
  A2(1,2) += p.z;
  A2(2,0) += p.x;
  A2(2,1) += p.z;
  A2(2,2) += 1.0;
  B2(0)   -= p.y * p.x;
  B2(1)   -= p.y * p.z;
  B2(2)   -= p.y;
  A3(0,0) += p.y * p.y;
  A3(0,1) += p.y * p.z;
  A3(0,2) += p.y;
  A3(1,0) += p.z * p.y;
  A3(1,1) += p.z * p.z;
  A3(1,2) += p.z;
  A3(2,0) += p.y;
  A3(2,1) += p.z;
  A3(2,2) += 1.0;
  B3(0)   -= p.x * p.y;
  B3(1)   -= p.x * p.z;
  B3(2)   -= p.x;
  npoints++;
}

int CalculatePlane::planar(int axis,Point *org,Point *xa,Point *ya,Point *za,double *d)
{double len;
  if (axis == 0)
    {  if (A1.solve(&X1,&B1,1))
         {  zaxis.x = X1(0);  zaxis.y = X1(1);  zaxis.z = 1.0;  D = X1(2); 
         }
       else
         return 0;
    }
  if (axis == 1)
    {  if (A2.solve(&X2,&B2,1))
         {  zaxis.x = X2(0);  zaxis.y = 1.0;  zaxis.z = X2(1);  D = X2(2); 
         }
       else
         return 0;
    }
  if (axis == 2)
    {  if (A3.solve(&X3,&B3,1))
         {  zaxis.x = 1.0;  zaxis.y = X3(0);  zaxis.z = X3(1);  D = X3(2); 
         }
       else
         return 0;
    } 

  D = -zaxis.dot(origin);

  if (db.workplanes.getcurrent() == NULL)
    {  xaxis.setxyz(1.0,0.0,0.0);  yaxis.setxyz(0.0,1.0,0.0);
    }
  else
    {  xaxis = db.workplanes.getcurrent()->getxaxis();
       yaxis = db.workplanes.getcurrent()->getyaxis();
    }
  if (zaxis.dot(xaxis.cross(yaxis)) < 0.0)
    {  zaxis = -zaxis;  D = -D;
    }
  len = zaxis.length();
  zaxis /= len;  D /= len;
  origin = origin - zaxis * (zaxis.dot(origin) + D);
  xaxis = yaxis.cross(zaxis);
  if (xaxis.length() < db.getatoler())
    {  yaxis.setxyz(0.0,1.0,0.0);
       xaxis = zaxis.cross(yaxis);
       if (xaxis.length() < db.getatoler())
         {  yaxis.setxyz(1.0,0.0,0.0);
            xaxis = zaxis.cross(yaxis);
         }
    }
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();
  yaxis.normalize();
  zaxis.normalize();
  *org = origin;
  *xa = xaxis;
  *ya = yaxis;
  *za = zaxis;
  *d = D;
  return 1;
}

double CalculatePlane::check(Point p)
{  return zaxis.dot(p) + D;
}

int Loops::get_offset_side(Point p,double offsetd)
{Point origin,xaxis,yaxis,zaxis,startp;
 double D,od;
 int reverse,side;
 LinearEntity *e;

    if (nloops() > 1) 
        return 0;  
    if (! planar(&origin,&xaxis,&yaxis,&zaxis,&D)) 
        return 0;
    list.start();
    e = (LinearEntity *)list.next();
    od = offsetd;
    if (e == 0 || ! e->beginoffset(p,&zaxis,&od,(LinearEntity *)list.peek(),&reverse,&startp))
    {   return 0;
    }

    side = 0;
    if (fabs(od) >= db.getptoler())
    {   if (fabs(zaxis.z) > fabs(zaxis.y))
            side = zaxis.z > 0.0;
        else
            side = zaxis.y > 0.0;
    }

    return side;
}

int Loops::offset(int insert,int cornermode,Point p,double offsetd,EntityList *olist)
{Point origin,xaxis,yaxis,zaxis,endp,startp;
 double D,od;
 int c,reverse,rev1,rev2;
 LinearEntity *e,*enext;
 Entity *first,*last;
 EntityList olist1,olist2,list1,list2,*tolist;
 

    if (nloops() > 1) 
        return 0;
    if (! planar(&origin,&xaxis,&yaxis,&zaxis,&D)) 
        return 0;
    list1 = list.copy();
    list1.start();
    if ((e = (LinearEntity *) list1.next()) == NULL)
    {   list1.destroy();  
        return 0;
    }
    od = offsetd;
    if (! e->beginoffset(p,&zaxis,&offsetd,(LinearEntity *)list1.peek(),&reverse,&startp))
    {   list1.destroy();  
        return 0;
    }

    if (isclosed)
    {Point dummy;
       list1.start();
       first = last = (Entity *) list1.peek();
       while (! list1.atend()) last = list1.next();
       list1.add(last);
       list1.addatend(first);
       list1.start();
       if ((e = (LinearEntity *) list1.next()) == NULL)
         {  list1.destroy();  return 0;
         }
       enext = (LinearEntity *) list1.peek();
       dummy = zaxis;
       if (! e->beginoffset(p,&dummy,&offsetd,enext,&rev1,&startp))
         {  list1.destroy();  return 0;
         }
       if (enext == NULL)
         rev2 = 0;
       else
         {  if (rev1) endp = e->start(); else endp = e->end();
            if ((endp - enext->start()).length() < (endp - enext->end()).length())
              rev2 = 0;
            else
              rev2 = 1;
         }
       if (rev2 != reverse) zaxis = -zaxis;
       reverse = rev1;
       if (! e->offsetpoint(zaxis,offsetd,reverse,1,&startp))
         {  list1.destroy();  return 0;
         }
    }
  for (list1.start(),c = 0 ; ((e = (LinearEntity *) list1.next()) != NULL) ; c++)
    {  if (isclosed && (c == 0 || list1.peek() == NULL))
         tolist = & olist2;
       else
         tolist = & olist1;
       if (! e->offset(zaxis,offsetd,cornermode,(LinearEntity *)list1.peek(),&reverse,&startp,tolist))
         {  list1.destroy();  return 0;
         }
    }
  for (olist2.start() ; (e = (LinearEntity *) olist2.next()) != NULL ; )
    delete e;
  olist2.destroy();
  list1.destroy();
  if (insert)
    {  db.saveundo(UD_STARTBLOCK,NULL);
       for (olist1.start() ; (e = (LinearEntity *) olist1.next()) != NULL ; )
         db.geometry.add(e);
       db.saveundo(UD_ENDBLOCK,NULL);
    }
  if (olist != NULL)
    {  olist1.reverse();  *olist = olist1;
    }
  else
    olist1.destroy();
  return 1;
}

int Loops::nloops(void)
{int n;
  for (list.start(),n = 1 ;  ! list.atend() ; )
    if (list.next() == NULL) n++;
  return n;
}

void Loops::trace(View3dSurface *vs,LinearEntity *entity,Point start,Point end,Point uv1,Point uv2,double side)
{LinearEntity *e;
 double angle;
  db.geometry.clearstatus();
  list.add(entity);
  entity->setstatus(1);
  while ((start - end).length() > db.getptoler())
    if ((e = db.geometry.trace(vs,this,end,uv1,uv2,side,&end,&uv1,&uv2,&angle)) != 0)
      {  list.add(e);
         e->setstatus(1);
      }
    else
      {  list.destroy();
         break;
      }
}

void Loops::create(EntityList *slist,int draw)
{LinearEntity *e,*e1;
 Point p,st,en;
 int reverse;
 EntityList tlist,duplist,shortlist;

  //  Remove short entities
  for (slist->start() ; (e = (LinearEntity *) slist->next()) != 0 ; )
    if (((LinearEntity *)e)->length() < db.getptoler() / 100.0)
      shortlist.add(e);
  for (shortlist.start() ; (e = (LinearEntity *) shortlist.next()) != 0 ; )
    {  slist->del(e);  if (draw) e->unhighlight();
    }
  shortlist.destroy();

  tlist = *slist;

  //  Remove duplicates
  for (slist->start() ; (e = (LinearEntity *) slist->next()) != NULL ; )
    {  for (tlist.start() ; (e1 = (LinearEntity *) tlist.next()) != e ; );
       for ( ; (e1 = (LinearEntity *) tlist.next()) != NULL ; )
         if (e != e1 && e->issame(Geometry,*e1) && ! duplist.inlist(e1))
           duplist.add(e1);
    }
  for (duplist.start() ; (e = (LinearEntity *) duplist.next()) != NULL ; )
    {  slist->del(e);  if (draw) e->unhighlight();
    }
  duplist.destroy();

  while (! slist->empty())
    {  if (list.length() > 0) list.add(NULL);
       slist->start();  e = (LinearEntity *) slist->next();
       slist->del(e);  
       if (e == 0)
         continue;
       if (draw) e->unhighlight();  list.add(e);
       start = e->start();  end = e->end();
       reverse = 0;
       while (! slist->empty() && (start-end).length() > db.getttoler())
         {  for (slist->start() ;  (e = (LinearEntity *) slist->next()) != NULL ; )
              {  st = e->start();  en = e->end();
                 if ((end - st).length() < db.getttoler())
                   {  list.add(e);  slist->del(e);  if (draw) e->unhighlight();  end = en;  break;
                   }
                 else if ((end - en).length() < db.getttoler())
                   {  list.add(e);  slist->del(e);  if (draw) e->unhighlight();  end = st;  break;
                   }
              }
            if (e == NULL)
              {  if (closed)
                   {  list.destroy();  return;
                   }
                 else if (reverse)
                   break;
                 else
                   {  list.reverse();
                      p = start;  start = end;  end = p;
                      reverse = 1;
                   }
              }
         }
       if (closed && (start-end).length() > db.getttoler())
         {  list.destroy();  return;
         }
       isclosed = (start - end).length() < db.getttoler();
    }
  p = start;  start = end;  end = p;
}

void Loops::reverse(void)
{Point p;
  list.reverse();
  p = start;  start = end;  end = p;
}

int Loops::linearorplanar(Entity *e)
{Point org,xaxis,yaxis,zaxis;
 double d;
 int ok;
  list.add(e);
  ok = linear(&org,&zaxis) || planar(&org,&xaxis,&yaxis,&zaxis,&d);
  list.del(e);
  return ok;
}

int Loops::planar(Point *origin,Point *xaxis,Point *yaxis,Point *zaxis,double *D)
{CalculatePlane cp;
 Point points[4],org,xa,ya,za;
 double d1,d2,dmin,D1;
 int i,j,defined;
 LinearEntity *e;

  cp.initialize();
  for (list.start() ; ! list.atend() ; )
    if ((e = (LinearEntity *) list.next()) != NULL)
      {  e->segment(4,LE_INCLUDESTART,points);
         for (i = 0 ; i < 4 ; i++) cp.add(points[i]);
      }
  for (j = 0,defined = 0,dmin = -1.0 ; j < 3 ; j++)
    {  if (cp.planar(j,&org,&xa,&ya,&za,&D1))
         {  for (list.start(),d1 = -1.0 ; ! list.atend() ; )
              if ((e = (LinearEntity *) list.next()) != NULL)
                {  e->segment(4,LE_INCLUDEEND,points);
                   for (i = 0 ; i < 4 ; i++)
                     {  d2 = fabs(cp.check(points[i]));
                        if (d1 < 0.0 || d2 > d1)
                          d1 = d2;
                     }
                }
            if (d1 < db.getttoler() && (! defined || d1 < dmin))
              {  defined = 1;  dmin = d1;
                 *origin = org;  *xaxis = xa;  *yaxis = ya;  *zaxis = za;  *D = D1;
              }
         }
    }
  return defined;
}

int Loops::linear(Point *origin,Point *direction)
{Point points[4];
 int i;
 LinearEntity *e;

  for (list.start() ; ! list.atend() ; )
    if ((e = (LinearEntity *) list.next()) != 0)
      {  e->segment(4,LE_INCLUDEEND,points);
         for (i = 1 ; i < 4 ; i++)
           if ((points[i] - points[0]).length() > db.getptoler())
             {  *origin = points[i];
                *direction = (points[i] - points[0]).normalize();
                break;
             }
         if (i == 4)
           return 0;
         break;
      }
  for (list.start() ; ! list.atend() ; )
    if ((e = (LinearEntity *) list.next()) != NULL)
      {  e->segment(4,LE_INCLUDEEND,points);
         for (i = 0 ; i < 4 ; i++)
           if ((*origin + *direction * direction->dot(points[i] - *origin) - points[i]).length() > db.getptoler())
             return 0;
      }
  return 1;
}

