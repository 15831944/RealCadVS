
#include "ncwin.h"
#include <time.h>

#if MENUS == FULLMENUS

void LoadLayerNames(void)
{
  //  Add the named layers
  db.layernames.setlayername(EAVE_LAYER, (char*)"Eave");
  db.layernames.setlayername(PITCHING_LAYER, (char*)"Pitching");
  db.layernames.setlayername(RIDGE_LAYER, (char*)"Ridge");
  db.layernames.setlayername(HIP_LAYER, (char*)"Hip");
  db.layernames.setlayername(VALLEY_LAYER, (char*)"Valley");
  db.layernames.setlayername(DUTCHGABLE_LAYER, (char*)"Dutch Gable");
  db.layernames.setlayername(DUTCHGABLEOVER_LAYER, (char*)"Dutch Gable Over Roof");
  db.layernames.setlayername(DUTCHGABLEOVERHIP_LAYER, (char*)"Dutch Gable Over Roof Hip");
  db.layernames.setlayername(DUTCHGABLEBASE_LAYER, (char*)"Dutch Gable Base");
  db.layernames.setlayername(ROOFPLAN_LAYER, (char*)"Roof Plan");
  db.layernames.setlayername(FACIA_LAYER, (char*)"Facia");
  db.layernames.setlayername(TILES_LAYER, (char*)"Tiles");
  db.layernames.setlayername(ROOF_LAYER, (char*)"Roof");
  db.layernames.setlayername(ROOFDIMENSIONS_LAYER, (char*)"Roof Dimensions");

  db.layernames.setlayername(FULLTRUSS_LAYER, (char*)"Full Truss");
  db.layernames.setlayername(DUTCHGABLEFULLTRUSS_LAYER, (char*)"Dutch Gable Full Truss");
  db.layernames.setlayername(HIPFULLTRUSS_LAYER, (char*)"Hip Full Truss");
  db.layernames.setlayername(HALFTRUSS_LAYER, (char*)"Half Truss");
  db.layernames.setlayername(HIPHALFTRUSS_LAYER, (char*)"Hip Half Truss");
  db.layernames.setlayername(DUTCHGABLEHALFTRUSS_LAYER, (char*)"Dutch Gable Half Truss");
  db.layernames.setlayername(HIPTRUSS_LAYER, (char*)"Hip Truss");
  db.layernames.setlayername(VERTICALWEBTRUNCATEDTRUSS_LAYER, (char*)"Vertical Web Truncated Truss");
  db.layernames.setlayername(TRUNCATEDTRUSS_LAYER, (char*)"Truncated Truss");
  db.layernames.setlayername(BEAM_LAYER, (char*)"Beam");
  db.layernames.setlayername(TRUSSPLAN_LAYER, (char*)"Truss Plan");

  db.layernames.setlayername(WALLFRAMING_LAYER, (char*)"Wall Frames");
  db.layernames.setlayername(WALLFRAMEBRACING_LAYER, (char*)"Wall Frame Bracing");

  db.layernames.setlayername(TOPCHORD1_LAYER, (char*)"Top Chord 1");
  db.layernames.setlayername(TOPCHORD2_LAYER, (char*)"Top Chord 2");
  db.layernames.setlayername(TOPCHORD3_LAYER, (char*)"Top Chord 3");
  db.layernames.setlayername(BOTTOMCHORD_LAYER, (char*)"Bottom Chord");
  db.layernames.setlayername(WEB_LAYER, (char*)"Web Member");
  db.layernames.setlayername(WEB1_LAYER, (char*)"King Web Member");
  db.layernames.setlayername(WEB2_LAYER, (char*)"Half Truss King Web Member");
  db.layernames.setlayername(WEB3_LAYER, (char*)"Hip Truss Web Member");
  db.layernames.setlayername(WEB4_LAYER, (char*)"Hip Truss King Web Member");
  db.layernames.setlayername(CLIP_LAYER, (char*)"Clip Layer");
  db.layernames.setlayername(NOTCH_LAYER, (char*)"Notch Layer");
  db.layernames.setlayername(HIPFULLNOTCH_LAYER, (char*)"Hip Full Notch Layer");
  db.layernames.setlayername(HALFHIPNOTCH_LAYER, (char*)"Hip Half Notch Layer");
  db.layernames.setlayername(TOPPLATE_LAYER, (char*)"Top Plate Layer");
  db.layernames.setlayername(BOTTOMPLATE_LAYER, (char*)"Bottom Plate Layer");
  db.layernames.setlayername(NOGGING_LAYER, (char*)"Nogging member");
  db.layernames.setlayername(SILL_LAYER, (char*)"Sill Member");
  db.layernames.setlayername(LINTEL_LAYER, (char*)"Lintel Member");
  db.layernames.setlayername(STUD_LAYER, (char*)"Stud Member");


}
void ArchitectActive::setactive(int a)
{  active = a;  v.setinteger("ar::active",a);
}

ArchitectMenuWindow::ArchitectMenuWindow(Window *win,int x1,int y1,int w,int h,ArchitectActive *aa) :
  MenuWindow(win,x1,y1,w,h)
{int x2,y2,xm,ym,wm,hm,mf;
 RCCHAR text[300];

  architectactive = aa;
  buttonmenu = architectactive->getbuttonmenu();
  architectactive->setactive(1);

  //  Try to dock Arch Menu

  x2 = x1 >= 0 ? x1 : int(cadwindow->getsubwindow()->getwidth() + x1);
  y2 = y1 >= 0 ? y1 : int(cadwindow->getsubwindow()->getheight() + y1);

  if (buttonmenus.match("ApplicationMenu::Architect") == 0)
    {  buttonmenus.add("ApplicationMenu::Architect",buttonmenu);

       GetPrivateProfileString(_RCT("Layout"),_RCT("ApplicationMenu::Architect"),_RCT(""),text,300,home.getinifilename());

       if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5)
         {  if (mf == 1)
              {  addcontrol(buttonmenu);
                 buttonmenu->move(this,0,0,2);
              }
            else
              {   cadwindow->delfloatingmenuwindow(this);
                  ShowWindow(hWnd,SW_HIDE);
                  buttonmenu->move(cadwindow,xm,ym,2);
                  cadwindow->sizeevent(-1,-1);
              } 
         } 
       else 
         {  addcontrol(buttonmenu);
            buttonmenu->move(this,0,0,2);
         }
    }


}

ArchitectMenuWindow::~ArchitectMenuWindow()
{ architectactive->setactive(0);
}

ArchitectActive architectactive;

extern void architect_command1(int *cstate,HighLevelEvent *event,void **);
extern void architect_command2(int *cstate,HighLevelEvent *event,void **);
extern void architect_command3(int *cstate,HighLevelEvent *event,void **);
extern void architect_command4(int *cstate,HighLevelEvent *event,void **);
extern void architect_command5(int *cstate,HighLevelEvent *event,void **);
extern void architect_command6(int *cstate,HighLevelEvent *event,void **);
extern void architect_command7(int *cstate,HighLevelEvent *event,void **);
extern void architect_command8(int *cstate,HighLevelEvent *event,void **);
extern void architect_command9(int *cstate,HighLevelEvent *event,void **);
extern void architect_command10(int *cstate,HighLevelEvent *event,void **);
extern void architect_command11(int *cstate,HighLevelEvent *event,void **);
extern void architect_command12(int *cstate,HighLevelEvent *event,void **);
extern void architect_command13(int *cstate,HighLevelEvent *event,void **);
extern void architect_command14(int *cstate,HighLevelEvent *event,void **);
extern void architect_command15(int *cstate,HighLevelEvent *event,void **);
extern void architect_command16(int *cstate,HighLevelEvent *event,void **);
extern void architect_command17(int *cstate,HighLevelEvent *event,void **);
extern void architect_command18(int *cstate,HighLevelEvent *event,void **);
extern void architect_command19(int *cstate,HighLevelEvent *event,void **);
extern void architect_command20(int *cstate,HighLevelEvent *event,void **);
extern void architect_command21(int *cstate,HighLevelEvent *event,void **);
extern void architect_command22(int *cstate,HighLevelEvent *event,void **);
extern void architect_command23(int *cstate,HighLevelEvent *event,void **);
extern void architect_command24(int *cstate,HighLevelEvent *event,void **);
extern void architect_command25(int *cstate,HighLevelEvent *event,void **);
extern void architect_command26(int *cstate,HighLevelEvent *event,void **);
extern void architect_command27(int *cstate,HighLevelEvent *event,void **);
extern void architect_command28(int *cstate,HighLevelEvent *event,void **);
extern void architect_command29(int *cstate,HighLevelEvent *event,void **);

extern int check_license(RCCHAR *,RCCHAR *,RCCHAR *,int application,int showmessages);

void ArchitectDimensionVisibilityCallback(int viewno,BitMask *visibility)
{View *view1,*view2;

  if ((view1 = db.views3d.match(viewno)) != 0)
    {  if (_stricmp(view1->getname(),"Top") == 0)
         {  view2 = db.views3d.match("Ground");
            visibility->set(view2->getviewno(),1); 
         }
       else if (_stricmp(view1->getname(),"Ground") == 0)
         {  view2 = db.views3d.match("Top");
            visibility->set(view2->getviewno(),1); 
         }
    }

}


void architect_command0(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;

  dimensionvisibilitycallback = ArchitectDimensionVisibilityCallback;


  check_license(_RCT("Architect Application"),_RCT("RealCADKey3"),_RCT("RealCADKey4"),1,1);
  if (architectactive.getactive()) return;
  if (architectactive.getbuttonmenu() == NULL)
    {ButtonMenu *menu = new ButtonMenu(0,0,42,42,15);
       if (menu != NULL)
         {  commands.add(1,21001,0,architect_command1, (char*)"Insert wall");
            commands.add(1,21002,0,architect_command2, (char*)"Wall options");
            commands.add(1,21022,0,architect_command22, (char*)"Insert column");
            commands.add(1,21023,0,architect_command23, (char*)"Insert column grid");
            commands.add(1,21024,0,architect_command24, (char*)"Column defaults");
            commands.add(1,21003,0,architect_command3, (char*)"Trim wall");
            commands.add(1,21004,0,architect_command4, (char*)"Join wall");
            commands.add(1,21017,0,architect_command17, (char*)"Trim walls to a corner");
            commands.add(1,21005,0,architect_command5, (char*)"Insert door");
            commands.add(1,21006,0,architect_command6, (char*)"Insert window");
            commands.add(1,21007,0,architect_command7, (char*)"Insert/Modify roof");
            commands.add(1,21018,0,architect_command18, (char*)"Insert floor");
            commands.add(1,21019,0,architect_command19, (char*)"Insert ceiling");
            commands.add(1,21008,0,architect_command8, (char*)"Insert stairs");
            commands.add(1,21009,0,architect_command9, (char*)"New floor");
            commands.add(1,21010,0,architect_command10, (char*)"New section");
            commands.add(1,21011,0,architect_command11, (char*)"New elevation");
            commands.add(1,21012,0,architect_command12, (char*)"New detail");
            commands.add(1,21013,0,architect_command13, (char*)"Edit Notation");
            commands.add(1,21014,0,architect_command14, (char*)"Architectural Symbols");
            commands.add(1,21015,0,architect_command15, (char*)"Architectural Grid");
            commands.add(1,21016,0,architect_command16, (char*)"Architectural perspective");
            commands.add(1,21020,0,architect_command20, (char*)"Define cost categories");
            commands.add(1,21021,0,architect_command21, (char*)"Bill of materials");
            commands.add(1,21025,0,architect_command25, (char*)"Change architectural layer and attributes");
            commands.add(1,21026,0,architect_command26, (char*)"Number doors and windows");
            commands.add(1,21027,0,architect_command27, (char*)"Insert door and window table");
            commands.add(1,21028,0,architect_command28, (char*)"Notation options");
            commands.add(1,21029,0,architect_command29, (char*)"Architectural dimension");
            menu->add(new Button(-1,-1, (char*)"ArWall",21001));
            menu->add(new Button(-1,-1, (char*)"ArWallDefaults",21002));
            menu->add(new Button(-1,-1, (char*)"ArColumn",21022));
            menu->add(new Button(-1,-1, (char*)"ArColumnGrid",21023));
            menu->add(new Button(-1,-1, (char*)"ArColumnDefaults",21024));
            menu->add(new Button(-1,-1, (char*)"ArTrim",21003));
            menu->add(new Button(-1,-1, (char*)"ArTrimCorner",21017));
            menu->add(new Button(-1,-1, (char*)"ArTrimIntersection",21037));
            menu->add(new Button(-1,-1, (char*)"ArJoin",21004));
            menu->add(new Button(-1,-1, (char*)"ArDoor",21005));
            menu->add(new Button(-1,-1, (char*)"ArWindow",21006));
            menu->add(new Button(-1,-1, (char*)"ArRoof",21007));
            menu->add(new Button(-1,-1, (char*)"ArInsertFloor",21018));
            menu->add(new Button(-1,-1, (char*)"ArInsertCeiling",21019));
            menu->add(new Button(-1,-1, (char*)"ArStairs",21008));
            menu->add(new Button(-1,-1, (char*)"ArFloor",21009));
            menu->add(new Button(-1,-1, (char*)"ArSection",21010));
            menu->add(new Button(-1,-1, (char*)"ArElevation",21011));
            menu->add(new Button(-1,-1, (char*)"ArDetail",21012));
            menu->add(new Button(-1,-1, (char*)"ArNotation",21013));
            menu->add(new Button(-1,-1, (char*)"ArPerspective",21016));
            menu->add(new Button(-1,-1, (char*)"ArSymbols",21014));
            menu->add(new Button(-1,-1, (char*)"ArGrid",21015));
            menu->add(new Button(-1,-1, (char*)"ArCost",21020));
            menu->add(new Button(-1,-1, (char*)"ArBOM",21021));
            menu->add(new Button(-1,-1, (char*)"ArLayer",21025));
            menu->add(new Button(-1,-1, (char*)"ArNumber",21026));
            menu->add(new Button(-1,-1, (char*)"ArTable",21027));
            menu->add(new Button(-1,-1, (char*)"ArNotationOptions",21028));
            menu->add(new Button(-1,-1, (char*)"ArDimension",21029));
            architectactive.setbuttonmenu(menu);
         }
    }
  if (architectactive.getbuttonmenu() == NULL)
    { RCMessageBox(cadwindow->gethwnd(),TEXT("Failed to create the architect button menu"),TEXT("Architect"),MB_ICONSTOP);
       return;
    }
  new ArchitectMenuWindow(cadwindow->getsubwindow(),
                          int(cadwindow->getsubwindow()->getwidth())-(24*2+GetSystemMetrics(SM_CXDLGFRAME)*3),0,
                          24*2,25*20,&architectactive);

  LoadLayerNames();

  *cstate = ExitState;
}

extern "C" void WINAPI showarchitectmenu(void);

void WINAPI showarchitectmenu(void)
{int state;
  state = InitialState;
  architect_command0(&state,0,0);
}

struct RBArchitect1_data
  {Point   pfirst1,pfirst2,plast,p1,xaxis,yaxis,zaxis,pside;
   double offset,angle1,angle4,width,height,length,snapangle,e,p,pd,ph,rhs,lhs,rha,lha,attribute[ArMaxWallAttributes],distance;
   short nattributes;
   int mc,rc,fixlength,sidelocated;
   BitMask options;
   int nwalls,shiftstate,gridstate,positionlocation;
   RCCHAR name[300];
   RBArchitect1_data() : options(32) {}
   Entity *wall;
  };

void RBArchitect1(int dm,void *data,Point *p2,View3dSurface *)
{RBArchitect1_data *rbdata = (RBArchitect1_data *) data;
 double angle2,angle3,cosa,l,angle;
 Point p1,p3,p4,p5,p6,xa,ya,normal;


  if (rbdata->nwalls > 0 && rbdata->sidelocated == 0)
    {  if ((rbdata->p1 - rbdata->plast).cross(*p2 - rbdata->plast).z * rbdata->offset > 0.0)
          rbdata->offset = -rbdata->offset;
    }

  angle3 = 0.0;
  p1 = rbdata->p1;
  if (rbdata->wall != 0)
    {  p1 = rbdata->wall->nearp(*p2);
       p1 += (*p2 - p1).normalize() * rbdata->offset;
    }
  if (rbdata->nwalls > 1 && (*p2 - rbdata->pfirst1).length() < rbdata->width * 2.0)
    {  if (dm == RB_ERASE && ! rbdata->shiftstate ||
           dm != RB_ERASE && ! cadwindow->getshiftstate())
         {  rbdata->shiftstate = 0;
            p5 = rbdata->pfirst1;
            p6.x = rbdata->xaxis.dot(p1 - rbdata->pfirst1);
            p6.y = rbdata->yaxis.dot(p1 - rbdata->pfirst1);
            if ((l = sqrt(p6.x*p6.x + p6.y*p6.y)) < db.getptoler()) return;
            p6.x /= l;  p6.y /= l;
            if (p6.x > 1.0) p6.x = 1.0;  else if (p6.x < -1.0) p6.x = -1.0;
            angle = acos(p6.x);
            if (p6.y < 0.0) angle = 2.0 * M_PI - angle;
            if (fabs(rbdata->snapangle) > 1.0E-10)
              angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
            p6 = p5 + rbdata->xaxis * (l * cos(angle)) +
                      rbdata->yaxis * (l * sin(angle));
            Line line1(rbdata->plast,p1);
            Line line2(p6,rbdata->pfirst1);
            Intersect i(&line1,&line2);
            if (i.nintersections() != 0)
              p1 = ((PointE *)i.intersection(0))->getp();
         }
       cosa = (rbdata->pfirst2 - rbdata->pfirst1).normalize().dot((p1 - rbdata->pfirst1).normalize());
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       angle3 = acos(cosa);
       if (rbdata->zaxis.dot((rbdata->pfirst2 - rbdata->pfirst1).cross(p1 - rbdata->pfirst1)) > 0.0)
         angle3 = 2.0 * M_PI - angle3;
       angle3 = M_PI / 2.0 - angle3 / 2.0;
       p4 = rbdata->pfirst1;

    }
  else if (dm == RB_ERASE && ! rbdata->shiftstate ||
           dm != RB_ERASE && ! cadwindow->getshiftstate())
    {  rbdata->shiftstate = 0;
       p3 = p1;
       p4.x = rbdata->xaxis.dot(*p2 - p3);
       p4.y = rbdata->yaxis.dot(*p2 - p3);
       if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
       p4.x /= l;  p4.y /= l;
       if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
       angle = acos(p4.x);
       if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
       if (fabs(rbdata->snapangle) > 1.0E-10)
         angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
       p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                 rbdata->yaxis * (l * sin(angle));
       Line line1(p1,p4);
       p4 = line1.nearp(*p2);
    }
  else
    {  rbdata->shiftstate = 1;
       p4 = *p2;
    }
  if (rbdata->nwalls == 0)
    angle2 = 0.0;
  else
    {  cosa = (p4 - p1).normalize().dot((rbdata->plast - p1).normalize());
       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
       angle2 = acos(cosa);
       if (rbdata->zaxis.dot((p4 - p1).cross(rbdata->plast - p1)) > 0.0)
         angle2 = 2.0 * M_PI - angle2;
       angle2 = M_PI / 2.0 - angle2 / 2.0;
    }

  if (rbdata->nwalls > 0)
    {  if (rbdata->sidelocated == 0)
         angle2 = 0.0; 
       xa = (p1 - rbdata->plast).normalize() * rbdata->offset;
       ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
       p5 = rbdata->plast - ya - xa*tan(rbdata->angle1);
       p6 = p1 - ya - xa*tan(-angle2);
       Wall wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,rbdata->angle1,angle2,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
       wall.draw(DM_INVERT);
    }
  if (rbdata->nwalls > 1)
    {  xa = (rbdata->pfirst2 - rbdata->pfirst1).normalize() * rbdata->offset;
       ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
       p5 = rbdata->pfirst1 - ya - xa*tan(angle3);
       p6 = rbdata->pfirst2 - ya - xa*tan(-rbdata->angle4);
       Wall wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,angle3,rbdata->angle4,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
       wall.draw(DM_INVERT);
    }
  
  if (rbdata->nwalls > 0 && rbdata->sidelocated || rbdata->nwalls == 0)
    {  xa = (p4 - p1).normalize() * rbdata->offset;
       ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
       p5 = p1 - ya - xa*tan(angle2);
       p6 = p4 - ya - xa*tan(-angle3);
       if (rbdata->fixlength && (rbdata->nwalls <= 1 || (*p2 - rbdata->pfirst1).length() > rbdata->width * 2.0))
         p6 = p5 + (p6 - p5).normalize() * rbdata->length;
       Wall wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,angle2,angle3,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
       wall.draw(DM_INVERT);
    }
}

class WSRadioButtonDialogControl : public RadioButtonDialogControl
{public:
  WSRadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {};
  void load(Dialog *);
  int select(Dialog *);
};

void WSRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  dialog->getcontrol(id2+1)->enable(dialog,oldvalue > 0);
  dialog->getcontrol(id2+101)->enable(dialog,oldvalue > 0);
}

int WSRadioButtonDialogControl::select(Dialog *dialog)
{ RadioButtonDialogControl::select(dialog);
  dialog->getcontrol(id2+1)->enable(dialog,oldvalue > 0);
  dialog->getcontrol(id2+101)->enable(dialog,oldvalue > 0);
  return 0;
}

void architect_command1(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect1_data *rbdata = (RBArchitect1_data *) *data;
 Point p1,p2,p3,p4,p5,p6,xa,ya;
 Wall *wall;
 double cosa,angle2,angle3,l,angle;
 Dialog dialog("ArInsertWall_Dialog");
 DialogControl *dc;
 RCCHAR label[300],wm[300];

  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect1_data;  *data = rbdata;
       if (rbdata != 0)
         {  rbdata->length = 3000.0;
            rbdata->fixlength = 1;
            Wall::getdefaults(false,rbdata->name,wm,&rbdata->width,&rbdata->height,&rbdata->e,&rbdata->p,&rbdata->pd,&rbdata->ph,&rbdata->mc,&rbdata->rc,&rbdata->lhs,&rbdata->rhs,&rbdata->lha,&rbdata->rha,&rbdata->nattributes,rbdata->attribute,&rbdata->options,&rbdata->options);
            if (ArWallCavity < rbdata->nattributes && rbdata->attribute[ArWallCavity] != 0.0)
              rbdata->offset = (rbdata->width + rbdata->attribute[ArWallWidth2] + rbdata->attribute[ArWallGap]) / 2.0;
            else    
              rbdata->offset = rbdata->width / 2.0;
            rbdata->options.clear(1);
            rbdata->sidelocated = 0;
            rbdata->nwalls = 0;
            rbdata->angle1 = 0.0;
            rbdata->snapangle = v.getreal("ln::autosnapangle") * M_PI / 180.0;
            rbdata->shiftstate = 0;
            rbdata->wall = 0;
            *cstate = 1;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            *cstate = 2;
         }
       else if (event->isa(EntitySelected))
         {  state.destroyselection(0);
            rbdata->wall = ((EntitySelectedEvent *) event)->getentity();
            rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
            rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
            rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);

            Dialog dialog("ArInsertWallStart_Dialog");
            rbdata->positionlocation = 1;
            rbdata->distance = 3000.0;
            strcpy(label,"Distance");
            dialog.add(new WSRadioButtonDialogControl(100,100,101,&rbdata->positionlocation));
            dialog.add(new WSRadioButtonDialogControl(101,100,101,&rbdata->positionlocation));
            dialog.add(dc = new RealDialogControl(102,&rbdata->distance,ScaleByLength));
            dialog.add(new StringDialogControl(202,label,300));
            if (dialog.process() == TRUE)
              *cstate = 3;
            else
              {  rbdata->wall->unhighlight();
                 *cstate = ExitState;
              } 
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       db.grid.setsnap(rbdata->gridstate);
       state.destroyselection(1);
       if (event->isa(Coordinate3d) || event->isa(EntitySelected))
         {  if (rbdata->nwalls > 0 && rbdata->sidelocated == 0)
              {  rbdata->sidelocated = 1;
                 break;
              }
            if (event->isa(EntitySelected))
              {  if (rbdata->wall != 0)
                   {  p1 = db.grid.snappoint(((EntitySelectedEvent *) event)->getwindow(),((EntitySelectedEvent *) event)->getp());
                      p2 = ((EntitySelectedEvent *) event)->getentity()->nearp(p1);
                      rbdata->p1 = rbdata->wall->nearp(p2);
                      p2 += (rbdata->p1 - p2).normalize() * rbdata->offset;
                      rbdata->p1 += (p2 - rbdata->p1).normalize() * rbdata->offset;
                   }
                 else
                   {  p2 = ((EntitySelectedEvent *) event)->getentity()->nearp(rbdata->p1);
                      p2 += (rbdata->p1 - p2).normalize() * (((Wall *)((EntitySelectedEvent *) event)->getentity())->gettotalwidth() / 2.0);
                   }
              }
            else
              {  if (rbdata->wall != 0)
                   {  p2 = ((Coordinate3dEvent *) event)->getp();
                      rbdata->p1 = rbdata->wall->nearp(p2);
                      rbdata->p1 += (p2 - rbdata->p1).normalize() * rbdata->offset;
                   }
                 else
                   p2 = ((Coordinate3dEvent *) event)->getp();
              }

            if (rbdata->nwalls > 1 && (p2 - rbdata->pfirst1).length() < rbdata->width * 2.0)
              {  p2 = rbdata->pfirst1;
                 if (! cadwindow->getshiftstate())
                   {  p5 = rbdata->pfirst1;
                      p6.x = rbdata->xaxis.dot(rbdata->p1 - rbdata->pfirst1);
                      p6.y = rbdata->yaxis.dot(rbdata->p1 - rbdata->pfirst1);
                      if ((l = sqrt(p6.x*p6.x + p6.y*p6.y)) < db.getptoler()) return;
                      p6.x /= l;  p6.y /= l;
                      if (p6.x > 1.0) p6.x = 1.0;  else if (p6.x < -1.0) p6.x = -1.0;
                      angle = acos(p6.x);
                      if (p6.y < 0.0) angle = 2.0 * M_PI - angle;
                      if (fabs(rbdata->snapangle) > 1.0E-10)
                        angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
                      p6 = p5 + rbdata->xaxis * (l * cos(angle)) +
                                rbdata->yaxis * (l * sin(angle));
                      Line line1(rbdata->plast,rbdata->p1);
                      Line line2(p6,rbdata->pfirst1);
                      Intersect i(&line1,&line2);
                      if (i.nintersections() != 0)
                        rbdata->p1 = ((PointE *)i.intersection(0))->getp();
                   }
                 *cstate = 1;
              }
            else if (! cadwindow->getshiftstate())
              {  p3 = rbdata->p1;
                 p4.x = rbdata->xaxis.dot(p2 - p3);
                 p4.y = rbdata->yaxis.dot(p2 - p3);
                 if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
                 p4.x /= l;  p4.y /= l;
                 if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                 angle = acos(p4.x);
                 if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
                 if (fabs(rbdata->snapangle) > 1.0E-10)
                   angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
                 Line line1(p3,p3 + rbdata->xaxis * (l * cos(angle)) +
                           rbdata->yaxis * (l * sin(angle)));
                 p2 = line1.nearp(p2);
                 if (rbdata->fixlength)
                   p2 = rbdata->p1 + (p2 - rbdata->p1).normalize() * rbdata->length;
              }
            else if (rbdata->fixlength)
              p2 = rbdata->p1 + (p2 - rbdata->p1).normalize() * rbdata->length;

            if (rbdata->nwalls == 0)
              angle2 = 0.0;
            else
              {  cosa = (p2 - rbdata->p1).normalize().dot((rbdata->plast - rbdata->p1).normalize());
                 if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                 angle2 = acos(cosa);
                 if (rbdata->zaxis.dot((p2 - rbdata->p1).cross(rbdata->plast - rbdata->p1)) > 0.0)
                 angle2 = 2.0 * M_PI - angle2;
                 angle2 = M_PI / 2.0 - angle2 / 2.0;
              }
            if (rbdata->nwalls == 1)
              {  rbdata->pfirst1 = rbdata->plast;
                 rbdata->pfirst2 = rbdata->p1;
                 rbdata->angle4 = angle2;
              }
            else if (rbdata->nwalls > 0)
              {  if (*cstate == 1)
                   {  cosa = (rbdata->pfirst2 - rbdata->pfirst1).normalize().dot((rbdata->p1 - rbdata->pfirst1).normalize());
                      if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                      angle3 = acos(cosa);
                      if (rbdata->zaxis.dot((rbdata->pfirst2 - rbdata->pfirst1).cross(rbdata->p1 - rbdata->pfirst1)) > 0.0)
                        angle3 = 2.0 * M_PI - angle3;
                      angle3 = M_PI / 2.0 - angle3 / 2.0;
                      xa = (rbdata->pfirst2 - rbdata->pfirst1).normalize() * rbdata->offset;
                      ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                      p5 = rbdata->pfirst1 - ya - xa*tan(angle3);
                      p6 = rbdata->pfirst2 - ya - xa*tan(-rbdata->angle4);
                      rbdata->options.set(ArWallFrameExtendStart);   
                      if (rbdata->offset > 0.0)
                        wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,-rbdata->angle4,-angle3,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      else
                        wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,angle3,rbdata->angle4,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      if (wall != 0 && wall->getdefined())
                        db.geometry.add(wall);
                      else
                        delete wall;
                      xa = (rbdata->pfirst1 - rbdata->p1).normalize() * rbdata->offset;
                      ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                      p5 = rbdata->p1 - ya - xa*tan(angle2);
                      p6 = rbdata->pfirst1 - ya - xa*tan(-angle3);
                      if (rbdata->offset > 0.0)
                        wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,-angle3,-angle2,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      else
                        wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,angle2,angle3,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      if (wall != 0 && wall->getdefined())
                        db.geometry.add(wall);
                      else
                        delete wall;
                      *cstate = 1;
                      rbdata->nwalls = -1;
                   }
                 xa = (rbdata->p1 - rbdata->plast).normalize() * rbdata->offset;
                 ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                 p5 = rbdata->plast - ya - xa*tan(rbdata->angle1);
                 p6 = rbdata->p1 - ya - xa*tan(-angle2);
                 rbdata->options.set(ArWallFrameExtendStart);   
                 if (rbdata->offset > 0.0)           
                   wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,-angle2,-rbdata->angle1,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                 else
                   wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,rbdata->angle1,angle2,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                 if (wall != 0 && wall->getdefined())
                   db.geometry.add(wall);
                 else
                   delete wall;
              }
            rbdata->plast = rbdata->p1;
            rbdata->p1 = p2;
            rbdata->angle1 = angle2;
            rbdata->nwalls++;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  if (rbdata->nwalls > 0)
              {  if (rbdata->nwalls > 1)
                   {  xa = (rbdata->pfirst2 - rbdata->pfirst1).normalize() * rbdata->offset;
                      ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                      p5 = rbdata->pfirst1 - ya - xa*tan(0.0);
                      p6 = rbdata->pfirst2 - ya - xa*tan(-rbdata->angle4);
                      rbdata->options.set(ArWallFrameExtendStart);   
                      rbdata->options.set(ArWallCloseStartBit);   
                      if (rbdata->offset > 0.0) 
                        wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,-rbdata->angle4,0.0,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      else 
                        wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,0.0,rbdata->angle4,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      rbdata->options.clear(ArWallCloseStartBit);   
                      if (wall != 0 && wall->getdefined())
                        db.geometry.add(wall);
                      else
                        delete wall;
                   }
                 xa = (rbdata->p1 - rbdata->plast).normalize() * rbdata->offset;
                 ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                 p5 = rbdata->plast - ya - xa*tan(rbdata->angle1);
                 p6 = rbdata->p1 - ya - xa*tan(0.0);
                 rbdata->options.set(ArWallFrameExtendStart);
                 if (rbdata->nwalls == 1)  
                   rbdata->options.set(ArWallCloseStartBit);
                 rbdata->options.set(ArWallCloseEndBit);
                 if (rbdata->offset > 0.0) 
                   wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,0.0,-rbdata->angle1,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                 else 
                   wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,rbdata->angle1,0.0,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                 rbdata->options.clear(ArWallCloseEndBit);
                 if (wall != 0 && wall->getdefined())
                   db.geometry.add(wall);
                 else
                   delete wall;
              }
            if (event->isa(Abort))
              {  *cstate = 1;
                 rbdata->nwalls = 0;
              }
            else
              *cstate = ExitState;
         }
       if (rbdata->wall != 0)
         {  rbdata->wall->unhighlight();
            rbdata->wall = 0;
         }
       break;
     case 3 :
       rubberband.end(0);
       rbdata->wall->unhighlight();
       if (event->isa(Coordinate3d))
         {  if (rbdata->positionlocation == 0)
              p1 = ((Coordinate3dEvent *) event)->getp();
            else
              {  p3 = ((Coordinate3dEvent *) event)->getp();
                 p1 = p3 + (((Wall *)rbdata->wall)->getp2() - ((Wall *)rbdata->wall)->getp1()).normalize() * rbdata->distance;
                 p2 = p3 + (((Wall *)rbdata->wall)->getp1() - ((Wall *)rbdata->wall)->getp2()).normalize() * rbdata->distance;
                 if ((rbdata->p1 - p3).dot(p1 - p3) > 0.0)
                   rbdata->p1 = p1;
                 else
                   rbdata->p1 = p2;
              }
            rbdata->wall = 0;
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    {  if (*cstate == 2 && rbdata->nwalls == 1)
         state.selmask.entity.clearandset(xyz_mask,end_list);
       else 
         state.selmask.entity.clearandset(xyz_mask,wall_entity,end_list);
       if (*cstate == 2 && (rbdata->nwalls != 1 || rbdata->sidelocated != 0))
         {Dialog dialog("ArInsertWallLength_Dialog");
          DialogControl *dc;
            dialog.add(dc = new RealDialogControl(100,&rbdata->length,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1100,dc));
            dialog.add(new CheckBoxDialogControl(101,&rbdata->fixlength));
            if (dialog.process() == FALSE)
              {  if (rbdata->wall != 0)
                   rbdata->wall->unhighlight();
                 if (rbdata->nwalls > 0)
                   {  if (rbdata->nwalls > 1)
                        {  xa = (rbdata->pfirst2 - rbdata->pfirst1).normalize() * rbdata->offset;
                           ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                           p5 = rbdata->pfirst1 - ya - xa*tan(0.0);
                           p6 = rbdata->pfirst2 - ya - xa*tan(-rbdata->angle4);
                           rbdata->options.set(ArWallFrameExtendStart);   
                           rbdata->options.set(ArWallCloseStartBit);   
                           if (rbdata->offset > 0.0) 
                             wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,-rbdata->angle4,0.0,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                           else
                             wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,0.0,rbdata->angle4,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                           rbdata->options.clear(ArWallCloseStartBit);   
                           if (wall != 0 && wall->getdefined())
                             db.geometry.add(wall);
                           else
                             delete wall;
                        }
                      xa = (rbdata->p1 - rbdata->plast).normalize() * rbdata->offset;
                      ya = xa.cross(rbdata->zaxis).normalize() * fabs(rbdata->offset);
                      p5 = rbdata->plast - ya - xa*tan(rbdata->angle1);
                      p6 = rbdata->p1 - ya - xa*tan(0.0);
                      rbdata->options.set(ArWallFrameExtendStart);
                      rbdata->options.set(ArWallCloseEndBit);
                      if (rbdata->offset > 0.0) 
                        wall = new Wall(p6,p5,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,0.0,-rbdata->angle1,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      else 
                        wall = new Wall(p5,p6,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,rbdata->e,rbdata->p,rbdata->pd,rbdata->ph,rbdata->mc,rbdata->rc,rbdata->lhs,rbdata->rhs,rbdata->lha,rbdata->rha,rbdata->angle1,0.0,rbdata->nattributes,rbdata->attribute,rbdata->options,rbdata->options);
                      rbdata->options.clear(ArWallCloseEndBit);
                      if (wall != 0 && wall->getdefined())
                        db.geometry.add(wall);
                      else
                        delete wall;
                   }
                 *cstate = ExitState;
              }
         }
    }

  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCARCHITECT+2);
  else if (*cstate == 2)
    {  rbdata->gridstate = db.grid.getsnap();
       rubberband.begin(0);
       rubberband.add(RBArchitect1,rbdata);
       if (rbdata->nwalls == 1 && rbdata->sidelocated == 0)
         {  cadwindow->prompt->normalprompt("Locate the outside of the wall.  The wall will be placed on the other side.");
            db.grid.setsnap(0);
         }
       else
         cadwindow->prompt->normalprompt(NCARCHITECT+3);
    }
  else if (*cstate == 3)
    {  rubberband.begin(0);
       rubberband.add(RBArchitect1,rbdata);
       if (rbdata->positionlocation == 0)  
         cadwindow->prompt->normalprompt("Locate the position of the wall");
       else
         cadwindow->prompt->normalprompt("Locate the reference position of the wall");
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void architect_command2(int *cstate,HighLevelEvent *,void **)
{double width,height,ea,p,pd,ph,lhs,rhs,lha,rha,attributes[ArMaxWallAttributes];
 int mc,rc;
 short nattributes;
 BitMask change(32),options(32);
 RCCHAR name[300],wm[300];
 EntityList list;
 EntityHeader header;
 Entity *e;
  *cstate = ExitState;
  list = state.getselection();
  if (list.empty())
    {  Wall::getdefaults(false,name,wm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,&nattributes,attributes,&options,&options);
       if (Wall::setup(0,name,wm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,nattributes,attributes,&options,&options))
         Wall::setdefaults(name,wm,width,height,ea,p,pd,ph,mc,rc,lhs,rhs,lha,rha,nattributes,attributes,options,options);
    }
  else
    {  Wall::getdefaults(false,name,wm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,&nattributes,attributes,&options,&options);
       if (Wall::setup(&change,name,wm,&width,&height,&ea,&p,&pd,&ph,&mc,&rc,&lhs,&rhs,&lha,&rha,nattributes,attributes,&options,&options))
         {  db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if (e->isa(wall_entity) && ! ((Wall *) e)->isacolumn())
                ((Wall *)e)->change(change,header,name,_RCT(""),width,height,ea,p,pd,ph,mc,rc,lhs,rhs,lha,rha,nattributes,attributes,options,options);
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
}

struct RBArchitect3_data
  {Wall *wall;
   Point p1,p2;
   int end;
   double angle1,angle2;
  };

void RBArchitect3(int,void *data,Point *p2,View3dSurface *)
{RBArchitect3_data *rbdata = (RBArchitect3_data *) data;
 BitMask options(32);
 Point p3;
  Line line(rbdata->p1,rbdata->p2);
  p3 = line.nearp(*p2);
  if (rbdata->end == 1)
    {  Wall wall(p3,rbdata->p2,rbdata->wall->getzaxis(),
                    rbdata->wall->getname(),_RCT(""),
                    rbdata->wall->getwidth(),rbdata->wall->getheight(),
                    rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                    rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                    rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                    rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                    rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                    rbdata->angle1,rbdata->angle2,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
  else
    {  Wall wall(rbdata->p1,p3,rbdata->wall->getzaxis(),
                    rbdata->wall->getname(),_RCT(""),
                    rbdata->wall->getwidth(),rbdata->wall->getheight(),
                    rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                    rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                    rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                    rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                    rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                    rbdata->angle2,rbdata->angle1,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
}

void architect_command3(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect3_data *rbdata = (RBArchitect3_data *) *data;
 Point p1,p2,p3,p4,p5,p6;
 Wall *wall;
 double cosa,angle;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect3_data;  *data = rbdata;
       if (rbdata != 0)
         *cstate = 1;
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            rbdata->wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            if ((rbdata->wall->getp1() - ((EntitySelectedEvent *) event)->getp()).length() <
                (rbdata->wall->getp2() - ((EntitySelectedEvent *) event)->getp()).length())
              {  rbdata->angle1 = 0.0;
                 rbdata->angle2 = rbdata->wall->getangle2();
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 1;
              }
            else
              {  rbdata->angle1 = rbdata->wall->getangle1();
                 rbdata->angle2 = 0.0;
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 2;
              }
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       state.destroyselection(1);
       if (event->isa(Coordinate3d))
         {  Line line(rbdata->p1,rbdata->p2);
            p3 = line.nearp(((Coordinate3dEvent *)event)->getp());
            if (rbdata->end == 1)
              {  if ((p3 - rbdata->p2).length() > db.getptoler())
                   rbdata->wall->changegeometry(p3,rbdata->p2,rbdata->angle1,rbdata->angle2);
              }
            else
              {  if ((p3 - rbdata->p1).length() > db.getptoler())
                   rbdata->wall->changegeometry(rbdata->p1,p3,rbdata->angle1,rbdata->angle2);
              }
            *cstate = 1;
         }
       else if (event->isa(EntitySelected))
         {  wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            Line line1(wall->getp1(),wall->getp2());
            Line line2(rbdata->p1,rbdata->p2);
            Intersect i(&line1,&line2);
            if (i.nintersections() == 1)
              {  cosa = line1.direction(0.0).dot(line2.direction(0.0));
                 if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                 angle = acos(cosa) - M_PI / 2.0;
                 if (line1.direction(0.0).cross(line2.direction(0.0)).dot(wall->getzaxis()) > 0.0)
                   angle = -angle;
                 p3 = ((PointE *)i.intersection(0))->getp();
                 if (rbdata->end == 1)
                   {  p3 = p3 + line2.direction(0.0) * (wall->gettotalwidth() / 2.0 / fabs(sin(M_PI/2.0-angle)));
                      if ((p3 - rbdata->p2).length() > db.getptoler())
                        rbdata->wall->changegeometry(p3,rbdata->p2,angle,rbdata->angle2);
                   }
                 else
                   {  p3 = p3 - line2.direction(0.0) * (wall->gettotalwidth() / 2.0 / fabs(sin(M_PI/2.0-angle)));
                      angle = -angle;
                      if ((p3 - rbdata->p1).length() > db.getptoler())
                        rbdata->wall->changegeometry(rbdata->p1,p3,rbdata->angle1,angle);
                   }
              }
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  *cstate = 1;
         }
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(wall_entity,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+35);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(wall_entity,xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect3,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+36);
    }
  else if (*cstate == ExitState)
    delete rbdata;

}

struct RBArchitect17_data
  {Wall *wall;
   Point p1,p2;
   int end;
   double angle1,angle2;
  };

void RBArchitect17(int,void *data,Point *p2,View3dSurface *)
{RBArchitect17_data *rbdata = (RBArchitect17_data *) data;
 BitMask options(32);
 Point p3;
  Line line(rbdata->p1,rbdata->p2);
  p3 = line.nearp(*p2);
  if (rbdata->end == 1)
    {  Wall wall(p3,rbdata->p2,rbdata->wall->getzaxis(),
                    rbdata->wall->getname(),_RCT(""),
                    rbdata->wall->getwidth(),rbdata->wall->getheight(),
                    rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                    rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                    rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                    rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                    rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                    rbdata->angle1,rbdata->angle2,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
  else
    {  Wall wall(rbdata->p1,p3,rbdata->wall->getzaxis(),
                     rbdata->wall->getname(),_RCT(""),
                     rbdata->wall->getwidth(),rbdata->wall->getheight(),
                     rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                     rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                     rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                     rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                     rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                     rbdata->angle2,rbdata->angle1,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
}

void architect_command17(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect17_data *rbdata = (RBArchitect17_data *) *data;
 Point p1,p2,p3,p4,p5,p6;
 Wall *wall;
 double cosa,angle,a,d1,d2;
 int end,extend;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect17_data;  *data = rbdata;
       if (rbdata != 0)
         *cstate = 1;
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            rbdata->wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            if ((rbdata->wall->getp1() - ((EntitySelectedEvent *) event)->getp()).length() <
                (rbdata->wall->getp2() - ((EntitySelectedEvent *) event)->getp()).length())
              {  rbdata->angle1 = 0.0;
                 rbdata->angle2 = rbdata->wall->getangle2();
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 1;
              }
            else
              {  rbdata->angle1 = rbdata->wall->getangle1();
                 rbdata->angle2 = 0.0;
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 2;
              }
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       state.destroyselection(1);
       if (event->isa(EntitySelected))
         {  wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            if ((wall->getp1() - ((EntitySelectedEvent *) event)->getp()).length() <
                (wall->getp2() - ((EntitySelectedEvent *) event)->getp()).length())
              end = 1;
            else
              end = 2;
            Line line1(wall->getp1(),wall->getp2());
            Line line2(rbdata->p1,rbdata->p2);
            Intersect i(&line1,&line2);
            if (i.nintersections() == 1)
              {  p3 = ((PointE *)i.intersection(0))->getp();
                 if (rbdata->end == 1)
                   {  d1 = (p3 - rbdata->p2).length();  p1 = rbdata->p2;
                   }
                 else
                   {  d1 = (p3 - rbdata->p1).length();  p1 = rbdata->p1;
                   }
                 if (end == 1)
                   {  d2 = (p3 - wall->getp2()).length();  p2 = wall->getp2();
                   }
                 else
                   {  d2 = (p3 - wall->getp1()).length();  p2 = wall->getp1();
                   }
                 if (d1 > db.getptoler() && d2 > db.getptoler())
                   {  cosa = (p2 - p3).normalize().dot((p1 - p3).normalize());
                      if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                      angle = acos(cosa);
                      if (wall->getzaxis().dot((p2 - p3).cross(p1 - p3)) > 0.0)
                        angle = 2.0 * M_PI - angle;
                      db.saveundo(UD_STARTBLOCK,NULL);
                      if ((rbdata->wall->getp2() - rbdata->wall->getp1()).dot(p3 - p1) < 0.0)
                        a = M_PI / 2.0 - (2.0 * M_PI - angle) / 2.0;
                      else
                        a = M_PI / 2.0 - angle / 2.0;
                      //  Only extend if the angle is exactly 90.
                      extend = fabs(fabs(a) - M_PI / 4.0) < 0.01;
                      if (rbdata->end == 1)
                        {BitMask options(32),change(32);
                           change.set(18); 
                           change.set(30);   //  Set the extend start bit 
                           options.set(ArWallFrameExtendStart,extend);
                           rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           rbdata->wall->changegeometry(p3,rbdata->p2,a,rbdata->wall->getangle2());
                        } 
                      else
                        {BitMask options(32),change(32);
                           change.set(19);
                           change.set(31);   //  Set the extend end bit
                           options.set(ArWallFrameExtendEnd,extend);
                           rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           rbdata->wall->changegeometry(rbdata->p1,p3,rbdata->wall->getangle1(),a);
                        }
                      if ((wall->getp2() - wall->getp1()).dot(p3 - p2) > 0.0)
                        a = M_PI / 2.0 - (2.0 * M_PI - angle) / 2.0;
                      else
                        a = M_PI / 2.0 - angle / 2.0;
                      if (end == 1)
                        {BitMask options(32),change(32);
                           change.set(18);
                           change.set(30);   //  Clear the extend start bit
                           wall->change(change,*wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           wall->changegeometry(p3,wall->getp2(),a,wall->getangle2());
                        } 
                      else
                        {BitMask options(32),change(32);
                           change.set(19);
                           change.set(31);   //  Clear the extend end bit
                           wall->change(change,*wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           wall->changegeometry(wall->getp1(),p3,wall->getangle1(),a);
                        } 
                      db.saveundo(UD_ENDBLOCK,NULL);
                   }
              }
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = 1;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(wall_entity,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+39);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(wall_entity,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect17,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+40);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect4_data
  {Wall *wall;
   Point p1,p2;
   int end;
   double angle1,angle2;
  };

void RBArchitect4(int,void *data,Point *p2,View3dSurface *)
{RBArchitect4_data *rbdata = (RBArchitect4_data *) data;
 BitMask options(32);
 Point p3;
  Line line(rbdata->p1,rbdata->p2);
  p3 = line.nearp(*p2);
  if (rbdata->end == 1)
    {  Wall wall(p3,rbdata->p2,rbdata->wall->getzaxis(),
                    rbdata->wall->getname(),_RCT(""),
                    rbdata->wall->getwidth(),rbdata->wall->getheight(),
                    rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                    rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                    rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                    rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                    rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                    rbdata->angle1,rbdata->angle2,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
  else
    {  Wall wall(rbdata->p1,p3,rbdata->wall->getzaxis(),
                    rbdata->wall->getname(),_RCT(""),
                    rbdata->wall->getwidth(),rbdata->wall->getheight(),
                    rbdata->wall->geteaves(),rbdata->wall->getpitch(),
                    rbdata->wall->getpeakdistance(),rbdata->wall->getpeakdistance(),
                    rbdata->wall->getmiddlecolour(),rbdata->wall->getrightcolour(),
                    rbdata->wall->getlhatchspacing(),rbdata->wall->getrhatchspacing(),
                    rbdata->wall->getrhatchangle(),rbdata->wall->getrhatchangle(),
                    rbdata->angle2,rbdata->angle1,rbdata->wall->getnattributes(),rbdata->wall->getattributes(),options,options);
       wall.draw(DM_INVERT);
    }
}

void architect_command4(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect4_data *rbdata = (RBArchitect4_data *) *data;
 Point p1,p2,p3,p4,p5,p6;
 Wall *wall;
 double cosa,angle;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect4_data;  *data = rbdata;
       if (rbdata != 0)
         *cstate = 1;
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            rbdata->wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            if ((rbdata->wall->getp1() - ((EntitySelectedEvent *) event)->getp()).length() <
                (rbdata->wall->getp2() - ((EntitySelectedEvent *) event)->getp()).length())
              {  rbdata->angle1 = 0.0;
                 rbdata->angle2 = rbdata->wall->getangle2();
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 1;
              }
            else
              {  rbdata->angle1 = rbdata->wall->getangle1();
                 rbdata->angle2 = 0.0;
                 rbdata->p1 = rbdata->wall->getp1();
                 rbdata->p2 = rbdata->wall->getp2();
                 rbdata->end = 2;
              }
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       state.destroyselection(1);
       if (event->isa(Coordinate3d))
         {  Line line(rbdata->p1,rbdata->p2);
            p3 = line.nearp(((Coordinate3dEvent *)event)->getp());
            if (rbdata->end == 1)
              {  if ((p3 - rbdata->p2).length() > db.getptoler())
                   {BitMask options(32),change(32);
                      change.set(18); 
                      change.set(30);   //  Set the extend start bit 
                      rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                      rbdata->wall->changegeometry(p3,rbdata->p2,rbdata->angle1,rbdata->angle2);
                   }
              }
            else
              {  if ((p3 - rbdata->p1).length() > db.getptoler())
                   {BitMask options(32),change(32);
                      change.set(19);
                      change.set(31);   //  Set the extend start bit
                      rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                      rbdata->wall->changegeometry(rbdata->p1,p3,rbdata->angle1,rbdata->angle2);
                   }
              }
            *cstate = 1;
         }
       else if (event->isa(EntitySelected))
         {  wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            Line line1(wall->getp1(),wall->getp2());
            Line line2(rbdata->p1,rbdata->p2);
            Intersect i(&line1,&line2);
            if (i.nintersections() == 1)
              {  cosa = line1.direction(0.0).dot(line2.direction(0.0));
                 if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                 angle = acos(cosa) - M_PI / 2.0;
                 if (line1.direction(0.0).cross(line2.direction(0.0)).dot(wall->getzaxis()) > 0.0)
                   angle = -angle;
                 p3 = ((PointE *)i.intersection(0))->getp();
                 if (rbdata->end == 1)
                   {  p3 = p3 + line2.direction(0.0) * (wall->gettotalwidth() / 2.0 / fabs(sin(M_PI/2.0-angle)));
                      if ((p3 - rbdata->p2).length() > db.getptoler())
                        {BitMask options(32),change(32);
                           db.saveundo(UD_STARTBLOCK,NULL);
                           change.set(18);
                           change.set(30);   //  Set the extend start bit
                           options.set(ArWallFrameExtendStart);
                           rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           rbdata->wall->changegeometry(p3,rbdata->p2,angle,rbdata->angle2);
                           wall->breakwall(rbdata->wall);
                           db.saveundo(UD_ENDBLOCK,NULL);
                        }
                   }
                 else
                   {  p3 = p3 - line2.direction(0.0) * (wall->gettotalwidth() / 2.0 / fabs(sin(M_PI/2.0-angle)));
                      angle = -angle;
                      if ((p3 - rbdata->p1).length() > db.getptoler())
                        {BitMask options(32),change(32);
                           db.saveundo(UD_STARTBLOCK,NULL);
                           change.set(19);
                           change.set(31);   //  Set the extend start bit
                           options.set(ArWallFrameExtendEnd);
                           rbdata->wall->change(change,*rbdata->wall,_RCT(""),_RCT(""),0.0,0.0,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
                           rbdata->wall->changegeometry(rbdata->p1,p3,rbdata->angle1,angle);
                           wall->breakwall(rbdata->wall);
                           db.saveundo(UD_ENDBLOCK,NULL);
                        }
                   }
              }
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  *cstate = 1;
         }
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(wall_entity,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+37);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(wall_entity,xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect4,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+38);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect5_data
  {int state,centre,fromend;
   double enddistance,length,width,height,topframingallowance,bottomframingallowance,sideframingallowance;
   RCCHAR library[300];
   RCCHAR name[300];
   RCCHAR label[300];
   Point p1,xaxis,yaxis;
   Wall *wall;
   Figure *figure;
  };

void RBArchitect5(int,void *data,Point *p2,View3dSurface *)
{RBArchitect5_data *rbdata = (RBArchitect5_data *) data;
 Point o,scale,rotation,p3;
 Byte state1;
 GeneralList list;
  scale.setxyz(1.0,1.0,1.0);  rotation.setxyz(0.0,0.0,0.0);
  state1 = 1;
  if (rbdata->state == 3)
    {  if ((*p2 - rbdata->p1).dot(rbdata->xaxis) > 0.0)
         state1 += (Byte)1;
       if ((*p2 - rbdata->p1).dot(rbdata->yaxis) < 0.0)
         state1 += (Byte)2;
       o = rbdata->p1;
    }
  else if (rbdata->state == 4)
    {  p3 = rbdata->wall->nearp(*p2);
       if ((rbdata->wall->nearp(rbdata->p1) - p3).dot(rbdata->wall->getp2() - rbdata->wall->getp1()) > 0.0)
         o = p3 + (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * rbdata->enddistance;
       else
         o = p3 - (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * (rbdata->enddistance + rbdata->width);
    }
  else
    o = rbdata->wall->nearp(*p2);

  rbdata->figure->newtransform(o,rbdata->xaxis,rbdata->yaxis,scale,rotation);
  rbdata->figure->setstate1(state1);
  if (rbdata->figure->getdefined()) 
    rbdata->figure->draw(DM_INVERT);

}

class ArCatListDialogControl : public ListDialogControl
  {private:
    RCCHAR *type,lastvalue[300];
   public:
     ArCatListDialogControl(int id,RCCHAR *t,RCCHAR *category) :
         ListDialogControl(id,0,0,category,300)  {  type = t;  }
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void ArCatListDialogControl::load(Dialog *dialog)
{RCCHAR key1[256],*s1;
 int i;
  for (i = 0 ; ; i++)
    {  sprintf(key1,"ar::%scategory(%d)",type,i);
       if (strlen(s1 = v.getstring(key1)) > 0)
         {  removeitem(dialog,s1);
            additem(dialog,s1);
         }
       else
         break;
    }
  dialog->SetDlgItemText(id,svalue);
  dialog->getcontrol(id+1)->load(dialog);
  strcpy(lastvalue,svalue);
}

void ArCatListDialogControl::changefocus(Dialog *dialog,int)
{RCCHAR value[300];
  if (dialog->currentvalue(id,value,300))
    {  if (strcmp(value,lastvalue) != 0)
         {  dialog->getcontrol(id+1)->load(dialog);
            strcpy(lastvalue,value);
         }
    }
}

class ArListDialogControl : public ListDialogControl
  {private:
    RCCHAR *type;
   public:
     ArListDialogControl(int id,RCCHAR *t,RCCHAR *library) :
         ListDialogControl(id,0,0,library,300)  {  type = t;  }
     void load(Dialog *);
     int store(Dialog *);
  };

void ArListDialogControl::load(Dialog *dialog)
{
  RCCHAR category[300],*s1,*s2;
  RCCHAR key1[256],key2[256];
 int i,c;
  removeallitems(dialog);
  dialog->currentvalue(id-1,category,300);
  for (i = 0,c = 0 ; ; i++)
    {  sprintf(key1,"ar::%scategory(%d)",type,i);
       if (RCSTRLEN(s1 = v.getstring(key1,s1)) == 0)
         break;
       if (RCSTRCMP(s1,category) == 0)
         {  sprintf(key1,"ar::%sdesc(%d)",type,i);
            if (RCSTRLEN(v.getstring(key1)) > 0)
              {  SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)v.getstring(key1)));
                 sprintf(key2,"ar::%sname(%d)",type,i);
                 s1 = v.getstring(key2,s1);
                 s2 = svalue + strlen(svalue) - strlen(s1);
                 if (c == 0 || RCSTRCMP(s1,s2) == 0)
                   dialog->SetDlgItemText(id,v.getstring(key1));
                 c++;
              }
            else
              break;
         }
    }
  changefocus(dialog);
}

int ArListDialogControl::store(Dialog *dialog)
{RCCHAR key1[256],key2[256],value[256];
 int i;
  dialog->GetDlgItemText(id,value,256);
  for (i = 0 ; ; i++)
    {  sprintf(key1,"ar::%sdesc(%d)",type,i);
       if (strcmp(v.getstring(key1),value) == 0)
         {  sprintf(key2,"ar::%sname(%d)",type,i);
            strcpy(svalue,home.getpublicfilename(v.getstring(key2)));
            return 1;
         }
       else if (strlen(v.getstring(key1)) == 0)
         break;
    }
  return 0;
}

class ADRadioButtonDialogControl : public RadioButtonDialogControl
{public:
  ADRadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {};
  void load(Dialog *);
  int select(Dialog *);
};

void ADRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  dialog->getcontrol(id2+1)->enable(dialog,oldvalue >= 2);
  dialog->getcontrol(id2+101)->enable(dialog,oldvalue >= 2);
  dialog->getcontrol(id2+1001)->enable(dialog,oldvalue >= 2);
}

int ADRadioButtonDialogControl::select(Dialog *dialog)
{ RadioButtonDialogControl::select(dialog);
  dialog->getcontrol(id2+1)->enable(dialog,oldvalue >= 2);
  dialog->getcontrol(id2+101)->enable(dialog,oldvalue >= 2);
  dialog->getcontrol(id2+1001)->enable(dialog,oldvalue >= 2);
  return 0;
}

class ArWindowDisplayDialogControl : public DisplayDialogControl
  {private:
     int loaded;
   public:
     RCCHAR *library,*type,oldlibrary[300];
     ArWindowDisplayDialogControl(int id,RCCHAR *library_,RCCHAR *type_) : DisplayDialogControl(id) {  library = library_;  type = type_;  strcpy(oldlibrary,library);  loaded = 0;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

void ArWindowDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View front(rs0.gets(),0,0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&front,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 int i,x1,y1,w,h;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0);
 BitMask options(32);
 double du,dv,width,height;
 GeneralList list;

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
       IntersectClipRect(output.gethdc(),1,1,int(output.getwidth()-1),int(output.getheight()-1));
       
       du = output.getumax() - output.getumin();
       dv = output.getvmax() - output.getvmin();
       if (du > dv)
         {  dv /= du;  du = 1.0;  
         }
       else
         {  du /= dv;  dv = 1.0;  
         }
       output.zoom(-du * 300,-dv * 300,du * 3000,dv * 3000);

       //  Load the current figure name
       ((ArListDialogControl *)dialog->getcontrol(101))->store(dialog);
       if (strcmp(library,oldlibrary) != 0)
         {//  Load the default width, height and head height
          RCCHAR categorykey[300],desckey[300],widthkey[300],heightkey[300],headheightkey[300],description[300],labelkey[300];

            dialog->GetDlgItemText(101,description,300);
            for (i = 0 ; ; i++)
              {  sprintf(desckey,"ar::%sdesc(%d)",type,i);
                 sprintf(categorykey,"ar::%scategory(%d)",type,i);
                 sprintf(widthkey,"ar::%swidth(%d)",type,i);
                 sprintf(heightkey,"ar::%sheight(%d)",type,i);
                 sprintf(headheightkey,"ar::%sheadheight(%d)",type,i);
                 sprintf(labelkey,"ar::%slabel(%d)",type,i);
                 if (strlen(v.getstring(desckey)) == 0)
                   break;
                 else if (loaded > 1 && strcmp(v.getstring(desckey),description) == 0 && id > 103)
                   {  dialog->SetDlgItemText(id-2,v.getstring(widthkey));
                      dialog->SetDlgItemText(id-1,v.getstring(heightkey));
                      dialog->SetDlgItemText(id+4,v.getstring(labelkey));
                      if (id == 110)
                        dialog->SetDlgItemText(102,v.getstring(headheightkey));
                   }
             }
         }

       strcpy(oldlibrary,library);
      
       
       if (dialog->currentvalue(id-2,&width) && dialog->currentvalue(id-1,&height))
         {Point p1,p2;
          BitMask options(32);


            x1 = (int)output.getumin();
            y1 = (int)output.getvmin();
            w = (int)(output.getumax() - x1);
            h = (int)(output.getvmax() - y1);

            if (id > 103)
              options.set(1);

            org.setxyz((output.getumin() + output.getumax() - width) / 2.0,0.0,(output.getvmin() + output.getvmax() - height) / 2.0);

            Figure figure(header,library,_RCT(""),org,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);

            if (id > 103)
              {  list.add(new Parameter("Width",width));
                 list.add(new Parameter("Height",height));
           
                 figure.loadfigure(&list);
              }

            figure.draw(DM_NORMAL,NULL,&output);

         }
    }

  loaded++;

}

void AddFrameAllowance(double top,double bottom,double side,Entity *figure)
{RCCHAR attribute[300],*at;
  sprintf(attribute,"AR::FrameAllowance::%.0lf,%.0lf,%.0lf",top,bottom,side);
  if ((at = new RCCHAR[strlen(attribute) + 1]) != NULL)
    {  strcpy(at,attribute);
       figure->addat(at);
       figure->setattribute(at);
       db.saveundo(UD_INSERTATT,figure);
    }
}

void AddTextAnnotation(Wall *wall,Point p,double width,double height,RCCHAR *label)
{Point xaxis,yaxis;
 BitMask options(32);
 RCCHAR l1[300],l2[300];


  strcpy(l1,label);
  _strlwr(l1);
  if (strstr(l1,"<width>") != 0)
    {  strcpy(l2,label);
       strcpy(l2+(strstr(l1,"<width>")-l1),"%.0lf");
       strcpy(l2+(strstr(l1,"<width>")-l1)+5,label+(strstr(l1,"<width>")-l1)+7);
       sprintf(label,l2,width);  
    }
 
  strcpy(l1,label);
  _strlwr(l1);
  if (strstr(l1,"<height>") != 0)
    {  strcpy(l2,label);
       strcpy(l2+(strstr(l1,"<height>")-l1),"%.0lf");
       strcpy(l2+(strstr(l1,"<height>")-l1)+5,label+(strstr(l1,"<height>")-l1)+8);
       sprintf(label,l2,height);  
    }

  xaxis = (wall->getp2() - wall->getp1()).normalize();
  yaxis = (wall->getp2() - wall->getp1()).cross(wall->getzaxis()).normalize();

  p = p + xaxis * width / 2.0 - yaxis * 250.0;
  

  if (fabs(xaxis.x) < 0.001)
    {  xaxis.setxyz(0.0,1.0,0.0);
       yaxis.setxyz(-1.0,0.0,0.0);
    }
  else
    {  if (xaxis.x < 0.0)
         xaxis = -xaxis;
       if (yaxis.y < 0.0)
         yaxis = -yaxis;
    }

  options.set(1);  options.set(3);
  Text *text = new Text(_RCT("Arial"),p,xaxis,yaxis,0.0,200.0,0,0,0,label,0,options);
  if (text != NULL && text->getdefined())
    {  //text->EntityHeader::change(text->getcolour(),WALLFRAMING_LAYER,text->getstyle(),text->getweight());
       db.geometry.add(text);
    }
  else
    delete text;

}

void architect_command5(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect5_data *rbdata = (RBArchitect5_data *) *data;
 Point scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),p2,p3,p4,p5,p6,dir;
 Figure *figure;
 BitMask options(32);
 Byte state1;
 ResourceString rs89(NCARCHITECT+89);
 double length;
 RCCHAR category[300];

  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect5_data;  *data = rbdata;
       if (rbdata != 0)
         {Dialog dialog("ArDoor_dialog");
          DialogControl *dc;
            strcpy(category,v.getstring("ar::doorcategory"));
            strcpy(rbdata->library,v.getstring("ar::doorlibrary"));
            strcpy(rbdata->name,_RCT(""));
            rbdata->fromend = v.getinteger("ar::fromend");
            rbdata->enddistance = v.getreal("ar::enddistance");
            rbdata->width = v.getreal("ar::doorwidth");
            rbdata->height = v.getreal("ar::doorheight");
            rbdata->topframingallowance = v.getreal("ar::doortopframingallowance");
            rbdata->bottomframingallowance = v.getreal("ar::doorbottomframingallowance");
            rbdata->sideframingallowance = v.getreal("ar::doorsideframingallowance");
            strcpy(rbdata->label,v.getstring("ar::doorlabel"));
            rbdata->figure = 0;
            dialog.add(new ArCatListDialogControl(100,_RCT("door"),category));
            dialog.add(new ArListDialogControl(101,_RCT("door"),rbdata->library));
            dialog.add(new ADRadioButtonDialogControl(102,102,105,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(103,102,105,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(104,102,105,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(105,102,105,&rbdata->fromend));
            dialog.add(dc = new RealDialogControl(106,&rbdata->enddistance,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1106,dc));
            dialog.add(dc = new RealDialogControl(107,&rbdata->width,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1107,dc));
            dialog.add(dc = new RealDialogControl(108,&rbdata->height,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1108,dc));
            dialog.add(new StringDialogControl(113,rbdata->label,300));
            dialog.add(new StringDialogControl(206,rs89.gets(),strlen(rs89.gets())+1));
            dialog.add((DisplayDialogControl *) new ArWindowDisplayDialogControl(109,rbdata->library,_RCT("door")));
            if (dialog.process() == TRUE)
              {  v.setstring("ar::doorcategory",category);
                 v.setstring("ar::doorlibrary",rbdata->library);
                 v.setinteger("ar::fromend",rbdata->fromend);
                 v.setreal("ar::enddistance",rbdata->enddistance);
                 v.setreal("ar::doorwidth",rbdata->width);
                 v.setreal("ar::doorheight",rbdata->height);
                 v.setreal("ar::doortopframingallowance",rbdata->topframingallowance);
                 v.setreal("ar::doorbottomframingallowance",rbdata->bottomframingallowance);
                 v.setreal("ar::doorsideframingallowance",rbdata->sideframingallowance);
                 v.setstring("ar::doorlabel",rbdata->label);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected) && ! ((Wall *)((EntitySelectedEvent *) event)->getentity())->isacolumn())
         {BitMask options(32);
          GeneralList list;
            rbdata->wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            rbdata->wall->highlight();
            rbdata->xaxis = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
            rbdata->yaxis = rbdata->wall->getzaxis().cross(rbdata->xaxis).normalize();

            options.set(0);
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            rbdata->figure = new Figure(rbdata->library,rbdata->name,Point(0.0,0.0,0.0),rbdata->xaxis,rbdata->yaxis,Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,options);
            list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
            list.add(new Parameter("Width",rbdata->width));
            list.add(new Parameter("Height",rbdata->height));
            rbdata->figure->loadfigure(&list);

            if (rbdata->fromend == 0)
              *cstate = 2;
            else if (rbdata->fromend == 1)
              {Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 figure.loadfigure(&list);
                 p2 = ((EntitySelectedEvent *) event)->getp();
                 p3 = rbdata->wall->point(2);
                 p4 = rbdata->wall->point(3);
                 p5 = rbdata->wall->point(4);
                 p6 = rbdata->wall->point(5);
                 if ((rbdata->wall->nearp(p2)-p2).dot(rbdata->wall->nearp(p3)-p3) > 0.0)
                   {  length = figure.cutoutlength(0,_RCT("1"));
                      rbdata->p1 = rbdata->wall->nearp((p3 + p4) / 2.0 - (p4 - p3).normalize() * length / 2.0);
                   }
                 else
                   {  length = figure.cutoutlength(0,_RCT("4"));
                      rbdata->p1 = rbdata->wall->nearp((p5 + p6) / 2.0 - (p6 - p5).normalize() * length / 2.0);
                   }
                 *cstate = 3;
              }
            else if (rbdata->fromend == 2)
              {Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 figure.loadfigure(&list);
                 p2 = ((EntitySelectedEvent *) event)->getp();
                 p3 = rbdata->wall->point(2);
                 p4 = rbdata->wall->point(3);
                 p5 = rbdata->wall->point(4);
                 p6 = rbdata->wall->point(5);
                 dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                 if ((Line(p3,p4).nearp(p2) - p2).length() < (Line(p5,p6).nearp(p2) - p2).length())
                   {  dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                      if ((p3 - p2).length() < (p4 - p2).length())
                        rbdata->p1 = rbdata->wall->getp1() + dir * ((p3 - rbdata->wall->getp1()).dot(dir) + rbdata->enddistance);
                      else
                        rbdata->p1 = rbdata->wall->getp2() - dir * ((rbdata->wall->getp2() - p4).dot(dir) + rbdata->enddistance + rbdata->width);
                   }
                 else 
                   {  dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                      if ((p5 - p2).length() < (p6 - p2).length())
                        rbdata->p1 = rbdata->wall->getp1() + dir * ((p5 - rbdata->wall->getp1()).dot(dir) + rbdata->enddistance);
                      else
                        rbdata->p1 = rbdata->wall->getp2() - dir * ((rbdata->wall->getp2() - p6).dot(dir) + rbdata->enddistance + rbdata->width);
                   }

                 *cstate = 3;
              }
            else
              {Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 figure.loadfigure(&list);
                 rbdata->length = figure.cutoutlength(0,_RCT("1"));
                 length = figure.cutoutlength(0,_RCT("4"));
                 if (rbdata->length < length)
                   rbdata->length = length;
                 rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
                 *cstate = 4;
              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = rbdata->wall->nearp(((Coordinate3dEvent *) event)->getp());
            *cstate = 3;
         }
       else if (event->isa(Abort))
         {  rbdata->wall->unhighlight();
            *cstate = 1;
         }
       else if (event->isa(Exit))
         {  rbdata->wall->unhighlight();
            *cstate = ExitState;
         }
       break;
     case 3 :
       rubberband.end(0);
       rbdata->wall->unhighlight();
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            state1 = 1;
            if ((p2 - rbdata->p1).dot(rbdata->xaxis) > 0.0)
              state1 += (Byte)1;
            if ((p2 - rbdata->p1).dot(rbdata->yaxis) < 0.0)
              state1 += (Byte)2;
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            figure = new Figure(rbdata->library,rbdata->name,rbdata->p1,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,0,0,options);
            if (figure != 0 && figure->getdefined())
              {EntityList movedlist;
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 figure->loadfigure(&list);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(figure);
                 AddFrameAllowance(rbdata->topframingallowance,rbdata->bottomframingallowance,rbdata->sideframingallowance,figure);
                 AddTextAnnotation(rbdata->wall,rbdata->p1,rbdata->width,rbdata->height,rbdata->label);
                 rbdata->wall->add(figure);
                 movedlist.add(rbdata->wall);
                 db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
                 movedlist.destroy();
                 figure->addattributetext();
                 db.saveundo(UD_ENDBLOCK,NULL);
                 figure->draw(DM_NORMAL);
              }
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 4 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p2 = rbdata->wall->nearp(((Coordinate3dEvent *) event)->getp());
            if ((rbdata->p1 - p2).dot(rbdata->wall->getp2() - rbdata->wall->getp1()) > 0.0)
              rbdata->p1 = p2 + (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * rbdata->enddistance;
            else
              rbdata->p1 = p2 - (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * (rbdata->enddistance + rbdata->width);
            *cstate = 3;
         }
       else if (event->isa(Abort))
         {  rbdata->wall->unhighlight();
            *cstate = 1;
         }
       else if (event->isa(Exit))
         {  rbdata->wall->unhighlight();
            *cstate = ExitState;
         }
       break;
    }

  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCARCHITECT+4);
       state.selmask.entity.clearandset(wall_entity,end_list);
    }
  else if (*cstate == 2 || *cstate == 3 || *cstate == 4)
    {  rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBArchitect5,rbdata);
       if (*cstate == 4)
         cadwindow->prompt->normalprompt(NCARCHITECT+90);
       else
         cadwindow->prompt->normalprompt(NCARCHITECT+3+*cstate);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == ExitState)
    {  delete rbdata->figure;
       delete rbdata;
    }
}

struct RBArchitect6_data
  {RCCHAR library[128];
   RCCHAR name[32];
   RCCHAR label[300];
   Point p1,xaxis,yaxis;
   Wall *wall;
   double width,height,sillheight,headheight,enddistance,length,topframingallowance,bottomframingallowance,sideframingallowance;
   double frontwindowwidth,sidewindowwidth,depth;
   int state,fromend;
   Figure *figure;
  };

void RBArchitect6(int,void *data,Point *p2,View3dSurface *)
{RBArchitect6_data *rbdata = (RBArchitect6_data *) data;
 Point o,scale,rotation,p1,p3,p4,p5;
 BitMask options(32);
 GeneralList list;
  scale.setxyz(1.0,1.0,1.0);  rotation.setxyz(0.0,0.0,0.0);
  options.set(0);
  options.set(1);
  if (rbdata->state == 2)
    p1 = rbdata->wall->nearp(*p2) + rbdata->wall->getzaxis() * rbdata->height;
  else
    {  p3 = rbdata->wall->nearp(*p2);
       p4 = p3 + (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * rbdata->enddistance;
       p5 = p3 - (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * (rbdata->enddistance + rbdata->width);
       if ((rbdata->wall->nearp(rbdata->p1) - p3).dot(rbdata->wall->getp2() - rbdata->wall->getp1()) > 0.0)
         p1 = p4;
       else
         p1 = p5;
    }

  rbdata->figure->newtransform(p1,rbdata->xaxis,rbdata->yaxis,scale,rotation);
  if (rbdata->figure->getdefined()) 
    rbdata->figure->draw(DM_INVERT);

}

void architect_command6(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect6_data *rbdata = (RBArchitect6_data *) *data;
 Point scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),p1,p2,p3,p4,p5,p6,dir;
 Figure *fig;
 BitMask options(32);
 ResourceString rs9(NCARCHITECT+9),rs10(NCARCHITECT+10),rs89(NCARCHITECT+89);
 double length;
 RCCHAR category[300];
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect6_data;  *data = rbdata;
       if (rbdata != 0)
         {Dialog dialog("ArWindow_dialog");
          DialogControl *dc;
            strcpy(category,v.getstring("ar::windowcategory"));
            strcpy(rbdata->library,v.getstring("ar::windowlibrary"));
            strcpy(rbdata->name,_RCT(""));
            rbdata->sillheight = v.getreal("ar::windowsillheight");
            rbdata->width = v.getreal("ar::windowwidth");
            rbdata->height = v.getreal("ar::windowheight");
            rbdata->fromend = v.getinteger("ar::fromend");
            rbdata->enddistance = v.getreal("ar::enddistance");
            rbdata->topframingallowance = v.getreal("ar::windowtopframingallowance");
            rbdata->bottomframingallowance = v.getreal("ar::windowbottomframingallowance");
            rbdata->sideframingallowance = v.getreal("ar::windowsideframingallowance");
            strcpy(rbdata->label,v.getstring("ar::windowlabel"));
            rbdata->figure = 0;
            dialog.add(new ArCatListDialogControl(100,_RCT("window"),category));
            dialog.add(new ArListDialogControl(101,_RCT("window"),rbdata->library));
            dialog.add(dc = new RealDialogControl(102,&rbdata->sillheight,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1102,dc));
            dialog.add(new ADRadioButtonDialogControl(103,103,106,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(104,103,106,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(105,103,106,&rbdata->fromend));
            dialog.add(new ADRadioButtonDialogControl(106,103,106,&rbdata->fromend));
            dialog.add(dc = new RealDialogControl(107,&rbdata->enddistance,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1107,dc));
            dialog.add(dc = new RealDialogControl(108,&rbdata->width,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1108,dc));
            dialog.add(dc = new RealDialogControl(109,&rbdata->height,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1109,dc));
            dialog.add(new StringDialogControl(114,rbdata->label,300));
            dialog.add(new StringDialogControl(207,rs89.gets(),strlen(rs89.gets())+1));
            dialog.add((DisplayDialogControl *) new ArWindowDisplayDialogControl(110,rbdata->library,_RCT("window")));
            if (dialog.process() == TRUE)
              {  v.setstring("ar::windowcategory",category);
                 v.setstring("ar::windowlibrary",rbdata->library);
                 v.setinteger("ar::fromend",rbdata->fromend);
                 v.setreal("ar::enddistance",rbdata->enddistance);
                 v.setreal("ar::windowsillheight",rbdata->sillheight);
                 v.setreal("ar::windowwidth",rbdata->width);
                 v.setreal("ar::windowheight",rbdata->height);
                 v.setreal("ar::windowtopframingallowance",rbdata->topframingallowance);
                 v.setreal("ar::windowbottomframingallowance",rbdata->bottomframingallowance);
                 v.setreal("ar::windowsideframingallowance",rbdata->sideframingallowance);
                 v.setstring("ar::windowlabel",rbdata->label);
            
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       state.destroyselection(1);
       if (event->isa(EntitySelected) && ! ((Wall *)((EntitySelectedEvent *) event)->getentity())->isacolumn())
         {BitMask options(32);
          GeneralList list;

            rbdata->wall = (Wall *) ((EntitySelectedEvent *) event)->getentity();
            rbdata->wall->highlight();
            rbdata->xaxis = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
            rbdata->yaxis = rbdata->wall->getzaxis().cross(rbdata->xaxis).normalize();

            options.set(0);
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            rbdata->figure = new Figure(rbdata->library,rbdata->name,Point(0.0,0.0,0.0),rbdata->xaxis,rbdata->yaxis,Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,options);
            list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
            list.add(new Parameter("Width",rbdata->width));
            list.add(new Parameter("Height",rbdata->height));
            rbdata->figure->loadfigure(&list);

            if (rbdata->fromend == 0)
              *cstate = 2;
            else if (rbdata->fromend == 3)
              {GeneralList list;
                 Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 figure.loadfigure(&list);
                 rbdata->length = figure.cutoutlength(0,_RCT("1"));
                 length = figure.cutoutlength(0,_RCT("4"));
                 if (rbdata->length < length)
                   rbdata->length = length;
                 rbdata->p1 = rbdata->wall->nearp(((EntitySelectedEvent *) event)->getp());
                 *cstate = 3;
              }
            else
              {  if (rbdata->fromend == 1)
                   {GeneralList list;
                    Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                      list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                      list.add(new Parameter("Width",rbdata->width));
                      list.add(new Parameter("Height",rbdata->height));
                      figure.loadfigure(&list);
                      p2 = ((EntitySelectedEvent *) event)->getp();
                      p3 = rbdata->wall->point(2);
                      p4 = rbdata->wall->point(3);
                      p5 = rbdata->wall->point(4);
                      p6 = rbdata->wall->point(5);
                      if ((rbdata->wall->nearp(p2)-p2).dot(rbdata->wall->nearp(p3)-p3) > 0.0)
                        {  length = figure.cutoutlength(0,_RCT("1"));
                           p1 = rbdata->wall->nearp((p3 + p4) / 2.0 - (p4 - p3).normalize() * length / 2.0);
                        }
                      else
                        {  length = figure.cutoutlength(0,_RCT("4"));
                           p1 = rbdata->wall->nearp((p5 + p6) / 2.0 - (p6 - p5).normalize() * length / 2.0);
                        }
                   }
                 else
                   {GeneralList list;
                    Figure figure(rbdata->library,rbdata->name,rbdata->wall->getp1(),rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                      list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                      list.add(new Parameter("Width",rbdata->width));
                      list.add(new Parameter("Height",rbdata->height));
                      figure.loadfigure(&list);
                      p2 = ((EntitySelectedEvent *) event)->getp();
                      p3 = rbdata->wall->point(2);
                      p4 = rbdata->wall->point(3);
                      p5 = rbdata->wall->point(4);
                      p6 = rbdata->wall->point(5);
                      dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                      if ((Line(p3,p4).nearp(p2) - p2).length() < (Line(p5,p6).nearp(p2) - p2).length())
                        {  dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                           if ((p3 - p2).length() < (p4 - p2).length())
                             p1 = rbdata->wall->getp1() + dir * ((p3 - rbdata->wall->getp1()).dot(dir) + rbdata->enddistance);
                           else
                             p1 = rbdata->wall->getp2() - dir * ((rbdata->wall->getp2() - p4).dot(dir) + rbdata->enddistance + rbdata->width);
                        }
                      else 
                        {  dir = (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize();
                           if ((p5 - p2).length() < (p6 - p2).length())
                             p1 = rbdata->wall->getp1() + dir * ((p5 - rbdata->wall->getp1()).dot(dir) + rbdata->enddistance);
                           else
                             p1 = rbdata->wall->getp2() - dir * ((rbdata->wall->getp2() - p6).dot(dir) + rbdata->enddistance + rbdata->width);
                        }
                   }
                 rbdata->wall->unhighlight();
                 p1 += rbdata->wall->getzaxis() * (rbdata->sillheight - rbdata->height);
                 options.set(1);
                 options.set(FigureOriginalStyle);
                 options.set(FigureOriginalWeight);
                 fig = new Figure(rbdata->library,rbdata->name,p1,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                 if (fig != 0 && fig->getdefined())
                   {EntityList movedlist;
                    GeneralList list;
                     list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                     list.add(new Parameter("Width",rbdata->width));
                     list.add(new Parameter("Height",rbdata->height));
                     fig->loadfigure(&list);
                     db.saveundo(UD_STARTBLOCK,NULL);
                     db.geometry.add(fig);
                     AddFrameAllowance(rbdata->topframingallowance,rbdata->bottomframingallowance,rbdata->sideframingallowance,fig);
                     AddTextAnnotation(rbdata->wall,p1,rbdata->width,rbdata->height,rbdata->label);
                     rbdata->wall->add(fig);
                     movedlist.add(rbdata->wall);
                     db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
                     movedlist.destroy();
                     fig->addattributetext();
                     db.saveundo(UD_ENDBLOCK,NULL);
                     fig->draw(DM_NORMAL);
                   }
                 else
                   delete fig;
              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       rbdata->wall->unhighlight();
       if (event->isa(Coordinate3d))
         {  p2 = rbdata->wall->nearp(((Coordinate3dEvent *) event)->getp()) + rbdata->wall->getzaxis() * (rbdata->sillheight - rbdata->height);
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            fig = new Figure(rbdata->library,rbdata->name,p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
            if (fig != 0 && fig->getdefined())
              {EntityList movedlist;
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 fig->loadfigure(&list);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(fig);
                 AddFrameAllowance(rbdata->topframingallowance,rbdata->bottomframingallowance,rbdata->sideframingallowance,fig);
                 AddTextAnnotation(rbdata->wall,p2,rbdata->width,rbdata->height,rbdata->label);
                 rbdata->wall->add(fig);
                 movedlist.add(rbdata->wall);
                 db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
                 movedlist.destroy();
                 fig->addattributetext();
                 db.saveundo(UD_ENDBLOCK,NULL);
                 fig->draw(DM_NORMAL);
              }
            else
              delete fig;
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       rubberband.end(0);
       rbdata->wall->unhighlight();
       if (event->isa(Coordinate3d))
         {  p2 = rbdata->wall->nearp(((Coordinate3dEvent *) event)->getp());
            if ((rbdata->p1 - p2).dot(rbdata->wall->getp2() - rbdata->wall->getp1()) > 0.0)
              p2 = p2 + (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * rbdata->enddistance;
            else
              p2 = p2 - (rbdata->wall->getp2() - rbdata->wall->getp1()).normalize() * (rbdata->enddistance + rbdata->width);
            p2 += rbdata->wall->getzaxis() * (rbdata->sillheight - rbdata->height);
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            fig = new Figure(rbdata->library,rbdata->name,p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
            if (fig != 0 && fig->getdefined())
              {EntityList movedlist;
               GeneralList list;
                 list.add(new Parameter("Thickness",rbdata->wall->gettotalwidth()));
                 list.add(new Parameter("Width",rbdata->width));
                 list.add(new Parameter("Height",rbdata->height));
                 fig->loadfigure(&list);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(fig);
                 AddFrameAllowance(rbdata->topframingallowance,rbdata->bottomframingallowance,rbdata->sideframingallowance,fig);
                 AddTextAnnotation(rbdata->wall,p2,rbdata->width,rbdata->height,rbdata->label);
                 rbdata->wall->add(fig);
                 movedlist.add(rbdata->wall);
                 db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
                 movedlist.destroy();
                 fig->addattributetext();
                 db.saveundo(UD_ENDBLOCK,NULL);
                 fig->draw(DM_NORMAL);
              }
            else
              delete fig;
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCARCHITECT+7);
       state.selmask.entity.clearandset(wall_entity,end_list);
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBArchitect6,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+8);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 3)
    {  rbdata->state = 3;
       rubberband.begin(0);
       rubberband.add(RBArchitect6,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+90);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == ExitState)
    {  delete rbdata->figure;
       delete rbdata;
    }
}

class ArPolygon
  {public:
     EntityList list;
     Point origin,xaxis,yaxis,zaxis,start,end;
  };

class ArVertex
  {public:
     ArPolygon *poly1,*poly2;
     Point p,dir;
     double length;
     int original;
  };

class RNListDialogControl : public ListDialogControl
  {public:
     RNListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void RNListDialogControl::load(Dialog *dialog)
{RCCHAR name[300],key[300];
 int status;
  ListDialogControl::load(dialog);
  status = v.getfirststring(_RCT("ar::Cost::Roof::"),key,name);
  while (status)
    {  ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,key+16);
       status = v.getnextstring(_RCT("ar::Cost::Roof::"),key,name);
    }
}

void visiblecb(Entity *e,void *v)
{ if (e != 0)
    e->setvisible(*((BitMask *)v));
}

class ARButtonDialogControl : public ButtonDialogControl
  {public:
     ARButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int ARButtonDialogControl::select(Dialog *dialog)
{ dialog->store();
  return id;
}

class ARRadioButtonDialogControl : public RadioButtonDialogControl
  {public:
    ARRadioButtonDialogControl(int id,int id1,int id2,int *value) : RadioButtonDialogControl(id,id1,id2,value)  { }
    void load(Dialog *dialog);
    int select(Dialog *dialog);
  };


void ARRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1107),*value == 1);
}

int ARRadioButtonDialogControl::select(Dialog *dialog)
{int status,value;
  status = RadioButtonDialogControl::select(dialog);
  value = 0;
  currentvalue(dialog,&value);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1107),value == 1);
  return status;
}

void ResetToHipEnd(Line *ridgeline,Line **hipline1,Line **hipline2,Point p1,Point p2,Point p3)
{
    /*
    int j,isdutchgable;
 Point p4,p5,p6,p7;
 Plane *plane,*plane1,*plane2;
 Line *dgline1,*dgline2,*dgline3;
 Entity *e;
 EntityList l;
 double pitch;

  p6 = p2;  p7 = p3;
 
  plane = plane1 = plane2 = 0;
  dgline1 = dgline2 = 0;
  isdutchgable = 0;
  l = (*hipline1)->getalist();
  for (l.start() ; (e = l.next()) != 0 ; )
    if ((*hipline2)->getalist().inlist(e))
      plane = (Plane *) e;

  l = ridgeline->getalist();
  for (l.start() ; (e = l.next()) != 0 ; )
    if (e->isa(plane_entity))
      {  if ((*hipline1)->getalist().inlist(e))
           plane1 = (Plane *) e;
         if ((*hipline2)->getalist().inlist(e))
           plane2 = (Plane *) e;
      }

  if ((*hipline1)->getlayer() == DUTCHGABLE_LAYER)
    {//  Find the normal hip line connected to this entity
       if ((p1 - (*hipline1)->getp1()).length() < (p1 - (*hipline1)->getp2()).length())
         p2 = (*hipline1)->getp2();
       else
         p2 = (*hipline1)->getp1();
 
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(line_entity) && e->getlayer() == HIP_LAYER && (((((Line *)e)->getp1() - p2).length() < db.getptoler() || (((Line *)e)->getp2() - p2).length() < db.getptoler())))
           {  dgline1 = *hipline1; 
              *hipline1 = (Line *)e;
              isdutchgable++;
              break;
           }
    
    }

  if ((*hipline2)->getlayer() == DUTCHGABLE_LAYER)
    {//  Find the normal hip line connected to this entity
       if ((p2 - (*hipline2)->getp1()).length() < (p2 - (*hipline2)->getp2()).length())
         p3 = (*hipline2)->getp2();
       else
         p3 = (*hipline2)->getp1();
 
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(line_entity) && e->getlayer() == HIP_LAYER && (((((Line *)e)->getp1() - p3).length() < db.getptoler() || (((Line *)e)->getp2() - p3).length() < db.getptoler())))
           {  dgline2 = *hipline2; 
              *hipline2 = (Line *)e;
              isdutchgable++;
              break;
           }
    }
  else
    {  p2 = p1;
       p3 = p1;
    }

  if (isdutchgable && plane != 0 && plane1 != 0 && plane2 != 0)
    {//  Delete the dutch gable entities - The three extra lines and the plane
       l = plane->getlist().copy();
       for (l.start() ; (e = l.next()) != 0 ; )
         if (e != dgline1 && e != dgline2)
           dgline3 = (Line *) e;

       if (plane1 != 0 && dgline1 != 0)
         plane1->DeleteEdge(dgline1);
       if (plane2 != 0 && dgline2 != 0)
         plane2->DeleteEdge(dgline2);

       l = (*hipline1)->getalist();
       for (l.start() ; (e = l.next()) != 0 ; )
         if (e->isa(plane_entity) && e != plane1 && e != plane2)
           ((Plane *)e)->DeleteEdge(dgline3);

       l = plane->getlist().copy();

       db.geometry.del(plane);

       for (l.start() ; (e = l.next()) != 0 ; )
         db.geometry.del(e);

    }


  InfinitePlane iptmp((*hipline1)->direction(0.0).cross((*hipline2)->direction(0.0)),p6);
  Intersect i(ridgeline,&iptmp);
  if (i.nintersections() > 0)
    {EntityList *l;

       p4 = ((PointE *)i.intersection(0))->getp();
       p5 = p4;  p5.z = p7.z;  
       pitch = atan2(p4.z - p7.z,(p5 - p7).length());

       //  Erase the entities from the screen
       for (j = 0 ; j < 3 ; j++)
         {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &(*hipline1)->getalist() : &(*hipline2)->getalist());
            for (l->start() ; (e = l->next()) != 0 ; )
              ;//e->draw(DM_ERASE);
         }
      
       ridgeline->draw(DM_ERASE);
       (*hipline1)->draw(DM_ERASE);
       (*hipline2)->draw(DM_ERASE);

       ridgeline->movehandlepoint(0,0,((ridgeline->getp1() - p1).length() > db.getptoler()) + 1,p4);
       (*hipline1)->movehandlepoint(0,0,(((*hipline1)->getp1() - p2).length() > db.getptoler()) + 1,p4);
       (*hipline2)->movehandlepoint(0,0,(((*hipline2)->getp1() - p3).length() > db.getptoler()) + 1,p4);
                         
       //  Recalculate the plane equations for the new roof shape and redraw the entities
       for (j = 0 ; j < 3 ; j++)
         {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &(*hipline1)->getalist() : &(*hipline2)->getalist());
            for (l->start() ; (e = l->next()) != 0 ; )
              if (e->isa(plane_entity))
                {  ((Plane *)e)->ReCalculatePlane();
                   cadwindow->invalidatedisplaylist(e);
                   e->draw(DM_NORMAL);
                } 
         }

       ridgeline->draw(DM_NORMAL);
       (*hipline1)->draw(DM_NORMAL);
       (*hipline2)->draw(DM_NORMAL);

    }
    */
} 


void DeleteRoofPlan(double *zdepth,BitMask *visible)
{Entity *e;
 View *vw;
 int i;

  *zdepth = 0.0;
  visible->clearall();
  for (i = 0 ; i < 32 ; i++)
    if ((vw = db.views3d.match(i)) != 0 && ! vw->getvisibility())
      visible->set(i);

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(line_entity) && e->getlayer() == ROOFPLAN_LAYER)
      {  db.geometry.del(e);
         db.geometry.start();
      }
   
}

void DeleteTrussPlan(void)
{Entity *e;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(line_entity) && e->getlayer() == TRUSSPLAN_LAYER)
      {  db.geometry.del(e);
         db.geometry.start();
      }
    
}

void CopyToRoofPlan(double zdepth,BitMask *visible)
{Entity *e;
 Line *line;
 Point p1,p2,p3,p4;

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(line_entity) && (e->getlayer() >= EAVE_LAYER && e->getlayer() <= DUTCHGABLE_LAYER && e->getlayer() != PITCHING_LAYER || e->getlayer() == DUTCHGABLEOVER_LAYER))
      {  p1 = ((Line *)e)->getp1();  p1.z = zdepth;
         p2 = ((Line *)e)->getp2();  p2.z = zdepth;
         line = new Line(p1,p2);
         line->EntityHeader::change(line->getcolour(),ROOFPLAN_LAYER,2,line->getweight());
         line->setvisible(*visible);
         db.geometry.add(line);
      }
}

void CopyToTrussPlan(void)
{Entity *e;
 Line *line;
 Point p1,p2,p3,p4;
 double d1,d2;
 int i;

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(line_entity) && (e->getlayer() == PITCHING_LAYER || e->getlayer() == BEAM_LAYER))
      {  p1 = ((Line *)e)->getp1();  p1.z = 0.0;
         p2 = ((Line *)e)->getp2();  p2.z = 0.0;
         line = new Line(p1,p2);
         line->EntityHeader::change(line->getcolour(),TRUSSPLAN_LAYER,e->getlayer() == PITCHING_LAYER ? 2 : line->getstyle(),line->getweight());
         //line->setvisible(*visible);
         db.geometry.add(line);
      }
    else if (e->isa(curve_entity) && e->getlayer() >= FULLTRUSS_LAYER && e->getlayer() <= TRUNCATEDTRUSS_LAYER && e->getlayer() != TRUSSPLAN_LAYER)
      {  p1 = ((Curve *)e)->point(0);  p1.z = 0.0;
         p2 = ((Curve *)e)->point(1);  p2.z = 0.0;
         d1 = 0.0;
         d2 = (p2 - p1).dot(p2 - p1);

         for (i = 0 ; i < ((Curve *)e)->npoints() ; i++)
           {  p3 = ((Curve *)e)->point(i);
              p3.z = 0.0;
              if ((p2 - p1).dot(p3 - p1) < d1)
                {  d1 = (p2 - p1).dot(p3 - p1);
                   p1 = p3;
                }
              if ((p2 - p1).dot(p3 - p1) > d2)
                {  d2 = (p2 - p1).dot(p3 - p1);
                   p2 = p3;
                }
           }
         line = new Line(p1,p2);
         line->EntityHeader::change(line->getcolour(),TRUSSPLAN_LAYER,line->getstyle(),line->getweight());
         //line->setvisible(*visible);
         db.geometry.add(line);
      }

}

void CopyRoofToTiles(void)
{Transform ident,up;
 Entity *e;
 Plane *plane;
 EntityList elist;

  //  Delete existing tile planes
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(plane_entity) && e->getlayer() == TILES_LAYER)
      elist.add(e);
  for (elist.start() ; (e = elist.next()) != 0 ; )
    db.geometry.del(e);
  
  elist.destroy();

  //  Delete existing tile lines
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getlayer() == TILES_LAYER)
      elist.add(e);
  for (elist.start() ; (e = elist.next()) != 0 ; )
    db.geometry.del(e);
  elist.destroy();

  //  Copy the roof layer up to the tile layer.
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(plane_entity) && e->getlayer() == ROOF_LAYER)
      {  plane = (Plane *) e;
         up.translate(0.0,0.0,200.0);
         plane->setcopye(0);
         elist = plane->getlist();
         for (elist.start() ; (e = elist.next()) != 0 ; )
           e->setcopye(0);
         plane->clone(up);
         if (plane->getcopye() != 0)
           {  plane->getcopye()->EntityHeader::change(plane->getcolour(),TILES_LAYER,plane->getstyle(),plane->getweight());
              elist = ((Plane *)plane->getcopye())->getlist();
              for (elist.start() ; (e = elist.next()) != 0 ; )
                e->EntityHeader::change(e->getcolour(),TILES_LAYER,e->getstyle(),e->getweight());
              db.geometry.add(plane->getcopye());
           }
      }

}

void CreateGutters(void)
{
    /*
    Line *eaveline,*gutters[11];
 Plane *roofplane;
 Entity *e;
 EntityList list,elist;
 Point p1,p2,p3,p4,p5,p6;
 Transform up;
 BitMask o(32);


  //  Delete the existing gutters - delete the planes first and then the lines
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(plane_entity) && e->getlayer() == FACIA_LAYER)
      {  db.geometry.del(e);  
         db.geometry.start();
      } 
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getlayer() == FACIA_LAYER)
      {  db.geometry.del(e);  
         db.geometry.start();
      } 

  db.geometry.clearstatus();

  for (;;)
    {  
       //  Find the next unprocessed eave line.
       eaveline = 0;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->getstatus() == 0 && e->isa(line_entity)  && e->getlayer() == EAVE_LAYER && fabs(((Line *)e)->direction(0.0).z) < 0.01)
           {  eaveline = (Line *) e;
              eaveline->setstatus(1);
              break;    
           }
       if (eaveline == 0)
         break;

       //  Find a connect roof plane 
       roofplane = 0;
       list = eaveline->getalist();
       for (list.start() ; (e = list.next()) != 0 ; )
         if (e->isa(plane_entity) && e->getlayer() == ROOF_LAYER)
           {  roofplane = (Plane *)e;
              break;
           }
                 
       if (roofplane == 0)
         continue;

       p1 = eaveline->getp1();
       p2 = eaveline->getp2();

       //  Use the roof plane to determine the side to place the gutter
       Intersect i1(roofplane,&Line(p1+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize(),p1+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize() + Point(0.0,0.0,1.0)));
       if (i1.nintersections() > 0)
         p5 = ((PointE *)i1.intersection(0))->getp();
       Intersect i2(roofplane,&Line(p1+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize(),p1+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize() + Point(0.0,0.0,1.0)));
       if (i2.nintersections() > 0)
         p6 = ((PointE *)i2.intersection(0))->getp();
       if (p5.z > p6.z)
         {  p5 = p1;  p1 = p2;  p2 = p5;
         }

       //  Find connected eave lines to determine the angle of the gutters 
       p3 = p1;
       p4 = p2;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(line_entity) && e->getlayer() == EAVE_LAYER && e != eaveline && fabs(((Line *)e)->direction(0.0).z) < 0.01)
           {  if ((p1 - ((Line *)e)->getp1()).length() < 1.0)
                p3 = ((Line *)e)->getp2();
              if ((p1 - ((Line *)e)->getp2()).length() < 1.0)
                p3 = ((Line *)e)->getp1();
              if ((p2 - ((Line *)e)->getp1()).length() < 1.0)
                p4 = ((Line *)e)->getp2();
              if ((p2 - ((Line *)e)->getp2()).length() < 1.0)
                p4 = ((Line *)e)->getp1();
           }

       //  Now calculate the angle of the gutters
       gutters[0] = (Line *) eaveline;

       //  Gutters and Facia
       gutters[1] = new Line(p1+Point(0.0,0.0,100.0),p2+Point(0.0,0.0,100.0));
       gutters[1]->EntityHeader::change(gutters[1]->getcolour(),FACIA_LAYER,gutters[1]->getstyle(),gutters[1]->getweight());
       db.geometry.add(gutters[1]);

       gutters[2] = new Line(p1+Point(0.0,0.0,200.0),p2+Point(0.0,0.0,200.0));
       gutters[2]->EntityHeader::change(gutters[2]->getcolour(),FACIA_LAYER,gutters[2]->getstyle(),gutters[2]->getweight());
       db.geometry.add(gutters[2]);

       Line line1(p1+Point(0.0,0.0,100.0)+(p2 - p1).cross(Point(0.0,0.0,1.0)).normalize()*100.0,p2+Point(0.0,0.0,100.0)+(p2 - p1).cross(Point(0.0,0.0,1.0)).normalize()*100.0);
       Line line2(p1+Point(0.0,0.0,100.0),p1+Point(0.0,0.0,100.0)+((p1 - p2).cross(Point(0.0,0.0,1.0)).normalize() + (p3 - p1).cross(Point(0.0,0.0,1.0)).normalize()));
       Line line3(p2+Point(0.0,0.0,100.0),p2+Point(0.0,0.0,100.0)+((p2 - p1).cross(Point(0.0,0.0,1.0)).normalize() + (p4 - p2).cross(Point(0.0,0.0,1.0)).normalize()));
       Intersect i3(&line1,&line2);
       Intersect i4(&line1,&line3);

       if (i3.nintersections() > 0 && i4.nintersections() > 0)
         {  p5 = ((PointE *)i3.intersection(0))->getp();
            p6 = ((PointE *)i4.intersection(0))->getp();
             
            gutters[3] = new Line(p5,p6);
            gutters[3]->EntityHeader::change(gutters[3]->getcolour(),FACIA_LAYER,gutters[3]->getstyle(),gutters[3]->getweight());
            db.geometry.add(gutters[3]);

            gutters[4] = new Line(p5+Point(0.0,0.0,100.0),p6+Point(0.0,0.0,100.0));
            gutters[4]->EntityHeader::change(gutters[4]->getcolour(),FACIA_LAYER,gutters[4]->getstyle(),gutters[4]->getweight());
            db.geometry.add(gutters[4]);

            gutters[5] = new Line(p1,gutters[2]->getp1());
            gutters[5]->EntityHeader::change(gutters[5]->getcolour(),FACIA_LAYER,gutters[5]->getstyle(),gutters[5]->getweight());
            db.geometry.add(gutters[5]);

            gutters[6] = new Line(gutters[1]->getp1(),gutters[3]->getp1());
            gutters[6]->EntityHeader::change(gutters[6]->getcolour(),FACIA_LAYER,gutters[6]->getstyle(),gutters[6]->getweight());
            db.geometry.add(gutters[6]);

            gutters[7] = new Line(gutters[3]->getp1(),gutters[4]->getp1());
            gutters[7]->EntityHeader::change(gutters[7]->getcolour(),FACIA_LAYER,gutters[7]->getstyle(),gutters[7]->getweight());
            db.geometry.add(gutters[7]);

            gutters[8] = new Line(p2,gutters[2]->getp2());
            gutters[8]->EntityHeader::change(gutters[8]->getcolour(),FACIA_LAYER,gutters[8]->getstyle(),gutters[8]->getweight());
            db.geometry.add(gutters[8]);

            gutters[9] = new Line(gutters[1]->getp2(),gutters[3]->getp2());
            gutters[9]->EntityHeader::change(gutters[9]->getcolour(),FACIA_LAYER,gutters[9]->getstyle(),gutters[9]->getweight());
            db.geometry.add(gutters[9]);

            gutters[10] = new Line(gutters[3]->getp2(),gutters[4]->getp2());
            gutters[10]->EntityHeader::change(gutters[10]->getcolour(),FACIA_LAYER,gutters[10]->getstyle(),gutters[10]->getweight());
            db.geometry.add(gutters[10]);

            list = elist;
            list.add(gutters[1]);
            list.add(gutters[6]);
            list.add(gutters[3]);
            list.add(gutters[9]);
            Plane *plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            plane->EntityHeader::change(plane->getcolour(),FACIA_LAYER,plane->getstyle(),plane->getweight());
            if (plane != 0 && plane->getdefined())
              {  //plane->setvisible(visible1);
                 db.geometry.add(plane);
              }
            list = elist; 
            list.add(gutters[3]);
            list.add(gutters[7]);
            list.add(gutters[4]);
            list.add(gutters[10]);
            plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            plane->EntityHeader::change(plane->getcolour(),FACIA_LAYER,plane->getstyle(),plane->getweight());
            if (plane != 0 && plane->getdefined())
              {  //plane->setvisible(visible1);
                 db.geometry.add(plane);
              }
            list = elist; 
            list.add(gutters[0]);
            list.add(gutters[5]);
            list.add(gutters[2]);
            list.add(gutters[8]);
            plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            plane->EntityHeader::change(plane->getcolour(),FACIA_LAYER,plane->getstyle(),plane->getweight());
            if (plane != 0 && plane->getdefined())
              {  //plane->setvisible(visible1);
                 db.geometry.add(plane);
              }


         } 
    }

*/
}

struct RBArchitect7_data
  {Point p1,p2,p3,p4,dir;
   Plane *roofplane,*roofplane1,*roofplane2;
   Line *eaveline,*eaveline1,*eaveline2,*hipline1,*hipline2,*ridgeline;
   int state;
   double ridgelength;
  };

void RBArchitect7(int,void *data,Point *p2,View3dSurface *)
{RBArchitect7_data *rbdata = (RBArchitect7_data *) data;
 Point p3,p4;
  if (rbdata->state == 101)
    {  p3 = Line(rbdata->p1,rbdata->p2).nearp(*p2);  p3.z = 0.0;
       p4 = rbdata->p2;  p4.z = 0.0;
       Line line(p3,p4);
       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 102)
    {  p3 = rbdata->p1;  p3.z = 0.0;
       p4 = Line(rbdata->p1,rbdata->p2).nearp(*p2);  p4.z = 0.0;
       Line line(p3,p4);
       line.draw(DM_INVERT);
    }
}

void RBArchitect7a(int dm,void *data,Point *p2,View3dSurface *)
{RBArchitect7_data *rbdata = (RBArchitect7_data *) data;
 Point p3,p4,p5,p6;

  p3 = rbdata->p1 + rbdata->dir * rbdata->ridgelength;
  p4 = rbdata->p1 - rbdata->dir * rbdata->ridgelength;
  p5 = rbdata->p1 + rbdata->dir.cross(Point(0.0,0.0,1.0)) * rbdata->ridgelength;
  p6 = rbdata->p1 - rbdata->dir.cross(Point(0.0,0.0,1.0)) * rbdata->ridgelength;

  if ((*p2 - p4).length() < (*p2 - p3).length())
    p3 = p4;
  if ((*p2 - p5).length() < (*p2 - p3).length())
    p3 = p5;
    rbdata->dir = rbdata->dir.cross(Point(0.0,0.0,1.0));
  if ((*p2 - p6).length() < (*p2 - p3).length())
    p3 = p6;

  p5 = rbdata->p1;  p5.z = 0.0;  p6 = p3;  p6.z = 0.0;
  Line line(p5,p6);
  line.draw(DM_INVERT);

  rbdata->p3 = p3;

}

void architect_command7(int *cstate,HighLevelEvent *event,void **data)
{
    /*
    RBArchitect7_data *rbdata = (RBArchitect7_data *) *data;
 EntityList wlist,llist1,llist2,olist1,olist2,olist3,list,elist;
 GeneralList plist,plist1,plist2,vlist1,vlist2,vlistnew;
 Entity *e;
 int i,n;
 Line *line1,*line2,*line3,*line4,*line5,*line6,*ridgeline,*hipline1,*hipline2,*line;
 ArPolygon *polygon,*poly,*plast;
 ArVertex *vertex,*v1,*v2;
 Point p(1.0E10,2.3E10,0.0),origin,xaxis,yaxis,zaxis(0.0,0.0,1.0),p1,p2,p3,p4,p5,p6,ridgedirection,za;
 BitMask options(32);
 ResourceString rs11(NCARCHITECT+11),rs12(NCARCHITECT+12);
 ResourceString rs13(NCARCHITECT+13),rs14(NCARCHITECT+14);
 ResourceString rs103(NCARCHITECT+103),rs0(NCVIEWA),rs1(NCVIEWA+1),rs2(NCVIEWA+2);
 ResourceString rs3(NCVIEWA+3),rs4(NCVIEWA+4),rs5(NCVIEWA+5),rs6(NCVIEWA+6);
 ResourceString rs7(NCVIEWA+7),rs8(NCVIEWA+8);
 double pitch,D;
 Transform t;
 int nlines,result,roofstyle;
 double sign,eaves,pitchingoffset,zdepth,gablesetback,eavecutback,distancefromend,ridgelength;
 BitMask o(32),visible1(MaxViews),visible2(MaxViews);
 Dialog dialog("ArRoof_dialog");
 DialogControl *dc;
 RCCHAR name[300],*at;
 Transform ident,up;
 typedef Line *LineP;


  LoadLayerNames();

  strcpy(name,v.getstring("ar::roofname"));
  pitch = v.getreal("ar::roofpitch");
  eaves = v.getreal("ar::roofeaves");
  gablesetback = v.getreal("ar::gablesetback");
  eavecutback = v.getreal("ar::eavecutback");
  if (gablesetback == 0.0) 
    gablesetback = 1500.0;
  distancefromend = v.getreal("ar::dutchgabledistancefromend");
  ridgelength = v.getreal("ar::dutchgableridgelength");

  roofstyle = v.getinteger("ar::roofstyle");
  dialog.add(new RNListDialogControl(100,name,sizeof(name)));
  dialog.add(dc = new RealDialogControl(101,&pitch,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&eaves,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));

  dialog.add(new ARRadioButtonDialogControl(104,104,106,&roofstyle));
  dialog.add(new ARRadioButtonDialogControl(105,104,106,&roofstyle));
  dialog.add(new ARRadioButtonDialogControl(106,104,106,&roofstyle));
  dialog.add(dc = new RealDialogControl(107,&gablesetback,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(new ARButtonDialogControl(108));
  dialog.add(dc = new RealDialogControl(109,&eavecutback,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(new ARButtonDialogControl(110));
  dialog.add(dc = new RealDialogControl(111,&distancefromend,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(dc = new RealDialogControl(112,&ridgelength,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(new ARButtonDialogControl(113));

  result = 0;
  if (*cstate == InitialState)
    result = dialog.process();
  if (*cstate != InitialState && *cstate < 100 || result == 108)
    {  v.setreal("ar::gablesetback",gablesetback);
       v.setinteger("ar::roofstyle",roofstyle);
       switch (*cstate)
         {case InitialState :
            *cstate = 1;
            break;
          case 1 :
            if (event->isa(EntitySelected))
              {  state.destroyselection(1);
                 line = (Line *)((EntitySelectedEvent *) event)->getentity();
                 if (line->getlayer() == ROOFPLAN_LAYER || line->getlayer() == TILES_LAYER)
                   {//  Find the corresponding 3D ridge line
                      for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                        if (e->isa(line_entity) && e->getlayer() != ROOFPLAN_LAYER && e->getlayer() != FACIA_LAYER && e->getlayer() != TILES_LAYER)
                          {  p1 = ((Line *)e)->getp1(); 
                             p2 = ((Line *)e)->getp2(); 
                             p1.z = line->getp1().z;
                             p2.z = line->getp2().z;
                             if ((p1-line->getp1()).length() + (p2-line->getp2()).length() < db.getptoler())
                               {  line = (Line *) e;
                                  break;
                               }
                          }
                   }

                 if (line->getlayer() != RIDGE_LAYER)
                   {  cadwindow->MessageBox("Select near the end of the ridge line to modify","Modify Roof",MB_OK);
                      *cstate = ExitState;
                      break;
                   }

                 if (((EntitySelectedEvent *) event)->getendselected() == 1)
                   {  p1 = line->start();
                      ridgedirection = -((LinearEntity *)line)->direction(0.0);
                   }  
                 else
                   {  p1 = line->end();
                      ridgedirection = ((LinearEntity *)line)->direction(1.0);
                   }  

                 for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                   if (e->isa(line_entity) && ((((Line *)e)->getp1() - p1).length() < db.getptoler() || (((Line *)e)->getp2() - p1).length() < db.getptoler()))
                     list.add(e);

                 if (list.length() != 3)
                   cadwindow->MessageBox("This cannot be modified","Modify roof",MB_OK);
                 else
                   {  list.start();
                      line1 = (Line *) list.next();
                      line2 = (Line *) list.next();
                      line3 = (Line *) list.next();
                      ridgeline = hipline1 = hipline2 = 0;
                      if (fabs((line1->getp1() - line1->getp2()).z) < db.getptoler())
                        {  ridgeline = line1;  hipline1 = line2;  hipline2 = line3;
                        }
                      else if (fabs((line2->getp1() - line2->getp2()).z) < db.getptoler())
                        {  ridgeline = line2;  hipline1 = line1;  hipline2 = line3;
                        }
                      else if (fabs((line3->getp1() - line3->getp2()).z) < db.getptoler())
                        {  ridgeline = line3;  hipline1 = line1;  hipline2 = line2;
                        }

                      if (ridgeline == 0)
                        cadwindow->MessageBox("The ridge line could not be found","Modify roof",MB_OK);
                      else
                        {  if ((hipline1->getp1() - p1).length() < db.getptoler())
                             p2 = hipline1->getp2();
                           else
                             p2 = hipline1->getp1();
                           if ((hipline2->getp1() - p1).length() < db.getptoler())
                             p3 = hipline2->getp2();
                           else
                             p3 = hipline2->getp1();

                          db.saveundo(UD_STARTBLOCK,NULL);

                          ResetToHipEnd(ridgeline,&hipline1,&hipline2,p1,p2,p3);

                           if (((EntitySelectedEvent *) event)->getendselected() == 1)
                             p1 = ridgeline->start();
                           else
                             p1 = ridgeline->end();

                           if ((hipline1->getp1() - p1).length() < db.getptoler())
                             p2 = hipline1->getp2();
                           else
                             p2 = hipline1->getp1();
                           if ((hipline2->getp1() - p1).length() < db.getptoler())
                             p3 = hipline2->getp2();
                           else
                             p3 = hipline2->getp1();

                           if (roofstyle == 0)  //  Hip end
                             {//  Nothing - always reset back to a hip end first
                             }
                           else if (roofstyle == 1)
                             {  p4 = p1 + ridgedirection * gablesetback;
                                Intersect i1(hipline1,&InfinitePlane(((p3-p1).normalize() - (p2-p1).normalize()).cross(Point(0.0,0.0,1.0)),p4));
                                Intersect i2(hipline2,&InfinitePlane(((p3-p1).normalize() - (p2-p1).normalize()).cross(Point(0.0,0.0,1.0)),p4));
                                if (i1.nintersections() > 0 && i2.nintersections() > 0)
                                  {EntityList list;
                                   Loops loops(1);
                                   Point o,za;
                                   EntityList *l;
                                   BitMask options(32);
                                   int j;

                                     p5 = ((PointE *)i1.intersection(0))->getp();
                                     p6 = ((PointE *)i2.intersection(0))->getp();

                                     line1 = new Line(p4,p5);
                                     line2 = new Line(p5,p6);
                                     line3 = new Line(p6,p4);

                                     line1->EntityHeader::change(line1->getcolour(),DUTCHGABLE_LAYER,line1->getstyle(),line1->getweight());
                                     line2->EntityHeader::change(line2->getcolour(),DUTCHGABLEBASE_LAYER,line2->getstyle(),line2->getweight());
                                     line3->EntityHeader::change(line3->getcolour(),DUTCHGABLE_LAYER,line3->getstyle(),line3->getweight());

                                     db.geometry.add(line1);
                                     db.geometry.add(line2);
                                     db.geometry.add(line3);

                                     list.add(line1);
                                     list.add(line2);
                                     list.add(line3);

                                     loops.create(&list);
                                     if (! loops.getloops().empty())
                                       {  Plane *plane = new Plane(loops.getloops(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
                                          plane->EntityHeader::change(plane->getcolour(),ROOF_LAYER,plane->getstyle(),plane->getweight());
                                          if (plane != 0 && plane->getdefined())
                                            {  db.geometry.add(plane);
                                               if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                                                 {  strcpy(at,"ar::Cost::Roof::");
                                                    strcat(at,name);
                                                    plane->addat(at);
                                                    plane->setattribute(at);
                                                    db.saveundo(UD_INSERTATT,plane);
                                                 }
                                            }
                                       }


                                     //  Erase the entities from the screen
                                     for (j = 0 ; j < 3 ; j++)
                                       {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &hipline1->getalist() : &hipline2->getalist());
                                          for (l->start() ; (e = l->next()) != 0 ; )
                                            ;//e->draw(DM_ERASE);
                                       }
                                     ridgeline->draw(DM_ERASE);
                                     hipline1->draw(DM_ERASE);
                                     hipline2->draw(DM_ERASE);


                                     //  Add the three edges to the existing roof planes.
                                     for (j = 0 ; j < 3 ; j++)
                                       {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &hipline1->getalist() : &hipline2->getalist());
                                          for (l->start() ; (e = l->next()) != 0 ; )
                                            if (e->isa(plane_entity))
                                              {Transform t;
                                                 o = ((Plane *)e)->getorigin();
                                                 za = ((Plane *)e)->getxaxis().cross(((Plane *)e)->getyaxis());
                                                 if (fabs((line1->getp1() - o).dot(za)) < db.getptoler() && fabs((line1->getp2() - o).dot(za)) < db.getptoler())
                                                   ((Plane *)e)->AddEdge(p1,line1);
                                                 if (fabs((line2->getp1() - o).dot(za)) < db.getptoler() && fabs((line2->getp2() - o).dot(za)) < db.getptoler())
                                                   ((Plane *)e)->AddEdge(p1,line2);
                                                 if (fabs((line3->getp1() - o).dot(za)) < db.getptoler() && fabs((line3->getp2() - o).dot(za)) < db.getptoler())
                                                   ((Plane *)e)->AddEdge(p1,line3);

                                              }
                                       }

                                     //  Move the ends of the existing roof lines   
                                     ridgeline->movehandlepoint(0,0,((ridgeline->getp1() - p1).length() > db.getptoler()) + 1,p4);
                                     hipline1->movehandlepoint(0,0,((hipline1->getp1() - p1).length() > db.getptoler()) + 1,p5);
                                     hipline2->movehandlepoint(0,0,((hipline2->getp1() - p1).length() > db.getptoler()) + 1,p6);


                                     //  Recalculate the plane equations for the new roof shape and redraw the entities
                                     for (j = 0 ; j < 3 ; j++)
                                       {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &hipline1->getalist() : &hipline2->getalist());
                                          for (l->start() ; (e = l->next()) != 0 ; )
                                            if (e->isa(plane_entity))
                                              {  ((Plane *)e)->ReCalculatePlane();
                                                 cadwindow->invalidatedisplaylist(e);
                                                 e->draw(DM_NORMAL);
                                              } 
                                       }
                                     ridgeline->draw(DM_NORMAL);
                                     hipline1->draw(DM_NORMAL);
                                     hipline2->draw(DM_NORMAL);


                                  }
                             }
                           else if (roofstyle == 2)  //  Gable end
                             {Intersect i(ridgeline,&InfinitePlane((p3 - p2).cross(Point(0.0,0.0,1.0)),p2));
                              int j; 

                                if (i.nintersections() > 0)
                                  {  p4 = ((PointE *)i.intersection(0))->getp();
                                     EntityList *l;

                                     //  Erase the entities from the screen
                                     for (j = 0 ; j < 3 ; j++)
                                       {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &hipline1->getalist() : &hipline2->getalist());
                                          for (l->start() ; (e = l->next()) != 0 ; )
                                            ;//e->draw(DM_ERASE);
                                       }
                                     ridgeline->draw(DM_ERASE);
                                     hipline1->draw(DM_ERASE);
                                     hipline2->draw(DM_ERASE);

                                     ridgeline->movehandlepoint(0,0,((ridgeline->getp1() - p1).length() > db.getptoler()) + 1,p4);
                                     hipline1->movehandlepoint(0,0,((hipline1->getp1() - p1).length() > db.getptoler()) + 1,p4);
                                     hipline2->movehandlepoint(0,0,((hipline2->getp1() - p1).length() > db.getptoler()) + 1,p4);
                         
                                     //  Recalculate the plane equations for the new roof shape and redraw the entities
                                     for (j = 0 ; j < 3 ; j++)
                                       {  l = (j == 0 ? &ridgeline->getalist() : j == 1 ? &hipline1->getalist() : &hipline2->getalist());
                                          for (l->start() ; (e = l->next()) != 0 ; )
                                            if (e->isa(plane_entity))
                                              {  ((Plane *)e)->ReCalculatePlane();
                                                 cadwindow->invalidatedisplaylist(e);
                                                 e->draw(DM_NORMAL);
                                              } 
                                       }
                                     ridgeline->draw(DM_NORMAL);
                                     hipline1->draw(DM_NORMAL);
                                     hipline2->draw(DM_NORMAL);

                                   }
                             } 

                           DeleteRoofPlan(&zdepth,&visible2);
                           CopyToRoofPlan(zdepth,&visible2);
                           CopyRoofToTiles(); 
                           CreateGutters(); 

                           db.saveundo(UD_ENDBLOCK,NULL);
                        }
                   }
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
         }

       if (*cstate == 1)
         {  state.selmask.entity.clearandset(linear_mask,end_list);
            cadwindow->prompt->normalprompt("Select the ridge line near the end to modify");
         }    
    }
  else if (*cstate != InitialState && *cstate < 200 || result == 110)
    {  v.setreal("ar::eavecutback",eavecutback);
       switch (*cstate)
         {case InitialState :
            rbdata = new RBArchitect7_data;  *data = rbdata;
            *cstate = 100;
            break;
          case 100 :
            if (event->isa(EntitySelected))
              {EntityList list;

                 state.destroyselection(1);
                 rbdata->eaveline = (Line *)((EntitySelectedEvent *) event)->getentity();
                 if (rbdata->eaveline->getlayer() != EAVE_LAYER)
                   {//  Find the corresponding eave line
                      for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                        if (e->isa(line_entity) && e->getlayer() == EAVE_LAYER)
                          {  p1 = ((Line *)e)->getp1(); 
                             p2 = ((Line *)e)->getp2(); 
                             p1.z = rbdata->eaveline->getp1().z;
                             p2.z = rbdata->eaveline->getp2().z;
                             if ((p1-rbdata->eaveline->getp1()).length() + (p2-rbdata->eaveline->getp2()).length() < db.getptoler() ||
                                 (p1-rbdata->eaveline->getp2()).length() + (p2-rbdata->eaveline->getp1()).length() < db.getptoler())
                               {  rbdata->eaveline = (Line *) e;
                                  break;
                               }
                          }
                   }

                 if (rbdata->eaveline->getlayer() != EAVE_LAYER)
                   {  cadwindow->MessageBox("Select an eave line to modify","Modify Eaves",MB_OK);
                      *cstate = ExitState;
                      break;
                   }

                 rbdata->roofplane = 0;
                 list = rbdata->eaveline->getalist();
                 for (list.start() ; (e = list.next()) != 0 ; )
                   if (e->isa(plane_entity) && e->getlayer() == ROOF_LAYER)
                     {  rbdata->roofplane = (Plane *)e;
                        break;
                     }
                 
                 if (rbdata->roofplane == 0)
                   {  cadwindow->MessageBox("Select an eave line connected to a roof plane to modify","Modify Eaves",MB_OK);
                      *cstate = ExitState;
                      break;
                   }

                 p1 = rbdata->eaveline->getp1();
                 p2 = rbdata->eaveline->getp2();

                 Intersect i1(rbdata->roofplane,&Line(p1+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback,p1+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback + Point(0.0,0.0,1.0)));
                 if (i1.nintersections() > 0)
                   p3 = ((PointE *)i1.intersection(0))->getp();
                 Intersect i2(rbdata->roofplane,&Line(p2+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback,p2+(p2-p1).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback + Point(0.0,0.0,1.0)));
                 if (i2.nintersections() > 0)
                   p4 = ((PointE *)i2.intersection(0))->getp();

                 Intersect i3(rbdata->roofplane,&Line(p1+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback,p1+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback + Point(0.0,0.0,1.0)));
                 if (i3.nintersections() > 0)
                   p5 = ((PointE *)i3.intersection(0))->getp();
                 Intersect i4(rbdata->roofplane,&Line(p2+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback,p2+(p1-p2).cross(Point(0.0,0.0,1.0)).normalize() * eavecutback + Point(0.0,0.0,1.0)));
                 if (i4.nintersections() > 0)
                   p6 = ((PointE *)i4.intersection(0))->getp();

                 if (p3.z > p5.z)
                   {  rbdata->p1 = p3;  rbdata->p2 = p4;
                      rbdata->p3 = p3;  rbdata->p4 = p4;
                   }
                 else
                   {  rbdata->p1 = p5;  rbdata->p2 = p6;
                      rbdata->p3 = p5;  rbdata->p4 = p6;
                   }

                 rbdata->eaveline1 = rbdata->eaveline2 = rbdata->hipline1 = rbdata->hipline2 = 0;
                 for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                   if (e->isa(line_entity) && e->getlayer() == EAVE_LAYER && e != rbdata->eaveline)
                     {  if ((p1 - ((Line *)e)->getp1()).length() < 1.0 || (p1 - ((Line *)e)->getp2()).length() < 1.0)
                          rbdata->eaveline1 = (Line *)e; 
                        if ((p2 - ((Line *)e)->getp1()).length() < 1.0 || (p2 - ((Line *)e)->getp2()).length() < 1.0)
                          rbdata->eaveline2 = (Line *)e; 
                     }
                   else if (e->isa(line_entity) && e->getlayer() == HIP_LAYER)
                     {  if ((p1 - ((Line *)e)->getp1()).length() < 1.0 || (p1 - ((Line *)e)->getp2()).length() < 1.0)
                          rbdata->hipline1 = (Line *)e; 
                        if ((p2 - ((Line *)e)->getp1()).length() < 1.0 || (p2 - ((Line *)e)->getp2()).length() < 1.0)
                          rbdata->hipline2 = (Line *)e; 
                     }

                 rbdata->roofplane1 = 0;
                 list = rbdata->eaveline1->getalist();
                 for (list.start() ; (e = list.next()) != 0 ; )
                   if (e->isa(plane_entity) && e->getlayer() == ROOF_LAYER)
                     {  rbdata->roofplane1 = (Plane *)e;
                        break;
                     }

                 rbdata->roofplane2 = 0;
                 list = rbdata->eaveline2->getalist();
                 for (list.start() ; (e = list.next()) != 0 ; )
                   if (e->isa(plane_entity) && e->getlayer() == ROOF_LAYER)
                     {  rbdata->roofplane2 = (Plane *)e;
                        break;
                     }

                 *cstate = 101;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
          case 101 : 
            rubberband.end(0);
            if (event->isa(Coordinate3d))
              {  rbdata->p1 = Line(rbdata->p1,rbdata->p2).nearp(((Coordinate3dEvent *) event)->getp());
                 *cstate = 102;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
          case 102 : 
            rubberband.end(0);
            if (event->isa(Coordinate3d))
              {EntityList list,plist;
               int cuthipline1,cuthipline2;

                 db.saveundo(UD_STARTBLOCK,NULL);

                 rbdata->p2 = Line(rbdata->p1,rbdata->p2).nearp(((Coordinate3dEvent *) event)->getp());

                 //  Switch rbdata->p1 and rbdata->p2 if the direction is different to the eave line direction.
                 if (rbdata->eaveline->direction(0.0).dot(rbdata->p2 - rbdata->p1) < 0.0)
                   {  p1 = rbdata->p1;  rbdata->p1 = rbdata->p2;  rbdata->p2 = p1;
                   } 

                 // Check the first hip lines for intersections
                 cuthipline1 = 0;
                 if (rbdata->hipline1 != 0 && rbdata->eaveline1 != 0 && rbdata->roofplane1 != 0)
                   {Intersect i1(rbdata->hipline1,&Line(rbdata->p1,rbdata->p2));
                      if (i1.nintersections() > 0 && rbdata->hipline1->ison(((PointE *)i1.intersection(0))->getp()) && Line(rbdata->p1,rbdata->p2).ison(((PointE *)i1.intersection(0))->getp()))
                        {  rbdata->p1 = ((PointE *)i1.intersection(0))->getp();
                           if (rbdata->hipline1->getp1().z > rbdata->hipline1->getp2().z)
                             {  p5 = rbdata->hipline1->getp2();
                                cuthipline1 = 2;
                             } 
                           else
                             {  p5 = rbdata->hipline1->getp1();
                                cuthipline1 = 1;
                             } 

                           line = new Line(rbdata->p1,rbdata->eaveline1->nearp(rbdata->p1));
                           line->EntityHeader::change(line->getcolour(),EAVE_LAYER,line->getstyle(),line->getweight());
                           db.geometry.add(line);
                           rbdata->roofplane1->AddEdge(p5,line);

                           if ((p5 - rbdata->eaveline1->getp1()).length() > (p5 - rbdata->eaveline1->getp2()).length())
                             rbdata->eaveline1->movehandlepoint(0,0,2,rbdata->eaveline1->nearp(rbdata->p1));
                           else
                             rbdata->eaveline1->movehandlepoint(0,0,1,rbdata->eaveline1->nearp(rbdata->p1));
                           
                           rbdata->hipline1->movehandlepoint(0,0,cuthipline1,rbdata->p1);

                           rbdata->p3 += (rbdata->p3 - rbdata->p4).normalize() * 100000.0;
                           cuthipline1 = 1;
                        }
                   }

                 if (cuthipline1 == 0)
                   {  //  Does not intersect the hipline - Add the connecting line
                      rbdata->p3 = rbdata->eaveline->nearp(rbdata->p1);
                      line = new Line(rbdata->p1,rbdata->p3);
                      line->EntityHeader::change(line->getcolour(),EAVE_LAYER,line->getstyle(),line->getweight());
                      db.geometry.add(line);
                      list.add(line);
                   }


                 // Check the second hip lines for intersections
                 cuthipline2 = 0;
                 if (rbdata->hipline2 != 0 && rbdata->eaveline2 != 0 && rbdata->roofplane2 != 0)
                   {Intersect i1(rbdata->hipline2,&Line(rbdata->p1,rbdata->p2));
                      if (i1.nintersections() > 0 && rbdata->hipline2->ison(((PointE *)i1.intersection(0))->getp()) && Line(rbdata->p1,rbdata->p2).ison(((PointE *)i1.intersection(0))->getp()))
                        {  rbdata->p2 = ((PointE *)i1.intersection(0))->getp();
                           if (rbdata->hipline2->getp1().z > rbdata->hipline2->getp2().z)
                             {  p5 = rbdata->hipline2->getp2();
                                cuthipline2 = 2;
                             }
                           else
                             {  p5 = rbdata->hipline2->getp1();
                                cuthipline2 = 1;
                             }

                           line = new Line(rbdata->p2,rbdata->eaveline2->nearp(rbdata->p2));
                           line->EntityHeader::change(line->getcolour(),EAVE_LAYER,line->getstyle(),line->getweight());
                           db.geometry.add(line);
                           rbdata->roofplane2->AddEdge(p5,line);

                           if ((p5 - rbdata->eaveline2->getp1()).length() > (p5 - rbdata->eaveline2->getp2()).length())
                             rbdata->eaveline2->movehandlepoint(0,0,2,rbdata->eaveline2->nearp(rbdata->p2));
                           else
                             rbdata->eaveline2->movehandlepoint(0,0,1,rbdata->eaveline2->nearp(rbdata->p2));

                           rbdata->hipline2->movehandlepoint(0,0,cuthipline2,rbdata->p2);

                           rbdata->p4 += (rbdata->p4 - rbdata->p3).normalize() * 100000.0;
                           cuthipline2 = 1; 
                        }
                   }

                 if (cuthipline2 == 0)
                   {  //  Does not intersect the hipline - Add the connecting line
                      rbdata->p4 = rbdata->eaveline->nearp(rbdata->p2);
                      line = new Line(rbdata->p2,rbdata->p4);
                      line->EntityHeader::change(line->getcolour(),EAVE_LAYER,line->getstyle(),line->getweight());
                      db.geometry.add(line);
                      list.add(line);
                   }

                 line = new Line(rbdata->p1,rbdata->p2);
                 line->EntityHeader::change(line->getcolour(),EAVE_LAYER,line->getstyle(),line->getweight());
                 db.geometry.add(line);
                 list.add(line);

                 if (cuthipline1 == 0 || cuthipline2 == 0)
                   {  list.add(rbdata->eaveline);
                      rbdata->eaveline->breakanddelete(0,rbdata->eaveline->nearp(rbdata->p3),rbdata->eaveline->nearp(rbdata->p4));
                      if (rbdata->eaveline->getcopye() != 0)
                        list.add(rbdata->eaveline->getcopye());
                   }

                 plist = rbdata->roofplane->getlist();
                 for (plist.start() ; (e = plist.next()) != 0 ; )
                   if (e != rbdata->eaveline)
                     list.add(e);

                 Loops loops(1);
                 loops.create(&list);

                 Plane *plane = new Plane(loops.getloops(),rbdata->roofplane->getxspacing(),rbdata->roofplane->getyspacing(),rbdata->roofplane->getangle(),rbdata->roofplane->getcrossangle(),rbdata->roofplane->gettransparency(),rbdata->roofplane->getred(),rbdata->roofplane->getgreen(),rbdata->roofplane->getblue(),rbdata->roofplane->gettransparencyrange(),RGB(0,0,0),_RCT(""),rbdata->roofplane->getoptions());
                 plane->EntityHeader::change(plane->getcolour(),ROOF_LAYER,plane->getstyle(),plane->getweight());
                 if (plane->getdefined())
                   {  db.geometry.add(plane);
                      db.geometry.del(rbdata->roofplane);
                   }

                 DeleteRoofPlan(&zdepth,&visible2);
                 CreateGutters(); 
                 if (cuthipline1 != 0 && cuthipline2 != 0)
                   db.geometry.del(rbdata->eaveline);  //  Eaveline no longer used.

                 CopyToRoofPlan(zdepth,&visible2);
                 CopyRoofToTiles(); 
                 db.saveundo(UD_ENDBLOCK,NULL);

                 *cstate = 100;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;

         }

       if (*cstate == 100)
         {  state.selmask.entity.clearandset(linear_mask,end_list);
            cadwindow->prompt->normalprompt("Select the eave line to cutback");
         }
       else if (*cstate == 101)
         {  state.selmask.entity.clearandset(xyz_mask,end_list);
            cadwindow->prompt->normalprompt("Select the start point of the cutback");
            rbdata->state = *cstate;
            rubberband.begin(0);
            rubberband.add(RBArchitect7,rbdata);
         }
       else if (*cstate == 102)
         {  state.selmask.entity.clearandset(xyz_mask,end_list);
            cadwindow->prompt->normalprompt("Select the end point of the cutback");
            rbdata->state = *cstate;
            rubberband.begin(0);
            rubberband.add(RBArchitect7,rbdata);
         }
       else if (*cstate == ExitState)
         delete rbdata;

    }
  else if (*cstate != InitialState && *cstate < 300 || result == 113)
    {  v.setreal("ar::dutchgabledistancefromend",distancefromend);
       v.setreal("ar::dutchgableridgelength",ridgelength);
       switch (*cstate)
         {case InitialState : 
            rbdata = new RBArchitect7_data;  *data = rbdata;
            *cstate = 200;
            break;
          case 200 : 
            if (event->isa(EntitySelected))
              {  state.destroyselection(1);
                 line = (Line *)((EntitySelectedEvent *) event)->getentity();
                 if (line->getlayer() == ROOFPLAN_LAYER)
                   {//  Find the corresponding 3D ridge line
                      for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                        if (e->isa(line_entity) && e->getlayer() != ROOFPLAN_LAYER && e->getlayer() != FACIA_LAYER && e->getlayer() != TILES_LAYER)
                          {  p1 = ((Line *)e)->getp1(); 
                             p2 = ((Line *)e)->getp2(); 
                             p1.z = line->getp1().z;
                             p2.z = line->getp2().z;
                             if ((p1-line->getp1()).length() + (p2-line->getp2()).length() < db.getptoler())
                               {  line = (Line *) e;
                                  break;
                               }
                          }
                   }

                 if (line->getlayer() != RIDGE_LAYER)
                   {  cadwindow->MessageBox("Select near the end of the ridge line to modify","Modify Roof",MB_OK);
                      *cstate = ExitState;
                      break;
                   }

                 if (((EntitySelectedEvent *) event)->getendselected() == 1)
                   {  rbdata->dir = -((LinearEntity *)line)->direction(0.0);
                      rbdata->p1 = line->start() - rbdata->dir * distancefromend;
                      rbdata->ridgeline = line;
                      rbdata->ridgelength = ridgelength;
                   }  
                 else
                   {  rbdata->dir = ((LinearEntity *)line)->direction(1.0);
                      rbdata->p1 = line->end() - rbdata->dir * distancefromend;
                      rbdata->ridgeline = line;
                      rbdata->ridgelength = ridgelength;
                   }

                 *cstate = 201;

              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
          case 201 : 
            rubberband.end(0);
            if (event->isa(Coordinate3d))
              {EntityList list1,list2,list3;
               BitMask options(32);

                 rbdata->dir = (rbdata->p3 - rbdata->p1).cross(Point(0.0,0.0,1.0)).normalize(); 
                 p1 = rbdata->p3 + rbdata->dir * rbdata->ridgelength;
                 p2 = rbdata->p3 - rbdata->dir * rbdata->ridgelength;

                 list = rbdata->ridgeline->getalist();
                 for (list.start() ; (e = list.next()) != 0 ; )
                   if (e->isa(plane_entity))
                     {  Intersect i1(e,&Line(p1,p1+Point(0.0,0.0,1.0)));
                        if (i1.nintersections() > 0 && ((PointE *)i1.intersection(0))->getp().z < p1.z)
                          {  p1 = ((PointE *)i1.intersection(0))->getp();
                          }
                        Intersect i2(e,&Line(p2,p2+Point(0.0,0.0,1.0)));
                        if (i2.nintersections() > 0 && ((PointE *)i2.intersection(0))->getp().z < p2.z)
                          {  p2 = ((PointE *)i2.intersection(0))->getp();
                          }
                     }

                 db.saveundo(UD_STARTBLOCK,NULL);

                 line1 = new Line(rbdata->p1,rbdata->p3);
                 line1->EntityHeader::change(line1->getcolour(),RIDGE_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line1);
                 line2 = new Line(rbdata->p1,p1);
                 line2->EntityHeader::change(line1->getcolour(),DUTCHGABLEOVERHIP_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line2);
                 line3 = new Line(rbdata->p1,p2);
                 line3->EntityHeader::change(line1->getcolour(),DUTCHGABLEOVERHIP_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line3);
                 line4 = new Line(rbdata->p3,p1);
                 line4->EntityHeader::change(line1->getcolour(),DUTCHGABLEOVER_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line4);
                 line5 = new Line(rbdata->p3,p2);
                 line5->EntityHeader::change(line1->getcolour(),DUTCHGABLEOVER_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line5);
                 line6 = new Line(p1,p2);
                 line6->EntityHeader::change(line1->getcolour(),DUTCHGABLEOVER_LAYER,line1->getstyle(),line1->getweight());
                 db.geometry.add(line6);

                 list1.add(line1);  list1.add(line4);  list1.add(line2);
                 Plane *plane1 = new Plane(list1,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
                 plane1->EntityHeader::change(plane1->getcolour(),ROOF_LAYER,plane1->getstyle(),plane1->getweight());
                 if (plane1 != 0 && plane1->getdefined())
                   {  db.geometry.add(plane1);
                      if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                        {  strcpy(at,"ar::Cost::Roof::");
                           strcat(at,name);
                           plane1->addat(at);
                           plane1->setattribute(at);
                           db.saveundo(UD_INSERTATT,plane1);
                        }
                   }
                 
                 list2.add(line1);  list2.add(line3);  list2.add(line5);
                 Plane *plane2 = new Plane(list2,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
                 plane2->EntityHeader::change(plane2->getcolour(),ROOF_LAYER,plane2->getstyle(),plane2->getweight());
                 if (plane2 != 0 && plane2->getdefined())
                   {  db.geometry.add(plane2);
                      if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                        {  strcpy(at,"ar::Cost::Roof::");
                           strcat(at,name);
                           plane2->addat(at);
                           plane2->setattribute(at);
                           db.saveundo(UD_INSERTATT,plane2);
                        }
                   }
                 
                 list3.add(line4);  list3.add(line5);  list3.add(line6);
                 Plane *plane3 = new Plane(list3,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
                 plane3->EntityHeader::change(plane3->getcolour(),ROOF_LAYER,plane3->getstyle(),plane3->getweight());
                 if (plane3 != 0 && plane3->getdefined())
                   {  db.geometry.add(plane3);
                      if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                        {  strcpy(at,"ar::Cost::Roof::");
                           strcat(at,name);
                           plane3->addat(at);
                           plane3->setattribute(at);
                           db.saveundo(UD_INSERTATT,plane3);
                        }
                   }

                 DeleteRoofPlan(&zdepth,&visible2);
                 CopyToRoofPlan(zdepth,&visible2);
                 CopyRoofToTiles(); 
                 CreateGutters(); 

                 db.saveundo(UD_ENDBLOCK,NULL);

                 *cstate = ExitState;
              }
            else if (event->isa(Abort) || event->isa(Exit))
              *cstate = ExitState;
            break;
         }

       if (*cstate == 200)
         {  state.selmask.entity.clearandset(linear_mask,end_list);
            cadwindow->prompt->normalprompt("Select the ridge line to place the roof section on");
         }
       else if (*cstate == 201)
         {  state.selmask.entity.clearandset(xyz_mask,end_list);
            cadwindow->prompt->normalprompt("Locate the direction of the roof section");
            rbdata->state = *cstate;
            rubberband.begin(0);
            rubberband.add(RBArchitect7a,rbdata);
         }
       else if (*cstate == ExitState)
         delete rbdata;

    }
  else
    {  v.setreal("ar::roofpitch",pitch);
       v.setreal("ar::roofeaves",eaves);
       v.setstring("ar::roofname",name);
     
       //  Make the eaves at least 0.2mm wide to allow the roof to be created correctly.
       if (eaves < 0.2)
         eaves = 0.2;  

       visible1.clearall();
       if (db.views3d.match(rs103.gets()) != 0)
         visible1.set(db.views3d.match(rs103.gets())->getviewno());
       if (db.views3d.match(rs0.gets()) != 0)
         visible1.set(db.views3d.match(rs0.gets())->getviewno());
       if (db.views3d.match(rs1.gets()) != 0)
         visible1.set(db.views3d.match(rs1.gets())->getviewno());
       if (db.views3d.match(rs2.gets()) != 0)
         visible1.set(db.views3d.match(rs2.gets())->getviewno());
       if (db.views3d.match(rs3.gets()) != 0)
         visible1.set(db.views3d.match(rs3.gets())->getviewno());
       if (db.views3d.match(rs4.gets()) != 0)
         visible1.set(db.views3d.match(rs4.gets())->getviewno());
       if (db.views3d.match(rs5.gets()) != 0)
         visible1.set(db.views3d.match(rs5.gets())->getviewno());
       if (db.views3d.match(rs6.gets()) != 0)
         visible1.set(db.views3d.match(rs6.gets())->getviewno());
       if (db.views3d.match(rs7.gets()) != 0)
         visible1.set(db.views3d.match(rs7.gets())->getviewno());
       if (db.views3d.match(rs8.gets()) != 0)
         visible1.set(db.views3d.match(rs8.gets())->getviewno());

       visible2.clearall();
       visible2.set(state.getlastviewno());
       zdepth = 0.0;
       if (db.views3d.match(state.getlastviewno()) != 0)
         zdepth = db.views3d.match(state.getlastviewno())->getref().z;

       wlist = state.getselection().copy();
       if (wlist.length() == 0)
         {  cadwindow->MessageBox(rs11.gets(),rs12.gets(),MB_ICONINFORMATION);
            *cstate = ExitState;
            return;
         }

       pitchingoffset = 0.0;
       za.setxyz(0.0,0.0,2200.0);
       for (wlist.start() ; (e = wlist.next()) != 0 ; )
         {  if (! e->isa(wall_entity) && ! e->isa(line_entity))
              {  cadwindow->MessageBox(rs13.gets(),rs12.gets(),MB_ICONINFORMATION);
                 wlist.destroy();
                 return;
              }
            if (e->isa(wall_entity))
              {  pitchingoffset = ((Wall *)e)->gettotalwidth() - ((Wall *)e)->getwidth();
                 za = ((Wall *)e)->getzaxis() * (((Wall *)e)->getheight() - (pitchingoffset + eaves) * tan(pitch * M_PI / 180.0));
              }
         }

       db.saveundo(UD_STARTBLOCK,NULL);

       for (wlist.start() ; (e = wlist.next()) != 0 ; )
         {  if (e->isa(wall_entity))
              line = new Line(((Wall *)e)->point(4)+za,((Wall *)e)->point(5)+za);
            else
              line = new Line(((Line *)e)->getp1()+za,((Line *)e)->getp2()+za);
            line->setvisible(visible1);
            llist1.add(line);
         }

       //  Join up any lines that are linear
       for (llist1.start() ; (line1 = (Line *)llist1.next()) != 0 ; )
         {  llist2 = llist1;
            for (llist2.start() ; (line2 = (Line *)llist2.next()) != 0 ; )
              if (line1 != line2)
                {  if ((line1->getp1() - line2->getp1()).length() < db.getptoler() && fabs(line1->direction(0.0).dot(line2->direction(0.0))) > cos(0.01))
                     {  line = new Line(line1->getp2(),line2->getp2());
                        llist1.add(line);
                        llist1.del(line1); 
                        llist1.del(line2); 
                        llist1.start();
                        break;
                     }
                   if ((line1->getp1() - line2->getp2()).length() < db.getptoler() && fabs(line1->direction(0.0).dot(line2->direction(0.0))) > cos(0.01))
                     {  line = new Line(line1->getp2(),line2->getp1());
                        llist1.add(line);
                        llist1.del(line1); 
                        llist1.del(line2); 
                        llist1.start();
                        break;
                     }
                   if ((line1->getp2() - line2->getp1()).length() < db.getptoler() && fabs(line1->direction(0.0).dot(line2->direction(0.0))) > cos(0.01))
                     {  line = new Line(line1->getp1(),line2->getp2());
                        llist1.add(line);
                        llist1.del(line1); 
                        llist1.del(line2); 
                        llist1.start();
                        break;
                     }
                   if ((line1->getp2() - line2->getp2()).length() < db.getptoler() && fabs(line1->direction(0.0).dot(line2->direction(0.0))) > cos(0.01))
                     {  line = new Line(line1->getp1(),line2->getp1());
                        llist1.add(line);
                        llist1.del(line1); 
                        llist1.del(line2); 
                        llist1.start();
                        break;
                     }
                }
         }

       llist2 = llist1.copy();
       Loops aloops(1);
       aloops.create(&llist1);
       aloops.offset(0,0,p,eaves,&olist1);
       aloops.planar(&origin,&xaxis,&yaxis,&zaxis,&D);
       Polygon3d poly3(origin,xaxis,yaxis,olist1,10,0.0);
       sign = (poly3.getarea() > 0.0 ? -1.0 : 1.0);
       if (sign > 0.0)
         {EntityList newlist;
            for (llist2.start() ; (line1 = (Line *)llist2.next()) != 0 ; )
              {  line = new Line(line1->getp2(),line1->getp1());
                 newlist.add(line);
              }
            llist1 = newlist;
         }
       else
         llist1 = llist2;

       llist2 = llist1.copy();

       Loops loops(1);
       loops.create(&llist1);

       if (! llist1.empty() || loops.getloops().empty() || loops.nloops() != 1 || ! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
         {  cadwindow->MessageBox(rs14.gets(),rs12.gets(),MB_ICONINFORMATION);
            wlist.destroy();
            llist1.destroy();
            llist2.destroy();
            loops.destroy();
            db.saveundo(UD_ENDBLOCK,NULL);
            return;
         }

       loops.offset(0,0,p, eaves,&olist1);
       loops.offset(0,0,p,-eaves,&olist2);

       if (zaxis.z < 0.0) yaxis = -yaxis;

       Polygon3d poly1(origin,xaxis,yaxis,olist1,10,0.0);
       Polygon3d poly2(origin,xaxis,yaxis,olist2,10,0.0);

       n = olist1.length();

       if (fabs(poly1.getarea()) > fabs(poly2.getarea()))
         {  for (olist1.start(),i = 0 ; (e = olist1.next()) != 0 ; i++)
              {  e->setvisible(visible1);
                 e->EntityHeader::change(e->getcolour(),EAVE_LAYER,e->getstyle(),e->getweight());
                 db.geometry.add(e);

              }
            for (olist2.start() ; (e = olist2.next()) != 0 ; )
              delete e;
            olist2.destroy();
            sign = (poly1.getarea() > 0.0 ? -1.0 : 1.0);
            loops.offset(0,0,p,eaves+100.0,&olist3);
            if (pitchingoffset > 1.0E-5)
              loops.offset(0,0,p,-pitchingoffset,&olist2);
            else
              {  for (llist2.start() ; (e = llist2.next()) != 0 ; )
                   {  e->setcopye(0);
                      olist2.add(e->clone(ident));
                   }
              }
         }
       else
         {  for (olist1.start() ; (e = olist1.next()) != 0 ; )
              delete e;
            for (olist2.start(),i = 0 ; (e = olist2.next()) != 0 ; i++)
              {  e->setvisible(visible1);
                 e->EntityHeader::change(e->getcolour(),EAVE_LAYER,e->getstyle(),e->getweight());
                 db.geometry.add(e);

              }
            olist1.destroy();
            olist1 = olist2;  olist2 = elist;
            sign = (poly2.getarea() > 0.0 ? -1.0 : 1.0);
            loops.offset(0,0,p,-eaves-100.0,&olist3);
            if (pitchingoffset > 1.0E-5)
              loops.offset(0,0,p,pitchingoffset,&olist2);
            else
              {  for (llist2.start() ; (e = llist2.next()) != 0 ; )
                   {  e->setcopye(0);
                      olist2.add(e->clone(ident));
                   }
              }
         }


       //  Now insert the eaves ceiling
       list = olist1.copy();  list.reverse();  list.add(0);
       for (olist2.start() ; (e = olist2.next()) != 0 ; )
         {  list.add(e);
            e->setcopye(0);
            up.translate(0.0,0.0,(pitchingoffset + eaves) * tan(pitch * M_PI / 180.0));
            e->clone(up);  
            e->getcopye()->EntityHeader::change(e->getcolour(),PITCHING_LAYER,e->getstyle(),e->getweight());
            db.geometry.add(e->getcopye());
         }
       list.reverse();

       //  For now - don't insert the eaves plane - it affects eave cutbacks.
       //Plane *plane = new Plane(list,0.0,0.0,0.0,0.0,_RCT(""),o);
       //plane->EntityHeader::change(plane->getcolour(),FACIA_LAYER,plane->getstyle(),plane->getweight());
       //plane->dbtraverseassociated(visiblecb,&visible1);
       //if (plane != 0 && plane->getdefined())
       //  {  plane->setvisible(visible1);
       //     db.geometry.add(plane);
       //  }

       //  Now try to insert a roof.

       if (pitch < 0.001)
         {  db.saveundo(UD_ENDBLOCK,NULL);
            return;
         }

       for (olist1.start(),i = 0 ; (e = olist1.next()) != 0 ; i++)
         {  polygon = new ArPolygon;
            polygon->origin = ((Line *)e)->getp1();
            polygon->xaxis = ((Line *)e)->getp2() - ((Line *)e)->getp1();
            polygon->yaxis = (xaxis.cross(yaxis)).cross(polygon->xaxis) * sign;

            t.rotate(((Line *)e)->getp1(),polygon->xaxis,pitch * M_PI / 180.0 * sign);
            polygon->yaxis = t.transform(polygon->origin + polygon->yaxis) - polygon->origin;
            polygon->zaxis = polygon->xaxis.cross(polygon->yaxis);
            polygon->xaxis.normalize();
            polygon->yaxis.normalize();
            polygon->zaxis.normalize();
            polygon->list.add(e);
            plist.add(polygon);
         }
       plast = polygon;
       plist.reverse();

       for (olist1.start(),plist.start() ; (e = olist1.next()) != 0 && (polygon = (ArPolygon *) plist.next()) != 0 ; )
         {  vertex = new ArVertex;
            vertex->p = ((Line *)e)->getp1();
            vertex->dir = (plast->zaxis.cross(polygon->zaxis)).normalize() * sign;
            if (vertex->dir.z < 0.0) 
              vertex->dir = -vertex->dir;
            vertex->poly1 = plast;
            vertex->poly2 = polygon;
            vertex->length = MAXDOUBLE;
            vertex->original = 1;
            plast = polygon;
            vlist1.add(vertex);
         }

       vlist1.reverse();

       olist1.destroy();  olist2.destroy();

       for(nlines = 1 ; nlines > 0 ;)
         {ArVertex *vmin1,*vmin2;
          double l,lmin;

            //  Calculate intersections between lines that are adjacent
            for (vlist1.start() ; (v1 = (ArVertex *) vlist1.next()) != 0 ; )
              {  vlist2 = vlist1;
                 for (vlist2.start() ; (v2 = (ArVertex *) vlist2.next()) != 0 ; )
                   if (v1 != v2 && (v1->poly1 == v2->poly1 ||
                                    v1->poly1 == v2->poly2 ||
                                    v1->poly2 == v2->poly1 ||
                                    v1->poly2 == v2->poly2))
                     {  Line line1(v1->p,v1->p+v1->dir);
                        Line line2(v2->p,v2->p+v2->dir);

                        //  Do the two lines intersect?
                        Intersect i(&line1,&line2);
                        if (i.nintersections() == 1 &&
                              (v1->original && v1->dir.dot(((PointE *)i.intersection(0))->getp() - v1->p) > 0.0 || ! v1->original) && (((PointE *)i.intersection(0))->getp() - v2->p).length() <= v2->length + db.getptoler())
                          {  l = (((PointE *)i.intersection(0))->getp() - v1->p).length();
                             if (l < v1->length)
                               v1->length = l;
                             if (! v1->original && v1->dir.dot(((PointE *)i.intersection(0))->getp() - v1->p) < 0.0)
                               v1->dir = -v1->dir;
                          }

                        //  Does v2 lie on the line starting at v1?
                        if ((v2->p - line1.nearp(v2->p)).length() < db.getptoler() &&
                              (v1->original && v1->dir.dot(v2->p - v1->p) > 0.0 || ! v1->original))
                          {  l = (v2->p - v1->p).length();
                             if (l < v1->length)
                               {  v1->length = l;
                                  v2->length = 0.0;
                               }
                             if (! v1->original && v1->dir.dot(v2->p - v1->p) < 0.0)
                               v1->dir = -v1->dir;
                          }

                     }


              }

            
            //  Find the shortest line
            lmin = -1.0;
            vmin1 = vmin2 = 0;
            for (vlist1.start() ; (v1 = (ArVertex *) vlist1.next()) != 0 ; )
              {  vlist2 = vlist1;
                 for (vlist2.start() ; (v2 = (ArVertex *) vlist2.next()) != 0 ; )
                   if (v1 != v2 && (v1->poly1 == v2->poly1 ||
                                    v1->poly1 == v2->poly2 ||
                                    v1->poly2 == v2->poly1 ||
                                    v1->poly2 == v2->poly2) &&
                       v1->length != MAXDOUBLE && v2->length != MAXDOUBLE &&
                       (v1->p + v1->dir * v1->length - v2->p - v2->dir * v2->length).length() < db.getptoler())
                      {  if (v1->length > v2->length)  //  Choose the longest
                           l = v1->length;
                         else
                           l = v2->length;
                         if (lmin < 0.0 || l < lmin)
                           {  lmin = l;  
                              vmin1 = v1;
                              vmin2 = v2;  
                           }
                      } 
              }


//MSG msg;
//while (! PeekMessage(&msg,0,WM_KEYFIRST,WM_KEYLAST,PM_REMOVE));
                   
            //  Insert the shortest pair of lines.
            if (vmin1 != 0 && vmin2 != 0)
              {  v1 = vmin1;  v2 = vmin2;

                 if (v1->length > 0.0)                   
                   {  line1 = new Line(v1->p,v1->p + v1->dir * v1->length);
                      line1->setvisible(visible1);
                      line1->EntityHeader::change(line1->getcolour(),fabs(line1->direction(0.0).z) < db.getptoler() ? RIDGE_LAYER : HIP_LAYER,line1->getstyle(),line1->getweight());
                      db.geometry.add(line1);
                      v1->poly1->list.add(line1);
                      v1->poly2->list.add(line1);
                   }
                 if (v2->length > 0.0)
                   {  line2 = new Line(v2->p,v2->p + v2->dir * v2->length);
                      line2->setvisible(visible1);
                      line2->EntityHeader::change(line2->getcolour(),fabs(line2->direction(0.0).z) < db.getptoler() ? RIDGE_LAYER : HIP_LAYER,line2->getstyle(),line2->getweight());
                      db.geometry.add(line2);
                      v2->poly1->list.add(line2);
                      v2->poly2->list.add(line2);
                   }

                 if (v1->length > 0.0 && v2->length > 0.0)
                   {  vertex = new ArVertex;
                      if (v1->poly1 == v2->poly1)
                        {  vertex->poly1 = v1->poly2;
                           vertex->poly2 = v2->poly2;
                        }
                      else if (v1->poly1 == v2->poly2)
                        {  vertex->poly1 = v1->poly2;
                           vertex->poly2 = v2->poly1;
                        }
                      else if (v1->poly2 == v2->poly1)
                        {  vertex->poly1 = v1->poly1;
                           vertex->poly2 = v2->poly2;
                        }
                      else if (v1->poly2 == v2->poly2)
                        {  vertex->poly1 = v1->poly1;
                           vertex->poly2 = v2->poly1;
                        }
                      vertex->p = v1->p + v1->dir * v1->length;
                      vertex->dir = (vertex->poly1->zaxis.cross(vertex->poly2->zaxis)).normalize() * sign;
                      if (vertex->dir.z < 0.0) 
                        vertex->dir = -vertex->dir;
                      vertex->length = MAXDOUBLE;
                      vertex->original = 0;
                      vlistnew.add(vertex);
                   }  

                 vlist1.del(v1);
                 vlist1.del(v2);
                 vlist1.start(); 

              }
            else
              break;  //  No more lines to process
            
            for (vlistnew.start() ; (v1 = (ArVertex *)vlistnew.next()) != 0 ; )
              vlist1.add(v1);
            vlistnew.destroy(); 
 
        }

       //  Try to make up one last join
       if (vlist1.length() == 2)
         {  vlist1.start();
            v1 = (ArVertex *) vlist1.next();
            v2 = (ArVertex *) vlist1.next();
            line = new Line(v1->p,v2->p);
            line->setvisible(visible1);
            line->EntityHeader::change(line->getcolour(),fabs(line->direction(0.0).z) < db.getptoler() ? RIDGE_LAYER : HIP_LAYER,line->getstyle(),line->getweight());
            db.geometry.add(line);
            v1->poly1->list.add(line);
            v1->poly2->list.add(line);

         }

       for (plist.start() ; (polygon = (ArPolygon *) plist.next()) != 0 ; )
         {Loops loops(1);
          EntityList list;

            polygon->list.reverse();
            list = polygon->list.copy();  
            loops.create(&list);
            if (list.empty() && ! loops.getloops().empty())
              {  Plane *plane = new Plane(loops.getloops(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 plane->EntityHeader::change(plane->getcolour(),ROOF_LAYER,plane->getstyle(),plane->getweight());
                 if (plane != 0 && plane->getdefined())
                   {  plane->setvisible(visible1);
                      db.geometry.add(plane);
                      if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                        {  strcpy(at,"ar::Cost::Roof::");
                           strcat(at,name);
                           plane->addat(at);
                           plane->setattribute(at);
                           db.saveundo(UD_INSERTATT,plane);
                        }
                   }
                 polygon->list.destroy();
              }
            else
              //  The polygon was not closed - add it to the unused list and try to match it later with another unused polygon.
              plist1.add(polygon);
         }

       for (plist1.start() ; (polygon = (ArPolygon *) plist1.next()) != 0 ; )
         {  plist2 = plist1;
            for (plist2.start() ; (poly = (ArPolygon *) plist2.next()) != 0 ; )
              if (poly != polygon && fabs(polygon->zaxis.dot(polygon->origin - poly->origin)) < 1.0 && fabs(polygon->zaxis.dot(poly->zaxis)) > cos(1.0/180.0*acos(-1.0)))
                {//  The two polygons are on the same plane - try to join them up.
                 EntityList list;
                 Entity *e;
                 Loops loops(1);
          
                   list = polygon->list.copy();
                   for (poly->list.start() ; (e = poly->list.next()) != 0 ; )
                     list.add(e);

                   loops.create(&list);
                   if (list.empty() && ! loops.getloops().empty())
                     {  Plane *plane = new Plane(loops.getloops(),0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                        plane->EntityHeader::change(plane->getcolour(),ROOF_LAYER,plane->getstyle(),plane->getweight());
                        if (plane != 0 && plane->getdefined())
                          {  plane->setvisible(visible1);
                             db.geometry.add(plane);
                             if ((at = new RCCHAR[strlen(name) + 17]) != NULL)
                               {  strcpy(at,"ar::Cost::Roof::");
                                  strcat(at,name);
                                  plane->addat(at);
                                  plane->setattribute(at);
                                  db.saveundo(UD_INSERTATT,plane);
                               }
                          }
                        polygon->list.destroy();
                        poly->list.destroy(); 
                     }

                }

         }

       for (llist2.start() ; (e = llist2.next()) != 0 ; )
         {  e->draw(DM_ERASE);
            delete e;
         }

       llist1.destroy();
       llist2.destroy();
       wlist.destroy();

       CopyToRoofPlan(zdepth,&visible2);
       CopyRoofToTiles();
       CreateGutters();

       db.saveundo(UD_ENDBLOCK,NULL);

       *cstate = ExitState;

    }
*/
}

class FNListDialogControl : public ListDialogControl
  {public:
     FNListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void FNListDialogControl::load(Dialog *dialog)
{RCCHAR name[300],key[300];
 int status;
  ListDialogControl::load(dialog);
  status = v.getfirststring(_RCT("ar::Cost::Floor::"),key,name);
  while (status)
    {  ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,key+17);
       status = v.getnextstring(_RCT("ar::Cost::Floor::"),key,name);
    }
}

void architect_command18(int *cstate,HighLevelEvent *,void **)
{EntityList blist,wlist,hlist,llist1,llist2,olist1,olist2,list,elist;
 GeneralList plist,vlist1,vlist2;
 Entity *e,*line;
 Point p(1.0E10,2.3E10,0.0),origin,xaxis,yaxis,zaxis(0.0,0.0,1.0),za(0.0,0.0,0.0);
 BitMask options(32);
 ResourceString rs41(NCARCHITECT+41),rs42(NCARCHITECT+42);
 ResourceString rs43(NCARCHITECT+43),rs14(NCARCHITECT+14);
 ResourceString rs103(NCARCHITECT+103),rs0(NCVIEWA);
 double D,floorheight,xspacing,yspacing,angle,crossangle,transparency;
 Transform t;
 double wwidth;
 BitMask o(32),visible(MaxViews);
 Dialog dialog("ArFloorOptions_dialog");
 DialogControl *dc;
 RCCHAR name[300],*at;

  *cstate = ExitState;
  strcpy(name,v.getstring("ar::floorname"));
  floorheight = v.getreal("ar::floorheight");
  xspacing = v.getreal("ar::floorxspacing");
  yspacing = v.getreal("ar::flooryspacing");
  angle = v.getreal("ar::floorangle");
  crossangle = v.getreal("ar::floorcrossangle");

  dialog.add(new FNListDialogControl(100,name,sizeof(name)));
  dialog.add(dc = new RealDialogControl(101,&floorheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&xspacing,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&yspacing,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&angle));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&crossangle));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  if (dialog.process() == FALSE) return;
  v.setstring("ar::floorname",name);
  v.setreal("ar::floorheight",floorheight);
  v.setreal("ar::floorxspacing",xspacing);
  v.setreal("ar::flooryspacing",yspacing);
  v.setreal("ar::floorangle",angle);
  v.setreal("ar::floorcrossangle",crossangle);
  transparency = 0.0;
  
  visible.clearall();
  if (db.views3d.match(rs103.gets()) != 0)
    visible.set(db.views3d.match(rs103.gets())->getviewno());
  if (db.views3d.match(rs0.gets()) != 0)
    visible.set(db.views3d.match(rs0.gets())->getviewno());

  blist = state.getselection().copy();
  if (blist.length() == 0)
    {  cadwindow->MessageBox(rs41.gets(),rs42.gets(),MB_ICONINFORMATION);
       *cstate = ExitState;
       return;
    }

  wwidth = 0.0;
  za.setxyz(0.0,0.0,floorheight);
  for (blist.start() ; (e = blist.next()) != 0 ; )
    {  if (! e->isa(wall_entity) && ! e->isa(line_entity) && ! e->isa(circle_entity))
         {  cadwindow->MessageBox(rs43.gets(),rs42.gets(),MB_ICONINFORMATION);
            wlist.destroy();
            return;
         }
       if (e->isa(wall_entity))
         {  za = ((Wall *)e)->getzaxis()*floorheight;
            wwidth = ((Wall *)e)->gettotalwidth();
            wlist.add(e);
         }
       else
           hlist.add(e);
    }


  if (! wlist.empty())
    {  for (wlist.start() ; (e = wlist.next()) != 0 ; )
         {  line = new Line(((Wall *)e)->getp1() + za,((Wall *)e)->getp2()+za);
            line->setvisible(visible);
            llist1.add(line);
            llist2.add(line);
         }
       Loops loops(1);
       loops.create(&llist1);

       if (! llist1.empty() || loops.getloops().empty() || ! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
         {  cadwindow->MessageBox(rs14.gets(),rs42.gets(),MB_ICONINFORMATION);
            blist.destroy();
            wlist.destroy();
            hlist.destroy();
            llist1.destroy();
            llist2.destroy();
            loops.destroy();
            return;
         }

       db.saveundo(UD_STARTBLOCK,NULL);

       loops.offset(0,0,p,wwidth/2.0,&olist1);
       loops.offset(0,0,p,-wwidth/2.0,&olist2);

       if (zaxis.z < 0.0) yaxis = -yaxis;

       Polygon3d poly1(origin,xaxis,yaxis,olist1,10,0.0);
       Polygon3d poly2(origin,xaxis,yaxis,olist2,10,0.0);

       if (fabs(poly1.getarea()) > fabs(poly2.getarea()))
         {  for (olist1.start() ; (e = olist1.next()) != 0 ; )
              {  e->setvisible(visible);
                 db.geometry.add(e);
              }
            for (olist2.start() ; (e = olist2.next()) != 0 ; )
              delete e;
            olist2.destroy();
         }
       else
         {  for (olist1.start() ; (e = olist1.next()) != 0 ; )
              delete e;
            for (olist2.start() ; (e = olist2.next()) != 0 ; )
              {  e->setvisible(visible);
                 db.geometry.add(e);
              }
            olist1.destroy();
            olist1 = olist2;
         }
    }
  else
    db.saveundo(UD_STARTBLOCK,NULL);

  for (hlist.start() ; (e = hlist.next()) != 0 ; )
    {  if (e->isa(line_entity))
         line = new Line(((Line *)e)->getp1() + za,((Wall *)e)->getp2()+za);
       else if (e->isa(circle_entity))
         line = new Circle(((Circle *)e)->getorigin() + za,
                           ((Circle *)e)->getxaxis(),((Circle *)e)->getyaxis(),
                           ((Circle *)e)->getradius(),((Circle *)e)->getstarta(),((Circle *)e)->getsweepa());
       line->setvisible(visible);
       olist1.add(line);
    }

  Loops loops1(1);
  loops1.create(&olist1);

  if (! olist1.empty() || loops1.getloops().empty() || ! loops1.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
    {  cadwindow->MessageBox(rs14.gets(),rs42.gets(),MB_ICONINFORMATION);
       blist.destroy();
       wlist.destroy();
       hlist.destroy();
       olist1.destroy();
       llist1.destroy();
       llist2.destroy();
       loops1.destroy();
       db.saveundo(UD_ENDBLOCK,NULL);
       return;
    }


//  Now insert the floor
  Plane *plane = new Plane(loops1.getloops(),xspacing,yspacing,angle,crossangle,transparency,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
  if (plane != 0 && plane->getdefined())
    {  plane->setvisible(visible);
       db.geometry.add(plane);
       if ((at = new RCCHAR[strlen(name) + 18]) != NULL)
         {  strcpy(at,"ar::Cost::Floor::");
            strcat(at,name);
            plane->addat(at);
            plane->setattribute(at);
            db.saveundo(UD_INSERTATT,plane);
         }
    }

  db.saveundo(UD_ENDBLOCK,NULL);
}

class CNListDialogControl : public ListDialogControl
  {public:
     CNListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void CNListDialogControl::load(Dialog *dialog)
{RCCHAR name[300],key[300];
 int status;
  ListDialogControl::load(dialog);
  status = v.getfirststring(_RCT("ar::Cost::Ceiling::"),key,name);
  while (status)
    {  ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,key+19);
       status = v.getnextstring(_RCT("ar::Cost::Ceiling::"),key,name);
    }
}

void architect_command19(int *cstate,HighLevelEvent *,void **)
{EntityList blist,wlist,hlist,llist1,llist2,olist1,olist2,list,elist;
 GeneralList plist,vlist1,vlist2;
 Entity *e,*line;
 Point p(1.0E10,2.3E10,0.0),origin,xaxis,yaxis,zaxis(0.0,0.0,1.0),za(0.0,0.0,0.0);
 BitMask options(32),visible(MaxViews);
 ResourceString rs44(NCARCHITECT+44),rs45(NCARCHITECT+45);
 ResourceString rs46(NCARCHITECT+46),rs14(NCARCHITECT+14);
 ResourceString rs103(NCARCHITECT+103),rs0(NCVIEWA);
 double D,ceilingheight,xspacing,yspacing,angle,crossangle,transparency;
 Transform t;
 double wwidth;
 BitMask o(32);
 Dialog dialog("ArCeiling_dialog");
 DialogControl *dc;
 RCCHAR name[300],*at;

  *cstate = ExitState;
  strcpy(name,v.getstring("ar::ceilingname"));
  ceilingheight = v.getreal("ar::ceilingheight");
  xspacing = v.getreal("ar::ceilingxspacing");
  yspacing = v.getreal("ar::ceilingyspacing");
  angle = v.getreal("ar::ceilingangle");
  crossangle = v.getreal("ar::ceilingcrossangle");

  dialog.add(new CNListDialogControl(100,name,sizeof(name)));
  dialog.add(dc = new RealDialogControl(101,&ceilingheight));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&xspacing,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&yspacing,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&angle));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&crossangle));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  if (dialog.process() == FALSE) return;
  v.setstring("ar::ceilingname",name);
  v.setreal("ar::ceilingheight",ceilingheight);
  v.setreal("ar::ceilingxspacing",xspacing);
  v.setreal("ar::ceilingyspacing",yspacing);
  v.setreal("ar::ceilingangle",angle);
  v.setreal("ar::ceilingcrossangle",crossangle);
  transparency = 0.0;

  visible.clearall();
  if (db.views3d.match(rs103.gets()) != 0)
    visible.set(db.views3d.match(rs103.gets())->getviewno());
  if (db.views3d.match(rs0.gets()) != 0)
    visible.set(db.views3d.match(rs0.gets())->getviewno());

  blist = state.getselection().copy();
  if (blist.length() == 0)
    {  cadwindow->MessageBox(rs44.gets(),rs45.gets(),MB_ICONINFORMATION);
       *cstate = ExitState;
       return;
    }

  wwidth = 0.0;
  za.setxyz(0.0,0.0,ceilingheight);
  for (blist.start() ; (e = blist.next()) != 0 ; )
    {  if (! e->isa(wall_entity) && ! e->isa(line_entity) && ! e->isa(circle_entity))
         {  cadwindow->MessageBox(rs46.gets(),rs45.gets(),MB_ICONINFORMATION);
            blist.destroy();
            return;
         }
       if (e->isa(wall_entity))
         {  za = ((Wall *)e)->getzaxis()* ceilingheight;
            wwidth = ((Wall *)e)->gettotalwidth();
            wlist.add(e);
         }
       else
         hlist.add(e);
    }

  if (! wlist.empty())
    {  for (wlist.start() ; (e = wlist.next()) != 0 ; )
         {  line = new Line(((Wall *)e)->getp1() + za,((Wall *)e)->getp2()+za);
            line->setvisible(visible);
            llist1.add(line);
            llist2.add(line);
         }

       Loops loops(1);
       loops.create(&llist1);

       if (! llist1.empty() || loops.getloops().empty() || ! loops.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
         {  cadwindow->MessageBox(rs14.gets(),rs45.gets(),MB_ICONINFORMATION);
            wlist.destroy();
            llist1.destroy();
            llist2.destroy();
            loops.destroy();
            return;
         }

       db.saveundo(UD_STARTBLOCK,NULL);

       loops.offset(0,0,p,wwidth/2.0,&olist1);
       loops.offset(0,0,p,-wwidth/2.0,&olist2);

       if (zaxis.z < 0.0) yaxis = -yaxis;

       Polygon3d poly1(origin,xaxis,yaxis,olist1,10,0.0);
       Polygon3d poly2(origin,xaxis,yaxis,olist2,10,0.0);

       if (fabs(poly1.getarea()) < fabs(poly2.getarea()))
         {  for (olist1.start() ; (e = olist1.next()) != 0 ; )
              {  e->setvisible(visible);
                 db.geometry.add(e);
              }
            for (olist2.start() ; (e = olist2.next()) != 0 ; )
              delete e;
            olist2.destroy();
         }
       else
         {  for (olist1.start() ; (e = olist1.next()) != 0 ; )
              delete e;
            for (olist2.start() ; (e = olist2.next()) != 0 ; )
              {  e->setvisible(visible);
                 db.geometry.add(e);
              }
            olist1.destroy();
            olist1 = olist2;  olist2 = elist;
         }
    }
  else
    db.saveundo(UD_STARTBLOCK,NULL);

  for (hlist.start() ; (e = hlist.next()) != 0 ; )
    {  if (e->isa(line_entity))
         line = new Line(((Line *)e)->getp1() + za,((Wall *)e)->getp2()+za);
       else if (e->isa(circle_entity))
         line = new Circle(((Circle *)e)->getorigin() + za,
                           ((Circle *)e)->getxaxis(),((Circle *)e)->getyaxis(),
                           ((Circle *)e)->getradius(),((Circle *)e)->getstarta(),((Circle *)e)->getsweepa());
       line->setvisible(visible);
       olist1.add(line);
    }

  Loops loops1(1);
  loops1.create(&olist1);

  if (! olist1.empty() || loops1.getloops().empty() || ! loops1.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
    {  cadwindow->MessageBox(rs14.gets(),rs45.gets(),MB_ICONINFORMATION);
       blist.destroy();
       wlist.destroy();
       hlist.destroy();
       olist1.destroy();
       llist1.destroy();
       llist2.destroy();
       loops1.destroy();
       db.saveundo(UD_ENDBLOCK,NULL);
       return;
    }

  Plane *plane = new Plane(loops1.getloops(),xspacing,yspacing,angle,crossangle,transparency,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
  if (plane != 0 && plane->getdefined())
    {  plane->setvisible(visible);
       db.geometry.add(plane);
       if ((at = new RCCHAR[strlen(name) + 20]) != 0)
         {  strcpy(at,"ar::Cost::Ceiling::");
            strcat(at,name);
            plane->addat(at);
            plane->setattribute(at);
            db.saveundo(UD_INSERTATT,plane);
         }
    }

  db.saveundo(UD_ENDBLOCK,NULL);
}

struct RBArchitect8_data
  {Point p1,p2,xaxis,yaxis,zaxis;
   double width,height,rise,run,distance,landing,diameter;
   int layout;
   int state;
  };

void RBArchitect8(int,void *data,Point *p2,View3dSurface *)
{RBArchitect8_data *rbdata = (RBArchitect8_data *) data;
 double w,h,r1,r2,sina,a,da;
 int i,nsteps;
 Point p1,p22,p3,p4,p5,p6,p7,p8,p9,p10;
  if (rbdata->state == 2)
    {  Line line(rbdata->p1,*p2);
       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {  nsteps = int(rbdata->height / rbdata->rise + 0.999);
       if (nsteps < 1) nsteps = 1;
       rbdata->yaxis = (rbdata->p2 - rbdata->p1).normalize();
       rbdata->xaxis = rbdata->yaxis.cross(((*p2 - rbdata->p1).cross(rbdata->yaxis))).normalize();
       w = rbdata->width;
       h = (nsteps - 1) * rbdata->run;
       for (i = 0 ; i < nsteps ; i++)
         {  Line line1(rbdata->p1+rbdata->yaxis*(i*h/(nsteps-1)),rbdata->p1+rbdata->yaxis*(i*h/(nsteps-1))+rbdata->xaxis*w);
            line1.draw(DM_INVERT);
         }
       Line line2(rbdata->p1,rbdata->p1+rbdata->yaxis*h);
       line2.draw(DM_INVERT);
       Line line3(rbdata->p1+rbdata->xaxis*w,rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*h);
       line3.draw(DM_INVERT);
    }
  else if (rbdata->state == 5)
    {  Line line(rbdata->p1,*p2);
       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 6)
    {  nsteps = ((int(rbdata->height / rbdata->rise + 0.999)+1)/2) * 2;
       if (nsteps < 2) nsteps = 2;
       rbdata->xaxis = (rbdata->p2 - rbdata->p1).normalize();
       rbdata->yaxis = (rbdata->xaxis.cross(*p2 - rbdata->p1)).cross(rbdata->xaxis).normalize();
       w = rbdata->width * 2.0 + rbdata->distance;
       h = (nsteps/2-1) * rbdata->run + rbdata->landing;
       p1 = rbdata->p1;
       p22 = rbdata->p1 + rbdata->xaxis * h;
       p3 = rbdata->p1 + rbdata->xaxis * h + rbdata->yaxis * w;
       p4 = rbdata->p1 + rbdata->yaxis * w;
       p5 = rbdata->p1 + rbdata->yaxis * (w - rbdata->width);
       p6 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run + rbdata->yaxis * (w - rbdata->width);
       p7 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run + rbdata->yaxis * (w - (rbdata->width+rbdata->distance));
       p8 = rbdata->p1 + rbdata->yaxis * (w-(rbdata->width+rbdata->distance));
       p9 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run;
       p10 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run + rbdata->yaxis * w;
       Line line1(p1,p22);  line1.draw(DM_INVERT);
       Line line2(p22,p3);  line2.draw(DM_INVERT);
       Line line3(p3,p4);  line3.draw(DM_INVERT);
       Line line4(p5,p6);  line4.draw(DM_INVERT);
       Line line5(p9,p10); line5.draw(DM_INVERT);
       Line line6(p7,p8);  line6.draw(DM_INVERT);
       for (i = 0 ; i < nsteps / 2 - 1 ; i++)
         {  Line line1(p1 + (p9 - p1) * double(i) / double(nsteps/2-1),p8 + (p7 - p8) * double(i) / double(nsteps/2-1));
            line1.draw(DM_INVERT);
            Line line2(p5 + (p6 - p5) * double(i) / double(nsteps/2-1),p4 + (p10 - p4) * double(i) / double(nsteps/2-1));
            line2.draw(DM_INVERT);
         }
    }
  else if (rbdata->state == 8)
    {  Line line(rbdata->p1,*p2);
       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 9)
    {  nsteps = int(rbdata->height / rbdata->rise + 0.999);
       if (nsteps < 1) nsteps = 1;
       rbdata->xaxis = (rbdata->p2 - rbdata->p1).normalize();
       rbdata->yaxis = (rbdata->xaxis.cross(*p2 - rbdata->p1)).cross(rbdata->xaxis).normalize();
       Circle circle(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->diameter/2.0,0.0,2.0*M_PI);
       r1 = rbdata->diameter / 2.0;
       r2 = rbdata->diameter / 2.0 + rbdata->width;
       sina = rbdata->run/2.0/(rbdata->diameter/2.0 + rbdata->width);
       if (sina < -1.0) sina = -1.0; else if (sina > 1.0) sina = 1.0;
       da = asin(sina) * 2.0;
       p1 = rbdata->p1 + rbdata->xaxis * r1;
       p22 = rbdata->p1 + rbdata->xaxis * r2;
       Line line1(p1,p22);  line1.draw(DM_INVERT);
       for (i = 0,a = da ; i < nsteps - 1 ; i++,a += da)
         {  p3 = rbdata->p1 + rbdata->xaxis * r1*cos(a) + rbdata->yaxis * r1 * sin(a);
            p4 = rbdata->p1 + rbdata->xaxis * r2*cos(a) + rbdata->yaxis * r2 * sin(a);
            Line line1(p3,p4);  line1.draw(DM_INVERT);
            Line line2(p22,p4);  line2.draw(DM_INVERT);
            p22 = p4;
         }
    }
}

class StairDisplayDialogControl : public DisplayDialogControl
  {private:
     double scale;
   public:
     StairDisplayDialogControl(int id) : DisplayDialogControl(id) {  scale = 0.0;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

void StairDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),p1,p2,p3,p4,p5,p6,p7,p8,p9,p10;
 BitMask options(32);
 double r1,r2,du,dv,ox,oy,width,height,rise,run,distance,landing,diameter,w1,h1,w2,h2,w3,h3,s,a,da,sina;
 int i,layout,n;
  if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
#if ! defined(_WIN32_WCE)
//WCEFIX
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);
#endif
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(100,&layout) &&
           dialog->currentvalue(101,&width) &&
           dialog->currentvalue(102,&height) &&
           dialog->currentvalue(103,&rise) &&
           dialog->currentvalue(104,&run) &&
           dialog->currentvalue(105,&distance) &&
           dialog->currentvalue(106,&landing) &&
           dialog->currentvalue(107,&diameter))
         {Point p1,p2;
          BitMask options(32);
            du = output.getumax() - output.getumin();
            dv = output.getvmax() - output.getvmin();
            if (scale == 0.0)
              {  n = int(height / rise + 0.01);
                 if (n < 1) n = 1;
                 w1 = (n - 1) * run;
                 h1 = width;
                 n = ((n + 1) / 2) * 2;
                 w2 = (n / 2 - 1) * run + landing;
                 h2 = width * 2.0 + distance;
                 w3 = diameter + width * 2.0;
                 h3 = diameter + width * 2.0;
                 if (w2 > w1) w1 = w2;
                 if (w3 > w1) w1 = w3;
                 if (h2 > h1) h1 = h2;
                 if (h3 > h1) h1 = h3;
                 if (w1 < 1.0) w1 = 1.0;
                 if (h1 < 1.0) h1 = 1.0;
                 scale = du / w1 * 0.8;
                 s = dv / h1 * 0.8;
                 if (s < scale) scale = s;
              }
            if (layout == 0)
              {  n = int(height / rise + 0.999);
                 if (n < 1) n = 1;
                 w1 = (n - 1) * run * scale;
                 h1 = width * scale;
                 ox = (du - w1) / 2.0 + output.getumin();
                 oy = (dv - h1) / 2.0 + output.getvmin();
                 p1.setxyz(ox,oy,0.0);     p2.setxyz(ox+w1,oy,0.0);
                 p3.setxyz(ox,oy+h1,0.0);  p4.setxyz(ox+w1,oy+h1,0.0);
                 Line line1(p1,p2);
                 line1.draw(DM_NORMAL,0,&output);
                 Line line2(p3,p4);
                 line2.draw(DM_NORMAL,0,&output);
                 for (i = 0 ; i < n ; i++)
                   {  Line line(p1 + (p2 - p1) * double(i) / double(n-1),p3 + (p4 - p3) * double(i) / double(n-1));
                      line.draw(DM_NORMAL,0,&output);
                   }
              }
            else if (layout == 1)
              {  n = ((int(height / rise + 0.999)+1)/2) * 2;
                 if (n < 2) n = 2;
                 w1 = ((n/2 - 1) * run + landing) * scale;
                 h1 = (width * 2.0 + distance) * scale;
                 ox = (du - w1) / 2.0 + output.getumin();
                 oy = (dv - h1) / 2.0 + output.getvmin();
                 p1.setxyz(ox,oy,0.0);                            p2.setxyz(ox+w1,oy,0.0);
                 p3.setxyz(ox+w1,oy+h1,0.0);                      p4.setxyz(ox,oy+h1,0.0);
                 p5.setxyz(ox,oy+h1-width*scale,0.0);             p6.setxyz(ox+(n/2-1)*run*scale,oy+h1-width*scale,0.0);
                 p7.setxyz(ox+(n/2-1)*run*scale,oy+h1-(width+distance)*scale,0.0);  p8.setxyz(ox,oy+h1-(width+distance)*scale,0.0);
                 p9.setxyz(ox+(n/2-1)*run*scale,oy,0.0);          p10.setxyz(ox+(n/2-1)*run*scale,oy+h1,0.0);
                 Line line1(p1,p2);  line1.draw(DM_NORMAL,0,&output);
                 Line line2(p2,p3);  line2.draw(DM_NORMAL,0,&output);
                 Line line3(p3,p4);  line3.draw(DM_NORMAL,0,&output);
                 Line line4(p5,p6);  line4.draw(DM_NORMAL,0,&output);
                 Line line5(p9,p10);  line5.draw(DM_NORMAL,0,&output);
                 Line line6(p7,p8);  line6.draw(DM_NORMAL,0,&output);
                 for (i = 0 ; i < n/2-1 ; i++)
                   {  Line line1(p1 + (p9 - p1) * double(i) / double(n/2-1),p8 + (p7 - p8) * double(i) / double(n/2-1));
                      line1.draw(DM_NORMAL,0,&output);
                      Line line2(p5 + (p6 - p5) * double(i) / double(n/2-1),p4 + (p10 - p4) * double(i) / double(n/2-1));
                      line2.draw(DM_NORMAL,0,&output);
                   }
              }
            else if (layout == 2)
              {  n = int(height / rise + 0.999);
                 if (n < 1) n = 1;
                 ox = du / 2.0 + output.getumin();
                 oy = dv / 2.0 + output.getvmin();
                 p1.setxyz(ox,oy,0.0);
                 Circle circle(p1,xaxis,yaxis,diameter/2.0*scale,0.0,2.0*M_PI);
                 r1 = diameter / 2.0 * scale;
                 r2 = (diameter / 2.0 + width) * scale;
                 sina = run/2.0/(diameter/2.0 + width);
                 if (sina < -1.0) sina = -1.0; else if (sina > 1.0) sina = 1.0;
                 da = asin(sina) * 2.0;
                 p1.setxyz(ox+r1,oy,0.0);  p2.setxyz(ox+r2,oy,0.0);
                 Line line1(p1,p2);  line1.draw(DM_NORMAL,0,&output);
                 for (i = 0,a = da ; i < n - 1 ; i++,a += da)
                   {  p3.setxyz(ox+r1*cos(a),oy+r1*sin(a),0.0);
                      p4.setxyz(ox+r2*cos(a),oy+r2*sin(a),0.0);
                      Line line1(p3,p4);  line1.draw(DM_NORMAL,0,&output);
                      Line line2(p2,p4);  line2.draw(DM_NORMAL,0,&output);
                      p2 = p4;
                   }
              }
         }
    }
}

class StairListDialogControl : public ListDialogControl
  {public:
     StairListDialogControl(int id,int n,RCCHAR **l,int *v) : ListDialogControl(id,n,l,v) {}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void StairListDialogControl::load(Dialog *dialog)
{ ListDialogControl ::load(dialog);
  changefocus(dialog);
}

void StairListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int index;
  currentvalue(dialog,&index);
  EnableWindow(GetDlgItem(dialog->gethdlg(),105),index == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),205),index == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),305),index == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),index == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),306),index == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),index == 2);
  EnableWindow(GetDlgItem(dialog->gethdlg(),207),index == 2);
  EnableWindow(GetDlgItem(dialog->gethdlg(),307),index == 2);
  DialogControl::changefocus(dialog,updatescreen);
}

void architect_command8(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect8_data *rbdata = (RBArchitect8_data *) *data;
 Point p1,p2,p3,p4,p5,p6,zaxis;
 int i,nsteps;
 Line *line1,*line2,*line3,*line4,*line5,*line6,*line7;
 Circle *arc1,*arc2;
 double w,h,r1,r2,sina,a,da;
 BitMask o(32);
 EntityList list,emptylist,list1,list2,list3,list4,list5,list6,list7;
 Plane *plane;
 RuledSurface *rs;
 RCCHAR *layoutl[3];
 ResourceString rs94(NCARCHITECT+94),rs95(NCARCHITECT+95),rs96(NCARCHITECT+96);
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect8_data;  *data = rbdata;
       if (rbdata != 0)
         {Dialog dialog("ArStairs_dialog");
          DialogControl *dc;
            layoutl[0] = rs94.gets();
            layoutl[1] = rs95.gets();
            layoutl[2] = rs96.gets();
            rbdata->layout = v.getinteger("ar::stairlayout");
            rbdata->width = v.getreal("ar::stairwidth");
            rbdata->height = v.getreal("ar::stairheight");
            rbdata->rise = v.getreal("ar::stairrise");
            rbdata->run = v.getreal("ar::stairrun");
            rbdata->distance = v.getreal("ar::stairdistance");
            rbdata->landing = v.getreal("ar::stairlanding");
            rbdata->diameter = v.getreal("ar::stairdiameter");
            dialog.add(new StairListDialogControl(100,3,layoutl,&rbdata->layout));
            dialog.add(dc = new RealDialogControl(101,&rbdata->width,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1101,dc));
            dialog.add(dc = new RealDialogControl(102,&rbdata->height,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1102,dc));
            dialog.add(dc = new RealDialogControl(103,&rbdata->rise,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1103,dc));
            dialog.add(dc = new RealDialogControl(104,&rbdata->run,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1104,dc));
            dialog.add(dc = new RealDialogControl(105,&rbdata->distance,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1105,dc));
            dialog.add(dc = new RealDialogControl(106,&rbdata->landing,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1106,dc));
            dialog.add(dc = new RealDialogControl(107,&rbdata->diameter,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1107,dc));
            dialog.add(new StairDisplayDialogControl(108));
            if (dialog.process() == TRUE)
              {  v.setinteger("ar::stairlayout",rbdata->layout);
                 v.setreal("ar::stairwidth",rbdata->width);
                 v.setreal("ar::stairheight",rbdata->height);
                 v.setreal("ar::stairrise",rbdata->rise);
                 v.setreal("ar::stairrun",rbdata->run);
                 v.setreal("ar::stairdistance",rbdata->distance);
                 v.setreal("ar::stairlanding",rbdata->landing);
                 v.setreal("ar::stairdiameter",rbdata->diameter);
                 if (rbdata->layout == 0)
                   *cstate = 1;
                 else if (rbdata->layout == 1)
                   *cstate = 4;
                 else if (rbdata->layout == 2)
                   *cstate = 7;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
     case 4 :
     case 7 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            (*cstate)++;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
     case 5 :
     case 8 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
            (*cstate)++;
         }
       else if (event->isa(Abort))
         (*cstate)--;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p3 = ((Coordinate3dEvent *) event)->getp();
            nsteps = int(rbdata->height / rbdata->rise + 0.999);
            if (nsteps < 0) nsteps = 1;
            rbdata->yaxis = (rbdata->p2 - rbdata->p1).normalize();
            rbdata->xaxis = rbdata->yaxis.cross(((p3 - rbdata->p1).cross(rbdata->yaxis))).normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis).normalize() * rbdata->height / nsteps;
            if (zaxis.z < 0.0) zaxis = -zaxis;
            w = rbdata->width;
            h = rbdata->run * (nsteps-1);
            rbdata->p1 += rbdata->yaxis * h;
            rbdata->yaxis = -rbdata->yaxis;
            db.saveundo(UD_STARTBLOCK,NULL);
            line1 = new Line(rbdata->p1,rbdata->p1+rbdata->xaxis*w);
            db.geometry.add(line1);
            for (i = 1 ; i <= nsteps ; i++)
              {  line2 = new Line(rbdata->p1+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*i,rbdata->p1+rbdata->yaxis*((i-1)*h/(nsteps-1))+rbdata->xaxis*w+zaxis*i);
                 db.geometry.add(line2);
                 line4 = new Line(rbdata->p1+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*(i-1),rbdata->p1+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*i);
                 db.geometry.add(line4);
                 line6 = new Line(rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*(i-1),rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*i);
                 db.geometry.add(line6);

                 list = emptylist;  list.add(line1);  list.add(line4);  list.add(line2);  list.add(line6);
                 plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 if (plane != 0 && plane->getdefined())
                   db.geometry.add(plane);

                 if (i < nsteps)
                   {  line3 = new Line(rbdata->p1+rbdata->yaxis*(i*h/(nsteps-1))+zaxis*i,rbdata->p1+rbdata->yaxis*(i*h/(nsteps-1))+rbdata->xaxis*w+zaxis*i);
                      db.geometry.add(line3);
                      line5 = new Line(rbdata->p1+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*i,rbdata->p1+rbdata->yaxis*(i*h/(nsteps-1))+zaxis*i);
                      db.geometry.add(line5);
                      line7 = new Line(rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*((i-1)*h/(nsteps-1))+zaxis*i,rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*(i*h/(nsteps-1))+zaxis*i);
                      db.geometry.add(line7);

                      list = emptylist;  list.add(line5);  list.add(line3);  list.add(line7);  list.add(line2);
                      plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                      if (plane != 0 && plane->getdefined())
                        db.geometry.add(plane);

                      list1.add(line4);  list1.add(line5);
                      list2.add(line6);  list2.add(line7);
                   }

                 line1 = line3;
              }

            line4 = new Line(rbdata->p1+rbdata->yaxis*h+zaxis*(nsteps-1),rbdata->p1+rbdata->yaxis*h);
            db.geometry.add(line4);
            line5 = new Line(rbdata->p1+rbdata->yaxis*h,rbdata->p1);
            db.geometry.add(line5);

            line6 = new Line(rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*h+zaxis*(nsteps-1),rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*h);
            db.geometry.add(line6);
            line7 = new Line(rbdata->p1+rbdata->xaxis*w+rbdata->yaxis*h,rbdata->p1+rbdata->xaxis*w);
            db.geometry.add(line7);

            list1.add(line4);  list1.add(line5);
            list2.add(line6);  list2.add(line7);

            plane = new Plane(list1,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list2,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            db.saveundo(UD_ENDBLOCK,NULL);

            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 6 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p3 = ((Coordinate3dEvent *) event)->getp();
            nsteps = ((int(rbdata->height / rbdata->rise + 0.999)+1)/2) * 2;
            if (nsteps < 2) nsteps = 2;
            rbdata->xaxis = (rbdata->p2 - rbdata->p1).normalize();
            rbdata->yaxis = (rbdata->xaxis.cross(p3 - rbdata->p1)).cross(rbdata->xaxis).normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis).normalize() * rbdata->height / nsteps;
            if (zaxis.z < 0.0) zaxis = -zaxis;
            w = rbdata->width * 2.0 + rbdata->distance;
            rbdata->p1 += rbdata->yaxis * w;
            rbdata->yaxis = -rbdata->yaxis;
            db.saveundo(UD_STARTBLOCK,NULL);
            line1 = new Line(rbdata->p1,rbdata->p1+rbdata->yaxis*rbdata->width);
            db.geometry.add(line1);
            for (i = 1 ; i <= nsteps / 2 ; i++)
              {  line2 = new Line(rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+zaxis*i,rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*i);
                 db.geometry.add(line2);
                 line4 = new Line(rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+zaxis*(i-1),rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+zaxis*i);
                 db.geometry.add(line4);
                 line6 = new Line(rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*(i-1),rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*i);
                 db.geometry.add(line6);

                 list = emptylist;  list.add(line1);  list.add(line4);  list.add(line2);  list.add(line6);
                 plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 if (plane != 0 && plane->getdefined())
                   db.geometry.add(plane);

                 if (i < nsteps / 2)
                   {  line3 = new Line(rbdata->p1+rbdata->xaxis*i*rbdata->run+zaxis*i,rbdata->p1+rbdata->xaxis*i*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*i);
                      db.geometry.add(line3);
                      line5 = new Line(rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+zaxis*i,rbdata->p1+rbdata->xaxis*i*rbdata->run+zaxis*i);
                      db.geometry.add(line5);
                      line7 = new Line(rbdata->p1+rbdata->xaxis*(i-1)*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*i,rbdata->p1+rbdata->xaxis*i*rbdata->run+rbdata->yaxis*rbdata->width+zaxis*i);
                      db.geometry.add(line7);
                      list = emptylist;  list.add(line5);  list.add(line3);  list.add(line7);  list.add(line2);
                      plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                      if (plane != 0 && plane->getdefined())
                        db.geometry.add(plane);
                      list1.add(line4);  list1.add(line5);
                      list2.add(line6);  list2.add(line7);
                   }
                 else
                   list1.add(line4);
                 line1 = line3;
              }

            p6 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run + zaxis*(nsteps/2) + rbdata->yaxis * (w - rbdata->width);
            line1 = new Line(p6,p6+rbdata->yaxis*rbdata->width);
            db.geometry.add(line1);
            for (i = 1 ; i <= nsteps / 2 ; i++)
              {  line2 = new Line(p6-rbdata->xaxis*((i-1)*rbdata->run)+zaxis*i,p6-rbdata->xaxis*((i-1)*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*i);
                 db.geometry.add(line2);
                 line4 = new Line(p6-rbdata->xaxis*((i-1)*rbdata->run)+zaxis*(i-1),p6-rbdata->xaxis*((i-1)*rbdata->run)+zaxis*i);
                 db.geometry.add(line4);
                 line6 = new Line(p6-rbdata->xaxis*((i-1)*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*(i-1),p6-rbdata->xaxis*((i-1)*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*i);
                 db.geometry.add(line6);

                 list = emptylist;  list.add(line1);  list.add(line4);  list.add(line2);  list.add(line6);
                 plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 if (plane != 0 && plane->getdefined())
                   db.geometry.add(plane);

                 if (i < nsteps / 2)
                   {  line3 = new Line(p6-rbdata->xaxis*(i*rbdata->run)+zaxis*i,p6-rbdata->xaxis*(i*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*i);
                      db.geometry.add(line3);
                      line5 = new Line(p6-rbdata->xaxis*((i-1)*rbdata->run)+zaxis*(i),p6-rbdata->xaxis*(i*rbdata->run)+zaxis*i);
                      db.geometry.add(line5);
                      line7 = new Line(p6-rbdata->xaxis*((i-1)*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*i,p6-rbdata->xaxis*(i*rbdata->run)+rbdata->yaxis*rbdata->width+zaxis*i);
                      db.geometry.add(line7);

                      list = emptylist;  list.add(line5);  list.add(line3);  list.add(line7);  list.add(line2);
                      plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                      if (plane != 0 && plane->getdefined())
                        db.geometry.add(plane);

                      list3.add(line4);  list3.add(line5);
                      list4.add(line6);  list4.add(line7);
                   }
                 else
                   list6.add(line1);
                 line1 = line3;
              }

            p6 = rbdata->p1 + rbdata->xaxis * (nsteps/2-1)*rbdata->run + zaxis*(nsteps/2);
            line1 = new Line(p6,p6+rbdata->yaxis*w);
            db.geometry.add(line1);
            line2 = new Line(p6+rbdata->yaxis*w,p6+rbdata->yaxis*w+rbdata->xaxis*rbdata->landing);
            db.geometry.add(line2);
            line3 = new Line(p6+rbdata->yaxis*w+rbdata->xaxis*rbdata->landing,p6+rbdata->xaxis*rbdata->landing);
            db.geometry.add(line3);
            line4 = new Line(p6+rbdata->xaxis*rbdata->landing,p6);
            db.geometry.add(line4);

            list1.add(line4);
            list5.add(line3);
            list4.reverse();  list4.add(line2);

            list = emptylist;  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
            plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            p6 = rbdata->p1 + rbdata->xaxis * ((nsteps/2-1)*rbdata->run + rbdata->landing);
            line1 = new Line(p6 + zaxis*(nsteps/2),p6);
            db.geometry.add(line1);
            list1.add(line1);
            list5.add(line1);
            line1 = new Line(p6,rbdata->p1);
            db.geometry.add(line1);
            list1.add(line1);

            line1 = new Line(p6,p6 + rbdata->yaxis * w);
            db.geometry.add(line1);
            list5.add(line1);

            line1 = new Line(p6 + rbdata->yaxis * w,p6 + rbdata->yaxis * w + zaxis * (nsteps / 2));
            db.geometry.add(line1);
            list4.add(line1);
            list5.add(line1);

            line1 = new Line(p6 + rbdata->yaxis * w,rbdata->p1 + rbdata->yaxis * w);
            db.geometry.add(line1);
            list4.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->yaxis * w,rbdata->p1 + rbdata->yaxis * w + zaxis * (nsteps - 1));
            db.geometry.add(line1);
            list4.add(line1);
            list6.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->yaxis * w,rbdata->p1 + rbdata->yaxis * (w - rbdata->width));
            db.geometry.add(line1);
            list6.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->yaxis * (w - rbdata->width),rbdata->p1 + rbdata->yaxis * (w - rbdata->width) + zaxis * (nsteps - 1));
            db.geometry.add(line1);
            list6.add(line1);
            list3.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->yaxis * (w - rbdata->width),rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * (w - rbdata->width));
            db.geometry.add(line1);
            list3.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * (w - rbdata->width),rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * (w - rbdata->width) + zaxis * (nsteps / 2));
            db.geometry.add(line1);
            list3.add(line1);
            list7.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * (w - rbdata->width) + zaxis * (nsteps / 2),rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width + zaxis * (nsteps / 2));
            db.geometry.add(line1);
            list7.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width + zaxis * (nsteps / 2),rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width);
            db.geometry.add(line1);
            list7.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width + zaxis * (nsteps / 2-1),rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width);
            db.geometry.add(line1);
            list2.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width,rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * (w - rbdata->width));
            db.geometry.add(line1);
            list7.add(line1);

            line1 = new Line(rbdata->p1 + rbdata->xaxis * (nsteps/2-1) * rbdata->run + rbdata->yaxis * rbdata->width,rbdata->p1 + rbdata->yaxis * rbdata->width);
            db.geometry.add(line1);
            list2.add(line1);

            plane = new Plane(list1,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list2,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list3,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list4,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list6,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            plane = new Plane(list7,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
            if (plane != 0 && plane->getdefined())
              db.geometry.add(plane);

            db.saveundo(UD_ENDBLOCK,NULL);

            *cstate = 4;
         }
       else if (event->isa(Abort))
         *cstate = 4;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 9 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p3 = ((Coordinate3dEvent *) event)->getp();
            nsteps = int(rbdata->height / rbdata->rise + 0.999);
            if (nsteps < 1) nsteps = 1;
            rbdata->xaxis = (rbdata->p2 - rbdata->p1).normalize();
            rbdata->yaxis = (rbdata->xaxis.cross(p3 - rbdata->p1)).cross(rbdata->xaxis).normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis).normalize();
            if (zaxis.z < 0.0)
              zaxis = -zaxis;
            r1 = rbdata->diameter / 2.0;
            r2 = rbdata->diameter / 2.0 + rbdata->width;
            sina = rbdata->run/2.0/(rbdata->diameter/2.0 + rbdata->width);
            if (sina < -1.0) sina = -1.0; else if (sina > 1.0) sina = 1.0;
            da = asin(sina) * 2.0;
            p1 = rbdata->p1 + rbdata->xaxis * r1;
            p2 = rbdata->p1 + rbdata->xaxis * r2;

            db.saveundo(UD_STARTBLOCK,NULL);

            arc1 = new Circle(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->diameter/2.0,0.0,2.0*M_PI);
            db.geometry.add(arc1);
            arc2 = new Circle(rbdata->p1+zaxis*rbdata->height,rbdata->xaxis,rbdata->yaxis,rbdata->diameter/2.0,0.0,2.0*M_PI);
            db.geometry.add(arc2);
            rs = new RuledSurface(arc1,arc2,12,2,o);
            db.geometry.add(rs);

            for (i = 1,a = 0.0 ; i < nsteps ; i++,a += da)
              {  p1 = rbdata->p1 + rbdata->xaxis * r1*cos(a) + rbdata->yaxis * r1 * sin(a) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 p2 = rbdata->p1 + rbdata->xaxis * r2*cos(a) + rbdata->yaxis * r2 * sin(a) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 p3 = rbdata->p1 + rbdata->xaxis * r1*cos(a+da/2.0) + rbdata->yaxis * r1 * sin(a+da/2.0) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 p4 = rbdata->p1 + rbdata->xaxis * r2*cos(a+da/2.0) + rbdata->yaxis * r2 * sin(a+da/2.0) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 p5 = rbdata->p1 + rbdata->xaxis * r1*cos(a+da) + rbdata->yaxis * r1 * sin(a+da) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 p6 = rbdata->p1 + rbdata->xaxis * r2*cos(a+da) + rbdata->yaxis * r2 * sin(a+da) + zaxis * rbdata->height * double(nsteps-i) / double(nsteps);
                 arc1 = new Circle(1,p1,p3,p5);
                 db.geometry.add(arc1);
                 arc2 = new Circle(1,p2,p4,p6);
                 db.geometry.add(arc2);
                 line1 = new Line(p1,p2);
                 db.geometry.add(line1);
                 line2 = new Line(p5,p6);
                 db.geometry.add(line2);

                 list = emptylist;  list.add(line1);  list.add(arc1);  list.add(line2);  list.add(arc2);
                 plane = new Plane(list,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),o);
                 if (plane != 0 && plane->getdefined())
                   db.geometry.add(plane);

              }
            db.saveundo(UD_ENDBLOCK,NULL);
            *cstate = 7;
         }
       else if (event->isa(Abort))
         *cstate = 7;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate != ExitState)
    {  if (*cstate != 1 && *cstate != 4 &&*cstate != 7)
         {  rubberband.begin(0);
            rubberband.add(RBArchitect8,rbdata);
         }
       rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate < 4)
         cadwindow->prompt->normalprompt(NCARCHITECT+25+*cstate);
       else if (*cstate < 7)
         cadwindow->prompt->normalprompt(NCARCHITECT+93+*cstate);
       else
         cadwindow->prompt->normalprompt(NCARCHITECT+93+*cstate);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class FloorDialogControl : public DialogControl
  {private:
    int *value;
   public:
     FloorDialogControl(int i,int *v) : DialogControl(i)  {  value = v;  }
	  int type(void) {  return 999;  };
	  void load(Dialog *);
     int store(Dialog *);
     int validate(Dialog *);
	  void change(Dialog *,int);
     int currentvalue(Dialog *,int *);
     void vscrollevent(Dialog *,int,int,int);
     void changefocus(Dialog *,int updatescreen = 1);
  };

class CFListDialogControl : public ListDialogControl
  {public:
     CFListDialogControl(int i,RCCHAR *v) : ListDialogControl(i,0,0,v,300)  {  }
	  void load(Dialog *);
  };

void CFListDialogControl::load(Dialog *d)
{int floor;
 ResourceString rs19(NCARCHITECT+19),rs25(NCARCHITECT+25),rs106(NCARCHITECT+106);
 RCCHAR string[300];
  additem(d,rs106.gets());
  strcpy(svalue,rs106.gets());
  ListDialogControl::load(d);
  for (floor = 0 ; floor < 100 ; floor++)
    {  if (floor == 0)
         strcpy(string,rs19.gets());
       else
         sprintf(string,rs25.gets(),floor);
       if (db.views3d.match(string) != 0)
         additem(d,string);
    }
}

void FloorDialogControl::load(Dialog *dialog)
{RCCHAR string[128];
 ResourceString rs19(NCARCHITECT+19);
  if (*value == 0)
    strcpy(string,rs19.gets());
  else
    sprintf(string,"%d",*value);
  dialog->SetDlgItemText(id,string);
}

void FloorDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{int v;
 RCCHAR string[128];
 ResourceString rs19(NCARCHITECT+19);
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v--;
		 else if (scrollcode == SB_LINEUP)
         v++;
       if (v < -20) v = -20;
       if (v > 300) v = 300;
       if (v == 0)
         strcpy(string,rs19.gets());
       else
         sprintf(string,"%d",v);
       dialog->SetDlgItemText(id,string);
       changefocus(dialog);
    }
}

void FloorDialogControl::change(Dialog *dialog,int v)
{RCCHAR string[128];
 ResourceString rs19(NCARCHITECT+19);
  if (v == 0)
    strcpy(string,rs19.gets());
  else
    sprintf(string,"%d",v);
  dialog->SetDlgItemText(id,string);
}

int FloorDialogControl::store(Dialog *dialog)
{RCCHAR string[128],*eptr;
 int v;
 ResourceString rs19(NCARCHITECT+19),rs20(NCARCHITECT+20),rs21(NCARCHITECT+21);
  dialog->GetDlgItemText(id,string,128);
  _strupr(string);
  if (strcmp(string,_strupr(rs19.gets())) == 0)
    v = 0;
  else
    {  v = int(strtol(string,&eptr,10));
       if (*eptr != 0)
         {  dialog->MessageBox(rs20.gets(),_RCT(""),MB_ICONSTOP);
            return FALSE;
         }
    }
  if (v < -20 || v > 300)
    {  dialog->MessageBox(rs21.gets(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  *value = v;
  return TRUE;
}

int FloorDialogControl::validate(Dialog *dialog)
{RCCHAR string[128],*eptr;
 int v;
 ResourceString rs19(NCARCHITECT+19),rs20(NCARCHITECT+20),rs21(NCARCHITECT+21);
  dialog->GetDlgItemText(id,string,128);
  _strupr(string);
  if (strcmp(string,_strupr(rs19.gets())) == 0)
    v = 0;
  else
    {  v = int(strtol(string,&eptr,10));
       if (*eptr != 0)
         {  dialog->MessageBox(rs20.gets(),_RCT(""),MB_ICONSTOP);
            return FALSE;
         }
    }
  if (v < -20 || v > 300)
    {  dialog->MessageBox(rs21.gets(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  return TRUE;
}

int FloorDialogControl::currentvalue(Dialog *dialog,int *newvalue)
{RCCHAR string[128],*eptr;
 int v;
 ResourceString rs19(NCARCHITECT+19),rs20(NCARCHITECT+20),rs21(NCARCHITECT+21);
  dialog->GetDlgItemText(id,string,128);
  _strupr(string);
  if (strcmp(string,_strupr(rs19.gets())) == 0)
    v = 0;
  else
    {  v = int(strtol(string,&eptr,10));
       if (*eptr != 0)
         return 0;
    }
  if (v < -20 || v > 300)
    return 0;
  *newvalue = v;
  return 1;
}

void FloorDialogControl::changefocus(Dialog *dialog,int)
{int floor;
 double floorspacing;
   if (currentvalue(dialog,&floor) && dialog->currentvalue(101,&floorspacing))
     ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,floor * floorspacing);
}

class FloorRealDialogControl : public RealDialogControl
  {public:
     FloorRealDialogControl(int id,double *v) : RealDialogControl(id,v) {}
     void changefocus(Dialog *dialog,int updatescreen = 1);
     void vscrollevent(Dialog *dialog,int s1,int s2,int s3);
  };

void FloorRealDialogControl::changefocus(Dialog *dialog,int)
{int floor;
 double floorspacing;
   if (((FloorDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&floor) && dialog->currentvalue(101,&floorspacing))
     ((RealDialogControl *)dialog->getcontrol(102))->change(dialog,floor * floorspacing);
}

void FloorRealDialogControl::vscrollevent(Dialog *dialog,int s1,int s2,int s3)
{ RealDialogControl::vscrollevent(dialog,s1,s2,s3);
  changefocus(dialog);
}

void visiblecb1(Entity *e,void *v)
{ if (e != 0)
    e->getvisible()->set((LONG)v);
}

void architect_command9(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("ArFloor_dialog");
 DialogControl *dc;
 int floor,perspective,viewno;
 double floorspacing,floorheight,front,back;
 ResourceString rs19(NCARCHITECT+19),rs22(NCARCHITECT+22),rs23(NCARCHITECT+23),rs24(NCARCHITECT+24),rs25(NCARCHITECT+25),rs103(NCARCHITECT+103);
 RCCHAR name[256],floorviewname[300];
 Point eye,ref,up,dir;
 View *oldv,*newv;
 Transform t;
 Entity *e;
  floor = v.getinteger("ar::floor");
  floorspacing = v.getreal("ar::floorspacing");
  floorheight = floor * floorspacing;
  dialog.add(dc = new FloorDialogControl(100,&floor));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new FloorRealDialogControl(101,&floorspacing));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&floorheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(new CFListDialogControl(103,floorviewname));
  if (dialog.process() == TRUE)
    {  v.setinteger("ar::floor",floor);
       v.setreal("ar::floorspacing",floorspacing);
       perspective = 0;  front = -floorspacing + 1.0;   back = 1;
		 eye.setxyz(0.0,0.0,floorheight+1.0);
       ref.setxyz(0.0,0.0,floorheight);
       up.setxyz (0.0,1.0,0.0);
       if (floor == 0)
         strcpy(name,rs19.gets());
       else
         sprintf(name,rs25.gets(),floor);
       if (db.views3d.match(name) != NULL)
         cadwindow->MessageBox(rs23.gets(),rs22.gets(),MB_ICONINFORMATION);
       else if ((viewno = db.getnextviewno()) < 0)
         cadwindow->MessageBox(rs24.gets(),rs22.gets(),MB_ICONINFORMATION);
	      else if ((oldv = db.views3d.match(rs103.gets())) != 0)
         {  if ((newv = new View(name,perspective,viewno,eye,ref,up,front,back)) != 0)
              {  db.views3d.add(newv);
                 //  For now, make NO entities visible in the new floor.
                 for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                 //  if (e->getvisible()->getnbitsset() > 1)
                 //    e->getvisible()->set(newv->getviewno(),e->getvisible()->test(oldv->getviewno()) != 0);
                   e->getvisible()->set(newv->getviewno(),0);
                 cadwindow->create3dsubwindow(newv);
                 if ((oldv = db.views3d.match(floorviewname)) != 0)  //  Copy from another view
                   {  dir.setxyz(0.0,0.0,ref.z - oldv->getref().z);
                      t.translate(dir);
                      for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                        if (e->isa(wall_entity) && fabs(((Wall *)e)->getp1().z - oldv->getref().z) < 0.001)
                          {  e->setcopye(0);
                             e->clone(t);
                             e->getcopye()->getvisible()->set(newv->getviewno());
                             e->getcopye()->dbtraverseassociated(visiblecb1,(void *)newv->getviewno());
                             db.geometry.add(e->getcopye());
                          }
                   }
              }
         }
    }
  *cstate = ExitState;
}

struct RBArchitect10_data
  {Point p1,xaxis,yaxis,zaxis;
   int shiftstate;
   double snapangle;
   BitMask visible;
   RCCHAR section[300],sheet[300],viewname[20];
   RBArchitect10_data() : visible(MaxViews) {}
  };

void RBArchitect10(int dm,void *data,Point *p2,View3dSurface *)
{RBArchitect10_data *rbdata = (RBArchitect10_data *) data;
 Point p3,p4;
 double l,angle;
  if (dm == RB_ERASE && ! rbdata->shiftstate ||
      dm != RB_ERASE && ! cadwindow->getshiftstate())
    {  rbdata->shiftstate = 0;
       p3 = rbdata->p1;
       p4.x = rbdata->xaxis.dot(*p2 - p3);
       p4.y = rbdata->yaxis.dot(*p2 - p3);
       if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
       p4.x /= l;  p4.y /= l;
       if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
       angle = acos(p4.x);
       if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
       if (fabs(rbdata->snapangle) > 1.0E-10)
         angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
       p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                 rbdata->yaxis * (l * sin(angle));
       Line line(rbdata->p1,p4);
       p4 = line.nearp(*p2);
    }
  else
    {  rbdata->shiftstate = 1;
       p4 = *p2;
    }
  Line line(rbdata->p1,p4);
  line.draw(DM_INVERT);
}

class ArchitecturalNotation
{public:
  static Entity *areareference(Point,double,BitMask &,RCCHAR *,RCCHAR *);
  static Entity *sectionreference(Point,Point,double,BitMask &,RCCHAR *,RCCHAR *);
  static Entity *elevationreference(Point,Point,double,BitMask &,RCCHAR *,RCCHAR *);
  static Entity *windowdoorreference(Point,double,BitMask &,RCCHAR *,RCCHAR *);
};

Entity *ArchitecturalNotation::areareference(Point o,double scale,BitMask &visible,RCCHAR *area,RCCHAR *sheet)
{BitMask options(32);
 Point p1,p2,origin,xaxis,yaxis;
 RCCHAR *f,*t;
 double angle,height,width,slant;
 EntityList list;
 Circle *circle;
 Line *line;
 Text *text1,*text2;
 Group *group;
 Byte halocolour;
  origin = o;
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);
  p1.setxyz(-6.0 * scale,0.0,0.0);  p1 += o;
  p2.setxyz( 6.0 * scale,0.0,0.0);  p2 += o;
  circle = new Circle(origin,xaxis,yaxis,6.0 * scale,0.0,2.0*M_PI);
  circle->setvisible(visible);
  circle->EntityHeader::change(circle->getcolour(),circle->getlayer(),circle->getstyle(),4);
  line = new Line(p1,p2);
  line->setvisible(visible);
  line->EntityHeader::change(line->getcolour(),line->getlayer(),line->getstyle(),3);
  origin.setxyz(0.0,2.5 * scale,0.0);  origin += o;
  Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
  options.set(1,1);  options.set(2,0);  options.set(3,1);  options.set(4,0);
  text1 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,area,0,options);
  text1->setvisible(visible);
  origin.setxyz(0.0,-2.5 * scale,0.0);  origin += o;
  text2 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,sheet,0,options);
  text2->setvisible(visible);
  options.clearall();
  list.add(circle);  list.add(line); list.add(text1);  list.add(text2);
  options.set(0);
  group = new Group(list,"Architect","NotationR",options);
  group->setvisible(visible);
  return group;
}

Entity *ArchitecturalNotation::sectionreference(Point o,Point d,double scale,BitMask &visible,RCCHAR *section,RCCHAR *sheet)
{BitMask options(32);
 Point p1,p2,origin,xaxis,yaxis,zaxis(0.0,0.0,1.0),xa,ya;
 RCCHAR *f,*t;
 double angle,height,width,slant;
 EntityList list;
 Circle *circle;
 Line *line1,*line2,*line3,*line4;
 Text *text1,*text2;
 Group *group;
 Byte halocolour;
  origin = o;
  xa = (d - o).normalize();
  ya = zaxis.cross(xa).normalize();
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);
  p1.setxyz(-6.0 * scale,0.0,0.0);  p1 += o;
  p2.setxyz( 6.0 * scale,0.0,0.0);  p2 += o;
  circle = new Circle(origin,xaxis,yaxis,6.0 * scale,0.0,2.0*M_PI);
  circle->setvisible(visible);
  circle->EntityHeader::change(circle->getcolour(),circle->getlayer(),circle->getstyle(),4);
  line1 = new Line(p1,p2);
  line1->setvisible(visible);
  line1->EntityHeader::change(line1->getcolour(),line1->getlayer(),line1->getstyle(),3);
  line2 = new Line(o + xa * 6.0 * scale,o + xa * 12.0 * scale);
  line2->setvisible(visible);
  line2->EntityHeader::change(line2->getcolour(),line2->getlayer(),line2->getstyle(),3);
  line3 = new Line(o-xa/sqrt(2.0)*scale*6.0+ya/sqrt(2.0)*6.0*scale,o+ya*sqrt(2.0)*6.0*scale);
  line3->setvisible(visible);
  line3->EntityHeader::change(line3->getcolour(),line3->getlayer(),line3->getstyle(),4);
  line4 = new Line(o+xa/sqrt(2.0)*scale*6.0+ya/sqrt(2.0)*6.0*scale,o+ya*sqrt(2.0)*6.0*scale);
  line4->setvisible(visible);
  line4->EntityHeader::change(line4->getcolour(),line4->getlayer(),line4->getstyle(),4);
  origin.setxyz(0.0,2.5 * scale,0.0);  origin += o;
  Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
  options.set(1,1);  options.set(2,0);  options.set(3,1);  options.set(4,0);
  text1 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,section,0,options);
  text1->setvisible(visible);
  origin.setxyz(0.0,-2.5 * scale,0.0);  origin += o;
  text2 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,sheet,0,options);
  text2->setvisible(visible);
  options.clearall();
  list.add(circle);
  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
  list.add(text1);  list.add(text2);
  options.set(0);
  group = new Group(list,"Architect","NotationR",options);
  group->setvisible(visible);
  return group;
}

Entity *ArchitecturalNotation::elevationreference(Point o,Point d,double scale,BitMask &visible,RCCHAR *elevation,RCCHAR *sheet)
{BitMask options(32);
 Point p1,p2,origin,xaxis,yaxis,zaxis(0.0,0.0,1.0),xa,ya;
 RCCHAR *f,*t;
 double angle,height,width,slant;
 EntityList list;
 Circle *circle;
 Line *line1,*line2,*line3;
 Text *text1,*text2;
 Group *group;
 Byte halocolour;

  origin = o;
  ya = (d - o).normalize();
  xa = zaxis.cross(ya).normalize();
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);
  p1.setxyz(-6.0 * scale,0.0,0.0);  p1 += o;
  p2.setxyz( 6.0 * scale,0.0,0.0);  p2 += o;
  circle = new Circle(origin,xaxis,yaxis,6.0 * scale,0.0,2.0*M_PI);
  circle->setvisible(visible);
  circle->EntityHeader::change(circle->getcolour(),circle->getlayer(),circle->getstyle(),4);
  line1 = new Line(p1,p2);
  line1->setvisible(visible);
  line1->EntityHeader::change(line1->getcolour(),line1->getlayer(),line1->getstyle(),3);
  line2 = new Line(o-xa/sqrt(2.0)*scale*6.0+ya/sqrt(2.0)*6.0*scale,o+ya*sqrt(2.0)*6.0*scale);
  line2->setvisible(visible);
  line2->EntityHeader::change(line2->getcolour(),line2->getlayer(),line2->getstyle(),4);
  line3 = new Line(o+xa/sqrt(2.0)*scale*6.0+ya/sqrt(2.0)*6.0*scale,o+ya*sqrt(2.0)*6.0*scale);
  line3->setvisible(visible);
  line3->EntityHeader::change(line3->getcolour(),line3->getlayer(),line3->getstyle(),4);
  origin.setxyz(0.0,2.5 * scale,0.0);  origin += o;
  Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
  options.set(1,1);  options.set(2,0);  options.set(3,1);  options.set(4,0);
  text1 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,elevation,0,options);
  text1->setvisible(visible);
  origin.setxyz(0.0,-2.5 * scale,0.0);  origin += o;
  text2 = new Text(f,origin,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,sheet,0,options);
  text2->setvisible(visible);
  options.clearall();
  list.add(circle);
  list.add(line1);  list.add(line2);  list.add(line3);
  list.add(text1);  list.add(text2);
  options.set(0);
  group = new Group(list,"Architect","NotationR",options);
  group->setvisible(visible);
  return group;
}

Entity *ArchitecturalNotation::windowdoorreference(Point o,double scale,BitMask &visible,RCCHAR *area,RCCHAR *sheet)
{BitMask options(32);
 Point p1,p2,p3,p4,p5,p6,p7,p8,xaxis,yaxis;
 RCCHAR *f,*t;
 double angle,height,width,slant;
 EntityList list;
 Line *line[5];
 Text *text1,*text2;
 Group *group;
 int i;
 Byte halocolour;
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);
  p1.setxyz(-6.0 * scale,-5.0 * scale,0.0);  p1 += o;
  p2.setxyz( 6.0 * scale,-5.0 * scale,0.0);  p2 += o;
  p3.setxyz( 6.0 * scale, 5.0 * scale,0.0);  p3 += o;
  p4.setxyz(-6.0 * scale, 5.0 * scale,0.0);  p4 += o;
  p5.setxyz(-6.0 * scale, 0.0 * scale,0.0);  p5 += o;
  p6.setxyz( 6.0 * scale, 0.0 * scale,0.0);  p6 += o;
  p7.setxyz( 0.0 * scale, 2.5 * scale,0.0);  p7 += o;
  p8.setxyz( 0.0 * scale,-2.5 * scale,0.0);  p8 += o;
  line[0] = new Line(p1,p2);
  line[1] = new Line(p2,p3);
  line[2] = new Line(p3,p4);
  line[3] = new Line(p4,p1);
  line[4] = new Line(p5,p6);

  for (i = 0 ; i < 5 ; i++)
    {  line[i]->setvisible(visible);
       line[i]->EntityHeader::change(line[i]->getcolour(),line[i]->getlayer(),line[i]->getstyle(),4);
    }
  Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
  options.set(1,1);  options.set(2,0);  options.set(3,1);  options.set(4,0);
  text1 = new Text(f,p7,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,area,0,options);
  text1->setvisible(visible);
  text2 = new Text(f,p8,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,sheet,0,options);
  text2->setvisible(visible);
  options.clearall();
  for (i = 0 ; i < 5 ; i++)
    list.add(line[i]);
  list.add(text1);  list.add(text2);
  options.set(0);
  group = new Group(list,"Architect","NotationWDR",options);
  group->setvisible(visible);
  return group;
}

void architect_command10(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect10_data *rbdata = (RBArchitect10_data *) *data;
 Point origin,xaxis,yaxis,zaxis,p1,p2,eye,ref,up,p3,p4;
 Entity *e,*ge;
 Transform t1,t2;
 int c,j;
 int viewno;
 View *v;
 BitMask visible(32),options(32);
 RCCHAR name[300];
 double l,angle;
 Dialog dialog("ArNotation_dialog");
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect10_data;  *data = rbdata;
       if (rbdata != 0)
         {  strcpy(name,"Section number");
            for (c = 1 ; c <= 100 ; c++)
              {  sprintf(rbdata->viewname,"Section-%2.2d",c);
                 sprintf(rbdata->section,"%d",c);
                 if (db.views3d.match(rbdata->viewname) == 0)
                   break;
              }
            strcpy(rbdata->sheet,::v.getstring("ar::sheet"));
            dialog.add(new StringDialogControl(100,name,300));
            dialog.add(new StringDialogControl(101,rbdata->section,300));
            dialog.add(new StringDialogControl(102,rbdata->sheet,300));
            if (dialog.process() == TRUE)
              {  ::v.setstring("ar::sheet",rbdata->sheet);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis).normalize();
            rbdata->shiftstate = 0;
            rbdata->snapangle = ::v.getreal("ln::autosnapangle") * M_PI / 180.0;
            rbdata->visible.clearall();
            rbdata->visible.set(((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  if ((viewno = db.getnextviewno()) >= 0)
				          {  eye.setxyz(   0.0,  0.0,  1.0);
                 ref.setxyz(   0.0,  0.0,  0.0);
                 up.setxyz (   0.0,  1.0,  0.0);
                 v = new View(rbdata->viewname,0,viewno | VW_ONEVIEWONLY,eye,ref,up,-1.0,1.0);
                 db.views3d.add(v);
                 visible.set(viewno);
                 cadwindow->create3dsubwindow(v);
                 p2 = ((Coordinate3dEvent *) event)->getp();
                 if (! cadwindow->getshiftstate())
                   {  p3 = rbdata->p1;
                      p4.x = rbdata->xaxis.dot(p2 - p3);
                      p4.y = rbdata->yaxis.dot(p2 - p3);
                      if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                        {  p4.x /= l;  p4.y /= l;
                           if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                           angle = acos(p4.x);
                           if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
                           if (fabs(rbdata->snapangle) > 1.0E-10)
                             angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
                           p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                                     rbdata->yaxis * (l * sin(angle));
                           Line line(rbdata->p1,p4);
                           p2 = line.nearp(p2);
                        }
                   }
                 xaxis = (p2 - rbdata->p1).normalize();
                 yaxis = rbdata->zaxis;
                 zaxis = xaxis.cross(yaxis).normalize();
                 InfinitePlane iplane(zaxis,rbdata->p1);
                 t1.translate(-rbdata->p1);
                 t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
                 t1.apply(t2);
                 for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                   if (e->isa(plane_entity) || e->isa(wall_entity) || e->isa(figure_entity))
                     {Intersect i(e,&iplane);
                        for (j = 0 ; j < i.nintersections() ; j++)
                          {  e = i.intersection(j)->clone(t1);
                             e->setvisible(visible);
                             db.geometry.add(e);
                          }
                     }
                 origin.setxyz(0.0,0.0,0.0);
                 db.saveundo(UD_STARTBLOCK,0);
                 if ((e = ArchitecturalNotation::sectionreference(rbdata->p1,p2,::v.getreal("ar::notationscale"),rbdata->visible,rbdata->section,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 if ((e = ArchitecturalNotation::areareference(origin,::v.getreal("ar::notationscale"),visible,rbdata->section,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 db.saveundo(UD_ENDBLOCK,0);
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//

  if (*cstate == 1 || *cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+46+*cstate);
       if (*cstate == 2)
         {  rubberband.begin(0);
            rubberband.add(RBArchitect10,rbdata);
         }
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect11_data
  {Point p1,xaxis,yaxis,zaxis;
   int shiftstate;
   double snapangle;
   BitMask visible;
   RCCHAR elevation[300],sheet[300],viewname[20];
   RBArchitect11_data() : visible(MaxViews) {}
  };

void RBArchitect11(int dm,void *data,Point *p2,View3dSurface *)
{RBArchitect11_data *rbdata = (RBArchitect11_data *) data;
 Point p3,p4;
 double l,angle;
  if (dm == RB_ERASE && ! rbdata->shiftstate ||
      dm != RB_ERASE && ! cadwindow->getshiftstate())
    {  rbdata->shiftstate = 0;
       p3 = rbdata->p1;
       p4.x = rbdata->xaxis.dot(*p2 - p3);
       p4.y = rbdata->yaxis.dot(*p2 - p3);
       if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
       p4.x /= l;  p4.y /= l;
       if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
       angle = acos(p4.x);
       if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
       if (fabs(rbdata->snapangle) > 1.0E-10)
         angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
       p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                 rbdata->yaxis * (l * sin(angle));
       Line line(rbdata->p1,p4);
       p4 = line.nearp(*p2);
    }
  else
    {  rbdata->shiftstate = 1;
       p4 = *p2;
    }
  Line line(rbdata->p1,p4);
  line.draw(DM_INVERT);
}

void architect_command11(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect11_data *rbdata = (RBArchitect11_data *) *data;
 Point origin,xaxis,yaxis,zaxis,p1,p2,eye,ref,up,p3,p4,pmin,pmax;
 Entity *e,*ge;
 Transform t1,t2;
 int c;
 int viewno;
 View *v;
 BitMask visible(MaxViews),options(32);
 RCCHAR name[300];
 double du,dv,l,angle;
 Dialog dialog("ArNotation_dialog");
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect11_data;  *data = rbdata;
       if (rbdata != 0)
         {  strcpy(name,"Elevation number");
            for (c = 1 ; c <= 100 ; c++)
              {  sprintf(rbdata->viewname,"Elevation-%2.2d",c);
                 sprintf(rbdata->elevation,"%d",c);
                 if (db.views3d.match(rbdata->viewname) == 0)
                   break;
              }
            strcpy(rbdata->sheet,::v.getstring("ar::sheet"));
            dialog.add(new StringDialogControl(100,name,300));
            dialog.add(new StringDialogControl(101,rbdata->elevation,300));
            dialog.add(new StringDialogControl(102,rbdata->sheet,300));
            if (dialog.process() == TRUE)
              {  ::v.setstring("ar::sheet",rbdata->sheet);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis).normalize();
            rbdata->shiftstate = 0;
            rbdata->snapangle = ::v.getreal("ln::autosnapangle") * M_PI / 180.0;
            rbdata->visible.clearall();
            rbdata->visible.set(((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  if ((viewno = db.getnextviewno()) >= 0)
				          {  p2 = ((Coordinate3dEvent *) event)->getp();
                 if (! cadwindow->getshiftstate())
                   {  p3 = rbdata->p1;
                      p4.x = rbdata->xaxis.dot(p2 - p3);
                      p4.y = rbdata->yaxis.dot(p2 - p3);
                      if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                        {  p4.x /= l;  p4.y /= l;
                           if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                           angle = acos(p4.x);
                           if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
                           if (fabs(rbdata->snapangle) > 1.0E-10)
                             angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
                           p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                                     rbdata->yaxis * (l * sin(angle));
                           Line line(rbdata->p1,p4);
                           p2 = line.nearp(p2);
                        }
                   }
                 zaxis = (p2 - rbdata->p1).normalize();
                 yaxis = rbdata->zaxis;
                 xaxis = yaxis.cross(zaxis).normalize();
                 t1.translate(-rbdata->p1);
                 t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
                 t1.apply(t2);

                 eye = rbdata->p1;
                 ref = eye + zaxis;
                 up = yaxis;
                 v = new View(rbdata->viewname,0,viewno | VW_ONEVIEWONLY,eye,ref,up,-1.0E6,1.0E6);
                 db.views3d.add(v);
                 visible.set(viewno);
                 for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                   {  e->setstatus(0);
                      e->getvisible()->set(viewno,e->getvisible()->test(0) != 0);
                   }
                 cadwindow->create3dsubwindow(v);

                 db.geometry.extents(cadwindow->getcurrentsurface(),&pmin,&pmax);
                 if (pmax.x - pmin.x > db.getptoler() && pmax.y - pmin.y > db.getptoler())
                   {  du = (pmax.x - pmin.x) * 0.02;
                      dv = (pmax.y - pmin.y) * 0.02;
                      cadwindow->getcurrentwindow()->zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
                   }

                 visible.clearall();
                 HideImage image(cadwindow->getcurrentwindow(),&visible,&t1);
                 image.hide();

                 for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                   e->getvisible()->set(viewno,e->getstatus());

                 eye.setxyz(   0.0,  0.0,  1.0);
                 ref.setxyz(   0.0,  0.0,  0.0);
                 up.setxyz (   0.0,  1.0,  0.0);
                 v->changeview(eye,ref,up);
                 cadwindow->updatewindows(v);

                 origin.setxyz(0.0,0.0,0.0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 if ((e = ArchitecturalNotation::elevationreference(rbdata->p1,p2,::v.getreal("ar::notationscale"),rbdata->visible,rbdata->elevation,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 visible.set(viewno);
                 if ((e = ArchitecturalNotation::areareference(origin,::v.getreal("ar::notationscale"),visible,rbdata->elevation,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//

  if (*cstate == 1 || *cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+51+*cstate);
       if (*cstate == 2)
         {  rubberband.begin(0);
            rubberband.add(RBArchitect11,rbdata);
         }
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect12_data
  {RCCHAR detail[300],sheet[300],viewname[20];
   EntityList list;
  };

void architect_command12(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect12_data *rbdata = (RBArchitect12_data *) *data;
 ResourceString rs49(NCARCHITECT+49),rs50(NCARCHITECT+50);
 Entity *e,*ge;
 Transform t;
 int c,viewno;
 View *v;
 BitMask visible(MaxViews),options(32);
 RCCHAR name[300];
 Point eye,ref,up,p1,origin;
 Dialog dialog("ArNotation_dialog");
   switch (*cstate)
     {case InitialState :
       rbdata = new RBArchitect12_data;  *data = rbdata;
       if (rbdata != 0)
         {  rbdata->list = state.getselection().copy();
            state.destroyselection(1);
            if (rbdata->list.empty())
              {  cadwindow->MessageBox(rs49.gets(),rs50.gets(),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  strcpy(name,"Detail number");
                 for (c = 1 ; c <= 100 ; c++)
                   {  sprintf(rbdata->viewname,"Detail-%2.2d",c);
                      sprintf(rbdata->detail,"%d",c);
                      if (db.views3d.match(rbdata->viewname) == 0)
                        break;
                   }
                 strcpy(rbdata->sheet,::v.getstring("ar::sheet"));
                 dialog.add(new StringDialogControl(100,name,300));
                 dialog.add(new StringDialogControl(101,rbdata->detail,300));
                 dialog.add(new StringDialogControl(102,rbdata->sheet,300));
                 if (dialog.process() == TRUE)
                   {  ::v.setstring("ar::sheet",rbdata->sheet);
                      *cstate = 1;
                   }
                 else
                  *cstate = ExitState;
              }
          }
        else
          *cstate = ExitState;
        break;
      case 1 :
        if (event->isa(Coordinate3d))
          {  p1 = ((Coordinate3dEvent *) event)->getp();
             if ((viewno = db.getnextviewno()) >= 0)
               {  eye.setxyz(   0.0,  0.0,  1.0);
                  ref.setxyz(   0.0,  0.0,  0.0);
                  up.setxyz (   0.0,  1.0,  0.0);
                  v = new View(rbdata->viewname,0,viewno | VW_ONEVIEWONLY,eye,ref,up,-1.0,1.0);
                  db.views3d.add(v);
                  visible.set(viewno);
                  cadwindow->create3dsubwindow(v);
                  db.saveundo(UD_STARTBLOCK,NULL);
                  for (rbdata->list.start() ; (e = rbdata->list.next()) != 0 ; )
                    {  t.translate(-p1);
                       e->setcopye(0);
                       e = e->clone(t);
                       e->setvisible(visible);
                       e->dbtraverseassociated(visiblecb,&visible);
                       db.geometry.add(e);
                    }
                 origin.setxyz(0.0,0.0,0.0);
                 if ((e = ArchitecturalNotation::areareference(origin,::v.getreal("ar::notationscale"),visible,rbdata->detail,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 visible.clearall();
                 visible.set(((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno());
                 if ((e = ArchitecturalNotation::areareference(p1,::v.getreal("ar::notationscale"),visible,rbdata->detail,rbdata->sheet)) != 0)
                   {EntityList list;
                      list = ((Group *)e)->getlist();
                      for (list.start() ; (ge = list.next()) != 0 ; )
                        db.geometry.add(ge);
                      db.geometry.add(e);
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
               }
             *cstate = ExitState;
          }
        else if (event->isa(Abort) || event->isa(Exit))
          *cstate = ExitState;
        break;
    }

//
//  About to enter state actions
//

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+51);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void change_notation(Group *g)
{Dialog dialog("ArNotation_dialog");
 EntityList list;
 Text *text1,*text2;
 Entity *e;
 RCCHAR name[300],number[300],sheet[300];
 BitMask change(32);
  text1 = 0;  text2 = 0;
  list = g->getlist();
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(text_entity))
      {  if (text2 == 0)
           text2 = (Text *)e;
         else
           text1 = (Text *)e;
      }
  if (text1 == 0 || text2 == 0)
    return;
  strcpy(name,"Number");
#ifdef _USING_QTCHAR
  RCSTRCPY(number,text1->gettext());
  RCSTRCPY(sheet,text2->gettext());
#else
  strcpy(number,text1->gettext());
  strcpy(sheet,text2->gettext());
#endif
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(new StringDialogControl(101,number,300));
  dialog.add(new StringDialogControl(102,sheet,300));
  if (dialog.process() == TRUE)
    {  change.set(31);
       text1->change(change,*text1,0,0,0,0,0,0,CHAR2RCCHR(number),0,change);
       text2->change(change,*text2,0,0,0,0,0,0,CHAR2RCCHR(sheet),0,change);
    }
}

void architect_command13(int *cstate,HighLevelEvent *event,void **)
{EntityList list;
 Entity *e;
  switch (*cstate)
    {case InitialState :
       list = state.getselection();
       if (list.length() == 1)
         {  list.start();
            e = list.next();
            if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"Architect") == 0 && strcmp(((Group *)e)->getinstance(),"NotationR") == 0)
              change_notation((Group *)e);
         }
       state.destroyselection(1);
       *cstate = 1;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            e = ((EntitySelectedEvent *)event)->getentity();
            if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"Architect") == 0 && strcmp(((Group *)e)->getinstance(),"NotationR") == 0)
              change_notation((Group *)e);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+54);
    }
}

struct RBArchitect14_data
  {int state,numberofpoints,axisorder;
   RCCHAR library[128];
   RCCHAR name[32];
   Point p1,p2,xaxis,yaxis,rotation;
  };

void RBArchitect14(int dm,void *data,Point *p2,View3dSurface *v)
{RBArchitect14_data *rbdata = (RBArchitect14_data *) data;
 Point scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),xaxis,yaxis,xa;
 BitMask options(32);
  options.set(0);  options.set(1);
  if (rbdata->state == 1)
    {  if (dm == RB_DRAW)
         {  if (v != 0)
              {  rbdata->xaxis = v->getxaxis();
                 rbdata->yaxis = v->getyaxis();
              }
            else
              {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
                 rbdata->yaxis.setxyz(0.0,1.0,0.0);
              }
         }
       xaxis = rbdata->xaxis;
       yaxis = rbdata->yaxis;
       rbdata->p1 = *p2;
    }
  else if (rbdata->state == 2)
    {  xaxis = (*p2 - rbdata->p1).normalize();
       yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis).normalize();
    }
  else
    {  xaxis = (rbdata->p2 - rbdata->p1).normalize();
       yaxis = (*p2 - rbdata->p1);
       yaxis = (xaxis.cross(yaxis)).cross(xaxis).normalize();
    }
  if (rbdata->axisorder)
    {  xa = xaxis;  xaxis = yaxis;  yaxis = xa;
    }
  if (xaxis.cross(yaxis).z < 0.0)
    scale.z = -1.0;
  Figure figure(rbdata->library,rbdata->name,rbdata->p1,xaxis,yaxis,scale,rbdata->rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
  if (figure.getdefined()) figure.draw(DM_INVERT);
}


class ArSymbolDisplayDialogControl : public DisplayDialogControl
  {public:
     RCCHAR *library,*type,oldlibrary[300];
     ArSymbolDisplayDialogControl(int id,RCCHAR *library_,RCCHAR *type_) : DisplayDialogControl(id) {  library = library_;  type = type_;  strcpy(oldlibrary,library);  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

void ArSymbolDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 int x1,y1,w,h;
 EntityHeader header(0,0,0,0);
 Point p1,p2,org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0);
 BitMask options(32);
 double du,dv,width,height;
 GeneralList list;

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
       IntersectClipRect(output.gethdc(),1,1,int(output.getwidth()-1),int(output.getheight()-1));
       
       du = output.getumax() - output.getumin();
       dv = output.getvmax() - output.getvmin();
       if (du > dv)
         {  dv /= du;  du = 1.0;  
         }
       else
         {  du /= dv;  dv = 1.0;  
         }
       output.zoom(-du * 300,-dv * 300,du * 3000,dv * 3000);

       //  Load the current figure name
       ((ArListDialogControl *)dialog->getcontrol(101))->store(dialog);

       x1 = (int)output.getumin();
       y1 = (int)output.getvmin();
       w = (int)(output.getumax() - x1);
       h = (int)(output.getvmax() - y1);

       width = 2000.0;  height = 2000.0;
       org.setxyz((output.getumin() + output.getumax() - width) / 2.0,(output.getvmin() + output.getvmax() - height) / 2.0,0.0);
       Figure figure(header,library,_RCT(""),org,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,NULL,options);
       figure.draw(DM_NORMAL,NULL,&output);

    }
}

void architect_command14(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect14_data *rbdata = (RBArchitect14_data *) *data;
 Point p3,scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),xa;
 Figure *figure;
 BitMask options(32);
 RCCHAR category[300];
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect14_data;  *data = rbdata;
       if (rbdata != 0)
         {Dialog dialog("ArSymbol_dialog");
            strcpy(category,v.getstring("ar::symbolcategory"));
            strcpy(rbdata->library,v.getstring("ar::symbollibrary"));
            rbdata->axisorder = v.getinteger("ar::symbolaxisorder");
            strcpy(rbdata->name,_RCT(""));
            rbdata->rotation.setxyz(0.0,0.0,0.0);
            dialog.add(new ArCatListDialogControl(100,_RCT("symbol"),category));
            dialog.add(new ArListDialogControl(101,_RCT("symbol"),rbdata->library));
            dialog.add(new CheckBoxDialogControl(102,&rbdata->axisorder));
            dialog.add((DisplayDialogControl *) new ArSymbolDisplayDialogControl(103,rbdata->library,_RCT("symbol")));
            dialog.add(new ButtonDialogControl(104));
            dialog.add(new ButtonDialogControl(105));
            if ((rbdata->numberofpoints = dialog.process() - 103) > 0)
              {  v.setstring("ar::symbolcategory",category);
                 v.setstring("ar::symbollibrary",rbdata->library);
                 v.setinteger("ar::symbolaxisorder",rbdata->axisorder);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            if (rbdata->numberofpoints == 1)
              {  options.set(1);
                 options.set(FigureOriginalStyle);
                 options.set(FigureOriginalWeight);
                 figure = new Figure(rbdata->library,rbdata->name,rbdata->p1,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
                 if (figure != 0 && figure->getdefined())
                   {  db.geometry.add(figure);
                      figure->addattributetext();
                   }
                 *cstate = 1;
              }
            else
              *cstate = 2;
         }
       else if (event->isa(User))
         {  if (((UserEvent *)event)->getuserid() == 1)
              rbdata->rotation.z += v.getreal("fi::deltarotationz");
            else if (((UserEvent *)event)->getuserid() == 2)
              rbdata->rotation.z -= v.getreal("fi::deltarotationz");
            else if (((UserEvent *)event)->getuserid() == 3)
              rbdata->rotation.x += v.getreal("fi::deltarotationx");
            else if (((UserEvent *)event)->getuserid() == 4)
              rbdata->rotation.x -= v.getreal("fi::deltarotationx");
            else if (((UserEvent *)event)->getuserid() == 5)
              rbdata->rotation.y += v.getreal("fi::deltarotationy");
            else if (((UserEvent *)event)->getuserid() == 6)
              rbdata->rotation.y -= v.getreal("fi::deltarotationy");
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 3;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p3 = ((Coordinate3dEvent *) event)->getp();
            xaxis = (rbdata->p2 - rbdata->p1).normalize();
            yaxis = (p3 - rbdata->p1);
            yaxis = (xaxis.cross(yaxis)).cross(xaxis).normalize();
            if (rbdata->axisorder)
              {  xa = xaxis;  xaxis = yaxis;  yaxis = xa;
              }
            if (xaxis.cross(yaxis).z < 0.0)
              scale.z = -1.0;
            options.set(1);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight);
            figure = new Figure(rbdata->library,rbdata->name,rbdata->p1,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
            if (figure != 0 && figure->getdefined())
              {  db.geometry.add(figure);
                 figure->addattributetext();
              }
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

  if (*cstate == 1 || *cstate == 2 || *cstate == 3)
    {  rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBArchitect14,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+15+*cstate);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect15_data
  {Point   p1,p2,xaxis,yaxis;
   double xgrid,ygrid,xextension,yextension,plotscale;
   int circled;
   BitMask visible;
   RBArchitect15_data() : visible(MaxViews) {};
  };

void insert_argrid(RBArchitect15_data *rbdata,int insert)
{Point p1,p2,p3,p4,p5,p6,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 RCCHAR *f,*t,font[256],txt[4096];
 double height,width,slant,angle;
 BitMask options(32);
 Line *line;
 Text *text;
 Circle *circle;
 int i,j;
 Byte halocolour;
  p1.setxyz(0.0,0.0,0.0);
  p2.setxyz(rbdata->xaxis.dot(rbdata->p2-rbdata->p1),
            rbdata->yaxis.dot(rbdata->p2-rbdata->p1),0.0);
  if (p2.x < p1.x)
    {  p1.x = p2.x;  p2.x = 0.0;
    }
  if (p2.y < p1.y)
    {  p1.y = p2.y;  p2.y = 0.0;
    }
  if (insert)
    {  db.saveundo(UD_STARTBLOCK,NULL);
       Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
       for (p3 = p1 - yaxis * rbdata->yextension,i = 0 ; p3.x <= p2.x + 0.001 ; p3.x += rbdata->xgrid,i++)
         {  p4 = p3 + yaxis * (rbdata->yextension*2 + p2.y - p1.y - fmod(p2.y-p1.y,rbdata->ygrid));
            line = new Line(rbdata->p1 + rbdata->xaxis * p3.x + rbdata->yaxis * p3.y,
                            rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y);
            if (line != 0 && line->getdefined())
              {  line->setvisible(rbdata->visible);
                 db.geometry.add(line);
              }
            else
              delete line;

            sprintf(txt,"%c",'A' + i);
            options.set(1,1);  options.set(3,1);
            text = new Text(font,rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y + rbdata->yaxis * 3.0 * rbdata->plotscale,
                            rbdata->xaxis,rbdata->yaxis,0.0,4.0*rbdata->plotscale,0.0,0.0,0,txt,0,options);
            if (text != NULL && text->getdefined())
              {  text->setvisible(rbdata->visible);
                 db.geometry.add(text);
              }
            else
              delete text;

            if (rbdata->circled)
              {  circle = new Circle(rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y + rbdata->yaxis * 3.0 *rbdata->plotscale,
                                     rbdata->xaxis,rbdata->yaxis,3.0*rbdata->plotscale,0,2.0*M_PI);
                 if (circle != NULL && circle->getdefined())
                   {  circle->setvisible(rbdata->visible);
                      db.geometry.add(circle);
                   }
                 else
                   delete circle;
              }
         }

       for (p3 = p1 - xaxis * rbdata->xextension,j = 0 ; p3.y <= p2.y + 0.001 ; p3.y += rbdata->ygrid,j++);

       for (p3 = p1 - xaxis * rbdata->xextension,i = 0 ; p3.y <= p2.y + 0.001 ; p3.y += rbdata->ygrid,i++)
         {  p4 = p3 + xaxis * (rbdata->xextension*2 + p2.x - p1.x - fmod(p2.x-p1.x,rbdata->xgrid));
            line = new Line(rbdata->p1 + rbdata->xaxis * p3.x + rbdata->yaxis * p3.y,
                            rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y);
            if (line != 0 && line->getdefined())
              {  line->setvisible(rbdata->visible);
                 db.geometry.add(line);
              }
            else
              delete line;

            sprintf(txt,"%d",j - i);
            options.set(1,1);  options.set(3,1);
            text = new Text(font,rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y + rbdata->xaxis * 3.0 * rbdata->plotscale,
                            rbdata->xaxis,rbdata->yaxis,0.0,4.0*rbdata->plotscale,0.0,0.0,0,txt,0,options);
            if (text != NULL && text->getdefined())
              {  text->setvisible(rbdata->visible);
                 db.geometry.add(text);
              }
            else
              delete text;

            if (rbdata->circled)
              {  circle = new Circle(rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y + rbdata->xaxis * 3.0 *rbdata->plotscale,
                                     rbdata->xaxis,rbdata->yaxis,3.0*rbdata->plotscale,0,2.0*M_PI);
                 if (circle != NULL && circle->getdefined())
                   {  circle->setvisible(rbdata->visible);
                      db.geometry.add(circle);
                   }
                 else
                   delete circle;
              }
         }

       db.saveundo(UD_ENDBLOCK,NULL);

    }
  else
    {  p3 = rbdata->p1 + rbdata->xaxis * p1.x + rbdata->yaxis * p1.y;
       p4 = p3 + rbdata->xaxis * (p2.x - p1.x - fmod(p2.x - p1.x,rbdata->xgrid)) + rbdata->yaxis * (p2.y - p1.y - fmod(p2.y - p1.y,rbdata->ygrid));
       p5 = rbdata->xaxis * rbdata->xextension;
       p6 = rbdata->yaxis * rbdata->yextension;
       Line line1(p3-p5,p3 + rbdata->xaxis * (p2.x-p1.x - fmod(p2.x-p1.x,rbdata->xgrid)+rbdata->xextension));
       line1.draw(DM_INVERT);
       Line line2(p4+p5,p4 - rbdata->xaxis * (p2.x-p1.x - fmod(p2.x-p1.x,rbdata->xgrid)+rbdata->xextension));
       line2.draw(DM_INVERT);
       Line line3(p3-p6,p3 + rbdata->yaxis * (p2.y-p1.y - fmod(p2.y-p1.y,rbdata->xgrid)+rbdata->yextension));
       line3.draw(DM_INVERT);
       Line line4(p4+p6,p4 - rbdata->yaxis * (p2.y-p1.y - fmod(p2.y-p1.y,rbdata->xgrid)+rbdata->yextension));
       line4.draw(DM_INVERT);
    }
}

void RBArchitect15(int,void *data,Point *p2,View3dSurface *)
{ ((RBArchitect15_data *) data)->p2 = *p2;
  insert_argrid(((RBArchitect15_data *) data),0);
}

void architect_command15(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect15_data *rbdata = (RBArchitect15_data *) *data;
 Dialog dialog("ArGrid_dialog");
 DialogControl *dc;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect15_data;  *data = rbdata;
       if (rbdata != 0)
         {  rbdata->xgrid = v.getreal("ar::xgrid");
            rbdata->ygrid = v.getreal("ar::ygrid");
            rbdata->xextension = v.getreal("ar::xextension");
            rbdata->yextension = v.getreal("ar::yextension");
            rbdata->plotscale = v.getreal("ar::plotscale");
            rbdata->circled = v.getinteger("ar::circled");
            dialog.add(dc = new RealDialogControl(100,&rbdata->xgrid,100,1E-10,1E10,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1100,dc));
            dialog.add(dc = new RealDialogControl(101,&rbdata->ygrid,100,1E-10,1E10,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1101,dc));
            dialog.add(dc = new RealDialogControl(102,&rbdata->xextension,10,1E-10,1E10,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1102,dc));
            dialog.add(dc = new RealDialogControl(103,&rbdata->yextension,10,1E-10,1E10,ScaleByLength));
            dialog.add(new ScrollBarDialogControl(1103,dc));
            dialog.add(dc = new RealDialogControl(104,&rbdata->plotscale,1.0,1E-10,1E10));
            dialog.add(new ScrollBarDialogControl(1104,dc));
            dialog.add(new CheckBoxDialogControl(105,&rbdata->circled));
            if (dialog.process() == TRUE)
              {  v.setreal("ar::xgrid",rbdata->xgrid);
                 v.setreal("ar::ygrid",rbdata->ygrid);
                 v.setreal("ar::xextension",rbdata->xextension);
                 v.setreal("ar::yextension",rbdata->yextension);
                 v.setreal("ar::plotscale",rbdata->plotscale);
                 v.setinteger("ar::circled",rbdata->circled);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->visible.clearall();
            rbdata->visible.set(((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            insert_argrid(rbdata,1);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCARCHITECT);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBArchitect15,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+1);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect16_data
  {Point eye;
   double viewangle,height;
   View *view;
   View3dWindow *window;
  };

void RBArchitect16(int,void *data,Point *p2,View3dSurface *)
{RBArchitect16_data *rbdata = (RBArchitect16_data *) data;
  Line line(rbdata->eye,*p2);
  line.draw(DM_INVERT);
}

void architect_command16(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect16_data *rbdata = (RBArchitect16_data *) *data;
 Point ref,up(0.0,0.0,1.0);
 double l;
 ResourceString rs29(NCARCHITECT+29);
 ResourceString rs30(NCARCHITECT+30);
 ResourceString rs31(NCARCHITECT+31);
 ResourceString rs32(NCARCHITECT+32);
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentsurface() == NULL)
           {  cadwindow->MessageBox(rs30.gets(),rs29.gets(),MB_ICONINFORMATION);
              *cstate = ExitState;
           }
         else
           {  rbdata = new RBArchitect16_data;  *data = rbdata;
              if (rbdata != 0)
                {Dialog dialog("ArPerspective_dialog");
                 DialogControl *dc;
                   rbdata->height = v.getreal("ar::viewheight");
                   rbdata->viewangle = v.getreal("ar::viewangle");
                   rbdata->view = cadwindow->getcurrentsurface()->getview();
                   rbdata->window = cadwindow->getcurrentwindow();
                   dialog.add(dc = new RealDialogControl(100,&rbdata->height,ScaleByLength));
                   dialog.add(new ScrollBarDialogControl(1100,dc));
                   dialog.add(dc = new RealDialogControl(101,&rbdata->viewangle));
                   dialog.add(new ScrollBarDialogControl(1101,dc));
                   if (dialog.process() == TRUE)
                     {  v.setreal("ar::viewheight",rbdata->height);
                        v.setreal("ar::viewangle",rbdata->viewangle);
                        *cstate = 1;
                     }
                   else
                     *cstate = ExitState;
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->eye = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Exit) || event->isa(Abort))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  ref = ((Coordinate3dEvent *) event)->getp();
              rbdata->eye.z += rbdata->height;  ref.z += rbdata->height;
              db.savezoomundo(2,rbdata->window);
              if (! rbdata->view->changeview(rbdata->eye,ref,up))
                {  cadwindow->MessageBox(rs31.gets(),rs29.gets(),MB_ICONINFORMATION);
                   *cstate = 1;
                }
              else
                {  if (! rbdata->view->changeview(1))
                     {  cadwindow->MessageBox(rs32.gets(),rs29.gets(),MB_ICONINFORMATION);
                        *cstate = 1;
                     }
                   else
                     {  l = (rbdata->eye - ref).length() * tan(rbdata->viewangle/2.0*M_PI/180.0);
                        rbdata->window->zoom(-l,-l,l,l);
                        cadwindow->updatewindows(rbdata->view);
                        *cstate = 1;
                     }
                }
           }
         else if (event->isa(Exit) || event->isa(Abort))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCARCHITECT+33);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 2)
    {  cadwindow->prompt->normalprompt(NCARCHITECT+34);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect16,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class NListDialogControl : public ListDialogControl
  {public:
     NListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void NListDialogControl::load(Dialog *)
{
}

void NListDialogControl::changefocus(Dialog *dialog,int)
{RCCHAR category[300],name[300],key[300],rate[300];
 int t;
 double r;
  if (dialog->currentvalue(100,category,sizeof(category)) &&
      dialog->currentvalue(101,name,sizeof(name)))
    {  sprintf(key,"ar::Cost::%s::%s",category,name);
       strcpy(rate,v.getstring(key));
       t = 0;  r = 0.0;
       sscanf(rate,"%d,%lf",&t,&r);
       ((RadioButtonDialogControl *)dialog->getcontrol(102))->change(dialog,t);
       ((RealDialogControl *)dialog->getcontrol(106))->change(dialog,r);
    }
}

class CListDialogControl : public ListDialogControl
  {public:
     CListDialogControl(int id,int n,RCCHAR **l,RCCHAR *name,int len) : ListDialogControl(id,n,l,name,len) {}
     void load(Dialog *);
     void changefocus(Dialog *,int updatescreen = 1);
  };

void CListDialogControl::load(Dialog *dialog)
{RCCHAR key[300],rate[300];
 int i,status;
  ListDialogControl::load(dialog);
  status = v.getfirststring(_RCT("ar::Cost::"),key,rate);
  i = 0;
  while (status)
    {  if (strchr(key+10,':') != 0)
         *strchr(key+10,':') = 0;
       if (i == 0)
         ((CListDialogControl *)dialog->getcontrol(100))->change(dialog,key+10);
       ((ListDialogControl *)dialog->getcontrol(100))->removeitem(dialog,key+10);
       ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,key+10);
       status = v.getnextstring(_RCT("ar::Cost::"),key,rate);
       i++;
    }
  changefocus(dialog);
}

void CListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{RCCHAR category[300],name[300],key[300],fullkey[300];
 int n,status;
  ((ListDialogControl *)dialog->getcontrol(101))->removeallitems(dialog);
  if (dialog->currentvalue(100,category,sizeof(category)))
    {  sprintf(key,"ar::Cost::%s::",category);
       status = v.getfirststring(key,fullkey,name);
       n = 0;
       while (status)
         {  if (n == 0)
              ((ListDialogControl *)dialog->getcontrol(101))->change(dialog,fullkey+strlen(key));
            ((ListDialogControl *)dialog->getcontrol(101))->additem(dialog,fullkey+strlen(key));
            status = v.getnextstring(key,fullkey,name);
            n++;
         }
       ((NListDialogControl *)dialog->getcontrol(101))->changefocus(dialog,updatescreen);
    }
}

void architect_command20(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("ArCosting_dialog");
 DialogControl *dc;
 int type;
 double rate;
 RCCHAR name[300],key[300],srate[300],category[300];
 static RCCHAR *categoryl[] = {  _RCT("Wall"), _RCT("Floor"), _RCT("Ceiling"), _RCT("Roof"), _RCT("Column")  };
  strcpy(category,categoryl[0]);
  strcpy(name,_RCT(""));
  type = 0;
  rate = 0.0;
  dialog.add(new CListDialogControl(100,5,categoryl,category,sizeof(category)));
  dialog.add(new NListDialogControl(101,name,sizeof(name)));
  dialog.add(new RadioButtonDialogControl(102,102,105,&type));
  dialog.add(new RadioButtonDialogControl(103,102,105,&type));
  dialog.add(new RadioButtonDialogControl(104,102,105,&type));
  dialog.add(new RadioButtonDialogControl(105,102,105,&type));
  dialog.add(dc = new RealDialogControl(106,&rate));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  if (dialog.process() == TRUE && strlen(name) > 0 && strlen(category) > 0)
    {  sprintf(key,"ar::Cost::%s::%s",category,name);
       sprintf(srate,"%d,%.5lf",type,rate);
       v.setstring(key,srate);
    }
  *cstate = ExitState;
}


class SortFile
  {private:
     struct fileinfo
       {FILE *f;
        RCCHAR nextline[255];
        int nlines,eof;
       } a,b,c;
     int copy(fileinfo *x,fileinfo *y);
     void distribute(void);
     void initialize(fileinfo *fi);
     void mergerun(void);
     void merge(int *);
   public:
     SortFile(RCCHAR *name1,RCCHAR *name2,RCCHAR *name3);
  };

int SortFile::copy(fileinfo *x,fileinfo *y)
{RCCHAR line[255];
  strcpy(line,x->nextline);
  fprintf(y->f,x->nextline);
  x->eof = fgets(x->nextline,255,x->f) == 0;
  if (x->eof)
    return 1;
  else
    return strcmp(line,x->nextline) > 0;
}

void SortFile::distribute(void)
{ do {  while (! copy(&c,&a));
        if (! c.eof)
          {  while (! copy(&c,&b));
          }
     } while (! c.eof);
}

void SortFile::mergerun(void)
{ for (;;)
    {  if (strcmp(a.nextline,b.nextline) < 0)
         {  if (copy(&a,&c) != 0)
              {  while (! copy(&b,&c));
                 break;
              }
            else
              break;
         }
       else
         {  if (copy(&b,&c) != 0)
              {  while (! copy(&a,&c));
                 break;
              }
            else
              break;
         }
    }
}

void SortFile::merge(int *l)
{  *l = 0;
   while (! a.eof && ! b.eof)
     {  mergerun();
        (*l)++;
     }
   while (! a.eof)
     {  while (! copy(&a,&c));
        (*l)++;
     }
   while (! b.eof)
     {  while (! copy(&b,&c));
        (*l)++;
     }
}

void SortFile::initialize(fileinfo *fi)
{ fi->eof = fgets(fi->nextline,255,fi->f) == 0;
  fi->nlines = ! fi->eof;
}

SortFile::SortFile(RCCHAR *file1,RCCHAR *file2,RCCHAR *file3)
{int l;
  do {  if ((a.f = fopen(file2,"w")) == 0)
          return;
        if ((b.f = fopen(file3,"w")) == 0)
          {  fclose(b.f);
             return;
          }
        if ((c.f = fopen(file1,"r")) == 0)
          {  fclose(a.f);  fclose(b.f);
             return;
          }
        initialize(&c);
        distribute();
        fclose(a.f);  fclose(b.f);  fclose(c.f);
        if ((a.f = fopen(file2,"r")) == 0)
          return;
        if ((b.f = fopen(file3,"r")) == 0)
          {  fclose(a.f);
             return;
          }
        if ((c.f = fopen(file1,"w")) == 0)
          {  fclose(a.f);  fclose(b.f);
             return;
          }
        initialize(&a);  initialize(&b);
        merge(&l);
        fclose(a.f);  fclose(b.f);  fclose(c.f);
     } while (l > 1);
}

void architect_command21(int *cstate,HighLevelEvent *,void **)
{Printer printer(1);
 double tabs[20],xcost,total,subtotal,rate,quantity,subquantity;
 int i,type;
 long nrecords;
 Entity *e;
 RCCHAR line[300],category[300],description[300],cost[300],ocategory[300],odescription[300],srate[300],key[300],*at,tfile1[300],tfile2[300],tfile3[300];
 FILE *infile,*outfile;
 AttributeList al;
  for (i = 0 ; i < 20 ; i++)
    tabs[i] = i * 10.0;
  printer.print(0);
  if (printer.getdefined())
    {  program->setbusy(1);
       strcpy(tfile1,home.gettempfilename(_RCT("arch1")));
       strcpy(tfile2,home.gettempfilename(_RCT("arch2")));
       strcpy(tfile3,home.gettempfilename(_RCT("arch3")));
       if ((outfile = fopen(tfile1,"w")) == 0)
         {  cadwindow->MessageBox("The architect temporary file cannot be opened","Bill of materials",MB_ICONINFORMATION);
            return;
         }
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(figure_entity) && ((Figure *)e)->getinfo(description,category,cost))
           fprintf(outfile,"%-20s%-100s%-20s%20d\n",category,description,cost,1);
         else if (e->isa(wall_entity) && ((Wall *)e)->isacolumn())
           {  sprintf(key,"ar::Cost::Column::%s",((Wall *)e)->getname());
              strcpy(srate,v.getstring(key));
              type = 0;  rate = 0.0;
              sscanf(srate,"%d,%lf",&type,&rate);
              if (type == 0)
                quantity = 1;
              else if (type == 1)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() / 1000.0;
              else if (type == 2)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() * ((Wall *)e)->getheight() / 1000.0 / 1000.0;
              else if (type == 3)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() * ((Wall *)e)->getheight() * ((Wall *)e)->gettotalwidth() / 1000.0 / 1000.0 / 1000.0;
              fprintf(outfile,"%-20s%-100s%20lf%20.8lf\n","Column",((Wall *)e)->getname(),rate,quantity);
           }
         else if (e->isa(wall_entity) && ! ((Wall *)e)->isacolumn())
           {  sprintf(key,"ar::Cost::Wall::%s",((Wall *)e)->getname());
              strcpy(srate,v.getstring(key));
              type = 0;  rate = 0.0;
              sscanf(srate,"%d,%lf",&type,&rate);
              if (type == 0)
                quantity = 1;
              else if (type == 1)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() / 1000.0;
              else if (type == 2)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() * ((Wall *)e)->getheight() / 1000.0 / 1000.0;
              else if (type == 3)
                quantity = (((Wall *)e)->getp2() - ((Wall *)e)->getp1()).length() * ((Wall *)e)->getheight() * ((Wall *)e)->gettotalwidth() / 1000.0 / 1000.0 / 1000.0;
              fprintf(outfile,"%-20s%-100s%20lf%20.8lf\n","Wall",((Wall *)e)->getname(),rate,quantity);
           }
         else if (e->isa(plane_entity))
           {  al = e->getatlist();
              for (al.start() ; (at = al.next()) != 0 ; )
                if (strncmp(at,"ar::Cost::",10) == 0)
                  {  strcpy(srate,v.getstring(at));
                     type = 0;  rate = 0.0;
                     sscanf(srate,"%d,%lf",&type,&rate);
                     strcpy(category,at+10);
                     if (strchr(category,':') == 0 || *(strchr(category,':')+1) != ':')
                       strcpy(description,_RCT(""));
                     else
                       {  strcpy(description,strchr(category,':')+2);
                          *strchr(category,':') = 0;
                       }
                     fprintf(outfile,"%-20s%-100s%20lf%20.8lf\n",category,description,rate,((Plane *)e)->getarea(100,0.0) / 1000.0 / 1000.0);
                     break;
                  }
           }
       fclose(outfile);
       SortFile sortfile(tfile1,tfile2,tfile3);

       printer.newpage();
       printer.setmargins(10.0,10.0,10.0,10.0);
       printer.setcolumns(20,tabs);
       nrecords = 0;  quantity = 0;
       xcost = subquantity = total = subtotal = 0.0;
       if ((infile = fopen(tfile1,"r")) != 0)
         {  while (fgets(line,sizeof(line),infile) != 0 && strlen(line) > 140)
              {  if (printer.getattop())
                   {  printer.setlarge();
                      printer.printtext((char*)"Architect Schedule of Materials",0,1);
                      printer.newline(2);
                      printer.setnormal();
                   }
                 i = 19;  while (i > 0 && line[i] == ' ') line[i] = 0;
                 strncpy(category,line,20);  category[20] = 0;
                 i = 119;  while (i > 21 && line[i] == ' ') line[i] = 0;
                 strncpy(description,line+20,100);  description[100] = 0;
                 i = 139;  while (i > 121 && line[i] == ' ') line[i] = 0;
                 strncpy(cost,line+120,20);  cost[20] = 0;
                 sscanf(line+140,"%lf",&quantity);
                 if (nrecords > 0 &&
                        (strcmp(category,ocategory) != 0 ||
                         strcmp(description,odescription) != 0))
                   {  sprintf(line,"%.1lf",subquantity);
                      printer.printtext(line,3,2);
                      printer.printtext(odescription,5);
                      sprintf(line,"%2.2lf",xcost);
                      printer.printtext(line,15,2);
                      sprintf(line,"%2.2lf",xcost*subquantity);
                      printer.printtext(line,17,2);
                      subtotal += xcost * subquantity;
                      total += xcost * subquantity;
                      subquantity = 0.0;
                      printer.newline();
                   }
                 if (strcmp(category,ocategory) != 0)
                   {  printer.setbold();
                      if (nrecords > 0)
                        {  printer.newline();
                           printer.printtext((char*)"Category Total",15,2);
                           sprintf(line,"%2.2lf",subtotal);
                           printer.printtext(line,17,2);
                           printer.newline(4);
                           subtotal = 0.0;
                        }
                      printer.printtext(category,2);
                      printer.newline();
                      printer.printtext((char*)"Quantity",3,2);
                      printer.printtext((char*)"Description",5);
                      printer.printtext((char*)"Unit Cost\t\tCost",15,2);
                      printer.newline(2);
                      printer.setnormal();
                   }
                 strcpy(ocategory,category);
                 strcpy(odescription,description);
                 subquantity += quantity;
                 xcost = strtod(cost,(RCCHAR**)0);
                 nrecords++;
              }
            if (nrecords == 0)
              printer.abort();
            else
              {  sprintf(line,"%.1lf",subquantity);
                 printer.printtext(line,3,2);
                 printer.printtext(odescription,5);
                 sprintf(line,"%2.2lf",xcost);
                 printer.printtext(line,15,2);
                 sprintf(line,"%2.2lf",xcost*subquantity);
                 printer.printtext(line,17,2);
                 subtotal += xcost * subquantity;
                 total += xcost * subquantity;
                 printer.newline(2);

                 printer.setbold();
                 printer.printtext((char*)"Category Total",15,2);
                 sprintf(line,"%2.2lf",subtotal);
                 printer.printtext(line,17,2);
                 printer.newline(2);

                 printer.printtext((char*)"Grand Total",15,2);
                 sprintf(line,"%2.2lf",total);
                 printer.printtext(line,17,2);
                 printer.newline(2);
              }
            fclose(infile);
         }
       printer.newpage();
       program->setbusy(0);
#if ! defined(_WIN32_WCE)
       _unlink(tfile1);
       _unlink(tfile2);
       _unlink(tfile3);
#endif
    }
  *cstate = ExitState;
}

struct RBArchitect22_data
  {Point p1,xaxis,yaxis,zaxis;
   double width,height,snapangle;
   BitMask options;
   int state,shiftstate,gridstate;
   RCCHAR name[300];
   RBArchitect22_data() : options(32) {}
  };

void RBArchitect22(int dm,void *data,Point *p2,View3dSurface *v)
{RBArchitect22_data *rbdata = (RBArchitect22_data *) data;
 double l,angle;
 Point p1,p3,p4,p5,p6,xaxis,yaxis;
  if (rbdata->state == 1)
    {  if (v != 0)
         {  rbdata->xaxis = v->getxaxis();
            rbdata->yaxis = v->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
         }
       Wall wall(*p2+rbdata->yaxis*rbdata->width/2.0,*p2+rbdata->xaxis*rbdata->width+rbdata->yaxis*rbdata->width/2.0,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,0.0,0.0,0.0,0.0,256,256,200000.0,200000.0,0.0,0.0,0.0,0.0,0,0,rbdata->options,rbdata->options);
       wall.draw(DM_INVERT);
    }
  else
    {  if (dm == RB_ERASE && ! rbdata->shiftstate ||
           dm != RB_ERASE && ! cadwindow->getshiftstate())
         {  p3 = rbdata->p1;
            p4.x = rbdata->xaxis.dot(*p2 - p3);
            p4.y = rbdata->yaxis.dot(*p2 - p3);
            if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
            p4.x /= l;  p4.y /= l;
            if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
            angle = acos(p4.x);
            if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
            if (fabs(rbdata->snapangle) > 1.0E-10)
              angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
            p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                      rbdata->yaxis * (l * sin(angle));
            rbdata->shiftstate = 0;
         }
       else
         {  p4 = *p2;
            rbdata->shiftstate = 1;
         }
       xaxis = (p4 - rbdata->p1).normalize();
       yaxis = Point(0.0,0.0,1.0).cross(xaxis).normalize();
       Wall wall(rbdata->p1+yaxis*rbdata->width/2.0,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->width/2.0,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,0.0,0.0,0.0,0.0,256,256,200000.0,200000.0,0.0,0.0,0.0,0.0,0,0,rbdata->options,rbdata->options);
       wall.draw(DM_INVERT);
    }
}

void architect_command22(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect22_data *rbdata = (RBArchitect22_data *) *data;
 Point p1,p2,p3,p4,p5,p6,xaxis,yaxis;
 Wall *wall;
 double l,angle;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect22_data;  *data = rbdata;
       if (rbdata != 0)
         {  strcpy(rbdata->name,v.getstring("ar::columnname"));
            rbdata->width = v.getreal("ar::columnwidth");
            rbdata->height = v.getreal("ar::columnheight");
            rbdata->snapangle = v.getreal("ln::autosnapangle") * M_PI / 180.0;
            rbdata->shiftstate = 0;
            rbdata->options.set(1);
            *cstate = 1;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       db.grid.setsnap(rbdata->gridstate);
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            if (! cadwindow->getshiftstate())
              {  p3 = rbdata->p1;
                 p4.x = rbdata->xaxis.dot(p2 - p3);
                 p4.y = rbdata->yaxis.dot(p2 - p3);
                 if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                   {  p4.x /= l;  p4.y /= l;
                      if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                      angle = acos(p4.x);
                      if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
                      if (fabs(rbdata->snapangle) > 1.0E-10)
                        angle = floor(angle / rbdata->snapangle + 0.5) * rbdata->snapangle;
                      p2 = p3 + rbdata->xaxis * (l * cos(angle)) +
                                rbdata->yaxis * (l * sin(angle));
                   }
              }
            xaxis = p2 - rbdata->p1;
            if (xaxis.length() > db.getptoler())
              {  xaxis = (p2 - rbdata->p1).normalize();
                 yaxis = Point(0.0,0.0,1.0).cross(xaxis).normalize();
                 wall = new Wall(rbdata->p1+yaxis*rbdata->width/2.0,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->width/2.0,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,0.0,0.0,0.0,0.0,256,256,200000.0,200000.0,0.0,0.0,0.0,0.0,0,0,rbdata->options,rbdata->options);
                 if (wall != 0 && wall->getdefined())
                   db.geometry.add(wall);
                 else
                   delete wall;
                 *cstate = 1;
              }
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 2)
         {  rbdata->gridstate = db.grid.getsnap();
            db.grid.setsnap(0);
         }
       rubberband.begin(0);
       rubberband.add(RBArchitect22,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+65+*cstate);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArchitect23_data
  {Point   p1,p2,xaxis,yaxis,zaxis;
   double xspacing,yspacing,width,height;
   RCCHAR name[300];
  };

void RBArchitect23(int,void *data,Point *p,View3dSurface *)
{RBArchitect23_data *rbdata = (RBArchitect23_data *) data;
 Point p1,p2,p3,p4,p5;
 BitMask options(32);
  p1.setxyz(0.0,0.0,0.0);
  p2.setxyz(rbdata->xaxis.dot(*p-rbdata->p1),rbdata->yaxis.dot(*p-rbdata->p1),0.0);
  if (p2.x < p1.x)
    {  p1.x = p2.x;  p2.x = 0.0;
    }
  if (p2.y < p1.y)
    {  p1.y = p2.y;  p2.y = 0.0;
    }
  for (p3 = p1 ; p3.x <= p2.x + 0.001 ; p3.x += rbdata->xspacing)
    for (p4 = p3 ; p4.y <= p2.y + 0.001 ; p4.y += rbdata->yspacing)
      {  p5 = rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y;
         options.set(1);
         Wall wall(p5-rbdata->xaxis*rbdata->width/2.0,p5+rbdata->xaxis*rbdata->width/2.0,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,0.0,0.0,0.0,0.0,256,256,200000.0,200000.0,0.0,0.0,0.0,0.0,0,0,options,options);
         wall.draw(DM_INVERT);
      }
}

void architect_command23(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect23_data *rbdata = (RBArchitect23_data *) *data;
 Point p1,p2,p3,p4,p5,p6,xaxis;
 Wall *wall;
 Dialog dialog("ArColumnGrid_dialog");
 BitMask options(32);
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect23_data;  *data = rbdata;
       if (rbdata != 0)
         {  strcpy(rbdata->name,v.getstring("ar::columnname"));
            rbdata->width = v.getreal("ar::columnwidth");
            rbdata->height = v.getreal("ar::columnheight");
            rbdata->xspacing = v.getreal("ar::columnxspacing");
            rbdata->yspacing = v.getreal("ar::columnyspacing");
            dialog.add(new RealDialogControl(100,&rbdata->xspacing,1.0,1.0E-10,1.0E20,ScaleByLength));
            dialog.add(new RealDialogControl(101,&rbdata->yspacing,1.0,1.0E-10,1.0E20,ScaleByLength));
            if (dialog.process() == TRUE)
              *cstate = 1;
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
            rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
            rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *) event)->getp();
            p1.setxyz(0.0,0.0,0.0);
            p2.setxyz(rbdata->xaxis.dot(p2-rbdata->p1),rbdata->yaxis.dot(p2-rbdata->p1),0.0);
            if (p2.x < p1.x)
              {  p1.x = p2.x;  p2.x = 0.0;
              }
            if (p2.y < p1.y)
              {  p1.y = p2.y;  p2.y = 0.0;
              }
            for (p3 = p1 ; p3.x <= p2.x + 0.001 ; p3.x += rbdata->xspacing)
              for (p4 = p3 ; p4.y <= p2.y + 0.001 ; p4.y += rbdata->yspacing)
                {  p5 = rbdata->p1 + rbdata->xaxis * p4.x + rbdata->yaxis * p4.y;
                   options.set(1);
                   wall = new Wall(p5-rbdata->xaxis*rbdata->width/2.0,p5+rbdata->xaxis*rbdata->width/2.0,rbdata->zaxis,rbdata->name,_RCT(""),rbdata->width,rbdata->height,0.0,0.0,0.0,0.0,256,256,200000.0,200000.0,0.0,0.0,0.0,0.0,0,0,options,options);
                   if (wall != 0 && wall->getdefined())
                     db.geometry.add(wall);
                   else
                     delete wall;
                }
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 2)
         {  rubberband.begin(0);
            rubberband.add(RBArchitect23,rbdata);
         }
       cadwindow->prompt->normalprompt(NCARCHITECT+67+*cstate);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class ColNListDialogControl : public ListDialogControl
  {public:
     ColNListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void ColNListDialogControl::load(Dialog *dialog)
{RCCHAR name[300],key[300];
 int status;
  ListDialogControl::load(dialog);
  status = v.getfirststring(_RCT("ar::Cost::Column::"),key,name);
  while (status)
    {  ((ListDialogControl *)dialog->getcontrol(111))->additem(dialog,key+18);
       status = v.getnextstring(_RCT("ar::Cost::Column::"),key,name);
    }
}

void architect_command24(int *cstate,HighLevelEvent *,void **)
{double width,height;
 BitMask change(32),options(32);
 RCCHAR name[300];
 EntityList list;
 EntityHeader header;
 Entity *e;
 Dialog dialog("ArColumn_dialog");
 DialogControl *dc;
 ResourceString rs55(NCARCHITECT+55);
  *cstate = ExitState;
  list = state.getselection();
  dialog.add(new ColNListDialogControl(111,name,300));
  dialog.add(dc = new RealDialogControl(100,&width,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&height,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  if (! list.empty())
    {  dialog.title(rs55.gets());
       change.clearall();
       dialog.add(new CheckBoxDialogControl(200,&change,5));
       dialog.add(new CheckBoxDialogControl(201,&change,6));
       dialog.add(new CheckBoxDialogControl(211,&change,4));
    }
  if (list.empty())
    {  strcpy(name,v.getstring("ar::columnname"));
       width = v.getreal("ar::columnwidth");
       height = v.getreal("ar::columnheight");
       if (dialog.process() == TRUE)
         {  v.setstring("ar::columnname",name);
            v.setreal("ar::columnwidth",width);
            v.setreal("ar::columnheight",height);
         }
    }
  else
    {  strcpy(name,v.getstring("ar::columnname"));
       width = v.getreal("ar::columnwidth");
       height = v.getreal("ar::columnheight");
       if (dialog.process() == TRUE)
         {  db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if (e->isa(wall_entity) && ((Wall *) e)->isacolumn())
                ((Wall *)e)->change(change,header,name,_RCT(""),width,height,0.0,0.0,0.0,0.0,0,0,0.0,0.0,0.0,0.0,0,0,options,options);
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
}

void architect_command25(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("ArLayer_dialog");
 BitMask change(32);
 EntityList list;
 Entity *e;
 int l;
 ResourceString rs70(NCARCHITECT+70),rs71(NCARCHITECT+71),rs72(NCARCHITECT+72),
                rs73(NCARCHITECT+73),rs74(NCARCHITECT+74),rs75(NCARCHITECT+75),
                rs76(NCARCHITECT+76),rs77(NCARCHITECT+77),rs78(NCARCHITECT+78),
                rs79(NCARCHITECT+79),rs80(NCARCHITECT+80),rs81(NCARCHITECT+81),
                rs82(NCARCHITECT+82),rs83(NCARCHITECT+83),rs84(NCARCHITECT+84),
                rs85(NCARCHITECT+85),rs86(NCARCHITECT+86),rs87(NCARCHITECT+87);
 AttributeList atlist;
 RCCHAR *at;
 static RCCHAR *attribl[16] = {  _RCT("1"),_RCT("4"),_RCT("5"),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT(""),_RCT("Explode")};
 static RCCHAR *layerl[16];
 static int layernumber[] = {  900,900,900,801,802,803,804,801,802,803,804,805,806,807,808,900  };
  *cstate = ExitState;
  change.set(1);
  if (state.getselection().empty())
    {  cadwindow->MessageBox(rs70.gets(),rs71.gets(),MB_ICONSTOP);
       return;
    }
  layerl[0] = rs72.gets();
  layerl[1] = rs73.gets();
  layerl[2] = rs74.gets();
  layerl[3] = rs75.gets();
  layerl[4] = rs76.gets();
  layerl[5] = rs77.gets();
  layerl[6] = rs78.gets();
  layerl[7] = rs79.gets();
  layerl[8] = rs80.gets();
  layerl[9] = rs81.gets();
  layerl[10] = rs82.gets();
  layerl[11] = rs83.gets();
  layerl[12] = rs84.gets();
  layerl[13] = rs85.gets();
  layerl[14] = rs86.gets();
  layerl[15] = rs87.gets();
  l = v.getinteger("ar::layer");
  dialog.add(new ListDialogControl(100,16,layerl,&l));
  if (dialog.process() == TRUE)
    {  v.setinteger("ar::layer",l);
       list = state.getselection();
       db.saveundo(UD_STARTBLOCK,NULL);
       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->change(change,0,layernumber[l],0,0);
            if (strlen(attribl[l]) > 0)
              {  atlist = e->getatlist().copy();
                 for (atlist.start() ; (at = atlist.next()) != 0 ; )
                   if (strcmp(at,"1") == 0 || strcmp(at,"4") == 0 || strcmp(at,"5") == 0 || strcmp(at,"Explode") == 0)
                     {  e->delat(at);  e->setattribute(at);
                        db.saveundo(UD_DELETEATT,e);
                        delete at;
                     }
                 atlist.destroy();
                 if ((at = new RCCHAR[strlen(attribl[l])+1]) != 0)
                   {  strcpy(at,attribl[l]);
                      e->addat(at);  e->setattribute(at);
                      db.saveundo(UD_INSERTATT,e);
                   }
              }
         }
       db.saveundo(UD_ENDBLOCK,NULL);
    }
}

struct RBArchitect26_data
  {RCCHAR description[300],label[300],number[300];
   Figure *figure;
   BitMask visible;
   RBArchitect26_data() : visible(MaxViews) {}
  };

void RBArchitect26(int,void *data,Point *p,View3dSurface *)
{RBArchitect26_data *rbdata = (RBArchitect26_data *) data;
 Entity *e;
  if ((e = ArchitecturalNotation::windowdoorreference(*p,v.getreal("ar::notationscale"),rbdata->visible,rbdata->label,rbdata->number)) != 0)
    {  e->draw(DM_INVERT);
       delete e;
    }
}

void architect_command26(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect26_data *rbdata = (RBArchitect26_data *) *data;
 Entity *e1,*e2;
 Text *text1,*text2;
 RCCHAR category[300],cost[300];
 EntityList list;
 int n,nmax;
 RCCHAR *at;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect26_data;  *data = rbdata;
       if (rbdata != 0)
         *cstate = 1;
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  rbdata->figure = (Figure *) ((EntitySelectedEvent *)event)->getentity();
            rbdata->figure->getinfo(rbdata->description,category,cost);
            rbdata->visible.clearall();
            rbdata->visible.set(((EntitySelectedEvent *) event)->getwindow()->getview()->getviewno());
            strcpy(rbdata->label,strcmp(category,"Door") == 0 ? "D" : "W");
            nmax = 0;
            for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
              if (e1->isa(group_entity) &&
                    strcmp(((Group *)e1)->getclassname(),"Architect") == 0 &&
                    strcmp(((Group *)e1)->getinstance(),"NotationWDR") == 0)
                {  list = ((Group *)e1)->getlist();
                   text1 = text2 = 0;
                   for (list.start() ; (e2 = list.next()) != 0 ; )
                     if (e2->isa(text_entity))
                       {  if (text1 == 0)
                            text1 = (Text *)e2;
                          else
                            text2 = (Text *)e2;
                       }
                   if (text1 != 0 && text2 != 0)
                     if (RCSTRCMP(text1->gettext(),rbdata->label) == 0)
                       {  n = RCCHAR2LONG(text2->gettext(),0,10);
                          if (n > nmax)
                            nmax = n;
                       }
                     else if (RCSTRCMP(text2->gettext(),rbdata->label) == 0)
                       {  n = RCCHAR2LONG(text1->gettext(),0,10);
                          if (n > nmax)
                            nmax = n;
                       }
                }
            sprintf(rbdata->number,"%2.2d",nmax+1);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       rbdata->figure->unhighlight();
       if (event->isa(Coordinate3d))
         {  if ((e1 = ArchitecturalNotation::windowdoorreference(((Coordinate3dEvent *)event)->getp(),v.getreal("ar::notationscale"),rbdata->visible,rbdata->label,rbdata->number)) != 0)
              {  db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(e1);
                 if ((at = new RCCHAR[strlen(rbdata->description) + 5]) != 0)
                   {  strcpy(at,"ar::");  strcat(at,rbdata->description);
                      e1->addat(at);
                      e1->setattribute(at);
                      db.saveundo(UD_INSERTATT,e1);
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(figure_entity,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+91);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect26,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+92);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class SymbolObject : public OrderedObject
  {public:
     RCCHAR label[300],description[300];
     SymbolObject(RCCHAR *l,RCCHAR *d)  {  strcpy(label,l);  strcpy(description,d);  }
     int operator<(OrderedObject &oo)
       {  return strcmp(label,((SymbolObject *) &oo)->label) == -1;
       }
  };

class SortedSymbolList : public SortedGeneralList
  {public:
     int add(RCCHAR *l,RCCHAR *d)  {  return SortedGeneralList::add(new SymbolObject(l,d));  }
     SymbolObject *next()  {  return (SymbolObject *)GeneralList::next();  }
     void destroy(void);
  };

void SortedSymbolList::destroy(void)
{SymbolObject *oo;
  for (start() ;  (oo = next()) != 0 ; )
    delete oo;
  GeneralList::destroy();
}

struct RBArchitect27_data
  {double width,height,column;
   BitMask visible;
   RBArchitect27_data() : visible(MaxViews) {}
  };

void RBArchitect27(int,void *data,Point *p,View3dSurface *)
{RBArchitect27_data *rbdata = (RBArchitect27_data *) data;
 Point p1(0.0,0.0,0.0),p2(rbdata->width,0.0,0.0),
       p3(rbdata->width,rbdata->height,0.0),p4(0.0,rbdata->height,0.0),
       p5(rbdata->column,0.0,0.0),p6(rbdata->column,rbdata->height,0.0);
  p1 += *p;
  p2 += *p;
  p3 += *p;
  p4 += *p;
  p5 += *p;
  p6 += *p;
  Line line1(p1,p2);  line1.draw(DM_INVERT);
  Line line2(p2,p3);  line2.draw(DM_INVERT);
  Line line3(p3,p4);  line3.draw(DM_INVERT);
  Line line4(p4,p1);  line4.draw(DM_INVERT);
  Line line5(p5,p6);  line5.draw(DM_INVERT);
}

void architect_command27(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect27_data *rbdata = (RBArchitect27_data *) *data;
 double scale;
 int i;
 Point p,p1,p2,p3,p4,p5,p6,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 Line *line[5];
 Group *group;
 Entity *e1,*e2;
 BitMask options(32),visible(MaxViews);
 EntityList list,emptylist;
 RCCHAR label[300],*at;
 AttributeList al;
 int viewno,n;
 SymbolObject *so;
 RCCHAR *f,*t;
 double angle,height,width,slant;
 Text *text,*text1,*text2;
 SortedSymbolList ssl;
 Byte halocolour;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect27_data;  *data = rbdata;
       if (rbdata != 0)
         {  scale = v.getreal("ar::notationscale");
            rbdata->width = (v.getreal("ar::width1") + v.getreal("ar::width2")) * scale;
            rbdata->column = v.getreal("ar::width1") * scale;
            rbdata->height = 6.0 * scale;
            *cstate = 1;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  viewno = ((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno();
            visible.clearall();
            visible.set(viewno);
            for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
              if (e1->isa(group_entity) &&
                    strcmp(((Group *)e1)->getclassname(),"Architect") == 0 &&
                    strcmp(((Group *)e1)->getinstance(),"NotationWDR") == 0 &&
                    e1->getvisible()->test(viewno))
                {  list = ((Group *)e1)->getlist();
                   text1 = text2 = 0;
                   for (list.start() ; (e2 = list.next()) != 0 ; )
                     if (e2->isa(text_entity))
                       {  if (text1 == 0)
                            text1 = (Text *)e2;
                          else
                            text2 = (Text *)e2;
                       }
                   if (text1 != 0 && text2 != 0)
                     {  if (RCSTRCMP(text1->gettext(),"D") == 0 || RCSTRCMP(text1->gettext(),"W") == 0)
                          sprintf(label,"%s%s",text1->gettext(),text2->gettext());
                        else if (RCSTRCMP(text2->gettext(),"D") == 0 || RCSTRCMP(text2->gettext(),"W") == 0)
                          sprintf(label,"%s%s",text2->gettext(),text1->gettext());
                        else
                          strcpy(label,_RCT(""));
                        if (strlen(label) > 0)
                          {  al = e1->getatlist();
                             for (al.start() ; (at = al.next()) != 0 ; )
                               if (strncmp(at,"ar::",4) == 0)
                                 {  ssl.add(label,at+4);
                                    break;
                                 }
                          }
                     }
                }
            n = ssl.length();
            if (n > 0)
              {  list = emptylist;
                 scale = v.getreal("ar::notationscale");
                 db.saveundo(UD_STARTBLOCK,NULL);
                 p = ((Coordinate3dEvent *)event)->getp();
                 p1.setxyz(0.0,0.0,0.0);  p1 += p;
                 p2.setxyz(rbdata->width,0.0,0.0);  p2 += p;
                 p3.setxyz(rbdata->width,rbdata->height*n,0.0);  p3 += p;
                 p4.setxyz(0.0,rbdata->height*n,0.0);  p4 += p;
                 p5.setxyz(rbdata->column,0.0,0.0);  p5 += p;
                 p6.setxyz(rbdata->column,rbdata->height*n,0.0);  p6 += p;
                 line[0] = new Line(p1,p2);
                 line[1] = new Line(p2,p3);
                 line[2] = new Line(p3,p4);
                 line[3] = new Line(p4,p1);
                 line[4] = new Line(p5,p6);
                 for (i = 0 ; i < 5; i++)
                   {  line[i]->EntityHeader::change(line[i]->getcolour(),line[i]->getlayer(),line[i]->getstyle(),4);
                      line[i]->setvisible(visible);
                      db.geometry.add(line[i]);
                      list.add(line[i]);
                   }
                 for (i = 0,ssl.start() ; i < n ; i++)
                   {  so = ssl.next();
                      p1.setxyz(scale * 3.0,((n - 1 - i) * 6.0 + 1.0) * scale,0.0);
                      Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
                      text = new Text(f,p1+p,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,so->label,0,options);
                      if (text != 0 && text->getdefined())
                        {  text->setvisible(visible);
                           db.geometry.add(text);
                           list.add(text);
                        }
                      p1.setxyz(scale * 3.0 + rbdata->column,((n - 1 - i) * 6.0 + 1.0)* scale,0.0);
                      Text::getdefaults(&f,&width,&height,&slant,&angle,&halocolour,&t,&options);
                      text = new Text(f,p1+p,xaxis,yaxis,0.0,5.0 * scale,0.0,0.0,0,so->description,0,options);
                      if (text != 0 && text->getdefined())
                        {  text->setvisible(visible);
                           db.geometry.add(text);
                           list.add(text);
                        }
                      if (i != n - 1)
                        {  p1.setxyz(0.0,(n - 1 - i) * 6.0 * scale,0.0);
                           p2.setxyz(rbdata->width,(n - 1 - i) * 6.0 * scale,0.0);
                           line[0] = new Line(p1+p,p2+p);
                           line[0]->EntityHeader::change(line[0]->getcolour(),line[0]->getlayer(),line[0]->getstyle(),4);
                           line[0]->setvisible(visible);
                           db.geometry.add(line[0]);
                           list.add(line[0]);
                        }
                   }
                 options.set(0);
                 group = new Group(list,"Architect","NotationWDT",options);
                 group->setvisible(visible);
                 db.geometry.add(group);
                 db.saveundo(UD_ENDBLOCK,NULL);
                 ssl.destroy();
              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect27,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+93);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void architect_command28(int *cstate,HighLevelEvent *,void **)
{Dialog dialog("ArNotationOptions_dialog");
 DialogControl *dc;
 double scale,width1,width2;
  scale = v.getreal("ar::notationscale");
  width1 = v.getreal("ar::width1");
  width2 = v.getreal("ar::width2");
  dialog.add(dc = new RealDialogControl(100,&scale));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&width1,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&width2,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  if (dialog.process() == TRUE)
    {  v.setreal("ar::notationscale",scale);
       v.setreal("ar::width1",width1);
       v.setreal("ar::width2",width2);
    }
  *cstate = ExitState;
}



void start_architect(void)
{int perspective,viewno;
 double floorspacing,front,back;
 ResourceString rs19(NCARCHITECT+19),rs103(NCARCHITECT+103);
 RCCHAR name[256];
 Point eye,ref,up;
 View *newv;

  perspective = 0;  front = -1.0E6;   back = 1.0E6;
  eye.setxyz(0.0,0.0,1.0);
  ref.setxyz(0.0,0.0,0.0);
  up.setxyz (0.0,1.0,0.0);
  strcpy(name,rs103.gets());
  viewno = db.getnextviewno();
  if ((newv = new View(name,perspective,viewno,eye,ref,up,front,back)) != 0)
    {  db.views3d.add(newv);
       cadwindow->create3dsubwindow(newv);
    }


  floorspacing = v.getreal("ar::floorspacing");
  perspective = 0;  front = -floorspacing + 1.0;   back = 1;
  eye.setxyz(0.0,0.0,1.0);
  ref.setxyz(0.0,0.0,0.0);
  up.setxyz (0.0,1.0,0.0);
  strcpy(name,rs19.gets());
  viewno = db.getnextviewno();
  if ((newv = new View(name,perspective,viewno,eye,ref,up,front,back)) != 0)
    {  db.views3d.add(newv);
       cadwindow->create3dsubwindow(newv,20,20);
    }

}

struct RBArchitect29_data
  {RCCHAR label[300];
   Point p1,xa,ya;
   RCCHAR font[256],text[4096];
   double height,width,slant,angle;
   int length;
   Byte halocolour;
   BitMask options,visible;
   RBArchitect29_data() : options(32),visible(MaxViews) {}
  };

void RBArchitect29(int,void *data,Point *p2,View3dSurface *)
{RBArchitect29_data *rbdata = (RBArchitect29_data *) data;
  sprintf(rbdata->text+rbdata->length,"\r\n%.0lfx%.0lf",fabs(p2->x - rbdata->p1.x),fabs(p2->y - rbdata->p1.y));
  Text text(rbdata->font,(*p2+rbdata->p1)/2.0,rbdata->xa,rbdata->ya,0.0,::v.getreal("ar::notationscale") * 3.0,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->text,0,rbdata->options);
  text.setvisible(rbdata->visible);
  text.draw(DM_INVERT);
}

void architect_command29(int *cstate,HighLevelEvent *event,void **data)
{RBArchitect29_data *rbdata = (RBArchitect29_data *) *data;
 RCCHAR *f,*t;
 Dialog dialog("ArDimension_dialog");
 Point p2;
  switch (*cstate)
    {case InitialState :
       rbdata = new RBArchitect29_data;  *data = rbdata;
       if (rbdata != 0)
         {  Text::getdefaults(&f,&rbdata->width,&rbdata->height,&rbdata->slant,&rbdata->angle,&rbdata->halocolour,&t,&rbdata->options);
            strcpy(rbdata->font,f);
            rbdata->options.set(1);
            rbdata->options.clear(2);
            rbdata->options.set(3);
            rbdata->options.clear(4);
            rbdata->xa.setxyz(1.0,0.0,0.0);
            rbdata->ya.setxyz(0.0,1.0,0.0);
            strcpy(rbdata->text,v.getstring("ar::dimensionlabel"));
            dialog.add(new StringDialogControl(100,rbdata->text,sizeof(rbdata->text)));
            if (dialog.process() == TRUE)
              {  rbdata->length = strlen(rbdata->text);
                 v.setstring("ar::dimensionlabel",rbdata->text);
                 *cstate = 1;
              }
            else
              *cstate = ExitState;
         }
       else
         *cstate = ExitState;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
            rbdata->visible.clearall();
            rbdata->visible.set(((Coordinate3dEvent *) event)->getwindow()->getview()->getviewno());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p2 = ((Coordinate3dEvent *)event)->getp();
            rbdata->options.set(1);
            rbdata->options.clear(2);
            rbdata->options.set(3);
            rbdata->options.clear(4);
            sprintf(rbdata->text+rbdata->length,"\r\n%.0lfx%.0lf",fabs(p2.x - rbdata->p1.x),fabs(p2.y - rbdata->p1.y));
            Text *text = new Text(rbdata->font,(p2+rbdata->p1)/2.0,rbdata->xa,rbdata->ya,0.0,::v.getreal("ar::notationscale") * 3.0,rbdata->slant,rbdata->angle,rbdata->halocolour,rbdata->text,0,rbdata->options);
            if (text != 0 && text->getdefined())
              {  text->setvisible(rbdata->visible);
                 db.geometry.add(text);
              }
            else
              delete text;
            *cstate = 1;
         }
       else if (event->isa(Abort))
         *cstate = 1;
       else if (event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCARCHITECT+104);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBArchitect29,rbdata);
       cadwindow->prompt->normalprompt(NCARCHITECT+105);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

Point GetDSP(RCCHAR *labelname)
{RCCHAR key[300],text[300];
 Point p(0.0,0.0,0.0);
  sprintf(key,"SheetPosition-%s-X",labelname);
  GetPrivateProfileString(_RCT("SBS"),key,_RCT("0.0"),text,300,home.getinifilename());  p.x = atof(text);
  sprintf(key,"SheetPosition-%s-Y",labelname);
  GetPrivateProfileString(_RCT("SBS"),key,_RCT("0.0"),text,300,home.getinifilename());  p.y = atof(text);
  return p;
}

double GetDSSize(RCCHAR *labelname,double x)
{RCCHAR key[300],text[300],def[300];
  sprintf(key,"TextSize-%s",labelname);
  sprintf(def,"%.3lf",x);
  GetPrivateProfileString(_RCT("SBS"),key,def,text,300,home.getinifilename());
  return atof(text);
}

RCCHAR *GetJobData(const RCCHAR *name,RCCHAR *buffer)
{Entity *e;

  //  Search the database for a group entity called AdradApp and Data
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity))
      if (strcmp(((Group *)e)->getclassname(),"ArchitectApp") == 0 && strcmp(((Group *)e)->getinstance(),"Data") == 0)
        break;
 
  //  If the entity was found, look for the particular data.
  if (e != 0)
    {AttributeList atlist;
     RCCHAR *at;

       atlist = e->getatlist().copy();
       for (atlist.start() ; (at = atlist.next()) != NULL ; )
         if (strncmp(at,name,strlen(name)) == 0)
           {  strcpy(buffer,at + strlen(name));
              return buffer;
           } 
         
    }

  //  Either the group entity was not found, or the data was not found so return the empty string.
  strcpy(buffer,_RCT(""));
  return buffer;
}


RCCHAR *FormatIntersections(int bufferindex,Group *wallframe,Curve *c,int layer,double minimumenddistance,double offset,RCCHAR *dimple1,RCCHAR *dimple2)
{static RCCHAR buffer[2][600];
 SortedDoubleList sdl;
 Entity *e;
 EntityList list;
 Point p1,p2,p3,p4;
 double l;
 int i,usep1,usep2;
 double x;

  Line line1(c->point(0),c->point(1));
  Line line2(c->point(2),c->point(3));
  list = wallframe->getlist();
  l = (c->point(1) - c->point(0)).length();


  for (list.start() ; (e = list.next()) != 0 ; )
    {  usep1 = usep2 = 0;
       if (layer == STUD_LAYER && e->getlayer() == STUD_LAYER && e->isa(curve_entity) ||
           layer == LINTEL_LAYER && e->getlayer() == LINTEL_LAYER && e->isa(curve_entity) ||
           layer == LINTEL_LAYER && e->getlayer() == SILL_LAYER && e->isa(curve_entity) ||
           layer == LINTEL_LAYER && e->getlayer() == NOGGING_LAYER && e->isa(curve_entity))
         {  p1 = (((Curve *)e)->point(1) + ((Curve *)e)->point(2)) / 2.0;
            p2 = (((Curve *)e)->point(3) + ((Curve *)e)->point(4)) / 2.0;
            if (e->getlayer() == LINTEL_LAYER)
              {  p1.y -= 1.5;  p2.y -= 1.5;
              }
            else if (e->getlayer() == SILL_LAYER)
              {  p1.y += 1.5;  p2.y += 1.5;
              }
            usep1 = usep2 = 1;
         }
       else if (layer == CLIP_LAYER && e->getlayer() == CLIP_LAYER && e->isa(circle_entity))
         {  p1 = ((Circle *)e)->getorigin();
            usep1 = 1;
         }
       else if (layer == BOLT_LAYER && e->getlayer() == BOLT_LAYER && e->isa(circle_entity))
         {  p1 = ((Circle *)e)->getorigin();
            if (fabs((p1 - line1.nearp(p1)).length() + (p1 - line2.nearp(p1)).length() - (c->point(2) - c->point(1)).length()) < db.getptoler())
              {//  Bolt is in the middle of the member.
                 p1 = line1.nearp(p1);
                 usep1 = 1;
              }
         }
       else if (layer == NOTCH_LAYER && (e->getlayer() == NOTCH_LAYER || e->getlayer() == HALFHIPNOTCH_LAYER || e->getlayer() == HIPFULLNOTCH_LAYER) && e->isa(curve_entity))
         {  p1 = (((Curve *)e)->point(0) + ((Curve *)e)->point(2)) / 2.0;
            if (fabs((p1 - line1.nearp(p1)).length() + (p1 - line2.nearp(p1)).length() - (c->point(2) - c->point(1)).length()) < db.getptoler())
              {//  Notch is in the middle of the member.
                 p1 = line1.nearp(p1);
                 usep1 = 1;
              }
         }

       if (usep1)
         {  if ((p1 - line1.nearp(p1)).length() < db.getptoler() && line1.ison(p1))
              sdl.addunique((p1 - c->point(0)).length());
            if ((p1 - line2.nearp(p1)).length() < db.getptoler() && line2.ison(p1))
              sdl.addunique((p1 - c->point(3)).length());
         }
             
       if (usep2)
         {  if ((p2 - line1.nearp(p2)).length() < db.getptoler() && line1.ison(p2))
              sdl.addunique((p2 - c->point(0)).length());
            if ((p2 - line2.nearp(p2)).length() < db.getptoler() && line2.ison(p2))
              sdl.addunique((p2 - c->point(3)).length());
         }

    }

  sprintf(buffer[bufferindex],",%d",sdl.length());
  for (sdl.start(),i = 0 ; ! sdl.atend() ; i++)
    {  x = sdl.next() + offset;
       if (x < minimumenddistance)
         x = minimumenddistance;
       if (x > l + offset * 2 - minimumenddistance)
         x = l + offset * 2 - minimumenddistance;

       if (x >= 17.0 && x <= 18.99 && layer == STUD_LAYER)
         x = 17.0;
       if (l-x >= 17.0 && l-x <= 18.99 && layer == STUD_LAYER)
         x = l - 17.0;

       if (i == 0)
         sprintf(buffer[bufferindex]+strlen(buffer[bufferindex]),",%.0lf%s",x,dimple1);
       else if (sdl.atend()) 
         sprintf(buffer[bufferindex]+strlen(buffer[bufferindex]),",%.0lf%s",x,dimple2);
       else
         sprintf(buffer[bufferindex]+strlen(buffer[bufferindex]),",%.0lf",x);
    }
  sdl.destroy();

  return buffer[bufferindex];
}


void ClearStatus(EntityList *list)
{Entity *e;
  for (list->start() ; (e = list->next()) != 0 ; )
    e->setstatus(0);
}


Curve *GetNextLeftMostMember(EntityList *list,int layer)
{Entity *e;
 Curve *c;
 double x,xmin;

  //  Select the leftmost member.
  c = 0;  xmin = 0.0;
  for (list->start() ; (e = list->next()) != 0 ; )
    if (e->isa(curve_entity) && e->getstatus() == 0 &&
          (e->getlayer() == layer || layer == WEB_LAYER && (e->getlayer() == WEB1_LAYER || e->getlayer() == WEB2_LAYER || e->getlayer() == WEB3_LAYER || e->getlayer() == WEB4_LAYER)))
      {  x = (((Curve *)e)->point(0).x + ((Curve *)e)->point(1).x) / 2.0;
         if (c == 0 || x < xmin)
           {  xmin = x;
              c = (Curve *) e;
           }
      }

  if (c != 0)
    c->setstatus(1);

  return c;
}

void ExportWallFrame(FILE *outfile,Group *wallframe,int frameno)
{EntityList list,l;
 Entity *e;
 Curve *c,*c1;
 double ngextra,sdextra,slextra,llextra,studwidth;
 Point p1,p2,p3,p[5];
 int found;
 SortedPointList spl;
 BitMask options(32);

  sdextra = 70.0;
  ngextra = 35.0;
  slextra = 35.0;
  llextra = 35.0;
  studwidth = 35.0;

  fprintf(outfile,"WN,WL%3.3d\n",frameno);

  list = wallframe->getlist();

  //  Export the top plate
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,TOPPLATE_LAYER)) != 0)
    fprintf(outfile,"TP,WL%3.3d,%.0lf%s%s\n",frameno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,wallframe,c,STUD_LAYER,0.0,0.0,_RCT(""),_RCT("")),FormatIntersections(1,wallframe,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));

  //  Export the bottom plate
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,BOTTOMPLATE_LAYER)) != 0)
    fprintf(outfile,"BP,WL%3.3d,%.0lf%s\n",frameno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,wallframe,c,STUD_LAYER,0.0,0.0,_RCT(""),_RCT("")));

  //  Export the studs
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,STUD_LAYER)) != 0)
    {  fprintf(outfile,"SD,WL%3.3d,%.0lf%s\n",frameno,(c->point(0) - c->point(1)).length() + sdextra,FormatIntersections(0,wallframe,c,LINTEL_LAYER,0.0,sdextra/2.0,_RCT(""),_RCT("")));
       if (atoi(FormatIntersections(0,wallframe,c,LINTEL_LAYER,0.0,sdextra/2.0,_RCT(""),_RCT(""))+1) > 1)
         fprintf(outfile,"SB,WL%3.3d,%.0lf%s\n",frameno,(c->point(0) - c->point(1)).length() + sdextra,FormatIntersections(0,wallframe,c,LINTEL_LAYER,0.0,sdextra/2.0,_RCT(""),_RCT("")));
    }

  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,NOGGING_LAYER)) != 0)
    {  p1 = c->point(0);
       p2 = c->point(1);
       
       //  Join up other nogging pieces
       for (;;)
         {  found = 0;
            for (list.start() ; (e = list.next()) != 0 ; )
              if (e->getlayer() == NOGGING_LAYER && e->isa(curve_entity) && e->getstatus() == 0)
                {  c = (Curve *) e;
                   if ((p1 - Point(studwidth,0.0,0.0) - c->point(1)).length() < db.getptoler())
                     {  p1 = c->point(0);
                        spl.add(c->point(1));
                        c->setstatus(1);
                        found = 1;
                        break;
                     }
                   if ((p2 + Point(studwidth,0.0,0.0) - c->point(0)).length() < db.getptoler())
                     {  p2 = c->point(1);
                        spl.add(c->point(0));
                        c->setstatus(1);
                        found = 1;
                        break;
                     }
                }
            if (! found)
              break;
         }  

       fprintf(outfile,"NG,WL%3.3d,%.0lf,%d",frameno,(p2 - p1).length() + ngextra,spl.length());
       for (spl.start() ; ! spl.atend() ; )
         {  p3 = spl.next();
            //if (p3.x - p1.x > 20.0 && p3.x  - p1.x < p2.x - p1.x - 20.0)
            fprintf(outfile,",%.0lf",p3.x - p1.x);
         }
       fprintf(outfile,"\n");

       spl.destroy();

    }

  //  Export the lintels - They might need to be joined.
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,LINTEL_LAYER)) != 0)
    {  p1 = c->point(0);
       p2 = c->point(1);

       l = list;
       for (l.start() ; (e = l.next()) != 0 ; )
         if (e->getlayer() == LINTEL_LAYER && e->isa(curve_entity) && e->getstatus() == 0)
           {  c1 = (Curve *) e;
              if ((p2 - c1->point(0)).length() < 1.0)
                {  p2 = c1->point(1);
                   c1->setstatus(1);
                }
              if ((p1 - c1->point(1)).length() < 1.0)
                {  p1 = c1->point(0);
                   c1->setstatus(1);
                }
           }

       p[0] = p1;
       p[1] = p2;
       p[2] = p2;   p[2].y = c->point(2).y;
       p[3] = p1;   p[3].y = c->point(3).y;
       p[4] = p[0];
       Curve curve(5,0,p,0,0,options);

       fprintf(outfile,"LL,WL%3.3d,%.0lf%s\n",frameno,(p1 - p2).length()+slextra,FormatIntersections(0,wallframe,&curve,STUD_LAYER,0.0,slextra/2.0,_RCT(""),_RCT("")));

    }

  //  Export the sills
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,SILL_LAYER)) != 0)
    fprintf(outfile,"SL,WL%3.3d,%.0lf%s\n",frameno,(c->point(0) - c->point(1)).length()+llextra,FormatIntersections(0,wallframe,c,STUD_LAYER,0.0,slextra/2.0,_RCT(""),_RCT("")));

}

void ExportWallFrames(void)
{FILE *outfile;
 int frameno,lastframeno;
 RCCHAR text[300],filename[300];
 Entity *e,*enext;

  strcpy(filename,db.getname());
  if (strstr(filename,"Untitled") != 0)
    strcpy(filename,"WallFrame");
  if (strrchr(filename,'.') != 0)
    strrchr(filename,'.')[0] = 0;
  strcat(filename,".wll");

  if ((outfile = fopen(filename,"w")) == 0)
    {  sprintf(text,"The wall frame file '%s' could not be saved.",filename);
       cadwindow->MessageBox(text,"Export Wall Frames",MB_OK);
       return;
    }

  db.geometry.clearstatus();
  lastframeno = -1;
  for (;;)
    {  frameno = 1000000;  enext = 0;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->getstatus() == 0 && e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"Wall Frame") == 0 && atoi(((Group *)e)->getinstance()+3) < frameno && atoi(((Group *)e)->getinstance()+3) > lastframeno)
           {  enext = e;
              frameno = atoi(((Group *)e)->getinstance()+3);
           }
       if (enext == 0)
         break;
       ExportWallFrame(outfile,(Group *)enext,frameno);
       enext->setstatus(1);
       lastframeno = frameno;
    } 

  fclose(outfile);

}

void GetCrushLengths(Curve *c,Group *truss,double *crushlength1,double *crushlength2)
{EntityList list;
 Entity *e;
 double l,length;

  Line line1(c->point(0),c->point(1));
  Line line2(c->point(2),c->point(3));
  length = line1.length();
  *crushlength1 = *crushlength2 = 0;

  list = truss->getlist();
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(curve_entity) && ((Curve *)e)->npoints() == 5)
      {  Line line3(((Curve *)e)->point(0),((Curve *)e)->point(1));
         Line line4(((Curve *)e)->point(2),((Curve *)e)->point(3));
         Intersect i1(&line1,&line3);
         Intersect i2(&line1,&line4);
         Intersect i3(&line2,&line3);
         Intersect i4(&line2,&line4);

         if (i1.nintersections() > 0 && line1.ison(((PointE *)i1.intersection(0))->getp()))
           {  l = (((PointE *)i1.intersection(0))->getp() - c->point(0)).length();
              if (l < length / 2.0)
                {  if (l > *crushlength1)
                     *crushlength1 = l;
                } 
              else
                {  if (length - l > *crushlength2)
                     *crushlength2 = length - l;
                } 
              
           }

         if (i2.nintersections() > 0 && line2.ison(((PointE *)i2.intersection(0))->getp()))
           {  l = (((PointE *)i2.intersection(0))->getp() - c->point(0)).length();
              if (l < length / 2.0)
                {  if (l > *crushlength1)
                     *crushlength1 = l;
                } 
              else
                {  if (length - l > *crushlength2)
                     *crushlength2 = length - l;
                } 
              
           }

         if (i3.nintersections() > 0 && line2.ison(((PointE *)i3.intersection(0))->getp()))
           {  l = (((PointE *)i3.intersection(0))->getp() - c->point(2)).length();
              if (l < length / 2.0)
                {  if (l > *crushlength1)
                     *crushlength1 = l;
                } 
              else
                {  if (length - l > *crushlength2)
                     *crushlength2 = length - l;
                } 
              
           }

         if (i4.nintersections() > 0 && line2.ison(((PointE *)i4.intersection(0))->getp()))
           {  l = (((PointE *)i4.intersection(0))->getp() - c->point(2)).length();
              if (l < length / 2.0)
                {  if (l > *crushlength1)
                     *crushlength1 = l;
                } 
              else
                {  if (length - l > *crushlength2)
                     *crushlength2 = length - l;
                }
           }

      }

}

void ExportTruss(FILE *outfile,Group *truss,int trussno)
{int i,halftruss;
 EntityList list;
 Entity *e;
 Curve *c;
 double crushlength1,crushlength2,slotoffset;

  slotoffset = 35.0;

  fprintf(outfile,"TN,TR%2.2d\n",trussno);
  list = truss->getlist();

  //  Export the first top chord

  for (list.start(),halftruss = 0 ; (e = list.next()) != 0 ; )
    if (e->getlayer() == TOPCHORD1_LAYER || e->getlayer() == TOPCHORD2_LAYER ||  e->getlayer() == TOPCHORD3_LAYER)
      halftruss++;
  halftruss = halftruss == 1;  //  Half truss if there is only one top chord.


  //  Export the first top chord
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,TOPCHORD1_LAYER)) != 0)
    {  if (halftruss)
         fprintf(outfile,"TC,TR%2.2d TC1,%.0lf%s,R%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("DD"),_RCT("")),FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       else
         fprintf(outfile,"TC,TR%2.2d TC1,%.0lf%s,R%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("DD"),_RCT("UD")),FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));
    }

  //  Export the second top chord
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,TOPCHORD2_LAYER)) != 0)
    fprintf(outfile,"TC,TR%2.2d TC2,%.0lf%s,R%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("DD"),_RCT("DD")),FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));

  //  Export the third top chord
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,TOPCHORD3_LAYER)) != 0)
    fprintf(outfile,"TC,TR%2.2d TC3,%.0lf%s,R%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("UD"),_RCT("DD")),FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));

  //  Export the bottom chord
  ClearStatus(&list);
  while ((c = GetNextLeftMostMember(&list,BOTTOMCHORD_LAYER)) != 0)
    {  if (halftruss)
         fprintf(outfile,"BC,TR%2.2d BC,XX,%.0lf%s,1,%.0lf,C%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("UD"),_RCT("")),slotoffset,FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       else
         fprintf(outfile,"BC,TR%2.2d BC,XX,%.0lf%s,2,%.0lf,%.0lf,C%s\n",trussno,(c->point(0) - c->point(1)).length(),FormatIntersections(0,truss,c,BOLT_LAYER,0.0,0.0,_RCT("UD"),_RCT("DD")),slotoffset,(c->point(0) - c->point(1)).length()-35.0,FormatIntersections(1,truss,c,CLIP_LAYER,0.0,0.0,_RCT(""),_RCT("")));
    }

  //  Export the web members
  ClearStatus(&list);
  i = 1;
  while ((c = GetNextLeftMostMember(&list,WEB_LAYER)) != 0)
    {  GetCrushLengths(c,truss,&crushlength1,&crushlength2);
       if (c->getlayer() == WEB_LAYER)
         fprintf(outfile,"WB,TR%2.2d Web %d,%.0lf,%.0lf,%.0lf\n",trussno,i,(c->point(0) - c->point(1)).length(),crushlength1,crushlength2);
       else if (c->getlayer() == WEB1_LAYER)
         fprintf(outfile,"W1,TR%2.2d Web %d,%.0lf,%.0lf,%.0lf%s\n",trussno,i,(c->point(0) - c->point(1)).length(),crushlength1,crushlength2,FormatIntersections(0,truss,c,NOTCH_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       else if (c->getlayer() == WEB2_LAYER)
         fprintf(outfile,"W2,TR%2.2d Web %d,%.0lf,%.0lf,%.0lf%s\n",trussno,i,(c->point(0) - c->point(1)).length(),crushlength1,crushlength2,FormatIntersections(0,truss,c,NOTCH_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       else if (c->getlayer() == WEB3_LAYER)
         fprintf(outfile,"W4,TR%2.2d Web %d,%.0lf,%.0lf,%.0lf%s\n",trussno,i,(c->point(0) - c->point(1)).length(),crushlength1,crushlength2,FormatIntersections(0,truss,c,NOTCH_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       else if (c->getlayer() == WEB4_LAYER)
         fprintf(outfile,"W3,TR%2.2d Web %d,%.0lf,%.0lf,%.0lf%s\n",trussno,i,(c->point(0) - c->point(1)).length(),crushlength1,crushlength2,FormatIntersections(0,truss,c,NOTCH_LAYER,0.0,0.0,_RCT(""),_RCT("")));
       i++; 
    }

}

void ExportTrusses(void)
{FILE *outfile;
 int trussno,lasttrussno;
 RCCHAR text[300],filename[300];
 Entity *e,*enext;

  strcpy(filename,db.getname());
  if (strstr(filename,"Untitled") != 0)
    strcpy(filename,"Truss");
  if (strrchr(filename,'.') != 0)
    strrchr(filename,'.')[0] = 0;
  strcat(filename,".tru");
  if ((outfile = fopen(filename,"w")) == 0)
    {  sprintf(text,"The truss file '%s' could not be saved.",filename);
       cadwindow->MessageBox(text,"Export Trusses",MB_OK);
       return;
    }

  db.geometry.clearstatus();
  lasttrussno = -1;
  for (;;)
    {  trussno = 1000000;  enext = 0;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->getstatus() == 0 && e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"Truss") == 0 && atoi(((Group *)e)->getinstance()+4) < trussno && atoi(((Group *)e)->getinstance()+4) > lasttrussno)
           {  enext = e;
              trussno = atoi(((Group *)e)->getinstance()+4);
           }
       if (enext == 0)
         break;
       ExportTruss(outfile,(Group *)enext,trussno);
       enext->setstatus(1);
       lasttrussno = trussno;
    } 

  fclose(outfile);

}

void AddTemporaryAttribute(Entity *e)
{RCCHAR *at;
  at = new RCCHAR[strlen("TemporaryEntity") + 1];
  strcpy(at,"TemporaryEntity");
  e->addat(at);
  e->setattribute(at);
  db.saveundo(UD_INSERTATT,e);
}

View2d *CreateSBSDrawing(RCCHAR *viewname,RCCHAR *label,int sheetno)
{View2d *v;
 int i,viewno;
 BitMask foptions(32),toptions(32),visible(MaxViews);
 RCCHAR text[300],key[300],company[300],*at;
 Entity *e;
 AttributeList atlist;


  sprintf(text,"%d",sheetno);

  //  Create a new 2d drawing if necessary
  if ((v = db.views2d.match(viewname)) == 0)
    {  v = new View2d(viewname,420.0,297.0,viewno = db.getnextviewno());
       db.views2d.add(v);
    }
  else
    viewno = v->getviewno();

  //  Delete any temporary drawing entities
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getvisible()->test(viewno))
      {  atlist = e->getatlist();
         for (atlist.start() ; (at = atlist.next()) != NULL ; )
           if (strcmp(at,"TemporaryEntity") == 0)
             break;
         if (at != 0)
           {  db.geometry.del(e);
              db.geometry.start();
           }
      } 

  db.header.getvisible()->set(viewno,0);

  visible = *db.header.getvisible();
  db.header.getvisible()->clearall();
  db.header.getvisible()->set(viewno);

  //  Insert Drawing Sheet
  foptions.set(FigureOriginalStyle);
  foptions.set(FigureOriginalWeight);
  db.geometry.add(e = new Figure(home.getpublicfilename("SBS\\DrawingSheet-A3.cad"),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,foptions));  AddTemporaryAttribute(e);

  //  Insert Job Info Text
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("SheetName")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("SheetName"),5.0),0,0,0,label,0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("SheetNumber")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("SheetNumber"),3.0),0,0,0,text,0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("JobNumber")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("JobNumber"),3.0),0,0,0,GetJobData(_RCT("jobnumber::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("CreationDate")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("CreationDate"),3.0),0,0,0,GetJobData(_RCT("creationdate::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("DrawnBy")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("DrawnBy"),3.0),0,0,0,GetJobData(_RCT("drawnby::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("ClientName")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("ClientName"),4.5),0,0,0,GetJobData(_RCT("clientname::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("ClientAddress1")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("ClientAddress1"),4.5),0,0,0,GetJobData(_RCT("clientaddress1::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("ClientAddress2")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("ClientAddress2"),4.5),0,0,0,GetJobData(_RCT("clientaddress2::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("ClientAddress3")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("ClientAddress3"),4.5),0,0,0,GetJobData(_RCT("clientaddress3::"),text),0,toptions));  AddTemporaryAttribute(e);
  db.geometry.add(e = new Text(_RCT("Arial"),GetDSP(_RCT("Scale")),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,GetDSSize(_RCT("Scale"),3.0),0,0,0,_RCT("1:100"),0,toptions));  AddTemporaryAttribute(e);

  //  Insert the logo figure
  for (i = 1 ; ; i++)
    {  sprintf(key,"CompanyName%d",i);
       GetPrivateProfileString(_RCT("SBS"),key,_RCT(""),text,300,home.getinifilename());
       GetJobData(_RCT("company::"),company);
       if (strcmp(company,text) == 0 && strlen(company) > 0)
         {  sprintf(key,"CompanyFigureName%d",i);
            GetPrivateProfileString(_RCT("SBS"),key,_RCT(""),text,300,home.getinifilename());
            foptions.set(FigureOriginalStyle);
            foptions.set(FigureOriginalWeight);
            db.geometry.add(e = new Figure(home.getpublicfilename(text),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,foptions));  AddTemporaryAttribute(e);
            break;
         }
       else if (strlen(text) == 0)
         {//  Use a default logo
            GetPrivateProfileString(_RCT("SBS"),_RCT("CompanyName1"),_RCT(""),company,300,home.getinifilename());
            if (strlen(company) > 0)
              {  GetPrivateProfileString(_RCT("SBS"),_RCT("CompanyFigureName1"),_RCT(""),text,300,home.getinifilename());
                 foptions.set(FigureOriginalStyle);
                 foptions.set(FigureOriginalWeight);
                 db.geometry.add(e = new Figure(home.getpublicfilename(text),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,0,foptions));  AddTemporaryAttribute(e);
              }
            break;  
         }
    }

  db.header.setvisible(visible);

  return v;

}


void GenerateElevations(int sheetno)
{View2d *v;
 int i,j,viewno;
 Entity *e,*elast;
 BitMask foptions(32),toptions(32),visible(MaxViews);
 double SheetNumberX,SheetNumberY;
 Point xaxis,yaxis,zaxis,eye,ref,up,pmin,pmax,pmin1,pmax1;
 Transform t1,t2;
 RCCHAR text[300],titleblock[300];

  GetPrivateProfileString(_RCT("SBS"),_RCT("WallFrameTitleBlock"),_RCT(""),titleblock,300,home.getinifilename());
  GetPrivateProfileString(_RCT("SBS"),_RCT("WallFrameSheetNumberX"),_RCT("0.0"),text,300,home.getinifilename());  SheetNumberX = atof(text);
  GetPrivateProfileString(_RCT("SBS"),_RCT("WallFrameSheetNumberY"),_RCT("0.0"),text,300,home.getinifilename());  SheetNumberY = atof(text);

  //
  //  Create the elevation drawing
  //
  v = CreateSBSDrawing(_RCT("Elevations"),_RCT("Elevations"),sheetno);

  //  Make sure no existing entities are visible in this view
  viewno = v->getviewno();
  db.geometry.start();
  elast = db.geometry.next();

  for (i = 0 ; i < 4 ; i++)
    {  db.geometry.clearstatus();
       if (i == 3)
         {View front(_RCT("Front"),0,0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,-100000.0,100000.0);
          View3dWindow *view3dwindow = new View3dWindow(cadwindow,cadwindow->subwindow,&front,0,0,640,480,-50000.0,-50000.0,50000.0,50000.0);

            xaxis.setxyz(0.01,0.0,0.0);
            yaxis.setxyz(0.0,0.0,0.01);
            zaxis.setxyz(0.0,0.01,0.0);
            
            t1.translate(Point(20000.0,0.0,21000.0));
            t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
            t1.apply(t2);

            visible.clearall();
            visible.set(viewno,1);
            HideImage image(view3dwindow,&visible,&t1);
            image.hide();

            DestroyWindow(view3dwindow->gethwnd());

         } 
       else if (i == 2)
         {View front(_RCT("Back"),0,0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,-100000.0,100000.0);
          View3dWindow *view3dwindow = new View3dWindow(cadwindow,cadwindow->subwindow,&front,0,0,640,480,-50000.0,-50000.0,50000.0,50000.0);

            xaxis.setxyz(-0.01,0.0,0.0);
            yaxis.setxyz(0.0,0.0,0.01);
            zaxis.setxyz(0.0,-0.01,0.0);
            
            t1.translate(Point(-20000.0,0.0,15000.0));
            t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
            t1.apply(t2);

            visible.clearall();
            visible.set(viewno,1);
            HideImage image(view3dwindow,&visible,&t1);
            image.hide();

            DestroyWindow(view3dwindow->gethwnd());

         } 
       else if (i == 1)
         {View front(_RCT("Left"),0,0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,-100000.0,100000.0);
          View3dWindow *view3dwindow = new View3dWindow(cadwindow,cadwindow->subwindow,&front,0,0,640,480,-50000.0,-50000.0,50000.0,50000.0);

            xaxis.setxyz(0.0,0.01,0.0);
            yaxis.setxyz(0.0,0.0,0.01);
            zaxis.setxyz(-0.01,0.0,0.0);
            
            t1.translate(Point(20000.0,0.0,9000.0));
            t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
            t1.apply(t2);

            visible.clearall();
            visible.set(viewno,1);
            HideImage image(view3dwindow,&visible,&t1);
            image.hide();

            DestroyWindow(view3dwindow->gethwnd());

         } 
       else if (i == 0)
         {View front(_RCT("Right"),0,0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,-100000.0,100000.0);
          View3dWindow *view3dwindow = new View3dWindow(cadwindow,cadwindow->subwindow,&front,0,0,640,480,-50000.0,-50000.0,50000.0,50000.0);

            xaxis.setxyz(0.0,-0.01,0.0);
            yaxis.setxyz(0.0,0.0,0.01);
            zaxis.setxyz(0.1,0.0,0.0);
            
            t1.translate(Point(20000.0,0.0,3000.0));
            t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
            t1.apply(t2);

            visible.clearall();
            visible.set(viewno,1);
            HideImage image(view3dwindow,&visible,&t1);
            image.hide();

            DestroyWindow(view3dwindow->gethwnd());

         } 

       for (db.geometry.start(),j = 0 ; (e = db.geometry.next()) ; j++)
         if (e->getstatus())
           {  if (j == 0)
                e->extents(0,&pmin,&pmax);
              else
                {  e->extents(0,&pmin1,&pmax1);
                   if (pmin1.x < pmin.x) pmin.x = pmin1.x;
                   if (pmax1.x > pmax.x) pmax.x = pmax1.x;
                }
           }

       if (j > 0)
         {Transform t;
            t.translate(180.0 - (pmin.x + pmax.x) / 2.0,0.0,0.0);
            for (db.geometry.start(),j = 0 ; (e = db.geometry.next()) ; j++)
              if (e->getstatus())
                e->move(&db,t);

            e = new Line(Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0,0.0),Point(180.0 + (pmax.x - pmin.x) / 2.0 + 40.0,30.0 + i * 60.0,0.0));
            e->setvisible(visible);
            db.geometry.add(e);

            if (i == 3)
              e = new Text((char*)"Arial",Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0 - 10.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,8.0,0,0,0, (char*)"Front elevation",0,toptions);
            else if (i == 2)
              e = new Text((char*)"Arial",Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0 - 10.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,8.0,0,0,0, (char*)"Rear elevation",0,toptions);
            else if (i == 1)
              e = new Text((char*)"Arial",Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0 - 10.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,8.0,0,0,0, (char*)"Right elevation",0,toptions);
            else if (i == 0)
              e = new Text((char*)"Arial",Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0 - 10.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,8.0,0,0,0, (char*)"Left elevation",0,toptions);
            e->setvisible(visible);
            db.geometry.add(e);

            e = new Text((char*)"Arial",Point(180.0 - (pmax.x - pmin.x) / 2.0 - 40.0,30.0 + i * 60.0 + 1.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,3.0,0,0,0, (char*)"Floor",0,toptions);
            e->setvisible(visible);
            db.geometry.add(e);

         }

    }

  //  Insert the temporary attribute to all the new entities
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 && e != elast ; )
    AddTemporaryAttribute(e);

}

void UpdateLayerTable(int general,int roofshape,int roofplan,int trussshape,int trussplan,int wallframe,BitMask *layertable)
{int i;
  for (i = 0 ; i < 1000 ; i++)
    {  if (i == TRUSSPLAN_LAYER)
         layertable->set(i,trussplan);
       else if (i >= 720 && i <= 749)
         layertable->set(i,trussshape);
       else if (i == ROOFPLAN_LAYER)
         layertable->set(i,roofplan);
       else if (i >= 700 && i <= 719) 
         layertable->set(i,roofshape);
       else if (i >= 750 && i <= 774)
         layertable->set(i,wallframe);
       else
         layertable->set(i,general);
    }  
}

void Delete2dView(RCCHAR *viewname,View *view)
{View2d *v;
 Entity *e;
  if ((v = db.views2d.match(viewname)) != 0)
    {  cadwindow->deletewindows(v); 
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         {  if ((e->getvisible()->test(v->getviewno()) || view != 0 && e->getvisible()->test(view->getviewno())) && ! e->getvisible()->test(0))
              db.geometry.del(e);
         }
       db.views2d.del(v);
       delete v; 
    }

}

#define WebWidth           74.93
#define BottomChordWidth   83.31
#define BCHoleOffsetB      51.69
#define BCHoleOffsetT      31.62
#define WebHolePosition    35.0
#define ApexXOffset        55.0 
#define ApexYOffset        70.0    //  Irish 80.0

void InsertTrussMember(int layer,Point p1,Point p2,double width,EntityList *list)
{Point xa,ya,p[5];
 BitMask options(32);
 Curve *c;


  xa = (p2 - p1).normalize();
  ya = Point(0.0,0.0,1.0).cross(xa).normalize() * width;
  p[0] = p1;
  p[1] = p2;
  p[2] = p2 + ya;
  p[3] = p1 + ya;
  p[4] = p1;
  c = new Curve(5,0,p,0,0,options);
  c->EntityHeader::change(c->getcolour(),layer,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);
}


void InsertCeilingClips(Point p1,Point p2,double wallgap,double spacing,EntityList *list)
{double l;
 int i,n;
 Circle *c;

  l = (p2 - p1).length() / 2.0;
  n = int(((l - wallgap) / spacing) + 0.95);
  //  Centre clip
  db.geometry.add(c = new Circle(p1 + (p2 - p1).normalize() * l,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,acos(-1.0),acos(-1.0)));
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  list->add(c);

  //  Insert clips from the centre out
  for (i = 1 ; i < n ; i++)
    {  c = new Circle(p1 + (p2 - p1).normalize() * (l + i * spacing),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,acos(-1.0),acos(-1.0));
       c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
       db.geometry.add(c);
       list->add(c);
       c = new Circle(p1 + (p2 - p1).normalize() * (l - i * spacing),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,acos(-1.0),acos(-1.0));
       c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
       db.geometry.add(c);
       list->add(c);
    }
  //  Insert the two clips next to the wall
  c = new Circle(p1 + (p2 - p1).normalize() * wallgap,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,acos(-1.0),acos(-1.0));  
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);
  c = new Circle(p2 + (p1 - p2).normalize() * wallgap,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,acos(-1.0),acos(-1.0));
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);
}

void InsertRoofClips(Point p1,Point p2,double topgap,double bottomgap,double spacing1,double spacing2,EntityList *list)
{double a,l,d;
 Point dir,zaxis;
 Circle *c;

  a = atan2(p2.y - p1.y,p2.x - p1.x);
  l = (p2 - p1).length();

  dir = (p2 - p1).normalize();
  if (p1.x > p2.x)
    {  zaxis.setxyz(0.0,0.0,1.0);
       a += acos(-1.0);
    }
  else
    zaxis.setxyz(0.0,0.0,-1.0);  

  //  Insert the top clip
  c = new Circle(p1 + dir * topgap + dir.cross(zaxis) * BottomChordWidth,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,a,acos(-1.0));
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);

  //  Insert the middle clips
  for (d = topgap + spacing1 ; d < l - bottomgap - spacing1 -  40.0 ; d += spacing2)
    {  c = new Circle(p1 + dir * d + dir.cross(zaxis) * BottomChordWidth,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,a,acos(-1.0));
       c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
       db.geometry.add(c); 
       list->add(c);
    } 

  //  Insert the bottom clips
  c = new Circle(p2 - dir * (bottomgap + spacing1) + dir.cross(zaxis) * BottomChordWidth,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,a,acos(-1.0));
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);
  c = new Circle(p2 - dir * bottomgap + dir.cross(zaxis) * BottomChordWidth,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,a,acos(-1.0));
  c->EntityHeader::change(c->getcolour(),CLIP_LAYER,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);

}

void InsertNotch(int layer,Point p1,EntityList *list)
{Point pt[5];
 Curve *c;
 BitMask options(32);

  pt[0] = p1 + Point(-10.0,-10.0,0.0);
  pt[1] = p1 + Point(-10.0,10.0,0.0);
  pt[2] = p1 + Point(10.0,10.0,0.0);
  pt[3] = p1 + Point(10.0,-10.0,0.0);
  pt[4] = p1 + Point(-10.0,-10.0,0.0);
  c = new Curve(5,0,pt,0,0,options);
  c->EntityHeader::change(c->getcolour(),layer,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);

}

void GenerateHipTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,matched,trusscategory;
 Point p1,p2,p3a,p3b,p4,p5,p6,p7,p8,xa,ya,tcdir,bcdir,dir;
 double l,bclength,topnotch,deltalength,pitch,scale;
 SortedDoubleList sdl;
 Entity *e;
 EntityList list;
 Circle *cr;
 Point point[5];

  //  Points - Standard
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Top of main vertical web
  //    point(3)   Eave end of the top chord

  //  Points - Flyover
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Top of main vertical web
  //    point(3)   Apex point
  //    point(4)   Eave end of the top chord

  topnotch = (c->point(2).z - c->point(0).z) - 230.0;
  if (c->npoints() == 4)
    {  point[0] = c->point(0);
       point[1] = c->point(1);
       point[2] = c->point(2);
       point[3] = c->point(3);
    }
  else
    {  point[0] = c->point(0);
       point[1] = c->point(1);
       point[2] = c->point(2);
       point[3] = c->point(4);
       point[4] = c->point(3);
    }

  //  Adjust the length of the bottom chord - to force the holes to be 35mm from the end.
  bcdir = point[2] - point[0];  bcdir.z = 0.0;
  pitch = atan2(fabs(point[2].z - point[0].z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (point[1] - point[0]).length();
  scale = (bclength + deltalength) / bclength;
  bcdir = point[3] - point[0];
  tcdir = point[4] - point[2];
  p1 = point[0];
  point[0] = (point[0] - p1) * scale + p1;
  point[1] = (point[1] - p1) * scale + p1;
  point[2] = (point[2] - p1) * scale + p1;
  point[3] = point[0] + bcdir;
  point[4] = point[2] + tcdir;

  bclength = (point[1] - point[0]).length();
  point[1] -= (point[1] - point[0]).normalize() * 98.0;
  point[2] -= (point[1] - point[0]).normalize() * 98.0 + Point(0.0,0.0,98.0/bclength*(point[2].z - point[1].z));

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p5.setxyz(0.0,0.0,0.0);
  p1 = p5 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < 4 ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p6 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 2)
         p3a = p6 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3b = p5 + (p6 - p5).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4 = p6 + (p6 - p5).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p5 = p6;  
    }


  Line bcline(point[0],point[1]);
  //  First make a list of half (hip) trusses that touch this truss.
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(curve_entity) && e->getlayer() == HIPHALFTRUSS_LAYER)
      if ((bcline.nearp(((Curve *)e)->point(1)) - ((Curve *)e)->point(1)).length() < 2.0 && ((Curve *)e)->npoints() >= 3)
        {  l = (((Curve *)e)->point(1) - point[0]).length();
           if (l > bcline.length() - WebWidth/2.0)
             l = bcline.length() - WebWidth / 2.0;
           sdl.start();
           matched = 0;
           while (! matched && ! sdl.atend())
             matched = fabs(l - sdl.next()) < 2.0;
           if (! matched && (((Curve *)e)->point(2) - ((Curve *)e)->point(1)).length() > 290.0)
             sdl.add(l);
        }



  Line line1(o+p1,o+p2);
  Line line2(o+p2,o+p3a);
  Line line3(o+p3b,o+p4);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line1);

  //  Now insert the members
  p5.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < 4 ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         InsertTrussMember(BOTTOMCHORD_LAYER,o+p5+Point(deltalength,0.0,0.0),o+p6,BottomChordWidth,&list);
       else if (i == 2)
         {  l = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i2.intersection(0))->getp()).length();
            p7 = o + p5 + (p6 - p5).normalize()*(BCHoleOffsetB - WebHolePosition);
            p8 = o + p5 + (p6 - p5).normalize()*(BCHoleOffsetB + l + WebHolePosition);
            InsertTrussMember(WEB4_LAYER,p7,p8,WebWidth,&list);
            InsertNotch(HIPFULLNOTCH_LAYER,p7 + Point(-WebWidth/2.0,230.0,0.0),&list);  
            if (topnotch > 350.0)
              InsertNotch(HIPFULLNOTCH_LAYER,p7 + Point(-WebWidth/2.0,topnotch,0.0),&list);
         }   
       else if (i == 3)
         {  if (c->npoints() == 4)
              InsertTrussMember(TOPCHORD1_LAYER,o+p6,o+p5,BottomChordWidth,&list);
            else 
              {//  Flyover half truss
                 p5.setxyz(xa.dot(point[4] - point[0]),point[4].z - point[0].z,0.0);
                 InsertTrussMember(TOPCHORD1_LAYER,o+p6,o+p5,BottomChordWidth,&list);
              }
         }
       p5 = p6;  
    }

  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  bclength = (p2 - p1).length();
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
    }

  //  Insert the remaining webs
  p5 = ((PointE *)i3.intersection(0))->getp();
  tcdir = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).normalize();
  tcdir /= tcdir.x;
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).normalize();

  for (sdl.start() ; ! sdl.atend() ; )
    {  l = sdl.next() - (p5.x - o.x);
       cr = new Circle(p5 + bcdir * (l - 66.0 + deltalength),10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       cr = new Circle(p5 + tcdir * (l - 66.0 + deltalength),10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  sdl.start();
  if (! sdl.atend())
    l = sdl.next() - (p5.x - o.x);
  else
    l = -1.0;
  while (l > 0.0)
    {  p6 = p5 + tcdir * (l - 66.0 + deltalength);
       p7 = p5 + bcdir * (l - 66.0 + deltalength);
       dir = (p7 - p6).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p6 = p6 - (p7 - p6).normalize() * WebHolePosition;
       p7 = p7 + (p7 - p6).normalize() * WebHolePosition;
       InsertTrussMember(WEB3_LAYER,p7-dir,p6-dir,WebWidth,&list);

       InsertNotch(HALFHIPNOTCH_LAYER,p7 + Point(0.0,230.0,0.0),&list);  
       if (tcdir.y * (l + (p5.x - o.x)) - 230.0 > 350.0)
         InsertNotch(HALFHIPNOTCH_LAYER,p7 + Point(0.0,tcdir.y * (l + (p5.x - o.x)) - 230.0,0.0),&list);
       
       p7 = p5 + bcdir * (l - 66.0 + deltalength);
       if (! sdl.atend())
         l = sdl.next() - (p5.x - o.x);
       else
         l = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).length() + 66.0 - deltalength;
       p6 = p5 + tcdir * (l - 66.0 + deltalength);
       dir = (p7 - p6).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p6 = p6 - (p7 - p6).normalize() * WebHolePosition;
       p7 = p7 + (p7 - p6).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p6+dir,p7+dir,WebWidth,&list);

       if (fabs(l - (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).length() - 66.0 + deltalength) < 1.0)
         l = -1.0;

    }

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));

}

void GenerateHalfTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,trusscategory;
 Point p1,p2,p3a,p3b,p4,p5,p6,p7,p8,xa,ya,tcdir,bcdir,dir,point[5];
 double l,bclength,topnotch,pitch,deltalength,scale;
 EntityList list;
 Circle *cr;

  //  Points - Standard half truss
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Top of main vertical web
  //    point(3)   Eave end of the top chord

  //  Points - Flyover half truss
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Top of main vertical web
  //    point(3)   Apex of flyover
  //    point(4)   Eave end of the top chord

  topnotch = (c->point(2).z - c->point(0).z) - 230.0;

  if (c->npoints() == 4)
    {  point[0] = c->point(0);
       point[1] = c->point(1);
       point[2] = c->point(2);
       point[3] = c->point(3);
    }
  else
    {  point[0] = c->point(0);
       point[1] = c->point(1);
       point[2] = c->point(2);
       point[3] = c->point(4);
       point[4] = c->point(3);
    }

  if (c->getlayer() == HIPHALFTRUSS_LAYER)
    {  //  Shorten truss by 94mm
       bclength = (point[1] - point[0]).length();
       point[1] -= (point[1] - point[0]).normalize() * 66.0;
       point[2] -= (point[1] - point[0]).normalize() * 66.0 + Point(0.0,0.0,66.0/bclength*(point[2].z - point[1].z));
    }
  else if (c->getlayer() == DUTCHGABLEHALFTRUSS_LAYER)
    {  //  Dutch gable half truss shorten by 70mm
       bclength = (point[1] - point[0]).length();
       point[1] -= (point[1] - point[0]).normalize() * 42.0;
       point[2] -= (point[1] - point[0]).normalize() * 42.0 + Point(0.0,0.0,42.0/bclength*(point[2].z - point[1].z));
    }

  //  Adjust the length of the bottom chord - to force the holes to be 35mm from the end.
  bcdir = point[2] - point[0];  bcdir.z = 0.0;
  pitch = atan2(fabs(point[2].z - point[0].z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (point[1] - point[0]).length();
  scale = (bclength + deltalength) / bclength;
  bcdir = point[3] - point[0];
  tcdir = point[4] - point[2];
  point[0] *= scale;
  point[1] *= scale;
  point[2] *= scale;
  point[3] = point[0] + bcdir;
  point[4] = point[2] + tcdir;

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p5.setxyz(0.0,0.0,0.0);
  p1 = p5 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < 4 ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p6 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 2)
         p3a = p6 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3b = p5 + (p6 - p5).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4 = p6 + (p6 - p5).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p5 = p6;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p2,o+p3a);
  Line line3(o+p3b,o+p4);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line1);

  //  Now insert the members
  p5.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < 4 ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         InsertTrussMember(BOTTOMCHORD_LAYER,o+p5+Point(deltalength,0.0,0.0),o+p6,BottomChordWidth,&list);
       else if (i == 2)
         {  l = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i2.intersection(0))->getp()).length();
            p7 = o + p5 + (p6 - p5).normalize() * (BCHoleOffsetB - WebHolePosition);
            p8 = o + p5 + (p6 - p5).normalize() * (BCHoleOffsetB + l + WebHolePosition);
            InsertTrussMember(WEB2_LAYER,p7,p8,WebWidth,&list);
            InsertNotch(NOTCH_LAYER,p7 + Point(-WebWidth/2.0,230.0,0.0),&list);  
            if (topnotch > 350.0)
              InsertNotch(NOTCH_LAYER,p7 + Point(-WebWidth/2.0,topnotch,0.0),&list);
         }   
       else if (i == 3)
         {  if (c->npoints() == 4)
              InsertTrussMember(TOPCHORD1_LAYER,o+p6,o+p5,BottomChordWidth,&list);
            else 
              {//  Flyover half truss
                 p5.setxyz(xa.dot(point[4] - point[0]),point[4].z - point[0].z,0.0);
                 InsertTrussMember(TOPCHORD1_LAYER,o+p6,o+p5,BottomChordWidth,&list);
              }
         }
       p5 = p6;  
    }

  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  bclength = (p2 - p1).length();
  if (0) //highwindspeed
    {  if (bclength * 2 < 6000.0)
         trusscategory = 3;
       else if (bclength * 2 < 9000.0)
         trusscategory = 4;
       else if (bclength * 2 < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength * 2 < 4500.0)
         trusscategory = 1;
       else if (bclength * 2 < 10500.0)
         trusscategory = 3;
       else if (bclength * 2 < 12000.0)
         trusscategory = 4;
       else
         trusscategory = 5; 
    }

  //  Insert the remaining webs
  p5 = ((PointE *)i3.intersection(0))->getp();
  tcdir = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / trusscategory;
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / (trusscategory > 1 ? trusscategory - 1 : 1);

  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + tcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory-1  ; i++)
    {  cr = new Circle(p5 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p6 = p5 + tcdir * ((i+1)/2 + 1);
       p7 = p5 + bcdir * (i/2+1);
       dir = (p7 - p6).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p6 = p6 - (p7 - p6).normalize() * WebHolePosition;
       p7 = p7 + (p7 - p6).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p6+dir,p7+dir,WebWidth,&list);
    } 

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));

}

void GenerateFullTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,trusscategory;
 Point p1,p2,p3,p4a,p4b,p5,p6,p7,p8,xa,ya,tcdir1,tcdir2,bcdir,dir,point[5];
 double bclength,pitch,deltalength,scale;
 EntityList list;
 Circle *cr;

  //  Points
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Eave end of the right hand top chord
  //    point(3)   Apex point
  //    point(4)   Eave end of the left hand top chord
  point[0] = c->point(0);
  point[1] = c->point(1);
  point[2] = c->point(2);
  point[3] = c->point(3);
  point[4] = c->point(4);
  
  bcdir = point[3] - point[2];  bcdir.z = 0.0;

  //  Adjust the length of the bottomn chord - to force the holes to be 35mm from the end.
  pitch = atan2(fabs(c->point(3).z - c->point(2).z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (c->point(1) - c->point(0)).length();
  scale = (bclength + 2.0 * deltalength) / bclength;
  tcdir1 = point[4] - point[0];
  tcdir2 = point[2] - point[1];
  point[0] *= scale;
  point[1] *= scale;
  point[3] *= scale;
  point[2] = point[1] + tcdir2;
  point[4] = point[0] + tcdir1;

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p6.setxyz(0.0,0.0,0.0);
  p1 = p6 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < c->npoints() ; i++)
    {  p7.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p7 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3 = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4a = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       else if (i == 4)
         {  p4b = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p5 = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p6 = p7;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p3,o+p4a);
  Line line3(o+p4b,o+p5);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line1);

  bclength = (p2 - p1).length();
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
       else
         trusscategory = 6; 
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 7500.0)
         trusscategory = 2;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
       else 
         trusscategory = 5;
    }

  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  tcdir1 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / trusscategory;
  tcdir2 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i1.intersection(0))->getp()) / trusscategory;
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / (trusscategory - 0.5) / 2.0;

  //  Now insert the members
  p5.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < c->npoints() ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         {  InsertTrussMember(BOTTOMCHORD_LAYER,o+p5+Point(deltalength,0.0,0.0),o+p6-Point(deltalength,0.0,0.0),BottomChordWidth,&list);
            InsertCeilingClips(o+p5+Point(bclength,0.0,0.0),o+p6-Point(bclength,0.0,0.0),150.0,600.0,&list);
         }
       else if (i == 3)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) + (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD1_LAYER,o+p7,o+p5,BottomChordWidth,&list);
            InsertRoofClips(o+p7,o+p5,140.0,110.0,900.0,1200.0,&list);
         }
       else if (i == 4)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) - (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD2_LAYER,o+p6,o+p7,BottomChordWidth,&list);
            InsertRoofClips(o+p6,o+p7,140.0,110.0,900.0,1200.0,&list);
         } 
       p5 = p6;  
    }

  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  //  Insert the remaining webs
  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + tcdir1 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p6 + tcdir2 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       cr = new Circle(p6 - bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  for (i = 0 ; i < trusscategory * 2 - 2 ; i++)
    {  p7 = p5 + tcdir1 * ((i+1)/2 + 1);
       if (i == trusscategory * 2 - 3)
         p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
       p8 = p5 + bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);

       p7 = p6 + tcdir2 * ((i+1)/2 + 1);
       if (i == trusscategory * 2 - 3)
         p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
       p8 = p6 - bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);
    } 

  if (trusscategory == 1)
    {//  Generate a single vertical web member for these trusses.
       p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
       p8 = p5 + bcdir * 0.5;
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);

       cr = new Circle(p5 + bcdir * 0.5,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       
    }

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));


}

void GenerateVerticalWebTruncatedTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,trusscategory;
 Point p1,p2,p3,p4a,p4b,p5a,p5b,p6,p7,p8,p9,p10,p11,p12,xa,ya,tcdir1,tcdir2,bcdir,dir,point[6];
 double bclength,topnotch,deltalength,pitch;
 EntityList list;
 Circle *cr;

  //  Points
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Eave end of the right hand top chord
  //    point(3)   Right hand top chord
  //    point(4)   Left hand top chord
  //    point(5)   Eave end of the left hand top chord

  point[0] = c->point(0);
  point[1] = c->point(1);
  point[2] = c->point(2);
  point[3] = c->point(3);
  point[4] = c->point(4);
  point[5] = c->point(5);
  
  bcdir = point[3] - point[2];  bcdir.z = 0.0;

  //  Adjust the length of the bottom chord - to force the holes to be 35mm from the end.
  pitch = atan2(fabs(c->point(3).z - c->point(2).z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (c->point(1) - c->point(0)).length();
  tcdir1 = point[4] - point[0];
  tcdir2 = point[2] - point[1];
  point[0] -= Point(deltalength,0.0,0.0);
  point[1] += Point(deltalength,0.0,0.0);
  point[2] += Point(deltalength,0.0,0.0);
  point[3] += Point(deltalength,0.0,0.0);
  point[4] -= Point(deltalength,0.0,0.0);
  point[5] -= Point(deltalength,0.0,0.0);

  topnotch = (c->point(3).z - c->point(0).z) - 230.0;

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p7.setxyz(0.0,0.0,0.0);
  p1 = p7 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < c->npoints() ; i++)
    {  p8.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p8 + Point(0.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3 = p8 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4a = p7 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       else if (i == 4)
         {  p4b = p8 - (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetB;
            p5a = p7 - (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetB;
         }   
       else if (i == 5)
         {  p5b = p8 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p6 = p7 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p7 = p8;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p3,o+p4a);
  Line line3(o+p4b,o+p5a);
  Line line4(o+p5b,o+p6);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line4);
  Intersect i4(&line4,&line1);

  bclength = fabs(((PointE *)i2.intersection(0))->getp().x - ((PointE *)i1.intersection(0))->getp().x) * 2;
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
    }

  p7 = ((PointE *)i3.intersection(0))->getp();
  p8 = ((PointE *)i1.intersection(0))->getp();
  tcdir1 = (((PointE *)i3.intersection(0))->getp() - ((PointE *)i4.intersection(0))->getp()) / trusscategory;
  tcdir2 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i1.intersection(0))->getp()) / trusscategory;
  Line bcline(((PointE *)i4.intersection(0))->getp(),((PointE *)i1.intersection(0))->getp());
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i4.intersection(0))->getp()).normalize() * (bcline.nearp(((PointE *)i3.intersection(0))->getp()) - ((PointE *)i4.intersection(0))->getp()).length()  / (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);

  //  Now insert the members
  p7.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < c->npoints() ; i++)
    {  p8.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         {  InsertTrussMember(BOTTOMCHORD_LAYER,o+p7+Point(deltalength,0.0,0.0),o+p8-Point(deltalength,0.0,0.0),BottomChordWidth,&list);
            InsertCeilingClips(o+p7+Point(deltalength,0.0,0.0),o+p8-Point(deltalength,0.0,0.0),150.0,600.0,&list);
         }
       else if (i == 3)
         {  p9 = ((PointE *)i2.intersection(0))->getp() + (p8 - p7).normalize() * 35.0;
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD1_LAYER,p9,o+p7,BottomChordWidth,&list);
            InsertRoofClips(p9,o+p7,140.0,110.0,900.0,1200.0,&list);
         }
       else if (i == 4)
         {  p9 = ((PointE *)i2.intersection(0))->getp();
            p10 = ((PointE *)i3.intersection(0))->getp();
            p9 = p9 + (p9 - p10).normalize() * 35.0; 
            p10 = p10 + (p10 - p9).normalize() * 35.0; 
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            p10 = p10 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD3_LAYER,p10+Point(110.0,0.0,0.0),p9-Point(110.0,0.0,0.0),BottomChordWidth,&list);
         } 
       else if (i == 5)
         {  p9 = ((PointE *)i3.intersection(0))->getp() - (p8 - p7).normalize() * 35.0;
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD2_LAYER,o+p8,p9,BottomChordWidth,&list);
            InsertRoofClips(o+p8,p9,140.0,110.0,900.0,1200.0,&list);
         } 
       p7 = p8;  
    }

  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp()-Point(162.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp()-Point(110.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp()+Point(110.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp()+Point(162.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i4.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  p11 = (((PointE *)i1.intersection(0))->getp() + ((PointE *)i4.intersection(0))->getp())/2.0;
  cr = new Circle(p11,10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  p12 = (((PointE *)i2.intersection(0))->getp() + ((PointE *)i3.intersection(0))->getp())/2.0;
  cr = new Circle(p12,10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  //  Insert the remaining webs
  p7 = ((PointE *)i4.intersection(0))->getp();
  p8 = ((PointE *)i1.intersection(0))->getp();
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p7 + tcdir1 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p8 + tcdir2 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; trusscategory > 1 && i < trusscategory-1 || trusscategory == 1 && i < trusscategory  ; i++)
    {  cr = new Circle(p7 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       cr = new Circle(p8 - bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p9 = p7 + tcdir1 * ((i+1)/2 + 1);
       p10 = p7 + bcdir * (i/2+1);
       if (i == trusscategory * 2 - 4)
         p10.x += 55.0;
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

       p9 = p8 + tcdir2 * ((i+1)/2 + 1);
       p10 = p8 - bcdir * (i/2+1);
       if (i == trusscategory * 2 - 4)
         p10.x -= 55.0;
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);
    } 

  p9 = p8 + tcdir2 * trusscategory;
  p10 = p8 - bcdir * (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir-Point(55.0,77.0,0.0),p10+dir-Point(55.0,0.0,0.0),WebWidth,&list);
     
  p9 = p8 + tcdir2 * trusscategory;
  cr = new Circle(p9-Point(55.0,77.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  p9 = p7 + tcdir1 * trusscategory;
  p10 = p7 + bcdir * (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir+Point(55.0,-77.0,0.0),p10+dir+Point(55.0,0.0,0.0),WebWidth,&list);

  p9 = p7 + tcdir1 * trusscategory;
  cr = new Circle(p9+Point(55.0,-77,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  cr = new Circle(p8 - bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(-55.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(p8 - bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(-162.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(p7 + bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(55.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(p7 + bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(162.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  p9 = p11;
  p10 = p12;
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

  p9 = p8 - bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) - Point(162.0,0.0,0.0);
  p10 =  p8 + tcdir2 * trusscategory - Point(162.0,0.0,0.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

  p9 = p7 + bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(162.0,0.0,0.0);
  p10 =  p7 + tcdir1 * trusscategory + Point(162.0,0.0,0.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

  p9 = p11;
  p10 =  p7 + tcdir1 * trusscategory + Point(162.0,0.0,0.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

  p9 = p11;
  p10 =  p8 + tcdir2 * trusscategory - Point(162.0,0.0,0.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

  InsertNotch(NOTCH_LAYER,p11 + Point(0.0,230.0,0.0),&list);  
  if (topnotch > 350.0)
    InsertNotch(NOTCH_LAYER,p11 + Point(0.0,topnotch,0.0),&list);

  InsertNotch(NOTCH_LAYER,p7 + bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(162.0,230.0,0.0),&list);  
  if (topnotch > 350.0)
    InsertNotch(NOTCH_LAYER,p7 + bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(162.0,topnotch,0.0),&list);

  InsertNotch(NOTCH_LAYER,p8 - bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(-162.0,230.0,0.0),&list);  
  if (topnotch > 350.0)
    InsertNotch(NOTCH_LAYER,p8 - bcdir * (trusscategory > 1 ? trusscategory - 1 : 1.0) + Point(-162.0,topnotch,0.0),&list);

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));


}

void GenerateTruncatedTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,n,trusscategory;
 Point p1,p2,p3,p4a,p4b,p5a,p5b,p6,p7,p8,p9,p10,p11,p12,xa,ya,tcdir1,tcdir2,bcdir,dir,point[6];
 double bclength,topnotch,deltalength,pitch;
 EntityList list;
 Circle *cr;

  //  Points
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Eave end of the right hand top chord
  //    point(3)   Right hand top chord
  //    point(4)   Right hand top chord
  //    point(5)   Eave end of the left hand top chord
  point[0] = c->point(0);
  point[1] = c->point(1);
  point[2] = c->point(2);
  point[3] = c->point(3);
  point[4] = c->point(4);
  point[5] = c->point(5);
  
  bcdir = point[3] - point[2];  bcdir.z = 0.0;

  //  Adjust the length of the bottomn chord - to force the holes to be 35mm from the end.
  pitch = atan2(fabs(c->point(3).z - c->point(2).z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (c->point(1) - c->point(0)).length();
  tcdir1 = point[4] - point[0];
  tcdir2 = point[2] - point[1];
  point[0] -= Point(deltalength,0.0,0.0);
  point[1] += Point(deltalength,0.0,0.0);
  point[2] += Point(deltalength,0.0,0.0);
  point[3] += Point(deltalength,0.0,0.0);
  point[4] -= Point(deltalength,0.0,0.0);
  point[5] -= Point(deltalength,0.0,0.0);


  topnotch = (c->point(3).z - c->point(0).z) - 230.0;

  xa = c->point(1) - c->point(0);
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p7.setxyz(0.0,0.0,0.0);
  p1 = p7 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < c->npoints() ; i++)
    {  p8.setxyz(xa.dot(c->point(i) - c->point(0)),c->point(i).z - c->point(0).z,0.0);
       if (i == 1)
         p2 = p8 + Point(0.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3 = p8 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4a = p7 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       else if (i == 4)
         {  p4b = p8 - (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetB;
            p5a = p7 - (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetB;
         }   
       else if (i == 5)
         {  p5b = p8 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p6 = p7 + (p8 - p7).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p7 = p8;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p3,o+p4a);
  Line line3(o+p4b,o+p5a);
  Line line4(o+p5b,o+p6);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line4);
  Intersect i4(&line4,&line1);

  bclength = fabs(((PointE *)i2.intersection(0))->getp().x - ((PointE *)i1.intersection(0))->getp().x) * 2;
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
    }

  p7 = ((PointE *)i3.intersection(0))->getp();
  p8 = ((PointE *)i1.intersection(0))->getp();
  tcdir1 = (((PointE *)i3.intersection(0))->getp() - ((PointE *)i4.intersection(0))->getp()) / trusscategory;
  tcdir2 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i1.intersection(0))->getp()) / trusscategory;
  Line bcline(((PointE *)i4.intersection(0))->getp(),((PointE *)i1.intersection(0))->getp());
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i4.intersection(0))->getp()).normalize() * (bcline.nearp(((PointE *)i3.intersection(0))->getp()) - ((PointE *)i4.intersection(0))->getp()).length()  / (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);

  //  Now insert the members
  p7.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < c->npoints() ; i++)
    {  p8.setxyz(xa.dot(c->point(i) - c->point(0)),c->point(i).z - c->point(0).z,0.0);
       if (i == 1)
         {  InsertTrussMember(BOTTOMCHORD_LAYER,o+p7+Point(deltalength,0.0,0.0),o+p8-Point(deltalength,0.0,0.0),BottomChordWidth,&list);
            InsertCeilingClips(o+p7+Point(deltalength,0.0,0.0),o+p8-Point(deltalength,0.0,0.0),150.0,600.0,&list);
         }
       else if (i == 3)
         {  p9 = ((PointE *)i2.intersection(0))->getp() + (p8 - p7).normalize() * 35.0;
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD1_LAYER,p9,o+p7,BottomChordWidth,&list);
            InsertRoofClips(p9,o+p7,140.0,110.0,900.0,1200.0,&list);
         }
       else if (i == 4)
         {  p9 = ((PointE *)i2.intersection(0))->getp();
            p10 = ((PointE *)i3.intersection(0))->getp();
            p9 = p9 + (p9 - p10).normalize() * 35.0; 
            p10 = p10 + (p10 - p9).normalize() * 35.0; 
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            p10 = p10 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD3_LAYER,p10+Point(110.0,0.0,0.0),p9-Point(110.0,0.0,0.0),BottomChordWidth,&list);
         } 
       else if (i == 5)
         {  p9 = ((PointE *)i3.intersection(0))->getp() - (p8 - p7).normalize() * 35.0;
            p9 = p9 - (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * BCHoleOffsetT;
            InsertTrussMember(TOPCHORD2_LAYER,o+p8,p9,BottomChordWidth,&list);
            InsertRoofClips(o+p8,p9,140.0,110.0,900.0,1200.0,&list);
         } 
       p7 = p8;  
    }

  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp()-Point(110.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp()+Point(110.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i4.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  p11 = (((PointE *)i1.intersection(0))->getp() + ((PointE *)i4.intersection(0))->getp())/2.0;
  p12 = (((PointE *)i2.intersection(0))->getp() + ((PointE *)i3.intersection(0))->getp())/2.0;

  //  Insert the remaining webs
  p7 = ((PointE *)i4.intersection(0))->getp();
  p8 = ((PointE *)i1.intersection(0))->getp();
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p7 + tcdir1 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p8 + tcdir2 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory-1  ; i++)
    {  cr = new Circle(p7 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       cr = new Circle(p8 - bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  cr = new Circle(p7 + bcdir * (trusscategory > 1.0 ? trusscategory - 1 : 1.0) + Point(55.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(p8 - bcdir * (trusscategory > 1.0 ? trusscategory - 1 : 1.0) - Point(55.0,0.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);


  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p9 = p7 + tcdir1 * ((i+1)/2 + 1);
       p10 = p7 + bcdir * (i/2+1);
       if (i == trusscategory * 2 - 4)
         p10.x += 55.0;
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

       p9 = p8 + tcdir2 * ((i+1)/2 + 1);
       p10 = p8 - bcdir * (i/2+1);
       if (i == trusscategory * 2 - 4)
         p10.x -= 55.0;
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);
    } 

  p9 = p8 + tcdir2 * trusscategory;
  p10 = p8 - bcdir * (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir-Point(55.0,77.0,0.0),p10+dir-Point(55.0,0.0,0.0),WebWidth,&list);
     
  p9 = p8 + tcdir2 * trusscategory;
  cr = new Circle(p9-Point(55.0,77.0,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  p9 = p7 + tcdir1 * trusscategory;
  p10 = p7 + bcdir * (trusscategory > 1.0 ? trusscategory - 1.0 : 1.0);
  dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
  p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
  InsertTrussMember(WEB_LAYER,p9+dir+Point(55.0,-77.0,0.0),p10+dir+Point(55.0,0.0,0.0),WebWidth,&list);

  p9 = p7 + tcdir1 * trusscategory;
  cr = new Circle(p9+Point(55.0,-77,0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  p11 =  p7 + tcdir1 * trusscategory;
  p12 =  p8 + tcdir2 * trusscategory;
  n = int(((p12 - p11).length() + 1499.0) / 1500.0);
  p11 = (p12 - p11) / n;
       
  for (i = 0 ; i < n ; i++)
    {  p9 = p7 + bcdir * (trusscategory > 1.0 ? trusscategory - 1 : 1.0) + p11 * i;
       p10 =  p8 + tcdir2 * trusscategory - p11 * (n - i - 1);
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       if (i == 0)
         p9.x += 55.0;  
       if (i == n - 1)
         p10 += Point(-55.0,-77.0,0.0);

       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

       p9 = p8 - bcdir * (trusscategory > 1.0 ? trusscategory - 1 : 1.0) - p11 * (n - i - 1);
       p10 =  p7 + tcdir1 * trusscategory + p11 * i;
       dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
       p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
       if (i == 0)
         p10 += Point(55.0,-77.0,0.0);  
       if (i == n - 1)
         p9.x -= 55.0;  

       InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);

       if (i < n - 1)
         {  p9 = p7 + bcdir * (trusscategory > 1.0 ? trusscategory - 1 : 1.0) + p11 * (i+1);
            p10 = p7 + tcdir1 * trusscategory + p11 * (i+1);
            cr = new Circle(p9,10.0);
            cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
            db.geometry.add(cr);
            list.add(cr);
            cr = new Circle(p10,10.0);
            cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
            db.geometry.add(cr);
            list.add(cr);
            dir = (p10 - p9).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
            p9 = p9 - (p10 - p9).normalize() * WebHolePosition;
            p10 = p10 + (p10 - p9).normalize() * WebHolePosition;
            InsertTrussMember(WEB_LAYER,p9+dir,p10+dir,WebWidth,&list);
         }

    }

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));


}


void GenerateHipFullTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,trusscategory;
 Point p1,p2,p3,p4a,p4b,p5,p6,p7,p8,xa,ya,tcdir1,tcdir2,bcdir,dir,point[5];
 double bclength,topnotch,pitch,deltalength,scale;
 EntityList list;
 Circle *cr;

  //  Points
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Eave end of the right hand top chord
  //    point(3)   Apex point
  //    point(4)   Eave end of the left hand top chord

  point[0] = c->point(0);
  point[1] = c->point(1);
  point[2] = c->point(2);
  point[3] = c->point(3);
  point[4] = c->point(4);
  
  bcdir = point[3] - point[2];  bcdir.z = 0.0;

  //  Adjust the length of the bottom chord - to force the holes to be 35mm from the end.
  pitch = atan2(fabs(c->point(3).z - c->point(2).z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (c->point(1) - c->point(0)).length();
  scale = (bclength + 2.0 * deltalength) / bclength;
  tcdir1 = point[4] - point[0];
  tcdir2 = point[2] - point[1];
  point[0] *= scale;
  point[1] *= scale;
  point[3] *= scale;
  point[2] = point[1] + tcdir2;
  point[4] = point[0] + tcdir1;

  topnotch = (c->point(3).z - c->point(0).z) - 230.0;

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p6.setxyz(0.0,0.0,0.0);
  p1 = p6 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < c->npoints() ; i++)
    {  p7.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p7 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3 = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4a = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       else if (i == 4)
         {  p4b = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p5 = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p6 = p7;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p3,o+p4a);
  Line line3(o+p4b,o+p5);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line1);

  bclength = (p2 - p1).length();
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
       else
         trusscategory = 6;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
       else
         trusscategory = 5;
    }

  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  tcdir1 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / trusscategory;
  tcdir2 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i1.intersection(0))->getp()) / trusscategory;
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()) / (trusscategory <= 1 ? 1.0 : trusscategory - 1.0) / 2.0;

  //  Now insert the members
  p5.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < c->npoints() ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         {  InsertTrussMember(BOTTOMCHORD_LAYER,o+p5+Point(deltalength,0.0,0.0),o+p6-Point(deltalength,0.0,0.0),BottomChordWidth,&list);
            InsertCeilingClips(o+p5+Point(deltalength,0.0,0.0),o+p6-Point(deltalength,0.0,0.0),150.0,600.0,&list);
         }
       else if (i == 3)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) + (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD1_LAYER,o+p7,o+p5,BottomChordWidth,&list);
            InsertRoofClips(o+p7,o+p5,140.0,110.0,900.0,1200.0,&list);
         }
       else if (i == 4)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) - (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD2_LAYER,o+p6,o+p7,BottomChordWidth,&list);
            InsertRoofClips(o+p6,o+p7,140.0,110.0,900.0,1200.0,&list);
         } 
       p5 = p6;  
    }


  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  //  Insert the remaining webs
  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + tcdir1 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p6 + tcdir2 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    } 
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       cr = new Circle(p6 - bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  //  Insert the king web
  p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
  p8 = (((PointE *)i1.intersection(0))->getp() + ((PointE *)i3.intersection(0))->getp()) / 2.0;
  dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
  p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
  InsertTrussMember(WEB1_LAYER,p8-dir,p7-dir,WebWidth,&list);

  InsertNotch(NOTCH_LAYER,p8 + Point(0.0,230.0,0.0),&list);  
  if (topnotch > 350.0)
    InsertNotch(NOTCH_LAYER,p8 + Point(0.0,topnotch,0.0),&list);

  //  Insert the other web members
  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p7 = p5 + tcdir1 * ((i+1)/2 + 1);
       if (i == trusscategory * 2 - 3)
         p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
       p8 = p5 + bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);

       p7 = p6 + tcdir2 * ((i+1)/2 + 1);
       if (i == trusscategory * 2 - 3)
         p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
       p8 = p6 - bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);

    } 

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));


}

void GenerateDutchGableFullTrussMembers(RCCHAR *label,Point o,Curve *c)
{int i,trusscategory,matched,matchedmiddle;
 Point p1,p2,p3,p4a,p4b,p5,p6,p7,p8,xa,ya,tcdir1,tcdir2,bcdir,dir,point[5];
 double l,lstart,lend,bclength,topnotch,pitch,deltalength,scale;
 SortedDoubleList sdl;
 Entity *e;
 EntityList list;
 Circle *cr;

  //  Points
  //    point(0)   Start bottom chord
  //    point(1)   End bottom chord
  //    point(2)   Eave end of the right hand top chord
  //    point(3)   Apex point
  //    point(4)   Eave end of the left hand top chord

  point[0] = c->point(0);
  point[1] = c->point(1);
  point[2] = c->point(2);
  point[3] = c->point(3);
  point[4] = c->point(4);
  
  bcdir = point[3] - point[2];  bcdir.z = 0.0;

  //  Adjust the length of the bottomn chord - to force the holes to be 35mm from the end.
  pitch = atan2(fabs(c->point(3).z - c->point(2).z),bcdir.length());
  deltalength = (BCHoleOffsetB / fabs(tan(pitch)) - BCHoleOffsetT / fabs(sin(pitch))) - 35.0;
  bclength = (c->point(1) - c->point(0)).length();
  scale = (bclength + 2.0 * deltalength) / bclength;
  tcdir1 = point[4] - point[0];
  tcdir2 = point[2] - point[1];
  p1 = (point[0] + point[1]) / 2.0;
  point[0] = (point[0] - p1) * scale + p1;
  point[1] = (point[1] - p1) * scale + p1;
  point[3] = (point[3] - p1) * scale + p1;
  point[2] = point[1] + tcdir2;
  point[4] = point[0] + tcdir1;

  Line bcline(point[0],point[1]);
  sdl.add(bcline.length() / 2.0);
  matchedmiddle = 0;
  //  First make a list of dutch gable half trusses that touch this truss.
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(curve_entity) && e->getlayer() == DUTCHGABLEHALFTRUSS_LAYER)
      if ((bcline.nearp(((Curve *)e)->point(1)) - ((Curve *)e)->point(1)).length() < 2.0)
        {  l = (((Curve *)e)->point(1) - point[0]).length();
           if (l > bcline.length() - WebWidth/2.0)
             l = bcline.length() - WebWidth / 2.0;

           if (fabs(bcline.length() / 2.0 - l) < 2.0)
             matchedmiddle = 1;

           sdl.start();
           matched = 0;
           while (! matched && ! sdl.atend())
             matched = fabs(l - sdl.next()) < 2.0;
           if (! matched)
             {  sdl.add(l);
//p6.setxyz(l,0.0,0.0);
//db.geometry.add(new PointE(p6 + o));
             }
        }

  xa = point[1] - point[0];
  ya = xa.cross(Point(0.0,0.0,1.0));
  xa.normalize();  ya.normalize();
  p6.setxyz(0.0,0.0,0.0);
  p1 = p6 + Point(0.0,BCHoleOffsetB,0.0);
  //  First calculate the bolt hole lines.
  for (i = 1 ; i < c->npoints() ; i++)
    {  p7.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         p2 = p7 + Point(-WebWidth/2.0,BCHoleOffsetB,0.0);
       else if (i == 3)
         {  p3 = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p4a = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       else if (i == 4)
         {  p4b = p6 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
            p5 = p7 + (p7 - p6).normalize().cross(Point(0.0,0.0,1.0)) * BCHoleOffsetT;
         }   
       p6 = p7;  
    }

  Line line1(o+p1,o+p2);
  Line line2(o+p3,o+p4a);
  Line line3(o+p4b,o+p5);

  Intersect i1(&line1,&line2);
  Intersect i2(&line2,&line3);
  Intersect i3(&line3,&line1);

  bclength = (p2 - p1).length();
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
    }

  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  tcdir1 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).normalize();
  tcdir1 /= fabs(tcdir1.x);
  tcdir2 = (((PointE *)i2.intersection(0))->getp() - ((PointE *)i1.intersection(0))->getp()).normalize();
  tcdir2 /= fabs(tcdir2.x);
  bcdir = (((PointE *)i1.intersection(0))->getp() - ((PointE *)i3.intersection(0))->getp()).normalize();

  //  Now insert the members
  p5.setxyz(0.0,0.0,0.0);
  for (i = 1 ; i < c->npoints() ; i++)
    {  p6.setxyz(xa.dot(point[i] - point[0]),point[i].z - point[0].z,0.0);
       if (i == 1)
         {  InsertTrussMember(BOTTOMCHORD_LAYER,o+p5+Point(deltalength,0.0,0.0),o+p6-Point(deltalength,0.0,0.0),BottomChordWidth,&list);
            InsertCeilingClips(o+p5+Point(deltalength,0.0,0.0),o+p6-Point(deltalength,0.0,0.0),150.0,600.0,&list);
         }
       else if (i == 3)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) + (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD1_LAYER,o+p7,o+p5,BottomChordWidth,&list);
            InsertRoofClips(o+p7,o+p5,140.0,110.0,900.0,1200.0,&list);
         }
       else if (i == 4)
         {  p7 = Line(p5,p6).nearp(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0)-o) - (p6 - p5).normalize() * 40.0;
            InsertTrussMember(TOPCHORD2_LAYER,o+p6,o+p7,BottomChordWidth,&list);
            InsertRoofClips(o+p6,o+p7,140.0,110.0,900.0,1200.0,&list);
         } 
       p5 = p6;  
    }


  cr = new Circle(((PointE *)i1.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i2.intersection(0))->getp() + Point(-ApexXOffset,-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);
  cr = new Circle(((PointE *)i3.intersection(0))->getp(),10.0);
  cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
  db.geometry.add(cr);
  list.add(cr);

  //  Insert the king web
  p7 = ((PointE *)i2.intersection(0))->getp() + Point(0.0,-ApexYOffset-ApexXOffset*fabs(tcdir1.y / tcdir1.x),0.0);
  p8 = (((PointE *)i1.intersection(0))->getp() + ((PointE *)i3.intersection(0))->getp()) / 2.0;
  dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
  p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
  InsertTrussMember(WEB1_LAYER,p8-dir,p7-dir,WebWidth,&list);

  p5 = ((PointE *)i3.intersection(0))->getp();
  p6 = ((PointE *)i1.intersection(0))->getp();
  for (sdl.start() ; ! sdl.atend() ; )
    {  l = sdl.next() - (p5.x - o.x);
       cr = new Circle(p5 + bcdir * l,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
       if (l < (p6 - p5).length() / 2.0 - 50.0)
         {  cr = new Circle(p5 + tcdir1 * l,10.0);
            cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
            db.geometry.add(cr);
            list.add(cr);
         }
       else if (l > (p6 - p5).length() / 2.0 + 50.0)
         {  cr = new Circle(p6 + tcdir2 * ((p6 - p5).length() - l),10.0);
            cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
            db.geometry.add(cr);
            list.add(cr);
         }
    }

  sdl.start();
  if (! sdl.atend())
    {  l = sdl.next() - (p5.x - o.x);
       topnotch = tcdir1.y * (l + WebWidth / 2.0) - 230.0;
       lstart = l;
    } 
       
  while (! sdl.atend())
    {  if (l < (p6 - p5).length() / 2.0 - 50.0)
         p7 = p5 + tcdir1 * l;
       else
         p7 = p6 + tcdir2 * ((p6 - p5).length() - l);
       p8 = p5 + bcdir * l;
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       if (l < (p6 - p5).length() / 2.0 - 50.0 || l > (p6 - p5).length() / 2.0 + 50.0)  //  Don't insert the king web here
         InsertTrussMember(WEB1_LAYER,p8-dir,p7-dir,WebWidth,&list);

       if (matchedmiddle || l < (p6 - p5).length() / 2.0 - 50.0 || l > (p6 - p5).length() / 2.0 + 50.0)
         {  InsertNotch(NOTCH_LAYER,p8 + Point(0.0,230.0,0.0),&list);  
            if (topnotch > 350.0)
              InsertNotch(NOTCH_LAYER,p8 + Point(0.0,topnotch,0.0),&list);
         }
       
       if (l < (p6 - p5).length() / 2.0 - 50.0)
         {  p7 = p5 + tcdir1 * l;
            l =  sdl.next() - (p5.x - o.x);
            p8 = p5 + bcdir * l;
         }
       else
         {  p8 = p5 + bcdir * l;
            l =  sdl.next() - (p5.x - o.x);
            p7 = p6 + tcdir2 * ((p6 - p5).length() - l);
         }
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p8-dir,p7-dir,WebWidth,&list);
    }

  //  Insert the last vertical member
  lend = l;
  if (l < (p6 - p5).length() / 2.0 - 50.0)
    p7 = p5 + tcdir1 * l;
  else
    p7 = p6 + tcdir2 * ((p6 - p5).length() - l);
  p8 = p5 + bcdir * l;
  dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
  p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
  p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
  InsertTrussMember(WEB1_LAYER,p8-dir,p7-dir,WebWidth,&list);
  InsertNotch(NOTCH_LAYER,p8 + Point(0.0,230.0,0.0),&list);  
  if (topnotch > 350.0)
    InsertNotch(NOTCH_LAYER,p8 + Point(0.0,topnotch,0.0),&list);


  //  Insert extra webs beyond the sides of the vertical members.
  //  Insert the remaining webs

  bclength = (p5 - p6).length() - (lend - lstart);
  if (0) //highwindspeed
    {  if (bclength < 6000.0)
         trusscategory = 3;
       else if (bclength < 9000.0)
         trusscategory = 4;
       else if (bclength < 12000.0)
         trusscategory = 5;
    }
  else
    {  if (bclength < 4500.0)
         trusscategory = 1;
       else if (bclength < 10500.0)
         trusscategory = 3;
       else if (bclength < 12000.0)
         trusscategory = 4;
       else 
         trusscategory = 5;
    }

  tcdir1 = tcdir1 * lstart / trusscategory;
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p5 + tcdir1 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  bcdir = bcdir * lstart / (trusscategory > 1 ? trusscategory - 1 : 1);
  for (i = 1 ; i < trusscategory-1  ; i++)
    {  cr = new Circle(p5 + bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p7 = p5 + tcdir1 * ((i+1)/2 + 1);
       p8 = p5 + bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);
    } 

  
  tcdir2 = tcdir2 * ((p5 - p6).length() - lend) / trusscategory;
  for (i = 1 ; i < trusscategory  ; i++)
    {  cr = new Circle(p6 + tcdir2 * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  bcdir = bcdir.normalize() * ((p5 - p6).length() - lend) / (trusscategory > 1 ? trusscategory - 1 : 1);
  for (i = 1 ; i < trusscategory-1  ; i++)
    {  cr = new Circle(p6 - bcdir * i,10.0);
       cr->EntityHeader::change(cr->getcolour(),BOLT_LAYER,cr->getstyle(),cr->getweight());
       db.geometry.add(cr);
       list.add(cr);
    }

  for (i = 0 ; i < trusscategory * 2 - 3 ; i++)
    {  p7 = p6 + tcdir2 * ((i+1)/2 + 1);
       p8 = p6 - bcdir * (i/2+1);
       dir = (p8 - p7).cross(Point(0.0,0.0,1.0)).normalize() * WebWidth / 2.0;
       p7 = p7 - (p8 - p7).normalize() * WebHolePosition;
       p8 = p8 + (p8 - p7).normalize() * WebHolePosition;
       InsertTrussMember(WEB_LAYER,p7+dir,p8+dir,WebWidth,&list);
    } 

  db.geometry.add(new Group(list,_RCT("Truss"),label,BitMask(32)));

}

void GenerateTrussMembers(Point o,Curve *c,RCCHAR *label)
{int i;
 Point p1,p2,xa,ya;
 EntityList list;

  if ((c->getlayer() == HIPHALFTRUSS_LAYER || c->getlayer() == HALFTRUSS_LAYER || c->getlayer() == DUTCHGABLEHALFTRUSS_LAYER) && (c->npoints() == 4 || c->npoints() == 5))
    GenerateHalfTrussMembers(label,o,c);
  else if (c->getlayer() == FULLTRUSS_LAYER && c->npoints() == 5)
    GenerateFullTrussMembers(label,o,c);
  else if (c->getlayer() == DUTCHGABLEFULLTRUSS_LAYER && c->npoints() == 5)
    GenerateDutchGableFullTrussMembers(label,o,c);
  else if (c->getlayer() == HIPFULLTRUSS_LAYER && c->npoints() == 5)
    GenerateHipFullTrussMembers(label,o,c);
  else if (c->getlayer() == HIPTRUSS_LAYER && (c->npoints() == 4 || c->npoints() == 5))
    GenerateHipTrussMembers(label,o,c);
  else if (c->getlayer() == VERTICALWEBTRUNCATEDTRUSS_LAYER && c->npoints() == 6)
    GenerateVerticalWebTruncatedTrussMembers(label,o,c);
  else if (c->getlayer() == TRUNCATEDTRUSS_LAYER && c->npoints() == 6)
    GenerateTruncatedTrussMembers(label,o,c);
  else
    {  xa = c->point(1) - c->point(0);
       ya = xa.cross(Point(0.0,0.0,1.0));
       xa.normalize();  ya.normalize();
       p1.setxyz(0.0,0.0,0.0);
       for (i = 1 ; i < c->npoints() ; i++)
         {  p2.setxyz(xa.dot(c->point(i) - c->point(0)),c->point(i).z - c->point(0).z,0.0);
            InsertTrussMember(WEB_LAYER,o+p1,o+p2,BottomChordWidth,&list);
            p1 = p2;  
         }
    }

}

void TallyTrussMembers(int *nbolts,double *weblength,double *chordlength,int *hipfullbrackets,int *halfhipbrackets,int *girderbrackets,int *doublegirderbrackets)
{Entity *e,*ge;
 EntityList list;
 double length;

  //  Count the number of bolts on the trusses.
  *nbolts = 0;
  *weblength = 0.0;
  *chordlength = 0.0;
  *hipfullbrackets = 0;
  *halfhipbrackets = 0;
  *girderbrackets = 0;
  *doublegirderbrackets = 0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strcmp(((Group *)e)->getinstance(),"Truss") != 0)
      {  list = ((Group *)e)->getlist();
         for (list.start() ; (ge = list.next()) != 0 ; )
           if (ge->getlayer() == BOLT_LAYER)
             (*nbolts)++;
           else if (ge->getlayer() == HIPFULLNOTCH_LAYER)
             (*hipfullbrackets)++;
           else if (ge->getlayer() == HALFHIPNOTCH_LAYER)
             (*halfhipbrackets)++;
 
      } 
    else if (e->isa(curve_entity) && ((Curve *)e)->getn() > 1)
      {  length = (((Curve *)e)->point(1) - ((Curve *)e)->point(0)).length();
         if (e->getlayer() == TOPCHORD1_LAYER || e->getlayer() == TOPCHORD2_LAYER || e->getlayer() == TOPCHORD3_LAYER && e->isa(curve_entity) ||
             e->getlayer() == BOTTOMCHORD_LAYER)
           (*chordlength) += length;
         if (e->getlayer() == WEB_LAYER || e->getlayer() == WEB1_LAYER || e->getlayer() == WEB2_LAYER || e->getlayer() == WEB3_LAYER || e->getlayer() == WEB4_LAYER)
           (*weblength) += length;

      }
    else if (e->isa(figure_entity))
      {Point p3;
       RCCHAR library[300];
        strcpy(library,((Figure *)e)->getlibrary());
        _strlwr(library);
        if (strstr(library,"dbgirderbracket") != 0)
          (*doublegirderbrackets)++;
        else if (strstr(library,"girderbracket") != 0)
          (*girderbrackets)++;
      }

  (*hipfullbrackets) /= 2;

}



void TallyWallFrameMembers(double *studlength,double *nogginglength,double *platelength,double *silllintellength)
{EntityList list;
 Entity *e,*ge;
 
  *studlength = *nogginglength = *platelength = *silllintellength = 0.0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strcmp(((Group *)e)->getinstance(),"Wall Frame") != 0)
      {  list = ((Group *)e)->getlist();
         for (list.start() ; (ge = list.next()) != 0 ; )
           if (ge->getlayer() == TOPPLATE_LAYER && ge->isa(curve_entity))
             (*platelength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
           else if (ge->getlayer() == BOTTOMPLATE_LAYER && ge->isa(curve_entity))
             (*platelength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
           else if (ge->getlayer() == STUD_LAYER && ge->isa(curve_entity))
             (*studlength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
           else if (ge->getlayer() == NOGGING_LAYER && ge->isa(curve_entity))
             (*nogginglength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
           else if (ge->getlayer() == SILL_LAYER && ge->isa(curve_entity))
             (*silllintellength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
           else if (ge->getlayer() == LINTEL_LAYER && ge->isa(curve_entity))
             (*silllintellength) += (((Curve *)ge)->point(0) - ((Curve *)ge)->point(1)).length();
      }

}

#else

void start_architect(void)
{
}

void architect_command0(int *,HighLevelEvent *,void **)
{
}

void architect_command2(int *,HighLevelEvent *,void **)
{
}

void architect_command24(int *,HighLevelEvent *,void **)
{
}

#endif

