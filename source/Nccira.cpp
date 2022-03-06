//
// Implementation of the 'Circle' class
//

#include "ncwin.h"

void Circle::circlellc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Line *l1=0,*l2=0;
 Circle *circle;
 Point ldir1,ldir2,ldir3,zaxis,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,o1,o2,o3;
 double an,cosa,x1,x2,r1,r2,r3,d1,d2,d4,d5,d6,a,b,c,disc;
 int i;
   if (e1->isa(circle_entity))
     {  circle = (Circle *) e1;  p4 = p1;  l1 = (Line *) e2; p5 = p2;  l2 = (Line *) e3;  p6 = p3;
     }
   else if (e2->isa(circle_entity))
     {  circle = (Circle *) e2;  p4 = p2;  l1 = (Line *) e1; p5 = p1;  l2 = (Line *) e3;  p6 = p3;
     }
   else if (e3->isa(circle_entity))
     {  circle = (Circle *) e3;  p4 = p3;  l1 = (Line *) e1; p5 = p1;  l2 = (Line *) e2;  p6 = p2;
     }
   defined = 0;  d6 = 0.0;
   Intersect i1(l1,l2);
   if (i1.nintersections() == 1)
     {  p7 = ((PointE *) i1.intersection(0))->getp();
        for (i = 0 ; i < 8 ; i++)
          {  ldir1 = l1->direction(0.0);  ldir2 = l2->direction(0.0);
             if (i % 2) ldir2 = -ldir2;
             if ((i / 2) % 2) r1 = -circle->getradius(); else r1 = circle->getradius();
             ldir3 = (ldir1 + ldir2).normalize();
             if (ldir3.length() < 1.0E-10) continue;
             Line line(p7,p7+ldir3);
             p8 = line.nearp(circle->getorigin());
             d1 = (p8 - circle->getorigin()).length();
             d2 = (p8 - p7).length();
             if (i / 4) d2 = -d2;
             cosa = ldir1.dot(ldir3);
             if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
             an = acos(cosa);
             a = 1.0 - sin(an) * sin(an);
             b = - 2.0 * d2 - 2.0 * sin(an) * r1;
             c = d1 * d1 + d2 * d2 - r1 * r1;
             disc = b * b - 4.0 * a * c;
             if (fabs(a) > 1.0E-10 && disc >= 0.0)
               {  x1 = (-b + sqrt(disc)) / 2.0 / a;
                  r1 = x1 * sin(an);
                  x2 = (-b - sqrt(disc)) / 2.0 / a;
                  r2 = x2 * sin(an);
                  o1 = p7 + ldir3 * x1;
                  p9 = o1 + (o1 - circle->getorigin()).normalize() * fabs(r1);
                  if (fabs((circle->getorigin() - p9).length() - circle->getradius()) > db.getptoler() * 100.0)
                    p9 = o1 + (circle->getorigin() - o1).normalize() * fabs(r1);
                  p10 = l1->nearp(o1);
                  p11 = l2->nearp(o1);
                  o2 = p7 + ldir3 * x2;
                  p12 = o2 + (o2 - circle->getorigin()).normalize() * fabs(r2);
                  if (fabs((circle->getorigin() - p12).length() - circle->getradius()) > db.getptoler() * 100.0)
                    p12 = o2 + (circle->getorigin() - o2).normalize() * fabs(r2);
                  p13 = l1->nearp(o2);
                  p14 = l2->nearp(o2);
                  d4 = (p9 - p4).length() + (p10 - p5).length() + (p11 - p6).length();
                  d5 = (p12 - p4).length() + (p13 - p5).length() + (p14 - p6).length();
                  if (fabs((o1 - circle->getorigin()).length() - fabs(r1) - circle->getradius()) < db.getptoler() * 100.0 ||
                      fabs((o1 - circle->getorigin()).length() - fabs(fabs(r1) - circle->getradius())) < db.getptoler() * 100.0)
                    {  if (! defined || d4 < d6)
                         {  d6 = d4;  o3 = o1;  r3 = r1;  p15 = p9;  p16 = p10;  p17 = p11;  defined = 1;
                         }
                    }
                  if (fabs((o2 - circle->getorigin()).length() - fabs(r2) - circle->getradius()) < db.getptoler() * 100.0 ||
                      fabs((o2 - circle->getorigin()).length() - fabs(fabs(r2) - circle->getradius())) < db.getptoler() * 100.0)
                    {  if (! defined || d5 < d6)
                         {  d6 = d5;  o3 = o2;  r3 = r2;  p15 = p12;  p16 = p13;  p17 = p14;  defined = 1;
                         }
                    }
               }
          }
        if (defined)
          {  xaxis = (p16 - o3).normalize();
             yaxis = xaxis.cross(p17 - o3).cross(xaxis).normalize();
          }
     }
   else if (fabs(l1->direction(0.0).dot(l2->direction(0.0))) > cos(db.getatoler()))
     {  p12 = l1->nearp(circle->getorigin());
        p13 = l2->nearp(circle->getorigin());
        p7 = (p12 + p13) / 2.0;
        d1 = (p7 - circle->getorigin()).length();
        r3 = (p12 - p13).length() / 2.0;
        for (i = 0 ; i < 4 ; i++)
          {  if ((i % 2) == 0)
               x1 = (circle->getradius() + r3) * (circle->getradius() + r3) - d1 * d1;
             else
               x1 = (circle->getradius() - r3) * (circle->getradius() - r3) - d1 * d1;
             if (x1 < -db.getptoler() * db.getptoler()) continue;
             if (x1 < 0.0) x1 = 0.0;
             x1 = sqrt(x1);
             if (i / 2) x1 = -x1;
             o1 = p7 + l1->direction(0.0) * x1;
             p9 = o1 + (o1 - circle->getorigin()).normalize() * fabs(r3);
             if (fabs((circle->getorigin() - p9).length() - circle->getradius()) > db.getptoler() * 100.0)
               p9 = o1 + (circle->getorigin() - o1).normalize() * fabs(r3);
             p10 = p12 + l1->direction(0.0) * x1;
             p11 = p13 + l1->direction(0.0) * x1;
             d4 = (p9 - p4).length() + (p10 - p5).length() + (p11 - p6).length();
             if (! defined || d4 < d6)
               {  d6 = d4;  o3 = o1;  p15 = p9;  p16 = p10;  p17 = p11;  defined = 1;
               }
          }
        if (defined)
          {  xaxis = l1->direction(0.0);
             yaxis = xaxis.cross(p17 - o3).cross(xaxis).normalize();
          }
     }
   if (defined)
     {  origin = o3;
        radius = fabs(r3);
        if (arc)
          {  if (e1->isa(circle_entity))
               {  starta = angle(p15);  *mida = angle(p16);  sweepa = angle(p17);
               }
             else if (e2->isa(circle_entity))
               {  starta = angle(p16);  *mida = angle(p15);  sweepa = angle(p17);
               }
             else if (e3->isa(circle_entity))
               {  starta = angle(p16);  *mida = angle(p17);  sweepa = angle(p15);
               }
          }
     }
}

void Circle::circlepcc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Circle *c1,*c2;
 Point ldir1,ldir2,ldir3,zaxis,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,o1,o2,o3;
 double an,cosa,x1,x2,r1,r2,r3,r4,r5,d1,d2,d3,d4,d5,d6,h1,h2,a,b,c,A,B,C,D,disc;
 Transform t;
 int i;
   if (e1 == NULL)
     {  p4 = p1;  c1 = (Circle *) e2;  p5 = p2;  c2 = (Circle *) e3; p6 = p3;
     }
   else if (e2 == NULL)
     {  p4 = p2;  c1 = (Circle *) e1;  p5 = p1;  c2 = (Circle *) e3; p6 = p3;
     }
   else if (e3 == NULL)
     {  p4 = p3;  c1 = (Circle *) e1;  p5 = p1;  c2 = (Circle *) e2;  p6 = p2;
     }
   defined = 0;  d6 = 0.0;
   ldir1 = c2->getorigin() - c1->getorigin();
   d3 = ldir1.length();
   ldir2 = ldir1.cross(c1->getxaxis().cross(c1->getyaxis())).normalize();
   if (d3 > db.getptoler())
     {  ldir1 /= d3;
        Line line(c1->getorigin(),c1->getorigin() + ldir1);
        p7 = line.nearp(p4);
        d1 = (p7 - p4).length();
        d2 = ldir1.dot(p7 - c1->getorigin());
        for (i = 0 ; i < 8 ; i++)
          {  r1 = c1->getradius();
             if (i % 2) r1 = -r1;
             r2 = c2->getradius();
             if ((i/2) % 2) r2 = -r2;
             A = (d3 * d3 + r1 * r1 - r2 * r2) / 2.0 / d3;
             B = (r1 - r2) / d3;
             C = 2.0 * A * d2 - d2 * d2 - r1 * r1 - d1 * d1;
             D = -2.0 * r1 + 2.0 * d2 * B;
             a = D * D  + 4.0 * d1 * d1 * B * B - 4.0 * d1 * d1;
             b = 2.0 * C * D - 8.0 * d1 * d1 * r1 + 8.0 * d1 * d1 * A * B;
             c = C * C - 4.0 * d1 * d1 * r1 * r1 + 4.0 * d1 * d1 * A * A;
             disc = b * b - 4.0 * a * c;
             if (fabs(a) > 1.0E-10 && disc >= 0.0)
               {  r4 = (-b + sqrt(disc)) / 2.0 / a;
                  x1 = A + B * r4;
                  h1 = (r1 + r4) * (r1 + r4) - x1 * x1;
                  if (h1 >= 0.0)
                    {  h1 = sqrt(h1);
                       if (i/4) h1 = -h1;
                       o1 = c1->getorigin() + ldir1 * x1 + ldir2 * h1;
                       p9 = o1 + (o1 - c1->getorigin()).normalize() * fabs(r4);
                       if (fabs((c1->getorigin() - p9).length() - c1->getradius()) > db.getptoler() * 100.0)
                         p9 = o1 + (c1->getorigin() - o1).normalize() * fabs(r4);
                       p10 = o1 + (o1 - c2->getorigin()).normalize() * fabs(r4);
                       if (fabs((c2->getorigin() - p10).length() - c2->getradius()) > db.getptoler() * 100.0)
                         p9 = o1 + (c2->getorigin() - o1).normalize() * fabs(r4);
                       d4 = (p9 - p5).length() + (p10 - p6).length();
                       if (fabs((p4 - o1).length() - fabs(r4)) < db.getptoler() * 100.0 && (! defined || d4 < d6))
                         {  d6 = d4;  o3 = o1;  r3 = r4;  p13 = p9;  p14 = p10;  defined = 1;
                         }
                    }
                  r5 = (-b - sqrt(disc)) / 2.0 / a;
                  x2 = A + B * r5;
                  h2 = (r1 + r5) * (r1 + r5) - x2 * x2;
                  if (h2 >= 0.0)
                    {  h2 = sqrt(h2);
                       if (i/4) h2 = -h2;
                       o2 = c1->getorigin() + ldir1 * x2 + ldir2 * h2;
                       p11 = o2 + (o2 - c1->getorigin()).normalize() * fabs(r5);
                       if (fabs((c1->getorigin() - p11).length() - c1->getradius()) > db.getptoler() * 100.0)
                         p11 = o2 + (c1->getorigin() - o2).normalize() * fabs(r5);
                       p12 = o2 + (o2 - c2->getorigin()).normalize() * fabs(r5);
                       if (fabs((c2->getorigin() - p12).length() - c2->getradius()) > db.getptoler() * 100.0)
                         p12 = o2 + (c2->getorigin() - o2).normalize() * fabs(r5);
                       d5 = (p11 - p5).length() + (p12 - p6).length();
                       if (fabs((p4 - o2).length() - fabs(r5)) < db.getptoler() * 100.0 && (! defined || d5 < d6))
                         {  d6 = d5;  o3 = o2;  r3 = r5;  p13 = p11;  p14 = p12;  defined = 1;
                         }
                    }
               }
          }
     }
   else
     {  r3 = fabs(c1->getradius() - c2->getradius()) / 2.0;
        ldir1 = p4 - c1->getorigin();
        if (ldir1.length() > db.getptoler())
          {  ldir1.normalize();
             ldir2 = ldir1.cross(c1->getxaxis().cross(c2->getyaxis()));
             p7 = c1->nearp(p4);  p8 = c2->nearp(p4);  p9 = (p7 + p8) / 2.0;
             d1 = ldir1.dot(p4 - p9);
             if (d1 > r3) d1 = r3;
             if (d1 < -r3) d1 = -r3;
             r1 = (p9 - c1->getorigin()).length();
             if (fabs(r1) > db.getptoler() && fabs(r1 + d1) > db.getptoler())
               {  cosa = - (r3 * r3 - r1 * r1 - (r1 + d1) * (r1 + d1)) / 2.0 / r1 / (r1 + d1);
                  if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                  an = acos(cosa);
                  for (i = 0 ; i < 2 ; i++)
                    {  if (i == 1) an = -an;
                       t.identity();
                       t.rotate(c1->getorigin(),c1->getxaxis().cross(c1->getyaxis()),an);
                       o1 = t.transform((p7 + p8) / 2.0);
                       p11 = c1->nearp(o1);
                       p12 = c2->nearp(o1);
                       d5 = (p11 - p5).length() + (p12 - p6).length();
                       if (fabs((p4 - o1).length() - fabs(r3)) < db.getptoler() * 100.0 && (! defined || d5 < d6))
                         {  d6 = d5;  o3 = o1;  p13 = p11;  p14 = p12;  defined = 1;
                         }
                    }
               }
          }
     }
   if (defined)
     {  origin = o3;
        radius = fabs(r3);
        xaxis = ldir1;
        yaxis = ldir2;
        if (arc)
          {  if (e1 == NULL)
               {  starta = angle(p4);  *mida = angle(p13);  sweepa = angle(p14);
               }
             else if (e2 == NULL)
               {  starta = angle(p13);  *mida = angle(p4);  sweepa = angle(p14);
               }
             else if (e3 == NULL)
               {  starta = angle(p13);  *mida = angle(p14);  sweepa = angle(p4);
               }
          }
     }
}

void Circle::circlelcc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Line *line;
 Circle *c1,*c2;
 Point ldir1,ldir2,ldir3,zaxis,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,o1,o2,o3;
 double x1,r1,r2,r3,r4,d1,d2,d3,d4,d6,a,b,c,A,B,disc;
 int i;
   if (e1->isa(line_entity))
     {  line = (Line *) e1;  p4 = p1;  c1 = (Circle *) e2;  p5 = p2;  c2 = (Circle *) e3; p6 = p3;
     }
   else if (e2->isa(line_entity))
     {  line = (Line *) e2;  p4 = p2;  c1 = (Circle *) e1;  p5 = p1;  c2 = (Circle *) e3; p6 = p3;
     }
   else if (e3->isa(line_entity))
     {  line = (Line *) e3;  p4 = p3;  c1 = (Circle *) e1;  p5 = p1;  c2 = (Circle *) e2;  p6 = p2;
     }
   defined = 0;  d6 = 0.0;
   p7 = line->nearp(c1->getorigin());
   p8 = line->nearp(c2->getorigin());
   d1 = (p7 - c1->getorigin()).length();
   d2 = (p8 - c2->getorigin()).length();
   if ((p7 - c1->getorigin()).dot(p8 - c2->getorigin()) < 0.0)
     d2 = -d2;
   d3 = (p7 - p8).length();
   ldir1 = line->direction(0.0);
   if (ldir1.length() < db.getptoler()) return;
   if (ldir1.dot(p8 - p7) < 0.0) ldir1 = -ldir1;
   ldir2 = ldir1.cross(c1->getxaxis().cross(c1->getyaxis())).normalize();
   d4 = ldir1.length();
   if (d3 > db.getptoler())
     {  ldir1 /= d4;
        for (i = 0 ; i < 16 ; i++)
          {  r1 = c1->getradius();
             if (i % 2) r1 = -r1;
             r2 = c2->getradius();
             if ((i/2) % 2) r2 = -r2;
             A = 1.0 / 2.0 / (r1 + d1);
             B = (d1 * d1 - r1 * r1) / 2.0 / (r1 + d1);
             a = 1.0 - 2.0 * d2 * A - 2.0 * r2 * A;
             b = - 2.0 * d3;
             c = d2 * d2 + d3 * d3 - 2.0 * d2 * B - 2.0 * r2 * B - r2 * r2;
             disc = b * b - 4.0 * a * c;
             if (fabs(a) > 1.0E-10 && disc >= 0.0)
               {  if ((i / 4) % 2)
                    x1 = (-b + sqrt(disc)) / 2.0 / a;
                  else
                    x1 = (-b - sqrt(disc)) / 2.0 / a;
                  r4 = A * x1 * x1 + B;
                  if (i / 8)
                    o1 = p7 + ldir1 * x1 + ldir2 * r4;
                  else
                    o1 = p7 + ldir1 * x1 - ldir2 * r4;
                  p9 = line->nearp(o1);
                  p10 = o1 + (o1 - c1->getorigin()).normalize() * fabs(r4);
                  if (fabs((c1->getorigin() - p10).length() - c1->getradius()) > db.getptoler() * 100.0)
                    p10 = o1 + (c1->getorigin() - o1).normalize() * fabs(r4);
                  p11 = o1 + (o1 - c2->getorigin()).normalize() * fabs(r4);
                  if (fabs((c2->getorigin() - p11).length() - c2->getradius()) > db.getptoler() * 100.0)
                    p11 = o1 + (c2->getorigin() - o1).normalize() * fabs(r4);
                  d4 = (p9 - p4).length() + (p10 - p5).length() + (p11 - p6).length();
                  if ((fabs((o1 - c1->getorigin()).length() - fabs(r4) - c1->getradius()) < db.getptoler() * 100.0 ||
                       fabs((o1 - c1->getorigin()).length() - fabs(fabs(r4) - c1->getradius())) < db.getptoler() * 100.0) &&
                      (fabs((o1 - c2->getorigin()).length() - fabs(r4) - c2->getradius()) < db.getptoler() * 100.0 ||
                       fabs((o1 - c2->getorigin()).length() - fabs(fabs(r4) - c2->getradius())) < db.getptoler() * 100.0))
                    {  if (! defined || d4 < d6)
                         {  d6 = d4;  o3 = o1;  r3 = r4;  p12 = p9;  p13 = p10;  p14 = p11;  defined = 1;
                         }
                    }
               }
          }
     }
   else
     {  p7 = line->nearp(c1->getorigin());
        d1 = (p7 - c1->getorigin()).length();
        ldir1 = line->direction(0.0);
        ldir2 = ldir1.cross(c1->getxaxis().cross(c1->getyaxis())).normalize();
        if (ldir2.dot(p7 - c1->getorigin()) < 0.0) ldir2 = -ldir2;
        if (c1->getradius() < c2->getradius())
          {  r1 = c1->getradius();  r2 = c2->getradius();
          }
        else
          {  r1 = c2->getradius();  r2 = c1->getradius();
          }
        r3 = (r2 - r1) / 2.0;
        for (i = 0 ; i < 4 ; i++)
          {  if (i / 2)
               x1 = (r1 + r3) * (r1 + r3) - (d1 - r3) * (d1 - r3);
             else
               x1 = (r1 + r3) * (r1 + r3) - (d1 + r3) * (d1 + r3);
             if (x1 < -db.getptoler() * db.getptoler()) continue;
             if (x1 < 0.0) x1 = 0.0;
             x1 = sqrt(x1);
             if (i % 2) x1 = -x1;
             if (i / 2)
               o1 = p7 + ldir1 * x1 - ldir2 * r3;
             else
               o1 = p7 + ldir1 * x1 + ldir2 * r3;
             p9 = line->nearp(o1);
             p10 = c1->nearp(o1);
             p11 = c2->nearp(o1);
             d4 = (p9 - p4).length() + (p10 - p5).length() + (p11 - p6).length();
             if (! defined || d4 < d6)
               {  d6 = d4;  o3 = o1;  p12 = p9;  p13 = p10;  p14 = p11;  defined = 1;
               }
          }
     }
   if (defined)
     {  origin = o3;
        radius = fabs(r3);
        xaxis = ldir1;
        yaxis = ldir2;
        if (arc)
          {  if (e1->isa(line_entity))
               {  starta = angle(p12);  *mida = angle(p13);  sweepa = angle(p14);
               }
             else if (e2->isa(line_entity))
               {  starta = angle(p13);  *mida = angle(p12);  sweepa = angle(p14);
               }
             else if (e3->isa(line_entity))
               {  starta = angle(p13);  *mida = angle(p14);  sweepa = angle(p12);
               }
          }
     }
}

void Circle::circleccc(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3,double *mida)
{Circle *c1,*c2,*c3;
 Point ldir1,ldir2,ldir3,zaxis,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,o1,o2,o3;
 double rmin,rmax,x1,y1,x2,x3,y3,r1,r2,r3,r4,r5,r6,r7,r8,d1,d3,d4,d5,d7,A,B;
 Transform t;
 int i,j,def1;
   c1 = (Circle *) e1;  c2 = (Circle *) e2;  c3 = (Circle *) e3;
   p4 = p1;  p5 = p2;  p6 = p3;
   ldir1 = c2->getorigin() - c1->getorigin();
   if (ldir1.length() < db.getptoler())
     {  c2 = (Circle *) e3;  c3 = (Circle *) e2;
        p5 = p3;  p6 = p2;
        ldir1 = c2->getorigin() - c1->getorigin();
     }
   defined = 0;
   Circle circle(0,e1,p4,e2,p5,NULL,p6);
   if (circle.getdefined() && ldir1.length() > db.getptoler())
     {  rmin = rmax = circle.getradius();  rmin /= 4.0;  rmax *= 8.0;
        ldir1.normalize();
        ldir2 = ldir1.cross(c1->getxaxis().cross(c1->getyaxis())).normalize();
        x2 = ldir1.dot(c2->getorigin() - c1->getorigin());
        x1 = ldir1.dot(c3->getorigin() - c1->getorigin());
        y1 = ldir2.dot(c3->getorigin() - c1->getorigin());
        d7 = 0.0;
        for (i = 0 ; i < 16 ; i++)
          {  r1 = (i % 2     ? -c1->getradius() : c1->getradius());
             r2 = ((i/2) % 2 ? -c2->getradius() : c2->getradius());
             r3 = ((i/4) % 2 ? -c3->getradius() : c3->getradius());
             A = (r1 - r2) / x2;
             B = (x2 * x2 - r2 * r2 + r1 * r1) / 2.0 / x2;
             for (r4 = rmin,def1 = 0,d3 = 0.0 ; r4 < rmax ; r4 += (rmax - rmin) / 100.0)
               {  x3 = A * r4 + B;
                  y3 = (r1 + r4) * (r1 + r4) - x3 * x3;
                  if (y3 < 0.0) continue;
                  y3 = sqrt(y3);
                  if (i / 8) y3 = -y3;
                  d1 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) - fabs(r4 + r3);
                  if (def1 && d3 * d1 < 0.0 || fabs(d1) < db.getptoler())
                    {  r5 = r4 - (rmax - rmin) / 100.0;  d5 = d3;
                       r6 = r4;
                       d3 = d1;
                       for (j = 0 ; j < 50 ; j++)
                         {  r7 = (r5 + r6) / 2.0;
                            x3 = A * r7 + B;
                            y3 = (r1 + r7) * (r1 + r7) - x3 * x3;
                            if (y3 < 0.0) continue;
                            y3 = sqrt(y3);
                            if (i / 8) y3 = -y3;
                            d1 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) - fabs(r7 + r3);
                            if (d1 * d5 >= 0.0)
                              {  r5 = r7;  d5 = d1;
                              }
                            else
                              {  r6 = r7;
                              }
                         }
                       o1 = c1->getorigin() + ldir1 * x3 + ldir2 * y3;
                       p9 = o1 + (o1 - c1->getorigin()).normalize() * r7;
                       if (fabs((c1->getorigin() - p9).length() - c1->getradius()) > db.getptoler() * 1000.0)
                         p9 = o1 + (c1->getorigin() - o1).normalize() * r7;
                       p10 = o1 + (o1 - c2->getorigin()).normalize() * r7;
                       if (fabs((c2->getorigin() - p10).length() - c2->getradius()) > db.getptoler() * 1000.0)
                         p10 = o1 + (c2->getorigin() - o1).normalize() * r7;
                       p11 = o1 + (o1 - c3->getorigin()).normalize() * r7;
                       if (fabs((c3->getorigin() - p11).length() - c3->getradius()) > db.getptoler() * 1000.0)
                         p11 = o1 + (c3->getorigin() - o1).normalize() * r7;
                       d4 = (p9 - p4).length() + (p10 - p5).length() + (p11 - p6).length();
                       if (! defined || d4 < d7)
                         {  d7 = d4;  o3 = o1;  r8 = r7;  p12 = p9;  p13 = p10;  p14 = p11;  defined = 1;
                         }
                    }
                  else
                    d3 = d1;
                  def1 = 1;
               }
          }
     }
   if (defined)
     {  origin = o3;
        radius = r8;
        xaxis = ldir1;
        yaxis = ldir2;
        if (arc)
          {  if (c2 == (Circle *) e2)
               {  starta = angle(p12);  *mida = angle(p13);  sweepa = angle(p14);
               }
             else
               {  starta = angle(p12);  *mida = angle(p14);  sweepa = angle(p13);
               }
          }
     }
}

Circle::Circle(int arc,Entity *e1,Point p1,Entity *e2,Point p2,Entity *e3,Point p3)
{
#ifdef USING_WIDGETS
     qgi1=0;
     qgi2=0;
#endif

    double mida;
  if (e1 == NULL && e2 == NULL && e3 == NULL)
    circleppp(arc,p1,p2,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity) &&
           e2 != NULL && e2->isa(line_entity) &&
           e3 != NULL && e3->isa(line_entity))
     circlelll(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity) &&
           e2 != NULL && e2->isa(line_entity) &&
           e3 == NULL                         ||
           e1 != NULL && e1->isa(line_entity) &&
           e2 == NULL                         &&
           e3 != NULL && e3->isa(line_entity) ||
           e1 == NULL                         &&
           e2 != NULL && e2->isa(line_entity) &&
           e3 != NULL && e3->isa(line_entity))
     circlepll(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity) && e2 == NULL && e3 == NULL ||
           e1 == NULL && e2 != NULL && e2->isa(line_entity) && e3 == NULL ||
           e1 == NULL && e2 == NULL && e3 != NULL && e3->isa(line_entity))
     circleppl(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(circle_entity) && e2 == NULL && e3 == NULL ||
           e1 == NULL && e2 != NULL && e2->isa(circle_entity) && e3 == NULL ||
           e1 == NULL && e2 == NULL && e3 != NULL && e3->isa(circle_entity))
     circleppc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity) && e2 != NULL && e2->isa(circle_entity) && e3 == NULL ||
           e2 != NULL && e2->isa(line_entity) && e3 != NULL && e3->isa(circle_entity) && e1 == NULL ||
           e3 != NULL && e3->isa(line_entity) && e1 != NULL && e1->isa(circle_entity) && e2 == NULL ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(line_entity) && e3 == NULL ||
           e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(line_entity) && e1 == NULL ||
           e3 != NULL && e3->isa(circle_entity) && e1 != NULL && e1->isa(line_entity) && e2 == NULL)
     circleplc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity) && e2 != NULL && e2->isa(line_entity) && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(line_entity) && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(line_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(line_entity) && e3 != NULL && e3->isa(line_entity))
     circlellc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 == NULL && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 == NULL && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(circle_entity) && e3 == NULL)
     circlepcc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(line_entity)   && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(line_entity)   && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(line_entity))
     circlelcc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else if (e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(circle_entity) ||
           e1 != NULL && e1->isa(circle_entity) && e2 != NULL && e2->isa(circle_entity) && e3 != NULL && e3->isa(circle_entity))
    circleccc(arc,e1,p1,e2,p2,e3,p3,&mida);
  else
    defined = 0;
  if (defined)
    {  if (arc)
         {  if (mida < starta) mida += 2.0 * M_PI;
            if (sweepa < starta) sweepa += 2.0 * M_PI;
            if (mida > sweepa)
              {  mida = sweepa;  sweepa = starta;  starta = mida;
              }
            while (sweepa > 2.0 * M_PI) sweepa -= 2.0 * M_PI;
            while (starta > 2.0 * M_PI) starta -= 2.0 * M_PI;
            if (sweepa < starta) sweepa += 2.0 * M_PI;
              sweepa -= starta;
         }
       else
         {  starta = 0.0;  sweepa = 2.0 * M_PI;
         }
    }
}

void Circle::reverse(void)
{ if (getposition() != 0)
    db.saveundo(UD_MOVED,this);
  starta += sweepa;
  sweepa = -sweepa;

  if (getposition() != 0 && db.MovedCallback != 0)
    db.MovedCallback(this);

}
