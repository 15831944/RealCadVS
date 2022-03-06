
#include "ncwin.h"

#if MENUS != STUDENTMENUS

struct RBPatch1_data
{
#ifdef USING_WIDGETS
    QList<QGraphicsItem *> qglines;
#endif
   int maxn,npoints,un,vn,n,udegree,vdegree,triangulated;
   Point *polygon;
};

#ifdef NEW_RUBBERB
void RBPatch1(int dmode,void *data,Point *p2,View3dSurface *)
{
 RBPatch1_data *rbdata = (RBPatch1_data *) data;
 int i,j,n;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  rbdata->polygon[rbdata->npoints] = *p2;
  int cnt=0;
  for (i = 0 ; i <= rbdata->vn ; i++)
    {  n = (i < rbdata->vn ? rbdata->un : rbdata->n + 1);
       for (j = 1 ; j < n ; j++)
         {Line line(rbdata->polygon[i*rbdata->un+j-1],rbdata->polygon[i*rbdata->un+j]);
           if(cnt < rbdata->qglines.size())
           {
               //line.qgi = rbdata->qglines[cnt];
               line.draw(DM_INVERT);
               //rbdata->qglines[cnt] = line.qgi;
               cnt++;
           }
           else
           {
               //line.qgi = 0;
               line.draw(DM_INVERT);
               //rbdata->qglines.append(line.qgi);
               cnt++;
           }
         }
    }
  for (j = 0 ; j < rbdata->un ; j++)
    {  n = rbdata->vn + (j < rbdata->n + 1);
       for (i = 1 ; i < n ; i++)
         {Line line(rbdata->polygon[(i-1)*rbdata->un+j],rbdata->polygon[i*rbdata->un+j]);
            if(cnt < rbdata->qglines.size())
            {
                //line.qgi = rbdata->qglines[cnt];
                line.draw(DM_INVERT);
                //rbdata->qglines[cnt] = line.qgi;
                cnt++;
            }
            else
            {
                //line.qgi = 0;
                line.draw(DM_INVERT);
                //rbdata->qglines.append(line.qgi);
                cnt++;
            }
         }
    }
}
#else
#ifdef USING_WIDGETS
void RBPatch1(int,void *data,Point *p2,View3dSurface *)
{
 RBPatch1_data *rbdata = (RBPatch1_data *) data;
 int i,j,n;
  rbdata->polygon[rbdata->npoints] = *p2;
  int cnt=0;
  for (i = 0 ; i <= rbdata->vn ; i++)
    {  n = (i < rbdata->vn ? rbdata->un : rbdata->n + 1);
       for (j = 1 ; j < n ; j++)
         {Line line(rbdata->polygon[i*rbdata->un+j-1],rbdata->polygon[i*rbdata->un+j]);
           if(cnt < rbdata->qglines.size())
           {
               line.qgi = rbdata->qglines[cnt];
               line.draw(DM_INVERT);
               rbdata->qglines[cnt] = line.qgi;
               cnt++;
           }
           else
           {
               line.qgi = 0;
               line.draw(DM_INVERT);
               rbdata->qglines.append(line.qgi);
               cnt++;
           }
         }
    }
  for (j = 0 ; j < rbdata->un ; j++)
    {  n = rbdata->vn + (j < rbdata->n + 1);
       for (i = 1 ; i < n ; i++)
         {Line line(rbdata->polygon[(i-1)*rbdata->un+j],rbdata->polygon[i*rbdata->un+j]);
            if(cnt < rbdata->qglines.size())
            {
                line.qgi = rbdata->qglines[cnt];
                line.draw(DM_INVERT);
                rbdata->qglines[cnt] = line.qgi;
                cnt++;
            }
            else
            {
                line.qgi = 0;
                line.draw(DM_INVERT);
                rbdata->qglines.append(line.qgi);
                cnt++;
            }
         }
    }
}
#else
void RBPatch1(int,void *data,Point *p2,View3dSurface *)
{
 RBPatch1_data *rbdata = (RBPatch1_data *) data;
 int i,j,n;
  rbdata->polygon[rbdata->npoints] = *p2;
  for (i = 0 ; i <= rbdata->vn ; i++)
    {  n = (i < rbdata->vn ? rbdata->un : rbdata->n + 1);
       for (j = 1 ; j < n ; j++)
         {Line line(rbdata->polygon[i*rbdata->un+j-1],rbdata->polygon[i*rbdata->un+j]);
            line.draw(DM_INVERT);
         }
    }
  for (j = 0 ; j < rbdata->un ; j++)
    {  n = rbdata->vn + (j < rbdata->n + 1);
       for (i = 1 ; i < n ; i++)
         {Line line(rbdata->polygon[(i-1)*rbdata->un+j],rbdata->polygon[i*rbdata->un+j]);
            line.draw(DM_INVERT);
         }
    }
}
#endif
#endif

class PMCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     PMCheckBoxDialogControl(int id,int *x) : CheckBoxDialogControl(id,x)  {};
     void load(Dialog *);
     int select(Dialog *);
  };
 
void PMCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),100),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101),!oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1100),!oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1101),!oldvalue);
}

int PMCheckBoxDialogControl::select(Dialog *dialog)
{int status;
  status = CheckBoxDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),100),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101),!oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1100),!oldvalue);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1101),!oldvalue);
  return status;
}


/**
 * @brief patch_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert patch by defining polygon mesh
 *
 * KMJ : Working in Qt version
 *
 */
void patch_command1(int *cstate,HighLevelEvent *event,void **data)
{Dialog dialog("PatchMesh_Dialog");
 RBPatch1_data *rbdata = (RBPatch1_data *) *data;
 DialogControl *dc;
 ResourceString rs0(NCPATCH),rs1(NCPATCH+1),rs7(NCPATCH+7);
 Patch *patch;
 BitMask options(32);
 int xmesh,ymesh,ud,vd;
 EntityList elist;
 Point *polygon;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(419,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBPatch1_data) == 0)
           *cstate = ExitState;
         else
           {  *data = rbdata;
              rbdata->udegree = v.getinteger("sp::udegree");
              rbdata->vdegree = v.getinteger("sp::vdegree");
              rbdata->triangulated = v.getinteger("sp::triangulated");
              rbdata->n = rbdata->un = rbdata->vn = rbdata->npoints = 0;
              rbdata->maxn = 200;
#ifdef USING_WIDGETS
              rbdata->qglines.clear();
#endif
              if ((rbdata->polygon = new Point[rbdata->maxn]) != 0)
                {  dialog.add(new PMCheckBoxDialogControl(102,&rbdata->triangulated));
                   dialog.add(dc = new IntegerDialogControl(100,&rbdata->udegree,2,10));
                   //dialog.add(new ScrollBarDialogControl(1100,dc));
                   dialog.add(dc = new IntegerDialogControl(101,&rbdata->vdegree,2,10));
                   //dialog.add(new ScrollBarDialogControl(1101,dc));
                   if (dialog.process() == TRUE)
                     {  v.setinteger("sp::udegree",rbdata->udegree);
                        v.setinteger("sp::vdegree",rbdata->vdegree);
                        v.setinteger("sp::triangulated",rbdata->triangulated);
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
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->npoints == rbdata->maxn)
                {  if ((polygon = new Point[rbdata->maxn * 2]) == 0)
                     *cstate = ExitState;
                   else
                     {  memcpy(polygon,rbdata->polygon,sizeof(Point) * rbdata->maxn);
                        delete [] rbdata->polygon;
                        rbdata->polygon = polygon;
                        rbdata->maxn *= 2;
                     }
                }
              if (*cstate != ExitState)
                {  rbdata->polygon[rbdata->npoints] = ((Coordinate3dEvent *) event)->getp();
                   rbdata->npoints++;
                   rbdata->n++;
                   rbdata->un++;
                   *cstate = 1;
                }
           }
         else if (event->isa(NewCommand))
           {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(!rbdata->qglines.empty() && rbdata->qglines.back())
      {
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines.back());
          delete rbdata->qglines.back();
          rbdata->qglines.pop_back();
      }

  }
#endif
#endif
             if (rbdata->npoints > 0)
                {  rbdata->npoints--;
                   rbdata->n--;
                   rbdata->un--;
                }
           }
         else if (event->isa(Abort))
           {  if (rbdata->un == 0)
                *cstate = ExitState;
              else
                *cstate = 2;
              rbdata->vn++;
              rbdata->n = 0;
           }
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->npoints == rbdata->maxn)
                {  if ((polygon = new Point[rbdata->maxn * 2]) == 0)
                     *cstate = ExitState;
                   else
                     {  memcpy(polygon,rbdata->polygon,sizeof(Point) * rbdata->maxn);
                        delete [] rbdata->polygon;
                        rbdata->polygon = polygon;
                        rbdata->maxn *= 2;
                     }
                }
              if (*cstate != ExitState)
                {  rbdata->polygon[rbdata->npoints] = ((Coordinate3dEvent *) event)->getp();
                   rbdata->npoints++;
                   rbdata->n++;
                   *cstate = 2;
                }
           }
         else if (event->isa(NewCommand))
           {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
          if(!rbdata->qglines.empty() && rbdata->qglines.back())
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines.back());
              delete rbdata->qglines.back();
              rbdata->qglines.pop_back();
          }
          if(rbdata->qglines.back())
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines.back());
              delete rbdata->qglines.back();
              rbdata->qglines.pop_back();
          }
  }
#endif
#endif
                if (rbdata->n > 0)
                {  rbdata->npoints--;
                   rbdata->n--;
                }
              else if (rbdata->vn > 1)
                {  rbdata->npoints--;
                   rbdata->n = rbdata->un-1;
                   rbdata->vn--;
                }
              else
                {  rbdata->npoints--;
                   rbdata->un--;
                   rbdata->n = rbdata->un;
                   rbdata->vn = 0;
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort))
           {
             if (rbdata->n == 0)
                {  if (rbdata->un < 3 || rbdata->vn < 3)
                     cadwindow->MessageBox(rs7.gets(),rs1.gets(),MB_ICONINFORMATION);
                   else
                     {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      for(int k=0; k<rbdata->qglines.size(); k++)
      {
          if(rbdata->qglines[k])
          {
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines[k]);
              delete rbdata->qglines[k];
              rbdata->qglines[k]=0;
          }
      }
      rbdata->qglines.clear();
  }
#endif
#endif
                     Patch::getdefaults(&ud,&vd,&xmesh,&ymesh,&options);

                        if (rbdata->triangulated)
                        {long i,ix,iy,nindex,*index;

                            nindex = (rbdata->un-1) * (rbdata->vn-1) * 6;
                            index = new long[nindex];
                            for (i = 0 ; i < nindex ; i += 6)
                            {   ix = (i/6) % (rbdata->un - 1);
                                iy = (i/6) / (rbdata->un - 1);
                                index[i] = iy * rbdata->un + ix;
                                index[i+1] = iy * rbdata->un + ix + 1;
                                index[i+2] = (iy + 1) * rbdata->un + ix + 1;

                                index[i+3] = iy * rbdata->un + ix;
                                index[i+4] = (iy + 1) * rbdata->un + ix + 1;
                                index[i+5] = (iy + 1) * rbdata->un + ix;

                            }

                            patch = new Patch(rbdata->un * rbdata->vn,rbdata->polygon,nindex,index,options);
                        }
                        else
                            patch = new Patch(rbdata->un,rbdata->vn,rbdata->udegree,rbdata->vdegree,
                                                xmesh,ymesh,rbdata->polygon,0,0,0,elist,options);
                        if (patch->getdefined())
                          db.geometry.add(patch);
                     }
                   rbdata->n = rbdata->un = rbdata->vn = rbdata->npoints = 0;
                   *cstate = 1;
                }
              else if (rbdata->n != rbdata->un)
                {  cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
                   rbdata->n = rbdata->un = rbdata->vn = rbdata->npoints = 0;
                   *cstate = 1;
                }
              else
                {  rbdata->n = 0;
                   rbdata->vn++;
                   *cstate = 2;
                }
           }
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }



//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,undo_mask,end_list);
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBPatch1,rbdata);
       cadwindow->prompt->normalprompt(NCPATCH+2);
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBPatch1,rbdata);
       cadwindow->prompt->normalprompt(NCPATCH+3);
    }
  else if (*cstate == ExitState)
    {
#ifdef USING_WIDGETS
          if(rbdata)
          {
              for(int k=0; k<rbdata->qglines.size(); k++)
              {
                  if(rbdata->qglines[k])
                  {
                      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines[k]);
                      delete rbdata->qglines[k];
                      rbdata->qglines[k]=0;
                  }
              }
              rbdata->qglines.clear();
          }
#endif
       if (rbdata != 0) delete [] rbdata->polygon;
       delete rbdata;
       rbdata=0;
    }
}

struct RBPatch2_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qglines[4];
    qreal zValue;
#endif
    int state,un,vn,udegree,vdegree;
   Point p[4];
};

#ifdef NEW_RUBBERB
void RBPatch2(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBPatch2_data *rbdata = (RBPatch2_data *) data;
  int i;
  rbdata->p[rbdata->state-1] = *p2;
  int ct=0;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  for (i = 1 ; i < rbdata->state ; i++)
  {
      Line line(rbdata->p[i-1],rbdata->p[i]);
      //line.qgi = rbdata->qglines[i-1]; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      //rbdata->qglines[ct] = line.qgi;
      ct++;
  }
  if (rbdata->state == 4)
  {
      Line line(rbdata->p[3],rbdata->p[0]);
      //line.qgi = rbdata->qglines[3]; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      //rbdata->qglines[3] = line.qgi;
  }
}
#else
void RBPatch2(int,void *data,Point *p2,View3dSurface *)
{
  RBPatch2_data *rbdata = (RBPatch2_data *) data;
  int i;
  rbdata->p[rbdata->state-1] = *p2;
  int ct=0;
  for (i = 1 ; i < rbdata->state ; i++)
  {
      Line line(rbdata->p[i-1],rbdata->p[i]);
      line.qgi = rbdata->qglines[i-1]; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      rbdata->qglines[ct] = line.qgi;
      ct++;
  }
  if (rbdata->state == 4)
  {
      Line line(rbdata->p[3],rbdata->p[0]);
      line.qgi = rbdata->qglines[3]; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      rbdata->qglines[3] = line.qgi;
  }
}
#endif

/**
 * @brief patch_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for defining a path by corner points
 *
 * KMJ : Working in Qt version
 *
 */
void patch_command2(int *cstate,HighLevelEvent *event,void **data)
{
 Dialog dialog("PatchCorners_Dialog");
 RBPatch2_data *rbdata = (RBPatch2_data *) *data;
 DialogControl *dc;
 Patch *patch;
 BitMask options(32);
 int i,j,xmesh,ymesh,ud,vd;
 EntityList elist;
 Point *polygon,du1,du2,p1,p2;
 double u1,v1;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(419,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBPatch2_data) == 0)
           *cstate = ExitState;
         else
           {  *data = rbdata;
#ifdef USING_WIDGETS
             for(int k=0; k<4; k++)
                rbdata->qglines[k]=0;
             rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
              rbdata->un = v.getinteger("sp::un");
              rbdata->vn = v.getinteger("sp::vn");
              rbdata->udegree = v.getinteger("sp::udegree");
              rbdata->vdegree = v.getinteger("sp::vdegree");
              dialog.add(dc = new IntegerDialogControl(100,&rbdata->udegree,2,10));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(dc = new IntegerDialogControl(101,&rbdata->vdegree,2,10));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(dc = new IntegerDialogControl(102,&rbdata->un,3,200));
              //dialog.add(new ScrollBarDialogControl(1102,dc));
              dialog.add(dc = new IntegerDialogControl(103,&rbdata->vn,3,200));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              if (dialog.process() == TRUE)
                {  v.setinteger("sp::un",rbdata->un);
                   v.setinteger("sp::vn",rbdata->vn);
                   v.setinteger("sp::udegree",rbdata->udegree);
                   v.setinteger("sp::vdegree",rbdata->vdegree);
                   rbdata->state = *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p[0] = ((Coordinate3dEvent *) event)->getp();
              rbdata->state = *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p[1] = ((Coordinate3dEvent *) event)->getp();
              rbdata->state = *cstate = 3;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p[2] = ((Coordinate3dEvent *) event)->getp();
              rbdata->state = *cstate = 4;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 4 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p[3] = ((Coordinate3dEvent *) event)->getp();
              du1 = rbdata->p[1] - rbdata->p[0];
              du2 = rbdata->p[2] - rbdata->p[3];
              if ((polygon = new Point[rbdata->un * rbdata->vn]) != 0)
                {  for (i = 0 ; i < rbdata->un ; i++)
                     for (j = 0 ; j < rbdata->vn ; j++)
                       {  u1 = double(i) / double(rbdata->un - 1);
                          v1 = double(j) / double(rbdata->vn - 1);
                          p1 = rbdata->p[0] + du1 * u1;
                          p2 = rbdata->p[3] + du2 * u1;
                          polygon[j * rbdata->un + i] = p1 + (p2 - p1) * v1;
                       }
                   Patch::getdefaults(&ud,&vd,&xmesh,&ymesh,&options);
                   patch = new Patch(rbdata->un,rbdata->vn,rbdata->udegree,rbdata->vdegree,
                                     xmesh,ymesh,polygon,0,0,0,elist,options);
                   if (patch->getdefined())
                     db.geometry.add(patch);
                   delete [] polygon;
                }
              rbdata->state = *cstate = 1;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }

#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      for(int k=0; k<4; k++)
      {
        if(rbdata->qglines[k])
        {
            cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qglines[k]);
            delete rbdata->qglines[k]; rbdata->qglines[k]=0;
        }
      }
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBPatch2,rbdata);
       cadwindow->prompt->normalprompt(NCPATCH+7+*cstate);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBPatch3_data
{
#ifdef USING_WIDGETS
   QGraphicsItem *qgcopy,*qgline;
   qreal zValue;
#endif
   Patch *patch1,*patch2,*copy;
   Plane *plane;
   EntityList plist; 
   int *indicies;
   Point move,scale,origin,move_p1,scale_origin,rotate_p1,rotate_p2;
   double flattenz,smoothingdistance,rotationangle;
   int button,state,n,translatemode,flattenmode,pointselectionmode;
};

#ifdef NEW_RUBBERB
void RBPatch3(int dmode, void *data,Point *p2,View3dSurface *)
{
    RBPatch3_data *rbdata = (RBPatch3_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }

    if (rbdata->state == 11)
    {
        Transform t;
        t.translate(*p2 - rbdata->move_p1);
        rbdata->copy->movepoints(0,rbdata->n,rbdata->indicies,&t);

        //rbdata->copy->qgi = rbdata->qgcopy; rbdata->copy->zValue = rbdata->zValue;
        rbdata->copy->draw(DM_INVERT);
        //rbdata->qgcopy = rbdata->copy->qgi;
    }
    else if (rbdata->state == 31)
    {
        Line line(*p2,rbdata->rotate_p1);

        //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
        line.draw(DM_INVERT);
        //rbdata->qgline = line.qgi;
    }
}
#else
#ifdef USING_WIDGETS
void RBPatch3(int dmode,void *data,Point *p2,View3dSurface *)
{
    RBPatch3_data *rbdata = (RBPatch3_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    if (rbdata->state == 11)
    {
        Transform t;
        t.translate(*p2 - rbdata->move_p1);
        rbdata->copy->movepoints(0,rbdata->n,rbdata->indicies,&t);

        //rbdata->copy->qgi = rbdata->qgcopy; rbdata->copy->zValue = rbdata->zValue;
        rbdata->copy->draw(DM_INVERT);
        //rbdata->qgcopy = rbdata->copy->qgi;
    }
    else if (rbdata->state == 31)
    {
        Line line(*p2,rbdata->rotate_p1);

        //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
        line.draw(DM_INVERT);
        //rbdata->qgline = line.qgi;
    }
}
#else
void RBPatch3(int,void *data,Point *p2,View3dSurface *)
{
    RBPatch3_data *rbdata = (RBPatch3_data *) data;
    if (rbdata->state == 11)
    {
        Transform t;
        t.translate(*p2 - rbdata->move_p1);
        rbdata->copy->movepoints(0,rbdata->n,rbdata->indicies,&t);
        rbdata->copy->draw(DM_INVERT);
    }
    else if (rbdata->state == 31)
    {
        Line line(*p2,rbdata->rotate_p1);
        line.draw(DM_INVERT);
    }
}
#endif
#endif

class PARadioButtonDialogControl : public RadioButtonDialogControl
{public:
   PARadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};


void PARadioButtonDialogControl::load(Dialog *dialog)
{
    RadioButtonDialogControl::load(dialog);

    if (id >= 100 && id <= 102)
    {   EnableWindow(GetDlgItem(dialog->gethdlg(),103),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1103),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),104),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1104),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1105),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1106),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),107),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1107),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),108),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1108),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),109),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1109),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),110),oldvalue == 2);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),111),oldvalue == 2);
        EnableWindow(GetDlgItem(dialog->gethdlg(),112),oldvalue == 2);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1112),oldvalue == 2);

    }
    else if (id >= 110 && id <= 111)
    {   EnableWindow(GetDlgItem(dialog->gethdlg(),112),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1112),oldvalue == 0);
    }


}

int PARadioButtonDialogControl::select(Dialog *dialog)
{ RadioButtonDialogControl::select(dialog);

    if (id >= 100 && id <= 102)
    {   EnableWindow(GetDlgItem(dialog->gethdlg(),103),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1103),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),104),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1104),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1105),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),106),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1106),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),107),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1107),oldvalue == 0);
        EnableWindow(GetDlgItem(dialog->gethdlg(),108),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1108),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),109),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1109),oldvalue == 0);

        EnableWindow(GetDlgItem(dialog->gethdlg(),110),oldvalue == 2);
        EnableWindow(GetDlgItem(dialog->gethdlg(),111),oldvalue == 2);
        EnableWindow(GetDlgItem(dialog->gethdlg(),112),oldvalue == 2);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1112),oldvalue == 2);

    }
    else if (id >= 110 && id <= 111)
    {   EnableWindow(GetDlgItem(dialog->gethdlg(),112),oldvalue == 0);
        //EnableWindow(GetDlgItem(dialog->gethdlg(),1112),oldvalue == 0);
    }

  return 0;
}

class PAButtonDialogControl : public ButtonDialogControl
{public:
   PAButtonDialogControl(int id) : ButtonDialogControl(id) {}
   int select(Dialog *);
};

int PAButtonDialogControl::select(Dialog *dialog)
{   if (dialog->validate())
    {   dialog->store();
        return id;
    }
    else
        return 0;
}

void flatten_patch(Patch *patch,double z)
{int i,n;
 Point *p,p1;
 
    db.saveundo(UD_MOVED,patch);
    patch->draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(patch);

    p = patch->getpolygon();
    n = patch->npoints();

    for (i = 0 ; i < n ; i++)
    {   p1 = db.workplanes.getcurrent()->modeltoworkplane(p[i]);
        p1.z = z;
        p[i] = p1;
    }

    patch->draw(DM_NORMAL);

}


void smooth_patch(Patch *patch,EntityList list,double smoothingdistance,int *index)
{int i,j,n,n_seg,n_average;
 Point *p,*p_new,p1,p_sum;
 Entity *e;

    if (smoothingdistance < 0.05)
        return;
 
    db.saveundo(UD_MOVED,patch);
    patch->draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(patch);

    p = patch->getpolygon();
    n = patch->npoints();
    p_new = new Point[n];

    for (j = 0 ; j < n ; j++)
        index[j] = 0;

    for (list.start() ; ! list.atend() ; )
    {   e = list.next();
        if (e != 0)
        {   n_seg = (int)(((LinearEntity *)e)->length() / smoothingdistance * 4 + 1);
            for (i = 0 ; i <= n_seg ; i++)
            {   p1 = ((LinearEntity *)e)->position(double(i) / double(n_seg));
                for (j = 0 ; j < n ; j++)
                    if (index[j] == 0 && fabs(p1.x - p[j].x) < smoothingdistance && fabs(p1.y - p[j].y) < smoothingdistance)
                        index[j] = 1;
            }
        }
    }

    for (i = 0 ; i < n ; i++)
        if (index[i] != 0)
        {   p_sum = Point(0.0,0.0,0.0);
            n_average = 0;
            for (j = 0 ; j < n ; j++)
                if (fabs(p[j].x - p[i].x) < smoothingdistance && fabs(p[j].y - p[i].y) < smoothingdistance)
                {   p_sum += p[j];
                    n_average++;
                }
            p_new[i] = p[i];
            if (n_average > 0)
                p_new[i].z = (p_sum / n_average).z;

        }
        else
            p_new[i] = p[i];

    for (i = 0 ; i < n ; i++)
        p[i] = p_new[i];

    patch->draw(DM_NORMAL);

    delete [] p_new;

}



/**
 * @brief patch_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Move Patch Control Points command
 *
 *  KMJ : Working in Qt version
 *
 */
void patch_command3(int *cstate,HighLevelEvent *event,void **data)
{
 RBPatch3_data *rbdata = (RBPatch3_data *) *data;
 EntityList list;
 Entity *e;
 int i,j,imin;
 Transform t;
 View3dSurface *surface;
 Point offset,p,p1,p2,p3,pi,normal,*triangle;
 double s,d,dmin,l1,l2,D,denom;
 ResourceString rs12(NCPATCH+12),rs13(NCPATCH+13),rs14(NCPATCH+14);
 ResourceString rs15(NCPATCH+15);
 Dialog dialog("PatchMove_Dialog");
 DialogControl *dc;
 Plane *plane;

    if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {   ::state.sendevent(new NewCommandEvent(419,0));
        *cstate = ExitState;
        return;
    }

    switch (*cstate)
    {case InitialState :
        if ((rbdata = new RBPatch3_data) == NULL)
            *cstate = ExitState;
        else if (state.getselection().length() == 0)
        {   cadwindow->MessageBox(rs13.gets(),rs12.gets(),MB_ICONINFORMATION);
            rbdata->copy = rbdata->patch1 = 0;
            rbdata->indicies = 0;
#ifdef USING_WIDGETS
                rbdata->qgcopy=0; rbdata->qgline=0;
                rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
            *cstate = ExitState;
        }
        else if (state.getselection().length() > 1)
        {   cadwindow->MessageBox(rs14.gets(),rs12.gets(),MB_ICONINFORMATION);
            rbdata->copy = rbdata->patch1 = 0;
            rbdata->indicies = 0;
#ifdef USING_WIDGETS
                rbdata->qgcopy=0; rbdata->qgline=0;
                rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
            *cstate = ExitState;
        }
        else
        {
#ifdef USING_WIDGETS
            rbdata->qgcopy=0; rbdata->qgline=0;
            rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
            Transform ident;
            ident.identity();
            list = state.getselection();
            list.start();  e = list.next();
            if (e->isa(patch_entity))
            {   
                rbdata->translatemode = v.getinteger("sp::translatemode");
                rbdata->move.setxyz(v.getreal("sp::movedx"),v.getreal("sp::movedy"),v.getreal("sp::movedz"));
                rbdata->scale.setxyz(v.getreal("sp::scalex"),v.getreal("sp::scaley"),v.getreal("sp::scalez"));
                rbdata->rotationangle = v.getreal("sp::rotationangle");
                rbdata->flattenmode = v.getinteger("sp::flattenmode");
                rbdata->flattenz = v.getreal("sp::flattenz");
                rbdata->pointselectionmode = v.getinteger("sp::pointselectionmode");
                rbdata->smoothingdistance = v.getreal("sp::smoothingdistance");

                rbdata->patch1 = 0;
                rbdata->indicies = 0;
                rbdata->n = 0;
                rbdata->copy = 0;

                dialog.add(new PARadioButtonDialogControl(100,100,102,&rbdata->translatemode));
                dialog.add(new PARadioButtonDialogControl(101,100,102,&rbdata->translatemode));
                dialog.add(new PARadioButtonDialogControl(102,100,102,&rbdata->translatemode));
                dialog.add(dc = new RealDialogControl(103,&rbdata->move.x,ScaleByLength));
                //dialog.add(new ScrollBarDialogControl(1103,dc));
                dialog.add(dc = new RealDialogControl(104,&rbdata->move.y,ScaleByLength));
                //dialog.add(new ScrollBarDialogControl(1104,dc));
                dialog.add(dc = new RealDialogControl(105,&rbdata->move.z,ScaleByLength));
                //dialog.add(new ScrollBarDialogControl(1105,dc));

                dialog.add(dc = new RealDialogControl(106,&rbdata->scale.x,DontScale));
                //dialog.add(new ScrollBarDialogControl(1106,dc));
                dialog.add(dc = new RealDialogControl(107,&rbdata->scale.y,DontScale));
                //dialog.add(new ScrollBarDialogControl(1107,dc));
                dialog.add(dc = new RealDialogControl(108,&rbdata->scale.z,DontScale));
                //dialog.add(new ScrollBarDialogControl(1108,dc));

                dialog.add(dc = new RealDialogControl(109,&rbdata->rotationangle,DontScale));
                //dialog.add(new ScrollBarDialogControl(1109,dc));

                dialog.add(new PARadioButtonDialogControl(110,110,111,&rbdata->flattenmode));
                dialog.add(new PARadioButtonDialogControl(111,110,111,&rbdata->flattenmode));

                dialog.add(dc = new RealDialogControl(112,&rbdata->flattenz,ScaleByLength));
                //dialog.add(new ScrollBarDialogControl(1112,dc));

                dialog.add(new PARadioButtonDialogControl(113,113,114,&rbdata->pointselectionmode));
                dialog.add(new PARadioButtonDialogControl(114,113,114,&rbdata->pointselectionmode));


                dialog.add(dc = new RealDialogControl(115,&rbdata->smoothingdistance,ScaleByLength));
                //dialog.add(new ScrollBarDialogControl(1115,dc));

                dialog.add(new PAButtonDialogControl(116));
                dialog.add(new PAButtonDialogControl(117));
                dialog.add(new PAButtonDialogControl(118));

                rbdata->button = dialog.process();

                if (rbdata->button == 118)
                {   *cstate = 200;
                    rbdata->patch1 = (Patch *) e;  
                }
                else if (rbdata->button != FALSE)
                {   v.setinteger("sp::translatemode",rbdata->translatemode);
                    v.setreal("sp::movedx",rbdata->move.x);
                    v.setreal("sp::movedy",rbdata->move.y);
                    v.setreal("sp::movedz",rbdata->move.z);
                    v.setreal("sp::scalex",rbdata->scale.x);
                    v.setreal("sp::scaley",rbdata->scale.y);
                    v.setreal("sp::scalez",rbdata->scale.z);
                    v.setreal("sp::rotationangle",rbdata->rotationangle);
                    v.setinteger("sp::flattenmode",rbdata->flattenmode);
                    v.setreal("sp::flattenz",rbdata->flattenz);
                    v.setinteger("sp::pointselectionmode",rbdata->pointselectionmode);
                    v.setreal("sp::smoothingdistance",rbdata->smoothingdistance);

                    rbdata->patch1 = (Patch *) e;  
                    rbdata->patch1->setcopye(NULL);
                    rbdata->indicies = new int [rbdata->patch1->npoints()];
                    rbdata->n = 0;
                    rbdata->copy = (Patch *) rbdata->patch1->clone(ident);
                        
                    state.destroyselection(1);
                        
                    if (rbdata->pointselectionmode == 0)
                    {   t.identity();
                        for (i = 0 ; i < rbdata->patch1->npoints() ; i++)
                        {PointE *point;
                            point = new PointE(*rbdata->patch1,rbdata->patch1->point(i));
                            rbdata->plist.add(point);
                            db.geometry.add(point,3,0);
                        }
                        rbdata->plist.reverse();
                        *cstate = 1;  //  Select individual points
                    }
                    else 
                        *cstate = 2;  //  Select a plane that selected the points

                }
                else                 
                {   rbdata->copy = rbdata->patch1 = 0;
                    rbdata->indicies = 0;
                    *cstate = ExitState;
                }
            }
            else                 
            {   cadwindow->MessageBox(rs15.gets(),rs12.gets(),MB_ICONINFORMATION);
                rbdata->copy = rbdata->patch1 = 0;
                rbdata->indicies = 0;
                *cstate = ExitState;
            }

            *data = rbdata;
        }
        break;
     case 1 :
        if (event->isa(Coordinateuv))
        {   if ((surface = (View3dSurface *) ((CoordinateuvEvent *) event)->window()) != NULL)
            {   p1 = surface->uvtoscreen(((CoordinateuvEvent *) event)->coordinate());
                p1.y = surface->getheight() - p1.y;
                p1.z = 0.0;
                for (i = 0,dmin = 0.0 ; i < rbdata->patch1->npoints() ; i++)
                {   p2 = surface->modeltoscreen(rbdata->patch1->point(i));
                    p2.z = 0.0;
                    d = (p2 - p1).length();
                    if (i == 0 || d < dmin)
                    {   imin = i;  dmin = d;
                    }
                }
                if (dmin < 10.0)
                {   for (i = 0 ; i < rbdata->n ; i++)
                        if (rbdata->indicies[i] == imin)
                            break;
                    if (i == rbdata->n)
                    {   rbdata->indicies[rbdata->n] = imin;
                        rbdata->n++;
                        for (i = 0,rbdata->plist.start() ; i <= imin ; i++)
                            e = rbdata->plist.next();
                        e->highlight();
                    }
                }
            }
        }
        else if (event->isa(Abort))
        {   if (rbdata->n == 0)
            {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                    db.geometry.del(e,1,0);
                rbdata->plist.destroy();
                rbdata->n = 0;
                *cstate = ExitState;
            }
            else
            {   
                rbdata->plane = 0;
                    
                if (rbdata->button == 116)
                {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                        if (e->getselected())
                            e->unhighlight();
                    rbdata->patch1->delete_triangulated_patch_vertex(rbdata->n,0,rbdata->indicies);
                    rbdata->n = 0;
                }
                else if (rbdata->button == 117)
                {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                        if (e->getselected())
                            e->unhighlight();
                    rbdata->patch1->delete_triangulated_patch_vertex(rbdata->n,1,rbdata->indicies);
                    rbdata->n = 0;
                }
                else if (rbdata->translatemode == 0)
                {   
                    if (fabs(rbdata->scale.x - 1.0) > 0.001 || fabs(rbdata->scale.y - 1.0) > 0.001 || fabs(rbdata->scale.z - 1.0) > 0.001)
                        *cstate = 20;   //  Get scale origin
                    else if (fabs(rbdata->rotationangle) > 0.001)
                        *cstate = 30;   //  Get rotation axis
                    else
                        *cstate = 100;  //  Perform the actual movement
                }
                else if (rbdata->translatemode == 1)
                    *cstate = 10;
                else// if (rbdata->translatemode == 2)
                {//  Flatten
                    *cstate = 101;
                }
            }
        }
        else if (event->isa(Exit))
            *cstate = ExitState;
        break;
     case 2 :
        if (event->isa(EntitySelected))
        {   plane = (Plane *)((EntitySelectedEvent *) event)->getentity();
            if ((surface = (View3dSurface *) ((EntitySelectedEvent *) event)->getwindow()) != NULL)
            {   Polygon3d polygon3d(plane->getorigin(),plane->getxaxis(),plane->getyaxis(),plane->getlist(),0,
                                         (surface->getumax() - surface->getumin()) / surface->getwidth());
                polygon3d.pick(surface,0.0,0.0,0);  //  First call converts to screen coordinates

                for (i = 0,dmin = 0.0 ; i < rbdata->patch1->npoints() ; i++)
                {   p2 = surface->modeltoscreen(rbdata->patch1->point(i));
                    p2.z = 0.0;
                    if (polygon3d.pick(0,p2.x,p2.y,0) >= 0)
                    {   rbdata->indicies[rbdata->n] = i;
                        rbdata->n++;
                    }
                }

            }

            rbdata->plane = plane;

            if (rbdata->button == 116)
            {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                    if (e->getselected())
                        e->unhighlight();
                rbdata->patch1->delete_triangulated_patch_vertex(rbdata->n,0,rbdata->indicies);
                rbdata->n = 0;
            }
            else if (rbdata->button == 117)
            {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                    if (e->getselected())
                        e->unhighlight();
                rbdata->patch1->delete_triangulated_patch_vertex(rbdata->n,1,rbdata->indicies);
                rbdata->n = 0;
            }
            else if (rbdata->translatemode == 0)
            {   if (fabs(rbdata->scale.x - 1.0) > 0.001 || fabs(rbdata->scale.y - 1.0) > 0.001 || fabs(rbdata->scale.z - 1.0) > 0.001)
                    *cstate = 20;   //  Get scale origin
                else if (fabs(rbdata->rotationangle) > 0.001)
                    *cstate = 30;   //  Get rotation axis
                else
                    *cstate = 100;  //  Perform the actual movement

            }
            else if (rbdata->translatemode == 1)
                *cstate = 10;        
            else if (rbdata->translatemode == 2)
            {   *cstate = 101;
            }

        }
        else if (event->isa(Abort) || event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;

     case 10 :  //  Translate coordinates
        if (event->isa(Coordinate3d))
        {   rbdata->move = rbdata->move_p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 11;
        }
        else if (event->isa(Abort))
        {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                e->unhighlight();
            rbdata->n = 0;
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;
    case 11 :
        rubberband.end(0);
        if (event->isa(Coordinate3d))
        {   rbdata->move = ((Coordinate3dEvent *) event)->getp() - rbdata->move_p1;
            if (fabs(rbdata->scale.x - 1.0) > 0.001 || fabs(rbdata->scale.y - 1.0) > 0.001 || fabs(rbdata->scale.z - 1.0) > 0.001)
                *cstate = 20;   //  Get scale origin
            else if (fabs(rbdata->rotationangle) > 0.001)
                *cstate = 30;   //  Get rotation axis
            else
                *cstate = 100;  //  Perform the actual movement
        }
        else if (event->isa(Abort))
        {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                e->unhighlight();
            rbdata->n = 0;
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;

     case 20 :  //  Scale origin
        if (event->isa(Coordinate3d))
        {   rbdata->scale_origin = ((Coordinate3dEvent *) event)->getp();
            if (fabs(rbdata->rotationangle) > 0.001)
                *cstate = 30;   //  Get rotation axis
            else
                *cstate = 100;  //  Perform the actual movement
        }
        else if (event->isa(Abort))
        {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                e->unhighlight();
            rbdata->n = 0;
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;

     case 30 :  //  Rotation axis
        if (event->isa(Coordinate3d))
        {   rbdata->rotate_p1 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 31;
        }
        else if (event->isa(Abort))
        {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                e->unhighlight();
            rbdata->n = 0;
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;
    case 31 :
        rubberband.end(0);
        if (event->isa(Coordinate3d))
        {   rbdata->rotate_p2 = ((Coordinate3dEvent *) event)->getp();
            *cstate = 100;  //  Perform the actual movement
        }
        else if (event->isa(Abort))
        {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                e->unhighlight();
            rbdata->n = 0;
            *cstate = 1;
        }
        else if (event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;

     case 200 :
        if (event->isa(EntitySelected))
        {   rbdata->patch2 = (Patch *)((EntitySelectedEvent *) event)->getentity();

            db.saveundo(UD_MOVED,rbdata->patch1);
            rbdata->patch1->draw(DM_ERASE);
            cadwindow->invalidatedisplaylist(rbdata->patch1);

            rbdata->patch2->triangulate(0.0);

            dmin = -1.0;  

            for (i = 0 ; i < rbdata->patch1->npoints() ; i++)
            {   p = rbdata->patch1->point(i);
                triangle = rbdata->patch2->gettriangles();
                for (j = 0 ; j < rbdata->patch2->getntriangles() ; j++, triangle += 6)
                {   if (triangle->z == RCDBL_MAX)
                        triangle += 2;

                    if (triangle[0].x > p.x && triangle[1].x > p.x && triangle[2].x > p.x ||
                        triangle[0].y > p.y && triangle[1].y > p.y && triangle[2].y > p.y ||
                        triangle[0].x < p.x && triangle[1].x < p.x && triangle[2].x < p.x ||
                        triangle[0].y < p.y && triangle[1].y < p.y && triangle[2].y < p.y)
                    {   triangle += 3;
                        continue;  //  Don't do the intersection as the simple min/max test has failed
                    }

                    p1 = *triangle++;
                    p2 = *triangle++;
                    p3 = *triangle++;

                    l1 = (p1 - p3).length();
                    l2 = (p2 - p3).length();
                    normal = (p2 - p1).cross(p3 - p1);
                    s = normal.length();
                    if (s > 1.0E-100)
                        normal /= s;
                    D = -normal.dot(p1);
                    denom = normal.dot(Point(0.0,0.0,1.0));
                    if (fabs(denom) > db.getatoler())
                    {   s = -(p.dot(normal) + D) / denom;
                        pi = Point(0.0,0.0,1.0) * s + p;
                        if ((pi - p1).dot(normal.cross(p2 - p1)) > -1.0E-8 * (l1 + l2) &&
                            (pi - p2).dot(normal.cross(p3 - p2)) > -1.0E-8 * (l1 + l2) &&
                            (pi - p3).dot(normal.cross(p1 - p3)) > -1.0E-8 * (l1 + l2))
                        {   rbdata->patch1->getpolygon()[i].z = pi.z;
                            break;
                        }
                    }
                }
            }

            rbdata->patch1->draw(DM_NORMAL);

        }
        else if (event->isa(Abort) || event->isa(Exit))
        {   *cstate = ExitState;
        }
        break;

    }

#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
    if(rbdata)
    {
        if(rbdata->qgcopy)
            cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcopy);
        delete rbdata->qgcopy; rbdata->qgcopy=0;
        if(rbdata->qgline)
            cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
        delete rbdata->qgline; rbdata->qgline=0;
    }
#endif
#endif

    if (*cstate == 100)
    {Transform t1,t2,t3;
        //  Transform the points
        for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
            if (e->getselected())
                e->unhighlight();
        t1.translate(rbdata->move);
        t2.rotate(rbdata->rotate_p1,(rbdata->rotate_p2 - rbdata->rotate_p1).normalize(),rbdata->rotationangle/180.0*acos(-1.0));
        t3.scale(rbdata->scale_origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(0.0,0.0,1.0),rbdata->scale);
        t3.apply(t2);
        t3.apply(t1);
        rbdata->patch1->movepoints(1,rbdata->n,rbdata->indicies,&t3);
        if (rbdata->plane != 0)
            smooth_patch(rbdata->patch1,rbdata->plane->getlist(),rbdata->smoothingdistance,rbdata->indicies);
        delete rbdata->copy;
        t.identity();  
        rbdata->patch1->setcopye(NULL);
        rbdata->copy = (Patch *) rbdata->patch1->clone(t);
        rbdata->n = 0;
        *cstate = rbdata->pointselectionmode == 0 ? 1 : 2;
       
    }
    else if (*cstate == 101)
    {   //  Flatten the points
        for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
            if (e->getselected())
                e->unhighlight();
        db.saveundo(UD_MOVED,rbdata->patch1);
        rbdata->patch1->draw(DM_ERASE);
        cadwindow->invalidatedisplaylist(rbdata->patch1);
        for (i = 0 ; i < rbdata->n ; i++)
            if (rbdata->flattenmode == 0 || rbdata->plane == 0)
                rbdata->patch1->getpolygon()[rbdata->indicies[i]].z = rbdata->flattenz;
            else
            {   Line line(rbdata->patch1->getpolygon()[rbdata->indicies[i]],rbdata->patch1->getpolygon()[rbdata->indicies[i]]+Point(0.0,0.0,1.0));
                Intersect in(rbdata->plane,&line);
                if (in.nintersections() == 1)
                    rbdata->patch1->getpolygon()[rbdata->indicies[i]].z = ((PointE *)in.intersection(0))->getp().z;
            }    
        if (rbdata->plane != 0)
            smooth_patch(rbdata->patch1,rbdata->plane->getlist(),rbdata->smoothingdistance,rbdata->indicies);
        rbdata->patch1->draw(DM_NORMAL);
        *cstate = rbdata->pointselectionmode == 0 ? 1 : 2;
    }

    if (*cstate == 1)
    {   state.selmask.entity.clearandset(uv_mask,end_list);
        cadwindow->prompt->normalprompt(NCPATCH+16);
        rbdata->state = 1;
    }
    else if (*cstate == 2)
    {   state.selmask.entity.clearandset(plane_entity,end_list);
        cadwindow->prompt->normalprompt("Select the plane that defines the points to select");
        rbdata->state = 2;
    }
    else if (*cstate == 10)
    {   state.selmask.entity.clearandset(xyz_mask,end_list);
        cadwindow->prompt->normalprompt(NCPATCH+17);
        rbdata->state = 10;
    }
    else if (*cstate == 11)
    {  rubberband.begin(0);
       rubberband.add(RBPatch3,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCPATCH+18);
       rbdata->state = 11;
    }
    else if (*cstate == 20)
    {   state.selmask.entity.clearandset(xyz_mask,end_list);
        cadwindow->prompt->normalprompt("Locate the origin of the scale");
        rbdata->state = 20;
    }
    else if (*cstate == 30)
    {   state.selmask.entity.clearandset(xyz_mask,end_list);
        cadwindow->prompt->normalprompt("Locate the start of the rotation axis");
        rbdata->state = 30;
    }
    else if (*cstate == 31)
    {  rubberband.begin(0);
       rubberband.add(RBPatch3,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the end of the rotation axis");
       rbdata->state = 31;
    }
    else if (*cstate == 200)
    {   state.selmask.entity.clearandset(patch_entity,end_list);
        cadwindow->prompt->normalprompt("Select the triangulated patch to cut from the first patch");
        rbdata->state = 2;
    }
    else if (*cstate == ExitState)
    {   if (rbdata != 0)
        {   if (rbdata->patch1 != 0)
            {   for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                    db.geometry.del(e,1,0);
                rbdata->n = 0;
                rbdata->plist.destroy();
            }
            delete [] rbdata->indicies;
            delete rbdata->copy;
            delete rbdata;
         }
    }
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief patch_command4
 * @param cstate
 * @param event
 *
 * Entry point for the Patch Options command
 *
 */
void patch_command4(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 int xmesh,ymesh,udegree,vdegree;
 BitMask options(32),change(32);
 Entity *e;
 EntityHeader header;
 ResourceString rs4(NCPATCH+4);
 ResourceString rs5(NCPATCH+5);
 int status,length;
 Patch *patch;

  if (*cstate == InitialState)
    {  list = state.getselection().copy();
       if (list.empty())
         {  Patch::getdefaults(&udegree,&vdegree,&xmesh,&ymesh,&options);
            status = Patch::setup(NULL,&udegree,&vdegree,&xmesh,&ymesh,&options);
            if (status == TRUE)
              {  Patch::setdefaults(udegree,vdegree,xmesh,ymesh,options);
                 *cstate = ExitState;
              }
            else if (status == 113)// clone (105->113)
              *cstate = 1;
            else if (status == 114)// clone all (106->114)
              *cstate = 3;
            else if (status == 110)// surface properties
                *cstate = ExitState;
            else if (status == 111)// compare
                *cstate = ExitState;
            else if (status == 112)// triangulate
                *cstate = ExitState;
            else
              *cstate = ExitState;
         }
       else
         {  Patch::getdefaults(&udegree,&vdegree,&xmesh,&ymesh,&options);
            status = Patch::setup(&change,&udegree,&vdegree,&xmesh,&ymesh,&options,&header);
            if (status == TRUE)
            {
                 length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(patch_entity))
                     ((Patch *)e)->change(change,header,udegree,vdegree,xmesh,ymesh,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();
                 *cstate = ExitState;
            }
            else if (status == 113)// clone (105->113)
              *cstate = 2;
            else if (status == 114)// clone all (106->114)
              *cstate = 4;
            else if (status == 110)// surface properties
            {
                //SurfaceProperties sp;
                //sp.verify(this,0);
                *cstate = ExitState;
            }
            else if (status == 111)// compare
            {
                ::state.sendevent(new NewCommandEvent(426,0));
                *cstate = ExitState;
            }
            else if (status == 112)// triangulate
            {
                int length1 = list.length();
                RemoveLockedEntities(&list,0);
                int length2 = list.length();
                if(length2 == 1)
                {
                    list.start();
                    e = list.next();
                    state.destroyselection(1);
                    Patch *tsurf=0;
                    if ((tsurf = ((Patch*)e)->triangulatedclone(1)) != 0)
                    {   db.saveundo(UD_STARTBLOCK,NULL);
                        db.geometry.add(tsurf);
                        db.geometry.del(e);
                        db.saveundo(UD_ENDBLOCK,NULL);
                    }
                }
                else
                    cadwindow->MessageBox("Entities on locked layers cannot be changed","Change",MB_OK);
                *cstate = ExitState;
            }
            else
              *cstate = ExitState;
         }
    }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            patch = (Patch *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  Patch::setdefaults(patch->getudegree(),patch->getvdegree(),patch->getxmesh(),patch->getymesh(),patch->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(patch->getcolour(),patch->getlayer(),patch->getstyle(),patch->getweight());
                      v.setinteger("df::colour",patch->getcolour());
                      v.setinteger("df::layer",patch->getlayer());
                      v.setinteger("df::style",patch->getstyle());
                      v.setinteger("df::weight",patch->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              } 
            else
              {  length = list.length();
                 RemoveLockedEntities(&list,0);
                 if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 4 ; i <= 8 ; i++) change.set(i,1);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(patch_entity))
                     ((Patch *)e)->change(change,*patch,patch->getudegree(),patch->getvdegree(),patch->getxmesh(),patch->getymesh(),patch->getoptions());
                 db.saveundo(UD_ENDBLOCK,NULL);
                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);
                 state.setselection(list,0);
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
    }


  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(patch_entity,end_list);
       cadwindow->prompt->normalprompt("Select the patch entity to clone from");
    }

}

void patch_compare_command(int *cstate,HighLevelEvent *event,void **)
{static Patch *patch1;
 Patch *patch2;   
 Entity *e;
 int i,j;
 Point *triangle,p,pi,p1,p2,p3,normal;
 double t,D,l1,l2,denom;
 EntityList list;

    if (*cstate == InitialState)
    {   list = state.getselection().copy();
        list.start();
        e = list.next();
        list.destroy();
        if (e->isa(patch_entity) && ((Patch *)e)->getoptions().test(PatchTriangulated))
        {   patch1 = (Patch *)e;
            *cstate = 1;
        }
        else
            *cstate = ExitState;
    }
    else if (*cstate == 1)
    {  if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            e = (Patch *)((EntitySelectedEvent *) event)->getentity();
            if (e->isa(patch_entity) && ((Patch *)e)->getoptions().test(PatchTriangulated))
            {   patch2 = (Patch *)e;
            
                patch2->triangulate(0.0);

                for (i = 0 ; i < patch1->npoints() ; i++)
                {   p = patch1->point(i);
                    triangle = patch2->gettriangles();
                    for (j = 0 ; j < patch2->getntriangles() ; j++, triangle += 6)
                    {   if (triangle->z == RCDBL_MAX)
                            triangle += 2;

                        if (triangle[0].x > p.x && triangle[1].x > p.x && triangle[2].x > p.x ||
                            triangle[0].y > p.y && triangle[1].y > p.y && triangle[2].y > p.y ||
                            triangle[0].x < p.x && triangle[1].x < p.x && triangle[2].x < p.x ||
                            triangle[0].y < p.y && triangle[1].y < p.y && triangle[2].y < p.y)
                        {   triangle += 3;
                            continue;  //  Don't do the intersection as the simple min/max test has failed
                        }

                        p1 = *triangle++;
                        p2 = *triangle++;
                        p3 = *triangle++;

                        l1 = (p1 - p3).length();
                        l2 = (p2 - p3).length();
                        normal = (p2 - p1).cross(p3 - p1);
                        t = normal.length();
                        if (t > 1.0E-100)
                            normal /= t;
                        D = -normal.dot(p1);
                        denom = normal.dot(Point(0.0,0.0,1.0));
                        if (fabs(denom) > db.getatoler())
                        {   t = -(p.dot(normal) + D) / denom;
                            pi = Point(0.0,0.0,1.0) * t + p;
                            if ((pi - p1).dot(normal.cross(p2 - p1)) > -1.0E-8 * (l1 + l2) &&
                                (pi - p2).dot(normal.cross(p3 - p2)) > -1.0E-8 * (l1 + l2) &&
                                (pi - p3).dot(normal.cross(p1 - p3)) > -1.0E-8 * (l1 + l2))
                            {   pi.z -= p.z;
                                if (pi.z < -20.0)
                                    patch1->set_colour_map(i,RGB(255,0,0));
                                else if (pi.z < 0.0)
                                    patch1->set_colour_map(i,RGB(-255*pi.z/20.0,255+255*pi.z/20.0,0));
                                else if (pi.z < 20.0)
                                    patch1->set_colour_map(i,RGB(0,255-255*pi.z/20.0,255*pi.z/20.0));
                                else
                                    patch1->set_colour_map(i,RGB(0,0,255));
                                break;
                            }
                        }
                    }
                }
            }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
    }


  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(patch_entity,end_list);
       cadwindow->prompt->normalprompt("Select the patch entity to compare to");
    }

}


#endif
