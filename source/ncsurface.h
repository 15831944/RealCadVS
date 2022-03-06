#ifndef NCSURFACE_H
#define NCSURFACE_H

class Surface
  {protected:
     HWND hWnd;
     HDC hDC;
     HPEN horiginalpen;
     RCCOLORREF *colourindex,background,fillcolourref,colourref;
     double scale,plotscale,width,height,originx,originy,angle,xdotspermm,ydotspermm,widthmm,heightmm,clipx1,clipy1,clipx2,clipy2;
     double a1,b1,c1,a2,b2,c2,lastx,lasty;
     int newhdc,transform,drawmode,colour,style,weight,allocatedpens,blackandwhite,bandoffset;
     LineStyle *linestyle;
     PatternLineStyle *patternlinestyle;

     //  Stroked Export Values
     int fileformat;
     int nstart,nglobal,nparameter,ndirectory;
     FILE *outfile;
   public:
     Surface();
     ~Surface();
     void setscale(double s)  {  scale = s;  }
     double getscale(void)  {  return scale;  }
     void setbandoffset(int bo)  {  bandoffset = bo;  }
     double getbandoffset(void)  {  return bandoffset;  }
     double getplotscale(void);
     void setplotscale(double s);
     int openhpgl(char *);
     void closehpgl(void);
     int openiges(char *);
     void closeiges(void);
     void setfileformat(int ff)  {  fileformat = ff;  ndirectory = nparameter = 0;  }
     void sethdc(HWND,HDC);
     void sethwnd(HWND);
     void freehdc(void);
     void allocatepens(int);
     HDC gethdc(void)  {  return hDC;  }
     HWND gethwnd(void)  {  return hWnd;  }
     double getwidth(void)  {  return width;  }
     double getheight(void)  {  return height;  }
     double getoriginx(void)  {  return originx;  }
     double getoriginy(void)  {  return originy;  }
     double getwidthmm(void)  {  return width / xdotspermm;  }
     double getheightmm(void)  {  return height / ydotspermm;  }
     double getxdotspermm(void)  {  return xdotspermm;  }
     double getydotspermm(void)  {  return ydotspermm;  }
     void setxdotspermm(double xdpmm)  {  xdotspermm = xdpmm;  }
     void setydotspermm(double ydpmm)  {  ydotspermm = ydpmm;  }
     void setrotate(void);
     void setbackground(RCCOLORREF bg)  {  background = bg;  }
     RCCOLORREF getbackground()  {  return background;  }
     RCCOLORREF getcolour()  {  return colourref;  }
     virtual void setup(double linestylescale,double length,int,int,int,int,PatternLineStyle *);
     void setcolour(int c,int dm);
     void setfillcolour(RCCOLORREF cr);
     void setstyle(double linestylescale,double length,int s,int dm);
     void setweight(int w,int dm);
     void line(double,double,double,double,int alreadytransformed = 0);
     void fill(Point **,int *nvert,int nloops,int alreadytransformed = 0);
     void dot(double,double);
     void setclip(double,double,double,double);
     int clip2d(double *,double *,double *,double *);
     void moveto(double,double);
     void lineto(double,double);
     void textout(int t,double x,double y,char *s,int l);
     void sizeevent(double,double);
     void sendsizeevent(int,int);
     void settransform2d(double,double,double,double,double,double);
     void gettransform2d(double *,double *,double *,double *,double *,double *);
     int gettransform2d(void)  {  return transform;  }
     double getrotation2d(void);
     void transform2d(double *,double *);
     void cleartransform(void);
     int saveimage(int,const char *,int quality);
#if defined(_WIN32_WCE)
     int saveimage(int,const TCHAR *,int quality);
#endif
     int getblackandwhite(void)  {  return blackandwhite;  }
     void setblackandwhite(int baw)  {  blackandwhite = baw;  }
  };

#endif // NCSURFACE_H
