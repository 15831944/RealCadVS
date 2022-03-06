
#define MAX_INTERSECTIONS 100

class Intersect
  {private:
     int n;
     Entity *ints[MAX_INTERSECTIONS];
     void intersect(Entity *,Entity *);
     void LineLine(Line *,Line *);
     void LineCircle(Line *,Circle *);
     void LineEllipse(Line *,EllipseE *);
     void LineCurve(Line *,Curve *);
     void CircleCircle(Circle *,Circle *);
     void CircleEllipse(Circle *,EllipseE *);
     void CircleCurve(Circle *,Curve *);
     void EllipseEllipse(EllipseE *,EllipseE *);
     void EllipseCurve(EllipseE *,Curve *);
     void CurveCurve(Curve *,Curve *);
     void LineInfinitePlane(Line *,InfinitePlane *);
     void LinePlane(Line *,Plane *);
     void CirclePlane(Circle *,Plane *);
     void PlaneInfinitePlane(Plane *,InfinitePlane *);
     void WallInfinitePlane(Wall *,InfinitePlane *);
     void FigureInfinitePlane(Figure *,InfinitePlane *);
     void UVSurfaceInfinitePlane(UVSurfaceEntity *,InfinitePlane *);
     void UVSurfaceLine(UVSurfaceEntity *,Line *,double u,double v,double du,double dv);
   public:
     Intersect(Entity *,Entity *);
     Intersect(Entity *,Entity *,Point);
     ~Intersect();
     int nintersections(void) {  return n;  }
     Entity *intersection(int i)
       {  if (i < 0 || i >= n) return NULL;
          return ints[i];
       }
  };