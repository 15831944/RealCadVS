#ifndef NC_DIALOG_H
#define NC_DIALOG_H

#include "ncwin.h"
//#include "qtdefs.h"
#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QCheckBox>

class QWidget;
class Figure;
class sunlightInfo;
class Colour;

// KMJ: MOD
enum {  DisplayControl,CustomControl,ScrollBarControl,RealControl,IntegerControl,ListControl,StringControl,CheckBoxControl,RadioButtonControl,ButtonControl,ScaleControl
   , RCListControl };

#define DC_NOERASEBACKGROUND 1

class  Dialog;
class Colour;

class DialogControl
  {
#ifdef USING_WIDGETS
protected:
    QWidget *m_qw;
public:
    void setQW(QWidget *w) { m_qw = w; }
    QWidget* getQW(){ return m_qw; }
#endif
protected:
     int id,options;
   public:
#ifdef USING_WIDGETS
     DialogControl(int i, QWidget *qw=0) {  id = i;  options = 0; m_qw = qw; }
#else
     DialogControl(int i) {  id = i;  options = 0; }
#endif
     virtual ~DialogControl() {}
     int getid(void)  {  return id;  }
     void setoptions(int o)  {  options = o;  }
     int getoptions(void)  {  return options;  }
     virtual int type(void) = 0;
     virtual void load(Dialog *) {  }
     virtual int store(Dialog *) {  return 1;  }
     virtual int select(Dialog *) {  return 0;  }
     virtual void vscrollevent(Dialog *,int,int,int) {  }
     virtual void changefocus(Dialog *,int updatescreen = 1);
     virtual void selectitem(Dialog *);
     virtual void enable(Dialog *,int);
  	 virtual int validate(Dialog *) {  return 1;  }
     virtual void lbuttondblclkevent(Dialog *,int,int,int);

  };

class ScrollBarDialogControl : public DialogControl
  {protected:
     int min,max;
     DialogControl *dc;
   public:
     ScrollBarDialogControl(int i,DialogControl *dc1,int _min=0,int _max=1000) : DialogControl(i) {  dc = dc1; ;min=_min; max=_max;  }
     //ScrollBarDialogControl(int i,DialogControl *dc1) : DialogControl(i) {  dc = dc1;  }
     virtual int type(void) {  return ScrollBarControl;  }
     void vscrollevent(Dialog *,int,int,int);
     void load(Dialog *dialog);
  };

class CustomDialogControl : public DialogControl
  {protected:
      RCHWND hWnd;
   public:
     CustomDialogControl(int id) : DialogControl(id) {}
	  int type(void)  {  return CustomControl;  }
     void sethwnd(RCHWND hwnd)  {  hWnd = hwnd;  }
     RCHWND gethwnd(void)  {  return hWnd;  }
     virtual void draw(Dialog *,RCHDC) = 0;
     virtual void mousemoveevent(Dialog *,int,int,int) {};
     virtual void lbuttonupevent(Dialog *,int,int,int) {};
     virtual void lbuttondownevent(Dialog *,int,int,int) {};
     virtual void timerevent(Dialog *) {};
  };

class DisplayDialogControl : public DialogControl
  {protected:
     RCHWND hWnd;
   public:
      DisplayDialogControl(int id) : DialogControl(id) {hWnd=0;}
     int type(void)  {  return DisplayControl;  }
     void sethwnd(RCHWND hwnd)  {  hWnd = hwnd;  }
     RCHWND gethwnd(void)  {  return hWnd;  }
     virtual void draw(Dialog *,RCHDC) = 0;
  };

class SSDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR lastname[300];
   public:
     SSDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(lastname,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
     void clear(void)  {  strcpy(lastname,"");  }
  };

class RadioButtonDialogControl : public DialogControl
  {protected:
     int oldvalue,id1,id2,*value;
   public:
  	  RadioButtonDialogControl(int i,int i1,int i2,int *v) : DialogControl(i)
       {  id1 = i1;  id2 = i2;  oldvalue = *v;  value = v;  }
     int type(void) {  return RadioButtonControl;  };
	    void load(Dialog *);
     int store(Dialog *);
     void change(Dialog *,int);
     int select(Dialog *);
     int currentvalue(Dialog *,int *);
     int ischecked(Dialog *dlg)
     {
         int nv = 0;
         if(currentvalue(dlg,&nv))
             return nv;
         else
             return 0;
     }

     void selectitem(Dialog *dialog);
     int getid1(void)  {  return id1;  } 
  };

class ButtonDialogControl : public DialogControl
  {public:
     ButtonDialogControl(int i) : DialogControl(i) {}
	  int type(void) {  return ButtonControl;  };
     int select(Dialog *) {  return id;  }
  };

class ListDialogControl : public DialogControl
  {
  private:
	  int ctype;
  protected:
     short *shvalue;
	 int n,length,*value;
     RCCHAR **clist,*csvalue;
     RCCHAR **list,*svalue;
     QString *stlist,stvalue;
   public:

	    ListDialogControl(int i,int num,RCCHAR **l);
	    ListDialogControl(int i,int num,RCCHAR **l,int *v);
	    ListDialogControl(int i,int num,RCCHAR **l,short *v);
	    ListDialogControl(int i,int num,RCCHAR **l,RCCHAR *s,int len);
        ListDialogControl(int i,int num,QString *l,int *v);

     int type(void) {  return ListControl;  }
     void load(Dialog *);
     int store(Dialog *);
	 void change(Dialog *,int);
     virtual void change(Dialog *,RCCHAR *);
     void additem(Dialog *,RCCHAR *);
     void removeitem(Dialog *,RCCHAR *);
     void removeallitems(Dialog *);
     int getitem(Dialog *,int,RCCHAR *,int);
     int validate(Dialog *);
	 int currentvalue(Dialog *,short *);
     int currentvalue(Dialog *,int *);
     int currentvalue(Dialog *,RCCHAR *,int);
#ifdef USING_WIDGETS
     // KMJ: new
     int getLength() { return n; }
     void setLenth(int num) { n = num; }
#endif
  };

class RCListDialogControl : public DialogControl
  {protected:
     short *shvalue;
	 int n,length,*value;
     RCCHAR **list,*svalue;
   public:

     RCListDialogControl(int i,int num,RCCHAR **l);
     RCListDialogControl(int i,int num,RCCHAR **l,int *v);
     RCListDialogControl(int i,int num,RCCHAR **l,short *v);
     RCListDialogControl(int i,int num,RCCHAR **l,RCCHAR *s,int len);
     int type(void) {  return ListControl;  };
     void load(Dialog *);
     int store(Dialog *);
	 void change(Dialog *,int);
     virtual void change(Dialog *,RCCHAR *);
     void additem(Dialog *,RCCHAR *);
     void removeitem(Dialog *,RCCHAR *);
	 void removeallitems(Dialog *);
     int getitem(Dialog *,int,RCCHAR *,int);
     int validate(Dialog *);
	 int currentvalue(Dialog *,short *);
     int currentvalue(Dialog *,int *);
     int currentvalue(Dialog *,RCCHAR *,int);
  };

class ScaleDialogControl : public DialogControl
  {protected:
     double *scale;
   public:
  	  ScaleDialogControl(int i,double *);
	    int type(void) {  return ScaleControl;  };
     void load(Dialog *);
     int store(Dialog *);
	    void change(Dialog *,double);
     int validate(Dialog *);
	    int currentvalue(Dialog *,double *);
  };


class StringDialogControl : public DialogControl
  {private:
     RCCHAR *string;
     int length;
#ifdef USING_WIDGETS
        // KMJ: new
        RCCHAR *specialValueText; // used by options dialogs
        RCCHAR *defaultValue; // used by options dialogs
#endif
	public:
     StringDialogControl(int i,RCCHAR *s,int l) : DialogControl(i) { string = s;  length = l; specialValueText=0; defaultValue=0;}
     StringDialogControl(int i,char *s,int l) : DialogControl(i) { string = CHAR2RCCHR(s);  length = l; specialValueText=0; defaultValue=0; }
     int type(void) {  return StringControl;  };
     void load(Dialog *);
	  int store(Dialog *);
     void change(Dialog *,RCCHAR *);
     void change(Dialog *,char*);
#if defined(_WIN32_WCE)
     void change(Dialog *,TCHAR *);
#endif
     int currentvalue(Dialog *,RCCHAR *,int);
#ifdef USING_WIDGETS
     void setSpecialValueText(RCCHAR *text)
     {
         delete [] specialValueText;
         specialValueText = new RCCHAR[strlen(text)+1];
         strcpy(specialValueText,text);
     }
     RCCHAR* getSpecialValueText()
     {
         return specialValueText;
     }
     void setDefaultValue(RCCHAR *text)
     {
         delete [] defaultValue;
         defaultValue = new RCCHAR[strlen(text)+1];
         strcpy(defaultValue,text);
     }
     RCCHAR* getDefaultValue()
     {
        return defaultValue;
     }
     virtual ~StringDialogControl()
     {
         delete [] specialValueText;
         delete [] defaultValue;
     }
#endif
  };

enum ScaleBy {  DontScale, ScaleByLength, ScaleByArea, ScaleByVolume  };

class RealDialogControl : public DialogControl
  {protected:
        RCCHAR originaltext[300];
#ifdef USING_WIDGETS
        // KMJ: new
        RCCHAR specialValueText[300]; // used by options dialogs
        double defaultValue; // used by options dialogs
#endif
     double *value,originalvalue,step,rmin,rmax;
     ScaleBy scaleby;
	  public:
        RealDialogControl(int i,double *v,const ScaleBy sb = DontScale, double df=0.0, QWidget *qw=0);
        RealDialogControl(int i,double *v,double s,const ScaleBy sb = DontScale, double df=0.0, QWidget *qw=0);
        RealDialogControl(int i,double *v,double s,double rmin1,double rmax1,const ScaleBy sb = DontScale, double df=0.0, QWidget *qw=0);
	    int type(void) {  return RealControl;  };
     void load(Dialog *);
     int store(Dialog *);
     int validate(Dialog *);
     void change(Dialog *,double);
     int currentvalue(Dialog *,double *);
     void vscrollevent(Dialog *,int,int,int);
     void changefocus(Dialog *,int updatescreen = 1);
#ifdef USING_WIDGETS
     // KMJ: new
     void setSpecialValueText(RCCHAR *text);
     RCCHAR *getSpecialValueText() { return specialValueText; }
     void setDefaultValue(double val)
     {
         defaultValue = val;
         //QString str = QString("%1").arg(val, 0, 'f', db.getnumericdisplaytolerance());
         //if(QString(specialValueText) != "*Varies*")
         //   strcpy(specialValueText,str.data());
     }
     double getDefaultValue() { return defaultValue; }
#endif
  };

class IntegerDialogControl : public DialogControl
  {private:
        int *ivalue;
        short *svalue;
	    long *lvalue,imin,imax;
#ifdef USING_WIDGETS
        // KMJ: new
        RCCHAR specialValueText[300];
        int defaultValue;
#endif
   public:
	    IntegerDialogControl(int i,int *v);
	    IntegerDialogControl(int i,short *v);
	    IntegerDialogControl(int i,long *v);
	    IntegerDialogControl(int i,int *v,long imin1,long imax1);
	    IntegerDialogControl(int i,short *v,long imin1,long imax1);
	    IntegerDialogControl(int i,long *v,long imin1,long imax1);
	    int type(void) {  return IntegerControl;  };
	    void load(Dialog *);
     int store(Dialog *);
     int validate(Dialog *);
	    void change(Dialog *,long);
     int currentvalue(Dialog *,int *);
     int currentvalue(Dialog *,short *);
     int currentvalue(Dialog *,long *);
     void vscrollevent(Dialog *,int,int,int);
     void changefocus(Dialog *,int updatescreen = 1);
#ifdef USING_WIDGETS
     // KMJ: new
     void setSpecialValueText(RCCHAR *text);
     RCCHAR *getSpecialValueText() { return specialValueText; }
     void setDefaultValue(int val) { defaultValue = val; }
     int getDefaultValue() { return defaultValue; }
#endif
  };

class WeightAssocRealDialogControl : public RealDialogControl
{
public:
     WeightAssocRealDialogControl(int id,double *v,  double df=0.0 ,QWidget *qw=0) : RealDialogControl(id,v,DontScale,df,qw){ }
     void changefocus(Dialog *,int updatescreen = 1);
     void vscrollevent(Dialog *,int,int,int);
     void change(Dialog *d,double v);
};

class SSListDialogControl : public ListDialogControl
  {private:
     RCCHAR name[300];
   public:
     SSListDialogControl(int id) : ListDialogControl(id,0,0,name,300)  {  name[0] = 0;  };
     void load(Dialog *);
     void sort();
     int compare(RCCHAR left[300], RCCHAR right[300]);
     void changefocus(Dialog *dialog,int updatescreen = 1);
     void change(Dialog *,int);
     void change(Dialog *dialog,RCCHAR *str);
  };

class StyleDisplayDialogControl : public DisplayDialogControl
  {private:
     int lastindex;
     int lastsegment;
     LineStyle **linestyle;
     Dialog *parentdialog;
   public:
     StyleDisplayDialogControl(int id,Dialog *d,LineStyle **ls) : DisplayDialogControl(id)
        {  lastindex = lastsegment = -1;  parentdialog = d;  linestyle = ls;
        }
     void draw(Dialog *dialog,RCHDC hDC);
     LineStyle *getLinestyle() { return *linestyle; }
  };

/**
 * @brief The GPColourDisplayDialogControl class
 * General Properties Colour Dialog Current sample control
 * for use with the DefineColour_Dialog when defining palette colours
 */
class GPColourDisplayDialogControl : public DisplayDialogControl
  {private:
     GeneralList controllist;
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
     Colour fixedcolour;
   public:
     GPColourDisplayDialogControl(int id,Dialog *d,Colour c) : DisplayDialogControl(id)
        {  lastindex = -1;  parentdialog = d;  lastcolour = c; fixedcolour=c;}
     void add(DisplayDialogControl *dc){ controllist.add(dc); }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The GPColourSampleDialogControl class
 * General Properties Colour Dialog other samples control
 * for use with the DefineColour_Dialog when defining palette colours
 */
class GPColourSampleDialogControl : public DisplayDialogControl
  {private:
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
     DisplayDialogControl *parentControl;
     Colour fixedcolour;
   public:
     GPColourSampleDialogControl(int id, DisplayDialogControl *pc, Dialog *d,Colour c) : DisplayDialogControl(id)
        {  lastindex = -1; parentControl = pc,  parentdialog = d;  lastcolour = c; fixedcolour=c;}
     void draw(Dialog *dialog,RCHDC hDC);
     void change(Colour c) { lastcolour = c; }
  };

/**
 * @brief The OtherColourDisplayDialogControl class
 * Other Colour Dialog current colour control
 * for use with the DefineColour_Dialog when defining colours other than the palette
 */
class OtherColourDisplayDialogControl : public  DisplayDialogControl
  {private:
     GeneralList controllist;
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
     Colour fixedcolour;
   public:
     OtherColourDisplayDialogControl(int id,Dialog *d,Colour c) :  DisplayDialogControl(id )
        {  lastindex = -1;  parentdialog = d;  lastcolour = c; fixedcolour=c;  }
     OtherColourDisplayDialogControl(int id,Dialog *d,Colour c,Colour fc) :  DisplayDialogControl(id )
        {  lastindex = -1;  parentdialog = d;  lastcolour = c; fixedcolour=fc; }
     void add(DisplayDialogControl *dc){ controllist.add(dc); }
     void draw(Dialog *dialog,RCHDC hDC);
     void setIndex(int id) { lastindex = id; }
   };

/**
 * @brief The OtherColourSampleDialogControl class
 * Other Colour Dialog sample colour control
 * for use with the DefineColour_Dialog when defining colours other than the palette
 */
class OtherColourSampleDialogControl : public DisplayDialogControl
  {
protected:
//private:
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
     DisplayDialogControl *parentControl;
     Colour fixedcolour;
   public:
     OtherColourSampleDialogControl(int id, DisplayDialogControl *pc, Dialog *d,Colour c) : DisplayDialogControl(id )
        {  lastindex = -1; parentControl = pc,  parentdialog = d;  lastcolour = c; fixedcolour = c; }
     OtherColourSampleDialogControl(int id, DisplayDialogControl *pc, Dialog *d,Colour c,Colour fc) : DisplayDialogControl(id )
        {  lastindex = -1; parentControl = pc,  parentdialog = d;  lastcolour = c; fixedcolour = fc; }
     virtual void draw(Dialog *dialog,RCHDC hDC);
     virtual void change(Colour c) { lastcolour = c; }
     //virtual void change(Colour c);
  };

//class BackColourDialogSampleControl : public DisplayDialogControl
class BackColourDialogSampleControl : public OtherColourSampleDialogControl
  {
private:
//private:
     //int lastindex;
     //Dialog *parentdialog;
     //Colour lastcolour;
     //DisplayDialogControl *parentControl;
   public:
     BackColourDialogSampleControl(int id, DisplayDialogControl *pc, Dialog *d,Colour c) :  OtherColourSampleDialogControl(id,pc,d,c) /*DisplayDialogControl(id )*/
        {  lastindex = -1; parentControl = pc,  parentdialog = d;  lastcolour = c; }
     virtual void draw(Dialog *dialog,RCHDC hDC);
     virtual void change(Colour c) { lastcolour = c; }
  };

/*
class GridColourDisplayDialogControl : public  DisplayDialogControl
  {private:
     GeneralList controllist;
     int lastindex;
     Dialog *parentdialog;
     Colour lastcolour;
     Colour fixedcolour;
   public:
     GridColourDisplayDialogControl(int id,Dialog *d,Colour c) :  DisplayDialogControl(id )
        {  lastindex = -1;  parentdialog = d;  lastcolour = c; fixedcolour=c; }
     GridColourDisplayDialogControl(int id,Dialog *d,Colour c,Colour fc) :  DisplayDialogControl(id )
        {  lastindex = -1;  parentdialog = d;  lastcolour = c; fixedcolour=fc; }
     void add(DisplayDialogControl *dc){ controllist.add(dc); }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/

/**
 * @brief The GridColourSampleControl class
 * this is used by the define clour dialog when defining grid colours
 */
//class GridColourDialogSampleControl : public DisplayDialogControl
class GridColourDialogSampleControl : public OtherColourSampleDialogControl
  {
private:
//private:
     //int lastindex;
     //Dialog *parentdialog;
     //Colour lastcolour;
     //DisplayDialogControl *parentControl;
     //Colour fixedcolour;
    int gridstyle; // 0 =  dots; 1 = lines
    int ownerid;
   public:
     GridColourDialogSampleControl(int id, DisplayDialogControl *pc, Dialog *d,Colour c,Colour fc) :  OtherColourSampleDialogControl(id,pc,d,c) /*DisplayDialogControl(id )*/
        {  lastindex = -1; parentControl = pc,  parentdialog = d;  lastcolour = c; fixedcolour=fc; gridstyle=0; ownerid=0; }
     virtual void draw(Dialog *dialog,RCHDC hDC);
     virtual void change(Colour c) ;
     Colour getColour() { return lastcolour; }
     int getgridstyle() { return gridstyle; }
     void setgridstyle(int v) { gridstyle=v; }
     int getownderid() { return ownerid; }
     void setownerid(int id) { ownerid=id; }
  };

/**
 * @brief The ColourSampleControl class
 * this is used for the colour sample swatches on a dialog
 */
class ColourSampleControl : public  DisplayDialogControl
{
private:
    GeneralList controllist;
    int lastindex;
    Dialog *parentdialog;
    Colour *lastcolour;
    Colour defaultValue;
    RCCHAR specialValueText[50];
public:
    ColourSampleControl(int id,Dialog *d,Colour *c) :  DisplayDialogControl(id )
    {  lastindex = -1;  parentdialog = d;  lastcolour = c; }
    ColourSampleControl(int id,Dialog *d,double *r, double *g, double *b) :  DisplayDialogControl(id )
    {  lastindex = -1;  parentdialog = d;  lastcolour = new Colour(*r,*g,*b); }
    void add(DisplayDialogControl *dc){ controllist.add(dc); }
    void draw(Dialog *dialog,RCHDC hDC);
    void change(Colour c){lastcolour->red = c.red; lastcolour->green = c.green; lastcolour->blue = c.blue; }
    Colour getColour() { return *lastcolour; }
    int getColour(double *red, double *green, double *blue) { lastcolour->get(red,green,blue); return 1; }
    void setDefaultValue(Colour c) { defaultValue = c; }
    Colour getDefaultValue() { return defaultValue; }
    void setSpecialValueText(RCCHAR *text){strcpy(specialValueText,text);}
    RCCHAR* getSpecialValueText(){return specialValueText;}
};

/**
 * @brief The GridColourSampleControl class
 * this is used for the colour sample swatches on the grid dialog
 */
class GridColourSampleControl : public  DisplayDialogControl
{
private:
    GeneralList controllist;
    int lastindex;
    Dialog *parentdialog;
    Colour *lastcolour;
public:
    GridColourSampleControl(int id,Dialog *d,Colour *c) :  DisplayDialogControl(id )
    {  lastindex = -1;  parentdialog = d;  lastcolour = c; }
    void add(DisplayDialogControl *dc){ controllist.add(dc); }
    void draw(Dialog *dialog,RCHDC hDC);
    void change(Colour c){*lastcolour = c;}
    Colour getColour() { return *lastcolour; }
};

/**
 * @brief The ColourApplyButtonDialogControl class
 * an apply button control for use with the DefineColour_Dialog
 * the mode value indicates which RealCAD colour element is being changed
 * Mode = 0 :- changes the current palette colour
 * Mode = 1 :- changes the current background colour
 * Mode = 2 :- returns the colour values
 */
class ColourApplyButtonDialogControl : public ButtonDialogControl
{
public:
    int mode;
public:
   ColourApplyButtonDialogControl(int id, int m=0) :
       ButtonDialogControl(id) { mode = m;  }
   int select(Dialog *);
};

/**
 * @brief The FontListDialogControl class
 * the list control for use with a QFontComboBox
 */
class FontListDialogControl : public ListDialogControl
  {
   protected:
    RCCHAR specialValueText[300];
    RCCHAR defaultFont[300];
   public:
     QString font;
     FontListDialogControl(int id,int n,RCCHAR **l,int *f) : ListDialogControl(id,n,l,f)
     { strcpy(specialValueText,_RCT("")); strcpy(defaultFont,_RCT("")); }
     void change(Dialog *dialog,int);
     void load(Dialog *dialog);
     void changefocus(Dialog *dialog,int updatescreen = 1);
     void setSpecialValueText(RCCHAR *text);
     RCCHAR *getSpecialValueText();
     void setDefaultFont(RCCHAR *dfont);
     RCCHAR *getDefaultFont();
  };


/**
 * @brief The TextDisplayDialogControl class
 */
class TextDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR **fonts;
     Byte *charsets;
     BitMask *change;
   public:
     TextDisplayDialogControl(int id,RCCHAR **fs,Byte *cs, BitMask *ch=NULL) : DisplayDialogControl(id) {  fonts = fs;  charsets = cs; change = ch; }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The PatternStyleDisplayDialogControl class
 */
class PatternStyleDisplayDialogControl : public DisplayDialogControl
  {private:
     Dialog *parentdialog;
     RCCHAR oldpatternlinestyle[300];
   public:
     PatternStyleDisplayDialogControl(int id,Dialog *d) : DisplayDialogControl(id)
        {  parentdialog = d;  strcpy(oldpatternlinestyle,"");
        }
     void draw(Dialog *dialog,RCHDC hDC);
     void UpdateLineStyle(RCCHAR *pls)  {  strcpy(oldpatternlinestyle,pls);  }
  };

/**
 * @brief The DimensionDefaults class
 */
class DimensionDefaults
  {public:
     double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
     int precision,tolprecision;
#ifdef USING_WIDGETS
     int usePrefix1,useSuffix1;
#endif
     BitMask options;
     TerminatorInfo term1,term2;
     TextInfo vinfo,tinfo,pinfo,sinfo;
     DimensionDefaults() : options(32) {}
     void draw(RCHWND hWnd,RCHDC hDC,int all);
  };

/**
 * @brief The LeaderDisplayDialogControl class
 */
class LeaderDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     LeaderDisplayDialogControl(int id,DimensionDefaults *d,RCCHAR **fs,Byte *cs) : DisplayDialogControl(id) {  dd = d;  fonts = fs;  charsets = cs; }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The DimensionDisplayDialogControl class
 */
class DimensionDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     DimensionDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The NTerminatorDisplayDialogControl class
 */
class NTerminatorDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     NTerminatorDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The CTerminatorDisplayDialogControl class
 */
class CTerminatorDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     CTerminatorDisplayDialogControl(int id,DimensionDefaults *dd1) : DisplayDialogControl(id) { dd = dd1; }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The TTextDisplayDialogControl class
 */
class TTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     TTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The SEntryDisplayDialogControl class
 */
class SEntryDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     SEntryDisplayDialogControl(int id,DimensionDefaults *dd1) :
              DisplayDialogControl(id) {  dd = dd1;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The PEntryDisplayDialogControl class
 */
class PEntryDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
   public:
     PEntryDisplayDialogControl(int id,DimensionDefaults *dd1) :
              DisplayDialogControl(id) {  dd = dd1;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The VTextDisplayDialogControl class
 */
class VTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     VTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The PTextDisplayDialogControl class
 */
class PTextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     PTextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

/**
 * @brief The STextDisplayDialogControl class
 */
class STextDisplayDialogControl : public DisplayDialogControl
  {private:
     DimensionDefaults *dd;
     RCCHAR **fonts;
     Byte *charsets;
   public:
     STextDisplayDialogControl(int id,DimensionDefaults *dd1,RCCHAR **fs,Byte *cs) :
              DisplayDialogControl(id) {  dd = dd1;  fonts = fs;  charsets = cs;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class ColourScrollBarDialogControl : public ScrollBarDialogControl
  {public:
     ColourScrollBarDialogControl(int i,DialogControl *dc1,int min=0,int max=1000) :
        ScrollBarDialogControl(i,dc1,min,max) {  }
     void vscrollevent(Dialog *,int,int,int);
     void setscrollpos(Dialog *,int);
  };

class TransparencyScrollBarDialogControl : public ScrollBarDialogControl
  {public:
     TransparencyScrollBarDialogControl(int i,DialogControl *dc1,int min=0,int max=1000) :
        ScrollBarDialogControl(i,dc1,min,max) {  }
     void vscrollevent(Dialog *,int,int,int);
     void setscrollpos(Dialog *,int);
  };

class CheckBoxDialogControl : public DialogControl
  {protected:
     int oldvalue,bit,*ivalue;
     short *svalue;
     long *lvalue;
     BitMask *bvalue;
   public:
     CheckBoxDialogControl(int i,int *v) : DialogControl(i)  {  oldvalue = *v;  ivalue = v;  lvalue = NULL;  svalue = NULL;  bvalue = NULL;  }
     //CheckBoxDialogControl(int i,int *v, int b) : DialogControl(i)  {  oldvalue = *v;  ivalue = v;  lvalue = NULL;  svalue = NULL;  bvalue = NULL; bit = b; }
     CheckBoxDialogControl(int i,short *v) : DialogControl(i)  {  oldvalue = *v;  svalue = v;  lvalue = NULL;  ivalue = NULL;  bvalue = NULL;  }
     CheckBoxDialogControl(int i,long *v) : DialogControl(i)  {  oldvalue = int(*v);  lvalue = v;  svalue = NULL;  ivalue = NULL;  bvalue = NULL;  }
     CheckBoxDialogControl(int i,BitMask *v,int b) : DialogControl(i)  {  oldvalue = v->test(b);  bvalue = v;  lvalue = NULL;  svalue = NULL;  ivalue = NULL;  bit = b;  }
     int type(void) {  return CheckBoxControl;  };
     void load(Dialog *);
  	  int store(Dialog *);
     int select(Dialog *);
     int currentvalue(Dialog *,int *);
     int currentvalue(Dialog *,short *);
     int currentvalue(Dialog *,long *);
     int currentvalue(Dialog *,BitMask *);
     void change(Dialog *,long);
  };

class TriStateCheckBoxDialogControl : public CheckBoxDialogControl
{
protected:
    long defaultValue;
    bool triState;
public:
    TriStateCheckBoxDialogControl(int i,int *v,bool t=true,int d=0) : CheckBoxDialogControl(i,v)  {  oldvalue = *v;  ivalue = v;  lvalue = NULL;  svalue = NULL;  bvalue = NULL; triState = t; defaultValue=d; }
    void load(Dialog *);
    int select(Dialog *);
    int change(Dialog *,long);
    void setTriState(bool onoff) { triState = onoff; if(getQW()) ((QCheckBox*)getQW())->setTristate(triState); }
    bool isTriState() { return triState; }
    void setDefaultValue(int dv) { defaultValue=dv; }
    int getDefaultValue() { return defaultValue; }
};

class TransparentPlaneButtonDialogControl : public ButtonDialogControl
{
protected:
    double *ptransparency; // pointers to returned colour values
public:
     TransparentPlaneButtonDialogControl(int id, double *value=0)
         : ButtonDialogControl(id), ptransparency(value) {}
     int select(Dialog *);
     int currentvalue(double *value);
};

class BitMapETransparencyRangeDisplayControl : public DisplayDialogControl
{
private:
  int csampleId;
  int tcontrolId;
  int widthId;
  int heightId;
  double *width;
  double *height;
public:
  RCCHAR bitmapPath[600];
public:
  BitMapETransparencyRangeDisplayControl(int id, int cid, int tid, RCCHAR *pbitmappath, double *w=0, double *h=0)
      : DisplayDialogControl(id), csampleId(cid), tcontrolId(tid), width(w), height(h)
  { strcpy(bitmapPath,pbitmappath); }
  void draw(Dialog *dialog,RCHDC hDC);
};

class BitMapETransparencyRangeDialogControl : public ButtonDialogControl
{
protected:
    int tsampleId;
    int csampleId;
    int bitmapId;
    int widthId;
    int heightId;
    double *ptrans, *ptred, *ptgreen, *ptblue;
    double deftrange, defRed, defGreen, defBlue;
    RCCHAR specialValueText[100];

public:
    BitMapETransparencyRangeDialogControl(int id, int tid, int cid, int bid, double *range=0, double *red=0, double *green=0, double *blue=0, int wid=0, int hid=0);
    int select(Dialog *);
    int currentvalue(double *range, double *red, double *green, double *blue);
    int setDefaultValue(double range, double red, double green, double blue)
    {deftrange=range;defRed=red;defGreen=green;defBlue=blue;}
    void getDefaultValue(double *range, double *red, double *green, double *blue)
    {*range=deftrange;*red=defRed;*green=defGreen;*blue=defBlue;}
    void setSpecialValueText(RCCHAR *text){ strcpy(specialValueText,text); }
    RCCHAR* getSpecialValueText() {return specialValueText;}

};

class GradientPlaneButtonDialogControl : public ButtonDialogControl
{
protected:
    int sampleid; // the id of the colour sample graphic
    double *pr, *pg, *pb; // pointers to returned colour values
    double defRed,defGreen,defBlue;
    RCCHAR specialValueText[50];
public:
     GradientPlaneButtonDialogControl(int id, int bid, double *red=0, double *green=0, double *blue=0)
         : ButtonDialogControl(id), sampleid(bid), pr(red), pg(green), pb(blue) {}
     int select(Dialog *);
     int currentvalue(double *red, double *green, double *blue);
     int setDefaultValue(double red, double green, double blue)
     {defRed=red;defGreen=green;defBlue=blue; return 1;}
     void getDefaultValue(double *red, double *green, double *blue)
     {*red=defRed;*green=defGreen;*blue=defBlue;}
     void setSpecialValueText(RCCHAR *text){ strcpy(specialValueText,text); }
     RCCHAR* getSpecialValueText() {return specialValueText;}
};

class BitMapEPlaneButtonDialogControl : public ButtonDialogControl
{
protected:
    int sampleid; // the id of the colour sample graphic
    double *tr, *tg, *tb; // pointers to returned colour values
    double defRed,defGreen,defBlue;
    RCCHAR specialValueText[50];
public:
     BitMapEPlaneButtonDialogControl(int id, int bid, double *red=0, double *green=0, double *blue=0)
         : ButtonDialogControl(id)
     {sampleid = bid; tr = red; tg = green; tb = blue;}
     int select(Dialog *);
     int currentvalue(double *red, double *green, double *blue);
     int setDefaultValue(double red, double green, double blue)
     {defRed=red;defGreen=green;defBlue=blue; return 1;}
     void getDefaultValue(double *red, double *green, double *blue)
     {*red=defRed;*green=defGreen;*blue=defBlue;}
     void setSpecialValueText(RCCHAR *text){ strcpy(specialValueText,text); }
     RCCHAR* getSpecialValueText() {return specialValueText;}
};

class BitMapEDisplayDialogControl : public DisplayDialogControl
{
   private:
     double iwidth,iheight;
   public:
     BitMapEDisplayDialogControl(int id,double iw=1.0,double ih=1.0) : DisplayDialogControl(id)
     {
         iwidth = iw;  iheight = ih;
         if (fabs(iwidth) < 1.0E-10) iwidth = 1.0;
         if (fabs(iheight) < 1.0E-10) iheight = 1.0;
     }
     void draw(Dialog *dialog,RCHDC hDC);
};

class PlaneDisplayDialogControl : public DisplayDialogControl
  {public:
     PlaneDisplayDialogControl(int id) : DisplayDialogControl(id) { }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class RuledSurfaceDisplayDialogControl : public DisplayDialogControl
  {public:
     RuledSurfaceDisplayDialogControl(int id) : DisplayDialogControl(id) { }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class SurfaceOfRevDisplayDialogControl : public DisplayDialogControl
  {public:
     SurfaceOfRevDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class PatchDisplayDialogControl : public DisplayDialogControl
  {public:
     PatchDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class FigureDisplayDialogControl : public DisplayDialogControl
  {private:
   public:
     FigureDisplayDialogControl(int id) : DisplayDialogControl(id) {  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class HPDisplayDialogControl : public DisplayDialogControl
  {private:
     int *base;
     int fillstyle;
     RCCHAR lastdirectory[300];
   public:
     HPDisplayDialogControl(int id,int fs,int *bs) : DisplayDialogControl(id) {  fillstyle = fs;  base = bs;  strcpy(lastdirectory,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class MTDisplayDialogControl : public DisplayDialogControl
  {private:
     int *base;
     RCCHAR lastdirectory[300];
     //View3dOffScreenBitmap *output;
   public:
     //MTDisplayDialogControl(int id,int *bs,View3dOffScreenBitmap *op) : DisplayDialogControl(id) {  base = bs;  strcpy(lastdirectory,"");  output = op;  }
     MTDisplayDialogControl(int id,int *bs) : DisplayDialogControl(id) {  base = bs;  strcpy(lastdirectory,""); }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class MaterialTextureDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR lastdirectoryname[300];
     RCCHAR **filenamelist;
     int exists,lastbase,*base;
     //View3dOffScreenBitmap *output;
   public:
     //MaterialTextureDisplayDialogControl(int id,int *b,View3dOffScreenBitmap *op) : DisplayDialogControl(id) {   strcpy(lastdirectoryname,"InvalidDirectoryName");  filenamelist = 0;  exists = 0;   base = b;  lastbase = -1; output = op;  }
     MaterialTextureDisplayDialogControl(int id,int *b) : DisplayDialogControl(id) {   strcpy(lastdirectoryname,"InvalidDirectoryName");  filenamelist = 0;  exists = 0;   base = b;  lastbase = -1;   }
     void draw(Dialog *dialog,RCHDC hDC);
  };

extern "C" long FAR PASCAL dialogcb(HWND,UINT,WPARAM,LPARAM);
extern "C" LRESULT FAR PASCAL customcontrolcb(int,WPARAM,LPARAM);

enum  { DPOK = 1,DPTR = 2,DBELOW = 3 };

class Dialog
  {
	
#ifdef USING_WIDGETS
    friend class RCDialog;
protected:
    QWidget *m_qw;
public:
    // will use this to replace the code in the dialogcb function later
    void initdialog(){;}
    // gets the hDlg handle as a QWidget pointer
    void setQW(QWidget *w) { m_qw = w; }
    QWidget* getQW(){ return m_qw; }
    void sethdlg(RCHWND hwnd) {  hDlg = hwnd; }
#endif
private:
    friend long FAR PASCAL dialogcb(HWND,UINT,WPARAM,LPARAM);
	friend LRESULT FAR PASCAL customcontrolcb(int,WPARAM,LPARAM);
   private:
     int modal,position;
     RCHWND hDlg; // this is a pointer to a QDialog
     RCCHAR name[128],dtitle[128],dlabel[128];
     GeneralList controllist;
     GeneralList customlist;
     GeneralList displaylist;
     GeneralList disablelist;
     DLGPROC addressproc;
     Dialog *lcurrent;
     int lselchange;
     HHOOK lhhook;
#if ! defined(_WIN32_WCE)
     HOOKPROC lhookproc;
#endif
     HGLOBAL hglobaldialog;
     RCCHAR *globaldialog;
     long globallength;
   public:
	 Dialog(const RCCHAR *,int);
     Dialog(const char *,int);
     Dialog(const RCCHAR *);
     Dialog(const char *);
     Dialog(const RCCHAR *,const RCCHAR *,double *,ScaleBy sb);
     Dialog(const char *,const char *,double *,ScaleBy sb);
     Dialog(const RCCHAR *,const RCCHAR *,int *);
     Dialog(const RCCHAR *,const RCCHAR *,double *,double,double,double,ScaleBy sb);
     Dialog(const RCCHAR *,const RCCHAR *,int *,long,long);
     Dialog(const RCCHAR *,const RCCHAR *,RCCHAR *,int);
     void SetDlgItemText(int id,RCCHAR *);
     void SetDlgItemText(int id,char *);
     void GetDlgItemText(int id,RCCHAR *,int l);
     void GetDlgItemText(int id,char *,int l);
     void setposition(int dp)  {  position = dp;  }
     int getposition(void)  {  return position;  }
     void creatememory(int,int,int,int,int,DWORD,DWORD);
     void createcontrol(RCCHAR *,RCCHAR *,int,int,int,int,int,DWORD);
	  void add(DialogControl *c)  {  controllist.add(c);  }
     void add(CustomDialogControl *c)  {  customlist.add(c);  }
     void add(DisplayDialogControl *c)  {  displaylist.add(c);  }
     void adddisable(int i)  {  disablelist.add((void *)i);  }
     int getmodal(void)  {  return modal;  }
     int setmodal(int onoff)  { modal = onoff;  }
     void load(void);
     void store(void);
#ifdef USING_WIDGETS
     DialogControl *getdisplaycontrol(int);
#endif
     DialogControl *getcontrol(int);
     GeneralList getcustomlist(void)  {  return customlist;  }
     GeneralList getdisplaylist(void)  {  return displaylist;  }
     RCHWND gethdlg(void) {  return hDlg;  }
     void update(int);
     void updatecustomcontrols(void);
     int validate(void);
     int currentvalue(int,short *);
     int currentvalue(int,int *);
	  int currentvalue(int,double *);
     int currentvalue(int,BitMask *);
     int currentvalue(int,RCCHAR *,int);
     //int currentvalue(int,RCCHAR *,int);

     void title(const RCCHAR *t)  {  strcpy(dtitle,t);  }
     void title(const char *t)  {  title(CHAR2RCCHR(t));  }
     void label(const RCCHAR *l)  {  strcpy(dlabel,l);  }
     int process(RCHWND hwnd = 0,RCHINSTANCE hinstance = 0);

     int MessageBox(const RCCHAR *,const RCCHAR *,int);
     int MessageBox(const char *,const char *,int);
     int MessageBox(const RCCHAR *,const char *,int);

     ~Dialog();
     virtual void setfocus(HWND) {};
     virtual void killfocus(HWND) {};
     virtual void exitsizemove(void) {};

  };
  

enum SymbolType {  DimSym,AsSym,IfSym,ThenSym,ElseSym,EndIfSym,DoSym,WhileSym,LoopSym,ForSym,ToSym,StepSym,NextSym,SelectSym,CaseSym,EndSym,PrintSym,AndSym,OrSym,NotSym,OpenSym,CloseSym,InputSym,OutputSym,RemSym,FunctionSym,SubSym,HashSym,VariableNameSym,LessSym,GreaterSym,LessEqualSym,GreaterEqualSym,NotEqualSym,PlusSym,MinusSym,EqualsSym,ColonSym,SemiColonSym,DivideSym,MultiplySym,LBracketSym,RBracketSym,FunctionNameSym,VariableSym,CommaSym,DotSym,EolSym,BadSym,IntegerConstantSym,RealConstantSym,StringConstantSym,PointConstantSym  };

class Expression
  {private:
     RCCHAR rcstring[300],*rcs;
     char ctemp[600];
     char *string;
     int pos;
     //RCCHAR sname[256];
     char sname[256];
     SymbolType stype;
     double svalue;
     void nextsymbol(void);
     int factor(double *);
     int term(double *);
     int expression(double *);
   public:
     Expression(char *s);
     Expression(RCCHAR *s);
     int evaluate(long *);
     int evaluate(double *);
  };

class Calculator
  {private:
     double x,xop,y,m;
     int error,active,clear,degrees,lastop,opused,xcleared;
     RCCHAR svalue[256];
   public:
     Calculator() {  lastop = active = error = opused = 0;  degrees = 1;  clear = 1;  m = x = y = 0.0;  }
     int process(RCHWND hwnd,RCCHAR *);
     RCCHAR *getreal(void);
     RCCHAR *getinteger(void);
     double getvalue(RCCHAR *value);
     RCCHAR *process(int button,RCCHAR *value,Dialog *);
  };

class RTColourScrollBarDialogControl : public ScrollBarDialogControl
  {public:
     RTColourScrollBarDialogControl(int i,DialogControl *dc1) :
        ScrollBarDialogControl(i,dc1) {  }
     void vscrollevent(Dialog *,int,int,int);
     void setscrollpos(Dialog *,double);
  };

class RTColourDisplayDialogControl : public DisplayDialogControl
  {private:
     Dialog *parentdialog;
   public:
     RTColourDisplayDialogControl(int id,Dialog *d) : DisplayDialogControl(id)
        {  parentdialog = d;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class SelectColourButtonDialogControl : public ButtonDialogControl
{protected:
   int baseid;
public:
   RCCHAR title[200];
 public:
   SelectColourButtonDialogControl(int i,int bid) : ButtonDialogControl(i) {  baseid = bid; strcpy(title,_RCT("")); }
   int select(Dialog *);
};

class MPListDialogControl : public ListDialogControl
  {private:
     Figure *figure;
   public:
     MPListDialogControl(Figure *figure_,int id,RCCHAR *name,int length) :
         ListDialogControl(id,0,0,name,length)  {  figure = figure_;  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

class EIListDialogControl : public ListDialogControl
{
public:
    RCCHAR *labels;
    RCCHAR *values;
    EIListDialogControl(int id,int num,RCCHAR *l,RCCHAR *v) : ListDialogControl(id,num,&l) { labels = l; values = v; }
    void load(Dialog *);
};

class TextureDisplayDialogControl : public DisplayDialogControl
  {private:
     char lastfilename[300];
     int exists;
   public:
     TextureDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(lastfilename,"");  exists = 0;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

class LightVerifyDisplayControl : DisplayDialogControl
{
public:
    sunlightInfo *suninfo;
public:
    LightVerifyDisplayControl(int id, sunlightInfo *sli) : DisplayDialogControl(id) { suninfo = sli; }
    void draw(Dialog *,RCHDC);
};

class SysPathButtonDialogControl : public ButtonDialogControl
{
 public:
   SysPathButtonDialogControl(int i) : ButtonDialogControl(i) {  }
   int select(Dialog *);
};

// list selection control for the layout SelectView_Dialog
class SelectViewZoneListDialogControl : public ListDialogControl
{
public:
    int buddyId;
public:
     SelectViewZoneListDialogControl(int id,int bid, int n,RCCHAR **list,RCCHAR *index, int len) :
         ListDialogControl(id,n,list,index,len)  {buddyId = bid;}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
     int select(Dialog *);
};
#endif
