 
//#if ! defined(__Entity)
//#define __Entity
#ifndef _NCENTITY_H_
#define _NCENTITY_H_

#include "ncwin.h"
#include "ncbmask.h"
#include "Ncview.h"
#include "qtdefs.h"

enum entity_types
  { end_list = -1,
    many_mask = 0,
    uv_mask = 1,
    xyz_mask = 2,
    linear_mask = 3,
    surface_mask = 4,
    uvsurface_mask = 5,
    dimension_mask = 6,
    undo_mask = 7,
    point_entity = 10,
    line_entity = 11,
    circle_entity = 12,
    infiniteplane_entity = 13,
    plane_entity = 14,
    text_entity = 15,
    ruledsurface_entity = 16,
    surfaceofrev_entity = 17,
    lineardimension_entity = 18,
    figure_entity = 19,
    curve_entity = 20,
    group_entity = 21,
    angulardimension_entity = 22,
    ellipse_entity = 23,
    distancebearingdimension_entity = 24,
    circulardimension_entity = 25,
    light_entity = 26,
    wall_entity = 27,
    patch_entity = 28,
    bitmap_entity = 29,
    beam_entity = 30,
    ncsolid_entity = 31
  };

enum other_type
  {  view3d_record = 1001,
     workplane_record = 1002,
     grid_record = 1003,
     values_record = 1004,
     windows_record = 1005,
     origin_record = 1006,
     tables_record = 1007,
     view2d_record = 1008,
     layername_record = 1009,
     masktables_record = 1010,
     splitter_record = 1011
  };

class BitMask;
class View3dSurface;
class Entity;
class Rubberband;
class RCView3dPrinter;
class Box3;
class Box2;

enum EntityCallback {  ECDelete,ECMoved,ECVerify,ECUndo,ECTrim,ECSelection,ECRightClick,ECInsert,ECUserDelete  };
enum DefaultChangeType  {  DCLayer = 1,DCColour = 2,DCStyle = 4,DCWeight = 8  };
enum CCCommandCallback  {  CCTrimCommand,CCStartCommand,CCEndCommand  };

typedef int (WINAPI *PropertyCallback)(Entity *,int action,int id,void *data,char *text);
typedef int (WINAPI *ModifyEntityCallback)(Entity *);
typedef int (WINAPI *CommandCallback)(int command,int state,void *data);
typedef int (WINAPI *MessageCallback)(HWND,UINT,WPARAM,LPARAM);
typedef void (WINAPI *OpenFunction)(void);
typedef void (*DimensionVisibilityCallback)(int viewno,BitMask *visibility);
typedef void (*DefaultChangedCallback)(int type);

extern DimensionVisibilityCallback dimensionvisibilitycallback;

extern void FigureExtents(RCCHAR *library,View3dSurface *vs,Point *pmin,Point *pmax);
extern void SetOpenFunction(OpenFunction);
extern void SetDefaultChangedCallback(DefaultChangedCallback);
extern BitMask * GetLockedLayerTable(void);
extern Rubberband rubberband;

// KMJ:
class CloneInfo
{
public:
    int mode; // 0 = colour, 1 = layer, 2= style, 3 = weight
    int colour;
    int layer;
    int style;
    double weight;
    int iweight;
public:
    CloneInfo();
    CloneInfo(int m, int c, int l, int s, double w);
    CloneInfo(int m, int c, int l, int s, int iw);
    CloneInfo(const CloneInfo &o);
};

//
//  Operation = 0 Perform trim operation, Operation = 1 Rubberband  Operation = 2  Check to see if this entity can be trimmed
struct TrimCallbackData
  {int operation,end;
   Point p;
   Entity *trimentity,*trimtoentity;
  }; 

class CoordinateSystem
  {private:
     Point origin,xaxis,yaxis,zaxis;
   public:
     CoordinateSystem()
       {  origin.setxyz(0.0,0.0,0.0);  xaxis.setxyz(1.0,0.0,0.0);  yaxis.setxyz(0.0,1.0,0.0);
       }
     CoordinateSystem(Point o,Point xa,Point ya)
       {  origin = o;  xaxis = xa;  yaxis = ya;  zaxis = xaxis.cross(yaxis).normalize();
       }
     void change(Point o,Point xa,Point ya)
       {  origin = o;  xaxis = xa;  yaxis = ya;  zaxis = xaxis.cross(yaxis).normalize();
       }
     void set(Point origin_,Point xaxis_,Point yaxis_)  
       {  origin = origin_;  xaxis = xaxis_;  yaxis = yaxis_;  zaxis = xaxis.cross(yaxis).normalize();
       }
     Point getorigin(void) const {  return origin;  }
     Point getxaxis(void) const {  return xaxis;  }
     Point getyaxis(void) const {  return yaxis;  }
     void setorigin(Point o) {  origin = o;  }
     void setxaxis(Point xa) {  xaxis = xa;  }
     void setyaxis(Point ya) {  yaxis = ya;  }
     Point tomodel(Point p) const {  return origin + xaxis * p.x + yaxis * p.y + zaxis * p.z;  }
     Point frommodel(Point p) const
       {Point p1(p - origin);  Point tmp(xaxis.dot(p1),yaxis.dot(p1),zaxis.dot(p1));  return tmp;  }
     double frommodelx(Point p) const
       {  return xaxis.dot(p - origin);
       } 
     double frommodely(Point p) const
       {  return yaxis.dot(p - origin);
       } 
     double frommodelz(Point p) const
       {  return zaxis.dot(p - origin);
       }
     int save(CadFile *) const;
     int load(CadFile *);
  };

#if defined(WIN32)
#define HUGE
#else
#define HUGE huge
#endif

class EntityList : public GeneralList
{public:
  int add(Entity *entity)  {  return GeneralList::add(entity);  }
  void del(Entity *entity)  {  GeneralList::del(entity);  }
  void highlight(void);
  void unhighlight(void);
  inline Entity *next(void) {  return (Entity *) GeneralList::next();  }
  inline Entity *last(void)  {  return (Entity *) GeneralList::last();  }
  EntityList copy(void)  {EntityList tmp;  tmp.setlist(GeneralList::copy());  return tmp;  }
  int save(CadFile *);
  int load(CadFile *);
  int savegeometry(CadFile *);
  int loadgeometry(int,CadFile *);
  void fixupreferences(Entity **);
};

class AttributeList : public GeneralList
{
public:
//#ifdef _USING_QTCHAR
  int add(RCCHAR *attribute)  {  return GeneralList::add(attribute);  }
  void del(RCCHAR *attribute)  {  GeneralList::del(attribute);  }
  RCCHAR *next(void) {  return (RCCHAR *) GeneralList::next();  }
//#else
//  int add(char *attribute)  {  return GeneralList::add(attribute);  }
//  void del(char *attribute)  {  GeneralList::del(attribute);  }
//  char *next(void) {  return (char *) GeneralList::next();  }
//#endif
  AttributeList copy(void) {AttributeList tmp;  tmp.setlist(GeneralList::copy());   return tmp;  }
  int save(CadFile *);
  int load(CadFile *);
};

class Dialog;
class PatternLineStyle;
// TODO: KMJ Do Attributes need to be unicode?
class EntityHeader
  {protected:
     short layer;
     Byte colour,style,weight;
     BitMask visible;
     AttributeList atlist;
     EntityList relatedlist;
     PatternLineStyle *patternlinestyle;
     float linestylescale;
   public:
     EntityHeader();
     EntityHeader(int,int,int,int);
     EntityHeader(const EntityHeader &);
     ~EntityHeader();
     short getlayer(void) const {  return layer;  }
     Byte getcolour(void) const {  return colour;  }
     Byte getstyle(void) const {  return style;  }
     Byte getweight(void) const {  return weight;  }
     float getlinestylescale(void)  {  return linestylescale;  }
     void setlinestylescale(float lss)  {  linestylescale = lss;  }
     void setlayer(short l)  {  layer = l;  }
     void setcolour(Byte c)  {  colour = c;  }
     void setstyle(Byte s)  {  style = s;  }
     void setweight(Byte w)  {  weight = w;  }
     PatternLineStyle *getpatternlinestyle(void) const {  return patternlinestyle;  }
     void setpatternlinestyle(PatternLineStyle *pls);
     void change(int,int,int,int);
     void setvisible(BitMask &v)  {  visible = v;  }
     BitMask *getvisible(void)  {  return &visible;  }
     AttributeList getatlist() const {  return atlist;  }

     int addat(RCCHAR *att)  {  return atlist.add(att);  }
     void delat(RCCHAR *att)  {  atlist.del(att);  }

     void substituteat(AttributeList list)  {  atlist.destroy();  atlist = list;  }
     int issame(const EntityHeader &);
     void verify(Dialog *,int *,int *,int *,double *) const;
     int saveheader(CadFile *);
     static EntityHeader *loadheader(CadFile *);
     int saveheaderu(CadFile *);
     int loadheaderu(CadFile *);
     int saveheaderv(CadFile *);
     int loadheaderv(CadFile *);
     void CopyAttributes(Entity *);
  };

class HideImage;

class SortedPointTable
  {private:
     int n;
     SortedPointNormList list;
     Point *sortedpoints;
   public:
     SortedPointTable() {  n = 0;  sortedpoints = NULL;  }
     ~SortedPointTable();
     int add(Point p);
     int endlist(void);
     int match(Point p);
  };

enum {  Geometry,Attributes  };

class View3dWindow;
class DXFOut;
class SurfaceInfo;
class Surface;

#define MaxViews 160

typedef void (*EntityCallBack)(Entity *,void *);

extern bool isUnicodeEntity( Entity * e );

class Entity : public EntityHeader
  {
#ifdef USING_WIDGETS
public:
    // pointers to graphics item for this entity
    // null if not drawn in the scene
    qreal zValue;
    QGraphicsItem *qgi; // Qt graphics item 1
    //QVector<QGraphicsItem*> qgiLines; // Qt graphics item for multiple view ports
    //QVector<QGraphicsItem*> qgi3dVitems; // Qt graphics item for multiple views
    //void putqgi(Surface *s,QGraphicsItem *gi);
    //QGraphicsItem * getqgi(Surface *s);
    bool usedByPlane; // child of a plane flag
    bool usedByFigure; // child of a figure flag
#endif
protected:
     OneGeneral *position;
     EntityList alist;
     Byte defined,selected,status;
     long id;
     union
       {  Entity *copye;
          char *cattribute;
          RCCHAR *attribute;
          long index;
          SurfaceInfo *surfaceinfo;
       };
   public:
     Entity(void);
     Entity(const EntityHeader &);
     virtual ~Entity();
     int isvisible(View3dSurface *);
     int isselectable(View3dSurface *);
     void highlight(void);
     void unhighlight(void);
     OneGeneral *getposition(void) const {  return position;  }
     void setposition(OneGeneral *og)  {  position = og;  }
     char *getcattribute(void) const {  return cattribute;  }
     void setcattribute(char *a)  {  cattribute = a;  }
#ifdef USING_WIDGETS
     void setattribute(char *a)  {  attribute = _RCT(a);  }
     RCCHAR *getattribute(void) const {  return attribute;  }
     void setattribute(RCCHAR *a)  {  attribute = a;  }
#endif
     long getindex(void) const {  return index;  }
     void setindex(long i) {  index = i;  }
     long getid(void) const {  return id;  }
     void setid(long i) {  id = i;  }
     int getdefined(void) const {  return defined;  }
     int getstatus(void) const {  return status;  }
     void setstatus(int s)  {  status = Byte(s);  }
     int getselected(void) const {  return selected & 127;  }
     void setselected(int s)  {  selected = short(s) | short(selected & 128);  }
     int gethandles(void) const {  return (selected & 128) == 128;  }
     void sethandles(int s)  {  selected = Byte((selected & 127) | (s != 0) * 128);  }
     int addrelationship(Entity *relatedentity,long ApplicationID,long Relationship);
     int deleterelationship(Entity *relatedentity,long ApplicationID,long Relationship);
     int getrelationship(int index,Entity **relatedentity,long *ApplicationID,long *Relationship);
#ifdef USING_WIDGETS
     virtual int process_property(int action,int id,void *data,void *text)  
	 {  
		 if( isUnicodeEntity(this) )
			 return process_property(action,id,data,(RCCHAR*)text);
		 else
			 return process_property(action,id,data,(char*)text);
	 }

     virtual int process_property(int action,int id,void *data,RCCHAR *text)  {  return 0;  }
     virtual int process_property(int action,int id,void *data,char *text)  {  return 0;  }
#else
     virtual int process_property(int action,int id,void *data,char *text)  {  return 0;  }
#endif
     virtual int pickhandlepoint(View3dSurface *,int,int,int,Point *) {  return 0;  };
     virtual void movehandlepoint(View3dSurface *,int,int,Point) {};
     Entity *getcopye(void) const {  return copye;  }
     void setcopye(Entity *e)  {  copye = e;  }
     int adda(Entity *e)  {  return alist.add(e);  }
     void dela(Entity *e)  {  alist.del(e);  }
     EntityList getalist(void) const {  return alist;  }
     void draw(int mode,Transform *t = NULL);
     virtual int hide(HideImage *image) = 0;
     virtual int type(void) const = 0;
     virtual int isa(int) const = 0;
     virtual int issame(int,const Entity &) const = 0;
#ifdef USING_WIDGETS
     virtual void draw(int,Transform *,View3dSurface *, QPainter *) = 0;
     virtual void drawGL(int,Transform *,View3dSurface *) {}
     void drawGL(int,Transform *t = NULL);
     virtual int hideGL(HideImage *image) {return 0;}
#endif
     virtual void draw(int,Transform *,View3dSurface *) = 0;
     virtual void addtodisplaylist(View3dSurface *,int) = 0;
     void change(const BitMask&,int,int,int,int);
     virtual int match(const BitMask &) const = 0;
     virtual double pick(View3dSurface *,double,double,Point *) = 0;
     virtual int pick(View3dSurface *,int,int,int,int,int) = 0;
     virtual int pick(View3dSurface *,int,Point *,int) = 0;
     virtual int pickoriginpoint(View3dSurface *,int,int,int,Point *) {  return 0;  }
     virtual int npoints(void) = 0;
     virtual Point point(int) = 0;
     virtual Point nearp(Point) = 0;
     virtual int islinear(void) {  return 0;  }
     virtual int issurface(void) {  return 0;  }
     virtual int isdimension(void) {  return 0;  }
     virtual int isuvsurface(void) {  return 0;  }
     virtual void dbinsert(Database *,int draw = 3);
     virtual void dbdelete(void);
     virtual void dbsetassociated(void) {};
     virtual void dbtraverseassociated(EntityCallBack,void *) {};
     int stretch(Database *,Transform &,SortedPointTable *,int erase = 1);
     virtual int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1) = 0;
     virtual int move(Database *,Transform &,int erase = 1) = 0;
     virtual void transform(Transform &,int draw = 1) {}  //  Does a simple low level move with checking for connectivity
     void cloneattributes(Entity *);
     virtual Entity *clone(Transform &) = 0;
     virtual Entity *deepclone(Transform &t) {  return clone(t);  }
     virtual int project(Database *,Transform &,int) = 0;
     virtual int save(CadFile *) = 0;
     virtual int save(UndoFile *) = 0;
     virtual void fixupreferences(Entity **);
     virtual void verify(void) = 0;
     virtual int explode(void) = 0;
     virtual void extents(View3dSurface *,Point *,Point *) = 0;
     int saveunknowngeometry(CadFile *);
     static int loadunknowngeometry(int,CadFile *,Entity **);
     virtual int savegeometry(CadFile *) = 0;
     virtual int savedxf(int,DXFOut *) = 0;
     virtual int loadgeometry(CadFile *) = 0;
     virtual int saveattributes(CadFile *) = 0;
     virtual int loadattributes(CadFile *) = 0;
     virtual int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter) = 0;
     virtual void setdefaults(void) {};
  };

inline long CadFile::getentityindex(Entity *e)  {  return e->getindex();  }
inline long UndoFile::getentityindex(Entity *e)  {  return e->getid();  }
inline bool isUnicodeEntity( Entity * e )
{
	if( e->type() == text_entity ||
		e->type() == lineardimension_entity ||
		e->type() == angulardimension_entity ||
		e->type() == distancebearingdimension_entity  ||
		e->type() == circulardimension_entity )
		return true;

	return false;
}

class PointE : public Entity
  {
    friend class CadBasicProgram;
private :
     Point p;
   public :
     PointE() {  defined = 0; qgi = 0;}
     PointE(const EntityHeader &,Point p1);
     PointE(Point p1);
     Point getp(void) {  return p;  }
     int type(void) const;
     int isa(int) const;
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(point_entity);  }
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int mode,Transform *t,View3dSurface * );
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL)
     {
         Entity::draw(mode,t);
     }
     int hide(HideImage *image);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int process_property(int action,int id,void *data,RCCHAR *text);
   };

class InfinitePlane : public Entity
   {private:
      Point normal;
      double D;
    public:
      InfinitePlane() {  defined = 0;  }
      InfinitePlane(Point n,double d)  {  normal = n;  D = d;  }
      InfinitePlane(const EntityHeader &header,Point n,double d) : Entity(header) {  normal = n;  D = d;  }
      InfinitePlane(Point n,Point o)  {  normal = n;  D = -n.dot(o);  }
      Point getnormal(void)  {  return normal;  }
      double getD(void)  {  return D;  }
      int type(void) const {  return infiniteplane_entity;  }
      int isa(int type) const {  return infiniteplane_entity == type;  }
      int issame(int,const Entity &) const;
      int match(const BitMask &mask) const {  return mask.test(infiniteplane_entity);  }
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
#endif
      void draw(int,Transform *,View3dSurface *) {}
      void addtodisplaylist(View3dSurface *,int) {}
      void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
      int hide(HideImage *image);
      double pick(View3dSurface *,double,double,Point *) {  return -1.0;  }
      int pick(View3dSurface *,int,int,int,int,int) {  return 0;  }
      int pick(View3dSurface *,int,Point *,int) { return 0; }
      int npoints(void) {  return 0;  }
      Point point(int)  {Point tmp(0.0,0.0,0.0);  return tmp;  }
      Point nearp(Point);
      int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
      int move(Database *,Transform &,int) {  return 0;  }
      Entity *clone(Transform &) {  return new InfinitePlane(*this,normal,D);  }
      int project(Database *,Transform &,int);
      void verify(void);
      int explode(void);
      void extents(View3dSurface *,Point *,Point *);
      int save(CadFile *);
      static Entity *load(int,CadFile *);
      int save(UndoFile *);
      static Entity *load(UndoFile *);
      int savegeometry(CadFile *);
      int savedxf(int,DXFOut *);
      int loadgeometry(CadFile *);
      int saveattributes(CadFile *);
      int loadattributes(CadFile *);
      int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
   };

enum  { LE_INCLUDESTART = 1,LE_INCLUDEEND = 2,LE_REVERSE = 4 };

class LinearEntity : public Entity
  {
public:
     LinearEntity(void) { }
     LinearEntity(const EntityHeader &header) : Entity(header) {};
     virtual double length(void) const = 0;
     virtual Point position(double) const = 0;
     virtual Point positionbylength(double,double *t) const = 0;
     virtual Point direction(double) const = 0;
     virtual int beginoffset(Point,Point *,double *,LinearEntity *,int *,Point *) = 0;
     virtual int offset(Point,double,int,LinearEntity *,int *,Point *,EntityList *list) = 0;
     virtual int offsetpoint(Point,double,int,int,Point *) = 0;
     virtual Point start() = 0;
     virtual Point end() = 0;
     virtual double gett(Point) = 0;
     virtual int nsegments(double) = 0;
     virtual void segment(int,int,Point *) = 0;
#ifdef USING_WIDGETS
     virtual LinearEntity *divide(Point, Point *dp=0) = 0;
     virtual void divide(int, PointList *dplist=0) = 0;
#else
     virtual LinearEntity *divide(Point) = 0;
     virtual void divide(int) = 0;
#endif
     virtual int pickendpoint(View3dSurface *,int,int,int,Point *) = 0;
     virtual void moveendpoint(int,int,Point) = 0;
     virtual int trace(View3dSurface *,Point,Point,Point,double,Point *,Point *,Point *,double *) = 0;
     virtual double pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *) = 0;
     virtual void breakanddelete(int,Point,Point) = 0;
     int islinear(void) {  return 1;  }
  };

class Curve;
class EllipseE;
class Surface;

class Line : public LinearEntity
  {
    friend class CadBasicProgram;
#ifdef USING_WIDGETS
public:
    // pointers to graphics items for the handles of the line
    // null if not drawn in the scene
    QGraphicsItem *qgi1; // Qt graphics item 1
    QGraphicsItem *qgi2; // Qt graphics item 1
    // for drawing in Composite drawing views
    // using a one scene multiple views scenario
    // the entity is drawn multiple times in the scene
#endif
   private:
     Point p1,p2;
     void line(int tangentonly,Curve *,Point,Point);
     void line(int tangentonly,EllipseE *,Point,Point);
   public:
     Line() {  defined = 0;  qgi = 0; qgi1=0; qgi2=0; usedByPlane=false;}
     Line(const EntityHeader &,Point,Point);
     Line(Point p1,Point p2);
     Line(Entity *,Point,Point);
     Line(Entity *,Point,Entity *,Point);
     Line(Point,Entity *,Point,Entity *,Point,double constrainlength);
     Point getp1(void)  {  return p1;  }
     Point getp2(void)  {  return p2;  }
     void setpts(Point pt1, Point pt2) { p1 = pt1; p2 = pt2; }
     void setp1(Point pt) { p1 = pt; }
     void setp2(Point pt) { p2 = pt; }
     int type(void) const;
     int isa(int) const;
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(line_entity) || mask.test(linear_mask);  }
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *);
     void drawGL(int mode,Transform *t,View3dSurface *);
     void drawGL(int mode,Transform *t = NULL) { Entity::drawGL(mode,t); }
     int hideGL(HideImage *image);
#endif
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int pickendpoint(View3dSurface *,int,int,int,Point *);
     int pickoriginpoint(View3dSurface *,int,int,int,Point *);
     void moveendpoint(int,int,Point);
     void breakanddelete(int,Point,Point);
     int trace(View3dSurface *,Point,Point,Point,double,Point *,Point *,Point *,double *);
     double pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *);
     double length(void) const;
     Point position(double t) const;
     Point positionbylength(double l,double *t) const;
     Point direction(double t) const;
     int beginoffset(Point,Point *,double *,LinearEntity *,int *,Point *);
     int offset(Point,double,int,LinearEntity *,int *,Point *,EntityList *);
     int offsetpoint(Point,double,int,int,Point *);
     Point start();
     Point end();
     double gett(Point);
     int nsegments(double = 0.0)  { return 1;  }
     void segment(int,int,Point *);
     LinearEntity *divide(Point, Point *dp=0 );
     void divide(int, PointList *dplist=0);
     int npoints(void);
     Point point(int);
     Point nearp(Point point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     void transform(Transform &,int draw = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     void reverse(void);
     int ison(Point p,int includeends = 0);
     int process_property(int action,int id,void *data,RCCHAR *text);
  };

class Circle : public LinearEntity
  {
    friend class CadBasicProgram;
#ifdef USING_WIDGETS
public:
    // pointers to graphics items for the handles of the circle
    // null if not drawn in the scene
    QGraphicsItem *qgi1; // Qt graphics item 1
    QGraphicsItem *qgi2; // Qt graphics item 1
    unsigned int gldlist; // opengl display list name
#endif
   private:
     Point   origin,xaxis,yaxis;
     double  radius,starta,sweepa;
     void circle(int,double,Curve *,Point,Entity *,Point);
     void circleppp(int,Point,Point,Point,double *);
     void circlelll(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circlepll(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circleppl(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circleppc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circleplc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circlellc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circlepcc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circlelcc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     void circleccc(int,Entity *,Point,Entity *,Point,Entity *,Point,double *);
     double calculated(Point,Point,double,Entity *,Point ep,Point *,Point *,Point *);
   public:
     Circle() {  defined = 0; qgi = 0; qgi1=0; qgi2=0; usedByPlane=false; }
     Circle(const EntityHeader &,Point,Point,Point,double,double,double);
     Circle(Point,Point,Point,double,double,double);
     Circle(int,Point,Point,Point,Point);
     Circle(Point,double);
     Circle(Point,Point);
     Circle(Point,Entity *,Point);
     Circle(int,Point,Point,Point);
     Circle(int,double,Point,Point,Point);
     Circle(int,double,Entity *,Point,Point);
     Circle(int,double,Entity *,Point,Entity *,Point);
     Circle(int,Entity *,Point,Entity *,Point,Entity *,Point);
     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     double getstarta(void)  {  return starta;  }
     double getsweepa(void)  {  return sweepa;  }
     double getradius(void)  {  return radius;  }
     void filletangles(int,Point,Point);
     int type(void) const;
     int isa(int) const;
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(circle_entity) || mask.test(linear_mask);  }
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *);
     void drawGL(int mode,Transform *t,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     void segment(int,int,Point *);
     int nsegments(double);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int pickendpoint(View3dSurface *,int,int,int,Point *);
     int pickoriginpoint(View3dSurface *,int,int,int,Point *);
     void moveendpoint(int,int,Point);
     void breakanddelete(int,Point,Point);
     int trace(View3dSurface *,Point,Point,Point,double,Point *,Point *,Point *,double *);
     double pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *);
     double length(void) const;
     double angle(Point p);
     Point position(double t) const;
     Point positionbylength(double l,double *t) const;
     Point direction(double t) const;
     int beginoffset(Point,Point *,double *,LinearEntity *,int *,Point *);
     int offset(Point,double,int,LinearEntity *,int *,Point *,EntityList *);
     int offsetpoint(Point,double,int,int,Point *);
     Point start();
     Point end();
     double gett(Point);
     LinearEntity *divide(Point, Point *dp=0);
     void divide(int, PointList *dplist=0);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     void transform(Transform &,int draw = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     void reverse(void);
     int process_property(int action,int id,void *data,RCCHAR *text);
  };

class EllipseE : public LinearEntity
  {private:
     Point   origin,xaxis,yaxis;
     double  major,minor,starta,sweepa;
   public:
     EllipseE() {  defined = 0;  usedByPlane=false;}
     EllipseE(const EntityHeader &,Point,Point,Point,double,double,double,double);
     EllipseE(Point,Point,Point,double,double,double,double);
     EllipseE(Point,Point,Point);
     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     double getstarta(void)  {  return starta;  }
     double getsweepa(void)  {  return sweepa;  }
     double getmajor(void)  {  return major;  }
     double getminor(void)  {  return minor;  }
     double togeometric(double);
     double toparametric(double);
     int type(void) const;
     int isa(int) const;
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(ellipse_entity) || mask.test(linear_mask);  }
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int mode,Transform *t,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     void segment(int,int,Point *);
     int nsegments(double);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int pickendpoint(View3dSurface *,int,int,int,Point *);
     void moveendpoint(int,int,Point);
     void breakanddelete(int,Point,Point);
     int trace(View3dSurface *,Point,Point,Point,double,Point *,Point *,Point *,double *);
     double pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *);
     double length(void) const;
     double angle(Point p);
     Point position(double t) const;
     Point positionbylength(double l,double *t) const;
     Point direction(double t) const;
     int beginoffset(Point,Point *,double *,LinearEntity *,int *,Point *);
     int offset(Point,double,int,LinearEntity *,int *,Point *,EntityList *);
     int offsetpoint(Point,double,int,int,Point *);
     Point start();
     Point end();
     double gett(Point);
     LinearEntity *divide(Point, Point *dp=0);
     void divide(int, PointList *dplist=0);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     void transform(Transform &,int draw = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int process_property(int action,int id,void *data,RCCHAR *text);
  };

class Curve : public LinearEntity
  {
#ifdef USING_WIDGETS
    public:
    // pointers to graphic item for the handles of the curve
    // null if not drawn in the scene
    QList<QGraphicsItem *> qgHandles; // list of handles
    QGraphicsItem *qgih; // Qt graphics handle
#endif
    private:
     short n,degree,property_point_index;
     Point *polygon;
     double  *knot,*cweight,*tknot,*tbasis;
     BitMask options;
     void rbasis(double,double *,double *,int *) const;
     void tempknots(double **);
     void param(Point *,double **);
   public:
     Curve() : options(32) {  defined = 0;  polygon = NULL;  knot = cweight = tknot = tbasis = NULL; usedByPlane=false; }
     Curve(const EntityHeader &,int,int,Point *,double *,double *,const BitMask &);
     Curve(int,int,Point *,double *,double *,const BitMask &);
     Curve(int,int,Point *,const BitMask &);
     Curve(int,int,int,Point *,const BitMask &);
     ~Curve();
     int getn(void)  {  return n;  }
     int getdegree(void)  {  return degree;  }
     Point *getpolygon(void)  {  return polygon;  }
     double *getknot(void)  {  return knot;  }
     double *getcweight(void)  {  return cweight;  }
     BitMask getoptions(void)  {  return options;  }
     int type(void) const {  return curve_entity;  }
     int isa(int type) const {  return curve_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(curve_entity) || mask.test(linear_mask);  }
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int drawmode,Transform *trans,View3dSurface *vs);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     void newbasis(void);
     void deletebasis(void);
     void segment(int,int,Point *);
     int nsegments(double);
     void insertpoint(int,int,int,Point);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int pickendpoint(View3dSurface *,int,int,int,Point *);
     void moveendpoint(int,int,Point);
     void breakanddelete(int,Point,Point);
     int trace(View3dSurface *,Point,Point,Point,double,Point *,Point *,Point *,double *);
     double pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *);
     void movepoint(int,int,Point);
     double length(void) const;
     double angle(Point p);
     Point position(double t) const;
     Point positionbylength(double l,double *t) const;
     Point direction(double t) const;
     int beginoffset(Point,Point *,double *,LinearEntity *,int *,Point *);
     int offset(Point,double,int,LinearEntity *,int *,Point *,EntityList *);
     int offsetpoint(Point,double,int,int,Point *);
     Point start();
     Point end();
     double gett(Point);
     LinearEntity *divide(Point, Point *dp=0);
     void divide(int, PointList *dplist=0);
     void subdivide(double,Curve **,Curve **);
     int insertknots(int,double *,int *,int *,Point **,double **,double **,double **) const;
     int posdir(double,double **,double **,Point *,Point *) const;
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     Point nearp(int normalonly,Point,double *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     void transform(Transform &,int draw = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     void change(const BitMask &change,const EntityHeader &eh,int d,const BitMask &o);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     EntityList convert(int nlines);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
//#ifdef _USING_QTCHAR
//     int process_property(int action,int id,void *data,RCCHAR *text);
//#else
     int process_property(int action,int id,void *data,char *text);
//#endif
  };

#define  MAXTEXTHEIGHT 2000.0
#define  GGO_BUF_SIZE 4096

//  options use
//    Bit 0    :  Symbol Font
//    Bit 1,2  :  00  Left Horizontal  10  Centre Horizontal  10  Right Horizontal
//    Bit 3,4  :  00  Bottom Vertical  10  Centre Vertical    10  Top Vertical
//    Bit 5    :  Bold
//    Bit 6    :  Underlined
//    Bit 7    :  Italic
//    Bit 8    :  Boxed
//    Bit 9    :  Paper sized text
//    Bit 10   :  Outline
//    Bit 11   :  Force drawing in hidden line views
//    Bit 12   :  Haloed text
//    Bit 13   :  Text size in points.

enum {  TextSymbolFont, TextHorizontalJustification1, TextHorizontalJustification2, TextVerticalJustification1, TextVerticalJustification2, TextBold, TextUnderLined, TextItalic, TextBoxed, TextPaperSized, TextOutline, TextShowInHiddenLine, TextHaloed, TextSizeInPoints  };

class Text : public Entity
  {
#ifdef USING_WIDGETS
public:
    // pointers to graphics item for the box and fill
    QGraphicsItem *qgbox;
    QGraphicsItem *qgfill;

    // temp char data for use with the old api
    char *tmptext,*tmplabel,*tmpfont;
#endif
private:

     RCCHAR *text,*label,*font;

     Point origin,xaxis,yaxis;
     BitMask options;
     double height,width,vertspacing,angle;
     Byte halocolour;
   public:
     Text() : options(32) {  defined = 0; text=0; label=0; font=0; tmptext=0; tmplabel=0; tmpfont=0;}

     Text(const EntityHeader &,RCCHAR *,Point,Point,Point,double,double,double,double,Byte,RCCHAR *,RCCHAR *,const BitMask&);
     Text(RCCHAR *,Point,Point,Point,double,double,double,double,Byte,RCCHAR *,RCCHAR *,const BitMask&);
     Text(const EntityHeader &,char *,Point,Point,Point,double,double,double,double,Byte,char *,char *,const BitMask&);
     Text(char *,Point,Point,Point,double,double,double,double,Byte,char *,char *,const BitMask&);

     ~Text();
     int type(void) const {  return text_entity;  }
     int isa(int type) const {  return text_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(text_entity);  }

     RCCHAR *gettext(void) {  return text;  }
     RCCHAR *gettextu(void) {  return text;  }
     RCCHAR *getlabel(void) {  return label;  }
     RCCHAR *getfont(void) {  return font;  }

     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     BitMask getoptions(void)  {  return options;  }
     double getheight(void)  {  return height;  }
     double getangle(void)  {  return angle;  }
     double getwidth(void)  {  return width;  }
     Byte gethalocolour(void)  {  return halocolour;  }
     double getvertspacing(void)  {  return vertspacing;  }
     void ScaleTextHeight(double s)  {  height *= s;  /*width *= s;*/  } // KMJ: width is a % scale factor so don't scale here
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     int pickoriginpoint(View3dSurface *,int,int,int,Point *);
#ifdef USING_WIDGETS
     char *getchartext(void);
     char *getcharlabel(void);
     char *getcharfont(void);

     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int ,Transform *,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);     
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,RCCHAR *,double,double,double,double,Byte,RCCHAR *,RCCHAR *,const BitMask&);
     static int setup(BitMask *,RCCHAR *,double *,double *,double *,double *,Byte *,BitMask *,RCCHAR *text=0,RCCHAR *label=0,EntityHeader *header=0);
#ifdef _USING_QTCHAR
     static void getdefaults(RCCHAR **,double *,double *,double *,double *,Byte *,RCCHAR **,BitMask *);
     static void setdefaults(RCCHAR *,double,double,double,double,Byte,RCCHAR *,const BitMask&);
#endif
     static void getdefaults(char **,double *,double *,double *,double *,Byte *,char **,BitMask *);
     static void setdefaults(char *,double,double,double,double,Byte,char *,const BitMask&);
     static int getfonts(RCCHAR *,int *,RCCHAR ***,Byte **,int *);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
#ifdef _USING_QTCHAR
     static int extents(View3dSurface *,const RCCHAR *,const BitMask&,const RCCHAR *,double,double,double,double *,double *,double *,double *);
#endif
     static int extents(View3dSurface *,const char *,const BitMask&,const char *,double,double,double,double *,double *,double *,double *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);

     int process_property(int action,int id,void *data,RCCHAR *text);
     //int process_property(int action,int id,void *data,char *text);

     void setdefaults(void);
  };

class Group : public Entity
  {private:
#ifdef USING_WIDGETS
    // temp char data for use with the old api
     char *tmpclassname,*tmpinstance;
#endif
     RCCHAR *classname,*instance;
     BitMask options;
     EntityList list;
   public:
     Group() : options(32) {  defined = 0; classname=0; instance=0; tmpclassname=0; tmpinstance=0;}
     Group(const EntityHeader &,EntityList,const RCCHAR *,const RCCHAR *,const BitMask&);
     Group(const EntityHeader &,EntityList,const char *,const char *,const BitMask&);
     Group(EntityList,const RCCHAR *,const RCCHAR *,const BitMask&);
     Group(EntityList,const char *,const char *,const BitMask&);
     ~Group();
     int type(void) const {  return group_entity;  }
     int isa(int type) const {  return group_entity == type;  } 
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(group_entity);  }
     EntityList getlist(void)  {  return list;  }
     RCCHAR *getclassname(void)  {  return classname;  }
     RCCHAR *getinstance(void)  {  return instance;  }
     BitMask getoptions(void)  {  return options;  }
     int deletechildren(void)  {  return options.test(0);  }
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     void add(int position,Entity *);
     void add(Entity *);
     void del(Entity *);
     void fixupreferences(Entity * *index);
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     char *getcharclassname(void);
     char *getcharinstance(void);
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int,Transform *,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,const RCCHAR *,const RCCHAR *,const BitMask&);
     static int setup(BitMask *,RCCHAR *,RCCHAR *,BitMask *);
     static void getdefaults(RCCHAR **,RCCHAR **,BitMask *);
     static void setdefaults(RCCHAR *,RCCHAR *,const BitMask&);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int process_property(int action,int id,void *data,RCCHAR *text);

  };

class TerminatorInfo
  {public:
     Byte style,weight;
     double length,height;
     int save(CadFile *);
     int load(CadFile *);
  };

class TextInfo
  {
   public:
	 RCCHAR *font,*text;
     double width,height,angle,vertspacing;
     Byte halocolour;
     BitMask options;
     TextInfo() : options(32) 
	 {
		 font = 0;
	     text = 0;
	 }
     int save(CadFile *);
     int load(CadFile *);

  };

typedef int (*DimensionCallBack)(Entity *,void *);

class LineTerminator
  {private:
     double length,height;
     Byte weight;
   public:
     LineTerminator(double l,double h,Byte w)
       {  length = l;  height = h;  weight = w;
       }
     double getlength(void)  {  return length;  }
     double getheight(void)  {  return height;  }
     int getweight(void)  {  return weight;  }
#ifdef USING_WIDGETS
     void drawGL(EntityHeader *header,int drawmode,Transform *,View3dSurface *,CoordinateSystem *,int,double,double);
#endif
     void draw(EntityHeader *header,int drawmode,Transform *,View3dSurface *,CoordinateSystem *,int,double,double);
     void addtodisplaylist(EntityHeader *header,View3dSurface *,CoordinateSystem *,int,double,double);
     double pick(EntityHeader *header,View3dSurface *,int,int,Point *,CoordinateSystem *,int,double,double);
     int pick(EntityHeader *header,View3dSurface *,int,int,int,int,int,CoordinateSystem *,int,double,double);
     virtual int symmetric(void) = 0;
     virtual void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double) = 0;
     static LineTerminator *create(double s,const TerminatorInfo &);
     static RCCHAR *name(const TerminatorInfo &);
  };

class ArrowLineTerminator : public LineTerminator
  {public:
     ArrowLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class FilledArrowLineTerminator : public LineTerminator
  {public:
     FilledArrowLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class OpenArrowLineTerminator : public LineTerminator
  {public:
     OpenArrowLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class DotLineTerminator : public LineTerminator
  {public:
     DotLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class FilledDotLineTerminator : public LineTerminator
  {public:
     FilledDotLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class SlashLineTerminator : public LineTerminator
  {public:
     SlashLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {  }
     int symmetric(void) {  return 0;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double);
  };

class NoLineTerminator : public LineTerminator
  {public:
     NoLineTerminator(double l,double h,Byte w) : LineTerminator(l,h,w) {}
     void draw(Transform *,View3dSurface *,CoordinateSystem *,int,double,double) {  }
     void addtodisplaylist(EntityHeader *header,View3dSurface *,CoordinateSystem *,int,double,double) {}
     double pick(View3dSurface *,int,int,Point *,CoordinateSystem *,int,double,double) {  return -1.0;  }
     int pick(View3dSurface *,int,int,int,int,int,CoordinateSystem *,int,double,double) {  return 0;  }
     int symmetric(void) {  return 1;  }
     void generate(EntityHeader *,DimensionCallBack,void *,CoordinateSystem *,int,double,double) {}
  };

enum {  DimCommon = 0, DimStacked = 1, DimShowValue = 2, DimPrefixValueTextStyle = 3,DimSuffixValueTextStyle = 4,  DimTolerance1 = 5, DimTolerance2 = 6, DimPlacement1 = 7, DimPlacement2 = 8, DimLinePosition = 9,DimLineVisible = 10,ToleranceDefined = 11,GapFromDimension = 12,ScaleDefined = 13,ScaleText = 14,MoveTextDefined = 15, EquationDefined = 16, FixedDimensionPosition = 17, FixDimensionLineLength = 18, DimensionTextYOffset = 19, DimensionFreeLeader = 20, DimensionReverse = 21,HideLeftDimensionLine = 22, HideRightDimensionLine = 23, EnableDimensionOffsets = 24  };

class Dimension : public Entity
  {
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgihandle;
#endif
protected:
     double gap,extension,uppertol,lowertol,stackdis,scale,textyoffset;
     short precision,tolprecision;

     TextInfo valueinfo,toleranceinfo,prefixinfo,suffixinfo;
     TerminatorInfo term1,term2;
     BitMask options;
     PointList textpointlist;
     CoordinateSystem cs;
     RCCHAR *equation;
     virtual void generate(View3dSurface *,DimensionCallBack,void *) = 0;
     void formatvalue(RCCHAR *,RCCHAR*,double,int);
   public:
     Dimension() : options(32) {  defined = 0; qgi=0; qgihandle=0; }
     Dimension(const EntityHeader &,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     Dimension(double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     ~Dimension();
     Point getorigin(void)  {  return cs.getorigin();  }
     Point getxaxis(void)  {  return cs.getxaxis();  }
     Point getyaxis(void)  {  return cs.getyaxis();  }
     double getgap(void)  {  return gap;  }
     double getextension(void)  {  return extension;  }
     double gettextyoffset(void)  {  return textyoffset;  }
     double getuppertol(void)  {  return uppertol;  }
     double getlowertol(void)  {  return lowertol;  }
     double getstackdis(void)  {  return stackdis;  }
     double getscale(void)  {  return scale;  }
     short getprecision(void)  {  return precision;  }
     short gettolprecision(void)  {  return tolprecision;  }
     TextInfo *getvaluetextinfo(void)  {  return &valueinfo;  }
     TextInfo *getprefixtextinfo(void)  {  return &prefixinfo;  }
     TextInfo *getsuffixtextinfo(void)  {  return &suffixinfo;  }
     TextInfo *gettolerancetextinfo(void)  {  return &toleranceinfo;  }
     TerminatorInfo *getterminfo1(void)  {  return &term1;  }
     TerminatorInfo *getterminfo2(void)  {  return &term2;  }
     BitMask getoptions(void) {  return options;  }
     int issame(int,const Entity &) const;
     int isdimension(void) {  return 1;  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     virtual void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     virtual void drawGL(int,Transform *,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     virtual void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     void change(const BitMask&,const EntityHeader&,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     static int setup(BitMask *, double *, double *, double *, double *, double *, double *, double *, int *, int *, TerminatorInfo *term1, TerminatorInfo *term2, TextInfo *, TextInfo *, TextInfo *, TextInfo *, BitMask *, EntityHeader *header=0);
     static void getdefaults(double *,double *,double *,double *,double *,double *,double *,int *,int *,TerminatorInfo *term1,TerminatorInfo *term2,TextInfo *,TextInfo *,TextInfo *,TextInfo *,BitMask *);
     static void setdefaults(double, double, double, double, double, double, double, int, int, const TerminatorInfo &term1, const TerminatorInfo &term2, const TextInfo &, const TextInfo &, const TextInfo &, const TextInfo &, const BitMask&);
     static int setupleader(BitMask *,double *,double *,double *,double *,double *,double *,int *,int *,TerminatorInfo *term1,TerminatorInfo *term2,TextInfo *,TextInfo *,TextInfo *,TextInfo *,BitMask *,EntityHeader *header=0);
     static void getleaderdefaults(double *,double *,double *,double *,double *,double *,int *,int *,TerminatorInfo *term1,TerminatorInfo *term2,TextInfo *,TextInfo *,TextInfo *,TextInfo *,BitMask *);
     static void setleaderdefaults(double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int picktextindex(View3dSurface *,int x,int y,Point *p);
     void movetext(int i,Point p);
     virtual void addpoint(Point p,int update)  {};
     virtual void delpoint(Point p,int exactmatch,int update)  {};
     virtual void setoffset(int index,double offset)  {};
     virtual double getoffset(int index)  {  return 0.0;  };
     const RCCHAR *getequation(void)  {  return equation;  }
     void setequation( RCCHAR *eq)
     {
         delete equation;
         equation = new RCCHAR[strlen(eq)+1];
         strcpy(equation,eq);
     }

     int process_property(int action,int id,void *data,RCCHAR *text);
	 int process_property(int action,int id,void *data,char *text);

     void setdefaults(void);
  };

class LinearDimension : public Dimension
  {private:
     SortedPointList list;
     void generate(View3dSurface *,DimensionCallBack,void *);
   public:
     LinearDimension() : Dimension() {  defined = 0;  }
     LinearDimension(const EntityHeader &,Point,Point,Point,SortedPointList,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     LinearDimension(Point,Point,Point,SortedPointList,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     ~LinearDimension();
     SortedPointList getlist(void)  {  return list;  }
     Point GetTextPosition(int index);
     int type(void) const {  return lineardimension_entity;  }
     int isa(int type) const {  return lineardimension_entity == type;  }
     int match(const BitMask &mask) const {  return mask.test(lineardimension_entity) || mask.test(dimension_mask);  }
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     void addpoint(Point p,int update);
     void delpoint(Point p,int exactmatch,int update);
     void setoffset(int index,double offset);
     double getoffset(int index);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int,Transform *,View3dSurface *);
#endif
     void draw(int,Transform *,View3dSurface *);
  };

class AngularDimension : public Dimension
  {private:
     Point origin;
     PointList list;
     void generate(View3dSurface *,DimensionCallBack,void *);
   public:
     AngularDimension() : Dimension() {  defined = 0;  }
     AngularDimension(const EntityHeader &,Point,Point,Point,Point,PointList,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     AngularDimension(Point,Point,Point,Point,PointList,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     ~AngularDimension();
     Point getdorigin(void)  {  return origin;  }
     PointList getlist(void)  {  return list;  }
     int type(void) const {  return angulardimension_entity;  }
     int isa(int type) const {  return angulardimension_entity == type;  }
     int match(const BitMask &mask) const {  return mask.test(angulardimension_entity) || mask.test(dimension_mask);  }
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
  };

class DistanceBearingDimension : public Dimension
  {private:
     Point p1,p2;
     BitMask display;
     void generate(View3dSurface *,DimensionCallBack,void *);
   public:
     DistanceBearingDimension() : Dimension(),display(32) {  defined = 0;  }
     DistanceBearingDimension(const EntityHeader &,Point,Point,Point,Point,Point,BitMask&,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     DistanceBearingDimension(Point,Point,Point,Point,Point,BitMask&,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     ~DistanceBearingDimension();
     Point getp1(void)  {  return p1;  }
     Point getp2(void)  {  return p2;  }
     BitMask getdisplay(void)  {  return display;  }
     int type(void) const {  return distancebearingdimension_entity;  }
     int isa(int type) const {  return distancebearingdimension_entity == type;  }
     int match(const BitMask &mask) const {  return mask.test(distancebearingdimension_entity) || mask.test(dimension_mask);  }
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
  };

class CircularDimension : public Dimension
  {private:
     Point p1,p2;
     Circle *circle;
     BitMask style;
     void generate(View3dSurface *,DimensionCallBack,void *);
   public:
     CircularDimension() : Dimension(),style(32) {  defined = 0;  }
     CircularDimension(const EntityHeader &,Point,Point,Point,Point,Point,BitMask&,Circle *,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     CircularDimension(Point,Point,Point,Point,Point,BitMask&,Circle *,double,double,double,double,double,double,double,int,int,const TerminatorInfo &term1,const TerminatorInfo &term2,const TextInfo &,const TextInfo &,const TextInfo &,const TextInfo &,const BitMask&);
     ~CircularDimension();
     Point getp1(void)  {  return p1;  }
     Point getp2(void)  {  return p2;  }
     Circle *getcircle(void)  {  return circle;  }
     BitMask getstyle(void)  {  return style;   }
     int type(void) const {  return circulardimension_entity;  }
     int isa(int type) const {  return circulardimension_entity == type;  } 
     int match(const BitMask &mask) const {  return mask.test(circulardimension_entity) || mask.test(dimension_mask);  }
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     void fixupreferences(Entity * *index);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
#ifdef USING_WIDGETS
     void drawGL(int,Transform *,View3dSurface *);
#endif
     void draw(int,Transform *,View3dSurface *);
     int pickoriginpoint(View3dSurface *,int,int,int,Point *);
  };

class SurfaceEntity;
class Polygon3d;
class RenderImage;

typedef int (*PolygonCallBack)(SurfaceEntity *,void *,int,void *,void *);

class SurfaceEntity : public Entity
  {protected:
    int ntriangles;
    Point *triangles;
   public:
     SurfaceEntity(void) {  ntriangles = -1;  triangles = 0;  }
     SurfaceEntity(const EntityHeader &header) : Entity(header) {  ntriangles = -1;  triangles = 0;  }
     int issurface(void) {  return 1;  }
     virtual void nfacets(double,int *,int *) = 0;
     virtual int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *) = 0;
     virtual void triangulate(double error,Transform *t=0) = 0;
     int getntriangles(void)  {  return ntriangles;  }
     Point *gettriangles(void)  {  return triangles;  }
     void cleartriangles(void);
  };

class LoadedFigureInfo
  {private:
     RCCHAR *filename;
     EntityList list;
   public:
     LoadedFigureInfo(const RCCHAR *,EntityList);
     ~LoadedFigureInfo();
     RCCHAR *getlibrary(void) const {  return filename;  }
     RCCHAR *getfilename(void) const {  return filename;  }
     EntityList getlist(void) const {  return list;  }
  };

class LoadedFigureList : public GeneralList
  {public:
     int add(LoadedFigureInfo *lfi)  {  return GeneralList::add(lfi);  }
     LoadedFigureInfo *next(void)  {  return (LoadedFigureInfo *) GeneralList::next();  }
  };

class LoadedFigures
  {private:
     LoadedFigureList list;
   public:
     LoadedFigures();
     ~LoadedFigures();
     int load(const RCCHAR *,EntityList *);
     void clear(void);
  };


class Parameter
  {private:
     RCCHAR name[300];
     double value;
   public:
     Parameter(const RCCHAR *name,double value);
     Parameter(const char *name,double value);
     int GetValue(const RCCHAR *name,double *value);
     RCCHAR *GetName(void)  {  return name;  }
     double GetValue(void)  {  return value;  }
     void SetValue(double value_)  {  value = value_;  }
  };

class StringExpression
  {private:
     const RCCHAR *line;
     int pos,symboltype,valuetype;
     RCCHAR fname[300];
     double xvalue;
     GeneralList *dimensionvalues;
     int *type;
   public:
     StringExpression(const RCCHAR *l,GeneralList *dimensionvalues_);
     void nextsymbol(void);
     int expression(double *x);
     int term(double *x);
     int factor(double *x);
  };

//  Options usage :  Bit 0    Loaded figure
//                   Bit 1    2D/3D figure
//                   Bit 2    State is set
//                   Bit 3    Has associated unpacked data
//                   Bit 4    Loaded parametric figure
//                   Bit 5    Original Layer
//                   Bit 6    Original Colour
//                   Bit 7    Original Style
//                   Bit 8    Original Weight
//                   Bit 9    Disable 2D/3D figures, but allow state processing - Bit 1 must also be set.
//                   Bit 10   FigureOrientToView
//                   Bit 11   FigureOrientXYToView
//                   Bit 12   FigureAttachFile
//                   Bit 13   FigureUseAlternativeName
//                   Bit 14   FigureElevation
//                   Bit 15   FigureUseSize
//                   Bit 16   FigureSaveSizeInfo
//                   Bit 17   FigureHasClipBoundary
enum {  FigureLoaded, Figure2D3D, FigureState, FigureUnpacked, FigureParametric, FigureOriginalLayer, FigureOriginalColour, FigureOriginalStyle, FigureOriginalWeight, FigureEnableStates, FigureOrientToView, FigureOrientXYToView, FigureAttachFile, FigureUseAlternativeName, FigureElevation, FigureUseSize, FigureSaveSizeInfo, FigureHasClipBoundary  };

typedef void (*FigureCallback)(Entity *e,View3dSurface *vs,void *d);

class Figure : public SurfaceEntity  
{
#ifdef USING_WIDGETS
    QGraphicsItem *qhndl[2]; // 2 groups for handles
#endif
private:
     RCCHAR *filename,*altfilename;
     RCCHAR *rcattachment[3];
     char *attachment[3];
     long attachmentlength[3];
     Byte state1,state2;
     int fstyle,missing,disabledisplaylist;
     EntityList unpacked;
     EntityList entitylist[3];
     GeneralList parameterlist;
     BitMask options;
     Transform transform;
     Point size;
     short nclipboundary;
     Point *clipboundary;
     short ncopies;
     double parentscale;
     Point *mextents; // stores the bounding box of the figure
     void updatetransform(Point,Point,Point,Point,Point);
     // v8
     void updatesizetransform(Point,Point,Point,Point,Point);
     //
   public:
     Figure() : options(32) {  defined = 0;  missing = 0;  nclipboundary = 0;  clipboundary = 0; qgi=0; mextents=0; zValue=QGMINZVALUE; size.setxyz(0.0,0.0,0.0); }
     Figure(const EntityHeader &,RCCHAR *,RCCHAR *,Point,Point,Point,Point,Point,Point,int,Byte,Byte,Transform *,const BitMask&);
     Figure(RCCHAR *,RCCHAR *,Point,Point,Point,Point,Point,Point,int,Byte,Byte,Transform *,const BitMask&);
     ~Figure();
     int type(void) const {  return figure_entity;  }
     int isa(int type) const {  return figure_entity == type;  }
     RCCHAR *getlibrary(void)  {  return filename;  }
     RCCHAR *getfilename(void)  {  return filename;  }
     RCCHAR *getaltfilename(void)  {  return altfilename;  }
     Point getsize(void)  {  return size;  } 
     void setsize(Point s)  {  size = s;  } 
     int getncopies(void)  {  return ncopies;  } 
     void setncopies(int n)  {  ncopies = n;  } 
     RCCHAR *getcurrentfilename(void);
     RCCHAR *getarchitectfilename(int i);
     Point getorigin(void)    {Point zero(transform.mat(0,3),transform.mat(1,3),transform.mat(2,3));  return zero;  }
     Point getxaxis(void)  {Point zero(0.0,0.0,0.0);  return zero;  }
     Point getyaxis(void)  {Point zero(0.0,0.0,0.0);  return zero;  }
     Point getscale(void);
     Point getrotation(void);
     Byte getstate1(void)  {  return state1;  }
     Byte getstate2(void)  {  return state2;  }
     void setstate1(Byte state1_)  {  state1 = state1_;  }
     void setstate2(Byte state2_)  {  state2 = state2_;  }
     BitMask *getoptions(void)  {  return &options;  }
     int issame(int,const Entity &) const;
     RCCHAR *getlibrary(void) const {  return filename;  }
     RCCHAR *getfilename(void) const {  return filename;  }
     RCCHAR *getaltfilename(void) const {  return altfilename;  }
     void SetupMatrix(View3dSurface *);
     EntityList getunpacked(void)  {  return unpacked;  }
     int match(const BitMask &mask) const {  return mask.test(figure_entity);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void getsubfigures(CStringList *slist);
     void drawGL(int,Transform *,View3dSurface *);
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     int hideGL(HideImage *image);
     void ProcessFigureGL(View3dSurface *vs,Transform *t,FigureCallback fcb,void *d);
     void drawGLlod(Transform *,View3dSurface *);
     void deleteExtents() {delete [] mextents; mextents=0;}
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error, Transform *t=0);
     int cutout(int,RCCHAR *,EntityList *);
     double cutoutlength(int,RCCHAR *);
     int cutplane(Point,double,RCCHAR *,RCCHAR *,Point *,Point *,Point *,Point *,Point *);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     void fixupreferences(Entity * *index);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,RCCHAR *,RCCHAR *,Point,Point,Point,int nc,Byte,Byte,const BitMask&);
     static int setup(BitMask *, Point *, Point *, Byte *, Byte *, BitMask *, RCCHAR *fn=0, RCCHAR *afn=0, EntityHeader *inheader=0, Point *onOrigin=0, Point *size=0);
     static void getdefaults(Point *,Point *,Byte *,Byte *,BitMask *,Point *si=0);
     static void setdefaults(Point,Point,Byte,Byte,const BitMask&,Point *si=0);
     static int select(RCCHAR *, RCCHAR *);
     void newtransform(Point org,Point xa,Point ya,Point scale,Point rotation);
     // v8
     void newsizetransform(Point org,Point xa,Point ya,Point scale,Point rotation);
     //
     void getsubfigures(StringList *l);
     int defaultsfrommatrix(Point,Point,Point *,Point *);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int getstate(int s) {  return (s == 0 ? state1 : state2);  }
     int getinfo(RCCHAR *,RCCHAR *,RCCHAR *);
     void addattributetext(void);
     Transform *gettransform(void)  {  return & transform;  }
     void settransform(Transform t)  {  transform = t;  }
     int getmissing(void)  {  return missing;  }
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     void loadfigure(GeneralList *list);
     void firstparameter();
     int getparameterlistlength()  {  return parameterlist.length();  }
     int nextparameter(RCCHAR *,double *);
     EntityList getentitylist(void);
     int getentitycount();
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     int pickoriginpoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
     void attachfigure(void);
     void detachfigure(void);
     void ProcessFigure(View3dSurface *vs,Transform *t,FigureCallback fcb,void *d);
     void ProcessFigureEntity(int arch,double fscale,const BitMask *,Entity *e,View3dSurface *vs,FigureCallback fcb,void *d);
     void SetClipBoundary(int n,Point *cb)  {  nclipboundary = n;  clipboundary = cb;  }

     int process_property(int action,int id,void *data,RCCHAR *text);

     void setdefaults(void);


     static bool ResolveLibraryPath(RCCHAR *figurenamewithnopath, RCCHAR *foundpath, int sizeofpath);
  };

class Plane;


//  options use
//    Bit 1    :  Column entity
//    Bit 2    :  Wall entity - close the start
//    Bit 3    :  Wall entity - close the end

enum {  ArWallWidth2, ArWallHeight2, ArWallGap, ArWallCavity, ArStudSpacing, ArMaxWallAttributes  };
enum {  ArWallColumnBit = 1, ArWallCloseStartBit, ArWallCloseEndBit, ArWallAttributes, ArWallFrameExtendStart, ArWallFrameExtendEnd, ArWallFrame, ArStudsUnderTrusses  };

//
//  Architectural Layers
//
//    Roof entities

#define EAVE_LAYER               700
#define PITCHING_LAYER           701
#define RIDGE_LAYER              702
#define HIP_LAYER                703
#define VALLEY_LAYER             704
#define DUTCHGABLE_LAYER         705
#define ROOFPLAN_LAYER           706
#define FACIA_LAYER              707
#define TILES_LAYER              708
#define ROOF_LAYER               709
#define ROOFDIMENSIONS_LAYER     710
#define DUTCHGABLEOVER_LAYER     711
#define DUTCHGABLEBASE_LAYER     712
#define DUTCHGABLEOVERHIP_LAYER  713

//    Truss entities

#define FULLTRUSS_LAYER                  720
#define HALFTRUSS_LAYER                  721
#define HIPTRUSS_LAYER                   722
#define TRUSSPLAN_LAYER                  723
#define TRUSSTEXT_LAYER                  724
#define HIPHALFTRUSS_LAYER               725
#define DUTCHGABLEHALFTRUSS_LAYER        726
#define DUTCHGABLEFULLTRUSS_LAYER        727
#define HIPFULLTRUSS_LAYER               728
#define VERTICALWEBTRUNCATEDTRUSS_LAYER  729
#define TRUNCATEDTRUSS_LAYER             730
#define BEAM_LAYER                       731

//  Truss and Wall frame members
#define TOPCHORD1_LAYER             850
#define TOPCHORD2_LAYER             851
#define TOPCHORD3_LAYER             852
#define BOTTOMCHORD_LAYER           853
#define WEB_LAYER                   854
#define WEB1_LAYER                  855
#define WEB2_LAYER                  856
#define WEB3_LAYER                  857
#define WEB4_LAYER                  858
#define CLIP_LAYER                  859
#define NOTCH_LAYER                 860
#define TOPPLATE_LAYER              861
#define BOTTOMPLATE_LAYER           862
#define NOGGING_LAYER               863
#define SILL_LAYER                  864
#define LINTEL_LAYER                865
#define STUD_LAYER                  866
#define BOLT_LAYER                  867
#define HALFHIPNOTCH_LAYER          868
#define HIPFULLNOTCH_LAYER          869

//    Wall frame entities - frame position
#define WALLFRAMING_LAYER           750
#define WALLFRAMEBRACING_LAYER      751

class Wall : public SurfaceEntity
  {private:
     Point p1,p2,zaxis;
     double angle1,angle2,width,height,eaves,pitch,peakdistance,peakheight,lhatchspacing,rhatchspacing,lhatchangle,rhatchangle,*attributes;
     short middlecolour,rightcolour,nattributes;
     RCCHAR *name;
     BitMask options,options2;
     EntityList openinglist;
     SortedDoubleList break2dlist1;
     SortedDoubleList break2dlist2;
     SortedDoubleList break2dlist3;
     SortedDoubleList break2dlist4;
   public:
     Wall() : options(32),options2(32) {  defined = 0;  }
     Wall(const EntityHeader&, Point,Point,Point,RCCHAR *,RCCHAR *strWM,double,double,double,double,double,double,int,int,double,double,double,double,double,double,short,double *,const BitMask&,const BitMask&,bool onInsert=false);
     Wall(                     Point,Point,Point,RCCHAR *,RCCHAR *strWM,double,double,double,double,double,double,int,int,double,double,double,double,double,double,short,double *,const BitMask&,const BitMask&,bool onInsert=false);
     ~Wall();
     Point getp1(void)  {  return p1;  }
     Point getp2(void)  {  return p2;  }
     Point getzaxis(void)  {  return zaxis;  }
     RCCHAR *getname(void)  {  return name;  }
     double getwidth(void)  {  return width;  }
     double gettotalwidth(void) const;
     double getheight(void)  {  return height;  }
     double geteaves(void)  {  return eaves;  }
     double getpitch(void)  {  return pitch;  }
     int getmiddlecolour(void)  {  return middlecolour;  }
     int getrightcolour(void)  {  return rightcolour;  }
     double getpeakdistance(void)  {  return peakdistance;  }
     double getpeakheight(void)  {  return peakheight;  }
     double getlhatchspacing(void)  {  return lhatchspacing;  }
     double getrhatchspacing(void)  {  return rhatchspacing;  }
     double getlhatchangle(void)  {  return lhatchangle;  }
     double getrhatchangle(void)  {  return rhatchangle;  }
     double getangle1(void)  {  return angle1;  }
     double getangle2(void)  {  return angle2;  }
     double getattribute(int index)  {  return attributes[index];  }
     short getnattributes(void)  {  return nattributes;  }
     double *getattributes(void)  {  return attributes;  }

     Plane *getleaf(RCCHAR *leaf);
     int type(void) const {  return wall_entity;  }
     int isa(int type) const {  return wall_entity == type;  }
     int isacolumn(void)  {  return options.test(1);  }
     BitMask *getoptions(void)  {  return &options;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(wall_entity);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     EntityList getopeninglist(void)  {  return openinglist;  }
     int add(Figure *);
     void del(Figure *);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error,Transform *t=0);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     void fixupreferences(Entity **index);
     int pickpoint(View3dSurface *,int,int,int,Point *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,RCCHAR *,RCCHAR *strWM,double,double,double,double,double,double,int,int,double,double,double,double,short,double *,const BitMask&,const BitMask&);
     EntityList changegeometry(Point,Point,double,double);
     static int setup(BitMask *,RCCHAR *,RCCHAR *strWaterMark,double *,double *,double *,double *,double *,double *,int *,int *,double *,double *,double *,double *,short,double *,BitMask *,BitMask *,Wall *bwall=NULL);
     static void getdefaults(BOOL bFromProfile, RCCHAR *,RCCHAR *,double *,double *,double *,double *,double *,double *,int *,int *,double *,double *,double *,double *,short *,double *,BitMask *,BitMask *);
     static void setdefaults(RCCHAR *,RCCHAR *strWaterMark,double,double,double,double,double,double,int,int,double,double,double,double,short,double *,const BitMask&,const BitMask&);
     void verify(void);
     int explode(void);
     void breakwall(Wall *);
     int unbreakwall(Wall *);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     double getarea(int nsegs,double error);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int frame(Point,Point *,short viewno,int frameno);
     int process_property(int action,int id,void *data,RCCHAR *text);
     void setdefaults(void);
  };


class Beam : public SurfaceEntity
  {
public:
	enum BeamTypes
	{
		NotDefinedType		=0,
		SupaFloorBearer	=1,
		SupaFloorJoist	=2
	};
	enum BeamProfile
	{
		NotDefinedProfile		=0,
		CSection,
		CSectionNoLip
	};
	enum BeamAttributes
	{
		ArBeamCrimpStart,
		ArBeamCrimpEnd,
		ArMaxBeamAttributes
	};
	enum BeamOptions
	{
	};
private:
	 int iType;
	 int iProfile;
     Point ptStart;
	 Point ptEnd;
	 Point ptZAxis;
     double dblWidth;
	 double dblHeight;
	 double dblThickness;
	 double *attributes;
     short nattributes;
     char *strName;

	 int iDrawCentreLine;
	 int iCentreLineColour;
	 int iCentreLineStyle;
	 double dblCentreLineWeight;

	 int iDrawOutsideLineBold;
	 int iOutsideLineColour;
	 int iOutsideLineStyle;
	 double dblOutsideLineWeight;

	 int iDrawInsideLineBold;
	 int iInsideLineColour;
	 int iInsideLineStyle;
	 double dblInsideLineWeight;

     BitMask options, optTopPlane, optBottomPlane, optOutsidePlane, optInsidePlane;

   public:
	 Beam() : options(32), optTopPlane(32), optBottomPlane(32), optOutsidePlane(32), optInsidePlane(32)  {  defined = 0;  }
     Beam(const EntityHeader&,short _iType, short _iProfile, Point _p1,Point _p2,Point _zaxis,char *n,char *_strLabel,double _width,double _height,double _dblThickness,double _dblLip,short nattributes_,double *attributes_,const BitMask &o);
     Beam(short _iType, short _iProfile, Point _ptStart,Point _ptEnd,Point _ptZAxis,char *n,char *_strLabel,double _dblWidth,double _dblHeight, short nattributes_,double *attributes_,const BitMask &o);
     Beam(short _iType, short _iProfile, Point _ptStart,Point _ptEnd,Point _ptZAxis,char *n,char *_strLabel,double _dblWidth,double _dblHeight,double _dblThickness,double _dblLip, short nattributes_,double *attributes_,const BitMask &o);
     ~Beam();

     Point GetStartPoint(void)  {  return ptStart;  }
     Point GetEndPoint(void)  {  return ptEnd;  }
	 double GetLength(void) { return (ptEnd-ptStart).length(); }
     Point GetZAxis(void)  {  return ptZAxis;  }
	 short GetBeamType(void) { return iType; }
	 short GetBeamProfile(void) { return iProfile; }
     char *GetName(void)  {  return strName;  }
     double GetWidth(void)  {  return dblWidth;  }
     double GetHeight(void)  {  return dblHeight;  }
     void SetHeight(double _height)  { dblHeight = _height;  }
     double GetAttribute(int index)  {  return attributes[index];  }
     short GetNumberAttributes(void)  {  return nattributes;  }
     double *GetAttributes(void)  {  return attributes;  }
	 Point GetDirection(void) { return (ptEnd-ptStart).normalize(); }

	 void SetDefaultLineProperties();
	 void SetCentreLineProperties(int iState, int iColour, int iStyle, double dblWeight);
	 void SetOutsideLineProperties(int iState, int iColour, int iStyle, double dblWeight);
	 void SetInsideLineProperties(int iState, int iColour, int iStyle, double dblWeight);

	 //Sets bits out of the options bitmaks
     Plane *getleaf(char *leaf);
     int type(void) const {  return beam_entity;  }
     int isa(int type) const {  return beam_entity == type;  }
     BitMask *getoptions(void)  {  return &options;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(beam_entity);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int add(Figure *);
     void del(Figure *);
     int add(Line *);
     void del(Line *);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     void fixupreferences(Entity * *index);
     int pickpoint(View3dSurface *,int,int,int,Point *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask &change,const EntityHeader &eh,char *strBeamType,double dblWidth,double dblHeight,double dblThickness, double dblLip, int iType, 
            int iDrawOutsideLineBold, int iOutsideLineColour, int iOutsideLineStyle, double dblOutsideLineWeight,
            int iDrawCentreLine, int iCentreLineColour, int iCentreLineStyle, double dblCentreLineWeight,
            int iDrawInsideLineBold, int iInsideLineColour, int iInsideLineStyle, double dblInsideLineWeight,
            short nattributes,double *attributes,BitMask &o);
     EntityList changegeometry(Point pn1,Point pn2);
     static int setup(BitMask *change,char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
            int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
            int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
            int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
            short *nattributes_,double *attributes_,BitMask *o);
     static void getdefaults(BOOL bFromProfile, char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
            int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
            int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
            int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
            short *nattributes,double *attributes,BitMask *o);
     static void setdefaults(char *strBeamType,double dblWidth,double dblHeight,double dblThickness, double dblLip, int iType,
            int iDrawOutsideLineBold, int iOutsideLineColour, int iOutsideLineStyle, double dblOutsideLineWeight,
            int iDrawCentreLine, int iCentreLineColour, int iCentreLineStyle, double dblCentreLineWeight,
            int iDrawInsideLineBold, int iInsideLineColour, int iInsideLineStyle, double dblInsideLineWeight,
            short nattributes,double *attributes,BitMask &o);
	 void change(const BitMask &change,const EntityHeader &eh,char *n,double w,double h,short nattributes_,double *attributes_,const BitMask &op);
     void getinfo(char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
               int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
               int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
               int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
               short *nattributes,double *attributes,BitMask *o);
	 void getinfo(char *name,double *w,double *h,short *nattributes,double *attributes,BitMask *o);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     double getarea(int nsegs,double error);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
};

#if defined(WIN32)
typedef unsigned char *PixelBuffer;
#else
typedef unsigned char huge *PixelBuffer;
#endif

enum {  BitmapESameShape,BitmapEHJust1,BitmapEHJust2,BitmapEVJust1,BitmapEVJust2,
        BitmapEStyle1,BitmapEStyle2,BitmapEStyle3,BitmapEStoreJPEG,
        BitmapETransparent,BitmapELinkFile,BitmapENewFormat
     };

class BitMapE : public Entity
  {
#ifdef USING_WIDGETS
    QGraphicsItem *qhndl[4]; // 4 groups for handles
#endif
private:
     RCCHAR *filename;
     Point origin,xaxis,yaxis;
     double width,height,transparencyrange,opacity,red,green,blue;
     int originallength;
     RCIMAGE *qbmi;
     //QPixmap *qbmi;
     BITMAPINFO *bmi;
     PixelBuffer pbuffer;
     PixelBuffer originalbuffer;
     BitMask options;
     int sizeofbmi(void);
     long sizeofbuffer(void);
     int loadjpeg(FILE *infile);
     int loadpng(RCCHAR *name);
     int load(RCCHAR *name);
     static int getshapejpeg(FILE *infile,long *w,long *h);
   public:
#ifdef USING_WIDGETS
    //bool usedByPlane; // defined in base class
    //bool usedByFigure;// defined in base class
#endif
     BitMapE() : options(32)
     {
         defined = 0;
         filename = 0;
         pbuffer=0;
         bmi=0;
         options.clearall();
#ifdef USING_WIDGETS
         usedByPlane=false;
         usedByFigure=false;
         qbmi=0;
         originalbuffer=0;
         originallength=0;
         for(int i=0; i<4; i++)
              qhndl[i]=0;
#endif
     }
     BitMapE(const EntityHeader &,RCCHAR *name,RCIMAGE *,PixelBuffer,Point,Point,Point,double,double,double,double,double,double,double,BitMask&);
     BitMapE(const EntityHeader &,RCCHAR *name,BITMAPINFO *,PixelBuffer,Point,Point,Point,double,double,double,double,double,double,double,BitMask&, int version=0);
     BitMapE(const EntityHeader &,RCCHAR *,Point,Point,Point,double,double,double,double,double,double,double,BitMask&);
     BitMapE(RCCHAR *,Point,Point,Point,double,double,double,double,double,double,double,BitMask&);
     ~BitMapE();
     static int getshape(RCCHAR *name,long *w,long *h);
     Point getorigin(void)  {  return origin;  }
     void setorigin(Point o)  {  origin = o;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
#ifdef USING_WIDGETS
     void setxaxis(Point xa) { xaxis = xa; }
     void setyaxis(Point ya) { yaxis = ya; }
#endif
     RCCHAR *getfilename(void)  {  return filename;  }
     double getwidth(void)  {  return width;  }
     double getheight(void)  {  return height;  }
     double getopacity(void)  {  return opacity;  }
     double gettransparencyrange(void)  {  return transparencyrange;  }
     double getred(void)  {  return red;  }
     double getgreen(void)  {  return green;  }
     double getblue(void)  {  return blue;  }
     RCIMAGE *getqbmi(void)  {  return qbmi;  }
     //QPixmap *getqbmi(void)  {  return qbmi;  }
     BITMAPINFO *getbmi(void)  {  return bmi;  }
     PixelBuffer getpbuffer(void)  {  return pbuffer;  }
     BitMask getoptions(void)  {  return options;  }
     void setoptions(const BitMask &mask)  { options = mask; }
     int type(void) const {  return bitmap_entity;  }
     int isa(int type) const {  return bitmap_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(bitmap_entity);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int drawmode,Transform *t,View3dSurface *vs);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,RCCHAR *,double,double,double,double,double,double,double,const BitMask&);
     static int setup(BitMask *,double *,double *,double *,double *,double *,double *,double *,BitMask *,RCCHAR *fn=0,Point *origin=0,EntityHeader *header=0);
     static void getdefaults(double *,double *,double *,double *,double *,double *,double *,BitMask *);
     static void setdefaults(double,double,double,double,double,double,double,const BitMask&);
     static int select(RCCHAR *);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     void transpose(void);
     void reverse(int);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int pickhandlepoint(View3dSurface *,int,int,int,Point *);
     void movehandlepoint(View3dSurface *,int,int,Point);
     int process_property(int action,int id,void *data,RCCHAR *text);

     void setdefaults(void);

  };


enum PlaneOptions { PlaneUnknownState = 31, PlaneFilled = 0, PlanePatterned = 1, PlaneAnchored = 3, PlaneBitmapFilled = 4, PlaneGradientFilled = 5, PlaneNoFill = 6, PlaneGradientStyle1 = 7,PlaneGradientStyle2 = 8,PlaneGradientStyle3 = 9,PlaneGradientStyle4 = 10, PlaneDisplayEdges = 11, PlaneHasTransparency = 12, PlaneHasTransparentColour = 13 };

class RCGraphicsPlaneItem;

class Plane : public SurfaceEntity
  {
#ifdef USING_WIDGETS
    public:
    // cache for the bitmap fill file
     BitMapE *bitmape;
     RCGraphicsPlaneItem *qgplane;
     QGraphicsItem *qgrenderplane;
     EntityList patternEnts;
#endif
   private:
     Point origin,xaxis,yaxis,anchor;
     EntityList list;
     BitMask options;
     double xspacing,yspacing,angle,crossangle,transparency,transparencyrange,red,green,blue;
     int drawnhighlighted;
     RCCHAR *patternfilename;
     RCCOLORREF gradientcolour;
   public:
     Plane() : options(32) {  defined = 0; bitmape=0; qgi=0; qgplane=0; patternfilename =0; qgrenderplane=0; zValue=QGMINZVALUE; }
     Plane(const EntityHeader &,EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     Plane(EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     Plane(const EntityHeader &,Point,Point,Point,EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     Plane(Point,Point,Point,EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     Plane(const EntityHeader &,Point,Point,Point,Point,EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     Plane(Point,Point,Point,Point,EntityList,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     ~Plane();
     Point getorigin(void)  {  return origin;  }
     Point getAnchor(void)  {  return anchor;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     EntityList getlist(void)  {  return list;  }
     void setlist(EntityList l)  {  list = l;  }
     BitMask getoptions(void)  {  return options;  }
     double getxspacing(void)  {  return xspacing;  }
     double getyspacing(void)  {  return yspacing;  }
     double gettransparency(void)  {  return transparency;  }
     double gettransparencyrange(void)  {  return transparencyrange;  }
     double getred(void)  {  return red;  }
     double getgreen(void)  {  return green;  }
     double getblue(void)  {  return blue;  }
     double getangle(void)  {  return angle;  }
     double getcrossangle(void)  {  return crossangle;  }
     RCCOLORREF getgradientcolour(void)  {  return gradientcolour;  }
     RCCHAR *getpatternfilename(void)  {  return patternfilename;  }
     int type(void) const {  return plane_entity;  }
     int isa(int type) const {  return plane_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(plane_entity) || mask.test(surface_mask);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int mode,Transform *t,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     double getarea(int nsegs,double error);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void nfacets(double,int *nhor,int *nver)  {  *nhor = *nver = 0;  }
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error, Transform *t=0);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     Entity *deepclone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     static int setup(BitMask *,double *,double *,double *,double *,double *,double *,double *,double *,double *,RCCOLORREF *,RCCHAR *,BitMask *,EntityHeader *header=0, BitMask *dgc=0);
     static void getdefaults(double *,double *,double *,double *,double *,double *,double *,double *,double *,RCCOLORREF *,RCCHAR *,BitMask *);
     static void setdefaults(double,double,double,double,double,double,double,double,double,RCCOLORREF,RCCHAR *,const BitMask&);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     void fixupreferences(Entity * *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     Entity *AddVertex(Line *line1,Line *line2,Entity **newedges);
     void AddEdge(Point p,Line *l);
     void DeleteEdge(Line *l);
     void ReplaceEdges(Line *line,Point p);
     void ReCalculatePlane(void);
     void AlignPlane(Point p1,Point p2);
     void SetAnchorPoint(Point p,int draw);
     void SetAnchorPoint(Point p) { anchor = p; }
     void Cut(Point o,Point n,EntityList **list1,EntityList **list2);
     void Offset(double offset,double height,EntityList **list);
     int LiesOn(Point p);
     int process_property(int action,int id,void *data,RCCHAR *text);
     void setdefaults(void);
   };

class Patch;

class UVSurfaceEntity : public SurfaceEntity
  {protected:
     short xmesh,ymesh;
   public:
     UVSurfaceEntity(void) {}
     UVSurfaceEntity(const EntityHeader &header) : SurfaceEntity(header) {};
     Patch *triangulatedclone(int showinterface);
     virtual Point position(double,double) const = 0;
     virtual Point normal(double,double) const = 0;
     int isuvsurface(void) {  return 1;  }
     int getxmesh(void)  {  return xmesh;  }
     int getymesh(void)  {  return ymesh;  }
  };


enum HiddenLineDrawMode {  HLNormal, HLEdges, HLEdgesOnly, HLEdgesBorder  };

class RuledSurface : public UVSurfaceEntity
  {private:
     LinearEntity *e1,*e2;
     BitMask options;
   public:
     Point position(double,double) const;
     Point normal(double,double) const;
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error,Transform *t=0);
     RuledSurface() : options(32) {  defined = 0;  }
     RuledSurface(const EntityHeader &,LinearEntity *e1,LinearEntity *e2,int,int,const BitMask&);
     RuledSurface(LinearEntity *e1,LinearEntity *e2,int,int,const BitMask&);
     ~RuledSurface();
     int type(void) const {  return ruledsurface_entity;  }
     int isa(int type) const {  return ruledsurface_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(ruledsurface_entity) || mask.test(surface_mask) | mask.test(uvsurface_mask);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
#ifdef USING_WIDGETS
     void drawGL(int,Transform *,View3dSurface *);
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     int hideGL(HideImage *image);
#endif
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void)  {  return 0;  }
     Point point(int)  {Point tmp(0.0,0.0,0.0);  return tmp;  }
     Point nearp(Point);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     Entity *deepclone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,int,int,const BitMask&);
     static int setup(BitMask *,int *,int *,BitMask *,EntityHeader *header=0);
     static void getdefaults(int *,int *,BitMask *);
     static void setdefaults(int,int,const BitMask&);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     void fixupreferences(Entity * *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     LinearEntity *gete1(void)  {  return e1;  }
     LinearEntity *gete2(void)  {  return e2;  }
     BitMask getoptions(void)  {  return options;  }
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int process_property(int action,int id,void *data,RCCHAR *text);
     void setdefaults(void);
  };

class SurfaceOfRev : public UVSurfaceEntity
  {private:
     LinearEntity *e;
     double angle,deltaz;
     Point origin,axis;
     BitMask options;
   public:
     Point position(double,double) const;
     Point normal(double,double) const;
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error,Transform *t=0);
     SurfaceOfRev() : options(32) {  defined = 0;  }
     SurfaceOfRev(const EntityHeader &,Point,Point,double,double,LinearEntity *e,int,int,const BitMask&);
     SurfaceOfRev(Point,Point,double,double,LinearEntity *e,int,int,const BitMask&);
     ~SurfaceOfRev();
     int type(void) const {  return surfaceofrev_entity;  }
     int isa(int type) const {  return surfaceofrev_entity == type;  } 
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(surfaceofrev_entity) || mask.test(surface_mask) || mask.test(uvsurface_mask);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
#ifdef USING_WIDGETS
     void drawGL(int,Transform *,View3dSurface *);
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     int hideGL(HideImage *image);
#endif
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void)  {  return 0;  }
     Point point(int)  {Point tmp(0.0,0.0,0.0);  return tmp;  }
     Point nearp(Point);
     void dbinsert(Database *,int draw = 3);
     void dbdelete(void);
     void dbsetassociated(void);
     void dbtraverseassociated(EntityCallBack,void *);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     Entity *deepclone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,Point,Point,double,double,int,int,const BitMask&);
     static int setup(BitMask *, double *, double *, int *, int *, BitMask *, EntityHeader *header=0);
     static void getdefaults(double *,double *,int *,int *,BitMask *);
     static void setdefaults(double,double,int,int,const BitMask&);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     void fixupreferences(Entity * *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     LinearEntity *gete(void)  {  return e;  }
     double getangle(void)  {  return angle;  }
     double getdeltaz(void)  {  return deltaz;  }
     Point getorigin(void)  {  return origin;  }
     Point getaxis(void)  {  return axis;  }
     BitMask getoptions(void)  {  return options;  }
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     int process_property(int action,int id,void *data,RCCHAR *text);
     void setdefaults(void);
  };


enum {  PatchEdgeDisplayBit1 = 1, PatchEdgeDisplayBit2, PatchTriangulated  };

typedef void (*PatchCallBack)(View3dSurface *vs,Point p1,Point p2,void *);

class Patch : public UVSurfaceEntity
  {
#ifdef USING_WIDGETS
    QGraphicsItemGroup *qgHLines;
#endif
    private:
     short un,vn,udegree,vdegree;
     int ubasis,vbasis,property_u_index,property_v_index;
     Point *polygon;
     long *colour_map;
     long nindex,nvertices,*index;                //  Indexs into the points stored in polygon
     double  *uknot,*vknot,*cweight,*utknot,*vtknot,*utbasis,*vtbasis,*utdbasis,*vtdbasis;
     EntityList trimcurves;
     BitMask options;
     void rbasis(int,int,double *,double,double *,int *) const;
     void rdbasis(int,int,double *,double,double *,double *,int *) const;
     void tempknots(int,double **);
     void generate_triangulated_patch_edges(View3dSurface *vs,Transform *t,PatchCallBack pcb,void *);
   public:
     Point position(double,double) const;
     Point normal(double,double) const;
     void nfacets(double,int *,int *);
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error,Transform *t=0);
     Patch() : options(32) {  defined = 0; polygon=0; index=0; qgi=0; }
     Patch(const EntityHeader &,int n1,int n2,int d1,int d2,int,int,Point *p,double *k1,double *k2,
             double *w,EntityList tcurves,const BitMask &o);
     Patch(int n1,int n2,int d1,int d2,int,int,Point *p,double *k1,double *k2,
             double *w,EntityList tcurves,const BitMask &o);
     Patch(long nvertices,Point *polygon,long nindex,long *index,const BitMask &o);
     Patch(const EntityHeader &,long nvertices,Point *polygon,long nindex,long *index,const BitMask &o);
     ~Patch();
     int type(void) const {  return patch_entity;  }
     int isa(int type) const {  return patch_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(patch_entity) || mask.test(surface_mask) || mask.test(uvsurface_mask);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void drawGL(int,Transform *,View3dSurface *);
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     void generate_triangulated_patch_edges(PatchCallBack pcb);
     void delete_triangulated_patch_vertex(int n,int keep,int *delete_index);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void movepoints(int,int,int *,Transform *t);
     void newbasis(void);
     void deletebasis(void);
     void newdbasis(void);
     void deletedbasis(void);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,int,int,int,int,const BitMask&);
     static int setup(BitMask *, int *, int *, int *, int *, BitMask *, EntityHeader *header=0);
     static void getdefaults(int *,int *,int *,int *,BitMask *);
     static void setdefaults(int,int,int,int,const BitMask&);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     short getun(void)  {  return un;  }
     short getvn(void)  {  return vn;  }
     short getudegree(void)  {  return udegree;  }
     short getvdegree(void)  {  return vdegree;  }
     Point *getpolygon(void)  {  return polygon;  }
     double *getuknot(void)  {  return uknot;  }
     double *getvknot(void)  {  return vknot;  }
     double *getcweight(void)  {  return cweight;  }
     EntityList gettrimcurves(void)  {  return trimcurves;  }
     BitMask getoptions(void)  {  return options;  }
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
     void set_colour_map(int index,long colour);
     void clear_colour_map(void);
     int process_property(int action,int id,void *data,RCCHAR *text);
     void setdefaults(void);
};

class NCSolid : public SurfaceEntity
  {private:
     Point origin,xaxis,yaxis,zaxis,lasttoolposition,dmin,dmax;
     short nx,ny,nz,twidth,xmesh,ymesh,dirty;
     double delta;
     double *sheight;
     double *theight;
   public:
     NCSolid() {  defined = 0;  }
     NCSolid(const EntityHeader &,Point origin,Point xaxis,Point yaxis,Point size,int maxsegments,int,int);
     NCSolid(Point origin,Point xaxis,Point yaxis,Point size,int maxsegments,int,int);
     NCSolid(const EntityHeader &,Point origin,Point xaxis,Point yaxis,double delta,short nx,short ny,short nz,short xmesh,short ymesh,double *sheight);
     ~NCSolid();
     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     int type(void) const {  return ncsolid_entity;  }
     int isa(int type) const {  return ncsolid_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(ncsolid_entity) || mask.test(surface_mask);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     void nfacets(double,int *nx1,int *ny1)  {  *nx1 = 0;  *ny1 = 0;  }
     int facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *);
     void triangulate(double error,Transform *t=0);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     int savedxf(int,DXFOut *) {  return 1;  };
     void definetool(int shape,double diameter,double angle,double length);
     void removetool(Point p);
     void cleardirty(void)  {  dirty = 0;  }
     int getdirty(Point *ptmin,Point *ptmax)  {  *ptmin = dmin;  *ptmax = dmax;  return dirty;  }
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);
   };

//
//  Various classes used to create entity loops and polygons
//

class CalculatePlane
  {private:
     Vector X1,X2,X3,B1,B2,B3;
     Matrix A1,A2,A3;
     Point origin,xaxis,yaxis,zaxis;
     double D;
     long npoints;
   public:
     CalculatePlane() : X1(3),X2(3),X3(3),B1(3),B2(3),B3(3),A1(3),A2(3),A3(3) {}
     void initialize(void);
     void add(Point);
     double check(Point);
     int planar(int,Point *,Point *,Point *,Point *,double *);
  };

class Loops
  {private:
     int closed,isclosed;
     EntityList list;
     Point start,end;
   public:
     Loops(int c,EntityList l) {  list = l;  closed = c;  }
     Loops(int c) {  closed = c;  }
     void create(EntityList *,int draw = 1);
     void create(View3dSurface *,Point,Point,Point,Point);
     void trace(View3dSurface *,LinearEntity *,Point,Point,Point,Point,double);
     void destroy(void)   {  list.destroy();  }
     EntityList getloops(void) {  return list;  }
     Point getstart() {  return start;  }
     Point getend()  {  return end;  }
     void reverse(void);
     int linearorplanar(Entity *);
     int linear(Point *,Point *);
     int planar(Point *,Point *,Point *,Point *,double *);
     int offset(int insert,int cornermode,Point p,double d,EntityList *);
     int get_offset_side(Point p,double d);
     int nloops(void);
  };

typedef void (*HTCallback)(Point,Point,void *);

struct UV
  {double u,v;
     UV() {}
     UV(double u1,double v1) {  u = u1;  v = v1;  }
  };

class Polygon3d
{
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi; // this is for the clipping/filling path
    int *loopDirections; // values: 1 (could be cw or ccw) or -1 (opposite direction to 1); 0 means undefined
    int *loopContainments; // values: +1 not contained -1 contained
#endif
protected:
     int nloops;
     int *nvert;
     Point origin,xaxis,yaxis,pmin,pmax;
     Point **verts;
     Point **normals;
     UV **uvs;
     int defined;
   public:
     Polygon3d(Point,Point,Point,EntityList,int nsegs,double);
     Polygon3d(Point,Point,Point);
     Polygon3d(Point,Point,Point,Point);
     Polygon3d(Point,Point,Point,Point *,UV *);
     Polygon3d(Point,Point,Point,int,int *,Point **);
     ~Polygon3d();
     int getdefined(void)  {  return defined;  }
     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     Point getpmin(void)  {  return pmin;  }
     Point getpmax(void)  {  return pmax;  }
     Point **getverts(void)  {  return verts;  }
     int *getnvert(void)  {  return nvert;  }  
     int getnloops(void)  {  return nloops;  }  
     int clip(Point,double);
     int clip(View3dSurface *);
     int hide(View3dSurface *,Point,Point,Segments *);
     int addedges(SurfaceEntity *,RenderImage *);
     void transform(Transform *);
     void modeltoview(View3dSurface *);
     void modeltoscreen(View3dSurface *);
     int hatchline(int,Point,double,Point,Point,HTCallback,void *);
     double pick(View3dSurface *,double,double,Point *);
#ifdef USING_WIDGETS
     void findLoopDirections();
     int findLoopDirection(int lid);
     int getLoopDirection(int lid);
     void findLoopContainments();
     int findLoopContainment(int lid);
     int getLoopContainment(int lid);
     void fixLoopDirections();
     void reverseLoopDirection(int lid);
     int getLoopSense(int lid);
     //QGraphicsItem * setclipregion(Transform *,View3dSurface *);
     QPainterPath setclipregion(Transform *,View3dSurface *);
     QGraphicsItem * draw(int, Transform *, View3dSurface *, bool invisible=false);
     void fill(int,Transform *,View3dSurface *);
     QGraphicsItem * fill(int, Transform *, View3dSurface *, QBrush *brush );
     QGraphicsItem * fill(int,Transform *,View3dSurface *, int fillstyle );
     void fillGL(int,Transform *,View3dSurface *);
     void drawGL(int, Transform *, View3dSurface * );
#else
     void setclipregion(Transform *,View3dSurface *);
     void draw(int,Transform *,View3dSurface *);
     void fill(int,Transform *,View3dSurface *);
#endif
     void setextents(void);
     int checkextents(Point p1,Point p2,int checkz = 0);
     double getarea(void);
     int addtodisplaylist(View3dSurface *,int filled,RCCOLORREF colour);
#ifdef USING_WIDGETS
     QGraphicsItem * addedgestodisplaylist(View3dSurface *);
     QPainterPath addpathtodisplaylist(View3dSurface *);
#else
     void addedgestodisplaylist(View3dSurface *);
#endif
  };

struct RealColour {double red,green,blue;  };
class VoxelPolygon;

class RayTracePolygon
  {protected:
     RealColour colour;
     SurfaceInfo *surfaceinfo;
     Point origin,xaxis,yaxis,zaxis,mapu,mapv,mapbmu,mapbmv;
     int defined;
     double d;
   public:
     long rayid;
     UV uv;
     double t;
     short i,j,k;
     RayTracePolygon() {  rayid = -1;  }
     int getdefined(void)  {  return defined;  }
     virtual ~RayTracePolygon();
     void setsurfaceinfo(SurfaceEntity *,SurfaceInfo *,SurfaceInfo *);
     void solidtexture(Point,Point *,RealColour *);
     virtual int intersect(Point,Point,double *,UV *) = 0;
     virtual void getcolour(Point,const UV &,RealColour *,Point *,Point *,SurfaceInfo **) = 0;
     virtual void copy(VoxelPolygon *) = 0;
     virtual void getinfo(int *,int *,Point *,Point *) = 0;
     virtual void transform(Transform *) = 0;
     virtual int isface(void) = 0;
  };

class Face : public RayTracePolygon
  {private:
     int nloops;
     UV **uvs;
     int *nvert;
     double umin,umax,vmin,vmax;
   public:
     Face(Point,Point,Point,EntityList,int,double);
     ~Face();
     int intersect(Point,Point,double *,UV *);
     void getcolour(Point,const UV &,RealColour *,Point *,Point *,SurfaceInfo **);
     void copy(VoxelPolygon *);
     void getinfo(int *,int *,Point *,Point *);
     void transform(Transform *);
     int isface(void) {  return 1;  }
  };

class Triangle : public RayTracePolygon
  {private:
     int flipped;
     double x1,x2;
     Point mapnx,mapny,mapnz;
   public:
     Triangle(int,Point,Point,Point,Point,Point,Point,UV,UV,UV);
     int intersect(Point,Point,double *,UV *);
     void getcolour(Point,const UV &,RealColour *,Point *,Point *,SurfaceInfo **);
     void copy(VoxelPolygon *);
     void getinfo(int *nloops,int *nvert,Point *,Point *);
     void transform(Transform *);
     int isface(void)  {  return 0;  }
//     void *operator new(size_t size);
//     void operator delete(void *a);
  };

struct LightInfo;
class sunlightInfo;

class Light : public Entity
  {private:
     Point origin,direction;
     double size,intensity,fadedistance,fadeexponent,coneangle,concentration;
     BitMask options;
   public:
     Light() : options(32) {  defined = 0;  }
     Light(const EntityHeader &,Point,Point,double,double,double,double,double,double,const BitMask&);
     Light(Point,Point,double,double,double,double,double,double,const BitMask&);
     ~Light() {};
     int type(void) const {  return light_entity;  }
     int isa(int type) const {  return light_entity == type;  }
     int issame(int,const Entity &) const;
     int match(const BitMask &mask) const {  return mask.test(light_entity);  }
     double pick(View3dSurface *,double,double,Point *);
     int pick(View3dSurface *,int,int,int,int,int);
     int pick(View3dSurface *,int,Point *,int);
#ifdef USING_WIDGETS
     void draw(int mode,Transform *t,View3dSurface *, QPainter *) {}
     void drawGL(int,Transform *,View3dSurface *);
     int hideGL(HideImage *image);
#endif
     void draw(int,Transform *,View3dSurface *);
     void addtodisplaylist(View3dSurface *,int);
     void draw(int mode,Transform *t = NULL) {  Entity::draw(mode,t);  }
     int hide(HideImage *image);
     int npoints(void);
     Point point(int);
     Point nearp(Point);
     int stretchent(Database *,Transform &,SortedPointTable *,int erase = 1);
     int move(Database *,Transform &,int erase = 1);
     Entity *clone(Transform &);
     int project(Database *,Transform &,int);
     void change(const BitMask&,const EntityHeader&,double,double,double,double,double,double,const BitMask&);
     static int setup(BitMask *,double *,double *,double *,double *,double *,double *,BitMask *,EntityHeader *h=0, sunlightInfo *sli=0);
     static void getdefaults(double *,double *,double *,double *,double *,double *,BitMask *);
     static void setdefaults(double,double,double,double,double,double,const BitMask&);
     void verify(void);
     int explode(void);
     void extents(View3dSurface *,Point *,Point *);
     int save(CadFile *);
     static Entity *load(int,CadFile *);
     int save(UndoFile *);
     static Entity *load(UndoFile *);
     int savegeometry(CadFile *);
     int savedxf(int,DXFOut *);
     int loadgeometry(CadFile *);
     int saveattributes(CadFile *);
     int loadattributes(CadFile *);
     void copyinfo(LightInfo *);
     int exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter);

     BitMask getoptions(void)  {  return options;  }
     Point getorigin(void)  {  return origin;  }
     Point getdirection(void)  {  return direction;  }
     double getsize(void)  {  return size;  }
     double getintensity(void)  {  return intensity;  }
     double getfadedistance(void)  {  return fadedistance;  }
     double getfadeexponent(void)  {  return fadeexponent;  }
     double getconeangle(void)  {  return coneangle;  }
     double getconcentration(void)  {  return concentration;  }

  };

class DatabaseList : public EntityList
{public:
  int add(Database *,Entity *entity,int);
  void del(Entity *entity,int);
  void tofront(Entity *);
  void toback(Entity *);
  void forward(Entity *);
  void backward(Entity *);
};

enum {  UD_INSERT,UD_DELETE,UD_STARTBLOCK,UD_ENDBLOCK,UD_CHANGEHEADER,
        UD_CHANGEATTRIBUTES,UD_CHANGEGROUP,UD_MANYMOVED,UD_MOVED,
        UD_INSERTATT,UD_DELETEATT,UD_CHANGEWORKPLANE,UD_CHANGEVIEW2D,
        UD_REPEATCOUNT  
     };

class GeometryDatabase
{
    friend class Database;
 private:
   long nextid,tablesize;
   DatabaseList list;
   Entity **idtoentity;
 public:
   GeometryDatabase();
  ~GeometryDatabase();
   int add(Entity *entity,int draw = 3,int undo = 1);
   void del(Entity *entity,int draw = 1,int undo = 1);
   void tofront(Entity *e)  {  list.tofront(e);  }
   void toback(Entity *e)  {  list.toback(e);  }
   void forward(Entity *e)  {  list.forward(e);  }
   void backward(Entity *e)  {  list.backward(e);  }
   void reverse(void);
   void start(void)  {  list.start();  }
   Entity *next(void) {  return list.next();  }
   void end(void)  {  list.end();  }
   Entity *last(void) {  return list.last();  }
#ifdef USING_WIDGETS
   //void drawGL(int, View3dSurface *window);
   void draw(int, View3dSurface *window, QPainter *painter);
   void draw(int, View3dSurface *window, QGraphicsScene *scene);
   void drawforQtPrint(int, View3dSurface *window, QGraphicsScene *scene);
   void drawforQtPrint(int, RCView3dPrinter *window, QGraphicsScene *scene);
   void drawforQtPrintGL(int, View3dSurface *window, QGraphicsScene *scene);
   void clearScene(int drawmode,View3dSurface *surface);
   void clearScene(int drawmode,RCView3dPrinter *surface);
   void hideGL(HideImage *image);
#endif
   void draw(int, View3dSurface *window);
   void hide(HideImage *image);
   void extents(View3dSurface *,Point *,Point *);
   EntityList pickall(View3dSurface *surface);
   Entity *pick(View3dSurface *,int,int,int,int,Entity **,Point *);
   Entity *pickNext(View3dWindow *,int,int,int,int,Entity **,Point *);
   Entity *pickhor(View3dSurface *,int,int,Point *,Point *,Point *,Point *,double *);
   EntityList pick(View3dSurface *,int,int,int,int,int);
   int pick(View3dSurface *,int,Point *,int,EntityList *);
   int pickoriginpoint(View3dSurface *vs,int distance,int x,int y,Point *);
   LinearEntity *trace(View3dSurface *,Loops *,Point,Point,Point,double,Point *,Point *,Point *,double *);
   int trace(LinearEntity *,EntityList *);
   Entity *getentity(long id);
   void clearstatus(void);
   void clear(void);
   int fixupreferences(Database *,long);
   int save(CadFile *);
   int save(CadFile *,EntityList);
   EntityList *getlist(void)  {  return & list;  }
   // v8 enhancement
   int pickmodelspaceoriginpoint(View3dSurface *surface,int snap,int x,int y,Point *p);

};

class Colour
  {private:
     double value(double n1,double n2,double hue);
   public:
     Colour() { red=0; green=0; blue=0; }
     Colour(int r, int g, int b) { red=r; green=g; blue=b; }
     Colour(double r, double g, double b) { set(r,g,b); }
     Colour(RCCOLORREF cref) { red = GetRValue(cref); green = GetGValue(cref); blue = GetBValue(cref); }
     Colour(const Colour &oc)
     {
         red = oc.red;
         green = oc.green;
         blue = oc.blue;
     }

     Byte red,green,blue;
     void sethls(double h,double l,double s);
     void gethls(double *h,double *l,double *s);
     void set(int r,int g,int b)  { red = Byte(r);  green = Byte(g);  blue = Byte(b); }
     void set(double r, double g, double b) { red = (int)(r * 255.0+0.5); green = (int)(g * 255.0+0.5); blue = (int)(b * 255.0+0.5); }
     void set(RCCOLORREF cref) { red = GetRValue(cref); green = GetGValue(cref); blue = GetBValue(cref);}
     void get(double *r, double *g, double *b) { *r = (double)red / 255.0; *g = (double)green / 255.0; *b = (double)blue / 255.0; }
     void get(RCCOLORREF *cref) { *cref = RGB(red,green,blue); }
     int save(CadFile *) const;
     int load(CadFile *);
     int load(RCCHAR *);
     int save(RCCHAR *);
     bool operator==(Colour oc)
     {
         return ((red == oc.red) && (green == oc.green) && (blue == oc.blue));
     }
     bool operator!=(Colour oc)
     {
         return ((red != oc.red) || (green != oc.green) || (blue != oc.blue));
     }
  };

class LineWeights
  {private:
     short nweights;
     double *weightmm;
#ifdef USING_WIDGETS
     int *weightpx;
     int *weightpx12;
     int oldweights[12];
#endif
   public:
     LineWeights();
     ~LineWeights();
     int mmtoweight(double);
     double weighttomm(int);
     int weighttohdc(RCHDC,int);
     int numweights(void) {  return nweights;  }
     int save(CadFile *) const;
     int load(CadFile *);
     void initialize(void);
     void clear();
     static bool isEqual(double _lhs, double _rhs);
     bool _isEqual(int _lhs, int _rhs);
     static bool isEqual(int _lhs, int _rhs);
#ifdef USING_WIDGETS
     void initweightpx();
     int weighttopx(int w);
     int weighttopxprint(int w, int dpi=12);
     int weightfromold(int w);
#endif
  };

class Surface;

class  LineStyle
  {private:
     GeneralList lengths;
     int on;
     double pos,endsegment,scale,length;
   public:
     LineStyle();
     LineStyle(LineStyle &other);
     ~LineStyle();
     void add(double);
     LineStyle *copy(void);
     void setnsegments(int i);
     int getnsegments(void);
     void setsegment(int i,double dash,double space);
     void getsegment(int i,double *dash,double *space);
     void start(double,double=-1);
#ifdef USING_WIDGETS
     void startGL(double,double);
     void drawGL(View3dSurface *, double, double, double, double);
     void drawGL(Surface *,double, double, double, double, double, double);
     void draw(QPainter *,Surface *,double,double,double,double);
     void draw(QPainterPath *,Surface *,double,double,double,double);
     void draw(QPainter *,double,double,double,double);
     QGraphicsItem * draw(QGraphicsItem *,Surface *,double,double,double,double);
     void draw(QGraphicsView *, double, double, double, double, QPen);
#endif
     void draw(Surface *,double,double,double,double);
     int save(CadFile *) const;
     int load(CadFile *);
     int save(RCCHAR *) const;
     int load(RCCHAR *);
     int savedxf(int,DXFOut *) const;
  };

class PatternLineStyle
  {private:
     double scale,height,lengthscale,shear,gap,offset,angle1,angle2,patternwidth;
     short orientation,fitting,size,lastsegment;
     RCCHAR filename[300];
     double pos,lastx1,lasty1,lastz1,lastx2,lasty2,lastz2,lastx3,lasty3,lastz3;
     Point *originalshape;
     Point *newshape;
     Surface *lastsurface;
   public: 
     PatternLineStyle();
     PatternLineStyle(PatternLineStyle *);
     PatternLineStyle(double,double,double,double,double,double,int,int,int,RCCHAR *);
     void GetInfo(double *,double *,double *,double *,double *,double *,int *,int *,int *,RCCHAR *);
     ~PatternLineStyle();
     void start(double,double);
     void end(void);
#ifdef USING_WIDGETS
     void startGL(double,double);
     void drawGL(View3dSurface*,double,double,double,double);
     void drawGL(Surface*,double,double,double,double,double,double);
     void draw(QPainter *,Surface*,double,double,double,double);
     void draw(QPainterPath *,Surface*,double,double,double,double);
     QGraphicsItem * draw(QGraphicsItem *,Surface*,double,double,double,double);
     void endGL();
     void endGL(View3dSurface*);
     QGraphicsItem * end(QGraphicsItem*);
     void end(QPainterPath*);
#endif
     void draw(Surface *,double,double,double,double);
     int save(CadFile *) const;
     int load(CadFile *);
  }; 

class LineStyles
  {private:
     int nstyles;
     LineStyle *styles[256];
   public:
     LineStyles();
     LineStyles(const LineStyles &other);
     ~LineStyles();
     LineStyle *indextostyle(int);
     void set(int,LineStyle *);
     int add(LineStyle *);
     void del(int i);
     int numstyles(void) {  return nstyles;  }
     void clear(void);
     int save(CadFile *) const;
     int load(CadFile *);
     int save(RCCHAR *) const;
     int load(RCCHAR *);
     int savedxf(DXFOut *) const;
  };


class LayerNames
  {private:
     RCCHAR *layernames[MAX_LAYERS];
   public:
     LayerNames();
     void clear(void);
     RCCHAR *getlayername(int i);
     RCCHAR *getlayername(int i,bool f);
     void setlayername(int i,RCCHAR *name);
     void setlayername(int i,char *name);
     int save(CadFile *) const;
     int load(CadFile *);
     int savedxf(DXFOut *) const;
  };

typedef Entity *(*EntityLoadFunction)(int,CadFile *);
typedef Entity *(*EntityUndoFunction)(UndoFile *);
typedef int (*OtherLoadFunction)(CadFile *,Database *);

class ClassFunctions
  {private:
     static int nfunctions;
     EntityLoadFunction *entityloadtable;
     EntityUndoFunction *entityundotable;
     OtherLoadFunction *otherloadtable;
   public:
     ClassFunctions();
     ~ClassFunctions();
     int registerentityload(int,EntityLoadFunction);
     int registerentityundo(int,EntityUndoFunction);
     int registerotherload(int,OtherLoadFunction);
     EntityLoadFunction entityload(int index);
     EntityUndoFunction entityundo(int index);
     OtherLoadFunction otherload(int index);
  };

class Window;

#define FormatNumberAlwaysShowUnits   1
#define FormatNumberShowSign          2
#define FormatNumberNoFractions       4
#define FormatNumberNeverShowUnits    8
#define FormatForEditField           16

enum {  SnapMaskOrigin,SnapMaskEnd,SnapMaskIntersection  };


class Database
{private:
   RCCHAR name[600];
   RCCHAR title[600];
   int changed,lineweightdisplay;
   int undoing,zoomstep,dragdetail,dynamiccursor,enableautoplotscale,numericdisplaytolerance,units,requiresopengl,enablefractions;
   double mainunitscale,subunitscale,minimumlineweight;
   RCCHAR mainunitname[300],subunitname[300];
   double atoler,ptoler,ttoler,stoler,dtoler,autoplotscale;
   double selectcolourred,selectcolourgreen,selectcolourblue;
   RCCHAR undofilename[300],redofilename[300],viewundofilename[300];
   UndoFile undofile,redofile,viewundofile;
   ModifyEntityCallback InsertCallback;
   ModifyEntityCallback UserDeleteCallback;
   ModifyEntityCallback DeleteCallback;
   ModifyEntityCallback VerifyCallback;
   ModifyEntityCallback SelectionCallback;
   ModifyEntityCallback UndoCallback;
   PropertyCallback CorePropertyCallback;
   PropertyCallback APIPropertyCallback;
   RCMSG last_message;
   void undo(const RCCHAR *,UndoFile *);
   void undo(const char *,UndoFile *); public:
   ModifyEntityCallback RightClickCallback;
   ModifyEntityCallback MovedCallback;
   CommandCallback TrimCommandCallback;
   CommandCallback StartCommandCallback;
   CommandCallback EndCommandCallback;
   MessageCallback MessageCB;
   GeometryDatabase geometry;
   Views3d views3d;
   Views2d views2d;
   int getnextviewno(void);
   AttributeMasks dismask;
   AttributeMasks lockedmask;
   BitMask snapmask;
   Colour colourtable[256];
   LineStyles linestyles;
   LineWeights lineweights;
   LayerNames layernames;
   EntityHeader header;
   Workplanes workplanes;
   Grid grid;
   Database();
   ~Database();
   void setupunits();
   int getundoavailable(void)  {  return ! undofile.closed() && undofile.position() > 0;  }
   int getredoavailable(void)  {  return ! redofile.closed() && redofile.position() > 0;  }
   double getptoler(void)  {  return ptoler;  }
   double getatoler(void)  {  return atoler;  }
   double getdtoler(void)  {  return dtoler;  }
   double getstoler(void)  {  return stoler;  }
   double getttoler(void)  {  return ttoler;  }
   int getrequiresopengl(void)  {  return requiresopengl;  }
   void setrequiresopengl(int rogl)  {  requiresopengl = rogl;  }
   double getminimumlineweight(void)  {  return minimumlineweight;  }
   double getmainunitscale(void)  {  return mainunitscale;  }
   double getsubunitscale(void)  {  return subunitscale;  }
   RCCHAR *getmainunitname(void)  {  return mainunitname;  }
   RCCHAR *getsubunitname(void)  {  return subunitname;  }
   double getautoplotscale(void)  {  return autoplotscale;  }
   int getnumericdisplaytolerance(void)  {  return numericdisplaytolerance;  }
   void setnumericdisplaytolerance(int ndt)  {  numericdisplaytolerance = ndt;  }
   RCCHAR *formatnumber(RCCHAR *buffer,double x,int showunits = 1);
   int getenableautoplotscale(void)  {  return enableautoplotscale;  }
   int getdragdetail(void)  {  return dragdetail; }
   void setdragdetail(int d) { dragdetail = d; }
   int getzoomstep(void)  {  return zoomstep;  }
   void setzoomstep(int s)  { zoomstep = s;  }
   int getdynamiccursor(void)  {  return dynamiccursor;  }
   void setptoler(double p)  {  ptoler = p;  }
   void setatoler(double a)  {  atoler = a;  }
   void setdtoler(double d)  {  dtoler = d;  }
   void setstoler(double s)  {  stoler = s;  }
   void setttoler(double t)  {  ttoler = t;  }
   void setminimumlineweight(double mlw)  {  minimumlineweight = mlw;  }
   void setunits(int u)  {  units = u;  setupunits();  }
   void setdynamiccursor(int d)  {  dynamiccursor = d;  }
   void setautoplotscale(double a)  {  autoplotscale = a;  }
   void setenableautoplotscale(int e)  {  enableautoplotscale = e;  }
   int getlineweightdisplay() { return lineweightdisplay; }
   void setlineweightdisplay(int d) { lineweightdisplay = d; }
   RCCHAR *getname(void)  {  return name;  }
   void setname(QString n);
   void setname(const RCCHAR *n);
   void setname(const char *n);
   RCCHAR *gettitle(void)  {  return title;  }
   void settitle(QString t)  {  strcpy(title,&t);  }
   void settitle(const RCCHAR *t)  {  strcpy(title,t);  }
   void settitle(const char *t)  {  strcpy(title,CHAR2RCCHR(t));  }
#ifdef USING_WIDGETS
   void draw(int dm,View3dSurface *window, QGraphicsScene *scene)
   {
        geometry.draw(dm,window,scene);
        grid.draw(dm,window);
        workplanes.draw(dm,window);
   }
   void drawforQtPrint(int dm,View3dSurface *window, QGraphicsScene *scene)
   {
       //grid.draw(2,window);
       //workplanes.draw(2,window);
       geometry.drawforQtPrint(dm,window,scene);
   }
   void drawforQtPrint(int dm,RCView3dPrinter *window, QGraphicsScene *scene)
   {
       //grid.draw(2,window);
       //workplanes.draw(2,window);
       geometry.drawforQtPrint(dm,window,scene);
   }
   void drawforQtPrintGL(int dm,View3dSurface *window, QGraphicsScene *scene)
   {
       //grid.draw(2,window);
       //workplanes.draw(2,window);
       geometry.drawforQtPrintGL(dm,window,scene);
   }
   void rubberbandOff();
   void clearScene(int dm,View3dSurface *window, QGraphicsScene *scene=0)
   {
       rubberbandOff();
       workplanes.clearScene(2 ,window);
       geometry.clearScene(2,window);
   }
   void clearScene(int dm,RCView3dPrinter *window, QGraphicsScene *scene=0)
   {
       rubberbandOff();
       workplanes.clearScene(2 ,window);
       geometry.clearScene(2,window);
   }
   void initColourTable(int type=0);
#endif
   void draw(int dm,View3dSurface *window)
     {
        geometry.draw(dm,window);
        //grid.draw(dm,window);
        //workplanes.draw(dm,window);
     }
   int save(CadFile *,EntityList *list = 0);
   int load(CadFile *);
   int savetables(CadFile *) const;
   int loadtables(CadFile *);
   int savemasktables(CadFile *) const;
   int loadmasktables(CadFile *);
   void undo(void);
   void undoview(void);
   void redo(void);
   void saveundo(int,void *);
   void savezoomundo(int,Window *);
   int loadgeometry(CadFile *,Transform *);
   void clear(void);
   void initialize(void);
   int getchanged(void) {  return changed;  }
   void setchanged(void) {  changed = 1;  }
   void clearchanged(void) {  changed = 0;   }
   void SetModifyCallback(int type,ModifyEntityCallback cb);
   void SetMessageCallback(MessageCallback mcb);
   void SetPropertyCallback(int api,PropertyCallback cb);
   int Properties(Entity *e,int action,int id,void *data,RCCHAR *text);
   int Properties(Entity *e,int action,int id,void *data,char *text);
   void Verify(Entity *);
   void Selection(Entity *);
   void EnableUndo(int i);
   void set_last_message(RCHWND hWnd, UINT message,WPARAM wParam, LPARAM lParam) {  last_message.hwnd = hWnd;  last_message.message = message;  last_message.wParam = wParam;  last_message.lParam = lParam;  }
   void get_last_message(RCHWND *hWnd, UINT *message,WPARAM *wParam, LPARAM *lParam) {  *hWnd = last_message.hwnd;  *message = last_message.message;  *wParam = last_message.wParam;  *lParam = last_message.lParam;  }
   // KMJ
   RCCOLORREF getSelectColourRef() { return RGB(selectcolourred*255,selectcolourgreen*255,selectcolourblue*255); }
   void setSelectColour(double red, double green, double blue) { selectcolourred=red;selectcolourgreen=green;selectcolourblue=blue; }
   void getSelectColour(double &red, double &green, double &blue) { red=selectcolourred;green=selectcolourgreen;blue=selectcolourblue; }
   int loadColourTable(RCCHAR *);
   int saveColourTable(RCCHAR *);
   void debug(void);
};

extern Database db;
extern ClassFunctions classfunctions;
extern LoadedFigures loadedfigures;

#endif
