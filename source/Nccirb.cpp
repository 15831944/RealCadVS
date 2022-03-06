
#include "ncwin.h"

void circle_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Circle"));
  *cstate = ExitState;
}

struct RBCircle1_data
{
#ifdef USING_WIDGETS
   qreal zValue;
   QGraphicsItem *qgcircle;
   QGraphicsItem *qgtext;
#endif

    Point origin,xaxis,yaxis,offset;
   double lasttextheight;
};

#ifdef NEW_RUBBERB
void RBCircle1(int dm,void *data,Point *p2,View3dSurface *v)
{RBCircle1_data *rbdata = (RBCircle1_data *) data;
 BitMask toptions(32);
 RCCHAR text[300];


 if(dm == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                (*p2-rbdata->origin).length(),0.0,M_PI*2.0);
  if (circle.getdefined())
  {
      circle.draw(DM_INVERT);
  }
  if (v != 0)
  {
       rbdata->lasttextheight = 6.0 * v->getscale();
       rbdata->offset = v->getuaxis() * 6.0 * v->getscale() +  v->getvaxis() * 6.0 * v->getscale();
  }

  strcpy(text,db.formatnumber(text,circle.getradius(),0));
  Text rtext(CHAR2RCCHR("Arial"),*p2 + rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
  if (::v.getinteger("wn::distancetracking"))
  {
      rtext.draw(DM_INVERT);
  }
}
#else
void RBCircle1(int dm,void *data,Point *p2,View3dSurface *v)
{RBCircle1_data *rbdata = (RBCircle1_data *) data;
 BitMask toptions(32);
 RCCHAR text[300];

 if(dm == RB_ERASE)
 {
     delete rbdata->qgcircle;
     rbdata->qgcircle=0;

     delete rbdata->qgtext;
     rbdata->qgtext=0;

     return;
 }
  Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                (*p2-rbdata->origin).length(),0.0,M_PI*2.0);
  if (circle.getdefined())
  {
      circle.qgi = rbdata->qgcircle; circle.zValue = rbdata->zValue;

      circle.draw(DM_INVERT);

      rbdata->qgcircle = circle.qgi; rbdata->zValue = circle.zValue;
  }
  if (v != 0)
    {  rbdata->lasttextheight = 6.0 * v->getscale();
       rbdata->offset = v->getuaxis() * 6.0 * v->getscale() +  v->getvaxis() * 6.0 * v->getscale();
    }

  strcpy(text,db.formatnumber(text,circle.getradius(),0));
  Text rtext(CHAR2RCCHR("Arial"),*p2 + rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
  if (::v.getinteger("wn::distancetracking"))
  {
      rtext.qgi = rbdata->qgtext; rtext.zValue = rbdata->zValue;

      rtext.draw(DM_INVERT);

      rbdata->qgtext = rtext.qgi; rbdata->zValue = rtext.zValue;
  }
}
#endif

/**
 * @brief circle_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert circle command
 *
 */
void circle_command1(int *cstate,HighLevelEvent *event,void **data)
{RBCircle1_data *rbdata = (RBCircle1_data *) *data;
 Point  p2;
 Entity *e;
 ResourceString rs0(NCCIRB),rs1(NCCIRB+1);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBCircle1_data;  *data = rbdata;
         rbdata->qgcircle=0; rbdata->qgtext=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;

         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              if ((p2-rbdata->origin).length() < db.getptoler())
                cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONSTOP);
              else
                {
                  Circle *circle = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,
                                               (p2-rbdata->origin).length(),0.0,M_PI*2.0);
                   if (circle->getdefined())
                   {
                       circle->qgi = rbdata->qgcircle;
                       circle->zValue = rbdata->zValue;
                       db.geometry.add(circle);
                       rbdata->qgcircle = 0;
                   }
                    else
                        delete circle;
                }
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              p2 = ((EntitySelectedEvent *)event)->getp();
              e = ((EntitySelectedEvent *)event)->getentity();
              Circle *circle = new Circle(rbdata->origin,e,p2);
              if (circle->getdefined())
              {
                  circle->qgi = rbdata->qgcircle;
                  circle->zValue = rbdata->zValue;
                  db.geometry.add(circle);
                  rbdata->qgcircle = 0;
              }
               else
                   delete circle;
              *cstate = 1;
           }
         else if (event->isa(Abort))
         {
           *cstate = 1;
         }
         else if (event->isa(Exit))
           *cstate = ExitState;

         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgcircle)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcircle);
             rbdata->qgcircle;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             rbdata->qgtext;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCCIRB+2);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       rubberband.begin(0);
       rubberband.add(RBCircle1,rbdata);
       cadwindow->prompt->normalprompt(NCCIRB+3);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBCircle2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
   int state;
   Entity *e;
   double radius;
   Point p,xaxis,yaxis;
};

#ifdef NEW_RUBBERB
void RBCircle2(int mode,void *data,Point *p2,View3dSurface *surface)
{RBCircle2_data *rbdata = (RBCircle2_data *) data;

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
       Circle circle(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,M_PI*2.0);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
  else if (rbdata->state == 2)
    {  Circle circle(0,rbdata->radius,rbdata->e,rbdata->p,*p2);
       if (circle.getdefined())
       {
           circle.draw(DM_INVERT);
       }
    }
}
#else
void RBCircle2(int,void *data,Point *p2,View3dSurface *surface)
{RBCircle2_data *rbdata = (RBCircle2_data *) data;
  if (rbdata->state == 1)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       Circle circle(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->radius,0.0,M_PI*2.0);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
  else if (rbdata->state == 2)
    {  Circle circle(0,rbdata->radius,rbdata->e,rbdata->p,*p2);
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
 * @brief circle_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert circle by radius command
 *
 */
void circle_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBCircle2_data *rbdata = (RBCircle2_data *) *data;
 Point  p;
 Entity *e;
 Dialog *dialog;
 ResourceString rs4(NCCIRB+4),rs5(NCCIRB+5);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBCircle2_data; rbdata->qgi=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         rbdata->radius = v.getreal("ar::radius");
         if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
           {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
              rbdata->yaxis.setxyz(0.0,1.0,0.0);
           }
         else
           {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
              rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
           }
         dialog = new Dialog(rs4.gets(),rs5.gets(),&rbdata->radius,ScaleByLength);
         if (dialog->process() == FALSE)
           *cstate = ExitState;
         else
           {  v.setreal("ar::radius",rbdata->radius);
              *cstate = 1;
           }
         delete dialog;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              Circle *circle = new Circle(p,rbdata->xaxis,rbdata->yaxis,
                                          rbdata->radius,0.0,M_PI*2.0);
              if (circle->getdefined())
              {
                  circle->zValue = rbdata->zValue;
                  db.geometry.add(circle);
              }
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->p = ((EntitySelectedEvent *)event)->getp();
              rbdata->e = ((EntitySelectedEvent *)event)->getentity();
              rbdata->e->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         rbdata->e->unhighlight();
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              Circle *circle = new Circle(0,rbdata->radius,rbdata->e,rbdata->p,p);
              if (circle->getdefined())
              {
                  circle->zValue = rbdata->zValue;
                  db.geometry.add(circle);
              }
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              p = ((EntitySelectedEvent *)event)->getp();
              e = ((EntitySelectedEvent *)event)->getentity();
              if (e != rbdata->e)
                {  Circle *circle = new Circle(0,rbdata->radius,rbdata->e,rbdata->p,e,p);
                   if (circle->getdefined())
                   {
                       circle->zValue = rbdata->zValue;
                       db.geometry.add(circle);
                   }
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
  rbdata->state = *cstate;
  if (*cstate == 1 || *cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
       rubberband.begin(0);
       rubberband.add(RBCircle2,rbdata);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCCIRB+6);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCCIRB+7);
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBCircle3_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcircle;
    QGraphicsItem *qgline;
#endif
   int state;
   Point p1,p2;
   Entity *e1,*e2;
};

#ifdef NEW_RUBBERB
void RBCircle3(int mode,void *data,Point *p3,View3dSurface *)
{
  RBCircle3_data *rbdata = (RBCircle3_data *) data;
  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (rbdata->state == 2)
  {
       Line line(rbdata->p1,*p3);
       //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
  }
  else if (rbdata->state == 3)
  {
      Circle circle(0,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,*p3);
       if (circle.getdefined())
       {
           //circle.qgi = rbdata->qgcircle; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           //rbdata->qgcircle = circle.qgi; rbdata->zValue = circle.zValue;
       }
  }
}
#else
void RBCircle3(int ,void *data,Point *p3,View3dSurface *)
{RBCircle3_data *rbdata = (RBCircle3_data *) data;
  if (rbdata->state == 2)
    {
       Line line(rbdata->p1,*p3);
       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
    }
  else if (rbdata->state == 3)
    {Circle circle(0,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,*p3);
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
 * @brief circle_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert circle by 3 points command
 *
 */
void circle_command3(int *cstate,HighLevelEvent *event,void **data)
{
 RBCircle3_data *rbdata = (RBCircle3_data *) *data;
 Point  p;
 Entity *e;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBCircle3_data; rbdata->qgcircle=0; rbdata->qgline=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         *cstate = 1; 
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->e1 = NULL;
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->e1 = ((EntitySelectedEvent *) event)->getentity();
              rbdata->e1->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);     
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              rbdata->e2 = NULL;
              *cstate = 3;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->p2 = ((EntitySelectedEvent *) event)->getp();
              rbdata->e2 = ((EntitySelectedEvent *) event)->getentity();
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
           {  Circle *circle = new Circle(0,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,NULL,((Coordinate3dEvent *) event)->getp());
              if (circle->getdefined())
                {
                   circle->zValue = rbdata->zValue;
                   db.geometry.add(circle);
                   *cstate = 1;
                   if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
                   if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
                }
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              Circle *circle = new Circle(0,rbdata->e1,rbdata->p1,rbdata->e2,rbdata->p2,e,p);
              if (circle->getdefined())
                {
                   circle->zValue = rbdata->zValue;
                   db.geometry.add(circle);
                   *cstate = 1;
                   if (rbdata->e1 != NULL) rbdata->e1->unhighlight();
                   if (rbdata->e2 != NULL) rbdata->e2->unhighlight();
                }
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
  if(rbdata->qgline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
  delete rbdata->qgline; rbdata->qgline=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate > 1)
    {  rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBCircle3,rbdata);
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCCIRB+8);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCCIRB+9);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCCIRB+10);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBCircle4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
   Entity *e;
   Point p1,xaxis,yaxis;
};

#ifdef NEW_RUBBERB
void RBCircle4(int mode,void *data,Point *p2,View3dSurface *)
{
 RBCircle4_data *rbdata = (RBCircle4_data *) data;
 Point p3;

 if(mode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  if (rbdata->e == 0)
  {
      Circle circle((rbdata->p1+*p2)/2.0,rbdata->xaxis,rbdata->yaxis,
                   (*p2-rbdata->p1).length()/2.0,0.0,M_PI*2.0);
       if (circle.getdefined())
       {
           //circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           //rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
  }
  else
  {
       Line line(*p2,rbdata->e->nearp(*p2));
       p3 = line.nearp(rbdata->p1);
       Circle circle((*p2+rbdata->e->nearp(p3))/2.0,rbdata->xaxis,rbdata->yaxis,
                                               (rbdata->e->nearp(p3)-*p2).length()/2.0,0.0,M_PI*2.0);
       if (circle.getdefined())
       {
           //circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           //rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
  }
}
#else
void RBCircle4(int,void *data,Point *p2,View3dSurface *)
{RBCircle4_data *rbdata = (RBCircle4_data *) data;
 Point p3;
  if (rbdata->e == 0)
    {Circle circle((rbdata->p1+*p2)/2.0,rbdata->xaxis,rbdata->yaxis,
                   (*p2-rbdata->p1).length()/2.0,0.0,M_PI*2.0);
       if (circle.getdefined())
       {
           circle.qgi = rbdata->qgi; circle.zValue = rbdata->zValue;
           circle.draw(DM_INVERT);
           rbdata->qgi = circle.qgi; rbdata->zValue = circle.zValue;
       }
    }
  else
    {  Line line(*p2,rbdata->e->nearp(*p2));
       p3 = line.nearp(rbdata->p1);
       Circle circle((*p2+rbdata->e->nearp(p3))/2.0,rbdata->xaxis,rbdata->yaxis,
                                               (rbdata->e->nearp(p3)-*p2).length()/2.0,0.0,M_PI*2.0);
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
 * @brief circle_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for insert circle by diameter
 *
 */
void circle_command4(int *cstate,HighLevelEvent *event,void **data)
{RBCircle4_data *rbdata = (RBCircle4_data *) *data;
 Point  p2,p3;
 Entity *e;
 ResourceString rs11(NCCIRB+11),rs12(NCCIRB+12);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBCircle4_data; rbdata->qgi=0;rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->e = 0;
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(0);
              rbdata->e = ((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->e == 0)
                {  p2 = ((Coordinate3dEvent *) event)->getp();
                   if ((p2-rbdata->p1).length() < db.getptoler())
                     cadwindow->MessageBox(rs11.gets(),rs12.gets(),MB_ICONSTOP);
                   else
                     {  Circle *circle = new Circle((rbdata->p1+p2)/2.0,rbdata->xaxis,rbdata->yaxis,
                                                    (p2-rbdata->p1).length()/2.0,0.0,M_PI*2.0);
                        if (circle->getdefined())
                        {
                            circle->zValue = rbdata->zValue;
                            db.geometry.add(circle);
                        }
                     }
                }
              else
                {  p2 = ((Coordinate3dEvent *) event)->getp();
                   Line line(p2,rbdata->e->nearp(p2));
                   p3 = line.nearp(rbdata->p1);
                   Circle *circle = new Circle((p2+rbdata->e->nearp(p3))/2.0,rbdata->xaxis,rbdata->yaxis,
                                                    (rbdata->e->nearp(p3)-p2).length()/2.0,0.0,M_PI*2.0);
                   if (circle->getdefined())
                   {
                       circle->zValue = rbdata->zValue;
                       db.geometry.add(circle);
                   }
                   rbdata->e->unhighlight();
                }
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              if (rbdata->e == 0)
                {  e = ((EntitySelectedEvent *)event)->getentity();
                   Line line(rbdata->p1,e->nearp(rbdata->p1));
                   p2 = line.nearp(((EntitySelectedEvent *)event)->getp());
                   Circle *circle = new Circle((rbdata->p1+e->nearp(p2))/2.0,rbdata->xaxis,rbdata->yaxis,
                                                    (e->nearp(p2)-rbdata->p1).length()/2.0,0.0,M_PI*2.0);
                   if (circle->getdefined())
                   {
                       circle->zValue = rbdata->zValue;
                       db.geometry.add(circle);
                   }
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort))
           {  if (rbdata->e != 0) rbdata->e->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  if (rbdata->e != 0) rbdata->e->unhighlight();
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
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCCIRB+13);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       rubberband.begin(0);
       rubberband.add(RBCircle4,rbdata);
       cadwindow->prompt->normalprompt(NCCIRB+14);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}
