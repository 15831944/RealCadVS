//
//  The definition of the Control Flow classes
//
#ifndef _NCMAIN_H
#define _NCMAIN_H

//#undef UNICODE

#define REALCAD          0     //  RealCAD
#define REALCAD_STUDENT  1     //  RealCAD student version
#define TRICAD           2     //  TriCad
#define AUSCAD           3     //  AUSCAD
#define TRICAD_DEMO      4     //  TriCad demo version
#define TRICAD_STUDENT   5     //  TriCad 16 bit student version
#define FLEXICAD         6     //  FlexiCad 16 bit student version
#define REALCAD_DEMO     7     //  RealCAD demo version

#define VERSION REALCAD
//#define CADAUSTRALIAVERSION 1

#define FULLMENUS        0     //  Everything
#define NOARCHITECTMENUS 1     //  Everything except architect
#define STUDENTMENUS     2     //  Student menus
#define FLEXICADMENUS    3     //  Flexicad Student menus

#define MENUS FULLMENUS
//#define MENUS STUDENTMENUS

//#ifndef MAJOR_VERSION
//#define MAJOR_VERSION 8
//#endif
//#ifndef MINOR_VERSION
//#define MINOR_VERSION 0
//#endif

#define MAX_LAYERS 2048
#define STRING_LENGTH 300

// start of windows includes
#ifndef _USING_QT

#ifdef _WIN32_WCE
#include <windows.h>

#include "tchar.h"
typedef int FILE;
extern int fclose(FILE *);
struct MINMAXINFO {int w,h;  };
extern int sscanf(const char *,const char *,...);
extern int sprintf(char *,const char *,...);
extern int fscanf(FILE *,const char *,...);
extern int fprintf(FILE *,const char *,...);
extern char *fgets(char *,int ,FILE *);
extern int fputc(int,FILE *);
extern double strtod(const char *,char **);
extern long strtol(const char *,char **,int);
extern FILE *fopen(const char *,const char *);
extern FILE *fopen(const TCHAR *,const TCHAR *);
extern double atof(const char *);
extern char *strlwr(char *);
extern int fread(void *,int,int,FILE *);
extern int fwrite(const void *,int,int,FILE *);
extern int fseek(FILE *,long,int);
extern char *strcpy(char *,const char *);
extern int stricmp(const char *,const char *);
extern char *strrchr(const char *,int);
extern int exit(int);
extern void BeginPath(HDC);
extern void CloseFigure(HDC);
extern void EndPath(HDC);
extern void SelectClipPath(HDC,int);
#define EOF -1
#define SEEK_SET 1
#define SEEK_END 2
#define DT_METAFILE 1056
int getc(FILE *);
int ungetc(int,FILE *);
long ftell(FILE *);
extern char *GlobalAlloc(int,int);
extern void GlobalFree(void *);
extern char *GlobalLock(void *);
extern void GlobalUnlock(void *);
extern void unlink(char *);
extern void unlink(TCHAR *);
extern void mkdir(char *);
extern unsigned long GlobalSize(HGLOBAL);
extern getcwd(char *,int);
extern getcwd(TCHAR *,int);
#define GHND 0
#define GMEM_FIXED 0
extern RCHFONT CreateFont(
  int nHeight,             // logical height of font
  int nWidth,              // logical average character width
  int nEscapement,         // angle of escapement
  int nOrientation,        // base-line orientation angle
  int fnWeight,            // font weight
  DWORD fdwItalic,         // italic attribute flag
  DWORD fdwUnderline,      // underline attribute flag
  DWORD fdwStrikeOut,      // strikeout attribute flag
  DWORD fdwCharSet,        // character set identifier
  DWORD fdwOutputPrecision,  // output precision
  DWORD fdwClipPrecision,  // clipping precision
  DWORD fdwQuality,        // output quality
  DWORD fdwPitchAndFamily,  // pitch and family
  const char *lpszFace         // pointer to typeface name string
);
#undef GetTextExtentPoint
extern void GetTextExtentPoint(HDC,const char *,int,LPSIZE);
extern void TextOut(HDC,int,int,const char *,int);
extern void ExtTextOut(HDC,int,int,int,int,const char *,int,int*);
extern void MoveToEx(HDC,int,int,LPPOINT);
extern void LineTo(HDC,int,int);
extern void FrameRect(HDC,CONST RECT *,RCHBRUSH);
extern void GetScrollRange(HWND,int ,int *,int *);
#define OUT_STROKE_PRECIS 0
#define PROOF_QUALITY 0
#define PS_DOT PS_DASH
#define WS_EX_TOOLWINDOW 0
#define WS_OVERLAPPEDWINDOW 0
#define WS_POPUPWINDOW 0
#define SW_NORMAL 0
#define OUT_TT_PRECIS 0
#define IDC_ARROW 0

#else

#include <stdio.h>
#include <dos.h>
#include <io.h>
#undef enable

#if defined(WIN95)
#include <ctl3d.h>
#endif

#include <direct.h>
#include <new.h>

#endif

#include <stdarg.h>
#include <stdlib.h>
#include <mmsystem.h>

#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
#include <digitalv.h>
#include <vfw.h>
#endif

#include <commdlg.h>
#include <shellapi.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#if ! defined(WIN32) && ! defined(_WIN32_WCE)
#include <ole2.h>
#include <ole2ver.h>
#include <storage.h>
#include <toolhelp.h>
#endif

#if defined(_MSC_VER) || defined(_WIN32_WCE)
#include "tchar.h"
#include <limits.h>
#include <float.h>
#include "shlobj.h"
#include <mapi.h>
#include <mapix.h>
#endif

// end of Windows includes
#endif

#include <stdint.h>

// Qt defines
#ifdef _USING_QT
#include "qtdefs.h"
#endif
//

#include "ncpoint.h"
#include "ncbmask.h"
#include "nclist.h"
#include "NCMATRIX.H"
#include "ncview.h"
#include "ncwork.h"
#include "ncgrid.h"
#include "ncval.h"
#include "Ncfile.h"
#include "ncent.h"
#include "Ncinters.h"
#include "ncdialog.h"
#include "Nccom.h"
#include "Ncshade.h"
#include "Nchide.h"
#include "NCRES.H"
#include "ncdxf.h"
#include "ncray.h"
//#include "typeinfo.h"
#include "RCOpenGL.h"

#define RCFLT_MIN FLT_MIN
#define RCFLT_MAX FLT_MAX
#define RCDBL_MAX FLT_MAX
#define RCDBL_MIN FLT_MIN

#if ! defined(M_PI)
#define M_PI 3.14159265358979323846
#endif
#if ! defined(M_PI_2)
#define M_PI_2 1.57079632679489661923
#endif
#if ! defined(M_PI_4)
#define M_PI_4 0.785398163397448309616
#endif
#if ! defined(MAXDOUBLE)
#define MAXDOUBLE 1.797693E+308
#endif
#define WM_F1DOWN WM_USER

#if ! defined(__Win)
#define __Win
#ifdef _USING_QT
#undef __Win
#endif
extern const char *llabel;
class RCGraphicsScene;

enum CursorShape {  CursorArrow, CursorZoom, CursorPosition, CursorRestore  };

class Program : public QObject
  {
#ifdef USING_WIDGETS
    Q_OBJECT
public:
    RCGraphicsScene *m_scene;
    void checkcustombutton(int cid);
    void resetcustommenus();
    QTimer *qpulsetimer;
    QTimer *qautosavetimer;
    bool pluginmessboxup;
    bool m_interrupt;
    bool m_IsRaytracing;
    RCHWND m_ProgressBarDlg;
#endif
    private:
       RCHINSTANCE hinst;
       RCHACCEL haccel;
       int ncmdshow,busy,commandid,licensed,daysleft;
       RCHCURSOR hcursor,herasecursor,hemptycursor,hcrosshaircursor,hcrosshairsnapcursor,hcrosshairentitycursor,hleftclickcursor,hrightclickcursor;
       RCHCURSOR hpositioncrosshaircursor,hpositioncrosshairsnapcursor,hpositioncrosshairentitycursor,hwaitcursor,hsizewecursor;
       RCHCURSOR hzoomcursor,harrowcursor,hdefaultcursor,hpositioncrosshairintcursor,hpositioncrosshairorigincursor,hcrosshairintcursor,hcrosshairorigincursor;
       //FARPROC *helpfilter,*nextfilter;
       HOOKPROC *helpfilter,*nextfilter;
       RCHBRUSH hbrush;
       char filename[300],homedirectory[300];
       GeneralList apps,cursorlist;
       DWORD nextautosave,nextautobackup;
       RCCHAR *commandline;
     public:
       Program(QObject *parent=0);
       Program(int & argc, char ** argv);
       Program(RCHINSTANCE,int,RCHACCEL,HOOKPROC,HOOKPROC, int argc, char ** arg);
       Program(RCHINSTANCE,int,RCHACCEL,HOOKPROC,HOOKPROC,RCCHAR *cl);
       ~Program();
       bool event(QEvent *);
       void init(RCHINSTANCE,int,RCHACCEL,HOOKPROC,HOOKPROC,RCCHAR *cl);
       RCHINSTANCE gethinst(void)  {  return hinst;  }
       int getncmdshow(void)  {  return ncmdshow;  }
       int getcommandid(void)  {  return commandid;  }
       RCCHAR *getcommandline(void)  {  return commandline;  }
       void clearcommandline(void)  {  commandline = _RCT("");  }
       void setcommandid(int cid)  {  commandid  = cid;  }
       //FARPROC *gethelpfilter(void)  {  return helpfilter;  }
       //FARPROC *getpnextfilter(void)  {  return nextfilter;  }
        HOOKPROC *gethelpfilter(void)  {  return helpfilter;  }
        HOOKPROC *getpnextfilter(void)  {  return nextfilter;  }
       RCHACCEL gethaccel(void)  {  return haccel;  }
       void sethaccel(RCHACCEL ha)  {  haccel = ha;  }
       RCHCURSOR getsizewecursor(void) {  return hsizewecursor; }
       RCHCURSOR getwaitcursor(void) {  return hwaitcursor;  }
       RCHCURSOR gethcursor(void) {  return hcursor;  }
       RCHCURSOR gethdefaultcursor(void) {  return hdefaultcursor;  }
       RCHCURSOR getharrowcursor(void) {  return harrowcursor;  }
       RCHCURSOR gethemptycursor(void) {  return hemptycursor;  }
       RCHCURSOR getherasecursor(void) {  return herasecursor;  }
       RCHCURSOR gethpositioncrosshaircursor(void) {  return hpositioncrosshaircursor;  }
       RCHCURSOR gethcrosshaircursor(void)
       {
           return hdefaultcursor == hpositioncrosshaircursor ? hpositioncrosshaircursor : hcrosshaircursor;
       }
       RCHCURSOR gethcrosshairsnapcursor(void) {  return hdefaultcursor == hpositioncrosshaircursor ? hpositioncrosshairsnapcursor : hcrosshairsnapcursor;  }
       RCHCURSOR gethcrosshairentitycursor(void) {  return hdefaultcursor == hpositioncrosshaircursor ? hpositioncrosshairentitycursor : hcrosshairentitycursor;  }
       RCHCURSOR gethcrosshairintcursor(void) {  return hdefaultcursor == hpositioncrosshaircursor ? hpositioncrosshairintcursor : hcrosshairintcursor;  }
       RCHCURSOR gethcrosshairorigincursor(void) {  return hdefaultcursor == hpositioncrosshaircursor ? hpositioncrosshairorigincursor : hcrosshairorigincursor;  }
       void ShowLeftClick(void);
       void ShowRightClick(void);
       RCHBRUSH getbuttonfacebrush(void)  {  return hbrush;  }
       int processmessages(int);
       void processpaintmessages(void);
       int setprogressbardlg(RCHWND progressbar);
       int tickprogressup();
       void cancelprogressbardlg();
       int getinterrupt(void);
       void setinterrupt(bool onoff=true){ m_interrupt = onoff;}
       int resetinterrupt(){return m_interrupt=false;}
       void setbusy(int);
       void loadapp(RCCHAR *);
       void loadapp(char *);
       RCHINSTANCE* loadcadmodule(RCCHAR *path);
       void unloadcadmodule(RCCHAR *path);
       void savedefaultapps(void);
       void loaddefaultapps(void);
       void filenewapps(void);
       void fileopenapps(void);
       void fileopendxfapps(void);
       void filesaveapps(void);
       void fileexitapps(void);

       char *getloadedappname(int index,char *name);
       RCCHAR *getloadedappname(int index,RCCHAR *name);
       bool ismoduleloaded(RCCHAR *name);
       bool ismoduleloaded(char *name);
       RCHMODULE *getloadedapp(RCCHAR *name);
       char *getloadedappversion(char *name,char *version);
       RCCHAR *getloadedappversion(RCCHAR *name,RCCHAR *version);
       char *getappversion(char *name,char *version);
       RCCHAR *getappversion(RCCHAR *name,RCCHAR *version);

       char *getmoduleversion(char *name,char *version);
       RCCHAR *getmoduleversion(RCCHAR *name,RCCHAR *version);
       char *getmodulename(char *name);
       RCCHAR *getmodulename(RCCHAR *name);
       void unloadapp(char *);
       void unloadapp(RCCHAR *);
       void setautosavetimer(void);
       void stopautosavetimer(void);
       void checkautosavetimer(void);
       void updatecursortype(int reversed=0);// reversed 0=black on white; mode 1 = white on black
       void setlicensed(int l)  {  licensed = l;  }  // just guessing but I think
       int getlicensed(void)  {  return licensed;  } // 1=Pro;2=Lt;3=Draft
       void setdaysleft(int d)  {  daysleft = d;  }
       int getdaysleft(void)  {  return daysleft;  }
       void setcursorshape(CursorShape);
       int calcCursorFromBackground();
       void help(int,int);
       void drawGraphicsCursor();
  };

extern Program *program;

enum DrawMode { DM_NORMAL,DM_INVERT,DM_ERASE,DM_SELECT,DM_GRID,DM_INVERTDASH,DM_DASH,DM_FLASH1,DM_FLASH2,DM_FLASH3, DM_HANDLE_SELECT, DM_GL, DM_PRINT, DM_PRINT_PREVIEW, DM_MARKER, DM_SCROLLING, DM_WPOTHER, DM_PREVIEW, DM_TSELECT };
//enum DrawMode { DM_NOOP=0,DM_NORMAL=1,DM_INVERT=2,DM_ERASE=4,DM_SELECT=8,DM_GRID=16,DM_INVERTDASH=32,DM_DASH=64,DM_FLASH1=128,DM_FLASH2=256,DM_FLASH3=512, DM_GL=1024 , DM_QT=2048 };

class HighLevelEvent;
class View3dWindow;
class View2dWindow;

typedef void (*Command)(int *,HighLevelEvent *,void **);

class CommandInfo
  {public:
     int applicationid,commandid,subcommand,allocated,implemented;
     Command command;
     RCCHAR *description,*shortdescription;
     CommandInfo(int appid,int comid,int subcom,Command c,RCCHAR *desc);
     CommandInfo(int appid,int comid,int subcom,Command c,int desc,int shortdesc = 0);
     ~CommandInfo();
  };

class Commands : public GeneralList
  {public:
     ~Commands();
     void add(int,int,int,Command,RCCHAR *);
     void add(int,int,int,Command,char *);
     void add(int,int,int,Command,int,int = 0);
     Command command(int);
     int subcommand(int);
     RCCHAR *description(int);
     RCCHAR *shortdescription(int);
     int command(RCCHAR *);
     int implemented(int);
  };

enum EventType {  EntitySelected,Coordinate3d,Coordinateuv,Coordinate2d,NewCommand,Abort,Exit,User  };

class HighLevelEvent
  {  public:
       virtual int isa(int) = 0;
       virtual int type(void) = 0;
  };

class EntitySelectedEvent : public HighLevelEvent
  {private:
     Entity *e;
     Point p,xaxis,yaxis;
     int addtoselection,endselected;
     View3dWindow *w;
   public:
     EntitySelectedEvent(Entity *entity,Point position,Point xa,Point ya,View3dWindow *win,int ats,int es)
       {  e = entity;  p = position;  xaxis = xa;  yaxis = ya;  w = win;  addtoselection = ats;  endselected = es;  }
     int isa(int eventid) {  return EntitySelected == eventid;  }
     int type(void) {  return EntitySelected;  }
     Entity *getentity(void) {  return e;  }
     int getaddtoselection(void)  {  return addtoselection;  }
     int getendselected(void)  {  return endselected;  }
     Point getp(void) {  return p;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     View3dWindow *getwindow(void)  {  return w;  }
  };

class Coordinate3dEvent : public HighLevelEvent
  {private:
     Point p,xaxis,yaxis;
     View3dWindow *w;
     int addtoselection;
   public:
     Coordinate3dEvent(Point p1,Point xa,Point ya,View3dWindow *win,int ats)
       {  p = p1;  xaxis = xa;  yaxis = ya;  w = win;   addtoselection = ats;  }
     int isa(int eventid) {  return Coordinate3d == eventid;  }
     int type(void) {  return Coordinate3d;  }
     Point getp(void) {  return p;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     View3dWindow *getwindow(void)  {  return w;  }
     int getaddtoselection(void)  {  return addtoselection;  }
  };

class CoordinateuvEvent : public HighLevelEvent
  {private:
     int control;
     Point p;
     View3dWindow *w;
     int addtoselection;
   public:
     CoordinateuvEvent(Point p1,int c,View3dWindow *window,int ats)
       {  p = p1;  control = c;  w = window;   addtoselection = ats;  }
     int isa(int eventid) {  return Coordinateuv == eventid;  }
     int type(void) {  return Coordinateuv;  }
     Point coordinate(void) {  return p;  }
     int getcontrol(void) {  return control;  }
     View3dWindow *window(void)  {  return w;  }
     int getaddtoselection(void)  {  return addtoselection;  }
  };

class Coordinate2dEvent : public HighLevelEvent
  {private:
     Point p;
   public:
     Coordinate2dEvent(Point p1)  {  p = p1;  }
     int isa(int eventid) {  return Coordinate2d == eventid;  }
     int type(void) {  return Coordinate2d;  }
     Point getp(void) {  return p;  }
  };

class NewCommandEvent : public HighLevelEvent
  {private:
     int commandid,repeat;
     Window *window;
   public:
     NewCommandEvent(int id,Window *w,int r = 0)   {  commandid = id;  window = w;  repeat = r;  }
     int isa(int eventid) {  return NewCommand == eventid;  }
     int type(void) {  return NewCommand;  }
     int getcommandid(void)  {  return commandid;  }
     int getrepeat(void)  {  return repeat;  }
     Window *getwindow(void)  {  return window;  }
  };

class AbortEvent : public HighLevelEvent
  {public:
     int isa(int eventid) {  return Abort == eventid;  }
     int type(void) {  return Abort;  }
  };

class UserEvent : public HighLevelEvent
  {private:
     int userid;
   public:
     UserEvent(int ui)  {  userid = ui;  }
     int isa(int eventid) {  return User == eventid;  }
     int type(void) {  return User;  }
     int getuserid(void) {  return userid;  }
  };

class ExitEvent : public HighLevelEvent
  {private:
     int commandid;
   public:
     ExitEvent() {  commandid  = -1;  }
     ExitEvent(int c)  {  commandid = c;  }
     int isa(int eventid) {  return Exit == eventid;  }
     int type(void) {  return Exit;  }
     int getcommandid(void)  {  return commandid;  }
  };

class CommandState
  {friend class CommandStack;
   private:
     Command command;
     int commandid,state;
     void *data;
     BitMask tempmask;
     AttributeMasks selmask;
     EntityHeader defheader;
     RCCHAR prompt[600];
     EntityList excludelist;
   public:
     CommandState(Command c,int cid,int s,void *d,const BitMask &tm,const AttributeMasks &am,const EntityHeader &eh,EntityList el,const RCCHAR *p) : tempmask(32)
       {  command = c;  commandid = cid;  state = s;  data = d;  tempmask = tm;  selmask = am;  defheader = eh;  excludelist = el;  strcpy(prompt,p);
       }
  };

class CommandStack
  {private:
     GeneralList stack;
   public:
     void push(Command,int,int,void *,const BitMask&,const AttributeMasks&,const EntityHeader&,EntityList&,const RCCHAR *);
     int pop(Command *,int *,int *,void **,BitMask *,AttributeMasks *,EntityHeader *,EntityList *,RCCHAR *);
     int length(void)  {  return stack.length();  }
  };

enum States {  InitialState = -1,ExitState = -2  };
enum StatusCode  {  Memory,ButtonMenuFull  };

class MetreCom
  {private:
     int buttonstate;
     int cursordrawn;
     Point p,dir;
   public:
     int initialize(void);
     int terminate(void);
     int processinput(void);
     void drawcursor(Point,Point);
  };

class StateMachine
  {
#ifdef USING_WIDGETS
    QGraphicsItem *qgiDragger;
    // for gl dragging line
    Line *glDraggerLine;
public:
    Transform glDraggerTform;
    void setupentitymaskEx(void);
    // KMJ : NOTE :
    // these two functions are to get at the command stack for tweaking the settings while in a command
    // use with care!! they may become redundant if we can modify the state machine to take care of this
    CommandStack tempcs;
    // this function allows you to push the arguments onto the state machine's internal command stack
    void cspush(Command a1,int a2,int a3,void *a4,const BitMask& a5,const AttributeMasks& a6,const EntityHeader& a7,EntityList& a8,const RCCHAR *a9)
    { cs.push(a1,a2,a3,a4,a5,a6,a7,a8,a9); }
    // this function allows you to pop the arguments off the top of the state machine's internal command stack
    int cspop(Command *a1,int *a2,int *a3,void **a4,BitMask *a5,AttributeMasks *a6,EntityHeader *a7,EntityList *a8,RCCHAR *a9)
    { return cs.pop(a1,a2,a3,a4,a5,a6,a7,a8,a9); }
    int mlastcslength; // a saved count of the command stack lenght
    // this function saves the current command stack lenght and returns it
    int savecslength(){ mlastcslength = cs.length(); return mlastcslength; }
    // this function returns the last command stack length
    int lastcslength(){ return mlastcslength; }
    // this function lets you rewind the command stack by the given count
    // it goes backwards in time!
    void unwindcs(int count)
    {
        RCCHAR prompt[300];
        for(int i=0; i<count; i++)
        {
            cs.pop(&command,&commandid,&state,&data,&tempentitymask,&selmask,&defheader,&excludelist,prompt);
            tempcs.push(command,commandid,state,data,tempentitymask,selmask,defheader,excludelist,prompt);
        }
    }
    // this function rewinds the command stack without replacing the stored item at the start of the rewind
    // this allows you to modify the items on the stack whithout having them over written
    // it allows you to change the future!
    void rewindcs(int count)
    {
        RCCHAR prompt[300];
        for(int i=0; i<count; i++)
        {
            if(i>1)
                tempcs.pop(&command,&commandid,&state,&data,&tempentitymask,&selmask,&defheader,&excludelist,prompt);
            cs.push(command,commandid,state,data,tempentitymask,selmask,defheader,excludelist,prompt);
        }
    }
    // KMJ : END NOTE :
#endif
    private:
       CommandStack cs;
       int state,dragon,dragdrawn,inputdevice,lastend,lastviewno,lastcontrol;
       void *data;
       Command command;
       int commandid,lastcommandid,lastsubcommandid;
       HighLevelEvent *event;
       Point startdragpos,lastdragpos,pos,lastpickpoint;
       View3dWindow *lastdragwindow;
       EntityList selection;
       Entity *handledentity;
       Entity *pickedentity;
       BitMask lastvisible;
       RCCHAR lastviewname[300];
       void drawdragGL(Point p);
       void drawdrag(Point p);
     public:
       BitMask tempentitymask;
       EntityHeader defheader;
       AttributeMasks selmask;
       char groupnamemask[300];
       EntityList excludelist;
       StateMachine();
       ~StateMachine();
       void setposition(Point p);
       Point getposition(void) {  return pos; }
       void fromposition(void);
       Point from(int csystem,Point p,int ix,int iy,int iz);
       BitMask &getlastvisible(void)  {  return lastvisible;  }
       RCCHAR *getlastviewname(void)  {  return lastviewname;  }
       void setlastviewname(char *vn)  {  strcpy(lastviewname,vn);  }
       int getlastend(void)  {  return lastend;  }
       int getlastviewno(void)  {  return lastviewno;  }
       Point getlastpickpoint(void)  {  return lastpickpoint;  }
       void startdragGL(Point p,View3dWindow *w,int handlenumber);
       void startdrag(Point p,View3dWindow *w,int handlenumber);
       int dragging(void)  {  return dragon;  }
       void setdragging(int dragflag=0)  { dragon = dragflag; }
       int getdragdrawn(void)  {  return dragdrawn;  }
       void dragtoGL(Point p,View3dWindow *w);
       void dragto(Point p,View3dWindow *w);
       void moveselectionGL(Point p,View3dWindow *w);
       void moveselection(Point p,View3dWindow *w);
       void copyselectionGL(Point p,View3dWindow *w);
       void copyselection(Point p,View3dWindow *w);
       void enddragGL(void);
       void enddrag(void);
       void destroyselection(int);
       void sendevent(HighLevelEvent *nextevent);
       int eventavailable(void);
       HighLevelEvent *getevent(void) {  return event;  }
       EntityList getselection(void) const {  return selection;  }
       void sethandledentity(Entity *e);
       Entity *gethandledentity(void);
       void setselection(EntityList,int highlighted = 0);
       void removefromselection(Entity *);
       void addtoselection(Entity *);
       void process(void);
       void fail(StatusCode code, const RCCHAR *message = NULL);
        void fail(StatusCode code, const char *message = NULL);
       void setupentitymask(void);
       int getinputdevice(void) {  return inputdevice;  }
       void setinputdevice(int);
       void timerevent(void);
       int getlastcommandid(void)  {  return lastcommandid;  }
       int getcommandid(void)  {  return commandid;  }
       int getcommandactive(void)  {  return command != 0;  }
       Entity *getpickedentity(void)  {  return pickedentity;  }
       void setpickedentity(Entity *e)  {  pickedentity = e;  }
       // kmj testing for plugin events under Qt
       void setcommandid(int id) { commandid = id; }
       void setlastcommandid(int id) { lastcommandid = id; }
       // kmj
       // kmj for open gl drag line mode
       void drawdraggerGL(int mode, View3dSurface *v);
       // kmj
};

class Window;

class WindowList : public GeneralList
  {public:
     Window *next(void) {  return (Window *) GeneralList::next();  }
  };

class View3dWindowList : public GeneralList
  {public:
#ifdef USING_WIDGETS
     int add(View3dWindow *window);
#endif
     View3dWindow *next(void) {  return (View3dWindow *) GeneralList::next();  }
     View3dWindowList copy(void)  {View3dWindowList tmp;  tmp.setlist(GeneralList::copy()); return tmp;  }
  };

class WindowManager
  {private:
     WindowList list;
     Window     *lastwindow;
   public:
     WindowManager();
     Window *getlastwindow()  {  return lastwindow;  }
     int add(Window *window);
     void del(Window *window);
     void destroy(void);
     Window *match(RCHWND);
  };

#ifdef DERIVE_FROM_WIDGETS
class Control : public QWidget
{
    Q_OBJECT
   protected:
    QWidget *_qparent;
#else
class Control
{
#endif
   public:
     int x,y,w,h,xpos,ypos,dragging,dragoffsetx,dragoffsety,lastdragx,lastdragy;
     int borderx1,bordery1,borderx2,bordery2;
     Window *parent;
     RCHDC     hDC; // this is the actual QWidget for this control
     Control(RCHDC hdc=0)
     {
         //qDebug() << "Entering : Control::Control ";
         dragging = 0;  borderx1 = bordery1 = borderx2 = bordery2 = 0; hDC = 0;
     }
     virtual int pick(int xp,int yp)
     {
         return dragging || (xp >= x && xp <= x + w - 1 && yp >= y && yp <= y + h - 1);
     }
     virtual void mousemoveevent(int,int,int) {}
     virtual void timerevent() {}
     virtual void fixup(Window *);
     virtual void draw(int) {};
     int getx(void)  {  return x;  }
     int gety(void)  {  return y;  }
     void drawborder();
     int drag(int,int,int,int,int);
     virtual void move(int dx,int dy);

    protected:
     void paintEvent(QPaintEvent *);
};

class Prompt;
class RCOneCoordinateEntry;

class OneCoordinateEntry
  {
    friend class CoordinateEntry;
#ifdef USING_WIDGETS
   // this is a composite widget that can be embeded in a toolbar etc.
    RCOneCoordinateEntry *m_CoordEntry;
    QWidget *m_qwPrompt; // prompt
    QWidget *m_qwEdit; //  window
    QWidget *m_qwIcon; // fixfindow
    bool isLocked;
public:
    QWidget *get_QWprompt() { return m_qwPrompt; }
    QWidget *get_QWedit() { return m_qwEdit; }
    QWidget *get_QWIcon() { return m_qwIcon; }
#endif
   private:
     int x,y,xpos,ypos,w,h,name;
     Prompt *prompt;
     int incremental;
     Window *window;
     Window *fixwindow;
     RCHICON hlocked,hunlocked;
   public:
#ifdef USING_WIDGETS
     OneCoordinateEntry(Window *,int,int,int,int,int,QWidget *qparent=0);
     void remove(QString str)
     {
         QString text;
         if(m_qwEdit)
         {
             text = ((QLineEdit*)m_qwEdit)->text();
             text.remove(QString(str),Qt::CaseInsensitive);
             ((QLineEdit*)m_qwEdit)->setText(text);
         }
     }
     void remove(RCCHAR* c)
     {
         remove(QString(c));
     }
#else
     OneCoordinateEntry(Window *,int,int,int,int,int);
#endif
     ~OneCoordinateEntry();
     void toggle(void);
     void cleartoggle(void);
     void togglefixed(void);
     void select(void);
     void deselect(void);
     bool isallselected(void);
     void set(double);
     int get(double,double *);
     int getlocked(void);
     void setlocked(int);
     void enablelock(bool);
     void move(Window *,int,int,int,int);
     int getx(void)  {  return x;  }
     int gety(void)  {  return y;  }
     void setvisible(int v);
     void disablew(void);
     void setenabled(bool);
  };

class CoordinateEntry : public Control
  {
#ifdef USING_WIDGETS
    QWidget *m_qw;
public:
    QWidget *getQW() { return m_qw; }
    bool containsWidget(QWidget *w);
#endif
    private:
     int csystem,newcoordinate,lastbuttons;
     OneCoordinateEntry *xentry,*yentry,*zentry;
   public:
     CoordinateEntry(Window *,int,int,int);
     ~CoordinateEntry();
     void setcsystem(int);
     int getcsystem() { return csystem; }
     void move(Window *,int,int,int);
     void charevent(UINT,DWORD);
     void draw(int);
     void position(Point p);
     int getx(void)  {  return xentry->getx();  }
     int gety(void)  {  return xentry->gety()-2;  }
     Point fix(Point);
     int getxfixed(void)  {  return xentry->getlocked();  }
     int getyfixed(void)  {  return yentry->getlocked();  }
     int getzfixed(void)  {  return zentry->getlocked();  }
     void setvisible(int v)  {  xentry->setvisible(v);  yentry->setvisible(v);  zentry->setvisible(v);  }
     void disablez(void)  {  zentry->disablew();  }
     void mousemoveevent(int,int,int);
     int pick(int xp,int yp);
     void move(int dx,int dy);
     void setEntryFocus(RCCHAR *name, bool onoff=true);

protected:
     void paintEvent(QPaintEvent *);
  };

class Button;
class StyleEntry : public Control
{
#ifdef USING_WIDGETS
    // this is a composite widget that can be embeded in a toolbar etc.
    QWidget *m_qw; // the toolbar widget
    RCstyleIndicator *m_ind;
public:
    QWidget *getQW() { return m_qw; }
    RCstyleIndicator *getIndicator();
#endif
   private:
     int lastbuttons;

     Button *entitybutton;
     Button *colourbutton;
     Button *stylebutton;
     Button *weightbutton;
     Button *layerbutton;
     Button *typebutton;
     Button *clonebutton;

     Window *colourwindow;
     Window *stylewindow;
     Window *weightwindow;
     Window *layerwindow;
     Window *typewindow;
     QComboBox *typeCombo;

   public:
     StyleEntry(Window *, int, int, int);
     ~StyleEntry();
     void move(Window *,int,int,int);
     void charevent(UINT,DWORD);
     void draw(int);
     void position(Point p);
     int getx(void)  {  return x;  }
     int gety(void)  {  return y;  }
     void mousemoveevent(int,int,int);
     int pick(int xp,int yp);
     void setcolour(int);
     void setlayer(int);
     void setstyle(int);
     void setweight(double);
     void settype(RCCHAR *);
     void updatedefaults(void);
     void fixup(Window *);
     RCHWND gettypehwnd();
     void move(int dx,int dy);
#ifdef USING_WIDGETS
     Button * getButton(QString bname);
     void selectButton(Button *button);
     void deselectButton(Button *button);
     bool containsWidget(QWidget *w);
#endif
  };



class OneCoordinateDisplay
  {
     friend class CoordinateDisplay;
#ifdef USING_WIDGETS
   // this is a composite widget that can be embeded in a toolbar etc.
    QWidget *m_qwPrompt; // prompt
public:
    QWidget *get_QWprompt() { return m_qwPrompt; }
#endif
   private:
     int x,y,xpos,ypos,w,h,name;
     Prompt *prompt;
     Window *window;
   public:
#ifdef USING_WIDGETS
     OneCoordinateDisplay(Window *,int,int,int,int,int,QWidget *qparent=0);
#else
     OneCoordinateDisplay(Window *,int,int,int,int,int);
#endif
     ~OneCoordinateDisplay();
     void set(double);
     void move(Window *,int,int,int,int);
     int getx(void)  {  return x;  }
     int gety(void)  {  return y;  }
     int getwidth(void)  {  return w;  }
     int getheight(void)  {  return h;  }
     RCHDC gethDC(void);
  };

class CoordinateDisplay : public Control
  {
#ifdef USING_WIDGETS
    QWidget *m_qw;
public:
    QWidget *getQW() { return m_qw; }
    void setQW(QWidget *qw) { m_qw = qw; }
#endif
   private:
     OneCoordinateDisplay *xdisplay,*ydisplay,*zdisplay;
   public:
     CoordinateDisplay(Window *,int,int,int);
     ~CoordinateDisplay();
     void move(Window *,int,int,int);
     void draw(int);
     void position(Point p);
     void mousemoveevent(int,int,int);
     int pick(int xp,int yp);
     int getx(void)  {  return xdisplay->getx();  }
     int gety(void)  {  return xdisplay->gety()-2;  }
     void move(int dx,int dy);
  };

enum {  CN_CENTRED = -10000 };

class Prompt : public Control
  {
#ifdef USING_WIDGETS
    QWidget *m_qw;

    void windowHelper(QWidget *qparent=0);
public:
    QWidget* getQW(){ return m_qw; }
    void setQW(QWidget *qw) { m_qw = qw; }
#endif
    private:
     int   promptdisplayed;
     RCCHAR  curprompt[600],nprompt[600],tprompt[600];
     RCCOLORREF colour,backgroundcolour;
   public:
#ifdef USING_WIDGETS
     Prompt(int,int,int,QWidget *parent=0);
     Prompt(int,int,char *,QWidget *parent=0);
     ~Prompt()
     {
         delete m_qw; m_qw=0;
     }
#else
     Prompt(int,int,int);
     Prompt(int,int,char *);
#endif
     void draw(int);
     void shownormalprompt(void);
     RCCHAR *normalprompt(void)  {  return nprompt;  }
     void showtempprompt(void);
     void normalprompt(const RCCHAR *prompt);
     void normalprompt(const char *prompt);
     void normalprompt(int id);
     void tempprompt(const RCCHAR *prompt);
     void tempprompt(int id);
     void move(Window *,int,int);
     void setcolour(RCCOLORREF cr);
     void setbackgroundcolour(RCCOLORREF cr);
     void mousemoveevent(int,int,int);
     int pick(int xp,int yp);
     void move(int dx,int dy);

    protected:
     void paintEvent(QPaintEvent *);
  };

class Button : public Control
  {
#ifdef USING_WIDGETS
    QWidget *m_qw;

    void windowHelper();
public:
    QWidget* getQW(){ return m_qw; }
    void setQW(QWidget *qw){ m_qw = qw; }
#endif
   private:
     int sharedbitmaps,commandid,butstate,over,swidth,sheight,overx,overy,timerticks,saveoverx,saveovery;
     RCHBITMAP hbitmap,hmaskbitmap,hsavebitmap;
     RCCHAR *description,*shortdescription,bname[300];
   public:
#ifdef USING_WIDGETS
     Button(int,int,RCHBITMAP,RCCHAR *,int,RCHINSTANCE hinstance = program->gethinst(),QWidget *qparent=0);
#endif
     Button(int,int,RCCHAR *,int,QWidget *qparent,RCHINSTANCE hinstance = program->gethinst());
     Button(int,int,RCCHAR *,int,RCHINSTANCE hinstance = program->gethinst(),QWidget *qparent=0);
     Button(int,int,char *,int,QWidget *qparent,RCHINSTANCE hinstance = program->gethinst());
     Button(int,int,char *,int,RCHINSTANCE hinstance = program->gethinst(),QWidget *qparent=0);
     Button(int xp,int yp,Button *button,int comid,QWidget *qparent=0);    //  Used when bitmaps need to be shared
     void init(int xp,int yp, RCCHAR *name,int comid,RCHINSTANCE hinstance, QWidget *qparent=0);
     ~Button();
     int pick(int,int);
     void mousemoveevent(int,int,int);
     void in(void);
     void out(void);
     void select(int s = 4);
     void deselect(void);
     void draw(int);
     int match(int c)  {  return commandid == c;  }
     int match(RCCHAR *name) {  return strcmp(name,bname) == 0;  }
     int match(char *name) {  return strcmp(name,bname) == 0;  }
     void timerevent(void);
     void save(int i,RCCHAR *);
     int getover(void)  {  return over;  }
#ifdef USING_WIDGETS
     QString name() { return QString(bname); }
     int comid() { return commandid; }
     QString shortDescription() { return QString(shortdescription); }
     QString Description() { return QString(description); }
     int buttonstate() { return butstate; }
     RCHBITMAP bitmap() { return hbitmap; }
     RCHBITMAP bitmask() { return hmaskbitmap; }
#endif
  };

class ButtonMenu : public Control
  {
#if USING_WIDGETS
    QWidget *m_qw;
public:
    QWidget* getQW(){ return m_qw; }
#endif
   private:
     int    nbuttons,ncolumns,nused,over,showbutton,orientation;
     Button **buttons;
     Button *lastbutton,*previousbutton;
   public:
     ButtonMenu(const int,const int,const int,const int,const int, QWidget *qparent=0);
    ~ButtonMenu();
     int pick(int,int);
     void mousemoveevent(int,int,int);
     void add(Button *);
     void del(Button *);
     void append(Button *);
     int del(int);
     void draw(int);
     void move(Window *,int,int,int);
     void fixup(Window *);
     void checkbutton(int c);
     void timerevent(void);
     void load(RCCHAR *);
     void load(char *);
     void store(RCCHAR *);
     void store(char *);
     Button *match(RCCHAR *);
     Button *match(int commandid);
     void restorepreviousbutton(void);
     void drawdivide(void);
     int getnusedbuttons(void)  {  return nused;  }
     void setshowbutton(int sb)  {  showbutton = sb;  }
     void setorientation(int o);
     int getorientation(void)  {  return orientation;  }
     int getncolumns(void)  {  return ncolumns;  }
     void setnbuttons(int n) { nbuttons = n; }
     int getnbuttons() { return nbuttons; }
     void move(int dx,int dy);
#if USING_WIDGETS
     int getOver() { return over; }
     void setOver(int OnOff) { over = OnOff; }
     void updateGeometry()
     {
         if(m_qw)
         {
             x=xpos=m_qw->pos().x();
             y=ypos=m_qw->pos().y();
             w=m_qw->width();
             h=m_qw->height();
         }
     }
     void resetlayout()
     {
         ((RCToolBox*)m_qw)->resetLayout(((RCToolBox*)m_qw)->getLayoutType());
     }

     bool setToolbox(RCToolBox *tb,int sId=0,int nmcols=0);
     void debug();
#endif
  };

class ViewTab : public Control
  {
#ifdef DERIVE_FROM_WIDGETS
    Q_OBJECT
#endif
   private:
     int current_tab_index,tiled,dock_top,font_size,last_buttons;
     RCHFONT hfont;
   public:
#ifdef DERIVE_FROM_WIDGETS
     ViewTab(QWidget * qwparent = 0) : Control(qwparent) {;}
#endif
     ViewTab(Window *parent,int fs);
    ~ViewTab();
     int pick(int,int);
     void mousemoveevent(int,int,int);
     void add(View *);
     void del(View *);
     int del(int);
     void draw(int);
     int drawtab(RCCHAR *name,int,int,int,int,int,int *);
     void update_window_state(void);
     void set_tab_index(int index)  {  current_tab_index = index;  }
     void size(int,int,int,int);
     void fixup(Window *);
     void checkbutton(int c);
     void select(HWND hwnd);
     void timerevent(void);
     int get_dock_top(void)  {  return dock_top;  }
     void set_tiled(int t)  {  tiled = t;  }
     int get_tiled(void)  {  return tiled;  }
     int get_tab_height(double);
  };

#define MAX_PROPERTIES               200

#define PP_GETLABEL            0
#define PP_GETVALUE            1
#define PP_SETVALUE            2
#define PP_CLICK               3
#define PP_GETNUMPROPERTIES    4

#ifdef _USING_QTCHAR
#define PROPACHAR	0
#define PROPUCHAR	1
typedef int (*ProcessProperty)(int action,int id,void *data,void *text);
#else
typedef int (*ProcessProperty)(int action,int id,void *data,char *text);
#endif

struct Property
{
#ifdef _USING_QTCHAR
  int chtype;
#endif
 RCCHAR label[STRING_LENGTH];
 ProcessProperty process_property;
 void *property_data;
 int id;
 int type;
 int state;

 RECT label_rect;
 RECT value_rect;
};

#define VP_GROUP     0
#define VP_PROPERTY  1

class ViewProperties : public Control
  {
#ifdef DERIVE_FROM_WIDGETS
    Q_OBJECT
#endif
   private:
     int dragging,dock_left,font_size,last_buttons,property_width,label_width,drag_x,drag_y,last_drag_x,last_drag_y,drag_drawn;
     int num_properties,control_property_index;
     Property properties[MAX_PROPERTIES];
     RCHFONT hfont;
     RCHWND control;
     Entity *last_entity;
     int entity_changed;
   public:
#ifdef DERIVE_FROM_WIDGETS
     ViewProperties(QWidget * qwparent = 0) : Control(qwparent) {;}
#endif
     ViewProperties(Window *parent,int fs);
    ~ViewProperties();
     int pick(int,int);
     void mousemoveevent(int,int,int);
     void draw(int);
     void size(int,int,int,int);
     void fixup(Window *);
     void checkbutton(int c);
     void timerevent(void);
     void clear_properties(void);
     int get_current_group_states(void);
     void add_property(RCCHAR *label,int open);
     void add_property(RCCHAR *label,int id,ProcessProperty pp,void *pd);
     int get_dock_left(void)  {  return dock_left;  }
     int get_properties_width(void);
     void restore_width(void);
     void set_last_entity(Entity *e) ;
     int get_entity_changed(void)  {  return entity_changed;  }
     void clear_entity_changed()  {  entity_changed = 0;  }
     RCHWND get_control()   {  return control;  }
  };

class ControlList : public GeneralList
{
public:
     Control *next(void) {  return (Control *) GeneralList::next();  }
};

class OneButtonMenu
{
public:
     RCCHAR name[300];
     RCCHAR title[300];
     ButtonMenu *menu;
     OneButtonMenu(const RCCHAR *n,ButtonMenu *m, const RCCHAR *t=0)
     {
         strcpy(name,n);
         menu = m;
         if(*t !=0)
             strcpy(title,t);
         else
             strcpy(title,name);
     }
     ~OneButtonMenu()
     {
         delete menu;
     }
};

class ButtonMenuList : public GeneralList
{
public:
  OneButtonMenu *next(void) {  return (OneButtonMenu *) GeneralList::next();  }
};

class ButtonMenus
{
 private:
   ButtonMenuList list;
 public:
   ~ButtonMenus();
   int add(const RCCHAR *name,ButtonMenu *menu, const RCCHAR *title=0)
   {
       return list.add(new OneButtonMenu(name,menu,title));
   }
   int add(const char *name,ButtonMenu *menu, const char *title=0)
   {
       return list.add(new OneButtonMenu(_RCT(name),menu,_RCT(title)));
   }
   ButtonMenu *match(const RCCHAR *);
   ButtonMenu *match(const char *);
   void deletemenu(ButtonMenu *menu);
   RCCHAR *match(const ButtonMenu *);
   RCCHAR *title(const ButtonMenu *);
   ButtonMenu *getmenu(int i);
   Button *match(int commandid);
};

class Cursor
{
#ifdef USING_WIDGETS
     QWidget *m_qw;
     QRubberBand *m_rubberband;
   public:
     QGraphicsItemGroup *qgi;
     QWidget* getQW(){ return m_qw; }
#endif

   private:
     int s,a,shape,x,y,xr,yr,width,height,drawn,size;
     int glxp,glyp; // for drawgl
     Window *parent;
     RCHDC hDC;
     void draw(int,int);
   public:
     Cursor(Window *,int w,int h);
     void erase(void);
     void off(void);
     void on(void);
     void style(int);
     void setshape(int);
     void action(int);
     void rubberband(int,int);
     void mousemoveevent(int,int,int);
     void sizeevent(int,int);
     void drawGL(View3dWindow *vs);
     int isdrawing() { return drawn; }
     int getaction() { return a; }
     void drawCursor();
};

typedef void (*RubberbandCallback)(int,void *,Point *,View3dSurface *);
typedef void (*RubberbandRawCallback)(int,void *,int,int,View3dSurface *);
enum {  RB_DRAW,RB_ERASE,RB_TIMER  };

class RBCallbackInfo
{public:
   bool drawable;
   int drawn,level,id;
   Point p;
   RubberbandCallback callback;
   void *data;
   RBCallbackInfo(int,int,RubberbandCallback,void *,bool=true);
};

class RBRawCallbackInfo
{public:
   int drawn,level,id,x,y;
   RubberbandRawCallback callback;
   void *data;
   RBRawCallbackInfo(int,int,RubberbandRawCallback,void *);
};

class RBCallbackList : public GeneralList
{public:
  RBCallbackInfo *next(void) {  return (RBCallbackInfo *) GeneralList::next();  }
};

class RBRawCallbackList : public GeneralList
{public:
  RBRawCallbackInfo *next(void) {  return (RBRawCallbackInfo *) GeneralList::next();  }
};

class Rubberband
{
private:
   int level,id;
   RBCallbackList list;
   RBRawCallbackList rawlist;
 public:
   int drawing;
   Rubberband()  {  id = level = drawing = 0; }
   ~Rubberband();
   void mousemoveevent(View3dSurface *,int,int,int);
   void input3devent(Point,Point);
   void timerevent(void);
   void begin(int);
   void off(void);
   void end(int);
   int add(RubberbandCallback,void *);
   int add(RubberbandRawCallback,void *);
};

class PatternLineStyle;

class Surface
  {
#ifdef USING_WIDGETS
private:
    QGraphicsScene *m_scene;
public:
    int qsidx;
    QPen curPen;
    QColor curColour;
    int curWeight;
    double curMMWeight,curGLWeight;
    QColor curTextColour;
    QGraphicsItem *qgi;


    void setScene(QGraphicsScene *scene){m_scene = scene;}
    QGraphicsScene *getScene();
    /*{
        qDebug() << "(getScene() surface : " << this << " m_scene : " << m_scene;
        return m_scene;
    }*/
    QWidget* getWidget() { return (QWidget*)hWnd; }
    void setWidet(QWidget *qw) { hWnd = qw; }
#endif
   protected:
     RCHWND hWnd;
     RCHDC hDC;
     RCHPEN horiginalpen;
     RCCOLORREF *colourindex,background,fillcolourref,colourref,selectcolourref;
#ifdef USING_WIDGETS
     RCCOLORREF minorcolourref,majorcolourref;
     double printxdotspermm,printydotspermm;
#endif
     double scale,plotscale,width,height,originx,originy,angle,xdotspermm,ydotspermm,widthmm,heightmm,clipx1,clipy1,clipx2,clipy2;
     double a1,b1,c1,a2,b2,c2,lastx,lasty,lastz;
     int newhdc,transform,drawmode,colour,style,weight,allocatedpens,blackandwhite,bandoffset,reverseblackwhite;
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
     int openhpgl(RCCHAR *);
     void closehpgl(void);
     int openiges(RCCHAR *);
     void closeiges(void);
     void setfileformat(int ff)  {  fileformat = ff;  ndirectory = nparameter = 0;  }
     void sethdc(RCHWND,RCHDC);
     void sethwnd(RCHWND);
     void freehdc(void);
     void allocatepens(int);
     RCHDC gethdc(void)  {  return hDC;  }
     RCHWND gethwnd(void)  {  return hWnd;  }
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
#ifdef USING_WIDGETS
     LineStyle *getlinestyle() { return linestyle; }
     RCCOLORREF getGridMinorColour()  {  return minorcolourref;  }
     RCCOLORREF getGridMajorColour()  {  return majorcolourref;  }
     double getprintxdotspermm(void)  {  return printxdotspermm;  }
     double getprintydotspermm(void)  {  return printydotspermm;  }
     void setprintxdotspermm(double xdpmm)  {  printxdotspermm = xdpmm;  }
     void setprintydotspermm(double ydpmm)  {  printydotspermm = ydpmm;  }
#endif
     RCCOLORREF getcolour()  {  return colourref;  }
     virtual void setup(double linestylescale,double length,int,int,int,int,PatternLineStyle *);
     void setcolour(int c,int dm);
     void setfillcolour(RCCOLORREF cr);
     void setstyle(double linestylescale,double length,int s,int dm);
     void setweight(int w,int dm);
#ifdef USING_WIDGETS
     virtual void setupGL(double linestylescale,double length,int,int,int,int,PatternLineStyle *);
     void movetoGL(double,double,double);
     void movetoGL(double,double);
     void linetoGL(double,double,double);
     void linetoGL(double,double);
     void lineGL(double, double, double, double, double, double, int alreadytransformed = 0);
     void line(QPainterPath*, double,double,double,double,int alreadytransformed = 0);
     void line(QPainter*, double,double,double,double,int alreadytransformed = 0);
     QGraphicsItem* line(QGraphicsItem*, double,double,double,double,int alreadytransformed = 0);
     QGraphicsItem* line(double,double,double,double,int alreadytransformed = 0);
     QGraphicsItem* dot(QGraphicsItem*,double,double);
     QGraphicsItem* dot(double,double);
     void dotGL(double,double,double);
     void dotGL(double,double);
     double getpixelmodelmm();
     int getCurStyleId() { return style; }
     int getDrawmode() { return drawmode; }
#else
     void line(double,double,double,double,int alreadytransformed = 0);
     void dot(double,double);
#endif
#ifdef USING_WIDGETS
     QGraphicsItem * fill(QGraphicsItem *, Point **verts, int *nvert, int nloops, int alreadytransformed=0, QBrush *brush=0);
     void fillGL(Point **verts,int *nvert,int nloops,int alreadytransformed=0);
     void cbox2dGL(Point cp, Point xaxis, Point yaxis, double width, double height);
     void cfillbox2dGL(Point cp, Point xaxis, Point yaxis, double width, double height);
     void cross2dGL(Point cp, Point xaxis, Point yaxis, double width, double height);
#endif
     void fill(Point **,int *nvert,int nloops,int alreadytransformed = 0);
     void setclip(double,double,double,double);
     int clip2d(double *,double *,double *,double *);
     void moveto(double,double);
#ifdef USING_WIDGETS
     void moveto(QPainterPath *,double,double);
     void lineto(QPainterPath *, double, double);
     void lineto(QPainter *,double,double);
     QGraphicsItem*  lineto(QGraphicsItem *,double,double);
     //void lineto(double,double);
     QGraphicsItem* lineto(double,double);
#else
     void lineto(double,double);
#endif
     void textout(int t,double x,double y,RCCHAR *s,int l);
     void sizeevent(double,double);
     void sendsizeevent(int,int);
     void settransform2d(double,double,double,double,double,double);
     void gettransform2d(double *,double *,double *,double *,double *,double *);
     int gettransform2d(void)  {  return transform;  }
     double getrotation2d(void);
     void transform2d(double *,double *);
     void untransform2d(double *,double *);
     void cleartransform(void);
     void settransform2d(int tf) { transform = tf; }
     int saveimage(int,const RCCHAR *,int quality);
#if defined(_WIN32_WCE)
     int saveimage(int,const TRCCHAR *,int quality);
#endif
     int getblackandwhite(void)  {  return blackandwhite;  }
     void setblackandwhite(int baw)  {  blackandwhite = baw;  }
     int getreverseblackwhite() { return reverseblackwhite; }
     void setreverseblackwhite(int revbw) { reverseblackwhite = revbw; }
     // QT functions
protected:
     void resizeEvent(QResizeEvent *);
     void paintEvent(QPaintEvent * event);
  };


#define DL_SCALE 128
#define DL_SHIFT 7
#define EXTEND_TABLE_SIZE 10000

#ifdef USING_WIDGETS
enum {  DL_pointat,DL_lineto,DL_moveto,DL_textout,DL_selectfont,DL_colour,DL_style,DL_weight,DL_end,DL_polygon3d
        ,DL_circleitem };
#else
enum {  DL_pointat,DL_lineto,DL_moveto,DL_textout,DL_selectfont,DL_colour,DL_style,DL_weight,DL_end,DL_polygon3d  };
#endif

class DisplayList
  {private:
     long tablesize;
     char ** entitydisplaylist,*dlposition;
     View3dSurface *surface;
     int errorstatus,countmode,minmaxvalid;
     double scalex,offsetx,scaley,offsety,sx,ox,sy,oy,lastdx,lastdy,minx,miny,maxx,maxy,lastx,lasty;
     long dllength,dlcount;
   public:
     DisplayList();
     ~DisplayList();
     int geterrorstatus(void);
     void seterrorstatus(int es) {  errorstatus = es;  }
     int getcountmode(void) {  return countmode;  }
     void setcountmode(void);
     void setsavemode(void);
     long getcount(void);
     long gettablesize(void);
     long getentitiesused(void);
     long getbytesused(void);
     int draw(Entity *,int drawmode, QPainter *);
     int draw(Entity *,int drawmode);
     double pick(Entity *,double,double);
     double pickhor(Entity *,int,int);
     int pick(Entity *,int,int,int,int,int);
     int defined(Entity *);
     void del(Entity *);
     int start(Entity *,long ncoordinates);
     int pointat(double x,double y);
     int moveto(double x,double y);
     int lineto(double x,double y);
#ifdef _USING_QTCHAR
     int selectfont(double,double,LOGFONTW *);
     int textout(double x,double y,RCCHAR *,int);
#else
     int selectfont(double,double,LOGFONT *);
     int textout(double x,double y,char *,int);
#endif
     int end(void);
     int addpolygon3d(Point,Point,Point,int,int *,Point **,int,RCCOLORREF);
     int changecolour(int colour);
     int changestyle(float linestylescale,int style);
     int changeweight(int weight);
     void setorgtransform(double,double,double,double);
     void setdrawtransform(double *,double *,double *,double *);
     int getextents(double *,double *,double *,double *);
     void clear();
     void initialise(int initialsize,View3dSurface *);
#ifdef USING_WIDGETS
     int addCircleItem(QGraphicsItem *);
#endif
  };

inline int DisplayList::geterrorstatus(void)  {  return errorstatus;  }
inline long DisplayList::getcount(void)  {  return dlcount;  }

enum { View3dSurf, View2dSurf, View3dSubsurf  };
enum RepaintStyle { Wireframe,Hidden,HiddenDashed,Shaded,ShadedHidden,ShadedHiddenDashed,ShadedTexture,ShadedHiddenTexture,RayTraced  };

class View3dSurface : public virtual Surface
  {
    friend class Polygon3d;
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgirb; // this is for rubberband graphics
    QGraphicsItem *qgimk; // this is for marker graphics
#endif
protected:
     View *view;
     double umax,vmax,umin,vmin,dumax,dvmax,dumin,dvmin;
     double m11,m12,m13,m14;
     double m21,m22,m23,m24;
     double m31,m32,m33,m34;
     double a1,a2,b1,b2,zmin,depth;
     Point uaxis,vaxis,waxis;

     class clipplanes
       {public:
          Point n1,n2,n3,n4,n5,n6;
          double d1,d2,d3,d4,d5,d6;
       } clip;
     int clipplanesvalid;

     RepaintStyle repaintstyle;
     BitMask *displaylayertable;
     BitMask *selectionlayertable;
     BitMask *lockedlayertable;
     int dirty;
     RCHDC printerhdc;
     int israytracing;
   public:
     DisplayList displaylist;
     Transform modeltransform;

     View3dSurface(View *,double,double,double,double,int dl,BitMask *);

     virtual int getsurfacetype(void)  {  return View3dSurf;  }
     View *getview(void);
     void setView(View *v);
     int testdisplaylayer(int l)  {  if (displaylayertable == 0) return 1; else return displaylayertable->test(l);  }
     void setdisplaylayertable(BitMask *l)  {  displaylayertable = l;  }
     BitMask *getdisplaylayertable(void)  {  return displaylayertable;  }
     int testselectionlayer(int l)  {  if (selectionlayertable == 0) return 1; else return selectionlayertable->test(l);  }
     void setselectionlayertable(BitMask *l)  {  selectionlayertable = l;  }
     BitMask *getselectionlayertable(void)  {  return selectionlayertable;  }
     double getdepth(void);
     void setdepth(double d);
     double getumin(void);
     double getumax(void);
     double getvmin(void);
     double getvmax(void);
     void setuvminmax(double,double,double,double);
     int inside(Point);
     int getclipplanesvalid(void)  {  return clipplanesvalid;  }
     void setclipplanesvalid(int cpv)  {  clipplanesvalid = cpv;  }
     RepaintStyle getrepaintstyle(void)  {  return repaintstyle;  }
     void setrepaintstyle(RepaintStyle r)  {  repaintstyle = r;  }
     Point uv(int,int);
     Point uvtomodel(Point);
     Point uvtoscreen(Point);
     Point screentouv(Point);
     Point modeltoview(Point);
     Point modeltouv(Point);
     Point viewtoscreen(Point);
     Point modeltoscreen(Point);
     int clip3dfb(Point *,Point *);
     int clip3d(Point *,Point *);
#ifdef USING_WIDGETS
     void clearScene();
     Point screentoview(Point p);
     Point viewtomodel(Point p);
     Point screentomodel(Point p);
     Point screentomodelex(Point p);
     Transform& setupmatrix(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int p);
     QGraphicsItem* line3d(QGraphicsItem*,Point,Point);
     void line3d(QPainter *painter, Point p1, Point p2);
     void line3dGL(Point p1, Point p2);
     void line2dGL(double x1, double y1, double x2, double y2, int transformtomodel = 0);
     double getpixelscale();
     void setRaytracing(int onoff) { israytracing = onoff; }
     int getRaytracing() { return israytracing; }
#endif
     void line3d(Point,Point);
     int coordinate3d(int,int,Point *,int fix = 1);
     int updatematrix(void);
     Point getxaxis(void);
     Point getyaxis(void);
     Point getuaxis(void);
     Point getvaxis(void);
     Point getwaxis(void);
     void gettransform(Transform *) const;
     void applytransform(const Transform &);
     void settransform(Transform &);
     double pick(Point,int,int,int);
     double pick(Point,Point,double,double,Point *);
     int pick(int,Point *,Point p1,Point p2,int,int *);
     int pick(Point,int,int,int,int,int);
     double pickhor(Point,Point,int,int,Point *,double *);
     void setdirty(Point,Point);
     void cleardirty(void)  {  dirty = 0;  }
     int getdirty(void)  {  return dirty;  }
     void setprinterhdc(RCHDC hdc) {  printerhdc = hdc;  }
     RCHDC getprinterhdc(void) {  return printerhdc;  }
     int requiresopengl(void);
  };

inline View *View3dSurface::getview(void) {  return view;  }
inline double Surface::getplotscale(void)  {  return plotscale;  }
inline void Surface::setplotscale(double s)  {  plotscale = s;  }
inline double View3dSurface::getdepth(void)  {  return depth;  }
inline void View3dSurface::setdepth(double d)  {  depth = d;  }
inline double View3dSurface::getumin(void)  {  return umin;  }
inline double View3dSurface::getumax(void)  {  return umax;  }
inline double View3dSurface::getvmin(void)  {  return vmin;  }
inline double View3dSurface::getvmax(void)  {  return vmax;  }
inline Point View3dSurface::getuaxis(void)  {  return uaxis;  }
inline Point View3dSurface::getvaxis(void)  {  return vaxis;  }
inline Point View3dSurface::getwaxis(void)  {  return waxis;  }

class View3dOffScreenBitmap;

class View3dSubSurface : public View3dSurface
{
#ifdef USING_WIDGETS
public:
    int ssidx; // the scene index for this ss
    //QGraphicsItem *qgi[4]; // these are the borders
    QGraphicsItem *qgi;      // this is a path item for the drawn borders
    QGraphicsItem *qgi2;     // this group is for the handles
    QGraphicsItem *qgicp;    // this is a path item for the clip borders
public:
    void clearScene();
    void drawborderGL(int mode);
    View3dSurface* getparent() { return parent; }
#endif
private:
   View3dSurface *parent;
   View3dOffScreenBitmap *view3doffscreenbitmap;
   OneView *oneview;
 public:
   View3dSubSurface(View3dSurface *,OneView *);
   ~View3dSubSurface();
   void paintevent(void);
   void sizeevent(int,int);
   void drawborder(int mode);
   void regenerate(void);
   int pick(int x,int y);
   OneView *getoneview(void)  {  return oneview;  }
   virtual int getsurfacetype(void)  {  return View3dSubsurf;  }

   double getpixelscale()
   {
       //return getpixelmodelmm();
       Point pix0 = screentomodel(Point(0.0,0.0,0.0));
       Point pix1 = screentomodel(Point(1.0,0.0,0.0));
       return (pix1 - pix0).length();
   }
};

class Printer : public virtual Surface
  {protected:
     int status,npens,selection;
     double printscale,physwidth,physheight,xoffset,yoffset,twidth,theight;
     RCPRINTDLG pd;
     DOCINFO di;
     int lineprinter,ncolumns,*columns,y,top,bottom,left,right;
     RCHFONT holdfont,hboldfont,hlargefont,hnormalfont,hsmallfont,hsmallboldfont;
   public:
     Printer(int lp = 0);
     ~Printer();
     void initialize(void);
     void getdefault(int portrait);
     void setupprinter(void);
     void firstpage(void);
     void lastpage(void);
     void print(int);
     void print(PRINTDLG *user_pd,DOCINFO *user_di);
     void abort(void);
     int getdefined(void) {  return status;  }
     int getselection(void)  {  return selection;  }
     double getprintscale(void)  {  return printscale;  }
     void setmargins(double,double,double,double);
     void setcolumns(int,double *);
     void starttable(int,int *);
     void endtable(void);
     void newline(int nlines = 1);
     void nearbottom(int);
     void newpage(void);
     void setbold(void);
     void setlarge(void);
     void setnormal(void);
     void setsmall(void);
     void setsmallbold(void);
     void setheadingone(void);
     void setheadingtwo(void);
     void printtext(RCCHAR *,int tabstop = 0,int just = 0);
     void printtext(char *,int tabstop = 0,int just = 0);
     void printtext(int x,int y,RCCHAR *);
     void barcode(int x,int y,int swidth,int sheight,RCCHAR *);
     void overline(int,int);
     int getattop(void);
#if defined(_WIN32_WCE)
     HDC gethdc(void)  {  return 0;  }
     double getxscale(void)  {  return 1.0;  }
     double getyscale(void)  {  return 1.0;  }
#else
     RCHDC gethdc(void)  {  return pd.hDC;  }
     double getxscale(void)  {  return double(GetDeviceCaps(pd.hDC,HORZRES)) / double(GetDeviceCaps(pd.hDC,HORZSIZE));  }
     double getyscale(void)  {  return double(GetDeviceCaps(pd.hDC,VERTRES)) / double(GetDeviceCaps(pd.hDC,VERTSIZE));  }
#endif
     double getx(void)  {  return left / getxscale();  }
     double gety(void)  {  return (top + y) / getyscale();  }
     double getphyswidth(void)  {  return physwidth;  }
     double getphysheight(void)  {  return physheight;  }
  };

class View3dPrinter : public Printer,public View3dSurface
  {public:
     View3dPrinter(View3dSurface *);
     View3dPrinter(View3dSurface *,double,double,double,double);
     int startprint(void);
     void endprint(double,double,double,double);
     void abortprint(void);
     double getphyswidthmm(void);
     double getphysheightmm(void);
  };

class View2dPrinter : public Printer,public View3dSurface
{private:
   View2d *view2d;
   View *top;
   GeneralList subsurfaces;
 public:
   View2dPrinter(View2d *,View *);
   ~View2dPrinter();
   void print(PRINTDLG *pd = 0,DOCINFO *di = 0);
   void printnext(View2d *);
};

// new View3ePrintet class for handling Qt printing
class RCView3dPrinter : public View3dSurface
{
private:
    QPrinter *qprinter;
    double printscale;
    int reverseBlackWhite;
    QImage backImage; // a backgound image used for the raytraced view

public:
    int fboWidth;
    int fboHeight;

public:
    RCView3dPrinter(View3dWindow *s, QPrinter *qprt);
    void render(QPainter *painter, double umin, double vmin, double umax, double vmax);
    ~RCView3dPrinter();
    int getsurfacetype(void)  {  return View3dSurf;  }
    void zoom(double scale); // zoom by
    void zoom(double u, double v); // move to
    void zoom(double u, double v, double scale ); // zoom to by scale
    void zoom(double u1,double v1,double u2,double v2); // zoom to window
    void setzoom(double umin1,double vmin1,double umax1,double vmax1);
    void fixzoom();
    void setprintscale(double scale) { printscale = scale; }
    double getprintscale() { return printscale; }
    double getphyswidthmm(void);
    double getphysheightmm(void);
    static void sceneDestroyed(QObject*);
    void debug(RCCHAR *filepath);
};

class RCView2dPrinter : public RCView3dPrinter
{
private:
   View2dWindow *view2dwindow;
   View2d *view2d;
   View *top;
   QPrinter *qprinter;
   double printscale;
public:
  GeneralList subsurfaces;
public:
   RCView2dPrinter(View3dWindow *s, QPrinter *qprt);
   ~RCView2dPrinter();
   //void clearScene();
   //void render(QPainter *painter, double umin, double vmin, double umax, double vmax);
   int getsurfacetype(void)  {  return View2dSurf;  }
   View2d* getvew2d();
   View* getview();
   View2dWindow* getview2dwindow();
};

class MetaFile : public virtual Surface
  {protected:
     int status;
     RCHENHMETAFILE hmf;
   public:
     MetaFile(RCCHAR *);
     ~MetaFile();
     void close(void);
     void copytoclipboard(void);
     int getdefined(void) {  return status;  }
  };

class View3dMetaFile : public MetaFile,public View3dSurface
  {public:
     View3dMetaFile(RCCHAR *,View3dSurface *);
     View3dMetaFile(RCCHAR *,View3dSurface *,double umin1,double vmin1,double umax1,double vmax1);
     void draw(void);
  };

class View2dMetaFile : public MetaFile,public View3dSurface
{private:
   View2d *view2d;
   View *top;
   GeneralList subsurfaces;
 public:
   View2dMetaFile(RCCHAR *,View2d *,View *);
   ~View2dMetaFile();
   void draw(void);
   void zoom(double umin1,double vmin1,double umax1,double vmax1);
};

class OffScreenBitmap : public virtual Surface
  {private:
     int status;
     RCHBITMAP hbitmapold;
   public:
     OffScreenBitmap(RCHDC,int,int);
     ~OffScreenBitmap();
     int getdefined(void) {  return status;  }
     int saveavistream(PAVISTREAM,int);
     RCHBITMAP getBitmap() { if(status) return hbitmapold; else return 0;}
  };

class View3dOffScreenBitmap : public OffScreenBitmap,public View3dSurface
  {public:
     View3dOffScreenBitmap(View3dSurface *,int pixelscale = 1);
     View3dOffScreenBitmap(View3dSurface *,int width,int height);
     void zoom(double u1,double v1,double u2,double v2)  {  umin = u1;  vmin = v1;  umax= u2;  vmax = v2;    updatematrix();  }
     void ChangeView(View3dSurface *);
     virtual void paintevent(void);

  };

//#ifdef USING_WIDGETS
class OffScreenQtSurface : public virtual Surface
{
private:
   int status;
public:
   OffScreenQtSurface(RCHDC,int,int);
   ~OffScreenQtSurface();
   int getdefined(void) {  return status;  }
};

class View3dOffScreenQtSurface : public OffScreenQtSurface, public View3dSurface
{
public:
    View3dOffScreenQtSurface(View3dSurface *,int pixelscale = 1);
    View3dOffScreenQtSurface(View3dSurface *,int width,int height);
    void zoom(double u1,double v1,double u2,double v2)
    {  umin = u1;  vmin = v1;  umax= u2;  vmax = v2;    updatematrix();  }
    void ChangeView(View3dSurface *);
    virtual void paintevent(void);
};

class View2dOffScreenQtSurface : public View3dOffScreenQtSurface
{
private:
    View2d *view2d;
public:
    View2dOffScreenQtSurface(View3dSurface *vw,View2d *v2d,int pixelscale = 1);
    View2dOffScreenQtSurface(View3dSurface *vw,View2d *v2d,int width,int height);
    void paintevent(void);
};

class View3dQtOutput : public View3dSurface
{
    QPrinter *mPrinter;
    QPainter *mPainter;
public:
     View3dQtOutput(QPrinter *printer,QPainter *painter,View *v,double scale);
     void zoom(double u1,double v1,double u2,double v2)
     { umin = u1; vmin = v1; umax= u2; vmax = v2; updatematrix(); }
     void paintevent(void);
};

class View2dQtOutput : public View3dSurface
{
private:
     View2d *view2d;
     View *view;
public:
     View2dQtOutput(RCHWND,RCHDC,View2d *,View*);
     void zoom(double u1,double v1,double u2,double v2)
     { umin = u1; vmin = v1; umax= u2; vmax = v2; updatematrix(); }
     void paintevent(void);
};
//#endif

class Window : public virtual Surface
  {
//#ifdef USING_WIDGETS
public:
    //QWidget* getWidget() { return (QWidget*)hWnd; }
    //void setWidet(QWidget *qw) { hWnd = qw; }
//#endif
  private:
     ControlList controllist;
   public:
     Window(char *,RCHWND,RCDWORD,int,int,int,int,int,RCPVOID params=NULL);
     Window(RCCHAR *,RCHWND,RCDWORD,int,int,int,int,int, RCPVOID params=NULL);
     virtual ~Window();
     int created()  {  return gethwnd() != NULL;  }
     void newmenu(HMENU);
     int addcontrol(Control *control,int draw = 0);
     void delcontrol(Control *control,int draw = 0);
     Control *getcontrol(int i);
     void title(const char *title);
     void title(const RCCHAR *title1,const RCCHAR *title2,const RCCHAR *title3);
     void title(const RCCHAR *title);
     virtual void mousemoveevent(int,int,int);
     virtual void mousewheelevent(int) {};
     virtual void mousewheelevent(int,int,int) {};
     virtual void lbuttondownevent(int,int,int);
     virtual void lbuttonupevent(int,int,int);
     virtual void mbuttonupevent(int,int,int);
     virtual void mbuttondownevent(int,int,int) {};
     virtual void mbuttondblclkevent(int,int,int) {};
     virtual void nclbuttondblclk(int,int,int) {};
     virtual void paintevent(void);
     virtual void sizeevent(int w,int h);
     virtual void charevent(UINT,DWORD) { }
     virtual void lbuttondblclkevent(int,int,int) {}
     virtual void rbuttondblclkevent(int,int,int) {};
     virtual void getminmaxinfoevent(MINMAXINFO *) {}
     virtual int getcanscroll(void) {  return 0;  }
     virtual void vscrollevent(int,int) {};
     virtual void hscrollevent(int,int) {};
     virtual void timerevent(void) {};

     // Qt functions
protected:
     void resizeEvent(QResizeEvent * event);
     void paintEvent(QPaintEvent * event);
  };

class Entity;

enum IconMenus { BM_MAINCOMMAND,BM_SUBCOMMAND,BM_MAININTERRUPT,BM_SUBINTERRUPT,BM_QUICK,BM_MAINAPPLICATION,BM_SUBAPPLICATION  };

typedef void (*DrawCallback)(int,void *,View3dSurface *);

class MenuWindow : public Window
  {public:
     MenuWindow(Window *,int,int,int,int);
     ~MenuWindow();
     void paintevent(void);
     void timerevent(void);
     void nclbuttondblclk(int,int,int);
     ButtonMenu *buttonmenu;
  };

class AnimationActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     AnimationActive() {  active = 0;  buttonmenu = NULL;  }
     ~AnimationActive()  {  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class AnimationMenuWindow : public MenuWindow
  {private:
     AnimationActive *animationactive;
   public:
     AnimationMenuWindow(Window *,int,int,int,int,AnimationActive *);
     ~AnimationMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

class ComposeActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     ComposeActive() {  active = 0;  buttonmenu = NULL;  }
     ~ComposeActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class ComposeMenuWindow : public MenuWindow
  {private:
     ComposeActive *composeactive;
   public:
     ComposeMenuWindow(Window *,int,int,int,int,ComposeActive *);
     ~ComposeMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };


class SimulationActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     SimulationActive() {  active = 0;  buttonmenu = NULL;  }
     ~SimulationActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class SimulationMenuWindow : public MenuWindow
  {private:
     SimulationActive *simulationactive;
   public:
     SimulationMenuWindow(Window *,int,int,int,int,SimulationActive *);
     ~SimulationMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

class ArchitectActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     ArchitectActive() {  active = 0;  buttonmenu = NULL;  }
     ~ArchitectActive()  {  }
     int getactive(void)  {  return active;  }
     void setactive(int a);
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class ArchitectMenuWindow : public MenuWindow
  {private:
     ArchitectActive *architectactive;
   public:
     ArchitectMenuWindow(Window *,int,int,int,int,ArchitectActive *);
     ~ArchitectMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };


class CustomMenuWindow;

class CustomMenuActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
     //CustomMenuWindow *custommenuwindow;
     RCToolBox *custommenuwindow;
   public:
     CustomMenuActive() {  active = 0;  buttonmenu = NULL;  custommenuwindow = 0;  }
     ~CustomMenuActive()  {  }
     int getactive(void)  {  return active;  }
     void setactive(int a);
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     //void setwindow(CustomMenuWindow *w)  {  custommenuwindow = w;  }
     void setwindow(RCToolBox *w)  {  custommenuwindow = w;  }
     RCToolBox * getwindow()  {  return custommenuwindow;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
     void fixup(void);
  };

class CustomMenuWindow : public MenuWindow
  {private:
     CustomMenuActive *custommenuactive;
   public:
     CustomMenuWindow(Window *,int,int,int,int,CustomMenuActive *);
     ~CustomMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };


class NCActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     NCActive() {  active = 0;  buttonmenu = NULL;  }
     ~NCActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class NCMenuWindow : public MenuWindow
  {private:
     NCActive *ncactive;
   public:
     NCMenuWindow(Window *,int,int,int,int,NCActive *);
     ~NCMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

#if VERSION != TRICAD_STUDENT
class VideoWindow : public Window
  {private:
     ButtonMenu *menu;
     RECT vidrect;
     int deviceid,y;
   public:
     VideoWindow(Window *,RCCHAR *filename,RCCHAR *title);
     ~VideoWindow();
     void paintevent(void);
     void play(void);
     void stop(void);
     void pause(void);
     void reverse(void);
     void stepforward(void);
     void stepreverse(void);
     void timerevent(void);
  };
#endif

enum {  PaintNone, PaintLine, PaintWindow  };

enum VariantType {  NoValue,IntegerValue,RealValue,StringValue,PointValue,PointEValue,LineValue,CircleValue,DialogValue  };

class Variant
  {private:
     RCCHAR name[32];
     int type;
     union {long i;
            double x,y,z;
            RCCHAR *s;
            void *data;
           };
     void deletedata(void);
   public:
     Variant() {  type = NoValue;  strcpy(name,"");  }
     ~Variant();
     int gettype() {  return type;  }

     void setname(RCCHAR *);
     int match(RCCHAR *);

     void setvalue(RCCHAR *);
     void setvalue(double x);
     void setvalue(double x,double y,double z);
     void setvalue(long i);
     void setvalue(int,void *);

     int getaddress(RCCHAR **);
     int getaddress(double **x);
     int getaddress(long **i);

     int getvalue(RCCHAR **);
     int getvalue(double *x);
     int getvalue(double *x,double *y,double *z);
     int getvalue(long *i);
     int getvalue(int,void **);

     RCCHAR *format(RCCHAR *,int f1,int f2);
     int read(RCCHAR *,RCCHAR **);

  };

class RTSymbols
  {private:
     GeneralList list;
     int workplanecoordinates;

   public:

     RTSymbols() {  workplanecoordinates = 0;  }

     Variant *match(RCCHAR *);

     void clear(void);

     void setvalue(RCCHAR *,RCCHAR *);
     void setvalue(RCCHAR *,double x);
     void setvalue(RCCHAR *,double x,double y,double z);
     void setvalue(RCCHAR *,long i);
     void setvalue(RCCHAR *,int,void *);

     int getvalue(RCCHAR *,RCCHAR **);
     int getvalue(RCCHAR *,double *x);
     int getvalue(RCCHAR *,double *x,double *y,double *z);
     int getvalue(RCCHAR *,long *i);
     int getvalue(RCCHAR *,int,void **);

     int getworkplanecoordinates(void)  {  return workplanecoordinates;  }

  };


#if VERSION != TRICAD_STUDENT

#define MAX_NEST 100

class CadBasicProgram
  {private:
     RCCHAR filename[300],title[300];
     RCCHAR **line;
     RCCHAR *breakpoint;
     long nlines,maxlines,inclines,currentlineno;
     int changed;

     int pos;
     SymbolType stype;
     RCCHAR sname[256];
     RCCHAR svalue[300];
     double xvalue;
     long ivalue;
     Point pvalue;
     void nextsymbol(void);
     int factor(Variant *);
     int term(Variant *);
     int expression(Variant *);
     int lfactor(Variant *);
     int lterm(Variant *);
     int lexpression(Variant *);
     int function(Variant *);

     int selectlevel,iflevel,whilelevel,forlevel,calllevel;
     struct {long result;} ifstack[MAX_NEST];
     struct {long lineno;  int type;  double xstep,xlast;  long istep,ilast;  RCCHAR name[32];  } forstack[MAX_NEST];
     struct {long lineno;  } whilestack[MAX_NEST];
     struct {long lineno;  } callstack[MAX_NEST];
   public:
     CadBasicProgram();

     RTSymbols rtsymbols;

     int filenew(RCHWND hwnd);
     int fileopen(RCHWND hwnd);
     int filesave(RCHWND hwnd);
     int filesaveas(RCHWND hwnd);
     int filechanged(RCHWND hwnd);
     void add(long,RCCHAR *);
     void del(RCCHAR *);
     int insertchar(long *x,long *y,RCCHAR c);
     int deletechar(long *x,long *y,int moveright,RCCHAR c);
     int cut(long x1,long y1,long x2,long y2);
     int paste(long x1,long y1,RCCHAR *buffer);
     int run(int);
     long getlineno(void)  {  return currentlineno;  }
     long getnlines(void)  {  return nlines;  }
     int getbreakpoint(long lineno);
     void setbreakpoint(long lineno,int set);
     RCCHAR *getline(long i)  {  return i < nlines ? line[i] : 0;  }
     RCCHAR *gettitle(void)  {  return title;  }
     void setchanged(int c)  {  changed = c;  }
     void evaluate(Window *);
     void evaluate(RCCHAR *line,RCCHAR *result);
  };

class CadBasicEditorWindow : public Window
  {private:
     RCHFONT hfont,holdfont;
     CadBasicProgram *cbp;
     int cheight,cwidth,selectiondrawn,captured;
     long x1,y1,x2,y2,xbase,ybase;
   public:
     CadBasicEditorWindow(Window *window,int x,int y,CadBasicProgram *bp,long lineno);
     ~CadBasicEditorWindow();
     void mousemoveevent(int x,int y,int);
     void lbuttonupevent(int x,int y,int);
     void paintevent(void);
     void charevent(UINT,DWORD);
     void vscrollevent(int,int);
     void paintselection(int clear);
     int deleteselection(void);
     void copyselection(void);
     void paste(void);
     void paintline(long y);
     void highlight(long line);
     void lbuttondownevent(int x,int y,int keys);
     void setcurrentline(long l);
     CadBasicProgram *getcbp(void)  {  return cbp;  }
     void run(void);
     void step(void);
     void pause(void);
     void stop(void);
  };

class CadBasicWindow : public Window
  {private:
     ButtonMenu *menu;
     CadBasicProgram *cbp;
     CadBasicEditorWindow *cbew;
     int running;
   public:
     CadBasicWindow(Window *window,CadBasicProgram *bp,long lineno);
     ~CadBasicWindow();
     Window *getcbew(void)  {  return cbew;  }
     void paintevent(void);
     void charevent(UINT,DWORD);
     CadBasicProgram *getcbp(void)  {  return cbp;  }
     void sizeevent(int,int);
     void run(void);
     void step(void);
     void pause(void);
     void stop(void);
     int getrunning(void)  {  return running;  }
     void timerevent(void) {  if (menu != 0) menu->timerevent();  };
  };


class CommandWindow : public Window
  {private:
     long maxlines,maxlength,nlines;
     RCCHAR **line;
     RCHFONT hfont,holdfont;
     int cheight,cwidth,selectiondrawn,captured;
     long x1,y1,x2,y2,xbase,ybase,lineno;
     int oldx,oldy,oldw,oldh;
   public:
     CommandWindow(Window *window,int x,int y);
     ~CommandWindow();
     void mousemoveevent(int x,int y,int);
     void lbuttonupevent(int x,int y,int);
     void paintevent(void);
     void charevent(UINT,DWORD);
     void vscrollevent(int,int);
     void paintselection(int clear);
     int deleteselection(void);
     void copyselection(void);
     void paste(void);
     void paintline(long y);
     void highlight(long line);
     void lbuttondownevent(int x,int y,int keys);
     void lbuttondblclkevent(int,int,int);
     void setcurrentline(long l);
     void insert(char *);
  };

#endif

class LayerWindow : public Window
  {private:
     RCHFONT hfont,holdfont;
     int cheight,cwidth;
     long ybase;
     int selectall,displayall;
     int oldx,oldy,oldw,oldh;
   public:
     LayerWindow(Window *window,int x,int y);
     ~LayerWindow();
     void mousemoveevent(int x,int y,int);
     void lbuttonupevent(int x,int y,int);
     void paintevent(void);
     void paintline(int clear);
     void vscrollevent(int,int);
     void lbuttondownevent(int x,int y,int keys);
     void lbuttondblclkevent(int,int,int);
  };

class SubWindow : public Window
  {private:
     int clickcommandid;
   public:
     SubWindow(RCCHAR *name,RCHWND hwnd,RCDWORD style,int x,int y,int w,int h);
     void paintevent(void);
     void setclickcommandid(int id);
     void lbuttondownevent(int,int,int);
  };

class View2dWindow;

class PrintPreviewWindow : public Window
  {private:
     View3dWindow *view3dwindow;
     View2dWindow *view2dwindow;
     double umin,vmin,umax,vmax,printscale;
     RCHWND paperhwnd; // RCPrintPreview_Dialog
     RCHDC paperhdc; // current scene?
   public:
     Printer printer;
     PrintPreviewWindow(RCCHAR *name,RCHWND hwnd,RCDWORD style,int x,int y,int w,int h);
     ~PrintPreviewWindow();
     void setview3dwindow(View3dWindow *);
     void setview2dwindow(View2dWindow *);
     void setzoom(double,double,double,double);
     void paintevent(void);
     void lbuttondownevent(int,int,int);
     double getprintscale(void)  {  return printscale;  }
     void setprintscale(double ps) {  printscale = ps;  }
     void print(void);
  };

#define MAX_OPENLIST 9

class OpenList
{
#ifdef USING_WIDGETS
    friend class QtMainWindow;
#endif
   private:
     Window *window;
     int n;
     RCCHAR names[MAX_OPENLIST][300];
     RCCHAR titles[MAX_OPENLIST][300];
   public:
     OpenList();
     void setwindow(Window *);
     void opendatabase(int);
     void openeddatabase(const RCCHAR *name,const RCCHAR *title);
};

class MainWindow;

#define MAX_DOWNPOINTS 200

static int v3dcount=0;
class View3dWindow : public Window, public View3dSurface
{
#ifdef USING_WIDGETS
public:
    int mcount;
    QGraphicsItem *qgich; // this is for the non windows cursor
    QGraphicsItem *qgi; // this is for the magic eraser con trail
    QList<QGraphicsItem *> qgList;
    void clearScene();
#endif
protected:
   MainWindow *parent;
   int buttondown,downx,downy,scrolling,offscreenpaint,showcommand,wheeldirection;
   Entity *downentity, *lastEntitySelected;
   BitMask visible;
   Cursor *cursor;
   Point downpoint[MAX_DOWNPOINTS];
   int ndownpoints,downcommand;
   DWORD downtime;
   View3dOffScreenBitmap *offscreenbitmap;
 public:
   View3dWindow(MainWindow *,Window *,View *view,int,int,int,int,DWORD wintype=(RCSCENEVW | RCGLWINTYPE | RCV3DWINTYPE /*| RCWINTYPE*/),RCPVOID=NULL);
   View3dWindow(MainWindow *,Window *,View *view,int,int,int,int,double,double,double,double,DWORD wintype=(RCSCENEVW | RCGLWINTYPE | RCV3DWINTYPE /* | RCWINTYPE*/),RCPVOID=NULL);
   ~View3dWindow();
   int getcanscroll(void) {  return 1;  }
   void fixzoom(void);
   void rubberband3d(Point);
   void rubberbanduv(Point);
   void rubberbandGL();
   void cursorerase(void) { cursor->erase(); }
   void cursoroff(void)  {  cursor->off();  }
   void cursoron(void)  {  cursor->on();  }
   void cursordrawGL() { cursor->drawGL(this); }
   int  cursorisdrawing() { return cursor->isdrawing(); }
   void cursormousemoveevent(int x,int y,int b)  {  cursor->mousemoveevent(x,y,b);  }
   void cursorrubberband(int rx,int ry)   {  cursor->rubberband(rx,ry);  }
   void cursoraction(int);
   int getcursoraction(){ return cursor->getaction(); }
   void cursorstyle(int);
   void drawCursor() { if(cursor != 0) cursor->drawCursor();}
   void setoffscreenpaint(int osp) {  offscreenpaint = osp;  }
   void getminmaxinfoevent(MINMAXINFO *);
   void setwheeldirection(int direction) { wheeldirection = direction; } // 1 = normal;  -1 = reversed
   int  getwheeldirection() { return wheeldirection; }
   View3dOffScreenBitmap *getoffscreenbitmap(void) {  return offscreenbitmap;  }
   void UpdateWindow(void);
   virtual BitMask &getvisibility(void);
   virtual void zoom(double,double,double,double);
   virtual void mousemoveevent(int,int,int);
   virtual void mousewheelevent(int);
   virtual void mousewheelevent(int,int,int);
   virtual void lbuttondownevent(int,int,int);
   virtual void lbuttonupevent(int,int,int);
   virtual void mbuttondblclkevent(int,int,int);
   virtual void mbuttondownevent(int,int,int);
   virtual void mbuttonupevent(int,int,int);
   virtual void paintevent();
   virtual void lbuttondblclkevent(int,int,int);
   virtual void sizeevent(int,int);
   virtual void vscrollevent(int,int);
   virtual void hscrollevent(int,int);
   virtual int save(CadFile *);
   static View3dWindow *load(CadFile *);
   virtual int save(UndoFile *,int);
   virtual int load(UndoFile *);
#if ! defined(_WIN32_WCE)
   int saveavistream(PAVISTREAM,int);
#endif
   void setshowcommand(int sc)  {  showcommand = sc;  }

   // KMJ
   int getscrolling() { return scrolling; }

   //LGM
   bool SwapCurrentSelectedEntity();

   Entity* getlastselectedentity() {return lastEntitySelected;}
};

class View2dWindow : public View3dWindow
{
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi;  // this group is for the border
    QGraphicsItem *qgi2; // this group is for the boundaries
public:
    void clearScene();
    double getlastumin() { return lastumin; }
    double getlastumax() { return lastumax; }
    double getlastvmin() { return lastvmin; }
    double getlastvmax() { return lastvmax; }
    void drawentGL(int drawmode,Transform *t,Entity *e);
#endif
private:
   View2d *view2d;
   View *top;
   //GeneralList subsurfaces;
   OneView *oneview;
   int drawn;
   double lastumin,lastvmin,lastumax,lastvmax;
 public:
   void drawboundary(double, double, double, double, int drawmode=DM_NORMAL);
   GeneralList subsurfaces;
   View2dWindow(MainWindow *,Window *,View2d *,View *,int,int,DWORD wintype=(RCSCENEVW | RCGLWINTYPE | RCV2DWINTYPE /*| RCWINTYPE*/), RCPVOID=NULL);
   View2dWindow(MainWindow *, Window *, View2d *, View *, int, int, int, int, double, double, double, double, DWORD wintype=(RCSCENEVW | RCGLWINTYPE | RCV2DWINTYPE /*| RCWINTYPE*/), RCPVOID=NULL);
   int getsurfacetype(void)  {  return View2dSurf;  }
   void drawent(int,Transform *,Entity *);
   void regenerate(void);
   void zoom(double,double,double,double);
   void mousemoveevent(int,int,int);
   void lbuttondownevent(int,int,int);
   void lbuttonupevent(int,int,int);
   void paintevent();
   void lbuttondblclkevent(int,int,int);
   virtual void vscrollevent(int,int);
   virtual void hscrollevent(int,int);
   void sizeevent(int,int);
   void update(int clear = 0);
   View2d *getview2d(void)  {  return view2d;  }
   BitMask &getvisibility(void);
   int save(CadFile *);
   static View2dWindow *load(CadFile *);
   int save(UndoFile *,int);
   int load(UndoFile *);
   View3dSubSurface *pick(Point p);
   int requiresopengl(void);
};

class View2dOffScreenBitmap : public View3dOffScreenBitmap
  {private:
     View2d *view2d;
   public:
     View2dOffScreenBitmap(View3dSurface *vw,View2d *v2d,int pixelscale = 1) : View3dOffScreenBitmap(vw,pixelscale) {  view2d = v2d;  plotscale = 1.0;  };
     View2dOffScreenBitmap(View3dSurface *vw,View2d *v2d,int width,int height) : View3dOffScreenBitmap(vw,width,height) {  view2d = v2d;  plotscale = 1.0;  };
     void paintevent(void);
  };

#define MenuBorderOffset 4
#define MenuHandleOffset 4

class MainWindow : public Window
{
#ifdef USING_WIDGETS
    friend class QtMainWindow;
public:
    QIcon appIcon;
    QGraphicsScene * getScene()
    {
        if(getcurrentwindow()->getScene())
            return getcurrentwindow()->getScene();
        else
            return 0;
    }
    QTimer *qtimer;
#endif
private:
   friend class View3dWindow;
   friend class View2dWindow;
   friend class QMainWindow;
   BitMask options;
   UINT timer;
   int dragviewboundary;
   MenuWindow *floatingmenu1;
   MenuWindow *floatingmenu2;
   MenuWindow *floatingmenu3;
   MenuWindow *floatingmenu4;

   //  Current menus
   ButtonMenu *maincmenu;
   ButtonMenu *subcmenu;
   ButtonMenu *mainimenu;
   ButtonMenu *subimenu;
   ButtonMenu *mainamenu;
   ButtonMenu *subamenu;
   ButtonMenu *lastmainamenu;
   ButtonMenu *lastsubamenu;
   ButtonMenu *quickmenu;

   //  Current menu positions
   int maincmenux,maincmenuy,subcmenux,subcmenuy;
   int mainimenux,mainimenuy,subimenux,subimenuy;
   int mainamenux,mainamenuy,subamenux,subamenuy;
   int quickmenux,quickmenuy;
   int coordinateentryx,coordinateentryy;
   int styleentryx,styleentryy;
   int coordinatedisplayx,coordinatedisplayy;
   int promptx,prompty;
   int topmenuareaheight,bottommenuareaheight;
   int leftmenuareawidth,rightmenuareawidth,draw,batch;

   View3dWindow *curview3d;
#if VERSION != TRICAD_STUDENT
   CadBasicWindow *curcadbasic;
   CommandWindow *curcommand,*commandwindow;
   LayerWindow *layerwindow;
   GeneralList cadbasiclist;
   RCCHAR applicationname[300];

#endif
   View3dWindowList view3dlist;
   static int iconheight,coordheight,promptheight,displayheight;
   static double fontsize;
   static RCHFONT hsfont;

   RCHBITMAP hbitmapold;
   RCHDC hoffscreenhdc;

   int offscreenbitmapused,offscreenbitmapwidth,offscreenbitmapheight;
   View3dOffScreenBitmap *offscreenbitmap;

 public:
   SubWindow  *subwindow;
   PrintPreviewWindow *printpreviewwindow;
   Prompt *prompt;
   Prompt *statusprompt;
   GeneralList floatingmenulist;
   StyleEntry *styleentry;
   CoordinateEntry *coordinateentry;
   CoordinateDisplay *coordinatedisplay;
   ViewTab *viewtab;
   ViewProperties *viewproperties;
   OpenList openlist;
   //MainWindow() { }
   MainWindow(int,int,int,int,int );
   ~MainWindow();

   int getDrawingBatch() { return batch; };
   View3dOffScreenBitmap *GetOffScreenBitmap(View3dSurface *vs);
   void ReleaseOffScreenBitmap(View3dOffScreenBitmap *);

   void update_properties(int default_group_state);

   int MessageBox(const RCCHAR *,const RCCHAR *,int);
   int MessageBox(const RCCHAR *,const char *,int);
   int MessageBox(const char *,const char *,int);

   int getdragviewboundary(void)  {  return dragviewboundary;  }
   void setdragviewboundary(int d)  {  dragviewboundary = d;  }
#ifdef USING_WIDGETS
   void buttonmenu(int,ButtonMenu *menu,ButtonMenu *mainMenu=0);
#else
   void buttonmenu(int,ButtonMenu *menu);
#endif
   void selectwindow(Window *);
   void movetoback(Window *);
   void drawent(int,Entity *);
   void movemenu(int);
   void movemenu(ButtonMenu *,int,int);
   void moveprompt(int state = -1);
   int getshiftstate(void);
   int getcontrolstate(void);
#ifdef USING_WIDGETS
   void drawent(int,Transform *,Entity *,QPainter *);
   void drawentGL(int drawmode,Transform *t,Entity *entity );
   void clearAllRubberBands();
   void clearAllViews();
   void updateAllViews();
#endif
   void drawent(int,Transform *,Entity *);
   void invalidatedisplaylist(Entity *);
   void drawcallback(int,void *,DrawCallback);
   void rubberband3d(Point);
   void rubberbanduv(Point);
   void cursoroff(void);
   void cursoron(void);
   void cursoraction(int);
   void cursorstyle(int);
   View3dWindowList *GetView3dList(void)  {  return &view3dlist;  }
   void paintall(int resetdefaultmode = 0,int offscreen = 0);
   void setdirtyall(Point,Point);
   void cleardirtyall(void);
   void destroyall(void);
   void create3dsubwindow(View *,int,int,int,int);
   void create3dsubwindow(View *,int x = 0,int y = 0);
   void create2dsubwindow(View2d *,int,int);
   void create2dsubwindow(View2d *);
   void addfloatingmenuwindow(MenuWindow *mw);
   void delfloatingmenuwindow(MenuWindow *mw);
   int IsVisible(Control *);
   RCHDC getoffscreenhdc(void)  {  return hoffscreenhdc;  }
   SubWindow *getsubwindow(void)  {  return subwindow;  }
   int issubwindow(View3dWindow *);
   View3dSurface *getcurrentsurface(void)  {  return curview3d;  }
   View3dWindow *getcurrentwindow(void)  {  return curview3d;  }
   void setcurrentwindow(View3dWindow *cw);
   void setcurrentsurface(View3dSurface *vs);
   View3dWindow *matchview3dwindow(RCHWND hwnd);
#if VERSION != TRICAD_STUDENT
   CadBasicWindow *getcadbasicwindow(void)  {  return curcadbasic;  }
   CommandWindow *getcommandwindow(void)  {  return curcommand;  }
   CommandWindow *getthecommandwindow(void)  {  return commandwindow;  }
   void addcadbasicwindow(CadBasicWindow *cbw);
   void delcadbasicwindow(CadBasicWindow *cbw);
   void setthecommandwindow(CommandWindow *cw)  {  commandwindow = cw;  }
   void setcommandwindow(CommandWindow *cw)  {  curcommand = cw;  }
   void setlayerwindow(LayerWindow *lw)  {  layerwindow = lw;  }
   LayerWindow *getlayerwindow(void) {  return layerwindow;  }
#endif
   void updatewindows(View *);
   void deletewindows(View2d *);
   void updatewindows(View2d *,int clear = 0);
   void updatetitle(void);
   void updatemenu(void);
   void updatemenus(int);
   void resetbuttonmenus();
   int viewused(int);
   void setdepth(void);
   void tile(void);
   void tab(void);
   void cascade(void);
   void deletesubwindow(View3dWindow *);
   void deletefloatingmenu(MenuWindow *);
   void showfloatingmenu(int);
   void paintevent(void);
   void timerevent(void);
   void sizeevent(int,int);
   void charevent(UINT,DWORD);
   int save(CadFile *);
   ButtonMenu *GetMainCADMenu(void) {  return maincmenu;  }
   void SetMainCADMenu(ButtonMenu *bm) { maincmenu = bm; }
   ButtonMenu *GetSubCADMenu(void) {  return subcmenu;  }
   ButtonMenu *GetInterruptCADMenu(void) {  return mainimenu;  }
   void SetInterruptCADMenu(ButtonMenu *bm) { mainimenu = bm; }
   ButtonMenu *GetApplicationCADMenu(void) {  return mainamenu;  }
   ButtonMenu *GetQuickCADMenu(void) {  return quickmenu;  }
   void SetQuickCADMenu(ButtonMenu *bm) { quickmenu = bm; }
   void seticon(RCHICON icon,RCCHAR *);
   void enabledraw(void)  {  draw = 0;  }
   void disabledraw(void)  {  draw = 1;  }
   void startdrawingbatch(void);
   void enddrawingbatch(void);
   static int load(CadFile *,Database *);
   static int loadsplitter(CadFile *,Database *);
   static RCHFONT gethsfont(void)  {  return hsfont;  }
   static int geticonheight(void)  {  return iconheight;  }
   static int getcoordheight(void)  {  return coordheight;  }
   static int getdisplayheight(void)  {  return displayheight;  }
   static int getpromptheight(void)  {  return promptheight;  }
   static void setfontsize(double f)  {  fontsize = f;  }

   // functions called from Qt Widgets
public:
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent * event);
    int addcontrol(Control *control,int draw = 0)
    {
       return Window::addcontrol(control,draw);
    }
    void updatemousedirection(int direction);
    void setantialiasing(int onoff);
    void replace3dsubwindow(View3dWindow *currentWindow, View * newView);
    void freezeBackImage();
    void unFreezeBackImage();
    // start up dialog
    void showSplashScreen();
    void hideSplashScreen();
    void deleteSpashScreen();
    void setRefreshDelay(int);
    void setReduceDetail(int);
    bool isMinimized();
};

class View3dOutput : public View3dSurface
  {public:
     View3dOutput(RCHWND,RCHDC,View *,double);
     void setfbclip(double a, double b);
     void zoom(double u1,double v1,double u2,double v2)  {  umin = u1;  vmin = v1;  umax= u2;  vmax = v2;    updatematrix();  }
     void paintevent(void);
  };

//class View3dBitmapOutput : public View3dOutput
class View3dBitmapOutput : public View3dSurface
{
public:
    QPixmap *mbitmap;
    int status;
public:
    View3dBitmapOutput(RCHWND,RCHDC,View *,double);
    View3dBitmapOutput(View *,double, double w, double h);
    void setWidth(double width);
    void setHeight(double height);
    void setSize(double width, double height);
    void fixzoom();
    void zoom(double u1,double v1,double u2,double v2);
    int getStatus() { return status; }
    RCHBITMAP * getBitmap();
    void paintevent(void);
    void render();
};

class View3dGLOutput : public View3dSurface
  {public:
     View3dGLOutput(RCHWND,RCHDC,View *,double);
     void zoom(double u1,double v1,double u2,double v2)  {  umin = u1;  vmin = v1;  umax= u2;  vmax = v2;    updatematrix();  }
     void drawbegin();
     void draw(Entity *e, Transform *t=0, int drawmode=DM_NORMAL);
     void drawend();
     void paintevent(void);
  };

class View2dOutput : public View3dSurface
  {private:
     View2d *view2d;
     View *view;
   public:
     View2dOutput(RCHWND,RCHDC,View2d *,View*);
     void zoom(double u1,double v1,double u2,double v2)  {  umin = u1;  vmin = v1;  umax= u2;  vmax = v2;    updatematrix();  }
     void paintevent(void);
  };

class Home
  {private:
     RCCHAR publicdirectory[300],publicfilename[600],executabledirectory[600],executablefilename[600],tempfilename[600],inifilename[300],licensefilename[300];
     RCCHAR applicationTitle[300],executableDir[600],executablePath[600];
   public:
     Home();
     Home(bool);
     ~Home();
     RCCHAR *getinifilename(void);
     RCCHAR *getlicensefilename(void);
     RCCHAR *getpublicfilename(RCCHAR *);
     RCCHAR *getpublicfilename(const char *);
     RCCHAR *getexecutablefilename(RCCHAR *);
     RCCHAR *gettempfilename(RCCHAR *);
     RCCHAR *getApplicationTitle();
     RCCHAR *getApplicationDir();
     RCCHAR *getApplicationPath();
     RCCHAR *getFigureFilepath(RCCHAR *extra);
  };

class CommandFile
  {private:
     FILE *infile;
     RCCHAR commandline[300];
     int lineavailable,lineno,xpos,ypos,zpos,ncolumns;
     double xscale,yscale,zscale;
   public:
     CommandFile();
     ~CommandFile();
     int select(void);
     void close(void);
     void send(RCCHAR *);
     int getnextstring(RCCHAR *);
     HighLevelEvent *gne(void);
     int gnd(int *,RCCHAR *);
     HighLevelEvent *getnextevent(void)
       {  if (infile == 0 && ! lineavailable)  return 0;
          return gne();
       }
     int getnextdialog(int *id,RCCHAR *value)
       {  if (infile == 0 && ! lineavailable)  return 0;
          return gnd(id,value);
       }
  };

class MainMenu
  {private:
     RCHMENU heditmenu,htransformmenu,harrangemenu;
     UINT cadformat;
   public:
     MainMenu()  {  heditmenu = htransformmenu = harrangemenu = 0;  cadformat = RCRegisterClipboardFormat("ISCad3d");  }
     void selectionchanged(int l);
     void pasteredoundo(HWND);
  };

#define IGES_LINE_LENGTH 83

class IGESRecord;

class IGESIn
  {private:
     FILE *infile;
     int ll;
     char parameterdelim[2];
     char recorddelim[2];
     char productidout[STRING_LENGTH];
     char filename[STRING_LENGTH];
     char systemid[STRING_LENGTH];
     char version[STRING_LENGTH];
     long bits,sprange,spprecision,dprange,dpprecision;
     char productidin[STRING_LENGTH];
     double modelscale;
     long units;
     char sunits[STRING_LENGTH];
     long nwidths;
     double maxwidth;
     char datetime[STRING_LENGTH];
     double granularity,size;
     char author[STRING_LENGTH];
     char organization[STRING_LENGTH];
     long igesversion,standard;
     EntityHeader h;
   public:
     IGESIn()  {  infile = NULL;  ll = 0;  }
     ~IGESIn();
     int open(RCCHAR *s);
     void close(void);
     long ftell(void);
     void fseek(const long offset);
     void fwrite(const char *);
     int getlinelength(void)  {  return ll;  }
     IGESRecord *getnextrecord(void);
     void processglobalsection(IGESRecord **);
     void processtext(long,long,long);
     void processline(long,long,long);
     void processarc(long,long,long,long);
     void processellipse(long,long,long,long);
     void processpoint(long,long,long,long);
     void processbspline(long,long,long,long);
     void processleader(long,long,long,long);
     void processmatrix(long,long,long,double *);
     void read(int usecolours);

     char getparameterdelim(void) {  return parameterdelim[0];  }
     char getrecorddelim(void)  {   return recorddelim[0];  }
     char *getproductidout(void)  {  return productidout;  }
     char *getfilename(void)  {  return filename;  }
     char *getsystemid(void)  {  return systemid;  }
     char *getversion(void)  {  return version;  }
     long getbits(void)  {  return bits;  }
     long getsprange(void)  {  return sprange;  }
     long getspprecision(void)  {  return spprecision;  }
     long getdprange(void)  {  return dprange;  }
     long getdpprecision(void)  {  return dpprecision;  }
     char *getproductidin(void)  {  return productidin;  }
     double getmodelscale(void)  {  return modelscale;  }
     long getunits(void)  {  return units;  }
     char *getsunits(void)  {  return sunits;  }
     long getnwidths(void)  {  return nwidths;  }
     double getmaxwidth(void)  {  return maxwidth;  }
     char *getdatetime(void)  {  return datetime;  }
     double getgranularity(void)  {  return granularity;  }
     char *getauthor(void)  {  return author;  }
     char *getorganization(void)  {  return organization;  }
     long getigesversion(void)  {  return igesversion;  }
     long getstandard(void)  {  return standard;  }
  };

class HPGLIn
  {private:
     FILE *infile;
     char command[1024];
     double lastx,lasty,texth,textw,texta,rotate;
     int down,relative;
     EntityHeader h;
   public:
     HPGLIn(void)  {  infile = 0;  lastx = 0.0;  lasty = 0.0;  texth = textw = 1.0;  texta = 0.0;  down = 0;  relative = 0;  }
     ~HPGLIn();
     int open(RCCHAR *s);
     int nextcommand(void);
     void processcommand(void);
     void read(void);
     void close(void);
  };

#define DEFAULT_RESOURCE_STRING_LENGTH 512

class ResourceString
  {private:
     int allocated;
     TCHAR *string;
     RCCHAR *RCstring;
   public:
     //ResourceString() { string=0; RCstring=0; allocated=0; }
     ResourceString(int id);
     ResourceString(int id,int size);
     ~ResourceString();
     RCCHAR *getws(void)  {  return RCstring;  }
#if defined(_WIN32_WCE)
     _TCHAR *getws(void);  //  Return the wide characters
#else
     RCCHAR *gets(void) {  return RCstring;  }//  Return the wide characters
#endif
  };

class Decode
  {private:
     FILE *infile;
     int bitcount,bits,error;
     int *zz;
     unsigned char *DLi,*DVij,*ALi,*AVij,*QT,*Vij;
     double C[8][8],Ct[8][8];
     int order[8][8];
     int maxcode[17],mincode[17],valptr[17],huffsize[256],huffcode[256];
     void decoder_tables(unsigned char *,unsigned char *);
     int decode(void);
     int nextbit(void);
     int recieve(int ssss);
     int decode_dc_cooefficients(void);
     int decode_ac_cooefficients(void);
   public:
     Decode(FILE *in);
     int decodenext(unsigned char *dli,unsigned char *dvih,unsigned char *ali,unsigned char *avih,unsigned char *qt,int *zz,int *prev);
     int reset(void);
     static int zigzagx[];
     static int zigzagy[];
     static long power2[];
  };


//
//  Hardware shading interface
//

#if 1
#define BS_FRONTBUFFERONLY   1
#define BS_IGNOREDEPTH       2
#define BS_NOCLEAR           4

typedef int (*D3DSetLight)(double x,double y,double z);
typedef int (*D3DSetLightEx)(int i,double x,double y,double z,double r,double g,double b);
typedef int (*D3DSetMaterial)(double r,double g,double b,double a,double d,double s);
typedef int (*D3DSetMaterialEx)(double r,double g,double b,double a,double d,double s,double t);
typedef int (*D3DSetBackground)(double r,double g,double b);
typedef int (*D3DSetView)(double xe,double ye,double ze,double xr,double yr,double zr,double xu,double yu,double zu,double front,double back,int perspective);
typedef int (*D3DSetViewport)(double umin,double vmin,double umax,double vmax);
typedef int (*D3DBeginScene)(int);
typedef int (*D3DEndScene)(void);
typedef int (*D3DDrawTriangle)(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3,double nx,double ny,double nz);
typedef int (*D3DDrawPolygon)(Point origin,Point xaxis,Point yaxis,int nloops,int *nvert,Point **verts);
typedef int (*D3DDrawTexturedTriangle)(Point *p);
typedef int (*D3DDrawTexturedTriangles)(int n,Point *p);
typedef int (*D3DOpenOpenGl)(RCHDC hdc,int surfacetype,int x,int y,int width,int height);
typedef int (*D3DCloseOpenGl)(HDC hdc);
typedef int (*D3DLoadTexture)(RCCHAR *filename,double r,double g,double b,double trange,double op,int *h);
typedef int (*D3DLoadMemoryTexture)(RCCHAR *filename,RCCHAR *filedata,int width,int height,double r,double g,double b,double trange,double op,int *h);
typedef int (*D3DSetTexture)(long texture);
typedef int (*D3DTriangulatePolygon)(int nloops,int *nvert,Point **vert,Point **triangles);
typedef int (*D3DReleaseTextures)(RCHDC hdc);
typedef int (*D3DFlush)(void);
#endif

static int usingd3d;
#if 0
extern D3DSetLight SetLight;
extern D3DSetLightEx SetLightEx;
extern D3DSetMaterial SetMaterial;
extern D3DSetMaterialEx SetMaterialEx;
extern D3DSetBackground SetBackground;
extern D3DSetView SetView;
extern D3DSetViewport SetViewport;
extern D3DBeginScene BeginScene;
extern D3DEndScene EndScene;
extern D3DDrawTriangle DrawTriangle;
extern D3DDrawPolygon DrawPolygon;
extern D3DDrawTexturedTriangle DrawTexturedTriangle;
extern D3DDrawTexturedTriangles DrawTexturedTriangles;
extern D3DOpenOpenGl OpenOpenGl;
extern D3DCloseOpenGl CloseOpenGl;
extern D3DLoadTexture LoadTexture;
extern D3DLoadMemoryTexture LoadMemoryTexture;
extern D3DSetTexture SetTexture;
extern D3DTriangulatePolygon TriangulatePolygon;
extern D3DReleaseTextures ReleaseTextures;
extern D3DFlush Flush;
#endif

#ifdef _USING_QT
//extern QApplication app;
extern RCApplication *app;
#endif

extern Values        v;
extern Database      db;
extern WindowManager wm;
extern StateMachine  state;
extern Commands      commands;
extern ButtonMenus   buttonmenus;
extern Rubberband    rubberband;
extern MainWindow   *cadwindow;
#if VERSION != TRICAD_STUDENT
extern VideoWindow  *activevideowindow;
#endif
extern MetreCom      metrecom;
extern int cad_version;
extern CommandFile   commandfile;
extern MainMenu mainmenu;
extern Calculator calculator;
extern Home home;
extern ArchitectActive architectactive;
extern AnimationActive animationactive;
extern NCActive ncactive;
extern CustomMenuActive custommenuactive;
#endif

//extern "C" long FAR PASCAL dialogcb(HWND,UINT,WPARAM,LPARAM);
extern LRESULT CALLBACK MainEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);

extern void DebugTraceLine(const char *,int);

#ifdef _RCDEBUG
extern int gMatrixUseCnt;
extern int goffscreenbmUseCnt;
#endif

extern void regenGLlists();
extern bool lgresizing;

#endif
