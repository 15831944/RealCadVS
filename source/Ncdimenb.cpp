
#include "ncwin.h"

extern void StretchDimension(Dimension *dimension,int index,double length,EntityList *list);
extern const RCCHAR *stristr(const RCCHAR *s1,const RCCHAR *s2);

void dimen_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Dimen"));
  *cstate = ExitState;
}

struct RBDimen1_data
  {
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
#endif
    int state;
   Point origin,xaxis,yaxis;
   PointList list;
   Line *line;
   Wall *wall;
   int viewno,wallleaf;
  };

#ifdef NEW_RUBBERB
void RBDimen1(int dmode,void *data,Point *p2,View3dSurface *s)
{RBDimen1_data *rbdata = (RBDimen1_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int i,precision,tolprecision,npoints;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 SortedPointList l;
 BitMask options(32),visible(MaxViews);

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (s != NULL)
    rbdata->viewno = s->getview()->getviewno();
  if (rbdata->state == 2)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       rbdata->list.start();
       while (! rbdata->list.atend()) l.add(rbdata->list.next());
       l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ld.setvisible(visible);

            //ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            //rbdata->qgi = ld.qgi;
         }
    }
  else
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       if (rbdata->line != 0)
         {  l.add(cs.frommodel(rbdata->line->getp1()));
            l.add(cs.frommodel(rbdata->line->getp2()));
         }
       else
         {  npoints = (rbdata->wall->npoints() - 6) / 2;
            if (rbdata->wallleaf)
              {  for (i = 6 + npoints ; i < 6 + npoints * 2 ; i++)
                   l.add(cs.frommodel(rbdata->wall->point(i)));
              }
            else
              {  for (i = 6 ; i < 6 + npoints ; i++)
                   l.add(cs.frommodel(rbdata->wall->point(i)));
              }
         }
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(*p2,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ld.setvisible(visible);

            //ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            //rbdata->qgi = ld.qgi;
         }
    }
}
#else
void RBDimen1(int,void *data,Point *p2,View3dSurface *s)
{RBDimen1_data *rbdata = (RBDimen1_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int i,precision,tolprecision,npoints;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 SortedPointList l;
 BitMask options(32),visible(MaxViews);

  if (s != NULL)
    rbdata->viewno = s->getview()->getviewno();
  if (rbdata->state == 2)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       rbdata->list.start();
       while (! rbdata->list.atend()) l.add(rbdata->list.next());
       l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ld.setvisible(visible);

            ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            rbdata->qgi = ld.qgi;
         }
    }
  else
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       if (rbdata->line != 0)
         {  l.add(cs.frommodel(rbdata->line->getp1()));
            l.add(cs.frommodel(rbdata->line->getp2()));
         }
       else 
         {  npoints = (rbdata->wall->npoints() - 6) / 2;
            if (rbdata->wallleaf)
              {  for (i = 6 + npoints ; i < 6 + npoints * 2 ; i++)
                   l.add(cs.frommodel(rbdata->wall->point(i)));
              }
            else
              {  for (i = 6 ; i < 6 + npoints ; i++)
                   l.add(cs.frommodel(rbdata->wall->point(i)));
              }
         }
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(*p2,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ld.setvisible(visible);

            ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            rbdata->qgi = ld.qgi;
         }
    }
}
#endif

/**
 * @brief dimen_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert horizontal dimension command
 *
 *
 */
void dimen_command1(int *cstate,HighLevelEvent *event,void **data)
{RBDimen1_data *rbdata = (RBDimen1_data *) *data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int npoints,i,precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 BitMask options(32),visible(MaxViews);
 SortedPointList list,empty;
 Point zaxis;
 ResourceString rs0(NCDIMENB);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen1_data) == NULL)
           {  state.fail(Memory,rs0.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
              rbdata->qgi=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         rbdata->line = 0;
         rbdata->wall = 0;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              rbdata->line = NULL;
              *cstate = 2;
           }
         else if (event->isa(EntitySelected) && ((EntitySelectedEvent *) event)->getentity()->isa(line_entity))
           {  rbdata->line = (Line *)((EntitySelectedEvent *) event)->getentity();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 3;
           }
         else if (event->isa(EntitySelected) && ((EntitySelectedEvent *) event)->getentity()->isa(wall_entity))
           {  rbdata->wall = (Wall *)((EntitySelectedEvent *) event)->getentity();
              rbdata->wallleaf = (Line(rbdata->wall->point(2),rbdata->wall->point(3)).nearp(((EntitySelectedEvent *) event)->getp()) - ((EntitySelectedEvent *) event)->getp()).length() > 1.0;
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
              rbdata->list.add(cs.frommodel(((Coordinate3dEvent *) event)->getp()));
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
           }
         else if (event->isa(Abort))
           {  if (rbdata->list.length() > 1)
                {  rbdata->list.start();
                   while (! rbdata->list.atend()) list.add(rbdata->list.next());
                   LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                   LinearDimension *ld = new LinearDimension(rbdata->origin,rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                   if (ld->getdefined())
                     {  visible.clearandset(rbdata->viewno,end_list);
                        if (dimensionvisibilitycallback != 0)
                          dimensionvisibilitycallback(rbdata->viewno,&visible);
                        ld->setvisible(visible);
                        db.geometry.add(ld);
                     }
                   else
                     delete ld;
                }
              rbdata->list.destroy();
              *cstate = 1;
           }
         else if (event->isa(NewCommand))
           {PointList list;
             if (rbdata->list.length() > 0)
               {  rbdata->list.start();
                  rbdata->list.next();
                  while (! rbdata->list.atend())
                    list.add(rbdata->list.next());
                  list.reverse();
                  rbdata->list.destroy();
                  rbdata->list = list;
               }
           }
         else if (event->isa(Exit))
           {  rbdata->list.destroy();  *cstate = ExitState;
           }
         break;
       case 3 :
         rubberband.end(0);
         if (rbdata->line != 0)
           rbdata->line->unhighlight();
         else 
           rbdata->wall->unhighlight();
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis);
              if (rbdata->line != 0)
                {  list.add(cs.frommodel(rbdata->line->getp1()));
                   list.add(cs.frommodel(rbdata->line->getp2()));
                }
              else
                {  npoints = (rbdata->wall->npoints() - 6) / 2;
                   if (rbdata->wallleaf)
                     {  for (i = 6 + npoints ; i < 6 + npoints * 2 ; i++)
                          list.add(cs.frommodel(rbdata->wall->point(i)));
                     }
                   else
                     {  for (i = 6 ; i < 6 + npoints ; i++)
                          list.add(cs.frommodel(rbdata->wall->point(i)));
                     }
                }
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
              LinearDimension *ld = new LinearDimension(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
              if (ld->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   ld->setvisible(visible);
                   db.geometry.add(ld);
                }
              else
                delete ld;
              *cstate = 1;
              list = empty;
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
      if(rbdata->qgi)
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,wall_entity,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+1);
       rbdata->state = *cstate;
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,undo_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen1,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+2);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen1,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+3);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

/**
 * @brief dimen_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert vertical dimension command
 *
 */
void dimen_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBDimen1_data *rbdata = (RBDimen1_data *) *data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int i,npoints,precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 BitMask options(32),visible(MaxViews);
 SortedPointList list;
 Point zaxis;
 ResourceString rs4(NCCIRB+4);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen1_data) == NULL)
           {  state.fail(Memory,rs4.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
              rbdata->qgi=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         rbdata->line = 0;
         rbdata->wall = 0;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis =  ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->yaxis = -((Coordinate3dEvent *) event)->getxaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected) && ((EntitySelectedEvent *) event)->getentity()->isa(line_entity))
           {  rbdata->line = (Line *)((EntitySelectedEvent *) event)->getentity();
              rbdata->xaxis =  ((EntitySelectedEvent *) event)->getyaxis();
              rbdata->yaxis = -((EntitySelectedEvent *) event)->getxaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 3;
           }
         else if (event->isa(EntitySelected) && ((EntitySelectedEvent *) event)->getentity()->isa(wall_entity))
           {  rbdata->wall = (Wall *)((EntitySelectedEvent *) event)->getentity();
              rbdata->wallleaf = (Line(rbdata->wall->point(2),rbdata->wall->point(3)).nearp(((EntitySelectedEvent *) event)->getp()) - ((EntitySelectedEvent *) event)->getp()).length() > 1.0;
              rbdata->xaxis =  ((EntitySelectedEvent *) event)->getyaxis();
              rbdata->yaxis = -((EntitySelectedEvent *) event)->getxaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
              rbdata->list.add(cs.frommodel(((Coordinate3dEvent *) event)->getp()));
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
           }
         else if (event->isa(NewCommand))
           {PointList list;
             if (rbdata->list.length() > 0)
               {  rbdata->list.start();
                  rbdata->list.next();
                  while (! rbdata->list.atend())
                    list.add(rbdata->list.next());
                  list.reverse();
                  rbdata->list.destroy();
                  rbdata->list = list;
               }
           }
         else if (event->isa(Abort))
           {  if (rbdata->list.length() > 1)
                {  rbdata->list.start();
                   while (! rbdata->list.atend()) list.add(rbdata->list.next());
                   LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                   LinearDimension *ld = new LinearDimension(rbdata->origin,rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                   if (ld->getdefined())
                     {  visible.clearandset(rbdata->viewno,end_list);
                        if (dimensionvisibilitycallback != 0)
                          dimensionvisibilitycallback(rbdata->viewno,&visible);
                        ld->setvisible(visible);
                        db.geometry.add(ld);
                     }
                   else
                     delete ld;
                }
              rbdata->list.destroy();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  rbdata->list.destroy();  *cstate = ExitState;
           }
         break;
       case 3 :
         rubberband.end(0);
         if (rbdata->line != 0)
           rbdata->line->unhighlight();
         else 
           rbdata->wall->unhighlight();
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis);
              if (rbdata->line != 0)
                {  list.add(cs.frommodel(rbdata->line->getp1()));
                   list.add(cs.frommodel(rbdata->line->getp2()));
                }
              else
                {  npoints = (rbdata->wall->npoints() - 6) / 2;
                   if (rbdata->wallleaf)
                     {  for (i = 6 + npoints ; i < 6 + npoints * 2 ; i++)
                          list.add(cs.frommodel(rbdata->wall->point(i)));
                     }
                   else
                     {  for (i = 6 ; i < 6 + npoints ; i++)
                          list.add(cs.frommodel(rbdata->wall->point(i)));
                     }
                }
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
              LinearDimension *ld = new LinearDimension(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
              if (ld->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   ld->setvisible(visible);
                   db.geometry.add(ld);
                }
              else
                delete ld;
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
  if(rbdata)
  {
      if(rbdata->qgi)
              cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,wall_entity,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+5);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,undo_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen1,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+6);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen1,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+7);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBDimen3_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
    QGraphicsItem *qgline;
#endif
   int state;
   Point p1,p2,origin,xaxis,yaxis;
   Point aorigin;
   PointList list;
   Circle *arc;
   Line *line1,*line2;
   int viewno;
};

#ifdef NEW_RUBBERB
void RBDimen3(int dmode,void *data,Point *p2,View3dSurface *v)
{
 RBDimen3_data *rbdata = (RBDimen3_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,a1,a2,cosa;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 PointList l;
 Point origin;
 BitMask options(32),visible(MaxViews);

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (v != NULL)
    rbdata->viewno = v->getview()->getviewno();
  if (rbdata->state == 2)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(cs.getorigin());  l.reverse();
       Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(rbdata->origin,rbdata->xaxis,rbdata->yaxis,*p2,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            //ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            //rbdata->qgi = ad.qgi;
         }
    }
  else if (rbdata->state == 3)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(*p2);  l.reverse();
       Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->aorigin,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            //ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            //rbdata->qgi = ad.qgi;
         }
    }
  else if (rbdata->state == 4)
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       l.add(rbdata->arc->position(0.0));
       l.add(rbdata->arc->position(1.0));
       if (rbdata->arc->getxaxis().cross(rbdata->arc->getyaxis()).dot(rbdata->xaxis.cross(rbdata->yaxis)) > 0.0)
         l.reverse();
       AngularDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->arc->getorigin(),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            //ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            //rbdata->qgi = ad.qgi;

         }
    }
  else if (rbdata->state == 5)
    {  Line line(*p2,rbdata->p1);

       //line.qgi = rbdata->qgline;
       line.draw(DM_INVERT);
       //rbdata->qgline = line.qgi;
    }
  else if (rbdata->state == 6)
    {Intersect i(rbdata->line1,rbdata->line2);
       if (i.nintersections() == 1)
         {Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            origin = ((PointE *)i.intersection(0))->getp();
            l.add(rbdata->p1);
            l.add(rbdata->p2);
            cosa = ((rbdata->p1-origin).normalize()).dot((rbdata->p2-origin).normalize());
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            if ((rbdata->p1-origin).cross(rbdata->p2-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
              a1 = 2.0*M_PI - acos(cosa);
            else
              a1 = acos(cosa);
            cosa = ((rbdata->p1-origin).normalize()).dot((*p2-origin).normalize());
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            if ((rbdata->p1-origin).cross(*p2-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
              a2 = 2.0*M_PI - acos(cosa);
            else
              a2 = acos(cosa);
            if (a1 >= a2)
              l.reverse();
            AngularDimension ad(*p2,rbdata->xaxis,rbdata->yaxis,origin,l,
                                extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
            if (ad.getdefined())
              {  visible.clearandset(rbdata->viewno,end_list);
                 if (dimensionvisibilitycallback != 0)
                   dimensionvisibilitycallback(rbdata->viewno,&visible);
                 ad.setvisible(visible);
              }

            //ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            //rbdata->qgi = ad.qgi;
         }
    }
}
#else
void RBDimen3(int,void *data,Point *p2,View3dSurface *v)
{RBDimen3_data *rbdata = (RBDimen3_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,a1,a2,cosa;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 PointList l;
 Point origin;
 BitMask options(32),visible(MaxViews);

  if (v != NULL)
    rbdata->viewno = v->getview()->getviewno();
  if (rbdata->state == 2)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(cs.getorigin());  l.reverse();
       Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(rbdata->origin,rbdata->xaxis,rbdata->yaxis,*p2,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            rbdata->qgi = ad.qgi;
         }
    }
  else if (rbdata->state == 3)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(*p2);  l.reverse();
       Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->aorigin,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            rbdata->qgi = ad.qgi;
         }
    }
  else if (rbdata->state == 4)
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       l.add(rbdata->arc->position(0.0));
       l.add(rbdata->arc->position(1.0));
       if (rbdata->arc->getxaxis().cross(rbdata->arc->getyaxis()).dot(rbdata->xaxis.cross(rbdata->yaxis)) > 0.0)
         l.reverse();
       AngularDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       AngularDimension ad(*p2,rbdata->xaxis,rbdata->yaxis,rbdata->arc->getorigin(),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ad.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            ad.setvisible(visible);

            ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            rbdata->qgi = ad.qgi;

         }
    }
  else if (rbdata->state == 5)
    {  Line line(*p2,rbdata->p1);

       line.qgi = rbdata->qgline;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi;
    }
  else if (rbdata->state == 6)
    {Intersect i(rbdata->line1,rbdata->line2);
       if (i.nintersections() == 1)
         {Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            origin = ((PointE *)i.intersection(0))->getp();
            l.add(rbdata->p1);
            l.add(rbdata->p2);
            cosa = ((rbdata->p1-origin).normalize()).dot((rbdata->p2-origin).normalize());
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            if ((rbdata->p1-origin).cross(rbdata->p2-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
              a1 = 2.0*M_PI - acos(cosa);
            else
              a1 = acos(cosa);
            cosa = ((rbdata->p1-origin).normalize()).dot((*p2-origin).normalize());
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            if ((rbdata->p1-origin).cross(*p2-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
              a2 = 2.0*M_PI - acos(cosa);
            else
              a2 = acos(cosa);
            if (a1 >= a2)
              l.reverse();
            AngularDimension ad(*p2,rbdata->xaxis,rbdata->yaxis,origin,l,
                                extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
            if (ad.getdefined())
              {  visible.clearandset(rbdata->viewno,end_list);
                 if (dimensionvisibilitycallback != 0)
                   dimensionvisibilitycallback(rbdata->viewno,&visible);
                 ad.setvisible(visible);
              }

            ad.qgi = rbdata->qgi;
            ad.draw(DM_INVERT);
            rbdata->qgi = ad.qgi;
         }
    }
}
#endif

/**
 * @brief RBDimen3
 * @param data
 * @param p2
 * @param v
 *
 * Entry point for the insert angular dimension command
 *
 *
 */
void dimen_command3(int *cstate,HighLevelEvent *event,void **data)
{RBDimen3_data *rbdata = (RBDimen3_data *) *data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,cosa,a1,a2;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 BitMask options(32),visible(MaxViews);
 Point aend,origin,zaxis;
 PointList list,empty;
 ResourceString rs8(NCCIRB+8);
 RCCHAR text[2];
 //degree ‹ = 0x00B0
 QChar degree(0x00B0);

  text[0] = degree;
  text[1] = 0;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen3_data) == NULL)
           {  state.fail(Memory,rs8.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
             rbdata->qgi=0;
             rbdata->qgline=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(0);
              if (((EntitySelectedEvent *) event)->getentity()->isa(circle_entity))
                {  rbdata->arc = (Circle *)((EntitySelectedEvent *) event)->getentity();
                   rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
                   rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
                   if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                     rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
                   *cstate = 4;
                }
              else
                {  rbdata->line1 = (Line *)((EntitySelectedEvent *) event)->getentity();
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     rbdata->p1 = rbdata->line1->getp1();
                   else
                     rbdata->p1 = rbdata->line1->getp2();
                   rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
                   rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
                   if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                     rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
                   *cstate = 5;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->aorigin = ((Coordinate3dEvent *) event)->getp();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
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
           {  rbdata->list.add(((Coordinate3dEvent *) event)->getp());
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
           }
         else if (event->isa(Abort))
           {  rbdata->list.reverse();
              Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
              if (sinfo.text == NULL || RCSTRLEN(sinfo.text) == 0)
                {  sinfo.text = text;
                }
              AngularDimension *ad = new AngularDimension(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->aorigin,rbdata->list,
                                             extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
              if (ad->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   ad->setvisible(visible);
                   db.geometry.add(ad);
                }
              else
                delete ad;
              rbdata->list = empty;
              *cstate = 1;
           }
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 4 :
         rubberband.end(0);
         rbdata->arc->unhighlight();
         if (event->isa(Coordinate3d))
           {  if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
              list.add(rbdata->arc->position(0.0));
              list.add(rbdata->arc->position(1.0));
              if (rbdata->arc->getxaxis().cross(rbdata->arc->getyaxis()).dot(rbdata->xaxis.cross(rbdata->yaxis)) > 0.0)
                list.reverse();
              if (sinfo.text == NULL || strlen(sinfo.text) == 0)
                {  sinfo.text = text;
                }
              AngularDimension *ad = new AngularDimension(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis,rbdata->arc->getorigin(),list,
                                             extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
              if (ad->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   ad->setvisible(visible);
                   db.geometry.add(ad);
                }
              else
                delete ad;
              list = empty;
              *cstate = 1;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         else if (event->isa(Exit))
           *cstate = ExitState;
         break;
       case 5 :
         rubberband.end(0);
         if (event->isa(EntitySelected))
           {  rbdata->line2 = (Line *)((EntitySelectedEvent *) event)->getentity();
              if (((EntitySelectedEvent *)event)->getendselected() == 1)
                rbdata->p2 = rbdata->line2->getp1();
              else
                rbdata->p2 = rbdata->line2->getp2();

              //  Check to see if the point is on the other line.  If it is, swap ends
              if ((rbdata->p2 - rbdata->line1->nearp(rbdata->p2)).length() < db.getptoler())
                {  rbdata->p2 = rbdata->line2->getp1();
                   if ((rbdata->p2 - rbdata->line1->nearp(rbdata->p2)).length() < db.getptoler())
                     rbdata->p2 = rbdata->line2->getp2();
                }
                   
              //  Check to see if the point is on the other line.  If it is, swap ends
              if ((rbdata->p1 - rbdata->line2->nearp(rbdata->p1)).length() < db.getptoler())
                {  rbdata->p1 = rbdata->line1->getp1();
                   if ((rbdata->p1 - rbdata->line2->nearp(rbdata->p1)).length() < db.getptoler())
                     rbdata->p1 = rbdata->line1->getp2();
                }

              *cstate = 6;
           }
         else if (event->isa(Abort))
           {  rbdata->line1->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  rbdata->line1->unhighlight();
              *cstate = ExitState;
           }
         break;
       case 6 :
         rubberband.end(0);
         rbdata->line1->unhighlight();
         rbdata->line2->unhighlight();
         if (event->isa(Coordinate3d))
           {  if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              Intersect i(rbdata->line1,rbdata->line2);
              if (i.nintersections() == 1)
                {Dimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                  origin = ((PointE *)i.intersection(0))->getp();
                  list.add(rbdata->p1);
                  list.add(rbdata->p2);
                  cosa = ((rbdata->p1-origin).normalize()).dot((rbdata->p2-origin).normalize());
                  if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                  if ((rbdata->p1-origin).cross(rbdata->p2-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
                    a1 = 2.0*M_PI - acos(cosa);
                  else
                    a1 = acos(cosa);
                  cosa = ((rbdata->p1-origin).normalize()).dot((((Coordinate3dEvent *) event)->getp()-origin).normalize());
                  if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                  if ((rbdata->p1-origin).cross(((Coordinate3dEvent *) event)->getp()-origin).dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
                    a2 = 2.0*M_PI - acos(cosa);
                  else
                    a2 = acos(cosa);
                  if (a1 >= a2)
                    list.reverse();
                  if (sinfo.text == NULL || strlen(sinfo.text) == 0)
                    {  sinfo.text = text;
                    }
                  AngularDimension *ad = new AngularDimension(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis,origin,list,
                                             extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                  if (ad->getdefined())
                    {  visible.clearandset(rbdata->viewno,end_list);
                       if (dimensionvisibilitycallback != 0)
                         dimensionvisibilitycallback(rbdata->viewno,&visible);
                       ad->setvisible(visible);
                       db.geometry.add(ad);
                    }
                  else
                    delete ad;
                  list = empty;
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
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
      if(rbdata->qgline)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
      delete rbdata->qgline;
      rbdata->qgline=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+9);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen3,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+10);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen3,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+11);
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen3,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+12);
    }
  else if (*cstate == 5)
    {  state.selmask.entity.clearandset(line_entity,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen3,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+28);
    }
  else if (*cstate == 6)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen3,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+29);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class RBDimen4_data
{
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi;
#endif
   public:
   Point xaxis,yaxis,p1,p2;
   double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
   int precision,tolprecision,viewno;
   TerminatorInfo term1,term2;
   TextInfo vinfo,tinfo,pinfo,sinfo;
   BitMask options,display;
   RBDimen4_data() : options(32),display(32) {}
};

#ifdef NEW_RUBBERB
void RBDimen4(int dmode,void *data,Point *p2,View3dSurface *v)
{RBDimen4_data *rbdata = (RBDimen4_data *) data;
 BitMask visible(MaxViews);

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (v != NULL)
    rbdata->viewno = v->getview()->getviewno();
  DistanceBearingDimension dbd(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->display,
     rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
     rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
  if (dbd.getdefined())
    {  visible.clearandset(rbdata->viewno,end_list);
       if (dimensionvisibilitycallback != 0)
         dimensionvisibilitycallback(rbdata->viewno,&visible);
       dbd.setvisible(visible);

       //dbd.qgi = rbdata->qgi;
       dbd.draw(DM_INVERT);
       //rbdata->qgi = dbd.qgi;
    }
}
#else
void RBDimen4(int,void *data,Point *p2,View3dSurface *v)
{RBDimen4_data *rbdata = (RBDimen4_data *) data;
 BitMask visible(MaxViews);
  if (v != NULL)
    rbdata->viewno = v->getview()->getviewno();
  DistanceBearingDimension dbd(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->display,
     rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
     rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
  if (dbd.getdefined())
    {  visible.clearandset(rbdata->viewno,end_list);
       if (dimensionvisibilitycallback != 0)
         dimensionvisibilitycallback(rbdata->viewno,&visible);
       dbd.setvisible(visible);

       dbd.qgi = rbdata->qgi;
       dbd.draw(DM_INVERT);
       rbdata->qgi = dbd.qgi;
    }
}
#endif

/**
 * @brief dimen_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry pont for insert distance and bearing dimension
 *
 *
 */
void dimen_command4(int *cstate,HighLevelEvent *event,void **data)
{RBDimen4_data *rbdata = (RBDimen4_data *) *data;
 BitMask visible(MaxViews);
 ResourceString rs15(NCDIMENB+15),rs16(NCDIMENB+16),rs17(NCDIMENB+17);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen4_data) == NULL)
           {  state.fail(Memory,rs17.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
             rbdata->qgi=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *)event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *)event)->getyaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              DistanceBearingDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->display.clearall();
              rbdata->display.set(0);
              rbdata->display.set(1);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Line *l = (Line *)((EntitySelectedEvent *)event)->getentity();
              state.destroyselection(1);
              rbdata->xaxis = ((EntitySelectedEvent *)event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *)event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getendselected() == 1)
                {  rbdata->p1 = l->getp1();  rbdata->p2 = l->getp2();
                }
              else
                {  rbdata->p1 = l->getp2();  rbdata->p2 = l->getp1();
                }
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              DistanceBearingDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->display.clearall();
              rbdata->display.set(0);
              rbdata->display.set(1);
              DistanceBearingDimension *dbd = new DistanceBearingDimension(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                rbdata->p2,rbdata->display,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (dbd != NULL && dbd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   dbd->setvisible(visible);
                   db.geometry.add(dbd);
                }
              else
                delete dbd;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              DistanceBearingDimension *dbd = new DistanceBearingDimension(rbdata->p1,rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                ((Coordinate3dEvent *)event)->getp(),rbdata->display,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (dbd != NULL && dbd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   dbd->setvisible(visible);
                   db.geometry.add(dbd);
                }
              else
                delete dbd;
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    } 
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt(rs15.gets());
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBDimen4,rbdata);
       cadwindow->prompt->normalprompt(rs16.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class RBDimen5_data
{
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi;
#endif
   public:
   Point xaxis,yaxis,p1;
   Circle *circle;
   double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
   int precision,tolprecision,viewno;
   TerminatorInfo term1,term2;
   TextInfo vinfo,tinfo,pinfo,sinfo;
   BitMask options,style;
   RBDimen5_data() : options(32),style(32) {}
};

#ifdef NEW_RUBBERB
void RBDimen5(int dmode,void *data,Point *p2,View3dSurface *v)
{
  RBDimen5_data *rbdata = (RBDimen5_data *) data;
  BitMask visible(MaxViews);

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if (v != 0)
    rbdata->viewno = v->getview()->getviewno();
  CircularDimension cd(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->style,rbdata->circle,
      rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
      rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
  if (cd.getdefined())
  {
       visible.clearandset(rbdata->viewno,end_list);
       if (dimensionvisibilitycallback != 0)
         dimensionvisibilitycallback(rbdata->viewno,&visible);
       cd.setvisible(visible);

       //cd.qgi = rbdata->qgi;
       cd.Entity::draw(DM_INVERT);
       //rbdata->qgi = cd.qgi;
  }
}
#else
void RBDimen5(int,void *data,Point *p2,View3dSurface *v)
{RBDimen5_data *rbdata = (RBDimen5_data *) data;
 BitMask visible(MaxViews);
  if (v != 0)
    rbdata->viewno = v->getview()->getviewno();
  CircularDimension cd(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->style,rbdata->circle,
      rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
      rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
  if (cd.getdefined())
    {  visible.clearandset(rbdata->viewno,end_list);
       if (dimensionvisibilitycallback != 0)
         dimensionvisibilitycallback(rbdata->viewno,&visible);
       cd.setvisible(visible);

       cd.qgi = rbdata->qgi;
       cd.Entity::draw(DM_INVERT);
       rbdata->qgi = cd.qgi;
    }
}
#endif

/**
 * @brief dimen_command5
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert radial dimension command
 *
 */
void dimen_command5(int *cstate,HighLevelEvent *event,void **data)
{RBDimen5_data *rbdata = (RBDimen5_data *) *data;
 BitMask visible(MaxViews);
 ResourceString rs17(NCDIMENB+17),rs18(NCDIMENB+18),rs19(NCDIMENB+19),rs20(NCDIMENB+20);
 static RCCHAR prefix[256];

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen5_data) == NULL)
           {  state.fail(Memory,rs17.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
             rbdata->qgi=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  rbdata->circle = (Circle *) ((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *)event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *)event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *)event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              RCSTRNCPY(prefix,rs18.getws(),256);  prefix[255] = 0;
              if (rbdata->pinfo.text == NULL || RCSTRLEN(rbdata->pinfo.text) == 0)
                rbdata->pinfo.text = prefix;
              rbdata->style.clearall();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension *cd = new CircularDimension(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,
              ((Coordinate3dEvent *)event)->getp(),rbdata->style,rbdata->circle,rbdata->extension,rbdata->gap,rbdata->stackdis,
              rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
              rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (cd != NULL && cd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   cd->setvisible(visible);
                   db.geometry.add(cd);
                }
              else
                delete cd;
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    } 
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(circle_entity,end_list);
       cadwindow->prompt->normalprompt(rs19.gets());
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBDimen5,rbdata);
       cadwindow->prompt->normalprompt(rs20.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

/**
 * @brief dimen_command6
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert diameter with leader dimension commmand
 *
 */
void dimen_command6(int *cstate,HighLevelEvent *event,void **data)
{RBDimen5_data *rbdata = (RBDimen5_data *) *data;
 BitMask visible(MaxViews);
 static RCCHAR text[256],font[256];
 /// NCDIMENB=1850
 /// rs21=1871; rs22=1872=f; rs23=1873=Symbol; rs24=1874; rs25=1875
 ResourceString rs21(NCDIMENB+21),rs22(NCDIMENB+22),rs23(NCDIMENB+23);
 ResourceString rs24(NCDIMENB+24),rs25(NCDIMENB+25);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen5_data) == NULL)
           {  state.fail(Memory,rs21.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->viewno = 0;
#ifdef USING_WIDGETS
             rbdata->qgi=0;
#endif
              *cstate = 1;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  rbdata->circle = (Circle *) ((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *)event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *)event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *)event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->style.clearall();
              rbdata->style.set(0);
              RCSTRNCPY(text,rs22.getws(),256);  text[255] = 0;
              RCSTRNCPY(font,rs23.getws(),256);  font[255] = 0;

              if (rbdata->pinfo.text == NULL || strlen(rbdata->pinfo.text) == 0)
                {  //rbdata->options.clear(3);

                   text[0] = (RCCHAR)216;
                   text[1] = 0;

                   rbdata->pinfo.text = text;
                   rbdata->pinfo.font = font;
                   rbdata->pinfo.height = rbdata->vinfo.height * 1.5;
                }
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension *cd = new CircularDimension(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                ((Coordinate3dEvent *)event)->getp(),rbdata->style,rbdata->circle,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (cd != NULL && cd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   cd->setvisible(visible);
                   db.geometry.add(cd);
                }
              else
                delete cd;
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    } 
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(circle_entity,end_list);
       cadwindow->prompt->normalprompt(rs24.gets());
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBDimen5,rbdata);
       cadwindow->prompt->normalprompt(rs25.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

/**
 * @brief dimen_command7
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for insert circular dimension
 *
 */
void dimen_command7(int *cstate,HighLevelEvent *event,void **data)
{RBDimen5_data *rbdata = (RBDimen5_data *) *data;
 Point p2(0.0,0.0,0.0);
 BitMask visible(MaxViews);
 static RCCHAR font[256],text[256];
 ResourceString rs22(NCDIMENB+22),rs23(NCDIMENB+23),rs26(NCDIMENB+26),rs27(NCDIMENB+27);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen5_data) == NULL)
           {  state.fail(Memory,rs26.gets());
              *cstate = ExitState;
           }
         else
           *cstate = 1;
         *data = rbdata;
         break;
       case 1 :
         state.destroyselection(1);
         if (event->isa(EntitySelected))
           {  rbdata->circle = (Circle *) ((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *)event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *)event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *)event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->style.clearall();
              rbdata->style.set(1);
              RCSTRNCPY(text,rs22.getws(),256);  text[255] = 0;
              RCSTRNCPY(font,rs23.getws(),256);  font[255] = 0;

              if (rbdata->pinfo.text == NULL || strlen(rbdata->pinfo.text) == 0)
                {  //rbdata->options.clear(3);

                   text[0] = (RCCHAR)216;
                   text[1] = 0;

                   rbdata->pinfo.text = text;
                   rbdata->pinfo.font = font;
                   rbdata->pinfo.height = rbdata->vinfo.height * 1.5;
                }
              CircularDimension *cd = new CircularDimension(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                p2,rbdata->style,rbdata->circle,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (cd != NULL && cd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   cd->setvisible(visible);
                   db.geometry.add(cd);
                }
              else
                delete cd;
            }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    } 
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(circle_entity,end_list);
       cadwindow->prompt->normalprompt(rs27.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

/**
 * @brief getDimensionChangeValues
 * @param change
 * @param list
 * Returns the bumber of text entities in the list
 * looks for leader entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getDimensionChangeValues(BitMask *change, EntityList &list,EntityHeader *hd,
                          double *extension,double *gap,double *stackdis, double *uppertol,double *lowertol,double *scale,double *textyoffset,int *precision,int *tolprecision,
                          TerminatorInfo *term1,TerminatorInfo *term2,
                          TextInfo *vinfo,TextInfo *tinfo,TextInfo *pinfo,TextInfo *sinfo,
                          BitMask *op)
{

    Entity *e;
    BitMask options(32),toptions(32);
    Dimension *leadere;

    // values to be compared
    // extension
    // gap
    // stackdis
    // uppertol
    // lowertol
    // scale
    // precision
    // tolprecision,
    // term1
    // term2,
    // textinfo1
    // textinfo2
    // textinfo3
    // textinfo4
    // op

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(lineardimension_entity)  ||
            e->isa(angulardimension_entity) ||
            e->isa(distancebearingdimension_entity) ||
            e->isa(circulardimension_entity) && ((CircularDimension *)e)->getcircle() != 0)
        {
            leadere = (Dimension*)e;

            if(i==0)
            {
                hd->setcolour(leadere->getcolour());
                hd->setlayer(leadere->getlayer());
                hd->setstyle(leadere->getstyle());
                hd->setweight(leadere->getweight());

                *extension = leadere->getextension();
                *gap = leadere->getgap() ;
                *stackdis = leadere->getstackdis();
                *uppertol = leadere->getuppertol();
                *lowertol = leadere->getlowertol();
                *scale = leadere->getscale();
                *textyoffset = leadere->gettextyoffset();
                *precision = leadere->getprecision();
                *tolprecision = leadere->getprecision();
                *term1 = *leadere->getterminfo1();
                *term2 = *leadere->getterminfo2();
                *vinfo = *leadere->getvaluetextinfo();
                *tinfo = *leadere->gettolerancetextinfo();

                qDebug() << "pinfo.font" << pinfo->font;
                qDebug() << "pinfo.text" << pinfo->text;

                *pinfo = *leadere->getprefixtextinfo();
                qDebug() << "pinfo.font" << pinfo->font;
                qDebug() << "pinfo.text" << pinfo->text;

                *sinfo = *leadere->getsuffixtextinfo();
                options = leadere->getoptions();
                *op = leadere->getoptions();

                RCCHAR *s=0;
                if ((s = new RCCHAR[RCSTRLEN(vinfo->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,vinfo->font);  vinfo->font = s;
                if ((s = new RCCHAR[RCSTRLEN(tinfo->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,tinfo->font);  tinfo->font = s;
                if ((s = new RCCHAR[RCSTRLEN(pinfo->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,pinfo->font);  pinfo->font = s;
                if ((s = new RCCHAR[RCSTRLEN(sinfo->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,sinfo->font);  sinfo->font = s;
                if ((s = new RCCHAR[RCSTRLEN(vinfo->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,vinfo->text);  vinfo->text = s;
                if ((s = new RCCHAR[RCSTRLEN(tinfo->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,tinfo->text);  tinfo->text = s;
                if ((s = new RCCHAR[RCSTRLEN(pinfo->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,pinfo->text);  pinfo->text = s;
                if ((s = new RCCHAR[RCSTRLEN(sinfo->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,sinfo->text);  sinfo->text = s;

                change->clearall();

                change->set(0);// colour
                change->set(1);// layer
                change->set(2);// style
                change->set(3);// weight

                change->set(5);// extension
                change->set(6);// gap
                change->set(7);// precision
                change->set(8);// option 0 common point dimension
                change->set(9);// option 1 stacked dimension
                change->set(10);// stack distance
                change->set(11);// option 5 and 6 tolerance options
                change->set(12);// uppertol
                change->set(13);// lowertol
                change->set(14);// tol precision

                change->set(15);// term1 style
                change->set(16);// term1 length
                change->set(17);// term1 height

                change->set(20);// term2 style
                change->set(21);// term2 length
                change->set(22);// term2 height

                change->set(25);// valueinfo font
                change->set(26);// valueinfo height
                change->set(27);// valueinfo width
                change->set(28);// valueinfo angle
                change->set(29);// valueinfo option 1 and 2
                change->set(30);// valueinfo option 3 and 4

                change->set(31);// valueinfo vert spacing
                change->set(32);// valueinfo option 5 bold
                change->set(33);// valueinfo option 6 underline
                change->set(34);// valueinfo option 7 italic
                change->set(35);// valueinfo option 8 boxed
                change->set(36);// dimension option 2 display value

                change->set(40);// prefixinfo font
                change->set(41);// prefixinfo height
                change->set(42);// prefixinfo width
                change->set(43);// prefixinfo angle
                change->set(44);// prefixinfo option 1 and 2 hjust
                change->set(45);// prefixinfo option 3 and 4 vjust
                change->set(46);// prefixinfo vspacing
                change->set(47);// prefixinfo option 5 bold
                change->set(48);// prefixinfo option 6 underline
                change->set(49);// prefixinfo option 7 italic
                change->set(50);// prefixinfo option 8 boxed

                change->set(55);// suffixinfo font
                change->set(56);// suffixinfo height
                change->set(57);// suffixinfo width
                change->set(58);// suffixinfo angle
                change->set(59);// suffixinfo option 1 and 2 hjust
                change->set(60);// suffixinfo option 3 and 4 vjust
                change->set(61);// suffixinfo vspacing
                change->set(62);// suffixinfo option 5 bold
                change->set(63);// suffixinfo option 6 underline
                change->set(64);// suffixinfo option 7 italic
                change->set(65);// suffixinfo option 8 boxed

                change->set(70);// dimension option 3
                change->set(71);// prefixinfo text
                change->set(75);// dimension option 4
                change->set(76);// suffixinfo text

                change->set(77);// dimension option 7 and 8 placement
                change->set(78);// dimension option 9 term position
                change->set(79);// dimension option 10 dim line visible

                change->set(80);// toleranceinfo font
                change->set(81);// toleranceinfo height
                change->set(82);// toleranceinfo width
                change->set(83);// toleranceinfo angle
                change->set(84);// toleranceinfo option 1 and 2 hjust
                change->set(85);// toleranceinfo option 3 and 4 vjust
                change->set(86);// toleranceinfo vertspacing
                change->set(87);// toleranceinfo option 5 bold
                change->set(88);// toleranceinfo option 6 underline
                change->set(89);// toleranceinfo option 7 italic
                change->set(90);// toleranceinfo option 8 boxed

                change->set(91);// dimension option 11 tolerance defined
                change->set(92);// dimension option 12 gap from dimension
                change->set(93);// scale
                change->set(94);// dimension option 14 scale text
                change->set(95);// dimension option 17 fixed dimension position
                change->set(96);// prefixinfo option 10 outlined
                change->set(97);// prefixinfo option 12 halo
                change->set(98);// prefixinfo halocolour
                change->set(99);// text yoffset

                change->set(100);// term1 style
                change->set(101);// term1 weight
                change->set(102);// term2 style
                change->set(103);// term2 weight
                change->set(104);// dimension options 21 reverse
                change->set(105);// dimension options 22 hide left dimension line
                change->set(106);// dimension options 23 hide right dimension line
            }
            else
            {
                if(hd->getcolour() != leadere->getcolour())
                    change->clear(0);
                if(hd->getlayer() != leadere->getlayer())
                    change->clear(1);
                if(hd->getstyle() != leadere->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),leadere->getweight()))
                    change->clear(3);

                if(fabs(*extension - leadere->getextension()) > 10.0e-5)
                    change->clear(5);
                if(fabs(*gap - leadere->getgap()) > 10.0e-5)
                    change->clear(6);
                if(*precision != leadere->getprecision())
                    change->clear(7);

                toptions = leadere->getoptions();
                if(toptions.test(0) != options.test(0))
                { change->clear(8); }
                if(toptions.test(1) != options.test(1))
                { change->clear(9); }
                if(fabs(*stackdis - leadere->getstackdis()) > 10.0e-5)
                    change->clear(10);
                if(toptions.test(5) != options.test(5) || toptions.test(6) != options.test(6))
                { change->clear(11); }
                if(fabs(*uppertol - leadere->getuppertol()) > 10.0e-5)
                    change->clear(12);
                if(fabs(*lowertol - leadere->getlowertol()) > 10.0e-5)
                    change->clear(13);
                if(*tolprecision != leadere->gettolprecision())
                    change->clear(14);

                if((term1->style & 0x7F) != (leadere->getterminfo1()->style & 0x7F))
                { change->clear(15); }// term1 style
                if(fabs(term1->length - leadere->getterminfo1()->length) > 10.0e-5 )
                { change->clear(16); }// term1 length
                if(fabs(term1->height - leadere->getterminfo1()->height) > 10.0e-5 )
                { change->clear(17); }// term1 height

                if((term2->style & 0x7F) != (leadere->getterminfo2()->style & 0x7F))
                { change->clear(20); }// term2 style
                if(fabs(term2->length - leadere->getterminfo2()->length) > 10.0e-5 )
                { change->clear(21); }// term2 length
                if(fabs(term2->height - leadere->getterminfo2()->height) > 10.0e-5 )
                { change->clear(22); }// term2 height

                if(strcmp(vinfo->font,leadere->getvaluetextinfo()->font) != 0)
                { change->clear(25); }
                if(fabs(vinfo->height - leadere->getvaluetextinfo()->height) > 10.0e-5)
                { change->clear(26); }
                if(fabs(vinfo->width - leadere->getvaluetextinfo()->width) > 10.e-5)
                { change->clear(27);  }
                if(fabs(vinfo->angle - leadere->getvaluetextinfo()->angle) > 10.e-5)
                { change->clear(28); }

                toptions = leadere->getvaluetextinfo()->options;
                if((vinfo->options.test(1) != toptions.test(1) || (vinfo->options.test(2) != toptions.test(2))))
                { change->clear(29); }
                if((vinfo->options.test(3) != toptions.test(3) || (vinfo->options.test(4) != toptions.test(4))))
                { change->clear(30); }
                if(fabs(vinfo->vertspacing - leadere->getvaluetextinfo()->vertspacing) > 10.e-5)
                { change->clear(31);  }
                if(vinfo->options.test(5) != toptions.test(5))
                { change->clear(32); } // bold
                if(vinfo->options.test(6) != toptions.test(6))
                { change->clear(33); }// underline
                if(vinfo->options.test(7) != toptions.test(7))
                { change->clear(34); }// italic
                if(vinfo->options.test(8) != toptions.test(8))
                { change->clear(35); }// boxed
                toptions = leadere->getoptions();
                if(options.test(2) != toptions.test(2))
                { change->clear(36); }

                if(strcmp(pinfo->font,leadere->getprefixtextinfo()->font) != 0)
                { change->clear(40); }
                if(fabs(pinfo->height - leadere->getprefixtextinfo()->height) > 10.0e-5)
                { change->clear(41); }
                if(fabs(pinfo->width - leadere->getprefixtextinfo()->width) > 10.e-5)
                { change->clear(42);  }
                if(fabs(pinfo->angle - leadere->getprefixtextinfo()->angle) > 10.e-5)
                { change->clear(43); }

                toptions = leadere->getprefixtextinfo()->options;
                if((pinfo->options.test(1) != toptions.test(1) || (pinfo->options.test(2) != toptions.test(2))))
                { change->clear(44); }
                if((pinfo->options.test(3) != toptions.test(3) || (pinfo->options.test(4) != toptions.test(4))))
                { change->clear(45); }
                if(fabs(pinfo->vertspacing - leadere->getprefixtextinfo()->vertspacing) > 10.e-5)
                { change->clear(46);  }
                if(pinfo->options.test(5) != toptions.test(5))
                { change->clear(47); } // bold
                if(pinfo->options.test(6) != toptions.test(6))
                { change->clear(48); }// underline
                if(pinfo->options.test(7) != toptions.test(7))
                { change->clear(49); }// italic
                if(pinfo->options.test(8) != toptions.test(8))
                { change->clear(50); }// boxed

                if(strcmp(sinfo->font,leadere->getsuffixtextinfo()->font) != 0)
                { change->clear(55); }
                if(fabs(sinfo->height - leadere->getsuffixtextinfo()->height) > 10.0e-5)
                { change->clear(56); }
                if(fabs(sinfo->width - leadere->getsuffixtextinfo()->width) > 10.e-5)
                { change->clear(57);  }
                if(fabs(sinfo->angle - leadere->getsuffixtextinfo()->angle) > 10.e-5)
                { change->clear(58); }

                toptions = leadere->getsuffixtextinfo()->options;
                if((sinfo->options.test(1) != toptions.test(1) || (sinfo->options.test(2) != toptions.test(2))))
                { change->clear(59); }
                if((sinfo->options.test(3) != toptions.test(3) || (sinfo->options.test(4) != toptions.test(4))))
                { change->clear(60); }
                if(fabs(sinfo->vertspacing - leadere->getsuffixtextinfo()->vertspacing) > 10.e-5)
                { change->clear(61);  }
                if(sinfo->options.test(5) != toptions.test(5))
                { change->clear(62); } // bold
                if(sinfo->options.test(6) != toptions.test(6))
                { change->clear(63); }// underline
                if(sinfo->options.test(7) != toptions.test(7))
                { change->clear(64); }// italic
                if(sinfo->options.test(8) != toptions.test(8))
                { change->clear(65); }// boxed

                toptions = leadere->getoptions();
                if(options.test(3) != toptions.test(3))
                { change->clear(70); }
                if(strcmp(pinfo->text,leadere->getprefixtextinfo()->text) != 0)
                { change->clear(71); }

                toptions = leadere->getoptions();
                if(options.test(4) != toptions.test(4))
                { change->clear(75); }
                if(strcmp(sinfo->text,leadere->getsuffixtextinfo()->text) != 0)
                { change->clear(76); }

                if(options.test(7) != toptions.test(7) || options.test(8) != toptions.test(8))
                { change->clear(77); }
                if(options.test(9) != toptions.test(9))
                { change->clear(78); }
                if(options.test(10) != toptions.test(10))
                { change->clear(79); }

                if(strcmp(tinfo->font,leadere->gettolerancetextinfo()->font) != 0)
                { change->clear(80); }
                if(fabs(tinfo->height - leadere->gettolerancetextinfo()->height) > 10.0e-5)
                { change->clear(81); }
                if(fabs(tinfo->width - leadere->gettolerancetextinfo()->width) > 10.e-5)
                { change->clear(82);  }
                if(fabs(tinfo->angle - leadere->gettolerancetextinfo()->angle) > 10.e-5)
                { change->clear(83); }

                toptions = leadere->gettolerancetextinfo()->options;
                if((tinfo->options.test(1) != toptions.test(1) || (tinfo->options.test(2) != toptions.test(2))))
                { change->clear(84); }
                if((tinfo->options.test(3) != toptions.test(3) || (tinfo->options.test(4) != toptions.test(4))))
                { change->clear(85); }
                if(fabs(tinfo->vertspacing - leadere->gettolerancetextinfo()->vertspacing) > 10.e-5)
                { change->clear(86);  }
                if(tinfo->options.test(5) != toptions.test(5))
                { change->clear(87); } // bold
                if(tinfo->options.test(6) != toptions.test(6))
                { change->clear(88); }// underline
                if(tinfo->options.test(7) != toptions.test(7))
                { change->clear(89); }// italic
                if(tinfo->options.test(8) != toptions.test(8))
                { change->clear(90); }// boxed

                toptions = leadere->getoptions();
                if(options.test(11) != toptions.test(11))
                { change->clear(91); }
                if(options.test(12) != toptions.test(12))
                { change->clear(92); }
                if(fabs(*scale - leadere->getscale()) > 10.e-5)
                { change->clear(93); }
                if(options.test(14) != toptions.test(14))
                { change->clear(94); }
                if(options.test(17) != toptions.test(17))
                { change->clear(95); }

                toptions = leadere->getprefixtextinfo()->options;
                if(pinfo->options.test(10) != toptions.test(10))// outlined
                { change->clear(96); }
                if(pinfo->options.test(12) != toptions.test(12))// halo
                { change->clear(97); }
                if(pinfo->halocolour != leadere->getprefixtextinfo()->halocolour)
                { change->clear(98); }
                if(fabs(*textyoffset - leadere->gettextyoffset()) > 10.0e-5)
                { change->clear(99); }

                if((term1->style & 0x80) != (leadere->getterminfo1()->style & 0x80))
                { change->clear(100); }
                if(term1->weight != leadere->getterminfo1()->weight)
                { change->clear(101); }
                if((term2->style & 0x80) != (leadere->getterminfo2()->style & 0x80))
                { change->clear(102); }
                if(term2->weight != leadere->getterminfo2()->weight)
                { change->clear(103); }

                toptions = leadere->getoptions();
                if(options.test(21) != toptions.test(21))
                { change->clear(104); }
                if(options.test(22) != toptions.test(22))
                { change->clear(105); }
                if(options.test(23) != toptions.test(23))
                { change->clear(106); }
            }
            i++;
        }
    }
    return i;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

void dimen_command8(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,dlength;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 RCCHAR *s;
 int precision,tolprecision;
 BitMask options(32),change(120);
 Point zero(0.0,0.0,0.0);
 EntityHeader header;
 Entity *e;
 ResourceString rs13(NCDIMENB+13);
 ResourceString rs14(NCDIMENB+14);
 int status,length;
 Dimension *dimension;
 EntityList stretchlist;
 int c  = v.getinteger("df::colour");
 int l  = v.getinteger("df::layer");
 int st = v.getinteger("df::style");
 int w  = v.getinteger("df::weight");
 header.setcolour(c);
 header.setlayer(l);
 header.setstyle(st);
 header.setweight(w);

 v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
    {  list = state.getselection().copy();
       if (list.empty()) // nothing pre selected
       {
            LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if ((s = new RCCHAR[RCSTRLEN(vinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.font);  vinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.font);  tinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.font);  pinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.font);  sinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(vinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.text);  vinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.text);  tinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.text);  pinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.text);  sinfo.text = s;
            status = LinearDimension::setup(NULL,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if (status == TRUE)
              {  LinearDimension::setdefaults(extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                 *cstate = ExitState;
              }
            else if (status == 221) // clone
              *cstate = 1;
            else if (status == 223) // clone all
              *cstate = 3;

            else
              *cstate = ExitState;
             
            delete [] vinfo.text;  delete [] vinfo.font;
            delete [] pinfo.text;  delete [] pinfo.font;
            delete [] sinfo.text;  delete [] sinfo.font;
            delete [] tinfo.text;  delete [] tinfo.font;
         }
       else // something  was pre selected
       {
#if 0
            if(list.length() > 1)
                LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            else
            {
                list.start();
                e = list.next();
                c  = ((Dimension *)e)->getcolour();
                l  = ((Dimension *)e)->getlayer();
                st = ((Entity *)e)->getstyle();
                w  = ((Dimension *)e)->getweight();
                header.setcolour(c);
                header.setlayer(l);
                header.setstyle(st);
                header.setweight(w);
                extension = ((Dimension *)e)->getextension();
                gap = ((Dimension *)e)->getgap();
                stackdis = ((Dimension *)e)->getstackdis();
                uppertol = ((Dimension *)e)->getuppertol();
                lowertol = ((Dimension *)e)->getlowertol();
                scale = ((Dimension *)e)->getscale();
                precision = ((Dimension *)e)->getprecision();
                tolprecision = ((Dimension *)e)->gettolprecision();
                term1 = *((Dimension *)e)->getterminfo1();
                term2 = *((Dimension *)e)->getterminfo2();
                vinfo = *((Dimension *)e)->getvaluetextinfo();
                tinfo = *((Dimension *)e)->getvaluetextinfo();
                pinfo = *((Dimension *)e)->getprefixtextinfo();
                sinfo = *((Dimension *)e)->getsuffixtextinfo();
                options = ((Dimension *)e)->getoptions();
            }
#endif
            LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if ((s = new RCCHAR[RCSTRLEN(vinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.font);  vinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.font);  tinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.font);  pinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.font);  sinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(vinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.text);  vinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.text);  tinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.text);  pinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.text);  sinfo.text = s;

            change.clearall();
            int found = getDimensionChangeValues(&change,list,&header,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);

            v.setinteger("op::changecount",found);
            if(found > 0)
                status = Dimension::setup(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
            else
                status = Dimension::setup(NULL,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);

#if 0            
            if(list.length() > 1)
            {
                status = Dimension::setup(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
                change.set(71,0);
                change.set(76,0);
                change.set(71,0);
            }
            else
            {
                status = Dimension::setup(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
                for (int i = 0 ; i < 4 ; i++)
                    change.set(i,1);
            }
#endif
            if (status == TRUE)
              {  length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isdimension())
                     ((Dimension *)e)->change(change,header,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();
                 *cstate = ExitState;
              }
            else if (status == 221) // clone
              *cstate = 2;
            else if (status == 223) // clone all
              *cstate = 4;
            else if (status == 228) // Modify Length
            {
                if(list.length() == 1)
                {
                    list.start();
                    Dimension *dent = (Dimension*)list.next();
                    Dialog dialog(_RCT("Change Length"),_RCT("Length"),&dlength,ScaleByLength);
                    GeneralList dimensionvalues;
                    Parameter *dv;
                    Entity *e;
                    double x;
                    int index;

                    index = state.getlastend();
                    if (index < 0)
                    index = 0;

                    dlength = (dent->point(index+2) - dent->point(index+1)).dot(dent->getxaxis());

                    //  Make a list of all the dimensions that have a zero length equation that is a simple variable name.
                    for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                    if (e->isdimension() && strpbrk(((Dimension *)e)->getequation()," +-/*()") == 0 && strlen(((Dimension *)e)->getequation()) > 0)
                    dimensionvalues.add(new Parameter(((Dimension *)e)->getequation(),(((Dimension *)e)->point(2) - ((Dimension *)e)->point(1)).dot(((Dimension *)e)->getxaxis())));

                    StringExpression expr(dent->getequation(),&dimensionvalues);
                    if (expr.expression(&x))
                    dlength = x;

                    if (dialog.process())
                    {  StretchDimension(dent,state.getlastend(),dlength,db.geometry.getlist());
                    if (strpbrk(dent->getequation()," +-/*()") == 0 && strlen(dent->getequation()) > 0)
                      {//  Stretch any other dimensions that have this variable in it.
                         dimensionvalues.add(new Parameter(dent->getequation(),(dent->point(2) - dent->point(1)).dot(dent->getxaxis())));
                         for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                           if (e->isdimension() && stristr(((Dimension *)e)->getequation(),dent->getequation()) != 0)
                             stretchlist.add(e);

                         for (stretchlist.start() ; (e = stretchlist.next()) != 0 ; )
                           {StringExpression expr(((Dimension *)e)->getequation(),&dimensionvalues);
                              if (expr.expression(&x))
                                StretchDimension((Dimension *)e,0,x,db.geometry.getlist());
                           }

                         stretchlist.destroy();

                      }
                    }

                    for (dimensionvalues.start() ; (dv = (Parameter *)dimensionvalues.next()) != 0 ; )
                    delete dv;
                    dimensionvalues.destroy();

                    //state.destroyselection(1);

                }
                state.destroyselection(1);
                *cstate = ExitState;
            }
            else if (status == 229) // modify equation
            {
                if(list.length() == 1)
                {
                     list.start();
                     Dimension *dent = (Dimension*)list.next();
                     RCCHAR newequation[500];
                     Dialog dialog(_RCT("Change Equation"),_RCT("Equation"),newequation,500);
                     strcpy(newequation,dent->getequation());
                     if (dialog.process())
                     {
                         // this function makes a copy of the string
                         dent->setequation(newequation);
                     }
                }
                state.destroyselection(1);
                *cstate = ExitState;
            }
            else if (status == 230) // modify value
            {
                if(list.length() == 1)
                {
                     list.start();
                     Dimension *dent = (Dimension*)list.next();
                     double valueoffset;
                     Dialog dialog(_RCT("Change Offset"),_RCT("Offset"),&valueoffset,ScaleByLength);
                     GeneralList dimensionvalues;
                     int index;

                     index = state.getlastend();
                     if (index < 0)
                        index = 0;

                     valueoffset = dent->getoffset(index);

                     if (dialog.process())
                     {
                         dent->draw(DM_ERASE);
                         dent->setoffset(index,valueoffset);
                         cadwindow->invalidatedisplaylist(dent);
                         dent->draw(DM_NORMAL);
                     }
                }
                state.destroyselection(1);
                *cstate = ExitState;
            }
            else if (status == 231)
            {
              state.sendevent(new NewCommandEvent(1812,0));
              *cstate = ExitState;
            }
            else if (status == 232)
            {
               state.sendevent(new NewCommandEvent(1813,0));
               *cstate = ExitState;
            }
            else if (status == 233)
            {
               state.sendevent(new NewCommandEvent(1814,0));
               *cstate = ExitState;
            }
            else
               *cstate = ExitState;

            delete [] vinfo.text;
            delete [] vinfo.font;
            delete [] pinfo.font;
            delete [] pinfo.text;
            delete [] sinfo.text;
            delete [] sinfo.font;
            delete [] tinfo.font;
            delete [] tinfo.text;
       }
    }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            dimension = (Dimension *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  LinearDimension::setdefaults(dimension->getextension(),dimension->getgap(),dimension->getstackdis(),dimension->getuppertol(),dimension->getlowertol(),dimension->getscale(),dimension->gettextyoffset(),dimension->getprecision(),dimension->gettolprecision(),*dimension->getterminfo1(),*dimension->getterminfo2(),*dimension->getvaluetextinfo(),*dimension->gettolerancetextinfo(),*dimension->getprefixtextinfo(),*dimension->getsuffixtextinfo(),dimension->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(dimension->getcolour(),dimension->getlayer(),dimension->getstyle(),dimension->getweight());
                      v.setinteger("df::colour",dimension->getcolour());
                      v.setinteger("df::layer",dimension->getlayer());
                      v.setinteger("df::style",dimension->getstyle());
                      v.setinteger("df::weight",dimension->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              } 
            else
              {  if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 4 ; i <= 95 ; i++) change.set(i,1);
                 length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isdimension())
                     ((Dimension *)e)->change(change,*dimension,dimension->getextension(),dimension->getgap(),dimension->getstackdis(),dimension->getuppertol(),dimension->getlowertol(),dimension->getscale(),dimension->gettextyoffset(),dimension->getprecision(),dimension->gettolprecision(),*dimension->getterminfo1(),*dimension->getterminfo2(),*dimension->getvaluetextinfo(),*dimension->gettolerancetextinfo(),*dimension->getprefixtextinfo(),*dimension->getsuffixtextinfo(),dimension->getoptions());
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
    {  state.selmask.entity.clearandset(dimension_mask,end_list);
       cadwindow->prompt->normalprompt("Select the dimension entity to clone from");
    }

}

/**
 * @brief getLeaderChangeValues
 * @param change
 * @param list
 * Returns the number of text entities in the list
 * looks for leader entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getLeaderChangeValues(BitMask *change, EntityList &list,EntityHeader *hd,
                          double *extension,double *gap,double *stackdis,double *uppertol,double *lowertol,double *scale,int *precision,int *tolprecision,
                          TerminatorInfo *term1,TerminatorInfo *term2,
                          TextInfo *textinfo1,TextInfo *textinfo2,TextInfo *textinfo3,TextInfo *textinfo4,
                          BitMask *op)
{
    Entity *e;
    BitMask options(32),toptions(32);
    Dimension *leadere;

    // values to be compared
    // extension
    // gap
    // stackdis
    // uppertol
    // lowertol
    // scale
    // precision
    // tolprecision,
    // term1
    // term2,
    // textinfo1
    // textinfo2
    // textinfo3
    // textinfo4
    // op

    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(circulardimension_entity) && ((CircularDimension *)e)->getcircle() == 0)
        {
            leadere = (Dimension*)e;

            if(i==0)
            {
                hd->setcolour(leadere->getcolour());
                hd->setlayer(leadere->getlayer());
                hd->setstyle(leadere->getstyle());
                hd->setweight(leadere->getweight());

                *extension = leadere->getextension();
                *gap = leadere->getgap() ;
                *stackdis = leadere->getstackdis();
                *uppertol = leadere->getuppertol();
                *lowertol = leadere->getlowertol();
                *scale = leadere->getscale();
                *precision = leadere->getprecision();
                *tolprecision = leadere->getprecision();
                *term1 = *leadere->getterminfo1();
                *term2 = *leadere->getterminfo2();
                *textinfo1 = *leadere->getvaluetextinfo();
                *textinfo2 = *leadere->gettolerancetextinfo();
                *textinfo3 = *leadere->getprefixtextinfo();
                *textinfo4 = *leadere->getsuffixtextinfo();
                options = leadere->getoptions();
                *op = leadere->getoptions();

                RCCHAR *s=0;
                if ((s = new RCCHAR[RCSTRLEN(textinfo1->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo1->font);  textinfo1->font = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo2->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo2->font);  textinfo2->font = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo3->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo3->font);  textinfo3->font = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo4->font) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo4->font);  textinfo4->font = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo1->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo1->text);  textinfo1->text = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo2->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo2->text);  textinfo2->text = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo3->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo3->text);  textinfo3->text = s;
                if ((s = new RCCHAR[RCSTRLEN(textinfo4->text) + 1]) == NULL) return 0;
                RCSTRCPY(s,textinfo4->text);  textinfo4->text = s;

                change->clearall();

                change->set(0);// colour
                change->set(1);// layer
                change->set(2);// style
                change->set(3);// weight


                change->set(40);// font
                change->set(41);// text hight (dimension prefix text)
                change->set(42);// text width
                change->set(43);// text angle
                change->set(44);// text hjust
                change->set(45);// text vjust
                change->set(46);// text vspacing
                change->set(47);// text op 5 bold
                change->set(48);// text op 6 underline
                change->set(49);// text op 7 italic
                change->set(50);// text op 8 boxed
                change->set(96);// text op 10 outlined
                change->set(15);// term1 style
                change->set(97);// text op 12 halo
                change->set(98);// text halo colour
                change->set(16);// term1 length
                change->set(17);// term1 height
                change->set(100);// term1 use term weight
                change->set(101);// term1 weight
                change->set(94);// text op 14 scaled

            }
            else
            {
                if(hd->getcolour() != leadere->getcolour())
                    change->clear(0);
                if(hd->getlayer() != leadere->getlayer())
                    change->clear(1);
                if(hd->getstyle() != leadere->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),leadere->getweight()))
                    change->clear(3);

                if(strcmp(textinfo3->font,leadere->getprefixtextinfo()->font) != 0)
                { change->clear(40); }
                if(fabs(textinfo3->height - leadere->getprefixtextinfo()->height) > 10.0e-5)
                { change->clear(41); }
                if(fabs(textinfo3->width - leadere->getprefixtextinfo()->width) > 10.e-5)
                { change->clear(42);  }
                if(fabs(textinfo3->angle - leadere->getprefixtextinfo()->angle) > 10.e-5)
                { change->clear(43); }
                if(fabs(textinfo3->vertspacing - leadere->getprefixtextinfo()->vertspacing) > 10.e-5)
                { change->clear(46);  }
                if(textinfo3->halocolour != leadere->getprefixtextinfo()->halocolour)
                { change->clear(98); }
                toptions = leadere->getprefixtextinfo()->options;
                // horiz just
                if((textinfo3->options.test(1) != toptions.test(1) || (textinfo3->options.test(2) != toptions.test(2))))
                { change->clear(44); }
                // vert just
                if((textinfo3->options.test(3) != toptions.test(3) || (textinfo3->options.test(4) != toptions.test(4))))
                { change->clear(45); }
                if(textinfo3->options.test(5) != toptions.test(5))
                { change->clear(47); } // bold
                if(textinfo3->options.test(6) != toptions.test(6))
                { change->clear(48); }// underline
                if(textinfo3->options.test(7) != toptions.test(7))
                { change->clear(49); }// italic
                if(textinfo3->options.test(8) != toptions.test(8))
                { change->clear(50); }// boxed
                if(textinfo3->options.test(10) != toptions.test(10))
                { change->clear(96); }// outline
                if(options.test(14)  != toptions.test(14))
                { change->clear(94);  }// scaled
                if(textinfo3->options.test(12) != toptions.test(12))
                { change->clear(97); }// halo on
                if((term1->style & 0x7F) != (leadere->getterminfo1()->style & 0x7F))
                { change->clear(15); }// term1 style
                if(fabs(term1->length - leadere->getterminfo1()->length) > 10.0e-5 )
                { change->clear(16); }// term1 length
                if(fabs(term1->height - leadere->getterminfo1()->height) > 10.0e-5 )
                { change->clear(17); }// term1 height
                if((term1->style & 0x80) != (leadere->getterminfo1()->style & 0x80))
                { change->clear(100); }// use term weight
                if(!LineWeights::isEqual(term1->weight,leadere->getterminfo1()->weight) > 10.0e-5 )
                { change->clear(101); }// term1 weight
            }
            i++;
        }
    }
    return i;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

void leader_command(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 double extension,gap,stackdis,uppertol,lowertol,scale;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 RCCHAR *s;
 int precision,tolprecision;
 BitMask options(32),change(120);
 Point zero(0.0,0.0,0.0);
 EntityHeader header;
 Entity *e;
 ResourceString rs13(NCDIMENB+13);
 ResourceString rs14(NCDIMENB+14);
 int status,length;
 Dimension *dimension;
 int c  = v.getinteger("df::colour");
 int l  = v.getinteger("df::layer");
 int st = v.getinteger("df::style");
 int w  = v.getinteger("df::weight");
 //EntityHeader entheader;
 header.setcolour(c);
 header.setlayer(l);
 header.setstyle(st);
 header.setweight(w);

 v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
  {
       list = state.getselection().copy();
       if (list.empty())// nothing pre selected
       {
            LinearDimension::getleaderdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if ((s = new RCCHAR[RCSTRLEN(vinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.font);  vinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.font);  tinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.font);  pinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.font);  sinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(vinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.text);  vinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.text);  tinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.text);  pinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.text);  sinfo.text = s;
            status = LinearDimension::setupleader(NULL,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if (status == TRUE)
              {  LinearDimension::setleaderdefaults(extension,gap,stackdis,uppertol,lowertol,scale,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                 *cstate = ExitState;
              } 
            else if (status == 122)
              *cstate = 1;
            else if (status == 123)
              *cstate = 3;
            else
              *cstate = ExitState;
             
            delete [] vinfo.text;  delete [] vinfo.font;
            delete [] pinfo.text;  delete [] pinfo.font;
            delete [] sinfo.text;  delete [] sinfo.font;
         }
       else // something  was pre selected
         {
#if 0
           if(list.length() > 1)
                LinearDimension::getleaderdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
           else
           {
               list.start();
               e = list.next();
               if(e && e->isa(circulardimension_entity) && ((CircularDimension *)e)->getcircle() == 0)
               {
                   c  = ((CircularDimension *)e)->getcolour();
                   l  = ((CircularDimension *)e)->getlayer();
                   st = ((Entity *)e)->getstyle();
                   w  = ((CircularDimension *)e)->getweight();
                   header.setcolour(c);
                   header.setlayer(l);
                   header.setstyle(st);
                   header.setweight(w);
                   extension = ((CircularDimension *)e)->getextension();
                   gap = ((CircularDimension *)e)->getgap();
                   stackdis = ((CircularDimension *)e)->getstackdis();
                   uppertol = ((CircularDimension *)e)->getuppertol();
                   lowertol = ((CircularDimension *)e)->getlowertol();
                   scale = ((CircularDimension *)e)->getscale();
                   precision = ((CircularDimension *)e)->getprecision();
                   tolprecision = ((CircularDimension *)e)->gettolprecision();
                   term1 = *((CircularDimension *)e)->getterminfo1();
                   term2 = *((CircularDimension *)e)->getterminfo2();
                   vinfo = *((CircularDimension *)e)->getvaluetextinfo();
                   tinfo = *((CircularDimension *)e)->getvaluetextinfo();
                   pinfo = *((CircularDimension *)e)->getprefixtextinfo();
                   sinfo = *((CircularDimension *)e)->getsuffixtextinfo();
                   options = ((CircularDimension *)e)->getoptions();
               }
               else
               {
                    *cstate = ExitState;
                    return;
               }
           }
#endif
            LinearDimension::getleaderdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            if ((s = new RCCHAR[RCSTRLEN(vinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.font);  vinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.font);  tinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.font);  pinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.font) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.font);  sinfo.font = s;
            if ((s = new RCCHAR[RCSTRLEN(vinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,vinfo.text);  vinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(tinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,tinfo.text);  tinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(pinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,pinfo.text);  pinfo.text = s;
            if ((s = new RCCHAR[RCSTRLEN(sinfo.text) + 1]) == NULL) return;
            RCSTRCPY(s,sinfo.text);  sinfo.text = s;
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found = getLeaderChangeValues(&change,list,&header,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
            v.setinteger("op::changecount",found);
            if(found > 0)
                status = Dimension::setupleader(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
            else
                status = Dimension::setupleader(NULL,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
#if 0
            if(found > 1)
            {
                status = Dimension::setupleader(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
                //change.set(71,0);
                //change.set(76,0);
                //change.set(71,0);
            }
            else
            {
                status = Dimension::setupleader(&change,&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options,&header);
                //for (int i = 0 ; i < 4 ; i++)
                //    change.set(i,1);
            }
#endif
            if (status == TRUE)
              {  length = list.length();
                 RemoveLockedEntities(&list,0);
                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(circulardimension_entity) && ((CircularDimension *)e)->getcircle() == 0)
                     ((Dimension *)e)->change(change,header,extension,gap,stackdis,uppertol,lowertol,scale,0.0,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                 db.saveundo(UD_ENDBLOCK,NULL);
                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();
                 *cstate = ExitState;
              }
            else if (status == 122)
              *cstate = 2;
            else if (status == 123)
              *cstate = 4;
            else
              *cstate = ExitState;
            delete [] vinfo.text;  delete [] vinfo.font;
            delete [] pinfo.text;  delete [] pinfo.font;
            delete [] sinfo.text;  delete [] sinfo.font;
         }
    }
  else if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  if (event->isa(EntitySelected))
         {  state.destroyselection(1);
            dimension = (Dimension *)((EntitySelectedEvent *) event)->getentity();
            if (dimension->isa(circulardimension_entity) && ((CircularDimension *)dimension)->getcircle() == 0)
              {  if (*cstate == 1 || *cstate == 3) 
                   {  LinearDimension::setleaderdefaults(dimension->getextension(),dimension->getgap(),dimension->getstackdis(),dimension->getuppertol(),dimension->getlowertol(),dimension->getscale(),dimension->getprecision(),dimension->gettolprecision(),*dimension->getterminfo1(),*dimension->getterminfo2(),*dimension->getvaluetextinfo(),*dimension->gettolerancetextinfo(),*dimension->getprefixtextinfo(),*dimension->getsuffixtextinfo(),dimension->getoptions());
                      if (*cstate == 3)
                        {  db.header.change(dimension->getcolour(),dimension->getlayer(),dimension->getstyle(),dimension->getweight());
                           v.setinteger("df::colour",dimension->getcolour());
                           v.setinteger("df::layer",dimension->getlayer());
                           v.setinteger("df::style",dimension->getstyle());
                           v.setinteger("df::weight",dimension->getweight());
                           if (cadwindow->getlayerwindow() != 0)
                             cadwindow->getlayerwindow()->paintevent();
                           cadwindow->updatetitle();
                        }   
                   } 
                 else
                   {  if (*cstate == 4)
                        for (int i = 0 ; i < 4 ; i++) 
                          change.set(i,1);
                      for (int i = 4 ; i <= 98 ; i++) change.set(i,1);
                      change.set(71,0);
                      change.set(76,0);
                      change.set(71,0);

                      length = list.length();
                      RemoveLockedEntities(&list,0);
                      db.saveundo(UD_STARTBLOCK,NULL);
                      for (list.start() ; (e = list.next()) != NULL ; )
                        if (e->isa(circulardimension_entity) && ((CircularDimension *)e)->getcircle() == 0)
                          ((Dimension *)e)->change(change,*dimension,dimension->getextension(),dimension->getgap(),dimension->getstackdis(),dimension->getuppertol(),dimension->getlowertol(),dimension->getscale(),dimension->gettextyoffset(),dimension->getprecision(),dimension->gettolprecision(),*dimension->getterminfo1(),*dimension->getterminfo2(),*dimension->getvaluetextinfo(),*dimension->gettolerancetextinfo(),*dimension->getprefixtextinfo(),*dimension->getsuffixtextinfo(),dimension->getoptions());
                      db.saveundo(UD_ENDBLOCK,NULL);
                      state.setselection(list,0);

                      if (list.length() != length)
                        cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                   }
                 *cstate = ExitState;
              }
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
    }


  if (*cstate == 1 || *cstate == 2 || *cstate == 3 || *cstate == 4)
    {  state.selmask.entity.clearandset(dimension_mask,end_list);
       cadwindow->prompt->normalprompt("Select the leader entity to clone from");
    }

}


/**
 * @brief dimen_command10
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert inside radius dimension command
 *
 */
void dimen_command10(int *cstate,HighLevelEvent *event,void **data)
{RBDimen5_data *rbdata = (RBDimen5_data *) *data;
 Point p2(0.0,0.0,0.0);
 BitMask visible(MaxViews);
 static RCCHAR text[256];
 ResourceString rs18(NCDIMENB+18),rs23(NCDIMENB+23),rs26(NCDIMENB+26),rs27(NCDIMENB+27);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen5_data) == NULL)
           {  state.fail(Memory,rs26.gets());
              *cstate = ExitState;
           }
         else
           *cstate = 1;        
         *data = rbdata;
#ifdef USING_WIDGETS
         rbdata->qgi=0;
#endif
         break;
       case 1 :
         state.destroyselection(1);
         if (event->isa(EntitySelected))
           {  rbdata->circle = (Circle *) ((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *)event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *)event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *)event)->getyaxis();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              CircularDimension::getdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,&rbdata->textyoffset,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->style.clearall();
              rbdata->style.set(0);
              rbdata->style.set(1);
              RCSTRNCPY(text,rs18.getws(),256);  text[255] = 0;
              if (rbdata->pinfo.text == NULL || RCSTRLEN(rbdata->pinfo.text) == 0)
                rbdata->pinfo.text = text;
              CircularDimension *cd = new CircularDimension(rbdata->circle->getorigin(),rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                p2,rbdata->style,rbdata->circle,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (cd != NULL && cd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   cd->setvisible(visible);
                   db.geometry.add(cd);
                }
              else
                delete cd;
            }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    } 
#ifdef USING_WIDGETS
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(circle_entity,end_list);
       cadwindow->prompt->normalprompt(rs27.gets());
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

class RBDimen11_data
{
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi;
#endif
public:
   int state;
   RCCHAR text[4096];
   Point origin,xaxis,yaxis,p1,p2;
   double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
   int precision,tolprecision,viewno;
   TerminatorInfo term1,term2;
   TextInfo vinfo,tinfo,pinfo,sinfo;
   BitMask options,style;
   RBDimen11_data() : options(32),style(32) {}
};

#ifdef NEW_RUBBERB
void RBDimen11(int dmode,void *data,Point *p2,View3dSurface *v)
{RBDimen11_data *rbdata = (RBDimen11_data *) data;
 BitMask visible(MaxViews);

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (v != 0)
    rbdata->viewno = v->getview()->getviewno();
  if (rbdata->state == 2)
    {  CircularDimension cd(rbdata->origin,rbdata->xaxis,rbdata->yaxis,*p2,*p2,rbdata->style,0,
          rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
          rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
       if (cd.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            cd.setvisible(visible);

            //cd.qgi = rbdata->qgi;
            cd.Entity::draw(DM_INVERT);
            //rbdata->qgi = cd.qgi;
         }
    }
  else
    {  CircularDimension cd(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->style,0,
          rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
          rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
       if (cd.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            cd.setvisible(visible);

            //cd.qgi = rbdata->qgi;
            cd.Entity::draw(DM_INVERT);
            //rbdata->qgi = cd.qgi;
         }
    }
}
#else
void RBDimen11(int,void *data,Point *p2,View3dSurface *v)
{RBDimen11_data *rbdata = (RBDimen11_data *) data;
 BitMask visible(MaxViews);
  if (v != 0)
    rbdata->viewno = v->getview()->getviewno();
  if (rbdata->state == 2)
    {  CircularDimension cd(rbdata->origin,rbdata->xaxis,rbdata->yaxis,*p2,*p2,rbdata->style,0,
          rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
          rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
       if (cd.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            cd.setvisible(visible);

            cd.qgi = rbdata->qgi;
            cd.Entity::draw(DM_INVERT);
            rbdata->qgi = cd.qgi;
         }
    }
  else
    {  CircularDimension cd(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->p1,*p2,rbdata->style,0,
          rbdata->extension,rbdata->gap,rbdata->stackdis,rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
          rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
       if (cd.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            if (dimensionvisibilitycallback != 0)
              dimensionvisibilitycallback(rbdata->viewno,&visible);
            cd.setvisible(visible);

            cd.qgi = rbdata->qgi;
            cd.Entity::draw(DM_INVERT);
            rbdata->qgi = cd.qgi;
         }
    }
}
#endif

/**
 * @brief dimen_command11
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the insert leader command
 *
 */
void dimen_command11(int *cstate,HighLevelEvent *event,void **data)
{RBDimen11_data *rbdata = (RBDimen11_data *) *data;
 BitMask visible(MaxViews);
 ResourceString rs17(NCDIMENB+17),rs18(NCDIMENB+18),rs19(NCDIMENB+19),rs20(NCDIMENB+20);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {
       // bugfix b1 for v7 this should open the leader option dialog
       //::state.sendevent(new NewCommandEvent(415,0));
       ::state.sendevent(new NewCommandEvent(424,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBDimen11_data) == NULL)
           {  state.fail(Memory,rs17.gets());
              *cstate = ExitState;
           }
         else
           {Dialog dialog("LTTextInsert_Dialog");
              strcpy(rbdata->text,v.getstring("tx::leadertext",rbdata->text));
              rbdata->qgi=0;
              dialog.add(new StringDialogControl(100,rbdata->text,4096));
              if (dialog.process() == TRUE)
                {  v.setstring("tx::leadertext",rbdata->text);
                   rbdata->viewno = 0;
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         *data = rbdata;
         rbdata->qgi=0;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *)event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *)event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *)event)->getyaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              rbdata->style.clearall();
              rbdata->style.set(2);
              CircularDimension::getleaderdefaults(&rbdata->extension,&rbdata->gap,&rbdata->stackdis,&rbdata->uppertol,&rbdata->lowertol,&rbdata->scale,
                 &rbdata->precision,&rbdata->tolprecision,&rbdata->term1,&rbdata->term2,&rbdata->vinfo,&rbdata->tinfo,&rbdata->pinfo,&rbdata->sinfo,
                 &rbdata->options);
              rbdata->pinfo.text = rbdata->text;
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  CircularDimension *cd = new CircularDimension(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->p1,
                ((Coordinate3dEvent *)event)->getp(),rbdata->style,0,rbdata->extension,rbdata->gap,rbdata->stackdis,
                rbdata->uppertol,rbdata->lowertol,rbdata->scale,rbdata->textyoffset,rbdata->precision,rbdata->tolprecision,
                rbdata->term1,rbdata->term2,rbdata->vinfo,rbdata->tinfo,rbdata->pinfo,rbdata->sinfo,rbdata->options);
              if (cd != NULL && cd->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
                   if (dimensionvisibilitycallback != 0)
                     dimensionvisibilitycallback(rbdata->viewno,&visible);
                   cd->setvisible(visible);
                   db.geometry.add(cd);
                }
              else
                delete cd;
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
  rbdata->state = *cstate;
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+34);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+35);
       rubberband.begin(0);
       rubberband.add(RBDimen11,rbdata);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBDimen11,rbdata);
    }
  else if (*cstate == ExitState)
  {

    delete rbdata;
  }
}


struct RBDimen12_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
#endif
     Entity *e,*copye;
     int textindex;
     Point p1,p2;
};

#ifdef NEW_RUBBERB
void RBDimen12(int dmode,void *data,Point *p2,View3dSurface *v)
{RBDimen12_data *rbdata = (RBDimen12_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
  ((Dimension *)rbdata->copye)->movetext(rbdata->textindex,*p2-rbdata->p2);
  rbdata->p2 = *p2;

  //rbdata->copye->qgi = rbdata->qgi;
  rbdata->copye->draw(DM_INVERT);
  //rbdata->qgi = rbdata->copye->qgi;

}
#else
void RBDimen12(int,void *data,Point *p2,View3dSurface *v)
{RBDimen12_data *rbdata = (RBDimen12_data *) data;

  ((Dimension *)rbdata->copye)->movetext(rbdata->textindex,*p2-rbdata->p2);
  rbdata->p2 = *p2;

  rbdata->copye->qgi = rbdata->qgi;
  rbdata->copye->draw(DM_INVERT);
  rbdata->qgi = rbdata->copye->qgi;

}
#endif

/**
 * @brief dimen_command12
 * @param cstate
 * @param event
 * @param data
 *
 *
 *
 */
void dimen_command12(int *cstate,HighLevelEvent *event,void **data)
{RBDimen12_data *rbdata = (RBDimen12_data *) *data;
 EntityList list;
 Entity *e,*e1;
 Transform t;

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBDimen12_data;
         *data = rbdata;
#ifdef USING_WIDGETS
             rbdata->qgi=0;
#endif
         list = state.getselection().copy();
         if (list.length() == 0)
           *cstate = 1;
         else if (list.length() == 1)
           {  list.start();
              e = list.next();
              if (e->isdimension())
                {  for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
                     e1->setcopye(0);
                   rbdata->e = e;
                   rbdata->copye = rbdata->e->clone(t);  
                   rbdata->textindex = state.getlastend();
                   rbdata->p1 = state.getlastpickpoint();
                   rbdata->p2 = state.getlastpickpoint();
                   state.setposition(rbdata->p1);
                   if (rbdata->textindex >= 0)
                     {  *cstate = 2;
                        state.destroyselection(0);
                     }
                   else
                     {  *cstate = 1;
                        state.destroyselection(1);
                     }
                }
              else
                {  cadwindow->MessageBox("Locate the dimension to modify","Dimension Move Text",MB_OK);
                   *cstate = ExitState;
                }
           }
         else
           {  cadwindow->MessageBox("Only one entity can be selected","Dimension Move Text",MB_OK);
              *cstate = ExitState;
           }
         list.destroy();
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
                e1->setcopye(0);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->e->setcopye(0);
              rbdata->copye = rbdata->e->clone(t);  
              rbdata->textindex = ((EntitySelectedEvent *) event)->getendselected();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->p2 = ((EntitySelectedEvent *) event)->getp();

              state.setposition(rbdata->p1);
              if (rbdata->textindex >= 0)
                {  *cstate = 2;
                   rbdata->e->highlight();
                }
              else
                rbdata->e->unhighlight();

           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  ((Dimension *)rbdata->e)->movetext(rbdata->textindex,((Coordinate3dEvent *)event)->getp()-rbdata->p1);
              rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              state.setposition(rbdata->p1);
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  rbdata->e->unhighlight();  
              delete rbdata->copye;
              *cstate = 1;
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata)
  {
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi;
      rbdata->qgi=0;
  }
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(dimension_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the dimension to modify.");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the new position of the text");
       rubberband.begin(0);
       rubberband.add(RBDimen12,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;

}

struct RBDimenAP_data
  {Dimension *e;
   Point p;
  };

#ifdef NEW_RUBBERB
void RBDimenAP(int dmode,void *data,Point *p2,View3dSurface *v)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
  rbdata->e->addpoint(*p2,0);
  rbdata->e->draw(DM_INVERT);
  rbdata->e->delpoint(*p2,1,0);
}
#else
void RBDimenAP(int,void *data,Point *p2,View3dSurface *v)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) data;
  rbdata->e->addpoint(*p2,0);  
  rbdata->e->draw(DM_INVERT);
  rbdata->e->delpoint(*p2,1,0);
}
#endif

void dimen_addpoint(int *cstate,HighLevelEvent *event,void **data)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) *data;
 EntityList list;
 
  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       rbdata = new RBDimenAP_data;  
       *data = rbdata;   
       if (list.length() == 0)
         *cstate = ExitState;
       else if (list.length() == 1)
         {  list.start();
            rbdata->e = (Dimension *)list.next();
            if (rbdata->e->isdimension())
              {  *cstate = 1;
                 state.destroyselection(1);
              }
            else
              {  cadwindow->MessageBox("Locate the dimension to modify","Dimension Delete Line",MB_OK);
                 *cstate = ExitState;
              }
         }
       else
         {  cadwindow->MessageBox("Only one entity can be selected","Dimension Delete Line",MB_OK);
            *cstate = ExitState;
         }
       list.destroy();
       break;
     case 1 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         ((Dimension *)rbdata->e)->addpoint(((Coordinate3dEvent *)event)->getp(),1);
       else if (event->isa(Abort) || event->isa(Exit))
         {  rbdata->e->unhighlight();
            *cstate = ExitState;
         }
       break;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the dimension point to add.");
       rubberband.begin(0);
       rubberband.add(RBDimenAP,rbdata);
    }

}


void dimen_delpoint(int *cstate,HighLevelEvent *event,void **)
{static Entity *e;
 EntityList list;
 
  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       if (list.length() == 0)
         *cstate = ExitState;
       else if (list.length() == 1)
         {  list.start();
            e = list.next();
            if (e->isdimension())
              {  *cstate = 1;
                 state.destroyselection(1);
              }
            else
              {  cadwindow->MessageBox("Locate the dimension to modify","Dimension Delete Line",MB_OK);
                 *cstate = ExitState;
              }
         }
       else
         {  cadwindow->MessageBox("Only one entity can be selected","Dimension Delete Line",MB_OK);
            *cstate = ExitState;
         }
       list.destroy();
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  ((Dimension *)e)->delpoint(((Coordinate3dEvent *)event)->getp(),0,1);
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  e->unhighlight();
            *cstate = ExitState;
         }
       break;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the dimension point to delete.");
    }

}

#ifdef NEW_RUBBERB
void RBDimenMP(int dmode,void *data,Point *p2,View3dSurface *v)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) data;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  rbdata->e->addpoint(*p2,0);
  rbdata->e->draw(DM_INVERT);
  rbdata->e->delpoint(*p2,1,0);
}
#else
void RBDimenMP(int,void *data,Point *p2,View3dSurface *v)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) data;
  rbdata->e->addpoint(*p2,0);  
  rbdata->e->draw(DM_INVERT);
  rbdata->e->delpoint(*p2,1,0);
}
#endif

void dimen_movepoint(int *cstate,HighLevelEvent *event,void **data)
{RBDimenAP_data *rbdata = (RBDimenAP_data *) *data;
 EntityList list;
 int l;
 
  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(415,0));
       *cstate = ExitState;
       return;
    }
  switch (*cstate)
    {case InitialState :
       list = state.getselection().copy();
       rbdata = new RBDimenAP_data;  
       *data = rbdata;   
       if (list.length() == 0)
         *cstate = ExitState;
       else if (list.length() == 1)
         {  list.start();
            rbdata->e = (Dimension *)list.next();
            if (rbdata->e->isdimension())
              {  *cstate = 1;
                 state.destroyselection(1);
              }
            else
              {  cadwindow->MessageBox("Locate the dimension to modify","Dimension Delete Line",MB_OK);
                 *cstate = ExitState;
              }
         }
       else
         {  cadwindow->MessageBox("Only one entity can be selected","Dimension Delete Line",MB_OK);
            *cstate = ExitState;
         }
       list.destroy();
       break;
     case 1 :
       if (event->isa(Coordinate3d))
         {  l = ((LinearDimension *)rbdata->e)->getlist().length();
            rbdata->p = ((Coordinate3dEvent *)event)->getp();
            rbdata->e->delpoint(rbdata->p,0,1);
            if (l != ((LinearDimension *)rbdata->e)->getlist().length())
              *cstate = 2; 
          }
       else if (event->isa(Abort) || event->isa(Exit))
         {  rbdata->e->unhighlight();
            *cstate = ExitState;
         }
       break;
     case 2 :
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  ((Dimension *)rbdata->e)->addpoint(((Coordinate3dEvent *)event)->getp(),1);
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  rbdata->e->addpoint(rbdata->p,1);  //  Add the deleted point back.
            rbdata->e->unhighlight();
            *cstate = ExitState;
         }
       break;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the dimension point to move.");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the new position of the dimension point.");
       rubberband.begin(0);
       rubberband.add(RBDimenMP,rbdata);
    }

}

