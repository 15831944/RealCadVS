

#include "ncwin.h"

struct RBEllipse1_data
  {
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    int state;
   double major,minor;
   Point origin,xaxis,yaxis,majp;
  };

#ifdef NEW_RUBBERB
void RBEllipse1(int mode,void *data,Point *p2,View3dSurface *surface)
{
 RBEllipse1_data *rbdata = (RBEllipse1_data *) data;
 Point xaxis,yaxis,zaxis;
 double aspect,major,minor;

 if(mode = RB_ERASE)
     cadwindow->clearAllRubberBands();

 if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       EllipseE ellipse(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->major,rbdata->minor,0.0,M_PI*2.0);
       if (ellipse.getdefined())
       {
            ellipse.draw(DM_INVERT);
       }
    }
  else if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->origin;
       aspect = rbdata->major / rbdata->minor;
       major = xaxis.length();
       minor = major / aspect;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  EllipseE ellipse(rbdata->origin,xaxis,yaxis,major,minor,0.0,M_PI * 2.0);
            if (ellipse.getdefined())
            {
                ellipse.draw(DM_INVERT);
            }
         }
    }
  else if (rbdata->state == 3)
    {  EllipseE ellipse(rbdata->origin,rbdata->majp,*p2);
       if (ellipse.getdefined())
       {
           ellipse.draw(DM_INVERT);
       }
    }
}
#else
void RBEllipse1(int,void *data,Point *p2,View3dSurface *surface)
{
 RBEllipse1_data *rbdata = (RBEllipse1_data *) data;
 Point xaxis,yaxis,zaxis;
 double aspect,major,minor;
  if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       EllipseE ellipse(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->major,rbdata->minor,0.0,M_PI*2.0);
       if (ellipse.getdefined())
       {
           ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
            ellipse.draw(DM_INVERT);
           rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
       }
    }
  else if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->origin;
       aspect = rbdata->major / rbdata->minor;
       major = xaxis.length();
       minor = major / aspect;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  EllipseE ellipse(rbdata->origin,xaxis,yaxis,major,minor,0.0,M_PI * 2.0);
            if (ellipse.getdefined())
            {
                ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
                ellipse.draw(DM_INVERT);
                rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
            }
         }
    }
  else if (rbdata->state == 3)
    {  EllipseE ellipse(rbdata->origin,rbdata->majp,*p2);
       if (ellipse.getdefined())
       {
           ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
           ellipse.draw(DM_INVERT);
           rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
       }
    }
}
#endif

/**
 * @brief ellipse_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Ellipse by centre, maor axis and circumference point command
 *
 */
void ellipse_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBEllipse1_data *rbdata = (RBEllipse1_data *) *data;
 Point  p,xaxis,yaxis,zaxis;
 ResourceString rs0(NCELLIPB),rs1(NCELLIPB+1), rs2(NCELLIPB+2),rs3(NCELLIPB+3);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBEllipse1_data;  *data = rbdata;
          rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         if (rbdata == NULL)
           {  state.fail(Memory,rs0.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->major = v.getreal("el::major");
              rbdata->minor = v.getreal("el::minor");
              if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
                   rbdata->yaxis.setxyz(0.0,1.0,0.0);
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              *cstate = 1;
           }
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->majp = ((Coordinate3dEvent *) event)->getp();
              xaxis = rbdata->majp - rbdata->origin;
              if (xaxis.length() > db.getptoler())
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
           {  p = ((Coordinate3dEvent *) event)->getp();
              EllipseE *ellipse = new EllipseE(rbdata->origin,rbdata->majp,p);
              if (ellipse != NULL && ellipse->getdefined())
              {
                ellipse->zValue = rbdata->zValue;
                db.geometry.add(ellipse);
              }
              else
                delete ellipse;
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
  cadwindow->clearAllRubberBands();
#else
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rbdata->state = 1;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBEllipse1,rbdata);
       cadwindow->prompt->normalprompt(rs1.gets());
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBEllipse1,rbdata);
       cadwindow->prompt->normalprompt(rs2.gets());
    }
  else if (*cstate == 3)
    {  rbdata->state = 3;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBEllipse1,rbdata);
       cadwindow->prompt->normalprompt(rs3.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBEllipse2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    int state;
   double major,minor;
   Point origin,xaxis,yaxis;
};

#ifdef NEW_RUBBERB
void RBEllipse2(int mode,void *data,Point *p2,View3dSurface *surface)
{RBEllipse2_data *rbdata = (RBEllipse2_data *) data;
 Point xaxis,yaxis,zaxis;

 if(mode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       EllipseE ellipse(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->major,rbdata->minor,0.0,M_PI*2.0);
       if (ellipse.getdefined())
       {
           //ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
           ellipse.draw(DM_INVERT);
           //rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
       }
    }
  else if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->origin;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();
       if (xaxis.length() > 1.0E-10 && yaxis.length() >1.0E-10)
         {  EllipseE ellipse(rbdata->origin,xaxis,yaxis,rbdata->major,rbdata->minor,0.0,M_PI * 2.0);
            if (ellipse.getdefined())
            {
                //ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
                ellipse.draw(DM_INVERT);
                //rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
            }
         }
    }
}
#else
void RBEllipse2(int,void *data,Point *p2,View3dSurface *surface)
{RBEllipse2_data *rbdata = (RBEllipse2_data *) data;
 Point xaxis,yaxis,zaxis;
  if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       EllipseE ellipse(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->major,rbdata->minor,0.0,M_PI*2.0);
       if (ellipse.getdefined())
       {
           ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
           ellipse.draw(DM_INVERT);
           rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
       }
    }
  else if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->origin;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();
       if (xaxis.length() > 1.0E-10 && yaxis.length() >1.0E-10)
         {  EllipseE ellipse(rbdata->origin,xaxis,yaxis,rbdata->major,rbdata->minor,0.0,M_PI * 2.0);
            if (ellipse.getdefined())
            {
                ellipse.qgi = rbdata->qgi; ellipse.zValue = rbdata->zValue;
                ellipse.draw(DM_INVERT);
                rbdata->qgi = ellipse.qgi; rbdata->zValue = ellipse.zValue;
            }
         }
    }
}
#endif

/**
 * @brief ellipse_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert ellipse by centre,
 * major and minor axis length and major axis point command
 *
 */
void ellipse_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBEllipse2_data *rbdata = (RBEllipse2_data *) *data;
 Point  p,xaxis,yaxis,zaxis;
 Dialog dialog(_RCT("EllipseInsert_Dialog"));
 DialogControl *dc;
 ResourceString rs4(NCELLIPB+4),rs5(NCELLIPB+5), rs6(NCELLIPB+6);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBEllipse2_data; rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         if (rbdata == NULL)
           {  state.fail(Memory,rs4.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->major = v.getreal("el::major");
              rbdata->minor = v.getreal("el::minor");
              if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
                   rbdata->yaxis.setxyz(0.0,1.0,0.0);
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              dialog.add(dc = new RealDialogControl(100,&rbdata->major,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(dc = new RealDialogControl(101,&rbdata->minor,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              if (dialog.process() == FALSE)
                *cstate = ExitState;
              else
                {  v.setreal("el::major",rbdata->major);
                   v.setreal("el::minor",rbdata->minor);
                   *cstate = 1;
                }
           }
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              xaxis = p - rbdata->origin;
              yaxis = zaxis.cross(xaxis);
              xaxis.normalize();  yaxis.normalize();
              if (xaxis.length() > 1.0E-10 && yaxis.length() >1.0E-10)
                {  EllipseE *ellipse = new EllipseE(rbdata->origin,xaxis,yaxis,rbdata->major,rbdata->minor,0.0,2.0 * M_PI);
                   if (ellipse != NULL && ellipse->getdefined())
                   {
                     ellipse->zValue = rbdata->zValue;
                     db.geometry.add(ellipse);
                   }
                   else
                     delete ellipse;
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgi)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
  delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rbdata->state = 1;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBEllipse2,rbdata);
       cadwindow->prompt->normalprompt(rs5.gets());
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBEllipse2,rbdata);
       cadwindow->prompt->normalprompt(rs6.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}
