
#include "ncwin.h"

struct RBLine6_data
{
#ifdef USING_WIDGETS
  qreal zValue;
  QVector<QGraphicsItem*> qgiVector1,qgiVector2,qgiVector3,qgiVector4;
  QGraphicsItem *qgiStart,*qgiEnd;
#endif
   int cloop,cstart,cend,nlines,offsetpos,count;
   double offsetd;
   Point p1,p2,pi1,pi2,plast,plast1,zaxis;
   Point *vlast,*v1,*v2,*v3,*v4;
};

int calculate_offset(Point p1,Point zaxis,double offsetd,double toffsetd,int nlines,Point dir1,Point dir2,Point *verts)
{
 Point n1,n2,n3;
 double cosa;
 int i;
  n1 = (zaxis.cross(dir1)).normalize();
  if (n1.length() < db.getptoler()) return 0;
  n2 = (zaxis.cross(dir2)).normalize();
  if (n2.length() < db.getptoler()) return 0;
  n3 = (n1 + n2).normalize();
  cosa = n1.dot(n2);
  if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
  cosa = cos(acos(cosa) / 2.0);
  if (fabs(cosa) < db.getatoler()) return 0;
  n3 /= fabs(cosa);
  for (i = 0 ; i < nlines ; i++)
    verts[i] = p1 + n3 * (toffsetd - i * offsetd);
  return 1;
}

int calculate_offsets(RBLine6_data *rbdata)
{
 double toffsetd;
 Point dir1,dir2;
 int i;
  switch (rbdata->offsetpos)
    {  case 0 : toffsetd = (rbdata->nlines - 1) * rbdata->offsetd;        break;
       case 1 : toffsetd = (rbdata->nlines - 1) * rbdata->offsetd / 2.0;  break;
       case 2 : toffsetd = 0.0;                                            break;
    }
  if (rbdata->count == 2)
    {  dir1 = (rbdata->p2-rbdata->p1).normalize();
       if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offsetd,
                              toffsetd,rbdata->nlines,dir1,dir1,rbdata->v1)) return 0;
       if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offsetd,
                              toffsetd,rbdata->nlines,dir1,dir1,rbdata->v2)) return 0;
    }
  else if (rbdata->count > 2)
    {  if (rbdata->cloop)
         {  dir1 = (rbdata->p1-rbdata->plast).normalize();
            dir2 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offsetd,
                                   toffsetd,rbdata->nlines,dir1,dir2,rbdata->v1)) return 0;
            dir1 = (rbdata->p2-rbdata->p1).normalize();
            dir2 = (rbdata->pi1-rbdata->p2).normalize();
            if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offsetd,
                                   toffsetd,rbdata->nlines,dir1,dir2,rbdata->v2)) return 0;
            dir1 = (rbdata->pi1-rbdata->p2).normalize();
            dir2 = (rbdata->pi2-rbdata->pi1).normalize();
            if (! calculate_offset(rbdata->pi1,rbdata->zaxis,rbdata->offsetd,
                                   toffsetd,rbdata->nlines,dir1,dir2,rbdata->v3)) return 0;
            if (rbdata->count == 3)
              {  for (i = 0 ; i < rbdata->nlines ; i++)
                   {  rbdata->v4[i] = rbdata->v1[i];
                      rbdata->vlast[i] = rbdata->v3[i];
                   }
              }
         }
       else
         {  dir1 = (rbdata->p1-rbdata->plast).normalize();
            dir2 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offsetd,
                                   toffsetd,rbdata->nlines,dir1,dir2,rbdata->v1)) return 0;
            dir1 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offsetd,
                                   toffsetd,rbdata->nlines,dir1,dir1,rbdata->v2)) return 0;
         }
    }
  return 1;
}

#ifdef NEW_RUBBERB
void RBLine6(int mode,void *data,Point *p2,View3dSurface *)
{
  RBLine6_data *rbdata = (RBLine6_data *) data;
  int i;
  rbdata->p2 = *p2;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (calculate_offsets(rbdata))
  {
      for (i = 0 ; i < rbdata->nlines ; i++)
      {
            Line line1(rbdata->v1[i],rbdata->v2[i]);
            if( i < rbdata->qgiVector1.size())
            {
               line1.draw(DM_INVERT);
            }
            else
            {
                line1.draw(DM_INVERT);
            }

            if (rbdata->count > 2)
            {
                 Line line1(rbdata->vlast[i],rbdata->v1[i]);

                 line1.draw(DM_INVERT);

                 if (rbdata->cloop)
                 {
                      Line line1(rbdata->v2[i],rbdata->v3[i]);

                      line1.draw(DM_INVERT);

                      if (rbdata->count > 3)
                      {
                          Line line1(rbdata->v3[i],rbdata->v4[i]);
                          line1.draw(DM_INVERT);
                      }
                 }
              }
       }
       if (! rbdata->cloop)
       {
            if (rbdata->count == 2 && rbdata->cstart)
            {
                 Line line1(rbdata->v1[0],rbdata->v1[rbdata->nlines-1]);
                 line1.draw(DM_INVERT);
            }
            if (rbdata->count == 3 && rbdata->cstart)
            {
                 Line line1(rbdata->vlast[0],rbdata->vlast[rbdata->nlines-1]);
                 line1.draw(DM_INVERT);
            }
            if (rbdata->cend)
            {
                 Line line1(rbdata->v2[0],rbdata->v2[rbdata->nlines-1]);
                 line1.draw(DM_INVERT);
            }
       }
  }
}
#else
void RBLine6(int,void *data,Point *p2,View3dSurface *)
{
  RBLine6_data *rbdata = (RBLine6_data *) data;
  int i;
  rbdata->p2 = *p2;
  if (calculate_offsets(rbdata))
  {
      for (i = 0 ; i < rbdata->nlines ; i++)
      {
            Line line1(rbdata->v1[i],rbdata->v2[i]);
            if( i < rbdata->qgiVector1.size())
            {
               line1.qgi = rbdata->qgiVector1.at(i); line1.zValue = rbdata->zValue;
               line1.draw(DM_INVERT);
               rbdata->qgiVector1.replace(i,line1.qgi); rbdata->zValue = line1.zValue;
            }
            else
            {
                line1.draw(DM_INVERT);
                rbdata->qgiVector1.append(line1.qgi);
            }

            if (rbdata->count > 2)
            {
                 Line line1(rbdata->vlast[i],rbdata->v1[i]);
                 if( i < rbdata->qgiVector2.size())
                 {
                    line1.qgi = rbdata->qgiVector2.at(i); line1.zValue = rbdata->zValue;
                    line1.draw(DM_INVERT);
                    rbdata->qgiVector2.replace(i,line1.qgi); rbdata->zValue = line1.zValue;
                 }
                 else
                 {
                     line1.draw(DM_INVERT);
                     rbdata->qgiVector2.append(line1.qgi);
                 }
                 if (rbdata->cloop)
                 {
                      Line line1(rbdata->v2[i],rbdata->v3[i]);
                      if( i < rbdata->qgiVector3.size())
                      {
                         line1.qgi = rbdata->qgiVector3.at(i);line1.zValue = rbdata->zValue;
                         line1.draw(DM_INVERT);
                         rbdata->qgiVector3.replace(i,line1.qgi);rbdata->zValue = line1.zValue;
                      }
                      else
                      {
                          line1.draw(DM_INVERT);
                          rbdata->qgiVector3.append(line1.qgi);
                      }
                      if (rbdata->count > 3)
                      {
                          Line line1(rbdata->v3[i],rbdata->v4[i]);
                          if( i < rbdata->qgiVector4.size())
                          {
                             line1.qgi = rbdata->qgiVector4.at(i);line1.zValue = rbdata->zValue;
                             line1.draw(DM_INVERT);
                             rbdata->qgiVector4.replace(i,line1.qgi);rbdata->zValue = line1.zValue;
                          }
                          else
                          {
                              line1.draw(DM_INVERT);
                              rbdata->qgiVector4.append(line1.qgi);
                          }
                      }
                 }
              }
       }
       if (! rbdata->cloop)
       {
            if (rbdata->count == 2 && rbdata->cstart)
            {
                 Line line1(rbdata->v1[0],rbdata->v1[rbdata->nlines-1]);
                 line1.qgi = rbdata->qgiStart;line1.zValue = rbdata->zValue;
                 line1.draw(DM_INVERT);
                 rbdata->qgiStart = line1.qgi;rbdata->zValue = line1.zValue;
            }
            if (rbdata->count == 3 && rbdata->cstart)
            {
                 Line line1(rbdata->vlast[0],rbdata->vlast[rbdata->nlines-1]);
                 line1.qgi = rbdata->qgiStart;line1.zValue = rbdata->zValue;
                 line1.draw(DM_INVERT);
                 rbdata->qgiStart = line1.qgi;rbdata->zValue = line1.zValue;
            }
            if (rbdata->cend)
            {
                 Line line1(rbdata->v2[0],rbdata->v2[rbdata->nlines-1]);
                 line1.qgi = rbdata->qgiEnd;line1.zValue = rbdata->zValue;
                 line1.draw(DM_INVERT);
                 rbdata->qgiEnd = line1.qgi;rbdata->zValue = line1.zValue;
            }
       }
  }
}
#endif

class LPCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
    LPCheckBoxDialogControl(int id,int *value) : CheckBoxDialogControl(id,value)  { }
    void load(Dialog *dialog);
    int select(Dialog *dialog);
  };


void LPCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),*ivalue == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),*ivalue == 0);
}

int LPCheckBoxDialogControl::select(Dialog *dialog)
{int status,value;
  status = CheckBoxDialogControl::select(dialog);
  value = 0;
  currentvalue(dialog,&value);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),value == 0);
  return status;
}

/**
 * @brief line_command6
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert parallel lines command
 *
 */
void line_command6(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine6_data *rbdata = (RBLine6_data *) *data;
 Dialog *dialog;
 DialogControl *dc;
 int i;
 Line *line;
 ResourceString rs12(NCLINEB+12),rs13(NCLINEB+13),rs14(NCLINEB+14);
  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBLine6_data) == NULL)
           {  state.fail(Memory,rs12.gets());
              *cstate = ExitState;
           }
         else
           {  *data = rbdata;
              rbdata->vlast = rbdata->v1 = rbdata->v2 = rbdata->v3 = rbdata->v4 = NULL;
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              if ((dialog = new Dialog("LineParallel_Dialog")) == NULL)
                {  state.fail(Memory,rs13.gets());
                   *cstate = ExitState;
                }
              else
                {  rbdata->nlines = v.getinteger("ln::nlines");
                   rbdata->cstart = v.getinteger("ln::cstart");
                   rbdata->cend = v.getinteger("ln::cend");
                   rbdata->offsetd = v.getreal("ln::offsetd");
                   rbdata->offsetpos = v.getinteger("ln::offsetpos");
                   rbdata->cloop = v.getinteger("ln::cloop");

                   rbdata->qgiVector1.clear();
                   rbdata->qgiVector2.clear();
                   rbdata->qgiVector3.clear();
                   rbdata->qgiVector4.clear();
                   rbdata->qgiStart=0;
                   rbdata->qgiEnd=0;
                   rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);

                   dialog->add(dc = new IntegerDialogControl(103,&rbdata->nlines,2,9999));
                   //dialog->add(new ScrollBarDialogControl(1103,dc));
                   dialog->add(dc = new RealDialogControl(104,&rbdata->offsetd,ScaleByLength));
                   //dialog->add(new ScrollBarDialogControl(1104,dc));
                   dialog->add(new LPCheckBoxDialogControl(105,&rbdata->cloop));
                   dialog->add(new CheckBoxDialogControl(106,&rbdata->cstart));
                   dialog->add(new CheckBoxDialogControl(107,&rbdata->cend));
                   dialog->add(new RadioButtonDialogControl(100,100,102,&rbdata->offsetpos));
                   dialog->add(new RadioButtonDialogControl(101,100,102,&rbdata->offsetpos));
                   dialog->add(new RadioButtonDialogControl(102,100,102,&rbdata->offsetpos));
                   if (dialog->process() == TRUE)
                     {  v.setinteger("ln::nlines",rbdata->nlines);
                        v.setinteger("ln::cstart",rbdata->cstart);
                        v.setinteger("ln::cend",rbdata->cend);
                        v.setreal("ln::offsetd",rbdata->offsetd);
                        v.setinteger("ln::offsetpos",rbdata->offsetpos);
                        v.setinteger("ln::cloop",rbdata->cloop);
                        rbdata->vlast = new Point[rbdata->nlines];
                        rbdata->v1 = new Point[rbdata->nlines];
                        rbdata->v2 = new Point[rbdata->nlines];
                        rbdata->v3 = new Point[rbdata->nlines];
                        rbdata->v4 = new Point[rbdata->nlines];
                        if (rbdata->vlast == NULL || rbdata->v1 == NULL || rbdata->v2 == NULL ||
                            rbdata->v3 == NULL || rbdata->v4 == NULL)
                          {  state.fail(Memory,rs14.gets());
                             *cstate = ExitState;
                          }
                        else
                          *cstate = 1;
                     }
                   else
                     *cstate = ExitState;
                   delete dialog;
                }
           }
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->pi1 = rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->zaxis = -((Coordinate3dEvent *) event)->getxaxis().cross(((Coordinate3dEvent *) event)->getyaxis());
              rbdata->count = 2;
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              if (rbdata->count == 2) rbdata->pi2 = rbdata->p2;
              if (calculate_offsets(rbdata))
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   if (! rbdata->cloop && rbdata->count == 2 && rbdata->cstart)
                     {  if ((line = new Line(rbdata->v1[0],rbdata->v1[rbdata->nlines-1])) != NULL)
                       {
                           line->zValue = rbdata->zValue;
                           db.geometry.add(line);
                       }
                     }
                   if (rbdata->cloop && rbdata->count > 3 ||
                       ! rbdata->cloop && rbdata->count > 2)
                     {  for (i = 0 ; i < rbdata->nlines ; i++)
                          if ((line = new Line(rbdata->vlast[i],rbdata->v1[i])) != NULL)
                          {
                              line->zValue = rbdata->zValue;
                              db.geometry.add(line);
                          }
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                   for (i = 0 ; i < rbdata->nlines ; i++)
                     {  rbdata->vlast[i] = rbdata->v1[i];
                        rbdata->v1[i] = rbdata->v2[i];
                     }
                   rbdata->count++;
                   if (rbdata->count > 3) rbdata->plast1 = rbdata->plast;
                   rbdata->plast = rbdata->p1;
                   rbdata->p1 = rbdata->p2;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  if (rbdata->cloop && rbdata->count > 3 ||
                  ! rbdata->cloop && rbdata->count > 2)
                {  rbdata->count--;
                   if (rbdata->cloop)
                     {  rbdata->p2 = rbdata->p1;  rbdata->p1 = rbdata->plast;  rbdata->plast = rbdata->plast1;
                     }
                   else
                     rbdata->p2 = rbdata->p1 + (rbdata->p1 - rbdata->plast);
                   if (calculate_offsets(rbdata))
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (i = 0 ; i < rbdata->nlines ; i++)
                          {  if (rbdata->cloop)
                               {  if ((line = new Line(rbdata->v1[i],rbdata->v2[i])) != NULL)
                                  {
                                    line->zValue = rbdata->zValue;
                                    db.geometry.add(line);
                                  }
                                  if ((line = new Line(rbdata->v2[i],rbdata->v3[i])) != NULL)
                                  {
                                      line->zValue = rbdata->zValue;
                                      db.geometry.add(line);
                                  }
                                  if (rbdata->count >= 3)
                                    {  if ((line = new Line(rbdata->v3[i],rbdata->v4[i])) != NULL)
                                       {
                                          line->zValue = rbdata->zValue;
                                          db.geometry.add(line);
                                       }
                                    }
                               }
                             else
                               {  if ((line = new Line(rbdata->vlast[i],rbdata->v1[i])) != NULL)
                                  {
                                    line->zValue = rbdata->zValue;
                                    db.geometry.add(line);
                                  }
                               }
                          }
                        if (! rbdata->cloop && rbdata->cend)
                          {  if ((line = new Line(rbdata->v1[0],rbdata->v1[rbdata->nlines-1])) != NULL)
                             {
                                line->zValue = rbdata->zValue;
                                db.geometry.add(line);
                             }
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                }
              if (event->isa(Abort))
                *cstate = 1;
              else
                *cstate = ExitState;
           }
         break;
     }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      for(int i=0; i<rbdata->qgiVector1.size(); i++)
      {
          if(rbdata->qgiVector1.at(i))
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector1.at(i));
          delete rbdata->qgiVector1.at(i);
      }
      rbdata->qgiVector1.clear();
      for(int i=0; i<rbdata->qgiVector2.size(); i++)
      {
          if(rbdata->qgiVector2.at(i))
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector2.at(i));
          delete rbdata->qgiVector2.at(i);
      }
      rbdata->qgiVector2.clear();
      for(int i=0; i<rbdata->qgiVector3.size(); i++)
      {
          if(rbdata->qgiVector3.at(i))
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector3.at(i));
          delete rbdata->qgiVector3.at(i);
      }
      rbdata->qgiVector3.clear();
      for(int i=0; i<rbdata->qgiVector4.size(); i++)
      {
          if(rbdata->qgiVector4.at(i))
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector4.at(i));
          delete rbdata->qgiVector4.at(i);
      }
      rbdata->qgiVector4.clear();
      if(rbdata->qgiStart)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiStart);
      delete rbdata->qgiStart;rbdata->qgiStart=0;
      if(rbdata->qgiEnd)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiEnd);
      delete rbdata->qgiEnd;rbdata->qgiEnd=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 2)
         {  rubberband.begin(0);
            rubberband.add(RBLine6,rbdata);
         }
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+15);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCLINEB+16);
  else if (*cstate == ExitState)
    {  if (rbdata != NULL)
         {  delete [] rbdata->v1;  delete [] rbdata->v2;  delete [] rbdata->v3;  delete [] rbdata->v4;  delete [] rbdata->vlast;  delete rbdata;
         }
    }
}

struct RBLine7_data
{
#ifdef USING_WIDGETS
     qreal zValue;
     QGraphicsItem *qgline,*qgi1,*qgi2,*qgi3,*qgi4;
#endif
     Point p1,p2,xaxis,yaxis;
     int state,mode,fixsize,hatch,corners;
     double width,height;
};

class RectCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     RectCheckBoxDialogControl(int id,int *v) : CheckBoxDialogControl(id,v) {}
     void load(Dialog *);
     int select(Dialog *);
  };

void RectCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),203),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),204),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),oldvalue);
}

int RectCheckBoxDialogControl::select(Dialog *dialog)
{int status;
  status = CheckBoxDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),203),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),204),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),oldvalue);
  return status;
}

class RectRadioButtonDialogControl : public RadioButtonDialogControl
  {public:
     RectRadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {}
     void load(Dialog *);
     int select(Dialog *);
  };

void RectRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue);
}

int RectRadioButtonDialogControl::select(Dialog *dialog)
{int status;
  status = RadioButtonDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue);
  return status;
}

#ifdef NEW_RUBBERB
void insert_rectangle(int insert,int hatch,Point p1,Point p2,Point p3,RBLine7_data *rbdata=0)
{
 EntityList list;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask options(32);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;

  if (insert)
    {ResourceString rs24(NCLINEB+24),rs25(NCLINEB+25);
       if ((p3 - p2).cross(p1 - p2).length() < 1.0E-10)
         cadwindow->MessageBox(rs24.gets(),rs25.gets(),MB_ICONINFORMATION);
       else
         {  Line *line1 = new Line(p1,p2);
            Line *line2 = new Line(p2,p3);
            Line *line3 = new Line(p3,p3+p1 - p2);
            Line *line4 = new Line(p3+p1-p2,p1);
            if (line1 != 0 && line2 != 0 && line3 != 0 && line4 != 0)
              {  db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(line1);line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line2);line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line3);line3->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line4);line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 if (hatch)
                   {  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                      Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                      Plane *plane = new Plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                      if (plane->getdefined())
                      {
                        plane->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                        db.geometry.add(plane);
                      }
                      else
                        delete plane;
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
         }
    }
  else
    {
       Line line1(p1,p2);
       //line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       //rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(p2,p3);
       //line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       //rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;

       Line line3(p3,p3+p1-p2); line3.zValue = rbdata->zValue;
       //line3.qgi = rbdata->qgi3;
       line3.draw(DM_INVERT);
       //rbdata->qgi3 = line3.qgi; rbdata->zValue = line3.zValue;

       Line line4(p3+p1-p2,p1);
       //line4.qgi = rbdata->qgi4; line4.zValue = rbdata->zValue;
       line4.draw(DM_INVERT);
       //rbdata->qgi4 = line4.qgi; rbdata->zValue = line4.zValue;
    }
}
#else
void insert_rectangle(int insert,int hatch,Point p1,Point p2,Point p3,RBLine7_data *rbdata=0)
{
 EntityList list;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask options(32);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;

  if (insert)
    {ResourceString rs24(NCLINEB+24),rs25(NCLINEB+25);
       if ((p3 - p2).cross(p1 - p2).length() < 1.0E-10)
         cadwindow->MessageBox(rs24.gets(),rs25.gets(),MB_ICONINFORMATION);
       else
         {  Line *line1 = new Line(p1,p2);
            Line *line2 = new Line(p2,p3);
            Line *line3 = new Line(p3,p3+p1 - p2);
            Line *line4 = new Line(p3+p1-p2,p1);
            if (line1 != 0 && line2 != 0 && line3 != 0 && line4 != 0)
              {  db.saveundo(UD_STARTBLOCK,NULL);
                 db.geometry.add(line1);line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line2);line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line3);line3->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 db.geometry.add(line4);line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                 if (hatch)
                   {  list.add(line1);  list.add(line2);  list.add(line3);  list.add(line4);
                      Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                      Plane *plane = new Plane(list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                      if (plane->getdefined())
                      {
                        plane->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                        db.geometry.add(plane);
                      }
                      else
                        delete plane;
                   }
                 db.saveundo(UD_ENDBLOCK,NULL);
              }
         }
    }
  else
    {
       Line line1(p1,p2);
       line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;


       Line line2(p2,p3);
       line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;

       Line line3(p3,p3+p1-p2); line3.zValue = rbdata->zValue;
       line3.qgi = rbdata->qgi3;
       line3.draw(DM_INVERT);
       rbdata->qgi3 = line3.qgi; rbdata->zValue = line3.zValue;

       Line line4(p3+p1-p2,p1);
       line4.qgi = rbdata->qgi4; line4.zValue = rbdata->zValue;
       line4.draw(DM_INVERT);
       rbdata->qgi4 = line4.qgi; rbdata->zValue = line4.zValue;
    }
}
#endif

#ifdef NEW_RUBBERB
void RBLine7(int dm,void *data,Point *p2,View3dSurface *v)
{RBLine7_data *rbdata = (RBLine7_data *) data;
 Point xaxis,yaxis,p3;

  if(dm == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if (rbdata->state == 1 && rbdata->fixsize)
  {
      if (dm == RB_DRAW)
      {
          if (v != 0)
          {
                 rbdata->xaxis = v->getxaxis();
                 rbdata->yaxis = v->getyaxis();
          }
          else
          {
                 rbdata->xaxis.setxyz(1.0,0.0,0.0);
                 rbdata->yaxis.setxyz(0.0,1.0,0.0);
          }
      }

       insert_rectangle(0,0,*p2,*p2+rbdata->xaxis*rbdata->width,*p2+rbdata->xaxis*rbdata->width+rbdata->yaxis*rbdata->height,rbdata);
  }
  else if (rbdata->state == 2)
  {
      if (rbdata->fixsize)
      {
            xaxis = (*p2 - rbdata->p1).normalize();
            yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis).normalize();
            insert_rectangle(0,0,rbdata->p1,rbdata->p1+xaxis*rbdata->width,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->height,rbdata);
      }
      else if (rbdata->mode == 0)
      {
         insert_rectangle(0,0,rbdata->p1,rbdata->p1+rbdata->xaxis*rbdata->xaxis.dot(*p2 - rbdata->p1),*p2,rbdata);
      }
      else
      {
          Line line(rbdata->p1,*p2);
          //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
          line.draw(DM_INVERT);
          //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
      }
  }
  else if (rbdata->state == 3)
  {
      if (rbdata->fixsize)
      {
            xaxis = (rbdata->p2 - rbdata->p1).normalize();
            yaxis = (*p2 - rbdata->p1).normalize();
            if (rbdata->corners)
              yaxis = (xaxis.cross(yaxis)).cross(xaxis).normalize();
            insert_rectangle(0,0,rbdata->p1,rbdata->p1+xaxis*rbdata->width,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->height,rbdata);
      }
      else
      {
          if (rbdata->corners)
          {
                 xaxis = (rbdata->p2 - rbdata->p1).normalize();
                 yaxis = (*p2 - rbdata->p1).normalize();
                 yaxis = (xaxis.cross(yaxis)).cross(xaxis);
                 Line line(rbdata->p2,rbdata->p2+yaxis);
                 p3 = line.nearp(*p2);
          }
          else
              p3 = *p2;

          insert_rectangle(0,0,rbdata->p1,rbdata->p2,p3,rbdata);
      }
  }
}
#else
void RBLine7(int dm,void *data,Point *p2,View3dSurface *v)
{RBLine7_data *rbdata = (RBLine7_data *) data;
 Point xaxis,yaxis,p3;
  if (rbdata->state == 1 && rbdata->fixsize)
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

       insert_rectangle(0,0,*p2,*p2+rbdata->xaxis*rbdata->width,*p2+rbdata->xaxis*rbdata->width+rbdata->yaxis*rbdata->height,rbdata);
    }
  else if (rbdata->state == 2)
    {  if (rbdata->fixsize)
       {
            xaxis = (*p2 - rbdata->p1).normalize();
            yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis).normalize();
            insert_rectangle(0,0,rbdata->p1,rbdata->p1+xaxis*rbdata->width,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->height,rbdata);
       }
       else if (rbdata->mode == 0)
       {
         insert_rectangle(0,0,rbdata->p1,rbdata->p1+rbdata->xaxis*rbdata->xaxis.dot(*p2 - rbdata->p1),*p2,rbdata);
       }
       else
       {
          Line line(rbdata->p1,*p2);
          line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
          line.draw(DM_INVERT);
          rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
       }
    }
  else if (rbdata->state == 3)
    {  if (rbdata->fixsize)
         {  xaxis = (rbdata->p2 - rbdata->p1).normalize();
            yaxis = (*p2 - rbdata->p1).normalize();
            if (rbdata->corners)
              yaxis = (xaxis.cross(yaxis)).cross(xaxis).normalize();
            insert_rectangle(0,0,rbdata->p1,rbdata->p1+xaxis*rbdata->width,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->height,rbdata);
         }
       else
         {  if (rbdata->corners)
              {  xaxis = (rbdata->p2 - rbdata->p1).normalize();
                 yaxis = (*p2 - rbdata->p1).normalize();
                 yaxis = (xaxis.cross(yaxis)).cross(xaxis);
                 Line line(rbdata->p2,rbdata->p2+yaxis);
                 p3 = line.nearp(*p2);
              }
            else
              p3 = *p2;
            insert_rectangle(0,0,rbdata->p1,rbdata->p2,p3,rbdata);
         }
    }
}
#endif

/**
 * @brief line_command7
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Rectangle command
 *
 */
void line_command7(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine7_data *rbdata = (RBLine7_data *) *data;
 Point xaxis,yaxis,p2,p3;
 ResourceString rs17(NCLINEB+17);
  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBLine7_data) == NULL)
           {  state.fail(Memory,rs17.gets());
              *cstate = ExitState;
           }
         else
           {Dialog dialog("Rectangle_Dialog");
            DialogControl *dc;
              rbdata->mode = v.getinteger("ln::rectmode");
              rbdata->fixsize = v.getinteger("ln::rectfixsize");
              rbdata->hatch = v.getinteger("ln::hatch");
              rbdata->height = v.getreal("ln::rectheight");
              rbdata->width = v.getreal("ln::rectwidth");
              rbdata->corners = v.getinteger("ln::corners");

              rbdata->qgline=0;rbdata->qgi1=0;rbdata->qgi2=0;rbdata->qgi3=0;rbdata->qgi4=0;
              rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);

              dialog.add(new RectRadioButtonDialogControl(100,100,101,&rbdata->mode));
              dialog.add(new RectRadioButtonDialogControl(101,100,101,&rbdata->mode));
              dialog.add(new RectCheckBoxDialogControl(102,&rbdata->fixsize));
              dialog.add(dc = new RealDialogControl(103,&rbdata->width,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              dialog.add(dc = new RealDialogControl(104,&rbdata->height,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1104,dc));
              dialog.add(new CheckBoxDialogControl(105,&rbdata->hatch));
              dialog.add(new CheckBoxDialogControl(106,&rbdata->corners));
              if (dialog.process() == TRUE)
               {  v.setinteger("ln::rectmode",rbdata->mode);
                  v.setinteger("ln::rectfixsize",rbdata->fixsize);
                  v.setinteger("ln::hatch",rbdata->hatch);
                  v.setreal("ln::rectheight",rbdata->height);
                  v.setreal("ln::rectwidth",rbdata->width);
                  v.setinteger("ln::corners",rbdata->corners);
                  *cstate = 1;
               }
             else
               *cstate = ExitState;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              if (rbdata->mode == 0 && rbdata->fixsize)
                insert_rectangle(1,rbdata->hatch,rbdata->p1,rbdata->p1+rbdata->xaxis*rbdata->width,rbdata->p1+rbdata->xaxis*rbdata->width+rbdata->yaxis*rbdata->height);
              else
                *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
             rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              if (rbdata->mode == 0)
                {  insert_rectangle(1,rbdata->hatch,rbdata->p1,
                                     rbdata->p1+rbdata->xaxis*rbdata->xaxis.dot(((Coordinate3dEvent *)event)->getp() - rbdata->p1),
                                     ((Coordinate3dEvent *)event)->getp());
                   *cstate = 1;
                }
              else
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
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              if (rbdata->fixsize)
                {  xaxis = (rbdata->p2 - rbdata->p1).normalize();
                   yaxis = (p2 - rbdata->p1).normalize();
                   if (rbdata->corners)
                     yaxis = (xaxis.cross(yaxis)).cross(xaxis).normalize();
                   insert_rectangle(1,rbdata->hatch,rbdata->p1,rbdata->p1+xaxis*rbdata->width,rbdata->p1+xaxis*rbdata->width+yaxis*rbdata->height);
                   *cstate = 1;
                }
              else
                {  if (rbdata->corners)
                     {  xaxis = (rbdata->p2 - rbdata->p1).normalize();
                        yaxis = (p2 - rbdata->p1).normalize();
                        yaxis = (xaxis.cross(yaxis)).cross(xaxis);
                        Line line(rbdata->p2,rbdata->p2+yaxis);
                        p3 = line.nearp(p2);
                     }
                   else
                     p3 = p2;
                   insert_rectangle(1,rbdata->hatch,rbdata->p1,rbdata->p2,p3);
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
  if (*cstate != ExitState)
    {  rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBLine7,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+19);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCLINEB+20);
  else if (*cstate == 3)
    cadwindow->prompt->normalprompt(NCLINEB+23);
  else if (*cstate == ExitState)
    delete rbdata;
}


class PolygonRadioButtonDialogControl : public RadioButtonDialogControl
  {public:
     PolygonRadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {}
     void load(Dialog *);
     int select(Dialog *);
  };

void PolygonRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),205),oldvalue == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue == 2);
  EnableWindow(GetDlgItem(dialog->gethdlg(),206),oldvalue == 2);
}

int PolygonRadioButtonDialogControl::select(Dialog *dialog)
{int status;
  status = RadioButtonDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),205),oldvalue == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue == 2);
  EnableWindow(GetDlgItem(dialog->gethdlg(),206),oldvalue == 2);
  return status;
}

struct RBLine8_data
  {
#ifdef USING_WIDGETS
    qreal zValue;
    QVector<QGraphicsItem*> qgiVector1; // bottom polygon
    QVector<QGraphicsItem*> qgiVector2; // middle polygon
    QVector<QGraphicsItem*> qgiVector3; // top polygon
    QVector<QGraphicsItem*> qgiVector4; // vertical edges
#endif
    Point origin,zaxis;
   int fixedsize,inside,n,hatch;
   double thickness,radius,chordlength;
  };


#ifdef NEW_RUBBERB
void create_polygon(int insert,int hatch,double thickness,Point origin,Point xaxis,Point yaxis,double radius,int n,RBLine8_data *rbdata=0)
{
 int i;
 double a;
 Point p3,p4,p5,p6;
 EntityList list1,list2,list3,elist;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask options(32);
 RCCHAR patternfilename[300];
 Line *line1,*line2,*line3,*line4;
 RCCOLORREF gradientcolour;

  if (insert)
    db.saveundo(UD_STARTBLOCK,NULL);

  for (i = 0 ; i <= n ; i++)
    {  a = double(i) / double(n) * 2.0 * M_PI;
       p4 = origin + xaxis * (radius * cos(a)) +
                     yaxis * (radius * sin(a));
       p6 = p4 + xaxis.cross(yaxis) * thickness;
       if (i > 0)
          {  if (insert)
               {  line1 = new Line(p3,p4);
                  if (line1 != NULL && line1->getdefined())
                    {
                       line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                       db.geometry.add(line1);
                       if (hatch)
                         list1.add(line1);
                    }
                  else
                    delete line1;
                  if (fabs(thickness) > db.getptoler())
                  {
                       line2 = new Line(p5,p6);
                       if (line2 != NULL && line2->getdefined())
                         {
                            line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                            db.geometry.add(line2);
                            if (hatch)
                              list2.add(line2);
                         }
                       else
                         delete line2;
                  }
               }
             else
               {
                  Line line1(p3,p4);
                  line1.draw(DM_INVERT);
                  /*
                  if (line1.getdefined())
                  {
                      if(i-1 < rbdata->qgiVector1.size())
                      {
                          if(rbdata)
                          {
                              line1.qgi = rbdata->qgiVector1.at(i-1);
                              line1.zValue = rbdata->zValue;
                          }
                          line1.draw(DM_INVERT);
                          if(rbdata)
                          {
                              rbdata->qgiVector1.replace(i-1,line1.qgi);
                              rbdata->zValue = line1.zValue;
                          }
                      }
                      else
                      {
                          line1.zValue=rbdata->zValue;
                          line1.draw(DM_INVERT);
                          rbdata->qgiVector1.append(line1.qgi);
                          rbdata->zValue = line1.zValue;
                      }
                  }
                  */
                  if (fabs(thickness) > db.getptoler())
                  {
                       Line line2(p5,p6);
                       Line line3((p3 + p5)/2.0,(p4 + p6) / 2.0);
                       if (line2.getdefined())
                       {
                           line2.draw(DM_INVERT);
                           /*
                           if(i-1 < rbdata->qgiVector2.size())
                           {
                               if(rbdata)
                               {
                                   line2.qgi = rbdata->qgiVector2.at(i-1);
                                   line2.zValue = rbdata->zValue;
                               }
                               line2.draw(DM_INVERT);
                               if(rbdata)
                               {
                                   rbdata->qgiVector2.replace(i-1,line2.qgi);
                                   rbdata->zValue = line2.zValue;
                               }
                           }
                           else
                           {
                               line2.zValue = rbdata->zValue;
                               line2.draw(DM_INVERT);
                               rbdata->qgiVector2.append(line2.qgi);
                               rbdata->zValue = line2.zValue;
                           }
                           */
                       }
                       if (line3.getdefined())
                       {
                           line3.draw(DM_INVERT);
                           /*
                           if(i-1 < rbdata->qgiVector3.size())
                           {
                               if(rbdata)
                               {
                                   line3.qgi = rbdata->qgiVector3.at(i-1);
                                   line3.zValue = rbdata->zValue;
                               }
                               line3.draw(DM_INVERT);
                               if(rbdata)
                               {
                                   rbdata->qgiVector3.replace(i-1,line3.qgi);
                                   rbdata->zValue = line3.zValue;
                               }
                           }
                           else
                           {
                               line3.zValue = rbdata->zValue;
                               line3.draw(DM_INVERT);
                               rbdata->qgiVector3.append(line3.qgi);
                               rbdata->zValue = line3.zValue;
                           }
                           */
                       }
                  }
               }
          }
       if (insert)
         {  if (fabs(thickness) > db.getptoler())
              {  line4 = new Line(p4,p6);
                 if (line4 != NULL && line4->getdefined())
                 {
                     line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                     db.geometry.add(line4);
                 }
                 else
                   delete line4;

                 if (i > 0 && hatch)
                   {  list3 = elist;
                      list3.add(line1);line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line3);line3->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line2);line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line4);line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                      Plane *plane = new Plane(list3,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                      if (plane->getdefined())
                      {
                          plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
                        db.geometry.add(plane);
                      }
                      else
                        delete plane;
                   }
              }
         }
       else
         {
           if (fabs(thickness) > db.getptoler())
           {
               Line line2(p4,p6);
               if (line2.getdefined())
               {
                   line2.draw(DM_INVERT);
                   /*
                   if(i < rbdata->qgiVector4.size() && rbdata->qgiVector4.size() > 0)
                   {
                       if(rbdata)
                       {
                           line2.zValue = rbdata->zValue;
                           line2.qgi = rbdata->qgiVector4.at(i);
                       }
                       line2.draw(DM_INVERT);
                       if(rbdata)
                       {
                           rbdata->zValue = line2.zValue;
                           rbdata->qgiVector4.replace(i,line2.qgi);
                       }
                   }
                   else
                   {
                       line2.zValue = rbdata->zValue;
                       line2.draw(DM_INVERT);
                       rbdata->qgiVector4.append(line2.qgi);
                       rbdata->zValue = line2.zValue;
                   }
                   */
                }
           }
         }
       p3 = p4;
       p5 = p6;
       line3 = line4;
    }
  if (insert)
    {  if (hatch)
         {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            Plane *plane = new Plane(list1,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
            if (plane->getdefined())
            {
                plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              db.geometry.add(plane);
            }
            else
              delete plane;
            if (fabs(thickness) > db.getptoler())
              {  Plane *plane = new Plane(list2,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 if (plane->getdefined())
                 {
                     plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
                   db.geometry.add(plane);
                 }
                 else
                   delete plane;
              }
         }
       db.saveundo(UD_ENDBLOCK,NULL);

    }
}
#else
void create_polygon(int insert,int hatch,double thickness,Point origin,Point xaxis,Point yaxis,double radius,int n,RBLine8_data *rbdata=0)
{
 int i;
 double a;
 Point p3,p4,p5,p6;
 EntityList list1,list2,list3,elist;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask options(32);
 RCCHAR patternfilename[300];
 Line *line1,*line2,*line3,*line4;
 RCCOLORREF gradientcolour;

  if (insert)
    db.saveundo(UD_STARTBLOCK,NULL);

  for (i = 0 ; i <= n ; i++)
    {  a = double(i) / double(n) * 2.0 * M_PI;
       p4 = origin + xaxis * (radius * cos(a)) +
                     yaxis * (radius * sin(a));
       p6 = p4 + xaxis.cross(yaxis) * thickness;
       if (i > 0)
          {  if (insert)
               {  line1 = new Line(p3,p4);
                  if (line1 != NULL && line1->getdefined())
                    {
                       line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                       db.geometry.add(line1);
                       if (hatch)
                         list1.add(line1);
                    }
                  else
                    delete line1;
                  if (fabs(thickness) > db.getptoler())
                  {
                       line2 = new Line(p5,p6);
                       if (line2 != NULL && line2->getdefined())
                         {
                            line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                            db.geometry.add(line2);
                            if (hatch)
                              list2.add(line2);
                         }
                       else
                         delete line2;
                  }
               }
             else
               {
                  Line line1(p3,p4);
                  if (line1.getdefined())
                  {
                      if(i-1 < rbdata->qgiVector1.size())
                      {
                          if(rbdata)
                          {
                              line1.qgi = rbdata->qgiVector1.at(i-1);
                              line1.zValue = rbdata->zValue;
                          }
                          line1.draw(DM_INVERT);
                          if(rbdata)
                          {
                              rbdata->qgiVector1.replace(i-1,line1.qgi);
                              rbdata->zValue = line1.zValue;
                          }
                      }
                      else
                      {
                          line1.zValue=rbdata->zValue;
                          line1.draw(DM_INVERT);
                          rbdata->qgiVector1.append(line1.qgi);
                          rbdata->zValue = line1.zValue;
                      }
                  }
                  if (fabs(thickness) > db.getptoler())
                  {
                       Line line2(p5,p6);
                       Line line3((p3 + p5)/2.0,(p4 + p6) / 2.0);
                       if (line2.getdefined())
                       {
                           if(i-1 < rbdata->qgiVector2.size())
                           {
                               if(rbdata)
                               {
                                   line2.qgi = rbdata->qgiVector2.at(i-1);
                                   line2.zValue = rbdata->zValue;
                               }
                               line2.draw(DM_INVERT);
                               if(rbdata)
                               {
                                   rbdata->qgiVector2.replace(i-1,line2.qgi);
                                   rbdata->zValue = line2.zValue;
                               }
                           }
                           else
                           {
                               line2.zValue = rbdata->zValue;
                               line2.draw(DM_INVERT);
                               rbdata->qgiVector2.append(line2.qgi);
                               rbdata->zValue = line2.zValue;
                           }
                       }
                       if (line3.getdefined())
                       {
                           if(i-1 < rbdata->qgiVector3.size())
                           {
                               if(rbdata)
                               {
                                   line3.qgi = rbdata->qgiVector3.at(i-1);
                                   line3.zValue = rbdata->zValue;
                               }
                               line3.draw(DM_INVERT);
                               if(rbdata)
                               {
                                   rbdata->qgiVector3.replace(i-1,line3.qgi);
                                   rbdata->zValue = line3.zValue;
                               }
                           }
                           else
                           {
                               line3.zValue = rbdata->zValue;
                               line3.draw(DM_INVERT);
                               rbdata->qgiVector3.append(line3.qgi);
                               rbdata->zValue = line3.zValue;
                           }
                       }
                  }
               }
          }
       if (insert)
         {  if (fabs(thickness) > db.getptoler())
              {  line4 = new Line(p4,p6);
                 if (line4 != NULL && line4->getdefined())
                 {
                     line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                     db.geometry.add(line4);
                 }
                 else
                   delete line4;

                 if (i > 0 && hatch)
                   {  list3 = elist;
                      list3.add(line1);line1->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line3);line3->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line2);line2->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      list3.add(line4);line4->zValue=QGDRAGZVALUE(QGMAXZVALUE);
                      Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                      Plane *plane = new Plane(list3,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                      if (plane->getdefined())
                      {
                          plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
                        db.geometry.add(plane);
                      }
                      else
                        delete plane;
                   }
              }
         }
       else 
         {
           if (fabs(thickness) > db.getptoler())
           {
               Line line2(p4,p6);
               if (line2.getdefined())
               {
                   if(i < rbdata->qgiVector4.size() && rbdata->qgiVector4.size() > 0)
                   {
                       if(rbdata)
                       {
                           line2.zValue = rbdata->zValue;
                           line2.qgi = rbdata->qgiVector4.at(i);
                       }
                       line2.draw(DM_INVERT);
                       if(rbdata)
                       {
                           rbdata->zValue = line2.zValue;
                           rbdata->qgiVector4.replace(i,line2.qgi);
                       }
                   }
                   else
                   {
                       line2.zValue = rbdata->zValue;
                       line2.draw(DM_INVERT);
                       rbdata->qgiVector4.append(line2.qgi);
                       rbdata->zValue = line2.zValue;
                   }
                }
           }
         }
       p3 = p4;
       p5 = p6;
       line3 = line4;
    }
  if (insert)
    {  if (hatch)
         {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
            Plane *plane = new Plane(list1,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
            if (plane->getdefined())
            {
                plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              db.geometry.add(plane);
            }
            else
              delete plane;
            if (fabs(thickness) > db.getptoler()) 
              {  Plane *plane = new Plane(list2,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 if (plane->getdefined())
                 {
                     plane->zValue = QGDRAGZVALUE(QGMAXZVALUE);
                   db.geometry.add(plane);
                 }
                 else
                   delete plane;
              }
         }
       db.saveundo(UD_ENDBLOCK,NULL);

    }
}
#endif

void RBLine8(int mode,void *data,Point *p2,View3dSurface *)
{
 RBLine8_data *rbdata = (RBLine8_data *) data;
 Point xaxis,yaxis;
 double len1,len2;
 Transform t;

#ifdef NEW_RUBBERB
 if(mode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
#endif

  xaxis = *p2 - rbdata->origin;  len1 = xaxis.length();
  yaxis = rbdata->zaxis.cross(xaxis);  len2 = yaxis.length();
  if (len1 > db.getptoler() && len2 > db.getptoler())
    {  xaxis /= len1;  yaxis /= len2;

       if (rbdata->fixedsize == 1)
         len1 = rbdata->radius;

       if (! rbdata->inside)
         {  len1 = len1 / cos(M_PI / double(rbdata->n));
            t.rotate(Point(0.0,0.0,0.0),Point(xaxis.cross(yaxis)),M_PI / double(rbdata->n));
            xaxis = t.transform(xaxis);
            yaxis = t.transform(yaxis);
         }

       if (rbdata->fixedsize == 2)
         len1 = rbdata->chordlength / sin(acos(-1.0) / rbdata->n) / 2.0;

       create_polygon(0,0,rbdata->thickness,rbdata->origin,xaxis,yaxis,len1,rbdata->n,rbdata);
    }
}

/**
 * @brief line_command8
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert polygon command
 *
 */
void line_command8(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine8_data *rbdata = (RBLine8_data *) *data;
 Dialog dialog("Polygon_Dialog");
 DialogControl *dc;
 Point p,p3,p4;
 ResourceString rs21(NCLINEB+21),rs22(NCLINEB+22);
 Transform t;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBLine8_data;  *data = rbdata;
         if (rbdata != NULL)
           {  rbdata->n = v.getinteger("ln::nsides");
              rbdata->fixedsize = v.getinteger("ln::fixedsize");
              rbdata->inside = v.getinteger("ln::inside");
              rbdata->hatch = v.getinteger("ln::hatch");
              rbdata->thickness = v.getreal("ln::thickness");
              rbdata->radius = v.getreal("ln::radius");
              rbdata->chordlength = v.getreal("ln::chordlength");

              rbdata->qgiVector1.clear();
              rbdata->qgiVector2.clear();
              rbdata->qgiVector3.clear();
              rbdata->qgiVector4.clear();
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);

              dialog.add(dc = new IntegerDialogControl(100,&rbdata->n,3,1000));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(dc = new RealDialogControl(101,&rbdata->thickness,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(new PolygonRadioButtonDialogControl(102,102,104,&rbdata->fixedsize));
              dialog.add(new PolygonRadioButtonDialogControl(103,102,104,&rbdata->fixedsize));
              dialog.add(new PolygonRadioButtonDialogControl(104,102,104,&rbdata->fixedsize));
              dialog.add(dc = new RealDialogControl(105,&rbdata->radius,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1105,dc));
              dialog.add(dc = new RealDialogControl(106,&rbdata->chordlength,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1106,dc));
              dialog.add(new CheckBoxDialogControl(107,&rbdata->inside));
              dialog.add(new CheckBoxDialogControl(108,&rbdata->hatch));

              if (dialog.process() == TRUE)
                {  v.setinteger("ln::nsides",rbdata->n);
                   v.setinteger("ln::inside",rbdata->inside);
                   v.setinteger("ln::hatch",rbdata->hatch);
                   v.setinteger("ln::fixedsize",rbdata->fixedsize);
                   v.setreal("ln::thickness",rbdata->thickness);
                   v.setreal("ln::radius",rbdata->radius);
                   v.setreal("ln::chordlength",rbdata->chordlength);
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
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->zaxis = ((Coordinate3dEvent *) event)->getxaxis().cross(((Coordinate3dEvent *) event)->getyaxis());
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Circle *circle = (Circle *) ((EntitySelectedEvent *) event)->getentity();
            Point xaxis,yaxis;
            double radius;
              state.destroyselection(1);
              xaxis = circle->getxaxis();
              yaxis = circle->getyaxis();
              radius = circle->getradius();

              if (rbdata->fixedsize == 1)
                radius = rbdata->radius;

              if (! rbdata->inside)
                {  radius = radius / cos(M_PI / double(rbdata->n));

                   t.rotate(Point(0.0,0.0,0.0),Point(xaxis.cross(yaxis)),M_PI / double(rbdata->n));
                   xaxis = t.transform(xaxis);
                   yaxis = t.transform(yaxis);
                }

              if (rbdata->fixedsize == 2)
                radius = rbdata->chordlength / sin(acos(-1.0) / rbdata->n) / 2.0;

              create_polygon(1,rbdata->hatch,rbdata->thickness,circle->getorigin(),xaxis,yaxis,radius,rbdata->n);
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {Point xaxis,yaxis;
            double len1,len2;
              xaxis = ((Coordinate3dEvent *)event)->getp() - rbdata->origin;  len1 = xaxis.length();
              yaxis = rbdata->zaxis.cross(xaxis);  len2 = yaxis.length();
              if (len1 > db.getptoler() && len2 > db.getptoler())
                {  xaxis /= len1;  yaxis /= len2;
                   if (rbdata->fixedsize == 1)
                     len1 = rbdata->radius;
                   if (! rbdata->inside)
                     {  len1 = len1 / cos(M_PI / double(rbdata->n));
                        t.rotate(Point(0.0,0.0,0.0),Point(xaxis.cross(yaxis)),M_PI / double(rbdata->n));
                        xaxis = t.transform(xaxis);
                        yaxis = t.transform(yaxis);
                     }

                   if (rbdata->fixedsize == 2)
                     len1 = rbdata->chordlength / sin(acos(-1.0) / rbdata->n) / 2.0;

                  create_polygon(1,rbdata->hatch,rbdata->thickness,rbdata->origin,xaxis,yaxis,len1,rbdata->n);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      for(int i=0; i<rbdata->qgiVector1.size(); i++)
      {
          if(rbdata->qgiVector1.at(i))
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector1.at(i));
              delete rbdata->qgiVector1.at(i);
          }
      }
      rbdata->qgiVector1.clear();
      for(int i=0; i<rbdata->qgiVector2.size(); i++)
      {
          if(rbdata->qgiVector2.at(i))
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector2.at(i));
              delete rbdata->qgiVector2.at(i);
          }
      }
      rbdata->qgiVector2.clear();
      for(int i=0; i<rbdata->qgiVector3.size(); i++)
      {
          if(rbdata->qgiVector3.at(i))
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector3.at(i));
              delete rbdata->qgiVector3.at(i);
          }
      }
      rbdata->qgiVector3.clear();
      for(int i=0; i<rbdata->qgiVector4.size(); i++)
      {
          if(rbdata->qgiVector4.at(i))
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiVector4.at(i));
              delete rbdata->qgiVector4.at(i);
          }
      }
      rbdata->qgiVector4.clear();
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,circle_entity,end_list);
       cadwindow->prompt->normalprompt(rs21.gets());
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(rs22.gets());
       rubberband.begin(0);
       rubberband.add(RBLine8,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

void line_command9(int *cstate,HighLevelEvent *,void **)
{
  Dialog dialog("Line_Dialog");
  double autosnapangle;
  DialogControl *dc;
  autosnapangle = v.getreal("ln::autosnapangle");
  dialog.add(dc = new RealDialogControl(100,&autosnapangle));
  //dialog.add(new ScrollBarDialogControl(1100,dc));
  if (dialog.process() == TRUE)
    v.setreal("ln::autosnapangle",autosnapangle);
  *cstate = ExitState;
}
