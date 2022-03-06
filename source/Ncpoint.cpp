//
//  Implementation of the 'Point' class
//

#include "ncwin.h"

Point::Point(double xv,double yv,double zv)
{
#ifdef USING_WIDGETS
    // init the Qt graphics item
    //qgi = 0;
#endif
    x = xv;  y = yv;  z = zv;
}

Point::Point()
{
#ifdef USING_WIDGETS
    // init the Qt graphics item
    //qgi = 0;
#endif
     //x = y = z = -1.0E38;
     //x = y = z = -RCREAL_MAX;
     x = y = z = -RCDBL_MAX;
}

Point Point::operator-(void) const
{
    Point tmp(-x,-y,-z);  return tmp;
}

Point Point::cross(const Point& p2) const
{Point tmp(y*p2.z-z*p2.y,z*p2.x-x*p2.z,x*p2.y-y*p2.x);
  return tmp;
}

Point &Point::normalize(void)
{double l;
  l = sqrt(x*x + y*y + z*z);
  if (l > 1.0E-12)
    {  x /= l;  y /= l;  z /= l;
    }
  return *this;
}

double Point::length() const
{ return sqrt(x*x + y*y + z*z);
}

// for compatibility
double Point::GetLength() const
{
    return length();
}

double Point::dot(const Point& p2) const
{ return x*p2.x+y*p2.y+z*p2.z;
}

Point Point::operator+(const Point& p) const
{Point tmp(x + p.x,y + p.y,z + p.z);
  return tmp;
}

Point Point::operator-(const Point& p) const
{Point tmp(x - p.x,y - p.y,z - p.z);
   return tmp;
}

Point Point::operator*(double scale) const
{Point tmp(x*scale,y*scale,z*scale);
  return tmp;
}

Point Point::operator/(double scale) const
{Point tmp(x/scale,y/scale,z/scale);
  return tmp;
}

int Point::operator==(const Point& p) const
{  return (fabs(x - p.x) < db.getptoler() &&
           fabs(y - p.y) < db.getptoler() &&
           fabs(z - p.z) < db.getptoler());
}

void Point::operator*=(const double s)
{  x *= s;
   y *= s;
   z *= s;
}

void Point::operator/=(const double s)
{  x /= s;
   y /= s;
   z /= s;
}

void  Point::operator+=(const Point& p)
{  x += p.x;
   y += p.y;
   z += p.z;
}

void  Point::operator-=(const Point& p)
{  x -= p.x;
   y -= p.y;
   z -= p.z;
}

void Point::dump(const RCCHAR *name) const
{RCCHAR mess[300];
  sprintf(mess,"%s : %f %f %f ",name,x,y,z);
  cadwindow->MessageBox(mess,"",MB_ICONSTOP);
}

int Point::save(CadFile *outfile) const
{short type;
  type = 0;
  if (! outfile->write(&type)) return 0;
  if (! outfile->write(&x)) return 0;
  if (! outfile->write(&y)) return 0;
  if (! outfile->write(&z)) return 0;
  return 1;
}

int Point::load(CadFile *infile)
{short type;
  if (! infile->read(&type)) return 0;
  if (! infile->read(&x)) return 0;
  if (! infile->read(&y)) return 0;
  if (! infile->read(&z)) return 0;
  return 1;
}

RCCHAR *Point::string(RCCHAR *s) const
{
  RCCHAR text1[300],text2[300],text3[300];
  sprintf(s,"X%ls,Y%ls,Z%ls",db.formatnumber(text1,x,0),db.formatnumber(text2,y,0),db.formatnumber(text3,z));
  return s;
}


