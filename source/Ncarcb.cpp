
#include "ncwin.h"

void arc_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Arc"));
  *cstate = ExitState;
}

struct RBArc_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    int state;
    Point startp,origin,xaxis,yaxis;
    double starta,sweepa,radius;
};

#ifdef NEW_RUBBERB
void RBArc1(int mode,void *data,Point *p2,View3dSurface *surface)
{
 RBArc_data *rbdata;
 Point xaxis,yaxis,zaxis;
 double a;
  rbdata = (RBArc_data *) data;
  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (rbdata->state == 2)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     (*p2-rbdata->origin).length(),0.0,M_PI * 2.0);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
  else if (rbdata->state == 3)
    {  xaxis = *p2 - rbdata->origin;
       if (xaxis.length() > db.getptoler())
         {  xaxis.normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            yaxis = zaxis.cross(xaxis);
            rbdata->xaxis = xaxis;
            if (yaxis.length() < db.getatoler())
              {  zaxis.setxyz(0.0,0.0,1.0);
                 yaxis = zaxis.cross(xaxis);
                 if (yaxis.length() < db.getatoler())
                   {  zaxis.setxyz(0.0,1.0,0.0);
                      yaxis = zaxis.cross(xaxis);
                   }
              }
            yaxis.normalize();
            rbdata->yaxis = yaxis;
            if (surface != NULL)
              {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                   rbdata->yaxis = - rbdata->yaxis;
              }
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
       a = circle1.angle(*p2);
       Circle circle2(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,a,M_PI_4);

       circle2.draw(DM_INVERT);

    }
  else if (rbdata->state == 4)
    {  yaxis = *p2 - rbdata->origin;  yaxis.normalize();
       zaxis = rbdata->xaxis.cross(yaxis);
       if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
         zaxis = zaxis * -1.0;
       if (zaxis.length() > db.getatoler())
         {  yaxis = zaxis.cross(rbdata->xaxis);
            yaxis.normalize();
            rbdata->yaxis = yaxis;
          }
       if (surface != NULL)
         {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
              rbdata->yaxis = -rbdata->yaxis;
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
       rbdata->starta = circle1.angle(rbdata->startp);
       a = circle1.angle(*p2) - rbdata->starta;
       if (a < 0.0) a += M_PI * 2.0;
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,rbdata->starta,a);

       circle.draw(DM_INVERT);
    }
}
#else
void RBArc1(int,void *data,Point *p2,View3dSurface *surface)
{
 RBArc_data *rbdata;
 Point xaxis,yaxis,zaxis;
 double a;
  rbdata = (RBArc_data *) data;
  if (rbdata->state == 2)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     (*p2-rbdata->origin).length(),0.0,M_PI * 2.0);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
  else if (rbdata->state == 3)
    {  xaxis = *p2 - rbdata->origin;
       if (xaxis.length() > db.getptoler())
         {  xaxis.normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            yaxis = zaxis.cross(xaxis);
            rbdata->xaxis = xaxis;
            if (yaxis.length() < db.getatoler())
              {  zaxis.setxyz(0.0,0.0,1.0);
                 yaxis = zaxis.cross(xaxis);
                 if (yaxis.length() < db.getatoler())
                   {  zaxis.setxyz(0.0,1.0,0.0);
                      yaxis = zaxis.cross(xaxis);
                   }
              }
            yaxis.normalize();
            rbdata->yaxis = yaxis;
            if (surface != NULL)
              {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                   rbdata->yaxis = - rbdata->yaxis;
              }
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
       a = circle1.angle(*p2);
       Circle circle2(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,a,M_PI_4);

       circle2.qgi = rbdata->qgi;circle2.zValue = rbdata->zValue;
       circle2.draw(DM_INVERT);
       rbdata->qgi = circle2.qgi; rbdata->zValue = circle2.zValue;

    }
  else if (rbdata->state == 4)
    {  yaxis = *p2 - rbdata->origin;  yaxis.normalize();
       zaxis = rbdata->xaxis.cross(yaxis);
       if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
         zaxis = zaxis * -1.0;
       if (zaxis.length() > db.getatoler())
         {  yaxis = zaxis.cross(rbdata->xaxis);
            yaxis.normalize();
            rbdata->yaxis = yaxis;  
          }
       if (surface != NULL)
         {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
              rbdata->yaxis = -rbdata->yaxis;
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
       rbdata->starta = circle1.angle(rbdata->startp);
       a = circle1.angle(*p2) - rbdata->starta;
       if (a < 0.0) a += M_PI * 2.0;
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,rbdata->starta,a);

       circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
       circle.draw(DM_INVERT);
       rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
    }
}
#endif

/**
 * @brief arc_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Arc by center circumference, start and end points
 *
 */
void arc_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBArc_data *rbdata = (RBArc_data *) *data;
 Point  xaxis,yaxis,zaxis,p2;
 Entity *e;
 double a;
 int defined;
 View3dSurface *surface;
//
//  Actions to take given the current state and the new event.
//

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBArc_data;  *data = rbdata;
         rbdata->qgi=0;rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
           {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
              rbdata->yaxis.setxyz(0.0,1.0,0.0);
           }
         else
           {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
              rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
           }
         *cstate = 1;
         break;
       case 1 :
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
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              rbdata->radius = (rbdata->origin - p2).length();
              if (rbdata->radius > db.getptoler())
                *cstate = 3;
           }
         else if (event->isa(EntitySelected))
           {  p2 = ((EntitySelectedEvent *) event)->getp();
              e = ((EntitySelectedEvent *) event)->getentity();
              Circle c(rbdata->origin,e,p2);
              if (c.getdefined())
                {  surface = ((EntitySelectedEvent *) event)->getwindow();
                   rbdata->radius = c.getradius();
                   rbdata->xaxis = c.getxaxis();
                   rbdata->yaxis = c.getyaxis();
                   if (surface != NULL)
                     {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                        rbdata->yaxis = -rbdata->yaxis;
                     }
                   *cstate = 3;
                }
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         defined = 0;
         if (event->isa(Coordinate3d))
           {  rbdata->startp = p2 = ((Coordinate3dEvent *) event)->getp();
              surface = ((Coordinate3dEvent *) event)->getwindow();
              defined = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              surface = ((EntitySelectedEvent *) event)->getwindow();
              p2 = ((EntitySelectedEvent *) event)->getp();
              e = ((EntitySelectedEvent *) event)->getentity();
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              Intersect i(&circle1,e,p2);
              if (i.nintersections() == 1)
                {  rbdata->startp = p2 = ((PointE *)i.intersection(0))->getp();
                   defined = 1;
                }
           }
         if (defined && (event->isa(Coordinate3d) || event->isa(EntitySelected)))
           {  xaxis = p2 - rbdata->origin;
              if (xaxis.length() > db.getptoler())
                {  xaxis.normalize();
                   zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                   yaxis = zaxis.cross(xaxis);
                   rbdata->xaxis = xaxis;
                   if (yaxis.length() < db.getatoler())
                     {  zaxis.setxyz(0.0,0.0,1.0);
                        yaxis = zaxis.cross(xaxis);
                        if (yaxis.length() < db.getatoler())
                          {  zaxis.setxyz(0.0,1.0,0.0);
                             yaxis = zaxis.cross(xaxis);
                          }
                     }
                   yaxis.normalize();
                   rbdata->yaxis = yaxis;
                   if (surface != NULL)
                     {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                        rbdata->yaxis = -rbdata->yaxis;
                     }
                }
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              rbdata->starta = circle1.angle(p2);
              *cstate = 4;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 4 :
         rubberband.end(0);
         defined = 0;
         if (event->isa(Coordinate3d))
           {  surface = ((Coordinate3dEvent *) event)->getwindow();
              p2 = ((Coordinate3dEvent *) event)->getp();
              defined = 1;
           }
         else if (event->isa(EntitySelected))
           {  surface = ((EntitySelectedEvent *) event)->getwindow();
              state.destroyselection(1);
              p2 = ((EntitySelectedEvent *) event)->getp();
              e = ((EntitySelectedEvent *) event)->getentity();
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              Intersect i(&circle1,e,p2);
              if (i.nintersections() == 1)
                {  p2 = ((PointE *)i.intersection(0))->getp();
                   defined = 1;
                }
           }
         if (defined && (event->isa(Coordinate3d) || event->isa(EntitySelected)))
           {  yaxis = p2 - rbdata->origin;  yaxis.normalize();
              zaxis = rbdata->xaxis.cross(yaxis);
              if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
                zaxis = zaxis * -1.0;
              if (zaxis.length() > db.getatoler())
                {  yaxis = zaxis.cross(rbdata->xaxis);
                   yaxis.normalize();
                   rbdata->yaxis = yaxis;  
                 }
              if (surface != NULL)
                {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                   rbdata->yaxis = -rbdata->yaxis;
                }
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              rbdata->starta = circle1.angle(rbdata->startp);
              a = circle1.angle(p2) - rbdata->starta;
              if (a < 0.0) a += M_PI * 2.0;
              rbdata->sweepa = a;
              if (rbdata->sweepa < 0.0) rbdata->sweepa += 2.0 * M_PI;
              *cstate = 1;
              Circle *circle = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,rbdata->starta,rbdata->sweepa);
              circle->zValue = rbdata->zValue;
              db.geometry.add(circle);
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
//  Actions to be performed as the new state is about to be entered
//

 if (*cstate == 1)
   state.selmask.entity.clearandset(xyz_mask,end_list);
 else if (*cstate > 1 && *cstate <= 4)
   {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
      rbdata->state = *cstate;
      rubberband.begin(0);
      rubberband.add(RBArc1,rbdata);
   }
 if (*cstate == 1)
   cadwindow->prompt->normalprompt(NCARCB);
 else if (*cstate == 2)
   cadwindow->prompt->normalprompt(NCARCB+1);
 else if (*cstate == 3)
   cadwindow->prompt->normalprompt(NCARCB+2);
 else if (*cstate == 4)
   cadwindow->prompt->normalprompt(NCARCB+3);
 else if (*cstate == ExitState)
   delete rbdata;
}

struct RBArc2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    int state;
    Point startp,origin,xaxis,yaxis,p1,p2;
    double starta,sweepa,radius;
    Entity *e;
};

#ifdef NEW_RUBBERB
void RBArc2(int mode,void *data,Point *p2,View3dSurface *surface)
{
 Point xaxis,yaxis,zaxis;
 RBArc2_data *rbdata = (RBArc2_data *) data;
 double a;
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
       Circle circle(*p2,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);

       circle.draw(DM_INVERT);

    }
  else if (rbdata->state == 2)
    {  xaxis = *p2 - rbdata->origin;
       if (xaxis.length() > db.getptoler())
         {  xaxis.normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            yaxis = zaxis.cross(xaxis);
            rbdata->xaxis = xaxis;
            if (yaxis.length() < db.getatoler())
              {  zaxis.setxyz(0.0,0.0,1.0);
                 yaxis = zaxis.cross(xaxis);
                 if (yaxis.length() < db.getatoler())
                   {  zaxis.setxyz(0.0,1.0,0.0);
                      yaxis = zaxis.cross(xaxis);
                   }
              }
            yaxis.normalize();
            rbdata->yaxis = yaxis;
            if (surface != NULL)
              {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                   rbdata->yaxis = -rbdata->yaxis;
              }
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);
       a = circle1.angle(*p2);
       Circle circle2(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,a,M_PI_4);

       circle2.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {  yaxis = *p2 - rbdata->origin;  yaxis.normalize();
       zaxis = rbdata->xaxis.cross(yaxis);
       if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
         zaxis = zaxis * -1.0;
       if (zaxis.length() > db.getatoler())
         {  yaxis = zaxis.cross(rbdata->xaxis);
            yaxis.normalize();
            rbdata->yaxis = yaxis;
          }
       if (surface != NULL)
         {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
              rbdata->yaxis = -rbdata->yaxis;
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);
       rbdata->starta = circle1.angle(rbdata->startp);
       a = circle1.angle(*p2) - rbdata->starta;
       if (a < 0.0) a += M_PI * 2.0;
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,rbdata->starta,a);

       circle.draw(DM_INVERT);
    }
  else if (rbdata->state == 4)
    {  Circle circle(1,rbdata->radius,rbdata->e,rbdata->p1,*p2);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
}
#else
void RBArc2(int,void *data,Point *p2,View3dSurface *surface)
{
 Point xaxis,yaxis,zaxis;
 RBArc2_data *rbdata = (RBArc2_data *) data;
 double a;
  if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       Circle circle(*p2,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);

       circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
       circle.draw(DM_INVERT);
       rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;

    }
  else if (rbdata->state == 2)
    {  xaxis = *p2 - rbdata->origin;
       if (xaxis.length() > db.getptoler())
         {  xaxis.normalize();
            zaxis = rbdata->xaxis.cross(rbdata->yaxis);
            yaxis = zaxis.cross(xaxis);
            rbdata->xaxis = xaxis;
            if (yaxis.length() < db.getatoler())
              {  zaxis.setxyz(0.0,0.0,1.0);
                 yaxis = zaxis.cross(xaxis);
                 if (yaxis.length() < db.getatoler())
                   {  zaxis.setxyz(0.0,1.0,0.0);
                      yaxis = zaxis.cross(xaxis);
                   }
              }
            yaxis.normalize();
            rbdata->yaxis = yaxis;
            if (surface != NULL)
              {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                   rbdata->yaxis = -rbdata->yaxis;
              }
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);
       a = circle1.angle(*p2);
       Circle circle2(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,a,M_PI_4);

       circle2.qgi = rbdata->qgi; circle2.zValue = rbdata->zValue;
       circle2.draw(DM_INVERT);
       rbdata->qgi = circle2.qgi; rbdata->zValue = circle2.zValue;
    }
  else if (rbdata->state == 3)
    {  yaxis = *p2 - rbdata->origin;  yaxis.normalize();
       zaxis = rbdata->xaxis.cross(yaxis);
       if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
         zaxis = zaxis * -1.0;
       if (zaxis.length() > db.getatoler())
         {  yaxis = zaxis.cross(rbdata->xaxis);
            yaxis.normalize();
            rbdata->yaxis = yaxis;  
          }
       if (surface != NULL)
         {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
              rbdata->yaxis = -rbdata->yaxis;
         }
       Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,0.0,M_PI * 2.0);
       rbdata->starta = circle1.angle(rbdata->startp);
       a = circle1.angle(*p2) - rbdata->starta;
       if (a < 0.0) a += M_PI * 2.0;
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                     rbdata->radius,rbdata->starta,a);

       circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
       circle.draw(DM_INVERT);
       rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
    }
  else if (rbdata->state == 4)
    {  Circle circle(1,rbdata->radius,rbdata->e,rbdata->p1,*p2);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
}
#endif

/**
 * @brief arc_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Arc by center, radius, start and end points
 *
 */
void arc_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBArc2_data *rbdata = (RBArc2_data *) *data;
 Point  p,p2,xaxis,yaxis,zaxis;
 int defined;
 double a;
 Entity *e;
 Dialog *dialog;
 Circle *circle;
 View3dSurface *surface;
 ResourceString rs4(NCARCB+4);
 ResourceString rs5(NCARCB+5);

//
//  Actions to take given the current state and the new event.
//

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBArc2_data) != NULL)
         {
             rbdata->qgi=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
             *data = rbdata;
              rbdata->radius = v.getreal("ar::radius");
              dialog = new Dialog(rs4.gets(),rs5.gets(),&rbdata->radius,ScaleByLength);
              if (dialog == NULL || dialog->process() == FALSE)
                *cstate = ExitState;
              else
                {  v.setreal("ar::radius",rbdata->radius);
                   *cstate = 1;
                }
              delete dialog;
         }
         else
           *cstate = ExitState;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              *cstate = 4;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         defined = 0;
         if (event->isa(Coordinate3d))
           {  rbdata->startp = p2 = ((Coordinate3dEvent *) event)->getp();
              surface = ((Coordinate3dEvent *) event)->getwindow();
              defined = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              p2 = ((EntitySelectedEvent *) event)->getp();
              e = ((EntitySelectedEvent *) event)->getentity();
              surface = ((EntitySelectedEvent *) event)->getwindow();
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              Intersect i(&circle1,e,p2);
              if (i.nintersections() == 1)
                {  rbdata->startp = p2 = ((PointE *)i.intersection(0))->getp();
                   defined = 1;
                }
           }
         if (defined && (event->isa(Coordinate3d) || event->isa(EntitySelected)))
           {  xaxis = p2 - rbdata->origin;
              if (xaxis.length() > db.getptoler())
                {  xaxis.normalize();
                   zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                   yaxis = zaxis.cross(xaxis);
                   rbdata->xaxis = xaxis;
                   if (yaxis.length() < db.getatoler())
                     {  zaxis.setxyz(0.0,0.0,1.0);
                        yaxis = zaxis.cross(xaxis);
                        if (yaxis.length() < db.getatoler())
                          {  zaxis.setxyz(0.0,1.0,0.0);
                             yaxis = zaxis.cross(xaxis);
                          }
                     }
                   yaxis.normalize();
                   rbdata->yaxis = yaxis;
                } 
              if (surface != NULL)
                {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                     rbdata->yaxis = -rbdata->yaxis;
                }
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              rbdata->starta = circle1.angle(p2);
              *cstate = 3;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         defined = 0;
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              surface = ((Coordinate3dEvent *) event)->getwindow();
              defined = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              p2 = ((EntitySelectedEvent *) event)->getp();
              e = ((EntitySelectedEvent *) event)->getentity();
              surface = ((EntitySelectedEvent *) event)->getwindow();
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              Intersect i(&circle1,e,p2);
              if (i.nintersections() == 1)
                {  p2 = ((PointE *)i.intersection(0))->getp();
                   defined = 1;
                }
           }
         if (defined && (event->isa(Coordinate3d) || event->isa(EntitySelected)))
           {  yaxis = p2 - rbdata->origin;  yaxis.normalize();
              zaxis = rbdata->xaxis.cross(yaxis);
              if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
                zaxis = zaxis * -1.0;
              if (zaxis.length() > db.getatoler())
                {  yaxis = zaxis.cross(rbdata->xaxis);
                   yaxis.normalize();
                   rbdata->yaxis = yaxis;  
                 }
              if (surface != NULL)
                {  if ((rbdata->xaxis.cross(rbdata->yaxis)).dot(surface->getuaxis().cross(surface->getvaxis())) < 0.0)
                     rbdata->yaxis = -rbdata->yaxis;
                }
              Circle circle1(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,2.0 * M_PI);
              rbdata->starta = circle1.angle(rbdata->startp);
              a = circle1.angle(p2) - rbdata->starta;
              if (a < 0.0) a += M_PI * 2.0;
              rbdata->sweepa = a;
              if (rbdata->sweepa < 0.0) rbdata->sweepa += 2.0 * M_PI;
              *cstate = 1;
              Circle *circle = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->radius,rbdata->starta,rbdata->sweepa);
              if (circle->getdefined())
              {
                  circle->zValue = rbdata->zValue;
                db.geometry.add(circle);
              }
              else
                delete circle;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 4 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              Circle *circle = new Circle(1,rbdata->radius,rbdata->e,rbdata->p1,rbdata->p2);
              if (! circle->getdefined())
                delete circle;
              else
              {
                  circle->zValue = rbdata->zValue;
                db.geometry.add(circle);
              }
              rbdata->e->draw(DM_NORMAL);
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              circle = new Circle(1,rbdata->radius,rbdata->e,rbdata->p1,e,p);
              if (! circle->getdefined())
                delete circle;
              else
              {
                  circle->zValue = rbdata->zValue;
                db.geometry.add(circle);
              }
              if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {  if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = ExitState;
           }
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
//  Actions to be performed as the new state is about to be entered
//

 if (*cstate >= 1)
   {  rbdata->state = *cstate;
      rubberband.begin(0);
      rubberband.add(RBArc2,rbdata);
   }
 if (*cstate == 1)
   {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
      cadwindow->prompt->normalprompt(NCARCB+6);
   }
 else if (*cstate == 2)
   {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
      cadwindow->prompt->normalprompt(NCARCB+7);
   }
 else if (*cstate == 3)
   {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
      cadwindow->prompt->normalprompt(NCARCB+8);
   }
 else if (*cstate == 4)
   {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
      cadwindow->prompt->normalprompt(NCARCB+17);
   }
 else if (*cstate == ExitState)
   delete rbdata;
}

struct RBArc3_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcircle;
    QGraphicsItem *qgLine;
#endif
    int state;
   Point p1,p2;
   Entity *e1,*e2;
};

#ifdef NEW_RUBBERB
void RBArc3(int mode,void *data,Point *p3,View3dSurface *)
{
  RBArc3_data *rbdata = (RBArc3_data *) data;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (rbdata->state == 2)
    {Line line(rbdata->p1,*p3);

       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {Circle circle(1,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,*p3);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
}
#else
void RBArc3(int ,void *data,Point *p3,View3dSurface *)
{RBArc3_data *rbdata = (RBArc3_data *) data;
  if (rbdata->state == 2)
    {Line line(rbdata->p1,*p3);

       line.qgi = rbdata->qgLine; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgLine = line.qgi; rbdata->zValue = line.zValue;
    }
  else if (rbdata->state == 3)
    {Circle circle(1,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,*p3);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgcircle; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgcircle = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
}
#endif

/**
 * @brief arc_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Arc by start, circumference and end points
 *
 */
void arc_command3(int *cstate,HighLevelEvent *event,void **data)
{

 RBArc3_data *rbdata = (RBArc3_data *) *data;
 Point  p;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBArc3_data; rbdata->qgcircle=0; rbdata->qgLine=0;
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE); *data = rbdata;
         *cstate = 1; 
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->e1 = NULL;
              rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e1 = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->e1->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);     
         if (event->isa(Coordinate3d))
           {  rbdata->e2 = NULL;
              rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e2 = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p2 = ((EntitySelectedEvent *) event)->getp();
              rbdata->e2->highlight();
              *cstate = 3;
           }
         else if (event->isa(Abort))
           {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
              *cstate = ExitState;
           }
         break;
       case 3 :
         rubberband.end(0);     
         if (event->isa(Coordinate3d))
           {  Circle *circle = new Circle(1,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,((Coordinate3dEvent *) event)->getp());
              if (circle->getdefined())
                {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
                   if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
                   circle->zValue = rbdata->zValue;
                   db.geometry.add(circle);
                   *cstate = 1;
                }
               else
                   delete circle;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              Circle *circle = new Circle(1,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,
                                          ((EntitySelectedEvent *) event)->getentity(),((EntitySelectedEvent *) event)->getp());
              if (circle->getdefined())
                {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
                   if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
                   circle->zValue = rbdata->zValue;
                   db.geometry.add(circle);
                   *cstate = 1;
                }
               else
                   delete circle;
           }
         else if (event->isa(Abort))
           {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
              if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
              if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
              *cstate = ExitState;
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcircle)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcircle);
  delete rbdata->qgcircle; rbdata->qgcircle=0;
  if(rbdata->qgLine)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgLine);
  delete rbdata->qgLine; rbdata->qgLine=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate >= 1 && *cstate <= 3)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBArc3,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCARCB+9);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCARCB+10);
  else if (*cstate == 3)
    cadwindow->prompt->normalprompt(NCARCB+11);
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBArc4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcircle;
    QGraphicsItem *qgLine;
#endif
   Point p1,p2;
   double radius;
   Entity *e;
   int state;
};

#ifdef NEW_RUBBERB
void RBArc4(int mode,void *data,Point *p2,View3dSurface *)
{RBArc4_data *rbdata = (RBArc4_data *) data;

    if(mode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
  if (rbdata->state == 2)
    {  if (rbdata->e == NULL)
         {  Line line(rbdata->p1,*p2);

            line.draw(DM_INVERT);
         }
       else
         {  Circle circle(1,rbdata->radius,rbdata->e,rbdata->p1,*p2);
            if (circle.getdefined())
            {
                circle.draw(DM_INVERT);
            }
         }
    }
  else if (rbdata->state == 3)
    {  Circle circle(1,rbdata->radius,rbdata->p1,rbdata->p2,*p2);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
}
#else
void RBArc4(int,void *data,Point *p2,View3dSurface *)
{RBArc4_data *rbdata = (RBArc4_data *) data;
  if (rbdata->state == 2)
    {  if (rbdata->e == NULL)
         {  Line line(rbdata->p1,*p2);

            line.qgi = rbdata->qgLine; line.zValue = rbdata->zValue;
            line.draw(DM_INVERT);
            rbdata->qgLine = line.qgi; rbdata->zValue = line.zValue;
         }
       else
         {  Circle circle(1,rbdata->radius,rbdata->e,rbdata->p1,*p2);
            if (circle.getdefined())
            {
                circle.qgi = rbdata->qgcircle; circle.zValue = rbdata->zValue;
                circle.draw(DM_INVERT);
                rbdata->qgcircle = circle.qgi; rbdata->zValue = circle.zValue;
            }
         }
    }
  else if (rbdata->state == 3)
    {  Circle circle(1,rbdata->radius,rbdata->p1,rbdata->p2,*p2);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgcircle; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgcircle = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
}
#endif

void arc_trimtangent(Circle *arc,LinearEntity *e)
{
 Point dir,p;
 if ((e->nearp(arc->start())-arc->start()).length() <
       (e->nearp(arc->end())-arc->end()).length())
    {  dir = arc->direction(0.0) * -1.0;  p = arc->start();
    }
  else
    {  dir = arc->direction(1.0);  p = arc->end();
    }
  if (e->isa(line_entity))
    {  if (dir.dot(((Line *)e)->getp1()) < dir.dot(((Line *)e)->getp2()))
         e->moveendpoint(1,1,p);
       else
         e->moveendpoint(1,2,p);
    }
  else if (e->isa(circle_entity))
    {Circle *c = (Circle *) e;
       if (dir.dot((p-c->getorigin()).cross(c->getxaxis().cross(c->getyaxis()))) < 0.0)
         e->moveendpoint(1,1,p);
       else
         e->moveendpoint(1,2,p);
    }
  else if (e->isa(curve_entity))
    {Curve *c = (Curve *) e;
     double t;
       c->nearp(0,p,&t);
       if (t >= 0.0)
         {  if (dir.dot(c->direction(t)) > 0.0)
              e->moveendpoint(1,1,p);
            else
              e->moveendpoint(1,2,p);
         }
    }
}

/**
 * @brief arc_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert Fillet Arc command
 *
 */
void arc_command4(int *cstate,HighLevelEvent *event,void **data)
{
 RBArc4_data *rbdata = (RBArc4_data *) *data;
 Point  dir1,dir2,p,pint,p1,p2;
 int status;
 Entity *e;
 Circle *circle;  
 Dialog *dialog;
 ResourceString rs12(NCARCB+12),rs13(NCARCB+13);

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBArc4_data; rbdata->qgcircle=0; rbdata->qgLine=0;
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE); *data = rbdata;
         if (rbdata != NULL)
           {  rbdata->radius = v.getreal("ar::radius");
              if ((dialog = new Dialog(rs12.gets(),rs13.gets(),&rbdata->radius,ScaleByLength)) != NULL)
                {  status = dialog->process();
                   delete dialog;
                   if (status == TRUE)
                     {  v.setreal("ar::radius",rbdata->radius);
                        *cstate = 1;
                     }
                   else
                     *cstate = ExitState;
                }
              else
                *cstate = ExitState;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  state.destroyselection(1);
              rbdata->e = NULL;
              rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->e->draw(DM_SELECT);
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              if (rbdata->e == NULL)
                *cstate = 3;
              else
                {  Circle *circle = new Circle(1,rbdata->radius,rbdata->e,rbdata->p1,rbdata->p2);
                   if (! circle->getdefined())
                     delete circle;
                   else
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        arc_trimtangent(circle,(LinearEntity *) rbdata->e);
                        circle->zValue = rbdata->zValue;
                        db.geometry.add(circle);
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                   rbdata->e->draw(DM_NORMAL);
                   *cstate = 1;
                }
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              if (rbdata->e == NULL)
                circle = new Circle(1,rbdata->radius,e,p,rbdata->p1);
              else
                circle = new Circle(1,rbdata->radius,rbdata->e,rbdata->p1,e,p);
              if (! circle->getdefined())
                delete circle;
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   if (rbdata->e != NULL) arc_trimtangent(circle,(LinearEntity *)rbdata->e);
                   arc_trimtangent(circle,(LinearEntity *) e);
                   circle->zValue = rbdata->zValue;
                   db.geometry.add(circle);
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
              if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {  if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  if (rbdata->e != NULL) rbdata->e->draw(DM_NORMAL);
              *cstate = ExitState;
           }
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              circle = new Circle(1,rbdata->radius,rbdata->p1,rbdata->p2,p);
              if (! circle->getdefined())
                delete circle;
              else
              {
                  circle->zValue = rbdata->zValue;
                db.geometry.add(circle);
              }
              *cstate = 1;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcircle)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcircle);
  delete rbdata->qgcircle; rbdata->qgcircle=0;
  if(rbdata->qgLine)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgLine);
  delete rbdata->qgLine; rbdata->qgLine=0;
#endif
#endif
//
//  About to enter state actions
//
  rbdata->state = *cstate;
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
  else if (*cstate == 3)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCARCB+14);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBArc4,rbdata);
       cadwindow->prompt->normalprompt(NCARCB+15);
    }
  else if (*cstate == 3)
    {  rubberband.begin(0);
       rubberband.add(RBArc4,rbdata);
       cadwindow->prompt->normalprompt(NCARCB+16);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}
