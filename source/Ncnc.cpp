
#include "ncwin.h"


/*  OrthotiCAD Interface
        
        Curves on layer 100 are cut using the end mill
        Triangulated patches on layer 100 are cut using the ball mill


        Select the OrthotiCAD post processor to generate the CNC data


*/

//#if (MENUS != STUDENTMENUS) && (MENUS != FLEXICADMENUS)

#define NCMillTaskName  _RCT("NCMill")
#define MAX_TOOLS 99

#if 0
D3DSetLight SetLight = 0;
D3DSetLightEx SetLightEx = 0;
D3DSetMaterial SetMaterial = 0;
D3DSetMaterialEx SetMaterialEx = 0;
D3DSetBackground SetBackground = 0;
D3DSetView SetView = 0;
D3DSetViewport SetViewport = 0;
D3DBeginScene BeginScene = 0;
D3DEndScene EndScene = 0;
D3DDrawTriangle DrawTriangle = 0;
D3DDrawPolygon DrawPolygon = 0;
D3DDrawTexturedTriangle DrawTexturedTriangle = 0;
D3DDrawTexturedTriangles DrawTexturedTriangles = 0;
D3DOpenOpenGl OpenOpenGl = 0;
D3DCloseOpenGl CloseOpenGl = 0;
D3DLoadTexture LoadTexture = 0;
D3DLoadMemoryTexture LoadMemoryTexture = 0;
D3DSetTexture SetTexture = 0;
D3DTriangulatePolygon TriangulatePolygon = 0;
D3DReleaseTextures ReleaseTextures = 0;
D3DFlush Flush = 0;
#endif

int LoadDirect3d(void)
{
#ifdef USING_WIDGETS
    return 0;
#if 0
  //QLibrary GLlib("/Users/ken/Documents/Projects/QTProjects/build-RealCADOpenGL-Desktop_Qt_5_2_1_clang_64bit-Debug/libRealCADOpenGL.dylib");
  QLibrary GLlib("libRealCADOpenGL");

  if(GLlib.isLoaded())
      qDebug() << "OpenGL Module Loaded";
  else
      qDebug() << "OpenGL Module NOT Loaded";

  if(GLlib.isLoaded())
  {
    OpenOpenGl = (D3DOpenOpenGl) GLlib.resolve("OpenOpenGl");
    CloseOpenGl = (D3DCloseOpenGl) GLlib.resolve("CloseOpenGl");

    OpenOpenGl(0,0,0,0,0,0);
    CloseOpenGl(0);

    return 1;
  }
  else
      return 0;

  if (SetLight != 0)
    return 0;

  SetLight = (D3DSetLight) SetLight;
  SetLightEx = (D3DSetLightEx)  SetLightEx ;
  SetMaterial = (D3DSetMaterial)  SetMaterial ;
  SetMaterialEx = (D3DSetMaterialEx)  SetMaterialEx ;
  SetBackground = (D3DSetBackground)  SetBackground ;
  SetView = (D3DSetView)   SetView ;
  SetViewport = (D3DSetViewport)  SetViewport
  BeginScene = (D3DBeginScene)  BeginScene ;
  EndScene = (D3DEndScene)  EndScene ;
  DrawTriangle = (D3DDrawTriangle) DrawTriangle ;
  DrawPolygon = (D3DDrawPolygon)  DrawPolygon ;
  DrawTexturedTriangle =  DrawTexturedTriangle ;
  DrawTexturedTriangles =  DrawTexturedTriangles ;
  OpenOpenGl = (D3DOpenOpenGl)  OpenOpenGl ;
  CloseOpenGl = (D3DCloseOpenGl) CloseOpenGl ;
  LoadTexture = (D3DLoadTexture)  LoadTexture ;
  LoadMemoryTexture =  (D3DLoadMemoryTexture)LoadMemoryTexture ;
  SetTexture =  (D3DSetTexture) SetTexture ;
  TriangulatePolygon =  (D3DTriangulatePolygon) TriangulatePolygon ;
  ReleaseTextures =  (D3DReleaseTextures) ReleaseTextures ;
  Flush = (D3DFlush)  Flush ;
  return 1;
#else
#if 0
  RCHINSTANCE hinstance;

  if (SetLight != 0)
    return 0;
  hinstance = LoadLibrary(_RCT("RealCADOpenGL.dll"));
  if (hinstance == 0)
    return 0;

  SetLight = (D3DSetLight) GetProcAddress(hinstance,"SetLight");
  SetLightEx = (D3DSetLightEx) GetProcAddress(hinstance,"SetLightEx");
  SetMaterial = (D3DSetMaterial) GetProcAddress(hinstance,"SetMaterial");
  SetMaterialEx = (D3DSetMaterialEx) GetProcAddress(hinstance,"SetMaterialEx");
  SetBackground = (D3DSetBackground) GetProcAddress(hinstance,"SetBackground");
  SetView = (D3DSetView) GetProcAddress(hinstance,"SetView");
  SetViewport = (D3DSetViewport) GetProcAddress(hinstance,"SetViewport");
  BeginScene = (D3DBeginScene) GetProcAddress(hinstance,"BeginScene");
  EndScene = (D3DEndScene) GetProcAddress(hinstance,"EndScene");
  DrawTriangle = (D3DDrawTriangle) GetProcAddress(hinstance,"DrawTriangle");
  DrawPolygon = (D3DDrawPolygon) GetProcAddress(hinstance,"DrawPolygon");
  DrawTexturedTriangle = (D3DDrawTexturedTriangle) GetProcAddress(hinstance,"DrawTexturedTriangle");
  DrawTexturedTriangles = (D3DDrawTexturedTriangles) GetProcAddress(hinstance,"DrawTexturedTriangles");
  OpenOpenGl = (D3DOpenOpenGl) GetProcAddress(hinstance,"OpenOpenGl");
  CloseOpenGl = (D3DCloseOpenGl) GetProcAddress(hinstance,"CloseOpenGl");
  LoadTexture = (D3DLoadTexture) GetProcAddress(hinstance,"LoadTexture");
  LoadMemoryTexture = (D3DLoadMemoryTexture) GetProcAddress(hinstance,"LoadMemoryTexture");
  SetTexture = (D3DSetTexture) GetProcAddress(hinstance,"SetTexture");
  TriangulatePolygon = (D3DTriangulatePolygon) GetProcAddress(hinstance,"TriangulatePolygon");
  ReleaseTextures = (D3DReleaseTextures) GetProcAddress(hinstance,"ReleaseTextures");
  Flush = (D3DFlush) GetProcAddress(hinstance,"Flush");
  return 1;
#endif
#endif
}

struct ToolInfo
  {int type;  
   double length,diameter,radang,unused;  
   Point offset;  
  };

class NCMillTask
  {private:
     RCCHAR currenttaskname[300];
     int currenttoolindex;
     Group *group;
     double width,height,length;
     Point home,origin,partorigin;

     void (WINAPI *NCInit)(RCCHAR *);
     void (WINAPI *NCToolChange)(int toolindex,double oldx,double oldy,double oldz,double x,double y,double z);
     void (WINAPI *NCClose)(void);
     void (WINAPI *NCSetCompensation)(int);
     void (WINAPI *NCLinearMovement)(int rapid,double x,double y,double z,int toolindex,double feed,double spindle);
     void (WINAPI *NCArcMovement)(int reverse,int plane,double ox,double oy,double oz,double a1,double a2,double radius,int toolindex,double feed,double spindle);
     void (WINAPI *NCDrillCycle)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z);
     void (WINAPI *NCPeckDrillCycle)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z);
     void (WINAPI *NCDeepPeckDrillCycle)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z);

   public:
     NCMillTask();
     ToolInfo tool[MAX_TOOLS];
     void cleartask(void)  {  strcpy(currenttaskname,"");  currenttoolindex = 1;  }
     void setcurrenttoolindex(int ti)  {  currenttoolindex = ti;  }
     int getcurrenttoolindex(void)  {  return currenttoolindex;  }
     void load(Group *);
     void excludegroup(void);
     void includegroup(void);
     RCCHAR *getcurrenttaskname(void)  {  return currenttaskname;  }
     double getwidth(void) {  return width;  }
     double getheight(void) {  return height;  }
     double getlength(void) {  return length;  }
     Point gethome(void) {  return home;  }
     Point getorigin(void) {  return origin;  }
     Point getpartorigin(void) {  return partorigin;  }
     void update(RCCHAR *,double,double,double,Point,Point,Point);
     void update(void);
     void insertbox(Group *);
     static void gettasks(RCCHAR *type,RCCHAR ***namelist,int *n);
     static Group *gettask(RCCHAR *type,RCCHAR *name);
     int checktoolpath(Group *);
     int deletetoolpath(Group *,int);
     int reordertoolpath(Group *,Group *,int);
     void inserttoolpath(Group *,Group *,int);
     void insertrapid(Group *,EntityList,int,int,int);
     void testrapid(Group *);
     void insertfeed(Group *,EntityList,int,int,int,double,double);
     void testfeed(Group *);
     void insertdrill(Group *,EntityList,int,int,int,double,double,double,double,double);
     void testdrill(Group *);
     void insertcontour(Group *,EntityList,int,int,int,int,int,double,double,double,double,double,double,int,double,double);
     void insertcontour(Group *,UVSurfaceEntity *,Point,int,int,int,int,int,double,double,double,double,double,double,int,double,double,int,double,double);
     void testcontour(Group *);
     int traverse(void);
     void test(int,int,double,int,int);
     void generate(RCCHAR *,RCCHAR *);
     void draw(int toolindex,int drawmode,Point);
  } ncmilltask;

NCMenuWindow::NCMenuWindow(Window *win,int x,int y,int w,int h,NCActive *aa) :
  MenuWindow(win,x,y,w,h)
{ ncactive = aa;
  buttonmenu = ncactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  ncactive->setactive(1);
}

NCMenuWindow::~NCMenuWindow()
{ ncactive->setactive(0);
}

NCActive ncactive;

extern void nc_command1(int *cstate,HighLevelEvent *event,void **);
extern void nc_command2(int *cstate,HighLevelEvent *event,void **);
extern void nc_command3(int *cstate,HighLevelEvent *event,void **);
extern void nc_command4(int *cstate,HighLevelEvent *event,void **);
extern void nc_command5(int *cstate,HighLevelEvent *event,void **);
extern void nc_command6(int *cstate,HighLevelEvent *event,void **);
extern void nc_command7(int *cstate,HighLevelEvent *event,void **);

extern int check_license(char *,char *,char *,int application,int showmessages);
extern int check_license(RCCHAR *,RCCHAR *,RCCHAR *,int application,int showmessages);

void nc_command(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
  check_license((char*)"Machining Application", (char*)"RealCADKey7", (char*)"RealCADKey8",3,1);
  if (ncactive.getactive()) return;
  if (ncactive.getbuttonmenu() == NULL)
    {ButtonMenu *menu = new ButtonMenu(0,0,12,12,15);
       if (menu != NULL)
         {  commands.add(1,20100,0,nc_command1, (char*)"Define task and stock");
            commands.add(1,20101,0,nc_command2, (char*)"Define tools");
            commands.add(1,20102,0,nc_command3, (char*)"Insert toolpath");
            commands.add(1,20103,0,nc_command4, (char*)"Delete toolpath");
            commands.add(1,20104,0,nc_command5, (char*)"Reorder toolpath");
            commands.add(1,20105,0,nc_command6, (char*)"Test and generate NC file");
            commands.add(1,20106,0,nc_command7, (char*)"OrthotiCAD Tool Path");
            menu->add(new Button(-1,-1, (char*)"NC1",20100));
            menu->add(new Button(-1,-1, (char*)"NC2",20101));
            menu->add(new Button(-1,-1, (char*)"NC3",20102));
            menu->add(new Button(-1,-1, (char*)"NC4",20103));
            menu->add(new Button(-1,-1, (char*)"NC5",20104));
            menu->add(new Button(-1,-1, (char*)"NC6",20105));
            menu->add(new Button(-1,-1, (char*)"NC7",20106));
            ncactive.setbuttonmenu(menu);
         }
    }
  if (ncactive.getbuttonmenu() == NULL)
    {  cadwindow->MessageBox("Failed to create the machining button menu","Machining",MB_ICONSTOP);
       return;
    }
  new NCMenuWindow(cadwindow->getsubwindow(),
                          int(cadwindow->getsubwindow()->getwidth())-160,0,
                          48,96,&ncactive);
  *cstate = ExitState;
}

int isatoolpath(EntityList list)
{Entity *e;
  if (list.length() != 1)
    return 0;
  list.start();
  e = list.next();
  return e->isa(group_entity) && strncmp(((Group *)e)->getclassname(),"Toolpath::",10) == 0;
}

static int cdisable0l[] = {  307,311,319,-100,-100,-102,-103,-105,-120,0  };
static int cdisable1l[] = {  311,319,-100,-100,-102,-103,-105,-106,-108,-109,-111,-120,0  };
static int cdisable2l[] = {  311,-100,-100,-102,-103,-105,-105,-108,-109,-111,-118,0  };
static int cdisable3l[] = {  319,-100,-100,-119,-120,0  };

static int *cdisablel[] = {  cdisable0l,cdisable1l,cdisable2l,cdisable3l  };

class NCChangeListDialogControl : public ListDialogControl
  {private:
    int toolpathtype;
   public:
     NCChangeListDialogControl(int tpt,int id,int n,RCCHAR **list,int *v) : ListDialogControl(id,n,list,v) {  toolpathtype = tpt;  }
     void load(Dialog *);
  };

void NCChangeListDialogControl::load(Dialog *dialog)
{int i,j,enabled;
  for (i = 100 ; i < 1121 ; i++)
    if (GetDlgItem(dialog->gethdlg(),i) != 0)
      {  enabled = 1;
         for (j = 0 ; cdisablel[toolpathtype][j] != 0 ; j++)
           {  if (cdisablel[toolpathtype][j] == i)
                {  enabled = 0;
                   break;
                }
              if (cdisablel[toolpathtype][j] < 0)
                {  if (-cdisablel[toolpathtype][j] <= i && i <= -cdisablel[toolpathtype][j+1] ||
                       -cdisablel[toolpathtype][j]+100 <= i && i <= -cdisablel[toolpathtype][j+1]+100 ||
                       -cdisablel[toolpathtype][j]+1000 <= i && i <= -cdisablel[toolpathtype][j+1]+1000)
                     {  enabled = 0;
                        break;
                     }
                   else
                     j++;
                }
           }
         EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
         EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
         EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
      }
  ListDialogControl::load(dialog);
}

void modifytoolpath(Group *tp)
{Dialog dialog("NCChange_Dialog");
 DialogControl *dc;
 int tpt,toolindex,approach,compensation,retractm,toolpathtype,useentityz;
 double feed,spindle,retract,holedepth,peck,plungefeed,retractfeed,initialz,finalz,depthcut;
 static RCCHAR *approachl[] = {  _RCT("Direct"),  _RCT("At depth"), _RCT("At height")  };
 static RCCHAR *compensationl[] = {  _RCT("None"), _RCT("Left"), _RCT("Right")  };
 static RCCHAR *toolpathtypel[] = {  _RCT("Peck drill"), _RCT("Deep peck drill"), _RCT("Drill") };

  toolindex = ncmilltask.getcurrenttoolindex();
  toolpathtype = 0;
  approach = v.getinteger("nc::approach");
  feed = v.getreal("nc::feed");
  spindle = v.getreal("nc::spindle");
  retract = v.getreal("nc::retract");
  holedepth = v.getreal("nc::holedepth");
  peck = v.getreal("nc::peck");
  compensation = v.getinteger("nc::compensation");
  retractm = v.getinteger("nc::retractm");
  plungefeed = v.getreal("nc::plungefeed");
  retractfeed = v.getreal("nc::retractfeed");
  initialz = v.getreal("nc::initialz");
  useentityz = v.getinteger("nc::useentityz");
  finalz = v.getreal("nc::finalz");
  depthcut = v.getreal("nc::depthcut");

  if (strcmp(tp->getclassname(),"Toolpath::Rapid") == 0)
    {  sscanf(tp->getinstance(),"Toolpath::%d,%d",&toolindex,&approach);
       tpt = 0;
    }
  else if (strcmp(tp->getclassname(),"Toolpath::Feed") == 0)
    {  sscanf(tp->getinstance(),"Toolpath::%d,%d,%lf,%lf",&toolindex,&approach,&feed,&spindle);
       tpt = 1;
    }
  else if (strcmp(tp->getclassname(),"Toolpath::Drill") == 0)
    {  sscanf(tp->getinstance(),"Toolpath::%d,%d,%lf,%lf,%lf,%lf,%lf",&toolindex,&toolpathtype,&feed,&spindle,&retract,&holedepth,&peck);
       tpt = 2;
       toolpathtype -= 4;
    }
  else if (strcmp(tp->getclassname(),"Toolpath::Contour") == 0)
    {  sscanf(tp->getinstance(),"Toolpath::%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf",
            &toolindex,&compensation,&approach,&retractm,&retract,&feed,&plungefeed,&retractfeed,&spindle,&initialz,&useentityz,&finalz,&depthcut);
       tpt = 3;
    }
  dialog.add(dc = new IntegerDialogControl(101,&toolindex,1,99));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(new ListDialogControl(103,3,compensationl,&compensation));
  dialog.add(new ListDialogControl(104,3,approachl,&approach));
  dialog.add(new ListDialogControl(105,3,approachl,&retractm));
  dialog.add(dc = new RealDialogControl(106,&retract,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&feed,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(dc = new RealDialogControl(108,&plungefeed,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(dc = new RealDialogControl(109,&retractfeed,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(dc = new RealDialogControl(110,&spindle));
  dialog.add(new ScrollBarDialogControl(1110,dc));
  dialog.add(dc = new RealDialogControl(111,&initialz,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(new CheckBoxDialogControl(112,&useentityz));
  dialog.add(dc = new RealDialogControl(113,&finalz));
  dialog.add(new ScrollBarDialogControl(1113,dc));
  dialog.add(dc = new RealDialogControl(114,&depthcut,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1114,dc));
  dialog.add(dc = new RealDialogControl(119,&holedepth,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1119,dc));
  dialog.add(dc = new RealDialogControl(120,&peck,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1120,dc));
  dialog.add(dc = new NCChangeListDialogControl(tpt,100,3,toolpathtypel,&toolpathtype));
  if (dialog.process() == TRUE)
    {BitMask change(32),options(32);
     RCCHAR instance[300];
       if (strcmp(tp->getclassname(),"Toolpath::Rapid") == 0)
         sprintf(instance,"Toolpath::%d,%d",toolindex,approach);
       else if (strcmp(tp->getclassname(),"Toolpath::Feed") == 0)
         sprintf(instance,"Toolpath::%d,%d,%lf,%lf",toolindex,approach,feed,spindle);
       else if (strcmp(tp->getclassname(),"Toolpath::Drill") == 0)
         sprintf(instance,"Toolpath::%d,%d,%lf,%lf,%lf,%lf,%lf",toolindex,toolpathtype+4,feed,spindle,retract,holedepth,peck);
       else if (strcmp(tp->getclassname(),"Toolpath::Contour") == 0)
         sprintf(instance,"Toolpath::%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf",
                 toolindex,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,useentityz,finalz,depthcut);
       change.set(5);
       tp->change(change,*tp,_RCT(""),instance,options);
    }
}

extern "C" void WINAPI showncmenu(void);

void WINAPI showncmenu(void)
{int state;
  state = InitialState;
  nc_command(&state,0,0);
}

void clearnctask(void)
{  ncmilltask.cleartask();
}

NCMillTask::NCMillTask()
{int i;
  strcpy(currenttaskname,"");
  for (i = 0 ; i < MAX_TOOLS ; i++)
    {  tool[i].type = 0;
       tool[i].length = 40.0;
       tool[i].diameter = 10.0;
       tool[i].radang = tool[i].unused = 0.0;
       tool[i].offset.setxyz(0.0,0.0,0.0);
    }
  group = 0;
}

void NCMillTask::load(Group *g)
{Entity *e;
 EntityList list;
 AttributeList atlist;
 RCCHAR *at;
 int i,toolindex,tooltype;
 double tlength,diameter,radang,unused;
 Point offset;
  width = v.getreal("nc::millwidth");
  length = v.getreal("nc::milllength");
  height = v.getreal("nc::millheight");
  origin.x = v.getreal("nc::milloriginx");
  origin.y = v.getreal("nc::milloriginy");
  origin.z = v.getreal("nc::milloriginz");
  partorigin.x = v.getreal("nc::millpartx");
  partorigin.y = v.getreal("nc::millparty");
  partorigin.z = v.getreal("nc::millpartz");
  home.x = v.getreal("nc::millhomex");
  home.y = v.getreal("nc::millhomey");
  home.z = v.getreal("nc::millhomez");
  for (i = 0 ; i < MAX_TOOLS ; i++)
    {  tool[i].type = 0;
       tool[i].length = 40.0;
       tool[i].diameter = 10.0;
       tool[i].radang = tool[i].unused = 0.0;
       tool[i].offset.setxyz(0.0,0.0,0.0);
    }
  currenttoolindex = 1;
  group = g;
  if (g != 0)
    {  list = g->getlist();
       list.start();
       if ((e = list.next()) != 0)
         origin = ((PointE *)e)->getp();
       if ((e = list.next()) != 0)
         partorigin = ((PointE *)e)->getp();
       if ((e = list.next()) != 0)
         home = ((PointE *)e)->getp();
       sscanf(g->getinstance()+strlen(NCMillTaskName)+2,"W%lf,L%lf,H%lf",&width,&length,&height);
       strcpy(currenttaskname,group->getclassname()+strlen(NCMillTaskName)+2);
       atlist = g->getatlist().copy();
       currenttoolindex = -1;
       for (atlist.start() ; (at = atlist.next()) != NULL ; )
         {  sscanf(at+strlen(NCMillTaskName)+2,"%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                    &toolindex,&tooltype,&tlength,&diameter,&radang,&unused,&offset.x,&offset.y,&offset.z);
            tool[toolindex-1].type = tooltype;
            tool[toolindex-1].length = tlength;
            tool[toolindex-1].diameter = diameter;
            tool[toolindex-1].radang = radang;
            tool[toolindex-1].unused = unused;
            tool[toolindex-1].offset = offset;
            if (currenttoolindex <= 0)
              currenttoolindex = toolindex;
         }
       if (currenttoolindex <= 0)
         currenttoolindex = 1;
       atlist.destroy();
    }
  else
    strcpy(currenttaskname,"");
}

void NCMillTask::insertbox(Group *g)
{Line *line;
  db.geometry.add(line = new Line(*g,origin,origin+Point(length,0.0,0.0)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,0.0,0.0),origin+Point(length,width,0.0)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,width,0.0),origin+Point(0.0,width,0.0)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(0.0,width,0.0),origin+Point(0.0,0.0,0.0)));  g->add(line);

  db.geometry.add(line = new Line(*g,origin+Point(0.0,0.0,height),origin+Point(length,0.0,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,0.0,height),origin+Point(length,width,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,width,height),origin+Point(0.0,width,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(0.0,width,height),origin+Point(0.0,0.0,height)));  g->add(line);

  db.geometry.add(line = new Line(*g,origin+Point(0.0,0.0,0.0),origin+Point(0.0,0.0,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,0.0,0.0),origin+Point(length,0.0,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(length,width,0.0),origin+Point(length,width,height)));  g->add(line);
  db.geometry.add(line = new Line(*g,origin+Point(0.0,width,0.0),origin+Point(0.0,width,height)));  g->add(line);

}

void NCMillTask::update(RCCHAR *n,double w,double l,double h,Point o,Point po,Point hm)
{Entity *e,*e1;
 EntityList list,list1;
 RCCHAR classname[300],instance[300],string[300],*at;
 BitMask change(32),options(32);
 Transform t;
 Group *g;
 int i;
 AttributeList atlist;

  state.selmask.layer.clear(999);
  width = w;
  length = l;
  height = h;
  origin = o;
  partorigin = po;
  home = hm;

  db.saveundo(UD_STARTBLOCK,NULL);
  sprintf(instance,"%s::W%lf,L%lf,H%lf",NCMillTaskName,width,length,height);
  if (group != 0)
    {  change.set(5);
       group->change(change,*group,_RCT(""),instance,options);
       list = group->getlist();
       list.start();
       if ((e = list.next()) != 0)
         {  t.identity();
            t.translate(origin-((PointE *)e)->getp());
            group->setstatus(1);
            e->setcopye(0);
            ((PointE *)e)->move(&db,t);
            ((PointE *)e)->draw(DM_NORMAL);
         }
       if ((e = list.next()) != 0)
         {  t.identity();
            t.translate(partorigin-((PointE *)e)->getp());
            group->setstatus(1);
            e->setcopye(0);
            ((PointE *)e)->move(&db,t);
            ((PointE *)e)->draw(DM_NORMAL);
         }
       if ((e = list.next()) != 0)
         {  t.identity();
            t.translate(home-((PointE *)e)->getp());
            group->setstatus(1);
            e->setcopye(0);
            ((PointE *)e)->move(&db,t);
            ((PointE *)e)->draw(DM_NORMAL);
         }
       if ((e = list.next()) != 0)
         {  list1 = ((Group *)e)->getlist();
            for (list1.start() ; (e1 = list1.next()) != 0 ; )
              {  ((Group *)e)->del(e1);
                 db.geometry.del(e1);
              }
            insertbox((Group *)e);
         }
       strcpy(currenttaskname,group->getclassname()+strlen(NCMillTaskName)+2);
       atlist = group->getatlist().copy();
       for (atlist.start() ; (at = atlist.next()) != NULL ; )
         {  group->delat(at);  group->setattribute(at);
            db.saveundo(UD_DELETEATT,group);
            delete at;
         }
       atlist.destroy();
    }
  else
    {  g = new Group(list1,"NC::Toolpaths","Toolpaths",options);
       ((EntityHeader *)g)->change(g->getcolour(),999,g->getstyle(),g->getweight());
       db.geometry.add(g);
       list.add(g);
       g = new Group(list1,"NC::Graphics","Stock",options);
       db.geometry.add(g);
       insertbox(g);
       list.add(g);
       e = new PointE(home);
       db.geometry.add(e);
       list.add(e);
       e = new PointE(partorigin);
       db.geometry.add(e);
       list.add(e);
       e = new PointE(origin);
       db.geometry.add(e);
       list.add(e);
       sprintf(classname,"%s::%s",NCMillTaskName,n);
       group = new Group(list,classname,instance,options);
       ((EntityHeader *)group)->change(group->getcolour(),999,group->getstyle(),group->getweight());
       db.geometry.add(group);
       strcpy(currenttaskname,n);
    }
  for (i = 0 ; i < MAX_TOOLS ; i++)
    if (tool[i].type != 0)
      {  sprintf(string,"%s::%d,%d,%f,%f,%f,%f,%f,%f,%f",NCMillTaskName,
                    i+1,tool[i].type,tool[i].length,tool[i].diameter,tool[i].radang,tool[i].unused,tool[i].offset.x,tool[i].offset.y,tool[i].offset.z);
         if ((at = new RCCHAR[strlen(string) + 1]) != 0)
           {  strcpy(at,string);
              group->addat(at);
              group->setattribute(at);
              db.saveundo(UD_INSERTATT,group);
           }
      }
  db.saveundo(UD_ENDBLOCK,NULL);
}

void NCMillTask::update(void)
{EntityList list,list1;
 RCCHAR string[300],*at;
 int i;
 AttributeList atlist;

  if (group != 0)
    {  db.saveundo(UD_STARTBLOCK,NULL);
       atlist = group->getatlist().copy();
       for (atlist.start() ; (at = atlist.next()) != NULL ; )
         {  group->delat(at);  group->setattribute(at);
            db.saveundo(UD_DELETEATT,group);
            delete at;
         }
       atlist.destroy();
       for (i = 0 ; i < MAX_TOOLS ; i++)
         if (tool[i].type != 0)
           {  sprintf(string,"%s::%d,%d,%f,%f,%f,%f,%f,%f,%f",NCMillTaskName,
                         i+1,tool[i].type,tool[i].length,tool[i].diameter,tool[i].radang,tool[i].unused,tool[i].offset.x,tool[i].offset.y,tool[i].offset.z);
              if ((at = new RCCHAR[strlen(string) + 1]) != 0)
                {  strcpy(at,string);
                   group->addat(at);
                   group->setattribute(at);
                   db.saveundo(UD_INSERTATT,group);
                }
           }
       db.saveundo(UD_ENDBLOCK,NULL);
    }
}

void NCMillTask::gettasks(RCCHAR *type,RCCHAR ***namelist,int *n)
{Entity *e;
 int n1;
  n1 = 0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strncmp(((Group *)e)->getclassname(),type,strlen(type)) == 0)
      n1++;
  *n = 0;
  if ((*namelist = new RCCHAR *[n1]) != 0)
    {  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(group_entity) && strncmp(((Group *)e)->getclassname(),type,strlen(type)) == 0)
           (*namelist)[(*n)++] = ((Group *)e)->getclassname() + strlen(type) + 2;
    }
}

Group *NCMillTask::gettask(RCCHAR *type,RCCHAR *name)
{Entity *e;
 RCCHAR string1[300],string2[300];
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strncmp(((Group *)e)->getclassname(),type,strlen(type)) == 0)
      {  strcpy(string1,((Group *)e)->getclassname() + strlen(type) + 2);  _strupr(string1);
         strcpy(string2,name);  _strupr(string2);
         if (strcmp(string1,string2) == 0)
           return (Group *)e;
      }
  return 0;
}


int NCMillTask::checktoolpath(Group *toolpath)
{EntityList list,list1;
 Entity *e;
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    {  if (! ((Group *)e)->getlist().inlist(toolpath))
         return 0;
       return 1;
    }
  return 0;
}

int NCMillTask::deletetoolpath(Group *toolpath,int deleteentities)
{EntityList list,list1;
 Entity *e;
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    {  if (! ((Group *)e)->getlist().inlist(toolpath))
         return 0;
       db.saveundo(UD_STARTBLOCK,NULL);
       ((Group *)e)->del(toolpath);
       if (deleteentities)
         {  while (toolpath->getlist().length() > 0)
              {  list1 = toolpath->getlist();
                 list1.start();
                 e = list1.next();
                 toolpath->del(e);
                 db.geometry.del(e);
              }
         }
       db.geometry.del(toolpath,0);
       db.saveundo(UD_ENDBLOCK,NULL);
       return 1;
    }
  return 0;
}

void NCMillTask::excludegroup(void)
{EntityList list;
 Entity *e;
  if (group == 0)
    return;
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    state.excludelist.add(e);
  state.excludelist.add(group);
}

void NCMillTask::includegroup(void)
{EntityList list;
 Entity *e;
  if (group == 0)
    return;
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    state.excludelist.del(e);
  state.excludelist.del(group);
}

int NCMillTask::reordertoolpath(Group *toolpath,Group *reftoolpath,int reorder)
{EntityList list,list1;
 int position;
 Group *tp;
 Entity *e;
  db.saveundo(UD_STARTBLOCK,NULL);
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    {  list1 = ((Group *)e)->getlist();
       if (reorder == 0)
         position = 0;
       else if (reorder == 1)
         position = list1.length();
       else
         {  for (list1.start(),position = 1 ; (tp = (Group *)list1.next()) != 0 ; position++)
              if (tp == reftoolpath)
                break;
            if (position > list1.length())
              position = list1.length();
         }
       ((Group *)e)->del(toolpath);
       ((Group *)e)->add(position,toolpath);
    }
  db.saveundo(UD_ENDBLOCK,NULL);
  return 1;
}

void NCMillTask::inserttoolpath(Group *newtoolpath,Group *toolpath,int order)
{EntityList list,list1;
 int position;
 Group *tp;
 Entity *e;
  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    {  list1 = ((Group *)e)->getlist();
       if (order == 0)
         position = 0;
       else if (order == 1)
         position = list1.length();
       else
         {  for (list1.start(),position = 1 ; (tp = (Group *)list1.next()) != 0 ; position++)
              if (tp == toolpath)
                break;
            if (position > list1.length())
              position = list1.length();
         }
       ((Group *)e)->add(position,newtoolpath);
    }
}

void NCMillTask::draw(int toolindex,int drawmode,Point origin)
{double d,d1,r,ra,l;
 BitMask visible(MaxViews);

  if (toolindex < 1 || toolindex > MAX_TOOLS || tool[toolindex-1].type == 0)
    return;
  d = tool[toolindex-1].diameter;
  r = d / 2.0;
  l = tool[toolindex-1].length;
  ra = tool[toolindex-1].radang;
  if (usingd3d)
    {  visible = *db.header.getvisible();
       db.header.getvisible()->clear(cadwindow->getcurrentwindow()->getview()->getviewno());
    }

  if (tool[toolindex-1].type == 1)
    {  Line line1(origin+Point(-r,0.0,0.0),origin+Point(r,0.0,0.0));  line1.draw(drawmode);
       Line line2(origin+Point(0.0,-r,0.0),origin+Point(0.0,r,0.0));  line2.draw(drawmode);
       Line line3(origin+Point(r,0.0,0.0),origin+Point(r,0.0,l));  line3.draw(drawmode);
       Line line4(origin+Point(-r,0.0,0.0),origin+Point(-r,0.0,l));  line4.draw(drawmode);
       Line line5(origin+Point(0.0,r,0.0),origin+Point(0.0,r,l));  line5.draw(drawmode);
       Line line6(origin+Point(0.0,-r,0.0),origin+Point(0.0,-r,l));  line6.draw(drawmode);
       Circle circle7(origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle7.draw(drawmode);
       Circle circle8(origin+Point(0.0,0.0,l/2),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle8.draw(drawmode);
    }
  else if (tool[toolindex-1].type == 2)
    {  Line line1(origin+Point(r,0.0,r),origin+Point(r,0.0,r+l));  line1.draw(drawmode);
       Line line2(origin+Point(-r,0.0,r),origin+Point(-r,0.0,r+l));  line2.draw(drawmode);
       Line line3(origin+Point(0.0,r,r),origin+Point(0.0,r,r+l));  line3.draw(drawmode);
       Line line4(origin+Point(0.0,-r,r),origin+Point(0.0,-r,r+l));  line4.draw(drawmode);
       Circle circle5(origin+Point(0.0,0.0,r),Point(1.0,0.0,0.0),Point(0.0,0.0,1.0),r,0.0,-M_PI);  circle5.draw(drawmode);
       Circle circle6(origin+Point(0.0,0.0,r),Point(0.0,1.0,0.0),Point(0.0,0.0,1.0),r,0.0,-M_PI);  circle6.draw(drawmode);
       Circle circle7(origin+Point(0.0,0.0,r),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle7.draw(drawmode);
       Circle circle8(origin+Point(0.0,0.0,r+l/2),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle8.draw(drawmode);
    }
  else if (tool[toolindex-1].type == 3)
    {  ra *= M_PI / 180.0;
       if (fabs(tan(ra/2.0)) > 1.0E-10)
         d1 = r / tan(ra / 2.0);
       else
         d1 = 0.0;
       Line line1(origin+Point(-r,0.0,d1),origin);  line1.draw(drawmode);
       Line line2(origin+Point(0.0,-r,d1),origin);  line2.draw(drawmode);
       Line line3(origin+Point(r,0.0,d1),origin);  line3.draw(drawmode);
       Line line4(origin+Point(0.0,r,d1),origin);  line4.draw(drawmode);
       Line line5(origin+Point(r,0.0,d1),origin+Point(r,0.0,d1+l));  line5.draw(drawmode);
       Line line6(origin+Point(-r,0.0,d1),origin+Point(-r,0.0,d1+l));  line6.draw(drawmode);
       Line line7(origin+Point(0.0,r,d1),origin+Point(0.0,r,d1+l));  line7.draw(drawmode);
       Line line8(origin+Point(0.0,-r,d1),origin+Point(0.0,-r,d1+l));  line8.draw(drawmode);
       Circle circle7(origin+Point(0.0,0.0,d1),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle7.draw(drawmode);
       Circle circle8(origin+Point(0.0,0.0,d1+l/2),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle8.draw(drawmode);
    }
  else if (tool[toolindex-1].type == 4)
    {  Line line1(origin+Point(-r+ra,0.0,0.0),origin+Point(r-ra,0.0,0.0));  line1.draw(drawmode);
       Line line2(origin+Point(0.0,-r+ra,0.0),origin+Point(0.0,r-ra,0.0));  line2.draw(drawmode);
       Line line3(origin+Point(r,0.0,ra),origin+Point(r,0.0,ra+l));  line3.draw(drawmode);
       Line line4(origin+Point(-r,0.0,ra),origin+Point(-r,0.0,ra+l));  line4.draw(drawmode);
       Line line5(origin+Point(0.0,r,ra),origin+Point(0.0,r,ra+l));  line5.draw(drawmode);
       Line line6(origin+Point(0.0,-r,ra),origin+Point(0.0,-r,ra+l));  line6.draw(drawmode);
       Circle circle7(origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r-ra,0.0,2.0*M_PI);  circle7.draw(drawmode);
       Circle circle8(origin+Point(0.0,0.0,ra),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle8.draw(drawmode);
       Circle circle9(origin+Point(0.0,0.0,ra+l/2),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),r,0.0,2.0*M_PI);  circle9.draw(drawmode);
       Circle circle10(origin+Point(r-ra,0.0,ra),Point(1.0,0.0,0.0),Point(0.0,0.0,1.0),ra,0.0,-M_PI/2.0);  circle10.draw(drawmode);
       Circle circle11(origin+Point(-r+ra,0.0,ra),Point(1.0,0.0,0.0),Point(0.0,0.0,1.0),ra,M_PI,M_PI/2.0);  circle11.draw(drawmode);
       Circle circle12(origin+Point(0.0,r-ra,ra),Point(0.0,1.0,0.0),Point(0.0,0.0,1.0),ra,0.0,-M_PI/2.0);  circle12.draw(drawmode);
       Circle circle13(origin+Point(0.0,-r+ra,ra),Point(0.0,1.0,0.0),Point(0.0,0.0,1.0),ra,M_PI,M_PI/2.0);  circle13.draw(drawmode);
    }
  if (usingd3d)
    *db.header.getvisible() = visible;
}

//  Implementation of the simulator is done using standard WAPI calls so that simulation and generation (in the DLL) are identical

     //  These are used during simulation only
static Point lastposition;
static int lasttoolindex,linetrails,tooltrails,generatemodel,stopped,frame;
static double distance,stepsize,displayevery;
static NCSolid *ncsolid;
#if defined(AVI)
static PAVIFILE avifile;
static PAVISTREAM avistream,avicompressedstream;
#endif

static int direct3dcb(SurfaceEntity *se,void *,int nvert,void *pts,void *)
{Point *p = (Point *) pts;
 Point n,l(1000.0,4000.0,20000.0);
  if (nvert > 0)
    {  n = (p[0]-p[1]).cross(p[2]-p[1]).normalize();
       if (n.dot(l - p[0]) < 0.0)
         n = -n;
       SetMaterial(db.colourtable[se->getcolour()].red/255.0,db.colourtable[se->getcolour()].green/255.0,db.colourtable[se->getcolour()].blue/255.0,0.3,0.7,1.0);
       if (nvert == 3)
         DrawTriangle(p[0].x,p[0].y,p[0].z,p[1].x,p[1].y,p[1].z,p[2].x,p[2].y,p[2].z,n.x,n.y,n.z);
       else if (nvert == 4)
         {  DrawTriangle(p[0].x,p[0].y,p[0].z,p[1].x,p[1].y,p[1].z,p[2].x,p[2].y,p[2].z,n.x,n.y,n.z);
            DrawTriangle(p[0].x,p[0].y,p[0].z,p[2].x,p[2].y,p[2].z,p[3].x,p[3].y,p[3].z,n.x,n.y,n.z);
         }                                                     
    }
  return 1;
}

void WINAPI NCTestLinearMovement(int rapid,double x,double y,double z,int toolindex,double,double)
{EntityHeader header;
 Point p(x,y,z),p1;
 DWORD time;
 double t;
 long i,n;
 Point pmin,pmax;
 BitMask visible(MaxViews);
  if (stopped)
    return;
  if (linetrails)
    {  if (rapid)
         header.change(0,state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
       else
         header.change(1,state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
       if (usingd3d)
         header.getvisible()->clear(cadwindow->getcurrentwindow()->getview()->getviewno());
       Line line(header,lastposition,p);
       line.draw(DM_NORMAL);
    }
  n = int((lastposition - p).length() / stepsize);
  if (n < 2) n = 2;
  for (i = 0 ; i <= n ; i++)
    {  t = double(i) / double(n);
       if (tooltrails)
         ncmilltask.draw(toolindex,DM_GRID,lastposition + (p - lastposition) * t);
       else if (generatemodel)
         {  distance += (p - lastposition).length() / n;
            ncsolid->removetool(lastposition + (p - lastposition) * t);
            if (distance > displayevery)
              {  cadwindow->invalidatedisplaylist(ncsolid);
                 if (generatemodel && ncsolid != 0)
                   {  //ncsolid->getdirty(&pmin,&pmax);
                      //ncsolid->cleardirty();
                      //ncsolid->removetool(lastposition + (p - lastposition) * t);
                      //cadwindow->setdirtyall(pmin,pmax);
                      if (usingd3d != 0)
                        {BitMask foptions(32);
                           foptions.set(1);
                           BeginScene(0);
                           SetLight(1000.0,4000.0,20000.0);
                           ncsolid->facet(foptions,0.0,0,0,0,direct3dcb,0);
                           EndScene();
                        }
                   }

                 //cadwindow->paintall(0,1+2*(usingd3d!=0));
                 //program->processpaintmessages();
                 //cadwindow->cleardirtyall();
                 distance = 0.0;
#if defined(AVI)
                 if (avicompressedstream != 0 && cadwindow->getcurrentwindow() != 0)
                   cadwindow->getcurrentwindow()->saveavistream(avicompressedstream,frame);
                 frame++;
#endif
              }
         }
       else
         {  ncmilltask.draw(toolindex,DM_INVERT,lastposition + (p - lastposition) * t);
#if ! defined(_WIN32_WCE)
            time = timeGetTime();
            while (time - timeGetTime() < 2);
#endif
            ncmilltask.draw(toolindex,DM_INVERT,lastposition + (p - lastposition) * t);
         }
       if (stopped || (stopped = program->getinterrupt()) != 0) break;
    }
  lastposition = p;
}

void WINAPI NCTestToolChange(int toolindex,double oldhomex,double oldhomey,double oldhomez,double homex,double homey,double homez)
{  if (lasttoolindex != -1)
     NCTestLinearMovement(1,oldhomex,oldhomey,oldhomez,toolindex,0.0,0.0);
   lastposition = Point(homex,homey,homez);
   lasttoolindex = toolindex;
   if (generatemodel)
     {  if (toolindex >= 1 && toolindex < MAX_TOOLS)
          ncsolid->definetool(ncmilltask.tool[toolindex-1].type,ncmilltask.tool[toolindex-1].diameter,ncmilltask.tool[toolindex-1].radang,ncmilltask.tool[toolindex-1].length);
        else
          ncsolid->definetool(0,1.0,0.0,0.0);
     }
}

void NCMillTask::insertrapid(Group *toolpath,EntityList list,int toolindex,int order,int approach)
{Group *newtoolpath;
 RCCHAR classname[300],instance[300];
 BitMask options(32);
  sprintf(classname,"Toolpath::Rapid");
  sprintf(instance,"Toolpath::%d,%d",toolindex,approach);
  db.geometry.add(newtoolpath = new Group(list,classname,instance,options));
  inserttoolpath(newtoolpath,toolpath,order);
}

void NCMillTask::testrapid(Group *tp)
{int toolindex,approach;
 EntityList list;
 PointE *p;
 Point p1,p2;
  if (stopped)
    return;
  sscanf(tp->getinstance(),"Toolpath::%d,%d",&toolindex,&approach);
  if (lasttoolindex != toolindex)
    {  if (lasttoolindex != -1)
         p1 = ncmilltask.gethome() + ncmilltask.tool[lasttoolindex-1].offset;
       else
         p1.setxyz(0.0,0.0,0.0);
       p2 = ncmilltask.gethome() + ncmilltask.tool[toolindex].offset;
       NCToolChange(toolindex,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
    }
  list = tp->getlist();
  for (list.start() ; (p = (PointE *)list.next()) != 0 ; )
    {  NCLinearMovement(1,p->getp().x,p->getp().y,p->getp().z,toolindex,0.0,0.0);
       if (stopped)
         break;
    }
}

void NCMillTask::insertfeed(Group *toolpath,EntityList list,int toolindex,int order,int approach,double feed,double spindle)
{Group *newtoolpath;
 RCCHAR classname[300],instance[300];
 BitMask options(32);
  sprintf(classname,"Toolpath::Feed");
  sprintf(instance,"Toolpath::%d,%d,%lf,%lf",toolindex,approach,feed,spindle);
  db.geometry.add(newtoolpath = new Group(list,classname,instance,options));
  inserttoolpath(newtoolpath,toolpath,order);
}

void NCMillTask::testfeed(Group *tp)
{int toolindex,approach;
 double feed,spindle;
 EntityList list;
 PointE *p;
 Point p1,p2;
  if (stopped)
    return;
  sscanf(tp->getinstance(),"Toolpath::%d,%d,%lf,%lf",&toolindex,&approach,&feed,&spindle);
  if (lasttoolindex != toolindex)
    {  if (lasttoolindex != -1)
         p1 = ncmilltask.gethome() + ncmilltask.tool[lasttoolindex-1].offset;
       else
         p1.setxyz(0.0,0.0,0.0);
       p2 = ncmilltask.gethome() + ncmilltask.tool[toolindex].offset;
       NCToolChange(toolindex,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
       lasttoolindex = toolindex;
    }
  list = tp->getlist();
  for (list.start() ; (p = (PointE *)list.next()) != 0 ; )
    {  NCLinearMovement(0,p->getp().x,p->getp().y,p->getp().z,toolindex,feed,spindle);
       if (stopped)
         break;
    }
}

void NCMillTask::insertdrill(Group *toolpath,EntityList list,int toolpathtype,int toolindex,int order,double feed,double spindle,double retract,double depth,double peck)
{Group *newtoolpath;
 RCCHAR classname[300],instance[300];
 BitMask options(32);
  sprintf(classname,"Toolpath::Drill");
  sprintf(instance,"Toolpath::%d,%d,%lf,%lf,%lf,%lf,%lf",toolindex,toolpathtype,feed,spindle,retract,depth,peck);
  db.geometry.add(newtoolpath = new Group(list,classname,instance,options));
  inserttoolpath(newtoolpath,toolpath,order);
}

void NCMillTask::testdrill(Group *tp)
{int toolindex,toolpathtype;
 double feed,spindle,retract,depth,peck,z;
 EntityList list;
 PointE *p;
 Point p1,p2;
  if (stopped)
    return;
  sscanf(tp->getinstance(),"Toolpath::%d,%d,%lf,%lf,%lf,%lf,%lf",&toolindex,&toolpathtype,&feed,&spindle,&retract,&depth,&peck);
  if (lasttoolindex != toolindex)
    {  if (lasttoolindex != -1)
         p1 = ncmilltask.gethome() + ncmilltask.tool[lasttoolindex-1].offset;
       else
         p1.setxyz(0.0,0.0,0.0);
       p2 = ncmilltask.gethome() + ncmilltask.tool[toolindex].offset;
       NCToolChange(toolindex,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
       lasttoolindex = toolindex;
    }
  list = tp->getlist();
  for (list.start() ; (p = (PointE *)list.next()) != 0 ; )
    {  if (toolpathtype == 4 && NCPeckDrillCycle != 0)
         NCPeckDrillCycle(toolindex,toolpathtype,feed,spindle,retract,depth,peck,p->getp().x,p->getp().y,p->getp().z);
       else if (toolpathtype == 5 && NCDeepPeckDrillCycle != 0)
         NCDeepPeckDrillCycle(toolindex,toolpathtype,feed,spindle,retract,depth,peck,p->getp().x,p->getp().y,p->getp().z);
       else if (toolpathtype == 6 && NCDrillCycle != 0)
         NCDrillCycle(toolindex,toolpathtype,feed,spindle,retract,depth,peck,p->getp().x,p->getp().y,p->getp().z);
       else
          {  NCLinearMovement(1,p->getp().x,p->getp().y,retract,toolindex,feed,spindle);
             z = retract;
             if (toolpathtype == 6)
               peck = retract - depth;
             do {  z -= peck;  if (z < depth) z = depth;
                   NCLinearMovement(0,p->getp().x,p->getp().y,z,toolindex,feed,spindle);
                   if (z != depth)
                     {  if (toolpathtype == 4)
                          NCLinearMovement(1,p->getp().x,p->getp().y,z+peck/20.0,toolindex,feed,spindle);
                        else if (toolpathtype == 5)
                          {  NCLinearMovement(1,p->getp().x,p->getp().y,retract,toolindex,feed,spindle);
                             NCLinearMovement(1,p->getp().x,p->getp().y,z+peck/20.0,toolindex,feed,spindle);
                          }
                        NCLinearMovement(0,p->getp().x,p->getp().y,z,toolindex,feed,spindle);
                     }
                   if (stopped)
                     break;
                } while (z != depth);
             NCLinearMovement(1,p->getp().x,p->getp().y,retract,toolindex,feed,spindle);
          }
    }
}

void NCMillTask::insertcontour(Group *toolpath,EntityList list,int toolindex,int order,int compensation,int approach,int retractm,double retract,double feed,double plungefeed,double retractfeed,double spindle,double initialz,int useentityz,double finalz,double depthcut)
{Group *newtoolpath;
 RCCHAR classname[300],instance[300];
 BitMask options(32);
  sprintf(classname,"Toolpath::Contour");
  sprintf(instance,"Toolpath::%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf",
          toolindex,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,useentityz,finalz,depthcut);
  db.geometry.add(newtoolpath = new Group(list,classname,instance,options));
  inserttoolpath(newtoolpath,toolpath,order);
}

void NCMillTask::insertcontour(Group *toolpath,UVSurfaceEntity *se,Point pickpoint,int toolindex,int order,int compensation,int approach,int retractm,double retract,double feed,double plungefeed,double retractfeed,double spindle,double initialz,int,double finalz,double depthcut,int path,double stepover,double surfaceoffset)
{const int n1 = 100;
 int i,j,k,n2,intersected,u_intersected;
 Curve *curve;
 Line *line;
 Patch *patch;
 PointE *p;
 EntityList list,ulist;
 Point pxy,pstart,pu_start,pu_end,pu_last_start,pu_last_end,p1,p2,p3,pi,lastpi,pmin,pmax,*triangle,normal;
 double s,u,v,distance,mindistance,ustart,vstart,du1,dv1,du2,dv2,u1,v1,length,sign,l1,l2,D,denom;
 Point point[n1+1];
 BitMask options(32);


    if (se->isa(patch_entity) && ((Patch *)se)->getoptions().test(PatchTriangulated))
    {//  Machine a triangulated surface
        patch = (Patch *)se;
        patch->extents(0,&pmin,&pmax);
        patch->triangulate(0.0);
        n2 = 0;
        intersected = 0;
        for (u = pmin.x + stepover,i = 0 ; u < pmax.x ; u += stepover,i++)
        {   u_intersected = 0;
            for (v = pmin.y ; v < pmax.y + stepover / 10 ; v += stepover / 10)
            {   if (v > pmax.y)
                    v = pmax.y;
                //  Intersect the vertical ray from this (u,v) with the patch
                triangle = patch->gettriangles();
                for (k = 0 ; k < patch->getntriangles() ; k++, triangle += 6)
                {   if (triangle->z == RCDBL_MAX)
                        triangle += 2;

                    pxy = Point(u,v,0.0);

                    if (triangle[0].x > pxy.x && triangle[1].x > pxy.x && triangle[2].x > pxy.x ||
                        triangle[0].y > pxy.y && triangle[1].y > pxy.y && triangle[2].y > pxy.y ||
                        triangle[0].x < pxy.x && triangle[1].x < pxy.x && triangle[2].x < pxy.x ||
                        triangle[0].y < pxy.y && triangle[1].y < pxy.y && triangle[2].y < pxy.y)
                    {   triangle += 3;
                        continue;  //  Don't do the intersection as the simple min/max test has failed
                    }

                    p1 = *triangle++;
                    p2 = *triangle++;
                    p3 = *triangle++;

                    l1 = (p1 - p3).length();
                    l2 = (p2 - p3).length();
                    normal = ((p2 - p1).cross(p3 - p1)).normalize();
                    D = -normal.dot(p1);
                    denom = normal.dot(Point(0.0,0.0,1.0));
                    if (fabs(denom) > db.getatoler())
                    {   s = -(pxy.dot(normal) + D) / denom;
                        pi = Point(0.0,0.0,1.0) * s + pxy;
                        if (pi.z < height - 0.01 && 
                            (pi - p1).dot(normal.cross(p2 - p1)) > -1.0E-8 * (l1 + l2) &&
                            (pi - p2).dot(normal.cross(p3 - p2)) > -1.0E-8 * (l1 + l2) &&
                            (pi - p3).dot(normal.cross(p1 - p3)) > -1.0E-8 * (l1 + l2))
                        {   if (normal.z < 0.0)
                                normal = -normal;
                            point[n2++] = pi + normal * surfaceoffset + Point(0.0,0.0,-surfaceoffset);
                            lastpi = point[n2-1];
                            if (intersected == 0)
                            {   pstart = point[n2-1];
                                intersected = 1;
                            }
                            if (u_intersected == 0)
                            {   u_intersected = 1;
                                pu_last_start = pu_start;
                                pu_start = point[n2-1];

                                if (path == 1 && i > 0 && (i % 2) == 0)
                                {//  Lace cutting connector
                                    line = new Line(pu_last_start,pu_start);
                                    db.geometry.add(line);
                                    list.add(line);
                                } 

                            }
                            if (n2 == n1)
                            {   curve = new Curve(n2,2,point,0,0,options);
                                if (curve->getdefined())
                                {   db.geometry.add(curve);
                                    ulist.add(curve);
                                }
                                point[0] = pi + normal * surfaceoffset + Point(0.0,0.0,-surfaceoffset);
                                n2 = 1;
                            }
                            break;
                        }
                    }
                }
                if (k == patch->getntriangles())
                {//  No intersection - so insert the curve if any now
                    if (n2 > 2)
                    {   curve = new Curve(n2,2,point,0,0,options);
                        if (curve->getdefined())
                        {   db.geometry.add(curve);
                            ulist.add(curve);
                        }
                        n2 = 0;
                    }
                }
            }

            if (n2 > 2)
            {   curve = new Curve(n2,2,point,0,0,options);
                if (curve->getdefined())
                {   db.geometry.add(curve);
                    ulist.add(curve);
                }
                n2 = 0;
            }

            pu_last_end = pu_end;
            pu_end = lastpi;

            if (path == 1 && (i % 2) == 1 && pu_last_end.x > -9.0e37)
            {  line = new Line(pu_last_end,pu_end);
               db.geometry.add(line);
               list.add(line);
            } 

            if ((i % 2) == 0)
                ulist.reverse();

            for (ulist.start() ; ! ulist.atend() ; )
                list.add(ulist.next());
            ulist.destroy();


        }
 
        if (list.length() > 0)
        {   list.reverse();
            list.add(p = new PointE(pstart));
            db.geometry.add(p);
            ncmilltask.insertcontour(toolpath,list,toolindex,order,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,1,finalz,depthcut);
        }

    }
    else
    {   mindistance = -1.0;  sign = 0.0;
        for (i = 0 ; i <= 10 ; i++)
            for (j = 0 ; j < 10 ; j++)
            {   distance = (se->position(i/10.0,j/10.0) - pickpoint).length();
                if (distance < mindistance || mindistance < 0.0)
                {   u = i / 10.0;  
                    v = j / 10.0;  
                    mindistance = distance;
                }
                sign += se->normal(i/10.0,j/10.0).z;
            }
        sign = sign > 0.0 ? 1.0 : -1.0;

        if (u > 1.0 - v && u > v)
        {//  u = 1.0 edge
            if (v < 0.5)
            {   ustart = 1.0;  
                vstart = 0.0;
                du1 = 0.0;       
                dv1 = 1.0;
                du2 = -1.0;  
                dv2 = 0.0;
            }
            else
            {   ustart = 1.0;    
                vstart = 1.0;
                du1 = 0.0;       
                dv1 = -1.0;
                du2 = -1.0;  
                dv2 = 0.0;
            }
        }
        else if (u > 1.0 - v && u <= v)
        {//  v = 1.0 edge
            if (u < 0.5)
            {   ustart = 0.0;  
                vstart = 1.0;
                du1 = 1.0;   
                dv1 = 0.0;
                du2 = 0.0;       
                dv2 = -1.0;
            }
            else
            {   ustart = 1.0;    
                vstart = 1.0;
                du1 = -1.0;  
                dv1 = 0.0;
                du2 = 0.0;       
                dv2 = -1.0;
            }
        }
        else if (u <= 1.0 - v && u > v)
        {//  v = 0.0 edge
            if (u < 0.5)
            {   ustart = 0.0;    
                vstart = 0.0;
                du1 = 1.0;   
                dv1 = 0.0;
                du2 = 0.0;       
                dv2 = 1.0;
            }
            else
            {   ustart = 1.0;    
                vstart = 0.0;
                du1 = -1.0;  
                dv1 = 0.0;
                du2 = 0.0;       
                dv2 = 1.0;
            }
        }
        else
        {//  u = 0.0 edge
            if (v < 0.5)
            {   ustart = 0.0;   
                vstart = 0.0;
                du1 = 0.0;      
                dv1 = 1.0;
                du2 = 1.0;      
                dv2 = 0.0;
            }
            else
            {   ustart = 0.0;    
                vstart = 1.0;
                du1 = 0.0;       
                dv1 = -1.0;
                du2 = 1.0;  
                dv2 = 0.0;
            }
        }

        length = 0.0;
        u1 = ustart;  v1 = vstart;
        for (j = 0 ; j <= n1 * 2 ; j++)
        {   p2 = se->position(u1,v1);
            if (j > 0)
                length += (p2 - p1).length();
            u1 += du2 / n1 / 2.0;
            v1 += dv2 / n1 / 2.0;
            p1 = p2;
        }
        du1 /= n1;  
        dv1 /= n1;
        n2 = int(length / stepover);
        if (n2 < 2) 
            n2 = 2;
        du2 /= n2;  
        dv2 /= n2;

        for (i = 0 ; i <= n2 ; i++)
        {   u1 = ustart + i * du2;  
            v1 = vstart + i * dv2;
            for (j = 0 ; j <= n1 ; j++,u1 += du1,v1 += dv1)
                point[j] = se->position(u1,v1) + se->normal(u1,v1) * sign * surfaceoffset - Point(0.0,0.0,surfaceoffset);
            curve = new Curve(10,3,n1+1,point,options);
            if (curve->getdefined())
            {   db.geometry.add(curve);
                list.add(curve);
            }
            if (path == 1) // Lace cutting
            {   if ((i % 2) == 0)
                {   u1 = ustart + i * du2 + n1 * du1;  
                    v1 = vstart + i * dv2 + n1 * dv1;
                }
                else
                {   u1 = ustart + i * du2;  
                    v1 = vstart + i * dv2;
                }
                for (j = 0 ; j <= n1 / 10 ; j++,u1 += du2 / int(n1 / 10),v1 += dv2 / int(n1 / 10))
                    point[j] = se->position(u1,v1) + se->normal(u1,v1) * sign * surfaceoffset - Point(0.0,0.0,surfaceoffset);
                curve = new Curve(4,3,int(n1/10)+1,point,options);
                if (curve->getdefined())
                {   db.geometry.add(curve);
                    list.add(curve);
                }
            }
        }
        list.reverse();
        list.add(p = new PointE(se->position(ustart,vstart) + se->normal(ustart,vstart) * sign * surfaceoffset - Point(0.0,0.0,surfaceoffset)));
        db.geometry.add(p);
        ncmilltask.insertcontour(toolpath,list,toolindex,order,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,1,finalz,depthcut);
    }


}

void NCMillTask::testcontour(Group *tp)
{int toolindex,compensation,approach,retractm,useentityz;
 long i,n,rapidmode;
 double feed,spindle,retract,plungefeed,retractfeed,initialz,finalz,depthcut,z,t,a1,a2,deltaz,D,offset,nextt;
 Point start,startp,startd,o1,p1,p2,origin,xaxis,yaxis,zaxis;
 EntityList list;
 PointE *p;
 LinearEntity *le;
 Circle *c;
 Curve *cu,*nextcu;
 EntityHeader header;
 Loops loops(0);

  if (stopped)
    return;

  sscanf(tp->getinstance(),"Toolpath::%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf",
          &toolindex,&compensation,&approach,&retractm,&retract,&feed,&plungefeed,&retractfeed,&spindle,&initialz,&useentityz,&finalz,&depthcut);
  if (lasttoolindex != toolindex)
    {  if (lasttoolindex != -1)
         p1 = ncmilltask.gethome() + ncmilltask.tool[lasttoolindex-1].offset;
       else
         p1.setxyz(0.0,0.0,0.0);
       p2 = ncmilltask.gethome() + ncmilltask.tool[toolindex].offset;
       NCToolChange(toolindex,p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
       lasttoolindex = toolindex;
    }
  if (NCSetCompensation != 0)
    NCSetCompensation(compensation);
  list = tp->getlist().copy();
  list.start();
  if ((p = (PointE *)list.next()) == 0)
    return;
  list.del(p);
  startp = p->getp();
  le = (LinearEntity *)list.next();
  if (le == 0)
    return;
  if ((startp - le->position(0.0)).length() < (startp - le->position(1.0)).length())
    startd = le->direction(0.0);
  else
    startd = -le->direction(1.0);

  if (compensation != 0)
    {  list.reverse();
       loops.create(&list,0);
       if (loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
         {  if (toolindex < 1 || toolindex > MAX_TOOLS || tool[toolindex-1].type == 0)
              offset = 0.0;
            else
              offset = tool[toolindex-1].diameter / 2.0;
            startp = startp+startd.cross(Point(0.0,0.0,1.0)).normalize()*(compensation == 1 ? -offset : offset);
            if (! loops.offset(0,0,startp,offset,&list))
              {  loops.destroy();
                 list = tp->getlist().copy();
                 list.start();
                 if ((p = (PointE *)list.next()) == 0)
                   return;
                 list.del(p);
                 startp = p->getp();
              }
         }
       else
         {  loops.destroy();
            list = tp->getlist().copy();
            list.start();
            list.del(list.next());
         }
    }

  for (;;)
     {  list.start();
        start = startp;
        if (useentityz)
          z = start.z + initialz;
        else
          z = initialz;
        deltaz = z - start.z;
        NCLinearMovement(1,start.x,start.y,retract,toolindex,plungefeed,spindle);
        NCLinearMovement(0,start.x,start.y,start.z + deltaz,toolindex,plungefeed,spindle);
        rapidmode = 0;
        while ((le = (LinearEntity *)list.next()) != 0)
          {  if ((le->start() - start).length() < (le->end() - start).length() || (le->start() - start).length() > 0.001 && (le->end() - start).length() > 0.001)
               {  if ((le->start() - start).length() > 0.001)
                    {  NCLinearMovement(1,start.x,start.y,retract,toolindex,feed,spindle);
                       NCLinearMovement(1,le->start().x,le->start().y,retract,toolindex,feed,spindle);
                       NCLinearMovement(0,le->start().x,le->start().y,le->start().z+deltaz,toolindex,feed,spindle);
                    }
                  if (le->isa(line_entity))
                    NCLinearMovement(0,le->end().x,le->end().y,le->end().z+deltaz,toolindex,feed,spindle);
                  else if (le->isa(circle_entity))
                    {  c = (Circle *) le;
                       Circle c1(header,c->getorigin(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),c->getradius(),c->getstarta(),c->getsweepa());
                       a1 = c1.angle(c->start());
                       a2 = c1.angle(c->end());
                       if (c->getxaxis().cross(c->getyaxis()).z < 0.0)
                         {  while (a2 < a1) a2 += 2.0 * M_PI;
                         }
                       else
                         {  while (a2 > a1) a2 -= 2.0 * M_PI;
                         }
                       if (NCArcMovement != 0)
                         {  NCArcMovement(c->getxaxis().cross(c->getyaxis()).z > 0.0,0,c->getorigin().x,c->getorigin().y,c->getorigin().z+deltaz,a1,a2-a1,c->getradius(),toolindex,feed,spindle);
                         }
                       else
                         {  header.change(1,state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
                            Circle circle(header,c->getorigin(),c->getxaxis(),c->getyaxis(),c->getradius(),c->getstarta(),c->getsweepa());
                            if (linetrails)
                              circle.draw(DM_NORMAL);
                            n = int(circle.length() / stepsize);
                            if (n < 2) n = 2;
                            for (i = 1 ; i <= n ; i++)
                              {  t = double(i) / double(n);
                                 NCLinearMovement(0,circle.position(t).x,circle.position(t).y,circle.position(t).z+deltaz,toolindex,feed,spindle);
                              }
                         }
                    }
                  else if (le->isa(curve_entity))
                    {  cu = (Curve *) le;
                       n = int(cu->length() / stepsize);
                       if (n < 2) n = 2;
                       for (i = 1 ; i <= n ; i++)
                         {  t = double(i) / double(n);
                            if (rapidmode == 0)
                              {  NCLinearMovement(0,cu->position(t).x,cu->position(t).y,cu->position(t).z+deltaz,toolindex,feed,spindle);
                                 if (cu->position(t).z+deltaz > retract)
                                   rapidmode = 1;
                              }
                            else
                              {  if (i == n && ((Entity *)list.peek()) != 0 && ((Entity *)list.peek())->isa(curve_entity))
                                   {  nextcu = (Curve *) list.peek();
                                      if ((nextcu->start() - cu->position(1.0)).length() < (nextcu->end() - cu->position(1.0)).length() ||
                                          (nextcu->start() - cu->position(1.0)).length() > 0.001 && (nextcu->end() - cu->position(1.0)).length() > 0.001)
                                        nextt = stepsize / (nextcu->length() > 0.0 ? nextcu->length() : stepsize);
                                      else
                                        nextt = 1.0 - stepsize / (nextcu->length() > 0.0 ? nextcu->length() : stepsize);
                                      if (nextcu->position(nextt).z + deltaz < retract)
                                        rapidmode = 0;
                                   }
                                 if (i < n && cu->position((double(i+1) / double(n))).z + deltaz < retract)
                                   rapidmode = 0;
                                 if (rapidmode == 0)
                                   NCLinearMovement(1,cu->position(t).x,cu->position(t).y,cu->position(t).z+deltaz,toolindex,feed,spindle);
                              }
                         }
                    }
                  start = le->end();
               }
             else
               {  if ((le->end() - start).length() > 0.001)
                     {  NCLinearMovement(1,start.x,start.y,retract,toolindex,feed,spindle);
                        NCLinearMovement(1,le->end().x,le->end().y,retract,toolindex,feed,spindle);
                        NCLinearMovement(0,le->end().x,le->end().y,le->end().z+deltaz,toolindex,feed,spindle);
                     }
                  if (le->isa(line_entity))
                    NCLinearMovement(0,le->start().x,le->start().y,le->start().z+deltaz,toolindex,feed,spindle);
                  else if (le->isa(circle_entity))
                    {  c = (Circle *) le;
                       Circle c1(header,c->getorigin(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),c->getradius(),c->getstarta(),c->getsweepa());
                       a1 = c1.angle(c->end());
                       a2 = c1.angle(c->start());
                       if (c->getxaxis().cross(c->getyaxis()).z < 0.0)
                         {  while (a2 < a1) a2 += 2.0 * M_PI;
                         }
                       else
                         {  while (a2 > a1) a2 -= 2.0 * M_PI;
                         }
                       if (NCArcMovement != 0)
                         {  NCArcMovement(c->getxaxis().cross(c->getyaxis()).z < 0.0,0,c->getorigin().x,c->getorigin().y,c->getorigin().z+deltaz,a1,a2-a1,c->getradius(),toolindex,feed,spindle);
                         }
                       else
                         {  header.change(1,state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
                            Circle circle(header,c->getorigin(),c->getxaxis(),c->getyaxis(),c->getradius(),c->getstarta(),c->getsweepa());
                            if (linetrails)
                              circle.draw(DM_NORMAL);
                            n = int(circle.length() / stepsize);
                            if (n < 2) n = 2;
                            for (i = 1 ; i <= n ; i++)
                              {  t = 1.0 - double(i) / double(n);
                                 NCLinearMovement(0,circle.position(t).x,circle.position(t).y,circle.position(t).z+deltaz,toolindex,feed,spindle);
                              }
                         }
                    }
                  else if (le->isa(curve_entity))
                    {  cu = (Curve *) le;
                       n = int(cu->length() / stepsize);
                       if (n < 2) n = 2;
                       for (i = 1 ; i <= n ; i++)
                         {  t = 1.0 - double(i) / double(n);
                            if (rapidmode == 0)
                              {  NCLinearMovement(0,cu->position(t).x,cu->position(t).y,cu->position(t).z+deltaz,toolindex,feed,spindle);
                                 if (cu->position(t).z+deltaz > retract)
                                   rapidmode = 1;
                              }
                            else
                              {  if (i == n && ((Entity *)list.peek())->isa(curve_entity))
                                   {  nextcu = (Curve *) list.peek();
                                      if ((nextcu->start() - cu->position(0.0)).length() < (nextcu->end() - cu->position(0.0)).length() ||
                                          (nextcu->start() - cu->position(0.0)).length() > 0.001 && (nextcu->end() - cu->position(0.0)).length() > 0.001)
                                        nextt = stepsize / (nextcu->length() > 0.0 ? nextcu->length() : stepsize);
                                      else
                                        nextt = 1.0 - stepsize / (nextcu->length() > 0.0 ? nextcu->length() : stepsize);
                                      if (nextcu->position(nextt).z + deltaz < retract)
                                        rapidmode = 0;
                                   }
                                 if (i < n && cu->position(1.0 - (double(i+1) / double(n))).z + deltaz < retract)
                                   rapidmode = 0;
                                 if (rapidmode == 0)
                                   NCLinearMovement(1,cu->position(t).x,cu->position(t).y,cu->position(t).z+deltaz,toolindex,feed,spindle);
                              }
                         }
                    }
                  start = le->start();
               }
             if (stopped)
               break;
          }
        NCLinearMovement(1,start.x,start.y,retract,toolindex,feed,spindle);
        if (stopped || initialz == finalz)
          break;
        initialz -= depthcut;
        if (initialz < finalz) initialz = finalz;
     }
  if (NCSetCompensation != 0)
    NCSetCompensation(0);
}

int NCMillTask::traverse(void)
{EntityList list,list1;
 Group *tp;
 Entity *e;
 int toolindex;

  list = group->getlist();
  list.start();
  if ((e = list.next()) != 0 && (e = list.next()) != 0 && (e = list.next()) != 0 &&
      (e = list.next()) != 0 && (e = list.next()) != 0)
    {  list1 = ((Group *)e)->getlist();
       for (list1.start() ; (tp = (Group *)list1.next()) != 0 ; )
         {  
            sscanf(tp->getinstance(),"Toolpath::%d",&toolindex);
            if (toolindex < 1 || toolindex > MAX_TOOLS ||
                tool[toolindex-1].type == 0 || tool[toolindex-1].diameter < 1.0E-10)
              {RCCHAR message[300];
                 sprintf(message,"Tool %d is not defined or has zero diameter.  Use Define Tool to define the tool correctly",toolindex);
                 cadwindow->MessageBox(message,"Test and Generate",MB_OK);
                 break;
              }

            if (strcmp(tp->getclassname(),"Toolpath::Rapid") == 0)
              testrapid(tp);
            else if (strcmp(tp->getclassname(),"Toolpath::Feed") == 0)
              testfeed(tp);
            else if (strcmp(tp->getclassname(),"Toolpath::Drill") == 0)
              testdrill(tp);
            else if (strcmp(tp->getclassname(),"Toolpath::Contour") == 0)
              testcontour(tp);
         }
    }
  return 1;
}

void NCMillTask::test(int lt,int tt,double ss,int gm,int de)
{Entity *e;

  lasttoolindex = -1;
  stopped = 0;

  linetrails = lt;
  tooltrails = tt;
  stepsize = ss;
  generatemodel = gm;
  displayevery = de;

  distance = 0;
  frame = 1;

#if defined(AVI)
  avicompressedstream = 0;
  avistream = 0;
  avifile = 0;
#endif

  state.destroyselection(1);
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(ncsolid_entity))
      {  db.geometry.del(e);
         db.geometry.start();
      }

  if (generatemodel)
    {  ncsolid = new NCSolid(origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(length,width,height),v.getinteger("nc::maxsegments"),v.getinteger("nc::xmesh"),v.getinteger("nc::ymesh"));
       db.geometry.add(ncsolid);
    }

  NCInit = 0;
  NCToolChange = NCTestToolChange;
  NCClose = 0;
  NCLinearMovement = NCTestLinearMovement;
  NCArcMovement = 0;
  NCSetCompensation = 0;
  NCDrillCycle = 0;
  NCPeckDrillCycle = 0;
  NCDeepPeckDrillCycle = 0;

  if (SetLight != 0 && cadwindow->getcurrentwindow() != 0 && cadwindow->getcurrentwindow()->getrepaintstyle() == Shaded && OpenOpenGl(cadwindow->getcurrentwindow()->gethdc(),0,0,0,(int)cadwindow->getcurrentwindow()->getwidth(),(int)cadwindow->getcurrentwindow()->getheight()) == 0)
    {View *view;
       SetLight(1000.0,4000.0,20000.0);
       SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
       view = cadwindow->getcurrentsurface()->getview();
       SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
       SetViewport(cadwindow->getcurrentsurface()->getumin(),cadwindow->getcurrentsurface()->getvmin(),cadwindow->getcurrentsurface()->getumax(),cadwindow->getcurrentsurface()->getvmax());
       usingd3d = 1;
    }
  else
    usingd3d = 0;

  program->setbusy(1);
  //cadwindow->paintall(0,1+2*(usingd3d!=0));
  program->processpaintmessages();
#if defined(AVI)
  if (generatemodel && strlen(v.getstring("nc::avifilename")) > 0 && cadwindow->getcurrentwindow() != 0)
    {AVISTREAMINFO strhdr;
     AVICOMPRESSOPTIONS opts;
       AVIFileInit();
       if (AVIFileOpen(&avifile,v.getstring("nc::avifilename"),OF_WRITE | OF_CREATE,0) != AVIERR_OK)
         avifile = 0;
       else
         {  memset(&strhdr, 0, sizeof(strhdr));
            strhdr.fccType = streamtypeVIDEO;// stream type
            strhdr.fccHandler = 0;
            strhdr.dwScale = 1;
            strhdr.dwRate = 15;
            strhdr.dwSuggestedBufferSize  = long(cadwindow->getcurrentwindow()->getwidth()) * long(cadwindow->getcurrentwindow()->getheight()) * 3;
            SetRect(&strhdr.rcFrame,0,0,(int)cadwindow->getcurrentwindow()->getwidth(),(int)cadwindow->getcurrentwindow()->getheight());
            if (AVIFileCreateStream(avifile,&avistream,&strhdr) != AVIERR_OK)
              {  avistream = 0;
                 AVIFileRelease(avifile);
                 avifile = 0;
              }
            else
              {  memset(&opts,0,sizeof(opts));
                 opts.dwFlags = AVICOMPRESSF_VALID;
                 opts.fccType = streamtypeVIDEO;
                 opts.fccHandler = mmioFOURCC('M', 'S', 'V', 'C');
                 opts.dwQuality = 7500;
                 opts.dwKeyFrameEvery = 15;
                 if (AVIMakeCompressedStream(&avicompressedstream,avistream,&opts,0) != AVIERR_OK)
                   {   avicompressedstream = 0;
                       AVIStreamRelease(avistream);
                       avistream = 0;
                       AVIFileRelease(avifile);
                       avifile = 0;
                   }
              }
         }
       if (avicompressedstream != 0 && cadwindow->getcurrentwindow() != 0)
         cadwindow->getcurrentwindow()->saveavistream(avicompressedstream,frame);
    }
#endif


  traverse();

  if (generatemodel)
    {  cadwindow->invalidatedisplaylist(ncsolid);
       //cadwindow->paintall(0,2/*Update View2dWindows as well*/);
       if (usingd3d != 0)
         {BitMask foptions(32);
            foptions.set(1);
            BeginScene(0);
            SetLight(1000.0,4000.0,20000.0);
            ncsolid->facet(foptions,0.0,0,0,0,direct3dcb,0);
            EndScene();
         }

#if defined(AVI)
       if (avicompressedstream != 0)
         AVIStreamRelease(avicompressedstream);
       if (avistream != 0)
	      AVIStreamRelease(avistream);
       if (avifile)
         AVIFileRelease(avifile);
       AVIFileExit();
#endif
    }

  program->setbusy(0);
}

void NCMillTask::generate(RCCHAR *postprocessor,RCCHAR *filename)
{FILE *outfile;
 RCHINSTANCE postinstance;
 RCCHAR *(WINAPI *NCPostName)(void);

  generatemodel = 0;
  lasttoolindex = -1;
  stopped = 0;
  stepsize = 1.0;

#if ! defined(_WIN32_WCE)
  if ((postinstance = LoadLibrary(postprocessor)) != 0)
    {  NCPostName = (RCCHAR *(WINAPI *)(void)) GetProcAddress(postinstance,TEXT("NCPostName"));
       if (NCPostName != 0)
         {  NCInit = (void (WINAPI *)(RCCHAR *)) GetProcAddress(postinstance,TEXT("NCInit"));
            NCToolChange = (void (WINAPI *)(int toolindex,double oldx,double oldy,double oldz,double x,double y,double z)) GetProcAddress(postinstance,TEXT("NCToolChange"));
            NCClose = (void (WINAPI *)(void))GetProcAddress(postinstance,TEXT("NCClose"));
            NCLinearMovement = (void (WINAPI *)(int rapid,double x,double y,double z,int toolindex,double feed,double spindle)) GetProcAddress(postinstance,TEXT("NCLinearMovement"));
            NCArcMovement = (void (WINAPI *)(int reverse,int plane,double ox,double oy,double oz,double a1,double a2,double radius,int toolindex,double feed,double spindle)) GetProcAddress(postinstance,TEXT("NCArcMovement"));
            NCDrillCycle = (void (WINAPI *)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z)) GetProcAddress(postinstance,TEXT("NCDrillCycle"));
            NCPeckDrillCycle = (void (WINAPI *)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z)) GetProcAddress(postinstance,TEXT("NCPeckDrillCycle"));
            NCDeepPeckDrillCycle = (void (WINAPI *)(int toolindex,int toolpathtype,double feed,double spindle,double retract,double depth,double peck,double x,double y,double z)) GetProcAddress(postinstance,TEXT("NCDeepPeckDrillCycle"));
            NCSetCompensation = (void (WINAPI *)(int)) GetProcAddress(postinstance,"NCSetCompensation");
         }
       else
         {  cadwindow->MessageBox("Post processor is not available","Generate",MB_ICONSTOP);
            return;
         }
    }
  else
#endif
    {  cadwindow->MessageBox("Post processor is not available","Generate",MB_ICONSTOP);
       return;
    }

  if (NCLinearMovement == 0)
    {  cadwindow->MessageBox("Post processor must contain a linear movement function","Generate",MB_ICONSTOP);
       FreeLibrary(postinstance);
       return;
    }

  if ((outfile = fopen(filename,"w")) == 0)
    {  cadwindow->MessageBox("Output file could not be opened.","Generate",MB_ICONSTOP);
       FreeLibrary(postinstance);
       return;
    }
  fclose(outfile);

  if (NCInit != 0)
    NCInit(filename);

  traverse();

  if (NCClose != 0)
    NCClose();

}

class NCListDialogControl : public ListDialogControl
  {private:
     RCCHAR oldtaskname[300];
   public:
     NCListDialogControl(int id,int n,RCCHAR **list,RCCHAR *v,int l) : ListDialogControl(id,n,list,v,l) {  strcpy(oldtaskname,v);  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

class NCButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     NCButtonDialogControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *);
  };

int NCButtonDialogControl::select(Dialog *dialog)
{int status;
  status = ButtonDialogControl::select(dialog);
  dialog->store();
  return status;
}

void NCListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void NCListDialogControl::changefocus(Dialog *dialog,int)
{RCCHAR taskname[300];
  if (dialog->currentvalue(100,taskname,300) && strcmp(taskname,oldtaskname) != 0)
    {  ncmilltask.load(NCMillTask::gettask(NCMillTaskName,taskname));
       ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,ncmilltask.getwidth());
       ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,ncmilltask.getlength());
       ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,ncmilltask.getheight());
       ((RealDialogControl *)dialog->getcontrol(105))->change(dialog,ncmilltask.getorigin().x);
       ((RealDialogControl *)dialog->getcontrol(106))->change(dialog,ncmilltask.getorigin().y);
       ((RealDialogControl *)dialog->getcontrol(107))->change(dialog,ncmilltask.getorigin().z);
       ((RealDialogControl *)dialog->getcontrol(109))->change(dialog,ncmilltask.gethome().x);
       ((RealDialogControl *)dialog->getcontrol(110))->change(dialog,ncmilltask.gethome().y);
       ((RealDialogControl *)dialog->getcontrol(111))->change(dialog,ncmilltask.gethome().z);
       ((RealDialogControl *)dialog->getcontrol(113))->change(dialog,ncmilltask.getpartorigin().x);
       ((RealDialogControl *)dialog->getcontrol(114))->change(dialog,ncmilltask.getpartorigin().y);
       ((RealDialogControl *)dialog->getcontrol(115))->change(dialog,ncmilltask.getpartorigin().z);
       strcpy(oldtaskname,taskname);
    }
}

void nc_command1(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog("NCTask_Dialog");
 DialogControl *dc;
 static double width,height,length;
 static RCCHAR taskname[300],**namelist;
 static Point home,origin,partorigin;
 RCCHAR string1[300],string2[300];
 int i,n,button;
 EntityList list;
 BitMask options(32);
  do {  switch (*cstate)
         {case InitialState :
            width = v.getreal("nc::millwidth");
            length = v.getreal("nc::milllength");
            height = v.getreal("nc::millheight");
            origin.x = v.getreal("nc::milloriginx");
            origin.y = v.getreal("nc::milloriginy");
            origin.z = v.getreal("nc::milloriginz");
            partorigin.x = v.getreal("nc::millpartx");
            partorigin.y = v.getreal("nc::millparty");
            partorigin.z = v.getreal("nc::millpartz");

            home.x = v.getreal("nc::millhomex");
            home.y = v.getreal("nc::millhomey");
            home.z = v.getreal("nc::millhomez");
            strcpy(taskname,ncmilltask.getcurrenttaskname());
            //  No break;
          case 1 :
            NCMillTask::gettasks(NCMillTaskName,&namelist,&n);
            dialog.add(new NCListDialogControl(100,n,namelist,taskname,300));
            dialog.add(dc = new RealDialogControl(102,&width,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1102,dc));
            dialog.add(dc = new RealDialogControl(103,&length,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1103,dc));
            dialog.add(dc = new RealDialogControl(104,&height,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1104,dc));
            dialog.add(dc = new RealDialogControl(105,&origin.x,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1105,dc));
            dialog.add(dc = new RealDialogControl(106,&origin.y,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1106,dc));
            dialog.add(dc = new RealDialogControl(107,&origin.z,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1107,dc));
            dialog.add(new NCButtonDialogControl(108));
            dialog.add(dc = new RealDialogControl(109,&home.x,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1109,dc));
            dialog.add(dc = new RealDialogControl(110,&home.y,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1110,dc));
            dialog.add(dc = new RealDialogControl(111,&home.z,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1111,dc));
            dialog.add(new NCButtonDialogControl(112));
            dialog.add(dc = new RealDialogControl(113,&partorigin.x,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1113,dc));
            dialog.add(dc = new RealDialogControl(114,&partorigin.y,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1114,dc));
            dialog.add(dc = new RealDialogControl(115,&partorigin.z,ScaleByLength));
            dialog.add(dc = new ScrollBarDialogControl(1115,dc));
            dialog.add(new NCButtonDialogControl(116));
          case 5 :
            button = dialog.process();
            if (button == 108)
              *cstate = 2;
            else if (button == 112)
              *cstate = 3;
            else if (button == 116)
              *cstate = 4;
            else if (button == TRUE)
              {  if (strlen(taskname) == 0)
                   {  cadwindow->MessageBox("The task name must be specified.","NC",MB_ICONINFORMATION);
                      *cstate = 5;
                   }
                 else
                   {  for (i = 0 ; i < n ; i++)
                        {  strcpy(string1,namelist[i]);  _strupr(string1);
                           strcpy(string2,taskname);  _strupr(string2);
                           if (strcmp(string1,string2) == 0)
                             break;
                        }
                      if (i == n)
                        {  ncmilltask.load(0);
                           ncmilltask.update(taskname,width,length,height,origin,partorigin,home);
                        }
                      else
                        { ncmilltask.load(NCMillTask::gettask(NCMillTaskName,taskname));
                          ncmilltask.update(_RCT(""),width,length,height,origin,partorigin,home);
                        }
                      v.setreal("nc::millwidth",width);
                      v.setreal("nc::milllength",length);
                      v.setreal("nc::millheight",height);
                      v.setreal("nc::milloriginx",origin.x);
                      v.setreal("nc::milloriginy",origin.y);
                      v.setreal("nc::milloriginz",origin.z);
                      v.setreal("nc::millpartx",partorigin.x);
                      v.setreal("nc::millparty",partorigin.y);
                      v.setreal("nc::millpartz",partorigin.z);
                      v.setreal("nc::millhomex",home.x);
                      v.setreal("nc::millhomey",home.y);
                      v.setreal("nc::millhomez",home.z);
                      *cstate = ExitState;
                   }
              }
            else
              *cstate = ExitState;
            break;
          case 2 :
            if (event->isa(Coordinate3d))
              {  origin = ((Coordinate3dEvent *) event)->getp();
                 *cstate = 1;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
          case 3 :
            if (event->isa(Coordinate3d))
              {  home = ((Coordinate3dEvent *) event)->getp();
                 *cstate = 1;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
          case 4 :
            if (event->isa(Coordinate3d))
              {  partorigin = ((Coordinate3dEvent *) event)->getp();
                 *cstate = 1;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
         }
    } while (*cstate == 1 || *cstate == 5);
  if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the part origin");
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the home position");
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the part origin");
    }
  else if (*cstate == ExitState)
    delete namelist;
}

class NCToolDisplayDialogControl : public DisplayDialogControl
  {private:
     int lasttoolindex;
   public:
     NCToolDisplayDialogControl(int id,int lti) : DisplayDialogControl(id) {  lasttoolindex = lti;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

static RCCHAR *tooltypel[] = {  _RCT("Unused"), _RCT("End mill"), _RCT("Ball mill"), _RCT("Drill"), _RCT("Radius")  };

void NCToolDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 int i,toolindex,tooltype;
 double x1,y1,w,h,length,diameter,radang,unused,d;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);
  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN) SelectObject(output.gethdc(),hpen);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(100,&toolindex) && toolindex != lasttoolindex)
         {//  Update the tool index information
            ((ListDialogControl *)dialog->getcontrol(101))->change(dialog,ncmilltask.tool[toolindex-1].type);
            ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,ncmilltask.tool[toolindex-1].length);
            ((RealDialogControl *)dialog->getcontrol(103))->change(dialog,ncmilltask.tool[toolindex-1].diameter);
            ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,ncmilltask.tool[toolindex-1].radang);
            ((RealDialogControl *)dialog->getcontrol(105))->change(dialog,ncmilltask.tool[toolindex-1].unused);
            ((RealDialogControl *)dialog->getcontrol(106))->change(dialog,ncmilltask.tool[toolindex-1].offset.x);
            ((RealDialogControl *)dialog->getcontrol(107))->change(dialog,ncmilltask.tool[toolindex-1].offset.y);
            ((RealDialogControl *)dialog->getcontrol(108))->change(dialog,ncmilltask.tool[toolindex-1].offset.z);
            lasttoolindex = toolindex;
         }
       if (dialog->currentvalue(101,&tooltype) &&
           dialog->currentvalue(102,&length) &&
           dialog->currentvalue(103,&diameter) &&
           dialog->currentvalue(104,&radang) &&
           dialog->currentvalue(105,&unused))
         {  x1 = output.getumin();
            y1 = output.getvmin();
            w = output.getumax() - x1;
            h = output.getvmax() - y1;
            if (tooltype == 0)
              {  for (i = 102 ; i <= 108 ; i++)
                   {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),FALSE);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+100),FALSE);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+1000),FALSE);
                   }
                 dialog->SetDlgItemText(204, (char*)"");
                 dialog->SetDlgItemText(205, (char*)"");
              }
            else if (tooltype == 3)
              {  for (i = 102 ; i <= 108 ; i++)
                   {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),i < 105 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+100),i < 105 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+1000),i < 105 || i > 105);
                   }
                 dialog->SetDlgItemText(204, (char*)"Angle");
                 dialog->SetDlgItemText(205, (char*)"");
              }
            else if (tooltype == 4)
              {  for (i = 102 ; i <= 108 ; i++)
                   {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),i < 105 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+100),i < 105 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+1000),i < 105 || i > 105);
                   }
                 dialog->SetDlgItemText(204, (char*)"Radius");
                 dialog->SetDlgItemText(205, (char*)"");
              }
            else
              {  for (i = 102 ; i <= 108 ; i++)
                   {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),i < 104 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+100),i < 104 || i > 105);
                      EnableWindow(GetDlgItem(dialog->gethdlg(),i+1000),i < 104 || i > 105);
                   }
                 dialog->SetDlgItemText(204, (char*)"Radius");
                 dialog->SetDlgItemText(205, (char*)"");
              }
            if (tooltype == 0)
              {  Line line1(Point(x1,y1,0.0),Point(x1+w,y1+h,0.0));
                 line1.draw(DM_NORMAL,NULL,&output);
                 Line line2(Point(x1,y1+h,0.0),Point(x1+w,y1,0.0));
                 line2.draw(DM_NORMAL,NULL,&output);
              }
            else if (tooltype == 1)
              {  Line line1(Point(x1+w/2.0-diameter/2.0,y1+h/2.0,0.0),
                            Point(x1+w/2.0+diameter/2.0,y1+h/2.0,0.0));
                 line1.draw(DM_NORMAL,NULL,&output);
                 Line line2(Point(x1+w/2.0-diameter/2.0,y1+h/2.0,0.0),
                            Point(x1+w/2.0-diameter/2.0,y1+h/2.0+length,0.0));
                 line2.draw(DM_NORMAL,NULL,&output);
                 Line line3(Point(x1+w/2.0+diameter/2.0,y1+h/2.0,0.0),
                            Point(x1+w/2.0+diameter/2.0,y1+h/2.0+length,0.0));
                 line3.draw(DM_NORMAL,NULL,&output);
              }
            else if (tooltype == 2)
              {  Circle circle1(Point(x1+w/2.0,y1+h/2.0+diameter/2.0,0.0),
                                Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),diameter/2.0,0.0,-M_PI);
                 circle1.draw(DM_NORMAL,NULL,&output);
                 Line line2(Point(x1+w/2.0-diameter/2.0,y1+h/2.0+diameter/2.0,0.0),
                            Point(x1+w/2.0-diameter/2.0,y1+h/2.0+diameter/2.0+length,0.0));
                 line2.draw(DM_NORMAL,NULL,&output);
                 Line line3(Point(x1+w/2.0+diameter/2.0,y1+h/2.0+diameter/2.0,0.0),
                            Point(x1+w/2.0+diameter/2.0,y1+h/2.0+diameter/2.0+length,0.0));
                 line3.draw(DM_NORMAL,NULL,&output);
              }
            else if (tooltype == 3)
              {  if (radang < 90)
                   {  radang = 90.0;
                      ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,radang);
                   }
                 radang *= M_PI / 180.0;
                 if (fabs(tan(radang/2.0)) > 1.0E-10)
                   d = diameter/2.0 / tan(radang / 2.0);
                 else
                   d = 0.0;
                 Line line1(Point(x1+w/2.0-diameter/2.0,y1+h/2.0+d,0.0),
                            Point(x1+w/2.0,y1+h/2.0,0.0));
                 line1.draw(DM_NORMAL,NULL,&output);
                 Line line2(Point(x1+w/2.0+diameter/2.0,y1+h/2.0+d,0.0),
                            Point(x1+w/2.0,y1+h/2.0,0.0));
                 line2.draw(DM_NORMAL,NULL,&output);
                 Line line3(Point(x1+w/2.0-diameter/2.0,y1+h/2.0+d,0.0),
                            Point(x1+w/2.0-diameter/2.0,y1+h/2.0+d+length,0.0));
                 line3.draw(DM_NORMAL,NULL,&output);
                 Line line4(Point(x1+w/2.0+diameter/2.0,y1+h/2.0+d,0.0),
                            Point(x1+w/2.0+diameter/2.0,y1+h/2.0+d+length,0.0));
                 line4.draw(DM_NORMAL,NULL,&output);
              }
            else if (tooltype == 4)
              {  if (radang > diameter / 2.0)
                   {  radang = diameter / 2.0;
                      ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,radang);
                   }
                 Line line1(Point(x1+w/2.0-diameter/2.0+radang,y1+h/2.0,0.0),
                            Point(x1+w/2.0+diameter/2.0-radang,y1+h/2.0,0.0));
                 line1.draw(DM_NORMAL,NULL,&output);
                 Circle circle2(Point(x1+w/2.0-diameter/2.0+radang,y1+h/2.0+radang,0.0),
                                Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),radang,M_PI,M_PI / 2.0);
                 circle2.draw(DM_NORMAL,NULL,&output);
                 Circle circle3(Point(x1+w/2.0+diameter/2.0-radang,y1+h/2.0+radang,0.0),
                                Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),radang,0.0,-M_PI / 2.0);
                 circle3.draw(DM_NORMAL,NULL,&output);
                 Line line4(Point(x1+w/2.0-diameter/2.0,y1+h/2.0+radang,0.0),
                            Point(x1+w/2.0-diameter/2.0,y1+h/2.0+radang+length,0.0));
                 line4.draw(DM_NORMAL,NULL,&output);
                 Line line5(Point(x1+w/2.0+diameter/2.0,y1+h/2.0+radang,0.0),
                            Point(x1+w/2.0+diameter/2.0,y1+h/2.0+radang+length,0.0));
                 line5.draw(DM_NORMAL,NULL,&output);
              }
         }
    }
}

class NCToolButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     NCToolButtonDialogControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *);
  };

int NCToolButtonDialogControl::select(Dialog *dialog)
{int toolindex,tooltype;
 double length,diameter,radang,unused;
 Point offset;
  if (dialog->validate() &&
      dialog->currentvalue(100,&toolindex) &&
      dialog->currentvalue(101,&tooltype) &&
      dialog->currentvalue(102,&length) &&
      dialog->currentvalue(103,&diameter) &&
      dialog->currentvalue(104,&radang) &&
      dialog->currentvalue(105,&unused) &&
      dialog->currentvalue(106,&offset.x) &&
      dialog->currentvalue(107,&offset.y) &&
      dialog->currentvalue(108,&offset.z))
    {  ncmilltask.tool[toolindex-1].type = tooltype;
       ncmilltask.tool[toolindex-1].length = length;
       ncmilltask.tool[toolindex-1].diameter = diameter;
       ncmilltask.tool[toolindex-1].radang = radang;
       ncmilltask.tool[toolindex-1].unused = unused;
       ncmilltask.tool[toolindex-1].offset = offset;
       ncmilltask.setcurrenttoolindex(toolindex);
       ncmilltask.update();
    }
  return 0;
}

void nc_command2(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog("NCTool_Dialog");
 DialogControl *dc;
 static double length,diameter,radius,unused;
 static int toolindex,tooltype;
 static Point offset;
 int button;

  do {  switch (*cstate)
         {case InitialState :

            ncmilltask.load(NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()));

            if (NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()) == 0)
              {  cadwindow->MessageBox("There is no NC task active.  Please select an NC task.","NC Tool Library",MB_ICONSTOP);
                 *cstate = ExitState;
                 break;
              }

            
            toolindex = 1;
            tooltype = 0;
            diameter = radius = unused = 0.0;
            offset.setxyz(0.0,0.0,0.0);

            //  No break

          case 1 :
            dialog.add(dc = new IntegerDialogControl(100,&toolindex,1,9999));
            dialog.add(new ScrollBarDialogControl(1100,dc));
            dialog.add(new ListDialogControl(101,5,tooltypel,&tooltype));
            dialog.add(dc = new RealDialogControl(102,&length,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1102,dc));
            dialog.add(dc = new RealDialogControl(103,&diameter,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1103,dc));
            dialog.add(dc = new RealDialogControl(104,&radius,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1104,dc));
            dialog.add(dc = new RealDialogControl(105,&unused));
            dialog.add(new ScrollBarDialogControl(1105,dc));
            dialog.add(dc = new RealDialogControl(106,&offset.x,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1106,dc));
            dialog.add(dc = new RealDialogControl(107,&offset.y,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1107,dc));
            dialog.add(dc = new RealDialogControl(108,&offset.z,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1108,dc));
            dialog.add(new NCButtonDialogControl(109));
            dialog.add(new NCToolButtonDialogControl(110));
            dialog.add((NCToolDisplayDialogControl *) new NCToolDisplayDialogControl(111,*cstate == 1 ? toolindex : -1));
            button = dialog.process();
            if (button == 109)
              *cstate = 2;
            else
              *cstate = ExitState;
            break;
          case 2 :
            if (event->isa(Coordinate3d))
              {  offset = ((Coordinate3dEvent *) event)->getp();
                 *cstate = 1;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
         }
     } while (*cstate == 1);
  if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position of the tool offset");
    }
}

class NCInsertListDialogControl : public ListDialogControl
  {public:
     NCInsertListDialogControl(int id,int n,RCCHAR **list,int *v) : ListDialogControl(id,n,list,v) {  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void NCInsertListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

static int disable0l[] = {  103,203,105,205,106,206,207,107,1107,208,108,1108,209,109,1109,307,311,315,319,-107,-120,0  };
static int disable1l[] = {  103,203,105,205,106,206,311,315,319,-108,-109,-111,-120,0  };
static int disable2l[] = {  315,319,-115,-120,0  };
static int disable3l[] = {  319,-103,-105,-108,-109,-115,-116,-118,-120,0  };
static int disable4l[] = {  311,315,-103,-105,-108,-109,-111,-118,0  };
static int disable5l[] = {  311,315,-103,-105,-108,-109,-111,-118,0  };
static int disable6l[] = {  311,315,-103,-105,-108,-109,-111,-118,-120,-120,0  };
static int disable7l[] = {  319,-103,-105,-112,-112,-118,-120,0  };

static int *disablel[] = {  disable0l,disable1l,disable2l,disable3l,disable4l,disable5l,disable6l,disable7l  };

void NCInsertListDialogControl::changefocus(Dialog *dialog,int)
{int toolpathtype,i,j,enabled;
  if (dialog->currentvalue(100,&toolpathtype))
    {  for (i = 100 ; i < 1121 ; i++)
         if (GetDlgItem(dialog->gethdlg(),i) != 0)
           {  enabled = 1;
              for (j = 0 ; disablel[toolpathtype][j] != 0 ; j++)
                {  if (disablel[toolpathtype][j] == i)
                     {  enabled = 0;
                        break;
                     }
                   if (disablel[toolpathtype][j] < 0)
                     {  if (-disablel[toolpathtype][j] <= i && i <= -disablel[toolpathtype][j+1] ||
                            -disablel[toolpathtype][j]+100 <= i && i <= -disablel[toolpathtype][j+1]+100 ||
                            -disablel[toolpathtype][j]+1000 <= i && i <= -disablel[toolpathtype][j+1]+1000)
                          {  enabled = 0;
                             break;
                          }
                        else
                          j++;
                     }
                }
              EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
              EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
              EnableWindow(GetDlgItem(dialog->gethdlg(),i),enabled);
           }
    }
}

struct RBNC3_data
  {public:
     Point start,end;
  };

void RBNC3(int,void *data,Point *p2,View3dSurface *)
{RBNC3_data *rbdata = (RBNC3_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

//
//  Callback for the pocket function
//

struct CBpocket_data
  {EntityList list;
   int ignore,direction,pathorder;
   Point start,p1last,p2last;
  };

void pocketline(Point p1,Point p2,void *data)
{CBpocket_data *pdata = (CBpocket_data *) data;
 Line *line;
  if (pdata->ignore)
    return;
  if (pdata->list.length() == 0)
    {  pdata->start = p1;
       pdata->p1last = p1;
       pdata->p2last = p2;
    }
  else
    {Line line(pdata->p1last,pdata->p2last);
       if ((line.nearp(p1) - p1).length() > 0.001)
         pdata->direction = ! pdata->direction;
    }
  if (pdata->direction && pdata->pathorder)
    db.geometry.add(line = new Line(p2,p1));
  else
    db.geometry.add(line = new Line(p1,p2));
  pdata->list.add(line);
}

void nc_command3(int *cstate,HighLevelEvent *event,void **data)
{RBNC3_data *rbdata = (RBNC3_data *) *data;
 Dialog dialog("NCInsert_Dialog");
 DialogControl *dc;
 static Point start,origin,lastpickpoint;
 static int toolindex,order,approach,compensation,retractm,toolpathtype,useentityz,path;
 static double feed,spindle,retract,holedepth,peck,plungefeed,retractfeed,initialz,finalz,depthcut,surfaceoffset,stepover,angle,offset;
 static RCCHAR *toolpathtypel[] = {  _RCT("Rapid"),  _RCT("Linear feed"), _RCT("Profile"), _RCT("Pocket"), _RCT("Peck drill"), _RCT("Deep peck drill"), _RCT("Drill"), _RCT("Single surface")  };
 static RCCHAR *orderl[] = {  _RCT("At Start"), _RCT("At End"), _RCT("After")  };
 static RCCHAR *approachl[] = {  _RCT("Direct"),  _RCT("At depth"), _RCT("At height")  };
 static RCCHAR *compensationl[] = {  _RCT("None"), _RCT("Left"), _RCT("Right")  };
 static RCCHAR *pathl[] = {  _RCT("Straight"),  _RCT("Lace")  };
 static EntityList list;
 static Group *toolpath;
 Entity *e;
 static EntityList emptylist,pathlist;
 PointE *p;
 Point p1,p2,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),n1,dir1;
 Loops loops(0);
 CBpocket_data pdata;
 UVSurfaceEntity *se;
  switch (*cstate)
    {case InitialState :
       ncmilltask.load(NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()));
       if (NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()) == 0)
         {  cadwindow->MessageBox("There is no NC task active.  Please select an NC task.","NC Insert toolpath",MB_ICONSTOP);
            *cstate = ExitState;
            break;
         }
       else if (isatoolpath(state.getselection()))
         {  pathlist = state.getselection().copy();
            pathlist.start();
            modifytoolpath((Group *)pathlist.next());
            pathlist.destroy();
            *cstate = ExitState;
            return;
         }
       rbdata = new RBNC3_data;  *data = rbdata;
       if (rbdata == 0)
         *cstate = ExitState;
       toolindex = ncmilltask.getcurrenttoolindex();
       toolpathtype = v.getinteger("nc::toolpathtype");
       order = v.getinteger("nc::order");
       approach = v.getinteger("nc::approach");
       feed = v.getreal("nc::feed");
       spindle = v.getreal("nc::spindle");
       retract = v.getreal("nc::retract");
       holedepth = v.getreal("nc::holedepth");
       peck = v.getreal("nc::peck");
       compensation = v.getinteger("nc::compensation");
       retractm = v.getinteger("nc::retractm");
       plungefeed = v.getreal("nc::plungefeed");
       retractfeed = v.getreal("nc::retractfeed");
       initialz = v.getreal("nc::initialz");
       useentityz = v.getinteger("nc::useentityz");
       finalz = v.getreal("nc::finalz");
       depthcut = v.getreal("nc::depthcut");
       surfaceoffset = v.getreal("nc::surfaceoffset");
       path = v.getinteger("nc::path");
       stepover = v.getreal("nc::stepover");
       angle = v.getreal("nc::angle");

       dialog.add(dc = new NCInsertListDialogControl(100,8,toolpathtypel,&toolpathtype));
       dialog.add(dc = new IntegerDialogControl(101,&toolindex,1,99));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new ListDialogControl(102,3,orderl,&order));
       dialog.add(new ListDialogControl(103,3,compensationl,&compensation));
       dialog.add(new ListDialogControl(104,3,approachl,&approach));
       dialog.add(new ListDialogControl(105,3,approachl,&retractm));
       dialog.add(dc = new RealDialogControl(106,&retract,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1106,dc));
       dialog.add(dc = new RealDialogControl(107,&feed,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1107,dc));
       dialog.add(dc = new RealDialogControl(108,&plungefeed,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1108,dc));
       dialog.add(dc = new RealDialogControl(109,&retractfeed,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1109,dc));
       dialog.add(dc = new RealDialogControl(110,&spindle));
       dialog.add(new ScrollBarDialogControl(1110,dc));
       dialog.add(dc = new RealDialogControl(111,&initialz,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1111,dc));
       dialog.add(new CheckBoxDialogControl(112,&useentityz));
       dialog.add(dc = new RealDialogControl(113,&finalz));
       dialog.add(new ScrollBarDialogControl(1113,dc));
       dialog.add(dc = new RealDialogControl(114,&depthcut,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1114,dc));
       dialog.add(dc = new RealDialogControl(115,&surfaceoffset,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1115,dc));
       dialog.add(new ListDialogControl(116,2,pathl,&path));
       dialog.add(dc = new RealDialogControl(117,&stepover,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1117,dc));
       dialog.add(dc = new RealDialogControl(118,&angle));
       dialog.add(new ScrollBarDialogControl(1118,dc));
       dialog.add(dc = new RealDialogControl(119,&holedepth,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1119,dc));
       dialog.add(dc = new RealDialogControl(120,&peck,ScaleByLength));
       dialog.add(new ScrollBarDialogControl(1120,dc));
       if (dialog.process() == TRUE)
         {  ncmilltask.setcurrenttoolindex(toolindex);
            v.setinteger("nc::toolpathtype",toolpathtype);
            v.setinteger("nc::order",order);
            v.setinteger("nc::approach",approach);
            v.setreal("nc::feed",feed);
            v.setreal("nc::spindle",spindle);
            v.setreal("nc::retract",retract);
            v.setreal("nc::holedepth",holedepth);
            v.setreal("nc::peck",peck);
            v.setinteger("nc::compensation",compensation);
            v.setinteger("nc::retractm",retractm);
            v.setreal("nc::plungefeed",plungefeed);
            v.setreal("nc::retractfeed",retractfeed);
            v.setreal("nc::initialz",initialz);
            v.setinteger("nc::useentityz",useentityz);
            v.setreal("nc::finalz",finalz);
            v.setreal("nc::depthcut",depthcut);
            v.setreal("nc::surfaceoffset",surfaceoffset);
            v.setinteger("nc::path",path);
            v.setreal("nc::stepover",stepover);
            v.setreal("nc::angle",angle);
            if (toolpathtype == 2)
              {  pathlist = state.getselection().copy();
                 state.destroyselection(1);
                 for (pathlist.start() ; (e = pathlist.next()) != 0 ; )
                   if (! e->islinear())
                     break;
                   else
                     origin = ((LinearEntity *)e)->start();
                 if (pathlist.length() == 0)
                   {  cadwindow->MessageBox("Please select the linear entities that make up the contour or pocket first","NC Tool path",MB_ICONINFORMATION);
                      *cstate = ExitState;
                      break;
                   }
                 if (e != 0)
                   {  cadwindow->MessageBox("Please select linear entities only","NC Tool path",MB_ICONINFORMATION);
                      pathlist.destroy();
                      *cstate = ExitState;
                      break;
                   }
                 else
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        {  cadwindow->MessageBox("Please select a single connected path to follow","NC Tool path",MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
              }
            else if (toolpathtype == 3)
              {Loops loops(1);
                 pathlist = state.getselection().copy();
                 state.destroyselection(1);
                 for (pathlist.start() ; (e = pathlist.next()) != 0 ; )
                   if (! e->islinear())
                     break;
                   else
                     origin = ((LinearEntity *)e)->start();
                 if (pathlist.length() == 0)
                   {  cadwindow->MessageBox("Please select the linear entities that make up the pocket first","NC Tool path",MB_ICONINFORMATION);
                      *cstate = ExitState;
                      break;
                   }
                 else if (toolindex < 1 || toolindex > MAX_TOOLS || ncmilltask.tool[toolindex-1].type == 0 ||
                          ncmilltask.tool[toolindex-1].diameter < 1.0E-10)
                   {  cadwindow->MessageBox("Please select a non zero diameter tool to use","NC Tool path",MB_ICONINFORMATION);
                      *cstate = ExitState;
                      break;
                   }
                 if (e != 0)
                   {  cadwindow->MessageBox("Please select linear entities only","NC Tool path",MB_ICONINFORMATION);
                      pathlist.destroy();
                      *cstate = ExitState;
                      break;
                   }
                 else
                   {  loops.create(&pathlist);
                      if (! pathlist.empty() || loops.getloops().empty())
                        {  cadwindow->MessageBox("Please select a closed boundary only","NC Tool path",MB_ICONINFORMATION);
                           pathlist.destroy();
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      list = loops.getloops();
                   }
              }
            else if (toolpathtype == 7)
              {  lastpickpoint = state.getlastpickpoint();
                 pathlist = state.getselection().copy();
                 state.destroyselection(1);
                 for (pathlist.start() ; (e = pathlist.next()) != 0 ; )
                   if (! e->isuvsurface())
                     break;
                 if (pathlist.length() == 0)
                   {  cadwindow->MessageBox("Please select the surface entities that defines the toolpath","NC Tool path",MB_ICONINFORMATION);
                      *cstate = ExitState;
                      break;
                   }
                 if (e != 0)
                   {  cadwindow->MessageBox("Please select surface entities entities only","NC Tool path",MB_ICONINFORMATION);
                      pathlist.destroy();
                      *cstate = ExitState;
                      break;
                   }
              }
            if (order == 2)
              *cstate = 1;
            else
              {  toolpath = 0;
                 *cstate =  100 + 100 * toolpathtype;
              }
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       ncmilltask.includegroup();
       if (event->isa(EntitySelected))
         {  if (! ncmilltask.checktoolpath((Group *)((EntitySelectedEvent *) event)->getentity()))
              cadwindow->MessageBox("The tool path is not part of the current task.","Reorder toolpath",MB_ICONINFORMATION);
            else
              {  toolpath = (Group *)((EntitySelectedEvent *) event)->getentity();
                 *cstate = 100 + 100 * toolpathtype;
              }
         }
       else if (event->isa(Abort))
         *cstate = ExitState;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 100 :
     case 200 :
     case 500 :
     case 600 :
     case 700 :
       if (event->isa(Coordinate3d))
         {  if (list.length() == 0)
              db.saveundo(UD_STARTBLOCK,NULL);
            list.add(p = new PointE(((Coordinate3dEvent *)event)->getp()));
            db.geometry.add(p);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  if (list.length() > 0)
              {  list.reverse();
                 if (toolpathtype == 0)
                   ncmilltask.insertrapid(toolpath,list,toolindex,order,approach);
                 else if (toolpathtype == 1)
                   ncmilltask.insertfeed(toolpath,list,toolindex,order,approach,feed,spindle);
                 else
                   ncmilltask.insertdrill(toolpath,list,toolpathtype,toolindex,order,feed,spindle,retract,holedepth,peck);
                 list = emptylist;
              }
            db.saveundo(UD_ENDBLOCK,NULL);
            *cstate = ExitState;
         }
       break;
     case 300 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p1 = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p1).length() > (rbdata->end - p1).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 list.reverse();
              }
            list.add(p = new PointE(rbdata->start));
            db.geometry.add(p);
            ncmilltask.insertcontour(toolpath,list,toolindex,order,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,useentityz,finalz,depthcut);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         list.destroy();
       list = emptylist;
       *cstate = ExitState;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       ncmilltask.excludegroup();
       cadwindow->prompt->normalprompt("Locate the tool path before the new toolpath that is to be inserted");
    }
  else if (*cstate == 100)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position to rapid to");
    }
  else if (*cstate == 200)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position to feed to");
    }
  else if (*cstate == 300)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the NC path");
       rubberband.begin(0);
       rubberband.add(RBNC3,rbdata);
    }
  else if (*cstate == 400)
    {//  First offset the boundaries in list -- Largest area loop is offset to reduce area, others to increase area.
     Loops loops(1);
     EntityList l,emptylist,olist1,olist2;
     int i,imax;
     double maxarea,area;
     Entity *e;
     LinearEntity *le;
       maxarea = 0.0;  imax = -1;
       for (list.start(),i = 0 ; ! list.atend() ; i++)
         {  le = (LinearEntity *)list.next();
            if (le != 0)
              l.add(le);
            if (le == 0 || list.atend())
              {Polygon3d polygon3d(origin,xaxis,yaxis,l,0,0.0);
                 area = polygon3d.getarea();
                 if (fabs(area) > fabs(maxarea))
                   {  maxarea = area;
                      imax = i;
                  }
                 l = emptylist;
              }
         }
       for (list.start(),i = 0 ; ! list.atend() ; i++)
         {  le = (LinearEntity *)list.next();
            if (le != 0)
              l.add(le);
            if (le == 0 || list.atend())
              {Loops loops(1);
               double area1,area2;
                loops.create(&l);
                Polygon3d polygon1(origin,xaxis,yaxis,loops.getloops().copy(),0,0.0);
                if (toolindex < 1 || toolindex > MAX_TOOLS || ncmilltask.tool[toolindex-1].type == 0)
                  offset = 0.0;
                else
                  offset = ncmilltask.tool[toolindex-1].diameter / 2.0;
                loops.offset(0,0,origin,offset,&olist1);
                Polygon3d polygon2(origin,xaxis,yaxis,olist1,0,0.0);
                area1 = polygon1.getarea();
                area2 = polygon2.getarea();
                if (i == imax && (area1 > 0.0 && area1 < area2 || area1 < 0.0 && area1 > area2) ||
                    i != imax && (area1 > 0.0 && area1 > area2 || area1 < 0.0 && area1 < area2))
                  {  loops.offset(0,0,origin,-offset,&olist1);
                  }
                if (olist2.length() > 0)
                  olist2.add(0);
                for (olist1.start() ; (e = olist1.next()) != 0 ; )
                  olist2.add(e);
                l = emptylist;
              }
         }
       Polygon3d polygon3d(origin,xaxis,yaxis,olist2,0,0.0);
       pdata.ignore = 1;
       pdata.direction = 0;
       pdata.pathorder = path;
       dir1 = xaxis * cos(angle * M_PI / 180.0) + yaxis * sin(angle * M_PI / 180.0);
       n1 = (dir1.cross(xaxis.cross(yaxis))).normalize();
       p1 = origin;
       while (! polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,pocketline,&pdata))
         p1 -= n1 * stepover;
       pdata.ignore = 0;
       p1 += n1 * stepover;
       while (! polygon3d.hatchline(1,n1,-n1.dot(p1),p1,dir1,pocketline,&pdata))
         p1 += n1 * stepover;
       pdata.list.reverse();
       pdata.list.add(p = new PointE(pdata.start));
       db.geometry.add(p);
       pdata.list.reverse();
       olist2.reverse();
       for (olist2.start() ; ! olist2.atend() ; )
         if ((e = olist2.next()) != 0)
           {  db.geometry.add(e);
              pdata.list.add(e);
           }
       olist2.destroy();
       pdata.list.reverse();
       ncmilltask.insertcontour(toolpath,pdata.list,toolindex,order,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,useentityz,finalz,depthcut);
       *cstate = ExitState;
       list = emptylist;
    }
  else if (*cstate == 500)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position to peck drill");
    }
  else if (*cstate == 600)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position to deep peck drill");
    }
  else if (*cstate == 700)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position to drill");
    }
  else if (*cstate == 800)
    {  for (pathlist.start() ; (se = (UVSurfaceEntity *) pathlist.next()) != 0 ; )
         ncmilltask.insertcontour(toolpath,se,lastpickpoint,toolindex,order,compensation,approach,retractm,retract,feed,plungefeed,retractfeed,spindle,initialz,1,finalz,depthcut,path,stepover,surfaceoffset);
       *cstate = ExitState;
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void nc_command4(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog("NCDelete_Dialog");
 static int deleteentities;
   switch (*cstate)
    {case InitialState :

       ncmilltask.load(NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()));

       if (NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()) == 0)
         {  cadwindow->MessageBox("There is no NC task active.  Please select an NC task.","NC Delete toolpaths",MB_ICONSTOP);
            *cstate = ExitState;
            break;
         }


       deleteentities = v.getinteger("nc::deleteentities");
       dialog.add(new CheckBoxDialogControl(100,&deleteentities));
       if (dialog.process() == TRUE)
         {  *cstate = 1;
            v.setinteger("nc::deleteentities",deleteentities);
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            if (! ncmilltask.deletetoolpath((Group *)((EntitySelectedEvent *) event)->getentity(),deleteentities))
              cadwindow->MessageBox("The tool path was not part of the current task and was not deleted.","Delete toolpath",MB_ICONINFORMATION);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       ncmilltask.excludegroup();
       cadwindow->prompt->normalprompt("Locate the tool path to delete from the current task");
    }
  else
    ncmilltask.includegroup();
}

void nc_command5(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog("NCReorder_Dialog");
 static RCCHAR *reorderl[] = {  _RCT("To Start"), _RCT("To End"), _RCT("After")  };
 static int reorder;
 static Group *group;
   switch (*cstate)
    {case InitialState :

       ncmilltask.load(NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()));

       if (NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()) == 0)
         {  cadwindow->MessageBox("There is no NC task active.  Please select an NC task.","NC Reorder toolpaths",MB_ICONSTOP);
            *cstate = ExitState;
            break;
         }

       reorder = v.getinteger("nc::reorder");
       dialog.add(new ListDialogControl(100,3,reorderl,&reorder));
       if (dialog.process() == TRUE)
         {  *cstate = 1;
            v.setinteger("nc::reorder",reorder);
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  if (! ncmilltask.checktoolpath((Group *)((EntitySelectedEvent *) event)->getentity()))
              cadwindow->MessageBox("The tool path is not part of the current task.","Reorder toolpath",MB_ICONINFORMATION);
            else
              {  group = (Group *)((EntitySelectedEvent *) event)->getentity();
                 if (reorder == 2)
                   *cstate = 2;
                 else
                   ncmilltask.reordertoolpath(group,0,reorder);
              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       if (event->isa(EntitySelected))
         {  if (! ncmilltask.checktoolpath((Group *)((EntitySelectedEvent *) event)->getentity()))
              cadwindow->MessageBox("The tool path is not part of the current task.","Reorder toolpath",MB_ICONINFORMATION);
            else
              {  ncmilltask.reordertoolpath(group,(Group *)((EntitySelectedEvent *) event)->getentity(),reorder);
                 *cstate = 1;
              }
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       ncmilltask.excludegroup();
       cadwindow->prompt->normalprompt("Locate the tool path to reorder");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the tool path before the tool path to move");
    }
  else
    ncmilltask.includegroup();
}

class NCFileButtonDialogControl : public ButtonDialogControl
  {public:
     NCFileButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int NCFileButtonDialogControl::select(Dialog *dialog)
{OPENFILENAME ofn;
 RCCHAR postprocessor[300],dirname[300],file[300],filetitle[300],filter[300],extension[300];
 int i,finished;
#if ! defined(_MSC_VER)
 //struct ffblk ffblk;
#endif
#if ! defined(_WIN32_WCE)
 OFSTRUCT ll;
#endif
 RCHINSTANCE postinstance;
 RCCHAR * (WINAPI *NCPostName)(void);
 RCCHAR * (WINAPI *NCFileExtension)(void);
 RCCHAR * (WINAPI *NCFilePrompt)(void);

  if (id == 103 || id == 105)
    {  dialog->store();
       return id;
    }
  else if (id == 111)
    {  
       _getcwd(dirname, sizeof(dirname));
#if ! defined(_WIN32_WCE)
       file[0] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lpstrTitle = _RCT("Select the AVI file name");
       for (i = 0; filter[i] != '\0'; i++)
         if (filter[i] == '|') filter[i] = '\0';
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = dialog->gethdlg();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile= file;
       ofn.nMaxFile = sizeof(file);
       ofn.lpstrFileTitle = filetitle;
       ofn.nMaxFileTitle = sizeof(filetitle);
       ofn.lpstrInitialDir = dirname;
       ofn.lpstrDefExt = _RCT("AVI");
       ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
       if (GetSaveFileName(&ofn))
         ((StringDialogControl *)dialog->getcontrol(110))->change(dialog,ofn.lpstrFile);
//WCEFIX
#endif
      return 0;
    }

  dialog->currentvalue(104,postprocessor,300);
#if ! defined(_WIN32_WCE)
#if defined(_MSC_VER) || defined(_MAC)
  RC_finddata_t fdfile;
  int handle;

  handle = _findfirst(home.getexecutablefilename(_RCT("mpost*.dll")),&fdfile);
  finished = 0;
  while (handle != -1)
    {  if (OpenFile(home.getexecutablefilename(CHAR2RCCHR(fdfile.name)),&ll,OF_EXIST) != HFILE_ERROR)
         {  if ((postinstance = LoadLibrary(home.getexecutablefilename(CHAR2RCCHR(fdfile.name)))) != 0)
              {  NCPostName = (RCCHAR *(WINAPI *)(void))GetProcAddress(postinstance,"NCPostName");
                 if (NCPostName != 0 && strcmp(postprocessor,NCPostName()) == 0)
                   {  NCFileExtension = (RCCHAR *(WINAPI *)(void))GetProcAddress(postinstance,"NCFileExtension");
                      NCFilePrompt = (RCCHAR *(WINAPI *)(void))GetProcAddress(postinstance,"NCFilePrompt");
                      if (NCFileExtension != 0 && NCFilePrompt != 0)
                        {  strcpy(filter,NCFilePrompt());
                           strcpy(extension,NCFileExtension());
                           break;
                        }
                   }
                 FreeLibrary(postinstance);
				  }
         }
       if (_findnext(handle,&fdfile) != 0)
         {  _findclose(handle);
            handle = -1;
            finished = 1;
         }
    }
  if (finished)
    {  cadwindow->MessageBox("Post processor is not available","Generate",MB_ICONSTOP);
       return 0;
    }

#else
  finished = findfirst(home.getexecutablefilename(_RCT("mpost*.dll")),&ffblk,0);

  while (! finished)
    {  if (OpenFile(home.getexecutablefilename(ffblk.ff_name),&ll,OF_EXIST) != HFILE_ERROR)
         {  if ((postinstance = LoadLibrary(home.getexecutablefilename(ffblk.ff_name))) != 0)
              {  (FARPROC) NCPostName = GetProcAddress(postinstance,"NCPostName");
                 if (NCPostName != 0 && strcmp(postprocessor,NCPostName()) == 0)
                   {  (FARPROC) NCFileExtension = GetProcAddress(postinstance,"NCFileExtension");
                      (FARPROC) NCFilePrompt = GetProcAddress(postinstance,"NCFilePrompt");
                      if (NCFileExtension != 0 && NCFilePrompt != 0)
                        {  strcpy(filter,NCFilePrompt());
                           strcpy(extension,NCFileExtension());
                           break;
                        }
                   }
                 FreeLibrary(postinstance);
				  }
         }
       finished = findnext(&ffblk);
    }
  if (finished)
    {  cadwindow->MessageBox("Post processor is not available","Generate",MB_ICONSTOP);
       return 0;
    }
#endif

  _getcwd(dirname, sizeof(dirname));
  file[0] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lpstrTitle = _RCT("Select the post processor file name");
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= file;
  ofn.nMaxFile = sizeof(file);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = extension;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  if (GetSaveFileName(&ofn))
    ((StringDialogControl *)dialog->getcontrol(106))->change(dialog,ofn.lpstrFile);
//WCEFIX
#endif
  return 0;
}

class GMCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     GMCheckBoxDialogControl(int id,int *v) : CheckBoxDialogControl(id,v) {} ;
     int select(Dialog *dialog);
     void load(Dialog *dialog);
  };

void GMCheckBoxDialogControl::load(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),109),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1109),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),209),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),309),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),210),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),110),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),111),oldvalue);
  CheckBoxDialogControl::load(dialog);
}

int GMCheckBoxDialogControl::select(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),109),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1109),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),209),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),309),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),210),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),110),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),111),! oldvalue);
  return CheckBoxDialogControl::select(dialog);
}

void nc_command6(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("NCTest_Dialog");
 DialogControl *dc;
 int tooltrails,linetrails,generatemodel;
#if ! defined(_WIN32_WCE)
 OFSTRUCT ll;
#endif
 RCHINSTANCE postinstance;
#if ! defined(_MSC_VER) || defined(_MAC)
 //struct ffblk ffblk;
#endif
 RCCHAR postname[300],lastpostname[300],filename[300],avifilename[300],*namel[100],*pathl[100];
 int i,post,nposts,finished,button;
 double stepsize,displayevery;
 RCCHAR *(WINAPI *NCPostName)(void);

  ncmilltask.load(NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()));

  if (NCMillTask::gettask(NCMillTaskName,ncmilltask.getcurrenttaskname()) == 0)
    {  cadwindow->MessageBox("There is no NC task active.  Please select an NC task.","NC Test and Generate",MB_ICONSTOP);
       *cstate = ExitState;
       return;
    }

  linetrails = v.getinteger("nc::linetrails");
  tooltrails = v.getinteger("nc::tooltrails");
  generatemodel = v.getinteger("nc::generatemodel");
  displayevery = v.getreal("nc::displayevery");
  stepsize = v.getreal("nc::stepsize");
  strcpy(lastpostname,v.getstring("nc::postname"));
  strcpy(filename,v.getstring("nc::filename"));
  strcpy(avifilename,v.getstring("nc::avifilename"));

#if ! defined(_WIN32_WCE)
#if defined(_MSC_VER) || defined(_MAC)
  struct RC_finddata_t fdfile;
  int handle;

  handle = _findfirst(home.getexecutablefilename(_RCT("mpost*.dll")),&fdfile);
  nposts = post = 0;
  while (handle != -1)
    {  if (OpenFile(home.getexecutablefilename(CHAR2RCCHR(fdfile.name)),&ll,OF_EXIST) != HFILE_ERROR)
         {  if ((postinstance = LoadLibrary(home.getexecutablefilename(CHAR2RCCHR(fdfile.name)))) != 0)
              {  NCPostName = (RCCHAR *(WINAPI *)(void))GetProcAddress(postinstance,"NCPostName");
                 if (NCPostName != 0)
                   {  strcpy(postname,NCPostName());
                      namel[nposts] = new RCCHAR[strlen(postname)+1];
                      strcpy(namel[nposts],postname);
                      pathl[nposts] = new RCCHAR[strlen(home.getexecutablefilename(CHAR2RCCHR(fdfile.name)))+1];
                      strcpy(pathl[nposts],home.getexecutablefilename(CHAR2RCCHR(fdfile.name)));
                      if (strcmp(postname,lastpostname) == 0)
                        post = nposts;
                      nposts++;
                   }
                 FreeLibrary(postinstance);
	          }
         }
       if (_findnext(handle,&fdfile) != 0)
         {  _findclose(handle);
            handle = -1;
            finished = 1;
         }
    }

#else

  finished = findfirst(home.getexecutablefilename(_RCT("mpost*.dll")),&ffblk,0);
  nposts = post = 0;
  while (! finished)
    {  if (OpenFile(home.getexecutablefilename(ffblk.ff_name),&ll,OF_EXIST) != HFILE_ERROR)
         {  if ((postinstance = LoadLibrary(home.getexecutablefilename(ffblk.ff_name))) != 0)
              {  (FARPROC) NCPostName = GetProcAddress(postinstance,"NCPostName");
                 if (NCPostName != 0)
                   {  strcpy(postname,NCPostName());
                      namel[nposts] = new RCCHAR[strlen(postname)+1];
                      strcpy(namel[nposts],postname);
                      pathl[nposts] = new RCCHAR[strlen(home.getexefilename(ffblk.ff_name))+1];
                      strcpy(pathl[nposts],home.getexecutablefilename(ffblk.ff_name));
                      if (strcmp(postname,lastpostname) == 0)
                        post = nposts;
                      nposts++;
                   }
                 FreeLibrary(postinstance);
				  }
         }
       finished = findnext(&ffblk);
    }
#endif
//WCEFIX
#endif

  dialog.add(new CheckBoxDialogControl(100,&linetrails));
  dialog.add(new CheckBoxDialogControl(101,&tooltrails));
  dialog.add(new GMCheckBoxDialogControl(102,&generatemodel));
  dialog.add(dc = new RealDialogControl(109,&displayevery,1.0,0.001,1.0E10));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(dc = new RealDialogControl(108,&stepsize,1.0,1E-10,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(new NCFileButtonDialogControl(103));
  dialog.add(new ListDialogControl(104,nposts,namel,&post));
  dialog.add(new NCFileButtonDialogControl(105));
  dialog.add(new StringDialogControl(106,filename,300));
  dialog.add(new NCFileButtonDialogControl(107));
  dialog.add(new StringDialogControl(110,avifilename,300));
  dialog.add(new NCFileButtonDialogControl(111));

  button = dialog.process();
  if (button == 103)
    {  v.setinteger("nc::linetrails",linetrails);
       v.setinteger("nc::tooltrails",tooltrails);
       v.setinteger("nc::generatemodel",generatemodel);
       v.setreal("nc::displayevery",displayevery);
       v.setstring("nc::postname",nposts > 0 ? namel[post] : _RCT(""));
       v.setstring("nc::filename",filename);
       v.setstring("nc::avifilename",avifilename);
       v.setreal("nc::stepsize",stepsize);
       ncmilltask.test(linetrails,tooltrails,stepsize,generatemodel,(int)displayevery);
    }
  else if (button == 105)
    {  v.setinteger("nc::linetrails",linetrails);
       v.setinteger("nc::tooltrails",tooltrails);
       v.setinteger("nc::generatemodel",generatemodel);
       v.setreal("nc::displayevery",displayevery);
       v.setstring("nc::postname",nposts > 0 ? namel[post] : _RCT(""));
       v.setstring("nc::filename",filename);
       v.setstring("nc::avifilename",avifilename);
       v.setreal("nc::stepsize",stepsize);
       ncmilltask.generate(pathl[post],filename);
    }
  for (i = 0 ; i < nposts ; i++)
    {  delete [] namel[i];
       delete [] pathl[i];
    }
  *cstate = ExitState;
}


void nc_command7(int *cstate,HighLevelEvent *event,void **)
{Dialog dialog("NCOrthotiCAD_Dialog");
 DialogControl *dc;
 Entity *e;
 double width,height,length;
 Point home,origin,partorigin;
 int order,result,n,cs,compensation;
 BitMask options(32);
 Point pmin,pmax,pmin1,pmax1,start,end,p1,middle_of_surface;
 int contour_cut_first,contour_enabled,side_enabled;
 double contour_tool_diameter,contour_offset,contour_feed_rate,contour_spindle_speed;
 double side_tool_diameter,side_feed_rate,side_spindle_speed,thickness;
 EntityList sidelist,list;
 Loops loops(0);
 Group *toolpath;
 PointE *p;
 UVSurfaceEntity *uvsurface;
 EntityList deletelist;
 RCCHAR filename[300];
  
    contour_cut_first = v.getinteger("oc::contour_cut_first");
    contour_enabled = v.getinteger("oc::contour_enabled");
    contour_tool_diameter = v.getreal("oc::contour_tool_diameter");
    contour_offset = v.getreal("oc::contour_offset");
    contour_feed_rate = v.getreal("oc::contour_feed_rate");
    contour_spindle_speed = v.getreal("oc::contour_spindle_speed");
    side_enabled = v.getinteger("oc::side_enabled");
    side_tool_diameter = v.getreal("oc::side_tool_diameter");
    side_feed_rate = v.getreal("oc::side_feed_rate");
    side_spindle_speed = v.getreal("oc::side_spindle_speed");
    thickness = v.getreal("oc::thickness");

    dialog.add(new CheckBoxDialogControl(100,&contour_cut_first));
    dialog.add(new CheckBoxDialogControl(101,&contour_enabled));
    dialog.add(dc = new RealDialogControl(102,&contour_tool_diameter,ScaleByLength));
    dialog.add(dc = new ScrollBarDialogControl(1102,dc));
    dialog.add(dc = new RealDialogControl(103,&contour_offset,ScaleByLength));
    dialog.add(dc = new ScrollBarDialogControl(1103,dc));
    dialog.add(dc = new RealDialogControl(104,&contour_feed_rate,ScaleByLength));
    dialog.add(dc = new ScrollBarDialogControl(1104,dc));
    dialog.add(dc = new RealDialogControl(105,&contour_spindle_speed,DontScale));
    dialog.add(dc = new ScrollBarDialogControl(1105,dc));
    dialog.add(new CheckBoxDialogControl(106,&side_enabled));
    dialog.add(dc = new RealDialogControl(107,&side_tool_diameter,ScaleByLength));
    dialog.add(dc = new ScrollBarDialogControl(1107,dc));
    dialog.add(dc = new RealDialogControl(108,&side_feed_rate,ScaleByLength));
    dialog.add(dc = new ScrollBarDialogControl(1108,dc));
    dialog.add(dc = new RealDialogControl(109,&side_spindle_speed,DontScale));
    dialog.add(dc = new ScrollBarDialogControl(1109,dc));
    dialog.add(dc = new RealDialogControl(110,&thickness,DontScale));
    dialog.add(dc = new ScrollBarDialogControl(1110,dc));
    dialog.add(dc = new ButtonDialogControl(111));

    if ((result = dialog.process()) != FALSE)
    {

        strcpy(filename,db.getname());
        if (strrchr(filename,'.') != 0)
            strcpy(strrchr(filename,'.'),".ncm");
        else
            strcat(filename,".ncm");
        v.setstring("nc::filename",filename);

        toolpath = NCMillTask::gettask(NCMillTaskName,_RCT("Orthotic"));

        if (toolpath != 0)
        {   db.geometry.clearstatus();
            toolpath->setstatus(1);
            toolpath->dbsetassociated();
            for (db.geometry.start() ; e = db.geometry.next() ; )
                if (e->getlayer() == 100)
                    e->setstatus(0);
            
            for (db.geometry.start() ; e = db.geometry.next() ; )
                if (e->getstatus() != 0)
                    deletelist.add(e);

            state.setselection(deletelist,1);
            delete_command(&cs,0,0);

        }

        if (result == 111)
        {   *cstate = ExitState;
            return;  // Delete the tool paths only
        }

        v.setinteger("oc::contour_cut_first",contour_cut_first);
        v.setinteger("oc::contour_enabled",contour_enabled);
        v.setreal("oc::contour_tool_diameter",contour_tool_diameter);
        v.setreal("oc::contour_offset",contour_offset);
        v.setreal("oc::contour_feed_rate",contour_feed_rate);
        v.setreal("oc::contour_spindle_speed",contour_spindle_speed);
        v.setinteger("oc::side_enabled",side_enabled);
        v.setreal("oc::side_tool_diameter",side_tool_diameter);
        v.setreal("oc::side_feed_rate",side_feed_rate);
        v.setreal("oc::side_spindle_speed",side_spindle_speed);
        v.setreal("oc::thickness",thickness);

        pmin = pmax = Point();
        uvsurface = 0;
        for (db.geometry.start(),n = 0 ; (e = db.geometry.next()) != 0 ; )
            if ((e->isa(curve_entity) || e->isa(patch_entity) && ((Patch *)e)->getoptions().test(PatchTriangulated)) && e->getlayer() == 100)
            {   if (n == 0)
                    e->extents(0,&pmin,&pmax);
                else
                {   e->extents(0,&pmin1,&pmax1);
                    if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                    if (pmin1.y < pmin.y) pmin.y = pmin1.y;
                    if (pmin1.z < pmin.z) pmin.z = pmin1.z;
                    if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                    if (pmax1.y > pmax.y) pmax.y = pmax1.y;
                    if (pmax1.z > pmax.z) pmax.z = pmax1.z;
                }
                if (e->isa(curve_entity))
                    sidelist.add(e);
                else
                {   e->extents(0,&pmin1,&pmax1);
                    middle_of_surface = (pmin1 + pmax1) / 2.0;
                    uvsurface = (Patch *)e;
                }
           
                n++;
            }

        length = pmax.x - pmin.x;
        width = pmax.y - pmin.y;
        height = thickness;
       
        origin.x = pmin.x;
        origin.y = pmin.y;
        origin.z = pmin.z;

        partorigin.x = pmax.x;
        partorigin.y = pmax.y;
        partorigin.z = pmax.z;

        home.x = pmax.x;
        home.y = pmax.y;
        home.z = thickness + 50.0;

        ncmilltask.load(0);
        ncmilltask.update(_RCT("Orthotic"),width,length,height,origin,partorigin,home);

        for (order = 0 ; order < 2 ; order++)
        {

            if (side_enabled && (contour_cut_first != 0 && order == 1 || contour_cut_first == 0 && order == 0))
            {   loops.create(&sidelist);
                list = loops.getloops();
                if (list.length() > 0)
                {   start = loops.getstart();
                    end = loops.getend();

                    list.start();
                    e = list.next();
                    if ((((LinearEntity *)e)->start() - start).length() < (((LinearEntity *)e)->end() - start).length())
                    {   if ((middle_of_surface - start).cross(((LinearEntity *)e)->direction(0.0)).z < 0.0)
                            compensation = 2;
                        else
                            compensation = 1;
                    }
                    else
                    {   if ((middle_of_surface - start).cross(((LinearEntity *)e)->direction(1.0)).z > 0.0)
                            compensation = 2;
                        else
                            compensation = 1;
                    }

                    toolpath = NCMillTask::gettask(NCMillTaskName,_RCT("Orthotic"));

                    list.add(p = new PointE(start));
                    db.geometry.add(p);

                    ncmilltask.insertcontour(toolpath,list,2,1,compensation,2,2,thickness + 2.0,side_feed_rate,side_feed_rate/5.0,side_feed_rate,side_spindle_speed,0.0,1,0.0,0.0);

                }
            }

            if (contour_enabled && (contour_cut_first == 0 && order == 1 || contour_cut_first != 0 && order == 0))
            {   if (uvsurface != 0)
                {   ncmilltask.insertcontour(toolpath,uvsurface,middle_of_surface,1,2,0,2,2,thickness + 2.0,contour_feed_rate,contour_feed_rate/5.0,contour_feed_rate,contour_spindle_speed,0.0,1,0.0,0.0,1,contour_offset,contour_tool_diameter/2.0);
                }
            }
        }

        ncmilltask.tool[0].type = 2;
        ncmilltask.tool[0].length = 40.0;
        ncmilltask.tool[0].diameter = contour_tool_diameter;
        ncmilltask.tool[0].radang = 0.0;
        ncmilltask.tool[0].unused = 0.0;
        ncmilltask.tool[0].offset = Point(0.0,0.0,0.0);

        ncmilltask.tool[1].type = 1;
        ncmilltask.tool[1].length = 40.0;
        ncmilltask.tool[1].diameter = side_tool_diameter;
        ncmilltask.tool[1].radang = 0.0;
        ncmilltask.tool[1].unused = 0.0;
        ncmilltask.tool[1].offset = Point(0.0,0.0,0.0);

        ncmilltask.update();

    }

    *cstate = ExitState;
}

#endif
//
//  NCSolid Implementation
//

NCSolid::NCSolid(const EntityHeader &header,Point o,Point xa,Point ya,Point sz,int maxsegs,int xm,int ym) :
  SurfaceEntity(header)
{Point d;
 int i,j;
  origin = o;
  xaxis = xa;
  yaxis = ya;
  zaxis = xaxis.cross(yaxis).normalize();
  xmesh = short(xm);
  ymesh = short(ym);

  d = sz / maxsegs;
  if (d.x >= d.y && d.x >= d.z)
    delta = d.x;
  else if (d.y >= d.x && d.y >= d.z)
    delta = d.y;
  else
    delta = d.z;

  nx = short(sz.x / delta);
  ny = short(sz.y / delta);
  nz = short(sz.z / delta);

  sheight = new double[nx*ny];

  if (sheight == 0)
    defined = 0;
  else
    {  for (i = 0 ; i < nx ; i++)
         for (j = 0 ; j < ny ; j++)
           sheight[i*ny + j] = nz - 1;
    }
  twidth = 0;
  theight = 0;

  dirty = 0;

}

NCSolid::NCSolid(Point o,Point xa,Point ya,Point sz,int maxsegs,int xm,int ym)
{Point d;
 int i,j;
  origin = o;
  xaxis = xa;
  yaxis = ya;
  zaxis = xaxis.cross(yaxis).normalize();
  xmesh = short(xm);
  ymesh = short(ym);

  d = sz / maxsegs;
  if (d.x >= d.y && d.x >= d.z)
    delta = d.x;
  else if (d.y >= d.x && d.y >= d.z)
    delta = d.y;
  else
    delta = d.z;

  nx = short(sz.x / delta);
  ny = short(sz.y / delta);
  nz = short(sz.z / delta);

  sheight = new double[nx*ny];

  if (sheight == 0)
    defined = 0;
  else
    {  for (i = 0 ; i < nx ; i++)
         for (j = 0 ; j < ny ; j++)
           sheight[i*ny + j] = (nz - 1);
    }
  twidth = 0;
  theight = 0;

  dirty = 0;

}

NCSolid::NCSolid(const EntityHeader &header,Point o,Point xa,Point ya,double del,short nx1,short ny1,short nz1,short xm,short ym,double *sh) :
  SurfaceEntity(header)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
  zaxis = xaxis.cross(yaxis).normalize();
  xmesh = short(xm);
  ymesh = short(ym);

  delta = del;

  nx = nx1;
  ny = ny1;
  nz = nz1;

  sheight = new double [nx * ny];

  if (sheight == 0)
    defined = 0;
  else
    memcpy(sheight,sh,sizeof(double) * nx * ny);
  twidth = 0;
  theight = 0;

}

NCSolid::~NCSolid()
{ delete [] sheight;
  delete [] theight;
}

double NCSolid::pick(View3dSurface *surface,double x,double y,Point *pos)
{ if (pos == NULL && surface->displaylist.defined(this))
    return surface->displaylist.pick(this,x,y);
  return 0.0;
}

int NCSolid::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{int i,j,count;
  if (vs->displaylist.defined(this) && vs->displaylist.pick(this,inside,x1,y1,x2,y2))
    return 1;
  count = 0;
  count += vs->pick(origin,inside,x1,y1,x2,y2);
  count += vs->pick(origin+xaxis*(nx-1)*delta,inside,x1,y1,x2,y2);
  count += vs->pick(origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta,inside,x1,y1,x2,y2);
  count += vs->pick(origin+yaxis*(ny-1)*delta,inside,x1,y1,x2,y2);
  for (i = 0 ; i < nx ; i++)
    for (j = 0 ; j < ny ; j++)
      count += vs->pick(origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta,inside,x1,y1,x2,y2);
  return count == nx * ny + 4;
}

int NCSolid::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{ return 0;
}

struct CBdrawline_data
  {View3dSurface *surface;
   Transform *transform;
   long count;
  };

int NCSolid::facet(const BitMask &foptions,double,int,int,View3dSurface *,PolygonCallBack cb,void *data)
{int i,j,j1,h1,h2,square,deltacolour;
 Point p1,p2,p3,p4,p5,n,xa,ya,za;
 double u1,u2,v1,v2;
 Triangle *triangle;
 Polygon3d *poly;


  xa = xaxis * delta;
  ya = yaxis * delta;
  za = zaxis * delta;

  if (foptions.test(1))
    {Point p[4];
       p[0] = origin;
       p[1] = origin+xa*nx;
       p[2] = origin+xa*nx+ya*ny;
       p[3] = origin+ya*ny;
       cb(this,0,4,p,data);
    }

  for (i = -1 ; i < nx ; i++)
    for (j = -1 ; j < ny ; j++)
      {  if (i == -1)
           {  if (j == -1 || j == ny - 1)
                continue;
              p1 = origin+ya*j;
              p2 = p1+za*sheight[(i+1)*ny + j];
              h2 = (int)sheight[(i+1)*ny + j];
              j1 = j + 1;
              if (h2 == sheight[(i+1)*ny + j1])
                {  while (j1 < ny - 1 && h2 == sheight[(i+1)*ny + j1 + 1])
                     j1++;
                }
              p3 = origin+ya*j1;
              p4 = p3+za*sheight[(i+1)*ny + j1];
              u1 = 0.0;
              u2 = 0.0;
              v1 = double(j) / double(ny-1);
              v2 = double(j1) / double(ny-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }
         else if (i == nx - 1)
           {  if (j == -1 || j == ny - 1)
                continue;
              p2 = origin+xa*i+ya*j;
              p1 = p2+za*sheight[i*ny + j];
              h1 = (int)sheight[i*ny + j];
              j1 = j + 1;
              if (h1 == sheight[i*ny + j1])
                {  while (j1 < ny - 1 && h1 == sheight[i*ny + j1 + 1])
                     j1++;
                }
              p4 = origin+xa*i+ya*j1;
              p3 = p4+za*sheight[i*ny + j1];
              u1 = 1.0;
              u2 = 1.0;
              v1 = double(j) / double(ny-1);
              v2 = double(j1) / double(ny-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }
         else if (j == -1)
           {  if (i == -1 || i == nx - 1)
                continue;
              p1 = origin+xa*i;
              p2 = origin+xa*(i+1);
              p3 = p1+za*sheight[i*ny + j + 1];
              p4 = p2+za*sheight[(i+1)*ny + j + 1];
              u1 = double(i) / double(nx-1);
              u2 = double(i+1) / double(nx-1);
              v1 = 0.0;
              v2 = 0.0;
              square = 0;
           }
         else if (j == ny - 1)
           {  if (i == -1 || i == nx - 1)
                continue;
              p3 = origin+xa*i+ya*j;
              p4 = origin+xa*(i+1)+ya*j;
              p1 = p3+za*sheight[i*ny + j];
              p2 = p4+za*sheight[(i+1)*ny + j];
              u1 = double(i) / double(nx-1);
              u2 = double(i+1) / double(nx-1);
              v1 = 1.0;
              v2 = 1.0;
              square = 0;
           }
         else
           {  p5 = origin+xa*i+ya*j;
              p1 = p5+za*sheight[i*ny + j];
              p2 = p5+xa+za*sheight[(i+1)*ny + j];
              h1 = (int)sheight[i*ny + j];
              h2 = (int)sheight[(i+1)*ny + j];
              j1 = j + 1;
              if (h1 == sheight[i*ny + j1] && h2 == sheight[(i+1)*ny + j1])
                {  while (j1 < ny - 1 && h1 == sheight[i*ny + j1 + 1] && h2 == sheight[(i+1)*ny + j1 + 1])
                     j1++;
                }
              p5 = origin+xa*i+ya*j1;
              p3 = p5+za*sheight[i*ny + j1];
              p4 = p5+xa+za*sheight[(i+1)*ny + j1];
              u1 = double(i) / double(nx-1);
              u2 = double(i+1) / double(nx-1);
              v1 = double(j) / double(ny-1);
              v2 = double(j1) / double(ny-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }

         if (i == -1 || i == nx - 1 || j == -1 || j == ny - 1 || p1.z >= (origin+za*(nz-1)).z &&  p2.z >= (origin+za*(nz-1)).z && p3.z >= (origin+za*(nz-1)).z)
           deltacolour = 0;
         else
           deltacolour = 1;

         if (foptions.test(0))
           {  n = (p2 - p1).cross(p3 - p1).normalize();
              if ((triangle = new Triangle(0,p1,p2,p3,n,n,n,UV(u1,v1),UV(u2,v1),UV(u1,v2))) != 0)
                {  if (! triangle->getdefined())
                     delete triangle;
                  else
                    {  EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                       if (! cb(this,triangle,0,0,data)) 
                         {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                            return 0;
                         }
                       EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                    }
               }
             else
               return 0;

             n = (p3 - p4).cross(p2 - p4).normalize();
             if ((triangle = new Triangle(0,p4,p3,p2,n,n,n,UV(u2,v2),UV(u1,v2),UV(u2,v1))) != 0)
               {  if (! triangle->getdefined())
                    delete triangle;
                  else
                    {  EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                       if (! cb(this,triangle,0,0,data)) 
                         {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                            return 0;
                         }
                       EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                    }
               }
             else
               return 0;
           }
         else
           {  if (foptions.test(1))
                {Point p[4];
                   if (square)
                     {  p[0] = p1;  p[1] = p2;  p[2] = p4;  p[3] = p3;
                        EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                        if (! cb(this,0,4,p,data))
                          {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                             return 0;
                          } 
                        EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                     }
                   else
                     {  p[0] = p1;  p[1] = p2;  p[2] = p3;
                        EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                        if (! cb(this,0,3,p,data))
                          {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                             return 0;
                          } 
                        p[0] = p4;  p[1] = p3;  p[2] = p2;
                        if (! cb(this,0,3,p,data))
                          {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                             return 0;
                          } 
                        EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                     }
                }
              else
                {  if (square)
                     poly = new Polygon3d(p1,p2,p4,p3);
                   else
                     poly = new Polygon3d(p1,p2,p3);
                   if (poly == NULL) return 0;
                   if (! poly->getdefined())
                     delete poly;
                   else
                     {  EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                        if (! cb((SurfaceEntity *)this,poly,0,0,data))
                          {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                             return 0;
                          }
                        EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                     }
                   if (! square)
                     {  poly = new Polygon3d(p4,p3,p2);
                        if (poly == NULL) return 0;
                        if (! poly->getdefined())
                          delete poly;
                        else
                          {  EntityHeader::change(getcolour()+deltacolour,getlayer(),getstyle(),getweight());
                             if (! cb((SurfaceEntity *)this,poly,0,0,data))
                               {  EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                                  return 0;
                               }
                             EntityHeader::change(getcolour()-deltacolour,getlayer(),getstyle(),getweight());
                          }
                     }
                }
           }
      }


  if (theight == 0)
    return 0;

  for (i = -1 ; i < twidth ; i++)
    for (j = -1 ; j < twidth ; j++)
      {  if (i == -1)
           {  if (j == -1 || j == twidth - 1)
                continue;
              p1 = lasttoolposition+ya*j+za*theight[0];
              p2 = lasttoolposition+ya*j+za*theight[(i+1)*twidth + j];
              h2 = (int)theight[(i+1)*twidth + j];
              j1 = j + 1;
              if (h2 == theight[(i+1)*twidth + j1])
                {  while (j1 < twidth - 1 && h2 == theight[(i+1)*twidth + j1 + 1])
                     j1++;
                }
              p3 = lasttoolposition+ya*j1+za*theight[0];
              p4 = lasttoolposition+ya*j1+za*theight[(i+1)*twidth + j1];
              u1 = 0.0;
              u2 = 0.0;
              v1 = double(j) / double(twidth-1);
              v2 = double(j1) / double(twidth-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }
         else if (i == twidth - 1)
           {  if (j == -1 || j == twidth - 1)
                continue;
              p1 = lasttoolposition+xa*i+ya*j+za*theight[i*twidth + j];
              p2 = lasttoolposition+xa*i+ya*j+za*theight[0];
              h1 = (int)theight[i*twidth + j];
              j1 = j + 1;
              if (h1 == theight[i*twidth + j1])
                {  while (j1 < twidth - 1 && h1 == theight[i*twidth + j1 + 1])
                     j1++;
                }
              p3 = lasttoolposition+xa*i+ya*j1+za*theight[i*twidth + j1];
              p4 = lasttoolposition+xa*i+ya*j1+za*theight[0];
              u1 = 1.0;
              u2 = 1.0;
              v1 = double(j) / double(twidth-1);
              v2 = double(j1) / double(twidth-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }
         else if (j == -1)
           {  if (i == -1 || i == twidth - 1)
                continue;
              p1 = lasttoolposition+xa*i+za*theight[0];
              p2 = lasttoolposition+xa*(i+1)+za*theight[0];
              p3 = lasttoolposition+xa*i+za*theight[i*twidth + j + 1];
              p4 = lasttoolposition+xa*(i+1)+za*theight[(i+1)*twidth + j + 1];
              u1 = double(i) / double(twidth-1);
              u2 = double(i+1) / double(twidth-1);
              v1 = 0.0;
              v2 = 0.0;
              square = 0;
           }
         else if (j == twidth - 1)
           {  if (i == -1 || i == twidth - 1)
                continue;
              p1 = lasttoolposition+xa*i+ya*j+za*theight[i*twidth + j];
              p2 = lasttoolposition+xa*(i+1)+ya*j+za*theight[(i+1)*twidth + j];
              p3 = lasttoolposition+xa*i+ya*j+za*theight[0];
              p4 = lasttoolposition+xa*(i+1)+ya*j+za*theight[0];
              u1 = double(i) / double(twidth-1);
              u2 = double(i+1) / double(twidth-1);
              v1 = 1.0;
              v2 = 1.0;
              square = 0;
           }
         else
           {  p1 = lasttoolposition+xa*i+ya*j+za*theight[i*twidth + j];
              p2 = lasttoolposition+xa*(i+1)+ya*j+za*theight[(i+1)*twidth + j];
              h1 = (int)theight[i*twidth + j];
              h2 = (int)theight[(i+1)*twidth + j];
              j1 = j + 1;
              if (h1 == theight[i*twidth + j1] && h2 == theight[(i+1)*twidth + j1])
                {  while (j1 < twidth - 1 && h1 == theight[i*twidth + j1 + 1] && h2 == theight[(i+1)*twidth + j1 + 1])
                     j1++;
                }
              p3 = lasttoolposition+xa*i+ya*j1+za*theight[i*twidth + j1];
              p4 = lasttoolposition+xa*(i+1)+ya*j1+za*theight[(i+1)*twidth + j1];
              u1 = double(i) / double(twidth-1);
              u2 = double(i+1) / double(twidth-1);
              v1 = double(j) / double(twidth-1);
              v2 = double(j1) / double(twidth-1);
              square = j1 > j + 1;
              j = j1 - 1;
           }

         if (foptions.test(0))
           {  n = (p2 - p1).cross(p3 - p1).normalize();
              if ((triangle = new Triangle(0,p1,p2,p3,n,n,n,UV(u1,v1),UV(u2,v1),UV(u1,v2))) != 0)
                {  if (! triangle->getdefined())
                     delete triangle;
                  else
                    {  EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
                       if (! cb(this,triangle,0,0,data))
                         {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                            return 0;
                         }
                       EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                    }
               }
             else
               return 0;

             n = (p3 - p4).cross(p2 - p4).normalize();
             if ((triangle = new Triangle(0,p4,p3,p2,n,n,n,UV(u2,v2),UV(u1,v2),UV(u2,v1))) != 0)
               {  if (! triangle->getdefined())
                    delete triangle;
                  else
                    {  EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
                       if (! cb(this,triangle,0,0,data))
                         {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                            return 0;
                         }
                       EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                    }
               }
             else
               return 0;
           }
         else
           {  if (foptions.test(1))
                {Point p[4];
                   EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
                   if (square)
                     {  p[0] = p1;  p[1] = p2;  p[2] = p4;  p[3] = p3;
                        if (! cb(this,0,4,p,data))
                          {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                             return 0;
                          }
                     }
                   else
                     {  p[0] = p1;  p[1] = p2;  p[2] = p3;
                        if (! cb(this,0,3,p,data))
                          {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                             return 0;
                          }
                        p[0] = p4;  p[1] = p3;  p[2] = p2;
                        if (! cb(this,0,3,p,data))
                          {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                             return 0;
                          }
                     }
                   EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                }
              else
                {  if (square)
                     poly = new Polygon3d(p1,p2,p4,p3);
                   else
                     poly = new Polygon3d(p1,p2,p3);
                   if (poly == NULL) return 0;
                   if (! poly->getdefined())
                     delete poly;
                   else
                     {  EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
                        if (! cb(this,poly,0,0,data))
                          {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                             return 0;
                          }
                        EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                     }
                   if (! square)
                     {  poly = new Polygon3d(p4,p3,p2);
                        if (poly == NULL) return 0;
                        if (! poly->getdefined())
                          delete poly;
                        else
                          {  EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
                             if (! cb(this,poly,0,0,data))
                               {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                                  return 0;
                               }
                             EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                          }
                     }
                }
           }
      }

  if (foptions.test(1))
    {Point p[4];
       p[0] = lasttoolposition+za*theight[0];
       p[1] = lasttoolposition+xa*(twidth-1)+za*theight[0];
       p[2] = lasttoolposition+xa*(twidth-1)+ya*(twidth-1)+za*theight[0];
       p[3] = lasttoolposition+ya*(twidth-1)+za*theight[0];
       EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
       cb(this,0,4,p,data);
       EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
    }
  else
    {  poly = new Polygon3d(
           lasttoolposition+za*theight[0],
           lasttoolposition+xa*(twidth-1)+za*theight[0],
           lasttoolposition+xa*(twidth-1)+ya*(twidth-1)+za*theight[0],
           lasttoolposition+ya*(twidth-1)+za*theight[0]);
       if (poly == NULL) return 0;
       if (! poly->getdefined())
         delete poly;
       else
         {  EntityHeader::change(getcolour()+2,getlayer(),getstyle(),getweight());
            if (! cb(this,poly,0,0,data))
              {  EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
                 return 0;
              }
            EntityHeader::change(getcolour()-2,getlayer(),getstyle(),getweight());
         }
    }
  return 0;
}

void NCSolid::triangulate(double, Transform *)
{
}


void NCSolid::addtodisplaylist(View3dSurface *vs,int child)
{int i,j,x,y;
 Point pt1,pt2;
  if (! child)
    {  vs->displaylist.setsavemode();
       if (! vs->displaylist.start(this,17*(16 + (nx-1) * xmesh * 2 + (ny-1) * ymesh * 2))) return;
    }
  for (i = 0 ; i < 8 ; i++)
    {  if (i == 0)
         {  pt1 = vs->modeltoview(origin);
            pt2 = vs->modeltoview(origin+xaxis*(nx-1)*delta);
         }
       else if (i == 1)
         {  pt1 = vs->modeltoview(origin+xaxis*(nx-1)*delta);
            pt2 = vs->modeltoview(origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta);
         }
       else if (i == 2)
         {  pt1 = vs->modeltoview(origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta);
            pt2 = vs->modeltoview(origin+yaxis*(ny-1)*delta);
         }
       else if (i == 3)
         {  pt1 = vs->modeltoview(origin+yaxis*(ny-1)*delta);
            pt2 = vs->modeltoview(origin);
         }
       else if (i == 4)
         {  pt1 = vs->modeltoview(origin);
            pt2 = vs->modeltoview(origin+zaxis*(nz-1)*delta);
         }
       else if (i == 5)
         {  pt1 = vs->modeltoview(origin+yaxis*(ny-1)*delta);
            pt2 = vs->modeltoview(origin+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta);
         }
       else if (i == 6)
         {  pt1 = vs->modeltoview(origin+xaxis*(nx-1)*delta);
            pt2 = vs->modeltoview(origin+xaxis*(nx-1)*delta+zaxis*(nz-1)*delta);
         }
       else if (i == 7)
         {  pt1 = vs->modeltoview(origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta);
            pt2 = vs->modeltoview(origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta);
         }
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
            if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
         }
    }

  for (x = 0 ; x < xmesh ; x++)
    for (j = 0 ; j < ny-1 ; j++)
      {  i = int((nx - 1) * x / double(xmesh-1));
         pt1 = vs->modeltoview(origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta);
         pt2 = vs->modeltoview(origin+xaxis*i*delta+yaxis*(j+1)*delta+zaxis*sheight[i*ny + j+1]*delta);
         if (vs->clip3dfb(&pt1,&pt2))
           {  pt1 = vs->viewtoscreen(pt1);
              pt2 = vs->viewtoscreen(pt2);
              if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
              if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
           }
      }

  for (y = 0 ; y < ymesh ; y++)
    for (i = 0 ; i < nx-1 ; i++)
      {  j = int((ny - 1) * y / double(ymesh-1));
         pt1 = vs->modeltoview(origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta);
         pt2 = vs->modeltoview(origin+xaxis*(i+1)*delta+yaxis*j*delta+zaxis*sheight[(i+1)*ny + j]*delta);
         if (vs->clip3dfb(&pt1,&pt2))
           {  pt1 = vs->viewtoscreen(pt1);
              pt2 = vs->viewtoscreen(pt2);
              if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
              if (! vs->displaylist.lineto(pt2.x,pt2.y)) return;
           }
      }
  if (! child && ! vs->displaylist.end()) return;
}

void NCSolid::draw(int drawmode,Transform *t,View3dSurface *vs)
{int i,j,x,y;
 Point pt1,pt2;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);
  if (t == NULL)
    {  if (vs->displaylist.draw(this,drawmode)) return;
       if (! vs->displaylist.geterrorstatus())
         addtodisplaylist(vs,0);
       if (vs->displaylist.draw(this,drawmode)) return;
    }

  for (i = 0 ; i < 8 ; i++)
    {  if (i == 0)
         {  pt1 = origin;
            pt2 = origin+xaxis*(nx-1)*delta;
         }
       else if (i == 1)
         {  pt1 = origin+xaxis*(nx-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
         }
       else if (i == 2)
         {  pt1 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
            pt2 = origin+yaxis*(ny-1)*delta;
         }
       else if (i == 3)
         {  pt1 = origin+yaxis*(ny-1)*delta;
            pt2 = origin;
         }
       else if (i == 4)
         {  pt1 = origin;
            pt2 = origin+zaxis*(nz-1)*delta;
         }
       else if (i == 5)
         {  pt1 = origin+yaxis*(ny-1)*delta;
            pt2 = origin+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta;
         }
       else if (i == 6)
         {  pt1 = origin+xaxis*(nx-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+zaxis*(nz-1)*delta;
         }
       else if (i == 7)
         {  pt1 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta;
         }

       if (t == NULL)
         {  pt1 = vs->modeltoview(pt1);
            pt2 = vs->modeltoview(pt2);
         }
       else
         {  pt1 = vs->modeltoview(t->transform(pt1));
            pt2 = vs->modeltoview(t->transform(pt2));
         }
       if (vs->clip3dfb(&pt1,&pt2))
         {  pt1 = vs->viewtoscreen(pt1);
            pt2 = vs->viewtoscreen(pt2);
            vs->line(int(pt1.x),int(pt1.y),int(pt2.x),int(pt2.y));
         }
    }

  for (x = 0 ; x < xmesh ; x++)
    for (j = 0 ; j < ny-1 ; j++)
      {  i = int((nx - 1) * x / double(xmesh-1));
         pt1 = origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta;
         pt2 = origin+xaxis*i*delta+yaxis*(j+1)*delta+zaxis*sheight[i*ny + j+1]*delta;
         if (t == NULL)
           {  pt1 = vs->modeltoview(pt1);
              pt2 = vs->modeltoview(pt2);
           }
         else
           {  pt1 = vs->modeltoview(t->transform(pt1));
              pt2 = vs->modeltoview(t->transform(pt2));
           }
         if (vs->clip3dfb(&pt1,&pt2))
           {  pt1 = vs->viewtoscreen(pt1);
              pt2 = vs->viewtoscreen(pt2);
              vs->line(int(pt1.x),int(pt1.y),int(pt2.x),int(pt2.y));
           }
      }

  for (y = 0 ; y < ymesh ; y++)
    for (i = 0 ; i < nx-1 ; i++)
      {  j = int((ny - 1) * y / double(ymesh-1));
         pt1 = origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta;
         pt2 = origin+xaxis*(i+1)*delta+yaxis*j*delta+zaxis*sheight[(i+1)*ny + j]*delta;
         if (t == NULL)
           {  pt1 = vs->modeltoview(pt1);
              pt2 = vs->modeltoview(pt2);
           }
         else
           {  pt1 = vs->modeltoview(t->transform(pt1));
              pt2 = vs->modeltoview(t->transform(pt2));
           }
         if (vs->clip3dfb(&pt1,&pt2))
           {  pt1 = vs->viewtoscreen(pt1);
              pt2 = vs->viewtoscreen(pt2);
              vs->line(int(pt1.x),int(pt1.y),int(pt2.x),int(pt2.y));
          }
      }
}

int NCSolid::hide(HideImage *image)
{int i,j,x,y;
 Point pt1,pt2;
  image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
  for (i = 0 ; i < 8 ; i++)
    {  if (i == 0)
         {  pt1 = origin;
            pt2 = origin+xaxis*(nx-1)*delta;
         }
       else if (i == 1)
         {  pt1 = origin+xaxis*(nx-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
         }
       else if (i == 2)
         {  pt1 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
            pt2 = origin+yaxis*(ny-1)*delta;
         }
       else if (i == 3)
         {  pt1 = origin+yaxis*(ny-1)*delta;
            pt2 = origin;
         }
       else if (i == 4)
         {  pt1 = origin;
            pt2 = origin+zaxis*(nz-1)*delta;
         }
       else if (i == 5)
         {  pt1 = origin+yaxis*(ny-1)*delta;
            pt2 = origin+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta;
         }
       else if (i == 6)
         {  pt1 = origin+xaxis*(nx-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+zaxis*(nz-1)*delta;
         }
       else if (i == 7)
         {  pt1 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
            pt2 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta+zaxis*(nz-1)*delta;
         }
       image->hide(this,pt1,pt2);
    }

  for (x = 0 ; x < xmesh ; x++)
    for (j = 0 ; j < ny-1 ; j++)
      {  i = int((nx - 1) * x / double(xmesh));
         pt1 = origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta;
         pt2 = origin+xaxis*i*delta+yaxis*(j+1)*delta+zaxis*sheight[i*ny + j+1]*delta;
         image->hide(this,pt1,pt2);
      }

  for (y = 0 ; y < ymesh ; y++)
    for (i = 0 ; i < nx-1 ; i++)
      {  j = int((ny - 1) * y / double(ymesh));
         pt1 = origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta;
         pt2 = origin+xaxis*(i+1)*delta+yaxis*j*delta+zaxis*sheight[(i+1)*ny + j]*delta;
         image->hide(this,pt1,pt2);
      }
  return 1;
}

int NCSolid::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int NCSolid::npoints(void)
{  return 0;
}

Point NCSolid::point(int)
{Point tmp(0.0,0.0,0.0);  return tmp;
}

Point NCSolid::nearp(Point)
{Point tmp(0.0,0.0,0.0);
  return tmp;
}

int NCSolid::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
{ if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  return 1;
}

int NCSolid::move(Database *db,Transform &t,int erase)
{Point o,xa,ya,za;
 Entity *e;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
  if (erase) draw(DM_ERASE);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  status |= 1;
  o = t.transform(origin);
  xa = t.transform(xaxis+origin) - o;  xa.normalize();
  ya = t.transform(yaxis+origin) - o;  ya.normalize();
  za = xa.cross(ya);
  ya = za.cross(xa);
  if (xa.length() > ::db.getptoler() && ya.length() > ::db.getptoler())
    {  xaxis = xa;  yaxis = ya;
    }
  origin = o;
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *NCSolid::clone(Transform &t)
{Point o,xa,ya,za;
  if (copye != NULL) return copye;
  o = t.transform(origin);
  xa = t.transform(xaxis+origin) - o;
  ya = t.transform(yaxis+origin) - o;
  za = xa.cross(ya);
  ya = za.cross(xa);
  if (xa.length() < db.getptoler() || ya.length() < db.getptoler())
    return 0;
  xa.normalize();  ya.normalize();
  copye = new NCSolid(*this,o,xa,ya,delta,nx,ny,nz,xmesh,ymesh,sheight);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int NCSolid::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

int NCSolid::save(CadFile *outfile)
{short type;
 int i,j;
  type = ncsolid_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&nx)) return 0;
  if (! outfile->write(&ny)) return 0;
  if (! outfile->write(&nz)) return 0;
  if (! outfile->write(&xmesh)) return 0;
  if (! outfile->write(&ymesh)) return 0;
  if (! outfile->write(&delta)) return 0;
  for (i = 0 ; i < nx ; i++)
    for (j = 0 ; j < ny ; j++)
      if (! outfile->write(&sheight[i*ny+j])) return 0;
  return 1;
}

Entity *NCSolid::load(int,CadFile *infile)
{NCSolid *ncsolid;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 short nx,ny,nz,xmesh,ymesh;
 double delta;
 double *sheight;
 int i,j;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&nx)) return 0;
  if (! infile->read(&ny)) return 0;
  if (! infile->read(&nz)) return 0;
  if (! infile->read(&xmesh)) return 0;
  if (! infile->read(&ymesh)) return 0;
  if (! infile->read(&delta)) return 0;
  sheight = new double [nx * ny];
  for (i = 0 ; i < nx ; i++)
    for (j = 0 ; j < ny ; j++)
      if (! infile->read(&sheight[i*ny+j])) return 0;
  ncsolid = new NCSolid(*header,origin,xaxis,yaxis,delta,nx,ny,nz,xmesh,ymesh,sheight);
  ncsolid->atlist = header->getatlist();
  delete header;
  delete [] sheight;
  return ncsolid;
}

int NCSolid::save(UndoFile *)
{ return 1;
}

Entity *NCSolid::load(UndoFile *)
{ return 0;
}

int NCSolid::saveattributes(CadFile *)
{ return 1;
}

int NCSolid::loadattributes(CadFile *)
{ return 1;
}

int NCSolid::savegeometry(CadFile *)
{ return 1;
}

int NCSolid::loadgeometry(CadFile *)
{ return 1;
}

void NCSolid::verify(void)
{
}

int NCSolid::explode(void)
{ return 1;
}

int NCSolid::issame(int,const Entity &) const
{ return 0;
}

void NCSolid::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{Point p3;
 int i,j;
  if (vs == NULL)
    *pmin = *pmax = origin;
  else
    *pmin = *pmax = vs->modeltouv(origin);

  for (i = 0 ; i < 3 ; i++)
    {  if (i == 0)
         p3 = origin+xaxis*(nx-1)*delta;
       else if (i == 1)
         p3 = origin+xaxis*(nx-1)*delta+yaxis*(ny-1)*delta;
       else if (i == 2)
         p3 =origin+yaxis*(ny-1)*delta;

        if (vs != NULL)
          p3 = vs->modeltouv(p3);

        if (p3.x < pmin->x) pmin->x = p3.x;
        if (p3.y < pmin->y) pmin->y = p3.y;
        if (p3.z < pmin->z) pmin->z = p3.z;
        if (p3.x > pmax->x) pmax->x = p3.x;
        if (p3.y > pmax->y) pmax->y = p3.y;
        if (p3.z > pmax->z) pmax->z = p3.z;
    }

  for (i = 0 ; i < nx ; i++)
    for (j = 0 ; j < ny ; j++)
      {  p3 = origin+xaxis*i*delta+yaxis*j*delta+zaxis*sheight[i*ny + j]*delta;
         if (vs != NULL)
           p3 = vs->modeltouv(p3);
         if (p3.x < pmin->x) pmin->x = p3.x;
         if (p3.y < pmin->y) pmin->y = p3.y;
         if (p3.z < pmin->z) pmin->z = p3.z;
         if (p3.x > pmax->x) pmax->x = p3.x;
         if (p3.y > pmax->y) pmax->y = p3.y;
         if (p3.z > pmax->z) pmax->z = p3.z;
      }
}

void NCSolid::definetool(int shape,double diameter,double angle,double length)
{int i,j;

  delete theight;
  theight = 0;

  if (shape == 1 || shape == 4)
    {  twidth = short(diameter / delta);
       twidth = short(short(twidth / 2) * 2 + 1);
       theight = new double [twidth * twidth];
       for (i = 0 ; i < twidth ; i++)
         for (j = 0 ; j < twidth ; j++)
           if (i * i - i * (twidth-1) + j * j - j * (twidth-1) + (twidth-1) * (twidth-1) / 4 < 0)
             theight[i * twidth + j] = 0;
           else
             theight[i * twidth + j] = length / delta;
    }
  else if (shape == 2)
    {  twidth = short(diameter / delta);
       twidth = short(short(twidth / 2) * 2 + 1);
       theight = new double [twidth * twidth];
       for (i = 0 ; i < twidth ; i++)
         for (j = 0 ; j < twidth ; j++)
           if (i * i - i * (twidth-1) + j * j - j * (twidth-1) + (twidth-1) * (twidth-1) / 4 < 0)
             theight[i * twidth + j] = twidth/2 - sqrt(double((twidth-1)*(twidth-1)/4 - (i*i-i*(twidth-1)+j*j-j*(twidth-1)+(twidth-1)*(twidth-1)/2)));
           else
             theight[i * twidth + j] = length / delta;
    }
  else if (shape == 3)
    {  twidth = short(diameter / delta);
       twidth = short(short(twidth / 2) * 2 + 1);
       theight = new double [twidth * twidth];
       for (i = 0 ; i < twidth ; i++)
         for (j = 0 ; j < twidth ; j++)
           if (i * i - i * (twidth-1) + j * j - j * (twidth-1) + (twidth-1) * (twidth-1) / 4 < 0)
             theight[i * twidth + j] = sqrt((double)(i*i-i*(twidth-1)+j*j-j*(twidth-1)+(twidth-1)*(twidth-1)/2)) / tan(angle / 2.0 * M_PI / 180.0);
           else
             theight[i * twidth + j] = length / delta;
    }

}


void NCSolid::removetool(Point p)
{int i,j,x,y,z,istart,iend,jstart,jend;
 Point p1,p2;
  if (theight == 0)
    return;
  lasttoolposition = p - Point(twidth / 2 * delta,twidth / 2 * delta,0.0);
  p -= origin;
  x = istart = int(p.x / delta - twidth / 2);  iend = istart + twidth - 1;
  y = jstart = int(p.y / delta - twidth / 2);  jend = jstart + twidth - 1;
  z = int(p.z / delta);

  if (dirty)
    {  p1 = lasttoolposition;
       p2 = p1 + Point(twidth * delta,twidth * delta,delta*theight[0]);
       if (p1.x < dmin.x) dmin.x = p1.x;
       if (p1.y < dmin.y) dmin.y = p1.y;
       if (p1.z < dmin.z) dmin.z = p1.z;
       if (p2.x > dmax.x) dmax.x = p2.x;
       if (p2.y > dmax.y) dmax.y = p2.y;
       if (p2.z > dmax.z) dmax.z = p2.z;
    }
  else
    {  dmin = lasttoolposition;
       dmax = dmin + Point(twidth * delta,twidth * delta,delta*theight[0]);
       dirty = 1;
    }

  if (istart < 0) istart = 0;
  if (jstart < 0) jstart = 0;
  if (iend > nx-1) iend = nx - 1;
  if (jend > ny-1) jend = ny - 1;

  for (i = istart ; i <= iend ; i++)
    for (j = jstart ; j <= jend ; j++)
      if (sheight[i*ny + j] > theight[(i-x)*twidth+j-y]+z)
        sheight[i*ny + j] = theight[(i-x)*twidth+j-y]+z;

}

