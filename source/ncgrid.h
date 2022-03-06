
#if ! defined(__Grid)
#define __Grid


class View3dSurface;

enum CursorPlane {  onworkplane,onviewplane  };

class Grid
  {
#ifdef USING_WIDGETS
public:
    // pointers to graphics items for the handles of the line
    // null if not drawn in the scene
    QGraphicsItem *qgiGrid; // Qt graphics item 1
    QGraphicsItem *qgiRef; // Qt graphics item 1

    void draw(int drawmode,View3dSurface *window, QPainter *painter);
    void drawGL(int drawmode,View3dSurface *window);

    int getStyle() { return style; }
    void setStyle(int s) { style=short(s); }
    int getCustColour() { return custcolour; }
    void setCustColour(int cc) { custcolour=short(cc); }
#endif
private:
     friend class Workplanes;
     double xspacing,yspacing,angle,cosa,sina;
     short xreference,yreference,snap,plane,display,referenceonly;
#ifdef USING_WIDGETS
     short style; // dots=0; lines=1
     short custcolour; // standard colour=0; custom colour=1
#endif
   public:
     Grid();
     void setup(void);
     Point snappoint(Point);
     Point snappoint(View3dSurface *,Point);
     void draw(int,View3dSurface *);
     int getdisplay(void)  {  return display;  }
     void setdisplay(int);
     int getplane(void)  {  return plane;  }
     void setplane(int);
     int getsnap(void)  {  return snap;  }
     void setsnap(int s)  {  snap = short(s);  }
     void clear(void);
     void initialize(void);
     int save(CadFile *);
     static int load(CadFile *,Database *);
     void setxspacing(double xs)  {  xspacing = xs;  }
     void setyspacing(double ys)  {  yspacing = ys;  }
     void setangle(double a)  {  angle = a;  }
     void setxreference(int xr)  {  xreference = xr;  }
     void setyreference(int yr)  {  yreference = yr;  }
#ifdef USING_WIDGETS
     void setoptions(int s,int p,int d,int r,int st=0, int ccol=0)  {  snap = s;  plane = p;  display = d;  referenceonly = r; style = st; custcolour=ccol;  }
#else
     void setoptions(int s,int p,int d,int r)  {  snap = s;  plane = p;  display = d;  referenceonly = r;  }
#endif
  };

extern void drawgrid(int drawmode,void *grid,View3dSurface *window);

#endif

