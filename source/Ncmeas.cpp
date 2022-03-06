
#include "ncwin.h"
#include "RCCentralwidget.h"

void measure_command0(int *cstate,HighLevelEvent *,void **)
{ 
#if defined(CADAUSTRALIAVERSION)
  //cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Measure"));
#else
  cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Measure"));
#endif
  *cstate = ExitState;
}

static int areacallback(SurfaceEntity *,void *polygon,int,void *,void *cbdata)
{double *area = (double *) cbdata;
  if (! ((Polygon3d *)polygon)->getdefined()) return 1;
  *area += fabs(((Polygon3d *)polygon)->getarea());
  delete ((Polygon3d *)polygon);
  return 1;
}

class MDialog : public Dialog
{private:
  RCHWND lasthwnd;
 public:
  MDialog(RCCHAR *name) : Dialog(name,1) {  lasthwnd = 0;  };
  MDialog(char *name) : Dialog(CHAR2RCCHR(name),1) {  lasthwnd = 0;  };
  void setfocus(RCHWND);
  void getfocus(RCHWND);
  void exitsizemove(void);
};

void MDialog::setfocus(RCHWND hwnd)
{ if (hwnd != gethdlg())
    lasthwnd = hwnd;
}

void MDialog::getfocus(RCHWND hwnd)
{
}

void MDialog::exitsizemove(void)
{ SetFocus(lasthwnd);
}

struct RBMeasure1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    Point p1;
   double totlength,totarea,area,length,scale;
   int nlength,narea,enablescale;
   EntityList list;
   Dialog *dialog;
   DialogControl *dc[10];
   RCCHAR direction[300];
};

#ifdef NEW_RUBBERB
void RBMeasure1(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBMeasure1_data *rbdata = (RBMeasure1_data *) data;
  Line line(rbdata->p1,*p2);

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (line.getdefined())
  {
      //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
  }

}
#else
void RBMeasure1(int,void *data,Point *p2,View3dSurface *)
{
  RBMeasure1_data *rbdata = (RBMeasure1_data *) data;
  Line line(rbdata->p1,*p2);
#ifdef USING_WIDGETS
  if (line.getdefined())
  {
      line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
  }
#else
  if (line.getdefined()) line.draw(DM_INVERT);
#endif
}
#endif

class MECheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     MECheckBoxDialogControl(int id,int *v) : CheckBoxDialogControl(id,v) {} ;
     int select(Dialog *dialog);
     void load(Dialog *dialog);
  };

void MECheckBoxDialogControl::load(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),114),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1114),oldvalue);
  CheckBoxDialogControl::load(dialog);
}

int MECheckBoxDialogControl::select(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),114),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1114),! oldvalue);
  return CheckBoxDialogControl::select(dialog);
}

/**
 * @brief measure_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Measure length and Area command
 *
 * KMJ : Working in Qt version
 */
void measure_command1(int *cstate,HighLevelEvent *event,void **data)
{RBMeasure1_data *rbdata = (RBMeasure1_data *) *data;
 double length,area;
 ResourceString rs0(NCMEAS),rs1(NCMEAS+1),rs7(NCMEAS+7),rs8(NCMEAS+8);
 RCCHAR text[300],message[600];
 Entity *e;
 BitMask options(32);
 Point p;

  switch (*cstate)
    {case InitialState :
       if ((rbdata = new RBMeasure1_data) == NULL)
         {  state.fail(Memory,"Measure command 1");
            *cstate = ExitState;
         }
       else if ((rbdata->dialog = new MDialog((char*)"MeasureLengthArea_Dialog")) == 0)
         {  state.fail(Memory,"Measure command 1");
            *cstate = ExitState;
         }
       else
         {  *cstate = 1;
            rbdata->area = rbdata->length = 0.0;
            rbdata->nlength = 0;
            rbdata->narea = 0;
            rbdata->totlength = 0.0;
            rbdata->totarea = 0.0;
            rbdata->scale = v.getreal("me::scale");
            rbdata->enablescale = v.getinteger("me::enablescale");
            strcpy(rbdata->direction,""); 
            area = length = 0.0;
            rbdata->list = state.getselection().copy();
            state.destroyselection(0);
            for (rbdata->list.start() ; (e = rbdata->list.next()) != 0 ; )
              if (e->islinear())
                {  length = ((LinearEntity *)e)->length();
                   rbdata->totlength += length;
                   rbdata->nlength++;
                   if (e->isa(line_entity))
                     {Point p;
                        p = ((Line *)e)->getp2() - ((Line *)e)->getp1();
                        if (rbdata->enablescale == 0)
                          p.string(rbdata->direction);
                        else
                          (p * rbdata->scale).string(rbdata->direction);
                     }
                   else
                     strcpy(rbdata->direction,""); 
                }
              else if (e->issurface())
                {  area = 0.0;
                   ((SurfaceEntity *)e)->facet(options,0.0,100,100,0,areacallback,&area);
                   strcpy(rbdata->direction,""); 
                   rbdata->totarea += area;
                   rbdata->narea++;
                }
              else
                {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONINFORMATION);
                   strcpy(rbdata->direction,""); 
                   rbdata->list.unhighlight();
                   rbdata->list.destroy();
                }
            rbdata->length = length;  rbdata->area = area;
            rbdata->dialog->add(rbdata->dc[0] = new IntegerDialogControl(100,&rbdata->nlength));
            rbdata->dialog->add(rbdata->dc[1] = new RealDialogControl(101,&rbdata->length,ScaleByLength));
            rbdata->dialog->add(rbdata->dc[2] = new RealDialogControl(102,&rbdata->totlength,ScaleByLength));
            rbdata->dialog->add(rbdata->dc[3] = new IntegerDialogControl(103,&rbdata->narea));
            rbdata->dialog->add(rbdata->dc[4] = new RealDialogControl(104,&rbdata->area,ScaleByArea));
            rbdata->dialog->add(rbdata->dc[5] = new RealDialogControl(105,&rbdata->totarea,ScaleByArea));
            rbdata->dialog->add(rbdata->dc[6] = new StringDialogControl(106,rbdata->direction,300));
            rbdata->dialog->add(rbdata->dc[7] = new MECheckBoxDialogControl(110,&rbdata->enablescale));
            rbdata->dialog->add(rbdata->dc[8] = new RealDialogControl(114,&rbdata->scale));
            //rbdata->dialog->add(rbdata->dc[9] = new ScrollBarDialogControl(1114,rbdata->dc[8]));

#ifdef USING_WIDGETS
            rbdata->qgi=0;
            rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif

            RCHWND hwnd = GetFocus();

            if (! rbdata->dialog->process())
              *cstate = ExitState;
            else
              {
                 SetFocus(hwnd);
                 if (v.getinteger("db::hidemeasuredialog") != 0)
                   ShowWindow(rbdata->dialog->gethdlg(),SW_HIDE);
                 else
                     ShowWindow(rbdata->dialog->gethdlg(),SW_SHOW);
              }

         }
       *data = rbdata;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 2;
         }
       else if (event->isa(EntitySelected))
         {  length = area = 0.0;
            ((CheckBoxDialogControl *)rbdata->dc[7])->currentvalue(rbdata->dialog,&rbdata->enablescale);
            if (rbdata->enablescale == 0)
              rbdata->scale = 1.0;
            else
              ((RealDialogControl *)rbdata->dc[8])->currentvalue(rbdata->dialog,&rbdata->scale);
            if (((EntitySelectedEvent *) event)->getentity()->islinear())
              {  length = ((LinearEntity *)((EntitySelectedEvent *) event)->getentity())->length();
                 rbdata->totlength += length;
                 rbdata->nlength++;
                 if (((EntitySelectedEvent *) event)->getentity()->isa(line_entity))
                   {Point p;
                      p = ((Line *)((EntitySelectedEvent *) event)->getentity())->getp2() - ((Line *)((EntitySelectedEvent *) event)->getentity())->getp1();
                      (p*rbdata->scale).string(rbdata->direction);
                   }
                 else
                   strcpy(rbdata->direction,""); 
              }
            else
              {  area = 0.0;
                 ((SurfaceEntity *)((EntitySelectedEvent *)event)->getentity())->facet(options,0.0,100,100,0,areacallback,&area);
                 rbdata->totarea += area;
                 rbdata->narea++;
                 strcpy(rbdata->direction,"");
              }
            ((IntegerDialogControl *)rbdata->dc[0])->change(rbdata->dialog,rbdata->nlength);
            ((RealDialogControl *)rbdata->dc[1])->change(rbdata->dialog,length*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[2])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
            ((IntegerDialogControl *)rbdata->dc[3])->change(rbdata->dialog,rbdata->narea);
            ((RealDialogControl *)rbdata->dc[4])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
            ((StringDialogControl *)rbdata->dc[6])->change(rbdata->dialog,rbdata->direction);

            rbdata->list.add(((EntitySelectedEvent *) event)->getentity());
            state.destroyselection(0);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {Point p;

            ((CheckBoxDialogControl *)rbdata->dc[7])->currentvalue(rbdata->dialog,&rbdata->enablescale);
            if (rbdata->enablescale == 0)
              rbdata->scale = 1.0;
            else
              ((RealDialogControl *)rbdata->dc[8])->currentvalue(rbdata->dialog,&rbdata->scale);

            area = 0.0;
            p = ((Coordinate3dEvent *) event)->getp() - rbdata->p1;
            length = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).length();
            (p*rbdata->scale).string(rbdata->direction);

            rbdata->totlength += length;
            rbdata->nlength++;
            ((IntegerDialogControl *)rbdata->dc[0])->change(rbdata->dialog,rbdata->nlength);
            ((RealDialogControl *)rbdata->dc[1])->change(rbdata->dialog,length*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[2])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
            ((IntegerDialogControl *)rbdata->dc[3])->change(rbdata->dialog,rbdata->narea);
            ((RealDialogControl *)rbdata->dc[4])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
            ((StringDialogControl *)rbdata->dc[6])->change(rbdata->dialog,rbdata->direction);
            *cstate = 1;
         }
       else if (event->isa(EntitySelected))
         {  ((CheckBoxDialogControl *)rbdata->dc[7])->currentvalue(rbdata->dialog,&rbdata->enablescale);
            if (rbdata->enablescale == 0)
              rbdata->scale = 1.0;
            else
              ((RealDialogControl *)rbdata->dc[8])->currentvalue(rbdata->dialog,&rbdata->scale);

            area = 0.0;
            p = ((EntitySelectedEvent *) event)->getentity()->nearp(rbdata->p1) - rbdata->p1;
            length = (((EntitySelectedEvent *) event)->getentity()->nearp(rbdata->p1) - rbdata->p1).length();
            (p*rbdata->scale).string(rbdata->direction);
            rbdata->totlength += length;
            rbdata->nlength++;
            ((IntegerDialogControl *)rbdata->dc[0])->change(rbdata->dialog,rbdata->nlength);
            ((RealDialogControl *)rbdata->dc[1])->change(rbdata->dialog,length*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[2])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
            ((IntegerDialogControl *)rbdata->dc[3])->change(rbdata->dialog,rbdata->narea);
            ((RealDialogControl *)rbdata->dc[4])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
            ((RealDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
            ((StringDialogControl *)rbdata->dc[6])->change(rbdata->dialog,rbdata->direction);

            *cstate = 1;
            state.destroyselection(1);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,linear_mask,surface_mask,end_list);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[0]->getid(),text,300);
       sprintf(message,"NL %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[1]->getid(),text,300);
       sprintf(message+strlen(message),"  L %s",text);
       sprintf(message+strlen(message),"  D %s",rbdata->direction);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[2]->getid(),text,300);
       sprintf(message+strlen(message),"  TL %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[3]->getid(),text,300);
       sprintf(message+strlen(message),"  NA %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[4]->getid(),text,300);
       sprintf(message+strlen(message),"  A %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[5]->getid(),text,300);
       sprintf(message+strlen(message),"  TA %s   ",text);
       strcpy(message,rs0.gets());
       cadwindow->prompt->normalprompt(message);


    }
  else if (*cstate == 2)
    {  GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[0]->getid(),text,300);
       sprintf(message,"NL %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[1]->getid(),text,300);
       sprintf(message+strlen(message),"  L %s",text);
       sprintf(message+strlen(message),"  D %s",rbdata->direction);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[2]->getid(),text,300);
       sprintf(message+strlen(message),"  TL %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[3]->getid(),text,300);
       sprintf(message+strlen(message),"  NA %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[4]->getid(),text,300);
       sprintf(message+strlen(message),"  A %s",text);
       GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[5]->getid(),text,300);
       sprintf(message+strlen(message),"  TA %s   ",text);
       strcpy(message,rs1.gets());
       cadwindow->prompt->normalprompt(message);

       state.selmask.entity.clearandset(xyz_mask,linear_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBMeasure1,rbdata);
       

    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  rbdata->list.unhighlight();
            rbdata->list.destroy();
            delete rbdata->dialog;
         }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
      if(rbdata && rbdata->qgi)
      {
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
          delete rbdata->qgi; rbdata->qgi=0;
      }
#endif
#endif
       delete rbdata;
       rbdata = 0;
    }

#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata && rbdata->qgi)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
  }
#endif
#endif

}

struct RBMeasure2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi1,*qgi2;
#endif
    int state;
   Point p1,p2;
   Line *line;
   Dialog *dialog;
   RealDialogControl *dc1,*dc2,*dc3;
   double x1,x2,x3;
};

#ifdef NEW_RUBBERB
void RBMeasure2(int dmode,void *data,Point *p2,View3dSurface *)
{
    RBMeasure2_data *rbdata = (RBMeasure2_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    if (rbdata->state == 2)
     {
          Line line(rbdata->p1,*p2);
          if (line.getdefined())
          {
              //line.qgi = rbdata->qgi1; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              //rbdata->qgi1 = line.qgi; rbdata->zValue = line.zValue;
          }
     }
     else
     {
          Line line1(rbdata->p1,rbdata->p2);
          if (line1.getdefined())
          {
              //line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
              line1.draw(DM_INVERT);
              //rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;
          }
          Line line2(rbdata->p2,*p2);
          if (line2.getdefined())
          {
              //line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
              line2.draw(DM_INVERT);
              //rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;
          }
     }
}
#else
void RBMeasure2(int,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
    RBMeasure2_data *rbdata = (RBMeasure2_data *) data;
     if (rbdata->state == 2)
     {
          Line line(rbdata->p1,*p2);
          if (line.getdefined())
          {
              line.qgi = rbdata->qgi1; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              rbdata->qgi1 = line.qgi; rbdata->zValue = line.zValue;
          }
     }
     else
     {
          Line line1(rbdata->p1,rbdata->p2);
          if (line1.getdefined())
          {
              line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
              line1.draw(DM_INVERT);
              rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;
          }
          Line line2(rbdata->p2,*p2);
          if (line2.getdefined())
          {
              line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
              line2.draw(DM_INVERT);
              rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;
          }
     }

#else
 RBMeasure2_data *rbdata = (RBMeasure2_data *) data;
  if (rbdata->state == 2)
    {  Line line(rbdata->p1,*p2);
       if (line.getdefined()) line.draw(DM_INVERT);
    }
  else
    {  Line line1(rbdata->p1,rbdata->p2);
       if (line1.getdefined()) line1.draw(DM_INVERT);
       Line line2(rbdata->p2,*p2);
       if (line2.getdefined()) line2.draw(DM_INVERT);
    }
#endif
}
#endif


/**
 * @brief measure_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Measure Angle command
 *
 * KMJ : working in Qt
 *
 */
void measure_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBMeasure2_data *rbdata = (RBMeasure2_data *) *data;
 EntityList list;
 double angle1,angle2,distance,cosa;
 Point dir1,dir2,dir3,dir4,normal,p3,p4;
 Entity *e1,*e2;
  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       state.destroyselection(1);
       if ((rbdata = new RBMeasure2_data) == NULL)
         {  state.fail(Memory,"Measure command 2");
            *cstate = ExitState;
         }
       else if ((rbdata->dialog = new Dialog("MeasureAngle_Dialog",1)) == 0)
         {  state.fail(Memory,"Measure command 2");
            *cstate = ExitState;
         }
       else
         {
            rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;
            if (list.length() == 2)
              {  list.start();
                 e1 = list.next();  e2 = list.next();
                 if (e1->isa(line_entity) && e2->isa(line_entity))
                   {  dir1 = ((Line *)e1)->direction(0.0).normalize();
                      dir2 = ((Line *)e2)->direction(0.0).normalize();
                      cosa = dir1.dot(dir2);
                      if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                      rbdata->x1 = acos(cosa) / M_PI * 180.0;
                      rbdata->x2 = 180.0 - rbdata->x1;
                      if (fabs(cosa) > cos(0.0001))
                        rbdata->x3 = (((Line *)e1)->getp1() - ((Line *)e2)->nearp(((Line *)e1)->getp1())).length();
                      else
                        rbdata->x3 = (dir1.cross(dir2).normalize()).dot(((Line *)e1)->getp1() - ((Line *)e2)->getp1());
                   }
              }
            list.destroy();
            rbdata->dialog->add(rbdata->dc1 = new RealDialogControl(100,&rbdata->x1,DontScale));
            rbdata->dialog->add(rbdata->dc2 = new RealDialogControl(101,&rbdata->x2,DontScale));
            rbdata->dialog->add(rbdata->dc3 = new RealDialogControl(102,&rbdata->x3,ScaleByLength));
#ifdef USING_WIDGETS
            rbdata->qgi1=0; rbdata->qgi2=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
            RCHWND hwnd = GetFocus();
            if (rbdata->dialog->process())
              {  SetFocus(hwnd);
                 *cstate = 1;
              } 
            else
              *cstate = ExitState;
         }
       *data = rbdata;
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 2;
         }
       else if (event->isa(EntitySelected))
         {  rbdata->line = (Line *) ((EntitySelectedEvent *)event)->getentity();
            rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
            state.destroyselection(0);
            *cstate = 4;
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
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = 1;
       break;
     case 3 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  dir1 = (rbdata->p1 - rbdata->p2).normalize();
            dir2 = (((Coordinate3dEvent *) event)->getp() - rbdata->p2).normalize();
            cosa = dir1.dot(dir2);
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            angle1 = acos(cosa) / M_PI * 180.0;
            angle2 = 360.0 - angle1;
            rbdata->dc1->change(rbdata->dialog,angle1);
            rbdata->dc2->change(rbdata->dialog,angle2);
            rbdata->dc3->change(rbdata->dialog,0.0);
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = 1;
       break;
     case 4 :
       if (event->isa(EntitySelected))
         {  dir1 = rbdata->line->direction(0.0).normalize();
            dir2 = ((Line *) ((EntitySelectedEvent *) event)->getentity())->direction(0.0).normalize();
            cosa = dir1.dot(dir2);
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            angle1 = acos(cosa) / M_PI * 180.0;
            angle2 = 180.0 - angle1;
            if (fabs(cosa) > cos(0.0001))
              {  distance = (rbdata->line->getp1() - ((Line *) ((EntitySelectedEvent *) event)->getentity())->nearp(rbdata->line->getp1())).length();
                 if (angle1 > 90.0)
                   {double t;  t = angle1;  angle1 = angle2;  angle2 = t;
                   }
              }
            else
              {  normal = dir1.cross(dir2).normalize();
                 distance = normal.dot(((Line *) ((EntitySelectedEvent *) event)->getentity())->getp1() - rbdata->line->getp1());
                 Line line(rbdata->line->getp1()+normal*distance,rbdata->line->getp2()+normal*distance);
                 Intersect i(((EntitySelectedEvent *) event)->getentity(),&line);
                 if (i.nintersections() == 1)
                   {  dir3 = (rbdata->p1+normal*distance) - ((PointE *)i.intersection(0))->getp();
                      dir4 = ((EntitySelectedEvent *) event)->getp() - ((PointE *)i.intersection(0))->getp();
                      if (dir3.dot(dir1) < 0.0) dir1 = -dir1;
                      if (dir4.dot(dir2) < 0.0) dir2 = -dir2;
                      cosa = dir1.dot(dir2);
                      if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                      angle1 = acos(cosa) / M_PI * 180.0;
                      angle2 = 180.0 - angle1;
                   }
                 else
                   {  angle1 = angle2 = distance = -1.0;
                   }
              }
            rbdata->dc1->change(rbdata->dialog,angle1);
            rbdata->dc2->change(rbdata->dialog,angle2);
            rbdata->dc3->change(rbdata->dialog,fabs(distance));
            state.destroyselection(1);
            rbdata->line->unhighlight();
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  rbdata->line->unhighlight();  //  CCNEW
            *cstate = 1;
         }
       break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi1)
          cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi1));
      delete rbdata->qgi1; rbdata->qgi1=0;
      if(rbdata->qgi2)
          cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi2));
      delete rbdata->qgi2; rbdata->qgi2=0;
  }
#endif
#endif
  if (*cstate == 1)
    {
       state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt(NCMEAS+2);
    }
  else if (*cstate == 2)
    {
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBMeasure2,rbdata);
       cadwindow->prompt->normalprompt(NCMEAS+3);
       rbdata->state = 2;
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBMeasure2,rbdata);
       cadwindow->prompt->normalprompt(NCMEAS+4);
       rbdata->state = 3;
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(line_entity,end_list);
       cadwindow->prompt->normalprompt(NCMEAS+5);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         delete rbdata->dialog;
       delete rbdata;
    }
}

struct RBMeasure3_data
{
    double area,totarea;
   EntityList list;
   Dialog *dialog;
   RealDialogControl *dc1,*dc2;
};

/**
 * @brief measure_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Measure Area Command
 *
 * KMJ : working in Qt version
 */
void measure_command3(int *cstate,HighLevelEvent *event,void **data)
{RBMeasure3_data *rbdata = (RBMeasure3_data *) *data;
 double area;
 BitMask options(32);
 ResourceString rs9(NCMEAS+9),rs10(NCMEAS+10);
 Entity *e;

  switch (*cstate)
    {case InitialState :
       if ((rbdata = new RBMeasure3_data) == NULL)
         {  state.fail(Memory,"Measure command 3");
            *cstate = ExitState;
         }
       else if ((rbdata->dialog = new Dialog("MeasureArea_Dialog",1)) == 0)
         {  state.fail(Memory,"Measure command 3");
            *cstate = ExitState;
         }
       else
         {  *cstate = 1;
            rbdata->totarea = rbdata->area = area = 0.0;
            rbdata->list = state.getselection().copy();
            state.destroyselection(0);
            for (rbdata->list.start() ; (e = rbdata->list.next()) != 0 ; )
              if (e->issurface())
                {  area = 0.0;
                   ((SurfaceEntity *)e)->facet(options,0.0,100,100,0,areacallback,&area);
                   rbdata->totarea += area;
                }
              else
                {  cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
                   rbdata->list.unhighlight();
                   rbdata->list.destroy();
                }
            rbdata->area = area;
            rbdata->dialog->add(rbdata->dc1 = new RealDialogControl(100,&rbdata->area,ScaleByArea));
            rbdata->dialog->add(rbdata->dc2 = new RealDialogControl(101,&rbdata->totarea,ScaleByArea));
            RCHWND hwnd = GetFocus();
            if (! rbdata->dialog->process())
              *cstate = ExitState;
            else
              SetFocus(hwnd); 
         }
       *data = rbdata;
       break;
     case 1 :
       if (event->isa(EntitySelected))
         {  area = 0.0;
            ((SurfaceEntity *)((EntitySelectedEvent *)event)->getentity())->facet(options,0.0,100,100,0,areacallback,&area);
            rbdata->totarea += area;
            rbdata->dc1->change(rbdata->dialog,area);
            rbdata->dc2->change(rbdata->dialog,rbdata->totarea);
            rbdata->list.add(((EntitySelectedEvent *)event)->getentity());
            state.destroyselection(0);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(surface_mask,end_list);
       cadwindow->prompt->normalprompt(NCMEAS+6);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  rbdata->list.unhighlight();
            rbdata->list.destroy();
            delete rbdata->dialog;
         }
       delete rbdata;
    }
}


/**
 * @brief The RBMeasure4_data struct
 *
 * This structure is a merger of RBMeasure1 and RVMeasure2
 */
struct RBMeasure4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi,*qgi1,*qgi2;
#endif
   Point p1,p2;
   double totlength,totarea,area,length,scale,x1,x2,x3;
   int mode,state,nlength,narea,enablescale;
   EntityList list;
   Line *line;
   Dialog *dialog;
   DialogControl *dc[14]; // angle uses 2,3,4
   RCCHAR direction[300];
};

#ifdef NEW_RUBBERB
void RBMeasure4(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBMeasure4_data *rbdata = (RBMeasure4_data *) data;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if(rbdata->mode == 0)
  {
      // length and area stuff
      Line line(rbdata->p1,*p2);
      if (line.getdefined())
      {
          //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
          line.draw(DM_INVERT);
          //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
      }
  }
  else
  {
    // angle stuff
     if (rbdata->state == 2)
     {
          Line line(rbdata->p1,*p2);
          if (line.getdefined())
          {
              //line.qgi = rbdata->qgi1; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              //rbdata->qgi1 = line.qgi; rbdata->zValue = line.zValue;
          }
     }
     else
     {
          Line line1(rbdata->p1,rbdata->p2);
          if (line1.getdefined())
          {
              //line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
              line1.draw(DM_INVERT);
              //rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;
          }
          Line line2(rbdata->p2,*p2);
          if (line2.getdefined())
          {
              //line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
              line2.draw(DM_INVERT);
              //rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;
          }
     }
  }
}
#else
void RBMeasure4(int,void *data,Point *p2,View3dSurface *)
{
  RBMeasure4_data *rbdata = (RBMeasure4_data *) data;

  if(rbdata->mode == 0)
  {
      // length and area stuff
      Line line(rbdata->p1,*p2);
#ifdef USING_WIDGETS
      if (line.getdefined())
      {
          line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
          line.draw(DM_INVERT);
          rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
      }
#else
      if (line.getdefined()) line.draw(DM_INVERT);
#endif
  }
  else
  {
    // angle stuff
#ifdef USING_WIDGETS
     if (rbdata->state == 2)
     {
          Line line(rbdata->p1,*p2);
          if (line.getdefined())
          {
              line.qgi = rbdata->qgi1; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              rbdata->qgi1 = line.qgi; rbdata->zValue = line.zValue;
          }
     }
     else
     {
          Line line1(rbdata->p1,rbdata->p2);
          if (line1.getdefined())
          {
              line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
              line1.draw(DM_INVERT);
              rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;
          }
          Line line2(rbdata->p2,*p2);
          if (line2.getdefined())
          {
              line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
              line2.draw(DM_INVERT);
              rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;
          }
     }

#else
 RBMeasure2_data *rbdata = (RBMeasure2_data *) data;
  if (rbdata->state == 2)
    {  Line line(rbdata->p1,*p2);
       if (line.getdefined()) line.draw(DM_INVERT);
    }
  else
    {  Line line1(rbdata->p1,rbdata->p2);
       if (line1.getdefined()) line1.draw(DM_INVERT);
       Line line2(rbdata->p2,*p2);
       if (line2.getdefined()) line2.draw(DM_INVERT);
    }
#endif
  }
}
#endif

class ME4CheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     ME4CheckBoxDialogControl(int id,int *v) : CheckBoxDialogControl(id,v) {} ;
     int select(Dialog *dialog);
     void load(Dialog *dialog);
  };

void ME4CheckBoxDialogControl::load(Dialog *dialog)
{
  EnableWindow(GetDlgItem(dialog->gethdlg(),108),oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1114),oldvalue);
  CheckBoxDialogControl::load(dialog);
}

int ME4CheckBoxDialogControl::select(Dialog *dialog)
{
  EnableWindow(GetDlgItem(dialog->gethdlg(),108),! oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1114),! oldvalue);
  return CheckBoxDialogControl::select(dialog);
}

class ME4RadioButtonDialogControl : public RadioButtonDialogControl
{public:
    RBMeasure4_data *rbdata;
   ME4RadioButtonDialogControl(int i,int i1,int i2,int *v,RBMeasure4_data *rbd=0) :
      RadioButtonDialogControl(i,i1,i2,v) {rbdata=rbd;}
   void load(Dialog *);
   int select(Dialog *);
};

void ME4RadioButtonDialogControl::load(Dialog *dialog)
{
 int i;
  RadioButtonDialogControl::load(dialog);

  // enable/disable the scale controls
  EnableWindow(GetDlgItem(dialog->gethdlg(),107), *value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),108), *value == 0);
  WritePrivateProfileString("DialogSettings","MeasureMode",(value == 0 ? "0": "1"),home.getinifilename());

}

int ME4RadioButtonDialogControl::select(Dialog *dialog)
{
  int i,status,value;
  status = RadioButtonDialogControl::select(dialog);
  value = 0;
  currentvalue(dialog,&value);

  // enable/disable the scale controls
  EnableWindow(GetDlgItem(dialog->gethdlg(),107), value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),108), value == 0);
  WritePrivateProfileString("DialogSettings","MeasureMode",(value == 0 ? "0": "1"),home.getinifilename());

  // init for the length and area values
  if(rbdata != 0)
  {
      rbdata->area = rbdata->length = 0.0;
      rbdata->nlength = 0;
      rbdata->narea = 0;
      rbdata->totlength = 0.0;
      rbdata->totarea = 0.0;
      rbdata->scale = v.getreal("me::scale");
      rbdata->enablescale = v.getinteger("me::enablescale");
      strcpy(rbdata->direction,"");
      // init the angle stuff
      rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;
      rbdata->mode = value;
#ifdef USING_WIDGETS
      if(rbdata->qgi)
         cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
      if(rbdata->qgi1)
          cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi1));
      delete rbdata->qgi1; rbdata->qgi1=0;
      if(rbdata->qgi2)
          cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi2));
      delete rbdata->qgi2; rbdata->qgi2=0;
#endif
  }

  return status;
}

void measure_command4(int *cstate,HighLevelEvent *event,void **data)
{
 RBMeasure4_data *rbdata = (RBMeasure4_data *) *data;
 EntityList list;
 double angle1,angle2,distance,cosa;
 Point dir1,dir2,dir3,dir4,normal,p3,p4;
 Entity *e1,*e2;
 double length,area;
 ResourceString rs0(NCMEAS),rs1(NCMEAS+1),rs7(NCMEAS+7),rs8(NCMEAS+8);
 RCCHAR text[300],message[600];
 Entity *e;
 BitMask options(32);
 Point p;

  int measureMode = 0;// 0 = length and area; 1 = angle
  measureMode = GetPrivateProfileInt("DialogSettings","MeasureMode",0,home.getinifilename());

  switch (*cstate)
    {case InitialState :      
       if ((rbdata = new RBMeasure4_data) == NULL)
         {  state.fail(Memory,"Measure command 4");
            *cstate = ExitState;
         }
       //else if ((rbdata->dialog = new MDialog("MeasureLengthArea_Dialog")) == 0)
       else if ((rbdata->dialog = new MDialog((char*)"Measure_Dialog")) == 0)
         {  state.fail(Memory,"Measure command 4");
            *cstate = ExitState;
         }
       else
         {
           // init for the length and area values
           rbdata->area = rbdata->length = 0.0;
           rbdata->nlength = 0;
           rbdata->narea = 0;
           rbdata->totlength = 0.0;
           rbdata->totarea = 0.0;
           rbdata->scale = v.getreal("me::scale");
           rbdata->enablescale = v.getinteger("me::enablescale");
           strcpy(rbdata->direction,"");
           // init the angle stuff
           rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;

           if(measureMode == 0)
           {
                // length and area stuff
                *cstate = 1;
                rbdata->area = rbdata->length = 0.0;
                rbdata->nlength = 0;
                rbdata->narea = 0;
                rbdata->totlength = 0.0;
                rbdata->totarea = 0.0;
                rbdata->scale = v.getreal("me::scale");
                rbdata->enablescale = v.getinteger("me::enablescale");
                strcpy(rbdata->direction,"");
                area = length = 0.0;
                rbdata->list = state.getselection().copy();
                state.destroyselection(0);
                for (rbdata->list.start() ; (e = rbdata->list.next()) != 0 ; )
                  if (e->islinear())
                    {  length = ((LinearEntity *)e)->length();
                       rbdata->totlength += length;
                       rbdata->nlength++;
                       if (e->isa(line_entity))
                         {Point p;
                            p = ((Line *)e)->getp2() - ((Line *)e)->getp1();
                            if (rbdata->enablescale == 0)
                              p.string(rbdata->direction);
                            else
                              (p * rbdata->scale).string(rbdata->direction);
                         }
                       else
                         strcpy(rbdata->direction,"");
                    }
                  else if (e->issurface())
                    {  area = 0.0;
                       ((SurfaceEntity *)e)->facet(options,0.0,100,100,0,areacallback,&area);
                       strcpy(rbdata->direction,"");
                       rbdata->totarea += area;
                       rbdata->narea++;
                    }
                  else
                    {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONINFORMATION);
                       strcpy(rbdata->direction,"");
                       rbdata->list.unhighlight();
                       rbdata->list.destroy();
                    }
                rbdata->length = length;  rbdata->area = area;

                // init the angle stuff
                rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;
          }
           else if(measureMode == 1)
           {
               list = state.getselection().copy();
               state.destroyselection(1);
               // angle stuff
               rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;
               if (list.length() == 2)
                 {  list.start();
                    e1 = list.next();  e2 = list.next();
                    if (e1->isa(line_entity) && e2->isa(line_entity))
                      {  dir1 = ((Line *)e1)->direction(0.0).normalize();
                         dir2 = ((Line *)e2)->direction(0.0).normalize();
                         cosa = dir1.dot(dir2);
                         if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                         rbdata->x1 = acos(cosa) / M_PI * 180.0;
                         rbdata->x2 = 180.0 - rbdata->x1;
                         if (fabs(cosa) > cos(0.0001))
                           rbdata->x3 = (((Line *)e1)->getp1() - ((Line *)e2)->nearp(((Line *)e1)->getp1())).length();
                         else
                           rbdata->x3 = (dir1.cross(dir2).normalize()).dot(((Line *)e1)->getp1() - ((Line *)e2)->getp1());
                      }
                 }
               *cstate = 1;
               list.destroy();
           }
           // init for the length and area values
           /*
           rbdata->area = rbdata->length = 0.0;
           rbdata->nlength = 0;
           rbdata->narea = 0;
           rbdata->totlength = 0.0;
           rbdata->totarea = 0.0;
           rbdata->scale = v.getreal("me::scale");
           rbdata->enablescale = v.getinteger("me::enablescale");
           strcpy(rbdata->direction,"");
           // init the angle stuff
           rbdata->x1 = rbdata->x2 = rbdata->x3 = 0.0;
            */
            // dialog setup
            // mode switch
            rbdata->dialog->add(rbdata->dc[0] = new ME4RadioButtonDialogControl(120,120,121,&measureMode,rbdata));
            rbdata->dialog->add(rbdata->dc[1] = new ME4RadioButtonDialogControl(121,120,121,&measureMode,rbdata));
            // angle stuff
            rbdata->dialog->add(rbdata->dc[2] = new RealDialogControl(109,&rbdata->x1,DontScale));
            rbdata->dialog->add(rbdata->dc[3] = new RealDialogControl(110,&rbdata->x2,DontScale));
            rbdata->dialog->add(rbdata->dc[4] = new RealDialogControl(111,&rbdata->x3,ScaleByLength));

            // length and area stuff
            rbdata->dialog->add(rbdata->dc[5] = new IntegerDialogControl(100,&rbdata->nlength));
            rbdata->dialog->add(rbdata->dc[6] = new RealDialogControl(101,&rbdata->length,ScaleByLength));
            rbdata->dialog->add(rbdata->dc[7] = new RealDialogControl(102,&rbdata->totlength,ScaleByLength));
            rbdata->dialog->add(rbdata->dc[8] = new IntegerDialogControl(103,&rbdata->narea));
            rbdata->dialog->add(rbdata->dc[9] = new RealDialogControl(104,&rbdata->area,ScaleByArea));
            rbdata->dialog->add(rbdata->dc[10] = new RealDialogControl(105,&rbdata->totarea,ScaleByArea));
            rbdata->dialog->add(rbdata->dc[11] = new StringDialogControl(106,rbdata->direction,300));
            rbdata->dialog->add(rbdata->dc[12] = new ME4CheckBoxDialogControl(107,&rbdata->enablescale));
            rbdata->dialog->add(rbdata->dc[13] = new RealDialogControl(108,&rbdata->scale));
            //rbdata->dialog->add(rbdata->dc[9] = new ScrollBarDialogControl(1114,rbdata->dc[8]));
            // dummy cancel button
            //rbdata->dialog->add(rbdata->dc[13] = new ButtonDialogControl(200));

#ifdef USING_WIDGETS
            rbdata->qgi=0;
            rbdata->qgi1=0;
            rbdata->qgi2=0;
            rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
            rbdata->mode = measureMode;
#endif

            RCHWND hwnd = GetFocus();

            if (! rbdata->dialog->process())
              *cstate = ExitState;
            else
              {
#ifdef USING_WIDGETS
                 QPoint winpos(((QtMainWindow*)cadwindow->gethwnd())->pos());
                 ((QDialog*)rbdata->dialog->gethdlg())->move(winpos.x(),winpos.y());
#endif
                 SetFocus(hwnd);
                 if (v.getinteger("db::hidemeasuredialog") != 0)
                   ShowWindow(rbdata->dialog->gethdlg(),SW_HIDE);
                 else
                   ShowWindow(rbdata->dialog->gethdlg(),SW_SHOW);
                 WritePrivateProfileString("DialogSettings","MeasureMode",(measureMode == 0 ? "0": "1"),home.getinifilename());
              }

         }
       *data = rbdata;
       break;
     case 1 :
       if(measureMode == 1)
       {
           if (event->isa(Coordinate3d))
             {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
                *cstate = 2;
             }
           else if (event->isa(EntitySelected))
             {  rbdata->line = (Line *) ((EntitySelectedEvent *)event)->getentity();
                rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
                state.destroyselection(0);
                *cstate = 4;
             }
           else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;
           break;
       }
       else if (measureMode == 0)
       {
           if (event->isa(Coordinate3d))
             {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
                *cstate = 2;
             }
           else if (event->isa(EntitySelected))
             {  length = area = 0.0;
                ((CheckBoxDialogControl *)rbdata->dc[12])->currentvalue(rbdata->dialog,&rbdata->enablescale);
                if (rbdata->enablescale == 0)
                  rbdata->scale = 1.0;
                else
                  ((RealDialogControl *)rbdata->dc[13])->currentvalue(rbdata->dialog,&rbdata->scale);
                if (((EntitySelectedEvent *) event)->getentity()->islinear())
                  {  length = ((LinearEntity *)((EntitySelectedEvent *) event)->getentity())->length();
                     rbdata->totlength += length;
                     rbdata->nlength++;
                     if (((EntitySelectedEvent *) event)->getentity()->isa(line_entity))
                       {Point p;
                          p = ((Line *)((EntitySelectedEvent *) event)->getentity())->getp2() - ((Line *)((EntitySelectedEvent *) event)->getentity())->getp1();
                          (p*rbdata->scale).string(rbdata->direction);
                       }
                     else
                       strcpy(rbdata->direction,"");
                  }
                else
                  {  area = 0.0;
                     ((SurfaceEntity *)((EntitySelectedEvent *)event)->getentity())->facet(options,0.0,100,100,0,areacallback,&area);
                     rbdata->totarea += area;
                     rbdata->narea++;
                     strcpy(rbdata->direction,"");
                  }
                ((IntegerDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->nlength);
                ((RealDialogControl *)rbdata->dc[6])->change(rbdata->dialog,length*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[7])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
                ((IntegerDialogControl *)rbdata->dc[8])->change(rbdata->dialog,rbdata->narea);
                ((RealDialogControl *)rbdata->dc[9])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[10])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
                ((StringDialogControl *)rbdata->dc[11])->change(rbdata->dialog,rbdata->direction);

                rbdata->list.add(((EntitySelectedEvent *) event)->getentity());
                state.destroyselection(0);
             }
           else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;
           break;
       }
     case 2 :
      if(measureMode == 1)
      {
          rubberband.end(0);
          if (event->isa(Coordinate3d))
            {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
               *cstate = 3;
            }
          else if (event->isa(Abort) || event->isa(Exit))
            *cstate = 1;
          break;
      }
      else if(measureMode == 0)
      {
           rubberband.end(0);
           if (event->isa(Coordinate3d))
             {Point p;

                ((CheckBoxDialogControl *)rbdata->dc[12])->currentvalue(rbdata->dialog,&rbdata->enablescale);
                if (rbdata->enablescale == 0)
                  rbdata->scale = 1.0;
                else
                  ((RealDialogControl *)rbdata->dc[13])->currentvalue(rbdata->dialog,&rbdata->scale);

                area = 0.0;
                p = ((Coordinate3dEvent *) event)->getp() - rbdata->p1;
                length = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).length();
                (p*rbdata->scale).string(rbdata->direction);

                rbdata->totlength += length;
                rbdata->nlength++;
                ((IntegerDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->nlength);
                ((RealDialogControl *)rbdata->dc[6])->change(rbdata->dialog,length*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[7])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
                ((IntegerDialogControl *)rbdata->dc[8])->change(rbdata->dialog,rbdata->narea);
                ((RealDialogControl *)rbdata->dc[9])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[10])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
                ((StringDialogControl *)rbdata->dc[11])->change(rbdata->dialog,rbdata->direction);
                *cstate = 1;
             }
           else if (event->isa(EntitySelected))
             {  ((CheckBoxDialogControl *)rbdata->dc[12])->currentvalue(rbdata->dialog,&rbdata->enablescale);
                if (rbdata->enablescale == 0)
                  rbdata->scale = 1.0;
                else
                  ((RealDialogControl *)rbdata->dc[13])->currentvalue(rbdata->dialog,&rbdata->scale);

                area = 0.0;
                p = ((EntitySelectedEvent *) event)->getentity()->nearp(rbdata->p1) - rbdata->p1;
                length = (((EntitySelectedEvent *) event)->getentity()->nearp(rbdata->p1) - rbdata->p1).length();
                (p*rbdata->scale).string(rbdata->direction);
                rbdata->totlength += length;
                rbdata->nlength++;
                ((IntegerDialogControl *)rbdata->dc[5])->change(rbdata->dialog,rbdata->nlength);
                ((RealDialogControl *)rbdata->dc[6])->change(rbdata->dialog,length*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[7])->change(rbdata->dialog,rbdata->totlength*rbdata->scale);
                ((IntegerDialogControl *)rbdata->dc[8])->change(rbdata->dialog,rbdata->narea);
                ((RealDialogControl *)rbdata->dc[9])->change(rbdata->dialog,area*rbdata->scale*rbdata->scale);
                ((RealDialogControl *)rbdata->dc[10])->change(rbdata->dialog,rbdata->totarea*rbdata->scale*rbdata->scale);
                ((StringDialogControl *)rbdata->dc[11])->change(rbdata->dialog,rbdata->direction);

                *cstate = 1;
                state.destroyselection(1);
             }
           else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;
           break;
      }
    case 3 :
        rubberband.end(0);
        if (event->isa(Coordinate3d))
          {  dir1 = (rbdata->p1 - rbdata->p2).normalize();
             dir2 = (((Coordinate3dEvent *) event)->getp() - rbdata->p2).normalize();
             cosa = dir1.dot(dir2);
             if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
             angle1 = acos(cosa) / M_PI * 180.0;
             angle2 = 360.0 - angle1;

             ((RealDialogControl*)rbdata->dc[2])->change(rbdata->dialog,angle1);
             ((RealDialogControl*)rbdata->dc[3])->change(rbdata->dialog,angle2);
             ((RealDialogControl*)rbdata->dc[4])->change(rbdata->dialog,0.0);
             *cstate = 1;
          }
        else if (event->isa(Abort) || event->isa(Exit))
          *cstate = 1;
        break;
    case 4 :
        if (event->isa(EntitySelected))
          {  dir1 = rbdata->line->direction(0.0).normalize();
             dir2 = ((Line *) ((EntitySelectedEvent *) event)->getentity())->direction(0.0).normalize();
             cosa = dir1.dot(dir2);
             if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
             angle1 = acos(cosa) / M_PI * 180.0;
             angle2 = 180.0 - angle1;
             if (fabs(cosa) > cos(0.0001))
               {  distance = (rbdata->line->getp1() - ((Line *) ((EntitySelectedEvent *) event)->getentity())->nearp(rbdata->line->getp1())).length();
                  if (angle1 > 90.0)
                    {double t;  t = angle1;  angle1 = angle2;  angle2 = t;
                    }
               }
             else
               {  normal = dir1.cross(dir2).normalize();
                  distance = normal.dot(((Line *) ((EntitySelectedEvent *) event)->getentity())->getp1() - rbdata->line->getp1());
                  Line line(rbdata->line->getp1()+normal*distance,rbdata->line->getp2()+normal*distance);
                  Intersect i(((EntitySelectedEvent *) event)->getentity(),&line);
                  if (i.nintersections() == 1)
                    {  dir3 = (rbdata->p1+normal*distance) - ((PointE *)i.intersection(0))->getp();
                       dir4 = ((EntitySelectedEvent *) event)->getp() - ((PointE *)i.intersection(0))->getp();
                       if (dir3.dot(dir1) < 0.0) dir1 = -dir1;
                       if (dir4.dot(dir2) < 0.0) dir2 = -dir2;
                       cosa = dir1.dot(dir2);
                       if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                       angle1 = acos(cosa) / M_PI * 180.0;
                       angle2 = 180.0 - angle1;
                    }
                  else
                    {  angle1 = angle2 = distance = -1.0;
                    }
               }
             ((RealDialogControl*)rbdata->dc[2])->change(rbdata->dialog,angle1);
             ((RealDialogControl*)rbdata->dc[3])->change(rbdata->dialog,angle2);
             ((RealDialogControl*)rbdata->dc[4])->change(rbdata->dialog,fabs(distance));
             state.destroyselection(1);
             rbdata->line->unhighlight();
             *cstate = 1;
          }
        else if (event->isa(Abort) || event->isa(Exit))
          {  rbdata->line->unhighlight();  //  CCNEW
             *cstate = 1;
          }
        break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(measureMode == 1)
  {
      if(rbdata)
      {
          if(rbdata->qgi1)
              cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi1));
          delete rbdata->qgi1; rbdata->qgi1=0;
          if(rbdata->qgi2)
              cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgi2));
          delete rbdata->qgi2; rbdata->qgi2=0;
      }
  }
#endif
#endif
  if (*cstate == 1)
    {
       if(measureMode == 1)
       {
           state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
           cadwindow->prompt->normalprompt(NCMEAS+2);
       }
       else if(measureMode == 0)
       {
           state.selmask.entity.clearandset(xyz_mask,linear_mask,surface_mask,end_list);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[5]->getid(),text,300);
           sprintf(message,"NL %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[6]->getid(),text,300);
           sprintf(message+strlen(message),"  L %s",text);
           sprintf(message+strlen(message),"  D %s",rbdata->direction);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[7]->getid(),text,300);
           sprintf(message+strlen(message),"  TL %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[8]->getid(),text,300);
           sprintf(message+strlen(message),"  NA %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[9]->getid(),text,300);
           sprintf(message+strlen(message),"  A %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[10]->getid(),text,300);
           sprintf(message+strlen(message),"  TA %s   ",text);
           strcpy(message,rs0.gets());
           cadwindow->prompt->normalprompt(message);
       }
    }
  else if (*cstate == 2)
    {
      if(measureMode == 1)
      {
          state.selmask.entity.clearandset(xyz_mask,end_list);
          rubberband.begin(0);
          rubberband.add(RBMeasure4,rbdata);
          cadwindow->prompt->normalprompt(NCMEAS+3);
          rbdata->state = 2;
      }
      else if(measureMode == 0)
      {
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[5]->getid(),text,300);
           sprintf(message,"NL %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[6]->getid(),text,300);
           sprintf(message+strlen(message),"  L %s",text);
           sprintf(message+strlen(message),"  D %s",rbdata->direction);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[7]->getid(),text,300);
           sprintf(message+strlen(message),"  TL %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[8]->getid(),text,300);
           sprintf(message+strlen(message),"  NA %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[9]->getid(),text,300);
           sprintf(message+strlen(message),"  A %s",text);
           GetDlgItemText(rbdata->dialog->gethdlg(),rbdata->dc[10]->getid(),text,300);
           sprintf(message+strlen(message),"  TA %s   ",text);
           strcpy(message,rs1.gets());
           cadwindow->prompt->normalprompt(message);

           state.selmask.entity.clearandset(xyz_mask,linear_mask,end_list);
           rubberband.begin(0);
           rubberband.add(RBMeasure4,rbdata);
      }
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBMeasure4,rbdata);
       cadwindow->prompt->normalprompt(NCMEAS+4);
       rbdata->state = 3;
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(line_entity,end_list);
       cadwindow->prompt->normalprompt(NCMEAS+5);
    }
  else if (*cstate == ExitState)
    {
      if (rbdata != 0)
         {  rbdata->list.unhighlight();
            rbdata->list.destroy();
            delete rbdata->dialog;
         }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
      if(measureMode == 0)
      {
        if(rbdata && rbdata->qgi)
        {
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
           delete rbdata->qgi; rbdata->qgi=0;
        }
      }
#endif
#endif
       delete rbdata;
       rbdata = 0;
    }

#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
      if(measureMode == 0)
      {
        if(rbdata && rbdata->qgi)
        {
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
           delete rbdata->qgi; rbdata->qgi=0;
        }
      }
#endif
#endif
}
