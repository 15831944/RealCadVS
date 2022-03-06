//
//  Definition of the 'Zone' class
//

#ifndef _NCVIEW_HEADER
#define _NCVIEW_HEADER

#include "ncwin.h"

class Point;
class QString;
class BitMask;
class CadFile;
class UndoFile;
class GeneralList;

class Zone
  {private:
        RCCHAR *name;
	    double umin,umax,vmin,vmax;
	  public:
        Zone(RCCHAR *,double,double,double,double);
        ~Zone();
	    void change(double,double,double,double);
	    double getumin(void)  {  return umin;  }
	    double getvmin(void)  {  return vmin;  }
	    double getumax(void)  {  return umax;  }
	    double getvmax(void)  {  return vmax;  }
        RCCHAR *getname(void)  {  return name;  }
	    int save(CadFile *);
	    static Zone *load(CadFile *);
	    int save(UndoFile *);
	    int load(UndoFile *);
  };

class ZoneList : public GeneralList
  {public:
	  Zone *next(void) {  return (Zone *) GeneralList::next();  }
	  int add(Zone *zone) {  return GeneralList::add(zone);  }
	  int save(CadFile *);
	  int load(CadFile *);
  };

//
//  Definition of the 'View' class
//

#if ! defined(__View)
#define __View
#define VW_ONEVIEWONLY 0x8000
#define VW_LAYERINFO   0x4000
class View
{
   private:
     RCCHAR *name;
     Point eye,ref,dop,up;
     double front,back;
     short perspective,viewno;
     BitMask *selectionlayertable;
     BitMask *displaylayertable;
     BitMask *lockedlayertable;
     RCCHAR *workplanename;
     short layer,style,colour,weight;
    public:
     ZoneList zonelist;
    public:
        View(const RCCHAR *,int,int,Point,Point,Point,double,double);
        View(const RCCHAR *,int,int,double,double,double,double,double,double,double,double,double,double,double);
        View(const RCCHAR *,int,int);
	    ~View();
     void setname(RCCHAR * n);
     RCCHAR *getname(void)  {  return name;  }
     Point geteye(void)  {  return eye;  }
     Point getref(void)  {  return ref;  }
     Point getdop(void)  {  return dop;  }
     Point getup(void)  {  return up;  }
     Point getright() { return (ref-eye).normalize().cross(up).normalize(); }
     double getfront(void)  {  return front;  }
     double getback(void)  {  return back;  }
	    int getperspective(void)  {  return perspective;  }
     int getviewno(void)  {  return (viewno & (short)(~VW_ONEVIEWONLY));  }
     int getvisibility(void)  {  return (viewno & VW_ONEVIEWONLY) == VW_ONEVIEWONLY;  }
     void setviewno(int v)  {  viewno = short(v | (viewno & VW_ONEVIEWONLY));  }
     void setvisibility(int v)  {  viewno = short((viewno & (short)(~VW_ONEVIEWONLY)) | (v * VW_ONEVIEWONLY));  }
	    void standard(int);
     int getlayer(void)  {  return layer;  }   
     int getcolour(void)  {  return colour;  }   
     int getstyle(void)  {  return style;  }   
     int getweight(void)  {  return weight;  }   
     void setlayer(int l)  {  layer = l;  }   
     void setcolour(int c)  {  colour = c;  }   
     void setstyle(int s)  {  style = s;  }   
     void setweight(int w)  {  weight = w;  }   
     BitMask *getselectionlayertable(void)  {  return selectionlayertable;  }
     void setselectionlayertable(BitMask *l)  {  selectionlayertable = l;  }
     BitMask *getdisplaylayertable(void)  {  return displaylayertable;  }
     void setdisplaylayertable(BitMask *l)  {  displaylayertable = l;  }
     BitMask *getlockedlayertable(void)  {  return lockedlayertable;  }
     void setlockedlayertable(BitMask *l)  {  lockedlayertable = l;  }
     RCCHAR *getworkplanename(void)  {  return workplanename;  }
     void setworkplanename(RCCHAR *n);
	    int changeview(Point,Point,Point);
     int changeview(int,Point);
	    int changeview(int);
	    int changeclip(double,double);
	    int save(CadFile *);
	    static View *load(CadFile *);
	    int save(UndoFile *);
	    int load(UndoFile *);
};

class ViewList : public GeneralList
  {public:
	  View *next(void) {  return (View *) GeneralList::next();  }
	  View *last(void) {  return (View *) GeneralList::last();  }
  };

class Database;
class Entity;
 
class Views3d
  {private:
	    ViewList list;
  	public:
     int add(View *v) {  return list.add(v);  }
     void del(View *v)  {  list.del(v);  }
     View *match(const RCCHAR *);
     View *match(const char *);
     View *match(const int);
     View *match(View *);
     void names(RCCHAR ***,int *,int *);
     void clear(void);
	    void initialize(void);
     int save(CadFile *);
	    static int load(CadFile *,Database *);
  };

class View2d;

enum OneViewOptions  {  OVHide, OVLayerTableDefined, OVShade, OVAngleDefined, OVDashHidden };

class OneView
{private:
   View *view;
   double umin,vmin,umax,vmax,scale,xpos,ypos,angle;
	  BitMask options,*layertable;
 public:
	  OneView(View *,double,double,double,double,double,double,double,double,const BitMask&,BitMask *);
	  void setclip(double,double,double,double);
   void setposition(double,double);
   View *getview(void)  {  return view;  }
   double getxpos(void)  {  return xpos;  }
   double getypos(void)  {  return ypos;  }
   double getumin(void)  {  return umin;  }
   double getvmin(void)  {  return vmin;  }
   double getumax(void)  {  return umax;  }
   double getvmax(void)  {  return vmax;  }
   double getscale(void)  {  return scale;  }
   double getangle(void)  {  return angle;  }
   BitMask *getlayertable(void)  {  return layertable;  }
   void setlayertable(BitMask *l)  {  layertable = l;  }
	  void setscale(double);
   int gethiddenlines(void)  {  return options.test(OVHide);  }
   int getshaded(void)  {  return options.test(OVShade);  }
   int getdashhidden(void)  {  return options.test(OVDashHidden);  }
	  int verify(Database *,View2d *);
   int change(Database *,View2d *);
   int save(CadFile *);
   static OneView *load(CadFile *);
   int save(UndoFile *);
   static OneView *load(UndoFile *);
};

class View2d
{private:
    RCCHAR *name;
  	double paperwidth,paperheight;
    GeneralList views;
    short viewno;
    int status;
 public:
    static int nsizes;
    //static RCCHAR *sizes[];
    static QString sizes[];
    static double widths[];
    static double heights[];
    View2d(RCCHAR *name,double,double,short);
    ~View2d();
	   ZoneList zonelist;
       RCCHAR *getname(void)  {  return name;  }
       void setname(RCCHAR *);
   	double getpaperwidth(void)  {  return paperwidth;  }
    double getpaperheight(void)  {  return paperheight;  }
    int add(OneView *);
    void del(OneView *);
    void start(void);
    OneView *next(void);
    int getviewno(void)  {  return (viewno & (short)(~VW_ONEVIEWONLY));  }   void sizeevent(double,double);
    int verify(Database *);
   	int change(Database *);
    void change(RCCHAR *name,double width,double height,double offsetx,double offsety);
    int save(CadFile *);
    static View2d *load(CadFile *);
    int save(UndoFile *);
    int load(UndoFile *);
    int getstatus(void) {  return status;  }
    void setstatus(int s) {  status = s;  }
#ifdef USING_WIDGETS
    static int matchPaperName(RCCHAR *name);
#endif
};

class View2dList : public GeneralList
  {public:
     View2d *next(void) {  return (View2d *) GeneralList::next();  }
     View2d *last(void) {  return (View2d *) GeneralList::last();  }
  };

class Views2d
  {private:
     View2dList list;
   public:
     int add(View2d *v) {  return list.add(v);  }
     void del(View2d *v)  {  list.del(v);  }
     View2d *match(const RCCHAR *);
     View2d *match(const int);
     View2d *match(View2d *);
     void names(RCCHAR ***,int *,int *);
     void clear(void);
     void initialize(void);
     int save(CadFile *);
     static int load(CadFile *,Database *);
  };

#endif

struct PlotScales
  {
   //RCCHAR *label;
   RCSTRING label;
   double scale;
  };

extern PlotScales DefaultPlotScales[];
RCCHAR *MatchPlotScale(double plotscale);

#endif
