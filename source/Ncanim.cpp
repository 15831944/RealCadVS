
#include "ncwin.h"

#if (MENUS != STUDENTMENUS)

AnimationMenuWindow::AnimationMenuWindow(Window *win,int x,int y,int w,int h,AnimationActive *aa) :
  MenuWindow(win,x,y,w,h)
{ animationactive = aa;
  buttonmenu = animationactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  animationactive->setactive(1);
}

AnimationMenuWindow::~AnimationMenuWindow()
{ animationactive->setactive(0);
}

AnimationActive animationactive;

extern void animation_command1(int *cstate,HighLevelEvent *event,void **);
extern void animation_command2(int *cstate,HighLevelEvent *event,void **);
extern void animation_command3(int *cstate,HighLevelEvent *event,void **);
extern void animation_command4(int *cstate,HighLevelEvent *event,void **);
extern void animation_command5(int *cstate,HighLevelEvent *event,void **);
extern void animation_command6(int *cstate,HighLevelEvent *event,void **);
extern void animation_command7(int *cstate,HighLevelEvent *event,void **);
extern void animation_command8(int *cstate,HighLevelEvent *event,void **);
extern void animation_command9(int *cstate,HighLevelEvent *event,void **);
extern void animation_command10(int *cstate,HighLevelEvent *event,void **);
extern void animation_command11(int *cstate,HighLevelEvent *event,void **);
extern void animation_command12(int *cstate,HighLevelEvent *event,void **);

//extern int check_license(char *label,char *label1,char *label2,int application,int showmessages);
extern int check_license(RCCHAR *label,RCCHAR *label1,RCCHAR *label2,int application,int showmessages);

void animation_command(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
#if MENUS != FLEXICADMENUS
  check_license(_RCT("Animation Application"),_RCT("RealCADKey5"),_RCT("RealCADKey6"),2,1);
#endif
  if (animationactive.getactive()) return;
  if (animationactive.getbuttonmenu() == NULL)
    {ButtonMenu *menu = buttonmenus.match("Animation");
       if (menu != NULL)
         animationactive.setbuttonmenu(menu);
    }
  if (animationactive.getbuttonmenu() == NULL)
    { RCMessageBox(cadwindow->gethwnd(),_RCT("Failed to create the animation button menu"),_RCT("Animation"),MB_ICONSTOP);
       return;
    }
  new AnimationMenuWindow(cadwindow->getsubwindow(),
                          int(cadwindow->getsubwindow()->getwidth())-120,0,
                          48,72,&animationactive);
  *cstate = ExitState;
}

struct RBAnimation1_data
  {public:
     Point start,end;
     int fixref;
     double starttime,finishtime,startfocaldistance,finishfocaldistance,startheight,finishheight;
     EntityList list;
     RCCHAR scene[256];
  };

void RBAnimation1(int,void *data,Point *p2,View3dSurface *)
{RBAnimation1_data *rbdata = (RBAnimation1_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

void animation_command1(int *cstate,HighLevelEvent *event,void **data)
{RBAnimation1_data *rbdata = (RBAnimation1_data *) *data;
 BitMask options(32);
 Point p,ref,p1;
 EntityList pathlist;
 int linear,point;
 Entity *e=0;
 RCCHAR info[256];
 DialogControl *dc=0;
 Loops loops(0);
  switch (*cstate)
    {case InitialState:
       rbdata = new RBAnimation1_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,_RCT("Animation camera path command"));
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"AN::camera:",11) == 0)
              {BitMask change(32),options(32);
               double starttime,finishtime,startfocaldistance,finishfocaldistance,startheight,finishheight;
               int fixref,version;
               RCCHAR scene[256],info[256];
                 startfocaldistance = finishfocaldistance = startheight = finishheight = 0.0;
                 sscanf(((Group *)e)->getinstance()+11,"%d %lf %lf %d %lf %lf %lf %lf",&version,
                             &starttime,&finishtime,&fixref,&startfocaldistance,&finishfocaldistance,&startheight,&finishheight);
                 if (finishfocaldistance == 0.0)
                   finishfocaldistance = startfocaldistance;
                 strcpy(scene,((Group *)e)->getclassname());
                 Dialog dialog(_RCT("Animationcamera_Dialog"));
                 dialog.title(_RCT("Change exisiting camera path"));
                 dialog.add(new StringDialogControl(100,scene,256));
                 dialog.add(dc = new RealDialogControl(101,&starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(new CheckBoxDialogControl(103,&fixref));
                 dialog.add(dc = new RealDialogControl(104,&startfocaldistance,0.1,0.000001,100000.0,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&finishfocaldistance,0.1,0.000001,100000.0,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 if (dialog.process() == TRUE)
                   {  
                      sprintf(info,"AN::camera:0 %.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf",starttime,finishtime,fixref,startfocaldistance,finishfocaldistance,startheight,finishheight);
                      change.set(4);  change.set(5);
                      ((Group *)e)->change(change,*e,scene,info,options);
                   }
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *) e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a single camera position or linear entities only"),_RCT("Animation camera command"),MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a camera path or the entities that define the camera path first"),_RCT("Animation camera command"),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a single connected path to follow"),_RCT("Animation camera command"),MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = p;
                      rbdata->end = p;
                   }
                 strcpy(rbdata->scene,v.getstring("an::scene"));
                 rbdata->starttime = v.getreal("an::starttime");
                 rbdata->finishtime = v.getreal("an::finishtime");
                 rbdata->fixref = v.getinteger("an::fixref");
                 rbdata->startfocaldistance = v.getreal("an::startfocaldistance");
                 rbdata->finishfocaldistance = v.getreal("an::startfocaldistance");
                 rbdata->startheight = v.getreal("an::startheight");
                 rbdata->finishheight = v.getreal("an::finishheight");
                 Dialog dialog(_RCT("Animationcamera_dialog"));
                 dialog.add(new StringDialogControl(100,rbdata->scene,256));
                 dialog.add(dc = new RealDialogControl(101,&rbdata->starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&rbdata->finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(new CheckBoxDialogControl(103,&rbdata->fixref));
                 dialog.add(dc = new RealDialogControl(104,&rbdata->startfocaldistance,0.1,0.000001,100000.0,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&rbdata->finishfocaldistance,0.1,0.000001,100000.0,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&rbdata->startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&rbdata->finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 if (dialog.process() == TRUE)
                   {  v.setstring("an::scene",rbdata->scene);
                      v.setreal("an::starttime",rbdata->starttime);
                      v.setreal("an::finishtime",rbdata->finishtime);
                      v.setinteger("an::fixref",rbdata->fixref);
                      v.setreal("an::startfocaldistance",rbdata->startfocaldistance);
                      v.setreal("an::finishfocaldistance",rbdata->finishfocaldistance);
                      v.setreal("an::startheight",rbdata->startheight);
                      v.setreal("an::finishheight",rbdata->finishheight);
                      if (point)
                        {  rbdata->fixref = 1;
                           *cstate = 2;
                        }
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            if (! rbdata->fixref)
              {  rbdata->list = rbdata->list;
                 rbdata->list.add(new PointE(rbdata->start));
                 ref.setxyz(0.0,0.0,0.0);
                 rbdata->list.add(new PointE(ref));
                 sprintf(info,"AN::camera:0 %.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf",rbdata->starttime,rbdata->finishtime,rbdata->fixref,rbdata->startfocaldistance,rbdata->finishfocaldistance,rbdata->startheight,rbdata->finishheight);
                 Group *group = new Group(rbdata->list,rbdata->scene,info,options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   delete group;
                 *cstate = ExitState;
              }
            else
              *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       if (event->isa(Coordinate3d))
         {  ref = ((Coordinate3dEvent *) event)->getp();
            rbdata->list.add(new PointE(rbdata->start));
            rbdata->list.add(new PointE(ref));
            sprintf(info,"AN::camera:0 %.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf",rbdata->starttime,rbdata->finishtime,rbdata->fixref,rbdata->startfocaldistance,rbdata->finishfocaldistance,rbdata->startheight,rbdata->finishheight);
            Group *group = new Group(rbdata->list,rbdata->scene,info,options);
            if (group != NULL && group->getdefined())
              db.geometry.add(group);
            else
              delete group;
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate near the start of the camera path"));
       rubberband.begin(0);
       rubberband.add(RBAnimation1,rbdata);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate the view reference point"));
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBAnimation2_data
  {public:
     Point start,end;
     double starttime,finishtime,startheight,finishheight;
     EntityList list;
     RCCHAR scene[256];
  };

void RBAnimation2(int,void *data,Point *p2,View3dSurface *)
{RBAnimation2_data *rbdata = (RBAnimation2_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

void animation_command2(int *cstate,HighLevelEvent *event,void **data)
{RBAnimation2_data *rbdata = (RBAnimation2_data *) *data;
 EntityList pathlist;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1;
 int point,linear;
 Entity *e;
 RCCHAR info[256];
 DialogControl *dc;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBAnimation2_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,_RCT("Animation camera reference path command"));
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"AN::cameraref:",14) == 0)
              {BitMask change(32),options(32);
               double starttime,finishtime,startheight,finishheight;
               int version;
               RCCHAR scene[256],info[256];
                 startheight = finishheight = 0.0;
                 sscanf(((Group *)e)->getinstance()+14,"%d %lf %lf %lf %lf",
                             &version,&starttime,&finishtime,&startheight,&finishheight);
                 strcpy(scene,((Group *)e)->getclassname());
                 Dialog dialog(_RCT("Animationreference_Dialog"));
                 dialog.title(_RCT("Change exisiting camera reference path"));
                 dialog.add(new StringDialogControl(100,scene,256));
                 dialog.add(dc = new RealDialogControl(101,&starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(103,&startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 if (dialog.process() == TRUE)
                   {  sprintf(info,"AN::cameraref:0 %.4lf %.4lf %.4lf %.4lf",starttime,finishtime,startheight,finishheight);
                      change.set(4);  change.set(5);
                      ((Group *)e)->change(change,*e,scene,info,options);
                   }
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *) e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a camera reference path or linear entities only"),_RCT("Animation camera reference command"),MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a camera reference path or the entities that define the camera path first"),_RCT("Animation camera reference command"),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a single connected path to follow"),_RCT("Animation camera reference command"),MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = p;
                      rbdata->end = p;
                   }
                 strcpy(rbdata->scene,v.getstring("an::scene"));
                 rbdata->starttime = v.getreal("an::starttime");
                 rbdata->finishtime = v.getreal("an::finishtime");
                 rbdata->startheight = v.getreal("an::startheight");
                 rbdata->finishheight = v.getreal("an::finishheight");
                 Dialog dialog(_RCT("Animationreference_Dialog"));
                 dialog.add(new StringDialogControl(100,rbdata->scene,256));
                 dialog.add(dc = new RealDialogControl(101,&rbdata->starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&rbdata->finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(103,&rbdata->startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&rbdata->finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 if (dialog.process() == TRUE)
                   {  v.setstring("an::scene",rbdata->scene);
                      v.setreal("an::starttime",rbdata->starttime);
                      v.setreal("an::finishtime",rbdata->finishtime);
                      v.setreal("an::startheight",rbdata->startheight);
                      v.setreal("an::finishheight",rbdata->finishheight);
                      if (point)
                        {  sprintf(info,"AN::cameraref:0 %.4lf %.4lf %.4lf %.4lf",rbdata->starttime,rbdata->finishtime,rbdata->startheight,rbdata->finishheight);
                           Group *group = new Group(rbdata->list,rbdata->scene,info,options);
                           if (group != NULL && group->getdefined())
                             db.geometry.add(group);
                           else
                             delete group;
                           *cstate = ExitState;
                        }
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            sprintf(info,"AN::cameraref:0 %.4lf %.4lf %.4lf %.4lf",rbdata->starttime,rbdata->finishtime,rbdata->startheight,rbdata->finishheight);
            Group *group = new Group(rbdata->list,rbdata->scene,info,options);
            if (group != NULL && group->getdefined())
              db.geometry.add(group);
            else
              delete group;
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(_RCT("Locate near the start of the camera reference path"));
       rubberband.begin(0);
       rubberband.add(RBAnimation2,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class RBAnimation3_data
  {public:
     Point start,end,scale1,rotation1,scale2,rotation2;
     double starttime,finishtime,startheight,finishheight;
     int followpath;
     EntityList list,grouplist;
     RCCHAR scene[256];
  };

void RBAnimation3(int,void *data,Point *p2,View3dSurface *)
{RBAnimation3_data *rbdata = (RBAnimation3_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

void animation_command3(int *cstate,HighLevelEvent *event,void **data)
{RBAnimation3_data *rbdata = (RBAnimation3_data *) *data;
 BitMask options(32);
 Loops loops(0);
 EntityList pathlist;
 PointE *origin;
 Point p,p1;
 int linear,point;
 Entity *e;
 DialogControl *dc;
 RCCHAR info[256];
  switch (*cstate)
    {case InitialState:
       rbdata = new RBAnimation3_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,_RCT("Animation object path command"));
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"AN::objectpath:",15) == 0)
              {BitMask change(32),options(32);
               Point scale1,rotation1,scale2,rotation2;
               double starttime,finishtime,startheight,finishheight;
               int followpath;
               RCCHAR scene[256],info[256];
               int version;
                 startheight = finishheight = 0.0;  followpath = 0;
                 sscanf(((Group *)e)->getinstance()+15,"%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",&version,
                           &starttime,&finishtime,
                           &scale1.x,&scale1.y,&scale1.z,&scale2.x,&scale2.y,&scale2.z,
                           &rotation1.x,&rotation1.y,&rotation1.z,&rotation2.x,&rotation2.y,&rotation2.z,&startheight,&finishheight,&followpath);
                 strcpy(scene,((Group *)e)->getclassname());
                 Dialog dialog(_RCT("Animationobject_Dialog"));
                 dialog.title(_RCT("Changing existing object path"));
                 dialog.add(new StringDialogControl(100,scene,256));
                 dialog.add(dc = new RealDialogControl(101,&starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(103,&rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&rotation2.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&rotation1.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&rotation2.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 dialog.add(dc = new RealDialogControl(108,&rotation2.z,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1108,dc));
                 dialog.add(dc = new RealDialogControl(109,&scale1.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1109,dc));
                 dialog.add(dc = new RealDialogControl(110,&scale2.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1110,dc));
                 dialog.add(dc = new RealDialogControl(111,&scale1.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1111,dc));
                 dialog.add(dc = new RealDialogControl(112,&scale2.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1112,dc));
                 dialog.add(dc = new RealDialogControl(113,&scale1.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1113,dc));
                 dialog.add(dc = new RealDialogControl(114,&scale2.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1114,dc));
                 dialog.add(dc = new RealDialogControl(115,&startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1115,dc));
                 dialog.add(dc = new RealDialogControl(116,&finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1116,dc));
                 dialog.add(new CheckBoxDialogControl(117,&rbdata->followpath));
                 if (dialog.process() == TRUE)
                   {  sprintf(info,"AN::objectpath:0 %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %d",
                         starttime,finishtime,
                         scale1.x,scale1.y,scale1.z,scale2.x,scale2.y,scale2.z,
                         rotation1.x,rotation1.y,rotation1.z,rotation2.x,rotation2.y,rotation2.z,
                         startheight,finishheight,followpath);
                      change.set(4);  change.set(5);
                      ((Group *)e)->change(change,*e,scene,info,options);
                   }
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *)e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select an object path or linear entities only"),_RCT("Animation object path command"),MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select an object path or the entities that define the object path first"),_RCT("Animation object path command"),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a single connected path to follow"),_RCT("Animation object path command"),MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = rbdata->end = p;
                   }
                 strcpy(rbdata->scene,v.getstring("an::scene"));
                 rbdata->starttime = v.getreal("an::starttime");
                 rbdata->finishtime = v.getreal("an::finishtime");
                 rbdata->scale1.setxyz(v.getreal("an::scalex1"),v.getreal("an::scaley1"),v.getreal("an::scalez1"));
                 rbdata->rotation1.setxyz(v.getreal("an::rotationx1"),v.getreal("an::rotationy1"),v.getreal("an::rotationz1"));
                 rbdata->scale2.setxyz(v.getreal("an::scalex2"),v.getreal("an::scaley2"),v.getreal("an::scalez2"));
                 rbdata->rotation2.setxyz(v.getreal("an::rotationx2"),v.getreal("an::rotationy2"),v.getreal("an::rotationz2"));
                 rbdata->startheight = v.getreal("an::startheight");
                 rbdata->finishheight = v.getreal("an::finishheight");
                 rbdata->followpath = v.getinteger("an::followpath");
                 Dialog dialog(_RCT("Animationobject_Dialog"));
                 dialog.add(new StringDialogControl(100,rbdata->scene,256));
                 dialog.add(dc = new RealDialogControl(101,&rbdata->starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&rbdata->finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(103,&rbdata->rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&rbdata->rotation2.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&rbdata->rotation1.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&rbdata->rotation2.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&rbdata->rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 dialog.add(dc = new RealDialogControl(108,&rbdata->rotation2.z,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1108,dc));
                 dialog.add(dc = new RealDialogControl(109,&rbdata->scale1.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1109,dc));
                 dialog.add(dc = new RealDialogControl(110,&rbdata->scale2.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1110,dc));
                 dialog.add(dc = new RealDialogControl(111,&rbdata->scale1.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1111,dc));
                 dialog.add(dc = new RealDialogControl(112,&rbdata->scale2.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1112,dc));
                 dialog.add(dc = new RealDialogControl(113,&rbdata->scale1.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1113,dc));
                 dialog.add(dc = new RealDialogControl(114,&rbdata->scale2.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1114,dc));
                 dialog.add(dc = new RealDialogControl(115,&rbdata->startheight));
                 dialog.add(new ScrollBarDialogControl(1115,dc));
                 dialog.add(dc = new RealDialogControl(116,&rbdata->finishheight));
                 dialog.add(new ScrollBarDialogControl(1116,dc));
                 dialog.add(new CheckBoxDialogControl(117,&rbdata->followpath));
                 if (dialog.process() == TRUE)
                   {  v.setstring("an::scene",rbdata->scene);
                      v.setreal("an::starttime",rbdata->starttime);
                      v.setreal("an::finishtime",rbdata->finishtime);
                      v.setreal("an::scalex1",rbdata->scale1.x);
                      v.setreal("an::scaley1",rbdata->scale1.y);
                      v.setreal("an::scalez1",rbdata->scale1.z);
                      v.setreal("an::scalex2",rbdata->scale2.x);
                      v.setreal("an::scaley2",rbdata->scale2.y);
                      v.setreal("an::scalez2",rbdata->scale2.z);
                      v.setreal("an::rotationx1",rbdata->rotation1.x);
                      v.setreal("an::rotationy1",rbdata->rotation1.y);
                      v.setreal("an::rotationz1",rbdata->rotation1.z);
                      v.setreal("an::rotationx2",rbdata->rotation2.x);
                      v.setreal("an::rotationy2",rbdata->rotation2.y);
                      v.setreal("an::rotationz2",rbdata->rotation2.z);
                      v.setreal("an::startheight",rbdata->startheight);
                      v.setreal("an::finishheight",rbdata->finishheight);
                      v.setinteger("an::followpath",rbdata->followpath);
                      if (point)
                        *cstate = 2;
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       state.destroyselection(0);
       if (event->isa(EntitySelected))
         {  rbdata->list.add(((EntitySelectedEvent *)event)->getentity());
            rbdata->grouplist.add(((EntitySelectedEvent *)event)->getentity());
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = 3;
       break;
     case 3 :
       if (event->isa(Coordinate3d))
         {  if ((origin = new PointE(((Coordinate3dEvent *)event)->getp())) != NULL)
              {  rbdata->list.add(origin);
                 sprintf(info,"AN::objectpath:0 %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %d",
                              rbdata->starttime,rbdata->finishtime,
                              rbdata->scale1.x,rbdata->scale1.y,rbdata->scale1.z,
                              rbdata->scale2.x,rbdata->scale2.y,rbdata->scale2.z,
                              rbdata->rotation1.x,rbdata->rotation1.y,rbdata->rotation1.z,
                              rbdata->rotation2.x,rbdata->rotation2.y,rbdata->rotation2.z,
                              rbdata->startheight,rbdata->finishheight,rbdata->followpath);
                 Group *group = new Group(rbdata->list,rbdata->scene,info,options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   delete group;
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the camera path");
       rubberband.begin(0);
       rubberband.add(RBAnimation3,rbdata);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(group_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the groups that define the object and sub-objects - End selections using ABORT");
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the object");
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) rbdata->grouplist.unhighlight();
       delete rbdata;
    }
}

#if VERSION != FLEXICAD
class RBAnimation12_data
  {public:
     Point start,end,scale1,rotation1,scale2,rotation2;
     double starttime,finishtime,startheight,finishheight,loops;
     int reverse,followpath;
     EntityList list,grouplist;
     RCCHAR scene[256];
  };

void RBAnimation12(int,void *data,Point *p2,View3dSurface *)
{RBAnimation3_data *rbdata = (RBAnimation3_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

void animation_command12(int *cstate,HighLevelEvent *event,void **data)
{RBAnimation12_data *rbdata = (RBAnimation12_data *) *data;
 BitMask options(32);
 Loops loops(0);
 EntityList pathlist;
 PointE *origin;
 Point p,p1;
 int linear,point;
 Entity *e;
 DialogControl *dc;
 RCCHAR info[256];
  switch (*cstate)
    {case InitialState:
       rbdata = new RBAnimation12_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,_RCT("Animation patch path command"));
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"AN::patchpath:",14) == 0)
              {BitMask change(32),options(32);
               Point scale1,rotation1,scale2,rotation2;
               double starttime,finishtime,startheight,finishheight,loops;
               RCCHAR scene[256],info[256];
               int version,reverse,followpath;
                 startheight = finishheight = 0.0;  followpath = 0;
                 sscanf(((Group *)e)->getinstance()+14,"%d %lf %lf %lf %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",&version,
                           &starttime,&finishtime,&loops,&reverse,
                           &scale1.x,&scale1.y,&scale1.z,&scale2.x,&scale2.y,&scale2.z,
                           &rotation1.x,&rotation1.y,&rotation1.z,&rotation2.x,&rotation2.y,&rotation2.z,
                           &startheight,&finishheight,&followpath);
                 strcpy(scene,((Group *)e)->getclassname());
                 Dialog dialog("Animationpatch_Dialog");
                 dialog.title("Changing existing patch path");
                 dialog.add(new StringDialogControl(100,scene,256));
                 dialog.add(dc = new RealDialogControl(101,&starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(118,&loops,1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1118,dc));
                 dialog.add(new CheckBoxDialogControl(119,&reverse));
                 dialog.add(dc = new RealDialogControl(103,&rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&rotation2.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&rotation1.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&rotation2.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 dialog.add(dc = new RealDialogControl(108,&rotation2.z,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1108,dc));
                 dialog.add(dc = new RealDialogControl(109,&scale1.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1109,dc));
                 dialog.add(dc = new RealDialogControl(110,&scale2.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1110,dc));
                 dialog.add(dc = new RealDialogControl(111,&scale1.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1111,dc));
                 dialog.add(dc = new RealDialogControl(112,&scale2.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1112,dc));
                 dialog.add(dc = new RealDialogControl(113,&scale1.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1113,dc));
                 dialog.add(dc = new RealDialogControl(114,&scale2.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1114,dc));
                 dialog.add(dc = new RealDialogControl(115,&startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1115,dc));
                 dialog.add(dc = new RealDialogControl(116,&finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1116,dc));
                 dialog.add(new CheckBoxDialogControl(117,&followpath));
                 if (dialog.process() == TRUE)
                   {  sprintf(info,"AN::patchpath:0 %.4lf %.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %d",
                         starttime,finishtime,loops,reverse,
                         scale1.x,scale1.y,scale1.z,scale2.x,scale2.y,scale2.z,
                         rotation1.x,rotation1.y,rotation1.z,rotation2.x,rotation2.y,rotation2.z,
                         startheight,finishheight,followpath);
                      change.set(4);  change.set(5);
                      ((Group *)e)->change(change,*e,scene,info,options);
                   }
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *)e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a patch path or linear entities only"),_RCT("Animation object path command"),MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a patch path or the entities that define the patch path first"),_RCT("Animation patch path command"),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a single connected path to follow"),_RCT("Animation patch path command"),MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = rbdata->end = p;
                   }
                 strcpy(rbdata->scene,v.getstring("an::scene"));
                 rbdata->starttime = v.getreal("an::starttime");
                 rbdata->finishtime = v.getreal("an::finishtime");
                 rbdata->loops = v.getreal("an::loops");
                 rbdata->reverse = v.getinteger("an::reverse");
                 rbdata->scale1.setxyz(v.getreal("an::scalex1"),v.getreal("an::scaley1"),v.getreal("an::scalez1"));
                 rbdata->rotation1.setxyz(v.getreal("an::rotationx1"),v.getreal("an::rotationy1"),v.getreal("an::rotationz1"));
                 rbdata->scale2.setxyz(v.getreal("an::scalex2"),v.getreal("an::scaley2"),v.getreal("an::scalez2"));
                 rbdata->rotation2.setxyz(v.getreal("an::rotationx2"),v.getreal("an::rotationy2"),v.getreal("an::rotationz2"));
                 rbdata->startheight = v.getreal("an::startheight");
                 rbdata->finishheight = v.getreal("an::finishheight");
                 rbdata->followpath = v.getinteger("an::followpath");
                 Dialog dialog("Animationpatch_Dialog");
                 dialog.add(new StringDialogControl(100,rbdata->scene,256));
                 dialog.add(dc = new RealDialogControl(101,&rbdata->starttime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1101,dc));
                 dialog.add(dc = new RealDialogControl(102,&rbdata->finishtime,0.1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1102,dc));
                 dialog.add(dc = new RealDialogControl(118,&rbdata->loops,1,0.0,100000.0));
                 dialog.add(new ScrollBarDialogControl(1118,dc));
                 dialog.add(new CheckBoxDialogControl(119,&rbdata->reverse));
                 dialog.add(dc = new RealDialogControl(103,&rbdata->rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1103,dc));
                 dialog.add(dc = new RealDialogControl(104,&rbdata->rotation2.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1104,dc));
                 dialog.add(dc = new RealDialogControl(105,&rbdata->rotation1.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1105,dc));
                 dialog.add(dc = new RealDialogControl(106,&rbdata->rotation2.y,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1106,dc));
                 dialog.add(dc = new RealDialogControl(107,&rbdata->rotation1.x,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1107,dc));
                 dialog.add(dc = new RealDialogControl(108,&rbdata->rotation2.z,1.0,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1108,dc));
                 dialog.add(dc = new RealDialogControl(109,&rbdata->scale1.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1109,dc));
                 dialog.add(dc = new RealDialogControl(110,&rbdata->scale2.x,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1110,dc));
                 dialog.add(dc = new RealDialogControl(111,&rbdata->scale1.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1111,dc));
                 dialog.add(dc = new RealDialogControl(112,&rbdata->scale2.y,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1112,dc));
                 dialog.add(dc = new RealDialogControl(113,&rbdata->scale1.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1113,dc));
                 dialog.add(dc = new RealDialogControl(114,&rbdata->scale2.z,0.1,0.0,0.0));
                 dialog.add(new ScrollBarDialogControl(1114,dc));
                 dialog.add(dc = new RealDialogControl(115,&rbdata->startheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1115,dc));
                 dialog.add(dc = new RealDialogControl(116,&rbdata->finishheight,ScaleByLength));
                 dialog.add(new ScrollBarDialogControl(1116,dc));
                 dialog.add(new CheckBoxDialogControl(117,&rbdata->followpath));
                 if (dialog.process() == TRUE)
                   {  v.setstring("an::scene",rbdata->scene);
                      v.setreal("an::starttime",rbdata->starttime);
                      v.setreal("an::finishtime",rbdata->finishtime);
                      v.setreal("an::loops",rbdata->loops);
                      v.setinteger("an::reverse",rbdata->reverse);
                      v.setreal("an::scalex1",rbdata->scale1.x);
                      v.setreal("an::scaley1",rbdata->scale1.y);
                      v.setreal("an::scalez1",rbdata->scale1.z);
                      v.setreal("an::scalex2",rbdata->scale2.x);
                      v.setreal("an::scaley2",rbdata->scale2.y);
                      v.setreal("an::scalez2",rbdata->scale2.z);
                      v.setreal("an::rotationx1",rbdata->rotation1.x);
                      v.setreal("an::rotationy1",rbdata->rotation1.y);
                      v.setreal("an::rotationz1",rbdata->rotation1.z);
                      v.setreal("an::rotationx2",rbdata->rotation2.x);
                      v.setreal("an::rotationy2",rbdata->rotation2.y);
                      v.setreal("an::rotationz2",rbdata->rotation2.z);
                      v.setreal("an::startheight",rbdata->startheight);
                      v.setreal("an::finishheight",rbdata->finishheight);
                      v.setinteger("an::followpath",rbdata->followpath);
                      if (point)
                        *cstate = 2;
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       state.destroyselection(0);
       if (event->isa(EntitySelected))
         {  rbdata->list.add(((EntitySelectedEvent *)event)->getentity());
            rbdata->grouplist.add(((EntitySelectedEvent *)event)->getentity());
            *cstate = 3;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 3 :
       state.destroyselection(0);
       if (event->isa(EntitySelected))
         {  rbdata->list.add(((EntitySelectedEvent *)event)->getentity());
            rbdata->grouplist.add(((EntitySelectedEvent *)event)->getentity());
            *cstate = 4;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 4 :
       if (event->isa(Coordinate3d))
         {  if ((origin = new PointE(((Coordinate3dEvent *)event)->getp())) != NULL)
              rbdata->list.add(origin);
            *cstate = 5;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 5 :
       if (event->isa(Coordinate3d))
         {  if ((origin = new PointE(((Coordinate3dEvent *)event)->getp())) != NULL)
              {  rbdata->list.add(origin);
                 sprintf(info,"AN::patchpath:0 %.4lf %.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %d",
                              rbdata->starttime,rbdata->finishtime,
                              rbdata->loops,rbdata->reverse,
                              rbdata->scale1.x,rbdata->scale1.y,rbdata->scale1.z,
                              rbdata->scale2.x,rbdata->scale2.y,rbdata->scale2.z,
                              rbdata->rotation1.x,rbdata->rotation1.y,rbdata->rotation1.z,
                              rbdata->rotation2.x,rbdata->rotation2.y,rbdata->rotation2.z,
                              rbdata->startheight,rbdata->finishheight,rbdata->followpath);
                 Group *group = new Group(rbdata->list,rbdata->scene,info,options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   delete group;
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the patch path");
       rubberband.begin(0);
       rubberband.add(RBAnimation3,rbdata);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(patch_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the initial patch of the animation");
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(patch_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the final patch of the animation");
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the first patch");
    }
  else if (*cstate == 5)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the second patch");
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) rbdata->grouplist.unhighlight();
       delete rbdata;
    }
}
#endif

class Path
  {private:
     EntityList list;
     double length;
   public:
     double starttime,finishtime,startheight,finishheight;
     Path(EntityList l,double,double,double,double);
     ~Path() {  list.destroy();  }
     Point getposition(double t);
     Point getdirection(double t);
  };

Path::Path(EntityList l,double stime,double ftime,double sheight,double fheight)
{Entity *e;
 Point p;
  starttime = stime;
  finishtime = ftime;
  startheight = sheight;
  finishheight = fheight;
  list = l;
  length = 0.0;
  list.start();
  if ((e = list.next()) == NULL) return;
  if (! e->isa(point_entity)) return;
  while ((e = list.next()) != NULL)
    if (e->islinear())
      length += ((LinearEntity *)e)->length();
}

Point Path::getdirection(double t)
{Entity *e;
 double l1,l2,t1,s;
 Point p,tmp(1.0,0.0,0.0);
  t = (t - starttime) / (finishtime - starttime);
  l1 = length * t;
  list.start();
  if ((e = list.next()) == NULL) return tmp;
  if (! e->isa(point_entity)) return tmp;
  p = ((PointE *)e)->getp();
  if (list.peek() == 0) return tmp;
  while ((e = list.next()) != 0)
    if (e->islinear())
      {  l2 = ((LinearEntity *)e)->length();
         if (l2 < db.getptoler()) continue;
         if (l2 < l1 - db.getptoler())
           l1 -= l2;
         else
           {  if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
                {  s = -1.0;  t1 = (l2 - l1) / l2;
                }
              else
                {  s = 1.0;   t1 = l1 / l2;
                }
              if (t1 < 0.0) t1 = 0.0; else if (t1 > 1.0) t1 = 1.0;
              return ((LinearEntity *)e)->direction(t1) * s;
           }
         if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
           p = ((LinearEntity *)e)->start();
         else
           p = ((LinearEntity *)e)->end();
      }
  return tmp;
}

Point Path::getposition(double t)
{Entity *e;
 double l1,l2,t1;
 Point p,tmp(0.0,0.0,0.0);
  t = (t - starttime) / (finishtime - starttime);
  l1 = length * t;
  list.start();
  if ((e = list.next()) == NULL) return tmp;
  if (! e->isa(point_entity)) return tmp;
  p = ((PointE *)e)->getp();
  if (list.peek() == NULL)
    return p + Point(0.0,0.0,startheight+(finishheight-startheight)*t);
  while ((e = list.next()) != NULL)
    if (e->islinear())
      {  l2 = ((LinearEntity *)e)->length();
         if (l2 < db.getptoler()) continue;
         if (l2 < l1 - db.getptoler())
           l1 -= l2;
         else
           {  if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
                t1 = (l2 - l1) / l2;
              else
                t1 = l1 / l2;
              if (t1 < 0.0) t1 = 0.0; else if (t1 > 1.0) t1 = 1.0;
              return ((LinearEntity *)e)->position(t1) + Point(0.0,0.0,startheight+(finishheight-startheight)*l1/l2);
           }
         if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
           p = ((LinearEntity *)e)->start() + Point(0.0,0.0,startheight);
         else
           p = ((LinearEntity *)e)->end() + Point(0.0,0.0,finishheight);
      }
  else if (e->isa(point_entity))
    return ((PointE *)e)->getp() + Point(0.0,0.0,startheight+(finishheight-startheight)*t);
  return tmp;
}

class ObjectPath
  {public:
     Group *parent;
     int issubobject,followpath;
     Point origin,rotation1,rotation2,scale1,scale2;
     Path *path;
     GeneralList objects,subobjects;
     void moveobject(double t0,Transform *subt,EntityList *newlist);
     ObjectPath(Group *,int,int,Point,Point,Point,Point,Path *,GeneralList,Point);
  };

ObjectPath::ObjectPath(Group *p,int iso,int fp,Point r1,Point s1,Point r2,Point s2,Path *np,GeneralList l,Point o)
{ parent = p;  issubobject = iso;  followpath = fp;
  rotation1 = r1; scale1 = s1;  rotation2 = r2; scale2 = s2;
  path = np;  objects = l;  origin = o;
}

void ObjectPath::moveobject(double t0,Transform *subt,EntityList *newlist)
{Transform t1,t2;
 double t;
 Entity *c,*e;
 Group *object;
 ObjectPath *subobject;
 Point xaxis,yaxis,zaxis;

  t = (t0 - path->starttime) / (path->finishtime - path->starttime);
  t1.translate(-origin);
  t2.scale(scale1.x + (scale2.x - scale1.x)*t,
           scale1.y + (scale2.y - scale1.y)*t,
           scale1.z + (scale2.z - scale1.z)*t);
  t1.apply(t2);
  t2.rotatex((rotation1.x + (rotation2.x - rotation1.x)*t) * M_PI / 180.0);
  t1.apply(t2);
  t2.rotatey((rotation1.y + (rotation2.y - rotation1.y)*t) * M_PI / 180.0);
  t1.apply(t2);
  t2.rotatez((rotation1.z + (rotation2.z - rotation1.z)*t) * M_PI / 180.0);
  t1.apply(t2);
  if (followpath)
    {  t2.identity();
       xaxis = path->getdirection(t0);
       yaxis = xaxis.cross(Point(0.0,0.0,1.0));
       if (yaxis.length() < 1.0E-10)
         yaxis = xaxis.cross(Point(0.0,1.0,.0));
       zaxis = xaxis.cross(yaxis);
       xaxis.normalize();
       yaxis.normalize();
       zaxis.normalize();
       t2.rotate(xaxis,yaxis,zaxis);
       t1.apply(t2);
    }
  t2.translate(path->getposition(t0));
  t1.apply(t2);
  if (subt != NULL) t1.apply(*subt);
 for (objects.start() ; (object = (Group *) objects.next()) != NULL ; )
    {  db.geometry.clearstatus();
       c = object->clone(t1);
       c->EntityHeader::change(c->getcolour(),0,c->getstyle(),c->getweight());
       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getcopye() != NULL)
           {  newlist->add(e->getcopye());
              e->getcopye()->EntityHeader::change(e->getcolour(),0,e->getstyle(),e->getweight());
              e->CopyAttributes(e->getcopye()); 
           }
       db.geometry.add(c,2);
    }
  for (subobjects.start() ; (subobject = (ObjectPath *) subobjects.next()) != NULL ; )
    subobject->moveobject(t0,&t1,newlist);
}

class PatchPath
  {public:
     Group *parent;
     int issubobject,reverse,followpath;
     Point origin1,origin2,rotation1,rotation2,scale1,scale2;
     Path *path;
     Patch *patch1,*patch2;
     double loops;
     void movepatch(double t0,Transform *subt,EntityList *newlist);
     PatchPath(Group *,int,int,double,int,Point,Point,Point,Point,Path *,Patch *,Patch *,Point,Point);
  };

PatchPath::PatchPath(Group *p,int iso,int fp,double l,int r,Point r1,Point s1,Point r2,Point s2,Path *np,Patch *p1,Patch *p2,Point o1,Point o2)
{ parent = p;  issubobject = iso;  followpath = fp;
  loops = l;  reverse = r;
  rotation1 = r1; scale1 = s1;  rotation2 = r2; scale2 = s2;
  path = np;
  patch1 = p1;  patch2 = p2;
  origin1 = o1;  origin2 = o2;
}

void PatchPath::movepatch(double t0,Transform *subt,EntityList *newlist)
{Transform t1,t2,t3,t4;
 double t,dt;
 Entity *c;
 Point *polygon,p1,p2;
 int i;
 long loopnumber;
 EntityList tcurves;
 BitMask o(32);
 Point xaxis,yaxis,zaxis;

  t = (t0 - path->starttime) / (path->finishtime - path->starttime);
  loopnumber = long(t * loops - 0.000001);   //  If t == 1.0 and loops == 1.0 want loopnumber 0
  dt = t * loops - loopnumber;
  if (reverse && (loopnumber % 2) == 1) dt = 1.0 - dt;

  t1.translate(-origin1);
  t2.scale(scale1.x + (scale2.x - scale1.x)*t,
           scale1.y + (scale2.y - scale1.y)*t,
           scale1.z + (scale2.z - scale1.z)*t);
  t1.apply(t2);
  t2.rotatex((rotation1.x + (rotation2.x - rotation1.x)*t) * M_PI / 180.0);
  t1.apply(t2);
  t2.rotatey((rotation1.y + (rotation2.y - rotation1.y)*t) * M_PI / 180.0);
  t1.apply(t2);
  t2.rotatez((rotation1.z + (rotation2.z - rotation1.z)*t) * M_PI / 180.0);
  t1.apply(t2);
  if (followpath)
    {  t2.identity();
       xaxis = path->getdirection(t0);
       yaxis = xaxis.cross(Point(0.0,0.0,1.0));
       if (yaxis.length() < 1.0E-10)
         yaxis = xaxis.cross(Point(0.0,1.0,.0));
       zaxis = xaxis.cross(yaxis);
       xaxis.normalize();
       yaxis.normalize();
       zaxis.normalize();
       t2.rotate(xaxis,yaxis,zaxis);
       t1.apply(t2);
    }
  t2.translate(path->getposition(t0));
  t1.apply(t2);
  if (subt != NULL) t1.apply(*subt);


  t3.identity();
  t4.scale(scale1.x + (scale2.x - scale1.x)*t,
           scale1.y + (scale2.y - scale1.y)*t,
           scale1.z + (scale2.z - scale1.z)*t);
  t3.apply(t4);
  t4.rotatex((rotation1.x + (rotation2.x - rotation1.x)*t) * M_PI / 180.0);
  t3.apply(t4);
  t4.rotatey((rotation1.y + (rotation2.y - rotation1.y)*t) * M_PI / 180.0);
  t3.apply(t4);
  t4.rotatez((rotation1.z + (rotation2.z - rotation1.z)*t) * M_PI / 180.0);
  t3.apply(t4);
  t4.translate(path->getposition(t0));
  t3.apply(t4);
  if (subt != NULL) t3.apply(*subt);

  if (patch1->getun() != patch2->getun() || patch1->getvn() != patch2->getvn())
    {  patch1->setcopye(0);
       c = patch1->clone(t1);
       c->EntityHeader::change(c->getcolour(),0,c->getstyle(),c->getweight());
       newlist->add(c);
       db.geometry.add(c,2);
    }
  else
    {  polygon = new Point[patch1->getun() * patch1->getvn()];
       for (i = 0 ; i < patch1->getun() * patch1->getvn() ; i++)
         {  p1 = patch1->getpolygon()[i]-origin1;
            p2 = patch2->getpolygon()[i]-origin2;
            polygon[i] = t3.transform(p1 + (p2 - p1) * dt);
         }
       c = new Patch(*patch1,patch1->getun(),patch1->getvn(),patch1->getudegree(),patch1->getvdegree(),patch1->getxmesh(),patch1->getxmesh(),polygon,0,0,0,tcurves,o);
       c->EntityHeader::change(c->getcolour(),0,c->getstyle(),c->getweight());
       newlist->add(c);
       db.geometry.add(c,2);
    }
}

class CameraPath
  {public:
     int fixreference;
     Point reference;
     double startfocaldistance,finishfocaldistance;
     Path *path;
  };

class RBAnimation4_data
  {public:
     View3dWindow *window;
     View3dOffScreenBitmap *bitmap;
     double starttime,finishtime,t,dt;
     int frame,format,wireframe,hide,shadebefore,shadeafter,shading,quality,nframestocopy,initialframe,animatelayers,startlayer,nframes;
     RCCHAR scene[300],prefix[300],copyavifile[300];
     GeneralList objects,cameras,camerarefs,patches;
#if defined(AVI) && (MENUS != FLEXICADMENUS)
     PAVIFILE avifile;
     PAVISTREAM avistream,avicompressedstream;
#endif
     RBAnimation4_data(GeneralList,GeneralList,GeneralList,GeneralList);
     ~RBAnimation4_data();
  };

RBAnimation4_data::RBAnimation4_data(GeneralList cams,GeneralList camrefs,GeneralList objs,GeneralList pats)
{int version,fixref,reverse,followpath;
 double starttime,finishtime,startheight,finishheight,startfocaldistance,finishfocaldistance,loops;
 Point scale1,rotation1,scale2,rotation2;
 GeneralList olist;
 Entity *e;
 Group *object,*camera,*cameraref,*patch;
 PointE *pnt,*origin1,*origin2;
 Patch *patch1,*patch2;
 ObjectPath *op,*subop;
 PatchPath *pp;
 CameraPath *cp;
 Path *rp;
 EntityList list;
  for (cams.start() ; (camera = (Group *) cams.next()) != NULL ; )
    {  finishfocaldistance = startheight = finishheight = 0.0;
       sscanf(camera->getinstance()+11,"%d %lf %lf %d %lf %lf %lf %lf",&version,
                   &starttime,&finishtime,&fixref,&startfocaldistance,&finishfocaldistance,&startheight,&finishheight);
       if (finishfocaldistance == 0.0)
         finishfocaldistance = startfocaldistance;
       list = camera->getlist().copy();
       list.start();
       if ((pnt = (PointE *)list.next()) == NULL) continue;
       list.del(pnt);
       if ((cp = new CameraPath) == NULL) return;
       cp->path = new Path(list,starttime,finishtime,startheight,finishheight);
       cp->fixreference = fixref;
       cp->reference = pnt->getp();
       cp->startfocaldistance = startfocaldistance;
       cp->finishfocaldistance = finishfocaldistance;
       if (cp->path == NULL) return;
       cameras.add(cp);
    }
  for (camrefs.start() ; (cameraref = (Group *) camrefs.next()) != NULL ; )
    {  startheight = finishheight = 0.0;
       sscanf(cameraref->getinstance()+14,"%d %lf %lf %lf %lf %lf %lf %lf",
                    &version,&starttime,&finishtime,&startheight,&finishheight);
       list = cameraref->getlist().copy();
       rp = new Path(list,starttime,finishtime,startheight,finishheight);
       if (rp == NULL) return;
       camerarefs.add(rp);
    }

//  Find the patch paths
  for (pats.start() ; (patch = (Group *) pats.next()) != NULL ; )
    {GeneralList patchlist;
       startheight = finishheight = 0.0;  followpath = 0;
       sscanf(((Group *)patch)->getinstance()+15,"%d %lf %lf %lf %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",&version,
                   &starttime,&finishtime,&loops,&reverse,
                   &scale1.x,&scale1.y,&scale1.z,&scale2.x,&scale2.y,&scale2.z,
                   &rotation1.x,&rotation1.y,&rotation1.z,&rotation2.x,&rotation2.y,&rotation2.z,
                   &startheight,&finishheight,&followpath);
       list = patch->getlist().copy();
       list.start();
       if ((origin2 = (PointE *)list.next()) == NULL) continue;
       list.del(origin2);
       if ((origin1 = (PointE *)list.next()) == NULL) continue;
       list.del(origin1);
       if ((patch2 = (Patch *)list.next()) == NULL) continue;
       list.del(patch2);
       if ((patch1 = (Patch *)list.next()) == NULL) continue;
       list.del(patch1);
       if ((pp = new PatchPath(patch,0,followpath,loops,reverse,rotation1,scale1,rotation2,scale2,
                                new Path(list,starttime,finishtime,startheight,finishheight),patch1,patch2,origin1->getp(),origin2->getp())) == NULL) return;
       if (pp->path == 0)
         {  delete pp;  return;
         }
       patches.add(pp);
    }

// Find the objects.

  for (objs.start() ; (object = (Group *) objs.next()) != NULL ; )
    {GeneralList objlist;
       sscanf(object->getinstance()+15,"%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",&version,
                    &starttime,&finishtime,
                    &scale1.x,&scale1.y,&scale1.z,&scale2.x,&scale2.y,&scale2.z,
                    &rotation1.x,&rotation1.y,&rotation1.z,&rotation2.x,&rotation2.y,&rotation2.z,
                    &startheight,&finishheight,&followpath);
       list = object->getlist().copy();
       list.start();
       if ((pnt = (PointE *)list.next()) == NULL) continue;
       list.del(pnt);
       for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(group_entity))
           {  objlist.add(e);  list.del(e);  list.start();
           }
       if ((op = new ObjectPath(object,0,followpath,rotation1,scale1,rotation2,scale2,
                                new Path(list,starttime,finishtime,startheight,finishheight),objlist,pnt->getp())) == NULL) return;
       if (op->path == NULL)
         {  delete op;  return;
         }
       objects.add(op);
    }
// Now find the subobjects.

  olist = objects;
  for (objects.start() ; (subop = (ObjectPath *) objects.next()) != NULL ; )
    {  for (olist.start() ; (op = (ObjectPath *) olist.next()) != NULL ; )
         if (op != subop)
           {  for (op->objects.start() ; (object = (Group *) op->objects.next()) != NULL ; )
                if (object == subop->parent)
                  {  subop->issubobject = 1;
                     op->objects.del(object);
                     op->objects.start();
                     op->subobjects.add(subop);
                  }
           }
    }
}

RBAnimation4_data::~RBAnimation4_data()
{ObjectPath *op;
 PatchPath *pp;
 CameraPath *cam;
 Path *path;

  for (objects.start() ; (op = (ObjectPath *) objects.next()) != NULL ; )
    {  delete op->path;  delete op;
    }
  objects.destroy();
  for (patches.start() ; (pp = (PatchPath *) patches.next()) != NULL ; )
    {  delete pp->path;  delete pp;
    }
  patches.destroy();
  for (cameras.start() ; (cam = (CameraPath *) cameras.next()) != NULL ; )
    {  delete cam->path;  delete cam;
    }
  cameras.destroy();
  for (camerarefs.start() ; (path = (Path *) camerarefs.next()) != NULL ; )
    delete path;
  camerarefs.destroy();
}

#define TIME_TOL 0.001

void RBAnimation4(int draw,void *data,int,int,View3dSurface *)
{RBAnimation4_data *rbdata = (RBAnimation4_data *) data;
 EntityList newentities,entitiesleft,emptylist;
 long nentities;
 Entity *e;
 ObjectPath *op;
 PatchPath *pp;
 CameraPath *cp;
 Path *rp;
 Point eye,ref,up,zaxis(0.0,0.0,1.0);
 double ambient,diffuse,focaldistance;
 RCCHAR name[300];
 int setreference;
 RECT rect;
 RCHBRUSH hbrush;
#if defined(AVI)
 MEMORYSTATUS ms;
#endif
  if (draw != RB_TIMER) return;
  setreference = 0;
  up = rbdata->bitmap->getview()->getup().normalize();
  eye = rbdata->bitmap->getview()->geteye();
  ref = rbdata->bitmap->getview()->getref();
  for (rbdata->camerarefs.start() ; (rp = (Path *) rbdata->camerarefs.next()) != NULL ; )
    if (rbdata->t >= rp->starttime - TIME_TOL && rbdata->t <= rp->finishtime + TIME_TOL)
      {  ref = rp->getposition(rbdata->t);
         setreference = 1;
      }
  for (rbdata->cameras.start() ; (cp = (CameraPath *) rbdata->cameras.next()) != NULL ; )
    if (rbdata->t >= cp->path->starttime - TIME_TOL && rbdata->t <= cp->path->finishtime + TIME_TOL)
      {  if (up.cross(cp->path->getdirection(rbdata->t)).length() < 0.01)
           up += (rbdata->bitmap->getview()->geteye() - rbdata->bitmap->getview()->getref());
         eye = cp->path->getposition(rbdata->t);
         focaldistance = cp->startfocaldistance + (cp->finishfocaldistance - cp->startfocaldistance) * (rbdata->t  - cp->path->starttime) / (cp->path->finishtime - cp->path->starttime);
         if (! setreference)
           {  if (cp->fixreference)
                ref = eye + (cp->reference - eye).normalize() * focaldistance;
              else
                ref = eye + cp->path->getdirection(rbdata->t) * focaldistance;
           }
         else
           ref = eye + (ref - eye).normalize() * focaldistance;
      }
  up = (eye - ref).cross(up).cross(eye - ref).normalize();
  if ((eye - ref).normalize().cross(zaxis).length() > db.getatoler())
    up = zaxis;
  for (rbdata->objects.start() ; (op = (ObjectPath *) rbdata->objects.next()) != NULL ; )
    if (rbdata->t >= op->path->starttime - TIME_TOL && rbdata->t <= op->path->finishtime + TIME_TOL)
      if (! op->issubobject)
        op->moveobject(rbdata->t,NULL,&newentities);
  for (rbdata->patches.start() ; (pp = (PatchPath *) rbdata->patches.next()) != NULL ; )
    if (rbdata->t >= pp->path->starttime - TIME_TOL && rbdata->t <= pp->path->finishtime + TIME_TOL)
      if (! pp->issubobject)
        pp->movepatch(rbdata->t,NULL,&newentities);
  if (rbdata->frame+1 >= rbdata->initialframe && rbdata->bitmap->getview()->changeview(eye,ref,up))
    {  rbdata->bitmap->updatematrix();
       rect.left = 0;  rect.top = 0;
       rect.right = int(rbdata->bitmap->getwidth());  rect.bottom = int(rbdata->bitmap->getheight());
       hbrush = CreateSolidBrush(rbdata->bitmap->getbackground());
       FillRect(rbdata->bitmap->gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
       if (rbdata->shadebefore)
         {  if (rbdata->shading)
              {  RayTraceImage *rti = new RayTraceImage(rbdata->bitmap);
                 if (rti != 0)
                   {  if (rti->loadpolygons())
                        rti->raytrace();
                      delete rti;
                   }
              }
            else
              {  ambient = v.getreal("sh::ambient");
                 diffuse = v.getreal("sh::diffuse");
                 RenderImage image(rbdata->bitmap,0,ambient,diffuse);
                 image.render();
              }
         }
       if (rbdata->wireframe)
         rbdata->bitmap->paintevent();
       else if (rbdata->hide)
         {  HideImage image(rbdata->bitmap,v.getinteger("hd::dashhiddenlines") * 2);
            image.hide();
         }
       if (rbdata->shadeafter)
         {  if (rbdata->shading)
              {  RayTraceImage *rti = new RayTraceImage(rbdata->bitmap);
                 if (rti != 0 && rti->loadpolygons())
                   rti->raytrace();
              }
            else
              {  ambient = v.getreal("sh::ambient");
                 diffuse = v.getreal("sh::diffuse");
                 RenderImage image(rbdata->bitmap,0,ambient,diffuse);
                 image.render();
              }
         }
       BitBlt(rbdata->window->gethdc(),0,0,int(rbdata->window->getwidth()),int(rbdata->window->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
    }
  do {  nentities = 0;  entitiesleft = emptylist;
        for (newentities.start() ; (e = newentities.next()) != NULL ; )
          if (e->getalist().empty())
            {  nentities++;  db.geometry.del(e,0);
            }
          else
            entitiesleft.add(e);
        newentities.destroy();
        newentities = entitiesleft;
     } while (nentities > 0);
  newentities.destroy();
  rbdata->frame++;

#if defined(AVI)
  memset(&ms,0,sizeof(ms));
  ms.dwLength = sizeof(ms);
  GlobalMemoryStatus(&ms);
  if (ms.dwAvailPageFile < 5000000 && rbdata->format == 1)
    {  AbortEvent *aevent = new AbortEvent;
       state.sendevent(aevent);
       rbdata->frame = -1;
       return;
    }
#endif

  sprintf(name,"Cancel the animation using the right button : Frame %d",rbdata->frame);
  cadwindow->prompt->normalprompt(name);
  if (rbdata->frame >= rbdata->initialframe)
    {  if (rbdata->format == 2)
         {  sprintf(name,"%s%3.3d.BMP",rbdata->prefix,rbdata->frame);
            rbdata->window->saveimage(1,name,0);
         }
       else if (rbdata->format == 3)
         {  sprintf(name,"%s%3.3d.TGA",rbdata->prefix,rbdata->frame);
            rbdata->window->saveimage(2,name,0);
         }
#if defined(AVI) && (MENUS != FLEXICADMENUS)
       else if (rbdata->format == 1)
         rbdata->window->saveavistream(rbdata->avicompressedstream,rbdata->frame-rbdata->initialframe+1+rbdata->nframestocopy);
#endif
    }
  rbdata->t += rbdata->dt;
  if (rbdata->animatelayers)
    {  db.dismask.layer.clear(rbdata->startlayer + rbdata->frame - 1);
       db.dismask.layer.set(rbdata->startlayer + rbdata->frame);
    }

  
  if (rbdata->t > rbdata->finishtime + TIME_TOL)
    {  AbortEvent *aevent = new AbortEvent;
       state.sendevent(aevent);
    }
}

class FListDialogControl : public ListDialogControl
  {private:
   public:
     FListDialogControl(int id,int n,RCCHAR **l,int *f) : ListDialogControl(id,n,l,f) {}
     void load(Dialog *dialog);
     void changefocus(Dialog *dialog,int updatescreen = 1);
  };

void FListDialogControl::load(Dialog *dialog)
{ ListDialogControl::load(dialog);
  changefocus(dialog);
}

void FListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int type;
  ListDialogControl::changefocus(dialog,updatescreen);
  if (dialog->currentvalue(105,&type))
    {  if (type == 0)
         {  EnableWindow(GetDlgItem(dialog->gethdlg(),106),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),206),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),107),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),208),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),108),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),109),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),110),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),1110),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),210),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),111),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),211),0);
            SetWindowText(GetDlgItem(dialog->gethdlg(),206),_RCT("Image file prefix"));
         }
       else if (type == 2 || type == 3)
         {  EnableWindow(GetDlgItem(dialog->gethdlg(),106),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),206),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),107),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),208),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),108),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),109),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),110),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),1110),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),210),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),111),0);
            EnableWindow(GetDlgItem(dialog->gethdlg(),211),0);
            SetWindowText(GetDlgItem(dialog->gethdlg(),206),_RCT("Image file prefix"));
         }
       else if (type == 1)
         {  EnableWindow(GetDlgItem(dialog->gethdlg(),106),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),206),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),107),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),208),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),108),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),109),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),110),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),1110),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),210),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),111),1);
            EnableWindow(GetDlgItem(dialog->gethdlg(),211),1);
            SetWindowText(GetDlgItem(dialog->gethdlg(),206),_RCT("AVI file name"));
         }
    }
}

class FButtonDialogControl : public ButtonDialogControl
  {public:
     FButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int FButtonDialogControl::select(Dialog *dialog)
{RCOPENFILENAME ofn;
 RCCHAR szDirName[256];
 RCCHAR szFile[256],szFileTitle[256];
 RCUINT  i;
 RCCHAR  szFilter[256];
 int type;
//WCEFIX
#if ! defined(_WIN32_WCE)
  //_getcwd(szDirName, sizeof(szDirName));
  _tgetcwd(szDirName, sizeof(szDirName));
#endif
  szFile[0] = '\0';
  dialog->currentvalue(105,&type);
  memset(&ofn, 0, sizeof(OPENFILENAME));
  if (id == 107 && type == 2)
    {  _tcscpy(szFilter,_RCT("Windows bitmap *.bmp|*.bmp|"));
       ofn.lpstrTitle = _RCT("Select the prefix for the Windows bitmap files");
    }
  else if (id == 107 && type == 3)
    {  _tcscpy(szFilter,_RCT("TARGA *.tga|*.tga|"));
       ofn.lpstrTitle = _RCT("Select the prefix for the TARGA files");
    }
  else
    {  _tcscpy(szFilter,_RCT("AVI file name *.avi|*.avi|"));
       ofn.lpstrTitle = _RCT("Select the AVI file");
    }
  for (i = 0; szFilter[i] != '\0'; i++)
    if (szFilter[i] == '|') szFilter[i] = '\0';
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  if (type == 2)
    ofn.lpstrDefExt = _RCT("BMP");
  else if (type == 3)
    ofn.lpstrDefExt = _RCT("TGA");
  else if (type == 1)
    ofn.lpstrDefExt = _RCT("AVI");
  if (id == 107)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  else
    ofn.Flags = OFN_PATHMUSTEXIST;
  if (id == 107 && GetSaveFileName(&ofn) || id != 107 && GetOpenFileName(&ofn))
    {  if (type != 3)
         {  if (_tcsrchr(szFile,'.') != 0 && _tcsrchr(szFile,'\\') == 0)
              *_tcsrchr(szFile,'.') = 0;
            else if (_tcsrchr(szFile,'.') != 0 && _tcsrchr(szFile,'\\') != 0 && _tcsrchr(szFile,'.') > _tcsrchr(szFile,'\\'))
              *_tcsrchr(szFile,'.') = 0;
         }
       #if ! defined(_WIN32_WCE)
       //WCEFIX
       ((StringDialogControl *)dialog->getcontrol(id-1))->change(dialog,ofn.lpstrFile);
       #endif
    }
  return 0;
}

void CopyAVIFile(RBAnimation4_data *rbdata)
{
#if defined(AVI) && (MENUS != FLEXICADMENUS)
 MCI_DGV_OPEN_PARMS mciOpen;
 MCI_DGV_WINDOW_PARMS mciWindow;
 MCI_DGV_RECT_PARMS mciRect;
 MCI_SET_PARMS mciSet;
 MCI_DGV_STEP_PARMS mciStep;
 MCI_DGV_UPDATE_PARMS mciUpdate;
 int deviceid,i,nframestocopy;
 RECT rect;

  nframestocopy = rbdata->nframestocopy;
  rbdata->nframestocopy = 0;

  if (nframestocopy == 0)
    return;

  mciOpen.lpstrElementName = rbdata->copyavifile;
  if (mciSendCommand(0,MCI_OPEN,MCI_WAIT|MCI_OPEN_ELEMENT,(DWORD)&mciOpen))
    return;
  mciWindow.hWnd = rbdata->window->gethwnd();
  mciWindow.nCmdShow = SW_SHOWNORMAL;
  mciWindow.lpstrText = 0;
  if (mciSendCommand(mciOpen.wDeviceID,MCI_WINDOW,MCI_WAIT|MCI_DGV_WINDOW_HWND,(DWORD)&mciWindow))
    return;

  deviceid = mciOpen.wDeviceID;

  if (mciSendCommand(deviceid,MCI_WHERE,MCI_WAIT|MCI_DGV_WHERE_SOURCE,(DWORD)&mciRect))
    {  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
       return;
    }
  if (mciRect.rc.right != rbdata->window->getwidth() || mciRect.rc.bottom != rbdata->window->getheight())
    {  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
       return;
    }

  if (mciSendCommand(mciOpen.wDeviceID,MCI_WHERE,MCI_WAIT|MCI_DGV_WHERE_SOURCE,(DWORD)&mciRect))
    {  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
       return;
    }

  GetClientRect(rbdata->window->gethwnd(),&rect);

  if (mciSendCommand(mciOpen.wDeviceID,MCI_SET,MCI_SET_ON | MCI_DGV_SET_SEEK_EXACTLY | MCI_WAIT,(DWORD)&mciSet))
    {  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
       return;
    }

  mciUpdate.hDC = rbdata->window->gethdc();
  mciSendCommand(deviceid,MCI_UPDATE,MCI_WAIT|MCI_DGV_UPDATE_HDC,(DWORD)&mciUpdate);

  rbdata->window->saveavistream(rbdata->avicompressedstream,1);
  rbdata->nframestocopy++;

  for (i = 1 ; i < nframestocopy ; i++)
    {  mciStep.dwFrames = 1;
       mciSendCommand(deviceid,MCI_STEP, MCI_DGV_STEP_FRAMES | MCI_WAIT,(DWORD)&mciStep);
//       mciStatus.dwItem = MCI_STATUS_MODE;
//       do {  mciSendCommand(deviceid,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mciStatus);
//          } while (mciStatus.dwReturn != MCI_MODE_STOP && mciStatus.dwReturn != MCI_MODE_PAUSE);
       rbdata->window->saveavistream(rbdata->avicompressedstream,i+1);
       rbdata->nframestocopy++;
    }

  mciSendCommand(deviceid,MCI_CLOSE,MCI_WAIT,0);
#endif
}

void animation_command4(int *cstate,HighLevelEvent *,void **data)
{RBAnimation4_data *rbdata = (RBAnimation4_data *) *data;
 int i,animatelayers,startlayer,shading,framessec,format,wireframe,hide,shadebefore,shadeafter,width,height,colour,quality,nframestocopy,initialframe;
 double starttime,finishtime;
 Dialog dialog("Animationrun_Dialog");
 DialogControl *dc;
 Group *group;
 GeneralList objects,cameras,camerarefs,patches;
 Entity *e;
 RCCHAR prefix[300],scene[300],copyavifile[300];
 RCCHAR *formatl[] = {  _RCT("Don't save"),_RCT("Windows AVI"),_RCT("Windows bitmap"),_RCT("TARGA")  };
 RCCHAR *qualityl[] = {  _RCT("0 - Worst"),_RCT("1"),_RCT("2"),_RCT("3"),_RCT("4"),_RCT("5 - Average"),_RCT("6"),_RCT("7"),_RCT("8"),_RCT("9"),_RCT("10 - Best")  };
#if defined(AVI) && (MENUS != FLEXICADMENUS)
 AVISTREAMINFO strhdr;
 AVICOMPRESSOPTIONS opts;
#endif
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentsurface() == NULL)
           { RCMessageBox(cadwindow->gethwnd(),_RCT("Please select a view to animate"),_RCT("Run animation"),MB_ICONINFORMATION);
              *cstate = ExitState;
              break;
           }
         strcpy(scene,v.getstring("an::scene"));
         framessec = v.getinteger("an::framessec");
         format = v.getinteger("an::format");
         strcpy(prefix,v.getstring("an::prefix"));
         strcpy(copyavifile,v.getstring("an::copyavifile"));
         wireframe = v.getinteger("an::wireframe");
         starttime = v.getreal("an::starttime");
         finishtime = v.getreal("an::finishtime");
         hide = v.getinteger("an::hide");
         animatelayers = v.getinteger("an::animatelayers");
         startlayer = v.getinteger("an::startlayer");
         shadebefore = v.getinteger("an::shadebefore");
         shadeafter = v.getinteger("an::shadeafter");
         shading = v.getinteger("an::shading");
         quality = v.getinteger("an::quality");
         initialframe = v.getinteger("an::initialframe");
         nframestocopy = v.getinteger("an::nframestocopy");
         width = int (cadwindow->getcurrentwindow()->getwidth() + 0.5);
         height = int (cadwindow->getcurrentwindow()->getheight() + 0.5);
         colour = 255;
         dialog.add(new StringDialogControl(100,scene,256));
         dialog.add(dc = new RealDialogControl(101,&starttime,0.1,0.0,100000.0));
         dialog.add(new ScrollBarDialogControl(1101,dc));
         dialog.add(dc = new RealDialogControl(102,&finishtime,0.1,0.0,100000.0));
         dialog.add(new ScrollBarDialogControl(1102,dc));
         dialog.add(dc = new IntegerDialogControl(103,&framessec,2,10000));
         dialog.add(new ScrollBarDialogControl(1103,dc));
         dialog.add(dc = new IntegerDialogControl(104,&initialframe,1,20000));
         dialog.add(new ScrollBarDialogControl(1104,dc));
#if VERSION == TRICAD
         dialog.add(new FListDialogControl(105,2,formatl,&format));
#else
         dialog.add(new FListDialogControl(105,4,formatl,&format));
#endif
         dialog.add(new StringDialogControl(106,prefix,300));
         dialog.add(new FButtonDialogControl(107));
         dialog.add(new StringDialogControl(108,copyavifile,300));
         dialog.add(new FButtonDialogControl(109));
         dialog.add(dc = new IntegerDialogControl(110,&nframestocopy,0,20000));
         dialog.add(new ScrollBarDialogControl(1110,dc));
         dialog.add(new FListDialogControl(111,11,qualityl,&quality));
         dialog.add(dc = new IntegerDialogControl(112,&width,2,10000));
         dialog.add(new ScrollBarDialogControl(1112,dc));
         dialog.add(dc = new IntegerDialogControl(113,&height,2,10000));
         dialog.add(new ScrollBarDialogControl(1113,dc));
         dialog.add(dc = new IntegerDialogControl(114,&colour,0,255));
         dialog.add(new ScrollBarDialogControl(1114,dc));
         dialog.add(new CheckBoxDialogControl(115,&wireframe));
         dialog.add(new CheckBoxDialogControl(116,&hide));
         dialog.add(new CheckBoxDialogControl(117,&shadebefore));
         dialog.add(new CheckBoxDialogControl(118,&shadeafter));
         dialog.add(new RadioButtonDialogControl(119,119,120,&shading));
         dialog.add(new RadioButtonDialogControl(120,119,120,&shading));
         dialog.add(dc = new IntegerDialogControl(121,&startlayer,1000,2047));
         dialog.add(new ScrollBarDialogControl(1121,dc));
         dialog.add(new CheckBoxDialogControl(122,&animatelayers));

         if (dialog.process() == TRUE)
           {  v.setstring("an::scene",scene);
              v.setinteger("an::framessec",framessec);
              v.setinteger("an::format",format);
              v.setstring("an::prefix",prefix);
              v.setstring("an::copyavifile",copyavifile);
              v.setinteger("an::wireframe",wireframe);
              v.setreal("an::starttime",starttime);
              v.setreal("an::finishtime",finishtime);
              v.setinteger("an::hide",hide);
              v.setinteger("an::animatelayers",animatelayers);
              v.setinteger("an::startlayer",startlayer);
              v.setinteger("an::shadebefore",shadebefore);
              v.setinteger("an::shadeafter",shadeafter);
              v.setinteger("an::shading",shading);
              v.setinteger("an::quality",quality);
              v.setinteger("an::nframestocopy",nframestocopy);
              v.setinteger("an::initialframe",initialframe);
              for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                if (e->isa(group_entity))
                  {  group = (Group *) e;
                     if (strcmp(group->getclassname(),scene) == 0)
                       {  if (strncmp(group->getinstance(),"AN::camera:",11) == 0)
                            cameras.add(group);
                          else if (strncmp(group->getinstance(),"AN::cameraref:",14) == 0)
                            camerarefs.add(group);
                          else if (strncmp(group->getinstance(),"AN::objectpath:",15) == 0)
                            objects.add(group);
                          else if (strncmp(group->getinstance(),"AN::patchpath:",14) == 0)
                            patches.add(group);
                       }
                  }
              if (animatelayers == 0 && cameras.length() == 0 && camerarefs.length() == 0 && objects.length() == 0 && patches.length() == 0)
                { RCMessageBox(cadwindow->gethwnd(),_RCT("No camera paths, no camera reference paths and no object paths were found"),_RCT("Run animation"),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else
                {  rbdata = new RBAnimation4_data(cameras,camerarefs,objects,patches);
                   *data = rbdata;
                   if (rbdata == NULL)
                     {  state.fail(Memory,_RCT("Run animation command"));
                        *cstate = ExitState;
                     }
                   else
                     {  rbdata->window = cadwindow->getcurrentwindow();
                        if (format == 1)
                          {  width = (width + 3) / 4;  width *= 4;
                             height = (height + 3) / 4;  height *= 4;
                          }
                        rbdata->window->sendsizeevent(width,height);
                        if ((rbdata->bitmap = new View3dOffScreenBitmap(rbdata->window)) != NULL)
                          {  if (! rbdata->bitmap->getdefined())
                               {  delete rbdata->bitmap;
                                  rbdata->bitmap = NULL;
                                  *cstate = ExitState;
                               }
                             else
                               {  rbdata->bitmap->setbackground(RGB(db.colourtable[colour].red,db.colourtable[colour].green,db.colourtable[colour].blue));
                                  rbdata->starttime = starttime;
                                  rbdata->finishtime = finishtime;
                                  rbdata->t = starttime;
                                  rbdata->dt = 1.0 / double(framessec);
                                  rbdata->frame = 0;
                                  rbdata->animatelayers = animatelayers;
                                  rbdata->startlayer = startlayer;
                                  rbdata->nframes = (int)((finishtime - starttime) * (framessec) + 1);
                                  strcpy(rbdata->prefix,prefix);
                                  strcpy(rbdata->scene,scene);
                                  rbdata->format = format;
                                  rbdata->wireframe = wireframe;
                                  rbdata->hide = hide;
                                  rbdata->shadebefore = shadebefore;
                                  rbdata->shadeafter = shadeafter;
                                  rbdata->shading = shading;
                                  rbdata->quality = quality;
                                  rbdata->initialframe = initialframe;
                                  rbdata->nframestocopy = nframestocopy;
                                  strcpy(rbdata->copyavifile,copyavifile);
                                  for (i = 900 ; i < 1000 ; i++)
                                    db.dismask.layer.clear(i);
                                  if (rbdata->animatelayers)
                                    {  for (i = 0 ; i <= rbdata->nframes ; i++)
                                         db.dismask.layer.clear(rbdata->startlayer+i);
                                       db.dismask.layer.set(rbdata->startlayer);
                                    }   
                                    
                                  *cstate = 1;
                                  if (format == 1)
                                    {
#if defined(AVI) && (MENUS != FLEXICADMENUS)
                                       if (strlen(rbdata->prefix) < 4 || _stricmp(rbdata->prefix+strlen(rbdata->prefix)-4,".avi") != 0)
                                         {  if (strrchr(rbdata->prefix,'.') > strrchr(rbdata->prefix,'\\'))
                                              strcpy(strrchr(rbdata->prefix,'.'),".AVI");
                                            else
                                              strcat(rbdata->prefix,".AVI");
                                         }
                                       rbdata->avicompressedstream = 0;
                                       rbdata->avistream = 0;
                                       rbdata->avifile = 0;
                                       AVIFileInit();
                                       if (AVIFileOpen(&rbdata->avifile,rbdata->prefix,OF_WRITE | OF_CREATE,0) != AVIERR_OK)
                                         {  delete rbdata->bitmap;
                                            rbdata->bitmap = 0;
                                           RCMessageBox(cadwindow->gethwnd(),"AVI file could not be created.","Run animation",MB_ICONINFORMATION);
                                            *cstate = ExitState;
                                         }
                                       else
                                         {  memset(&strhdr, 0, sizeof(strhdr));
                                            strhdr.fccType = streamtypeVIDEO;// stream type
                                            strhdr.fccHandler = 0;
                                            strhdr.dwScale = 1;
                                            strhdr.dwRate = framessec;
                                            strhdr.dwSuggestedBufferSize  = long(4*((width*3+3)/4)) * long(height);
                                            SetRect(&strhdr.rcFrame,0,0,width,height);
                                            if (AVIFileCreateStream(rbdata->avifile,&rbdata->avistream,&strhdr) != AVIERR_OK)
                                              {  delete rbdata->bitmap;
                                                 rbdata->bitmap = 0;
                                                 *cstate = ExitState;
                                              }
                                            else
                                              {  memset(&opts,0,sizeof(opts));
                                                 opts.dwFlags = AVICOMPRESSF_VALID;
                                                 opts.fccType = streamtypeVIDEO;
                                                 opts.fccHandler = mmioFOURCC('M', 'S', 'V', 'C');
                                                 opts.dwQuality = rbdata->quality * 1000;
                                                 opts.dwKeyFrameEvery = 15;
                                                 if (AVIMakeCompressedStream(&rbdata->avicompressedstream,rbdata->avistream,&opts,0) != AVIERR_OK)
                                                   {  delete rbdata->bitmap;
                                                      rbdata->bitmap = 0;
                                                      *cstate = ExitState;
                                                   }
                                                 else
                                                   CopyAVIFile(rbdata);
                                              }
                                         }
#endif
                                    }
                               }
                          }
                        else
                          *cstate = ExitState;
                     }
                }
           }
         else
           {  rbdata = NULL;  *cstate = ExitState;
           }
         break;
       case 1:
         rubberband.end(0);
         program->setbusy(0);
         cadwindow->updatewindows(rbdata->window->getview());
         for (i = 900 ; i < 1000 ; i++)
           db.dismask.layer.set(i);
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBAnimation4,rbdata);
       cadwindow->prompt->normalprompt("Cancel the animation using the right button");
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0 && rbdata->format == 1)
         {
#if defined(AVI) && (MENUS != FLEXICADMENUS)
            if (rbdata->avicompressedstream)
              AVIStreamRelease(rbdata->avicompressedstream);
            if (rbdata->avistream)
	           AVIStreamRelease(rbdata->avistream);
            if (rbdata->avifile)
              AVIFileRelease(rbdata->avifile);
            AVIFileExit();
            if (rbdata->frame == -1)
             RCMessageBox(cadwindow->gethwnd(),"AVI file cannot be created - memory low.","Run Animation",MB_ICONINFORMATION);
#endif
         }
       if (rbdata != NULL && rbdata->bitmap != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
}

#if (VERSION != FLEXICAD)
void animation_command5(int *cstate,HighLevelEvent *,void **)
{
#if ! defined(_WIN32_WCE)
 RCOPENFILENAME ofn;
 RCCHAR directory[256],filename[256],filetitle[256],filter[256];
 RCUINT  i;
  *cstate = ExitState;
  _getcwd(directory,sizeof(directory));
  filename[0] = '\0';
  _tcscpy(filter,_RCT("AVI files *.avi|*.avi|"));
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= filename;
  ofn.nMaxFile = sizeof(filename);
  ofn.lpstrFileTitle = filetitle;
  ofn.nMaxFileTitle = sizeof(filetitle);
  ofn.lpstrTitle = _RCT("Select the AVI file to view");
  ofn.lpstrInitialDir = directory;
  ofn.lpstrDefExt = _RCT("avi");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn))
#if ! defined(_WIN32_WCE)
    new VideoWindow(cadwindow->getsubwindow(),ofn.lpstrFile,ofn.lpstrFileTitle);
#else
    ;
#endif
#endif  //  Video Windows not supported by Windows CE
}

void animation_command6(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->stop();
#endif
  *cstate = ExitState;
}

void animation_command7(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->pause();
#endif
  *cstate = ExitState;
}

void animation_command8(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->play();
#endif
  *cstate = ExitState;
}

void animation_command9(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->reverse();
#endif
  *cstate = ExitState;
}

void animation_command10(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->stepforward();
#endif
  *cstate = ExitState;
}

void animation_command11(int *cstate,HighLevelEvent *event,void **)
{ 
#if ! defined(_WIN32_WCE)
  ((VideoWindow *)((NewCommandEvent *)event)->getwindow())->stepreverse();
#endif
  *cstate = ExitState;
}
#endif
#endif
