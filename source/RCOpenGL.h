#ifndef REALCADOPENGL_H
#define REALCADOPENGL_H

#define REALCADOPENGLSHARED_EXPORT

#include <QtOpenGL>
#include <QOpenGLWidget>
#ifdef _MAC
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "ncwin.h"
#include "RCOpenGL.h"

class Point;

static double pixelmm = 4;

typedef void* RCHDC;
typedef void* RCHGLRC;

typedef Point DPoint;

// KMJ:
class GLProperties
{
public:
    GLfloat falwidths[2];
    GLfloat fslwidths[2];
    GLfloat fslstepsize;
    GLfloat fapsizes[2];
    GLfloat fspsizes[2];
    GLfloat fapstepsize;
    GLfloat fspstepsize;
};

#if 0
class DPoint
  {public:
     double x,y,z;
     DPoint(double xv,double yv,double zv);
     DPoint();
     DPoint cross(const DPoint& p2) const;
     double dot(const DPoint& p2) const;
     double length() const;
     DPoint &normalize(void);
     DPoint operator+ (const DPoint& p) const;
     DPoint operator- (const DPoint& p) const;
     DPoint operator- (void) const;
     DPoint operator* (double) const;
     DPoint operator/ (double) const;
     int   operator==(const DPoint& p) const;
     void  operator*=(const double s);
     void  operator/=(const double s);
     void  operator+=(const DPoint& p);
     void  operator-=(const DPoint& p);
     DPoint &setxyz(double x1,double y1,double z1)
       { x = x1;  y = y1;  z = z1;  return *this; }
  };
#endif
#if 0
class Point
  {public:
     float x,y,z;
     Point(float xv,float yv,float zv);
     Point();
     Point cross(const Point& p2) const;
     float dot(const Point& p2) const;
     float length() const;
     Point &normalize(void);
     Point operator+ (const Point& p) const;
     Point operator- (const Point& p) const;
     Point operator- (void) const;
     Point operator* (float) const;
     Point operator/ (float) const;
     int   operator==(const Point& p) const;
     void  operator*=(const float s);
     void  operator/=(const float s);
     void  operator+=(const Point& p);
     void  operator-=(const Point& p);
     Point &setxyz(float x1,float y1,float z1)
       { x = x1;  y = y1;  z = z1;  return *this; }
  };
#endif

#define BS_FRONTBUFFERONLY   1
#define BS_IGNOREDEPTH       2
#define BS_NOCLEAR           4


extern GLProperties GLProps;

void RCGlError (GLenum errorCode);
void RCCheckGlError();
void RCGLInit();
const GLubyte* RCGetGLVersion();
const GLubyte* RCGetGLVendor();
const GLubyte* RCGetGLRenderer();
const GLubyte* RCGetGLSLVersion();
const GLubyte* RCGetGLExtensions();

int SetLight(double x,double y,double z);
int SetLightEx(int index,double x,double y,double z,double r,double g,double b);
int SetNoMaterial(int r,int g,int b);
int SetNoMaterial(double r, double g, double b, double t=0.0);
int SetMaterial(double r,double g,double b,double a,double d,double s);
int SetMaterialEx(double r,double g,double b,double a,double d,double s,double t);
int SetBackground(double r,double g,double b);
int SetView(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int perspective);
int RCSetView(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int perspective);
int RCSetViewing(int x,int y,int w,int h,double umin,double vmin,double umax,double vmax,Point eye, Point ref, Point up, double front, double back, int perspective);
int SetViewport(double umin,double vmin,double umax,double vmax);
int RCSetViewport(int x,int y,int w,int h,double umin,double vmin,double umax,double vmax,double front,double back,int p);
int BeginScene(int);
int EndScene(void);
int QtBeginScene(void);
int QtEndScene(void);
int DrawTriangle(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double nx,double ny,double nz);
int DrawTexturedTriangle( DPoint *p);
int DrawTexturedTriangles(int ntriangles, DPoint *p);
int OpenOpenGl(RCHDC hdc,int surfacetype,int x,int y,int width,int height);
int CloseOpenGl(HDC hdc);
int CloseDirect3d(void);
//int LoadTexture(char *filename,double r,double g,double b,double trange,double op,int *h);
int LoadTexture(RCCHAR *filename,double r,double g,double b,double trange,double op,int *h);
int LoadTexturePrint(RCCHAR *filename,double r,double g,double b,double trange,double op,int *h);
//int LoadMemoryTexture(char *filename,char *filedata,int width,int height,double r,double g,double b,double trange,double op,int *h);
int LoadMemoryTexture(RCCHAR *filename,char *filedata,int width,int height,double r,double g,double b,double trange,double op,int *h);
int SetTexture(long texture);
int ReleaseTextures(RCHDC hdc);
int ReleaseTextures();
int DrawPolygon(DPoint origin,DPoint xaxis,DPoint yaxis,int nloops,int *nvert,DPoint **vert);
int TriangulatePolygon(int nloops,int *nvert,DPoint **vert,DPoint **triangles);
int Flush(void);
int DrawLine(DPoint p1, DPoint p2, double w=1.0);
int DrawLineStrip( Point *points, int npoints, double w );
int DrawLineLoop( Point *points, int npoints, double w );

void setGLpixelMM(double ps);
double setGLpixelMM(void);
Point GLScreenToModel(Point p);
int glhInvertMatrixd2(double *m, double *out);


#endif
