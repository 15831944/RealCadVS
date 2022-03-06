// RealCADOpenGl.cpp : Defines the exported functions for the DLL application.
//

// RealCADOpenGL.cpp : Defines the entry point for the DLL application.
//


//#include "stdafx.h"
//#include "realcadopengl_global.h"
#include "ncwin.h"
#ifdef _MAC
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <math.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <stdio.h>
#include <float.h>
#include "RCView.h"
#include "RCGraphicsView.h"
#include "RCOpenGL.h"

#define MAX_WINDOWS 200

// KMJ:
GLProperties GLProps;

struct OpenGLDeviceCache
{
    RCHDC hdc;
    int windowx,windowy,windowwidth,windowheight;
    RCHGLRC hglrc;
} OpenGLDeviceCache[MAX_WINDOWS];
static int nopengldevices = 0;

#define MAX_TEXTURES 400

struct OpenGLTextureCache
{
   RCCHAR filename[300];
   char *filedata;
   int red,green,blue,transparencyrange,opacity;
   int texture; 
   RCHDC hdc;
} OpenGLTextureCache[MAX_TEXTURES];
static int nopengltextures = 0;

void __stdcall DPBegin (GLenum t);
void __stdcall DPVertex (void * vr);
void __stdcall DPEnd (void);
void __stdcall DPCombine (GLdouble coord[3],
                          GLdouble *vertex_data[4],
                          GLfloat weight[4], GLdouble **dataOut );
void __stdcall DPError (GLenum errorCode);

void __stdcall TPBegin (GLenum t);
void __stdcall TPVertex (void * vr);
void __stdcall TPCombine (GLdouble coord[3],
                          GLdouble *vertex_data[4],
                          GLfloat weight[4], GLdouble **dataOut );
void __stdcall TPEnd (void);

/*
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{ switch (ul_reason_for_call)
	  {case DLL_PROCESS_ATTACH:
   	case DLL_THREAD_ATTACH:
		  case DLL_THREAD_DETACH:
		  case DLL_PROCESS_DETACH:
			   break;
   }
  return TRUE;
}
*/

//
//  Point functions
//

#if 0
Point::Point(float xv,float yv,float zv)
{  x = xv;  y = yv;  z = zv;
}

Point::Point()
{  x = y = z = -1.0E38f;
}

Point Point::operator-(void) const
{Point tmp(-x,-y,-z);  return tmp;
}

Point Point::cross(const Point& p2) const
{Point tmp(y*p2.z-z*p2.y,z*p2.x-x*p2.z,x*p2.y-y*p2.x);
  return tmp;
}

Point &Point::normalize(void)
{float l;
  l = (float)sqrt(x*x + y*y + z*z);
  if (l > 1.0E-12)
    {  x /= l;  y /= l;  z /= l;
    }
  return *this;
}

float Point::length() const
{ return (float)sqrt(x*x + y*y + z*z);
}

float Point::dot(const Point& p2) const
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

Point Point::operator*(float scale) const
{Point tmp(x*scale,y*scale,z*scale);
  return tmp;
}

Point Point::operator/(float scale) const
{Point tmp(x/scale,y/scale,z/scale);
  return tmp;
}

int Point::operator==(const Point& p) const
{  return (fabs(x - p.x) < 1.0e-5f &&
           fabs(y - p.y) < 1.0e-5f &&
           fabs(z - p.z) < 1.0e-5f);
}

void Point::operator*=(const float s)
{  x *= s;
   y *= s;
   z *= s;
}

void Point::operator/=(const float s)
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
#endif

#if 0
DPoint::DPoint(double xv,double yv,double zv)
{  x = xv;  y = yv;  z = zv;
}

DPoint::DPoint()
{  x = y = z = -1.0E38f;
}

DPoint DPoint::operator-(void) const
{DPoint tmp(-x,-y,-z);  return tmp;
}

DPoint DPoint::cross(const DPoint& p2) const
{DPoint tmp(y*p2.z-z*p2.y,z*p2.x-x*p2.z,x*p2.y-y*p2.x);
  return tmp;
}

DPoint &DPoint::normalize(void)
{double l;
  l = sqrt(x*x + y*y + z*z);
  if (l > 1.0E-12)
    {  x /= l;  y /= l;  z /= l;
    }
  return *this;
}

double DPoint::length() const
{ return sqrt(x*x + y*y + z*z);
}

double DPoint::dot(const DPoint& p2) const
{ return x*p2.x+y*p2.y+z*p2.z;
}

DPoint DPoint::operator+(const DPoint& p) const
{DPoint tmp(x + p.x,y + p.y,z + p.z);
  return tmp;
}

DPoint DPoint::operator-(const DPoint& p) const
{DPoint tmp(x - p.x,y - p.y,z - p.z);
   return tmp;
}

DPoint DPoint::operator*(double scale) const
{DPoint tmp(x*scale,y*scale,z*scale);
  return tmp;
}

DPoint DPoint::operator/(double scale) const
{DPoint tmp(x/scale,y/scale,z/scale);
  return tmp;
}

int DPoint::operator==(const DPoint& p) const
{  return (fabs(x - p.x) < 1.0e-5 &&
           fabs(y - p.y) < 1.0e-5 &&
           fabs(z - p.z) < 1.0e-5);
}

void DPoint::operator*=(const double s)
{  x *= s;
   y *= s;
   z *= s;
}

void DPoint::operator/=(const double s)
{  x /= s;
   y /= s;
   z /= s;
}

void  DPoint::operator+=(const DPoint& p)
{  x += p.x;
   y += p.y;
   z += p.z;
}

void  DPoint::operator-=(const DPoint& p)
{  x -= p.x;
   y -= p.y;
   z -= p.z;
}
#endif

static RCHDC hdc;
static RCHGLRC  hglrc;
static GLuint texture;
static double near_plane = 1000.0,far_plane = -1000.0;
static int windowx,windowy,windowwidth,windowheight,perspective;
double lx,ly,lz;

extern "C"  char *GetVersionString(void)
{
  static char version[300];
  //sprintf_s(version,300,"RealCAD OpenGL Interface : V1.00   %s  %s",__DATE__,__TIME__);
  return version;
}

void RCGlError (GLenum errorCode)
{
    if(errorCode == GL_NO_ERROR)
        return;

     const GLubyte *estring;

     estring = gluErrorString(errorCode);
#ifdef _RCDEBUGxx
     fprintf(stderr,"OpenGL Error: %s\n", estring );
#endif
}

void RCCheckGlError()
{
#ifdef _RCDEBUGxx
    RCGlError(glGetError());
#endif
}

const GLubyte* RCGetGLVersion()
{
    return glGetString(GL_VERSION);
}

const GLubyte* RCGetGLVendor()
{
    return glGetString(GL_VENDOR);
}

const GLubyte* RCGetGLRenderer()
{
    return glGetString(GL_RENDERER);
}

const GLubyte* RCGetGLSLVersion()
{
    return glGetString(GL_SHADING_LANGUAGE_VERSION);
}

const GLubyte* RCGetGLExtensions()
{
    return glGetString(GL_EXTENSIONS);
}

void RCGLInitProps()
{

    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE,GLProps.falwidths);
    glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE,GLProps.fslwidths);
    glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY,&GLProps.fslstepsize);

    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE,GLProps.fapsizes);
    glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE,GLProps.fspsizes);
    glGetFloatv(GL_SMOOTH_POINT_SIZE_GRANULARITY,&GLProps.fspstepsize);

}

 int SetLight(double x,double y,double z)
{float p[4];

  lx = x;  ly = y;  lz = z;

  p[0] = (float)x; p[1] = (float)y; p[2] = (float)z; p[3] = 1.0f;
  glLightfv(GL_LIGHT0,GL_POSITION,p);
  p[0] = 1.0f; p[1] = 1.0f; p[2] = 1.0f; p[3] = 1.0f;
  glLightfv(GL_LIGHT0,GL_AMBIENT,p);
  p[0] = 1.0f; p[1] = 1.0f; p[2] = 1.0f; p[3] = 1.0f;
  glLightfv(GL_LIGHT0,GL_DIFFUSE,p);
  glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,20.0f);

  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
  glEnable(GL_LIGHT0);
  glDisable(GL_CULL_FACE);

  return 0;
}

 int SetLightEx(int index,double x,double y,double z,double red,double green,double blue)
{float p[4];
 int lightno;

  switch(index)
    {case 0 : lightno = GL_LIGHT0;  break;
     case 1 : lightno = GL_LIGHT1;  break;
     case 2 : lightno = GL_LIGHT2;  break;
     case 3 : lightno = GL_LIGHT3;  break;
     case 4 : lightno = GL_LIGHT4;  break;
     case 5 : lightno = GL_LIGHT5;  break;
     case 6 : lightno = GL_LIGHT6;  break;
     case 7 : lightno = GL_LIGHT7;  break;
     default : lightno = GL_LIGHT0;  break;
    }

  lx = x;  ly = y;  lz = z;

  p[0] = (float)x; p[1] = (float)y; p[2] = (float)z; p[3] = 1.0f;
  glLightfv(lightno,GL_POSITION,p);
  p[0] = (float)red; p[1] = (float)green; p[2] = (float)blue; p[3] = 1.0f;
  glLightfv(lightno,GL_AMBIENT,p);
  glLightfv(lightno,GL_DIFFUSE,p);
  glLightf(lightno,GL_SPOT_EXPONENT,20.0f);

  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
  glEnable(lightno);
  glDisable(GL_CULL_FACE);

  return 0;
}

 int SetNoMaterial(int r,int g,int b)
 {
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   glShadeModel(GL_FLAT);
   glDisable(GL_BLEND);

   glColor3ub((GLubyte)r,(GLubyte)g,(GLubyte)b);

   return 0;
 }

int SetNoMaterial(double r,double g,double b)
{

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glShadeModel(GL_FLAT);
  glDisable(GL_BLEND);

  glColor3f((GLfloat)r,(GLfloat)g,(GLfloat)b);

  return 0;
}

int SetNoMaterial(double r, double g, double b, double t)
{

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glShadeModel(GL_FLAT);

  if (t == 0.0)
    glDisable(GL_BLEND);
  else
  {
    glEnable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
  }

  glColor4f((GLfloat)r,(GLfloat)g,(GLfloat)b,(GLfloat)t);

  return 0;
}

int SetMaterial(double r,double g,double b,double a,double d,double s)
{float p[4];

  p[0] = (float)(r*a); p[1] = (float)(g*a); p[2] = (float)(b*a); p[3] = (float)1.0;
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,p);
  p[0] = (float)(r*d); p[1] = (float)(g*d); p[2] = (float)(b*d); p[3] = (float)1.0;
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,p);
  p[0] = (float)s; p[1] = (float)s; p[2] = (float)s; p[3] = (float)1.0;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,p);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10.0);

  glDisable(GL_BLEND);


  return 0;
}

 int SetMaterialEx(double r,double g,double b,double a,double d,double s,double t)
{float p[4];

  glDisable( GL_TEXTURE_2D );

  // transparency 0.0 = transparent 1.0 = opaque
  // OpenGL
  p[0] = (float)(r*a); p[1] = (float)(g*a); p[2] = (float)(b*a); p[3] = (float)t;
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,p);
  p[0] = (float)(r*d); p[1] = (float)(g*d); p[2] = (float)(b*d); p[3] = (float)t;
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,p);
  p[0] = (float)s; p[1] = (float)s; p[2] = (float)s; p[3] = (float)t;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,p);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,10.0);

  if (t == 0.0)
    glDisable(GL_BLEND);
  else
    {  glEnable(GL_BLEND);
       glDisable(GL_ALPHA_TEST);
       //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
       glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }

  return 0;
}

int SetBackground(double r,double g,double b)
{
  /*
  if (GetDeviceCaps(hdc,TECHNOLOGY) != DT_RASPRINTER)        //  Printer surface
    glClearColor((float)r,(float)g,(float)b,1.0f);
  else
    glClearColor(1.0f,1.0f,1.0f,1.0f);
  */
  glClearColor((float)r,(float)g,(float)b,1.0f);

  return 0;
}

static DPoint CurrentEyePoint;
static DPoint CurrentDir;
static DPoint CurrentUp;
static DPoint CurrentRight;

int RCSetViewing(int x,int y,int w,int h,double umin,double vmin,double umax,double vmax,Point eye, Point ref, Point up, double front, double back, int p)
{
    near_plane = front;
    far_plane = back;
    perspective = p;

    if(h == 0)
        h = 1;

    //glViewport(x,y,w,h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye.x,eye.y,eye.z,ref.x,ref.y,ref.z,up.x,up.y,up.z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(perspective)
    {
        double daspect = (double)w/(double)h;
        glOrtho(umin,umax,vmin,vmax,near_plane,far_plane);
        //daspect = (vmax-vmin)/(umax-umin);
        gluPerspective(60.0,1.0,near_plane,far_plane);
    }
    else
    {
        glOrtho(umin,umax,vmin,vmax,near_plane,far_plane);
    }


    return 0;
}

int RCSetView(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int p)
{
    near_plane = front;
    far_plane = back;
    perspective = p;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(xe,ye,ze,xr,yr,zr,xu,yu,zu);

    return 0;
}

int SetView(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int p)
{
#if 0
     GLdouble m[16];
     Point eye,ref,up,dir,right;
     double d;

      eye.x = (double)xe;  eye.y = (double)ye;  eye.z = (double)ze;
      ref.x = (double)xr;  ref.y = (double)yr;  ref.z = (double)zr;
      up.x = (double)xu;  up.y = (double)yu;  up.z = (double)zu;

      CurrentEyePoint.x = eye.x;
      CurrentEyePoint.y = eye.y;
      CurrentEyePoint.z = eye.z;

      dir = eye - ref;
      d = dir.length();

      perspective = p;
      if (perspective)
        {  //near_plane = d - d / (d+front);
           //far_plane = d - d / (d+back);
           near_plane = -front;
           far_plane = -back;
           //near_plane = -(d + front);
           //far_plane = -(d + back);
        }
      else
        {  far_plane = d - front;
           near_plane = d - back;
        }

      dir = dir.normalize();
      right = up.cross(dir);
      up = dir.cross(right);
      right = right.normalize();
      up = up.normalize();

      CurrentDir = dir;
      CurrentUp = up;
      CurrentRight = right;

      m[0] = right.x;
      m[4] = right.y;
      m[8] = right.z;
      m[1] = up.x;
      m[5] = up.y;
      m[9] = up.z;
      m[2] = dir.x;
      m[6] = dir.y;
      m[10] = dir.z;
      m[3] = 0.0;
      m[7] = 0.0;
      m[11] = 0.0;
      m[15] = 1.0;

      m[12] = -right.dot(eye);
      m[13] = -up.dot(eye);
      m[14] = -dir.dot(eye);

      glViewport(windowx,windowy,windowwidth+1,windowheight+1);
      //glViewport(windowx,windowy,windowwidth,windowheight);

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixd(m);
#else
 GLfloat m[16];
 Point eye,ref,up,dir,right; 
 double d;

  eye.x = (float)xe;  eye.y = (float)ye;  eye.z = (float)ze;
  ref.x = (float)xr;  ref.y = (float)yr;  ref.z = (float)zr;
  up.x = (float)xu;  up.y = (float)yu;  up.z = (float)zu;

  CurrentEyePoint.x = eye.x;
  CurrentEyePoint.y = eye.y;
  CurrentEyePoint.z = eye.z;

  dir = eye - ref;
  d = dir.length();

  perspective = p;
  if (perspective)
    {
       //near_plane = d - d / (d+front);
       //far_plane = d - d / (d+back);
       near_plane = -front;
       far_plane = -back;
       //near_plane = -(d + front);
       //far_plane = -(d + back);
    }
  else
    {  far_plane = d - front;  
       near_plane = d - back;
    }

  dir = dir.normalize();
  right = up.cross(dir);
  up = dir.cross(right);
  right = right.normalize();
  up = up.normalize();

  CurrentDir = dir;
  CurrentUp = up;
  CurrentRight = right;

  m[0] = right.x;
  m[4] = right.y;
  m[8] = right.z;
  m[1] = up.x;
  m[5] = up.y;
  m[9] = up.z;
  m[2] = dir.x;
  m[6] = dir.y;
  m[10] = dir.z;
  m[3] = 0.0;
  m[7] = 0.0;
  m[11] = 0.0;
  m[15] = 1.0;
    
  m[12] = -right.dot(eye);
  m[13] = -up.dot(eye);
  m[14] = -dir.dot(eye);

  glViewport(windowx,windowy,windowwidth+1,windowheight+1);
  //glViewport(windowx,windowy,windowwidth,windowheight);

  //gluPerspective(100.0,1.0,1.0,100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(m);
#endif
  return 0;
}

// a more OpenGL type of view port setting
int RCSetViewport(int x,int y,int w,int h,double umin,double vmin,double umax,double vmax,double front,double back,int p)
{
    near_plane = front;
    far_plane = back;
    perspective = p;

    if(h == 0)
        h = 1;

    glViewport(x,y,w,h);

    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(perspective)
    {
        double daspect = (double)w/(double)h;
        //glOrtho(umin,umax,vmin,vmax,near_plane,far_plane);
        //daspect = (vmax-vmin)/(umax-umin);
        gluPerspective(60.0,daspect,0.1,1000.0);
    }
    else
    {
        glOrtho(umin,umax,vmin,vmax,near_plane,far_plane);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 0;
}

int SetViewport(double umin,double vmin,double umax,double vmax)
{
 	glMatrixMode( GL_PROJECTION );

#if 0
    GLdouble m[16];
    if (perspective)
      {
         m[0] = (double)(2.0*near_plane/(umax-umin));           m[1] = 0.0l;                                 m[2] = 0.0f;                                                    m[3]  = 0.0l;
         m[4] = 0.0l;                                          m[5] = (double)(2.0*near_plane/(vmax-vmin));  m[6] = 0.0f;                                                    m[7]  = 0.0l;
         m[8] = (double)((umin+umax)/(umax-umin));              m[9] = (double)((vmax+vmin)/(vmax-vmin));     m[10] = (double)(far_plane/(near_plane-far_plane));              m[11] = -1.0l;
         m[12]  = 0.0l;                                        m[13] = 0.0l;                                m[14] = (double)(far_plane*near_plane/(near_plane-far_plane));   m[15] = 0.0l;

         //m[0] = (float)(2.0*near_plane/(umax-umin));           m[1] = 0.0f;                                 m[2] = 0.0f;                                                        m[3]  = 0.0f;
         //m[4] = 0.0f;                                          m[5] = (float)(2.0*near_plane/(vmax-vmin));  m[6] = 0.0f;                                                        m[7]  = 0.0f;
         //m[8] = (float)((umin+umax)/(umax-umin));              m[9] = (float)((vmax+vmin)/(vmax-vmin));     m[10] = (float)(near_plane+far_plane/(near_plane-far_plane));       m[11] = -1.0f;
         //m[12]  = 0.0f;                                        m[13] = 0.0f;                                m[14] = (float)(2*near_plane*far_plane/(near_plane-far_plane));       m[15] = 0.0f;
    }
    else
      {  m[0] = (double)(2.0/(umax-umin));           m[1] = 0.0l;                               m[2] = 0.0l;                                          m[3]  = 0.0l;
         m[4] = 0.0l;                               m[5] = (double)(2.0/(vmax-vmin));           m[6] = 0.0l;                                          m[7]  = 0.0l;
         m[8] = 0.0l;                               m[9] = 0.0l;                               m[10] = (double)(1.0/(near_plane-far_plane));         m[11] = 0.0l;
         m[12] = (double)((umin+umax)/(umin-umax));  m[13] = (double)((vmax+vmin)/(vmin-vmax));  m[14] = (double)(near_plane/(near_plane-far_plane));  m[15] = 1.0l;
      }


      glLoadMatrixd(m);
#else
    GLfloat m[16];
  if (perspective)
  {
       m[0] = (float)(2.0*near_plane/(umax-umin));           m[1] = 0.0f;                                 m[2] = 0.0f;                                                    m[3]  = 0.0f;
       m[4] = 0.0f;                                          m[5] = (float)(2.0*near_plane/(vmax-vmin));  m[6] = 0.0f;                                                    m[7]  = 0.0f;
       m[8] = (float)((umin+umax)/(umax-umin));              m[9] = (float)((vmax+vmin)/(vmax-vmin));     m[10] = (float)(far_plane/(near_plane-far_plane));              m[11] = -1.0f;
       m[12]  = 0.0f;                                        m[13] = 0.0f;                                m[14] = (float)(far_plane*near_plane/(near_plane-far_plane));   m[15] = 0.0f;

       //m[0] = (float)(2.0*near_plane/(umax-umin));           m[1] = 0.0f;                                 m[2] = 0.0f;                                                        m[3]  = 0.0f;
       //m[4] = 0.0f;                                          m[5] = (float)(2.0*near_plane/(vmax-vmin));  m[6] = 0.0f;                                                        m[7]  = 0.0f;
       //m[8] = (float)((umin+umax)/(umax-umin));              m[9] = (float)((vmax+vmin)/(vmax-vmin));     m[10] = (float)(near_plane+far_plane/(near_plane-far_plane));       m[11] = -1.0f;
       //m[12]  = 0.0f;                                        m[13] = 0.0f;                                m[14] = (float)(2*near_plane*far_plane/(near_plane-far_plane));       m[15] = 0.0f;
  }
  else
  {
       m[0] = (float)(2.0/(umax-umin));           m[1] = 0.0f;                               m[2] = 0.0f;                                          m[3]  = 0.0f;
       m[4] = 0.0f;                               m[5] = (float)(2.0/(vmax-vmin));           m[6] = 0.0f;                                          m[7]  = 0.0f;
       m[8] = 0.0f;                               m[9] = 0.0f;                               m[10] = (float)(1.0/(near_plane-far_plane));         m[11] = 0.0f;
       m[12] = (float)((umin+umax)/(umin-umax));  m[13] = (float)((vmax+vmin)/(vmin-vmax));  m[14] = (float)(near_plane/(near_plane-far_plane));  m[15] = 1.0f;
  }


 	glLoadMatrixf(m);
#endif

  return 0;
}

static int front;
static int firstTime=1;
static int beginOnce;

int QtBeginScene()
{
    //qDebug() << "in QtBeginScene";
    firstTime = 1;
    return firstTime;
}

int QtEndScene()
{
    //qDebug() << "in QtEndScene";
    firstTime = 0;
    return firstTime;
}

int BeginScene(int f)
{

  //if(!firstTime)
  //    return 0;

  glClearDepth(1.0);
  RCCheckGlError();

  //glClearDepth(0.0);
  front = f;

  //GdiFlush();

  if (front & BS_FRONTBUFFERONLY)
    glDrawBuffer(GL_FRONT);
  else
    glDrawBuffer(GL_BACK);
  RCCheckGlError();

  if(firstTime)
  {
      glClear( GL_COLOR_BUFFER_BIT*((f&BS_NOCLEAR) == 0) | GL_DEPTH_BUFFER_BIT );
      RCCheckGlError();
      firstTime = 0;
  }

  //glClear( GL_COLOR_BUFFER_BIT*((f&BS_NOCLEAR) == 0) );
  //glClear( GL_COLOR_BUFFER_BIT*((f&BS_NOCLEAR) == 0) | GL_DEPTH_BUFFER_BIT*((f&BS_IGNOREDEPTH) == 0) );

  if (front & BS_FRONTBUFFERONLY)
    glFinish();

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
  glDisable(GL_LIGHT4);
  glDisable(GL_LIGHT5);;
  glDisable(GL_LIGHT6);
  glDisable(GL_LIGHT7);
  glDisable(GL_CULL_FACE);
  RCCheckGlError();
  glEnable(GL_LIGHTING);
  RCCheckGlError();
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
  RCCheckGlError();

  if (front & BS_IGNOREDEPTH)
    glDisable(GL_DEPTH_TEST);
  else
    glEnable(GL_DEPTH_TEST);
  RCCheckGlError();
  //glDepthFunc(GL_LESS);
  glDepthFunc(GL_LEQUAL);
  RCCheckGlError();
  //glDepthFunc(GL_GEQUAL);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  RCCheckGlError();
  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glShadeModel(GL_SMOOTH);
  RCCheckGlError();

  glClearColor(1.0f,1.0f,1.0f,1.0f);
  RCCheckGlError();

  glFinish();
  RCCheckGlError();

  return 0;
}

int EndScene(void)
{
  int i;

  //GdiFlush();

  if (! front)
      if(hdc)
        ((QGLWidget*)((QGraphicsView*)hdc)->viewport())->swapBuffers();
  else
    glFinish();

  //wglMakeCurrent(0,0);
  /*
  if (GetDeviceCaps(hdc,TECHNOLOGY) == DT_RASPRINTER)        //  Printer surface
    {  wglDeleteContext(hglrc);

       //  Invalidate this cache entry  
      for (i = 0 ; i < nopengldevices ; i++)
      if (OpenGLDeviceCache[i].hglrc == hglrc)
         OpenGLDeviceCache[i].hdc = 0;
    }
  */

  return 0;
}

 int Flush(void)
{ 
  glFinish();
  //GdiFlush();
  return 0;
}

int DrawTriangle(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double nx,double ny,double nz)
{
  //GdiFlush();
  glBegin(GL_TRIANGLES);
#if 1
  glNormal3d(nx,ny,nz);
  glVertex3d(x1,y1,z1);
  glVertex3d(x2,y2,z2);
  glVertex3d(x3,y3,z3);
#else
  glNormal3f(nx,ny,nz);
  glVertex3f(x1,y1,z1);
  glVertex3f(x2,y2,z2);
  glVertex3f(x3,y3,z3);
#endif
  glEnd();

  return 0;

}

/**
 * @brief DrawQuad
 * @param cp
 * @param xaxis
 * @param yaxis
 * @param width
 * @param height
 *
 * draws an filled box
 * the box is centered on the given point snd aligned with the given x and y axis directions
 * uses the current drawing setup
 */
void DrawQuad(Point cp, Point xaxis, Point yaxis, double width, double height)
{
    Point hpt0,hpt1,hpt2,hpt3;
    double w = width * 0.5;
    double h = height * 0.5;

    hpt0 = cp - (xaxis*w) - (yaxis*h);
    hpt1 = cp + (xaxis*w) - (yaxis*h);
    hpt2 = cp + (xaxis*w) + (yaxis*h);
    hpt3 = cp - (xaxis*w) + (yaxis*h);

    glBegin(GL_QUADS);
#if 1
    glVertex3d(hpt0.x,hpt0.y,hpt0.z);
    glVertex3d(hpt1.x,hpt1.y,hpt1.z);
    glVertex3d(hpt2.x,hpt2.y,hpt2.z);
    glVertex3d(hpt3.x,hpt3.y,hpt3.z);
#else
    glVertex3f(hpt0.x,hpt0.y,hpt0.z);
    glVertex3f(hpt1.x,hpt1.y,hpt1.z);
    glVertex3f(hpt2.x,hpt2.y,hpt2.z);
    glVertex3f(hpt3.x,hpt3.y,hpt3.z);
#endif
    glEnd();
}

#define MAX_TRIANGULATED_VERTEX 3000

typedef void (__stdcall *cb)(void);

class TriangulatedPolygon
{public:
  int nvert;
  int nrawvert;
  int type;
  DPoint v[MAX_TRIANGULATED_VERTEX],p1,p2,p3,origin,xaxis,yaxis,zaxis;
  double umin,vmin,umax,vmax;
} TriangulatedPolygon;

void __stdcall TPBegin (GLenum t)
{ TriangulatedPolygon.type = t;
  TriangulatedPolygon.nrawvert = 0;
}

void __stdcall TPEnd (void)
{
}

void __stdcall TPVertex (void * vr)
{static DPoint p1,p2,p3;

  if (TriangulatedPolygon.type == GL_TRIANGLES)  
    {  if (TriangulatedPolygon.nvert < MAX_TRIANGULATED_VERTEX)
         TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = *((DPoint *) vr);
    }
  else if (TriangulatedPolygon.type == GL_TRIANGLE_STRIP)  
    {  p1 = p2;
       p2 = p3;
       p3 = *((DPoint *) vr);
       if (TriangulatedPolygon.nrawvert >= 2 && TriangulatedPolygon.nvert + 3 < MAX_TRIANGULATED_VERTEX)
         {  if (TriangulatedPolygon.nrawvert%2)
              {  TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p3;
                 TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p2;
                 TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p1;
              }
            else
              {  TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p1;
                 TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p2;
                 TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p3;
              }
         }
       TriangulatedPolygon.nrawvert++;
    }
  else if (TriangulatedPolygon.type == GL_TRIANGLE_FAN)  
    {  if (TriangulatedPolygon.nrawvert == 0)
         p1 = *((DPoint *) vr);
       else 
         {  p2 = p3;
            p3 = *((DPoint *) vr);
         }
       if (TriangulatedPolygon.nrawvert >= 2 && TriangulatedPolygon.nvert + 3 < MAX_TRIANGULATED_VERTEX)
         {  TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p1;
            TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p2;
            TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = p3;
         }
       TriangulatedPolygon.nrawvert++;
    }


}

void __stdcall TPCombine (GLdouble coord[3],
                          GLdouble *vertex_data[4],
                          GLfloat weight[4], GLdouble **dataOut )
{
    GLdouble *new_vertex;

    new_vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));

    new_vertex[0] = coord[0];
    new_vertex[1] = coord[1];
    new_vertex[2] = coord[2];

    *dataOut = new_vertex;
}

 int TriangulatePolygon(int nloops,int *nvert,DPoint **vert,DPoint **triangles)
{

 GLUtesselator *tess;
 int i,j;

  TriangulatedPolygon.nvert = 0;

  GdiFlush();

  tess = gluNewTess();
  GLdouble tesswind=0;
  gluGetTessProperty(tess, GLU_TESS_WINDING_RULE, &tesswind);
  //qDebug() << "current winding rule : " << tesswind;
  //if(tesswind == GLU_TESS_WINDING_ODD)
  //    qDebug() << "tess winding is odd";
  //else
  //    qDebug() << "tess winding is NOT odd";
  if(tesswind != GLU_TESS_WINDING_ODD)
    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

  gluTessCallback(tess, GLU_TESS_BEGIN,(cb) TPBegin);
  gluTessCallback(tess, GLU_TESS_VERTEX,(cb)TPVertex);
  gluTessCallback(tess, GLU_TESS_COMBINE,(cb) TPCombine);
  gluTessCallback(tess, GLU_TESS_END,(cb) TPEnd);
  gluTessCallback(tess, GLU_TESS_ERROR,(cb) DPError);

  gluTessBeginPolygon(tess, NULL); 
  for (i = 0 ; i < nloops ; i++)
    {  gluTessBeginContour(tess); 
       for (j = 0 ; j < nvert[i] ; j++)
         gluTessVertex(tess,(double *)&vert[i][j],(double *)&vert[i][j]); 
       gluTessEndContour(tess); 
    }

  gluTessEndPolygon(tess); 
  gluDeleteTess(tess);

  *triangles = TriangulatedPolygon.v;

  return TriangulatedPolygon.nvert/3;
}

void __stdcall DPError (GLenum errorCode)
{
    const GLubyte *estring;

    estring = gluErrorString(errorCode);
    fprintf(stderr,"Tessellation Error: %s\n", estring );
}

void __stdcall DPBegin (GLenum t)
{ TriangulatedPolygon.type = t;
  glBegin(TriangulatedPolygon.type);
  TriangulatedPolygon.nvert = 0;
}

void __stdcall DPEnd (void)
{int i;

  TriangulatedPolygon.zaxis = (TriangulatedPolygon.v[1]-TriangulatedPolygon.v[0]).cross(TriangulatedPolygon.v[2]-TriangulatedPolygon.v[1]).normalize();
  if (TriangulatedPolygon.zaxis.dot(CurrentEyePoint - TriangulatedPolygon.v[0]) > 0.0)
    {  //  Reverse the direction of the polygon so it is drawn correctly

       TriangulatedPolygon.zaxis = -TriangulatedPolygon.zaxis;
       glNormal3dv((double *)&TriangulatedPolygon.zaxis);

       if (TriangulatedPolygon.type == GL_TRIANGLES)  
         {  for (i = TriangulatedPolygon.nvert - 1 ; i >= 0 ; i--)
              {  if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
              }
         }
       else if (TriangulatedPolygon.type == GL_TRIANGLE_STRIP)  
         {  
            for (i = ((TriangulatedPolygon.nvert-1)/2)*2 ; i >= 0 ; i --)
              {  if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
              }
            glEnd();
            glBegin(GL_TRIANGLES);
            if ((TriangulatedPolygon.nvert/2)*2 == TriangulatedPolygon.nvert)
              {  i = TriangulatedPolygon.nvert-2;
                 if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
                 i = TriangulatedPolygon.nvert-1;
                 if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
                 i = TriangulatedPolygon.nvert-3;
                 if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
              }
         }
       else if (TriangulatedPolygon.type == GL_TRIANGLE_FAN)  
         {  i = 0;
            if (texture != 0)
              glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
            glVertex3dv((double *)&TriangulatedPolygon.v[i]);
            for (i = TriangulatedPolygon.nvert - 1 ; i >= 1 ; i--)
              {  if (texture != 0)
                   glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
                 glVertex3dv((double *)&TriangulatedPolygon.v[i]);
              }
         }
    }
  else
    {
       glNormal3dv((double *)&TriangulatedPolygon.zaxis);

       for (i = 0 ; i < TriangulatedPolygon.nvert ; i++)
         {  if (texture != 0)
              glTexCoord2d(TriangulatedPolygon.xaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.umax - TriangulatedPolygon.umin) - TriangulatedPolygon.umin,TriangulatedPolygon.yaxis.dot(TriangulatedPolygon.v[i] - TriangulatedPolygon.origin) / (TriangulatedPolygon.vmax - TriangulatedPolygon.vmin) - TriangulatedPolygon.vmax);
            glVertex3dv((double *)&TriangulatedPolygon.v[i]);
         }
    }


  glEnd();

}

void __stdcall DPVertex (void * vr)
{
    if (TriangulatedPolygon.nvert < MAX_TRIANGULATED_VERTEX)
        TriangulatedPolygon.v[TriangulatedPolygon.nvert++] = *((DPoint *) vr);
}

void __stdcall DPCombine (GLdouble coord[3],
                          GLdouble *vertex_data[4],
                          GLfloat weight[4], GLdouble **dataOut )
{
    GLdouble *new_vertex;

    new_vertex = (GLdouble *) malloc(3 * sizeof(GLdouble));

    new_vertex[0] = coord[0];
    new_vertex[1] = coord[1];
    new_vertex[2] = coord[2];

    *dataOut = new_vertex;
}

int DrawPolygon(DPoint o,DPoint xa,DPoint ya,int nloops,int *nvert,DPoint **vert)
{
  GLUtesselator *tess;
  int i,j;
  DPoint l(lx,ly,lz);
  double u,v;

  //GdiFlush();

  TriangulatedPolygon.origin = o;
  TriangulatedPolygon.xaxis = xa;
  TriangulatedPolygon.yaxis = ya;

  TriangulatedPolygon.zaxis = TriangulatedPolygon.xaxis.cross(TriangulatedPolygon.yaxis);

  if (texture != 0)
    {  TriangulatedPolygon.umin = TriangulatedPolygon.umax = TriangulatedPolygon.xaxis.dot(vert[0][0] - TriangulatedPolygon.origin);
       TriangulatedPolygon.vmin = TriangulatedPolygon.vmax = TriangulatedPolygon.yaxis.dot(vert[0][0] - TriangulatedPolygon.origin);
       for (i = 0 ; i < nloops ; i++)
         for (j = 0 ; j < nvert[i] ; j++)
           {  u = TriangulatedPolygon.xaxis.dot(vert[i][j] - TriangulatedPolygon.origin);
              v = TriangulatedPolygon.yaxis.dot(vert[i][j] - TriangulatedPolygon.origin);
              if (u < TriangulatedPolygon.umin) TriangulatedPolygon.umin = u; 
              if (v < TriangulatedPolygon.vmin) TriangulatedPolygon.vmin = v; 
              if (u > TriangulatedPolygon.umax) TriangulatedPolygon.umax = u; 
              if (v > TriangulatedPolygon.vmax) TriangulatedPolygon.vmax = v; 
           }
    }

  tess = gluNewTess();

  GLdouble tesswind=0;
  gluGetTessProperty(tess, GLU_TESS_WINDING_RULE, &tesswind);
  //qDebug() << "current winding rule : " << tesswind;
  //if(tesswind == GLU_TESS_WINDING_ODD)
  //    qDebug() << "tess winding is odd";
  //else
  //    qDebug() << "tess winding is NOT odd";

  if(tesswind != GLU_TESS_WINDING_ODD)
      gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

  gluTessCallback(tess, GLU_TESS_BEGIN,(cb) DPBegin);  
  gluTessCallback(tess, GLU_TESS_VERTEX,(cb)DPVertex);  
  gluTessCallback(tess, GLU_TESS_END,(cb) DPEnd);  
  gluTessCallback(tess, GLU_TESS_COMBINE,(cb) DPCombine);
  gluTessCallback(tess, GLU_TESS_ERROR,(cb) DPError);

  gluTessBeginPolygon(tess, NULL); 
  for (i = 0 ; i < nloops ; i++)
    {
       gluTessBeginContour(tess);
       for (j = 0 ; j < nvert[i] ; j++)
         gluTessVertex(tess,(double *)&vert[i][j],(double *)&vert[i][j]);
       gluTessEndContour(tess); 
    }
  gluTessEndPolygon(tess); 
  gluDeleteTess(tess);

  return 0;
}

int DrawTexturedTriangle(DPoint *p)
{
  DPoint zaxis;

  //GdiFlush();

  glBegin(GL_TRIANGLES);

  zaxis = (p[1]-p[0]).cross(p[2]-p[1]).normalize();
  if (zaxis.dot(CurrentEyePoint - p[0]) > 0.0)
    {  if (zaxis.dot(p[3]) > 0.0)
         {  p[3] = -p[3];
            p[4] = -p[4];
            p[5] = -p[5];
         }

       glNormal3d(p[5].x,p[5].y,p[5].z);
       glTexCoord2d(p[8].x,p[8].y);
       glVertex3d(p[2].x,p[2].y,p[2].z);

       glNormal3d(p[4].x,p[4].y,p[4].z);
       glTexCoord2d(p[7].x,p[7].y);
       glVertex3d(p[1].x,p[1].y,p[1].z);

       glNormal3d(p[3].x,p[3].y,p[3].z);
       glTexCoord2d(p[6].x,p[6].y);
       glVertex3d(p[0].x,p[0].y,p[0].z);
    }
  else
    {  if (zaxis.dot(p[3]) < 0.0)
         {  p[3] = -p[3];
            p[4] = -p[4];
            p[5] = -p[5];
         }
       glNormal3d(p[3].x,p[3].y,p[3].z);
       glTexCoord2d(p[6].x,p[6].y);
       glVertex3d(p[0].x,p[0].y,p[0].z);
       glNormal3d(p[4].x,p[4].y,p[4].z);
       glTexCoord2d(p[7].x,p[7].y);
       glVertex3d(p[1].x,p[1].y,p[1].z);
       glNormal3d(p[5].x,p[5].y,p[5].z);
       glTexCoord2d(p[8].x,p[8].y);
       glVertex3d(p[2].x,p[2].y,p[2].z);
    }

  glEnd();

  return 0;
}

int DrawTexturedTriangles(int ntriangles,DPoint *p)
{

    DPoint zaxis;
    int i;

    //GdiFlush();

    glBegin(GL_TRIANGLES);

                         
    for (i = 0 ; i < ntriangles ; i++, p += 9)
    {   if (p[0].z == RCDBL_MAX)
            p += 2;   
        zaxis = (p[1]-p[0]).cross(p[2]-p[1]).normalize();
        if (zaxis.dot(CurrentEyePoint - p[0]) > 0.0)
        {   if (zaxis.dot(p[3]) > 0.0)
            {   p[3] = -p[3];
                p[4] = -p[4];
                p[5] = -p[5];
            }

            glNormal3d(p[5].x,p[5].y,p[5].z);
            glTexCoord2d(p[8].x,p[8].y);
            glVertex3d(p[2].x,p[2].y,p[2].z);

            glNormal3d(p[4].x,p[4].y,p[4].z);
            glTexCoord2d(p[7].x,p[7].y);
            glVertex3d(p[1].x,p[1].y,p[1].z);

            glNormal3d(p[3].x,p[3].y,p[3].z);
            glTexCoord2d(p[6].x,p[6].y);
            glVertex3d(p[0].x,p[0].y,p[0].z);
        }
        else
        {   if (zaxis.dot(p[3]) < 0.0)
            {   p[3] = -p[3];
                p[4] = -p[4];
                p[5] = -p[5];
            }
            glNormal3d(p[3].x,p[3].y,p[3].z);
            glTexCoord2d(p[6].x,p[6].y);
            glVertex3d(p[0].x,p[0].y,p[0].z);
            glNormal3d(p[4].x,p[4].y,p[4].z);
            glTexCoord2d(p[7].x,p[7].y);
            glVertex3d(p[1].x,p[1].y,p[1].z);
            glNormal3d(p[5].x,p[5].y,p[5].z);
            glTexCoord2d(p[8].x,p[8].y);
            glVertex3d(p[2].x,p[2].y,p[2].z);

         }
   }

   glEnd();

   return 0;
}

 int OpenOpenGl(RCHDC ohdc,int surfacetype,int x,int y,int width,int height)
{
    /*
    int i;
    PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
//    PFD_SUPPORT_GDI |      // support GDI
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_DOUBLEBUFFER,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    24,                    // 24-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    0,                     // no alpha buffer 
    0,                     // shift bit ignored 
    0,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    32,                    // 32-bit z-buffer 
    0,                     // no stencil buffer 
    0,                     // no auxiliary buffer 
    0,                     // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
  }; 


  GdiFlush();

  windowwidth = width;
  windowheight = height;
  windowx = x;
  windowy = y;

  hdc = ohdc;  

  //  First check to see if this is already opened.
  for (i = 0 ; i < nopengldevices ; i++)
    if (OpenGLDeviceCache[i].hdc == hdc)
      {  hdc = OpenGLDeviceCache[i].hdc;
         hglrc = OpenGLDeviceCache[i].hglrc;
         wglMakeCurrent (hdc, hglrc);

         return 2;  //  return cached device
      }


  if (surfacetype == 1)        //  Printer surface
    pfd.dwFlags = PFD_SUPPORT_GDI | PFD_SUPPORT_OPENGL | PFD_DEPTH_DONTCARE;
  else if (surfacetype == 2)   //  Offscreen bitmap
    pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI | PFD_SUPPORT_OPENGL;
  else if (surfacetype == 3)   //  Doubled window with support for GDI
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_GDI | PFD_SUPPORT_OPENGL;
  else if (surfacetype == 0)  //  Draw on front surface only with GDI support
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_GDI | PFD_SUPPORT_OPENGL;

  //  Allocate or reuse a cache entry
  for (i = 0 ; i < nopengldevices ; i++)
    if (OpenGLDeviceCache[i].hdc == 0)
      break;

  if (i < nopengldevices)
    {//  Release the current device
       wglMakeCurrent(NULL, NULL) ; 
    }
  else 
    {  if (i == MAX_WINDOWS)
         {  i = 0;
            wglMakeCurrent(NULL, NULL) ; 
            wglDeleteContext(OpenGLDeviceCache[i].hglrc); 
         }
       else
         {  if (i == nopengldevices)
              nopengldevices++;
         }
    } 

  OpenGLDeviceCache[i].hdc = ohdc;
  OpenGLDeviceCache[i].windowwidth = windowwidth;
  OpenGLDeviceCache[i].windowheight = windowheight;
  OpenGLDeviceCache[i].windowx = windowx;
  OpenGLDeviceCache[i].windowy = windowy;
 // if (GetDeviceCaps(ohdc,TECHNOLOGY) != DT_RASPRINTER)        //  Printer surface

  SetPixelFormat(hdc,ChoosePixelFormat(hdc,&pfd),&pfd); 

  hglrc = wglCreateContext(hdc); 
  if (hglrc == 0)
    {  pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;
       pfd.cColorBits = 32;
       SetPixelFormat(hdc,ChoosePixelFormat(hdc,&pfd),&pfd); 
       hglrc = wglCreateContext(hdc); 
       if (hglrc == 0)
         {  pfd.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;
            pfd.cColorBits = 16;
            SetPixelFormat(hdc,ChoosePixelFormat(hdc,&pfd),&pfd); 
            hglrc = wglCreateContext(hdc); 
         }  
    }

#if defined(_RCDEBUG)
if (hglrc == 0)
  MessageBox(0,"OpenGL context creation failed.","",MB_OK);
#endif


  OpenGLDeviceCache[i].hglrc = hglrc;

  wglMakeCurrent (hdc, hglrc);

  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
  glEnable(GL_LIGHT0);
  glDisable(GL_CULL_FACE);
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  
  return hglrc != 0;
*/
    int i;

    hdc = ohdc;

    if (hdc == 0)
        return 0;

    windowwidth = width;
    windowheight = height;
    windowx = x;
    windowy = y;

    //qDebug() << "OpenOpenGl start";

    //  First check to see if this is already opened.
    for (i = 0 ; i < nopengldevices ; i++)
      if (OpenGLDeviceCache[i].hdc == hdc)
      {
           hdc = OpenGLDeviceCache[i].hdc;
           hglrc = OpenGLDeviceCache[i].hglrc;
           //wglMakeCurrent (hdc, hglrc);
           if(qobject_cast<QGLWidget*>((QObject*)hglrc))
               ((QGLWidget*)hglrc)->makeCurrent();
           else if(qobject_cast<QOpenGLWidget*>((QObject*)hglrc))
               ((QOpenGLWidget*)hglrc)->makeCurrent();
           // qDebug() << "calling makeCurrent";

            return 2;  //  return cached device
      }

    hglrc = ((QGraphicsView*)hdc)->viewport();

    //  Allocate or reuse a cache entry
    for (i = 0 ; i < nopengldevices ; i++)
      if (OpenGLDeviceCache[i].hdc == 0)
        break;

    if (i < nopengldevices)
    {//  Release the current device
         //wglMakeCurrent(NULL, NULL) ;
         //((QGLWidget*)hglrc)->doneCurrent();
        if(qobject_cast<QGLWidget*>((QObject*)hglrc))
            ((QGLWidget*)hglrc)->doneCurrent();
        else if(qobject_cast<QOpenGLWidget*>((QObject*)hglrc))
            ((QOpenGLWidget*)hglrc)->doneCurrent();
        //qDebug() << "calling doneCurrent 1";
    }
    else
    {
        if (i == MAX_WINDOWS)
        {
            i = 0;
            //wglMakeCurrent(NULL, NULL) ;
            //wglDeleteContext(OpenGLDeviceCache[i].hglrc);
            //((QGLWidget*)hglrc)->doneCurrent();
            if(qobject_cast<QGLWidget*>((QObject*)hglrc))
                ((QGLWidget*)hglrc)->doneCurrent();
            else if(qobject_cast<QOpenGLWidget*>((QObject*)hglrc))
                ((QOpenGLWidget*)hglrc)->doneCurrent();
            //qDebug() << "calling doneCurrent 2";
        }
        else
        {
            if (i == nopengldevices)
                nopengldevices++;
        }
    }

    // init the props
    RCGLInitProps();

    OpenGLDeviceCache[i].hdc = ohdc; // the QGraphicsView
    OpenGLDeviceCache[i].hglrc = hglrc; // the QGLWidget
    OpenGLDeviceCache[i].windowwidth = windowwidth;
    OpenGLDeviceCache[i].windowheight = windowheight;
    OpenGLDeviceCache[i].windowx = windowx;
    OpenGLDeviceCache[i].windowy = windowy;

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    glEnable(GL_LIGHT0);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f,1.0f,1.0f,1.0f);

    //qDebug() << "OpenOpenGl end";

    return hglrc != 0;
}

int ClearOpenGl()
{
    int i;
    GLuint t;


    //  Delete the textures as well
    for (i = 0 ; i < nopengltextures ; i++)
    {
          OpenGLTextureCache[i].hdc = 0;
          t = OpenGLTextureCache[i].texture;
          glDeleteTextures(1,&t);
    }

    for (i = 0 ; i < nopengldevices ; i++)
    {
          OpenGLDeviceCache[i].hdc = 0;
          OpenGLDeviceCache[i].hglrc = 0;
          if (i == nopengldevices - 1)
            nopengldevices--;
    }

    return 0;
}

int CloseOpenGl(HDC hdc)
{
 int i;
 GLuint t;


  //  Delete the textures as well
  for (i = 0 ; i < nopengltextures ; i++)
    if (OpenGLTextureCache[i].hdc == hdc)
    {

         OpenGLTextureCache[i].hdc = 0;
         t = OpenGLTextureCache[i].texture;
         glDeleteTextures(1,&t);
    }

  //  Find the hdc and if it is matched, delete the associated opengl context
  for (i = 0 ; i < nopengldevices ; i++)
    if (OpenGLDeviceCache[i].hdc == hdc)
    {
         //wglMakeCurrent(NULL, NULL) ;
         //wglDeleteContext(OpenGLDeviceCache[i].hglrc);
         OpenGLDeviceCache[i].hdc = 0;
         if (i == nopengldevices - 1)
           nopengldevices--;
    }

  return 0;
}


//int LoadTexture(char *filename,double r,double g,double b,double trange,double op,int *h)
int LoadTexture(RCCHAR *filename,double r,double g,double b,double trange,double op,int *h)
{
  //DIBBitmap dibbitmap;
  int i,ir,ig,ib,irange,iopacity;
  int width=0, height=0;

  *h = -1;

  ir = (int)(r * 255);
  ig = (int)(g * 255);
  ib = (int)(b * 255);
  irange = (int)(trange * 255);
  // rc transparency 0=complete opacity 1=complete transparency
  iopacity = (int)((1.0 - op) * 255);// OpenGL opacity 1=compete opacity 255=complete transparency
  //iopacity = (int)(op * 255);// OpenGL opacity 1=compete opacity 255=complete transparency

  for (i = 0 ; i < nopengltextures ; i++)
    if (strcmp(filename,OpenGLTextureCache[i].filename) == 0 && OpenGLTextureCache[i].hdc == hdc && 
        OpenGLTextureCache[i].red == ir && OpenGLTextureCache[i].green == ig  && OpenGLTextureCache[i].blue == ib &&
        OpenGLTextureCache[i].transparencyrange == irange && OpenGLTextureCache[i].opacity == iopacity)
      {  *h = (int)OpenGLTextureCache[i].texture;
         if (iopacity == (char)10000)
           {
             glDisable(GL_BLEND);
             RCCheckGlError();
              glEnable(GL_ALPHA_TEST);
             RCCheckGlError();
              glAlphaFunc(GL_NOTEQUAL,1.0);
             RCCheckGlError();
           }
         else
           {  glEnable(GL_BLEND);
              RCCheckGlError();
              glDisable(GL_ALPHA_TEST);
              RCCheckGlError();
              glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
              //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
              RCCheckGlError();
           }
         return 0;  //  return cached device
      }

  //  Allocate or reuse a cache entry
  for (i = 0 ; i < nopengltextures ; i++)
    if (OpenGLTextureCache[i].hdc == 0)
      break;

  if (i == MAX_WINDOWS)
    {GLuint t;
       t = OpenGLTextureCache[i].texture;
       glDeleteTextures(1,&t);
       RCCheckGlError();
    } 
  else
    {  if (i == nopengltextures)
         nopengltextures++;
    }
  strcpy(OpenGLTextureCache[i].filename,filename);
  OpenGLTextureCache[i].filedata = 0;
  OpenGLTextureCache[i].hdc = hdc;
  OpenGLTextureCache[i].red = ir;
  OpenGLTextureCache[i].green = ig;
  OpenGLTextureCache[i].blue = ib;
  OpenGLTextureCache[i].transparencyrange = irange;
  OpenGLTextureCache[i].opacity = iopacity;
  OpenGLTextureCache[i].texture = -1;

  glEnable(GL_TEXTURE_2D);
  RCCheckGlError();

  glGenTextures(1,&texture);
  RCCheckGlError();
  glBindTexture(GL_TEXTURE_2D,texture);
  RCCheckGlError();
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  RCCheckGlError();
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  RCCheckGlError();

  if (iopacity == (char)10000)
    {  glDisable(GL_BLEND);
       RCCheckGlError();
       glEnable(GL_ALPHA_TEST);
        RCCheckGlError();
       glAlphaFunc(GL_NOTEQUAL,1.0);
         RCCheckGlError();
    }
  else
    {  glEnable(GL_BLEND);
      RCCheckGlError();
       glDisable(GL_ALPHA_TEST);
      RCCheckGlError();
      glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
      //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      RCCheckGlError();
    }

  int w1;
  //char *rgbap=0,*rgbabuffer=0;
  uchar *rgbap=0,*rgbabuffer=0;
  QImage *qbmi = new QImage(QString(filename));
  if(!qbmi->isNull())
  {
      width = qbmi->width();
      height = qbmi->height();

      *qbmi = qbmi->convertToFormat(QImage::Format_RGB888);
      w1 = ((qbmi->width()*3 + 3)/4)*4; // stride in 32 bit words per scan line
      //rgbabuffer = new char [qbmi->width() * qbmi->height() * 4];
      rgbabuffer = new uchar [qbmi->width() * qbmi->height() * 4];

      // a GL image is upside down so we are inverting the Qt image here
      rgbap = rgbabuffer;
      // points to the last scan line in the image
      //pixel = filedata + (height-1) * width * 4 ;
      //char *pixel = (char*)qbmi->bits() + (qbmi->height()-1) * w1; // last scane line of image
      uchar *pixel = (uchar*)qbmi->bits() + (qbmi->height()-1) * w1; // last scane line of image

      for (int k = 0 ; k < qbmi->height() ; k++)
      {
          // NOTE :
          // getting a scan line from Qt might be faster??
          //pixel = (char*)qbmi->bits() + (qbmi->height()-k-1) * w1;
          pixel = (uchar*)qbmi->bits() + (qbmi->height()-k-1) * w1;
          for (int j = 0 ; j < qbmi->width() ; j++, pixel += 3,rgbap += 4)
          {
              //char *pxd = pixel;
              uchar *pxd = pixel;
              int pr = (int)*pxd; pxd++;
              int pg = (int)*pxd; pxd++;
              int pb = (int)*pxd;
              rgbap[0] = pr;   // red
              rgbap[1] = pg; // green
              rgbap[2] = pb;  // red
              if( irange > 0 &&
                  abs((pr&0xff) - (ir&0xff)) <= irange &&
                  abs((pg&0xff) - (ig&0xff)) <= irange &&
                  abs((pb&0xff) - (ib&0xff)) <= irange)
                    rgbap[3] = 0xff;
                  else
                    rgbap[3] = uchar(iopacity);
          }
      }
      delete qbmi;

      gluBuild2DMipmaps(GL_TEXTURE_2D,4,width,height,
                        GL_RGBA,GL_UNSIGNED_BYTE,rgbabuffer);
      RCCheckGlError();

      glEnable(GL_TEXTURE_2D);
      RCCheckGlError();

      *h = texture;
      OpenGLTextureCache[i].texture = texture;

  }
  /*
  if (dibbitmap.load(filename,ir,ig,ib,irange,iopacity))
    {
       glGenTextures(1,&texture);
       glBindTexture(GL_TEXTURE_2D,texture);
       glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
       glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
       glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
       glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

       if (iopacity == (char)10000)
         {  glDisable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_NOTEQUAL,1.0);
         }
       else
         {  glEnable(GL_BLEND);
            glDisable(GL_ALPHA_TEST);
            glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
//            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
         }

       gluBuild2DMipmaps(GL_TEXTURE_2D, 4,dibbitmap.getwidth(),dibbitmap.getheight(),
                         GL_BGRA_EXT , GL_UNSIGNED_BYTE,dibbitmap.getpbuffer());
       glEnable(GL_TEXTURE_2D);

       *h = texture;
       OpenGLTextureCache[i].texture = texture;

    }
  */


  delete [] rgbabuffer;

  return 0;

}

int LoadTexturePrint(RCCHAR *filename,double r,double g,double b,double trange,double op,int *h)
{
  //DIBBitmap dibbitmap;
  int i,ir,ig,ib,irange,iopacity;
  int width=0, height=0;

  *h = -1;

  ir = (int)(r * 255);
  ig = (int)(g * 255);
  ib = (int)(b * 255);
  irange = (int)(trange * 255);
  // rc transparency 0=complete opacity 1=complete transparency
  //iopacity = (int)((1.0 - op) * 255);// OpenGL opacity 1=compete opacity 255=complete transparency
  iopacity = (int)(op * 255);// OpenGL opacity 1=compete opacity 255=complete transparency

  for (i = 0 ; i < nopengltextures ; i++)
    if (strcmp(filename,OpenGLTextureCache[i].filename) == 0 && OpenGLTextureCache[i].hdc == hdc &&
        OpenGLTextureCache[i].red == ir && OpenGLTextureCache[i].green == ig  && OpenGLTextureCache[i].blue == ib &&
        OpenGLTextureCache[i].transparencyrange == irange && OpenGLTextureCache[i].opacity == iopacity)
      {  *h = (int)OpenGLTextureCache[i].texture;
         if (iopacity == (char)10000)
           {
             glDisable(GL_BLEND);
             RCCheckGlError();
              glEnable(GL_ALPHA_TEST);
             RCCheckGlError();
              glAlphaFunc(GL_NOTEQUAL,1.0);
             RCCheckGlError();
           }
         else
           {  glEnable(GL_BLEND);
              RCCheckGlError();
              glDisable(GL_ALPHA_TEST);
              RCCheckGlError();
              //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
              glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
              RCCheckGlError();
           }
         return 0;  //  return cached device
      }

  //  Allocate or reuse a cache entry
  for (i = 0 ; i < nopengltextures ; i++)
    if (OpenGLTextureCache[i].hdc == 0)
      break;

  if (i == MAX_WINDOWS)
    {GLuint t;
       t = OpenGLTextureCache[i].texture;
       glDeleteTextures(1,&t);
       RCCheckGlError();
    }
  else
    {  if (i == nopengltextures)
         nopengltextures++;
    }
  strcpy(OpenGLTextureCache[i].filename,filename);
  OpenGLTextureCache[i].filedata = 0;
  OpenGLTextureCache[i].hdc = hdc;
  OpenGLTextureCache[i].red = ir;
  OpenGLTextureCache[i].green = ig;
  OpenGLTextureCache[i].blue = ib;
  OpenGLTextureCache[i].transparencyrange = irange;
  OpenGLTextureCache[i].opacity = iopacity;
  OpenGLTextureCache[i].texture = -1;

  glEnable(GL_TEXTURE_2D);
  RCCheckGlError();

  glGenTextures(1,&texture);
  RCCheckGlError();
  glBindTexture(GL_TEXTURE_2D,texture);
  RCCheckGlError();
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  RCCheckGlError();
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  RCCheckGlError();
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  RCCheckGlError();

  if (iopacity == (char)10000)
    {  glDisable(GL_BLEND);
       RCCheckGlError();
       glEnable(GL_ALPHA_TEST);
        RCCheckGlError();
       glAlphaFunc(GL_NOTEQUAL,1.0);
         RCCheckGlError();
    }
  else
    {  glEnable(GL_BLEND);
      RCCheckGlError();
       glDisable(GL_ALPHA_TEST);
      RCCheckGlError();
      //glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      RCCheckGlError();
    }

  int w1;
  //char *rgbap=0,*rgbabuffer=0;
  uchar *rgbap=0,*rgbabuffer=0;
  QImage *qbmi = new QImage(QString(filename));
  if(!qbmi->isNull())
  {
      width = qbmi->width();
      height = qbmi->height();

      *qbmi = qbmi->convertToFormat(QImage::Format_RGB888);
      w1 = ((qbmi->width()*3 + 3)/4)*4; // stride in 32 bit words per scan line
      //rgbabuffer = new char [qbmi->width() * qbmi->height() * 4];
      rgbabuffer = new uchar [qbmi->width() * qbmi->height() * 4];

      // a GL image is upside down so we are inverting the Qt image here
      rgbap = rgbabuffer;
      // points to the last scan line in the image
      //pixel = filedata + (height-1) * width * 4 ;
      //char *pixel = (char*)qbmi->bits() + (qbmi->height()-1) * w1; // last scane line of image
      uchar *pixel = (uchar*)qbmi->bits() + (qbmi->height()-1) * w1; // last scane line of image

      for (int k = 0 ; k < qbmi->height() ; k++)
      {
          // NOTE :
          // getting a scan line from Qt might be faster??
          //pixel = (char*)qbmi->bits() + (qbmi->height()-k-1) * w1;
          pixel = (uchar*)qbmi->bits() + (qbmi->height()-k-1) * w1;
          for (int j = 0 ; j < qbmi->width() ; j++, pixel += 3,rgbap += 4)
          {
              //char *pxd = pixel;
              uchar *pxd = pixel;
              int pr = (int)*pxd; pxd++;
              int pg = (int)*pxd; pxd++;
              int pb = (int)*pxd;
              rgbap[0] = pr;   // red
              rgbap[1] = pg; // green
              rgbap[2] = pb;  // red
              if( irange > 0 &&
                  abs((pr&0xff) - (ir&0xff)) <= irange &&
                  abs((pg&0xff) - (ig&0xff)) <= irange &&
                  abs((pb&0xff) - (ib&0xff)) <= irange)
                    rgbap[3] = 0xff;
                  else
                    rgbap[3] = uchar(iopacity);
          }
      }
      delete qbmi;

      gluBuild2DMipmaps(GL_TEXTURE_2D,4,width,height,
                        GL_RGBA,GL_UNSIGNED_BYTE,rgbabuffer);
      RCCheckGlError();

      glEnable(GL_TEXTURE_2D);
      RCCheckGlError();

      *h = texture;
      OpenGLTextureCache[i].texture = texture;

  }

  delete [] rgbabuffer;

  return 0;

}

//int LoadMemoryTexture(char *filename,char *filedata,int width,int height,double r,double g,double b,double trange,double op,int *h)
int LoadMemoryTexture(RCCHAR *filename,char *filedata,int width,int height,double r,double g,double b,double trange,double op,int *h)
{

  int i,j,k,w1;
  char *pixel,*rgbabuffer,*rgbap;
  int ir,ig,ib,irange,iopacity;

  ir = (int)(r * 255);
  ig = (int)(g * 255);
  ib = (int)(b * 255);
  irange = (int)(trange * 255);
  iopacity = (int)((1.0-op) * 255);
  //iopacity = (int)(op * 255);// OpenGL opacity 1=compete opacity 255=complete transparency

  for (i = 0 ; i < nopengltextures ; i++)
    if ((strcmp(filename,OpenGLTextureCache[i].filename) == 0 && strlen(filename) > 0 || filedata == OpenGLTextureCache[i].filedata && strlen(filename) == 0) && hdc == OpenGLTextureCache[i].hdc &&
        OpenGLTextureCache[i].red == ir && OpenGLTextureCache[i].green == ig  && OpenGLTextureCache[i].blue == ib &&
        OpenGLTextureCache[i].transparencyrange == irange && OpenGLTextureCache[i].opacity == iopacity)
      {  *h = (int)OpenGLTextureCache[i].texture;
         if (iopacity == (char)10000)
           {  glDisable(GL_BLEND);
              glEnable(GL_ALPHA_TEST);
              glAlphaFunc(GL_NOTEQUAL,1.0);
           }
         else
           {  glEnable(GL_BLEND);
              glDisable(GL_ALPHA_TEST);
              glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
              //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
           }
         return 0;  //  return cached device
      }

  //  Allocate or reuse a cache entry
  for (i = 0 ; i < nopengltextures ; i++)
    if (OpenGLTextureCache[i].hdc == 0)
      break;

  if (i == MAX_WINDOWS)
    {GLuint t;
       t = OpenGLTextureCache[i].texture;
       glDeleteTextures(1,&t);
    } 
  else
    {  if (i == nopengltextures)
         nopengltextures++;
    }
  
  strcpy(OpenGLTextureCache[i].filename,filename);
  OpenGLTextureCache[i].filedata = filedata;
  OpenGLTextureCache[i].hdc = hdc;
  OpenGLTextureCache[i].red = ir;
  OpenGLTextureCache[i].green = ig;
  OpenGLTextureCache[i].blue = ib;
  OpenGLTextureCache[i].transparencyrange = irange;
  OpenGLTextureCache[i].opacity = iopacity;

  glEnable(GL_TEXTURE_2D);

  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  if (iopacity == (char)10000)
    {  glDisable(GL_BLEND);
       glEnable(GL_ALPHA_TEST);
       glAlphaFunc(GL_NOTEQUAL,1.0);
    }
  else
    {  glEnable(GL_BLEND);
       glDisable(GL_ALPHA_TEST);
       glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
       //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }

  w1 = ((width*3 + 3)/4)*4; // stride in 32 bit words per scan line
  //rgbabuffer = new char [w1 * height * 4 / 3 + 20];
  rgbabuffer = new char [width * height * 4];

  // a GL image is upside down so we are inverting the Qt image here
  rgbap = rgbabuffer;
  // points to the last scan line in the image
  //pixel = filedata + (height-1) * width * 4 ;
  pixel = filedata + (height-1) * w1; // last scane line of image

  //for (k = height-1 ; k >= 0 ; k--)
  for (k = 0 ; k < height ; k++)
  {
      // NOTE :
      // getting a scan line from Qt might be faster??
      //pixel = filedata + k * width * 4;
      pixel = filedata + (height-k-1) * w1;
      for (j = 0 ; j < width ; j++, pixel += 3,rgbap += 4)
      {
          //memcpy(rgbap,pixel,3);
          //QRgb *pxrgb = (QRgb*)pixel;
          //int pr = qRed(*pxrgb);
          //int pg = qGreen(*pxrgb);
          //int pb = qBlue(*pxrgb);
          char *pxd = pixel;
          int pr = (int)*pxd; pxd++;
          int pg = (int)*pxd; pxd++;
          int pb = (int)*pxd;
          rgbap[0] = pr;   // red
          rgbap[1] = pg; // green
          rgbap[2] = pb;  // red
          //rgbap[3] = 0; // 0 = totaly opaque; 255 = totaly transparent
          if( irange > 0 &&
              abs((pr&0xff) - (ir&0xff)) <= irange &&
              abs((pg&0xff) - (ig&0xff)) <= irange &&
              abs((pb&0xff) - (ib&0xff)) <= irange)
                rgbap[3] = 0xff;
              else
                rgbap[3] = iopacity;
          //rgbap[3] = abs((pixel[0]&0xff) - (ir&0xff)) <= irange &&
          //           abs((pixel[1]&0xff) - (ig&0xff)) <= irange &&
          //           abs((pixel[2]&0xff) - (ib&0xff)) <= irange ? 0xff : iopacity;

      }
      //pixel -= width * 8; // 2 scan lines up
  }

  gluBuild2DMipmaps(GL_TEXTURE_2D,4,width,height,
                    GL_RGBA,GL_UNSIGNED_BYTE,rgbabuffer);

  glEnable(GL_TEXTURE_2D);

  *h = texture;
  OpenGLTextureCache[i].texture = texture;

  delete [] rgbabuffer;

  return 0;
  
} 


 int SetTexture(long t)
{ texture = t;
  if (texture == 0) 
    glDisable(GL_TEXTURE_2D);
  else
    {  glEnable( GL_TEXTURE_2D );
       RCCheckGlError();
       glBindTexture(GL_TEXTURE_2D,texture);
       RCCheckGlError();
    }
  return 0;
}

 int ReleaseTextures(RCHDC hdc)
{

  int i;
  GLuint t;

  for (i = 0 ; i < nopengltextures ; i++)
    if (hdc == OpenGLTextureCache[i].hdc)
    {
         t = OpenGLTextureCache[i].texture;
         glDeleteTextures(1,&t);
         OpenGLTextureCache[i].hdc = 0;
    }

  //wglMakeCurrent(NULL,NULL);
  for (i = 0 ; i < MAX_WINDOWS ; i++)
    if (OpenGLDeviceCache[i].hdc == hdc)  
    {
          //wglDeleteContext(OpenGLDeviceCache[i].hglrc);
          OpenGLDeviceCache[i].hdc = 0;
    }

  return 1;
}

int ReleaseTextures()
{

  int i;
  GLuint t;

  for (i = 0 ; i < nopengltextures ; i++)
    {
         t = OpenGLTextureCache[i].texture;
         glDeleteTextures(1,&t);
         OpenGLTextureCache[i].hdc = 0;
    }

  return 1;
}


 /**
 * @brief DrawWideLine
 * @param p1
 * @param p2
 * @param w = line width in mm
 *
 * Draw a thick line in screen space using shaded triangles
 *
 */
void DrawThickLine(DPoint p1, DPoint p2, double w)
{
    // get the normal vector to the line
    View3dWindow *vs = ((RCView*)((RCGraphicsView*)hdc)->parent())->getView3dWindow();
    double sc = vs->getscale();
    double ps = vs->getpixelscale();
    ps = pixelmm;
    Point ldir = p2-p1;
    Point nldir = ldir.normalize();
    Point out = vs->getwaxis();
    double llen = ldir.length();
    double a = nldir.dot(out);
    Point lnorm = nldir.cross(vs->getwaxis()).normalize();
    Point edir = lnorm.cross(vs->getwaxis()).normalize();
    double tol = 1.0e-5*1.0e-5*vs->getwaxis().length();
    double lnormlen = lnorm.length();
    if(edir.length() < 1.0e-5)
    {
        lnorm = -ldir.cross(vs->getuaxis()).normalize();
        edir  = ldir.cross(lnorm).normalize();
    }
    else
    {
        lnorm = -lnorm.normalize();
        edir  = lnorm.cross(vs->getwaxis()).normalize();
    }

    double halfwidth = w / 2.0 * ps;

    // shorten by the half width
    //p1 = p1 + ldir * halfwidth;
    //p2 = p2 - ldir * halfwidth;

    // make the points for the line shape
    // two triangles using 4 points for the line part
    Point p3,p4,p5,p6;

    // start offset points
    p3 = p1 - lnorm * halfwidth;
    p5 = p1 + lnorm * halfwidth;

    // end offset points
    p4 = p2 - lnorm * halfwidth;
    p6 = p2 + lnorm * halfwidth;

    // draw the triangle strip
    // for debugging
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //glLineWidth(1.0);
    //
    glBegin(GL_TRIANGLE_STRIP);
#if 1
    glVertex3d(p3.x,p3.y,p3.z);
    glVertex3d(p4.x,p4.y,p4.z);
    glVertex3d(p5.x,p5.y,p5.z);
    glVertex3d(p6.x,p6.y,p6.z);
#else
    glVertex3f(p3.x,p3.y,p3.z);
    glVertex3f(p4.x,p4.y,p4.z);
    glVertex3f(p5.x,p5.y,p5.z);
    glVertex3f(p6.x,p6.y,p6.z);
#endif
    glEnd();

    // draw the points on the ends
    // version 1: 4 triangles  on each end
    // get the radius point along the line direction
    // end not flat to screen
    //Point ep2 = p1 - ldir * halfwidth;
    //Point ep3 = p1 - (ldir + lnorm).normalize() * halfwidth;
    //Point ep1 = p1 - (ldir + -lnorm).normalize() * halfwidth;
    // end flat to screen
    //Point edir = lnorm.cross(CurrentDir).normalize();
    Point ep2 = p1 - edir * halfwidth;
    Point ep3 = p1 - (edir + lnorm).normalize() * halfwidth;
    Point ep1 = p1 - (edir + -lnorm).normalize() * halfwidth;

    // draw the trangle fan
    glBegin(GL_TRIANGLE_FAN);
#if 1
    glVertex3d(p1.x,p1.y,p1.z);
    glVertex3d(p5.x,p5.y,p5.z);
    glVertex3d(ep1.x,ep1.y,ep1.z);
    glVertex3d(ep2.x,ep2.y,ep2.z);
    glVertex3d(ep3.x,ep3.y,ep3.z);
    glVertex3d(p3.x,p3.y,p3.z);
#else
    glVertex3f(p1.x,p1.y,p1.z);
    glVertex3f(p5.x,p5.y,p5.z);
    glVertex3f(ep1.x,ep1.y,ep1.z);
    glVertex3f(ep2.x,ep2.y,ep2.z);
    glVertex3f(ep3.x,ep3.y,ep3.z);
    glVertex3f(p3.x,p3.y,p3.z);
#endif
    glEnd();
    // end not flat to screen
    //ep2 = p2 + ldir * halfwidth;
    //ep3 = p2 + (ldir + lnorm).normalize() * halfwidth;
    //ep1 = p2 + (ldir + -lnorm).normalize() * halfwidth;
    // end flat to screen
    ep2 = p2 + edir * halfwidth;
    ep3 = p2 + (edir + lnorm).normalize() * halfwidth;
    ep1 = p2 + (edir + -lnorm).normalize() * halfwidth;
    //
    glBegin(GL_TRIANGLE_FAN);
#if 1
    glVertex3d(p2.x,p2.y,p2.z);
    glVertex3d(p4.x,p4.y,p4.z);
    glVertex3d(ep1.x,ep1.y,ep1.z);
    glVertex3d(ep2.x,ep2.y,ep2.z);
    glVertex3d(ep3.x,ep3.y,ep3.z);
    glVertex3d(p6.x,p6.y,p6.z);
#else
    glVertex3f(p2.x,p2.y,p2.z);
    glVertex3f(p4.x,p4.y,p4.z);
    glVertex3f(ep1.x,ep1.y,ep1.z);
    glVertex3f(ep2.x,ep2.y,ep2.z);
    glVertex3f(ep3.x,ep3.y,ep3.z);
    glVertex3f(p6.x,p6.y,p6.z);
#endif
    glEnd();
    // for debugging
    //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

 /**
 * @brief DrawLine
 * @param p1
 * @param p2
 * @param w = RealCAD GL width value
 * @return
 *
 *
 * Draw a line using OpenGL
 * changes the line drawing method depending on the current line width
 *
 */
int DrawLine(DPoint p1, DPoint p2, double w)
{
    // get current gl width
    //GLfloat dfwidth,dpsize,fwidth,gwidth;
    //glGetFloatv(GL_LINE_WIDTH,&dfwidth);
    //glGetFloatv(GL_POINT_SIZE,&dpsize);
    //glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE,GLProps.falwidths);

#if 0
    fwidth = w;
    if(fwidth < 5.0)
    {
        // draw OpenGL lines
        fwidth = w-1;
        if(fwidth >= GLProps.falwidths[1])
            fwidth = GLProps.falwidths[1]-2;
        else
            fwidth -= 2;

        // adjust the width for thick OpenGL lines
        if(fwidth > 5.0 )
        {
            double dy = fabs(p2.y-p1.y);
            double dx = fabs(p2.x-p1.x);
            gwidth = fwidth;
            if(dy > 0.001 && dx > 0.001)
            {
                double h = sqrt((fwidth*fwidth)+(fwidth*fwidth));
                double m = dy/dx;
                if(dy > dx)
                    m = dx/dy;
                gwidth = (fwidth + (h - fwidth) * m);
            }
            if(gwidth > GLProps.falwidths[1])
                gwidth = GLProps.falwidths[1];

            glLineWidth(gwidth);
        }

        // draw using opengl lines
        glBegin(GL_LINES);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glEnd();
    }
    else // draw nice thick lines
    {
        DrawThickLine(p1, p2, fwidth);
    }
#else
    // anything less than 6 (0.5mm)
    // is draw using gl lines
    if(w < 7)
    {
        // draw using opengl lines
        glBegin(GL_LINES);
        glVertex3d(p1.x,p1.y,p1.z);
        glVertex3d(p2.x,p2.y,p2.z);
        glEnd();
    }
    else
        DrawThickLine(p1, p2, w);
#endif

#if 0
    // if the width > 1 add a smooth point to the ends
    if(fwidth > 1.0)
    {
        GLfloat cpsize = dpsize;
        glEnable(GL_POINT_SMOOTH);
        if(gwidth > GLProps.falwidths[1])
            cpsize = GLProps.falwidths[1]-2;
        else if(gwidth > fwidth)
            cpsize = gwidth-2;
        else
            cpsize = fwidth-2;
        qDebug() << "setting gl point size : " << cpsize;
        glPointSize(cpsize);
        glGetFloatv(GL_POINT_SIZE,&cpsize);
        qDebug() << "current gl point size : " << cpsize;
        glBegin(GL_POINTS);
        glVertex3f(p1.x,p1.y,p1.z);
        glVertex3f(p2.x,p2.y,p2.z);
        glEnd();
        glDisable(GL_POINT_SMOOTH);
    }
    else
        qDebug() << "not setting gl point size : ";
#endif

    //glPointSize(dpsize);
    //(dfwidth);

    return 1;
}

int DrawLineStrip( Point *points, int npoints, double w )
{
    if( w < 5.0 )
    {
        glBegin(GL_LINE_STRIP);
        for(int i=0; i<npoints; i++)
        {
            glVertex3d(points[i].x,points[i].y,points[i].z);
        }
        glEnd();
    }
    else
    {
        for(int i=0; i<npoints-1; i++)
        {
            DrawThickLine(points[i], points[i+1], w);
        }
    }

    return 1;
}

int DrawLineLoop( Point *points, int npoints, double w )
{
    if( w < 5.0 )
    {
        glBegin(GL_LINE_LOOP);
        for(int i=0; i<npoints; i++)
        {
            glVertex3d(points[i].x,points[i].y,points[i].z);
        }
        glEnd();
    }
    else
    {
        for(int i=0; i<=npoints; i++)
        {
            DrawThickLine(points[i], points[i+1], w);
        }
        DrawThickLine(points[npoints-1], points[0], w);
    }

    return 1;
}

void DrawRBRect(int x1, int y1, int x2, int y2)
{
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    glColor4f(1.0f,1.0f,1.0f,1.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glRecti(x1,windowheight - y1, x2,windowwidth - y2);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_COLOR_LOGIC_OP);
}

void setGLpixelMM(double ps)
{
    pixelmm = ps;
}

double setGLpixelMM(void)
{
    return pixelmm;
}

void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
{
  resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
  resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
  resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
  resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

//This code comes directly from GLU except that it is for float
int glhInvertMatrixf2(float *m, float *out)
{
 float wtmp[4][8];
 float m0, m1, m2, m3, s;
 float *r0, *r1, *r2, *r3;
 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
 r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
    r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
    r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
    r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
    r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
    r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
    r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
    r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
    r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
    r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
    r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
    r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
 /* choose pivot - or die */
 if (fabsf(r3[0]) > fabsf(r2[0]))
    SWAP_ROWS_FLOAT(r3, r2);
 if (fabsf(r2[0]) > fabsf(r1[0]))
    SWAP_ROWS_FLOAT(r2, r1);
 if (fabsf(r1[0]) > fabsf(r0[0]))
    SWAP_ROWS_FLOAT(r1, r0);
 if (0.0 == r0[0])
    return 0;
 /* eliminate first variable     */
 m1 = r1[0] / r0[0];
 m2 = r2[0] / r0[0];
 m3 = r3[0] / r0[0];
 s = r0[1];
 r1[1] -= m1 * s;
 r2[1] -= m2 * s;
 r3[1] -= m3 * s;
 s = r0[2];
 r1[2] -= m1 * s;
 r2[2] -= m2 * s;
 r3[2] -= m3 * s;
 s = r0[3];
 r1[3] -= m1 * s;
 r2[3] -= m2 * s;
 r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) {
    r1[4] -= m1 * s;
    r2[4] -= m2 * s;
    r3[4] -= m3 * s;
 }
 s = r0[5];
 if (s != 0.0) {
    r1[5] -= m1 * s;
    r2[5] -= m2 * s;
    r3[5] -= m3 * s;
 }
 s = r0[6];
 if (s != 0.0) {
    r1[6] -= m1 * s;
    r2[6] -= m2 * s;
    r3[6] -= m3 * s;
 }
 s = r0[7];
 if (s != 0.0) {
    r1[7] -= m1 * s;
    r2[7] -= m2 * s;
    r3[7] -= m3 * s;
 }
 /* choose pivot - or die */
 if (fabsf(r3[1]) > fabsf(r2[1]))
    SWAP_ROWS_FLOAT(r3, r2);
 if (fabsf(r2[1]) > fabsf(r1[1]))
    SWAP_ROWS_FLOAT(r2, r1);
 if (0.0 == r1[1])
    return 0;
 /* eliminate second variable */
 m2 = r2[1] / r1[1];
 m3 = r3[1] / r1[1];
 r2[2] -= m2 * r1[2];
 r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3];
 r3[3] -= m3 * r1[3];
 s = r1[4];
 if (0.0 != s) {
    r2[4] -= m2 * s;
    r3[4] -= m3 * s;
 }
 s = r1[5];
 if (0.0 != s) {
    r2[5] -= m2 * s;
    r3[5] -= m3 * s;
 }
 s = r1[6];
 if (0.0 != s) {
    r2[6] -= m2 * s;
    r3[6] -= m3 * s;
 }
 s = r1[7];
 if (0.0 != s) {
    r2[7] -= m2 * s;
    r3[7] -= m3 * s;
 }
 /* choose pivot - or die */
 if (fabsf(r3[2]) > fabsf(r2[2]))
    SWAP_ROWS_FLOAT(r3, r2);
 if (0.0 == r2[2])
    return 0;
 /* eliminate third variable */
 m3 = r3[2] / r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
    r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3])
    return 0;
 s = 1.0 / r3[3];		/* now back substitute row 3 */
 r3[4] *= s;
 r3[5] *= s;
 r3[6] *= s;
 r3[7] *= s;
 m2 = r2[3];			/* now back substitute row 2 */
 s = 1.0 / r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
    r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
    r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
    r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
 m1 = r1[2];			/* now back substitute row 1 */
 s = 1.0 / r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
    r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
    r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
 m0 = r0[1];			/* now back substitute row 0 */
 s = 1.0 / r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
    r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
 MAT(out, 0, 0) = r0[4];
 MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
 MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
 MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
 MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
 MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
 MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
 MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
 MAT(out, 3, 3) = r3[7];
 return 1;
}

int glhInvertMatrixd2(double *m, double *out)
{
 double wtmp[4][8];
 double m0, m1, m2, m3, s;
 double *r0, *r1, *r2, *r3;
 r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
 r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
    r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
    r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
    r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
    r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
    r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
    r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
    r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
    r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
    r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
    r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
    r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
 /* choose pivot - or die */
 if (fabsf(r3[0]) > fabsf(r2[0]))
    SWAP_ROWS_DOUBLE(r3, r2);
 if (fabsf(r2[0]) > fabsf(r1[0]))
    SWAP_ROWS_DOUBLE(r2, r1);
 if (fabsf(r1[0]) > fabsf(r0[0]))
    SWAP_ROWS_DOUBLE(r1, r0);
 if (0.0 == r0[0])
    return 0;
 /* eliminate first variable     */
 m1 = r1[0] / r0[0];
 m2 = r2[0] / r0[0];
 m3 = r3[0] / r0[0];
 s = r0[1];
 r1[1] -= m1 * s;
 r2[1] -= m2 * s;
 r3[1] -= m3 * s;
 s = r0[2];
 r1[2] -= m1 * s;
 r2[2] -= m2 * s;
 r3[2] -= m3 * s;
 s = r0[3];
 r1[3] -= m1 * s;
 r2[3] -= m2 * s;
 r3[3] -= m3 * s;
 s = r0[4];
 if (s != 0.0) {
    r1[4] -= m1 * s;
    r2[4] -= m2 * s;
    r3[4] -= m3 * s;
 }
 s = r0[5];
 if (s != 0.0) {
    r1[5] -= m1 * s;
    r2[5] -= m2 * s;
    r3[5] -= m3 * s;
 }
 s = r0[6];
 if (s != 0.0) {
    r1[6] -= m1 * s;
    r2[6] -= m2 * s;
    r3[6] -= m3 * s;
 }
 s = r0[7];
 if (s != 0.0) {
    r1[7] -= m1 * s;
    r2[7] -= m2 * s;
    r3[7] -= m3 * s;
 }
 /* choose pivot - or die */
 if (fabsf(r3[1]) > fabsf(r2[1]))
    SWAP_ROWS_DOUBLE(r3, r2);
 if (fabsf(r2[1]) > fabsf(r1[1]))
    SWAP_ROWS_DOUBLE(r2, r1);
 if (0.0 == r1[1])
    return 0;
 /* eliminate second variable */
 m2 = r2[1] / r1[1];
 m3 = r3[1] / r1[1];
 r2[2] -= m2 * r1[2];
 r3[2] -= m3 * r1[2];
 r2[3] -= m2 * r1[3];
 r3[3] -= m3 * r1[3];
 s = r1[4];
 if (0.0 != s) {
    r2[4] -= m2 * s;
    r3[4] -= m3 * s;
 }
 s = r1[5];
 if (0.0 != s) {
    r2[5] -= m2 * s;
    r3[5] -= m3 * s;
 }
 s = r1[6];
 if (0.0 != s) {
    r2[6] -= m2 * s;
    r3[6] -= m3 * s;
 }
 s = r1[7];
 if (0.0 != s) {
    r2[7] -= m2 * s;
    r3[7] -= m3 * s;
 }
 /* choose pivot - or die */
 if (fabsf(r3[2]) > fabsf(r2[2]))
    SWAP_ROWS_DOUBLE(r3, r2);
 if (0.0 == r2[2])
    return 0;
 /* eliminate third variable */
 m3 = r3[2] / r2[2];
 r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
    r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
 /* last check */
 if (0.0 == r3[3])
    return 0;
 s = 1.0 / r3[3];		/* now back substitute row 3 */
 r3[4] *= s;
 r3[5] *= s;
 r3[6] *= s;
 r3[7] *= s;
 m2 = r2[3];			/* now back substitute row 2 */
 s = 1.0 / r2[2];
 r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
    r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
 m1 = r1[3];
 r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
    r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
 m0 = r0[3];
 r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
    r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
 m1 = r1[2];			/* now back substitute row 1 */
 s = 1.0 / r1[1];
 r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
    r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
 m0 = r0[2];
 r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
    r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
 m0 = r0[1];			/* now back substitute row 0 */
 s = 1.0 / r0[0];
 r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
    r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
 MAT(out, 0, 0) = r0[4];
 MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
 MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
 MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
 MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
 MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
 MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
 MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
 MAT(out, 3, 3) = r3[7];
 return 1;
}

void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
{
  result[0]=matrix1[0]*matrix2[0]+
    matrix1[4]*matrix2[1]+
    matrix1[8]*matrix2[2]+
    matrix1[12]*matrix2[3];
  result[4]=matrix1[0]*matrix2[4]+
    matrix1[4]*matrix2[5]+
    matrix1[8]*matrix2[6]+
    matrix1[12]*matrix2[7];
  result[8]=matrix1[0]*matrix2[8]+
    matrix1[4]*matrix2[9]+
    matrix1[8]*matrix2[10]+
    matrix1[12]*matrix2[11];
  result[12]=matrix1[0]*matrix2[12]+
    matrix1[4]*matrix2[13]+
    matrix1[8]*matrix2[14]+
    matrix1[12]*matrix2[15];
  result[1]=matrix1[1]*matrix2[0]+
    matrix1[5]*matrix2[1]+
    matrix1[9]*matrix2[2]+
    matrix1[13]*matrix2[3];
  result[5]=matrix1[1]*matrix2[4]+
    matrix1[5]*matrix2[5]+
    matrix1[9]*matrix2[6]+
    matrix1[13]*matrix2[7];
  result[9]=matrix1[1]*matrix2[8]+
    matrix1[5]*matrix2[9]+
    matrix1[9]*matrix2[10]+
    matrix1[13]*matrix2[11];
  result[13]=matrix1[1]*matrix2[12]+
    matrix1[5]*matrix2[13]+
    matrix1[9]*matrix2[14]+
    matrix1[13]*matrix2[15];
  result[2]=matrix1[2]*matrix2[0]+
    matrix1[6]*matrix2[1]+
    matrix1[10]*matrix2[2]+
    matrix1[14]*matrix2[3];
  result[6]=matrix1[2]*matrix2[4]+
    matrix1[6]*matrix2[5]+
    matrix1[10]*matrix2[6]+
    matrix1[14]*matrix2[7];
  result[10]=matrix1[2]*matrix2[8]+
    matrix1[6]*matrix2[9]+
    matrix1[10]*matrix2[10]+
    matrix1[14]*matrix2[11];
  result[14]=matrix1[2]*matrix2[12]+
    matrix1[6]*matrix2[13]+
    matrix1[10]*matrix2[14]+
    matrix1[14]*matrix2[15];
  result[3]=matrix1[3]*matrix2[0]+
    matrix1[7]*matrix2[1]+
    matrix1[11]*matrix2[2]+
    matrix1[15]*matrix2[3];
  result[7]=matrix1[3]*matrix2[4]+
    matrix1[7]*matrix2[5]+
    matrix1[11]*matrix2[6]+
    matrix1[15]*matrix2[7];
  result[11]=matrix1[3]*matrix2[8]+
    matrix1[7]*matrix2[9]+
    matrix1[11]*matrix2[10]+
    matrix1[15]*matrix2[11];
  result[15]=matrix1[3]*matrix2[12]+
    matrix1[7]*matrix2[13]+
    matrix1[11]*matrix2[14]+
    matrix1[15]*matrix2[15];
}

int GLhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
{
      //Transformation matrices
      float m[16], A[16];
      float in[4], out[4];
      //Calculation for inverting a matrix, compute projection x modelview
      //and store in A[16]
      MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
      //Now compute the inverse of matrix A
      if(glhInvertMatrixf2(A, m)==0)
         return 0;
      //Transformation of normalized coordinates between -1 and 1
      in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0-1.0;
      in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0-1.0;
      in[2]=2.0*winz-1.0;
      in[3]=1.0;
      //Objects coordinates
      MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
      if(out[3]==0.0)
         return 0;
      out[3]=1.0/out[3];
      objectCoordinate[0]=out[0]*out[3];
      objectCoordinate[1]=out[1]*out[3];
      objectCoordinate[2]=out[2]*out[3];
      return 1;
}


Point GLScreenToModel(Point p)
{
    float modelview[16],projection[16];
    float objectcoordinates[3];
    int viewport[4];

    glGetFloatv(GL_MODELVIEW_MATRIX,modelview);
    glGetFloatv(GL_PROJECTION_MATRIX,projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    float pixelz;
    glReadPixels(p.x,p.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&pixelz);

    if(GLhUnProjectf(p.x,p.y,pixelz,modelview,projection,viewport,objectcoordinates))
    {
        Point mp(objectcoordinates[0],objectcoordinates[1],objectcoordinates[2]);
        return mp;
    }
    else
        return Point(0,0,0);
}
