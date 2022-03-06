
#include "ncwin.h"
#include <QPixmap>
#include <QIcon>
#include "RCFigureBrowser.h"

//#define WINAPI __stdcall

// drawing is either 2 or 3
// drawing = 3 drawing to the screen
// drawing = 2 no drawing to the screen
static int drawing = 3;

//  If options bit 31 is set, then the halocolour exists
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

// KMJ: new for v7 buttonless commands
struct APICommandInfo
{
    int comid;
    char *modulename;
    char *commandname;
};
static GeneralList APICommandInfoList;

typedef void (WINAPI *APICommand)(void);
typedef void (WINAPI *APIRubberbandCallback)(int dm,Point p);
typedef void (WINAPI *APIRubberbandCallbackEx)(int dm,Point p,View3dSurface *v);
typedef int (WINAPI *PropertyCallback)(Entity *,int action,int id,void *data,char *text);

extern "C" int WINAPI InsertPoint(Point *p,PointE **);
extern "C" int WINAPI InsertLine(Point *p1,Point *p2,Line **);
extern "C" int WINAPI InsertCircle(Point *org,Point *xaxis,Point *yaxis,double r,double starta,double sweepa,Circle **);
extern "C" int WINAPI InsertEllipse(Point *,Point *,Point *,double,double,double,double,EllipseE **);
extern "C" int WINAPI InsertCurve(int n1,int d,Point *p,double *k,double *w,long o,Curve **);
extern "C" int WINAPI InsertText(char *,Point,Point,Point,double,double,double,double,char *,char *,long,Text **);
extern "C" int WINAPI InsertGroup(EntityList *,const char *,const char *,const long,Group **e);
extern "C" int WINAPI InsertBitMapE(char *,Point,Point,Point,double,double,long,BitMapE **);
extern "C" int WINAPI InsertBitMapEEx(char *,Point,Point,Point,double,double,double,double,double,double,double,long,BitMapE **);
extern "C" int WINAPI InsertPatch(int n1,int n2,int d1,int d2,int,int,Point *p,double *k1,double *k2,
                                  double *w,EntityList *tcurves,long,Patch **);
extern "C" int WINAPI InsertFigure(char *,char *,Point,Point,Point,Point,Point,Byte,Byte,long,Figure **);
extern "C" int WINAPI InsertRuledSurface(Entity *,Entity *,int xmesh,int ymesh,long options,RuledSurface **);
extern "C" int WINAPI InsertSurfaceOfRev(Point,Point,double,double,Entity *e,int,int,long options,SurfaceOfRev **);
extern "C" int WINAPI InsertPlane(EntityList *list,double xs,double ys,double a,double ca,long options,Plane **);
extern "C" int WINAPI InsertAngularDimension(Point org,Point xa,Point ya,Point aorg,int n,Point *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,AngularDimension **);
extern "C" int WINAPI InsertDistanceBearingDimension(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,DistanceBearingDimension **);
extern "C" int WINAPI InsertCircularDimension(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,CircularDimension **);
extern "C" int WINAPI InsertLinearDimension(Point org,Point xa,Point ya,int n,Point *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);
extern "C" int WINAPI GetPointInfo(PointE *,Point *p);
extern "C" int WINAPI GetLineInfo(Line *,Point *p1,Point *p2);
extern "C" int WINAPI GetCircleInfo(Circle *,Point *org,Point *xaxis,Point *yaxis,double *r,double *starta,double *sweepa);
extern "C" int WINAPI GetEllipseInfo(EllipseE *,Point *org,Point *xaxis,Point *yaxis,double *r1,double *r2,double *starta,double *sweepa);
extern "C" int WINAPI GetCurveInfo(Curve *,int *n1,int *d,Point **p,double **k,double **w,long *o);
extern "C" int WINAPI GetTextInfo(Text *,char **,Point *,Point *,Point *,double *,double *,double *,double *,char **,char **,long *);
extern "C" int WINAPI GetGroupInfo(Group *,EntityList **,char **,char **,long *);
extern "C" int WINAPI GetBitMapEInfo(BitMapE *,char **,Point *,Point *,Point *,double *,double *,long *);
extern "C" int WINAPI GetBitMapEInfoEx(BitMapE *,char **,Point *,Point *,Point *,double *,double *,double *,double *,double *,double *,double *,long *);
extern "C" int WINAPI GetPatchInfo(Patch *,int *n1,int *n2,int *d1,int *d2,int *,int *,Point **p,double **k1,double **k2,
                                  double **w,EntityList **tcurves,long *);
extern "C" int WINAPI GetFigureInfo(Figure *,char **,char **,Point *,Point *,Point *,Point *,Point *,Byte *,Byte *,long *);
extern "C" int WINAPI GetRuledSurfaceInfo(RuledSurface *,Entity **,Entity **,int *xmesh,int *ymesh,long *options);
extern "C" int WINAPI GetSurfaceOfRevInfo(SurfaceOfRev *,Point *,Point *,double *,double *,Entity **e,int *,int *,long *);
extern "C" int WINAPI GetPlaneInfo(Plane *,EntityList **list,double *xs,double *ys,double *a,double *ca,long *options);
extern "C" int WINAPI GetAngularDimensionInfo(AngularDimension *,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
extern "C" int WINAPI GetDistanceBearingDimensionInfo(DistanceBearingDimension *,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
extern "C" int WINAPI GetCircularDimensionInfo(CircularDimension *,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
extern "C" int WINAPI GetLinearDimensionInfo(LinearDimension *,Point *org,Point *xa,Point *ya,int *n,Point **,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
extern "C" int WINAPI GetDefaultHeaderInfo(int *layer,int *colour,int *style,int *weight,BitMask **visible);
extern "C" int WINAPI SetDefaultHeaderInfo(int layer,int colour,int style,int weight,BitMask *visible);
extern "C" int WINAPI GetHeaderInfo(Entity *,int *layer,int *colour,int *style,int *weight);
extern "C" long WINAPI GetEntityType(Entity *);
extern "C" RCHWND WINAPI GetMainHwnd(void);
extern "C" int WINAPI InsertButtonMenu(int n,int ncol,ButtonMenu **);
extern "C" int WINAPI InsertButton(ButtonMenu *,char *rname,RCHINSTANCE hinstance,int subcom,char *description,APICommand command);
extern "C" int WINAPI ShowButtonMenu(ButtonMenu *,int x,int y,int ncol,int nrow);
extern "C" int WINAPI ShowButtonMenuEx(char *,ButtonMenu *,int x,int y,int ncol,int nrow);
extern "C" int WINAPI SelectPosition(char *prompt,Point *);
extern "C" int WINAPI SelectEntity(char *prompt,long mask,Entity **,Point *);
extern "C" void WINAPI StartList(EntityList *list);
extern "C" Entity *WINAPI NextList(EntityList *list);
extern "C" int WINAPI AtEndList(EntityList *list);
extern "C" int WINAPI NewList(EntityList **list);
extern "C" void WINAPI DeleteList(EntityList *list);
extern "C" void WINAPI AddList(EntityList *list,Entity *);
extern "C" int WINAPI InList(EntityList *list,Entity *);
extern "C" void WINAPI ReverseList(EntityList *list);
extern "C" void WINAPI CopyList(EntityList *list,EntityList **);
extern "C" int WINAPI LengthList(EntityList *list);
extern "C" void WINAPI MoveEntity(Entity *,Point);
extern "C" Entity *WINAPI CopyEntity(Entity *,Point);
extern "C" void WINAPI DeleteEntity(Entity *);
extern "C" void WINAPI NewDialog(char *,Dialog **);
extern "C" void WINAPI DeleteDialog(Dialog *);
extern "C" int WINAPI ProcessDialog(Dialog *,RCHWND,RCHINSTANCE);
extern "C" void WINAPI InsertStringControl(Dialog *,int,char *,int);
extern "C" void WINAPI InsertIntegerControl(Dialog *,int,int *,int,int,int);
extern "C" void WINAPI InsertRealControl(Dialog *,int,double *,double,double,double);
extern "C" void WINAPI InsertCheckBoxControl(Dialog *,int,int *);
extern "C" void WINAPI InsertRadioButtonControl(Dialog *,int,int,int *);
extern "C" void WINAPI StartDatabaseList(EntityList *list);
extern "C" void WINAPI SetLongValue(Entity *,long);
extern "C" long WINAPI GetLongValue(Entity *);
extern "C" void WINAPI DeleteEntityList(EntityList *);
extern "C" void WINAPI GetExtents(Entity *,Point *,Point *);
extern "C" void WINAPI GetExtentsEx(Entity *e,Point *p1,Point *p2,Point eye,Point ref,Point up,int perspective,double front,double back,double plotscale);
extern "C" void WINAPI FileNew(int);
extern "C" void WINAPI FileOpen(const char *);
extern "C" void WINAPI FileSaveAs(const char *);
extern "C" void WINAPI DeleteAttribute(Entity *,const char *);
extern "C" void WINAPI AddAttribute(Entity *,const char *);
extern "C" char *WINAPI GetAttribute(Entity *,int index);
extern "C" void WINAPI NewBitMask(BitMask **bitmask,int n);
extern "C" void WINAPI SetBitMask(BitMask *newbitmask,BitMask *oldbitmask);
extern "C" int WINAPI GetBitMaskBit(BitMask *bitmask,int bit);
extern "C" void WINAPI SetBitMaskBit(BitMask *bitmask,int bit,int value);
extern "C" void WINAPI SetLayerName(int i,char *name);
extern "C" char *WINAPI GetLayerName(int i);
extern "C" void WINAPI SetLayerDisplayMask(int i,int value);
extern "C" int WINAPI GetLayerDisplayMask(int i);
extern "C" void WINAPI MoveEntityBefore(Entity *e1,Entity *e2);
extern "C" void WINAPI MoveEntityAfter(Entity *e1,Entity *e2);
extern "C" void WINAPI GetCurrentSelection(EntityList **list);
extern "C" void WINAPI SetCurrentSelection(EntityList *list);
extern "C" void WINAPI HighLightList(EntityList *list,int);
extern "C" void WINAPI BracketUndo(int);
extern "C" void WINAPI Repaint(int regenerate);
extern "C" int WINAPI InsertComposedDrawing(char *name,double width,double height,int *viewno);
extern "C" int WINAPI GetComposedDrawingInfo(int index,char *name,double *width,double *height);
extern "C" int WINAPI DeleteComposedDrawing(int index);
extern "C" int WINAPI InsertComposedDrawingView(int index,char *name,double umin,double vmin,double umax,double vmax,double scale,double xp,double yp,long o,BitMask *);
extern "C" int WINAPI GetComposedDrawingViewInfo(int index,int vindex,char *name,double *umin,double *vmin,double *umax,double *vmax,double *scale,double *xp,double *yp,long *o,BitMask **);
extern "C" int WINAPI DeleteComposedDrawingView(int index,int vindex);
extern "C" void WINAPI StartFigureList(Figure *,EntityList **list);
extern "C" void WINAPI DeleteFigureList(EntityList *list);
extern "C" void WINAPI DrawLine(int mode,Point p1,Point p2);
extern "C" void WINAPI DrawText1(int mode,char *,Point,Point,Point,double,double,double,double,char *,char *,long);
extern "C" void WINAPI ReverseEntity(Entity *);
extern "C" void WINAPI SetHeaderInfo(Entity *,int layer,int colour,int style,int weight);
extern "C" void WINAPI NewMenu(HMENU hmenu);
extern "C" int WINAPI ShowComposedDrawing(int index);
extern "C" void WINAPI SetLastPoint(Point p);
extern "C" void WINAPI DivideEntity(Entity *,Point);
extern "C" void WINAPI NearEntity(Entity *,Point *);
extern "C" long WINAPI GetEntityStatus(Entity *);
extern "C" void WINAPI SetEntityStatus(Entity *,long);
extern "C" void WINAPI SetRealValue(char *,double);
extern "C" double WINAPI GetRealValue(char *);
extern "C" void WINAPI SetStringValue(char *,char *);
extern "C" char *WINAPI GetStringValue(char *);
extern "C" void WINAPI SetIntegerValue(char *,int);
extern "C" int WINAPI GetIntegerValue(char *);
extern "C" void WINAPI SetBitMaskValue(char *,BitMask *);
extern "C" BitMask *WINAPI GetBitMaskValue(char *);
extern "C" int WINAPI GetPlanePatternFile(Plane *,char *filename);
extern "C" int WINAPI GetPatternLineStyle(Entity *,double *height,double *lengthscale,double *shear,double *gap,double *angle1,double *angle2,int *orientation,int *fitting,int *mirror,char *filename);
extern "C" double WINAPI GetArea(Entity *);
extern "C" double WINAPI GetLength(Entity *);
extern "C" void WINAPI SetRubberbandCallback(APIRubberbandCallback rcb);
extern "C" void WINAPI PreviewFigure(RCHWND hwnd,Point,Point,Point,int,char *filename);
extern "C" int WINAPI GetPosition(Entity *e,double,Point *p);
extern "C" int WINAPI GetDirection(Entity *e,double,Point *p);
extern "C" void WINAPI CommandFile(char *);
extern "C" int WINAPI InsertLight(Point org,Point d,double,double,double,double,double,double,long o,Light **);
extern "C" int WINAPI GetLightInfo(Light *,Point *org,Point *d,double *,double *,double *,double *,double *,double *,long *o);
extern "C" int WINAPI SetFigureParameters(Figure *,int nparameters,char **,double *);
extern "C" void WINAPI IntersectEntities(Entity *e1,Entity *e2,Intersect **);
extern "C" void WINAPI IntersectEntityPlane(Entity *e1,Point o,Point n,Intersect **);
extern "C" void WINAPI GetIntersectEntity(Intersect *,int,Entity **);
extern "C" void WINAPI DeleteIntersectEntities(Intersect *);
extern "C" void WINAPI CutPlane(Plane *,Point o,Point n,EntityList **list1,EntityList **list2);
extern "C" void WINAPI OffsetPlane(Plane *,double offset,double height,EntityList **list);
extern "C" int WINAPI GetSurfacePosition(Entity *e,double,double,Point *p);
extern "C" void WINAPI RemoveList(EntityList *list,Entity *e);
extern "C" void WINAPI DeleteEntityAll(Entity *);
extern "C" void WINAPI ChangeButtonMenu(int,ButtonMenu *);
extern "C" void WINAPI SwitchButtonMenu(char *name,ButtonMenu *);
extern "C" int WINAPI CreateView(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno);
extern "C" int WINAPI GetViewInfo(int viewno,char *name,int *perspective,Point *eye,Point *ref,Point *up,double *front,double *back);
extern "C" int WINAPI DeleteView(const char *name);
extern "C" void WINAPI AddGroupEntity(Group *,Entity *);
extern "C" void WINAPI DeleteGroupEntity(Group *,Entity *);
extern "C" void WINAPI SetEntityCallback(int,ModifyEntityCallback);
extern "C" char *WINAPI FormatNumber(double x,int showunits,char *buffer);
extern "C" void WINAPI GetUnitInfo(char *mainunitname,char *subunitname,double *mainunitscale,double *subunitscale);
extern "C" void WINAPI SetIcon(HICON hicon,char *title);
extern "C" char *WINAPI GetFileName(void);
extern "C" void WINAPI SetFileName(char *filename);
extern "C" void WINAPI EnableDrawing(int d);
extern "C" int WINAPI InsertLinearDimensionEx(Point org,Point xa,Point ya,int n,Point *,double e,double g,double s,double ut,double lt,double dc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);
extern "C" int WINAPI InsertComposedDrawingViewEx(int index,char *name,double umin,double vmin,double umax,double vmax,double angle,double scale,double xp,double yp,long o,BitMask *);
extern "C" int WINAPI GetComposedDrawingViewInfoEx(int index,int vindex,char *name,double *umin,double *vmin,double *umax,double *vmax,double *angle,double *scale,double *xp,double *yp,long *o,BitMask **);
extern "C" void WINAPI StrokeHiddenLineView(char *viewname,char *hiddenlineviewname,double xpos,double ypos);
extern "C" int WINAPI ModifyView(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno);
extern "C" void WINAPI GetExtentsEx(Entity *e,Point *p1,Point *p2,Point eye,Point ref,Point up,int perspective,double front,double back,double plotscale);
extern "C" int WINAPI GetViewWindowInfo(int,int *,char *,int *,RCHWND *);
extern "C" int WINAPI SelectViewWindow(int);
extern "C" int WINAPI CreateViewWindow(int viewtype,char *viewname,int *);
extern "C" int WINAPI RenderViewWindow(int index,int style,char *filename);
extern "C" int WINAPI GetColour(int index,RCCOLORREF *);
extern "C" int WINAPI SetColour(int index,RCCOLORREF);
extern "C" int WINAPI CommonDialog(int index,int *result);
extern "C" int WINAPI CreateWorkPlane(char *name,Point origin,Point xaxis,Point yaxis);
extern "C" int WINAPI DeleteWorkPlane(char *name);
extern "C" int WINAPI GetWorkPlaneInfo(int index,char *name,Point *origin,Point *xaxis,Point *yaxis);
extern "C" int WINAPI ModifyWorkPlane(char *name,Point origin,Point xaxis,Point yaxis);
extern "C" int WINAPI SelectWorkPlane(char *name);
extern "C" int WINAPI SetZoomViewWindow(int index,double umin,double vmin,double umax,double vmax);
extern "C" int WINAPI SetPlotScaleViewWindow(int index,double plotscale);
extern "C" int WINAPI GetZoomViewWindowInfo(int index,double *umin,double *vmin,double *umax,double *vmax);
extern "C" int WINAPI GetPlotScaleViewWindowInfo(int index,double *plotscale);
extern "C" int WINAPI SetPlaneAnchorPoint(Plane *plane,Point p,int redraw);
extern "C" int WINAPI InsertPlaneEx(EntityList *list,double xs,double ys,double a,double ca,char *patternfilename,long options,Plane **);
extern "C" int WINAPI GetAPIVersion(void);
extern "C" int WINAPI GetZoomScaleViewWindowInfo(int index,double *scale,double *u,double *v);
extern "C" int WINAPI SetZoomScaleViewWindow(int index,double scale,double u,double v);
extern "C" Group *WINAPI GetParentGroup(Entity *);
extern "C" int WINAPI DeleteStandardMenuButton(char *menu,int index);
extern "C" void WINAPI SetCommandCallback(int,CommandCallback);
extern "C" int WINAPI GetHeaderInfoEx(Entity *,int *layer,int *colour,int *style,int *weight,BitMask **vis);
extern "C" void WINAPI SetHeaderInfoEx(Entity *,int layer,int colour,int style,int weight,BitMask *vis);
extern "C" void WINAPI ChangeFigure(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,unsigned char s1,unsigned char s2,long o);
extern "C" void WINAPI DivideEntityEx(Entity *,Point,int copy,Entity **,Entity **);
extern "C" Group *WINAPI ExplodeEntity(Entity *,char *,char *,int options);
extern "C" void WINAPI DeleteBitMask(BitMask *);
extern "C" int WINAPI SetDefaultHeaderInfoEx(int layer,int colour,int style,int weight,BitMask *visible,int update);
extern "C" void WINAPI SetMessageCallback(MessageCallback);
extern "C" void WINAPI EnableUndo(int);
extern "C" int WINAPI SetViewLayerInfo(char *viewname,int layer,int colour,int style,int weight,BitMask *displaylayertable,BitMask *selectionlayertable,BitMask *lockedlayertable);
extern "C" int WINAPI GetViewLayerInfo(char *viewname,int *layer,int *colour,int *style,int *weight,BitMask **displaylayertable,BitMask **selectionlayertable,BitMask **lockedlayertable);
extern "C" void WINAPI SetLayerLockedMask(int i,int value);
extern "C" int WINAPI GetLayerLockedMask(int i);
extern "C" int WINAPI AddRelationship(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
extern "C" int WINAPI DeleteRelationship(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
extern "C" int WINAPI GetRelationship(Entity *entity,int index,Entity **relatedentity,long *ApplicationID,long *Relationship);
extern "C" Entity *WINAPI GetPickedEntity(void);
extern "C" char *WINAPI GetCADCommandLine(void);
extern "C" void WINAPI ClearCADCommandLine(void);
extern "C" void WINAPI GetPickedPoint(Point *);
extern "C" void WINAPI SetDatabaseModified(int m);
extern "C" int WINAPI GetDatabaseModified(void);
extern "C" View2dPrinter *WINAPI PrintStart(char *composeddrawingname);
extern "C" int WINAPI PrintNext(View2dPrinter *view2dprinter,char *composeddrawingname);
extern "C" void WINAPI PrintFinish(View2dPrinter *view2dprinter);
extern "C" void WINAPI ChangeText(Text *text,BitMask *change,char *f,double w,double h,double s,double a,Byte hc,char *t,char *l,long o);
extern "C" int WINAPI InsertTextEx(char *,Point,Point,Point,double,double,double,double,Byte,char *,char *,long,Text **);
extern "C" int WINAPI FileExport(EntityList *list,const char *filename);
extern "C" void WINAPI DrawFigure(int mode,char *,char *,Point,Point,Point,Point,Point,Point,int,Byte,Byte,double *,long);
extern "C" void WINAPI ChangeFigureEx(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,Point sc,int nc,unsigned char s1,unsigned char s2,long o);
extern "C" int WINAPI GetFigureInfoEx(Figure *,char **,char **,Point *,Point *,Point *,Point *,Point *,Point *,int *,Byte *,Byte *,long *);
extern "C" int WINAPI GetPlanePolygon(Plane *plane,Polygon3d **polygon3d,double error);
extern "C" void WINAPI DeletePlanePolygon(Polygon3d *polygon3d);
extern "C" void WINAPI GetPolygonInfo(Polygon3d *polygon,Point *origin,Point *xaxis,Point *yaxis,int *nloops,int **nvert,Point ***vert);
extern "C" int WINAPI SelectRGBColour(double *red,double *green,double *blue);
extern "C" void WINAPI ChangeDimension(Entity *e,BitMask *change,double ex,double g,double s,double ut,double lt,
                       double sc,double tyo,int p,int tp,
                       TerminatorInfo *t1,TerminatorInfo *t2,
                       TextInfo *ti1,TextInfo *ti2,TextInfo *ti3,TextInfo *ti4,
                       long o);
extern "C" void WINAPI HighLight(Entity *e,int);
extern "C" void WINAPI DrawEntity(Entity *e,int);
extern "C" void WINAPI AddPlaneEdge(Plane *e,Point p1,Line *edge);
extern "C" void WINAPI MoveLineEndPoint(Line *e,int draw,int end,Point p1);
extern "C" void WINAPI GetParentList(Entity *e,EntityList *list);
extern "C" void WINAPI RecalculatePlane(Plane *plane);
extern "C" void WINAPI DeletePlaneEdge(Plane *e,Line *edge);
extern "C" void WINAPI ChangePlane(Plane *plane,BitMask *change,double xs,double ys,double a,double ca,RCCOLORREF gc,char *filename,long o);
extern "C" int WINAPI SetViewWorkplane(char *viewname,char *workplanename);
extern "C" int WINAPI GetViewWorkplane(char *viewname,char *workplanename);
extern "C" int WINAPI SelectComposedDrawingView(char *prompt,int *viewno,int *index);
extern "C" int WINAPI ModifyPlaneList(Plane *,int add,EntityList *list);
extern "C" int WINAPI GetCurrentCommandId(void);
extern "C" int WINAPI SaveViewWindowBitmap(int index,int style,int bitmapsize,int savetype,int quality,char *savefilename);
extern "C" View2dPrinter *WINAPI PrintStartEx(char *composeddrawingname,PRINTDLG *pd,DOCINFO *di);
extern "C" char *WINAPI GetExecutableFilename(char *);
extern "C" char *WINAPI GetPublicFilename(char *);
extern "C" void WINAPI SetPropertyCallback(PropertyCallback);
extern "C" void WINAPI GetLastEvent(RCHWND *hWnd, UINT *message,WPARAM *wParam, LPARAM *lParam);
extern "C" int WINAPI InsertPlaneExEx(EntityList *list,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,char *patternfilename,long options,Plane **);

// KMJ new APIs
extern "C" int WINAPI GetCurrentSelectionLength(void);
extern "C" int WINAPI IsVisible(Entity *e);
extern "C" void WINAPI SetVisible(Entity *e, bool OnOff);
extern "C" int WINAPI GetDefaultTextInfo(char **f,double *w,double *h,double *s,double *a,Byte *hc,char **t,long *o);
extern "C" int WINAPI GetDefaultPlaneInfo(double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);
extern "C" int WINAPI GetPlaneInfoExEx(Plane *,EntityList **list,double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);

// KMJ added 2014/11/15
extern "C" int WINAPI GetLineStyle(int index, APILineStyleInfo **linestyle);
extern "C" int WINAPI SetLineStyle(int index, const APILineStyleInfo *linestyle );
extern "C" int WINAPI DeleteLineStyleInfo(APILineStyleInfo *linestyle);
extern "C" int WINAPI SaveDefaults();
extern "C" int WINAPI RestoreDefaults();

// KMJ added 2014/11/17
extern "C" int WINAPI SetDefaultPatternLineStyleInfo(double height, double lengthscale, int size, char *filename);
extern "C" int WINAPI GetDefaultPatternLineStyleInfo(double *height, double *lengthscale, int *size, char *filename);

// KMJ added 2014/12/09
extern "C" int WINAPI IsVisibleInView(Entity *e,int viewno);

// KMJ added 2014/12/10
extern "C" int WINAPI InsertLinearDimensionExEx(Point org,Point xa,Point ya,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,LinearDimension **ent);
extern "C" int WINAPI InsertAngularDimensionEx(Point org,Point xa,Point ya,Point aorg,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,AngularDimension **ent);
extern "C" int WINAPI InsertCircularDimensionEx(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,CircularDimension **ent);
extern "C" int WINAPI InsertDistanceBearingDimensionEx(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,double sc,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,DistanceBearingDimension **ent);
extern "C" int WINAPI GetLinearDimensionInfoEx(LinearDimension *ld,Point *org,Point *xa,Point *ya,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
extern "C" int WINAPI GetAngularDimensionInfoEx(AngularDimension *ad,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
extern "C" int WINAPI GetCircularDimensionInfoEx(CircularDimension *cd,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
extern "C" int WINAPI GetDistanceBearingDimensionInfoEx(DistanceBearingDimension *dbd,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,double *vs,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);

// KMJ added 2015/1/13
extern "C" int WINAPI GetTextInfoEx(Text *text,char **f,Point *org,Point *xaxis,Point *yaxis,double *w,double *h,double *s,double *a,Byte *hc,char **t,char **l,long *options);

// KMJ added 2015/1/21
extern "C" int WINAPI AttachFigure(Figure *fig, bool OnOff);

//
extern "C" int WINAPI IsVisibleInComposedDrawingView(Entity *e, int viewno, int index, int *visible);
extern "C" int WINAPI IsVisibleInViewWindow(Entity *e, int index);

// KMJ added
extern "C" void WINAPI SetRubberbandCallbackEx(APIRubberbandCallbackEx rcb);
extern "C" void WINAPI SetIconByPath(char *path,char *title);
extern "C" int WINAPI InsertCommand(int subcom,char *description,APICommand ccommand);
extern "C" int WINAPI SendCommandEvent(int commandid);
extern "C" int WINAPI SendCommand(char *description);

// KMJ added for the figure browser dialog
extern "C" int WINAPI NewFigureBrowser(void **dialog, void *parent);
extern "C" int WINAPI FigureBrowserInit(void *dialog);
extern "C" int WINAPI FigureBrowserSetFilter(void *dialog, char *filter );
extern "C" int WINAPI FigureBrowserSetRoorDir(void *dialog, char *rootdir );
extern "C" int WINAPI FigureBrowserSetPrefix(void *dialog, char *rootdir );
extern "C" int WINAPI FigureBrowserShowSettingsGroup(void *dialog, int group );
extern "C" int WINAPI FigureBrowserExec(void *dialog);
extern "C" int WINAPI DeleteFigureBrowser(void *dialog);
extern "C" char* WINAPI FigureBrowserGetFilename(void *dialog, char *filename);
extern "C" int WINAPI FigureBrowserSetFigureOptions(void *dialog, int origcolour, int origstyle, int origlayer, int origweight, int orienttoview);
extern "C" int WINAPI FigureBrowserSetTitle(void *dialog, char *title);

static GeneralList apicommandlist;
static GeneralList custommenulist;

struct InterfaceTable
  {int (WINAPI *InsertPoint)(Point *p,PointE **);
   int (WINAPI *InsertLine)(Point *p1,Point *p2,Line **);
   int (WINAPI *InsertCircle)(Point *org,Point *xaxis,Point *yaxis,double r,double starta,double sweepa,Circle **);
   int (WINAPI *InsertEllipse)(Point *,Point *,Point *,double,double,double,double,EllipseE **);
   int (WINAPI *InsertCurve)(int n1,int d,Point *p,double *k,double *w,long o,Curve **);
   int (WINAPI *InsertText)(char *,Point,Point,Point,double,double,double,double,char *,char *,long,Text **);
   int (WINAPI *InsertGroup)(EntityList *,const char *,const char *,const long,Group **);
   int (WINAPI *InsertBitMapE)(char *,Point,Point,Point,double,double,long,BitMapE **);
   int (WINAPI *InsertPatch)(int n1,int n2,int d1,int d2,int,int,Point *p,double *k1,double *k2,
                                  double *w,EntityList *tcurves,long,Patch **);
   int (WINAPI *InsertFigure)(char *,char *,Point,Point,Point,Point,Point,Byte,Byte,long,Figure **);
   int (WINAPI *InsertRuledSurface)(Entity *,Entity *,int xmesh,int ymesh,long options,RuledSurface **);
   int (WINAPI *InsertSurfaceOfRev)(Point,Point,double,double,Entity *e,int,int,long options,SurfaceOfRev **);
   int (WINAPI *InsertPlane)(EntityList *list,double xs,double ys,double a,double ca,long options,Plane **);
   int (WINAPI *InsertAngularDimension)(Point org,Point xa,Point ya,Point aorg,int n,Point *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,AngularDimension **);
   int (WINAPI *InsertDistanceBearingDimension)(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,DistanceBearingDimension **);
   int (WINAPI *InsertCircularDimension)(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,CircularDimension **);
   int (WINAPI *InsertLinearDimension)(Point org,Point xa,Point ya,int n,Point *,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);

   int (WINAPI *GetPointInfo)(PointE *,Point *p);
   int (WINAPI *GetLineInfo)(Line *,Point *p1,Point *p2);
   int (WINAPI *GetCircleInfo)(Circle *,Point *org,Point *xaxis,Point *yaxis,double *r,double *starta,double *sweepa);
   int (WINAPI *GetEllipseInfo)(EllipseE *,Point *org,Point *xaxis,Point *yaxis,double *r1,double *r2,double *starta,double *sweepa);
   int (WINAPI *GetCurveInfo)(Curve *,int *n1,int *d,Point **p,double **k,double **w,long *o);
   int (WINAPI *GetTextInfo)(Text *,char **,Point *,Point *,Point *,double *,double *,double *,double *,char **,char **,long *);
   int (WINAPI *GetGroupInfo)(Group *,EntityList **list,char **,char **,long *);
   int (WINAPI *GetBitMapEInfo)(BitMapE *,char **,Point *,Point *,Point *,double *,double *,long *);
   int (WINAPI *GetPatchInfo)(Patch *,int *n1,int *n2,int *d1,int *d2,int *,int *,Point **p,double **k1,double **k2,
                              double **w,EntityList **tcurves,long *);
   int (WINAPI *GetFigureInfo)(Figure *,char **,char **,Point *,Point *,Point *,Point *,Point *,Byte *,Byte *,long *);
   int (WINAPI *GetRuledSurfaceInfo)(RuledSurface *,Entity **,Entity **,int *xmesh,int *ymesh,long *options);
   int (WINAPI *GetSurfaceOfRevInfo)(SurfaceOfRev *,Point *,Point *,double *,double *,Entity **e,int *,int *,long *);
   int (WINAPI *GetPlaneInfo)(Plane *,EntityList **list,double *xs,double *ys,double *a,double *ca,long *options);
   int (WINAPI *GetAngularDimensionInfo)(AngularDimension *,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetDistanceBearingDimensionInfo)(DistanceBearingDimension *,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetCircularDimensionInfo)(CircularDimension *,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetLinearDimensionInfo)(LinearDimension *,Point *org,Point *xa,Point *ya,int *n,Point **,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *o);
   int (WINAPI *GetDefaultHeaderInfo)(int *layer,int *colour,int *style,int *weight,BitMask **visible);
   int (WINAPI *SetDefaultHeaderInfo)(int layer,int colour,int style,int weight,BitMask *visible);
   int (WINAPI *GetHeaderInfo)(Entity *,int *layer,int *colour,int *style,int *weight);
   long (WINAPI *GetEntityType)(Entity *);
   RCHWND (WINAPI *GetMainHwnd)(void);
   int (WINAPI *InsertButtonMenu)(int n,int ncol,ButtonMenu **);
   int (WINAPI *InsertButton)(ButtonMenu *,char *rname,RCHINSTANCE hinstance,int subcom,char *description,APICommand command);
   int (WINAPI *ShowButtonMenu)(ButtonMenu *,int x,int y,int ncol,int nrow);
   int (WINAPI *SelectPosition)(char *prompt,Point *);
   int (WINAPI *SelectEntity)(char *prompt,long mask,Entity **,Point *);
   void (WINAPI *StartList)(EntityList *list);
   Entity *(WINAPI *NextList)(EntityList *list);
   int (WINAPI *AtEndList)(EntityList *list);
   int (WINAPI *NewList)(EntityList **list);
   void (WINAPI *DeleteList)(EntityList *list);
   void (WINAPI *AddList)(EntityList *list,Entity *);
   void (WINAPI *ReverseList)(EntityList *list);
   void (WINAPI *CopyList)(EntityList *list,EntityList **);
   int (WINAPI *LengthList)(EntityList *list);
   void (WINAPI *MoveEntity)(Entity *,Point);
   Entity *(WINAPI *CopyEntity)(Entity *,Point);
   void (WINAPI *DeleteEntity)(Entity *);
   void (WINAPI *NewDialog)(char *,Dialog **);
   void (WINAPI *DeleteDialog)(Dialog *);
   int (WINAPI *ProcessDialog)(Dialog *,RCHWND,RCHINSTANCE);
   void (WINAPI *InsertStringControl)(Dialog *,int,char *,int);
   void (WINAPI *InsertIntegerControl)(Dialog *,int,int *,int,int,int);
   void (WINAPI *InsertRealControl)(Dialog *,int,double *,double,double,double);
   void (WINAPI *InsertCheckBoxControl)(Dialog *,int,int *);
   void (WINAPI *InsertRadioButtonControl)(Dialog *,int,int,int *);
   void (WINAPI *StartDatabaseList)(EntityList *list);
   void (WINAPI *SetLongValue)(Entity *,long);
   long (WINAPI *GetLongValue)(Entity *);
   void (WINAPI *DeleteEntityList)(EntityList *);
   void (WINAPI *GetExtents)(Entity *,Point *,Point *);
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
   void (WINAPI *DrawLine)(int mode,Point p1,Point p2);
   void (WINAPI *DrawText)(int mode,char *,Point,Point,Point,double,double,double,double,char *,char *,long);
   void (WINAPI *ReverseEntity)(Entity *);
   void (WINAPI *SetHeaderInfo)(Entity *,int layer,int colour,int style,int weight);
   void (WINAPI *NewMenu)(HMENU hmenu);
   int (WINAPI *ShowComposedDrawing)(int index);
   void (WINAPI *SetLastPoint)(Point);
   void (WINAPI *DivideEntity)(Entity *,Point);
   void (WINAPI *NearEntity)(Entity *,Point *);
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
   int (WINAPI *GetPatternLineStyle)(Entity *,double *height,double *lengthscale,double *shear,double *gap,double *angle1,double *angle2,int *orientation,int *fitting,int *mirror,char *filename);
   double (WINAPI *GetArea)(Entity *);
   double (WINAPI *GetLength)(Entity *);
   void (WINAPI *SetRubberbandCallback)(APIRubberbandCallback rcb);
   void (WINAPI *PreviewFigure)(RCHWND hwnd,Point,Point,Point,int,char *filename);
   int (WINAPI *GetPosition)(Entity *e,double,Point *p);
   int (WINAPI *GetDirection)(Entity *e,double,Point *p);
   void (WINAPI *CommandFile)(char *);
   int (WINAPI *InsertLight)(Point org,Point d,double,double,double,double,double,double,long o,Light **);
   int (WINAPI *GetLightInfo)(Light *,Point *org,Point *d,double *,double *,double *,double *,double *,double *,long *o);
   int (WINAPI *SetFigureParameters)(Figure *,int nparameters,char **,double *);
   void (WINAPI *IntersectEntityPlane)(Entity *e1,Point o,Point n,Intersect **);
   void (WINAPI *IntersectEntities)(Entity *e1,Entity *e2,Intersect **);
   void (WINAPI *GetIntersectEntity)(Intersect *,int,Entity **);
   void (WINAPI *DeleteIntersectEntities)(Intersect *);
   void (WINAPI *CutPlane)(Plane *,Point o,Point n,EntityList **plane1,EntityList **plane2);
   void (WINAPI *OffsetPlane)(Plane *,double offset,double height,EntityList **list);
   int (WINAPI *GetSurfacePosition)(Entity *e,double,double,Point *p);
   void (WINAPI *RemoveList)(EntityList *list,Entity *e);
   void (WINAPI *DeleteEntityAll)(Entity *);
   void (WINAPI *ChangeButtonMenu)(int,ButtonMenu *);
   int (WINAPI *CreateView)(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno);
   int (WINAPI *GetViewInfo)(int viewno,char *name,int *perspective,Point *eye,Point *ref,Point *up,double *front,double *back);
   int (WINAPI *DeleteView)(const char *name);
   void (WINAPI *AddGroupEntity)(Group *,Entity *);
   void (WINAPI *DeleteGroupEntity)(Group *,Entity *);
   void (WINAPI *SetEntityCallback)(int,ModifyEntityCallback);
   char *(WINAPI *FormatNumber)(double x,int showunits,char *buffer);
   void (WINAPI *GetUnitInfo)(char *mainunitname,char *subunitname,double *mainunitscale,double *subunitscale);
   void (WINAPI *SetIcon)(HICON hicon,char *title);
   char *(WINAPI *GetFileName)(void);
   void (WINAPI *SetFileName)(char *filename);
   void (WINAPI *EnableDrawing)(int d);
   int (WINAPI *InsertLinearDimensionEx)(Point org,Point xa,Point ya,int n,Point *,double e,double g,double s,double ut,double lt,double dc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *ti1,const APITextInfo *ti2,const APITextInfo *ti3,const long o,LinearDimension **);
   int (WINAPI *InsertComposedDrawingViewEx)(int viewno,char *name,double umin,double vmin,double umax,double vmax,double angle,double scale,double xp,double yp,long o,BitMask *);
   int (WINAPI *GetComposedDrawingViewInfoEx)(int viewno,int index,char *name,double *umin,double *vmin,double *umax,double *vmax,double *angle,double *scale,double *xp,double *yp,long *o,BitMask **);
   void (WINAPI *StrokeHiddenLineView)(char *viewname,char *hiddenlineviewname,double xpos,double ypos);
   int (WINAPI *ModifyView)(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno);
   void (WINAPI *GetExtentsEx)(Entity *e,Point *p1,Point *p2,Point eye,Point ref,Point up,int perspective,double front,double back,double plotscale);
   int (WINAPI *GetViewWindowInfo)(int,int *,char *,int *,RCHWND *);
   int (WINAPI *SelectViewWindow)(int);
   int (WINAPI *CreateViewWindow)(int viewtype,char *viewname,int *);
   int (WINAPI *RenderViewWindow)(int index,int style,char *filename);
   int (WINAPI *GetColour)(int index,RCCOLORREF *);
   int (WINAPI *SetColour)(int index,RCCOLORREF);
   int (WINAPI *CommonDialog)(int index,int *result);
   int (WINAPI *CreateWorkPlane)(char *name,Point origin,Point xaxis,Point yaxis);
   int (WINAPI *DeleteWorkPlane)(char *name);
   int (WINAPI *GetWorkPlaneInfo)(int index,char *name,Point *origin,Point *xaxis,Point *yaxis);
   int (WINAPI *ModifyWorkPlane)(char *name,Point origin,Point xaxis,Point yaxis);
   int (WINAPI *SelectWorkPlane)(char *name);
   int (WINAPI *SetZoomViewWindow)(int index,double umin,double vmin,double umax,double vmax);
   int (WINAPI *GetZoomViewWindowInfo)(int index,double *umin,double *vmin,double *umax,double *vmax);
   int (WINAPI *SetPlotScaleViewWindow)(int index,double plotscale);
   int (WINAPI *GetPlotScaleViewWindowInfo)(int index,double *plotscale);
   int (WINAPI *SetPlaneAnchorPoint)(Plane *plane,Point p,int redraw);
   int (WINAPI *InsertPlaneEx)(EntityList *list,double xs,double ys,double a,double ca,char *patternfilename,long options,Plane **);
   int (WINAPI *GetAPIVersion)(void);
   int (WINAPI *GetZoomScaleViewWindowInfo)(int index,double *scale,double *u,double *v);
   int (WINAPI *SetZoomScaleViewWindow)(int index,double scale,double u,double v);
   Group *(WINAPI *GetParentGroup)(Entity *);
   int (WINAPI *DeleteStandardMenuButton)(char *menu,int index);
   void (WINAPI *SetCommandCallback)(int,CommandCallback);
   int (WINAPI *GetHeaderInfoEx)(Entity *,int *layer,int *colour,int *style,int *weight,BitMask **vis);
   void (WINAPI *SetHeaderInfoEx)(Entity *,int layer,int colour,int style,int weight,BitMask *vis);
   void (WINAPI *ChangeFigure)(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,unsigned char s1,unsigned char s2,long o);
   void (WINAPI *DivideEntityEx)(Entity *,Point,int copy,Entity **,Entity **);
   Group *(WINAPI *ExplodeEntity)(Entity *,char *,char *,int options);
   int (WINAPI *SetDefaultHeaderInfoEx)(int layer,int colour,int style,int weight,BitMask *visible,int update);
   void (WINAPI *DeleteBitMask)(BitMask *bitmask);
   void (WINAPI *SetMessageCallback)(MessageCallback);
   void (WINAPI *EnableUndo)(int);
   int (WINAPI *SetViewLayerInfo)(char *viewname,int layer,int colour,int style,int weight,BitMask *displaylayertable,BitMask *selectionlayertable,BitMask *lockedlayertable);
   int (WINAPI *GetViewLayerInfo)(char *viewname,int *layer,int *colour,int *style,int *weight,BitMask **displaylayertable,BitMask **selectionlayertable,BitMask **lockedlayertable);
   void (WINAPI *SetLayerLockedMask)(int i,int value);
   int (WINAPI *GetLayerLockedMask)(int i);
   int (WINAPI *AddRelationship)(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
   int (WINAPI *DeleteRelationship)(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship);
   int (WINAPI *GetRelationship)(Entity *entity,int index,Entity **relatedentity,long *ApplicationID,long *Relationship);
   Entity *(WINAPI *GetPickedEntity)(void);
   int (WINAPI *ShowButtonMenuEx)(char *,ButtonMenu *,int x,int y,int ncol,int nrow);
   char *(WINAPI *GetCommandLine)(void);
   void (WINAPI *ClearCommandLine)(void);
   int (WINAPI *InsertBitMapEEx)(char *,Point,Point,Point,double,double,double,double,double,double,double,long,BitMapE **);
   int (WINAPI *GetBitMapEInfoEx)(BitMapE *,char **,Point *,Point *,Point *,double *,double *,double *,double *,double *,double *,double *,long *);
   void (WINAPI *GetPickedPoint)(Point *);
   void (WINAPI *SetDatabaseModified)(int m);
   int (WINAPI *GetDatabaseModified)(void);
   View2dPrinter * (WINAPI *PrintStart)(char *composeddrawingname);
   int (WINAPI *PrintNext)(View2dPrinter * view2dprinter,char *composeddrawingname);
   void (WINAPI *PrintFinish)(View2dPrinter * view2dprinter);
   void (WINAPI *ChangeText)(Text *text,BitMask *change,char *f,double w,double h,double s,double a,Byte hc,char *t,char *l,long o);
   int (WINAPI *InsertTextEx)(char *,Point,Point,Point,double,double,double,double,Byte,char *,char *,long,Text **);
   int (WINAPI *FileExport)(EntityList *list,const char *filename);
   void (WINAPI *DrawFigure)(int mode,char *,char *,Point,Point,Point,Point,Point,Point,int,Byte,Byte,double *,long);
   void (WINAPI *ChangeFigureEx)(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,Point si,int nc,unsigned char s1,unsigned char s2,long o);
   int (WINAPI *GetFigureInfoEx)(Figure *,char **,char **,Point *,Point *,Point *,Point *,Point *,Point *,int *,Byte *,Byte *,long *);
   int (WINAPI *GetPlanePolygon)(Plane *plane,Polygon3d **polygon3d,double error);
   void (WINAPI *DeletePlanePolygon)(Polygon3d *polygon3d);
   void (WINAPI *GetPolygonInfo)(Polygon3d *polygon,Point *origin,Point *xaxis,Point *yaxis,int *nloops,int **nvert,Point ***vert);
   int (WINAPI *SelectRGBColour)(double *red,double *green,double *blue);
   void (WINAPI *SwitchButtonMenu)(char *name,ButtonMenu *);
   void (WINAPI *ChangeDimension)(Entity *e,BitMask *change,double ex,double g,double s,double ut,double lt,
                       double sc,double tyo,int p,int tp,
                       TerminatorInfo *t1,TerminatorInfo *t2,
                       TextInfo *ti1,TextInfo *ti2,TextInfo *ti3,TextInfo *ti4,
                       long o);
   void (WINAPI *HighLight)(Entity *e,int highlight);
   void (WINAPI *DrawEntity)(Entity *e,int drawmode);
   void (WINAPI *MoveLineEndPoint)(Line *e,int draw,int end,Point p1);
   void (WINAPI *GetParentList)(Entity *e,EntityList *list);
   void (WINAPI *RecalculatePlane)(Plane *plane);
   void (WINAPI *AddPlaneEdge)(Plane *e,Point p1,Line *edge);
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
   void (WINAPI *GetLastEvent)(RCHWND *hWnd, UINT *message,WPARAM *wParam, LPARAM *lParam);
   int (WINAPI *InsertPlaneExEx)(EntityList *list,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,char *patternfilename,long options,Plane **);
#if 1
   // KMJ new APIs
   int (WINAPI *GetCurrentSelectionLength)(void);
   int (WINAPI *IsVisible)(Entity *e);
   void (WINAPI *SetVisible)(Entity *e, bool OnOff);
   int (WINAPI *GetDefaultTextInfo)(char **f,double *w,double *h,double *s,double *a,Byte *hc,char **t,long *o);
   int (WINAPI *GetDefaultPlaneInfo)(double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);
   int (WINAPI *GetPlaneInfoExEx)(Plane *,EntityList **list,double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *options);

   // KMJ added 2014/11/15
   int (WINAPI *GetLineStyle)(int index, APILineStyleInfo **linestyle);
   int (WINAPI *SetLineStyle)(int index, const APILineStyleInfo *linestyle );
   int (WINAPI *DeleteLineStyleInfo)(APILineStyleInfo *linestyle);
   int (WINAPI *SaveDefaults)();
   int (WINAPI *RestoreDefaults)();
   // KMJ added 2014/11/17
   int (WINAPI *SetDefaultPatternLineStyleInfo)(double height, double lengthscale, int size, char *filename);
   int (WINAPI *GetDefaultPatternLineStyleInfo)(double *height, double *lengthscale, int *size, char *filename);
   // KMJ added 2014/12/09
   int (WINAPI *IsVisibleInView)(Entity *e,int viewno);
   // KMJ added 2014/12/10
   int (WINAPI *InsertLinearDimensionExEx)(Point org,Point xa,Point ya,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,LinearDimension **ent);
   int (WINAPI *InsertAngularDimensionEx)(Point org,Point xa,Point ya,Point aorg,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,AngularDimension **ent);
   int (WINAPI *InsertCircularDimensionEx)(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,CircularDimension **ent);
   int (WINAPI *InsertDistanceBearingDimensionEx)(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,double sc,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,DistanceBearingDimension **ent);
   int (WINAPI *GetLinearDimensionInfoEx)(LinearDimension *ld,Point *org,Point *xa,Point *ya,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetAngularDimensionInfoEx)(AngularDimension *ad,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetCircularDimensionInfoEx)(CircularDimension *cd,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);
   int (WINAPI *GetDistanceBearingDimensionInfoEx)(DistanceBearingDimension *dbd,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,double *vs,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options);

// KMJ added 2015/1/13
   int (WINAPI *GetTextInfoEx)(Text *text,char **f,Point *org,Point *xaxis,Point *yaxis,double *w,double *h,double *s,double *a,Byte *hc,char **t,char **l,long *options);

// KMJ added 2015/1/21
   int (WINAPI *AttachFigure)(Figure *fig, bool OnOff);
//
   int (WINAPI *IsVisibleInComposedDrawingView)(Entity *e, int viewno, int index, int *visible);
   int (WINAPI *IsVisibleInViewWindow)(Entity *e, int index);

   // KMJ added
   void (WINAPI *SetRubberbandCallbackEx)(APIRubberbandCallbackEx rcb);
   void (WINAPI *SetIconByPath)(char *path,char *title);
   int  (WINAPI *InsertCommand)(int subcom,char *description,APICommand ccommand);
   int  (WINAPI *SendCommandEvent)(int commandid);
   int  (WINAPI *SendCommand)(char *description);

   // KMJ added for figure browser
   int (WINAPI *NewFigureBrowser)(void **dialog, void *parent);
   int (WINAPI *FigureBrowserInit)(void *dialog);
   int (WINAPI *FigureBrowserSetFilter)(void *dialog, char *filter );
   int (WINAPI *FigureBrowserSetRoorDir)(void *dialog, char *rootdir );
   int (WINAPI *FigureBrowserSetPrefix)(void *dialog, char *rootdir );
   int (WINAPI *FigureBrowserShowSettingsGroup)(void *dialog, int group );
   int (WINAPI *FigureBrowserExec)(void *dialog);
   int (WINAPI *DeleteFigureBrowser)(void *dialog);
   char *(WINAPI *FigureBrowserGetFilename)(void *dialog, char *filename);
   int (WINAPI *FigureBrowserSetFigureOptions)(void *dialog, int origcolour, int origstyle, int origlayer, int origweight, int orienttoview);
   int (WINAPI *FigureBrowserSetTitle)(void *dialog, char *title);
   //
#endif
  } it = { InsertPoint,InsertLine,InsertCircle,InsertEllipse,InsertCurve,InsertText,
            InsertGroup,InsertBitMapE,InsertPatch,InsertFigure,InsertRuledSurface,InsertSurfaceOfRev,InsertPlane,
            InsertAngularDimension,InsertDistanceBearingDimension,InsertCircularDimension,InsertLinearDimension,
            GetPointInfo,GetLineInfo,GetCircleInfo,GetEllipseInfo,GetCurveInfo,GetTextInfo,
            GetGroupInfo,GetBitMapEInfo,GetPatchInfo,GetFigureInfo,GetRuledSurfaceInfo,GetSurfaceOfRevInfo,
            GetPlaneInfo,GetAngularDimensionInfo,GetDistanceBearingDimensionInfo,GetCircularDimensionInfo,GetLinearDimensionInfo,
            GetDefaultHeaderInfo,SetDefaultHeaderInfo,GetHeaderInfo,GetEntityType,
            GetMainHwnd,InsertButtonMenu,InsertButton,ShowButtonMenu,SelectPosition,SelectEntity,
            StartList,NextList,AtEndList,NewList,DeleteList,AddList,ReverseList,CopyList,LengthList,
            MoveEntity,CopyEntity,DeleteEntity,NewDialog,DeleteDialog,ProcessDialog,InsertStringControl,
            InsertIntegerControl,InsertRealControl,InsertCheckBoxControl,InsertRadioButtonControl,
            StartDatabaseList,SetLongValue,GetLongValue,DeleteEntityList,GetExtents,FileNew,FileOpen,FileSaveAs,
            DeleteAttribute, AddAttribute, GetAttribute,
            NewBitMask,SetBitMask,GetBitMaskBit,SetBitMaskBit,
            SetLayerName,GetLayerName,SetLayerDisplayMask,GetLayerDisplayMask,MoveEntityBefore,MoveEntityAfter,
            GetCurrentSelection,SetCurrentSelection,HighLightList,BracketUndo,Repaint,
            InsertComposedDrawing,GetComposedDrawingInfo,DeleteComposedDrawing,
            InsertComposedDrawingView,GetComposedDrawingViewInfo,DeleteComposedDrawingView,
            StartFigureList,DeleteFigureList,DrawLine,DrawText1,ReverseEntity,SetHeaderInfo,
            NewMenu,ShowComposedDrawing,SetLastPoint,DivideEntity,NearEntity,
            GetEntityStatus,SetEntityStatus,
            SetRealValue,GetRealValue,SetStringValue,GetStringValue,SetIntegerValue,GetIntegerValue,SetBitMaskValue,
            GetBitMaskValue,GetPlanePatternFile,GetPatternLineStyle,
            GetArea,GetLength,SetRubberbandCallback,PreviewFigure,
            GetPosition,GetDirection,CommandFile,InsertLight,GetLightInfo,
            SetFigureParameters,IntersectEntityPlane,IntersectEntities,GetIntersectEntity,DeleteIntersectEntities,
            CutPlane,OffsetPlane,GetSurfacePosition,RemoveList,DeleteEntityAll,
            ChangeButtonMenu,CreateView,GetViewInfo,DeleteView,AddGroupEntity,
            DeleteGroupEntity,SetEntityCallback,FormatNumber,GetUnitInfo,
            SetIcon,GetFileName,SetFileName,EnableDrawing,InsertLinearDimensionEx,
            InsertComposedDrawingViewEx,GetComposedDrawingViewInfoEx,
            StrokeHiddenLineView,ModifyView,GetExtentsEx,GetViewWindowInfo,
            SelectViewWindow,CreateViewWindow,RenderViewWindow,GetColour,SetColour,             
            CommonDialog,CreateWorkPlane,DeleteWorkPlane,GetWorkPlaneInfo,
            ModifyWorkPlane,SelectWorkPlane,SetZoomViewWindow,GetZoomViewWindowInfo,
            SetPlotScaleViewWindow,GetPlotScaleViewWindowInfo,SetPlaneAnchorPoint,
            InsertPlaneEx,GetAPIVersion,GetZoomScaleViewWindowInfo,SetZoomScaleViewWindow,   
            GetParentGroup,DeleteStandardMenuButton,SetCommandCallback,
            GetHeaderInfoEx,SetHeaderInfoEx,ChangeFigure,DivideEntityEx,
            ExplodeEntity,SetDefaultHeaderInfoEx,DeleteBitMask,SetMessageCallback,EnableUndo,
            SetViewLayerInfo,GetViewLayerInfo,SetLayerLockedMask,GetLayerLockedMask,
            AddRelationship,DeleteRelationship,GetRelationship,GetPickedEntity,ShowButtonMenuEx,
            GetCADCommandLine,ClearCADCommandLine,
            InsertBitMapEEx,GetBitMapEInfoEx,GetPickedPoint,SetDatabaseModified,GetDatabaseModified,
            PrintStart,PrintNext,PrintFinish,ChangeText,InsertTextEx,FileExport,DrawFigure,ChangeFigureEx,
            GetFigureInfoEx,GetPlanePolygon,DeletePlanePolygon,GetPolygonInfo,
            SelectRGBColour,SwitchButtonMenu,ChangeDimension,HighLight,DrawEntity,
            MoveLineEndPoint,GetParentList,RecalculatePlane,AddPlaneEdge,InList,DeletePlaneEdge,
            ChangePlane,SetViewWorkplane,GetViewWorkplane,SelectComposedDrawingView,
            ModifyPlaneList,GetCurrentCommandId,SaveViewWindowBitmap,PrintStartEx,
            GetExecutableFilename,GetPublicFilename,SetPropertyCallback,GetLastEvent,InsertPlaneExEx,
            // KMJ new APIs
#if 1
            GetCurrentSelectionLength,IsVisible,SetVisible,GetDefaultTextInfo,GetDefaultPlaneInfo,
            GetPlaneInfoExEx,GetLineStyle,SetLineStyle,DeleteLineStyleInfo,SaveDefaults,RestoreDefaults,
            SetDefaultPatternLineStyleInfo,GetDefaultPatternLineStyleInfo,IsVisibleInView,
            InsertLinearDimensionExEx,InsertAngularDimensionEx,InsertCircularDimensionEx,InsertDistanceBearingDimensionEx,
            GetLinearDimensionInfoEx,GetAngularDimensionInfoEx,GetCircularDimensionInfoEx,GetDistanceBearingDimensionInfoEx,
            GetTextInfoEx,AttachFigure,IsVisibleInComposedDrawingView,IsVisibleInViewWindow,
            SetRubberbandCallbackEx,SetIconByPath,
            InsertCommand,SendCommandEvent,SendCommand,
            NewFigureBrowser,FigureBrowserInit,FigureBrowserSetFilter,FigureBrowserSetRoorDir,FigureBrowserSetPrefix,
            FigureBrowserShowSettingsGroup,FigureBrowserExec,DeleteFigureBrowser,FigureBrowserGetFilename,
            FigureBrowserSetFigureOptions,FigureBrowserSetTitle
#endif
         };

void Program::loadapp(char *path)
{
    loadapp(CHAR2RCCHR(path));
}

#ifdef USING_WIDGETS
// function for loading plugins as modules
// modules do not appear in the load application list
RCHINSTANCE * Program::loadcadmodule(RCCHAR *path)
{
    typedef char *(WINAPI *startfn)(InterfaceTable *it);
    startfn startmodule;
    QString qpath(path);

    qDebug() << "loadcadmodule - loadapp path : " << qpath;

     SetDllDirectory(home.getApplicationDir());

     QFile libfile(qpath);
     if(libfile.exists())
     {
         QLibrary *appinstance = new QLibrary(qpath,app);
         if(appinstance->load())
         {
               startmodule = (startfn)appinstance->resolve("startmodule");
               if (startmodule != 0)
               {
                    // run the start function
                    startmodule(&it);
                    //startfn(&it);
                    // app instance comparison does not work with QLibrary objects
                    // so we are comparing library file names
                    QLibrary *listinstance;
                    int i=0;
                    if(apps.length() > 0)
                    {
                        for (apps.start(),i = 0 ; (listinstance = (QLibrary*)apps.next()) != 0 ; i++)
                        {
                            if(listinstance->fileName() != appinstance->fileName())
                            {
                                if(!apps.inlist(appinstance))
                                   apps.add(appinstance);
                                break;
                            }
                        }
                    }
                    else
                        apps.add(appinstance);
                    return (RCHINSTANCE*)appinstance;
               }
               else
                   appinstance->unload();
         }
     }
     return 0;
}

void Program::loadapp(RCCHAR *path)
{
 //RCHINSTANCE appinstance;
 typedef char *(WINAPI *startfn)(InterfaceTable *it);
 startfn start;
 //char *(WINAPI *startfn)(InterfaceTable *it);
 QString qpath(path);

 qDebug() << "loadapp - loadapp path : " << qpath;

  //SetDllDirectory(home.getexecutablefilename(_RCT("dlls")));
  SetDllDirectory(home.getApplicationDir());

  //if (OpenFile(path,&ll,OF_EXIST) != HFILE_ERROR)
  QFile libfile(qpath);
  if(libfile.exists())
  {
      //if ((appinstance = LoadLibrary(path)) != 0)
      QLibrary *appinstance = new QLibrary(qpath,app);
      if(appinstance->load())
      {
            //start = (startfn)GetProcAddress(appinstance,"start");
            start = (startfn)appinstance->resolve("start");
            if (start != 0)
            {
                 // run the start function
                 start(&it);
                 //startfn(&it);
                 // app instance comparison does not work with QLibrary objects
                 // so we are comparing library file names
#if 1
                 QLibrary *listinstance;
                 int i=0;
                 if(apps.length() > 0)
                 {
                 for (apps.start(),i = 0 ; (listinstance = (QLibrary*)apps.next()) != 0 ; i++)
                 {
                     if(listinstance->fileName() != appinstance->fileName())
                     {
                         if(!apps.inlist(appinstance))
                            apps.add(appinstance);
                         break;
                     }
                 }
                 }
                 else
                     apps.add(appinstance);
#else
                 if (! apps.inlist(appinstance))
                   apps.add(appinstance);
#endif
            }
            else
                appinstance->unload();
              //FreeLibrary(appinstance);
      }
  }
}
#else
void Program::loadapp(RCCHAR *path)
{
#if ! defined(_WIN32_WCE)
 OFSTRUCT ll;
//WCEFIX
#endif
 RCHINSTANCE appinstance;
 typedef char *(WINAPI *startfn)(InterfaceTable *it);
 startfn start;

#if ! defined(_WIN32_WCE)

  SetDllDirectory(home.getexecutablefilename(_RCT("dlls")));

  if (OpenFile(path,&ll,OF_EXIST) != HFILE_ERROR)
    {  if ((appinstance = LoadLibrary(path)) != 0)
         {  start = (startfn)GetProcAddress(appinstance,"start");
            if (start != 0)
              {  start(&it);
                 if (! apps.inlist(appinstance))
                   apps.add(appinstance);
              }
            else
              FreeLibrary(appinstance);
         }
    }
//WCEFIX
#endif
}
#endif
void Program::savedefaultapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 RCCHAR key[300],filename[300];
 int i;

  for (apps.start(),i = 0 ; (appinstance = (QLibrary*)apps.next()) != 0 ; i++)
  {
       GetModuleFileName(appinstance,filename,300);
       sprintf(key,"App%d",i);
       WritePrivateProfileString(_RCT("Apps"),key,filename,home.getinifilename());
  }

  sprintf(key,"App%d",i);
  WritePrivateProfileString(_RCT("Apps"),key,_RCT(""),home.getinifilename());


}

void Program::loaddefaultapps(void)
{
 //char key[300],filename[300];
 RCCHAR key[300],filename[300];
 int i;
  for (i = 0 ;  ; i++)
  {
       sprintf(key,"App%d",i);
       GetPrivateProfileString(_RCT("Apps"),key,_RCT(""),filename,300,home.getinifilename());
       if (strlen(filename) == 0)
         break;
       loadapp(filename);
  }
}

void Program::filenewapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 typedef char *(WINAPI *newfn)(void);
 newfn fn;

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      if ((fn = (newfn)appinstance->resolve("filenew")) != 0)
         fn();
  }

}



OpenFunction openfunction = 0;

void SetOpenFunction(OpenFunction of)
{
    openfunction = of;
}


void WINAPI test(void)
{
    RCMessageBox(cadwindow->gethwnd(),"Opened","",MB_OK);
}

void Program::fileopenapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 OpenFunction fn;

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      //if ((fn = (OpenFunction)GetProcAddress(appinstance,"fileopen")) != 0)
      if ((fn = (OpenFunction)appinstance->resolve("fileopen")) != 0)
         fn();
  }

  if (openfunction != 0)
    openfunction();

}

void Program::filesaveapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 typedef char *(WINAPI *savefn)(void);
 savefn fn;

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      //if ((fn = (savefn)GetProcAddress(appinstance,"filesave")) != 0)
      if ((fn = (savefn)appinstance->resolve("filesave")) != 0)
         fn();
  }

}

void Program::fileexitapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 typedef void (WINAPI *exitfn)(void);
 exitfn fn;

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      //if ((fn = (exitfn)GetProcAddress(appinstance,"fileexit")) != 0)
      if ((fn = (exitfn)appinstance->resolve("fileexit")) != 0)
         fn();
  }

}

void Program::fileopendxfapps(void)
{
 //RCHINSTANCE appinstance;
 QLibrary * appinstance;
 typedef void (WINAPI *exitfn)(void);
 exitfn fn;

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      //if ((fn = (exitfn)GetProcAddress(appinstance,"fileopendxf")) != 0)
      if ((fn = (exitfn)appinstance->resolve("fileopendxf")) != 0)
         fn();
  }

}

RCCHAR *Program::getloadedappname(int index,RCCHAR *name)
{
    //RCHINSTANCE appinstance;
    QLibrary *appinstance;
    char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; index--)
    if (index == 0)
    {
         if(appinstance->load())
         {
             //getcadappname = (char *(WINAPI *)(void))GetProcAddress(appinstance,"getcadappname");
             getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
             if (getcadappname != NULL)
             {
                  strcpy(name,getcadappname());
                  return name;
             }
         }
    }

    return 0;
}

char *Program::getloadedappname(int index,char *name)
{
  //RCHINSTANCE appinstance;
  QLibrary *appinstance=0;
  char *(WINAPI *getcadappname)(void);

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; index--)
    if (index == 0)
    {
      if(appinstance->load())
      {
         getcadappname = (char *(WINAPI *)(void))GetProcAddress(appinstance,"getcadappname");
         if (getcadappname != NULL)
           {  strcpy(name,getcadappname());
              return name;
           }
      }
    }

  return 0;
}

// get the name of a give library module
// assumes that name is the fullpath to the library
RCCHAR *Program::getmodulename(RCCHAR *name)
{
    //RCHINSTANCE appinstance;
    //QLibrary *appinstance;
    const char *(WINAPI *getcadmodulename)(void);

    //appinstance = new QLibrary(QString(name),app);
    QLibrary appinstance(QString(name),app);
    //if(appinstance != 0 && appinstance->load())
    if(appinstance.load())
    {
        //getcadmodulename = (char *(WINAPI *)(void))appinstance->resolve("getcadmodulename");
        getcadmodulename = (const char *(WINAPI *)(void))appinstance.resolve("getcadmodulename");
        if (getcadmodulename != NULL)
        {
          strcpy(name,getcadmodulename());
          //delete appinstance;
          return name;
        }
    }

    //delete appinstance;
    return 0;
}

// get the name of a give library module
// assumes that name is the fullpath to the library
char *Program::getmodulename(char *name)
{
  QLibrary *appinstance=0;
  char *(WINAPI *getcadappname)(void);

    appinstance = new QLibrary(QString(home.getexecutablefilename(QString(name).data())),app);
    if(appinstance != 0 && appinstance->load())
    {
         getcadappname = (char *(WINAPI *)(void))GetProcAddress(appinstance,"getcadmodulename");
         if (getcadappname != NULL)
         {
              strcpy(name,getcadappname());
              delete appinstance;
              return name;
         }
    }

  delete appinstance;
  return 0;
}

// check if a library module is loaded
// assumes that name is the fullpath to the library
bool Program::ismoduleloaded(RCCHAR *name)
{
  QLibrary *appinstance=0;

  appinstance = new QLibrary(QString(name),app);
  if (appinstance != 0 && appinstance->load())
  {
       delete appinstance;
       return true;
  }

  qDebug() << "attemped to load module : " << QString(name);
  qDebug() << "module was not loaded : " << appinstance->errorString();

  delete appinstance;
  return false;
}

// check if a library module is loaded
// assumes that name is the fullpath to the library
bool Program::ismoduleloaded(char *name)
{
  QLibrary *appinstance=0;

  appinstance = new QLibrary(QString(name),app);
  if (appinstance != 0 && appinstance->load())
  {
       delete appinstance;
       return true;
  }
  delete appinstance;
  return false;
}

RCHMODULE *Program::getloadedapp(RCCHAR *name)
{
    QLibrary *appinstance;
    //char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ;)
    {
        if(appinstance->isLoaded())
        {
           QString appfilename = appinstance->fileName();
           if (appfilename.toLower().contains(QString(name)))
           {
                return (RCHMODULE*)appinstance;
           }
        }
    }

    return 0;
}

RCCHAR *Program::getloadedappversion(RCCHAR *name,RCCHAR *version)
{
    QLibrary *appinstance=0;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ;)
    {
        if(appinstance->isLoaded())
        {
           getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
           if (getcadappname != NULL/* && strcmp(name,getcadappname()) == 0*/)
           {
               QString appName = QString(getcadappname());
               //QString appfilename = appinstance->fileName();
               if (appName.toLower().contains(QString(name)))
               {
                    GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
                    if (GetVersionString != NULL)
                    {
                        strcpy(version,GetVersionString());
                        return version;
                    }
               }
           }
        }
    }
    // look for other loaded modules e.g. the built in DWG i/o library
#ifdef _MAC
    appinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib"))),app);
#else
    appinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("RealCADOpenDWG.dll"))),app);
#endif
    if (appinstance != 0 && appinstance->load())
    {
        getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
        if (getcadappname != NULL)
        {
            QString appName = QString(getcadappname());
            if (appName.toLower().contains(QString(name)))
            {
                 GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
                 if (GetVersionString != NULL)
                 {
                     strcpy(version,GetVersionString());
                     return version;
                 }
            }
        }
    }
    return 0;
}

char *Program::getloadedappversion(char *name,char *version)
{
    QLibrary *appinstance;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ;)
    {
        if(appinstance->isLoaded())
        {
           getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
           if (getcadappname != NULL && strcmp(name,getcadappname()) == 0)
           {
                GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
                if (GetVersionString != NULL)
                {
                    strcpy(version,GetVersionString());
                    return version;
                }
           }
        }
    }

    return 0;
}

// get the version string from a plugin
RCCHAR *Program::getappversion(RCCHAR *name,RCCHAR *version)
{
    QLibrary *appinstance;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ;)
    {
        if(appinstance->isLoaded())
        {
           getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
           if (getcadappname != NULL/* && strcmp(name,getcadappname()) == 0*/)
           {
               QString appName = QString(getcadappname());
               //QString appfilename = appinstance->fileName();
               if (appName.toLower().contains(QString(name)))
               {
                    GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
                    if (GetVersionString != NULL)
                    {
                        strcpy(version,GetVersionString());
                        return version;
                    }
               }
           }
        }
    }
    // look for other loaded modules e.g. the built in DWG i/o library
    // this requires the full path to the library
    appinstance = new QLibrary(QString(name),app);
    if (appinstance != 0 && appinstance->load())
    {
         GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
         if (GetVersionString != NULL)
         {
             strcpy(version,GetVersionString());
             return version;
         }
    }
    return 0;
}

char *Program::getappversion(char *name,char *version)
{
    QLibrary *appinstance;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ;)
    {
        if(appinstance->isLoaded())
        {
           getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
           if (getcadappname != NULL && strcmp(name,getcadappname()) == 0)
           {
                GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
                if (GetVersionString != NULL)
                {
                    strcpy(version,GetVersionString());
                    return version;
                }
           }
        }
    }

    return 0;
}

RCCHAR *Program::getmoduleversion(RCCHAR *name,RCCHAR *version)
{
    QLibrary *appinstance;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    // this requires the full path to the library
    appinstance = new QLibrary(QString(name),app);
    if (appinstance != 0 && appinstance->load())
    {
         GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
         if (GetVersionString != NULL)
         {
             strcpy(version,GetVersionString());
             delete appinstance;
             return version;
         }
    }
    delete appinstance;
    return 0;
}

char *Program::getmoduleversion(char *name,char *version)
{
    QLibrary *appinstance;
    char *(WINAPI *GetVersionString)(void);
    char *(WINAPI *getcadappname)(void);

    // this requires the full path to the library
    appinstance = new QLibrary(QString(name),app);
    if (appinstance != 0 && appinstance->load())
    {
         GetVersionString = (char *(WINAPI *)(void))appinstance->resolve("GetVersionString");
         if (GetVersionString != NULL)
         {
             strcpy(version,GetVersionString());
             delete appinstance;
             return version;
         }
    }
    delete appinstance;
    return 0;
}

void Program::unloadapp(char *name)
{
  //RCHINSTANCE appinstance;
  QLibrary *appinstance;
  char *(WINAPI *getcadappname)(void);

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      if(appinstance->load())
      {
         //getcadappname = (char *(WINAPI *)(void))GetProcAddress(appinstance,"getcadappname");
         getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
         if (getcadappname != NULL && strcmp(name,getcadappname()) == 0)
         {
             apps.del(appinstance);
             break;
         }
      }
  }

}

void Program::unloadapp(RCCHAR *name)
{
    // KMJ:
    // NOTE: currently we don't actually unload a library once it is loaded
    // we just make its tool menu invisible.
    // this works OK so far but needs looking at...
    //
  QLibrary *appinstance;
  //RCCHAR *(WINAPI *getcadappname)(void);
  char *(WINAPI *getcadappname)(void);
  char fullname[300];

  for (apps.start() ; (appinstance = (QLibrary*)apps.next()) != 0 ; )
  {
      if(appinstance->load())
      {
         //qDebug() << "name :" << QString(name);
         getcadappname = (char *(WINAPI *)(void))appinstance->resolve("getcadappname");
         //if (getcadappname != NULL)
         //    qDebug() << "getcadappname() :" << QString(getcadappname());
         if (getcadappname != NULL && strcmp(name,getcadappname()) == 0)
         {
             strcpy(fullname,"ApplicationMenu::");
             strcat(fullname,name);
             ButtonMenu *bm =0;
             if ((bm=buttonmenus.match(fullname)) != 0)
             {
                  if(bm->getQW())
                      bm->getQW()->hide();
                  // ToDo: add the toolbar to cadwindow control list??
                  //cadwindow->delcontrol(bm);
                  //buttonmenus.deletemenu(bm);
                  //custommenulist.del(bm);
             }
             apps.del(appinstance);
             // KMJ: not actually unloading the plugin yet
             //appinstance->unload();
             //qDebug() << appinstance->isLoaded();
             break;
         }
      }
  }
}


void Program::unloadcadmodule(RCCHAR *path)
{
    typedef char *(WINAPI *startfn)(InterfaceTable *it);
    startfn startmodule;
    QString qpath(path);

    qDebug() << "unloadcadmodule - loadapp path : " << qpath;

     SetDllDirectory(home.getApplicationDir());

     QFile libfile(qpath);
     if(libfile.exists())
     {
         QLibrary *appinstance = new QLibrary(qpath,app);
         if(appinstance->load())
         {
               startmodule = (startfn)appinstance->resolve("startmodule");
               if (startmodule != 0)
               {
                    // run the start function
                    //startmodule(&it);
                    // app instance comparison does not work with QLibrary objects
                    // so we are comparing library file names
                    QLibrary *listinstance;
                    int i=0;
                    if(apps.length() > 0)
                    {
                        for (apps.start(),i = 0 ; (listinstance = (QLibrary*)apps.next()) != 0 ; i++)
                        {
                            if(listinstance->fileName() == appinstance->fileName())
                            {
                                listinstance->unload();
                                if(apps.inlist(listinstance))
                                   apps.del(listinstance);
                                break;
                            }
                        }
                    }
               }
               appinstance->unload();
         }
     }
     return ;
}

void Program::checkcustombutton(int cid)
{
    // custom button menus
    ButtonMenu *bm;
    for (custommenulist.start() ; (bm = (ButtonMenu *) custommenulist.next()) != 0 ; )
        bm->checkbutton(cid);

}

void Program::resetcustommenus()
{
    // custom button menus
    ButtonMenu *bm;
    for (custommenulist.start() ; (bm = (ButtonMenu *) custommenulist.next()) != 0 ; )
        bm->resetlayout();
}

int WINAPI InsertPoint(Point *p,PointE **e)
{ *e = new PointE(*p);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertLine(Point *p1,Point *p2,Line **e)
{ *e = new Line(*p1,*p2);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertCircle(Point *org,Point *xaxis,Point *yaxis,double r,double starta,double sweepa,Circle **e)
{ *e = new Circle(*org,*xaxis,*yaxis,r,starta/180.0*M_PI,sweepa/180.0*M_PI);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertEllipse(Point *org,Point *xaxis,Point *yaxis,double r1,double r2,double starta,double sweepa,EllipseE **e)
{ *e = new EllipseE(*org,*xaxis,*yaxis,r1,r2,starta/180.0*M_PI,sweepa/180.0*M_PI);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertCurve(int n1,int d,Point *p,double *k,double *w,long o,Curve **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new Curve(n1,d,p,k,w,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertTextEx(char *font,Point org,Point xa,Point ya,double w,double h,double s,double a,Byte hc,char *t,char *l,long o,Text **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new Text(_RCT(font),org,xa,ya,w,h,s,a,hc,_RCT(t),l !=  0 ?_RCT(l) : 0,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertText(char *font,Point org,Point xa,Point ya,double w,double h,double s,double a,char *t,char *l,long o,Text **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new Text(_RCT(font),org,xa,ya,w,h,s,a,0,_RCT(t),l != 0 ? _RCT(l) : 0,options);
  return db.geometry.add(*e,drawing);
}



int WINAPI InsertGroup(EntityList *list,const char *clas,const char *instance,const long o,Group **e)
{
  BitMask options(32);
  int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new Group(*list,_RCT(clas),_RCT(instance),options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertBitMapE( char *name,Point org,Point xa,Point ya,double w,double h,long o,BitMapE **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new BitMapE(_RCT(name),org,xa,ya,w,h,1.0,1.0,1.0,0.004,1.0,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertBitMapEEx(char *name,Point org,Point xa,Point ya,double w,double h,double tr,double tg,double tb,double trange,double op,long o,BitMapE **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new BitMapE(_RCT(name),org,xa,ya,w,h,tr,tg,tb,trange,op,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertPatch(int n1,int n2,int d1,int d2,int xm,int ym,Point *p,double *k1,double *k2,
                                  double *w,EntityList *tcurves,long o,Patch **e)
{BitMask options(32);
 int i;
 EntityList tc;
  if (tcurves != 0)
    tc = *tcurves;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);
  *e = new Patch(n1,n2,d1,d2,xm,ym,p,k1,k2,w,tc,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertFigure(char *l,char *n,Point org,Point xa,Point ya,Point s,Point r,Byte s1,Byte s2,long o,Figure **e)
{
  BitMask options(32);
  int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  // it is safer to copy to a temp RCCHAR here for conversion from char
  RCCHAR *tmpl = new RCCHAR[QString(l).length()+1];
  strcpy(tmpl,l);
  RCCHAR *tmpn = new RCCHAR[QString(n).length()+1];
  strcpy(tmpn,n);

  *e = new Figure(tmpl,tmpn,org,xa,ya,s,r,Point(0.0,0.0,0.0),0,s1,s2,0,options);

  // delete the temp memory
  delete [] tmpl; tmpl=0;
  delete [] tmpn; tmpn=0;

  return db.geometry.add(*e,drawing);
}

int WINAPI InsertFigureEx(char *l,char *n,Point org,Point xa,Point ya,Point s,Point r,Point size,int ncopies,Byte s1,Byte s2,long o,Figure **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  // it is safer to copy to a temp RCCHAR here for conversion from char
  RCCHAR *tmpl = new RCCHAR[QString(l).length()+1];
  strcpy(tmpl,l);
  RCCHAR *tmpn = new RCCHAR[QString(n).length()+1];
  strcpy(tmpn,n);

  *e = new Figure(tmpl,tmpn,org,xa,ya,s,r,size,ncopies,s1,s2,0,options);

  // delete the temp memory
  delete [] tmpl; tmpl=0;
  delete [] tmpn; tmpn=0;

  return db.geometry.add(*e,drawing);
}

int WINAPI InsertRuledSurface(Entity *e1,Entity *e2,int xmesh,int ymesh,long o,RuledSurface **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *e = new RuledSurface((LinearEntity *)e1,(LinearEntity *)e2,xmesh,ymesh,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertSurfaceOfRev(Point origin,Point axis,double angle,double dz,Entity *se,int xmesh,int ymesh,long o,SurfaceOfRev **e)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *e = new SurfaceOfRev(origin,axis,angle,dz,(LinearEntity *)se,xmesh,ymesh,options);
  return db.geometry.add(*e,drawing);
}

int WINAPI InsertPlane(EntityList *list,double xs,double ys,double a,double ca,long o,Plane **e)
{BitMask options(32);
 int i;
 Loops loops(1);

  *e = 0;
  loops.create(list,drawing&1);
  if (! list->empty() || loops.getloops().empty())
    {  list->destroy();
       loops.destroy();
       return 0;
    }

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  *e = new Plane(loops.getloops(),xs,ys,a,ca,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
  if ((*e)->getdefined())
    return db.geometry.add(*e,drawing);
  else
    {  delete *e;
       *e = 0;
       return 0;
    }

}

int WINAPI InsertPlaneEx(EntityList *list,double xs,double ys,double a,double ca,char *patternfilename,long o,Plane **e)
{BitMask options(32);
 int i;
 Loops loops(1);

  *e = 0;
  loops.create(list);
  if (! list->empty() || loops.getloops().empty())
    {  list->destroy();
       loops.destroy();
       return 0;
    }

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  //if (patternfilename != 0 && strlen(patternfilename) > 0)
  //  options.set(PlanePatterned);

  // its safer to copy to a tmp RCCHAR for conversion from char
  RCCHAR *tmp = new RCCHAR[QString(patternfilename).length()+1];
  strcpy(tmp,patternfilename);

  *e = new Plane(loops.getloops(),xs,ys,a,ca,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),tmp,options);

  // delete the temp memory
  delete [] tmp; tmp=0;

  if ((*e)->getdefined())
    return db.geometry.add(*e,drawing);
  else
    {  delete *e;
       *e = 0;
       return 0;
    }

}


int WINAPI InsertPlaneExEx(EntityList *list,double xs,double ys,double a,double ca,double trans,double tr,double tg,double tb,double trange,RCCOLORREF gc,char *patternfilename,long o,Plane **e)
{BitMask options(32);
 int i;
 Loops loops(1);

  *e = 0;
  loops.create(list);
  if (! list->empty() || loops.getloops().empty())
    {  list->destroy();
       loops.destroy();
       return 0;
    }

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  //if (patternfilename != 0 && strlen(patternfilename) > 0)
  //  options.set(PlanePatterned);

  // its safer to copy to a tmp RCCHAR for conversion from char
  RCCHAR *tmp = new RCCHAR[QString(patternfilename).length()+1];
  strcpy(tmp,patternfilename);

  *e = new Plane(loops.getloops(),xs,ys,a,ca,trans,tr,tg,tb,trange,gc,tmp,options);

  // delete the temp memory
  delete [] tmp; tmp=0;

  if ((*e)->getdefined())
    return db.geometry.add(*e,drawing);
  else
    {  delete *e;
       *e = 0;
       return 0;
    }

}

int WINAPI InsertLight(Point org,Point d,double in,double s,double fd,double fe,double ca,double c,long o,Light **l)
{BitMask options(32);
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *l = new Light(org,d,in,s,fd,fe,ca,c,options);
  return db.geometry.add(*l,drawing);
}

void CopyTextInfo(TextInfo *ti,const APITextInfo *apiti)
{int i;
  //ti->font = CHAR2RCCHR(apiti->font);
  ti->font = new RCCHAR[strlen(apiti->font)+1];
  strcpy(ti->font,_RCT(apiti->font));
  //ti->text = CHAR2RCCHR(apiti->text);
  ti->text = new RCCHAR[strlen(apiti->text)+1];
  strcpy(ti->text,_RCT(apiti->text));
  ti->width = apiti->width;
  ti->height = apiti->height;
  ti->angle = apiti->angle;
  ti->vertspacing = apiti->vertspacing;
  if ((apiti->options & (1 << 31)) != 0)
    ti->halocolour = apiti->halocolour;
  ti->options.clearall();
  for (i = 0 ; i < 32 ; i++)
    if ((apiti->options & (1L <<i)) != 0)
      ti->options.set(i);
}

int WINAPI InsertAngularDimension(Point org,Point xa,Point ya,Point aorg,int n,Point *l,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const long o,AngularDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32);
 int i;
 PointList pl;
  for (i = 0 ; i < n ; i++)
    pl.add(l[i]);
  pl.reverse();
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti1);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new AngularDimension(org,xa,ya,aorg,pl,e,g,s,ut,lt,1.0,0.0,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertDistanceBearingDimension(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const long o,DistanceBearingDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32),display(32);
 int i;
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti1);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((disp & (1L <<i)) != 0)
      display.set(i);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new DistanceBearingDimension(org,xa,ya,start,end,display,e,g,s,ut,lt,1.0,0.0,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertCircularDimension(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const long o,CircularDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32),style(32);
 int i;
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti1);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((sty & (1L <<i)) != 0)
      style.set(i);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new CircularDimension(org,xa,ya,pnt1,pnt2,style,c,e,g,s,ut,lt,1.0,0.0,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertLinearDimension(Point org,Point xa,Point ya,int n,Point *l,double e,double g,double s,double ut,double lt,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const long o,LinearDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 int i;
 BitMask options(32);
 SortedPointList pl;
 CoordinateSystem cs(org,xa,ya);
  for (i = 0 ; i < n ; i++)
    pl.add(cs.frommodel(l[i]));
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti1);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new LinearDimension(org,xa,ya,pl,e,g,s,ut,lt,1.0,0.0,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertLinearDimensionEx(Point org,Point xa,Point ya,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const long o,LinearDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 int i;
 BitMask options(32);
 SortedPointList pl;
 CoordinateSystem cs(org,xa,ya);
  for (i = 0 ; i < n ; i++)
    pl.add(cs.frommodel(l[i]));
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti1);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new LinearDimension(org,xa,ya,pl,e,g,s,ut,lt,sc,tyo,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI GetPointInfo(PointE *pe,Point *p)
{ *p = pe->getp();
  return 1;
}

int WINAPI GetLineInfo(Line *l,Point *p1,Point *p2)
{ *p1 = l->getp1();
  *p2 = l->getp2();
  return 1;
}

int WINAPI GetCircleInfo(Circle *c,Point *org,Point *xaxis,Point *yaxis,double *r,double *starta,double *sweepa)
{ *org = c->getorigin();
  *xaxis = c->getxaxis();
  *yaxis = c->getyaxis();
  *r = c->getradius();
  *starta = c->getstarta()*180.0/M_PI;
  *sweepa = c->getsweepa()*180.0/M_PI;
  return 1;
}

int WINAPI GetEllipseInfo(EllipseE *e,Point *org,Point *xaxis,Point *yaxis,double *r1,double *r2,double *starta,double *sweepa)
{ *org = e->getorigin();
  *xaxis = e->getxaxis();
  *yaxis = e->getyaxis();
  *r1 = e->getmajor();
  *r2 = e->getminor();
  *starta = e->getstarta()*180.0/M_PI;
  *sweepa = e->getsweepa()*180.0/M_PI;
  return 1;
}

int WINAPI GetCurveInfo(Curve *curve,int *n1,int *d,Point **p,double **k,double **w,long *options)
{int i;
  *n1 = curve->getn();
  *d = curve->getdegree();
  *p = curve->getpolygon();
  *k = curve->getknot();
  *w = curve->getcweight();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (curve->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetTextInfo(Text *text,char **f,Point *org,Point *xaxis,Point *yaxis,double *w,double *h,double *s,double *a,char **t,char **l,long *options)
{int i;
  //*f = (char*)RCPRINTABLE(text->getfont());
  *f = text->getcharfont();
  *org = text->getorigin();
  *xaxis = text->getxaxis();
  *yaxis = text->getyaxis();
  *w = text->getwidth();
  *h = text->getheight();
  *s = text->getvertspacing();
  *a = text->getangle();
  //*t = (char*)RCPRINTABLE(text->gettext());
  *t = text->getchartext();
  //*l = (char*)RCPRINTABLE(text->getlabel());
  *l = text->getcharlabel();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (text->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetGroupInfo(Group *group,EntityList **list,char **clas,char **instance,long *options)
{int i;
  *list = new EntityList;
  **list = group->getlist();
  //*clas = RCHR2LOCAL(*clas,group->getclassname());
  *clas = group->getcharclassname();
  //*instance = RCHR2LOCAL(*instance,group->getinstance());
  *instance = group->getcharinstance();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (group->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetBitMapEInfo(BitMapE *bitmape,char **name,Point *org,Point *xa,Point *ya,double *w,double *h,long *options)
{int i;
  static char sname[600];
  strcpy(sname,QString(bitmape->getfilename()).toLatin1().data());
  *name = sname;
  //*name = RCHR2LOCAL(*name,bitmape->getfilename());
  *org = bitmape->getorigin();
  *xa = bitmape->getxaxis();
  *ya = bitmape->getyaxis();
  *w = bitmape->getwidth();
  *h = bitmape->getheight();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (bitmape->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetBitMapEInfoEx(BitMapE *bitmape,char **name,Point *org,Point *xa,Point *ya,double *w,double *h,double *tr,double *tg,double *tb,double *trange,double *op,long *options)
{int i;
  static char sname[600];
  strcpy(sname,QString(bitmape->getfilename()).toLatin1().data());
  *name = sname;
  //*name = RCHR2LOCAL(*name,bitmape->getfilename());
  *org = bitmape->getorigin();
  *xa = bitmape->getxaxis();
  *ya = bitmape->getyaxis();
  *w = bitmape->getwidth();
  *h = bitmape->getheight();

  *tr = bitmape->getred();
  *tg = bitmape->getgreen();
  *tb = bitmape->getblue();
  *op = bitmape->getopacity();
  *trange = bitmape->gettransparencyrange();

  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (bitmape->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}


int WINAPI GetPatchInfo(Patch *patch,int *n1,int *n2,int *d1,int *d2,int *xm,int *ym,Point **p,double **k1,double **k2,
                        double **w,EntityList **tcurves,long *options)
{int i;
  *n1 = patch->getun();
  *n2 = patch->getvn();
  *xm = patch->getxmesh();
  *ym = patch->getymesh();
  *d1 = patch->getudegree();
  *d2 = patch->getvdegree();
  *p = patch->getpolygon();
  *k1 = patch->getuknot();
  *k2 = patch->getvknot();
  *w = patch->getcweight();
  *tcurves = new EntityList;
  **tcurves = patch->gettrimcurves();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (patch->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetFigureInfo(Figure *figure,char **library,char **name,Point *org,Point *xaxis,Point *yaxis,Point *s,Point *r,Byte *s1,Byte *s2,long *options)
{int i;
  static char slibrary[500],sname[500];
  //*library = RCHR2LOCAL(*library,figure->getlibrary());
  //*name = RCHR2LOCAL(*name,figure->getaltfilename());
  strcpy(slibrary,QString(figure->getlibrary()).toLatin1().data());
  *library = slibrary;
  //*library = QString(figure->getlibrary()).toLatin1().data();
  strcpy(sname,QString(figure->getaltfilename()).toLatin1().data());
  *name = sname;
  //*name = QString(figure->getaltfilename()).toLatin1().data();
  *org = Point(figure->gettransform()->mat(0,3),figure->gettransform()->mat(1,3),figure->gettransform()->mat(2,3));
  *xaxis = Point(1.0,0.0,0.0);
  *yaxis = Point(0.0,1.0,0.0);
  *s = Point(sqrt(figure->gettransform()->mat(0,0)*figure->gettransform()->mat(0,0) + figure->gettransform()->mat(1,0) * figure->gettransform()->mat(1,0) + figure->gettransform()->mat(2,0) * figure->gettransform()->mat(2,0)),
             sqrt(figure->gettransform()->mat(0,1)*figure->gettransform()->mat(0,1) + figure->gettransform()->mat(1,1) * figure->gettransform()->mat(1,1) + figure->gettransform()->mat(2,1) * figure->gettransform()->mat(2,1)),
             sqrt(figure->gettransform()->mat(0,2)*figure->gettransform()->mat(0,2) + figure->gettransform()->mat(1,2) * figure->gettransform()->mat(1,2) + figure->gettransform()->mat(2,2) * figure->gettransform()->mat(2,2)));
  *r = Point(0.0,0.0,atan2(figure->gettransform()->mat(1,0),figure->gettransform()->mat(0,0))*180.0/acos(-1.0));
  *s1 = figure->getstate1();
  *s2 = figure->getstate2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (figure->getoptions()->test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetFigureInfoEx(Figure *figure,char **library,char **name,Point *org,Point *xaxis,Point *yaxis,Point *s,Point *r,Point *si,int *nc,Byte *s1,Byte *s2,long *options)
{
  int i;
  static char slibrary[500],sname[500];
  strcpy(slibrary,QString(figure->getlibrary()).toLatin1().data());
  *library = slibrary;
  //*library = QString(figure->getlibrary()).toLatin1().data();
  strcpy(sname,QString(figure->getaltfilename()).toLatin1().data());
  //*name = QString(figure->getaltfilename()).toLatin1().data();
  *name = sname;
  *org = Point(figure->gettransform()->mat(0,3),figure->gettransform()->mat(1,3),figure->gettransform()->mat(2,3));
  *xaxis = Point(1.0,0.0,0.0);
  *yaxis = Point(0.0,1.0,0.0);
  *s = Point(sqrt(figure->gettransform()->mat(0,0)*figure->gettransform()->mat(0,0) + figure->gettransform()->mat(1,0) * figure->gettransform()->mat(1,0) + figure->gettransform()->mat(2,0) * figure->gettransform()->mat(2,0)),
             sqrt(figure->gettransform()->mat(0,1)*figure->gettransform()->mat(0,1) + figure->gettransform()->mat(1,1) * figure->gettransform()->mat(1,1) + figure->gettransform()->mat(2,1) * figure->gettransform()->mat(2,1)),
             sqrt(figure->gettransform()->mat(0,2)*figure->gettransform()->mat(0,2) + figure->gettransform()->mat(1,2) * figure->gettransform()->mat(1,2) + figure->gettransform()->mat(2,2) * figure->gettransform()->mat(2,2)));
  *r = Point(0.0,0.0,atan2(figure->gettransform()->mat(1,0),figure->gettransform()->mat(0,0))*180.0/acos(-1.0));
  *s1 = figure->getstate1();
  *s2 = figure->getstate2();
  *nc = figure->getncopies();
  *si = figure->getsize();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (figure->getoptions()->test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetRuledSurfaceInfo(RuledSurface *rs,Entity **e1,Entity **e2,int *xmesh,int *ymesh,long *options)
{int i;
  *e1 = rs->gete1();
  *e2 = rs->gete2();
  *xmesh = rs->getxmesh();
  *ymesh = rs->getymesh();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (rs->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetSurfaceOfRevInfo(SurfaceOfRev *sr,Point *origin,Point *zaxis,double *angle,double *deltaz,Entity **e,int *xmesh,int *ymesh,long *options)
{int i;
  *origin = sr->getorigin();
  *zaxis = sr->getaxis();
  *angle = sr->getangle();
  *deltaz = sr->getdeltaz();
  *e = sr->gete();
  *xmesh = sr->getxmesh();
  *ymesh = sr->getymesh();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (sr->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetPlaneInfo(Plane *plane,EntityList **list,double *xs,double *ys,double *a,double *ca,long *options)
{int i;
  *list = new EntityList;
  **list = plane->getlist();
  *xs = plane->getxspacing();
  *ys = plane->getyspacing();
  *a = plane->getangle();
  *ca = plane->getcrossangle();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (plane->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetLightInfo(Light *light,Point *org,Point *d,double *i,double *s,double *fd,double *fe,double *ca,double *c,long *o)
{
  *org = light->getorigin();
  *d = light->getdirection();
  *i = light->getintensity();
  *s = light->getsize();
  *fd = light->getfadedistance();
  *fe = light->getfadeexponent();
  *ca = light->getconeangle();
  *c = light->getconcentration();

  return 1;

}

#define MAX_DIMENSIONPOINTS 100
static Point DimensionPoints[MAX_DIMENSIONPOINTS];

void CopyTextInfo(APITextInfo *apiti,TextInfo *ti)
{
  int i;
  apiti->font = new char[strlen(ti->font)+1];
  strcpy(apiti->font,ti->font);
  apiti->text = new char[strlen(ti->text)+1];
  strcpy(apiti->text,ti->text);
  apiti->width = ti->width;
  apiti->height = ti->height;
  apiti->angle = ti->angle;
  apiti->vertspacing = ti->vertspacing;
  apiti->options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (ti->options.test(i))
        apiti->options |= (1 << i);
}

int WINAPI GetAngularDimensionInfo(AngularDimension *ad,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *options)
{int i;
 PointList pl;
  *org = ad->getorigin();
  *xa = ad->getxaxis();
  *ya = ad->getyaxis();
  *aorg = ad->getdorigin();
  pl = ad->getlist();
  for (pl.start(),i = 0 ; ! pl.atend() ; )
    DimensionPoints[i++] = pl.next();
  *n = pl.length();
  *l = DimensionPoints;
  *g = ad->getgap();
  *e = ad->getextension();
  *ut = ad->getuppertol();
  *lt = ad->getlowertol();
  *s = ad->getstackdis();
  *p = ad->getprecision();
  *tp = ad->gettolprecision();
  CopyTextInfo(ti1,ad->getvaluetextinfo());
  CopyTextInfo(ti2,ad->getprefixtextinfo());
  CopyTextInfo(ti3,ad->getsuffixtextinfo());
  *t1 = *ad->getterminfo1();
  *t2 = *ad->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (ad->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetDistanceBearingDimensionInfo(DistanceBearingDimension *dbd,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *options)
{int i;
  *org = dbd->getorigin();
  *xa = dbd->getxaxis();
  *ya = dbd->getyaxis();
  *start = dbd->getp1();
  *end = dbd->getp2();
  *disp = 0;
  for (i = 0 ; i < 32 ; i++)
    if (dbd->getdisplay().test(i))
      *options |= (1 << i);
  *e = dbd->getextension();
  *g = dbd->getgap();
  *ut = dbd->getuppertol();
  *lt = dbd->getlowertol();
  *s = dbd->getstackdis();
  *p = dbd->getprecision();
  *tp = dbd->gettolprecision();
  CopyTextInfo(ti1,dbd->getvaluetextinfo());
  CopyTextInfo(ti2,dbd->getprefixtextinfo());
  CopyTextInfo(ti3,dbd->getsuffixtextinfo());
  *t1 = *dbd->getterminfo1();
  *t2 = *dbd->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (dbd->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetCircularDimensionInfo(CircularDimension *cd,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *options)
{int i;
  *org = cd->getorigin();
  *xa = cd->getxaxis();
  *ya = cd->getyaxis();
  *pnt1 = cd->getp1();
  *pnt2 = cd->getp2();
  *c = cd->getcircle();
  *sty = 0;
  for (i = 0 ; i < 32 ; i++)
    if (cd->getstyle().test(i))
      *sty |= (1 << i);
  *g = cd->getgap();
  *e = cd->getextension();
  *ut = cd->getuppertol();
  *lt = cd->getlowertol();
  *s = cd->getstackdis();
  *p = cd->getprecision();
  *tp = cd->gettolprecision();
  CopyTextInfo(ti1,cd->getvaluetextinfo());
  CopyTextInfo(ti2,cd->getprefixtextinfo());
  CopyTextInfo(ti3,cd->getsuffixtextinfo());
  *t1 = *cd->getterminfo1();
  *t2 = *cd->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (cd->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetLinearDimensionInfo(LinearDimension *ld,Point *org,Point *xa,Point *ya,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,long *options)
{int i;
 SortedPointList spl;
  *org = ld->getorigin();
  *xa = ld->getxaxis();
  *ya = ld->getyaxis();
  CoordinateSystem cs(*org,*xa,*ya);
  spl = ld->getlist();
  *n = spl.length();
  for (spl.start(),i = 0 ; ! spl.atend() ; )
    DimensionPoints[i++] = cs.tomodel(spl.next());
  *l = DimensionPoints;
  *g = ld->getgap();
  *e = ld->getextension();
  *ut = ld->getuppertol();
  *lt = ld->getlowertol();
  *s = ld->getstackdis();
  *p = ld->getprecision();
  *tp = ld->gettolprecision();
  CopyTextInfo(ti1,ld->getvaluetextinfo());
  CopyTextInfo(ti2,ld->getprefixtextinfo());
  CopyTextInfo(ti3,ld->getsuffixtextinfo());
  *t1 = *ld->getterminfo1();
  *t2 = *ld->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (ld->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetDefaultHeaderInfo(int *layer,int *colour,int *style,int *weight,BitMask **visible)
{ *layer = db.header.getlayer();
  *colour = db.header.getcolour();
  *style = db.header.getstyle();
  *weight = db.header.getweight();
  *visible = db.header.getvisible();
  return 1;
}

int WINAPI SetDefaultHeaderInfo(int layer,int colour,int style,int weight,BitMask *visible)
{ db.header.change(colour,layer,style,weight);
  db.header.setvisible(*visible);
  db.dismask.layer.set(layer,1);
  v.setinteger("df::layer",layer);
  v.setinteger("df::colour",colour);
  v.setinteger("df::style",style);
  v.setinteger("df::weight",weight);
  if (drawing & 1)
      cadwindow->updatetitle();
  return 1;
}

int WINAPI GetHeaderInfo(Entity *e,int *layer,int *colour,int *style,int *weight)
{ *layer = e->getlayer();
  *colour = e->getcolour();
  *style = e->getstyle();
  *weight = e->getweight();
  return 1;
}

void WINAPI SetHeaderInfo(Entity *e,int layer,int colour,int style,int weight)
{BitMask change(32),fchange(32),options(32);
  change.set(0);
  change.set(1);
  change.set(2);
  change.set(3);
  change.set(31,(drawing & 1) == 0);
  e->change(change,colour&255,layer&2047,style&255,weight&255);
  if (e->isa(figure_entity))
    {//  Change the 'original' option bits as well, based on bit 15.
       fchange.set(12);
       options = *((Figure *)e)->getoptions();
       options.set(FigureOriginalLayer,(layer & 0x8000) != 0);
       options.set(FigureOriginalColour,(colour & 0x8000) != 0);
       options.set(FigureOriginalStyle,(style & 0x8000) != 0);
       options.set(FigureOriginalWeight,(weight & 0x8000) != 0);
       ((Figure *)e)->change(fchange,*e,_RCT(""),_RCT(""),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,options);
       cadwindow->invalidatedisplaylist(e);
       if (drawing & 1)
         e->draw(DM_NORMAL);
    }
}

void WINAPI SetHeaderInfoEx(Entity *e,int layer,int colour,int style,int weight,BitMask *visible)
{BitMask change(32),fchange(32),options(32);
  change.set(0);
  change.set(1);
  change.set(2);
  change.set(3);
  e->setvisible(*visible);
  e->change(change,colour&255,layer&1023,style&255,weight&255);
  if (e->isa(figure_entity))
    {//  Change the 'original' option bits as well, based on bit 15.
       fchange.set(12);
       options = *((Figure *)e)->getoptions();
       options.set(FigureOriginalLayer,(layer & 0x8000) != 0);
       options.set(FigureOriginalColour,(colour & 0x8000) != 0);
       options.set(FigureOriginalStyle,(style & 0x8000) != 0);
       options.set(FigureOriginalWeight,(weight & 0x8000) != 0);
       ((Figure *)e)->change(fchange,*e,_RCT(""),_RCT(""),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,options);
       cadwindow->invalidatedisplaylist(e);
       if (drawing & 1)
         e->draw(DM_NORMAL);
    }

}


long WINAPI GetEntityType(Entity *e)
{ return 1L << e->type();
}

void WINAPI StartList(EntityList *list)
{ list->start();
}

void WINAPI RemoveList(EntityList *list,Entity *e)
{ list->del(e);
}

Entity *WINAPI NextList(EntityList *list)
{ return list->next();
}

int WINAPI AtEndList(EntityList *list)
{ return list->atend();
}

int WINAPI NewList(EntityList **list)
{ *list = new EntityList;
  return 1;
}

void WINAPI DeleteList(EntityList *list)
{ list->destroy();
  delete list;
}

void WINAPI AddList(EntityList *list,Entity *e)
{ list->add(e);
}

int WINAPI InList(EntityList *list,Entity *e)
{ return list->inlist(e);
}

void WINAPI ReverseList(EntityList *list)
{ list->reverse();
}

void WINAPI CopyList(EntityList *list,EntityList **newlist)
{ *newlist = new EntityList;
  **newlist = list->copy();
}

int WINAPI LengthList(EntityList *list)
{ return list->length();
}

void WINAPI MoveEntity(Entity *e,Point p)
{Transform t;
 EntityList movedlist;
  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  t.translate(p);
  e->dbsetassociated();
  e->setstatus(1);
  if (e->getalist().empty()) e->draw(DM_ERASE);
  e->move(&db,t,1);
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (! e->isa(group_entity) && e->getstatus())
      {  movedlist.add(e);  e->draw(DM_NORMAL);
      }
  //  Note the following code is not proven correct for all cases of nested groups
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (e->isa(group_entity) && e->getstatus() && e->getalist().empty())
      {  movedlist.add(e);  e->draw(DM_NORMAL);
      }
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (e->isa(group_entity) && e->getstatus() && ! e->getalist().empty())
      {  movedlist.add(e);  e->draw(DM_NORMAL);
      }
  db.saveundo(UD_MANYMOVED,&movedlist);
  movedlist.destroy();
  db.saveundo(UD_ENDBLOCK,NULL);
}

Entity *WINAPI CopyEntity(Entity *e,Point p)
{Transform t;
 Entity *copye;

  e->setcopye(NULL);
  e->setstatus(0);

  t.translate(p);
  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);
  db.geometry.add(copye = e->clone(t),drawing);
  db.saveundo(UD_ENDBLOCK,NULL);

  return copye;

}

void WINAPI DeleteEntity(Entity *e)
{EntityList list;
 int cstate,d;
  list.add(e);
  // if drawing = 3 (drawing&1) = 1 so (drawing&1) == 0 = false
  // if drawing = 2 (drawing&1) = 0 so (drawing&1) == 0 = true
  state.setselection(list,(drawing&1) == 0);
  cadwindow->drawent(DM_ERASE,NULL,e);
  d = ! (drawing&1);
  delete_command(&cstate,0,(void **)&d);
}

static void DEACallback(Entity *e,void *l)
{ ((EntityList *)l)->add(e);
}

void WINAPI DeleteEntityAll(Entity *e)
{EntityList list;
 int cstate,d;
  list.add(e);
  e->dbtraverseassociated(DEACallback,&list);
  state.setselection(list,drawing == 0);
  d = ! drawing;
  delete_command(&cstate,0,(void **)&d);
}

void WINAPI GetExtents(Entity *e,Point *p1,Point *p2)
{ e->extents(0,p1,p2);
}

void WINAPI DeleteEntityList(EntityList *list)
{int cstate;
  state.setselection(list->copy(),drawing == 0);
  delete_command(&cstate,0,0);
}

RCHWND WINAPI GetMainHwnd(void)
{ return cadwindow->gethwnd();
}

void WINAPI SetLongValue(Entity *e,long l)
{ e->setcopye((Entity *)l);
}


long WINAPI GetLongValue(Entity *e)
{ return (long) e->getcopye();
}


int WINAPI InsertButtonMenu(int n,int ncol,ButtonMenu **bm)
{
  *bm = new ButtonMenu(0,0,n,ncol,15,(QWidget*)cadwindow->gethwnd());
  return 1;
}

int WINAPI InsertButton(ButtonMenu *bm,char *rname,RCHINSTANCE hinstance,int subcom,char *description,APICommand ccommand)
{
  int commandid,cid;
  commandid = 31000;
  for (apicommandlist.start() ; (cid = (LONG) apicommandlist.next()) != 0 ; )
    {  if (cid > commandid)
         commandid = cid;
       apicommandlist.next();
    }
  commandid++;
  apicommandlist.add((void *)ccommand);
  apicommandlist.add((void *)commandid);
  commands.add(1,commandid,subcom,custom_command17,description);
  //bm->add(new Button(-1,-1,rname,commandid,hinstance));
  bm->add(new Button(-1,-1,rname,commandid,bm->getQW(),hinstance));
  return commandid;
}

int WINAPI InsertCommand(int subcom,char *description,APICommand ccommand)
{
    int commandid,cid;
    commandid = 31000;
    for (apicommandlist.start() ; (cid = (LONG) apicommandlist.next()) != 0 ; )
      {  if (cid > commandid)
           commandid = cid;
         apicommandlist.next();
      }
    commandid++;
    apicommandlist.add((void *)ccommand);
    apicommandlist.add((void *)commandid);
    commands.add(1,commandid,subcom,custom_command17,description);

    return commandid;
}

class CustomAPIMenuWindow : public MenuWindow
  {public:
     CustomAPIMenuWindow(Window *win,int x,int y,int w,int h) : MenuWindow(win,x,y,w,h) {};
     ~CustomAPIMenuWindow();
  };

CustomAPIMenuWindow::~CustomAPIMenuWindow()
{ custommenulist.del(getcontrol(0));
}

int WINAPI ShowButtonMenu(ButtonMenu *bm,int x1,int y1,int ncol,int nrow)
{MenuWindow *mw;
 int x2,y2;

  if (! custommenulist.inlist(bm))
    {  x2 = x1 >= 0 ? x1 : int(cadwindow->getsubwindow()->getwidth() + x1);
       y2 = y1 >= 0 ? y1 : int(cadwindow->getsubwindow()->getheight() + y1);
       mw = new CustomAPIMenuWindow(cadwindow->getsubwindow(),x2,y2,ncol * 24,nrow * 24);
       mw->addcontrol(bm);
       mw->buttonmenu = bm;
       bm->move(mw,0,0,ncol);
       custommenulist.add(bm);
    }

  return 1;
}


int WINAPI ShowButtonMenuEx(char *name,ButtonMenu *bm,int x1,int y1,int ncol,int nrow)
{MenuWindow *mw;
 int x2,y2,xm,ym,wm,hm,mf,bw,bh;
 RCCHAR fullname[300],text[300];

  strcpy(fullname,"ApplicationMenu::");
  strcat(fullname,name);
            
  x2 = x1 >= 0 ? x1 : int(cadwindow->getsubwindow()->getwidth() + x1);
  y2 = y1 >= 0 ? y1 : int(cadwindow->getsubwindow()->getheight() + y1);

  if ( buttonmenus.match(fullname) == 0)
    {
       buttonmenus.add(_RCT(fullname),bm,_RCT(name));

       /*
       if (bm->getnusedbuttons() > 0)
         {  bw = bm->match(-1)->w;
            bh = bm->match(-1)->h;
         }
       else
         {  bw = 24;
            bh = 24;
         }
       */

       //GetPrivateProfileString(_RCT("Layout"),fullname,_RCT(""),text,300,home.getinifilename());
       QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
       QByteArray state = settings.value("Layout/"+QString(fullname),"").toByteArray();
       if(state.size() > 0)
       //if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5)
       {
           RCToolBox::layoutType layouttype = RCToolBox::hLine;
               layouttype = RCToolBox::vLine;
           Qt::Orientation orentation = Qt::Horizontal;
           if(layouttype == RCToolBox::vLine || layouttype == RCToolBox::vBlock)
           if(nrow > ncol)
               orentation = Qt::Vertical;
           RCToolBox *rctb = new RCToolBox(layouttype,ncol,bm->getnusedbuttons(),0,(QtMainWindow*)cadwindow->gethwnd());
           rctb->setTitle(QString(name));
           rctb->setRole(QString(fullname));
           bm->setToolbox(rctb,0);
           //qDebug() << "dock widget object name : " << rctb->objectName();
           // now add it to the main window
           bool restored = ((QtMainWindow*)cadwindow->gethwnd())->restoreDockWidget(rctb);
           if(!restored)
               restored = rctb->restoreGeometry(state);
           if(!restored)
               ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::LeftDockWidgetArea,(QDockWidget*)bm->getQW(),orentation);
           rctb->show();
           rctb->raise();
           rctb->saveState();
           custommenulist.add(bm);
       }
       else 
       {
           //RCCHAR winpos[300];
           //GetPrivateProfileString(_RCT("WindowPlacement"),_RCT("Main"),_RCT(""),winpos,300,home.getinifilename());
           QByteArray state = settings.value("WindowPlacement/Main","").toByteArray();
            //mw = new CustomAPIMenuWindow(cadwindow->getsubwindow(),x2,y2,ncol * bw,nrow * bh);
            //mw->buttonmenu = bm;
            //mw->addcontrol(bm);
            //bm->move(mw,0,0,ncol);
            //cadwindow->buttonmenu(5,buttonmenus.match(fullname));
            RCToolBox::layoutType layouttype = RCToolBox::hLine;
            if(nrow > ncol)
               layouttype = RCToolBox::vLine;
            Qt::Orientation orentation = Qt::Horizontal;
            if(layouttype == RCToolBox::vLine ||layouttype == RCToolBox::vBlock)
               orentation = Qt::Vertical;
            RCToolBox *rctb = new RCToolBox(layouttype,ncol,bm->getnusedbuttons(),0,(QtMainWindow*)cadwindow->gethwnd());
            rctb->setTitle(QString(name));
            rctb->setRole(QString(fullname));
            bm->setToolbox(rctb,0);
            if(state.size() < 1)
            {
                // there is no main window position (might be the first run??)
                // dock to left side of the main window as a default position
                ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::LeftDockWidgetArea,(QDockWidget*)bm->getQW(),orentation);
                rctb->setFloating(false);
                rctb->show();
                rctb->raise();
                rctb->saveState();
            }
            else
            {
                // now add it to the main window and float it
                ((QtMainWindow*)cadwindow->gethwnd())->addDockWidget(Qt::LeftDockWidgetArea,(QDockWidget*)bm->getQW(),orentation);
                rctb->setFloating(true);
                rctb->show();
                rctb->raise();
                rctb->saveState();
            }
            custommenulist.add(bm);
       }
    }
  else
      bm->getQW()->show();

  return 1;
}


static APIRubberbandCallback rubberbandcallback = 0;
static APIRubberbandCallbackEx rubberbandcallbackEx = 0;
static int incallback = 0;
static int APICommandCancelled = 0;
static View3dWindow *lastapiwindow;

void ClearAPICommandCancelled(void)
{  APICommandCancelled = 0;
}

void WINAPI SetRubberbandCallback(APIRubberbandCallback rbc)
{
    rubberbandcallback = rbc;
    // KMj : temporary rubberbanding code
    if(rbc == 0)
        cadwindow->clearAllRubberBands();
}

// KMJ added for figure rubberband fix
void WINAPI SetRubberbandCallbackEx(APIRubberbandCallbackEx rbc)
{
    rubberbandcallbackEx = rbc;
    // KMj : temporary rubberbanding code
    if(rbc == 0)
        cadwindow->clearAllRubberBands();
}

void RBApi(int dm,void *data,Point *p2,View3dSurface *vs)
{
    if (rubberbandcallback != 0)
    {
         if (! incallback)
         {
             incallback = 1;
             if(dm == RB_ERASE)
                 cadwindow->clearAllRubberBands();
             else
                 rubberbandcallback(dm,*p2);
             incallback = 0;
         }
    }
    if (rubberbandcallbackEx != 0)
    {
         if (! incallback)
         {
             incallback = 1;
             if(dm == RB_ERASE)
                 cadwindow->clearAllRubberBands();
             else
                 rubberbandcallbackEx(dm,*p2,vs);
             incallback = 0;
         }
    }
}

void custom_command17(int *cstate,HighLevelEvent *event,void **)
{
  int commandid;
  APICommandCancelled = 0;
  for (apicommandlist.start() ; (commandid = (LONG) apicommandlist.next()) != 0 ; )
    {
       // KMJ modified to pick up selection end events from a plugin
      qDebug() << "event->getcommandid() : " << ((NewCommandEvent *)event)->getcommandid();
       if (commandid == ((NewCommandEvent *)event)->getcommandid())
       {
           ((APICommand)apicommandlist.next())();
            APICommandCancelled = 1;
       }
       else if (commandid == state.getcommandid() && ((EntitySelectedEvent *)event)->getendselected()) // KMJ: added
       {
            //((APICommand)apicommandlist.next())();
            APICommandCancelled = 0;
            return;
       }
       else
         apicommandlist.next();
    }
  APICommandCancelled = 0;
  *cstate = ExitState;
}

void ResetState(int commandactive)
{
  if (commandactive)
    return;

  //app->processEvents();

  state.selmask.entity.setandclear(uv_mask,xyz_mask,undo_mask,linear_mask,surface_mask,uvsurface_mask,
                   lineardimension_entity,angulardimension_entity,
                   distancebearingdimension_entity,circulardimension_entity,end_list);

  cadwindow->prompt->normalprompt(NCWIN+1);
  cadwindow->prompt->setcolour(RGB(0,150,0));

}

int WINAPI SelectPosition(char *prompt,Point *p)
{int modeless;
 
  if (APICommandCancelled != 0)
    return 0;

  modeless = state.getcommandid() == 0;

  lastapiwindow = 0;
  cadwindow->prompt->normalprompt(prompt);
  state.sendevent(0);
  state.selmask.entity.clearandset(xyz_mask,end_list);
  rubberband.begin(0);
  rubberband.add(RBApi,0);
  for (;;)
    {
       //app->processEvents();
       int count=0;
       while (! state.eventavailable())
       {
            if(count > 1000)
            {
              app->processEvents();
              count=0;
            }
              count++;
       }

       if ((modeless || state.getcommandid() >= 31000) && state.getevent()->isa(Coordinate3d))
         {  *p = ((Coordinate3dEvent *)state.getevent())->getp();
            lastapiwindow = ((Coordinate3dEvent *)state.getevent())->getwindow();
            rubberband.end(0);
            ResetState(0);
            return 1;
         }
       else if ((modeless || state.getcommandid() >= 31000) && state.getevent()->isa(Abort))
         {  state.sendevent(0);
            rubberband.end(0);
            return 0;
         }
       else if ((modeless || state.getcommandid() >= 31000) && state.getevent()->isa(Exit))
         {  state.sendevent(0);
            rubberband.end(0);
            APICommandCancelled = 1;
            return 0;
         }
       else
         {  if (state.getevent() != 0 && state.getevent()->isa(NewCommand))
              {int subcommandid;
                 subcommandid = commands.subcommand(((NewCommandEvent *) state.getevent())->getcommandid());
                 if (subcommandid != 1 && subcommandid != 2 && subcommandid != 5 && subcommandid != 6)
                   {  APICommandCancelled = 1;
                      return 0;
                   }
              }
            state.process();
            if (state.getcommandid() == 0)
              {  rubberband.end(0);
                 return 0;
              }
         }
    }
}


int WINAPI SelectEntity(char *prompt,long mask,Entity **e,Point *p)
{int i,commandactive;

  if (APICommandCancelled != 0)
    return 0;

  state.setpickedentity(0);
  cadwindow->prompt->normalprompt(prompt);
  state.sendevent(0);
  state.selmask.entity.clearall();
  for (i = 0 ; i < 32 ; i++)
    if (((1/*L*/ << i) & mask) != 0)
      state.selmask.entity.set(i);

  commandactive = state.getcommandid() != 0;
  if (! commandactive)
    {RCMSG msg;
       while (PeekMessage(&msg,0,WM_LBUTTONDOWN,WM_LBUTTONUP,0))
         program->processmessages(3);
       state.sendevent(0);  //  Clear any existing events
    }

  rubberband.begin(0);
  rubberband.add(RBApi,0);
  for (;;)
    {
      while (! state.eventavailable())
          app->processEvents();
         //program->processmessages(3);

       if ((state.getcommandid() >= 31000 || ! commandactive) && state.getevent()->isa(Coordinate3d))
         {  *p = ((Coordinate3dEvent *)state.getevent())->getp();
            *e = 0;
            ResetState(commandactive);
            rubberband.end(0);
            return 1;
         }
       else if ((state.getcommandid() >= 31000 || ! commandactive) && state.getevent()->isa(EntitySelected))
         {  *p = ((EntitySelectedEvent *)state.getevent())->getp();
            *e = ((EntitySelectedEvent *)state.getevent())->getentity();
            ResetState(commandactive);
            rubberband.end(0);
            return 1;
         }
       else if ((state.getcommandid() >= 31000 || ! commandactive) && state.getevent()->isa(Abort))
         {  state.sendevent(0);
            ResetState(commandactive);
            //ResetState(0);
            rubberband.end(0);
            return 0;
         }
       else if ((state.getcommandid() >= 31000 || ! commandactive) && state.getevent()->isa(Exit))
         {  state.sendevent(0);
            ResetState(commandactive);
            rubberband.end(0);
            return 0;
         }
       else
         {  if (state.getevent() != 0 && state.getevent()->isa(NewCommand))
              {int subcommandid;
                 subcommandid = commands.subcommand(((NewCommandEvent *) state.getevent())->getcommandid());
                 if (subcommandid != 1 && subcommandid != 2 && subcommandid != 5 && subcommandid != 6)
                   {  APICommandCancelled = 1;
                      ResetState(commandactive);
                      rubberband.end(0);
                      return 0;
                   }
              }
            state.process();
            if (commandactive && state.getcommandid() == 0)
              {  ResetState(commandactive);
                 rubberband.end(0);
                 return 0;
              }
         }

    }

}

void WINAPI NewDialog(char *name,Dialog **dialog)
{ *dialog = new Dialog(name);
}

void WINAPI DeleteDialog(Dialog *dialog)
{ delete dialog;
}

int WINAPI ProcessDialog(Dialog *dialog,RCHWND hwnd,RCHINSTANCE hinstance)
{ return dialog->process(hwnd,hinstance);
}

void WINAPI InsertStringControl(Dialog *dialog,int id,char *string,int len)
{ dialog->add(new StringDialogControl(id,_RCT(string),len));
}

void WINAPI InsertIntegerControl(Dialog *dialog,int id,int *v,int s,int imin,int imax)
{DialogControl *dc;
  dialog->add(dc = new IntegerDialogControl(id,v,imin,imax));
  if (s != 0)
    dialog->add(new ScrollBarDialogControl(id+1000,dc));
}

void WINAPI InsertRealControl(Dialog *dialog,int id,double *v,double s,double xmin,double xmax)
{DialogControl *dc;
  dialog->add(dc = new RealDialogControl(id,v,s,xmin,xmax));
  if (s != 0.0)
    dialog->add(new ScrollBarDialogControl(id+1000,dc));
}

void WINAPI InsertCheckBoxControl(Dialog *dialog,int id,int *v)
{ dialog->add(new CheckBoxDialogControl(id,v));
}

void WINAPI InsertRadioButtonControl(Dialog *dialog,int startid,int endid,int *v)
{int i;
  for (i = startid ; i <= endid ; i++)
    dialog->add(new RadioButtonDialogControl(i,startid,endid,v));
}

void customtimerevent(void)
{ButtonMenu *bm;
  for (custommenulist.start() ; (bm = (ButtonMenu *) custommenulist.next()) != 0 ; )
    bm->timerevent();
}

void WINAPI StartDatabaseList(EntityList *list)
{ *list = *db.geometry.getlist();
  list->start();
}

void WINAPI FileNew(int)
{
    db.initialize();
    //app->processEvents();
}

void WINAPI FileOpen(const char *filename)
{CadFile infile;
  if (infile.open(_RCT(filename),"rb"))
    {  db.clear();
       db.settitle(_RCT(filename));
       cadwindow->updatetitle();
       db.setname(_RCT(filename));
       if (! db.load(&infile))
         db.initialize();
       infile.close();
       cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
       cadwindow->paintall();
    }
}

void WINAPI FileSaveAs(const char *filename)
{CadFile outfile;
  db.setname(_RCT(filename));
  if (outfile.open(db.getname(),"wb"))
    {  db.save(&outfile);
       outfile.close();
       if (v.getinteger("db::autosaveenabled") != 0)
         program->setautosavetimer();

    }
}

extern int filesaveselectionas(EntityList *list,const RCCHAR *filename, const RCCHAR *filepath=0);
extern int filesaveselectionas(EntityList *list,const char *filename, const char *filepath=0);

int WINAPI FileExport(EntityList *list,const char *filename)
{CadFile outfile;
 char fn[300];

  strcpy(fn,filename);
  _strlwr(fn);
  if (strstr(fn,".cad") != 0)
    return filesaveselectionas(list,filename);
  else if (strstr(fn,".dxf") != 0)
    {DXFOut dxffile;
       if (dxffile.open((char *)filename))
         {  dxffile.write(list);
            dxffile.close();
            return 1;
         }
    }
  

  return 0;

}

void WINAPI DeleteAttribute(Entity *e,const char *a)
{AttributeList atlist;
 RCCHAR *at;
  atlist = e->getatlist().copy();
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    if (QString(a) == QString(at))
      {  e->delat(at);  e->setattribute(at);
         db.saveundo(UD_DELETEATT,e);
         break;    
      }
  atlist.destroy();
}

void WINAPI AddAttribute(Entity *e,const char *a)
{RCCHAR *at;
  at = new RCCHAR[strlen(a) + 1];
  strcpy(at,a);
  e->addat(at);  e->setattribute(at);
  db.saveundo(UD_INSERTATT,e);
}

char *WINAPI GetAttribute(Entity *e,int index)
{
  static char temp[600]="";
  temp[0] = 0;
  AttributeList atlist;
  RCCHAR *at;
  atlist = e->getatlist();
  for (atlist.start() ; (at = atlist.next()) != NULL ; index--)
    if (index == 0)
    {
        strcpy(temp,at);
        return temp;
    }
  return 0;
}

void WINAPI NewBitMask(BitMask **bitmask,int n)
{ *bitmask = new BitMask(n);
}

void WINAPI SetBitMask(BitMask *newbitmask,BitMask *oldbitmask)
{ *newbitmask = *oldbitmask;
}

int WINAPI GetBitMaskBit(BitMask *bitmask,int bit)
{ return bitmask->test(bit);
}

void WINAPI SetBitMaskBit(BitMask *bitmask,int bit,int value)
{ bitmask->set(bit,value);
}

void WINAPI SetLayerName(int i,char *name)
{ db.layernames.setlayername(i,_RCT(name));
}

char *WINAPI GetLayerName(int i)
{
    // no unicode here yet
    static char temp[300];
    strcpy(temp,db.layernames.getlayername(i));
    return temp;
    //return (char*)db.layernames.getlayername(i);
}

void WINAPI SetLayerDisplayMask(int i,int value)
{ db.dismask.layer.set(i,value);
}

int WINAPI GetLayerDisplayMask(int i)
{ return db.dismask.layer.test(i);
}

//  Move entity e2 to just before the reference entity e1
void WINAPI MoveEntityAfter(Entity *e1,Entity *e2)
{int i,i1,i2;
 Entity *e;


  if (e1 == 0)
    {  db.geometry.toback(e2);
       return;
    }

  //  Find the positions of two entities e1 and e2
  for (db.geometry.start(),i1 = 0,i2 = 0,i = 0 ; (e = db.geometry.next()) != 0 ; i++)
    {  if (e == e1)
         i1 = i;
       if (e == e2)
         i2 = i;
    }

  //  Now want to make i2 == i1 - 1
  if (i2 < i1 - 1)
    for (i = 0 ; i < i1 - 1 - i2 ; i++)
      db.geometry.backward(e2);
  else if (i2 > i1)
    for (i = 0 ; i < i2 - i1 ; i++)
      db.geometry.forward(e2);

}

//  Move entity e2 to just after the reference entity e1
void WINAPI MoveEntityBefore(Entity *e1,Entity *e2)
{int i,i1,i2;
 Entity *e;

  if (e1 == 0)
    {  db.geometry.tofront(e2);
       return;
    }

  //  Find the positions of two entities e1 and e2
  for (db.geometry.start(),i1 = 0,i2 = 0,i = 0 ; (e = db.geometry.next()) != 0 ; i++)
    {  if (e == e1)
         i1 = i;
       if (e == e2)
         i2 = i;
    }

  //  Now want to make i2 == i1 + 1
  if (i2 < i1)
    for (i = 0 ; i < i1 - i2 ; i++)
      db.geometry.backward(e2);
  else if (i2 > i1)
    for (i = 0 ; i < i2 - i1 - 1 ; i++)
      db.geometry.forward(e2);

}

void WINAPI GetCurrentSelection(EntityList **list)
{ *list = new EntityList;
  **list = state.getselection().copy();
}

void WINAPI SetCurrentSelection(EntityList *list)
{  if (list == 0)
     state.destroyselection(1);
   else
     state.setselection(*list); 
}

void WINAPI HighLightList(EntityList *list,int highlight)
{ if (highlight)
    list->highlight();
  else
    list->unhighlight();
}

void WINAPI HighLight(Entity *e,int highlight)
{ if (highlight)
    e->highlight();
  else
    e->unhighlight();
}

void WINAPI DrawEntity(Entity *e,int drawmode)
{
    if(drawmode == DM_INVERT) // rubberbanding hack
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    e->draw(drawmode);
}

void WINAPI BracketUndo(int start)
{ if (start)
    db.saveundo(UD_STARTBLOCK,NULL);
  else
    db.saveundo(UD_ENDBLOCK,NULL);
}

static void regenerateall(int,void *,View3dSurface *vs)
{ if (typeid(vs) == typeid(class View3dWindow) && ((View3dWindow *)vs)->getoffscreenbitmap() != 0)
    {  ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.clear();
       ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.setorgtransform(vs->getumin(),vs->getvmin(),vs->getumax(),vs->getvmax());
    }
}

void WINAPI Repaint(int regenerate)
{ state.sendevent(0);
  if (regenerate&1)
     cadwindow->drawcallback(0,NULL,regenerateall);
  cadwindow->paintall((regenerate&2)==2,(regenerate&4)==4);
  program->processpaintmessages();
}

int WINAPI InsertComposedDrawing(char *name,double width,double height,int *viewno)
{Entity *e;

  *viewno = db.getnextviewno();
  if (*viewno >= 0)
    {  db.views2d.add(new View2d(_RCT(name),width,height,short(*viewno)));
       //  Make sure no existing entities are visible in this view.
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         e->getvisible()->set(*viewno,0);
    }

  return *viewno >= 0; 
}

int WINAPI GetComposedDrawingInfo(int viewno,char *name,double *width,double *height)
{View2d *view2d;
  
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    {  strcpy(name,view2d->getname());
       *width = view2d->getpaperwidth();
       *height = view2d->getpaperheight();
    }

  return view2d != 0;

}

int WINAPI DeleteComposedDrawing(int viewno)
{View2d *view2d;
  
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    {  cadwindow->deletewindows(view2d);
       db.views2d.del(view2d);
    }

  return view2d != 0;

}

int WINAPI InsertComposedDrawingView(int viewno,char *name,double umin,double vmin,double umax,double vmax,double scale,double xp,double yp,long o,BitMask *layertable)
{View2d *view2d;
 View *view3d;
 BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
 
  view2d = db.views2d.match(viewno);
  view3d = db.views3d.match(name);
  if (view2d != 0 && view3d != 0)
    {  db.saveundo(UD_CHANGEVIEW2D,view2d);
       view2d->add(new OneView(view3d,umin,vmin,umax,vmax,scale,xp,yp,0.0,options,layertable));
    }

  return (view2d != 0) && (view2d != 0);

}

int WINAPI GetComposedDrawingViewInfo(int viewno,int index,char *name,double *umin,double *vmin,double *umax,double *vmax,double *scale,double *xp,double *yp,long *o,BitMask **layertable)
{OneView *oneview;
 View2d *view2d;
  
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    {  for (view2d->start() ; (oneview = view2d->next()) != 0 ; index--)
         if (index == 0)
           break;

       strcpy(name,oneview->getview()->getname());
       *umin = oneview->getumin();
       *vmin = oneview->getvmin();
       *umax = oneview->getumax();
       *vmax = oneview->getvmax();
       *scale = oneview->getscale();
       *xp = oneview->getxpos();
       *yp = oneview->getypos();
       *layertable = oneview->getlayertable();
       *o = oneview->gethiddenlines() + oneview->getshaded() * 4;   

    }

  return view2d != 0;

}

int WINAPI InsertComposedDrawingViewEx(int viewno,char *name,double umin,double vmin,double umax,double vmax,double angle,double scale,double xp,double yp,long o,BitMask *layertable)
{View2d *view2d;
 View *view3d;
 BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
 
  view2d = db.views2d.match(viewno);
  view3d = db.views3d.match(name);
  if (view2d != 0 && view3d != 0)
    {  db.saveundo(UD_CHANGEVIEW2D,view2d);
       view2d->add(new OneView(view3d,umin,vmin,umax,vmax,scale,xp,yp,angle,options,layertable));
    }

  return (view2d != 0) && (view2d != 0);

}

int WINAPI GetComposedDrawingViewInfoEx(int viewno,int index,char *name,double *umin,double *vmin,double *umax,double *vmax,double *angle,double *scale,double *xp,double *yp,long *o,BitMask **layertable)
{OneView *oneview;
 View2d *view2d;
  
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    {  for (view2d->start() ; (oneview = view2d->next()) != 0 ; index--)
         if (index == 0 || oneview == 0)
           break;

       if (oneview != 0)
         {  strcpy(name,oneview->getview()->getname());
            *umin = oneview->getumin();
            *vmin = oneview->getvmin();
            *umax = oneview->getumax();
            *vmax = oneview->getvmax();
            *angle = oneview->getangle();
            *scale = oneview->getscale();
            *xp = oneview->getxpos();
            *yp = oneview->getypos();
            *layertable = oneview->getlayertable();
            *o = oneview->gethiddenlines() + oneview->getshaded() * 4;   
         }

    }

  return view2d != 0 && oneview != 0;

}


int WINAPI DeleteComposedDrawingView(int viewno,int index)
{OneView *oneview;
 View2d *view2d;
  
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    {  for (view2d->start() ; (oneview = view2d->next()) != 0 ; index--)
         if (index == 0)
           {  db.saveundo(UD_CHANGEVIEW2D,view2d);
              view2d->del(oneview);
           }
    }

  return view2d != 0;

}

int WINAPI ShowComposedDrawing(int viewno)
{View2d *view2d;
 
  view2d = db.views2d.match(viewno);
  if (view2d != 0)
    cadwindow->create2dsubwindow(view2d);

  return view2d != 0;

}

void WINAPI StartFigureList(Figure *figure,EntityList **list)
{ *list = new EntityList;
  **list = figure->getentitylist();
  (*list)->start();
}


void WINAPI DeleteFigureList(EntityList *list)
{Entity *e;

  for (list->start() ; (e = list->next()) != 0 ; )
    e->setposition((OneGeneral *)1);  //  Don't delete sub entities

  for (list->start() ; (e = list->next()) != 0 ; )
    if (e->getposition() == 0)
      delete e;

  list->destroy();
 
}


void WINAPI DrawLine(int mode,Point p1,Point p2)
{Line line(p1,p2);
  //line.EntityHeader::change(line.getcolour(),db.header.getlayer(),line.getstyle(),line.getweight());
  // KMJ : temporary rubberbanding changes
  //if(mode == RB_ERASE)
  //    cadwindow->clearAllRubberBands();
  line.draw(mode);
}

void WINAPI DrawText1(int mode,char *font,Point org,Point xa,Point ya,double w,double h,double s,double a,char *t,char *l,long o)
{
 BitMask options(32);

 // KMJ : temporary rubberbanding changes
 //if(mode == RB_ERASE)
 //   cadwindow->clearAllRubberBands();
 //
 int i;
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  Text text(_RCT(font),org,xa,ya,w,h,s,a,0,_RCT(t),_RCT(l),options);
  //text.EntityHeader::change(db.header.getlayer(),text.getcolour(),text.getstyle(),text.getweight());
  text.draw(mode);
}

void WINAPI DrawFigure(int mode,char *l,char *n,Point org,Point xa,Point ya,Point s,Point r,Point si,int nc,Byte s1,Byte s2,double *tr,long o)
{BitMask options(32);
 int i;
 Transform transform,*t;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);

  if (tr != 0)
    {  transform.mat(0,0) = tr[0];  
       transform.mat(0,1) = tr[1];  
       transform.mat(0,2) = tr[2];  
       transform.mat(0,3) = tr[3];  
       transform.mat(1,0) = tr[4];  
       transform.mat(1,1) = tr[5];  
       transform.mat(1,2) = tr[6];  
       transform.mat(1,3) = tr[7];  
       transform.mat(2,0) = tr[8];  
       transform.mat(2,1) = tr[9];  
       transform.mat(2,2) = tr[10];  
       transform.mat(2,3) = tr[11];  
       t = &transform;
    }
  else
    t = 0;    

  Figure figure(_RCT(l),_RCT(n),org,xa,ya,s,r,si,nc,s1,s2,t,options);
  figure.draw(mode);
}



void WINAPI ReverseEntity(Entity *e)
{ if (e->isa(line_entity))
    ((Line *)e)->reverse();
  else if (e->isa(circle_entity))
    ((Circle *)e)->reverse();
}

void WINAPI NewMenu(HMENU hmenu)
{ cadwindow->newmenu(hmenu);
}

void WINAPI SetLastPoint(Point p)
{ state.setposition(p);
}

void WINAPI DivideEntity(Entity *e,Point p)
{Entity *e1;
  if (e->isa(line_entity))
    e1 = ((Line *)e)->divide(p);
  else if (e->isa(circle_entity))
    e1 = ((Circle *)e)->divide(p);
  else
    e1 = 0;

  if (e1 != 0)
    MoveEntityAfter(e,e1);

}

void WINAPI DivideEntityEx(Entity *e,Point p,int copy,Entity **e1,Entity **e2)
{Transform t;

  e->setcopye(0);
  if (copy)
    {  t.identity();
       *e1 = e->clone(t);
       db.geometry.add(*e1);
       MoveEntityAfter(e,*e1);
    }
  else
    *e1 = e;

  if (e->isa(line_entity))
    *e2 = ((Line *)*e1)->divide(p);
  else if (e->isa(circle_entity))
    *e2 = ((Circle *)*e1)->divide(p);
  else if (e->isa(curve_entity))
    *e2 = ((Curve *)*e1)->divide(p);
  else
    *e2 = 0;

  if (*e2 != 0)
    MoveEntityAfter(*e1,*e2);

}

void WINAPI NearEntity(Entity *e,Point *p)
{ *p = e->nearp(*p);
}

long WINAPI GetEntityStatus(Entity *e)
{ return e->getstatus();
}

void WINAPI SetEntityStatus(Entity *e,long status)
{ e->setstatus(status);
}

void WINAPI SetRealValue(char *name,double value)
{ v.setreal(name,value);

  if (_stricmp(name,"gd::xspacing") == 0)
    db.grid.setxspacing(value);
  else if (_stricmp(name,"gd::yspacing") == 0)
    db.grid.setyspacing(value);
  else if (_stricmp(name,"gd::angle") == 0)
    db.grid.setangle(value);

}

double WINAPI GetRealValue(char *name)
{ return v.getreal(name);
}

void WINAPI SetStringValue(char *name,char *value)
{
    v.setstring(name,value);
}

char *WINAPI GetStringValue(char *name)
{
    static char temp[1000]={""};
    temp[0]=0;
    strcpy(temp,QString(v.getstring(_RCT(name))).toLatin1().data());
    return temp;
}

void WINAPI SetIntegerValue(char *name,int value)
{ v.setinteger(name,value);

  if (_stricmp(name,"gd::xreference") == 0)
    db.grid.setxreference(value);
  //else if (_stricmp(name,"gd::yxreference") == 0) // KMJ bug??
  else if (_stricmp(name,"gd::yreference") == 0)
    db.grid.setyreference(value);
  else if (_stricmp(name,"gd::options") == 0)
    db.grid.setoptions((value&1) != 0,(value&2) != 0,(value&4) != 0,(value&8) != 0,(value&16) != 0,(value&32) != 0);
  else if (_stricmp(name,"db::numericdisplaytolerance") == 0)
    db.setnumericdisplaytolerance(value);
  else if(_stricmp(name,"db::units") == 0)
      db.setupunits();
}

int WINAPI GetIntegerValue(char *name)
{ return v.getinteger(name);
}

void WINAPI SetBitMaskValue(char *name,BitMask *value)
{ v.setbm(name,*value);
}

BitMask *WINAPI GetBitMaskValue(char *name)
{ return v.getbm(name);
}

int WINAPI GetPlanePatternFile(Plane *plane,char *filename)
{ if (plane->getpatternfilename() == 0)
    return 0;
  strcpy(filename,plane->getpatternfilename());
  return 1;
}

int WINAPI GetPatternLineStyle(Entity *e,double *height,double *lengthscale,double *shear,double *gap,double *angle1,double *angle2,int *orientation,int *fitting,int *mirror,char *filename)
{ if (e->getpatternlinestyle() == 0)
    return 0;
  e->getpatternlinestyle()->GetInfo(height,lengthscale,shear,gap,angle1,angle2,orientation,fitting,mirror,_RCT(filename));
  return 1;
}

static int areacallback(SurfaceEntity *,void *polygon,int,void *,void *cbdata)
{double *area = (double *) cbdata;
  if (! ((Polygon3d *)polygon)->getdefined()) return 1;
  *area += fabs(((Polygon3d *)polygon)->getarea());
  delete ((Polygon3d *)polygon);
  return 1;
}


double WINAPI GetArea(Entity *e)
{double area;
 BitMask options(32);
  area = 0.0;
  if (e->issurface())
    ((SurfaceEntity *)e)->facet(options,0.0,100,100,0,areacallback,&area);
  return area;
}

double WINAPI GetLength(Entity *e)
{double length;
  length = 0.0;
  if (e->islinear())
    length = ((LinearEntity *)e)->length();
  return length;
}

int WINAPI GetPosition(Entity *e,double t,Point *p)
{
  if (e->islinear())
    {  *p = ((LinearEntity *)e)->position(t);
       return 1;
    }
  else if (e->isa(point_entity))
    {  *p = ((PointE *)e)->getp();
       return 1;
    }
  else if (e->isa(figure_entity))
    {  *p = ((Figure *)e)->getorigin();
       return 1;
    }
  else if (e->isa(text_entity))
    {  *p = ((Text *)e)->getorigin();
       return 1;
    }
  else
    return 0;
}

int WINAPI GetSurfacePosition(Entity *e,double u,double v,Point *p)
{
  if (e->isuvsurface())
    {  *p = ((UVSurfaceEntity *)e)->position(u,v);
       return 1;
    }
  else
    return 0;
}

int WINAPI GetDirection(Entity *e,double t,Point *d)
{
  if (e->islinear())
    {  *d = ((LinearEntity *)e)->direction(t);
       return 1;
    }
  else if (e->isa(point_entity))
    {  *d = ((PointE *)e)->getp();
       return 1;
    }
  else
    return 0;
}

void WINAPI PreviewFigure(RCHWND hwnd,Point eye,Point ref,Point up,int persp,char *figurename)
{
 View top(_RCT("Top"),persp,0, eye.x, eye.y, eye.z, ref.x, ref.y, ref.z, up.x, up.y, up.z,1E10,-1E10);
 View3dOutput output(hwnd,0,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation,pmin,pmax;
 BitMask options(32);
 double s;


  if(qobject_cast<QGraphicsView *>((QWidget*)hwnd) != 0)
  {
      QGraphicsScene *scene=0;
      scene = ((QGraphicsView*)hwnd)->scene();
      delete scene;
      scene = new QGraphicsScene();
      ((QGraphicsView*)hwnd)->setScene(scene);
      ((QGraphicsView*)hwnd)->setSceneRect(QRect());

      output.setScene(scene);
      options.set(FigureOriginalColour);
      options.set(FigureOriginalStyle);
      options.set(FigureOriginalWeight);

      Figure figure1(_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
      figure1.extents(0,&pmin,&pmax);

      s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
      if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
        s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
      if (s < 0.00001)
        s = 1.0;

      s *= 1.1;

      org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
      Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
      db.dismask.layer.set(db.header.getlayer(),1);


      //  Prepare the surface for OpenGl drawing
      /*
      if (OpenOpenGl(output.gethdc(),3,0,0,(int)output.getwidth(),(int)output.getheight()) == 1)
        {//  This is a new OpenGL Device context
         View *view;

           view = output.getview();
           SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
           SetViewport(output.getumin(),output.getvmin(),output.getumax(),output.getvmax());
           SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
           BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
           SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);

        }

      figure2.drawGL(DM_NORMAL,NULL,&output);
      */

      //double dtol = db.getdtoler();
      //db.setdtoler(50.0);

      figure2.qgi=0;
      figure2.draw(DM_NORMAL,NULL,&output);

      QGraphicsItem *gitem = figure2.qgi;
      if(gitem != 0)
      {
          gitem->setData(0,"figure");
          ((QGraphicsView*)hwnd)->fitInView(gitem,Qt::KeepAspectRatio);
          ((QGraphicsView*)hwnd)->centerOn(gitem);
          ((QGraphicsView*)hwnd)->show();
      }
      //db.setdtoler(dtol);
  }
#if 0
  else
  {
      if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
      {
           oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
           rect.left = 0;  rect.top = 0;
           rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
           FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
           output.moveto(0,0);
           output.lineto(rect.right-1,0);
           output.lineto(rect.right-1,rect.bottom-1);
           output.lineto(0,rect.bottom-1);
           output.lineto(0,0);
           SelectObject(output.gethdc(),oldhpen);
           DeleteObject(hpen);
           IntersectClipRect(output.gethdc(),1,1,int(output.getwidth())-1,int(output.getheight())-1);

           options.set(FigureOriginalColour);
           options.set(FigureOriginalStyle);
           options.set(FigureOriginalWeight);

           Figure figure1(_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
           figure1.extents(0,&pmin,&pmax);

           s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
           if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
             s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
           if (s < 0.00001)
             s = 1.0;

           s *= 1.1;

           org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
           Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
           db.dismask.layer.set(db.header.getlayer(),1);

           //  Prepare the surface for OpenGl drawing
           if (OpenOpenGl(output.gethdc(),3,0,0,(int)output.getwidth(),(int)output.getheight()) == 1)
             {//  This is a new OpenGL Device context
              View *view;

                view = output.getview();
                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(output.getumin(),output.getvmin(),output.getumax(),output.getvmax());
                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
                BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
                SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);

             }

           figure2.draw(DM_NORMAL,NULL,&output);
        }
  }
#endif
}

void WINAPI CommandFile(char *line)
{ commandfile.send(_RCT(line));
}

int WINAPI SetFigureParameters(Figure *figure,int nparameters,char **name,double *value)
{GeneralList list;
 Entity *e;
 int i;
 EntityList alist;

  for (i = 0 ; i < nparameters ; i++)
    list.add(new Parameter(_RCT(name[i]),value[i]));

  figure->draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(figure);
  alist = figure->getalist();
  for (alist.start() ; (e = alist.next()) != 0 ; )
    {  e->draw(DM_ERASE);
       cadwindow->invalidatedisplaylist(e);
    } 
  figure->loadfigure(&list);
  for (alist.start() ; (e = alist.next()) != 0 ; )
    {  cadwindow->invalidatedisplaylist(e);
       e->draw(DM_NORMAL);
    } 
  figure->draw(DM_NORMAL);

  return 1;
}

void WINAPI IntersectEntities(Entity *e1,Entity *e2,Intersect **i)
{ *i = new Intersect(e1,e2);
}


void WINAPI IntersectEntityPlane(Entity *e1,Point o,Point n,Intersect **i)
{InfinitePlane ip(n,-o.dot(n));
  *i = new Intersect(e1,&ip);
}
  
void WINAPI GetIntersectEntity(Intersect *i,int index,Entity **e)
{ *e = i->intersection(index);
}

void WINAPI DeleteIntersectEntities(Intersect *i)
{ delete i;
}

void WINAPI CutPlane(Plane *plane,Point o,Point n,EntityList **list1,EntityList **list2)
{ plane->Cut(o,n,list1,list2);
}

//
//  Offset the selected plane - If offset > 0, area should increase otherwise the area should decrease
//
void WINAPI OffsetPlane(Plane *plane,double offset,double height,EntityList **list)
{ plane->Offset(offset,height,list);
}

void WINAPI ChangeButtonMenu(int menu,ButtonMenu *bm)
{ cadwindow->buttonmenu(menu,bm);
}

int WINAPI CreateView(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno)
{View *view;
 Entity *e;

  *viewno = db.getnextviewno();
  if (*viewno < 0)
    return 0;

  view = new View(_RCT(name),perspective & ~VW_ONEVIEWONLY,*viewno | (perspective & VW_ONEVIEWONLY),eye,ref,up,front,back);
  db.views3d.add(view);

  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    e->getvisible()->set(*viewno,e->getvisible()->test(0) != 0 && e->getvisible()->test(1) != 0);

  if (show)
   cadwindow->create3dsubwindow(view);


  return 1;
}

int WINAPI GetViewInfo(int viewno,char *name,int *perspective,Point *eye,Point *ref,Point *up,double *front,double *back)
{View *view;
  if ((view = db.views3d.match(viewno)) == 0)
    return 0;

  strcpy(name,view->getname());
  *perspective = view->getperspective();
  *eye = view->geteye();
  *ref = view->getref();
  *up = view->getup();
  *front = view->getfront();
  *back = view->getback();

  return 1;
}

int WINAPI DeleteView(const char *name)
{View *view;
  if ((view = db.views3d.match(name)) == 0)
    return 0;
  db.views3d.del(view);
  delete view;
  return 1;
}

void WINAPI AddGroupEntity(Group *group,Entity *e)
{ group->add(e);
}

void WINAPI DeleteGroupEntity(Group *group,Entity *e)
{ group->del(e);
}


void WINAPI SetEntityCallback(int type,ModifyEntityCallback cb)
{ db.SetModifyCallback(type,cb);
}

void WINAPI SetPropertyCallback(PropertyCallback pc)
{   db.SetPropertyCallback(1,pc);
}

char *WINAPI FormatNumber(double x,int showunits,char *buffer)
{
  RCCHAR rcbuffer[300];
  strcpy(rcbuffer,buffer);
  strcpy(buffer,db.formatnumber(rcbuffer,x,showunits));
  //strcpy(buffer,rcbuffer);
  return buffer;
}

void WINAPI GetUnitInfo(char *mainunitname,char *subunitname,double *mainunitscale,double *subunitscale)
{ *mainunitscale = db.getmainunitscale();
  *subunitscale = db.getsubunitscale();
  strcpy(mainunitname,db.getmainunitname());
  strcpy(subunitname,db.getsubunitname());
}

void WINAPI SetIcon(HICON hicon,char *title)
{
    RCCHAR t[300];
    strcpy(t,title);
    cadwindow->seticon((const QIcon&)*((QIcon*)hicon),t);
}

char *WINAPI GetFileName(void)
{
    static char cname[1024];
    strcpy(cname,db.getname());
    return cname;
}

void WINAPI SetFileName(char *filename)
{
    QString name(QDir::fromNativeSeparators(filename));
    db.setname(name.data());
    QString title = name.mid(name.lastIndexOf("/")+1);
    title = title.left(title.lastIndexOf("."));
    db.settitle(title.data());
    cadwindow->updatetitle();

}

// d=1 enables; d=0 disables drawing
// drawing is either 2 or 3
// drawing = 3 drawing to the screen
// drawing = 2 no drawing to the screen
void WINAPI EnableDrawing(int d)
{ drawing = d | 2;
}

int WINAPI ModifyView(const char *name,int perspective,Point eye,Point ref,Point up,double front,double back,int show,int *viewno)
{View *view;
 View3dWindowList *wl;
 View3dWindow *window;
 double l;

  if ((view = db.views3d.match(name)) != 0)
    {  if (! view->changeview(eye,ref,up))
         return 0;
       if (! view->changeview(perspective))
         return 0;
       if (! view->changeclip(front,back))
         return 0;
 
       wl = cadwindow->GetView3dList();

       for (wl->start();  (window = (View3dWindow *) wl->next()) != NULL ; )
         if (window->getview() == view)
           {  if (window->getoffscreenbitmap() != 0)
                window->getoffscreenbitmap()->displaylist.clear();
              l = (eye - ref).length();
              if (show == 0)
                window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
              else 
                window->zoom(-l,-l,l,l);
           }

       return 1;
    }
  else
    return 0;

}

void WINAPI GetExtentsEx(Entity *e,Point *p1,Point *p2,Point eye,Point ref,Point up,int perspective,double front,double back,double plotscale)
{View temp(_RCT(""),perspective,0,eye,ref,up,front,back);
 View3dOutput output(cadwindow->gethwnd(),cadwindow->gethdc(),&temp,1.0);

  if (plotscale == 0)
    plotscale = v.getreal("vw::plotscale");  

  output.setplotscale(plotscale);
  output.updatematrix();

  e->extents(&output,p1,p2);

}

int WINAPI GetViewWindowInfo(int index,int *viewtype,char *name,int *viewno,RCHWND *hwnd)
{View3dWindow *v3d;
 View3dWindowList *vl;

  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d == 0)
    return 0;

  if (v3d != 0)
    {  *viewtype = v3d->getsurfacetype() != View3dSurf;
       if (*viewtype == 0)
         {  strcpy(name,v3d->getview()->getname());
            *viewno = v3d->getview()->getviewno();
            *hwnd = v3d->gethwnd();
         }
       else
         {  strcpy(name,((View2dWindow *)v3d)->getview2d()->getname());
            *viewno = v3d->getview()->getviewno();
            *hwnd = v3d->gethwnd();
         }

       return 1;
    }
  else
    return 0;

}

int WINAPI SelectViewWindow(int index)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  vl = cadwindow->GetView3dList();
  for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);

  if (v3d != 0)
    {  cadwindow->selectwindow(v3d);
       db.header.setvisible(v3d->getvisibility());
       return 1;
    }
  else
    return 0;

}

int WINAPI CreateViewWindow(int viewtype,char *viewname,int *)
{View *view;
 View2d *view2d;

  if (viewtype == 0)
    {  view = db.views3d.match(viewname);
       if (view != 0)
         cadwindow->create3dsubwindow(view);
       else
         return 0;
    }
  else if (viewtype == 1)
    {  view2d = db.views2d.match(_RCT(viewname));
       if (view2d != 0)
         cadwindow->create2dsubwindow(view2d);
       else
         return 0;
    }
  else
    return 0;

  return 1;

}

int WINAPI RenderViewWindow(int index,int style,char *savefilename)
{View3dWindow *v3d;
 View3dWindowList *vl;
 char lsavefilename[300];

  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d == 0)
    return 0;

  if (style == 0)
    v3d->setrepaintstyle(Wireframe);
  else if (style == 1)
    {  v3d->setrepaintstyle(Hidden);
       HideImage image(v3d,1 + v.getinteger("hd::dashhiddenlines") * 2);
       image.hide();
    }
  else if (style == 2)
    {  v3d->setrepaintstyle(Shaded);
       RenderImage image(v3d,1,v.getreal("sh::ambient"),v.getreal("sh::diffuse"));
       image.render();
       if (v.getinteger("sh::hiddenlines"))
         {HideImage image((View3dSurface *)cadwindow->getcurrentsurface(),v.getinteger("hd::dashhiddenlines") * 2);
            image.hide();
         }
    }
  else if (style == 3)
    {  v3d->setrepaintstyle(RayTraced);
       RayTraceImage rti(v3d);
       if (rti.loadpolygons())
         rti.raytrace();
    }

  if (savefilename != 0)
    {  strcpy(lsavefilename,savefilename);
       _strlwr(lsavefilename);
       if (strstr(lsavefilename,".jpg") != 0) 
         v3d->saveimage(4,_RCT(savefilename),2);
       else if (strstr(lsavefilename,".tga") != 0) 
         v3d->saveimage(2,_RCT(savefilename),0);
       else if (strcmp(lsavefilename,"<clipboard>") == 0) 
         v3d->saveimage(3,_RCT(savefilename),0);
       else
         v3d->saveimage(1,_RCT(savefilename),0);
    }

  return 1;

}


int WINAPI SaveViewWindowBitmap(int index,int style,int bitmapsize,int savetype,int quality,char *savefilename)
{View3dSurface *v3d;
 View3dWindowList *vl;
 View3dOffScreenBitmap *v3dosb;
 View2dOffScreenBitmap *v2dosb;
 RECT rect;
 RCHBRUSH hbrush;

  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d == 0)
    return 0;

  v3dosb = 0;
  v2dosb = 0;

  if (v3d->getsurfacetype() == View3dSurf)
    {  v3dosb = new View3dOffScreenBitmap(v3d,bitmapsize+1);
       if (! v3dosb->getdefined())
         {  delete v3dosb;
            return 0;
         }
       v3d = v3dosb;
    }
  else 
    {  v2dosb = new View2dOffScreenBitmap((View2dWindow *)v3d,((View2dWindow *)v3d)->getview2d(),bitmapsize+1);
       if (! v2dosb->getdefined())
         {  delete v2dosb;
            return 0;
         }
       v3d = v3dosb;
    }

  rect.left = 0;  rect.top = 0;  rect.right = int(v3d->getwidth());  rect.bottom = int(v3d->getheight());
  hbrush = CreateSolidBrush(v3d->getbackground());
  FillRect(v3d->gethdc(),&rect,hbrush);
  DeleteObject(hbrush);

  if (style == 0)
    {  v3d->setrepaintstyle(Wireframe);
       db.geometry.draw(DM_NORMAL,v3d);
    }
  else if (style == 1)
    {  v3d->setrepaintstyle(Hidden);
       HideImage image(v3d,1 + v.getinteger("hd::dashhiddenlines") * 2);
       image.hide();
    }
  else if (style == 2)
    {  v3d->setrepaintstyle(Shaded);
       RenderImage image(v3d,1,v.getreal("sh::ambient"),v.getreal("sh::diffuse"));
       image.render();
       if (v.getinteger("sh::hiddenlines"))
         {HideImage image(v3d,v.getinteger("hd::dashhiddenlines") * 2);
            image.hide();
         }
    }
  else if (style == 3)
    {  v3d->setrepaintstyle(RayTraced);
       RayTraceImage rti(v3d);
       if (rti.loadpolygons())
         rti.raytrace();
    }

  if (savefilename != 0)
    v3d->saveimage(1+(_tcsstr(_RCT(savefilename),_RCT(".TGA")) != 0),_RCT(savefilename),0);

  delete v2dosb;
  delete v3dosb;

  return 1;

}

int WINAPI GetColour(int index,RCCOLORREF *c)
{ if (index == -1)    
    *c = RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0);
  else if (index >= 0 && index <= 255)
    *c = RGB(db.colourtable[index].red,db.colourtable[index].green,db.colourtable[index].blue);
  else
    return 0;
  return 1;    
}

int WINAPI SetColour(int index,RCCOLORREF c)
{ 
  if (index == -1)    
    {  v.setreal("wn::backgroundcolour.red",GetRValue(c) / 255.0);
       v.setreal("wn::backgroundcolour.green",GetGValue(c) / 255.0);
       v.setreal("wn::backgroundcolour.blue",GetBValue(c) / 255.0);
       cadwindow->paintall(0,0);
    }
  else if (index >= 0 && index <= 255)
    {  db.colourtable[index].red = GetRValue(c);
       db.colourtable[index].green = GetGValue(c);
       db.colourtable[index].blue = GetBValue(c);
    }
  else
    return 0;

  return 1;    
}

void WINAPI StrokeHiddenLineView(char *viewname,char *hiddenlineviewname,double xpos,double ypos)
{View *view3d,*viewflat,*v;
 Point xaxis,yaxis,zaxis,pmin,pmax;
 Transform t1,t2;
 double du,dv;
 BitMask visible(MaxViews);
 int i;

  view3d = db.views3d.match(viewname);
  viewflat = db.views3d.match(hiddenlineviewname);

  if (view3d == 0 || viewflat == 0)
    return;

  zaxis = (view3d->geteye() - view3d->getref()).normalize();
  yaxis = view3d->getup().normalize();
  xaxis = yaxis.cross(zaxis).normalize();
  yaxis = zaxis.cross(xaxis);
  

  View3dOutput view3doutput(cadwindow->gethwnd(),cadwindow->gethdc(),view3d,1.0);
 
  db.geometry.extents(&view3doutput,&pmin,&pmax);
  if (pmax.x - pmin.x > db.getptoler() && pmax.y - pmin.y > db.getptoler())
    {  du = (pmax.x - pmin.x) * 0.02;
       dv = (pmax.y - pmin.y) * 0.02;
       view3doutput.zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
    }

  t1.translate(-view3d->getref());
  t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
  t1.apply(t2);
  t2.translate(xpos-pmin.x,ypos-pmin.y,0.0);
  t1.apply(t2);


  visible.clearall();
  if (view3d->getvisibility())
    visible.set(view3d->getviewno());
  else
    {  for (i = 0 ; i < MaxViews ; i++)
         if ((v = db.views3d.match(i)) != 0 && ! v->getvisibility())
           visible.set(i);
    }

  HideImage image(&view3doutput,&visible,&t1);
  image.hide();

}

int WINAPI CommonDialog(int index,int *result)
{int cstate;
  
  cstate = index < 4 ? 1000 : 1001;

  QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);


  if (index == 0 || index == 4)
  {
    //colour_command(&cstate,0,(void **)result);
    settings.setValue("DialogSettings/GenProps::CurrentTab","2");
    settings.sync();
    general_property_command(&cstate,0,(void **)result);
  }
  else if (index == 1  || index == 5)
  {
    //layer_command(&cstate,0,(void **)result);
    settings.setValue("DialogSettings/GenProps::CurrentTab","0");
    settings.sync();
    general_property_command(&cstate,0,(void **)result);
  }
  else if (index == 2 || index == 6)
  {
    //style_command(&cstate,0,(void **)result);
    settings.setValue("DialogSettings/GenProps::CurrentTab","3");
    settings.sync();
    general_property_command(&cstate,0,(void **)result);
  }
  else if (index == 3 || index == 7)
  {
      settings.setValue("DialogSettings/GenProps::CurrentTab","1");
      settings.sync();
       double w;
       //weight_command(&cstate,0,(void **)&w);
       general_property_command(&cstate,0,(void **)&w);
       *result = db.lineweights.mmtoweight(w);
  }

  return 0;
}

int WINAPI CreateWorkPlane(char *name,Point origin,Point xaxis,Point yaxis)
{
  if (db.workplanes.match(name) != 0)
    return 0;
  db.workplanes.add(new Workplane(_RCT(name),origin,xaxis,yaxis,0.0));
               
  return 0;
}

int WINAPI DeleteWorkPlane(char *name)
{Workplane *workplane;
 
  workplane = db.workplanes.match(name);
  if (workplane == db.workplanes.getcurrent())
    return 0;
  db.workplanes.del(workplane); 
  delete workplane;

  return 1;
}

int WINAPI GetWorkPlaneInfo(int index,char *name,Point *origin,Point *xaxis,Point *yaxis)
{Workplane *workplane;

  if (index == -1)
    workplane = db.workplanes.getcurrent();
  else
    workplane = db.workplanes.match(index);
  
  if (workplane != 0)
    {  strcpy(name,workplane->getname());
       *origin = workplane->getorigin();
       *xaxis = workplane->getxaxis();
       *yaxis = workplane->getyaxis();
       return 1;
    }
  else
    return 0;

}

int WINAPI ModifyWorkPlane(char *name,Point origin,Point xaxis,Point yaxis)
{Workplane *workplane;
 
  if ((workplane = db.workplanes.match(name)) != 0)
    {  workplane->change(origin,xaxis,yaxis);
       return 1;
    }
  else
    return 0;
}

int WINAPI SelectWorkPlane(char *name)
{Workplane *workplane;
 
  if ((workplane = db.workplanes.match(name)) != 0)
    {  db.workplanes.setcurrent(workplane);
       return 1;
    }
  else
    return 0;

}


int WINAPI SetZoomViewWindow(int index,double umin,double vmin,double umax,double vmax)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  v3d->zoom(umin,vmin,umax,vmax);
       return 1;
    }
  else
    return 0;

}


int WINAPI SetPlotScaleViewWindow(int index,double plotscale)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  v3d->setplotscale(plotscale);
       return 1;
    }
  else
    return 0;

}


int WINAPI GetZoomViewWindowInfo(int index,double *umin,double *vmin,double *umax,double *vmax)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  *umin = v3d->getumin();
       *vmin = v3d->getvmin();
       *umax = v3d->getumax();
       *vmax = v3d->getvmax();
       return 1;
    }
  else
    return 0;

}

int WINAPI GetPlotScaleViewWindowInfo(int index,double *plotscale)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  *plotscale = v3d->getplotscale();
       return 1;
    }
  else
    return 0;

}

int WINAPI SetPlaneAnchorPoint(Plane *plane,Point p,int redraw)
{ if (plane != 0 && plane->isa(plane_entity))
    {  plane->SetAnchorPoint(p,redraw);
       return 1;
    }
  else
    return 0;
}

int WINAPI GetAPIVersion(void)
{ return sizeof(it);
}


int WINAPI GetZoomScaleViewWindowInfo(int index,double *scale,double *u,double *v)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  *scale = (v3d->getumax() - v3d->getumin()) / v3d->getwidthmm();
       *u = (v3d->getumin() + v3d->getumax()) / 2.0;
       *v = (v3d->getvmin() + v3d->getvmax()) / 2.0;
       return 1;
    }
  else
    return 0;

}


int WINAPI SetZoomScaleViewWindow(int index,double scale,double u,double v)
{View3dWindow *v3d;
 View3dWindowList *vl;
 
  if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d != 0)
    {  v3d->zoom(u-v3d->getwidthmm()*scale/2.0,v-v3d->getheightmm()*scale/2.0,u+v3d->getwidthmm()*scale/2.0,v+v3d->getwidthmm()*scale/2.0);
       return 1;
    }
  else
    return 0;

}

Group *WINAPI GetParentGroup(Entity *e)
{EntityList alist;
  alist = e->getalist();
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if (e->isa(group_entity))
      return (Group *)e;
  return 0;
}

int WINAPI DeleteStandardMenuButton(char *menu,int buttonindex)
{int result;
  if (buttonmenus.match(menu) != 0)
    {  result = buttonmenus.match(menu)->del(buttonindex);
       cadwindow->paintevent();
       return result;
    }  
  else
    return 0;
}
void WINAPI SetCommandCallback(int commandid,CommandCallback c)
{   if (commandid == CCTrimCommand)
        db.TrimCommandCallback = c;
    else if (commandid == CCStartCommand)
        db.StartCommandCallback = c;
    else if (commandid == CCEndCommand)
        db.EndCommandCallback = c;
}

int WINAPI GetHeaderInfoEx(Entity *e,int *layer,int *colour,int *style,int *weight,BitMask **visible)
{ *layer = e->getlayer();
  *colour = e->getcolour();
  *style = e->getstyle();
  *weight = e->getweight();
  *visible = e->getvisible();
  return 1;
}

void WINAPI ChangeFigure(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,unsigned char s1,unsigned char s2,long o)
{BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);

  figure->change(*change,*figure,_RCT(l),_RCT(n),s,r,Point(0.0,0.0,0.0),0,s1,s2,options);

}

void WINAPI ChangeFigureEx(Figure *figure,BitMask *change,char *l,char *n,Point s,Point r,Point size,int ncopies,unsigned char s1,unsigned char s2,long o)
{BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1/*L*/ <<i)) != 0)
      options.set(i);

  figure->change(*change,*figure,_RCT(l),_RCT(n),s,r,size,ncopies,s1,s2,options);

}

void WINAPI ChangeText(Text *text,BitMask *change,char *f,double w,double h,
                       double s,double a,Byte hc,char *t,char *l,long o)
{BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);

  text->change(*change,*text,CHAR2RCCHR(f),w,h,s,a,hc,CHAR2RCCHR(t),CHAR2RCCHR(l),options);

}

void WINAPI ChangeDimension(Entity *e,BitMask *change,double ex,double g,double s,double ut,double lt,
                       double sc,double tyo,int p,int tp,
                       TerminatorInfo *t1,TerminatorInfo *t2,
                       TextInfo *ti1,TextInfo *ti2,TextInfo *ti3,TextInfo *ti4,
                       long o)
{BitMask options(32);
 int i;
  if (e->isdimension())
    {  for (i = 0 ; i < 32 ; i++)
         if ((o & (1L <<i)) != 0)
           options.set(i);
       ((Dimension *)e)->change(*change,*e,ex,g,s,ut,lt,sc,tyo,p,tp,*t1,*t2,*ti1,*ti2,*ti3,*ti4,o);
    }

}


Group *WINAPI ExplodeEntity(Entity *e,char *classname,char *groupname,int options)
{Entity *firstentity,*e1;
 EntityList list;
 Group *group;

  state.destroyselection(0);

  db.geometry.start();
  firstentity = db.geometry.next();
  group = 0;
  if (e->explode())
    {  if (classname != 0)
         {  for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
              if (e1 == firstentity)
                break;
              else
                list.add(e1);

            if (list.length() > 0)
              {  group = new Group(list,_RCT(classname),groupname == 0 && e->isa(figure_entity) ? ((Figure *)e)->getlibrary() : _RCT(groupname),options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   {  delete group;
                      group = 0;
                   }
              }

            if (e->getalist().empty())
              db.geometry.del(e,0);
         }
       else if (e->getalist().empty())
         db.geometry.del(e,0);


    }
  
  return group;

}

int WINAPI SetDefaultHeaderInfoEx(int layer,int colour,int style,int weight,BitMask *visible,int update)
{ db.header.change(colour,layer,style,weight);
  v.setinteger("df::layer",layer);
  v.setinteger("df::colour",colour);
  v.setinteger("df::style",style);
  v.setinteger("df::weight",weight);
  db.header.setvisible(*visible);
  db.dismask.layer.set(layer,1);
  if (update)
    cadwindow->updatetitle();
  return 1;
}

void WINAPI DeleteBitMask(BitMask *bitmask)
{ delete bitmask;
}

void WINAPI SetMessageCallback(MessageCallback mcb)
{ db.SetMessageCallback(mcb);
}

void WINAPI EnableUndo(int enable)
{ db.EnableUndo(enable);
}


int WINAPI SetViewLayerInfo(char *viewname,int layer,int colour,int style,int weight,BitMask *displaylayertable,BitMask *selectionlayertable,BitMask *lockedlayertable)
{View *view3d;

  view3d = db.views3d.match(viewname);
  if (view3d != 0)
    {  view3d->setdisplaylayertable(displaylayertable);
       view3d->setselectionlayertable(selectionlayertable);
       view3d->setlockedlayertable(lockedlayertable);
       view3d->setcolour(colour);
       view3d->setlayer(layer);
       view3d->setstyle(style);
       view3d->setweight(weight);
       return 1;
    }  
  else
    return 0;

}

int WINAPI GetViewLayerInfo(char *viewname,int *layer,int *colour,int *style,int *weight,BitMask **displaylayertable,BitMask **selectionlayertable,BitMask **lockedlayertable)
{View *view3d;

  view3d = db.views3d.match(viewname);
  if (view3d != 0)
    {  *displaylayertable = view3d->getdisplaylayertable();
       *selectionlayertable = view3d->getselectionlayertable();
       *lockedlayertable = view3d->getlockedlayertable();
       *colour = view3d->getcolour();
       *layer = view3d->getlayer();
       *style = view3d->getstyle();
       *weight = view3d->getweight();
       return 1;
    }  
  else
    return 0;

}

int WINAPI SetViewWorkplane(char *viewname,char *workplanename)
{View *view3d;

  view3d = db.views3d.match(viewname);
  if (view3d != 0)
    {  view3d->setworkplanename(_RCT(workplanename));
       return 1;
    }  
  else
    return 0;

}

int WINAPI GetViewWorkplane(char *viewname,char *workplanename)
{View *view3d;

  view3d = db.views3d.match(viewname);
  if (view3d != 0)
    {  strcpy(workplanename,view3d->getworkplanename());
       return 1;
    }  
  else
    return 0;

}

void WINAPI SetLayerLockedMask(int i,int value)
{ db.lockedmask.layer.set(i,value);
}

int WINAPI GetLayerLockedMask(int i)
{ return db.lockedmask.layer.test(i);
}

int WINAPI AddRelationship(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship)
{ return entity->addrelationship(relatedentity,ApplicationID,Relationship);
}

int WINAPI DeleteRelationship(Entity *entity,Entity *relatedentity,long ApplicationID,long Relationship)
{ return entity->deleterelationship(relatedentity,ApplicationID,Relationship);

}

int WINAPI GetRelationship(Entity *entity,int index,Entity **relatedentity,long *ApplicationID,long *Relationship)
{ return entity->getrelationship(index,relatedentity,ApplicationID,Relationship);
}

Entity *WINAPI GetPickedEntity(void)
{ return state.getpickedentity();
}

void WINAPI GetPickedPoint(Point *p)
{ *p = state.getlastpickpoint();
}


char *WINAPI GetCADCommandLine(void)
{ return (char*)program->getcommandline();
}

void WINAPI ClearCADCommandLine(void)
{ program->clearcommandline();
}

void WINAPI SetDatabaseModified(int m)
{ if (m)
   db.setchanged();
  else
    db.clearchanged();
}

int WINAPI GetDatabaseModified(void)
{ return db.getchanged();
}

View2dPrinter *WINAPI PrintStart(char *composeddrawingname)
{View2dPrinter *view2dprinter;
	View2d *v;

  if ((v = db.views2d.match(_RCT(composeddrawingname)))  != 0)
    {  View *top = new View(v->getname(),0,v->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
	   view2dprinter = new View2dPrinter(v,top);
 	   view2dprinter->print(0,0);
       if (view2dprinter->getdefined()) 
         return view2dprinter;
       else
         return 0;
    }
  else
    return 0;

} 

View2dPrinter *WINAPI PrintStartEx(char *composeddrawingname,PRINTDLG *pd,DOCINFO *di)
{View2dPrinter *view2dprinter;
	View2d *v;

  if ((v = db.views2d.match(_RCT(composeddrawingname)))  != 0)
    {  View *top = new View(v->getname(),0,v->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
	   view2dprinter = new View2dPrinter(v,top);
 	   view2dprinter->print(pd,di);
       if (view2dprinter->getdefined()) 
         return view2dprinter;
       else
         return 0;
    }
  else
    return 0;

} 

int WINAPI PrintNext(View2dPrinter *view2dprinter,char *composeddrawingname)
{View2d *v;

  if ((v = db.views2d.match(_RCT(composeddrawingname)))  != 0)
    {  view2dprinter->newpage();
       view2dprinter->printnext(v);
       return 1;
    }
  else 
    return 0;

}

void WINAPI PrintFinish(View2dPrinter *view2dprinter)
{ delete view2dprinter;
}

int WINAPI GetPlanePolygon(Plane *plane,Polygon3d **polygon3d,double error)
{ if (plane->isa(plane_entity))
    {  *polygon3d = new Polygon3d(plane->getorigin(),plane->getxaxis(),plane->getyaxis(),plane->getlist(),0,error);
       return 1;
    }
  else
    return 0;
}

void WINAPI DeletePlanePolygon(Polygon3d *polygon3d)
{ delete polygon3d;
}

void WINAPI GetPolygonInfo(Polygon3d *polygon,Point *origin,Point *xaxis,Point *yaxis,int *nloops,int **nvert,Point ***vert)
{ *origin = polygon->getorigin();
  *xaxis = polygon->getxaxis();
  *yaxis = polygon->getyaxis();
  *nloops = polygon->getnloops();
  *nvert = polygon->getnvert();
  *vert = polygon->getverts();   
}

class ColourDisplayDialogControl : public DisplayDialogControl
  {private:
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
   public:
     ColourDisplayDialogControl(int id,Dialog *d,Colour c) : DisplayDialogControl(id)
        {  lastindex = -1;  parentdialog = d;  lastcolour = c;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/*
 // now defined in ncdialog.h
class ColourScrollBarDialogControl : public ScrollBarDialogControl
  {public:
     ColourScrollBarDialogControl(int i,DialogControl *dc1) :
        ScrollBarDialogControl(i,dc1) {  }
     void vscrollevent(Dialog *,int,int,int);
     void setscrollpos(Dialog *,int);
  };
*/

int WINAPI SelectRGBColour(double *red,double *green,double *blue)
{Dialog dialog("SelectRGBcolour_Dialog");
 Colour c;
 int result,ired,igreen,iblue;
 double hue,lightness,saturation;
 DialogControl *dc;

  ired = (int)(*red * 255.0 + 0.5);
  igreen = (int)(*green * 255.0 + 0.5);
  iblue = (int)(*blue * 255.0 + 0.5);
 
  c.set(ired,igreen,iblue);
  c.gethls(&hue,&lightness,&saturation); 

  dialog.add(dc = new IntegerDialogControl(101,&ired,0,255));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(new ColourScrollBarDialogControl(104,dc));
  dialog.add(dc = new IntegerDialogControl(102,&igreen,0,255));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(new ColourScrollBarDialogControl(105,dc));
  dialog.add(dc = new IntegerDialogControl(103,&iblue,0,255));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new ColourScrollBarDialogControl(106,dc));


  dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(new ColourScrollBarDialogControl(110,dc));
  dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0,1.0));
  dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(new ColourScrollBarDialogControl(111,dc));
  dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(new ColourScrollBarDialogControl(112,dc));

  dialog.add((DisplayDialogControl *) new ColourDisplayDialogControl(113,0,c));

//  dialog.add(new SetButtonDialogControl(114));
  int n = db.getnumericdisplaytolerance();
  db.setnumericdisplaytolerance(3);   
  result = dialog.process();
  db.setnumericdisplaytolerance(n);   
  
  if (result == 1)
    { *red = ired / 255.0;
      *green = igreen / 255.0;
      *blue = iblue / 255.0;
    }

  return result;

}

void WINAPI SwitchButtonMenu(char *name,ButtonMenu *bm)
{char fullname[300];
 ButtonMenu *oldbm;

  strcpy(fullname,"ApplicationMenu::");
  strcat(fullname,name);

  if (buttonmenus.match(fullname) != 0)
    {  oldbm = buttonmenus.match(fullname);
       buttonmenus.deletemenu(buttonmenus.match(fullname));
       buttonmenus.add(_RCT(fullname),bm);

       bm->parent = oldbm->parent;
       bm->xpos = oldbm->xpos;
       bm->ypos = oldbm->ypos;
       bm->w = oldbm->w;
       bm->h = oldbm->h;

       if (bm->parent != 0)
         {  bm->parent->delcontrol(oldbm);
            bm->parent->addcontrol(bm);
         }

       bm->fixup(bm->parent);  
       bm->draw(DM_NORMAL);
    }

}

void WINAPI AddPlaneEdge(Plane *plane,Point p1,Line *edge)
{ plane->AddEdge(p1,edge);
}

void WINAPI DeletePlaneEdge(Plane *plane,Line *edge)
{ plane->DeleteEdge(edge);
}

void WINAPI RecalculatePlane(Plane *plane)
{ plane->ReCalculatePlane();
  cadwindow->invalidatedisplaylist(plane);
}

void WINAPI MoveLineEndPoint(Line *line,int draw,int end,Point p1)
{ line->movehandlepoint(0,draw,end,p1);
}

void WINAPI GetParentList(Entity *e,EntityList *list)
{ *list = e->getalist();
}

void WINAPI ChangePlane(Plane *plane,BitMask *change,double xs,double ys,double a,double ca,RCCOLORREF gc,char *filename,long o)
{BitMask options(32);
 int i;

  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);

  plane->change(*change,*plane,xs,ys,a,0.0,ca,0.0,0.0,0.0,0.0,gc,_RCT(filename),options);

}

int WINAPI SelectComposedDrawingView(char *prompt,int *viewno,int *index)
{Point p;
 View3dSubSurface *ss;
 View2d *view2d;
 OneView *oneview;
 int gridstate;        
 
  cadwindow->setdragviewboundary(0);
  gridstate = db.grid.getsnap();
  db.grid.setsnap(0);
    
  while (SelectPosition(prompt,&p) && lastapiwindow != 0 && lastapiwindow->getsurfacetype() == View2dSurf)
    if ((ss = ((View2dWindow *)lastapiwindow)->pick(p)) != 0)
      {  view2d = ((View2dWindow *)lastapiwindow)->getview2d();
         *viewno = view2d->getviewno();
         for (view2d->start(),*index = 0 ; (oneview = view2d->next()) != 0 ; (*index)++)
           if (oneview == ss->getoneview())
             break;
         cadwindow->setdragviewboundary(1);
         db.grid.setsnap(gridstate);
         return oneview == ss->getoneview();
      }

  db.grid.setsnap(gridstate);
  cadwindow->setdragviewboundary(1);
  return 0;

}

int WINAPI ModifyPlaneList(Plane *plane,int add,EntityList *list)
{Entity *e;
 EntityList l;
 Point origin,xaxis,yaxis,zaxis;
 double D;

  Loops loops(1);
  loops.create(list);
  if (! list->empty() || loops.getloops().empty() ||
      ! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D) ||
      fabs(zaxis.dot(plane->getxaxis().cross(plane->getyaxis()))) < 0.999)
    {  l = loops.getloops();
       l.start();
       while ((e = l.next()) != 0)
         list->add(e);
       return 0;
    }

  l = loops.getloops();
  l.start();
  plane->getlist().addatend(0);
  while ((e = l.next()) != 0)
    {  if (add)
         {  plane->getlist().addatend(e);
            e->adda(plane);
         }
       else 
         {  plane->getlist().del(e);
            e->dela(plane);
         }
    }
  l.destroy();

  return 1;

}

int WINAPI GetCurrentCommandId(void)
{ return state.getcommandid();
}

char *WINAPI GetExecutableFilename(char *title)
{
    static char tmp[1024];
    memset(tmp,0,1024);
    strcpy(tmp,home.getexecutablefilename(_RCT(title)));
    return tmp;
}

char *WINAPI GetPublicFilename(char *title)
{
    static char tmp[600];
    memset(tmp,0,600);
    strcpy(tmp,home.getpublicfilename(_RCT(title)));
    return tmp;
}

void WINAPI GetLastEvent(RCHWND *hWnd, UINT *message,WPARAM *wParam, LPARAM *lParam)
{   db.get_last_message(hWnd,message,wParam,lParam);
}

// KMJ new api functions
int WINAPI GetCurrentSelectionLength()
{
    return state.getselection().length();
}

int WINAPI IsVisible(Entity *e)
{
    return e->isvisible((View3dSurface*)0);
}

int WINAPI IsVisibleInView(Entity *e, int viewno)
{
  return e->getvisible()->test(viewno);
}

int WINAPI IsVisibleInViewWindow(Entity *e, int index)
{
 View3dWindowList *vl=0;
 View3dWindow *v3d=0;
 View3dSurface *surface=0;
 View2d *view2d;
 View *view3d;

   if (index == -1)
    v3d = cadwindow->getcurrentwindow();
  else
    {  vl = cadwindow->GetView3dList();
       for (vl->start() ; (v3d = (View3dWindow *) vl->next()) != NULL && index > 0 ; index--);
    }

  if (v3d == 0)
    return 0;

  if (v3d != 0)
  {
      return e->isvisible((View3dSurface*)v3d);
       /*
       *viewtype = v3d->getsurfacetype() != View3dSurf;
       if (*viewtype == 0)
       {
           strcpy(name,v3d->getview()->getname());
            *viewno = v3d->getview()->getviewno();
            *hwnd = v3d->gethwnd();
       }
       else
       {
           strcpy(name,((View2dWindow *)v3d)->getview2d()->getname());
            *viewno = v3d->getview()->getviewno();
            *hwnd = v3d->gethwnd();
       }
       */
  }
  else
    return 0;

#if 0
  view2d = db.views2d.match(viewno);
  view3d = db.views3d.match(viewno);

  if(view3d != 0)
  {
    return (view3d == 0 || e->getvisible()->test(viewno)) &&
           (view3d != 0 && view3d->getdisplaylayertable() != 0 ? view3d->getdisplaylayertable()->test(e->getlayer()) : db.dismask.layer.test(e->getlayer())) &&
            e->match(db.dismask.entity) &&
            db.dismask.colour.test(e->getcolour()) &&
            db.dismask.style.test(e->getstyle()) &&
            db.dismask.weight.test(e->getweight());
  }
  else if(view2d != 0)
  {
      return e->isvisible((View3dSurface*)0);
      /*
      return (view2d == 0 || e->visible.test(viewno) &&
             (view2d != 0 && view2d->getdisplaylayertable() != 0 ? surface->testdisplaylayer(layer) : db.dismask.layer.test(layer)) &&
             match(db.dismask.entity) &&
             db.dismask.colour.test(colour) &&
             db.dismask.style.test(style) &&
             db.dismask.weight.test(weight);
       */
  }
#endif
  //return e->getvisible()->test(viewno);
}

int WINAPI IsVisibleInComposedDrawingView(Entity *e, int viewno, int index, int *visible)
{
 View *view=0;
 View3dWindowList *wl=0;
 View3dWindow *window=0;
 OneView *oneview=0;
 View2d *view2d=0;
 BitMask *layertable=0;

  *visible =0;

  view2d = db.views2d.match(viewno);
  if (view2d != 0)
  {
      if(!e->getvisible()->test(viewno))
      {
          for (view2d->start() ; (oneview = view2d->next()) != 0 ; index--)
             if (index == 0 || oneview == 0)
               break;

           if (oneview != 0)
           {
               layertable = oneview->getlayertable();

               if((layertable != 0 ? layertable->test(e->getlayer()) : db.dismask.layer.test(e->getlayer())) &&
                    e->match(db.dismask.entity) &&
                    db.dismask.colour.test(e->getcolour()) &&
                    db.dismask.style.test(e->getstyle()) &&
                    db.dismask.weight.test(e->getweight()))
                    *visible = 1;
                else
                    *visible =0;
           }
      }
  }

  return view2d != 0 && oneview != 0;
}

void WINAPI SetVisible(Entity *e, bool OnOff)
{
    BitMask visible(96);
    //visible.set(96,OnOff);
    if(OnOff)
        visible.setall();
    else
        visible.clearall();

    e->setvisible(visible);
}

int WINAPI GetDefaultTextInfo(char **f,double *w,double *h,double *s,double *a,Byte *hc,char **t,long *o)
{
    int i;
    BitMask options(32);

    Text::getdefaults(f,w,h,s,a,hc,t,&options);
    for (i = 0 ; i < 32 ; i++)
    if (options.test(i))
      *o |= (1 << i);
    return 1;
}

int WINAPI GetDefaultPlaneInfo(double *xspacing,double *yspacing,double *angle,double *crossangle,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *o)
{
  int i;
  BitMask options(32);

  *xspacing = v.getreal("pl::xspacing");
  *yspacing = v.getreal("pl::yspacing");
  *angle = v.getreal("pl::angle");
  *crossangle = v.getreal("pl::crossangle");
  strcpy(patternfilename,v.getstring("pl::patternfilename"));
  options = *v.getbm("pl::options");
  for (i = 0 ; i < 32 ; i++)
     if (options.test(i))
         *o |= (1 << i);
  *gc = v.getlong("pl::gradientcolour");
  *transparency = v.getreal("pl::transparency");
  *tr= v.getreal("pl::red");
  *tg = v.getreal("pl::green");
  *tb = v.getreal("pl::blue");
  *trange = v.getreal("pl::transparencyrange");
  return 1;
}

int WINAPI GetPlaneInfoExEx(Plane *plane,EntityList **list,double *xs,double *ys,double *a,double *ca,double *transparency,double *tr,double *tg,double *tb,double *trange,RCCOLORREF *gc,char *patternfilename,long *o)
{
  int i;
  BitMask options(32);

  *list = new EntityList;
  **list = plane->getlist();
  *xs = plane->getxspacing();
  *ys = plane->getyspacing();
  *a = plane->getangle();
  *ca = plane->getcrossangle();
  options = plane->getoptions();
  for (i = 0 ; i < 32 ; i++)
     if (options.test(i))
         *o |= (1 << i);
  *transparency = plane->gettransparency();
  *tr = plane->getred();
  *tg = plane->getgreen();
  *tb = plane->getblue();
  *trange = plane->gettransparencyrange();
  *gc = plane->getgradientcolour();
  if(plane->getpatternfilename() != 0)
      strcpy(patternfilename,plane->getpatternfilename());
  return 1;
}

// KMJ new api functions added 2014/11/15
int WINAPI GetLineStyle(int index, APILineStyleInfo **linestyle)
{
  if (index >= 0 && index <= 255)
  {
      LineStyle *ls=0;
      ls = db.linestyles.indextostyle(index);
      if(ls != 0 )
      {
          //ls = db.linestyles.indextostyle(index)->copy();
          *linestyle = new APILineStyleInfo;
          (*linestyle)->nsegs = ls->getnsegments();
          (*linestyle)->segs = new APILineStyleSegInfo[(*linestyle)->nsegs];
          for(int i=0; i < (*linestyle)->nsegs; i++)
              ls->getsegment(i+1,&(*linestyle)->segs[i].dash,&(*linestyle)->segs[i].space);

          return 1;
      }
      else
      {
          *linestyle = new APILineStyleInfo;
          (*linestyle)->nsegs = 0;
          (*linestyle)->segs  = 0;
          return 1;
      }
  }
 return 0;
}

int WINAPI SetLineStyle(int index, const APILineStyleInfo *linestyle)
{
  if (index >= 0 && index <= 255)
  {
      if (linestyle != 0 && linestyle->nsegs >  0 && linestyle->segs  != 0)
      {
          LineStyle *ls = new LineStyle();
          if(ls != 0)
          {
              ls->setnsegments(linestyle->nsegs);
              for(int i=0; i<linestyle->nsegs; i++)
                  ls->setsegment(i+1,linestyle->segs[i].dash,linestyle->segs[i].space);

              db.linestyles.del(index);
              db.linestyles.set(index,ls);

              return 1;
          }
      }
      else
      {
          LineStyle *ls = new LineStyle();
          if(ls != 0)
          {
              ls->setnsegments(0);
              db.linestyles.del(index);
              db.linestyles.set(index,ls);
          }
      }
  }

  return 0;
}

int WINAPI DeleteLineStyleInfo(APILineStyleInfo *linestyle)
{
    delete [] linestyle->segs;
    delete linestyle;
    return 1;
}

int WINAPI SaveDefaults()
{
    v.save();
    return 1;
}

int WINAPI RestoreDefaults()
{
    v.clear();
    v.initialize();
    return 1;
}

int WINAPI SetDefaultPatternLineStyleInfo(double height, double lengthscale, int size, char *filename)
{
    v.setreal("ps::height",height);
    v.setreal("ps::lengthscale",lengthscale);
    v.setinteger("ps::size",size);
    v.setstring("ps::filename",filename);

    return 1;
}

int WINAPI GetDefaultPatternLineStyleInfo(double *height, double *lengthscale, int *size, char *filename)
{
    *height = v.getreal("ps::height");
    *lengthscale = v.getreal("ps::lengthscale");
    *size = v.getinteger("ps::size");
    strcpy(filename,v.getstring("ps::filename"));

    return 1;
}

int WINAPI SetPatternLineStyleInfo(int index, char *name, double height, double lengthscale, int size, char *filename)
{
    char key[300],section[300],text[300];

    sprintf(key,"PatternLineStyle%d",index);
    WritePrivateProfileString("PatternLineStyles",key,name,home.getinifilename());

    sprintf(section,"PatternLineStyle-%s",name);
    sprintf(text,"%f",height);
    WritePrivateProfileString(section,"pls::height",text,home.getinifilename());
    sprintf(text,"%f",lengthscale);
    WritePrivateProfileString(section,"pls::lengthscale",text,home.getinifilename());
    sprintf(text,"%d",size);
    WritePrivateProfileString(section,"pls::size",text,home.getinifilename());
    WritePrivateProfileString(section,"pls::filename",filename,home.getinifilename());

    return 1;
}

int WINAPI AttachFigure(Figure *fig, bool OnOff)
{
    //fprintf(stdout,"attaching figure : %s\n",fig->getfilename());
    if(OnOff == true)
        fig->attachfigure();
    else
        fig->detachfigure();

    return 1;
}

int WINAPI GetAngularDimensionInfoEx(AngularDimension *ad,Point *org,Point *xa,Point *ya,Point *aorg,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options)
{int i;
 PointList pl;
  *org = ad->getorigin();
  *xa = ad->getxaxis();
  *ya = ad->getyaxis();
  *aorg = ad->getdorigin();
  pl = ad->getlist();
  for (pl.start(),i = 0 ; ! pl.atend() ; )
    DimensionPoints[i++] = pl.next();
  *n = pl.length();
  *l = DimensionPoints;
  *g = ad->getgap();
  *e = ad->getextension();
  *ut = ad->getuppertol();
  *lt = ad->getlowertol();
  *s = ad->getstackdis();
  *p = ad->getprecision();
  *tp = ad->gettolprecision();
  *vs = ad->getscale(); // value scale
  *yoff = ad->gettextyoffset(); // text offset
  CopyTextInfo(ti1,ad->getvaluetextinfo());
  CopyTextInfo(ti2,ad->getprefixtextinfo());
  CopyTextInfo(ti3,ad->getsuffixtextinfo());
  CopyTextInfo(ti4,ad->gettolerancetextinfo()); // tolerance text info
  *t1 = *ad->getterminfo1();
  *t2 = *ad->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (ad->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetCircularDimensionInfoEx(CircularDimension *cd,Point *org,Point *xa,Point *ya,Point *pnt1,Point *pnt2,long *sty,Circle **c,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options)
{int i;
  *org = cd->getorigin();
  *xa = cd->getxaxis();
  *ya = cd->getyaxis();
  *pnt1 = cd->getp1();
  *pnt2 = cd->getp2();
  *c = cd->getcircle();
  *sty = 0;
  for (i = 0 ; i < 32 ; i++)
    if (cd->getstyle().test(i))
      *sty |= (1 << i);
  *g = cd->getgap();
  *e = cd->getextension();
  *ut = cd->getuppertol();
  *lt = cd->getlowertol();
  *s = cd->getstackdis();
  *p = cd->getprecision();
  *tp = cd->gettolprecision();
  *vs = cd->getscale();
  *yoff = cd->gettextyoffset();
  CopyTextInfo(ti1,cd->getvaluetextinfo());
  CopyTextInfo(ti2,cd->getprefixtextinfo());
  CopyTextInfo(ti3,cd->getsuffixtextinfo());
  CopyTextInfo(ti4,cd->gettolerancetextinfo());
  *t1 = *cd->getterminfo1();
  *t2 = *cd->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (cd->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetDistanceBearingDimensionInfoEx(DistanceBearingDimension *dbd,Point *org,Point *xa,Point *ya,Point *start,Point *end,long *disp,double *e,double *g,double *s,double *ut,double *lt,double *vs,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options)
{int i;
  *org = dbd->getorigin();
  *xa = dbd->getxaxis();
  *ya = dbd->getyaxis();
  *start = dbd->getp1();
  *end = dbd->getp2();
  *disp = 0;
  for (i = 0 ; i < 32 ; i++)
    if (dbd->getdisplay().test(i))
      *options |= (1 << i);
  *e = dbd->getextension();
  *g = dbd->getgap();
  *ut = dbd->getuppertol();
  *lt = dbd->getlowertol();
  *s = dbd->getstackdis();
  *p = dbd->getprecision();
  *tp = dbd->gettolprecision();
  *vs = dbd->getscale();
  CopyTextInfo(ti1,dbd->getvaluetextinfo());
  CopyTextInfo(ti2,dbd->getprefixtextinfo());
  CopyTextInfo(ti3,dbd->getsuffixtextinfo());
  CopyTextInfo(ti4,dbd->gettolerancetextinfo());
  *t1 = *dbd->getterminfo1();
  *t2 = *dbd->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (dbd->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetLinearDimensionInfoEx(LinearDimension *ld,Point *org,Point *xa,Point *ya,int *n,Point **l,double *e,double *g,double *s,double *ut,double *lt,double *vs,double *yoff,int *p,int *tp,TerminatorInfo *t1,TerminatorInfo *t2,APITextInfo *ti1,APITextInfo *ti2,APITextInfo *ti3,APITextInfo *ti4,long *options)
{int i;
 SortedPointList spl;
  *org = ld->getorigin();
  *xa = ld->getxaxis();
  *ya = ld->getyaxis();
  CoordinateSystem cs(*org,*xa,*ya);
  spl = ld->getlist();
  *n = spl.length();
  for (spl.start(),i = 0 ; ! spl.atend() ; )
    DimensionPoints[i++] = cs.tomodel(spl.next());
  *l = DimensionPoints;
  *g = ld->getgap();
  *e = ld->getextension();
  *ut = ld->getuppertol();
  *lt = ld->getlowertol();
  *s = ld->getstackdis();
  *p = ld->getprecision();
  *tp = ld->gettolprecision();
  *vs = ld->getscale(); // value scale??
  *yoff = ld->gettextyoffset(); // yoffset??
  CopyTextInfo(ti1,ld->getvaluetextinfo());
  CopyTextInfo(ti2,ld->getprefixtextinfo());
  CopyTextInfo(ti3,ld->getsuffixtextinfo());
  CopyTextInfo(ti4,ld->gettolerancetextinfo()); // tolerance text info
  *t1 = *ld->getterminfo1();
  *t2 = *ld->getterminfo2();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (ld->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI GetTextInfoEx(Text *text,char **f,Point *org,Point *xaxis,Point *yaxis,double *w,double *h,double *s,double *a,Byte *hc,char **t,char **l,long *options)
{int i;
  //*f = QString(text->getfont()).toLatin1().data();
  *f = text->getcharfont();
  *org = text->getorigin();
  *xaxis = text->getxaxis();
  *yaxis = text->getyaxis();
  *w = text->getwidth();
  *h = text->getheight();
  *s = text->getvertspacing();
  *a = text->getangle();
  //*t = QString(text->gettext()).toLatin1().data();
  *t = text->getchartext();
  //*l = QString(text->getlabel()).toLatin1().data();
  *l = text->getcharlabel();
  *hc = text->gethalocolour();
  *options = 0;
  for (i = 0 ; i < 32 ; i++)
    if (text->getoptions().test(i))
      *options |= (1 << i);
  return 1;
}

int WINAPI InsertAngularDimensionEx(Point org,Point xa,Point ya,Point aorg,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,AngularDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32);
 int i;
 PointList pl;
  for (i = 0 ; i < n ; i++)
    pl.add(l[i]);
  pl.reverse();
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti4);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new AngularDimension(org,xa,ya,aorg,pl,e,g,s,ut,lt,sc,tyo,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}


int WINAPI InsertCircularDimensionEx(Point org,Point xa,Point ya,Point pnt1,Point pnt2,long sty,Circle *c,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,CircularDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32),style(32);
 int i;
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti4);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((sty & (1L <<i)) != 0)
      style.set(i);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new CircularDimension(org,xa,ya,pnt1,pnt2,style,c,e,g,s,ut,lt,sc,tyo,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertDistanceBearingDimensionEx(Point org,Point xa,Point ya,Point start,Point end,long disp,double e,double g,double s,double ut,double lt,double sc,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,DistanceBearingDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 BitMask options(32),display(32);
 int i;
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti4);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((disp & (1L <<i)) != 0)
      display.set(i);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new DistanceBearingDimension(org,xa,ya,start,end,display,e,g,s,ut,lt,sc,0.0,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

int WINAPI InsertLinearDimensionExEx(Point org,Point xa,Point ya,int n,Point *l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo *t1,const TerminatorInfo *t2,const APITextInfo *apiti1,const APITextInfo *apiti2,const APITextInfo *apiti3,const APITextInfo *apiti4,const long o,LinearDimension **ent)
{TextInfo ti1,ti2,ti3,ti4;
 int i;
 BitMask options(32);
 SortedPointList pl;
 CoordinateSystem cs(org,xa,ya);
  for (i = 0 ; i < n ; i++)
    pl.add(cs.frommodel(l[i]));
  CopyTextInfo(&ti1,apiti1);
  CopyTextInfo(&ti2,apiti4);
  CopyTextInfo(&ti3,apiti2);
  CopyTextInfo(&ti4,apiti3);
  for (i = 0 ; i < 32 ; i++)
    if ((o & (1L <<i)) != 0)
      options.set(i);
  *ent = new LinearDimension(org,xa,ya,pl,e,g,s,ut,lt,sc,tyo,p,tp,*t1,*t2,ti1,ti2,ti3,ti4,options);

  delete [] ti1.font; delete [] ti1.text;
  delete [] ti2.font; delete [] ti2.text;
  delete [] ti3.font; delete [] ti3.text;
  delete [] ti4.font; delete [] ti4.text;

  return db.geometry.add(*ent,drawing);
}

// KMJ: TODO pending
int WINAPI SendCommandEvent(int commandId)
{
    NewCommandEvent *event = new NewCommandEvent(commandId,0);
    state.sendevent(event);
    state.process();
    return 1;
}

int WINAPI SendCommand(char *description)
{
    int commandid=0;
    if((commandid = commands.command(QString(description).data())) >= 0)
    {
        NewCommandEvent *event = new NewCommandEvent(commandid,0);
        state.sendevent(event);
        state.process();
        return 1;
    }

    /*
    APICommandInfo *cmdinfo=0;
    APICommandInfoList.start();
    while((cmdinfo = (APICommandInfo*)APICommandInfoList.next()) != NULL)
    {
        if(strcmp(modulename,cmdinfo->modulename) >= 0 && strcmp(commandname,cmdinfo->commandname) >= 0)
            break;
    }
    if(cmdinfo != 0)
    {
        NewCommandEvent *event = new NewCommandEvent(cmdinfo->comid,0);
        state.sendevent(event);
        state.process();
        return 1;
    }
    */
    return 0;
}

void WINAPI SetIconByPath(char *path,char *title)
{
    cadwindow->appIcon = QIcon(QString(path));
    cadwindow->seticon((RCHICON)cadwindow->appIcon,_RCT(title));

}

// api for the figure browser
int WINAPI NewFigureBrowser(void **dialog, void *parent)
{
    *dialog = new RCFigureBrowser((QWidget*)parent);
    return *dialog != 0 ? 1 : 0;
}

int WINAPI FigureBrowserInit(void *dialog)
{
    ((RCFigureBrowser*)dialog)->OnInitDialog();
    return 1;
}

int WINAPI FigureBrowserSetFilter(void *dialog, char *filter )
{
    ((RCFigureBrowser*)dialog)->SetFilter(_RCT(filter));
    return 1;
}

int WINAPI FigureBrowserSetRoorDir(void *dialog, char *rootdir )
{
    ((RCFigureBrowser*)dialog)->SetRootDir(_RCT(rootdir));
    return 1;
}

int WINAPI FigureBrowserSetPrefix(void *dialog, char *prefix )
{
    ((RCFigureBrowser*)dialog)->SetPrefix(_RCT(prefix));
    return 1;
}

int WINAPI FigureBrowserShowSettingsGroup(void *dialog, int group )
{
    ((RCFigureBrowser*)dialog)->ShowSettingsGroup(RCFigureBrowser::settingType(group));
    return 1;
}

int WINAPI FigureBrowserExec(void *dialog)
{
    return ((RCFigureBrowser*)dialog)->exec();
}

int WINAPI DeleteFigureBrowser(void *dialog)
{
    delete (RCFigureBrowser*)dialog;
    dialog = 0;
    return 1;
}

char * WINAPI FigureBrowserGetFilename(void *dialog, char *filename)
{
    //static char tmp[600];
    //memset(tmp,0,600);
    if(filename != 0)
        strcpy(filename,(char*)QString(((RCFigureBrowser*)dialog)->GetFileName()).toLatin1().data());
    return filename;
}

int WINAPI FigureBrowserSetFigureOptions(void *dialog, int origcolour, int origstyle, int origlayer, int origweight, int orienttoview)
{
    ((RCFigureBrowser*)dialog)->SetFigureOriginalColour(origcolour);
    ((RCFigureBrowser*)dialog)->SetFigureOriginalStyle(origstyle);
    ((RCFigureBrowser*)dialog)->SetFigureOriginalLayer(origlayer);
    ((RCFigureBrowser*)dialog)->SetFigureOriginalWeight(origweight);
    ((RCFigureBrowser*)dialog)->SetFigureOrientToView(orienttoview);
    return 1;
}

int WINAPI FigureBrowserSetTitle(void *dialog, char *title)
{
    ((RCFigureBrowser*)dialog)->SetTitle(_RCT(title));
    return 1;
}
