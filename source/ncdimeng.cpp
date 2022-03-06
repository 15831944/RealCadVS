
#include "ncwin.h"

struct RBDimen9_data
  {
#ifdef USING_WIDGETS
    QGraphicsItem *qgi;
#endif

   int state;
   Point origin,direction,xaxis,yaxis;
   SortedPointList list;
   Line *line;
   int viewno;
  };

#ifdef NEW_RUBBERB
void RBDimen9(int dmode,void *data,Point *p2,View3dSurface *s)
{RBDimen9_data *rbdata = (RBDimen9_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 SortedPointList l;
 BitMask options(32),visible(MaxViews);
 Point xaxis,yaxis,zaxis;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }
  if (s != NULL)
    rbdata->viewno = s->getview()->getviewno();
  if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->direction;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();

       CoordinateSystem cs(rbdata->origin,xaxis,yaxis);
       l.add(cs.frommodel(rbdata->origin));  l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,xaxis,yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            //ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            //rbdata->qgi = ld.qgi;
         }
    }
  else if (rbdata->state == 3)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            //ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            //rbdata->qgi = ld.qgi;
         }
    }
  else
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       l.add(cs.frommodel(rbdata->line->getp1()));
       l.add(cs.frommodel(rbdata->line->getp2()));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(*p2,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            //ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            //rbdata->qgi = ld.qgi;
         }
    }
}
#else
void RBDimen9(int,void *data,Point *p2,View3dSurface *s)
{RBDimen9_data *rbdata = (RBDimen9_data *) data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 SortedPointList l;
 BitMask options(32),visible(MaxViews);
 Point xaxis,yaxis,zaxis;

  if (s != NULL)
    rbdata->viewno = s->getview()->getviewno();
  if (rbdata->state == 2)
    {  zaxis = rbdata->xaxis.cross(rbdata->yaxis);
       xaxis = *p2 - rbdata->direction;
       yaxis = zaxis.cross(xaxis);
       xaxis.normalize();  yaxis.normalize();

       CoordinateSystem cs(rbdata->origin,xaxis,yaxis);
       l.add(cs.frommodel(rbdata->origin));  l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,xaxis,yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            rbdata->qgi = ld.qgi;
         }
    }
  else if (rbdata->state == 3)
    {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
       l = rbdata->list.copy();  l.add(cs.frommodel(*p2));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(rbdata->origin,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            rbdata->qgi = ld.qgi;
         }
    }
  else
    {  CoordinateSystem cs(*p2,rbdata->xaxis,rbdata->yaxis);
       l.add(cs.frommodel(rbdata->line->getp1()));
       l.add(cs.frommodel(rbdata->line->getp2()));
       LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
       if (db.getdragdetail() == 1)
         {  term1.style = 0;
            term2.style = 0;
            options.clear(2);
         }
       LinearDimension ld(*p2,rbdata->xaxis,rbdata->yaxis,l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
       if (ld.getdefined())
         {  visible.clearandset(rbdata->viewno,end_list);
            ld.setvisible(visible);

            ld.qgi = rbdata->qgi;
            ld.Entity::draw(DM_INVERT);
            rbdata->qgi = ld.qgi;
         }
    }
}
#endif

/**
 * @brief dimen_command9
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Parallel Dimension command
 *
 */
void dimen_command9(int *cstate,HighLevelEvent *event,void **data)
{RBDimen9_data *rbdata = (RBDimen9_data *) *data;
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int precision,tolprecision;
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
         if ((rbdata = new RBDimen9_data) == NULL)
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
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              if (((Coordinate3dEvent *)event)->getwindow() != 0)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              rbdata->line = NULL;
#ifdef USING_WIDGETS
              rbdata->qgi=0;
#endif
              *cstate = 101;
           }
         else if (event->isa(EntitySelected))
           {  rbdata->line = (Line *)((EntitySelectedEvent *) event)->getentity();
              zaxis = ((EntitySelectedEvent *) event)->getxaxis().cross(((EntitySelectedEvent *) event)->getyaxis());
              rbdata->xaxis = rbdata->line->getp2() - rbdata->line->getp1();
              rbdata->yaxis = zaxis.cross(rbdata->xaxis);
              rbdata->xaxis.normalize();  rbdata->yaxis.normalize();
              if (((EntitySelectedEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((EntitySelectedEvent *)event)->getwindow()->getview()->getviewno();
              if (rbdata->xaxis.length() > db.getptoler() && rbdata->yaxis.length() > db.getptoler())
                {  *cstate = 4;
                   if (fabs(rbdata->xaxis.dot(((EntitySelectedEvent *) event)->getxaxis())) >
                       fabs(rbdata->yaxis.dot(((EntitySelectedEvent *) event)->getxaxis())))
                     {  if (rbdata->xaxis.dot(((EntitySelectedEvent *) event)->getxaxis()) < 0.0)
                          {  rbdata->xaxis = -rbdata->xaxis;
                             rbdata->yaxis = -rbdata->yaxis;
                          }
                     }
                   else
                     {  if (rbdata->yaxis.dot(((EntitySelectedEvent *) event)->getxaxis()) > 0.0)
                          {  rbdata->xaxis = -rbdata->xaxis;
                             rbdata->yaxis = -rbdata->yaxis;
                          }
                     }
                }
              else
                rbdata->line->unhighlight();
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 101 :
         if (event->isa(Coordinate3d))
           {  rbdata->direction = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort))
           *cstate = 1;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  zaxis = ((Coordinate3dEvent *) event)->getxaxis().cross(((Coordinate3dEvent *) event)->getyaxis());
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getp() - rbdata->direction;
              rbdata->yaxis = zaxis.cross(rbdata->xaxis);
              rbdata->xaxis.normalize();  rbdata->yaxis.normalize();
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              if (rbdata->xaxis.length() > db.getptoler() && rbdata->yaxis.length() > db.getptoler())
                {  *cstate = 3;
                   if (fabs(rbdata->xaxis.dot(((Coordinate3dEvent *) event)->getxaxis())) >
                       fabs(rbdata->yaxis.dot(((Coordinate3dEvent *) event)->getxaxis())))
                     {  if (rbdata->xaxis.dot(((Coordinate3dEvent *) event)->getxaxis()) < 0.0)
                          {  rbdata->xaxis = -rbdata->xaxis;
                             rbdata->yaxis = -rbdata->yaxis;
                          }
                     }
                   else
                     {  if (rbdata->yaxis.dot(((Coordinate3dEvent *) event)->getxaxis()) > 0.0)
                          {  rbdata->xaxis = -rbdata->xaxis;
                             rbdata->yaxis = -rbdata->yaxis;
                          }
                     }
                }
           }
         else if (event->isa(Abort))
           *cstate = 1;
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(rbdata->origin,rbdata->xaxis,rbdata->yaxis);
              rbdata->list.add(cs.frommodel(((Coordinate3dEvent *) event)->getp()));
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
           }
         else if (event->isa(Abort))
           {  if (rbdata->list.length() > 1)
                {  list = rbdata->list.copy();
                   LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
                   LinearDimension *ld = new LinearDimension(rbdata->origin,rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
                   if (ld->getdefined())
                     {  visible.clearandset(rbdata->viewno,end_list);
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
       case 4 :
         rubberband.end(0);
         rbdata->line->unhighlight();
         if (event->isa(Coordinate3d))
           {  CoordinateSystem cs(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis);
              list.add(cs.frommodel(rbdata->line->getp1()));
              list.add(cs.frommodel(rbdata->line->getp2()));
              if (((Coordinate3dEvent *)event)->getwindow() != NULL)
                rbdata->viewno = ((Coordinate3dEvent *)event)->getwindow()->getview()->getviewno();
              LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);
              LinearDimension *ld = new LinearDimension(((Coordinate3dEvent *) event)->getp(),rbdata->xaxis,rbdata->yaxis,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options);
              if (ld->getdefined())
                {  visible.clearandset(rbdata->viewno,end_list);
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
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt(NCDIMENB+30);
       rbdata->state = *cstate;
    }
  else if (*cstate == 101)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       cadwindow->prompt->normalprompt("Locate the first point that defines the direction of the dimension");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen9,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+31);
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen9,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+32);
    }
  else if (*cstate == 4)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBDimen9,rbdata);
       cadwindow->prompt->normalprompt(NCDIMENB+33);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

