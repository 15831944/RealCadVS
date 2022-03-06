/*
 * adapted to C++ code from the article
 * "An Incremental Angle ::Point in Polygon Test"
 * by Kevin Weiler, kjw@autodesk.com
 * in "Graphics Gems IV", Academic Press, 1994
 */

#include "ncwin.h"
using namespace std;

#include "ptin_poly.h"

typedef int quad_type;
 
//
// determine the quadrant of a polygon point relative to the test point:
//
inline quad_type quadrant( const Point &vertex, const Point &p )
{
  return (vertex.x > p.x)
    ? ((vertex.y > p.y) ? 0 : 3)
    : ( (vertex.y > p.y) ? 1 : 2);
}
 
//
// determine x intercept of a polygon edge
// with a horizontal line at the y value of the test point:
//
inline double x_intercept( const Point & pt1, const Point & pt2, double y )
{
  return pt2.x - ( (pt2.y - y) * ((pt1.x - pt2.x) / (pt1.y - pt2.y)) );
}
 
static void adjust_delta(
  int & delta,
  const Point & vertex,
  const Point & next_vertex,
  const Point & p )
{
  switch( delta ) {
    /* make quadrant deltas wrap around */
    case  3:  delta = -1; break;
    case -3:  delta =  1; break;
    /* check if went around point cw or ccw */
    case  2:
    case -2:
      if (x_intercept(vertex, next_vertex, p.y) > p.x)
        delta =  - (delta);
      break;
  }
}

/**
 * @brief polygonContains
 * @param polygon
 * @param p
 * @return true=inside; false=outside
 *
 * NOTE: points coincident with verticies and edges are considered to be outside
 *
 */
bool polygonContains(
  std::vector<Point> &polygon,
  const Point & p )
{
  quad_type quad, next_quad, delta, angle;
 
  /* initialize */
  const Point& vertex = polygon[0];
  quad = quadrant( vertex, p );
  angle = 0;
  /* loop on all vertices of polygon */
  for( int i = 0; i < polygon.size(); ++i ) {
    const Point & vertex = polygon[i];
    const Point & next_vertex = polygon[ (i+1 < polygon.size()) ? i+1 : 0];
    /* calculate quadrant and delta from last quadrant */
    next_quad = quadrant( next_vertex, p );
    delta = next_quad - quad;
    adjust_delta( delta, vertex, next_vertex, p );
    /* add delta to total angle sum */
    angle = angle + delta;
    /* increment for next step */
    quad = next_quad;
  }
  /* complete 360 degrees (angle of + 4 or -4 ) means inside */
  return (angle == +4) || (angle == -4);
      /* odd number of windings rule */
  /* if (angle & 4) return INSIDE; else return OUTSIDE; */
      /* non-zero winding rule */
  /* if (angle != 0) return INSIDE; else return OUTSIDE; */
}

void polyContainsTest()
{
    std::vector<Point> polygon;

    // ccw polygon
    polygon.push_back(Point(0,0,0));
    polygon.push_back(Point(100,0,0));
    polygon.push_back(Point(100,100,0));
    polygon.push_back(Point(0,100,0));

    // test points
    // mid point
    qDebug() << "*** testing polygonContains ***";
    qDebug() << "true = inside; false = outside";
    qDebug() << "with a ccw polygon";
    qDebug() << "test point inside    : " <<  polygonContains(polygon,Point(50,50,0))   << (polygonContains(polygon,Point(50,50,0))   ? " pass": " fail");
    qDebug() << "test point outside   : " <<  polygonContains(polygon,Point(110,110,0)) << (polygonContains(polygon,Point(110,110,0)) ? " fail": " pass");
    qDebug() << "test point on vertex : " <<  polygonContains(polygon,Point(100,100,0)) << (polygonContains(polygon,Point(100,100,0)) ? " fail": " pass");
    qDebug() << "test point on edge   : " <<  polygonContains(polygon,Point(100,50,0))  << (polygonContains(polygon,Point(100,50,0))  ? " fail": " pass");
    qDebug() << "********************************";
    qDebug() << "with a cw polygon";
    polygon.clear();
    polygon.push_back(Point(0,0,0));
    polygon.push_back(Point(0,100,0));
    polygon.push_back(Point(100,100,0));
    polygon.push_back(Point(100,0,0));
    qDebug() << "test point inside    : " <<  polygonContains(polygon,Point(50,50,0))   << (polygonContains(polygon,Point(50,50,0))   ? " pass": " fail");
    qDebug() << "test point outside   : " <<  polygonContains(polygon,Point(110,110,0)) << (polygonContains(polygon,Point(110,110,0)) ? " fail": " pass");
    qDebug() << "test point on vertex : " <<  polygonContains(polygon,Point(100,100,0)) << (polygonContains(polygon,Point(100,100,0)) ? " fail": " pass");
    qDebug() << "test point on edge   : " <<  polygonContains(polygon,Point(100,50,0))  << (polygonContains(polygon,Point(100,50,0))  ? " fail": " pass");
    qDebug() << "********************************";
}

void polyContainsTest(std::vector<Point> &inpolygon, Point tp)
{
    // test points
    std::vector<Point> polygon;
    polygon.assign(inpolygon.begin(),inpolygon.end());

    qDebug() << "*** testing polygonContains ***";
    qDebug() << "true = inside; false = outside";
    qDebug() << "with input polygon direction";
    qDebug() << "test point inside    : " <<  polygonContains(polygon,tp) << (polygonContains(polygon,tp) ? " pass": " fail");
    qDebug() << "test point outside   : " <<  polygonContains(polygon,tp) << (polygonContains(polygon,tp) ? " fail": " pass");
    qDebug() << "********************************";
    std::list<Point> revpolygon;
    for(int i=0; i < polygon.size(); i++)
        revpolygon.push_back(polygon.at(i));
    revpolygon.reverse();
    polygon.clear();
    polygon.assign(revpolygon.begin(),revpolygon.end());
    qDebug() << "with reversed polygon direction";
    qDebug() << "test point inside    : " <<  polygonContains(polygon,tp)   << (polygonContains(polygon,tp)   ? " pass": " fail");
    qDebug() << "test point outside   : " <<  polygonContains(polygon,tp) << (polygonContains(polygon,tp) ? " fail": " pass");
    qDebug() << "********************************";
}
