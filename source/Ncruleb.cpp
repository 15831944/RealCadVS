
#include "ncwin.h"

typedef int (WINAPI *runcommand)(char *);

static QGraphicsItem *RBR1_qgline1=0;
static QGraphicsItem *RBR1_qgline2=0;
#ifdef NEW_RUBBERB
void RBRuled1(int dmode,void *data,Point *p2,View3dSurface *)
{
  LinearEntity *e1 = (LinearEntity *) data;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  Line line1(e1->start(),*p2);
  //line1.qgi = RBR1_qgline1; line1.zValue = QGDRAGZVALUE(QGMAXZVALUE);
  line1.draw(DM_INVERT);
  //RBR1_qgline1 = line1.qgi;

  Line line2(e1->end(),*p2);
  //line2.qgi = RBR1_qgline2; line2.zValue = QGDRAGZVALUE(QGMAXZVALUE);
  line2.draw(DM_INVERT);
  //RBR1_qgline2 = line2.qgi;
}
#else
void RBRuled1(int,void *data,Point *p2,View3dSurface *)
{
  LinearEntity *e1 = (LinearEntity *) data;

  Line line1(e1->start(),*p2);
  line1.qgi = RBR1_qgline1; line1.zValue = QGDRAGZVALUE(QGMAXZVALUE);
  line1.draw(DM_INVERT);
  RBR1_qgline1 = line1.qgi;

  Line line2(e1->end(),*p2);
  line2.qgi = RBR1_qgline2; line2.zValue = QGDRAGZVALUE(QGMAXZVALUE);
  line2.draw(DM_INVERT);
  RBR1_qgline2 = line2.qgi;
}
#if 0
void RBRuled1(int,void *data,Point *p2,View3dSurface *)
{
  LinearEntity *e1 = (LinearEntity *) data;
  Line line1(e1->start(),*p2);  line1.draw(DM_INVERT);
  Line line2(e1->end(),*p2);    line2.draw(DM_INVERT);
}
#endif
#endif

/**
 * @brief ruledsurface_command1
 * @param cstate
 * @param event
 *
 * Entry point for the Insert Ruled Surface command
 *
 */
void ruledsurface_command1(int *cstate,HighLevelEvent *event,void **)
{static LinearEntity *e1;
 static int end;
 LinearEntity *e2;
 int xmesh,ymesh;
 BitMask options(32);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(410,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         *cstate = 1;
         break;
       case 1 :
         state.destroyselection(1);
         if (event->isa(EntitySelected))
           {  e1 = (LinearEntity *)((EntitySelectedEvent *)event)->getentity();
              end = ((EntitySelectedEvent *)event)->getendselected();
              e1->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         state.destroyselection(1);
         e1->unhighlight();
         if (event->isa(EntitySelected))
           {  e2 = (LinearEntity *)((EntitySelectedEvent *)event)->getentity();
              RuledSurface::getdefaults(&xmesh,&ymesh,&options);
              if (end == ((EntitySelectedEvent *)event)->getendselected())
                options.clear(0);
              else
                options.set(0);
              RuledSurface *surface = new RuledSurface(e1,e2,xmesh,ymesh,options);
              if (surface != NULL && surface->getdefined())
                db.geometry.add(surface);
              else
                delete surface;
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
  if(RBR1_qgline1)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(RBR1_qgline1);
      delete RBR1_qgline1;
      RBR1_qgline1=0;
  }
  if(RBR1_qgline2)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(RBR1_qgline2);
      delete RBR1_qgline2;
      RBR1_qgline2=0;
  }
#endif
#endif

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCRULEB);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBRuled1,e1);
       cadwindow->prompt->normalprompt(NCRULEB+1);
    }
}

int create_pipe(double radius,int xmesh,int ymesh,Point p1,Point p2,Circle **c1,Circle **c2,RuledSurface **r1)
{Point xaxis1,xaxis2,yaxis1,yaxis2,zaxis;
 BitMask options(32);
  if ((p2 - p1).length() < db.getptoler()) return 0;
  zaxis = (p2 - p1).normalize();
  xaxis1.setxyz(1.0,0.0,0.0);
  yaxis1 = xaxis1.cross(zaxis);
  xaxis2.setxyz(0.0,1.0,0.0);
  yaxis2 = xaxis2.cross(zaxis);
  if (yaxis1.length() < yaxis2.length())
    {  xaxis1 = xaxis2;  yaxis1 = yaxis2;
    }
  xaxis1 = yaxis1.cross(zaxis);
  xaxis1.normalize();  yaxis1.normalize();
  *c1 = new Circle(p1,xaxis1,yaxis1,radius,0.0,M_PI * 2.0);
  *c2 = new Circle(p2,xaxis1,yaxis1,radius,0.0,M_PI * 2.0);
  if (*c1 == NULL || *c2 == NULL || ! (*c1)->getdefined() || ! (*c2)->getdefined())
    {  delete c1;  delete c2;  return 0;
    }
  *r1 = new RuledSurface(*c1,*c2,xmesh,ymesh,options);
  return 1;
}

#if MENUS != STUDENTMENUS

struct RBRuled2_data
{
#ifdef USING_WIDGETS
   QGraphicsItem *qgc1,*qgc2,*qgr1,*qgline;
#endif
   Point p1;
   int xmesh,ymesh;
   double radius;
};

#ifdef NEW_RUBBERB
void RBRuled2(int dmode,void *data,Point *p2,View3dSurface *)
{
    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }

    RBRuled2_data *rbdata = (RBRuled2_data *) data;
    Circle *c1,*c2;
    RuledSurface *r1;
    RCCHAR **namelist;
    View *view;
    BitMask visibility(MaxViews);
    Point p3,p4,p5,p6;
    int i,n,index;
     if (! create_pipe(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,rbdata->p1,*p2,&c1,&c2,&r1)) return;

     //c1->qgi = rbdata->qgc1;
       c1->draw(DM_INVERT);
     //rbdata->qgc1 = c1->qgi;

     //c2->qgi = rbdata->qgc2;
       c2->draw(DM_INVERT);
     //rbdata->qgc2 = c2->qgi;

     //r1->qgi = rbdata->qgr1;
     r1->draw(DM_INVERT);
     //rbdata->qgr1 = r1->qgi;

     Line line(c1->start(),c2->start());
     //line.qgi = rbdata->qgline;
       line.draw(DM_INVERT);
     //rbdata->qgline = line.qgi;

     EntityHeader hd;
     db.views3d.names(&namelist,&n,&index);
     for (i = 0 ; i < n ; i++)
       if ((view = db.views3d.match(namelist[i])) != 0)
         {  if (fabs((rbdata->p1 - *p2).normalize().dot((view->geteye() - view->getref()).normalize())) < cos(M_PI/2.0-0.01))
              visibility.set(view->getviewno());
         }
     hd.setvisible(visibility);
     p3 = c1->position(0.0);
     p4 = c1->position(0.5);
     p5 = c2->position(0.0);
     p6 = c2->position(0.5);
     for (i = 0 ; i < (rbdata->ymesh/2)*2+1 ; i++)
       {  Line line(hd,p3+(p5-p3)*double(i)/double((rbdata->ymesh/2)*2),p4+(p6-p4)*double(i)/double((rbdata->ymesh/2)*2));

         //line.qgi = rbdata->qgline;
         line.draw(DM_INVERT);
         //rbdata->qgline = line.qgi;
       }
     delete [] namelist;

     delete r1;
}
#else
void RBRuled2(int,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
    RBRuled2_data *rbdata = (RBRuled2_data *) data;
    Circle *c1,*c2;
    RuledSurface *r1;
    RCCHAR **namelist;
    View *view;
    BitMask visibility(MaxViews);
    Point p3,p4,p5,p6;
    int i,n,index;
     if (! create_pipe(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,rbdata->p1,*p2,&c1,&c2,&r1)) return;

     c1->qgi = rbdata->qgc1;
       c1->draw(DM_INVERT);
     rbdata->qgc1 = c1->qgi;

     c2->qgi = rbdata->qgc2;
       c2->draw(DM_INVERT);
     rbdata->qgc2 = c2->qgi;

     r1->qgi = rbdata->qgr1;
     r1->draw(DM_INVERT);
     rbdata->qgr1 = r1->qgi;

     Line line(c1->start(),c2->start());
     line.qgi = rbdata->qgline;
       line.draw(DM_INVERT);
     rbdata->qgline = line.qgi;

     EntityHeader hd;
     db.views3d.names(&namelist,&n,&index);
     for (i = 0 ; i < n ; i++)
       if ((view = db.views3d.match(namelist[i])) != 0)
         {  if (fabs((rbdata->p1 - *p2).normalize().dot((view->geteye() - view->getref()).normalize())) < cos(M_PI/2.0-0.01))
              visibility.set(view->getviewno());
         }
     hd.setvisible(visibility);
     p3 = c1->position(0.0);
     p4 = c1->position(0.5);
     p5 = c2->position(0.0);
     p6 = c2->position(0.5);
     for (i = 0 ; i < (rbdata->ymesh/2)*2+1 ; i++)
       {  Line line(hd,p3+(p5-p3)*double(i)/double((rbdata->ymesh/2)*2),p4+(p6-p4)*double(i)/double((rbdata->ymesh/2)*2));

         line.qgi = rbdata->qgline;
         line.draw(DM_INVERT);
         rbdata->qgline = line.qgi;
       }
     delete [] namelist;

     delete r1;
#else
 RBRuled2_data *rbdata = (RBRuled2_data *) data;
 Circle *c1,*c2;
 RuledSurface *r1;
 RCCHAR **namelist;
 View *view;
 BitMask visibility(MaxViews);
 Point p3,p4,p5,p6;
 int i,n,index;
  if (! create_pipe(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,rbdata->p1,*p2,&c1,&c2,&r1)) return;

  c1->draw(DM_INVERT);
  c2->draw(DM_INVERT);
  r1->draw(DM_INVERT);
  Line line(c1->start(),c2->start());
  line.draw(DM_INVERT);

  EntityHeader hd;
  db.views3d.names(&namelist,&n,&index);
  for (i = 0 ; i < n ; i++)
    if ((view = db.views3d.match(namelist[i])) != 0)
      {  if (fabs((rbdata->p1 - *p2).normalize().dot((view->geteye() - view->getref()).normalize())) < cos(M_PI/2.0-0.01))
           visibility.set(view->getviewno());
      }
  hd.setvisible(visibility);
  p3 = c1->position(0.0);
  p4 = c1->position(0.5);
  p5 = c2->position(0.0);
  p6 = c2->position(0.5);
  for (i = 0 ; i < (rbdata->ymesh/2)*2+1 ; i++)
    {  Line line(hd,p3+(p5-p3)*double(i)/double((rbdata->ymesh/2)*2),p4+(p6-p4)*double(i)/double((rbdata->ymesh/2)*2));
       line.draw(DM_INVERT);
    }
  delete [] namelist;

  delete r1;
#endif
}
#endif

void ruledsurface_command2(int *cstate,HighLevelEvent *event,void **data)
{RBRuled2_data *rbdata = (RBRuled2_data *) *data;
 Dialog *dialog;
 Point p2;
 Line *line;
 Circle *c1,*c2;
 RuledSurface *r1;
 ResourceString rs2(NCRULEB+2),rs3(NCRULEB+3),rs4(NCRULEB+4),rs5(NCRULEB+5);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(410,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBRuled2_data) == NULL)
           {  state.fail(Memory,rs2.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->radius = v.getreal("rs::radius");
              rbdata->xmesh = v.getinteger("rs::xmesh");
              rbdata->ymesh = v.getinteger("rs::ymesh");
#ifdef USING_WIDGETS
              rbdata->qgc1=0; rbdata->qgc2=0; rbdata->qgline=0; rbdata->qgr1=0;
#endif
              if ((dialog = new Dialog(rs3.gets(),rs4.gets(),&rbdata->radius,ScaleByLength)) == NULL)
                {  state.fail(Memory,rs5.gets());
                   *cstate = ExitState;
                }
              else if (dialog->process() == TRUE)
                {  v.setreal("rs::radius",rbdata->radius);
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
              delete dialog;
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              line = (Line *) ((EntitySelectedEvent *)event)->getentity();
              if (create_pipe(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,line->getp1(),line->getp2(),&c1,&c2,&r1))
                {  db.geometry.add(c1);
                   db.geometry.add(c2);
                   db.geometry.add(r1);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              if (create_pipe(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,rbdata->p1,p2,&c1,&c2,&r1))
                {  db.geometry.add(c1);
                   db.geometry.add(c2);
                   db.geometry.add(r1);
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
       if(rbdata->qgc1)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgc1);
       delete rbdata->qgc1; rbdata->qgc1=0;
       if(rbdata->qgc2)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgc2);
       delete rbdata->qgc2; rbdata->qgc2=0;
       if(rbdata->qgr1)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgr1);
       delete rbdata->qgr1; rbdata->qgr1=0;
       if(rbdata->qgline)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
       delete rbdata->qgline; rbdata->qgline=0;
   }
#endif
#endif

  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBRuled2,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCRULEB+6);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCRULEB+7);
  else
    delete rbdata;
}

#endif

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief getRuledSurfChangeValues
 * @param change
 * @param list
 * @param hd
 * @param xm
 * @param ym
 * @param o
 * @return
 *
 * looks for Ruled Surface entites with the same property values and returns the change mask
 * if a bit in the change mask is set it means the values are the same and can be
 * displayed in the dialog for changing
 * if a bit is not set it means the values vary and the entity default option values will be used
 *
 */
int getRuledSurfChangeValues(BitMask *change,EntityList &list,EntityHeader *hd,int *xm, int *ym,BitMask *o)
{
    Entity *e;
    BitMask options(32),toptions(32);
    RuledSurface *surface;

    // values to be compared
    int i=0;
    for (list.start() ; (e = list.next()) != NULL ; )
    {
        if (e->isa(ruledsurface_entity))
        {
            surface = (RuledSurface*)e;

            if(i==0)
            {
                hd->setcolour(surface->getcolour());
                hd->setlayer(surface->getlayer());
                hd->setstyle(surface->getstyle());
                hd->setweight(surface->getweight());

                *xm  = surface->getxmesh();
                *ym  = surface->getymesh();
                options = surface->getoptions();
                *o = surface->getoptions();

                change->clearall();

                change->set(0);// def color
                change->set(1);// def layer
                change->set(2);// def style
                change->set(3);// def line weight

                change->set(4);// xmesh
                change->set(5);// ymesh
                change->set(6);// draw mode
            }
            else
            {
                toptions = surface->getoptions();

                if(hd->getcolour() != surface->getcolour())
                    change->clear(0);
                if(hd->getlayer() != surface->getlayer())
                    change->clear(1);
                if(hd->getstyle() != surface->getstyle())
                    change->clear(2);
                if(!LineWeights::isEqual(hd->getweight(),surface->getweight()))
                    change->clear(3);

                if(*xm != surface->getxmesh())
                    change->clear(4);// x mesh
                if(*ym != surface->getymesh())
                    change->clear(5);// y mesh
                if(options.test(1) != toptions.test(1) && options.test(2) != toptions.test(2))
                    change->clear(6);// draw mode
            }
            i++;
        }
    }
    return i;
}

/**
 * @brief ruledsurface_command3
 * @param cstate
 * @param event
 *
 * Entry point for the Ruled Surface Options command
 *
 *
 */
void ruledsurface_command3(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 int xmesh,ymesh;
 BitMask options(32),change(32);
 Entity *e;
 EntityHeader header;
 ResourceString rs8(NCRULEB+8);
 ResourceString rs9(NCRULEB+9);
 int status,length;
 RuledSurface *ruledsurface;

  v.setinteger("op::changecount",0);

  if (*cstate == InitialState)
    {  list = state.getselection().copy();
       if (list.empty())
       {
            RuledSurface::getdefaults(&xmesh,&ymesh,&options);
            status = RuledSurface::setup(NULL,&xmesh,&ymesh,&options);
            if (status == TRUE)
            {
                RuledSurface::setdefaults(xmesh,ymesh,options);
                 *cstate = ExitState;
            }
            else if (status == 110) // clone (104->110)
              *cstate = 1;
            else if (status == 111) // clone all (105->111)
              *cstate = 3;
            else if (status == 108) // surface properties
              *cstate = ExitState;
            else if (status == 109) // triangulate
              *cstate = ExitState;
            else
              *cstate = ExitState;
       }
       else
         {
            RuledSurface::getdefaults(&xmesh,&ymesh,&options);
            // get the change values for the selected ents and pass it to the dialog
            change.clearall();
            int found =  getRuledSurfChangeValues(&change,list,&header,&xmesh,&ymesh,&options);
            //status = RuledSurface::setup(&change,&xmesh,&ymesh,&options,&header);
            v.setinteger("op::changecount",found);
            if(found >= 1)
                status = RuledSurface::setup(&change,&xmesh,&ymesh,&options,&header);
            else
                status = RuledSurface::setup(&change,&xmesh,&ymesh,&options,&header);

            if (status == TRUE)
              {  
                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(ruledsurface_entity))
                     ((RuledSurface *)e)->change(change,header,xmesh,ymesh,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();

                 *cstate = ExitState;   
              }   
            else if (status == 110) // clone (104->110)
              *cstate = 2;
            else if (status == 111) // clone all (105->111)
              *cstate = 4;
            else if (status == 108) // surface properties
            {
                //SurfaceProperties sp;
                //sp.verify(this,0);
                *cstate = ExitState;
            }
            else if (status == 109) // triangulate
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
                    if ((tsurf = ((RuledSurface*)e)->triangulatedclone(1)) != 0)
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
            ruledsurface = (RuledSurface *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  RuledSurface::setdefaults(ruledsurface->getxmesh(),ruledsurface->getymesh(),ruledsurface->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(ruledsurface->getcolour(),ruledsurface->getlayer(),ruledsurface->getstyle(),ruledsurface->getweight());
                      v.setinteger("df::colour",ruledsurface->getcolour());
                      v.setinteger("df::layer",ruledsurface->getlayer());
                      v.setinteger("df::style",ruledsurface->getstyle());
                      v.setinteger("df::weight",ruledsurface->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              } 
            else
              {  if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 4 ; i <= 6 ; i++) change.set(i,1);

                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(ruledsurface_entity))
                     ((RuledSurface *)e)->change(change,*ruledsurface,ruledsurface->getxmesh(),ruledsurface->getymesh(),ruledsurface->getoptions());
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
    {  state.selmask.entity.clearandset(ruledsurface_entity,end_list);
       cadwindow->prompt->normalprompt("Select the ruled surface entity to clone from");
    }

}

void flatpattern_command(int *cstate,HighLevelEvent *,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libFlatPattern.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("FlatPattern.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Flat Pattern"); // Flat Pattern function
        }
        else
        {
            // put up a message to tell the user the Flat Pattern module is not installed
            QMessageBox::warning(0,"Flat Pattern","The Flat Pattern module is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}
