//
//  Declaration of the Point class
//

#if ! defined(__Point)
#define __Point

class CadFile;
class UndoFile;

class Point
  {
public:
     double x,y,z;
     Point(double xv,double yv,double zv);
     Point();
     Point cross(const Point& p2) const;
     double dot(const Point& p2) const;
     double length() const;
     double GetLength() const;
     Point &normalize(void);
     Point operator+ (const Point& p) const;
     Point operator- (const Point& p) const;
     Point operator- (void) const;
     Point operator* (double) const;
     Point operator/ (double) const;
     int   operator==(const Point& p) const;
     void  operator*=(const double s);
     void  operator/=(const double s);
     void  operator+=(const Point& p);
     void  operator-=(const Point& p);
     Point &setxyz(double x1,double y1,double z1)
       { x = x1;  y = y1;  z = z1;  return *this; }
     void dump(const RCCHAR *) const;
     int load(CadFile *);
     int save(CadFile *) const;
     RCCHAR *string(RCCHAR *) const;
#ifdef USING_WIDGETS
public:
    // pointers to graphics item for this point
    // null if not drawn in the scene
    //QGraphicsItem *qgi; // Qt graphics item
#endif
};


#endif
