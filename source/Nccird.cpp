
#include "ncwin.h"

void Circle::circleppp(int arc,Point p1,Point p2,Point p3,double *mida)
{Point normal,p4,p5,d1,d2;
  d1 = p1 - p2;  d2 = p3 - p2;
  normal = d1.cross(d2);
  p4 = (p1 + p2) / 2.0;
  p5 = (p2 + p3) / 2.0;
  d1 = d1.cross(normal);
  d2 = d2.cross(normal);
  Line line1(p4,p4+d1);
  Line line2(p5,p5+d2);
  Intersect i(&line1,&line2);
  if (i.nintersections() == 1)
    {  origin = ((PointE *)i.intersection(0))->getp();
       xaxis = (p1 - origin);  xaxis.normalize();
       yaxis = xaxis.cross(normal);  yaxis.normalize();
       radius = (p1 - origin).length();
       starta = 0.0;
       if (arc)
         {  sweepa = angle(p3);  *mida = sweepa / 2.0;
         } 
    }
  else
    defined = 0;
}

void Circle::circlelll(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Point p4,p5,p6,p7,p8,p9,dir1,dir2,dir3,n1,n2,n3,o;
 double len1=0,len2=0,len3=0,d=0,dmin=0;
 int i;
  Line *l1 = (Line *)e1;  Line *l2 = (Line *) e2;  Line *l3 = (Line *) e3;
  Intersect i1(l1,l2),i2(l2,l3),i3(l1,l3);
  if (i1.nintersections() == 1 && i2.nintersections() == 1 && i3.nintersections() == 1)
    {  p4 = ((PointE *)i1.intersection(0))->getp();
       p5 = ((PointE *)i2.intersection(0))->getp();
       p6 = ((PointE *)i3.intersection(0))->getp();
       dir1 = p5 - p4;  len1 = dir1.length();
       dir2 = p6 - p5;  len2 = dir2.length();
       dir3 = p4 - p6;  len3 = dir3.length();
    }
  else if (i1.nintersections() == 0 && i2.nintersections() == 1 && i3.nintersections() == 1)
    {  if (fabs(l1->direction(0.0).dot(l2->direction(0.0))) < cos(db.getatoler()))
         {  defined = 0;  return;
         }
       p4 = ((PointE *)i2.intersection(0))->getp();
       p5 = ((PointE *)i3.intersection(0))->getp();
       dir1 = p5 - p4;    len1 = dir1.length();
       dir2 = l1->direction(0.0);  len2 = dir2.length();
       dir3 = dir2;  len3 = dir3.length();
    }
  else if (i1.nintersections() == 1 && i2.nintersections() == 0 && i3.nintersections() == 1)
    {  if (fabs(l2->direction(0.0).dot(l3->direction(0.0))) < cos(db.getatoler()))
         {  defined = 0;  return;
         }
       p4 = ((PointE *)i1.intersection(0))->getp();
       p5 = ((PointE *)i3.intersection(0))->getp();
       dir1 = p5 - p4;    len1 = dir1.length();
       dir2 = l2->direction(0.0);  len2 = dir2.length();
       dir3 = dir2;  len3 = dir3.length();
    }
  else if (i1.nintersections() == 1 && i2.nintersections() == 1 && i3.nintersections() == 0)
    {  if (fabs(l1->direction(0.0).dot(l3->direction(0.0))) < cos(db.getatoler()))
         {  defined = 0;  return;
         }
       p4 = ((PointE *)i1.intersection(0))->getp();
       p5 = ((PointE *)i2.intersection(0))->getp();
       dir1 = p5 - p4;    len1 = dir1.length();
       dir2 = l1->direction(0.0);  len2 = dir2.length();
       dir3 = dir2;  len3 = dir3.length();
    }
  if (len1 < db.getptoler() || len2 < db.getptoler() || len3 < db.getptoler())
    {  defined = 0;  return;
    }
  dir1 /= len1; dir2 /= len2;  dir3 /= len3;
  for (i = 0,dmin = -1.0 ; i < 4 ; i++)
    {  n1 = dir1 * (-1.0 + 2.0 * (i % 2)) + dir3;
       n2 = dir2 * (-1.0 + 2.0 * (i / 2)) + dir1;
       Line line1(p4,p4+n1),line2(p5,p5+n2);
       Intersect i4(&line1,&line2);
       if (i4.nintersections() != 1) continue;
       o = ((PointE *)i4.intersection(0))->getp(); 
       p7 = l1->nearp(o);
       p8 = l2->nearp(o);
       p9 = l3->nearp(o);
       d = (p1 - p7).length() + (p2 - p8).length() + (p3 - p9).length();
       if (dmin < 0.0 || d < dmin)
         {  origin = o;
            radius = ((p7 - origin).length() + (p8 - origin).length() + (p9 - origin).length()) / 3.0;
            xaxis = (p7 - origin).normalize();
            yaxis = xaxis.cross(dir1.cross(dir2)).normalize();
            if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) continue;
            dmin = d;
            if (arc)
              {  starta = angle(p7);
                 *mida = angle(p8);
                 sweepa = angle(p9);
              }
         }
    }
  if (dmin < 0.0) defined = 0;
}

void Circle::circlepll(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Line *l1,*l2;
 Point o1,o2,dir1,dir2,dir3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,xa,ya;
 double len1,len2,len3,d1,d2,d3,d4,a,b,c,disc,x,y;
 int i;
   if (e3 == NULL)
     {  l1 = (Line *) e1;  p5 = p1;  l2 = (Line *) e2;  p6 = p2;  p4 = p3;
     }
   else if (e2 == NULL)
     {  l1 = (Line *) e1;  p5 = p1;  p4 = p2;  l2 = (Line *) e3;  p6 = p3;
     }
   else
     {  p4 = p1;  l1 = (Line *) e2;  p5 = p2;  l2 = (Line *) e3;  p6 = p3;
     }
   dir1 = l1->getp2() - l1->getp1();  len1 = dir1.length();
   dir2 = l2->getp2() - l2->getp1();  len2 = dir2.length();
   if (len1 < db.getptoler() || len2 < db.getptoler())
     {  defined = 0;  return;
     }
   dir1 /= len1;  dir2 /= len2;
   Intersect i1(l1,l2);
   if (i1.nintersections() == 0)
     {  if (fabs(dir1.dot(dir2)) < cos(db.getatoler()))
          {  defined = 0;
             return;
          }
        p7 = l1->nearp(p4);  p8 = l2->nearp(p4);
        p9 = (p7 + p8) / 2.0;
        radius = ((p7 - p9).length() + (p8 - p9).length()) / 2.0;
        if (radius < db.getptoler())
          {  defined = 0;  return;
          }
        dir2 = (p9 - p7) / radius;
        y = dir2.dot(p4 - p9);
        x = radius * radius - y * y;
        if (x < 0.0) x = 0.0;
        o1 = p9 + dir1 * sqrt(x);
        o2 = p9 - dir1 * sqrt(x);
     }
   else
     {  p7 = ((PointE *)i1.intersection(0))->getp();
        for (i = 0 ; i < 4 ; i++)
          {  p8 = dir1 * (-1.0 + 2.0 * (i % 2));
             p9 = dir2 * (-1.0 + 2.0 * (i / 2));
             d1 = p8.cross(p8.cross(p9)).dot(p4 - p7);
             d2 = p9.cross(p8.cross(p9)).dot(p4 - p7);
             if (d1 < 0.0 && d2 > 0.0)
               {  dir1 = p8;  dir2 = p9;  break;
               }
          }
        dir3 = dir1 + dir2;
        len3 = dir3.length();
        if (len3 < db.getptoler())
          {  defined = 0;  return;
          }
        dir3 /= len3;
        Line line1(p7,p7+dir3);
        p8 = line1.nearp(p4);
        d1 = (p8 - p4).length();
        d2 = (p8 - p7).length();
        a = dir1.dot(dir3);  a *= a;
        b = -2.0 * d2;
        c = d1 * d1 + d2 * d2;
        disc = b * b - 4.0 * a * c;
        if (a < 1.0E-10 || disc < 0.0)
          {  defined = 0;  return;
          }
        o1 = p7 + dir3 * ((-b + sqrt(disc)) / 2.0 / a);
        o2 = p7 + dir3 * ((-b - sqrt(disc)) / 2.0 / a);
     }
   p9 = l1->nearp(o1);  p10 = l2->nearp(o1);
   p11 = l1->nearp(o2);  p12 = l2->nearp(o2);
   d3 = (p9 - p5).length() + (p10 - p6).length();
   d4 = (p11 - p5).length() + (p12 - p6).length();
   if (d3 > d4)
     {  o1 = o2;  p9 = p11;  p10 = p12;
     }
   xa = (p9 - o1).normalize();
   ya = xa.cross(dir1.cross(dir2)).normalize();
   if (xa.length() < db.getptoler() || ya.length() < db.getptoler())
     {  defined = 0;  return;
     }
   origin = o1;
   radius = ((p9 - origin).length() + (p10 - origin).length()) / 2.0;
   xaxis = xa;
   yaxis = ya;
   if (arc)
     {  if (e1 == NULL)
          {  starta = angle(p4);
             *mida = angle(p9);
             sweepa = angle(p10);
          }
        else if (e2 == NULL)
          {  starta = angle(p9);
             *mida = angle(p4);
             sweepa = angle(p10);
          }
        else
          {  starta = angle(p9);
             *mida = angle(p10);
             sweepa = angle(p4);
          }
     }
}

void Circle::circleppl(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Line *line;
 Point dir1,dir2,dir3,p4,p5,p6,p7,p8,p9,p10,o1,o2;
 double x,r1,r2,d1,d2,d3,a,b,c,disc;
   if (e1 != NULL)
     {  line = (Line *) e1;  p4 = p1;  p5 = p2;  p6 = p3;
     }
   else if (e2 != NULL)
     {  p5 = p1;  line = (Line *) e2;  p4 = p2;  p6 = p3;
     }
   else
     {  p5 = p1;  p6 = p2;  line = (Line *) e3;  p4 = p3;
     }
   p7 = line->nearp(p5);  p8 = line->nearp(p6);
   dir1 = p5 - p7;  dir2 = p6 - p8;  dir3 = p8 - p7;
   d1 = dir1.length();  d2 = dir2.length();  d3 = dir3.length();
   if (d1 < db.getptoler() || d2 < db.getptoler() || d3 < db.getptoler())
     {  defined = 0;  return;
     }
   dir1 /= d1;  dir2 /= d2;  dir3 /= d3;
   if (dir1.dot(dir2) < cos(db.getatoler()))
     {  defined = 0;  return;
     }
   a = 1.0 - d2 / d1;  b = - 2.0 * d3;  c = d3 * d3 + d2 * d2 - d1 * d2;
   disc = b * b - 4.0 * a * c;
   if (disc < 0.0)
     {  defined = 0;  return;
     }
   if (fabs(a) < 1.0E-10)
     {  if (d3 < d1 * 2.0 - db.getptoler())
          {  defined = 0;  return;
          }
        x = -c / b;
        r1 = (x * x + d1 * d1) / 2.0 / d1;
        p9 = p7 + dir3 * x;
        o1 = p9 + dir1 * r1;
     }
   else
     {  x = (-b - sqrt(disc)) / 2.0 / a;
        r1 = (x * x + d1 * d1) / 2.0 / d1;
        p9 = p7 + dir3 * x;
        o1 = p9 + dir1 * r1;
        x = (-b + sqrt(disc)) / 2.0 / a;
        r2 = (x * x + d1 * d1) / 2.0 / d1;
        p10 = p7 + dir3 * x;
        o2  = p10 + dir1 * r2;
        if ((p9 - p4).length() > (p10 - p4).length())
          {  o1 = o2;  p9 = p10;
          }
     }
   origin = o1;
   radius = ((p9 - origin).length() + (p5 - origin).length() + (p6 - origin).length()) / 3.0;
   xaxis = dir1;
   yaxis = dir3;
   if (arc)
     {  if (e1 != NULL)
          {  starta = angle(p9);
             *mida = angle(p5);
             sweepa = angle(p6);
          }
        else if (e2 != NULL)
          {  starta = angle(p5);
             *mida = angle(p9);
             sweepa = angle(p6);
          }
        else
          {  starta = angle(p5);
             *mida = angle(p6);
             sweepa = angle(p9);
          }
     }
}

void Circle::circleppc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Circle *circle;
 Point xa,ya,zaxis,dir1,dir2,dir3,p4,p5,p6,p7,p8,p9,p10,p11,p12,o1,o2;
 double d1,d2,d3,d4,a,b,c,d,disc,r1,r2;
   if (e1 != NULL)
     {  circle = (Circle *) e1;  p4 = p1;  p5 = p2;  p6 = p3;
     }
   else if (e2 != NULL)
     {  p5 = p1;  circle = (Circle *) e2;  p4 = p2;  p6 = p3;
     }
   else
     {  p5 = p1;  p6 = p2;  circle = (Circle *) e3;  p4 = p3;
     }
   zaxis = circle->getxaxis().cross(circle->getyaxis());
   if (fabs(zaxis.dot(p5 - circle->getorigin())) > db.getptoler() ||
       fabs(zaxis.dot(p6 - circle->getorigin())) > db.getptoler())
     {  defined = 0;  return;
     }
   dir1 = p6 - p5;
   d1 = dir1.length();
   if (d1 < db.getptoler())
     {  defined = 0;  return;
     }
   dir1 /= d1;
   d1 /= 2.0;
   p7 = (p6 + p5) / 2.0;
   dir3 = dir1.cross(zaxis).normalize();
   Line line(p7,p7+dir3);
   p8 = line.nearp(circle->getorigin());
   dir2 = circle->getorigin() - p8;
   d2 = dir2.length();
   d3 = (p8 - p7).length();
   if (dir3.dot(p8 - p7) < 0.0)
     dir3 = -dir3;
   d = d2 * d2 - d1 * d1 - circle->getradius() * circle->getradius() + d3 * d3;
   a = 4.0 * circle->getradius() * circle->getradius() - 4.0 * d3 * d3;
   b = 4.0 * d * d3;
   c = 4.0 * d1 * d1 * circle->getradius() * circle->getradius() - d * d;
   disc = b * b - 4.0 * a * c;
   if (fabs(a) < 1.0E-10 || disc < 0.0)
     {  defined = 0;  return;
     }
   o1 = p7 + dir3 * ((-b + sqrt(disc)) / 2.0 / a);
   r1 = ((p5 - o1).length() + (p6 - o1).length()) / 2.0;
   p9 = (o1 - circle->getorigin()).normalize() * circle->getradius() + circle->getorigin();
   o2 = p7 + dir3 * ((-b - sqrt(disc)) / 2.0 / a);
   r2 = ((p5 - o2).length() + (p6 - o2).length()) / 2.0;
   p10 = (o2 - circle->getorigin()).normalize() * circle->getradius() + circle->getorigin();
   d3 = (p9 - p4).length();
   d4 = (p10 - p4).length();
   if (d3 > d4)
     {  o1 = o2;  r1 = r2;  p9 = p10;
     }
   xa = (p9 - o1).normalize();
   ya = xa.cross(zaxis).normalize();
   if (xa.length() < db.getptoler() || ya.length() < db.getptoler())
     {  defined = 0;  return;
     }
   origin = o1;
   radius = r1;
   xaxis = xa;
   yaxis = ya;
   if (arc)
     {  if (e1 != NULL)
          {  starta = angle(p9);
             *mida = angle(p5);
             sweepa = angle(p6);
          }
        else if (e2 != NULL)
          {  starta = angle(p5);
             *mida = angle(p9);
             sweepa = angle(p6);
          }
        else
          {  starta = angle(p5);
             *mida = angle(p6);
             sweepa = angle(p9);
          }
     }
}

void Circle::circleplc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Line *line=0;
 Circle *circle=0;
 Point dir1,dir2,dir3,zaxis,p4,p5,p6,p7,p8,p9,p10,p11,p12,o1,o2;
 double x1,x2,r1,r2,d1,d2,d3,d4,d5,l1,a,b,c,disc;
 int i;
   if (e1 != NULL && e1->isa(line_entity) && e2 != NULL && e2->isa(circle_entity))
     {  line = (Line *) e1;  p5 = p1;  circle = (Circle *) e2; p6 = p2;  p4 = p3;
     }
   else if (e2 != NULL && e2->isa(line_entity) && e3 != NULL && e3->isa(circle_entity))
     {  line = (Line *) e2;  p5 = p2;  circle = (Circle *) e3; p6 = p3;  p4 = p1;
     }
   else if (e3 != NULL && e3->isa(line_entity) && e1 != NULL && e1->isa(circle_entity))
     {  line = (Line *) e3;  p5 = p3;  circle = (Circle *) e1; p6 = p1;  p4 = p2;
     }
   else if (e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(line_entity))
     {  line = (Line *) e2;  p5 = p2;  circle = (Circle *) e1; p6 = p1;  p4 = p3;
     }
   else if (e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(line_entity))
     {  line = (Line *) e3;  p5 = p3;  circle = (Circle *) e2; p6 = p2;  p4 = p1;
     }
   else if (e3 != NULL && e3->isa(circle_entity) && e1 != NULL && e1->isa(line_entity))
     {  line = (Line *) e1;  p5 = p1;  circle = (Circle *) e3; p6 = p3;  p4 = p2;
     }
    if(circle)
        zaxis = circle->getxaxis().cross(circle->getyaxis());
    else
    {
         defined = 0;  return;
    }
   if (zaxis.dot(p4 - circle->getorigin()) > db.getptoler() ||
       zaxis.dot(line->getp1() - circle->getorigin()) > db.getptoler() ||
       zaxis.dot(line->getp2() - circle->getorigin()) > db.getptoler())
      {  defined = 0;  return;
      }
   p7 = line->nearp(p4);  p8 = line->nearp(circle->getorigin());
   d1 = (p7 - p4).length();  d2 = (p8 - circle->getorigin()).length();
   d3 = (p8 - p7).length();
   dir1 = line->getp2() - line->getp1();
   if ((l1 = dir1.length()) < db.getptoler())
     {  defined = 0;  return;
     }
   dir1 /= l1;
   if (dir1.dot(p8 - p7) < 0.0) dir1 = -dir1;
   if (d1 < db.getptoler())
     {  x1 = x2 = 0.0;
        dir2 = dir1.cross(zaxis);
        if (dir2.dot(circle->getorigin() - p8) < 0.0) dir2 = -dir2;
        r1 = (d2 * d2 - circle->getradius() * circle->getradius() + d3 * d3) / 2.0 / (circle->getradius() + d2);
        if (fabs(d2 - circle->getradius()) < db.getptoler())
          {  defined = 0;  return;
          }
        else if (d2 < circle->getradius())
          {  r2 = (d2 * d2 + d3 * d3 - circle->getradius() * circle->getradius()) / 2.0 / (circle->getradius() - d2);
             dir3 = -dir2;
          }
        else
          {  r2 = (d2 * d2 - circle->getradius() * circle->getradius() + d3 * d3) / 2.0 / (d2 - circle->getradius());
             dir3 = dir2;
          }
        if (r1 < 0.0)
          {  r1 = -r1;  dir2 = -dir2;
          }
        if (r2 < 0.0)
          {  r2 = -r2;  dir3 = -dir3;
          }
        o1 = p7 + dir1 * x1 + dir2 * r1;
        p9 = p7 + dir1 * x1;
        p10 = circle->getorigin() + (o1 - circle->getorigin()).normalize() * circle->getradius();
        o2 = p7 + dir1 * x2 + dir3 * r2;
        p11 = p7 + dir1 * x2;
        p12 = circle->getorigin() + (o2 - circle->getorigin()).normalize() * circle->getradius();
        d4 = (p9 - p5).length() + (p10 - p6).length();
        d5 = (p11 - p5).length() + (p12 - p6).length();
        if (d4 > d5)
          {  o1 = o2;  r1 = r2;  p9 = p11;  p10 = p12;
          }
     }
   else
     {  if ((p7 - p4).dot(p8 - circle->getorigin()) < 0.0 && d2 > circle->getradius())
          {  defined = 0;  return;
          }
        dir2 = dir3 = (p4 - p7).normalize();
        if ((p7 - p4).dot(p8 - circle->getorigin()) > 0.0)
          {  a = 2.0 * circle->getradius() - 2.0 * d1 + 2.0 * d2;
             b = 4.0 * d3 * d1;
             c = 2.0 * d1 * d1 * circle->getradius() +
                 2.0 * d1 * circle->getradius() * circle->getradius() -
                 2.0 * d1 * d3 * d3 - 2.0 * d2 * d2 * d1 + 2.0 * d1 * d1 * d2;
          }
        else
          {  a = 2.0 * circle->getradius() - 2.0 * d1 - 2.0 * d2;
             b = 4.0 * d3 * d1;
             c = 2.0 * d1 * d1 * circle->getradius() +
                 2.0 * d1 * circle->getradius() * circle->getradius() -
                 2.0 * d1 * d3 * d3 - 2.0 * d2 * d2 * d1 - 2.0 * d1 * d1 * d2;
          }
        disc = b * b - 4.0 * a * c;
        if (fabs(a) > 1.0E-10 && disc >= 0.0)
          {  x1 = (-b + sqrt(disc)) / 2.0 / a;
             r1 = (x1 * x1 + d1 * d1) / 2.0 / d1;
             x2 = (-b - sqrt(disc)) / 2.0 / a;
             r2 = (x2 * x2 + d1 * d1) / 2.0 / d1;
             o1 = p7 + dir1 * x1 + dir2 * r1;
             p9 = p7 + dir1 * x1;
             p10 = circle->getorigin() + (o1 - circle->getorigin()).normalize() * circle->getradius();
             o2 = p7 + dir1 * x2 + dir3 * r2;
             p11 = p7 + dir1 * x2;
             p12 = circle->getorigin() + (o2 - circle->getorigin()).normalize() * circle->getradius();
             d4 = (p9 - p5).length() + (p10 - p6).length();
             d5 = (p11 - p5).length() + (p12 - p6).length();
             if (d4 > d5)
               {  o1 = o2;  r1 = r2;  p9 = p11;  p10 = p12;
               }
          }
        else
          defined = 0;
        if ((p7 - p4).dot(p8 - circle->getorigin()) > 0.0)
          {  a = 2.0 * circle->getradius() + 2.0 * d1 - 2.0 * d2;
             b = -4.0 * d3 * d1;
             c = 2.0 * d1 * d1 * circle->getradius() -
                 2.0 * d1 * circle->getradius() * circle->getradius() +
                 2.0 * d1 * d3 * d3 + 2.0 * d2 * d2 * d1 - 2.0 * d1 * d1 * d2;
          }
        else
          {  a = 2.0 * circle->getradius() + 2.0 * d1 + 2.0 * d2;
             b = -4.0 * d3 * d1;
             c = 2.0 * d1 * d1 * circle->getradius() -
                 2.0 * d1 * circle->getradius() * circle->getradius() +
                 2.0 * d1 * d3 * d3 + 2.0 * d2 * d2 * d1 + 2.0 * d1 * d1 * d2;
          }
        disc = b * b - 4.0 * a * c;
        if (fabs(a) > 1.0E-10 && disc >= 0.0)
          {  x2 = (-b + sqrt(disc)) / 2.0 / a;
             r2 = (x2 * x2 + d1 * d1) / 2.0 / d1;
             o2 = p7 + dir1 * x2 + dir3 * r2;
             p11 = p7 + dir1 * x2;
             p12 = circle->getorigin() + (o2 - circle->getorigin()).normalize() * circle->getradius();
             if (defined)
               d4 = (p9 - p5).length() + (p10 - p6).length();
             d5 = (p11 - p5).length() + (p12 - p6).length();
             if (! defined || d4 > d5)
               {  o1 = o2;  r1 = r2;  p9 = p11;  p10 = p12;  defined = 1;
               }
             x2 = (-b - sqrt(disc)) / 2.0 / a;
             r2 = (x2 * x2 + d1 * d1) / 2.0 / d1;
             o2 = p7 + dir1 * x2 + dir3 * r2;
             p11 = p7 + dir1 * x2;
             p12 = circle->getorigin() + (o2 - circle->getorigin()).normalize() * circle->getradius();
             if (defined)
               d4 = (p9 - p5).length() + (p10 - p6).length();
             d5 = (p11 - p5).length() + (p12 - p6).length();
             if (! defined || d4 > d5)
               {  o1 = o2;  r1 = r2;  p9 = p11;  p10 = p12;  defined = 1;
               }
          }
        if (! defined) return;
     }
   xaxis = dir1;
   yaxis = dir2;
   origin = o1;
   radius = r1;
   if (arc)
     {  dir1 = (circle->getorigin() - o1).normalize();
        for (i = 0 ; i < 4 ; i++)
          {  p11 = o1 + dir1 * (r1 * (-1.0 + 2.0 * (i % 2)));
             p12 = circle->getorigin() + dir1 * (circle->getradius() * (-1.0 + 2.0 * (i / 2)));
             d1 = (p12 - p11).length();
             if (i == 0 || d1 < d2)
               {  p10 = p11;  d2 = d1;
               }
          }
        if (e1 != NULL && e1->isa(line_entity) && e2 != NULL && e2->isa(circle_entity))
          {  starta = angle(p9);  *mida = angle(p10);  sweepa = angle(p4);
          }
        else if (e2 != NULL && e2->isa(line_entity) && e3 != NULL && e3->isa(circle_entity))
          {  starta = angle(p4);  *mida = angle(p9);  sweepa = angle(p10);
          }
        else if (e3 != NULL && e3->isa(line_entity) && e1 != NULL && e1->isa(circle_entity))
          {  starta = angle(p10);  *mida = angle(p4);  sweepa = angle(p9);
          }
        else if (e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(line_entity))
          {  starta = angle(p10);  *mida = angle(p9);  sweepa = angle(p4);
          }
        else if (e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(line_entity))
          {  starta = angle(p4);  *mida = angle(p10);  sweepa = angle(p9);
          }
        else if (e3 != NULL && e3->isa(circle_entity) && e1 != NULL && e1->isa(line_entity))
          {  starta = angle(p9);  *mida = angle(p4);  sweepa = angle(p10);
          }
     }
}

Circle::Circle(int fillet,Point o,Point zaxis,Point start,Point end)
{
#ifdef USING_WIDGETS
     qgi1=0;
     qgi2=0;
#endif

    origin = o;
  xaxis = (start - o).normalize();
  yaxis = (zaxis.cross(xaxis)).normalize();
  radius = ((start - o).length() + (end - o).length()) / 2.0;
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler())
    {  defined = 0;  return;
    }
  starta = angle(start);
  sweepa = angle(end);
  if (sweepa < starta) sweepa += 2.0 * M_PI;
  sweepa -= starta;
  if (fillet && sweepa > M_PI)
    {  starta = angle(end);
       sweepa = angle(start);
       if (sweepa < starta) sweepa += 2.0 * M_PI;
        sweepa -= starta;
    }
}

Circle::Circle(Point o,Entity *e,Point p)
{
#ifdef USING_WIDGETS
     qgi1=0;
     qgi2=0;
#endif

     if (e->isa(line_entity))
    {Line *l = (Line *)e;
       origin = o;
       xaxis = l->direction(0.0);
       yaxis = (o - l->nearp(o));
       radius = yaxis.length();
       starta = 0.0;  sweepa = 2.0 * M_PI;
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  xaxis.normalize();  yaxis.normalize();
         }
       else
         defined = 0;
    }
  else if (e->isa(circle_entity))
    {Circle *c = (Circle *)e;
     double d,r1,r2;
     Point p1,p2,dir;
      origin = o;
      xaxis = c->xaxis;
      yaxis = c->yaxis;
      starta = 0.0;
      sweepa = 2.0 * M_PI;
      if ((c->xaxis.cross(c->yaxis)).dot(o - c->origin) < db.getptoler())
        {  dir = c->origin - o;
           d = dir.length();
           if (d < db.getptoler())
             radius = c->radius;
           else
             {  r1 = d + c->radius;  r2 = fabs(d - c->radius);
                dir /= d;
                p1 = origin + dir * r1;
                p2 = origin + dir * r2;
                if (fabs(r2) < db.getptoler() || (p1 - p).length() < (p2 - p).length())
                  radius = r1;
                else
                  radius = r2;
             }
        }
      else
        defined = 0;
    }
  else
    defined = 0;
}

void Circle::filletangles(int arc,Point p1,Point p2)
{double a1,a2,a3;
  if (arc)
    {  a1 = angle(p1);  a3 = a2 = angle(p2);
       if (a2 < a1) a2 += 2.0 * M_PI;
       if (a2 - a1 > M_PI)
         {  a2 = a1;  a1 = a3;
            if (a2 < a1) a2 += 2.0 * M_PI;
         }
       starta = a1;
       sweepa = a2 - a1;
    }
  else
    {  starta = 0.0;  sweepa = M_PI * 2.0;
    }
}

double Circle::calculated(Point p,Point dir,double radius,Entity *e,Point ep,Point *normal,Point *org,Point *circum)
{double t;
 Point p1,p2,n;
  for (t = 0.0 ; t <= 1.0 ; t += 0.1)
    {  if (e == NULL)
         p1 = ((ep - p).normalize()).cross(dir);
       else if (e->isa(point_entity))
         p1 = ((((PointE *)e)->getp() - p).normalize()).cross(dir);
       else if (e->islinear())
         p1 = ((((LinearEntity *)e)->position(t) - p).normalize()).cross(dir);
       else
         return 1000.0;
       if (p1.length() > db.getatoler())
         {  n = dir.cross(p1).normalize();
            if (n.dot(*normal) < 0.0) *normal = -n; else *normal = n;
            break;
         }
       if (e == NULL || e->isa(point_entity)) return 1000.0;
    }
  if (t > 1.0) return 1000.0;  // Cannot calculate tangent circle
  *org = p + (*normal) * radius;
  if (e == NULL)
    *circum = ep;
  else
    *circum = e->nearp(*org);
  return fabs((*org - *circum).length() - fabs(radius));
}

void Circle::circle(int arc,double r,Curve *curve,Point ep1,Entity *e,Point ep2)
{int nsegs,i,j,s;
 double t,t1,t2,t3,dt,d1,d2,d3,d4,d5,d6,d7,d8,dmin,*nknot,*tbasis,toler;
 Point p3,p4,p5,pmin,pos,dir,n,o,c,tangent,org,circum1,circum2;
  curve->newbasis();
  toler = db.getptoler();
  defined = 0;  dmin = -1.0;
  nknot = NULL;  tbasis = NULL;
  nsegs = 50;
  n.setxyz(1.0,1.0,1.0);
  for (s = -1 ; s <= 1 ; s += 2)
    {  t = 0.0;  dt = 1.0 / double(nsegs);
       if (! curve->posdir(t,&nknot,&tbasis,&p3,&dir))
         {  defined = 0;  return;
         }
       d1 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
       if (d1 < toler)
         {  if (! defined || (p3 - ep1).length() + (c - ep2).length() < dmin)
              {  dmin = (p3 - ep1).length() + (c - ep2).length();
                 tangent = dir;  org = o;  circum1 = p3;  circum2 = c;
                 defined = 1;
              }
         }
       if (! curve->posdir(dt,&nknot,&tbasis,&p3,&dir))
         {  defined = 0;  return;
         }
       d2 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
       if (d2 < toler)
         {  if (! defined || (p3 - ep1).length() + (c - ep2).length() < dmin)
              {  dmin = (p3 - ep1).length() + (c - ep2).length();
                 tangent = dir;  org = o;  circum1 = p3;  circum2 = c;
                 defined = 1;
              }
         }
       for (i = 0,t = 2.0 * dt ; i < nsegs - 1 ; i++,t += dt)
         {  if (! curve->posdir(t,&nknot,&tbasis,&p3,&dir))
              {  defined = 0;  return;
              }
            d3 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
            if (d3 < toler)
              {  if (! defined || (p3 - ep1).length() + (c - ep2).length() < dmin)
                   {  dmin = (p3 - ep1).length() + (c - ep2).length();
                      tangent = dir;  org = o;  circum1 = p3;  circum2 = c;
                      defined = 1;
                   }
              }
            if (d1 >= d2 && d2 <= d3)
              {  t1 = t - 2.0 * dt;  t2 = t - dt;  t3 = t;
                 d4 = d1;  d5 = d2;  d6 = d3;
                 for (j = 0 ; j < 50 ; j++)
                   {  if (! curve->posdir(t1 + (t2 - t1) / 2.0,&nknot,&tbasis,&p3,&dir))
                        {  defined = 0;  return;
                        }
                      d7 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
                      if (d4 >= d7 && d7 <= d5)
                        {  t3 = t2;  t2 = t1 + (t2 - t1) / 2.0;
                           d6 = d5;  d5 = d7;
                        }
                      else
                        {  if (! curve->posdir(t3 - (t3 - t2) / 2.0,&nknot,&tbasis,&p3,&dir))
                             {  defined = 0;  return;
                             }
                           d8 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
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
                 if (! curve->posdir((t1 + t3) / 2.0,&nknot,&tbasis,&p3,&dir))
                   {  defined = 0;  return;
                   }
                 d7 = calculated(p3,dir,r * s,e,ep2,&n,&o,&c);
                 if (d7 < toler)
                   {  if (! defined || (p3 - ep1).length() + (c - ep2).length() < dmin)
                        {  dmin = (p3 - ep1).length() + (c - ep2).length();
                           tangent = dir;  org = o;  circum1 = p3;  circum2 = c;
                           defined = 1;
                        }
                   }
              }
            d1 = d2;  d2 = d3;
         }
    }
  curve->deletebasis();
  delete nknot;  delete tbasis;
  if (defined)
    {  origin = org;
       xaxis = (circum1 - org).normalize();
       yaxis = tangent.normalize();
       radius = r;
       filletangles(arc,circum1,circum2);
    }
}

Circle::Circle(int arc,double r,Entity *e,Point p1,Point p2)
{
#ifdef USING_WIDGETS
     qgi1=0;
     qgi2=0;
#endif

    if (e->isa(line_entity))
    {Point p3,p4,p5;
     double d,len;
     Line *l = (Line *) e;
       p3 = l->nearp(p2);
       xaxis = l->direction(0.0);
       yaxis = p2  - p3;
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  xaxis.normalize();  yaxis.normalize();
            d = (p3 - p2).length();
            len = r * r - (d - r) * (d - r);
            if (len > 0.0)
              {  len = sqrt(len);
                 p4 = p3 + xaxis * len;  p5 = p3 - xaxis * len;
                 if ((p4 - p1).length() > (p5 - p1).length())
                   p4 = p5;
                 origin = p4 + yaxis * r;
                 radius = r;
                 filletangles(arc,p2,p4); 
              }
            else
              defined = 0;                  
         }
       else
         defined = 0;
    }
  else if (e->isa(circle_entity))
    {Circle *c = (Circle *) e;
     double a1,a2,cosa,d;
     Point p3,p4;
       d = (p2 - c->origin).length();
       if (d < c->radius)
         {  a1 = c->angle(p2);
            if (fabs(c->radius-r) > db.getptoler() && fabs(d) > db.getptoler())
              {  cosa = -(r*r - d * d - (c->radius-r)*(c->radius-r)) / 2.0 / (c->radius - r) / d;
                 if (cosa > -1.0 && cosa < 1.0)
                   {  a2 = acos(cosa);
                      p3 = c->origin + c->xaxis * (cos(a1+a2) * c->radius) + c->yaxis * (sin(a1+a2) * c->radius);
                      p4 = c->origin + c->xaxis * (cos(a1-a2) * c->radius) + c->yaxis * (sin(a1-a2) * c->radius);
                      if ((p3-p1).length() > (p4-p1).length())
                        {  p3 = p4;
                           origin = c->origin + c->xaxis * (cos(a1-a2) * (c->radius-r)) + c->yaxis * (sin(a1-a2) * (c->radius-r));
                        }
                      else
                        origin = c->origin + c->xaxis * (cos(a1+a2) * (c->radius-r)) + c->yaxis * (sin(a1+a2) * (c->radius-r));
                      xaxis = c->xaxis;  yaxis = c->yaxis;
                      radius = r;
                      filletangles(arc,p3,p2);
                   }
                 else
                   defined = 0;
              }
            else
              defined = 0;
         }
       else
         {  a1 = c->angle(p2);
            if (fabs(c->radius+r) > db.getptoler() && fabs(d) > db.getptoler())
              {  cosa = -(r*r - d * d - (c->radius+r)*(c->radius+r)) / 2.0 / (c->radius + r) / d;
                 if (cosa > -1.0 && cosa < 1.0)
                   {  a2 = acos(cosa);
                      p3 = c->origin + c->xaxis * (cos(a1+a2) * c->radius) + c->yaxis * (sin(a1+a2) * c->radius);
                      p4 = c->origin + c->xaxis * (cos(a1-a2) * c->radius) + c->yaxis * (sin(a1-a2) * c->radius);
                      if ((p3-p1).length() > (p4-p1).length())
                        {  p3 = p4;
                           origin = c->origin + c->xaxis * (cos(a1-a2) * (c->radius+r)) + c->yaxis * (sin(a1-a2) * (c->radius+r));
                        }
                      else
                        origin = c->origin + c->xaxis * (cos(a1+a2) * (c->radius+r)) + c->yaxis * (sin(a1+a2) * (c->radius+r));
                      xaxis = c->xaxis;  yaxis = c->yaxis;
                      radius = r;
                      if (r > c->radius)
                        {  cosa = -(r*r - d*d - (r-c->radius)*(r-c->radius)) / 2.0 / (r-c->radius) / d;
                           if (cosa > -1.0 && cosa < 1.0)
                             {  a2 = acos(cosa);
                                p4 = c->origin + c->xaxis * (cos(a1+a2+M_PI) * c->radius) + c->yaxis * (sin(a1+a2+M_PI) * c->radius);
                                if ((p3-p1).length() > (p4-p1).length())
                                  {  p3 = p4;
                                     origin = c->origin + c->xaxis * (cos(a1+a2) * (r-c->radius)) + c->yaxis * (sin(a1+a2) * (r-c->radius));
                                  }
                                p4 = c->origin + c->xaxis * (cos(a1-a2+M_PI) * c->radius) + c->yaxis * (sin(a1-a2+M_PI) * c->radius);
                                if ((p3-p1).length() > (p4-p1).length())
                                  {  p3 = p4;
                                     origin = c->origin + c->xaxis * (cos(a1-a2) * (r-c->radius)) + c->yaxis * (sin(a1-a2) * (r-c->radius));
                                  }
                             }
                        }
                      filletangles(arc,p3,p2);
                   }
                 else
                   defined = 0;
              }
            else
              defined = 0;
         }
    }
  else if (e->isa(curve_entity))
    circle(arc,r,(Curve *)e,p1,NULL,p2);
  else
    defined = 0;
}

int Circle::savegeometry(CadFile *outfile)
{ if (! outfile->write(&radius)) return 0;
  if (! outfile->write(&starta)) return 0;
  if (! outfile->write(&sweepa)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  return 1;
}

int Circle::loadgeometry(CadFile *infile)
{ if (cadwindow != 0)
    cadwindow->invalidatedisplaylist(this);
  if (! infile->read(&radius)) return 0;
  if (! infile->read(&starta)) return 0;
  if (! infile->read(&sweepa)) return 0;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  return 1;
}

class CircleVerifyButtonControl : public ButtonDialogControl
  {public:
     CircleVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int CircleVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,halocolour,layer,style,iweight;
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

/**
 * @brief Circle::verify
 *
 * Entry point for the Circle Verify command
 *
 * KMJ : Working in Qt version
 *
 */
void Circle::verify(void)
{Dialog dialog("CircleVerify_Dialog");
 int c,l,s;
 double length,sta,swa,r,w,d;
 Workplane *wp;
 Point xa,ya,o,zero(0.0,0.0,0.0);
 RCCHAR sorigin[300],sxaxis[300],syaxis[300];
  if ((wp = db.workplanes.getcurrent()) != NULL)
    {  o = wp->modeltoworkplane(origin);
       xa = wp->modeltoworkplane(xaxis) - wp->modeltoworkplane(zero);
       ya = wp->modeltoworkplane(yaxis) - wp->modeltoworkplane(zero);
    }
  else
    {  o = origin;
       xa = xaxis;
       ya = yaxis;
    }
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new CircleVerifyButtonControl(1000));
  dialog.add(new CircleVerifyButtonControl(1001));
  dialog.add(new CircleVerifyButtonControl(1002));
  dialog.add(new CircleVerifyButtonControl(1003));

  dialog.add(new StringDialogControl(104,o.string(sorigin),128));
  dialog.add(new StringDialogControl(105,xa.string(sxaxis),128));
  dialog.add(new StringDialogControl(106,ya.string(syaxis),128));
  r = radius;  dialog.add(new RealDialogControl(107,&r,1.0,0.001,1000000.0,ScaleByLength));
  d = radius * 2.0;  dialog.add(new RealDialogControl(111,&d,1.0,0.001,1000000.0,ScaleByLength));
  sta = starta * 180.0 / M_PI;  dialog.add(new RealDialogControl(108,&sta));
  swa = sweepa * 180.0 / M_PI;  dialog.add(new RealDialogControl(109,&swa));
  length = sweepa * radius;   dialog.add(new RealDialogControl(110,&length,ScaleByLength));
  //dialog.add(new ButtonDialogControl(112));
  dialog.setposition(DPOK);
  if (dialog.process() == TRUE)
  //if (dialog.process() == 112)
  {
       radius = r;
       radius = d / 2.0;
       setcolour(c);
       setstyle(s);
       setlayer(l);
       setweight(db.lineweights.mmtoweight(w));

        draw(DM_ERASE);
        cadwindow->invalidatedisplaylist(this);
        db.saveundo(UD_MOVED,this);
        draw(DM_NORMAL);
        if (getposition() != 0 && db.MovedCallback != 0)
            db.MovedCallback(this);
  }
}

void Circle::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{int    n1,i;
 double x1,y1,x2,y2,da,sina,cosa;
 Point p1;
  if (vs == NULL)
    n1 = 20;
  else
    n1 = nsegments((vs->getumax()-vs->getumin()) / vs->getwidth() * db.getdtoler());
  da = sweepa / n1;
  sina = sin(da);  cosa = cos(da);
  x1 = radius * cos(starta);
  y1 = radius * sin(starta);
  p1 = origin + xaxis * x1 + yaxis * y1;
  if (vs == NULL)
    *pmin = *pmax = p1;
  else
    *pmin = *pmax = vs->modeltouv(p1);
  for (i = 0 ; i < n1 ; i++)
    {  x2 = x1 * cosa - y1 * sina;
       y2 = x1 * sina + y1 * cosa;
       p1 = origin + xaxis * x2 + yaxis * y2;
       if (vs != NULL)
         p1 = vs->modeltouv(p1);
       if (p1.x < pmin->x) pmin->x = p1.x;
       if (p1.y < pmin->y) pmin->y = p1.y;
       if (p1.z < pmin->z) pmin->z = p1.z;
       if (p1.x > pmax->x) pmax->x = p1.x;
       if (p1.y > pmax->y) pmax->y = p1.y;
       if (p1.z > pmax->z) pmax->z = p1.z;
       x1 = x2;  y1 = y2;
    }
}

int Circle::issame(int geometry,const Entity &entity) const
{Point zaxis1,zaxis2;
 Circle *circle;
  if (! entity.isa(circle_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         circle = (Circle *)&entity;
         zaxis1 = circle->xaxis.cross(circle->yaxis).normalize();
         zaxis2 = xaxis.cross(yaxis).normalize();
         if (fabs(zaxis1.dot(zaxis2)) < 0.999999) return 0;
         if ((circle->origin - origin).length() > db.getptoler()) return 0;
         if (fabs(circle->radius - radius) > db.getptoler()) return 0;
         if (zaxis1.dot(zaxis2) > 0.0 &&
              (circle->position(0.0) - position(0.0)).length() < db.getptoler() &&
              (circle->position(1.0) - position(1.0)).length() < db.getptoler()) return 1;
         if (zaxis1.dot(zaxis2) < 0.0 &&
              (circle->position(1.0) - position(0.0)).length() < db.getptoler() &&
              (circle->position(0.0) - position(1.0)).length() < db.getptoler()) return -1;
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int Circle::explode(void)
{ return 0;
}

