
#include "ncwin.h"

void trim_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Trim"));
  *cstate = ExitState;
}

struct RBTrim1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    //QGraphicsItem *qgi, *qgie;
    QList<QGraphicsItem*> qgipoints,qgients;
#endif
    RBTrim1_data() {  panel = 0;  }
   EntityList elist,enewlist;
   Entity *panel;
   Point panelp;
   GeneralList endlist;
   int state;
};

#ifdef NEW_RUBBERB
void RBTrim1(int dmode,void *data,Point *p2,View3dSurface *)
{RBTrim1_data *rbdata = (RBTrim1_data *) data;
 LinearEntity *e,*enew;
 Transform identity;
 Point start,endp;
 TrimCallbackData tcbd;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (rbdata->panel != 0)
    return;

  if (rbdata->state == 2)
    {
      int i=0;
      for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
      {
            start = e->start();
            endp = e->end();
            if ((start - *p2).length() < (endp - *p2).length())
            {
                 PointE point(start);
                 //point.qgi = 0; point.zValue = rbdata->zValue;
                 point.draw(DM_INVERT);
                 //rbdata->qgipoints.append(point.qgi); rbdata->zValue = point.zValue;
            }
            else
            {
                PointE point(endp);
                //point.qgi = 0; point.zValue = rbdata->zValue;
                point.draw(DM_INVERT);
                //rbdata->qgipoints.append(point.qgi); rbdata->zValue = point.zValue;
            }
            i++;
      }
    }
  else
    {
      identity.identity();
      int i=0;
       for (rbdata->enewlist.start(),rbdata->endlist.start() ; (enew = (LinearEntity *)rbdata->enewlist.next()) != 0 ; )
         {  tcbd.operation = 1;
            tcbd.p = *p2;
            tcbd.end = (LONG)rbdata->endlist.peek();
            tcbd.trimentity = enew;
            tcbd.trimtoentity = 0;
            if (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,3,&tcbd) == 0)
              {  db.geometry.clearstatus();
                 enew->setcopye(0);
                 e = (LinearEntity *) enew->clone(identity);
                 e->moveendpoint(0,(LONG)rbdata->endlist.next(),*p2);
                 //e->qgi = 0; e->zValue = rbdata->zValue;
                 e->Entity::draw(DM_INVERT);
                 //rbdata->qgients.append(e->qgi); rbdata->zValue = e->zValue;
                 delete e;
              }
            i++;
         }
    }
}
#else
void RBTrim1(int,void *data,Point *p2,View3dSurface *)
{RBTrim1_data *rbdata = (RBTrim1_data *) data;
 LinearEntity *e,*enew;
 Transform identity;
 Point start,endp;
 TrimCallbackData tcbd;

  if (rbdata->panel != 0)
    return;  

  if (rbdata->state == 2)
    {
#ifdef USING_WIDGETS
      int i=0;
      for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
      {
            start = e->start();
            endp = e->end();
            if ((start - *p2).length() < (endp - *p2).length())
            {
                 PointE point(start);
                 if(rbdata->qgipoints.size() > i)
                 {
                    point.qgi = rbdata->qgipoints.at(i); point.zValue = rbdata->zValue;
                    point.draw(DM_INVERT);
                    rbdata->qgipoints[i]=point.qgi; rbdata->zValue = point.zValue;
                 }
                 else
                 {
                     point.qgi = 0; point.zValue = rbdata->zValue;
                     point.draw(DM_INVERT);
                     rbdata->qgipoints.append(point.qgi); rbdata->zValue = point.zValue;
                 }
            }
            else
            {
                if(rbdata->qgipoints.size() > i)
                {
                    PointE point(endp);
                    point.qgi = rbdata->qgipoints.at(i); point.zValue = rbdata->zValue;
                    point.draw(DM_INVERT);
                    rbdata->qgipoints[i] = point.qgi; rbdata->zValue = point.zValue;
                }
                else
                {
                    PointE point(endp);
                    point.qgi = 0; point.zValue = rbdata->zValue;
                    point.draw(DM_INVERT);
                    rbdata->qgipoints.append(point.qgi); rbdata->zValue = point.zValue;
                }
            }
            i++;
      }
#else
      for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
      {
            start = e->start();
            endp = e->end();
            if ((start - *p2).length() < (endp - *p2).length())
            {
                PointE point(start);
                 point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
                 point.draw(DM_INVERT);
                 rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
            }
            else
            {
                PointE point(endp);
                 point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
                 point.draw(DM_INVERT);
                 rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
            }
         }
#endif
    }
  else
    {
#ifdef USING_WIDGETS
      identity.identity();
      int i=0;
       for (rbdata->enewlist.start(),rbdata->endlist.start() ; (enew = (LinearEntity *)rbdata->enewlist.next()) != 0 ; )
         {  tcbd.operation = 1;
            tcbd.p = *p2;
            tcbd.end = (LONG)rbdata->endlist.peek();
            tcbd.trimentity = enew;
            tcbd.trimtoentity = 0;
            if (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,3,&tcbd) == 0)
              {  db.geometry.clearstatus();
                 enew->setcopye(0);
                 e = (LinearEntity *) enew->clone(identity);
                 e->moveendpoint(0,(LONG)rbdata->endlist.next(),*p2);
                 if(rbdata->qgients.size() > i)
                 {
                     e->qgi = rbdata->qgients.at(i); e->zValue = rbdata->zValue;
                     e->Entity::draw(DM_INVERT);
                     rbdata->qgients[i] = e->qgi; rbdata->zValue = e->zValue;
                 }
                 else
                 {
                     e->qgi = 0; e->zValue = rbdata->zValue;
                     e->Entity::draw(DM_INVERT);
                     rbdata->qgients.append(e->qgi); rbdata->zValue = e->zValue;
                 }
                 delete e;
              }
            i++;
         }
#else
      identity.identity();
      int i=0;
       for (rbdata->enewlist.start(),rbdata->endlist.start() ; (enew = (LinearEntity *)rbdata->enewlist.next()) != 0 ; )
         {  tcbd.operation = 1;
            tcbd.p = *p2;
            tcbd.end = (LONG)rbdata->endlist.peek();
            tcbd.trimentity = enew; 
            tcbd.trimtoentity = 0; 
            if (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,3,&tcbd) == 0)
              {  db.geometry.clearstatus();
                 enew->setcopye(0); 
                 e = (LinearEntity *) enew->clone(identity);
                 e->moveendpoint(0,(LONG)rbdata->endlist.next(),*p2);
                 e->Entity::draw(DM_INVERT);
                 delete e;
              }
            i++;
         }
#endif
    }
}
#endif


static void AssociatedTrimCallback(Entity *e,void *)
{ if (e != 0)
    e->setcopye(0);
}

extern void TrimPanel(Group *panel1,Point p1,Entity *trime,Point p2,int mitre);

/**
 * @brief trim_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Trim an entity to another entity
 *
 * KMJ : working in Qt version
 */
void trim_command1(int *cstate,HighLevelEvent *event,void **data)
{RBTrim1_data *rbdata = (RBTrim1_data *) *data;
 Point  p,p2;
 Entity *e,*enew,*e1;
 int end;
 ResourceString rs30(NCTRIM+30);
 ResourceString rs31(NCTRIM+31);
 TrimCallbackData tcbd;

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim1_data;
#ifdef USING_WIDGETS
         rbdata->qgipoints.clear(); rbdata->qgients.clear();
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           {  rbdata->elist = state.getselection().copy();
              if (rbdata->elist.length() == 0)
                *cstate = 1;
              else
                {  *cstate = 2;
                   for (rbdata->elist.start() ; (e = rbdata->elist.next()) != NULL ; )
                     {  tcbd.operation = 0;
                        tcbd.trimentity = e; 
                        if (! e->islinear() && (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,1,&tcbd) == 0))
                          {  cadwindow->MessageBox(rs30.gets(),rs31.gets(),MB_ICONINFORMATION);
                             *cstate = ExitState;
                             break;
                          } 
                     }
                   if (*cstate != ExitState)
                     {  state.destroyselection(0);
                        if (rbdata->elist.length() == 1 && state.getlastend() > 0)
                          {Transform ident;
                             rbdata->endlist.add((void *)state.getlastend());
                             rbdata->elist.start();
                             e = rbdata->elist.next();
                             ident.identity();
                             db.geometry.clearstatus();
                             e->setcopye(0); 
                             e->dbtraverseassociated(AssociatedTrimCallback,0);
                             rbdata->enewlist.add(e->clone(ident));
                             state.excludelist.add(e);
                             *cstate = 3;  //  Go straight to the select trim entity state.
                          }
                     }
                }
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {Transform ident;
              e = ((EntitySelectedEvent *) event)->getentity();
              tcbd.operation = 0;
              tcbd.trimentity = e; 
              if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
                {  rbdata->panel = e;
                   rbdata->panelp =  state.getlastpickpoint();
                   *cstate = 3;
                }
              else if (! e->islinear() && (db.TrimCommandCallback == 0 || db.TrimCommandCallback != 0 && db.TrimCommandCallback(CCTrimCommand,1,&tcbd) == 0))
                e->unhighlight(); 
              else
                {  rbdata->elist.add(e);
                   rbdata->endlist.add((void *)((EntitySelectedEvent *) event)->getendselected());
                   ident.identity();
                   db.geometry.clearstatus();
                   e->setcopye(0); 
                   e->dbtraverseassociated(AssociatedTrimCallback,0);
                   rbdata->enewlist.add(e->clone(ident));
                   e->highlight();
                   state.excludelist.add(e);
                   *cstate = 3;  //  Go to the entity select state
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
                {Transform ident;
                   if (e->islinear())
                     rbdata->endlist.add((void *)(((((LinearEntity *)e)->start() - p2).length() > (((LinearEntity *)e)->end() - p2).length())+1));
                   else 
                     {  //  Calculate the end to trim
                        tcbd.operation = 0;
                        tcbd.trimentity = e; 
                        tcbd.end = 1; 
                        tcbd.p = p2;
                        if (db.TrimCommandCallback != 0)
                          db.TrimCommandCallback(CCTrimCommand,2,&tcbd);
                        rbdata->endlist.add((void *)tcbd.end);
                        //rbdata->qgil1.append(e->qgi);
                     }
                   ident.identity();
                   db.geometry.clearstatus();
                   e->setcopye(0); 
                   e->dbtraverseassociated(AssociatedTrimCallback,0);
                   rbdata->enewlist.add(e->clone(ident));
                   state.excludelist.add(e);
                }
              rbdata->endlist.reverse();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  *cstate = 1;
              state.excludelist.destroy();
              rbdata->elist.unhighlight();
              rbdata->enewlist.destroy();
              rbdata->elist.destroy();
              rbdata->endlist.destroy();
           }
         break;
       case 3 :
         rubberband.end(0);
         rbdata->elist.unhighlight();
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);

              if (rbdata->panel != 0)
                {//  Trim the panel to a point
                   TrimPanel((Group *)rbdata->panel,rbdata->panelp,0,p2,0);
                }
              else
                {
                   for (rbdata->elist.start(),rbdata->endlist.start() ; (e = rbdata->elist.next()) != 0 ; )
                     {TrimCallbackData tcbd;
                        tcbd.operation = 0;
                        tcbd.p = p2;
                        tcbd.end = (LONG)rbdata->endlist.peek();
                        tcbd.trimentity = e; 
                        tcbd.trimtoentity = 0; 
                        if (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,3,&tcbd) == 0 && e->islinear())
                          ((LinearEntity *)e)->moveendpoint(1,(LONG)rbdata->endlist.next(),p2);
                        state.excludelist.del(e);
                    }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e1 = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);

              if (rbdata->panel != 0)
                {//  Trim the panel to a point
                   TrimPanel((Group *)rbdata->panel,rbdata->panelp,e1,p,0);
                }
              else
                {  for (rbdata->elist.start(),rbdata->endlist.start() ; (e = rbdata->elist.next()) != 0 ; )
                     {TrimCallbackData tcbd;
                        tcbd.operation = 0;
                        tcbd.p = p;
                        tcbd.end = (LONG)rbdata->endlist.peek();
                        tcbd.trimentity = e; 
                        tcbd.trimtoentity = e1; 
                        if (db.TrimCommandCallback == 0 || db.TrimCommandCallback(CCTrimCommand,3,&tcbd) == 0 && e->islinear())
                          {  Intersect i(e,e1,p);
                             end = (LONG)rbdata->endlist.next();
                             if (i.nintersections() == 1)
                               {  p2 = ((PointE *)i.intersection(0))->getp();
                                  ((LinearEntity *)e)->moveendpoint(1,end,p2);
                               }
                          }
                        state.excludelist.del(e);
                     }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {  state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           *cstate = ExitState;
         if (db.TrimCommandCallback == 0)
           {// It is tooo hard to tidy up temporary nested groups for now

              for (rbdata->enewlist.start() ; (enew = rbdata->enewlist.next()) != 0 ; )
                delete enew;
           }
         rbdata->enewlist.destroy();
         rbdata->elist.destroy();
         rbdata->endlist.destroy();

         break;
    }
  //
  //  About to enter state actions
  //
  if (*cstate == 1)
    {  if (db.TrimCommandCallback == 0)
         strcpy(state.groupnamemask,"RC::Panel");
       state.selmask.entity.clearandset(linear_mask,group_entity,end_list);
    }
  else if (*cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  else if (*cstate == 3)
    {  if (db.TrimCommandCallback == 0)
         strcpy(state.groupnamemask,"RC::Panel");
       state.selmask.entity.clearandset(xyz_mask,linear_mask,plane_entity,group_entity,end_list);
    }

  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRIM);
  else if (*cstate == 2)
    {  rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBTrim1,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+32);
    }
  else if (*cstate == 3)
    {  rbdata->state = 3;
       rubberband.begin(0);
       rubberband.add(RBTrim1,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+1);
    }
  else if (*cstate == ExitState)
    {  state.excludelist.destroy();

       if (rbdata != 0)
         {  rbdata->elist.destroy();
            rbdata->enewlist.destroy();
            rbdata->endlist.destroy();
         }

       delete rbdata;

       strcpy(state.groupnamemask,"");
       
    }
}

struct RBTrim2_data
{
#ifdef USING_WIDGETS
   qreal zValue;
   QList<QGraphicsItem*> qgilist;
#endif
   EntityList elist;
   double length;
   int tolength;
};

#ifdef NEW_RUBBERB
void RBTrim2(int dmode,void *data,Point *p2,View3dSurface *)
{RBTrim2_data *rbdata = (RBTrim2_data *) data;
 LinearEntity *enew,*e;
 Transform identity;
 double l,t;
 int end;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

 int i=0;
 for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
 {
      identity.identity();
      e->setcopye(0);
      enew = (LinearEntity *) e->clone(identity);
      end = ((e->start() - *p2).length() > (e->end() - *p2).length()) + 1;
      l = e->length();
      if (l > db.getptoler())
      {
           t = (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength ? rbdata->length / l : (l - rbdata->length) / l);
           enew->moveendpoint(0,end,enew->position(t));
           //enew->qgi = 0; enew->zValue = rbdata->zValue;
           enew->Entity::draw(DM_INVERT);
           //rbdata->qgilist.append(enew->qgi); rbdata->zValue = enew->zValue;
      }
      delete enew;
      i++;
 }
}
#else
void RBTrim2(int,void *data,Point *p2,View3dSurface *)
{RBTrim2_data *rbdata = (RBTrim2_data *) data;
 LinearEntity *enew,*e;
 Transform identity;
 double l,t;
 int end;

#ifdef USING_WIDGETS
 int i=0;
 for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
 {
      identity.identity();
      e->setcopye(0);
      enew = (LinearEntity *) e->clone(identity);
      end = ((e->start() - *p2).length() > (e->end() - *p2).length()) + 1;
      l = e->length();
      if (l > db.getptoler())
      {
           t = (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength ? rbdata->length / l : (l - rbdata->length) / l);
           enew->moveendpoint(0,end,enew->position(t));
           if(rbdata->qgilist.size() > i)
           {
               enew->qgi = rbdata->qgilist.at(i); enew->zValue = rbdata->zValue;
               enew->Entity::draw(DM_INVERT);
               rbdata->qgilist[i] = enew->qgi; rbdata->zValue = enew->zValue;
           }
           else
           {
               enew->qgi = 0; enew->zValue = rbdata->zValue;
               enew->Entity::draw(DM_INVERT);
               rbdata->qgilist.append(enew->qgi); rbdata->zValue = enew->zValue;
           }
      }
      delete enew;
      i++;
 }
#else
  for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
    {  identity.identity();
       e->setcopye(0);
       enew = (LinearEntity *) e->clone(identity);
       end = ((e->start() - *p2).length() > (e->end() - *p2).length()) + 1;
       l = e->length();
       if (l > db.getptoler())
         {  t = (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength ? rbdata->length / l : (l - rbdata->length) / l);
            enew->moveendpoint(0,end,enew->position(t));
            enew->Entity::draw(DM_INVERT);
         }
       delete enew;
    }
#endif
}
#endif

/**
 * @brief trim_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Trim bylength or trim entity to a length
 *
 */
void trim_command2(int *cstate,HighLevelEvent *event,void **data)
{RBTrim2_data *rbdata = (RBTrim2_data *) *data;
 double t,l;
 int end;
 Point p;
 Entity *e;
 ResourceString rs2(NCTRIM+2);
 ResourceString rs3(NCTRIM+3);
 ResourceString rs30(NCTRIM+30);
 ResourceString rs34(NCTRIM+34);
 Dialog dialog(_RCT("Trim_Dialog"));
 DialogControl *dc;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim2_data;
#ifdef USING_WIDGETS
         rbdata->qgilist.clear();
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           {  rbdata->length = v.getreal("tm::length");
              rbdata->tolength = v.getinteger("tm::tolength");
              dialog.add(dc = new RealDialogControl(100,&rbdata->length,ScaleByLength));
              // dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(new RadioButtonDialogControl(101,101,102,&rbdata->tolength));
              dialog.add(new RadioButtonDialogControl(102,101,102,&rbdata->tolength));
              if (dialog.process() == TRUE)
                {  v.setreal("tm::length",rbdata->length);
                   v.setinteger("tm::tolength",rbdata->tolength);
                   *cstate = 1;
                   rbdata->elist = state.getselection().copy();
                   if (rbdata->elist.length() > 0)
                     {  *cstate = 2;
                        for (rbdata->elist.start() ; (e = rbdata->elist.next()) != NULL ; )
                          if (! e->islinear())
                            {  cadwindow->MessageBox(rs30.gets(),rs34.gets(),MB_ICONINFORMATION);
                               *cstate = ExitState;
                               break;
                            }
                        if (*cstate != ExitState)
                          {  state.destroyselection(0);
                             if (rbdata->elist.length() == 1 && state.getlastend() > 0)
                               {Transform ident;
                                  rbdata->elist.start();
                                  e = rbdata->elist.next();
                                  e->unhighlight();
                                  end = state.getlastend();
                                  l = ((LinearEntity *)e)->length();
                                  if (l > db.getptoler())
                                    {  if (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength)
                                         t = rbdata->length / l;
                                       else
                                         t = (l - rbdata->length) / l;
                                       p = ((LinearEntity *)e)->position(t);
                                       ((LinearEntity *)e)->moveendpoint(1,end,p);
                                    }
                                  *cstate = 1;
                               }
                          }
                     }
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  e = ((EntitySelectedEvent *) event)->getentity();
              end = ((EntitySelectedEvent *) event)->getendselected();
              l = ((LinearEntity *)e)->length();
              if (l > db.getptoler())
                {  if (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength)
                     t = rbdata->length / l;
                   else
                     t = (l - rbdata->length) / l;
                   p = ((LinearEntity *)e)->position(t);
                   ((LinearEntity *)e)->moveendpoint(1,end,p);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         rbdata->elist.unhighlight();
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);
              for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
                {  end = ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length()) + 1;
                   l = ((LinearEntity *)e)->length();
                   if (l > db.getptoler())
                     {  t = (end == 1 && ! rbdata->tolength || end == 2 && rbdata->tolength ? rbdata->length / l : (l - rbdata->length) / l);
                        ((LinearEntity *)e)->moveendpoint(1,end,((LinearEntity *)e)->position(t));
                     }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              rbdata->elist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  for(int i=0; i<rbdata->qgilist.size(); i++)
  {
      if(rbdata->qgilist.at(i))
      {
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgilist.at(i));
          delete rbdata->qgilist[i]; rbdata->qgilist[i]=0;
      }
  }
  rbdata->qgilist.clear();
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(line_entity,circle_entity,ellipse_entity,curve_entity,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+4);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBTrim2,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+35);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         rbdata->elist.destroy();
       delete rbdata;
    }
}

struct RBTrim3_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QList<QGraphicsItem *> qgents, qgpoints;
    PointList gplist;
#endif
    EntityList elist,edlist;
    PointList plist;
};

#ifdef NEW_RUBBERB
void RBTrim3(int dmode, void *data,Point *p2,View3dSurface *)
{RBTrim3_data *rbdata = (RBTrim3_data *) data;
 Entity *e;
  int i=0;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();

      return;
  }

  for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
  {
       PointE point(e->nearp(*p2));
       //point.qgi = 0; point.zValue = rbdata->zValue;
       point.draw(DM_INVERT);
       //rbdata->qgents.append(point.qgi); rbdata->zValue = point.zValue;
  }
}
#else
void RBTrim3(int,void *data,Point *p2,View3dSurface *)
{RBTrim3_data *rbdata = (RBTrim3_data *) data;
 Entity *e;
#ifdef USING_WIDGETS
  int i=0;
  for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
  {
       PointE point(e->nearp(*p2));
       if(rbdata->qgents.size() > i)
       {
           point.qgi = rbdata->qgents.at(i); point.zValue = rbdata->zValue;
           point.draw(DM_INVERT);
           rbdata->qgents[i] = point.qgi; rbdata->zValue = point.zValue;
       }
       else
       {
           point.qgi = 0; point.zValue = rbdata->zValue;
           point.draw(DM_INVERT);
           rbdata->qgents.append(point.qgi); rbdata->zValue = point.zValue;
       }
       i++;
  }
#else
      for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
      {
          PointE point(e->nearp(*p2));
          point.draw(DM_INVERT);
      }
#endif
}
#endif

/**
 * @brief trim_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Divide entity by another entity
 *
 * KMJ : working in Qt version
 */
void trim_command3(int *cstate,HighLevelEvent *event,void **data)
{
 RBTrim3_data *rbdata = (RBTrim3_data *) *data;
 Point  p,p2;
 double d,dmin;
 int j;
 Entity *e,*e1,*e2;
 ResourceString rs163(NCMAIN+163),rs33(NCTRIM+33);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim3_data;
#ifdef USING_WIDGETS
         rbdata->qgents.clear(); rbdata->qgpoints.clear();
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);         
#endif
         *data = rbdata;
         rbdata->elist = state.getselection().copy();
         if (rbdata->elist.length() == 0)
           *cstate = 1;
         else
           {  *cstate = 2;
              for (rbdata->elist.start() ; (e = rbdata->elist.next()) != NULL ; )
                if (! e->islinear())
                  {  cadwindow->MessageBox(rs33.gets(),rs163.gets(),MB_ICONINFORMATION);
                     *cstate = ExitState;
                     break;
                  }
              if (*cstate != ExitState)
                {  for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
                     state.excludelist.add(e);
                   state.destroyselection(0);
                }
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {
             state.destroyselection(0);// KMJ: added for delete bug fix 2016/7/12
             rbdata->elist.add(((EntitySelectedEvent *) event)->getentity());
              state.excludelist.add(((EntitySelectedEvent *) event)->getentity());
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
          if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);
              rbdata->elist.unhighlight();
              for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
                {  ((LinearEntity *)e)->divide(p2);
                   state.excludelist.del(e);
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              rbdata->elist.destroy();
              *cstate = 1;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(0);

              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              if (rbdata->edlist.inlist(e))
                {EntityList edlist;
                 PointList plist;
                 Point p;
                 Entity *ed;
                   for (rbdata->edlist.start(),rbdata->plist.start() ;  ! rbdata->edlist.atend() ; )
                     {  ed = rbdata->edlist.next();
                        p = rbdata->plist.next();
                        if (ed != e)
                          {  edlist.add(ed);
                             plist.add(p);
                          }
                     }
                   e->unhighlight();
                   e->unhighlight(); //  Needs to be unhighlighted twice.
                   rbdata->edlist.destroy();
                   rbdata->plist.destroy();
                   rbdata->edlist = edlist;
                   rbdata->plist = plist;
                }
              else
                {  rbdata->edlist.add(e);
                   rbdata->plist.add(p);
                }

              if (! ((EntitySelectedEvent *)event)->getaddtoselection())
                {int many;
                   rbdata->elist.unhighlight();
                   rbdata->edlist.unhighlight();
                   many = rbdata->edlist.length() > 0;
                   db.saveundo(UD_STARTBLOCK,NULL);
                   for (rbdata->elist.start() ; (e1 = rbdata->elist.next()) != 0 ; )
                     {EntityList dlist;
                      Entity *enew;
                        dlist.add(e1);
                        for (rbdata->edlist.start(),rbdata->plist.start() ; ! rbdata->edlist.atend() ; )
                          {  e = rbdata->edlist.next();
                             p = rbdata->plist.next();
                             for (dlist.start() ; (e2 = dlist.next()) != 0 ; )
                               {Intersect i(e,e2);
                                  if (! many)
                                    {  for (j = 0,dmin = -1.0 ; j < i.nintersections() ; j++)
                                         {  d = (((PointE *)i.intersection(j))->getp() - p).length();
                                            if (dmin == -1.0 || d < dmin)
                                              {  dmin = d;  p2 = ((PointE *)i.intersection(j))->getp();
                                              }
                                         }
                                       if (dmin >= 0.0)
                                         ((LinearEntity *)e2)->divide(p2);
                                    }
                                  else
                                    {  for (j = 0 ; j < i.nintersections() ; j++)
                                         {  p2 = ((PointE *)i.intersection(j))->getp();
                                            if ((e2->nearp(p2) - p2).length() < db.getptoler())
                                              {
                                                 enew = ((LinearEntity *)e2)->divide(p2);
                                                 if (enew != 0)
                                                   dlist.add(enew);
                                              }
                                         }
                                    }

                               }
                             state.excludelist.del(e1);
                          }
                        dlist.destroy();
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                   *cstate = 1;
                   rbdata->elist.destroy();
                   rbdata->plist.destroy();
                   rbdata->edlist.destroy();
                }
           }
         else if (event->isa(Abort))
           {  rbdata->elist.unhighlight();
              rbdata->edlist.unhighlight();
              for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
                state.excludelist.del(e);
              rbdata->elist.destroy();
              rbdata->edlist.destroy();
              rbdata->plist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  rbdata->elist.unhighlight();
              rbdata->edlist.unhighlight();
              for (rbdata->elist.start() ; (e = rbdata->elist.next()) != 0 ; )
                state.excludelist.del(e);
              rbdata->elist.destroy();
              rbdata->edlist.destroy();
              rbdata->plist.destroy();
              *cstate = ExitState;
           }
         break;
    }
#ifdef USING_WIDGETS
//  cadwindow->clearAllViews();
#ifndef NEW_RUBBERB
//  cadwindow->clearAllRubberBands();
//#else
  for(int i=0; i<rbdata->qgents.size(); i++)
  {
      if(rbdata->qgents.at(i))
          cadwindow->getcurrentsurface()->getScene()->removeItem((rbdata->qgents.at(i)));
      delete rbdata->qgents[i]; rbdata->qgents[i]=0;
  }
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    state.selmask.entity.clearandset(linear_mask,end_list);
  else if (*cstate == 2)
    {  if (rbdata->edlist.length() == 0)
         state.selmask.entity.clearandset(xyz_mask,linear_mask,end_list);
       else
         state.selmask.entity.clearandset(linear_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRIM+5);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBTrim3,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+6);
    }
  else if (*cstate == ExitState)
    {
      state.excludelist.destroy();
       delete rbdata;
    }
}

struct RBTrim4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QList<QGraphicsItem*> qgiList;
#endif
    EntityList elist;
   double length;
};

#ifdef NEW_RUBBERB
void RBTrim4(int dmode,void *data,Point *p2,View3dSurface *)
{RBTrim4_data *rbdata = (RBTrim4_data *) data;
 LinearEntity *e;
 Transform identity;
 Point p;
 double t,l;
  int i=0;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
  {
       l = e->length();
       if (l > db.getptoler())
       {
            if ((e->start() - *p2).length() < (e->end() - *p2).length())
              t = rbdata->length / l;
            else
              t = (l - rbdata->length) / l;
            PointE point(e->position(t));
            point.draw(DM_INVERT);
       }
  }
}
#else
void RBTrim4(int,void *data,Point *p2,View3dSurface *)
{RBTrim4_data *rbdata = (RBTrim4_data *) data;
 LinearEntity *e;
 Transform identity;
 Point p;
 double t,l;
  int i=0;
  for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
  {
       l = e->length();
       if (l > db.getptoler())
       {
            if ((e->start() - *p2).length() < (e->end() - *p2).length())
              t = rbdata->length / l;
            else
              t = (l - rbdata->length) / l;
#ifdef USING_WIDGETS
            PointE point(e->position(t));
            if(rbdata->qgiList.size() > i)
            {
                point.qgi = rbdata->qgiList.at(i); point.zValue = rbdata->zValue;
                point.draw(DM_INVERT);
                rbdata->qgiList[i] = point.qgi; rbdata->zValue = point.zValue;
            }
            else
            {
                point.qgi = 0; point.zValue = rbdata->zValue;
                point.draw(DM_INVERT);
                rbdata->qgiList.append(point.qgi); rbdata->zValue = point.zValue;
            }
            i++;
#else
            PointE point(e->position(t));
            point.draw(DM_INVERT);
#endif
       }
  }
}
#endif

/**
 * @brief trim_command4
 * @param cstate
 * @param event
 * @param data
 *
 * divide entity by incremental length
 *
 * KMJ : working in Qt version
 */
void trim_command4(int *cstate,HighLevelEvent *event,void **data)
{RBTrim4_data *rbdata = (RBTrim4_data *) *data;
 double t,l,length;
 int end;
 Point p;
 LinearEntity *e;
 ResourceString rs7(NCTRIM+7);
 ResourceString rs8(NCTRIM+8),rs33(NCTRIM+33),rs164(NCMAIN+164);
 Dialog dialog(rs7.gets(),rs8.gets(),&length,ScaleByLength);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim4_data;
#ifdef USING_WIDGETS
         rbdata->qgiList.clear();
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           {  length = v.getreal("tm::length");
              if (dialog.process() == TRUE)
                {  v.setreal("tm::length",length);
                   rbdata->length = length;
                   rbdata->elist = state.getselection().copy();
                   if (rbdata->elist.length() == 0)
                     *cstate = 1;
                   else
                     {  *cstate = 2;
                        for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != NULL ; )
                          if (! e->islinear())
                            {  cadwindow->MessageBox(rs33.gets(),rs164.gets(),MB_ICONINFORMATION);
                               *cstate = ExitState;
                               break;
                            }
                        if (*cstate != ExitState)
                          {  state.destroyselection(0);
                             if (rbdata->elist.length() == 1 && state.getlastend() > 0)
                               {Transform ident;
                                  rbdata->elist.start();
                                  e = (LinearEntity *)rbdata->elist.next();
                                  e->unhighlight();
                                  l = ((LinearEntity *)e)->length();
                                  if (l > db.getptoler())
                                    {  if (state.getlastend() == 1)
                                         t = length / l;
                                       else
                                         t = (l - length) / l;
                                       p = ((LinearEntity *)e)->position(t);
                                       db.saveundo(UD_STARTBLOCK,NULL);
                                       ((LinearEntity *)e)->divide(p);
                                       db.saveundo(UD_ENDBLOCK,NULL);
                                    }
                                  *cstate = 1;
                               }
                          }
                     }
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = (LinearEntity *) ((EntitySelectedEvent *) event)->getentity();
              end = ((EntitySelectedEvent *) event)->getendselected();
              l = ((LinearEntity *)e)->length();
              if (l > db.getptoler())
                {  if (end == 1)
                     t = rbdata->length / l;
                   else
                     t = (l - rbdata->length) / l;
                   p = ((LinearEntity *)e)->position(t);
                   db.saveundo(UD_STARTBLOCK,NULL);
                   ((LinearEntity *)e)->divide(p);
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         rbdata->elist.unhighlight();
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *) event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);
              for (rbdata->elist.start() ; (e = (LinearEntity *)rbdata->elist.next()) != 0 ; )
                {  l = e->length();
                   if (l > db.getptoler())
                     {  if ((e->start() - p).length() < (e->end() - p).length())
                          t = rbdata->length / l;
                        else
                          t = (l - rbdata->length) / l;
                        e->divide(e->position(t));
                     }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              rbdata->elist.destroy();
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
  for(int i=0; i<rbdata->qgiList.size(); i++)
  {
      if(rbdata->qgiList.at(i))
      {
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiList.at(i));
          delete rbdata->qgiList[i]; rbdata->qgiList[i]=0;
      }
  }
  rbdata->qgiList.clear();
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+9);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBTrim4,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+36);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0) rbdata->elist.destroy();
       delete rbdata;
    }
}

/**
 * @brief trim_command5
 * @param cstate
 * @param event
 *
 * Divide entity into a number of sections
 */
void trim_command5(int *cstate,HighLevelEvent *event,void **)
{static int n;
 int status;
 Entity *e;
 Dialog *dialog;
 ResourceString rs10(NCTRIM+10),rs11(NCTRIM+11),rs33(NCTRIM+33);
 EntityList list;
  switch (*cstate)
    {  case InitialState :
         n = v.getinteger("tm::sections");
         dialog = new Dialog(rs10.gets(),rs11.gets(),&n);
         status = dialog->process();
         delete dialog;
         if (status == TRUE)
           {  v.setinteger("tm::sections",n);
              list = state.getselection().copy();
              *cstate = 1;
              for (list.start() ; (e = list.next()) != NULL ; )
                if (! e->islinear())
                  {  cadwindow->MessageBox(rs33.gets(),rs10.gets(),MB_ICONINFORMATION);
                     *cstate = ExitState;
                     break;
                  }
              if (*cstate != ExitState)
                {  state.destroyselection(1);
                   db.saveundo(UD_STARTBLOCK,NULL);
                   for (list.start() ; (e = list.next()) != 0 ; )
                     ((LinearEntity *)e)->divide(n);
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              db.saveundo(UD_STARTBLOCK,NULL);
              ((LinearEntity *)e)->divide(n);
              db.saveundo(UD_ENDBLOCK,NULL);
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+12);
    }
}

/**
 * @brief trim_command6
 * @param cstate
 * @param event
 *
 * Offset by given distance command
 *
 * KMJ : Working in Qt Version
 *
 */
void trim_command6(int *cstate,HighLevelEvent *event,void **)
{static Loops *loops;
 static int cornermode,oldsnap;
 static EntityList *slist;
 static double offsetd,toffsetd[2];
 static Entity *oneentity;
 EntityList list;
 Entity *e;
 Point origin,xaxis,yaxis,zaxis,p;
 double D;
 int linear,side,ok;
 Dialog dialog(_RCT("Offsetd_Dialog"));
 DialogControl *dc;
 ResourceString rs13(NCTRIM+13);
 ResourceString rs14(NCTRIM+14);
 ResourceString rs15(NCTRIM+15);
 ResourceString rs16(NCTRIM+16);
 ResourceString rs17(NCTRIM+17);
 ResourceString rs18(NCTRIM+18);

  switch (*cstate)
    {case InitialState :
       slist = NULL;  loops = NULL;  oneentity = 0;
       if ((loops = new Loops(0)) == NULL || (slist = new EntityList) == NULL)
         {  state.fail(Memory,rs13.gets());
            *cstate = ExitState;
         }
       else
         {  *cstate = 1;
            *slist = state.getselection().copy();
            list = slist->copy();
            state.destroyselection(1);
            ok = 0;
            for (list.start(),linear = 1 ; (e = list.next()) != NULL ; )
              if (! e->islinear())
                {  linear = 0;  break;
                }
            if (! linear)
              {  cadwindow->MessageBox(rs14.gets(),rs15.gets(),MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else if (list.length() == 0)
              {  cadwindow->MessageBox(rs16.gets(),rs15.gets(),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            if (*cstate == 1 && list.length() == 1)
              {  list.start();  oneentity = list.next();  ok = 1;
                 oneentity->highlight();
              }
            else if (*cstate == 1)
              {  loops->create(&list);
                 if (loops->nloops() > 1)
                   {  cadwindow->MessageBox(rs17.gets(),rs15.gets(),MB_ICONINFORMATION);
                      loops->destroy();
                      *cstate = ExitState;
                   }
                 else if (! loops->planar(&origin,&xaxis,&yaxis,&zaxis,&D))
                   {  cadwindow->MessageBox(rs18.gets(),rs15.gets(),MB_ICONINFORMATION);
                      loops->destroy();
                      *cstate = ExitState;
                   }
                 else
                   ok = 1;
              }
            if (ok)
              {  offsetd = v.getreal("tr::offsetd");
                 cornermode = v.getinteger("tr::cornermode");
                 dialog.add(dc = new RealDialogControl(100,&offsetd,ScaleByLength));
                 //dialog.add(new ScrollBarDialogControl(1100,dc));
                 dialog.add(new CheckBoxDialogControl(101,&cornermode));
                 if (dialog.process() == TRUE)
                   {  v.setreal("tr::offsetd",offsetd);
                      v.setinteger("tr::cornermode",cornermode);
                      toffsetd[0] = offsetd;
                      toffsetd[1] = offsetd;
                      if (oneentity == 0)
                        {  list = loops->getloops();
                           list.start();
                           ((Entity *)list.peek())->highlight();
                        }
                      *cstate = 1;
                   }
                 else
                   {  if (oneentity != 0)
                        oneentity->unhighlight();
                      *cstate = ExitState;
                   }
              }
         }
       break;
     case 1 :
       db.grid.setsnap(oldsnap);
       if (oneentity == 0)
         {  list = loops->getloops();
            list.start();
            ((Entity *)list.peek())->unhighlight();
         }
       else
         oneentity->unhighlight();
       if (event->isa(Coordinate3d))
         {  side = 0;
            p = ((Coordinate3dEvent *) event)->getp();
            if (oneentity == 0)
              {  
                 side = loops->get_offset_side(p,toffsetd[0]);
                 loops->offset(1,cornermode,p,toffsetd[side],NULL);
                 list = loops->getloops();
                 list.start();
                 ((Entity *)list.peek())->highlight();
              }
            else
              {  if (oneentity->isa(line_entity))
                   {  side = (p - oneentity->nearp(p)).cross(((Line *)oneentity)->direction(0.0)).z > 0.0;
                      p = (p - oneentity->nearp(p)).normalize() * toffsetd[side];
                      Line *line = new Line(((Line *)oneentity)->getp1()+p,((Line *)oneentity)->getp2()+p);
                      if (line != 0 && line->getdefined())
                        db.geometry.add(line);
                      else
                        delete line;
                   }
                 else if (oneentity->isa(circle_entity))
                   {Circle *circle;
                      side = (p - ((Circle *)oneentity)->getorigin()).length() > ((Circle *)oneentity)->getradius();
                      if (side)
                        circle = new Circle(((Circle *)oneentity)->getorigin(),((Circle *)oneentity)->getxaxis(),((Circle *)oneentity)->getyaxis(),
                                            ((Circle *)oneentity)->getradius()+toffsetd[side],((Circle *)oneentity)->getstarta(),((Circle *)oneentity)->getsweepa());
                      else
                        circle = new Circle(((Circle *)oneentity)->getorigin(),((Circle *)oneentity)->getxaxis(),((Circle *)oneentity)->getyaxis(),
                                            ((Circle *)oneentity)->getradius()-toffsetd[side],((Circle *)oneentity)->getstarta(),((Circle *)oneentity)->getsweepa());
                      if (circle != 0 && circle->getdefined())
                        db.geometry.add(circle);
                      else
                        delete circle;
                   }
                 else if (oneentity->isa(curve_entity) || oneentity->isa(ellipse_entity))
                   {Point zaxis(0.0,0.0,1.0),startp,dir1,dir2;
                    int reverse;
                    double t,d;
                    EntityList olist; 
                      reverse = 0;
                      dir1 = ((LinearEntity *)oneentity)->direction(0.0);
                      for (t = 0.0 ; t <= 1.0 ; t += 0.1)
                        {  dir2 = ((LinearEntity *)oneentity)->direction(t);
                           if (dir1.dot(dir2) < 0.99)
                             {  zaxis = dir1.cross(dir2).normalize();
                                break;
                             }
                        }

                      d = offsetd;
                      ((LinearEntity *)oneentity)->beginoffset(p,&zaxis,&d,0,&reverse,&startp);
                      if (fabs(offsetd) >= db.getptoler())
                      { if (fabs(zaxis.z) > fabs(zaxis.y))
                            side = zaxis.z > 0.0;
                        else
                            side = zaxis.y > 0.0;
                        d = toffsetd[side];
                      }
                      ((LinearEntity *)oneentity)->offset(zaxis,d,0,0,&reverse,&startp,&olist);
                      olist.start();
                      if (olist.length() > 0)
                        db.geometry.add(olist.next());
                   }
                 oneentity->unhighlight();
              }
            toffsetd[side] += offsetd;
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  loops->destroy();
            *cstate = ExitState;
         }
       break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+19);
       oldsnap = db.grid.getsnap();
       db.grid.setsnap(0);
    }
  else if (*cstate == ExitState)
    {  if (slist != NULL) state.setselection(*slist);
       delete slist;  delete loops;
    }
}

/**
 * @brief trim_command7
 * @param cstate
 * @param event
 *
 * Offset entities by screen picking a position_command
 *
 * KMJ : Working in Qt Version
 *
 */
void trim_command7(int *cstate,HighLevelEvent *event,void **)
{static Loops *loops;
 static int cornermode;
 static EntityList *slist;
 static Entity *oneentity;
 Dialog dialog(_RCT("Offset_Dialog"));
 EntityList list;
 Entity *e;
 Point origin,xaxis,yaxis,zaxis,p,p1;
 double D;
 int linear,ok;
 ResourceString rs15(NCTRIM+15);
 ResourceString rs20(NCTRIM+20);
 ResourceString rs21(NCTRIM+21);
 ResourceString rs22(NCTRIM+22);
 ResourceString rs23(NCTRIM+23);
 ResourceString rs24(NCTRIM+24);
  switch (*cstate)
    {case InitialState :
       slist = NULL;  loops = NULL;  oneentity = 0;
       if ((loops = new Loops(0)) == NULL || (slist = new EntityList) == NULL)
         {  state.fail(Memory,rs20.gets());
            *cstate = ExitState;
         }
       else
         {  *cstate = 1;
            *slist = state.getselection().copy();
            list = slist->copy();
            state.destroyselection(1);
            ok = 0;
            for (list.start(),linear = 1 ; (e = list.next()) != NULL ; )
              if (! e->islinear())
                {  linear = 0;  break;
                }
            if (! linear)
              {  cadwindow->MessageBox(rs21.gets(),rs15.gets(),MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else if (list.length() == 0)
              {  cadwindow->MessageBox(rs22.gets(),rs15.gets(),MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            if (*cstate == 1 && list.length() == 1)
              {  list.start();  oneentity = list.next();  ok = 1;
                 oneentity->highlight();
              }
            else if (*cstate == 1)
              {  loops->create(&list);
                 if (loops->nloops() > 1)
                   {  cadwindow->MessageBox(rs23.gets(),rs15.gets(),MB_ICONINFORMATION);
                      loops->destroy();
                      *cstate = ExitState;
                   }
                 else if (! loops->planar(&origin,&xaxis,&yaxis,&zaxis,&D))
                   {  cadwindow->MessageBox(rs24.gets(),rs15.gets(),MB_ICONINFORMATION);
                      loops->destroy();
                      *cstate = ExitState;
                   }
                 else
                   ok = 1;
              }
            if (ok)
              {  cornermode = v.getinteger("tr::cornermode");
                 dialog.add(new CheckBoxDialogControl(100,&cornermode));
                 if (dialog.process() == TRUE)
                   {  v.setinteger("tr::cornermode",cornermode);
                      if (oneentity == 0)
                        {  list = loops->getloops();
                           list.start();
                           ((Entity *)list.peek())->highlight();
                        }
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1 :
       if (oneentity == 0)
         {  list = loops->getloops();
            list.start();
            ((Entity *)list.peek())->unhighlight();
         }
       else
         oneentity->unhighlight();
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();

            if (loops->planar(&origin,&xaxis,&yaxis,&zaxis,&D))
              {  //  If the entities are planar, project the point onto the plane first.
                 InfinitePlane ip(zaxis,origin);
                 p = ip.nearp(p);           
              }  



            if (oneentity == 0)
              {  loops->offset(1,cornermode,p,0.0,NULL);
                 list = loops->getloops();
                 list.start();
                 ((Entity *)list.peek())->highlight();
              }
            else
              {  if (oneentity->isa(line_entity))
                   {  p = p - oneentity->nearp(p);
                      Line *line = new Line(((Line *)oneentity)->getp1()+p,((Line *)oneentity)->getp2()+p);
                      if (line != 0 && line->getdefined())
                        db.geometry.add(line);
                      else
                        delete line;
                   }
                 else if (oneentity->isa(circle_entity))
                   {Circle *circle;
                      p1 = oneentity->nearp(p);
                      if ((p - ((Circle *)oneentity)->getorigin()).length() > ((Circle *)oneentity)->getradius())
                        circle = new Circle(((Circle *)oneentity)->getorigin(),((Circle *)oneentity)->getxaxis(),((Circle *)oneentity)->getyaxis(),
                                            ((Circle *)oneentity)->getradius()+(p-p1).length(),((Circle *)oneentity)->getstarta(),((Circle *)oneentity)->getsweepa());
                      else
                        circle = new Circle(((Circle *)oneentity)->getorigin(),((Circle *)oneentity)->getxaxis(),((Circle *)oneentity)->getyaxis(),
                                            ((Circle *)oneentity)->getradius()-(p-p1).length(),((Circle *)oneentity)->getstarta(),((Circle *)oneentity)->getsweepa());
                      if (circle != 0 && circle->getdefined())
                        db.geometry.add(circle);
                      else
                        delete circle;
                   }
                 else if (oneentity->isa(curve_entity) || oneentity->isa(ellipse_entity))
                   {Point zaxis(0.0,0.0,1.0),startp,dir1,dir2;
                    int reverse;
                    double t,d;
                    EntityList olist;
                      d = 0.0;
                      reverse = 0;
                      dir1 = ((LinearEntity *)oneentity)->direction(0.0);
                      for (t = 0.0 ; t <= 1.0 ; t += 0.1)
                        {  dir2 = ((LinearEntity *)oneentity)->direction(t);
                           if (dir1.dot(dir2) < 0.99)
                             {  zaxis = dir1.cross(dir2).normalize();
                                break;
                             }
                        }
                      ((LinearEntity *)oneentity)->beginoffset(p,&zaxis,&d,0,&reverse,&startp);
                      ((LinearEntity *)oneentity)->offset(zaxis,d,0,0,&reverse,&startp,&olist);
                      olist.start();
                      if (olist.length() > 0)
                        db.geometry.add(olist.next());
                   }
                 oneentity->unhighlight();
              }
            *cstate = 1;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         {  loops->destroy();
            *cstate = ExitState;
         }
       break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+25);
    }
  else if (*cstate == ExitState)
    {  if (slist != NULL) state.setselection(*slist);
       delete slist;  delete loops;
    }
}

struct RBTrim8_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QList<QGraphicsItem*> qgilist1,qgilist2;
#endif
  int state;
   EntityList list;
   Point p1;
   Entity *e;
};

#ifdef NEW_RUBBERB
void RBTrim8(int dmode,void *data,Point *p2,View3dSurface *)
{
   RBTrim8_data *rbdata = (RBTrim8_data *) data;
   LinearEntity *e;

   if(dmode == RB_ERASE)
   {
       cadwindow->clearAllRubberBands();
       return;
   }
   for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
   {
        PointE point1(e->nearp(*p2));
        point1.draw(DM_INVERT);
        if (rbdata->state == 3)
        {
             if (rbdata->e == 0)
             {
                 PointE point2(e->nearp(rbdata->p1));
                 point2.draw(DM_INVERT);
             }
             else
             {
                  Intersect i(e,rbdata->e,rbdata->p1);
                  if (i.nintersections() == 1)
                  {
                       PointE point2(((PointE *)i.intersection(0))->getp());
                       point2.draw(DM_INVERT);
                  }
             }
        }
   }
}
#else
void RBTrim8(int,void *data,Point *p2,View3dSurface *)
{
   RBTrim8_data *rbdata = (RBTrim8_data *) data;
   LinearEntity *e;
#ifdef USING_WIDGETS
   int j=0;
   for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
   {
        if(rbdata->qgilist1.size() > j)
        {
            PointE point1(e->nearp(*p2));
            point1.qgi = rbdata->qgilist1.at(j); point1.zValue = rbdata->zValue;
            point1.draw(DM_INVERT);
            rbdata->qgilist1[j] = point1.qgi; rbdata->zValue = point1.zValue;
        }
        else
        {
            PointE point1(e->nearp(*p2));
            point1.qgi = 0; point1.zValue = rbdata->zValue;
            point1.draw(DM_INVERT);
            rbdata->qgilist1.append(point1.qgi); rbdata->zValue = point1.zValue;
        }
        if(rbdata->qgilist2.size() > j)
        {
            if (rbdata->state == 3)
            {
                 if (rbdata->e == 0)
                 {
                     PointE point2(e->nearp(rbdata->p1));
                     point2.qgi = rbdata->qgilist2.at(j); point2.zValue = rbdata->zValue;
                     point2.draw(DM_INVERT);
                     rbdata->qgilist2[j] = point2.qgi; rbdata->zValue = point2.zValue;
                 }
                 else
                 {
                      Intersect i(e,rbdata->e,rbdata->p1);
                      if (i.nintersections() == 1)
                      {
                           PointE point2(((PointE *)i.intersection(0))->getp());
                           point2.qgi = rbdata->qgilist2.at(j); point2.zValue = rbdata->zValue;
                           point2.draw(DM_INVERT);
                           rbdata->qgilist2[j] = point2.qgi; rbdata->zValue = point2.zValue;
                      }
                 }
            }
        }
        else
        {
            if (rbdata->state == 3)
            {
                 if (rbdata->e == 0)
                 {
                     PointE point2(e->nearp(rbdata->p1));
                     point2.qgi = 0; point2.zValue = rbdata->zValue;
                     point2.draw(DM_INVERT);
                     rbdata->qgilist2.append(point2.qgi); rbdata->zValue = point2.zValue;
                 }
                 else
                 {
                      Intersect i(e,rbdata->e,rbdata->p1);
                      if (i.nintersections() == 1)
                      {
                           PointE point2(((PointE *)i.intersection(0))->getp());
                           point2.qgi = 0; point2.zValue = rbdata->zValue;
                           point2.draw(DM_INVERT);
                           rbdata->qgilist2.append(point2.qgi); rbdata->zValue = point2.zValue;
                      }
                 }
            }
        }
        j++;
   }
#else
   for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
   {
        PointE point1(e->nearp(*p2));
        point1.draw(DM_INVERT);
        if (rbdata->state == 3)
        {
             if (rbdata->e == 0)
             {
                 PointE point2(e->nearp(rbdata->p1));
                 point2.draw(DM_INVERT);
             }
             else
             {
                  Intersect i(e,rbdata->e,rbdata->p1);
                  if (i.nintersections() == 1)
                  {
                       PointE point2(((PointE *)i.intersection(0))->getp());
                       point2.draw(DM_INVERT);
                  }
             }
        }
   }
#endif
}
#endif

/**
 * @brief trim_command8
 * @param cstate
 * @param event
 * @param data
 *
 * Break and delete entity
 *
 * KMJ : working in Qt version
 */
void trim_command8(int *cstate,HighLevelEvent *event,void **data)
{
 RBTrim8_data *rbdata = (RBTrim8_data *) *data;
 Point p1,p2;
 ResourceString rs26(NCTRIM+26);
 EntityList list;
 LinearEntity *e;
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBTrim8_data) == NULL)
         {
             state.fail(Memory,rs26.gets());
              *cstate = ExitState;
         }
         else
           {  list = state.getselection().copy();
              state.destroyselection(0);
              for (list.start() ; (e = (LinearEntity *)list.next()) != 0 ; )
                if (e->islinear())
                  {  rbdata->list.add(e);
                     state.excludelist.add(e);
                  }
                else
                  e->unhighlight();
              list.destroy();
              if (rbdata->list.length() > 0)
                *cstate = 2;
              else
                *cstate = 1;
#ifdef USING_WIDGETS
              rbdata->qgilist1.clear();
              rbdata->qgilist2.clear();
              rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(0);
              rbdata->list.add(((EntitySelectedEvent *) event)->getentity());
              *cstate = 2;
              state.excludelist.add(((EntitySelectedEvent *) event)->getentity());
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                }
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = ExitState;
           }
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Coordinate3d))
           {  rbdata->e = 0;
              rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort))
           {  for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                }
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                }
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = ExitState;
           }
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              db.saveundo(UD_STARTBLOCK,NULL);
              for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                   Intersect i1(e,((EntitySelectedEvent *) event)->getentity(),
                                 ((EntitySelectedEvent *) event)->getp());
                   if (i1.nintersections() == 1)
                     {  p2 = ((PointE *)i1.intersection(0))->getp();
                        if (rbdata->e == 0)
                          e->breakanddelete(0,e->nearp(rbdata->p1),p2);
                        else
                          {Intersect i(e,rbdata->e,rbdata->p1);
                             if (i.nintersections() == 1)
                               e->breakanddelete(0,((PointE *)i.intersection(0))->getp(),p2);
                          }
                     }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Coordinate3d))
           {  db.saveundo(UD_STARTBLOCK,NULL);
              for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                   p2 = e->nearp(((Coordinate3dEvent *) event)->getp());
                   if (rbdata->e == 0)
                     e->breakanddelete(0,e->nearp(rbdata->p1),p2);
                   else
                     {Intersect i(e,rbdata->e,rbdata->p1);
                        if (i.nintersections() == 1)
                          e->breakanddelete(0,((PointE *)i.intersection(0))->getp(),p2);
                     }
                }
              db.saveundo(UD_ENDBLOCK,NULL);
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {  for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                }
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {  for (rbdata->list.start() ; (e = (LinearEntity *)rbdata->list.next()) != 0 ; )
                {  e->unhighlight();
                   state.excludelist.del(e);
                }
              rbdata->list.destroy();
              state.excludelist.destroy();
              *cstate = ExitState;
           }
         break;
    }

#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
   for(int i=0; i<rbdata->qgilist1.size(); i++)
   {
       if(rbdata->qgilist1.at(i))
       {
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgilist1.at(i));
           delete rbdata->qgilist1[i]; rbdata->qgilist1[i]=0;
       }
   }
   rbdata->qgilist1.clear();
   for(int i=0; i<rbdata->qgilist2.size(); i++)
   {
       if(rbdata->qgilist2.at(i))
       {
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgilist2.at(i));
           delete rbdata->qgilist2[i]; rbdata->qgilist2[i]=0;
       }
   }
   rbdata->qgilist1.clear();
#endif
#endif

  if (*cstate == 1)
    {  state.excludelist.destroy();
       state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+27);
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBTrim8,rbdata);
       state.selmask.entity.clearandset(linear_mask,xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+28);
    }
  else if (*cstate == 3)
    {  rbdata->state = 3;
       rubberband.begin(0);
       rubberband.add(RBTrim8,rbdata);
       state.selmask.entity.clearandset(linear_mask,xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRIM+29);
    }
  else if (*cstate == ExitState)
    {
      delete rbdata;
       state.excludelist.destroy();
    }
}

void trim_command9(int *cstate,HighLevelEvent *,void **)
{EntityList list;
 int remove,groupfigures;
 Entity *e;
 ResourceString rs37(NCTRIM+37);
 ResourceString rs288(NCTRIM+288);
 Dialog dialog(_RCT("Explode_Dialog"));

  *cstate = ExitState;

  remove = v.getinteger("tr::remove");
  groupfigures = v.getinteger("tr::groupfigures");
  dialog.add(new CheckBoxDialogControl(100,&remove));
  dialog.add(new CheckBoxDialogControl(101,&groupfigures));
  if (dialog.process() == TRUE)
    {  v.setinteger("tr::remove",remove);
       v.setinteger("tr::groupfigures",groupfigures);
       list = state.getselection().copy();
       if (list.length() == 0)
         cadwindow->MessageBox(rs37.gets(),rs288.gets(),MB_ICONINFORMATION);
       else
         {  db.saveundo(UD_STARTBLOCK,NULL);
            if (remove)
              state.destroyselection(1);
            for (list.start() ; (e = list.next()) != NULL ; )
              {  if ((e->isdimension() || e->isa(figure_entity)) && groupfigures)
                   {Entity *firstentity,*e1;
                    RCCHAR *s1,*s2;
                    BitMask options(32);
                      EntityList list;
                      db.geometry.start();
                      firstentity = db.geometry.next();
                      if (e->explode())
                        {  for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
                             if (e1 == firstentity)
                               break;
                             else
                               list.add(e1);
                           if (list.length() > 0)
                             {Group *group;
                                Group::getdefaults(&s1,&s2,&options);
                                if (e->isa(figure_entity))
                                  group = new Group(list,_RCT("Exploded Figure"),((Figure *)e)->getlibrary(),options);
                                else
                                  group = new Group(list,_RCT("Exploded Dimension"),_RCT(""),options);
                                if (group != NULL && group->getdefined())
                                  db.geometry.add(group);
                                else
                                  delete group;
                             }
                           if (e->getalist().empty() && remove)
                           {
                                e->draw(DM_ERASE);
                                db.geometry.del(e,0);
                           }
                        }
                   }
                   else if (e->explode() && e->getalist().empty() && remove)
                   {
                        e->draw(DM_ERASE);
                        db.geometry.del(e,0);
                   }
              } 
            list.destroy();
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
}                 

struct RBTrim10_data  
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    int end;
   Entity *e,*panel;
   Point panelp;
  };

#ifdef NEW_RUBBERB
void RBTrim10(int dmode,void *data,Point *p2,View3dSurface *)
{RBTrim10_data *rbdata = (RBTrim10_data *) data;
 LinearEntity *e;
 Transform identity;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (rbdata->panel != 0)
    return;
  identity.identity();
  rbdata->e->setcopye(NULL);
  e = (LinearEntity *) rbdata->e->clone(identity);
  e->moveendpoint(0,rbdata->end,*p2);
  //e->qgi = rbdata->qgi; e->zValue = rbdata->zValue;
  e->Entity::draw(DM_INVERT);
  //rbdata->qgi = e->qgi; rbdata->zValue = e->zValue;
  delete e;
}
#else
void RBTrim10(int,void *data,Point *p2,View3dSurface *)
{RBTrim10_data *rbdata = (RBTrim10_data *) data;
 LinearEntity *e;
 Transform identity;
  if (rbdata->panel != 0)
    return;
  identity.identity();
  rbdata->e->setcopye(NULL);
  e = (LinearEntity *) rbdata->e->clone(identity);
  e->moveendpoint(0,rbdata->end,*p2);
  e->qgi = rbdata->qgi; e->zValue = rbdata->zValue;
  e->Entity::draw(DM_INVERT);
  rbdata->qgi = e->qgi; rbdata->zValue = e->zValue;
  delete e;
}
#endif

/**
 * @brief trim_command10
 * @param cstate
 * @param event
 * @param data
 *
 * Trim to a corner
 *
 * KMJ : working in Qt version
 */
void trim_command10(int *cstate,HighLevelEvent *event,void **data)
{RBTrim10_data *rbdata = (RBTrim10_data *) *data;
 Point  p,p2;
 Entity *e;
 int end;
 ResourceString rs30(NCTRIM+30);
 ResourceString rs31(NCTRIM+31);
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim10_data;
#ifdef USING_WIDGETS
         rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           *cstate = 1;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {Transform ident;
              state.destroyselection(1);

              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              if (rbdata->e->isa(group_entity))
                {  if (strcmp(((Group *)rbdata->e)->getclassname(),"RC::Panel") == 0)
                     {  rbdata->e = ((EntitySelectedEvent *) event)->getentity();
                        rbdata->panel = ((EntitySelectedEvent *) event)->getentity();
                        rbdata->panelp =  state.getlastpickpoint();
                     }
                   else
                     {  rbdata->e = 0;
                        break;
                     }
                }
              else
                {  rbdata->panel = 0;
                   rbdata->end = ((EntitySelectedEvent *) event)->getendselected();
                   ident.identity();
                }
              rbdata->e->highlight();
              state.excludelist.add(rbdata->e);
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         rbdata->e->unhighlight();
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              end = ((EntitySelectedEvent *) event)->getendselected();

              if (rbdata->panel != 0)
                {//  Trim the panel to a point
                   if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0) 
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        db.saveundo(UD_STARTBLOCK,NULL);
                        TrimPanel((Group *)rbdata->panel,rbdata->panelp,e,p,1);
                        db.saveundo(UD_ENDBLOCK,NULL);
                        db.saveundo(UD_STARTBLOCK,NULL);
                        TrimPanel((Group *)e,p,(Group *)rbdata->panel,rbdata->panelp,2);
                        db.saveundo(UD_ENDBLOCK,NULL);
                        db.saveundo(UD_ENDBLOCK,NULL); 
                     }
                }
              else
                {  Intersect i(rbdata->e,e,p);
                   if (i.nintersections() == 1)
                     {  p2 = ((PointE *)i.intersection(0))->getp();
                        db.saveundo(UD_STARTBLOCK,NULL);
                        ((LinearEntity *)e)->moveendpoint(1,end,p2);
                        ((LinearEntity *)rbdata->e)->moveendpoint(1,rbdata->end,p2);
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                }
     
              state.excludelist.destroy();
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {  state.excludelist.destroy();
              *cstate = 1;
           }
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
    {  state.selmask.entity.clearandset(linear_mask,group_entity,end_list);
       strcpy(state.groupnamemask,"RC::Panel");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(linear_mask,group_entity,end_list);
       strcpy(state.groupnamemask,"RC::Panel");
    }

  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRIM+38);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBTrim10,rbdata);
       cadwindow->prompt->normalprompt(NCTRIM+39);
    }
  else if (*cstate == ExitState)
    {  delete rbdata;
       strcpy(state.groupnamemask,"");
    }
}

struct RBTrim11_data
  {int state,type,insertonintersection,createsectionview;
   Point p1,p2;
   RCCHAR viewname[300];
   EntityList list;
  };

#ifdef NEW_RUBBERB
void RBTrim11(int dmode,void *data,Point *p2,View3dSurface *)
{RBTrim11_data *rbdata = (RBTrim11_data *) data;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }
    if (rbdata->state == 2 || rbdata->state == 12 || rbdata->state == 13)
    {
        Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
    }
    if (rbdata->state == 13)
    {
        Line line(rbdata->p1,rbdata->p2);  line.draw(DM_INVERT);
    }
}
#else
void RBTrim11(int ,void *data,Point *p2,View3dSurface *)
{RBTrim11_data *rbdata = (RBTrim11_data *) data;
  if (rbdata->state == 2 || rbdata->state == 12 || rbdata->state == 13)
    {  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
    }
  if (rbdata->state == 13)
    {  Line line(rbdata->p1,rbdata->p2);  line.draw(DM_INVERT);
    }
}
#endif

class SECheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     SECheckBoxDialogControl(int id,int *v) : CheckBoxDialogControl(id,v) {} ;
     int select(Dialog *dialog);
     void load(Dialog *dialog);
  };

void SECheckBoxDialogControl::load(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),204),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),oldvalue);
  CheckBoxDialogControl::load(dialog);
}

int SECheckBoxDialogControl::select(Dialog *dialog)
{ EnableWindow(GetDlgItem(dialog->gethdlg(),204),! oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),! oldvalue);
  return CheckBoxDialogControl::select(dialog);
}

void InsertSection(RCCHAR *viewname,int insertonintersection,int createsectionview,Point origin,Point xaxis,Point yaxis,EntityList list)
{int j,viewno;
 Point eye,ref,up;
 View *view;
 ResourceString rs45(NCTRIM+45),rs46(NCTRIM+46);
 Transform t1,t2;
 BitMask visible(MaxViews),options(32);
 Entity *e;

  if (createsectionview && strlen(viewname) == 0)
    cadwindow->MessageBox(rs45.gets(),rs46.gets(),MB_ICONINFORMATION);
  else
    {  if ((view = db.views3d.match(viewname)) != 0)
         {  viewno = view->getviewno();
            visible.set(viewno);
         }
       else if (! createsectionview || (viewno = db.getnextviewno()) >= 0)
         {  if (createsectionview)
              {  eye.setxyz(   0.0,  0.0,  1.0);
                 ref.setxyz(   0.0,  0.0,  0.0);
                 up.setxyz (   0.0,  1.0,  0.0);
                 view = new View(viewname,0,viewno | VW_ONEVIEWONLY,eye,ref,up,-1.0,1.0);
                 db.views3d.add(view);
                 visible.set(viewno);
                 cadwindow->create3dsubwindow(view);
              }
         }
       InfinitePlane iplane(xaxis.cross(yaxis),origin);
       t1.translate(-origin);
       t2.fromcoordinatesystem(xaxis,yaxis,xaxis.cross(yaxis));
       t1.apply(t2);
       for (list.start() ; (e = list.next()) != 0 ; )
         if (e->isa(plane_entity) || e->isa(wall_entity) || e->isa(figure_entity) || e->isuvsurface())
           {Intersect i(e,&iplane);
              for (j = 0 ; j < i.nintersections() ; j++)
                {  if (insertonintersection)
                     db.geometry.add(i.intersection(j));
                   if (createsectionview)
                     {  i.intersection(j)->setcopye(0);
                        e = i.intersection(j)->clone(t1);
                        e->setvisible(visible);
                        db.geometry.add(e);
                     }
                }
           }
    }
}

void trim_command11(int *cstate,HighLevelEvent *event,void **data)
{RBTrim11_data *rbdata = (RBTrim11_data *) *data;
 Point origin,xaxis,yaxis,zaxis,p1,p2,eye,ref,up,p3,p4;
 Entity *e;
 BitMask visible(MaxViews),options(32);
 Dialog dialog("Section_Dialog");
 ResourceString rs45(NCTRIM+45),rs46(NCTRIM+46);

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBTrim11_data;  *data = rbdata;
         rbdata->type = v.getinteger("se::type");
         rbdata->insertonintersection = v.getinteger("se::insertonintersection");
         rbdata->createsectionview = v.getinteger("se::createsectionview");
         strcpy(rbdata->viewname,v.getstring("se::viewname"));
         dialog.add(new RadioButtonDialogControl(100,100,101,&rbdata->type));
         dialog.add(new RadioButtonDialogControl(101,100,101,&rbdata->type));
         dialog.add(new CheckBoxDialogControl(102,&rbdata->insertonintersection));
         dialog.add(new StringDialogControl(104,rbdata->viewname,sizeof(rbdata->viewname)));
         dialog.add(new SECheckBoxDialogControl(103,&rbdata->createsectionview));
         if (dialog.process() == TRUE)
           {  v.setinteger("se::type",rbdata->type);
              v.setinteger("se::insertonintersection",rbdata->insertonintersection);
              v.setinteger("se::createsectionview",rbdata->createsectionview);
              v.setstring("se::viewname",rbdata->viewname);
              rbdata->list = state.getselection().copy();
              if (rbdata->createsectionview && strlen(rbdata->viewname) == 0)
                {  cadwindow->MessageBox(rs45.gets(),rs46.gets(),MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else
                {  if (rbdata->type == 0)
                     *cstate = 1;
                   else
                     *cstate = 11;
                }
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              zaxis = ((EntitySelectedEvent *)event)->getxaxis().cross(((EntitySelectedEvent *)event)->getyaxis());
              if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   InsertSection(rbdata->viewname,rbdata->insertonintersection,rbdata->createsectionview,l->getp1(),(l->getp2()-l->getp1()).normalize(),zaxis,rbdata->list);
                }
              state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              zaxis = ((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis());
              InsertSection(rbdata->viewname,rbdata->insertonintersection,rbdata->createsectionview,rbdata->p1,(p2-rbdata->p1).normalize(),zaxis,rbdata->list);
           }
         state.setselection(rbdata->list);
         *cstate = ExitState;
         break;
       case 11 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 12;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              if (e->isa(circle_entity))
                {Circle *c = (Circle *)e;
                   InsertSection(rbdata->viewname,rbdata->insertonintersection,rbdata->createsectionview,c->getorigin(),c->getxaxis(),c->getyaxis(),rbdata->list);
                }
              else if (e->isa(plane_entity))
                {Plane *p = (Plane *)e;
                   InsertSection(rbdata->viewname,rbdata->insertonintersection,rbdata->createsectionview,p->getorigin(),p->getxaxis(),p->getyaxis(),rbdata->list);
                }
              state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 12 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 13;
           }
         else if (event->isa(Exit) || event->isa(Abort))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
       case 13 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              xaxis = rbdata->p2 - rbdata->p1;
              yaxis = p2 - rbdata->p1;
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  xaxis.normalize();  yaxis.normalize();
                   InsertSection(rbdata->viewname,rbdata->insertonintersection,rbdata->createsectionview,rbdata->p1,xaxis,yaxis,rbdata->list);
                }
           }
         state.setselection(rbdata->list);
         *cstate = ExitState;
         break;
     }
  rbdata->state = *cstate;
  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBTrim11,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 11)
    state.selmask.entity.clearandset(xyz_mask,circle_entity,plane_entity,end_list);
  else if (*cstate == 12 || *cstate == 13)
    {  rubberband.begin(0);
       rubberband.add(RBTrim11,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRIM+40);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCTRIM+41);
  else if (*cstate == 11)
    cadwindow->prompt->normalprompt(NCTRIM+42);
  else if (*cstate == 12)
    cadwindow->prompt->normalprompt(NCTRIM+43);
  else if (*cstate == 13)
    cadwindow->prompt->normalprompt(NCTRIM+44);
  else
    delete rbdata;
}


