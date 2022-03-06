
#include "ncwin.h"

void Intersect::LineLine(Line *line1,Line *line2)
{Point d1,d2,p1,p2;
 double disc1,disc2,disc3,s,t;
  d1 = line1->getp2() - line1->getp1();
  d2 = line2->getp2() - line2->getp1();
  disc1 = d2.x * d1.y - d1.x * d2.y;
  disc2 = d2.x * d1.z - d1.x * d2.z; 
  disc3 = d2.y * d1.z - d1.y * d2.z;
  n = 1;
  if (fabs(disc1) >= fabs(disc2) && fabs(disc1) >= fabs(disc3) && fabs(disc1) > 1.0E-8)
    {  s = (-d2.y * (line2->getp1().x - line1->getp1().x) + d2.x * (line2->getp1().y - line1->getp1().y)) / disc1;
       t = (-d1.y * (line2->getp1().x - line1->getp1().x) + d1.x * (line2->getp1().y - line1->getp1().y)) / disc1;
       p1 = line1->getp1() + d1 * s;
       p2 = line2->getp1() + d2 * t;
       if (fabs(p1.z - p2.z) > db.getptoler() * 1.0E3) n = 0;
    }
  else if (fabs(disc2) >= fabs(disc1) && fabs(disc2) >= fabs(disc3) && fabs(disc2) > 1.0E-8)
    {  s = (-d2.z * (line2->getp1().x - line1->getp1().x) + d2.x * (line2->getp1().z - line1->getp1().z)) / disc2;
       t = (-d1.z * (line2->getp1().x - line1->getp1().x) + d1.x * (line2->getp1().z - line1->getp1().z)) / disc2;
       p1 = line1->getp1() + d1 * s;
       p2 = line2->getp1() + d2 * t;
       if (fabs(p1.y - p2.y) > db.getptoler() * 1.0E3) n = 0;
    }
  else if (fabs(disc3) > 1.0E-8)
    {  s = (-d2.z * (line2->getp1().y - line1->getp1().y) + d2.y * (line2->getp1().z - line1->getp1().z)) / disc3;
       t = (-d1.z * (line2->getp1().y - line1->getp1().y) + d1.y * (line2->getp1().z - line1->getp1().z)) / disc3;
       p1 = line1->getp1() + d1 * s;
       p2 = line2->getp1() + d2 * t;
       if (fabs(p1.x-p2.x) > db.getptoler() * 1.0E3) n = 0;
    }
  else
    n = 0;
  if (n == 1)
    {  ints[0] = new PointE((p1 + p2) * 0.5);
       if (ints[0] == NULL) n = 0;
    }  
}

void Intersect::LineCircle(Line *l,Circle *c)
{Point p1,p2,p3,zaxis,dir1,dir2;
 double A,B,C,disc;
  n = 0;
  dir1 = l->getp1() - c->getorigin();
  dir2 = l->getp2() - l->getp1();
  A = dir2.dot(dir2);
  if (A < db.getptoler()) return;
  dir2.normalize();
  B = 2.0 * dir1.dot(dir2);
  C = dir1.dot(dir1) - c->getradius() * c->getradius();
  disc = B * B - 4 * C;
  if (fabs(disc) < db.getptoler())
    {  n = 1;
       p1 = l->getp1() + dir2 * (-B / 2.0);
    }  
  else if (disc > 0.0)
    {  n = 2;
       p1 = l->getp1() + dir2 * ((-B + sqrt(disc)) / 2.0);
       p2 = l->getp1() + dir2 * ((-B - sqrt(disc)) / 2.0);
    }
  if (n > 0)
    {  zaxis = c->getxaxis().cross(c->getyaxis());
       if (fabs(zaxis.dot(p1) - zaxis.dot(c->getorigin())) > db.getptoler())
         {  n--;
            if (n == 1) p1 = p2;
         }
       else if (n == 2)
         {  p3 = p1;  p1 = p2;  p2 = p3;
         }
    }
  if (n > 0)
    {  if (fabs(zaxis.dot(p1) - zaxis.dot(c->getorigin())) > db.getptoler())
         {  n--;  if (n == 1) p1 = p2;
         }
    }
  ints[0] = ints[1] = NULL;
  if (n > 0) ints[0] = new PointE(p1);
  if (n > 1) ints[1] = new PointE(p2);
  if (ints[0] == NULL) n = 0; else if (ints[1] == NULL) n = 1; else n = 2;
}


struct ILCallBack
  {int clip;
   Point o,dir;
   double length;
   Entity **ints;
  };

static void intersectline(Point p1,Point p2,void *data)
{ILCallBack *il = (ILCallBack *) data;
 int i;
 double t1,t2,t3;

   for (i = 0 ; i < MAX_INTERSECTIONS ; i++)
     if (il->ints[i] == 0)
       {  if (il->clip)
            {  t1 = il->dir.dot(p1 - il->o);
               t2 = il->dir.dot(p2 - il->o);
               if (t1 > t2)
                 {  t3 = t1;  t1 = t2;  t2 = t3;
                 }
               if (t2 > 0.0 && t1 < il->length)
                 {  if (t1 < 0.0)
                      t1 = 0.0;
                    if (t2 > il->length)  
                      t2 = il->length;
                    p1 = il->o + il->dir * t1;
                    p2 = il->o + il->dir * t2;
                 }
               else
                 break;  //  Don't insert any entity 
            }
          il->ints[i] = new Line(p1,p2);
          break;
       }

}


void Intersect::LinePlane(Line *line,Plane *plane)
{InfinitePlane ip(plane->getxaxis().cross(plane->getyaxis()),plane->getorigin());
 ILCallBack il;
 Point n2;
 int i;

  LineInfinitePlane(line,&ip);

  if (n == 0)
    {//  Is the line lying on the plane?
 
       if ((line->getp1() - plane->nearp(line->getp1())).length() < db.getptoler() &&
           (line->getp2() - plane->nearp(line->getp2())).length() < db.getptoler())
         {  //  Line is on the plane - generate a hatch line.
            Polygon3d polygon3d(plane->getorigin(),plane->getxaxis(),plane->getyaxis(),plane->getlist(),0,0.001);

            n2 = (line->getp2() - line->getp1()).cross(plane->getxaxis().cross(plane->getyaxis()));
            n2.normalize();
            il.clip = 1;
            il.o = line->getp1();
            il.dir = (line->getp2() - line->getp1()).normalize();
            il.length = (line->getp2() - line->getp1()).length();
            il.ints = ints;
            polygon3d.hatchline(1,n2,-n2.dot(line->getp1()),il.o,il.dir,intersectline,&il);

            n = 0;
            for (i = 0 ; i < MAX_INTERSECTIONS ; i++)
              n += ints[i] != 0;

         }
    }

}

void Intersect::CirclePlane(Circle *circle,Plane *plane)
{Point n1,n2,dir,p1;
 double d1,d2,disc1,disc2,disc3;

  n1 = plane->getxaxis().cross(plane->getyaxis()).normalize();
  d1 = -n1.dot(plane->getorigin());
  n2 = circle->getxaxis().cross(circle->getyaxis());
  d2 = -n2.dot(circle->getorigin());

  disc1 = n1.y * n2.z - n1.z * n2.y;
  disc2 = n1.x * n2.z - n1.z * n2.x;
  disc3 = n1.x * n2.y - n1.y * n2.x;

  if (fabs(disc1) < 0.0001 && fabs(disc2) < 0.0001 && fabs(disc3) < 0.001)
    return;

  dir = n1.cross(n2).normalize();

  if (fabs(disc1) >= fabs(disc2) && fabs(disc1) >= fabs(disc3))
    {  p1.x = 0.0;
       p1.y = -( n2.z * d1 - n1.z * d2) / disc1;
       p1.z = -(-n2.y * d1 + n1.y * d2) / disc1;
    }
  else if (fabs(disc2) >= fabs(disc1) && fabs(disc2) >= fabs(disc3))
    {  p1.x = -( n2.z * d1 - n1.z * d2) / disc2;
       p1.y = 0.0;
       p1.z = -(-n2.x * d1 + n1.x * d2) / disc2;
    }
  else
    {  p1.x = -( n2.y * d1 - n1.y * d2) / disc3;
       p1.y = -(-n2.x * d1 + n1.x * d2) / disc3;
       p1.z = 0.0;
    }

  Line ltmp(p1,p1+dir);
  LineCircle(&ltmp,circle);

}


void Intersect::LineEllipse(Line *l,EllipseE *e)
{Point p1,p2,p3,zaxis,dir1,dir2;
 double x1,y1,x2,y2,A,B,C,disc;
  n = 0;
  x1 = e->getxaxis().dot(l->getp1() - e->getorigin());
  y1 = e->getyaxis().dot(l->getp1() - e->getorigin());
  x2 = e->getxaxis().dot(l->getp2() - e->getorigin());
  y2 = e->getyaxis().dot(l->getp2() - e->getorigin());
  A = e->getmajor() * e->getmajor() * (y2 - y1) * (y2 - y1) + e->getminor() * e->getminor() * (x2 - x1) * (x2 - x1);
  B = 2.0 * e->getmajor() * e->getmajor() * (y2 - y1) * y1 + 2.0 * e->getminor() * e->getminor() * (x2 - x1) * x1;
  C = e->getmajor() * e->getmajor() * y1 * y1 + e->getminor() * e->getminor() * x1 * x1 - e->getmajor() * e->getmajor() * e->getminor() * e->getminor();
  disc = B * B - 4 * A * C;
  if (fabs(A) < db.getptoler())
    n = 0;
  else if (fabs(disc) < db.getptoler())
    {  n = 1;
       p1 = l->getp1() + (l->getp2() - l->getp1()) * (-B / 2.0 / A);
    }
  else if (disc > 0.0)
    {  n = 2;
       p1 = l->getp1() + (l->getp2() - l->getp1()) * ((-B + sqrt(disc)) / 2.0 / A);
       p2 = l->getp1() + (l->getp2() - l->getp1()) * ((-B - sqrt(disc)) / 2.0 / A);
    }
  if (n > 0)
    {  zaxis = e->getxaxis().cross(e->getyaxis());
       if (fabs(zaxis.dot(p1) - zaxis.dot(e->getorigin())) > db.getptoler())
         {  n--;
            if (n == 1) p1 = p2;
         }
       else if (n == 2)
         {  p3 = p1;  p1 = p2;  p2 = p3;
         }
    }
  if (n > 0)
    {  if (fabs(zaxis.dot(p1) - zaxis.dot(e->getorigin())) > db.getptoler())
         {  n--;  if (n == 1) p1 = p2;
         }
    }
  ints[0] = ints[1] = NULL;
  if (n > 0) ints[0] = new PointE(p1);
  if (n > 1) ints[1] = new PointE(p2);
  if (ints[0] == NULL) n = 0; else if (ints[1] == NULL) n = 1; else n = 2;
}


extern Point ExtendedCurvePosition(Curve *curve,Point pdir0,Point pdir1,double t);

void Intersect::LineCurve(Line *l,Curve *c)
{int nsegs,i,j;
 double t,toler2,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8;
 Point o,ndir,p1,p2,pmin,pdir0,pdir1;


  o = l->getp1();
  ndir = l->direction(0.0);
  if (ndir.length() < db.getptoler()) return;

  c->newbasis();

  //  Use pdir0 and pdir1 to calculate the position beyond the start of the curve.
  nsegs = 60;
  toler2 = db.getptoler() * db.getptoler();
  dt = 1.0 / double(nsegs);

  pdir0 = (c->getpolygon()[0] - c->getpolygon()[1]).normalize();
  pdir0 *= (c->position(dt) - c->getpolygon()[0]).length() / dt;

  pdir1 = (c->getpolygon()[n-1] - c->getpolygon()[n-2]).normalize();
  pdir1 *= (c->position(1.0-dt) - c->position(1.0)).length() / dt;

  d1 = d2 = 0.0;
  for (i = 0,t = -dt ; i < nsegs + 3 ; i++,t += dt)
    {  p1 = ExtendedCurvePosition(c,pdir0,pdir1,t);
       p2 = o + ndir * (ndir.dot(p1 - o)) - p1;  d3 = p2.dot(p2);
       if (i > 1)
        if (d1 >= d2 && d2 <= d3)
         {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
            d4 = d1;  d5 = d2;  d6 = d3;
            for (j = 0 ; j < 50 ; j++)
              {  p1 = ExtendedCurvePosition(c,pdir0,pdir1,t1 + (t2 - t1) / 2.0);
                 p2 = o + ndir * (ndir.dot(p1 - o)) - p1;  d7 = p2.dot(p2);
                 if (d4 >= d7 && d7 <= d5)
                   {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                      d6 = d5;  d5 = d7;
                   }
                 else
                   {  p1 = ExtendedCurvePosition(c,pdir0,pdir1,t3 - (t3 - t2) / 2.0);
                      p2 = o + ndir * (ndir.dot(p1 - o)) - p1;  d8 = p2.dot(p2);
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
            p1 = ExtendedCurvePosition(c,pdir0,pdir1,(t1+t3) / 2.0);
            p2 = o + ndir * (ndir.dot(p1 - o)) - p1;
            d7 = p2.dot(p2);
            if (d7 < toler2 && n < MAX_INTERSECTIONS && (t1+t3)/2.0 >= -1.0E-5 && (t1+t3)/2.0 < 1.0+1.0E-5)
              {  ints[n] = new PointE(p1);
                 if (ints[n] != NULL) n++;
              }
         }
       d1 = d2;  d2 = d3;
    }
  c->deletebasis();
}

void Intersect::CircleCircle(Circle *c1,Circle *c2)
{Point xaxis,yaxis,zaxis,za1,za2;
 double r1,r2,d1,cosa,a;
  za1 = c1->getxaxis().cross(c1->getyaxis());
  za2 = c2->getxaxis().cross(c2->getyaxis());
  if (fabs(za1.dot(za2)) < cos(0.001) || fabs(za1.dot(c1->getorigin()-c2->getorigin())) > db.getptoler()) return;
  r1 = c1->getradius();  r2 = c2->getradius();
  xaxis = (c2->getorigin() - c1->getorigin());
  d1 = xaxis.length();
  xaxis.normalize();
  yaxis = (za1.cross(xaxis)).normalize();
  if (d1 < db.getptoler() || yaxis.length() < db.getptoler() || r1 < db.getptoler() || r2 < db.getptoler()) return;
  cosa = (r1 * r1 + d1 * d1 - r2 * r2) / 2.0 / r1 / d1;
  if (cosa > 1.0001 || cosa < -1.0001) return;
  if (cosa > 1.0) cosa = 1.0; else if (cosa < -1.0) cosa = -1.0;
  a = acos(cosa);
  ints[0] = ints[1] = NULL;
  ints[0] = new PointE(c1->getorigin() + xaxis * (r1 * cos(a)) + yaxis * (r1 * sin(a)));
  ints[1] = new PointE(c1->getorigin() + xaxis * (r1 * cos(a)) - yaxis * (r1 * sin(a)));
  if (ints[0] == NULL) n = 0; else if (ints[1] == NULL) n = 1; else n = 2;
}

void Intersect::CircleEllipse(Circle *c,EllipseE *e)
{Point dir,za1,za2;
 double x0,y0,f1,f2,f3,x1,y1,x2,y2,x3,y3,cosa,sina,a,da,a1,a2,a3,r2;
 int i,j,nsegs;
  za1 = c->getxaxis().cross(c->getyaxis());
  za2 = e->getxaxis().cross(e->getyaxis());
  if (fabs(za1.dot(za2)) < cos(0.001) || fabs(za1.dot(c->getorigin()-e->getorigin())) > db.getptoler()) return;
  dir = c->getorigin() - e->getorigin();
  r2 = c->getradius() * c->getradius();
  x0 = e->getxaxis().dot(dir);
  y0 = e->getyaxis().dot(dir);
  x1 = 1.0;
  y1 = 0.0;
  x3 = x1 * e->getmajor() - x0;  y3 = y1 * e->getminor() - y0;
  f1 = x3*x3 + y3 * y3 - r2;
  nsegs = e->length() / db.getptoler();//100;
  da = 2.0 * M_PI / nsegs;
  sina = sin(da);  cosa = cos(da);
  for (i = 0,a = 0.0 ; i < nsegs ; i++,a += da)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       x3 = x2 * e->getmajor() - x0;  y3 = y2 * e->getminor() - y0;
       f2 = x3*x3 + y3*y3 - r2;
       if (f1 * f2 <= 0.0)
         {  a1 = a;  a2 = a + da;
            for (j = 0 ; j < 50 ; j++)
              {  a3 = (a1 + a2) / 2.0;
                 x3 = cos(a3) * e->getmajor() - x0;  y3 = sin(a3) * e->getminor() - y0;
                 f3 = x3*x3 + y3*y3 - r2;
                 if (f1 * f3 <= 0.0)
                   {  f2 = f3;  a2 = a3;
                   }
                 else
                   {  f1 = f3;  a1 = a3;
                   }
              }
            if (n < MAX_INTERSECTIONS)
              {  ints[n] = new PointE(e->getorigin() + e->getxaxis() * (x3 + x0) + e->getyaxis() * (y3 + y0));
                 if (ints[n] != NULL) n++;
              }
         }
       x1 = x2;  y1 = y2;  f1 = f2;
    }
}

void Intersect::EllipseEllipse(EllipseE *e1,EllipseE *e2)
{Point za1,za2,p3,p4;
 double f1,f2,f3,x1,y1,x2,y2,x3,y3,cosa,sina,a,da,a1,a2,a3;
 int i,j,nsegs;
  za1 = e1->getxaxis().cross(e1->getyaxis());
  za2 = e2->getxaxis().cross(e2->getyaxis());
  if (fabs(za1.dot(za2)) < cos(0.001) || fabs(za1.dot(e1->getorigin()-e2->getorigin())) > db.getptoler()) return;
  x1 = 1.0;
  y1 = 0.0;
  x3 = x1 * e1->getmajor();  y3 = y1 * e1->getminor();
  p3 = e1->getorigin() + e1->getxaxis() * x3 + e1->getyaxis() * y3;
  p4 = e2->nearp(p3);
  f1 = (p3 - p4).dot(p4 - e2->getorigin());
  nsegs = 100;
  da = 2.0 * M_PI / nsegs;
  sina = sin(da);  cosa = cos(da);
  for (i = 0,a = 0.0 ; i < nsegs ; i++,a += da)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       x3 = x2 * e1->getmajor();  y3 = y2 * e1->getminor();
       p3 = e1->getorigin() + e1->getxaxis() * x3 + e1->getyaxis() * y3;
       p4 = e2->nearp(p3);
       f2 = (p3 - p4).dot(p4 - e2->getorigin());
       if (f1 * f2 <= 0.0)
         {  a1 = a;  a2 = a + da;
            for (j = 0 ; j < 50 ; j++)
              {  a3 = (a1 + a2) / 2.0;
                 x3 = cos(a3) * e1->getmajor();  y3 = sin(a3) * e1->getminor();
                 p3 = e1->getorigin() + e1->getxaxis() * x3 + e1->getyaxis() * y3;
                 p4 = e2->nearp(p3);
                 f3 = (p3 - p4).dot(p4 - e2->getorigin());
                 if (f1 * f3 <= 0.0)
                   {  f2 = f3;  a2 = a3;
                   }
                 else
                   {  f1 = f3;  a1 = a3;
                   }
              }
            if (n < 4)
              {  ints[n] = new PointE(e1->getorigin() + e1->getxaxis() * x3 + e1->getyaxis() * y3);
                 if (ints[n] != NULL) n++;
              }
            else
              break;
         }
       x1 = x2;  y1 = y2;  f1 = f2;
    }
}

void polygon_extents(int n,Point *polygon,double toler,Point *pmin,Point *pmax)
{int i;
  *pmin = *pmax = *polygon;
  for (i = 1,polygon++ ; i < n ; i++,polygon++)
    {  if (polygon->x < pmin->x) pmin->x = polygon->x;
       if (polygon->y < pmin->y) pmin->y = polygon->y;
       if (polygon->z < pmin->z) pmin->z = polygon->z;
       if (polygon->x > pmax->x) pmax->x = polygon->x;
       if (polygon->y > pmax->y) pmax->y = polygon->y;
       if (polygon->z > pmax->z) pmax->z = polygon->z;
    }
  pmin->x -= toler;  pmin->y -= toler;  pmin->z -= toler;
  pmax->x += toler;  pmax->y += toler;  pmax->z += toler;
}

#define check(p) if ((p).x < pmin->x) pmin->x = (p).x;   \
                 if ((p).y < pmin->y) pmin->y = (p).y;   \
                 if ((p).z < pmin->z) pmin->z = (p).z;   \
                 if ((p).x > pmax->x) pmax->x = (p).x;   \
                 if ((p).y > pmax->y) pmax->y = (p).y;   \
                 if ((p).z > pmax->z) pmax->z = (p).z;

void arc_extents(Point origin,Point xaxis,Point yaxis,double toler,double radius,double a1,double a2,Point *pmin,Point *pmax)
{double miny,maxy,minx,maxx,x,y;
 Point p;
  xaxis *= radius;  yaxis *= radius;
  minx = maxx = cos(a1);
  miny = maxy = sin(a1);
  x = cos(a2);  y = sin(a2);
  if (x < minx) minx = x;
  if (x > maxx) maxx = x;
  if (y < miny) miny = y;
  if (y > maxy) maxy = y;
  if (a1 <= M_PI * 0.5 && M_PI * 0.5 <= a2) maxy =  1.0;
  if (a1 <= M_PI       && M_PI       <= a2) minx = -1.0;
  if (a1 <= M_PI * 1.5 && M_PI * 1.5 <= a2) miny = -1.0;
  *pmin = *pmax = origin + xaxis * minx + yaxis * miny;
  p = origin + xaxis * minx + yaxis * maxy;  check(p);
  p = origin + xaxis * maxx + yaxis * miny;  check(p);
  p = origin + xaxis * maxx + yaxis * maxy;  check(p);
  pmin->x -= toler;  pmin->y -= toler;  pmin->z -= toler;
  pmax->x += toler;  pmax->y += toler;  pmax->z += toler;
}

void Intersect::CircleCurve(Circle *circle,Curve *curve)
{double t1[50],t2[50],t3[50],t4[50],knots1[3],knots2[3],*knot1,*weight1,*tbasis1,toler1,toler2,radius;
 Point pmin1,pmax1,pmin2,pmax2,pmin3,pmax3,pmin4,pmax4,*polygon1,p1,origin,xaxis,yaxis;
 int c,code[50],i,j,same,knotpos1[4],knotmult1[3];
  curve->newbasis();
  polygon1 = NULL;  weight1 = NULL;  knot1 = NULL;  tbasis1 = NULL;
  origin = circle->getorigin();
  xaxis = circle->getxaxis();
  yaxis = circle->getyaxis();
  radius = circle->getradius();
  toler1 = db.getptoler() / 1000.0;
  toler2 = db.getptoler() / 100.0;
  t1[0] = 0.0;  t2[0] = 2.0;
  t3[0] = circle->getstarta();  t4[0] = circle->getstarta() + circle->getsweepa() * 2.0;
  code[0] = 8;
  i = 0;
  do {  if (code[i] & 8)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 7;
          }
        else if (code[i] & 4)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = (t3[i]+t4[i])/2.0;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 11;
          }
        else if (code[i] & 2)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 13;
          }
        else if (code[i] & 1)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = (t3[i] + t4[i]) * 0.5;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 14;
          }
        else
          {  i--;  continue;
          }
        curve->insertknots(3,knots1,knotpos1,knotmult1,&polygon1,&weight1,&knot1,&tbasis1);
        polygon_extents(knotpos1[1]-knotpos1[0],polygon1+knotpos1[0],toler1,&pmin1,&pmax1);
        polygon_extents(knotpos1[2]-knotpos1[1],polygon1+knotpos1[1],toler1,&pmin2,&pmax2);
        arc_extents(origin,xaxis,yaxis,toler1,radius,knots2[0],knots2[1],&pmin3,&pmax3);
        arc_extents(origin,xaxis,yaxis,toler1,radius,knots2[1],knots2[2],&pmin4,&pmax4);
        c = ((pmax1.x > pmin3.x && pmin1.x < pmax3.x &&
              pmax1.y > pmin3.y && pmin1.y < pmax3.y &&
              pmax1.z > pmin3.z && pmin1.z < pmax3.z) << 3) | 
            ((pmax1.x > pmin4.x && pmin1.x < pmax4.x &&
              pmax1.y > pmin4.y && pmin1.y < pmax4.y &&
              pmax1.z > pmin4.z && pmin1.z < pmax4.z) << 2) |
            ((pmax2.x > pmin3.x && pmin2.x < pmax3.x &&
              pmax2.y > pmin3.y && pmin2.y < pmax3.y &&
              pmax2.z > pmin3.z && pmin2.z < pmax3.z) << 1) |
             (pmax2.x > pmin4.x && pmin2.x < pmax4.x &&
              pmax2.y > pmin4.y && pmin2.y < pmax4.y &&
              pmax2.z > pmin4.z && pmin2.z < pmax4.z);
        if (c != 0)
          {  if (i == 49 ||
                   pmax1.x - pmin1.x < toler2 && pmax1.y - pmin1.y < toler2 && pmax1.z - pmin1.z < toler2 &&
                   pmax3.x - pmin3.x < toler2 && pmax3.y - pmin3.y < toler2 && pmax3.z - pmin3.z < toler2)
               {  i--;
                  if (n < MAX_INTERSECTIONS)
                    {  p1 = (curve->position(knots1[1]) + circle->position((knots2[1] - circle->getstarta()) / circle->getsweepa())) / 2.0;
                       for (j = 0,same = 0 ; ! same && j < n ; j++)
                         same = (((PointE *)ints[j])->getp() - p1).length() < db.getptoler();
                       if (! same && n < MAX_INTERSECTIONS)
                         {  ints[n] = new PointE(p1);
                            if (ints[n] != NULL) n++;
                         }
                    }
               }
             else
               {  i++;
                  code[i] = c;
                  t1[i] = knots1[0];  t2[i] = knots1[2];
                  t3[i] = knots2[0];  t4[i] = knots2[2];
               }
          }
    } while (i >= 0);
  delete polygon1;  delete weight1;  delete knot1;  delete tbasis1;
  curve->deletebasis();
}

void ellipse_extents(Point origin,Point xaxis,Point yaxis,double toler,double major,double minor,double a1,double a2,Point *pmin,Point *pmax)
{double miny,maxy,minx,maxx,x,y;
 Point p;
  xaxis *= major;  yaxis *= minor;
  minx = maxx = cos(a1);
  miny = maxy = sin(a1);
  x = cos(a2);  y = sin(a2);
  if (x < minx) minx = x;
  if (x > maxx) maxx = x;
  if (y < miny) miny = y;
  if (y > maxy) maxy = y;
  if (a1 <= M_PI * 0.5 && M_PI * 0.5 <= a2) maxy =  1.0;
  if (a1 <= M_PI       && M_PI       <= a2) minx = -1.0;
  if (a1 <= M_PI * 1.5 && M_PI * 1.5 <= a2) miny = -1.0;
  *pmin = *pmax = origin + xaxis * minx + yaxis * miny;
  p = origin + xaxis * minx + yaxis * maxy;  check(p);
  p = origin + xaxis * maxx + yaxis * miny;  check(p);
  p = origin + xaxis * maxx + yaxis * maxy;  check(p);
  pmin->x -= toler;  pmin->y -= toler;  pmin->z -= toler;
  pmax->x += toler;  pmax->y += toler;  pmax->z += toler;
}

void Intersect::EllipseCurve(EllipseE *ellipse,Curve *curve)
{double t1[50],t2[50],t3[50],t4[50],knots1[3],knots2[3],*knot1,*weight1,*tbasis1,toler1,toler2,major,minor;
 Point pmin1,pmax1,pmin2,pmax2,pmin3,pmax3,pmin4,pmax4,*polygon1,p1,origin,xaxis,yaxis;
 int c,code[50],i,j,same,knotpos1[4],knotmult1[3];
  curve->newbasis();
  polygon1 = NULL;  weight1 = NULL;  knot1 = NULL;  tbasis1 = NULL;
  origin = ellipse->getorigin();
  xaxis = ellipse->getxaxis();
  yaxis = ellipse->getyaxis();
  major = ellipse->getmajor();
  minor = ellipse->getminor();
  toler1 = db.getptoler() / 1000.0;
  toler2 = db.getptoler() / 100.0;
  t1[0] = 0.0;  t2[0] = 2.0;
  t3[0] = ellipse->getstarta();  t4[0] = ellipse->getstarta() + ellipse->getsweepa() * 2.0;
  code[0] = 8;
  i = 0;
  do {  if (code[i] & 8)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 7;
          }
        else if (code[i] & 4)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = (t3[i]+t4[i])/2.0;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 11;
          }
        else if (code[i] & 2)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 13;
          }
        else if (code[i] & 1)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = (t3[i] + t4[i]) * 0.5;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 14;
          }
        else
          {  i--;  continue;
          }
        curve->insertknots(3,knots1,knotpos1,knotmult1,&polygon1,&weight1,&knot1,&tbasis1);
        polygon_extents(knotpos1[1]-knotpos1[0],polygon1+knotpos1[0],toler1,&pmin1,&pmax1);
        polygon_extents(knotpos1[2]-knotpos1[1],polygon1+knotpos1[1],toler1,&pmin2,&pmax2);
        ellipse_extents(origin,xaxis,yaxis,toler1,major,minor,knots2[0],knots2[1],&pmin3,&pmax3);
        ellipse_extents(origin,xaxis,yaxis,toler1,major,minor,knots2[1],knots2[2],&pmin4,&pmax4);
        c = ((pmax1.x > pmin3.x && pmin1.x < pmax3.x &&
              pmax1.y > pmin3.y && pmin1.y < pmax3.y &&
              pmax1.z > pmin3.z && pmin1.z < pmax3.z) << 3) | 
            ((pmax1.x > pmin4.x && pmin1.x < pmax4.x &&
              pmax1.y > pmin4.y && pmin1.y < pmax4.y &&
              pmax1.z > pmin4.z && pmin1.z < pmax4.z) << 2) |
            ((pmax2.x > pmin3.x && pmin2.x < pmax3.x &&
              pmax2.y > pmin3.y && pmin2.y < pmax3.y &&
              pmax2.z > pmin3.z && pmin2.z < pmax3.z) << 1) |
             (pmax2.x > pmin4.x && pmin2.x < pmax4.x &&
              pmax2.y > pmin4.y && pmin2.y < pmax4.y &&
              pmax2.z > pmin4.z && pmin2.z < pmax4.z);
        if (c != 0)
          {  if (i == 49 ||
                   pmax1.x - pmin1.x < toler2 && pmax1.y - pmin1.y < toler2 && pmax1.z - pmin1.z < toler2 &&
                   pmax3.x - pmin3.x < toler2 && pmax3.y - pmin3.y < toler2 && pmax3.z - pmin3.z < toler2)
               {  i--;
                  if (n < MAX_INTERSECTIONS)
                    {  p1 = (curve->position(knots1[1]) + ellipse->position((knots2[1] - ellipse->getstarta()) / ellipse->getsweepa())) / 2.0;
                       for (j = 0,same = 0 ; ! same && j < n ; j++)
                         same = (((PointE *)ints[j])->getp() - p1).length() < db.getptoler();
                       if (! same && n < MAX_INTERSECTIONS)
                         {  ints[n] = new PointE(p1);
                            if (ints[n] != NULL) n++;
                         }
                    }
               }
             else
               {  i++;
                  code[i] = c;
                  t1[i] = knots1[0];  t2[i] = knots1[2];
                  t3[i] = knots2[0];  t4[i] = knots2[2];
               }
          }
    } while (i >= 0);
  delete polygon1;  delete weight1;  delete knot1;  delete tbasis1;
  curve->deletebasis();
}

void Drawpolygon(int col,int n, Point *points)
{
    EntityHeader header(col,0,0,0);
    Line * line;

    for(int i=0; i < n-1; i++)
    {
        line = new Line(header,points[i],points[i+1]);
        db.geometry.add(line); line->draw(DM_NORMAL);
    }
}

void drawbox(int col,Point minp,Point maxp)
{
    Point p1(minp);
    Point p2(maxp.x,minp.y,0.0);
    Point p3(maxp);
    Point p4(minp.x,maxp.y,0.0);
    EntityHeader header(col,0,0,0);
    Line * line;

    line = new Line(header,p1,p2);
    db.geometry.add(line); line->draw(DM_NORMAL);
    line = new Line(header,p2,p3);
    db.geometry.add(line); line->draw(DM_NORMAL);
    line = new Line(header,p3,p4);
    db.geometry.add(line); line->draw(DM_NORMAL);
    line = new Line(header,p4,p1);
    db.geometry.add(line); line->draw(DM_NORMAL);
}

void Intersect::CurveCurve(Curve *c1,Curve *c2)
{double t1[50],t2[50],t3[50],t4[50],knots1[3],knots2[3],*knot1,*knot2,*weight1,*weight2,*tbasis1,*tbasis2,toler1,toler2;
 Point pmin1,pmax1,pmin2,pmax2,pmin3,pmax3,pmin4,pmax4,*polygon1,*polygon2,p1;
 int c,code[50],i,j,same,knotpos1[4],knotpos2[4],knotmult1[3],knotmult2[3];
  if (c1 == c2)
    return;

  if (c1->getdegree() <= 1)
    {  for (i = 0 ; i < c1->getn() - 1 ; i++)
         {  Line line(c1->getpolygon()[i],c1->getpolygon()[i+1]);
            LineCurve(&line,c2);
         }
       return;
    }

  c1->newbasis();  c2->newbasis();
  polygon1 = NULL;  weight1 = NULL;  knot1 = NULL;  tbasis1 = NULL;
  polygon2 = NULL;  weight2 = NULL;  knot2 = NULL;  tbasis2 = NULL;
  toler1 = db.getptoler() / 1000.0;
  toler2 = db.getptoler() / 100.0;
  t1[0] = 0.0;  t2[0] = 2.0;
  t3[0] = 0.0;  t4[0] = 2.0;
  code[0] = 8;
  i = 0;
  do {  if (code[i] & 8)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 7;
          }
        else if (code[i] & 4)
          {  knots1[0] = t1[i];  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.25;  knots1[2] = (t1[i] + t2[i]) / 2.0;
             knots2[0] = (t3[i]+t4[i])/2.0;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 11;
          }
        else if (code[i] & 2)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = t3[i];  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.25;  knots2[2] = (t3[i] + t4[i]) / 2.0;
             code[i] &= 13;
          }
        else if (code[i] & 1)
          {  knots1[0] = (t1[i] + t2[i]) * 0.5;  knots1[1] = t1[i] + (t2[i] - t1[i]) * 0.75;  knots1[2] = t2[i];
             knots2[0] = (t3[i] + t4[i]) * 0.5;  knots2[1] = t3[i] + (t4[i] - t3[i]) * 0.75;  knots2[2] = t4[i];
             code[i] &= 14;
          }
        else
          {
            i--;  continue;
          }
        c1->insertknots(3,knots1,knotpos1,knotmult1,&polygon1,&weight1,&knot1,&tbasis1);
        c2->insertknots(3,knots2,knotpos2,knotmult2,&polygon2,&weight2,&knot2,&tbasis2);

        // debug
        /*
        Drawpolygon(0,knotpos1[1]-knotpos1[0],polygon1+knotpos1[0]);
        Drawpolygon(0,knotpos1[2]-knotpos1[1],polygon1+knotpos1[1]);
        Drawpolygon(1,knotpos2[1]-knotpos2[0],polygon2+knotpos2[0]);
        Drawpolygon(1,knotpos2[2]-knotpos2[1],polygon2+knotpos2[1]);
        */
        // debug

        polygon_extents(knotpos1[1]-knotpos1[0],polygon1+knotpos1[0],toler1,&pmin1,&pmax1);
        polygon_extents(knotpos1[2]-knotpos1[1],polygon1+knotpos1[1],toler1,&pmin2,&pmax2);
        polygon_extents(knotpos2[1]-knotpos2[0],polygon2+knotpos2[0],toler1,&pmin3,&pmax3);
        polygon_extents(knotpos2[2]-knotpos2[1],polygon2+knotpos2[1],toler1,&pmin4,&pmax4);
        c = ((pmax1.x > pmin3.x && pmin1.x < pmax3.x &&
              pmax1.y > pmin3.y && pmin1.y < pmax3.y &&
              pmax1.z > pmin3.z && pmin1.z < pmax3.z) << 3) |
            ((pmax1.x > pmin4.x && pmin1.x < pmax4.x &&
              pmax1.y > pmin4.y && pmin1.y < pmax4.y &&
              pmax1.z > pmin4.z && pmin1.z < pmax4.z) << 2) |
            ((pmax2.x > pmin3.x && pmin2.x < pmax3.x &&
              pmax2.y > pmin3.y && pmin2.y < pmax3.y &&
              pmax2.z > pmin3.z && pmin2.z < pmax3.z) << 1) |
             (pmax2.x > pmin4.x && pmin2.x < pmax4.x &&
              pmax2.y > pmin4.y && pmin2.y < pmax4.y &&
              pmax2.z > pmin4.z && pmin2.z < pmax4.z);

        //debug
        /*
        drawbox(0,pmin1,pmax1);
        drawbox(0,pmin2,pmax2);
        drawbox(1,pmin3,pmax3);
        drawbox(1,pmin4,pmax4);
        */
        //debug

        if (c != 0)
          {  if (i == 49 ||
                   pmax1.x - pmin1.x < toler2 && pmax1.y - pmin1.y < toler2 && pmax1.z - pmin1.z < toler2 &&
                   pmax3.x - pmin3.x < toler2 && pmax3.y - pmin3.y < toler2 && pmax3.z - pmin3.z < toler2)
               {  i--;
                  if (n < 4)
                    {  p1 = (c1->position(knots1[1]) + c2->position(knots2[1])) / 2.0;
                       for (j = 0,same = 0 ; ! same && j < n ; j++)
                         same = (((PointE *)ints[j])->getp() - p1).length() < db.getptoler();
                       if (! same && n < 4)
                         {  ints[n] = new PointE(p1);
                            if (ints[n] != NULL) n++;
                         }
                    }
                  else
                    break;
               }
             else
               {  i++;
                  code[i] = c;
                  t1[i] = knots1[0];  t2[i] = knots1[2];
                  t3[i] = knots2[0];  t4[i] = knots2[2];
               }
          }
        else
          i--;  //  Back out from here
    } while (i >= 0);
  delete polygon1;  delete weight1;  delete knot1;  delete tbasis1;
  delete polygon2;  delete weight2;  delete knot2;  delete tbasis2;
  c1->deletebasis();
  c2->deletebasis();
}

void Intersect::LineInfinitePlane(Line *line,InfinitePlane *plane)
{Point dir;
 double t,l,denom;
  dir = line->getp2() - line->getp1();
  if ((l = dir.length()) > db.getptoler())
    {  denom = plane->getnormal().dot(dir);
       if (fabs(denom/l) > db.getatoler())
         {  t = -(line->getp1().dot(plane->getnormal()) + plane->getD()) / denom;
            ints[0] = new PointE(dir * t + line->getp1());
            n = 1;
            if (ints[0] == NULL) n = 0;
         }
    }
}

void Intersect::UVSurfaceInfinitePlane(UVSurfaceEntity *surface,InfinitePlane *plane)
{const int nsegs = 100;
 double u,v,u1,v1,u2,v2,u3,v3,du,dv,d,dmin;
 Point p1,p2,p3,p4,p5;
 GeneralList uvlist,suvlist;
 UV *uv1,*uv2,*uvmin;
 BitMask options(32);
 Point *points;
 int i;

   du = 1.0 / nsegs;
   dv = 1.0 / nsegs;

   for (u = 0.0 ; u <= 1.0+du/2.0 ; u += du)
     for (v = 0.0 ; v <= 1.0+du/2.0 ; v += dv)
       {  p2 = surface->position(u,v);
          if (v > 0.0 && (plane->getnormal().dot(p1) + plane->getD()) * (plane->getnormal().dot(p2) + plane->getD()) <= 0.0)
            {  v1 = v - dv;  v2 = v;
               p3 = p1;  p4 = p2;
               while ((p4 - p3).length() > db.getptoler())
                 {  v3 = (v1 + v2) / 2.0;
                    p5 = surface->position(u,v3);
                    if ((plane->getnormal().dot(p3) + plane->getD()) * (plane->getnormal().dot(p5) + plane->getD()) <= 0.0)
                      {  v2 = v3;  p4 = p5;
                      }
                    else
                      {  v1 = v3;  p3 = p5;
                      }
                 }
               uvlist.add(new UV(u,v3));
            }
          p1 = p2;
       }

   for (v = 0.0 ; v <= 1.0+dv/2.0 ; v += dv)
     for (u = 0.0 ; u <= 1.0+du/2.0 ; u += du)
       {  p2 = surface->position(u,v);
          if (u > 0.0 && (plane->getnormal().dot(p1) + plane->getD()) * (plane->getnormal().dot(p2) + plane->getD()) <= 0.0)
            {  u1 = u - du;  u2 = u;
               p3 = p1;  p4 = p2;
               while ((p4 - p3).length() > db.getptoler())
                 {  u3 = (u1 + u2) / 2.0;
                    p5 = surface->position(u3,v);
                    if ((plane->getnormal().dot(p3) + plane->getD()) * (plane->getnormal().dot(p5) + plane->getD()) <= 0.0)
                      {  u2 = u3;  p4 = p5;
                      }
                    else
                      {  u1 = u3;  p3 = p5;
                      }
                    if (fabs(u1 - u2) < 1.0E-12)
                      break;
                 }
               uvlist.add(new UV(u3,v));
            }
          p1 = p2;
       }

  if (! uvlist.empty())
    {  for (uvlist.start() ; (uv1 = (UV *)uvlist.next()) != 0 ; )
         if (uv1->u < 1.0E-10 || uv1->u > 1.0-1.0E-10 ||
             uv1->v < 1.0E-10 || uv1->v > 1.0-1.0E-10)
           break;
       if (uv1 == 0)
         {  uvlist.start();
            uv1 = (UV *)uvlist.next();
         }
       suvlist.add(uv1);
       uvlist.del(uv1);
    }

  while (! uvlist.empty())
    {  dmin = -1.0;
       for (uvlist.start() ; (uv2 = (UV *)uvlist.next()) != 0 ; )
         if ((d = (uv1->u - uv2->u)*(uv1->u - uv2->u) + (uv1->v - uv2->v)*(uv1->v - uv2->v)) < dmin || dmin < 0.0)
           {  uvmin = uv2;  dmin = d;
           }
       suvlist.add(uv1 = uvmin);
       uvlist.del(uv1);
    }

  points = new Point[suvlist.length()];
  n = 0;
  i = 0;
  while (! suvlist.empty())
    {  uv1 = 0;
       for (suvlist.start(),i = 0 ; (uv2 = (UV *)suvlist.next()) != 0 ; i++)
         {  if (i == 0 || fabs(uv2->u-uv1->u) < du * 1.001 && fabs(uv2->v-uv1->v) < dv * 1.001)
              {  points[i] = surface->position(uv2->u,uv2->v);
                 suvlist.del(uv2);
              }
            else
              {  if (n >= MAX_INTERSECTIONS)
                   break;
                 if (i == 2)
                   ints[n++] = new Line(points[0],points[1]);
                 else if (i == 3)
                   ints[n++] = new Curve(3,2,points,options);
                 else if (i == 4)
                   ints[n++] = new Curve(4,3,points,options);
                 else if (i <= 6)
                   ints[n++] = new Curve(i,3,points,options);
                 else if (i <= 10)
                   ints[n++] = new Curve(6,3,i,points,options);
                 else if (i > 10)
                   ints[n++] = new Curve(10,3,i,points,options);
                 if (ints[n-1]->getdefined() == 0)
                   n--;
                 break;
              }
            delete uv1;
            uv1 = uv2;
         }
       delete uv1;
    }

  if (i == 2)
    ints[n++] = new Line(points[0],points[1]);
  else if (i == 3)
    ints[n++] = new Curve(3,2,points,options);
  else if (i == 4)
    ints[n++] = new Curve(4,3,points,options);
  else if (i <= 6)
    ints[n++] = new Curve(i,3,points,options);
  else if (i <= 10)
    ints[n++] = new Curve(6,3,i,points,options);
  else if (i > 10)
    ints[n++] = new Curve(10,3,i,points,options);
  if (ints[n-1]->getdefined() == 0)
    n--;
}

void Intersect::UVSurfaceLine(UVSurfaceEntity *surface,Line *line,double us,double vs,double dus,double dvs)
{const int nsegs = 30;
 double u,v,du,dv,denom,l,t,D,l1,l2;
 Point p,p1,p2,p3,p4,p5,ldir,lorigin,normal;
 GeneralList uvlist,suvlist;
 BitMask options(32);

  if (us < 0.0) us = 0.0;
  if (vs < 0.0) vs = 0.0;
  if (us + dus > 1.0) us = 1.0 - dus;
  if (vs + dvs > 1.0) vs = 1.0 - dvs;
  du = dus / nsegs;
  dv = dvs / nsegs;

  ldir = line->getp2() - line->getp1();
  if ((l = ldir.length()) < db.getptoler())
    return;
  lorigin = line->getp1();

  for (u = us ; u < us+dus-du/2.0 ; u += du)
    for (v = vs ; v < vs+dvs-dv/2.0 ; v += dv)
      {  p1 = surface->position(u,v);
         p2 = surface->position(u+du,v);
         p3 = surface->position(u+du,v+dv);
         p4 = surface->position(u,v+dv);
         l1 = (p1 - p3).length();
         l2 = (p2 - p4).length();
         normal = (p2 - p1).cross(p3 - p1);
         t = normal.length();
         if (t > 1.0E-100)
           normal /= t;

         D = -normal.dot(p1);
         denom = normal.dot(ldir);
         if (fabs(denom/l) > db.getatoler())
           {  t = -(lorigin.dot(normal) + D) / denom;
              if (n >= MAX_INTERSECTIONS)
                return;
              p = ldir * t + lorigin;
              if ((p - p1).dot(normal.cross(p2 - p1)) > -1.0E-8 * (l1 + l2) &&
                  (p - p2).dot(normal.cross(p3 - p2)) > -1.0E-8 * (l1 + l2) &&
                  (p - p3).dot(normal.cross(p1 - p3)) > -1.0E-8 * (l1 + l2))
                {  if (l1 < db.getptoler() / 100.0 && l2 < db.getptoler() / 100.0 || du < 1.0E-10 || l1 < 1.0E-4)
                     ints[n++] = new PointE(p);
                   else
                     UVSurfaceLine(surface,line,u-du,v-dv,du*3.0,dv*3.0);
                }
           }

         normal = (p3 - p1).cross(p4 - p1).normalize();
         D = - normal.dot(p1);
         denom = normal.dot(ldir);
         if (fabs(denom/l) > db.getatoler())
           {  t = -(lorigin.dot(normal) + D) / denom;
              if (n >= MAX_INTERSECTIONS)
                return;
              p = ldir * t + lorigin;
              if ((p - p1).dot(normal.cross(p3 - p1)) > -1.0E-8 * (l1 + l2) &&
                  (p - p3).dot(normal.cross(p4 - p3)) > -1.0E-8 * (l1 + l2) &&
                  (p - p4).dot(normal.cross(p1 - p4)) > -1.0E-8 * (l1 + l2))
                {  if (l1 < db.getptoler() / 100.0 && l2 < db.getptoler() / 100.0 || du < 1.0E-10 || l1 < 1.0E-4)
                     ints[n++] = new PointE(p);
                   else
                     UVSurfaceLine(surface,line,u-du,v-dv,du*3.0,dv*3.0);
                }
           }

      }

}

void Intersect::PlaneInfinitePlane(Plane *plane,InfinitePlane *iplane)
{double d1,d2,disc1,disc2,disc3;
 Point p1,p2,n1,n2;
 int i;
 Polygon3d polygon3d(plane->getorigin(),plane->getxaxis(),plane->getyaxis(),plane->getlist(),0,0.001);
 ILCallBack il;

  n1 = plane->getxaxis().cross(plane->getyaxis()).normalize();
  d1 = - n1.dot(plane->getorigin());
  n2 = iplane->getnormal();
  d2 = iplane->getD();

  disc1 = n1.y * n2.z - n1.z * n2.y;
  disc2 = n1.x * n2.z - n1.z * n2.x;
  disc3 = n1.x * n2.y - n1.y * n2.x;

  if (fabs(disc1) < 0.0001 && fabs(disc2) < 0.0001 && fabs(disc3) < 0.001)
    return;

  p2 = n1.cross(n2).normalize();

  if (fabs(disc1) >= fabs(disc2) && fabs(disc1) >= fabs(disc3))
    {  p1.x = 0.0;
       p1.y = -( n2.z * d1 - n1.z * d2) / disc1;
       p1.z = -(-n2.y * d1 + n1.y * d2) / disc1;
    }
  else if (fabs(disc2) >= fabs(disc1) && fabs(disc2) >= fabs(disc3))
    {  p1.x = -( n2.z * d1 - n1.z * d2) / disc2;
       p1.y = 0.0;
       p1.z = -(-n2.x * d1 + n1.x * d2) / disc2;
    }
  else
    {  p1.x = -( n2.y * d1 - n1.y * d2) / disc3;
       p1.y = -(-n2.x * d1 + n1.x * d2) / disc3;
       p1.z = 0.0;
    }

  il.clip = 0;
  il.ints = ints;
  polygon3d.hatchline(1,n2,-n2.dot(p1),p1,p2,intersectline,&il);

  n = 0;
  for (i = 0 ; i < MAX_INTERSECTIONS ; i++)
    n += ints[i] != 0;

}

void Intersect::WallInfinitePlane(Wall *wall,InfinitePlane *plane)
{Plane *leaf;
#if (VERSION != FLEXICAD)
  if ((leaf = wall->getleaf(_RCT("1"))) != 0)
    PlaneInfinitePlane(leaf,plane);
  delete leaf;
  if ((leaf = wall->getleaf(_RCT("4"))) != 0)
    PlaneInfinitePlane(leaf,plane);
  delete leaf;
#endif
}

void Intersect::FigureInfinitePlane(Figure *figure,InfinitePlane *plane)
{RCCHAR library[300],name[300];
 Point org,xa,ya,s,r;
 Figure *ifigure;
 BitMask o(32);
  if (figure->cutplane(plane->getnormal(),plane->getD(),library,name,&org,&xa,&ya,&s,&r))
    {  ifigure = new Figure(*figure,library,name,org,xa,ya,s,r,Point(0.0,0.0,0.0),0,0,0,0,o);
       if (ifigure != 0 && ifigure->getdefined())
         {  ints[0] = ifigure;  n = 1;
         }
       else
         {  n = 0;
            delete figure;
         }
    }
}

void Intersect::intersect(Entity *e1,Entity *e2)
{int i;
 InfinitePlane *iplane1,*iplane2;

  iplane1 = iplane2 = 0;
  //  Convert e1 and e2 from Plane to InfinitePlane
  if (e1->isa(plane_entity) && e2->isa(plane_entity))
    e1 = iplane1 = new InfinitePlane(((Plane *)e1)->getxaxis().cross(((Plane *)e1)->getyaxis()),((Plane *)e1)->getorigin());
  else if (e1->isa(plane_entity) && ! e2->isa(infiniteplane_entity) && ! e2->isa(circle_entity) && ! e2->isa(line_entity))
    e1 = iplane1 = new InfinitePlane(((Plane *)e1)->getxaxis().cross(((Plane *)e1)->getyaxis()),((Plane *)e1)->getorigin());
  else if (e2->isa(plane_entity) && ! e1->isa(infiniteplane_entity) && ! e1->isa(circle_entity) && ! e1->isa(line_entity))
    e2 = iplane2 = new InfinitePlane(((Plane *)e2)->getxaxis().cross(((Plane *)e2)->getyaxis()),((Plane *)e2)->getorigin());

  for (i = 0 ; i < MAX_INTERSECTIONS ; i++)
    ints[i] = 0;
  n = 0;
  if (e1->isa(line_entity) && e2->isa(line_entity))
    LineLine((Line *)e1,(Line *)e2);
  else if (e1->isa(circle_entity) && e2->isa(circle_entity))
    CircleCircle((Circle *)e1,(Circle *)e2);
  else if (e1->isa(circle_entity) && e2->isa(plane_entity))
    CirclePlane((Circle *)e1,(Plane *)e2);
  else if (e2->isa(circle_entity) && e1->isa(plane_entity))
    CirclePlane((Circle *)e2,(Plane *)e1);
  else if (e1->isa(ellipse_entity) && e2->isa(ellipse_entity))
    EllipseEllipse((EllipseE *)e1,(EllipseE *)e2);
  else if (e1->isa(line_entity) && e2->isa(circle_entity))
    LineCircle((Line *)e1,(Circle *)e2);
  else if (e1->isa(circle_entity) && e2->isa(line_entity))
    LineCircle((Line *)e2,(Circle *)e1);
  else if (e1->isa(line_entity) && e2->isa(ellipse_entity))
    LineEllipse((Line *)e1,(EllipseE *)e2);
  else if (e1->isa(ellipse_entity) && e2->isa(line_entity))
    LineEllipse((Line *)e2,(EllipseE *)e1);
  else if (e1->isa(line_entity) && e2->isa(curve_entity))
    LineCurve((Line *)e1,(Curve *)e2);
  else if (e1->isa(curve_entity) && e2->isa(line_entity))
    LineCurve((Line *)e2,(Curve *)e1);
  else if (e1->isa(circle_entity) && e2->isa(ellipse_entity))
    CircleEllipse((Circle *)e1,(EllipseE *)e2);
  else if (e1->isa(ellipse_entity) && e2->isa(circle_entity))
    CircleEllipse((Circle *)e2,(EllipseE *)e1);
  else if (e1->isa(circle_entity) && e2->isa(curve_entity))
    CircleCurve((Circle *)e1,(Curve *)e2);
  else if (e1->isa(curve_entity) && e2->isa(circle_entity))
    CircleCurve((Circle *)e2,(Curve *)e1);
  else if (e1->isa(ellipse_entity) && e2->isa(curve_entity))
    EllipseCurve((EllipseE *)e1,(Curve *)e2);
  else if (e1->isa(curve_entity) && e2->isa(ellipse_entity))
    EllipseCurve((EllipseE *)e2,(Curve *)e1);
  else if (e1->isa(curve_entity) && e2->isa(curve_entity))
    CurveCurve((Curve *)e1,(Curve *)e2);
  else if (e1->isa(line_entity) && e2->isa(infiniteplane_entity))
    LineInfinitePlane((Line *)e1,(InfinitePlane *)e2);
  else if (e1->isa(infiniteplane_entity) && e2->isa(line_entity))
    LineInfinitePlane((Line *)e2,(InfinitePlane *)e1);
  else if (e1->isa(plane_entity) && e2->isa(infiniteplane_entity))
    PlaneInfinitePlane((Plane *)e1,(InfinitePlane *)e2);
  else if (e1->isa(infiniteplane_entity) && e2->isa(plane_entity))
    PlaneInfinitePlane((Plane *)e2,(InfinitePlane *)e1);
  else if (e1->isa(wall_entity) && e2->isa(infiniteplane_entity))
    WallInfinitePlane((Wall *)e1,(InfinitePlane *)e2);
  else if (e1->isa(infiniteplane_entity) && e2->isa(wall_entity))
    WallInfinitePlane((Wall *)e2,(InfinitePlane *)e1);
  else if (e1->isa(figure_entity) && e2->isa(infiniteplane_entity))
    FigureInfinitePlane((Figure *)e1,(InfinitePlane *)e2);
  else if (e1->isa(infiniteplane_entity) && e2->isa(figure_entity))
    FigureInfinitePlane((Figure *)e2,(InfinitePlane *)e1);
  else if (e1->isuvsurface() && e2->isa(infiniteplane_entity))
    UVSurfaceInfinitePlane((UVSurfaceEntity *)e1,(InfinitePlane *)e2);
  else if (e1->isa(infiniteplane_entity) && e2->isuvsurface())
    UVSurfaceInfinitePlane((UVSurfaceEntity *)e2,(InfinitePlane *)e1);
  else if (e1->isuvsurface() && e2->isa(line_entity))
    UVSurfaceLine((UVSurfaceEntity *)e1,(Line *)e2,0.0,0.0,1.0,1.0);
  else if (e1->isa(line_entity) && e2->isuvsurface())
    UVSurfaceLine((UVSurfaceEntity *)e2,(Line *)e1,0.0,0.0,1.0,1.0);
  else if (e1->isa(line_entity) && e2->isa(plane_entity))
    LinePlane((Line *)e1,(Plane *)e2);
  else if (e1->isa(plane_entity) && e2->isa(line_entity))
    LinePlane((Line *)e2,(Plane *)e1);
    
  delete iplane1;
  delete iplane2;
}

Intersect::Intersect(Entity *e1,Entity *e2)
{ intersect(e1,e2);
}

Intersect::Intersect(Entity *e1,Entity *e2,Point pnear)
{double d,dmin;
 Point p1,p2;
 int i;
  intersect(e1,e2);
  for (i = 0,dmin = -1.0 ; i < n ; i++)
    {  p1 = ((PointE *)intersection(i))->getp();
       d = (p1 - pnear).length();
       if (i == 0 || d < dmin)
         {  p2 = p1;  dmin = d;
         }
    }
  if (dmin >= 0.0)
    {  delete ints[0],ints[1],ints[2],ints[3];
       n = 1;
       ints[0] = new PointE(p2);
       if (ints[0] == NULL) n = 0;
    }
}

Intersect::~Intersect()
{int i;
  for (i = 0 ; i < MAX_INTERSECTIONS ; i++)
    if (ints[i] != 0 && ints[i]->getposition() == 0)
      delete ints[i];
}
