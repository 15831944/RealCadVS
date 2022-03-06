
#include <math.h>

#if defined(__cplusplus)
struct RCPoint
  {double x,y,z;
   RCPoint(double x_,double y_,double z_)  {  x = x_;  y = y_;  z = z_;  }
   RCPoint()  {  x = y = z = -1.0E38;  }
   RCPoint Cross(const RCPoint& p2) const  {  return RCPoint(y*p2.z-z*p2.y,z*p2.x-x*p2.z,x*p2.y-y*p2.x);  }
   double Dot(const RCPoint& p2) const  { return x*p2.x+y*p2.y+z*p2.z;  }
   double GetLength() const  {  return sqrt(x*x + y*y + z*z);  }
   RCPoint &Normalize(void)  {double l;  l = sqrt(x*x + y*y + z*z);  if (l > 1.0E-12) {  x /= l;  y /= l;  z /= l;  }  return *this;  }
   RCPoint operator+ (const RCPoint& p) const  {  return RCPoint(x + p.x,y + p.y,z + p.z);  }
   RCPoint operator- (const RCPoint& p) const  {  return RCPoint(x - p.x,y - p.y,z - p.z);  }
   RCPoint operator- (void) const  {  return RCPoint(-x,-y,-z);  }
   RCPoint operator* (double scale) const  {  return RCPoint(x*scale,y*scale,z*scale);  }
   RCPoint operator/ (double scale) const  {  return RCPoint(x/scale,y/scale,z/scale);  }
   int   operator==(const RCPoint& p) const  {  return fabs(x - p.x) < 1.0E-5 && fabs(y - p.y) < 1.0E-5 && fabs(z - p.z) < 1.0E-5;  }
   void  operator*=(const double s)  {  x *= s;  y *= s;  z *= s;  }
   void  operator/=(const double s)  {  x /= s;  y /= s;  z /= s;  }
   void  operator+=(const RCPoint& p)  {  x += p.x;  y += p.y;  z += p.z;  }
   void  operator-=(const RCPoint& p)  {  x -= p.x;  y -= p.y;  z -= p.z;  }
   RCPoint &SetXYZ(double x1,double y1,double z1)  { x = x1;  y = y1;  z = z1;  return *this; }
  };
#else
struct RCPoint 
  {double x,y,z;  
  };
#endif

#define MAX_VIEWS 96

#define VW_ONEVIEWONLY 0x8000


struct Entity;
struct PointE;
struct Line;
struct Circle;
struct EllipseE;
struct Curve;
struct Text;
struct GroupE;
struct BitMapE;
struct Patch;
struct Light;
struct Figure;
struct RuledSurface;
struct SurfaceOfRev;
struct Plane;
struct AngularDimension;
struct DistanceBearingDimension;
struct CircularDimension;
struct LinearDimension;
struct BitMask;
struct ButtonMenu;
struct EntityList;
struct Dialog;
struct Intersect;
struct View2dPrinter;
struct Polygon3d;
struct View3dSurface;

enum DrawMode { DM_NORMAL,DM_INVERT,DM_ERASE,DM_SELECT,DM_GRID,DM_INVERTDASH,DM_DASH,DM_FLASH1,DM_FLASH2,DM_FLASH3 };

enum {  FigureLoaded, Figure2D3D, FigureState, FigureUnpacked, FigureParametric, FigureOriginalLayer, FigureOriginalColour, FigureOriginalStyle, FigureOriginalWeight, FigureEnableStates, FigureOrientToView, FigureOrientXYToView, FigureAttachFile, FigureUseAlternativeName, FigureElevation, FigureUseSize, FigureSaveSizeInfo  };

#define PP_GETLABEL            0
#define PP_GETVALUE            1
#define PP_SETVALUE            2
#define PP_CLICK               3
#define PP_GETNUMPROPERTIES    4


typedef int (WINAPI *PropertyCallback)(Entity *,int action,int id,void *data,char *text);
typedef void (WINAPI *APIRubberbandCallback)(int dm,RCPoint p);
typedef void (WINAPI *APIRubberbandCallbackEx)(int dm,RCPoint p,View3dSurface *v);
typedef int (WINAPI *ModifyEntityCallback)(Entity *);
typedef int (WINAPI *CommandCallback)(int command,int state,void *data);
typedef int (WINAPI *MessageCallback)(HWND,UINT,WPARAM,LPARAM);
typedef unsigned char Byte;
typedef void (WINAPI *APICommand)(void);
enum EntityCallback {  ECDelete,ECMoved,ECVerify,ECUndo,ECTrim,ECSelection,ECRightClick,ECInsert,ECUserDelete  };
enum CCCommandCallback {  CCTrimCommandCallback  };

struct TrimCallbackData
  {int operation,end;
   RCPoint p;
   Entity *trimentity,*trimtoentity;
  }; 

enum entity_types
  { end_list = -1,
    many_mask = 0,
    uv_mask = 1<<1,
    xyz_mask = 1<<2,
    linear_mask = 1<<3,
    surface_mask = 1<<4,
    uvsurface_mask = 1<<5,
    dimension_mask = 1<<6,
    point_entity = 1<<10,
    line_entity = 1<<11,
    circle_entity = 1<<12,
    infiniteplane_entity = 1<<13,
    plane_entity = 1<<14,
    text_entity = 1<<15,
    ruledsurface_entity = 1<<16,
    surfaceofrev_entity = 1<<17,
    lineardimension_entity = 1<<18,
    figure_entity = 1<<19,
    curve_entity = 1<<20,
    group_entity = 1<<21,
    angulardimension_entity = 1<<22,
    ellipse_entity = 1<<23,
    distancebearingdimension_entity = 1<<24,
    circulardimension_entity = 1<<25,
    light_entity = 1<<26,
    wall_entity = 1<<27,
    patch_entity = 1<<28,
    bitmap_entity = 1<<29,
    notation_entity = 1<<30
  };

struct TerminatorInfo
  {Byte style,weight;
   double length,height;
  };

//  if options bit 31 is set, then the halo colour exists
struct APITextInfo
  {
    char *font,*text;
    //RCCHAR *font,*text;
   double width,height,angle,vertspacing;
   long options;
   int halocolour;
  };

struct APILineStyleSegInfo
{
    double dash,space;
};

struct APILineStyleInfo
{
    int nsegs;
    APILineStyleSegInfo *segs;
};

#define VectorLength(p1,p2) sqrt((p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z))

#define TextInfo APITextInfo

struct InterfaceTable
  {int (WINAPI *InsertPoint)(RCPoint *p,PointE **);
   int (WINAPI *InsertLine)(RCPoint *p1,RCPoint *p2,Line **);
   int (WINAPI *InsertCircle)(RCPoint *org,RCPoint *xaxis,RCPoint *yaxis,double r,double starta,double sweepa,Circle **);
   int (WINAPI *InsertEllipse)(RCPoint *,RCPoint *,RCPoint *,double,double,double,double,EllipseE **);
   int (WINAPI *InsertCurve)(int n1,int d,RCPoint *p,double *k,double *w,long o,Curve **);
   int (WINAPI *InsertText)(char *,RCPoint,RCPoint,RCPoint,double,double,double,double,char *,char *,long,Text **);
   int (WINAPI *InsertGroup)(EntityList *,const char *,const char *,const long,GroupE **);
   int (WINAPI *InsertBitMapE)(char *,RCPoint,RCPoint,RCPoint,double,double,long,BitMapE **);
   int (WINAPI *InsertPatch)(int n1,int n2,int d1,int d2,int,int,RCPoint *p,double *k1,double *k2,
                                  double *w,EntityList *tcurves,long,Patch **);
   int (WINAPI *InsertFigure)(char *,char *,RCPoint,RCPoint,RCPoint,RCPoint,RCPoint,Byte,Byte,long,Figure **);
   int (WINAPI *InsertRuledSurface)(Entity *,Entity *,int xmesh,int ymesh,long options,RuledSurface **);
   int (WINAPI *InsertSurfaceOfRev)(RCPoint,RCPoint,double,double,Entity *e,int,int,long options,SurfaceOfRev **);
   int (WINAPI *InsertPlane)(EntityList *list,double xs,double ys,double a,double ca,long options,Plane **);
   int (WINAPI *InsertAngularDimension)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint aorg,int n,RCPoint *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,AngularDimension **);
   int (WINAPI *InsertDistanceBearingDimension)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint start,RCPoint end,long disp,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,DistanceBearingDimension **);
   int (WINAPI *InsertCircularDimension)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint pnt1,RCPoint pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,CircularDimension **);
   int (WINAPI *InsertLinearDimension)(RCPoint org,RCPoint xa,RCPoint ya,int n,RCPoint *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);

   int (WINAPI *GetPointInfo)(PointE *,RCPoint *p);
   int (WINAPI *GetLineInfo)(Line *,RCPoint *p1,RCPoint *p2);
   int (WINAPI *GetCircleInfo)(Circle *,RCPoint *org,RCPoint *xaxis,RCPoint *yaxis,double *r,double *starta,double *sweepa);
   int (WINAPI *GetEllipseInfo)(EllipseE *,RCPoint *org,RCPoint *xaxis,RCPoint *yaxis,double *r1,double *r2,double *starta,double *sweepa);
   int (WINAPI *GetCurveInfo)(Curve *,int *n1,int *d,RCPoint **p,double **k,double **w,long *o);
   int (WINAPI *GetTextInfo)(Text *,char **,RCPoint *,RCPoint *,RCPoint *,double *,double *,double *,double *,char **,char **,long *);
   int (WINAPI *GetGroupInfo)(GroupE *,EntityList **list,char **,char **,long *);
   int (WINAPI *GetBitMapEInfo)(BitMapE *,char **,RCPoint *,RCPoint *,RCPoint *,double *,double *,long *);
   int (WINAPI *GetPatchInfo)(Patch *,int *n1,int *n2,int *d1,int *d2,int *,int *,RCPoint **p,double **k1,double **k2,
                              double **w,EntityList **tcurves,long *);
   int (WINAPI *GetFigureInfo)(Figure *,char **,char **,RCPoint *,RCPoint *,RCPoint *,RCPoint *,RCPoint *,Byte *,Byte *,long *);
   int (WINAPI *GetRuledSurfaceInfo)(RuledSurface *,Entity **,Entity **,int *xmesh,int *ymesh,long *options);
   int (WINAPI *GetSurfaceOfRevInfo)(SurfaceOfRev *,RCPoint *,RCPoint *,double *,double *,Entity **e,int *,int *,long *);
   int (WINAPI *GetPlaneInfo)(Plane *,EntityList **list,double *xs,double *ys,double *a,double *ca,long *options);
   int (WINAPI *GetAngularDimensionInfo)(AngularDimension *,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *aorg,int *n,RCPoint **l,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetDistanceBearingDimensionInfo)(DistanceBearingDimension *,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *start,RCPoint *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetCircularDimensionInfo)(CircularDimension *,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *pnt1,RCPoint *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetLinearDimensionInfo)(LinearDimension *,RCPoint *org,RCPoint *xa,RCPoint *ya,int *n,RCPoint **,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetDefaultHeaderInfo)(int *layer,int *colour,int *style,int *weight,BitMask **visible);
   int (WINAPI *SetDefaultHeaderInfo)(int layer,int colour,int style,int weight,BitMask *visible);
   int (WINAPI *GetHeaderInfo)(Entity *,int *layer,int *colour,int *style,int *weight);
   long (WINAPI *GetEntityType)(Entity *);
   HWND (WINAPI *GetMainHwnd)(void);
   int (WINAPI *InsertButtonMenu)(int n,int ncol,ButtonMenu **);
   int (WINAPI *InsertButton)(ButtonMenu *,char *rname,RCHINSTANCE hinstance,int subcom,char *description,APICommand command);
   int (WINAPI *ShowButtonMenu)(ButtonMenu *,int x,int y,int ncol,int nrow);
   int (WINAPI *SelectPosition)(char *prompt,RCPoint *);
   int (WINAPI *SelectEntity)(char *prompt,long mask,Entity **,RCPoint *);
   void (WINAPI *StartList)(EntityList *list);
   Entity *(WINAPI *NextList)(EntityList *list);
   int (WINAPI *AtEndList)(EntityList *list);
   int (WINAPI *NewList)(EntityList **list);
   void (WINAPI *DeleteList)(EntityList *list);
   void (WINAPI *AddList)(EntityList *list,Entity *);
   void (WINAPI *ReverseList)(EntityList *list);
   void (WINAPI *CopyList)(EntityList *list,EntityList **);
   int (WINAPI *LengthList)(EntityList *list);
   void (WINAPI *MoveEntity)(Entity *,RCPoint);
   Entity *(WINAPI *CopyEntity)(Entity *,RCPoint);
   void (WINAPI *DeleteEntity)(Entity *);
   void (WINAPI *NewDialog)(char *,Dialog **);
   void (WINAPI *DeleteDialog)(Dialog *);
   int (WINAPI *ProcessDialog)(Dialog *,HWND,RCHINSTANCE);
   void (WINAPI *InsertStringControl)(Dialog *,int,char *,int);
   void (WINAPI *InsertIntegerControl)(Dialog *,int,int *,int,int,int);
   void (WINAPI *InsertRealControl)(Dialog *,int,double *,double,double,double);
   void (WINAPI *InsertCheckBoxControl)(Dialog *,int,int *);
   void (WINAPI *InsertRadioButtonControl)(Dialog *,int,int,int *);
   void (WINAPI *StartDatabaseList)(EntityList *list);
   void (WINAPI *SetLongValue)(Entity *,long);
   long (WINAPI *GetLongValue)(Entity *);
   void (WINAPI *DeleteEntityList)(EntityList *);
   void (WINAPI *GetExtents)(Entity *,RCPoint *,RCPoint *);
   void (WINAPI *FileNew)(int);
   void (WINAPI *FileOpen)(const char *);
   void (WINAPI *FileSaveAs)(const char *);
   void (WINAPI *DeleteAttribute)(Entity *,const char *);
   void (WINAPI *AddAttribute)(Entity *,const char *);
   char *(WINAPI *GetAttribute)(Entity *,int i);
   void (WINAPI *NewBitMask)(BitMask **bitmask,int n);
   void (WINAPI *SetBitMask)(BitMask *newbitmask,BitMask *oldbitmask);
   int (WINAPI *GetBitMaskBit)(BitMask *bitmask,int bit);
   void (WINAPI *SetBitMaskBit)(BitMask *bitmask,int bit,int value);
   void (WINAPI *SetLayerName)(int i,char *name);
   char *(WINAPI *GetLayerName)(int i);
   void (WINAPI *SetLayerDisplayMask)(int i,int value);
   int (WINAPI *GetLayerDisplayMask)(int i);
   void (WINAPI *MoveEntityBefore)(Entity *e1,Entity *e2);
   void (WINAPI *MoveEntityAfter)(Entity *e1,Entity *e2);
   void (WINAPI *GetCurrentSelection)(EntityList **list);
   void (WINAPI *SetCurrentSelection)(EntityList *list);
   void (WINAPI *HighLightList)(EntityList *list,int);
   void (WINAPI *BracketUndo)(int);
   void (WINAPI *Repaint)(int regenerate);
   int (WINAPI *InsertComposedDrawing)(char *name,double width,double height,int *viewno);
   int (WINAPI *GetComposedDrawingInfo)(int viewno,char *name,double *width,double *height);
   int (WINAPI *DeleteComposedDrawing)(int viewno);
   int (WINAPI *InsertComposedDrawingView)(int viewno,char *name,double umin,double vmin,double umax,double vmax,double scale,double xp,double yp,long o,BitMask *);
   int (WINAPI *GetComposedDrawingViewInfo)(int viewno,int index,char *name,double *umin,double *vmin,double *umax,double *vmax,double *scale,double *xp,double *yp,long *o,BitMask **);
   int (WINAPI *DeleteComposedDrawingView)(int viewno,int vindex);
   void (WINAPI *StartFigureList)(Figure *,EntityList **list);
   void (WINAPI *DeleteFigureList)(EntityList *list);
   void (WINAPI *DrawLine)(int mode,RCPoint p1,RCPoint p2);
   void (WINAPI *DrawText)(int mode,char *,RCPoint,RCPoint,RCPoint,double,double,double,double,char *,char *,long);
   void (WINAPI *ReverseEntity)(Entity *);
   void (WINAPI *SetHeaderInfo)(Entity *,int layer,int colour,int style,int weight);
   void (WINAPI *NewMenu)(HMENU hmenu);
   int (WINAPI *ShowComposedDrawing)(int index);
   int (WINAPI *SetLastPoint)(RCPoint);
   void (WINAPI *DivideEntity)(Entity *,RCPoint);
   void (WINAPI *NearEntity)(Entity *,RCPoint *);
   long (WINAPI *GetEntityStatus)(Entity *);
   void (WINAPI *SetEntityStatus)(Entity *,long);
   void (WINAPI *SetRealValue)(char *,double);
   double (WINAPI *GetRealValue)(char *);
   void (WINAPI *SetStringValue)(char *,char *);
   char *(WINAPI *GetStringValue)(char *);
   void (WINAPI *SetIntegerValue)(char *,int);
   int (WINAPI *GetIntegerValue)(char *);
   void (WINAPI *SetBitMaskValue)(char *,BitMask *);
   BitMask *(WINAPI *GetBitMaskValue)(char *);
   int (WINAPI *GetPlanePatternFile)(Plane *,char *filename);
   int (WINAPI *GetPatternLineStyleInfo)(Entity *,double *height,double *lengthscale,double *shear,double *gap,double *angle1,double *angle2,int *orientation,int *fitting,int *mirror,char *filename);
   double (WINAPI *GetArea)(Entity *);
   double (WINAPI *GetLength)(Entity *);
   void (WINAPI *SetRubberbandCallback)(APIRubberbandCallback rcb);
   void (WINAPI *PreviewFigure)(HWND hwnd,RCPoint,RCPoint,RCPoint,int,char *filename);
   int (WINAPI *GetPosition)(Entity *e,double,RCPoint *p);
   int (WINAPI *GetDirection)(Entity *e,double,RCPoint *p);
   void (WINAPI *CommandFile)(char *);
   int (WINAPI *InsertLight)(RCPoint org,RCPoint d,double,double,double,double,double,double,long o,Light **);
   int (WINAPI *GetLightInfo)(Light *,RCPoint *org,RCPoint *d,double *,double *,double *,double *,double *,double *,long *o);
   int (WINAPI *SetFigureParameters)(Figure *,int nparameters,char **,double *);
   void (WINAPI *IntersectEntityPlane)(Entity *e1,RCPoint o,RCPoint n,Intersect **);
   void (WINAPI *IntersectEntities)(Entity *e1,Entity *e2,Intersect **);
   void (WINAPI *GetIntersectEntity)(Intersect *,int,Entity **);
   void (WINAPI *DeleteIntersectEntities)(Intersect *);
   void (WINAPI *CutPlane)(Plane *,RCPoint o,RCPoint n,Plane *plane1,Plane *plane2);
   void (WINAPI *OffsetPlane)(Plane *,double offset,double height,EntityList **list);
   int (WINAPI *GetSurfacePosition)(Entity *e,double,double,RCPoint *p);
   void (WINAPI *RemoveList)(EntityList *list,Entity *e);
   void (WINAPI *DeleteEntityAll)(Entity *e);
   void (WINAPI *ChangeButtonMenu)(int,ButtonMenu *bm);
   int (WINAPI *CreateView)(const char *name,int perspective,RCPoint eye,RCPoint ref,RCPoint up,double front,double back,int show,int *viewno);
   int (WINAPI *GetViewInfo)(int viewno,char *name,int *perspective,RCPoint *eye,RCPoint *ref,RCPoint *up,double *front,double *back);
   int (WINAPI *DeleteView)(const char *name);
   void (WINAPI *AddGroupEntity)(GroupE *,Entity *);
   void (WINAPI *DeleteGroupEntity)(GroupE *,Entity *);
   void (WINAPI *SetEntityCallback)(int,ModifyEntityCallback);
   char *(WINAPI *FormatNumber)(double x,int showunits,char *buffer);
   void (WINAPI *GetUnitInfo)(char *mainunitname,char *subunitname,double *mainunitscale,double *subunitscale);
   void (WINAPI *SetIcon)(HICON hicon,char *title);
   char *(WINAPI *GetFileName)(void);
   void (WINAPI *SetFileName)(char *filename);
   void (WINAPI *EnableDrawing)(int d);
   int (WINAPI *InsertLinearDimensionEx)(RCPoint org,RCPoint xa,RCPoint ya,int n,RCPoint *,double e,double g,double s,double ut,double lt,double dc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);
   int (WINAPI *InsertComposedDrawingViewEx)(int viewno,char *name,double umin,double vmin,double umax,double vmax,double angle,double scale,double xp,double yp,long o,BitMask *);
   int (WINAPI *GetComposedDrawingViewInfoEx)(int viewno,int index,char *name,double *umin,double *vmin,double *umax,double *vmax,double *angle,double *scale,double *xp,double *yp,long *o,BitMask **);
   void (WINAPI *StrokeHiddenLineView)(char *viewname,char *hiddenlineviewname,double xpos,double ypos);
   int (WINAPI *ModifyView)(const char *name,int perspective,RCPoint eye,RCPoint ref,RCPoint up,double front,double back,int show,int *viewno);
   void (WINAPI *GetExtentsEx)(Entity *e,RCPoint *p1,RCPoint *p2,RCPoint eye,RCPoint ref,RCPoint up,int perspective,double front,double back,double plotscale);
   int (WINAPI *GetViewWindowInfo)(int,int *,char *,int *,HWND *);
   int (WINAPI *SelectViewWindow)(int);
   int (WINAPI *CreateViewWindow)(int viewtype,char *viewname,int *);
   int (WINAPI *RenderViewWindow)(int index,int style,char *filename);
   int (WINAPI *GetColour)(int index,RCCOLORREF *);
   int (WINAPI *SetColour)(int index,RCCOLORREF);
   int (WINAPI *CommonDialog)(int index,int *result);
   int (WINAPI *CreateWorkPlane)(char *name,RCPoint origin,RCPoint xaxis,RCPoint yaxis);
   int (WINAPI *DeleteWorkPlane)(char *name);
   int (WINAPI *GetWorkPlaneInfo)(int index,char *name,RCPoint *origin,RCPoint *xaxis,RCPoint *yaxis);
   int (WINAPI *ModifyWorkPlane)(char *name,RCPoint origin,RCPoint xaxis,RCPoint yaxis);
   int (WINAPI *SelectWorkPlane)(char *name);
   int (WINAPI *SetZoomViewWindow)(int index,double umin,double vmin,double umax,double vmax);
   int (WINAPI *GetZoomViewWindowInfo)(int index,double *umin,double *vmin,double *umax,double *vmax);
   int (WINAPI *SetPlotScaleViewWindow)(int index,double plotscale);
   int (WINAPI *GetPlotScaleViewWindowInfo)(int index,double *plotscale);
   int (WINAPI *SetPlaneAnchorPoint)(Plane *plane,RCPoint p,int redraw);
   int (WINAPI *InsertPlaneEx)(EntityList *list,double xs,double ys,double a,double ca,char *patternfilename,long options,Plane **);
   int (WINAPI *GetAPIVersion)(void);
   int (WINAPI *GetZoomScaleViewWindowInfo)(int index,double *scale,double *u,double *v);
   int (WINAPI *SetZoomScaleViewWindow)(int index,double scale,double u,double v);
   GroupE *(WINAPI *GetParentGroup)(Entity *);
   int (WINAPI *DeleteStandardMenuButton)(char *menu,int index);
   void (WINAPI *SetCommandCallback)(int,CommandCallback);
   int (WINAPI *GetHeaderInfoEx)(Entity *,int *layer,int *colour,int *style,int *weight,BitMask **vis);
   void (WINAPI *SetHeaderInfoEx)(Entity *,int layer,int colour,int style,int weight,BitMask *vis);
   void (WINAPI *ChangeFigure)(Figure *figure,BitMask *change,char *l,char *n,RCPoint s,RCPoint r,unsigned char s1,unsigned char s2,long o);
   void (WINAPI *DivideEntityEx)(Entity *,RCPoint,int copy,Entity **,Entity **);
   GroupE *(WINAPI *ExplodeEntity)(Entity *,char *,char *,int options);
   int (WINAPI *SetDefaultHeaderInfoEx)(int layer,int colour,int style,int weight,BitMask *visible,int update);
   void (WINAPI *DeleteBitMask)(BitMask *);
   void (WINAPI *SetMessageCallback)(MessageCallback);
   void (WINAPI *EnableUndo)(int);
   int (WINAPI *SetViewLayerInfo)(char *viewname,int layer,int colour,int style,int weight,BitMask *dlayertable,BitMask *slayertable,BitMask *llayertable);
   int (WINAPI *GetViewLayerInfo)(char *viewname,int *layer,int *colour,int *style,int *weight,BitMask **dlayertable,BitMask **slayertable,BitMask **llayertable);
   void (WINAPI *SetLayerLockedMask)(int i,int value);
   int (WINAPI *GetLayerLockedMask)(int i);
   int (WINAPI *AddRelationship)(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
   int (WINAPI *DeleteRelationship)(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
   int (WINAPI *GetRelationship)(Entity *entity,int index,Entity **relatedentity,long *ApplicationID,long *Relationship);
   Entity *(WINAPI *GetPickedEntity)(void);
   int (WINAPI *ShowButtonMenuEx)(char *,ButtonMenu *,int x,int y,int ncol,int nrow);
   char *(WINAPI *GetCADCommandLine)(void);
   void (WINAPI *ClearCADCommandLine)(void);
   int (WINAPI *InsertBitMapEEx)(char *,RCPoint,RCPoint,RCPoint,double,double,double,double,double,double,double,long,BitMapE **);
   int (WINAPI *GetBitMapEInfoEx)(BitMapE *,char **,RCPoint *,RCPoint *,RCPoint *,double *,double *,double *,double *,double *,double *,double *,long *);
   void (WINAPI *GetPickedPoint)(RCPoint *);
   void (WINAPI *SetDatabaseModified)(int m);
   int (WINAPI *GetDatabaseModified)(void);
   View2dPrinter * (WINAPI *PrintStart)(char *composeddrawingname);
   int (WINAPI *PrintNext)(View2dPrinter *view2dprinter,char *composeddrawingname);
   void (WINAPI *PrintFinish)(View2dPrinter *view2dprinter);
   void (WINAPI *ChangeText)(Text *text,BitMask *change,char *f,double w,double h,double s,double a,Byte hc,char *t,char *l,long o);
   int (WINAPI *InsertTextEx)(char *,RCPoint,RCPoint,RCPoint,double,double,double,double,Byte,char *,char *,long,Text **);
   int (WINAPI *FileExport)(EntityList *list,const char *filename);
   void (WINAPI *DrawFigure)(int mode,char *,char *,RCPoint,RCPoint,RCPoint,RCPoint,RCPoint,RCPoint,int,Byte,Byte,double *,long);
   void (WINAPI *ChangeFigureEx)(Figure *figure,BitMask *change,char *l,char *n,RCPoint s,RCPoint r,RCPoint si,int nc,unsigned char s1,unsigned char s2,long o);
   int (WINAPI *GetFigureInfoEx)(Figure *,char **,char **,RCPoint *,RCPoint *,RCPoint *,RCPoint *,RCPoint *,RCPoint *,int *,Byte *,Byte *,long *);
   int (WINAPI *GetPlanePolygon)(Plane *plane,Polygon3d **polygon3d,double error);
   void (WINAPI *DeletePlanePolygon)(Polygon3d *polygon3d);
   void (WINAPI *GetPolygonInfo)(Polygon3d *polygon,RCPoint *origin,RCPoint *xaxis,RCPoint *yaxis,int *nloops,int **nvert,RCPoint ***vert);
   int (WINAPI *SelectRGBColour)(double *red,double *green,double *blue);
   void (WINAPI *SwitchButtonMenu)(char *name,ButtonMenu *);
   void (WINAPI *ChangeDimension)(Entity *e,BitMask *change,double ex,double g,double s,double ut,double lt,
                       double sc,double tyo,int p,int tp,
                       TerminatorInfo *t1,TerminatorInfo *t2,
                       TextInfo *ti1,TextInfo *ti2,TextInfo *ti3,TextInfo *ti4,
                       long o);
   void (WINAPI *HighLight)(Entity *e,int highlight);
   void (WINAPI *DrawEntity)(Entity *e,int drawmode);
   void (WINAPI *MoveLineEndPoint)(Line *line,int draw,int end,RCPoint p1);
   void (WINAPI *GetParentList)(Entity *e,EntityList *list);
   void (WINAPI *RecalculatePlane)(Plane *plane);
   void (WINAPI *AddPlaneEdge)(Plane *e,RCPoint p1,Line *edge);
   int (WINAPI *InList)(EntityList *list,Entity *);
   void (WINAPI *DeletePlaneEdge)(Plane *e,Line *edge);
   void (WINAPI *ChangePlane)(Plane *plane,BitMask *change,double xs,double ys,double a,double ca,RCCOLORREF gc,char *filename,long o);
   int (WINAPI *SetViewWorkplane)(char *viewname,char *workplanename);
   int (WINAPI *GetViewWorkplane)(char *viewname,char *workplanename);
   int (WINAPI *SelectComposedDrawingView)(char *prompt,int *viewno,int *index);
   int (WINAPI *ModifyPlaneList)(Plane *,int add,EntityList *list);
   int (WINAPI *GetCurrentCommandId)(void);
   int (WINAPI *SaveViewWindowBitmap)(int index,int style,int bitmapsize,int savetype,int quality,char *savefilename);
   View2dPrinter *(WINAPI *PrintStartEx)(char *composeddrawingname,PRINTDLG *pd,DOCINFO *di);
   char *(WINAPI *GetExecutableFilename)(char *);
   char *(WINAPI *GetPublicFilename)(char *);
   void (WINAPI *SetPropertyCallback)(PropertyCallback);
   void (WINAPI *GetLastEvent)(HWND *hWnd, UINT *message,WPARAM *wParam, LPARAM *lParam);
   int (WINAPI *InsertPlaneExEx)(EntityList *list,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,char *patternfilename,long options,Plane **);

   // KMJ new APIs
   int (WINAPI *GetCurrentSelectionLength)(void);
   int (WINAPI *IsVisible)(Entity *e);
   void (WINAPI *SetVisible)(Entity *e, bool OnOff);
   int (WINAPI *GetDefaultTextInfo)(char **f,double *w,double *h,double *s,double *a,Byte *hc,char **t,long *o);
   int (WINAPI *GetDefaultPlaneInfo)(double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);
   int (WINAPI *GetPlaneInfoExEx)(Plane *,EntityList **list,double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);

   // KMJ added 2014/11/15
   int (WINAPI *GetLineStyle)(int index, APILineStyleInfo **linestyle);
   int (WINAPI *SetLineStyle)(int index, const APILineStyleInfo *linestyle);
   int (WINAPI *DeleteLineStyleInfo)(APILineStyleInfo *linestyle);
   int (WINAPI *SaveDefaults)();
   int (WINAPI *RestoreDefaults)();
   // KMJ added 2014/11/17
   int (WINAPI *SetDefaultPatternLineStyleInfo)(double height, double lengthscale, int size, char *filename);
   int (WINAPI *GetDefaultPatternLineStyleInfo)(double *height, double *lengthscale, int *size, char *filename);
   // KMJ added 2014/12/09
   int (WINAPI *IsVisibleInView)(Entity *e,int viewno);
   // KMJ added 2014/12/10
   int (WINAPI *InsertLinearDimensionExEx)(RCPoint org,RCPoint xa,RCPoint ya,int n,RCPoint *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,LinearDimension **ent);
   int (WINAPI *InsertAngularDimensionEx)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint aorg,int n,RCPoint *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,AngularDimension **ent);
   int (WINAPI *InsertCircularDimensionEx)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint pnt1,RCPoint pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,CircularDimension **ent);
   int (WINAPI *InsertDistanceBearingDimensionEx)(RCPoint org,RCPoint xa,RCPoint ya,RCPoint start,RCPoint end,long disp,double e,double g,double s,double ut,double lt,double sc,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,DistanceBearingDimension **ent);
   int (WINAPI *GetLinearDimensionInfoEx)(LinearDimension *ld,RCPoint *org,RCPoint *xa,RCPoint *ya,int *n,RCPoint **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetAngularDimensionInfoEx)(AngularDimension *ad,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *aorg,int *n,RCPoint **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetCircularDimensionInfoEx)(CircularDimension *cd,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *pnt1,RCPoint *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetDistanceBearingDimensionInfoEx)(DistanceBearingDimension *dbd,RCPoint *org,RCPoint *xa,RCPoint *ya,RCPoint *start,RCPoint *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,double *vs,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   // KMJ added 2015/1/13
   int (WINAPI *GetTextInfoEx)(Text *text,char **f,RCPoint *org,RCPoint *xaxis,RCPoint *yaxis,double *w,double *h,double *s,double *a,Byte *hc,char **t,char **l,long *options);

   // KMJ added 2015/1/21
   int (WINAPI *AttachFigure)(Figure *fig, bool OnOff);

   // KMJ added 2015/7/04
   int (WINAPI *IsVisibleInComposedDrawingView)(Entity *e, int viewno, int index, int *visible);
   int (WINAPI *IsVisibleInViewWindow)(Entity *e, int index);

   // KMJ added
   void (WINAPI *SetRubberbandCallbackEx)(APIRubberbandCallbackEx rcb);
   //
   void (WINAPI *SetIconByPath)(char *path,char *title);
};

//#define Point RCPoint
typedef RCPoint Point;

#define FormatNumberNoFractions       4
#define FormatNumberNeverShowUnits    8


