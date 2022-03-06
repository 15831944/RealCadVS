
#include "ncwin.h"

void point_command0(int *cstate,HighLevelEvent *,void **)
{
  cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Point"),buttonmenus.match("Tool"));
  *cstate = ExitState;
}

struct DBPoint1_data
{
    View3dSurface *lastv;
    qreal zValue;
    QGraphicsItem *qgi;
    Entity *e;
};

#ifdef NEW_RUBBERB
void RBPoint1(int mode,void *data,Point *p2,View3dSurface *vs)
{
  DBPoint1_data *rbdata = (DBPoint1_data *) data;

#if 0
  if(vs && vs != rbdata->lastv )
  {
      cadwindow->clearAllRubberBands();
      rbdata->qgi=0;
      rbdata->lastv = vs;
  }
#endif

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      rbdata->qgi=0;
  }
  else if (rbdata->e == NULL)
  {
      PointE point(*p2);
      point.draw(DM_INVERT);
  }
  else
  {
       PointE point(rbdata->e->nearp(*p2));
       point.draw(DM_INVERT);
  }

  rbdata->lastv = vs;
}
#else
void RBPoint1(int mode,void *data,Point *p2,View3dSurface *vs)
{
  DBPoint1_data *rbdata = (DBPoint1_data *) data;

  if (rbdata->e == NULL)
  {      
      PointE point(*p2);
      point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
      point.draw(DM_INVERT);
      rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
  }
  else
  {
       PointE point(rbdata->e->nearp(*p2));
       point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
       point.draw(DM_INVERT);
       rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
  }
  rbdata->lastv = vs;
}
#endif

void point_command1(int *cstate,HighLevelEvent *event,void **data)
{DBPoint1_data *rbdata = (DBPoint1_data *) *data;
 Entity *e;
 Point p1,p2;
 double d,dmin;
 int j;
 ResourceString rs0(NCPOINTB),rs1(NCPOINTB+1);

 // debug
 //PointE *dbpt = new PointE(Point(0,0,0));
 //db.geometry.add(dbpt);
 //return;
 //

 switch (*cstate)
    {  case InitialState :
       rbdata = new DBPoint1_data;  *data = rbdata;  rbdata->e = NULL;
       rbdata->qgi=0; rbdata->zValue=QGMAXZVALUE+QGZVALINC;
       rbdata->lastv=0;
         *cstate = 1;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {
             PointE *point = new PointE(((Coordinate3dEvent *) event)->getp());
             point->zValue=rbdata->zValue;
             db.geometry.add(point);
           }
         else if (event->isa(EntitySelected))
         {
              state.destroyselection(0);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              *cstate = 2;
         }
         else if (event->isa(Abort) || event->isa(Exit))
         {

             *cstate = ExitState;
         }
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {
              PointE *point = new PointE(rbdata->e->nearp(((Coordinate3dEvent *) event)->getp()));
              point->zValue=rbdata->zValue;
              db.geometry.add(point);
           }
         else if (event->isa(EntitySelected))
           {  cadwindow->cursoroff();
              state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p1 = ((EntitySelectedEvent *) event)->getp();
              Intersect i(rbdata->e,e);
              for (j = 0,dmin = -1.0 ; j < i.nintersections() ; j++)
                if (i.intersection(j)->isa(point_entity))
                  {  d = (((PointE *)i.intersection(j))->getp() - p1).length();
                     if (j == 0 || d < dmin)
                       {  dmin = d;  p2 = ((PointE *)i.intersection(j))->getp();
                       }
                  }
              if (dmin >= 0.0)
                {
                   PointE *point = new PointE(p2);
                   point->zValue=rbdata->zValue;
                   db.geometry.add(point);
                }
           } 
         else if (event->isa(Abort))
           {  *cstate = 1;
              rbdata->e->unhighlight();
              rbdata->e = NULL;
           }
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
             // clean up the rubberband
#ifndef NEW_RUBBERB
             if(rbdata->qgi && rbdata->qgi->scene())
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
             delete rbdata->qgi;
             rbdata->qgi = 0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,surface_mask,end_list);
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBPoint1,rbdata);
       cadwindow->prompt->normalprompt(rs0.gets());
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBPoint1,rbdata);
       cadwindow->prompt->normalprompt(rs1.gets());
    }
  else if (*cstate == ExitState)
    {
      if (rbdata->e != NULL) rbdata->e->unhighlight();
      delete rbdata;
    }
}

struct DBPoint2_data
{
   View3dSurface *lastv;
   qreal zValue;
   QVector<QGraphicsItem*> qgiVector;
   int n,spacingmode;
   double spacing;
   Point p;
};

#ifdef NEW_RUBBERB
void RBPoint2(int mode,void *vrbdata,Point *p2,View3dSurface *v)
{
  DBPoint2_data *rbdata = (DBPoint2_data *) vrbdata;
  double x,t;
  Point d,p;

  if (rbdata->n < 2) return;

#if 0
  if(v && v != rbdata->lastv )
  {
      cadwindow->clearAllRubberBands();
      rbdata->lastv = v;
  }
#endif

  Line line(rbdata->p,*p2);
  if (rbdata->spacingmode == 1)
    rbdata->spacing = line.length() / (rbdata->n-1);

  int id=0;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
  }
  else if(v != 0)
  {
      for (x = 0.0 ;   x <= line.length() + 1.0E-10 && rbdata->spacing > 0.0 ; x += rbdata->spacing)
      {
           PointE point(line.positionbylength(x,&t));
           point.draw(DM_INVERT);
      }
  }

  rbdata->lastv = v;
}
#else
void RBPoint2(int mode,void *vrbdata,Point *p2,View3dSurface *v)
{
  DBPoint2_data *rbdata = (DBPoint2_data *) vrbdata;
  double x,t;
  Point d,p;

  if (rbdata->n < 2) return;

  Line line(rbdata->p,*p2);
  if (rbdata->spacingmode == 1)
    rbdata->spacing = line.length() / (rbdata->n-1);

  int id=0;
  for (x = 0.0 ;   x <= line.length() + 1.0E-10 && rbdata->spacing > 0.0 ; x += rbdata->spacing)
  {
       PointE point(line.positionbylength(x,&t));
       if(id < rbdata->qgiVector.size())
       {

           point.qgi = rbdata->qgiVector.at(id); point.zValue = rbdata->zValue;
           point.draw(DM_INVERT);
           rbdata->qgiVector.replace(id,point.qgi); rbdata->zValue = point.zValue;

       }
       else
       {
           point.draw(DM_INVERT);
           rbdata->qgiVector.append(point.qgi);
       }
       id++;
  }
  for(int i=id; i<rbdata->qgiVector.size(); i++)
  {
      if(rbdata->qgiVector.at(i) && rbdata->qgiVector.at(i)->scene())
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector.at(i));
      delete rbdata->qgiVector.at(i);
      rbdata->qgiVector[i] = 0;
  }
/*
  if(rbdata->spacingmode == 0)
  {
      int id=0;
      for (x = 0.0 ;   x <= line.length() + 1.0E-10 && rbdata->spacing > 0.0 ; x += rbdata->spacing)
      {
           PointE point(line.positionbylength(x,&t));
           if(id < rbdata->qgiVector.size())
           {
               point.qgi = rbdata->qgiVector.at(id);
               point.draw(DM_INVERT);
               rbdata->qgiVector.replace(id,point.qgi);
           }
           else
           {
               point.draw(DM_INVERT);
               rbdata->qgiVector.append(point.qgi);
           }
           id++;
      }
      for(int i=id; i<rbdata->qgiVector.size(); i++)
      {
          if(rbdata->qgiVector.at(i))
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector.at(i));
      }
  }
  else
  {
      if(rbdata->qgiVector.size() == 0)
      {
          for (x = 0.0 ;   x <= line.length() + 1.0E-10 && rbdata->spacing > 0.0 ; x += rbdata->spacing)
          {
               PointE point(line.positionbylength(x,&t));
               point.draw(DM_INVERT);
               rbdata->qgiVector.append(point.qgi);
          }
      }
      else
      {
          int id=0;
          for (x = 0.0 ;   x <= line.length() + 1.0E-10 && rbdata->spacing > 0.0 ; x += rbdata->spacing)
          {
               PointE point(line.positionbylength(x,&t));
               if(id < rbdata->qgiVector.size())
               {
                   point.qgi = rbdata->qgiVector.at(id);
                   point.draw(DM_INVERT);
                   rbdata->qgiVector.replace(id,point.qgi);
                   id++;
               }
          }
      }
  }
*/
}
#endif

class GPRadioButtonDialogControl : public RadioButtonDialogControl
{public:
   GPRadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void GPRadioButtonDialogControl::load(Dialog *dialog)
{
  int i;
  RadioButtonDialogControl::load(dialog);
  for (i = 104 ; i <= 104 ; i++)
  {
      EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
      EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
  }
  for (i = 105 ; i <= 105 ; i++)
  {
      EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
      EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
  }
}

int GPRadioButtonDialogControl::select(Dialog *dialog)
{
  int i;
  RadioButtonDialogControl::select(dialog);
  for (i = 104 ; i <= 104 ; i++)
  {
      EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
      EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
  }
  for (i = 105 ; i <= 105 ; i++)
  {
      EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
      EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
  }
  return 0;
}

void point_command2(int *cstate,HighLevelEvent *event,void **data)
{
 DBPoint2_data *rbdata = (DBPoint2_data *) *data;
 ResourceString rs2(NCPOINTB+2),rs3(NCPOINTB+3),rs4(NCPOINTB+4),rs5(NCPOINTB+5);
 ResourceString rs6(NCPOINTB+6),rs7(NCPOINTB+7);
 EntityList list;
 int extra;
 Dialog dialog("GeneratePoints_Dialog");
 DialogControl *dc;
 Entity *e;
 double x,t;
 
  switch (*cstate)
    {  case InitialState :
         rbdata = new DBPoint2_data;  *data = rbdata;
         if (rbdata == NULL)
           *cstate = ExitState;
         else
         {
              rbdata->spacingmode = v.getinteger("pt::spacingmode");
              rbdata->n = v.getinteger("pt::n");
              rbdata->spacing = v.getreal("pt::spacing");
              rbdata->lastv=0;
              rbdata->qgiVector.clear();rbdata->zValue=QGMAXZVALUE+QGZVALINC;

              dialog.add(new GPRadioButtonDialogControl(102,102,103,&rbdata->spacingmode));
              dialog.add(new GPRadioButtonDialogControl(103,102,103,&rbdata->spacingmode));
              dialog.add(dc = new RealDialogControl(104,&rbdata->spacing,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1104,dc));
              dialog.add(dc = new IntegerDialogControl(105,&rbdata->n));
              //dialog.add(new ScrollBarDialogControl(1105,dc));

              if (dialog.process() == TRUE)
              {
                   v.setreal("pt::spacing",rbdata->spacing);
                   //qDebug() << "pt::spacing : " << rbdata->spacing;
                   v.setinteger("pt::n",rbdata->n);
                   v.setinteger("pt::spacingmode",rbdata->spacingmode);
                 
                   *cstate = 1;
                   list = state.getselection().copy();
                   state.destroyselection(1);
                   for (list.start() ; (e = list.next()) != 0 ; )
                     if (e->isa(line_entity) || e->isa(circle_entity) || e->isa(curve_entity))
                     {
                         double t;
                          extra = (((LinearEntity *)e)->start() - ((LinearEntity *)e)->end()).length() < db.getptoler();
                           
                          if (rbdata->spacingmode == 1)
                            rbdata->spacing = ((LinearEntity *)e)->length() / (rbdata->n-1+extra);


                          db.saveundo(UD_STARTBLOCK,NULL);
                          for (x = 0.0 ;   x <= ((LinearEntity *)e)->length() + 1.0E-10 ; x += rbdata->spacing)
                            {  PointE *point = new PointE(((LinearEntity *)e)->positionbylength(x,&t));
                               point->zValue=rbdata->zValue;
                               db.geometry.add(point);
                            }
                          db.saveundo(UD_ENDBLOCK,NULL);

                     }
                     else
                     {
                         cadwindow->MessageBox(rs6.gets(),rs6.gets(),MB_ICONINFORMATION);
                          list.destroy();
                     }
              }
              else
                *cstate = ExitState;
         }
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Entity *e;  double t;
              e = ((EntitySelectedEvent *) event)->getentity();
              if (e->islinear())
                {  extra = (((LinearEntity *)e)->start() - ((LinearEntity *)e)->end()).length() < db.getptoler();
                   if (rbdata->spacingmode == 1)
                     rbdata->spacing = ((LinearEntity *)e)->length() / (rbdata->n-1+extra);

                   db.saveundo(UD_STARTBLOCK,NULL);
                   for (x = 0.0 ;   x <= ((LinearEntity *)e)->length() + 1.0E-10 ; x += rbdata->spacing)
                     {  if (((EntitySelectedEvent *)event)->getendselected() == 2)
                          {
                             PointE *point = new PointE(((LinearEntity *)e)->positionbylength(((LinearEntity *)e)->length() - x,&t));
                             point->zValue=rbdata->zValue;
                             db.geometry.add(point);
                          }
                        else
                          {
                             PointE *point = new PointE(((LinearEntity *)e)->positionbylength(x,&t));
                             point->zValue=rbdata->zValue;
                             db.geometry.add(point);
                          }
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);

                }
              state.destroyselection(1);
           }
         else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;
          break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {Point d,p;
              if (rbdata->n >= 2)
              {
                  Line line(rbdata->p,((Coordinate3dEvent *) event)->getp());
                   if (rbdata->spacingmode == 1)
                     rbdata->spacing = line.length() / (rbdata->n-1);

                   if(rbdata->spacing > 0 | rbdata->spacingmode == 0)
                   {
                       db.saveundo(UD_STARTBLOCK,NULL);
                       for (x = 0.0 ;   x <= line.length() + 1.0E-10 ; x += rbdata->spacing)
                         {  PointE *point = new PointE(line.positionbylength(x,&t));
                            point->zValue=rbdata->zValue;
                            db.geometry.add(point);
                         }
                       db.saveundo(UD_ENDBLOCK,NULL);
                   }
              }
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {Entity *e;  double t;
              e = ((EntitySelectedEvent *) event)->getentity();
              if (e->islinear())
                { if (rbdata->spacingmode == 1)
                     rbdata->spacing = ((LinearEntity *)e)->length() / rbdata->n;

                   db.saveundo(UD_STARTBLOCK,NULL);
                   for (x = 0.0 ; x <= ((LinearEntity *)e)->length() + 1.0E-10 ; x += rbdata->spacing)
                     {  if (((EntitySelectedEvent *)event)->getendselected() == 2)
                          {
                             PointE *point = new PointE(((LinearEntity *)e)->positionbylength(((LinearEntity *)e)->length() - x,&t));
                             point->zValue=rbdata->zValue;
                             db.geometry.add(point);
                          }
                        else
                          {
                             PointE *point = new PointE(((LinearEntity *)e)->positionbylength(x,&t));
                             point->zValue=rbdata->zValue;
                             db.geometry.add(point);
                          }
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }

              state.destroyselection(1);
              *cstate = 1;
           }
         else if (event->isa(Abort))
             *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             //cadwindow->clearAllRubberBands();
             //rbdata->lastv = 0;
#else
             for(int i=0; i<rbdata->qgiVector.size(); i++)
             {
                 if(rbdata->qgiVector.at(i))
                     rbdata->qgiVector.at(i)->scene()->removeItem(rbdata->qgiVector.at(i));
                 delete rbdata->qgiVector.at(i);
             }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(rs4.gets());
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBPoint2,rbdata);
       cadwindow->prompt->normalprompt(rs5.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

