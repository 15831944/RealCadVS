
#include "ncwin.h"

void figure_command0(int *cstate,HighLevelEvent *,void **)
{
   cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Figure"));
  *cstate = ExitState;
}

void new_figure_command0(int *cstate,HighLevelEvent *,void **)
{
  cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Components"));
  *cstate = ExitState;
}

struct RBFigure1_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgfigure;
    qreal zValue;
#endif
   RCCHAR library[600];
   RCCHAR name[32];
   Point xaxis,yaxis,rotation;
};

#ifdef NEW_RUBBERB
void RBFigure1(int drawmode,void *data,Point *p2,View3dSurface *vs)
{RBFigure1_data *rbdata = (RBFigure1_data *) data;
 Point scale,rotation,size;
 BitMask options(32);
 Byte state1,state2;
  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options,&size);
  //
  // KMJ changes
  scale.x = v.getreal("fi::diascalex");
  scale.y = v.getreal("fi::diascaley");
  scale.z = v.getreal("fi::diascalez");
  options = *v.getbm("fi::diaoptions");
  // v8
  size.x = v.getreal("fi::diasizex");
  size.y = v.getreal("fi::diasizey");
  size.z = v.getreal("fi::diasizez");
  //
  rotation = rbdata->rotation;
  options.set(0);

  if(drawmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (drawmode == RB_DRAW)
    {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
         {  if (vs != NULL)
              {  rbdata->xaxis = vs->getxaxis();
                 rbdata->yaxis = vs->getyaxis();
              }
            else
              {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
              }
         }
       else
         {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
            rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
         }
    }
  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,size,0,state1,state2,NULL,options);

  if (figure.getdefined())
  {
      //figure.qgi = rbdata->qgfigure; figure.zValue=rbdata->zValue;
      figure.draw(DM_INVERT);
      //rbdata->qgfigure = figure.qgi;
  }
}
#else
void RBFigure1(int drawmode,void *data,Point *p2,View3dSurface *v)
{RBFigure1_data *rbdata = (RBFigure1_data *) data;
 Point scale,rotation;
 BitMask options(32);
 Byte state1,state2;
  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  rotation = rbdata->rotation;
  options.set(0);

  if (drawmode == RB_DRAW)
    {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
         {  if (v != NULL)
              {  rbdata->xaxis = v->getxaxis();
                 rbdata->yaxis = v->getyaxis();
              }
            else
              {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
              }
         }
       else
         {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
            rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
         }
    }
  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

  if (figure.getdefined())
  {
      figure.qgi = rbdata->qgfigure; figure.zValue=rbdata->zValue;
      figure.draw(DM_INVERT);
      rbdata->qgfigure = figure.qgi;
  }
}
#endif

/**
 * @brief figure_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Figure by Origin Point command
 *
 * KMJ : Working in the Qt version
 *
 */
void figure_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBFigure1_data *rbdata = (RBFigure1_data *) *data;
 Point origin,xaxis,yaxis,scale,rotation,size;
 BitMask options(32);
 ResourceString rs0(NCFIGB+0);
 Byte state1,state2;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(413,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBFigure1_data) == NULL)
           {  state.fail(Memory,rs0.gets());
              *cstate = ExitState;
           }
         else
           {
#ifdef USING_WIDGETS
              rbdata->qgfigure=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
              if (Figure::select(rbdata->library,rbdata->name))
              {
                  Figure::getdefaults(&scale,&rbdata->rotation,&state1,&state2,&options,&size);
                  // kMJ: changes
                  scale.x = v.getreal("fi::diascalex");
                  scale.y = v.getreal("fi::diascaley");
                  scale.z = v.getreal("fi::diascalez");
                  // v8
                  size.x = v.getreal("fi::diasizex");
                  size.y = v.getreal("fi::diasizey");
                  size.z = v.getreal("fi::diasizez");
                  //
                  options = *v.getbm("fi::diaoptions");
                  //
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
           {  origin = ((Coordinate3dEvent *) event)->getp();
              xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              Figure::getdefaults(&scale,&rotation,&state1,&state2,&options,&size);
              // KMJ changes
              scale.x = v.getreal("fi::diascalex");
              scale.y = v.getreal("fi::diascaley");
              scale.z = v.getreal("fi::diascalez");
              // v8
              size.x = v.getreal("fi::diasizex");
              size.y = v.getreal("fi::diasizey");
              size.z = v.getreal("fi::diasizez");
              //
              options = *v.getbm("fi::diaoptions");
              //
              rotation = rbdata->rotation;
              Figure *figure = new Figure(rbdata->library,rbdata->name,origin,xaxis,yaxis,scale,rotation,size,0,state1,state2,NULL,options);
              if (figure != NULL && figure->getdefined())
              {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
                  if(rbdata->qgfigure)
                      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
                  delete rbdata->qgfigure;
                  rbdata->qgfigure = 0;
#endif
#endif
                  db.saveundo(UD_STARTBLOCK,NULL);
                   db.geometry.add(figure);
                   figure->addattributetext();
                   db.saveundo(UD_ENDBLOCK,NULL);
                   figure->draw(DM_NORMAL);
              }
              else
                delete figure;
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
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgfigure)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
      delete rbdata->qgfigure;
      rbdata->qgfigure = 0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+1);
       rubberband.begin(0);
       rubberband.add(RBFigure1,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBFigure2_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgfigure;
    qreal zValue;
#endif
   int state;
   Point origin,xaxis,yaxis;
   RCCHAR library[600];
   RCCHAR name[32];
};

#ifdef NEW_RUBBERB
void RBFigure2(int drawmode,void *data,Point *p2,View3dSurface *vs)
{
 RBFigure2_data *rbdata = (RBFigure2_data *) data;
 Point xaxis,yaxis,scale,rotation;
 BitMask options(32);
 Byte state1,state2;

 if(drawmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  // KMJ changes
  scale.x = v.getreal("fi::diascalex");
  scale.y = v.getreal("fi::diascaley");
  scale.z = v.getreal("fi::diascalez");
  options = *v.getbm("fi::diaoptions");
  //

  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (vs != NULL)
                     {  rbdata->xaxis = vs->getxaxis();
                        rbdata->yaxis = vs->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  //rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
    }
}
#else
void RBFigure2(int drawmode,void *data,Point *p2,View3dSurface *v)
{
 RBFigure2_data *rbdata = (RBFigure2_data *) data;
 Point xaxis,yaxis,scale,rotation;
 BitMask options(32);
 Byte state1,state2;
  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (v != NULL)
                     {  rbdata->xaxis = v->getxaxis();
                        rbdata->yaxis = v->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
    }
}
#endif

/**
 * @brief figure_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Figure by Origin and Direction command
 *
 * KMJ : Working in Qt version
 *
 */
void figure_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBFigure2_data *rbdata = (RBFigure2_data *) *data;
 Point origin,xaxis,yaxis,p2,scale,rotation;
 BitMask options(32);
 ResourceString rs2(NCFIGB+2);
 Byte state1,state2;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(413,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBFigure2_data) == NULL)
           {  state.fail(Memory,rs2.gets());
              *cstate = ExitState;
           }
         else
           {
#ifdef USING_WIDGETS
             rbdata->qgfigure = 0;
             rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
             if (Figure::select(rbdata->library,rbdata->name))
             {
                 // KMJ changes
                 scale.x = v.getreal("fi::diascalex");
                 scale.y = v.getreal("fi::diascaley");
                 scale.z = v.getreal("fi::diascalez");
                 options = *v.getbm("fi::diaoptions");
                 //
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
              xaxis = p2 - rbdata->origin;
              yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
              xaxis.normalize();  yaxis.normalize();
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
                  // KMJ changes
                  scale.x = v.getreal("fi::diascalex");
                  scale.y = v.getreal("fi::diascaley");
                  scale.z = v.getreal("fi::diascalez");
                  options = *v.getbm("fi::diaoptions");
                  //
                  rotation.setxyz(0.0,0.0,0.0);
                   Figure *figure = new Figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);
                   if (figure != NULL && figure->getdefined())
                     {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
                      if(rbdata->qgfigure)
                          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
                      delete rbdata->qgfigure;
                      rbdata->qgfigure = 0;
#endif
#endif
                       db.saveundo(UD_STARTBLOCK,NULL);
                        db.geometry.add(figure);
                        figure->addattributetext();
                        db.saveundo(UD_ENDBLOCK,NULL);
                        figure->draw(DM_NORMAL);
                     }
                   else
                     delete figure;
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgfigure)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
      delete rbdata->qgfigure;
      rbdata->qgfigure=0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+3);
       rubberband.begin(0);
       rubberband.add(RBFigure2,rbdata);
       if(rbdata)
            rbdata->state = 1;
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+4);
       rubberband.begin(0);
       rubberband.add(RBFigure2,rbdata);
       if(rbdata)
            rbdata->state = 2;
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBFigure3_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgfigure;
    qreal zValue;
#endif
    int state;
   Point origin,xaxis,yaxis;
   RCCHAR library[600];
   RCCHAR name[32];
};

#ifdef NEW_RUBBERB
void RBFigure3(int drawmode,void *data,Point *p2,View3dSurface *vs)
{RBFigure3_data *rbdata = (RBFigure3_data *) data;
 Point xaxis,yaxis,zaxis,scale,rotation;
 BitMask options(32);
 Byte state1,state2;

 if(drawmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  // KMJ changes
  scale.x = v.getreal("fi::diascalex");
  scale.y = v.getreal("fi::diascaley");
  scale.z = v.getreal("fi::diascalez");
  options = *v.getbm("fi::diaoptions");
  //
  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (vs != NULL)
                     {  rbdata->xaxis = vs->getxaxis();
                        rbdata->yaxis = vs->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  //rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 3 :
         xaxis = rbdata->xaxis;
         yaxis = *p2 - rbdata->origin;
         zaxis = rbdata->xaxis.cross(yaxis);
         yaxis = zaxis.cross(rbdata->xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
    }
}
#else
void RBFigure3(int drawmode,void *data,Point *p2,View3dSurface *v)
{RBFigure3_data *rbdata = (RBFigure3_data *) data;
 Point xaxis,yaxis,zaxis,scale,rotation;
 BitMask options(32);
 Byte state1,state2;
  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (v != NULL)
                     {  rbdata->xaxis = v->getxaxis();
                        rbdata->yaxis = v->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 3 :
         xaxis = rbdata->xaxis;
         yaxis = *p2 - rbdata->origin;
         zaxis = rbdata->xaxis.cross(yaxis);
         yaxis = zaxis.cross(rbdata->xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
    }
}
#endif

/**
 * @brief figure_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Figure by Origin, Xaxis and Yaxis command
 *
 * KMJ : Working in the Qt version
 *
 */
void figure_command3(int *cstate,HighLevelEvent *event,void **data)
{RBFigure3_data *rbdata = (RBFigure3_data *) *data;
 Point origin,xaxis,yaxis,zaxis,p2,scale,rotation;
 BitMask options(32);
 ResourceString rs5(NCFIGB+5);
 Byte state1,state2;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(413,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBFigure3_data) == NULL)
           {  state.fail(Memory,rs5.gets());
              *cstate = ExitState;
           }
         else
           {
#ifdef USING_WIDGETS
             rbdata->qgfigure = 0;
             rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
             if (Figure::select(rbdata->library,rbdata->name))
             {
                 // KMJ changes
                 scale.x = v.getreal("fi::diascalex");
                 scale.y = v.getreal("fi::diascaley");
                 scale.z = v.getreal("fi::diascalez");
                 options = *v.getbm("fi::diaoptions");
                 //
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
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         *data = rbdata;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              xaxis = p2 - rbdata->origin;
              yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
              xaxis.normalize();  yaxis.normalize();
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  rbdata->xaxis = xaxis;
                   rbdata->yaxis = yaxis;
                   *cstate = 3;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              xaxis = rbdata->xaxis;
              yaxis = p2 - rbdata->origin;
              zaxis = rbdata->xaxis.cross(yaxis);
              yaxis = zaxis.cross(rbdata->xaxis);
              xaxis.normalize();  yaxis.normalize();
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
                  // KMJ changes
                  scale.x = v.getreal("fi::diascalex");
                  scale.y = v.getreal("fi::diascaley");
                  scale.z = v.getreal("fi::diascalez");
                  options = *v.getbm("fi::diaoptions");
                  //
                  rotation.setxyz(0.0,0.0,0.0);
                   Figure *figure = new Figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);
                   if (figure != NULL && figure->getdefined())
                     {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
                      if(rbdata->qgfigure)
                          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
                      delete rbdata->qgfigure;
                      rbdata->qgfigure = 0;
#endif
#endif
                       db.saveundo(UD_STARTBLOCK,NULL);
                        db.geometry.add(figure);
                        figure->addattributetext();
                        db.saveundo(UD_ENDBLOCK,NULL);
                        figure->draw(DM_NORMAL);
                     }
                   else
                     delete figure;
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
        if(rbdata->qgfigure)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
        delete rbdata->qgfigure;
        rbdata->qgfigure = 0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+6);
       rubberband.begin(0);
       rubberband.add(RBFigure3,rbdata);
       rbdata->state = 1;
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+7);
       rubberband.begin(0);
       rubberband.add(RBFigure3,rbdata);
       rbdata->state = 2;
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+8);
       rubberband.begin(0);
       rubberband.add(RBFigure3,rbdata);
       rbdata->state = 3;
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief getFigureChangeValues
 * @param change
 * @param list
 * @param hd
 * @param s
 * @param r
 * @param s1
 * @param s2
 * @param fn
 * @param afn
 * @return
 *
 * looks for Figure entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getFigureChangeValues(BitMask *change,EntityList &list,EntityHeader *hd,Point *s,Point *r,Point *si, Byte *s1,Byte *s2,BitMask *o,RCCHAR **fn, RCCHAR **afn)
{
    Entity *e;
    BitMask options(32),toptions(32);
    Figure *figure;

    // values to be compared

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(figure_entity))
        {
            figure = (Figure*)e;

            if(i==0)
            {
                hd->setcolour(figure->getcolour());
                hd->setlayer(figure->getlayer());
                hd->setstyle(figure->getstyle());
                hd->setweight(figure->getweight());

                *s  = figure->getscale();
                *r  = figure->getrotation();
                *si = figure->getsize();
                *s1 = figure->getstate1();
                *s2 = figure->getstate2();
                *fn = figure->getfilename();
                *afn = figure->getaltfilename();
                options = *figure->getoptions();
                *o = *figure->getoptions();

                // v8 adjust size initialisation for older figures
                // if the size is 0 or -MAX_REAL it has never been used
                if(si->x <= 0.0 && si->y <= 0.0 && si->z <= 0.0)
                    si->setxyz(0.0,0.0,0.0);

                change->clearall();

                change->set(0);// def color
                change->set(1);// def layer
                change->set(2);// def style
                change->set(3);// def line weight

                change->set(4);// filename
                change->set(5);// alt filename
                change->set(6);// scale x
                change->set(7);// scale y
                change->set(8);// scale z
                change->set(9);// rotation x
                change->set(10);// rotation y
                change->set(11);// rotation z
                change->set(12);// use alt name
                change->set(13);// state 1
                change->set(14);// state 2
                change->set(15);// use original layer
                change->set(16);// use original colour
                change->set(17);// use original style
                change->set(18);// use original weight
                change->set(19); // orient to view
                // these are not currently used in options dialog so not used here
                change->set(20); // size x
                change->set(21); // size y
                change->set(22); // size z
                //change->clear(23); // figure elevation
                change->set(24); // use size
                //change->clear(25); // number of copies
            }
            else
            {
                toptions = *figure->getoptions();

                if(hd->getcolour() != figure->getcolour())
                    change->clear(0);
                if(hd->getlayer() != figure->getlayer())
                    change->clear(1);
                if(hd->getstyle() != figure->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),figure->getweight()))
                    change->clear(3);

                if(strcmp(*fn,figure->getfilename()) != 0)
                    change->clear(4);
                if(strcmp(*afn,figure->getaltfilename()) != 0)
                    change->clear(5);
                if(fabs(s->x - figure->getscale().x) > db.getptoler()/*10.0e-5*/)
                    change->clear(6);// scale x
                if(fabs(s->y - figure->getscale().y) > db.getptoler()/*10.0e-5*/)
                    change->clear(7);// scale y
                if(fabs(s->z - figure->getscale().z) > db.getptoler()/*10.0e-5*/)
                    change->clear(8);// scale z
                if(fabs(r->x - figure->getrotation().x) > db.getatoler()/*10.0e-5*/)
                    change->clear(9);// rotation x
                if(fabs(r->y - figure->getrotation().y) > db.getatoler()/*10.0e-5*/)
                    change->clear(10);// rotation y

                qDebug();
                qDebug() << "r->z : " << r->z;
                double fz = figure->getrotation().z;
                qDebug() << "figure->z : " << fz;
                qDebug();
                if(fabs(r->z - figure->getrotation().z) > db.getatoler()/*10.0e-5*/)
                    change->clear(11);// rotation z
                if(options.test(FigureUseAlternativeName) != toptions.test(FigureUseAlternativeName))
                    change->clear(12);// use alt name
                if(*s1 != figure->getstate1())
                    change->clear(13);// state 1
                if(*s2 != figure->getstate2())
                    change->clear(14);// state 1
                if(options.test(FigureOriginalLayer) != toptions.test(FigureOriginalLayer))
                    change->clear(15);// original layer
                if(options.test(FigureOriginalColour) != toptions.test(FigureOriginalColour))
                    change->clear(16);// original colour
                if(options.test(FigureOriginalStyle) != toptions.test(FigureOriginalStyle))
                    change->clear(17);// original style
                if(options.test(FigureOriginalWeight) != toptions.test(FigureOriginalWeight))
                    change->clear(18);// original weight
                if(options.test(FigureOrientToView) != toptions.test(FigureOrientToView))
                    change->clear(19);// orient to view
                // v8 adjust size initialisation for older figures
                // if the size is 0 or -MAX_REAL it has never been used
                Point fsi = figure->getsize();
                if(fsi.x <= 0.0 && fsi.y <= 0.0 && fsi.z <= 0.0)
                    fsi.setxyz(0.0,0.0,0.0);
                if(fabs(si->x - fsi.x) > db.getptoler()/*10.0e-5*/)
                    change->clear(20);// size x
                if(fabs(si->y - fsi.y) > db.getptoler()/*10.0e-5*/)
                    change->clear(21);// size y
                if(fabs(si->z - fsi.z) > db.getptoler()/*10.0e-5*/)
                    change->clear(22);// size z
                if(options.test(FigureUseSize) != toptions.test(FigureUseSize))
                    change->clear(24);// orient to view
            }
            i++;
        }
    }
    return i;
}

void figure_command4(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 BitMask options(32),change(32);
 Point zero(0.0,0.0,0.0),scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),origin(0.0,0.0,0.0),size(0.0,0.0,0.0);
 EntityHeader header;
 Entity *e;
 ResourceString rs9(NCFIGB+9);
 ResourceString rs10(NCFIGB+10);
 Byte state1,state2;
 int status,length;
 Figure *figure;
 RCCHAR *fn=0,*afn=0,filename[600],altfilename[600];

 v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
    {
      list = state.getselection().copy();
       if (list.empty())
         {
            Figure::getdefaults(&scale,&rotation,&state1,&state2,&options,&size);
            status = Figure::setup(NULL,&scale,&rotation,&state1,&state2,&options,NULL,NULL,NULL,NULL,&size);
            if (status == TRUE)
              {
                 Figure::setdefaults(scale,rotation,state1,state2,options,&size);
                 *cstate = ExitState;
              }
            else if (status == 118)
              *cstate = 1;
            else if (status == 122)
              *cstate = 3;
            else
              *cstate = ExitState;
         }
       else
         {
            size.setxyz(0.0,0.0,0.0);
            Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found = getFigureChangeValues(&change,list,&header,&scale,&rotation,&size,&state1,&state2,&options,&fn,&afn);
            if(filename != 0 && fn != 0)
                strcpy(filename,fn);
            if(altfilename != 0 && afn != 0)
                strcpy(altfilename,afn);
            v.setinteger("op::changecount",found);
            if(found ==1)
            {
                // get the first figure in the list
                for (list.start() ; (e = list.next()) != NULL ; )
                {
                    if(e->type() == figure_entity)
                        break;
                }
                Point origin(0.0,0.0,0.0);
                if(e->type() == figure_entity)
                {
                    origin = ((Figure*)e)->getorigin();
                    status = Figure::setup(&change,&scale,&rotation,&state1,&state2,&options,filename,altfilename,&header,&origin,&size);
                }
            }
            else if(found > 1)
                status = Figure::setup(&change,&scale,&rotation,&state1,&state2,&options,filename,altfilename,&header,0,&size);
            else
                status = Figure::setup(NULL,&scale,&rotation,&state1,&state2,&options);
            if (status == TRUE)
              {
                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(figure_entity))
                     ((Figure *)e)->change(change,header,NULL,NULL,scale,rotation,size,0,state1,state2,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();

                 *cstate = ExitState;   

              }   
            else if (status == 118)
              *cstate = 2;
            else if (status == 122)
              *cstate = 4;
            else if (status == 128)
            {// modify parametric figure
              if(list.length() == 1)
              {
                  Figure *ef=0;
                  Entity *e2=0;
                  list.start();
                  e = (Figure*)list.next();
                  if(e->isa(figure_entity))
                  {
                     ef = (Figure*)e;
                     Dialog dialog("ModifyParametricFigure_Dialog");
                     GeneralList list2;
                     double length,value;
                     RCCHAR name1[600],name2[600];
                     //  Make sure the figure is loaded
                     ef->loadfigure(&list2);
                     dialog.add(new MPListDialogControl(ef,100,name1,600));
                     dialog.add(new RealDialogControl(101,&length,ScaleByLength));
                     if (dialog.process())
                     {

                          for (ef->firstparameter() ; ef->nextparameter(name2,&value) ; )
                          {
                            if (strcmp(name1,name2) == 0)
                              list2.add(new Parameter(name1,length));
                            else
                              list2.add(new Parameter(name2,value));
                          }

                          ef->draw(DM_ERASE);
                          //cadwindow->invalidatedisplaylist(ef);

                          ef->loadfigure(&list2);

                          ef->draw(DM_NORMAL);
                          EntityList elist;
                          elist.start();
                          elist = ef->getalist();
                          for (elist.start() ; (e = elist.next()) != 0 ; )
                          {
                              //cadwindow->invalidatedisplaylist(e);
                              e->draw(DM_NORMAL);
                          }
                     }
                  }
              }
              list.destroy();
              *cstate = ExitState;
            }
            else 
              *cstate = ExitState;
         }
    }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            figure = (Figure *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  Figure::setdefaults(figure->getscale(),figure->getrotation(),figure->getstate1(),figure->getstate2(),*figure->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(figure->getcolour(),figure->getlayer(),figure->getstyle(),figure->getweight());
                      v.setinteger("df::colour",figure->getcolour());
                      v.setinteger("df::layer",figure->getlayer());
                      v.setinteger("df::style",figure->getstyle());
                      v.setinteger("df::weight",figure->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              } 
            else
              {  if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 6 ; i <= 19 ; i++) change.set(i,1);
                 length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(figure_entity))
                     {  origin = ((Figure *)e)->getorigin();
                        ((Figure *)e)->change(change,*figure,0,0,Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),figure->getsize(),figure->getncopies(),figure->getstate1(),figure->getstate2(),*figure->getoptions());
                        db.saveundo(UD_CHANGEATTRIBUTES,e);
                        e->draw(DM_ERASE);
                        cadwindow->invalidatedisplaylist(e);
                        // v8 enhance for size
                        if(figure->getoptions()->test(FigureUseSize))
                            ((Figure *)e)->newsizetransform(origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),figure->getsize(),figure->getrotation());
                        else
                            ((Figure *)e)->newtransform(origin,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),figure->getscale(),figure->getrotation());
                        e->draw(DM_NORMAL);
                     }
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
    {  state.selmask.entity.clearandset(figure_entity,end_list);
       cadwindow->prompt->normalprompt("Select the figure entity to clone from");
    }

}


extern int filesaveselectionas(EntityList *list,const RCCHAR *filename, const RCCHAR *filepath=0);
extern int filesaveselectionas(EntityList *list,const char *filename, const char *filepath=0);

/**
 * @brief figure_command5
 * @param cstate
 *
 * Entry Point for the Save Figure Command
 *
 * KMJ : Working in Qt version
 *
 */
void figure_command5(int *cstate,HighLevelEvent *,void **)
{
  //filesaveselectionas(0,(char*)0);
  filesaveselectionas(0,(RCCHAR*)0);
  *cstate = ExitState;
}

struct RBFigure6_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgfigure;
    QGraphicsItem *qgtext;
    qreal zValue;
#endif
    int state;
   Point origin,xaxis,yaxis,offset;
   double width,lasttextheight;
   RCCHAR library[600];
   RCCHAR name[32];
};

#ifdef NEW_RUBBERB
void RBFigure6(int drawmode,void *data,Point *p2,View3dSurface *vs)
{RBFigure6_data *rbdata = (RBFigure6_data *) data;
 Point xaxis,yaxis,scale,rotation;
 BitMask options(32),toptions(32);
 Byte state1,state2;
 RCCHAR text[600];

 if(drawmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  // KMJ changes
  scale.x = v.getreal("fi::diascalex");
  scale.y = v.getreal("fi::diascaley");
  scale.z = v.getreal("fi::diascalez");
  options = *v.getbm("fi::diaoptions");
  //

  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (vs != NULL)
                     {  rbdata->xaxis = vs->getxaxis();
                        rbdata->yaxis = vs->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  //rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 //rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  scale.x = (*p2 - rbdata->origin).length() / rbdata->width;
              scale.y = (*p2 - rbdata->origin).length() / rbdata->width;
              scale.z = (*p2 - rbdata->origin).length() / rbdata->width;
              Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  //figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  //rbdata->qgfigure = figure.qgi;
              }

              strcpy(text,db.formatnumber(text,(*p2 - rbdata->origin).length(),0));

              if (vs != 0)
                {  rbdata->lasttextheight = 6.0 * vs->getscale();
                   rbdata->offset = vs->getuaxis() * 6.0 * vs->getscale() +  vs->getvaxis() * 6.0 * vs->getscale();
                }


              Text _text(_RCT("Arial"),*p2 + rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);

              if (v.getinteger("wn::distancetracking"))
              {
                  //_text.qgi = rbdata->qgtext; _text.zValue = rbdata->zValue;
                  _text.draw(DM_INVERT);
                  //rbdata->qgtext = _text.qgi;
              }
           }
         break;
    }
}
#else
void RBFigure6(int drawmode,void *data,Point *p2,View3dSurface *v)
{RBFigure6_data *rbdata = (RBFigure6_data *) data;
 Point xaxis,yaxis,scale,rotation;
 BitMask options(32),toptions(32);
 Byte state1,state2;
 RCCHAR text[600];

  if (db.getdragdetail() == 1)
    return;
  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
  options.set(0);
  rotation.setxyz(0.0,0.0,0.0);
  switch (rbdata->state)
    {  case 1 :
         if (drawmode == RB_DRAW)
           {  if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  if (v != NULL)
                     {  rbdata->xaxis = v->getxaxis();
                        rbdata->yaxis = v->getyaxis();
                     }
                   else
                     {  rbdata->xaxis.setxyz(1.0,0.0,0.0);  rbdata->yaxis.setxyz(0.0,1.0,0.0);
                     }
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  rbdata->qgfigure = figure.qgi;
              }
           }
         else
           {  Figure figure(rbdata->library,rbdata->name,*p2,rbdata->xaxis,rbdata->yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

             if (figure.getdefined())
             {
                 figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                 figure.draw(DM_INVERT);
                 rbdata->qgfigure = figure.qgi;
             }
           }
         break;
       case 2 :
         xaxis = *p2 - rbdata->origin;
         yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
         xaxis.normalize();  yaxis.normalize();
         if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
           {  scale.x = (*p2 - rbdata->origin).length() / rbdata->width;
              scale.y = (*p2 - rbdata->origin).length() / rbdata->width;
              scale.z = (*p2 - rbdata->origin).length() / rbdata->width;
              Figure figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);

              if (figure.getdefined())
              {
                  figure.qgi = rbdata->qgfigure; figure.zValue = rbdata->zValue;
                  figure.draw(DM_INVERT);
                  rbdata->qgfigure = figure.qgi;
              }

              strcpy(text,db.formatnumber(text,(*p2 - rbdata->origin).length(),0));

              if (v != 0)
                {  rbdata->lasttextheight = 6.0 * v->getscale();
                   rbdata->offset = v->getuaxis() * 6.0 * v->getscale() +  v->getvaxis() * 6.0 * v->getscale();
                }


              Text _text(_RCT("Arial"),*p2 + rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);

              if (::v.getinteger("wn::distancetracking"))
              {
                  _text.qgi = rbdata->qgtext; _text.zValue = rbdata->zValue;
                  _text.draw(DM_INVERT);
                  rbdata->qgtext = _text.qgi;
              }
           }
         break;
    }
}
#endif

/**
 * @brief figure_command6
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert and Scale Figure by 2 points command
 *
 *
 */
void figure_command6(int *cstate,HighLevelEvent *event,void **data)
{RBFigure6_data *rbdata = (RBFigure6_data *) *data;
 Point origin,xaxis,yaxis,p2,scale,rotation,pmin,pmax;
 BitMask options(32);
 ResourceString rs2(NCFIGB+2);
 Byte state1,state2;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(413,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBFigure6_data) == NULL)
           {  state.fail(Memory,rs2.gets());
              *cstate = ExitState;
           }
         else
           {  if (Figure::select(rbdata->library,rbdata->name))
                {
                 *cstate = 1;
                   Figure figure(rbdata->library,rbdata->name,Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                   figure.extents(0,&pmin,&pmax);
                   rbdata->width = pmax.x;
                   if (rbdata->width < 0.0)
                     rbdata->width = -rbdata->width;
                   if (fabs(rbdata->width) < 1.0E-10)
                     rbdata->width = 1.0;
                   rbdata->lasttextheight = 4.0;
#ifdef USING_WIDGETS
                   rbdata->qgfigure=0;
                   rbdata->qgtext=0;
                   rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
                }
              else
                *cstate = ExitState;
           }
         *data = rbdata;
         break;
       case 1 :
         rubberband.end(0);
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
              xaxis = p2 - rbdata->origin;
              yaxis = (rbdata->xaxis.cross(rbdata->yaxis)).cross(xaxis);
              xaxis.normalize();  yaxis.normalize();
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  Figure::getdefaults(&scale,&rotation,&state1,&state2,&options);
                   // KMJ: changes
                   options = *v.getbm("fi::diaoptions");
                   //
                   rotation.setxyz(0.0,0.0,0.0);
                   scale.x = (p2 - rbdata->origin).length() / rbdata->width;
                   scale.y = (p2 - rbdata->origin).length() / rbdata->width;
                   scale.z = (p2 - rbdata->origin).length() / rbdata->width;
                   Figure *figure = new Figure(rbdata->library,rbdata->name,rbdata->origin,xaxis,yaxis,scale,rotation,Point(0.0,0.0,0.0),0,state1,state2,NULL,options);
                   if (figure != NULL && figure->getdefined())
                     {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
                        if(rbdata->qgfigure)
                          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
                        delete rbdata->qgfigure;
                        rbdata->qgfigure = 0;
                        if(rbdata->qgtext)
                          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
                        delete rbdata->qgtext;
                        rbdata->qgtext = 0;
#endif
#endif
                       db.saveundo(UD_STARTBLOCK,NULL);
                        db.geometry.add(figure);
                        figure->addattributetext();
                        db.saveundo(UD_ENDBLOCK,NULL);
                        figure->draw(DM_NORMAL);
                     }
                   else
                     delete figure;
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
    if(rbdata->qgfigure)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgfigure);
    delete rbdata->qgfigure;
    rbdata->qgfigure = 0;
    if(rbdata->qgtext)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
    delete rbdata->qgtext;
    rbdata->qgtext = 0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+3);
       rubberband.begin(0);
       rubberband.add(RBFigure6,rbdata);
       rbdata->state = 1;
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCFIGB+4);
       rubberband.begin(0);
       rubberband.add(RBFigure6,rbdata);
       rbdata->state = 2;
    }
  else if (*cstate == ExitState)
    delete rbdata;
}


