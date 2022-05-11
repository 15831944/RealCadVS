
#include "ncwin.h"

bool copyAlongMapContains(EntityList list) {
	Entity* e = 0;
	for (list.start(); (e = list.next()) != nullptr; )
		if (db.copyAlogMap.contains(e->getid())) return true;
	return false;
}

BitMask * GetLockedLayerTable(void)
{BitMask *lockedlayertable;
 View *view3d; 
  lockedlayertable = 0;
  if ((view3d = db.views3d.match(state.getlastviewname())) != 0)
    lockedlayertable = view3d->getlockedlayertable();  
  if (lockedlayertable == 0)
    lockedlayertable = &db.lockedmask.layer;

  return lockedlayertable;

}


void RemoveLockedEntities(EntityList *list,int unhighlight)
{BitMask *lockedlayertable;
 Entity *e;

  lockedlayertable = GetLockedLayerTable();

  for (list->start() ; (e = list->next()) != 0 ; )
    if (lockedlayertable->test(e->getlayer()))
      {  list->del(e);
         list->start();
         if (unhighlight)
           e->unhighlight();
      }

}

/**
 * @brief RemovePartialGroupEntities
 * @param list - should be currently selected
 * @param unhighlight - for triggering entity unhighlight
 *
 * Remove entities from list who's parent group entity is not selected.
 * Make resulting list the current selection.
 */
void RemovePartialGroupEntities(EntityList *list,int unhighlight) {
    Entity *e, *ge;
    EntityList alist;
    int parent;
    for (list->start() ; (e = list->next()) != NULL ; ) {
      alist = e->getalist();
          parent = 0;
          for (alist.start() ; (ge = alist.next()) != NULL ; ) {
              if (ge->isa(group_entity) && ! state.excludelist.inlist(ge) && ((Group *)ge)->getlist().inlist(e)) {
                  parent = 1;
              }
              if (!ge->getselected()) {
                  list->del(e);
                  list->start();
                  if (unhighlight) {
                    e->unhighlight();
                  }
              }
          }
      }
    state.destroyselection(1);
    for (list->start() ; (e = list->next()) != NULL ; ) {
        state.addtoselection(e);
    }
}

class TRRadioButtonDialogControl : public RadioButtonDialogControl
{public:
   TRRadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void TRRadioButtonDialogControl::load(Dialog *dialog)
{int i;
  RadioButtonDialogControl::load(dialog);
  for (i = id2+1 ; i <= 1100+id2+1 ; i += 100)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+1),! oldvalue);
       if (program->getlicensed() < 2)
           EnableWindow(GetDlgItem(dialog->gethdlg(),i+2),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+5),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+6),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+7),oldvalue);
       if (i == id2+1)
         i = 1000+id2+1;
    }

  if (id2 == 104)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),108),oldvalue < 2);
       //EnableWindow(GetDlgItem(dialog->gethdlg(),1108),oldvalue < 2);
    }

}

int TRRadioButtonDialogControl::select(Dialog *dialog)
{int i;
  RadioButtonDialogControl::select(dialog);
  for (i = id2+1 ; i <= 1100+id2+1 ; i += 100)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+1),! oldvalue);
       if (program->getlicensed() < 2)
           EnableWindow(GetDlgItem(dialog->gethdlg(),i+2),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+5),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+6),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),i+7),oldvalue);
       if (i == id2+1)
         i = 1000+id2+1;
    }

  if (id2 == 104)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),108),oldvalue < 2);
       //EnableWindow(GetDlgItem(dialog->gethdlg(),1108),oldvalue < 2);
    }

  return 0;
}

struct RBStretch_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgiline,*qgient;
    QList<QGraphicsItem*>qglist;
    EntityList clist;
#endif
   Point p,p1,p2;
   EntityList list;
   int type,stretchmode;
   View3dWindow *window;
};

#ifdef NEW_RUBBERB
void RBStretch(int dmode ,void *data,Point *p2,View3dSurface *)
{
 RBStretch_data *rbdata = (RBStretch_data *) data;
 Transform t;
 Entity *e;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  cadwindow->startdrawingbatch();
  if (db.getdragdetail() == 0)
  {
       t.translate(*p2 - rbdata->p);
       for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
       {
           e->draw(DM_INVERT,&t);
       }
  }
  else
  {
       Line line(*p2,rbdata->p);
       //line.qgi = rbdata->qgiline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       //rbdata->qgiline = line.qgi; rbdata->zValue = line.zValue;
  }
  cadwindow->enddrawingbatch();
}
#else
void RBStretch(int,void *data,Point *p2,View3dSurface *)
{RBStretch_data *rbdata = (RBStretch_data *) data;
 Transform t;
 Entity *e;
  cadwindow->startdrawingbatch();
  if (db.getdragdetail() == 0)
    {  t.translate(*p2 - rbdata->p);
       for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
       {
           e->draw(DM_INVERT,&t);
       }
    }
  else
    {
       Line line(*p2,rbdata->p);
       line.qgi = rbdata->qgiline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgiline = line.qgi; rbdata->zValue = line.zValue;
    }
  cadwindow->enddrawingbatch();
}
#endif

void stretchlist(EntityList *list,Transform t)
{Entity *e;
 EntityList movedlist;
 EntityList newlist;
 int i;
 SortedPointTable sptable;

  cadwindow->startdrawingbatch();


  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);

  for (list->start() ; (e = list->next()) != NULL ; )
    {  e->setstatus(1 | 16);
       e->dbsetassociated();
       e->draw(DM_ERASE);
    }

  //  Add the stretch points to the list - don't stretch the origin point of a circle.
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (e->getstatus())
      for (i = 0 ; i < e->npoints() - e->isa(circle_entity) ; i++)
        sptable.add(e->point(i));
  sptable.endlist();

  db.saveundo(UD_STARTBLOCK,NULL);
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    e->stretch(&db,t,&sptable,1);
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if ((e->getstatus() & 32) != 0)   //  Original entity has been processed during stretch
      movedlist.add(e);
  db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
  db.saveundo(UD_ENDBLOCK,NULL);

  for (list->start() ; (e = list->next()) != NULL ; )
    if (e->getcopye() == NULL)
      newlist.add(e);
    else
      newlist.add(e->getcopye());
  list->destroy();
  *list = newlist;
  db.saveundo(UD_STARTBLOCK,NULL);
  for (i = 0 ; i < 2 ; i++)
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if ((e->getstatus() & 4) == 0)   //  An original entity
      {  if (e != e->getcopye() && e->getcopye() != NULL && e->getalist().empty())
           {AttributeList elist;
              e->getcopye()->substituteat(e->getatlist().copy());
              e->substituteat(elist);
              db.geometry.del(e);
           }
      }
  db.saveundo(UD_ENDBLOCK,NULL);

  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if ((e->getstatus() & 32) != 0)   //  Original entity has been processed during stretch
      e->draw(DM_NORMAL);
    else if (e->getstatus() & ~8)
      e->draw(DM_NORMAL);

  db.saveundo(UD_ENDBLOCK,NULL);

  cadwindow->enddrawingbatch();

}

void stretchpointlist(View3dWindow *window,Point p1,Point p2,Transform t)
{Entity *e;
 EntityList movedlist;
 EntityList newlist;
 int i,x1,y1,x2,y2,x;
 SortedPointTable sptable;

  cadwindow->startdrawingbatch();

  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);

  p1 = window->uvtoscreen(p1);    x1 = int(p1.x);  y1 = int(window->getheight() - p1.y);
  p2 = window->uvtoscreen(p2);    x2 = int(p2.x);  y2 = int(window->getheight() - p2.y);
  if (x1 > x2)
    {  x = x1;  x1 = x2;   x2 = x;  
    }
  if (y1 > y2)
    {  x = y1;  y1 = y2;   y2 = x;  
    }
  //  Add the stretch points to the list - don't stretch the origin point of a circle.
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    for (i = 0 ; i < e->npoints() - e->isa(circle_entity) ; i++)
      {  if (window->pick(e->point(i),1,x1,y1,x2,y2))
           sptable.add(e->point(i));
      }  

  sptable.endlist();
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    e->stretch(&db,t,&sptable,1);
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if ((e->getstatus() & 4) == 0)   //  An original entity
      {  if (e != e->getcopye() && e->getcopye() != NULL && e->getalist().empty())
           {AttributeList elist;
              e->getcopye()->substituteat(e->getatlist().copy());
              e->substituteat(elist);
              db.geometry.del(e);
           }
      }
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if ((e->getstatus() & 4) == 0 && (e->getstatus() & ~8))   //  Original entity has been processed during stretch
      {  movedlist.add(e);
         e->draw(DM_NORMAL);
      }
    else if (e->getstatus() & ~8)
      e->draw(DM_NORMAL);
  db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
  movedlist.destroy();
  db.saveundo(UD_ENDBLOCK,NULL);

  cadwindow->enddrawingbatch();

}

void stretch_command(int *cstate,HighLevelEvent *event,void **data)
{RBStretch_data *rbdata = (RBStretch_data *) *data;
 Transform t;
 EntityList list;
 DialogControl *dc;
 Point zero(0.0,0.0,0.0),p2;
 Dialog *dialog;
 

 // debug
 Entity *e=0;
 for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
     qDebug() << "entity (" << e << ") qgi : " << e->qgi;


  switch (*cstate)
    {  case InitialState :
         rbdata = new RBStretch_data;
#ifdef USING_WIDGETS
         rbdata->qgiline=0; rbdata->qgient=0;
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         dialog = new Dialog("Stretch_Dialog");
         rbdata->p.setxyz(v.getreal("tr::stretchx"),v.getreal("tr::stretchy"),v.getreal("tr::stretchz"));
         rbdata->type = v.getinteger("tr::stretchtype");
         rbdata->stretchmode = v.getinteger("tr::stretchmode");
         dialog->add(new TRRadioButtonDialogControl(100,100,101,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(101,100,101,&rbdata->type));
         dialog->add(dc = new RealDialogControl(102,&rbdata->p.x,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1102,dc));
         dialog->add(dc = new RealDialogControl(103,&rbdata->p.y,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1103,dc));
         dialog->add(dc = new RealDialogControl(104,&rbdata->p.z,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1104,dc));
         dialog->add(new RadioButtonDialogControl(105,105,106,&rbdata->stretchmode));
         dialog->add(new RadioButtonDialogControl(106,105,106,&rbdata->stretchmode));

         if (program->getlicensed() >= 2)
           {
              dialog->adddisable(104);
              //dialog->adddisable(1104);
              rbdata->p.z = 0.0;
           }

         if (dialog->process() == TRUE)
           {  v.setreal("tr::stretchx",rbdata->p.x);
              v.setreal("tr::stretchy",rbdata->p.y);
              v.setreal("tr::stretchz",rbdata->p.z);
              v.setinteger("tr::stretchtype",rbdata->type);
              v.setinteger("tr::stretchmode",rbdata->stretchmode);
              if (db.workplanes.getcurrent() != NULL)
                rbdata->p = db.workplanes.getcurrent()->workplanetomodel(rbdata->p) -
                            db.workplanes.getcurrent()->workplanetomodel(zero);
              rbdata->list = state.getselection().copy();

              RemoveLockedEntities(&rbdata->list,0);


              state.destroyselection(1);
              if (rbdata->stretchmode == 1)
                {//  Get the window to stretch
                   *cstate = 100;
                }
              else 
                {  if (rbdata->list.length() == 0)
                     {ResourceString rs(NCMAINB+89);
                        cadwindow->MessageBox(rs.gets(),"",MB_OK);
                        *cstate = ExitState;
                     }
                   else if (rbdata->type == 0)
                     {  stretchlist(&rbdata->list,t.translate(rbdata->p));
                        state.setselection(rbdata->list);
                        *cstate = ExitState;
                     }
                   else
                     *cstate = 1;
                }
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *)event)->getp();
              cadwindow->rubberband3d(rbdata->p);
              cadwindow->cursoraction(1);
              cadwindow->cursorstyle(1);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Line *l = (Line *)((EntitySelectedEvent *)event)->getentity();
              state.destroyselection(1);
              *cstate = 1;
              if (((EntitySelectedEvent *)event)->getendselected() == 1)
                {  if (rbdata->stretchmode == 0)
                     stretchlist(&rbdata->list,t.translate(l->getp2()-l->getp1()));
                   else
                     {  stretchpointlist(rbdata->window,rbdata->p1,rbdata->p2,t.translate(l->getp2()-l->getp1()));
                        *cstate = 100;
                     }
                   rbdata->p = l->getp2();
                }
              else
                {  if (rbdata->stretchmode == 0)
                     stretchlist(&rbdata->list,t.translate(l->getp1()-l->getp2()));
                   else
                     {  stretchpointlist(rbdata->window,rbdata->p1,rbdata->p2,t.translate(l->getp2()-l->getp1()));
                        *cstate = 100;
                     }
                   rbdata->p = l->getp1();
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         cadwindow->cursoraction(0);
         cadwindow->cursorstyle(1);
         if (event->isa(Coordinate3d))
           {  if (rbdata->stretchmode == 0)
                stretchlist(&rbdata->list,t.translate((((Coordinate3dEvent *)event)->getp()-rbdata->p)));
              else
                {  stretchpointlist(rbdata->window,rbdata->p1,rbdata->p2,t.translate((((Coordinate3dEvent *)event)->getp()-rbdata->p)));
                   *cstate = 100;
                }
              rbdata->p = ((Coordinate3dEvent *)event)->getp();
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {
              state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
       case 100 :
         if (event->isa(Coordinateuv))
           {  if ((rbdata->window = ((CoordinateuvEvent *) event)->window()) != 0)
                {  rbdata->p1 = ((CoordinateuvEvent *) event)->coordinate();
                   rbdata->window->rubberbanduv(rbdata->p1);
                   rbdata->window->cursoraction(2);
                   rbdata->window->cursorstyle(0);
                   *cstate = 101;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 101 :
         cadwindow->cursoraction(0);
         cadwindow->cursorstyle(1);
         if (event->isa(Coordinateuv))
           {  if ((rbdata->window = ((CoordinateuvEvent *) event)->window()) != 0)
                {  rbdata->p2 = ((CoordinateuvEvent *) event)->coordinate();
                   rbdata->list.destroy();

                   //  Make up the new list to stretch.
                   if (rbdata->type == 0)
                     {  stretchpointlist(rbdata->window,rbdata->p1,rbdata->p2,t.translate(rbdata->p));
                        *cstate = ExitState;
                     }
                   else
                    *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
     }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgiline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgiline);
  delete rbdata->qgiline; rbdata->qgiline=0;
#endif
#endif
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 100 || *cstate == 101)
    state.selmask.entity.clearandset(uv_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRANS+17);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBStretch,rbdata);
       cadwindow->prompt->normalprompt(NCTRANS+18);
    }
  else if (*cstate == 100)
    {  cadwindow->prompt->normalprompt("Locate the first corner of the stretch window");
    }
  else if (*cstate == 101)
    {  cadwindow->prompt->normalprompt("Locate the second corner of the stretch window");
    }
  else
    delete rbdata;
}

struct RBTranslate_data
  {Point rotation,p,p1,p2,zaxis;
   EntityList list,dlist;
   int type,state,ncopies,surface;
  };

void RBTranslate(int dmode,void *data,Point *p2,View3dSurface *)
{RBTranslate_data *rbdata = (RBTranslate_data *) data;
 Transform t,tr;
 Entity *e;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  cadwindow->startdrawingbatch();

  if (db.getdragdetail() == 0 && rbdata->state < 100)
    {  t.translate(-rbdata->p);
       tr.rotatex(rbdata->rotation.x * acos(-1.0) / 180.0);
       t.apply(tr);
       tr.rotatey(rbdata->rotation.y * acos(-1.0) / 180.0);
       t.apply(tr);
       tr.rotatez(rbdata->rotation.z * acos(-1.0) / 180.0);
       t.apply(tr);
       tr.translate(*p2);
       t.apply(tr);
       for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
         e->draw(DM_INVERT,&t);
    }
  else if (rbdata->state < 100)
    {  Line line(*p2,rbdata->p);
       line.draw(DM_INVERT);
    }

  if (rbdata->state == 102 || rbdata->state == 112 || rbdata->state == 113)
    {  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
    }
  if (rbdata->state == 121)
    {  Line line(rbdata->p,*p2);  line.draw(DM_INVERT);
    }
  if (rbdata->state == 113)
    {  Line line1(rbdata->p1,rbdata->p2);  line1.draw(DM_INVERT);
       Line line2(rbdata->p2,*p2);  line2.draw(DM_INVERT);
    }

  cadwindow->enddrawingbatch();

}
void transformCopyAlogList(int entyId,int level,EntityList list, Transform t) {
	cadwindow->startdrawingbatch();	
	db.geometry.clearstatus();
	db.saveundo(UD_STARTBLOCK, NULL);
	Entity *e=0;
	for (list.start(); (e = list.next()) != NULL; )
		if (e->getcopye() == NULL) {
			Entity * copyEntity = e->clone(t);
			db.geometry.add(copyEntity);
			copyEntity->setRefId(copyEntity->getid());			
			db.copyAlogMap[entyId].undoMap[level].copyList.append(copyEntity->getid());
		}
	db.saveundo(UD_ENDBLOCK, NULL);
	cadwindow->enddrawingbatch();
}
void transformlist(int project,int ncopies,EntityList list,Transform t,int surface,int save=1)
{Entity *e;
 EntityList movedlist;

  cadwindow->startdrawingbatch();

  db.geometry.clearstatus();
  if(save)
	db.saveundo(UD_STARTBLOCK,NULL);
  
  if (! project && ncopies == 0)
    {  for (list.start() ; (e = list.next()) != NULL ; )
         {  e->dbsetassociated();
            e->setstatus(1);
         }
       for (list.start() ; (e = list.next()) != NULL ; )
         if (e->getalist().empty()) e->draw(DM_ERASE);
	   
       for (list.start() ; (e = list.next()) != NULL ; )
         if (e->getalist().empty()) e->move(&db,t,0,save);
       for (list.start() ; (e = list.next()) != NULL ; )
         if (! e->getalist().empty()) e->move(&db,t,1,save);
       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (! e->isa(group_entity) && e->getstatus())
           {  movedlist.add(e);  e->draw(DM_NORMAL);
           }
       //  Note the following code is not proven correct for all cases of nested groups
       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->isa(group_entity) && e->getstatus())
           {  movedlist.add(e);  e->draw(DM_NORMAL);
           }
	   if(save)
		 db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
       movedlist.destroy();
    }
  else
    {  for (list.start() ; (e = list.next()) != NULL ; )
         if (project)
           {  if (e->getcopye() == NULL)
                e->project(&db,t,surface);
           }
         else
		 {
			 Entity* copyEntity;
			 if (e->getcopye() == NULL) {
				 copyEntity = e->clone(t);
				 db.geometry.add(copyEntity);
				 // for any operation which has a copy action
				 if (db.CopyAlogEntityCallback != 0 && db.copyAlogMap.contains(e->getid())) {
					 copyAlongDialogData  dlgData = db.copyAlogMap[e->getid()];	
						 db.copyAlogMap.insert(copyEntity->getid(), dlgData.clone(t, e->isa(circle_entity)));
						 copyEntity->setRefId(copyEntity->getid());					
				 }
			 }
         }
    }
  if(save)
	db.saveundo(UD_ENDBLOCK,NULL);
  cadwindow->enddrawingbatch();

}


Curve *CircleToCurve(Circle *circle)
{int i,nsegments;
 Point *polygon;
 double *weight,*knot;
 BitMask options(32);

  nsegments = 4;
  polygon = new Point[nsegments*2+1];
  weight = new double[nsegments*2+1];
  knot = new double[nsegments*2+1+2+1];
  if (polygon != 0 && weight != 0 && knot != 0)
    {  for (i = 0 ; i < nsegments ; i++)
         {  polygon[i*2] = circle->position(double(i)/double(nsegments));
            polygon[i*2+2] = circle->position(double(i+1)/double(nsegments));
            Line line1(polygon[i*2],polygon[i*2]+circle->direction(double(i)/double(nsegments)));
            Line line2(polygon[i*2+2],polygon[i*2+2]+circle->direction(double(i+1)/double(nsegments)));
            Intersect i1(&line1,&line2);
            if (i1.nintersections() == 1)
              polygon[i*2+1] = ((PointE *)i1.intersection(0))->getp();
            else
              polygon[i*2+1] = (polygon[i*2] + polygon[i*2+2])/2.0;
            weight[i*2] = 1.0;
            weight[i*2+1] = cos(circle->getsweepa()/nsegments/2.0);
            weight[i*2+2] = 1.0;
            knot[i*2+1] = double(i) / double(nsegments);
            knot[i*2+2] = double(i) / double(nsegments);
         }
       knot[0] = 0.0;
       knot[nsegments*2+1] = 1.0;
       knot[nsegments*2+1+1] = 1.0;
       knot[nsegments*2+1+2] = 1.0;
       Curve *curve = new Curve(nsegments*2+1,2,polygon,knot,weight,options);
       if (curve != 0 && curve->getdefined())
         return curve;
       else  
         {  delete curve;
            return 0;
         }
    }

  return 0;
}

void projectlist(EntityList list,Point direction,Point p,Point normal,int surface,EntityList *newlist = 0)
{Entity *e,*e1,*e2,*e3;
 Curve *curve;
 EntityList movedlist;
 InfinitePlane ip(normal,p);
 BitMask options(32);
 EntityList plist,elist;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point org,xa,ya,za,*polygon;
 double D;
 RCCHAR patternfilename[300];
 int i,xmesh,ymesh;
 RCCOLORREF gradientcolour;

  cadwindow->startdrawingbatch();

  db.geometry.clearstatus();
  db.saveundo(UD_STARTBLOCK,NULL);

  if (newlist != 0)
    *newlist = elist;

  for (list.start() ; (e = list.next()) != NULL ; )
    if (e->isa(line_entity))
      {
        Line  ltmp = Line(((Line *)e)->getp1(),((Line *)e)->getp1()+direction);
        Intersect i1(&ip,&ltmp);
        ltmp = Line(((Line *)e)->getp2(),((Line *)e)->getp2()+direction);
        Intersect i2(&ip,&ltmp);

         if (i1.nintersections() == 1 && i2.nintersections() == 1)
           {  db.geometry.add(e1 = new Line(*e,((Line *)e)->getp1(),((PointE *)i1.intersection(0))->getp()));
              db.geometry.add(e2 = new Line(*e,((Line *)e)->getp2(),((PointE *)i2.intersection(0))->getp()));
              db.geometry.add(e3 = new Line(*e,((PointE *)i1.intersection(0))->getp(),((PointE *)i2.intersection(0))->getp()));
              if (newlist)
                newlist->add(e3);

              if (surface)
                {  plist.add(e);  plist.add(e1);  plist.add(e3);  plist.add(e2);
                   Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                   Loops loops(1,plist);
                   if (loops.planar(&org,&xa,&ya,&za,&D))
                     {  Plane *surface = new Plane(*e,org,xa,ya,plist,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                        if (surface != NULL && surface->getdefined())
                          db.geometry.add(surface);
                        else
                          delete surface;
                     }
                   plist = elist;
                }
           }

      }
    else if (e->isa(circle_entity) || e->isa(curve_entity))
      {  if (e->isa(circle_entity))
           curve = CircleToCurve((Circle *)e);
         else
           curve = (Curve *)e;

         polygon = new Point[curve->getn()];

         for (i = 0 ; i < curve->getn() ; i++)
           {
             Line ltmp = Line(*e,curve->getpolygon()[i],curve->getpolygon()[i]+direction);
             Intersect i1(&ip,&ltmp);
              if (i1.nintersections() == 0)
                break;
              polygon[i] = ((PointE *)i1.intersection(0))->getp();

           }

         if (i == curve->getn())
           {  db.geometry.add(e3 = new Curve(*e,curve->getn(),curve->getdegree(),polygon,curve->getknot(),curve->getcweight(),curve->getoptions()));
              if (newlist)
                newlist->add(e3);
              if (surface)
                {  if (surface)
                     {  RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                        RuledSurface *surface = new RuledSurface(*e,(LinearEntity *)e,(LinearEntity *)e3,xmesh,ymesh,options);
                        db.geometry.add(surface);
                     }
                }
           } 
  
         if (e->isa(circle_entity))
           delete curve;
      }

  db.saveundo(UD_ENDBLOCK,NULL);

  cadwindow->enddrawingbatch();

}

/**
 * @brief moveortranslate_command
 * @param mode
 * @param cstate
 * @param event
 * @param data
 *
 * Translate and move command
 *
 * KMJ :  Working in Qt version
 * TODO : fix the rubber banding
 */
void moveortranslate_command(int mode,int *cstate,HighLevelEvent *event,void **data)
{RBTranslate_data *rbdata = (RBTranslate_data *) *data;
 Transform t,tr;
 EntityList list;
 int i;
 DialogControl *dc;
 Point zero(0.0,0.0,0.0),p1,p2;
 Dialog *dialog;
  switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              rbdata = 0;
              break;
           }
         rbdata = new RBTranslate_data;  *data = rbdata;
         if (mode == 0) 
           {  dialog = new Dialog("Move_Dialog");
              rbdata->p.setxyz(v.getreal("tr::movex"),v.getreal("tr::movey"),v.getreal("tr::movez"));
              rbdata->type = v.getinteger("tr::movetype");
           }
         else
           {  dialog = new Dialog("Translate_Dialog");
              rbdata->p.setxyz(v.getreal("tr::copyx"),v.getreal("tr::copyy"),v.getreal("tr::copyz"));
              rbdata->type = v.getinteger("tr::copytype");
           }
         rbdata->rotation.setxyz(v.getreal("tr::deltarotationx"),v.getreal("tr::deltarotationy"),v.getreal("tr::deltarotationz"));
         rbdata->ncopies = v.getinteger("tr::ncopies");
         dialog->add(new TRRadioButtonDialogControl(100,100,101,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(101,100,101,&rbdata->type));
         dialog->add(dc = new RealDialogControl(102,&rbdata->p.x,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1102,dc));
         dialog->add(dc = new RealDialogControl(103,&rbdata->p.y,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1103,dc));
         dialog->add(dc = new RealDialogControl(104,&rbdata->p.z,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1104,dc));
         if (mode != 0)
           {  if (rbdata->ncopies < 1)
                rbdata->ncopies = 1;
              dialog->add(dc = new IntegerDialogControl(105,&rbdata->ncopies,1,10000));
              dialog->add(new ScrollBarDialogControl(1105,dc));
           }
         else
           rbdata->ncopies = 0;  
         //dialog->add(dc = new RealDialogControl(107,&rbdata->rotation.x));
         //dialog->add(new ScrollBarDialogControl(1107,dc));
         //dialog->add(dc = new RealDialogControl(108,&rbdata->rotation.y));
         //dialog->add(new ScrollBarDialogControl(1108,dc));
         //dialog->add(dc = new RealDialogControl(109,&rbdata->rotation.z));
         //dialog->add(new ScrollBarDialogControl(1109,dc));
#if 1
         // KMJ : is this needed??
         if (program->getlicensed() >= 2)
           {
              dialog->adddisable(104);
              //dialog->adddisable(1104);
              rbdata->p.z = 0.0;
           }
#endif
         if (dialog->process() == TRUE)
           {  if (mode == 0) 
                {  v.setreal("tr::movex",rbdata->p.x);
                   v.setreal("tr::movey",rbdata->p.y);
                   v.setreal("tr::movez",rbdata->p.z);
                   v.setinteger("tr::movetype",rbdata->type);
                }
              else
                {  v.setreal("tr::copyx",rbdata->p.x);
                   v.setreal("tr::copyy",rbdata->p.y);
                   v.setreal("tr::copyz",rbdata->p.z);
                   v.setinteger("tr::copytype",rbdata->type);
                }
              v.setreal("tr::deltarotationx",rbdata->rotation.x);
              v.setreal("tr::deltarotationy",rbdata->rotation.y);
              v.setreal("tr::deltarotationz",rbdata->rotation.z);
              rbdata->rotation.setxyz(0.0,0.0,0.0);
              if (mode != 0)
                v.setinteger("tr::ncopies",rbdata->ncopies);
              if (db.workplanes.getcurrent() != NULL)
                rbdata->p = db.workplanes.getcurrent()->workplanetomodel(rbdata->p) -
                            db.workplanes.getcurrent()->workplanetomodel(zero);
              rbdata->list = state.getselection().copy();

              if (mode == 0) {
                    RemoveLockedEntities(&rbdata->list,0);
                    RemovePartialGroupEntities(&rbdata->list,0);
              }

              if (rbdata->type == 0)
                {  if (rbdata->p.length() > db.getptoler() ||RCMessageBox(cadwindow->gethwnd(),_RCT("You are not moving the selected entities.\r\nConfirm this operation."),_RCT("Copy/Move"),MB_OKCANCEL) == IDOK)
                     {  if (rbdata->ncopies == 0)
                          transformlist(0,rbdata->ncopies,rbdata->list,t.translate(rbdata->p),0);
                        else
                          {  db.saveundo(UD_STARTBLOCK,NULL);
                             for (i = 1 ; i <= rbdata->ncopies ; i++)
                               {  t.translate(rbdata->p * i);
                                  transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                               }
                             db.saveundo(UD_ENDBLOCK,NULL);
                          }
                     }
                   *cstate = ExitState;
                }
              else
                *cstate = 1;
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Line *l = (Line *)((EntitySelectedEvent *)event)->getentity();
              state.destroyselection(1);
              if (((EntitySelectedEvent *)event)->getendselected() == 1)
                {  p1 = l->getp1();  p2 = l->getp2();
                }
              else
                {  p1 = l->getp2();  p2 = l->getp1();
                }
              if (rbdata->ncopies == 0)
                {  transformlist(0,rbdata->ncopies,rbdata->list,t.translate(p2-p1),0);
                   rbdata->p = p2;
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t.translate((p2-p1) * i);
                        transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->ncopies == 0)
                {  t.translate(-rbdata->p);
                   tr.rotatex(rbdata->rotation.x * acos(-1.0) / 180.0);
                   t.apply(tr);
                   tr.rotatey(rbdata->rotation.y * acos(-1.0) / 180.0);
                   t.apply(tr);
                   tr.rotatez(rbdata->rotation.z * acos(-1.0) / 180.0);
                   t.apply(tr);
                   tr.translate(((Coordinate3dEvent *)event)->getp());
                   t.apply(tr);
                   transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                   rbdata->p = ((Coordinate3dEvent *)event)->getp();
                   rbdata->rotation.setxyz(0.0,0.0,0.0);  //  Reset the rotation angle once it is placed.
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t.translate(-rbdata->p);
                        tr.rotatex(rbdata->rotation.x * acos(-1.0) / 180.0 * i);
                        t.apply(tr);
                        tr.rotatey(rbdata->rotation.y * acos(-1.0) / 180.0 * i);
                        t.apply(tr);
                        tr.rotatez(rbdata->rotation.z * acos(-1.0) / 180.0 * i);
                        t.apply(tr);
                        tr.translate((((Coordinate3dEvent *)event)->getp()-rbdata->p) * i + rbdata->p);
                        t.apply(tr);
                        transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
           }
         else if (event->isa(User))
           {  if (((UserEvent *)event)->getuserid() == 1)
                rbdata->rotation.z += v.getreal("tr::deltarotationz");
              else if (((UserEvent *)event)->getuserid() == 2)
                rbdata->rotation.z -= v.getreal("tr::deltarotationz");
              else if (((UserEvent *)event)->getuserid() == 3)
                rbdata->rotation.x += v.getreal("tr::deltarotationx");
              else if (((UserEvent *)event)->getuserid() == 4)
                rbdata->rotation.x -= v.getreal("tr::deltarotationx");
              else if (((UserEvent *)event)->getuserid() == 5)
                rbdata->rotation.y += v.getreal("tr::deltarotationy");
              else if (((UserEvent *)event)->getuserid() == 6)
                rbdata->rotation.y -= v.getreal("tr::deltarotationy");
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
     }
  if (rbdata != 0)
    rbdata->state = *cstate;
  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRANS);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBTranslate,rbdata);
       cadwindow->prompt->normalprompt(NCTRANS+1);
    }
  else
    delete rbdata;
}

void translate_command(int *cstate,HighLevelEvent *event,void **data)
{ moveortranslate_command(1,cstate,event,data);
}

void move_command(int *cstate,HighLevelEvent *event,void **data)
{ moveortranslate_command(0,cstate,event,data);
}

/**
 * @brief project_command
 * @param cstate
 * @param event
 * @param data
 *
 * Project command
 *
 * KMJ : Working in Qt Version
 *
*/
void project_command(int *cstate,HighLevelEvent *event,void **data)
{RBTranslate_data *rbdata = (RBTranslate_data *) *data;
 Transform t;
 EntityList list,elist,nextlist;
 DialogControl *dc;
 int i;
 Point zero(0.0,0.0,0.0),p1,p2,xaxis,yaxis,zaxis;
 Dialog *dialog;
 ResourceString rs2(NCTRANS+2);
 Entity *e;

  switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              rbdata = 0;
              break;
           }
         rbdata = new RBTranslate_data;  *data = rbdata;
         dialog = new Dialog("Project_Dialog");
         rbdata->p.setxyz(v.getreal("tr::projectx"),v.getreal("tr::projecty"),v.getreal("tr::projectz"));
         rbdata->type = v.getinteger("tr::projecttype");
         rbdata->ncopies = v.getinteger("tr::pncopies");
         rbdata->surface = v.getinteger("tr::surface");
         dialog->title(rs2.gets());
         dialog->add(new TRRadioButtonDialogControl(100,100,104,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(101,100,104,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(102,100,104,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(103,100,104,&rbdata->type));
         dialog->add(new TRRadioButtonDialogControl(104,100,104,&rbdata->type));
         dialog->add(dc = new RealDialogControl(105,&rbdata->p.x,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1105,dc));
         dialog->add(dc = new RealDialogControl(106,&rbdata->p.y,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1106,dc));
         dialog->add(dc = new RealDialogControl(107,&rbdata->p.z,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1107,dc));
         dialog->add(dc = new IntegerDialogControl(108,&rbdata->ncopies,1,10000));
         //dialog->add(new ScrollBarDialogControl(1108,dc));
         dialog->add(new CheckBoxDialogControl(109,&rbdata->surface));
#if 1
         // KMJ : is this needed?
         if (program->getlicensed() >= 2)
           {
              dialog->adddisable(107);
              //dialog->adddisable(1107);
              rbdata->p.z = 0.0;
           }
#endif
         if (dialog->process() == TRUE)
           {  v.setreal("tr::projectx",rbdata->p.x);
              v.setreal("tr::projecty",rbdata->p.y);
              v.setreal("tr::projectz",rbdata->p.z);
              v.setinteger("tr::projecttype",rbdata->type);
              v.setinteger("tr::pncopies",rbdata->ncopies);
              v.setinteger("tr::surface",rbdata->surface);
              if (db.workplanes.getcurrent() != NULL)
                rbdata->p = db.workplanes.getcurrent()->workplanetomodel(rbdata->p) -
                            db.workplanes.getcurrent()->workplanetomodel(zero);
              rbdata->list = state.getselection().copy();
              if (rbdata->type == 0)
                {  if (rbdata->ncopies == 0)
                     {  transformlist(1,rbdata->ncopies,rbdata->list,t.translate(rbdata->p),rbdata->surface);
                     } 
                   else
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (i = 1 ; i <= rbdata->ncopies ; i++)
                          {  t.translate(rbdata->p * i);
                             transformlist(1,rbdata->ncopies,rbdata->list,t,rbdata->surface);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }

                   *cstate = ExitState;
                }
              else if (rbdata->type == 2)
                *cstate = 101;
              else if (rbdata->type == 3)
                *cstate = 111;
              else if (rbdata->type == 4)
                *cstate = 150;
              else
                *cstate = 1;
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {Line *l = (Line *)((EntitySelectedEvent *)event)->getentity();
              state.destroyselection(1);
              if (((EntitySelectedEvent *)event)->getendselected() == 1)
                {  p1 = l->getp1();  p2 = l->getp2();
                }
              else
                {  p1 = l->getp2();  p2 = l->getp1();
                }
              if (rbdata->ncopies == 0)
                {  transformlist(1,rbdata->ncopies,rbdata->list,t.translate(p2-p1),rbdata->surface);
                   rbdata->p = p2;
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t.translate((p2-p1) * i);
                        transformlist(1,rbdata->ncopies,rbdata->list,t,rbdata->surface);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->ncopies == 0)
                {  transformlist(1,rbdata->ncopies,rbdata->list,t.translate((((Coordinate3dEvent *)event)->getp()-rbdata->p)),rbdata->surface);
                   rbdata->p = ((Coordinate3dEvent *)event)->getp();
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t.translate((((Coordinate3dEvent *)event)->getp()-rbdata->p) * i);
                        transformlist(1,rbdata->ncopies,rbdata->list,t,rbdata->surface);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;

       case 101 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              rbdata->zaxis = ((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis());
              *cstate = 102;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              zaxis = ((EntitySelectedEvent *)event)->getxaxis().cross(((EntitySelectedEvent *)event)->getyaxis());
              if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   rbdata->p1 = l->getp1();
                   rbdata->zaxis = (zaxis.cross(l->getp2()-l->getp1())).normalize();
                   *cstate = 120;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 102 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              zaxis = ((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis());
              rbdata->zaxis = (zaxis.cross(p2 - rbdata->p1)).normalize();
              rbdata->p1 = p2;
              *cstate = 120; 
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 111 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 112;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              if (e->isa(circle_entity))
                {Circle *c = (Circle *)e;
                   rbdata->p1 = c->getorigin();
                   rbdata->zaxis = (c->getxaxis().cross(c->getyaxis())).normalize();
                   *cstate = 120; 
                }
              else if (e->isa(plane_entity))
                {Plane *p = (Plane *)e;
                   rbdata->p1 = p->getorigin();
                   rbdata->zaxis = p->getxaxis().cross(p->getyaxis());
                   *cstate = 120; 
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 112 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 113;
           }
         else if (event->isa(Exit) || event->isa(Abort))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
       case 113 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              xaxis = rbdata->p2 - rbdata->p1;
              yaxis = p2 - rbdata->p1;
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  xaxis.normalize();  yaxis.normalize();
                   rbdata->p1 = p2;
                   rbdata->zaxis = (xaxis.cross(yaxis)).normalize();
                   *cstate = 120; 
                }
           }
         else if (event->isa(Exit) || event->isa(Abort))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
       case 120 :
         if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *)event)->getp();
              *cstate = 121;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              zaxis = ((EntitySelectedEvent *)event)->getxaxis().cross(((EntitySelectedEvent *)event)->getyaxis());
              if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   projectlist(rbdata->list,(l->getp2()-l->getp1()).normalize(),rbdata->p1,rbdata->zaxis,rbdata->surface);
                   state.setselection(rbdata->list);
                   *cstate = ExitState;
                }
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 121 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  projectlist(rbdata->list,(((Coordinate3dEvent *)event)->getp()-rbdata->p).normalize(),rbdata->p1,rbdata->zaxis,rbdata->surface);
              state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 150 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(0);
              rbdata->dlist.add(((EntitySelectedEvent *)event)->getentity());
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {Entity *e1,*e2,*elast;
            Point p,p1,p2,p3,p4,dir1,dir2,normal;
            int reverse,xmesh,ymesh;
            double angle,deltaz;
            Transform t;
            BitMask options(32);

              nextlist = rbdata->list;
              rbdata->dlist.reverse();
              rbdata->dlist.unhighlight();

              db.saveundo(UD_STARTBLOCK,NULL);
              elast = 0;
              for (rbdata->dlist.start() ; (e1 = rbdata->dlist.next()) != 0 ; )
                {  e2 = (Entity *) rbdata->dlist.peek();
                   p1 = ((LinearEntity *)e1)->start();
                   p2 = ((LinearEntity *)e1)->end();

                   if (e2 != 0)
                     {  p3 = ((LinearEntity *)e2)->start();
                        p4 = ((LinearEntity *)e2)->end();      

                        if ((p2 - p3).length() < db.getptoler())
                          {  //  Already in the correct order
                             reverse = 0;
                          } 
                        else if ((p2 - p4).length() < db.getptoler())
                          {  //  Reverse the second line
                             p = p3;  p3 = p4; p4 = p;
                             reverse = 0;
                          }
                        else if ((p1 - p3).length() < db.getptoler())
                          {  //  Reverse the first line
                             p = p1;  p1 = p2; p2 = p;
                             reverse = 1;
                          }
                        else if ((p1 - p4).length() < db.getptoler())
                          {  //  Reverse the both lines
                             p = p1;  p1 = p2; p2 = p;
                             p = p3;  p3 = p4; p4 = p;
                             reverse = 1;
                          }
                        else
                          {  cadwindow->MessageBox("This is not connected to the previous entity.","Project",MB_OK);
                             break; 
                          }
                     }
                   else if (elast != 0)
                     {  p3 = ((LinearEntity *)elast)->start();
                        p4 = ((LinearEntity *)elast)->end();      

                        if ((p4 - p1).length() < db.getptoler())
                          {  //  Already in the correct order
                             reverse = 0;
                          } 
                        else if ((p3 - p1).length() < db.getptoler())
                          {  //  Reverse the second line
                             reverse = 0;
                          }
                        else if ((p4 - p2).length() < db.getptoler())
                          {  //  Reverse the first line
                             p = p1;  p1 = p2; p2 = p;
                             reverse = 1;
                          }
                        else if ((p3 - p2).length() < db.getptoler())
                          {  //  Reverse the both lines
                             p = p1;  p1 = p2; p2 = p;
                             reverse = 1;
                          }
                        else
                          {  cadwindow->MessageBox("This is not connected to the previous entity.","Project",MB_OK);
                             break; 
                          }

                        p3 = ((LinearEntity *)e1)->end();
                        p4 = ((LinearEntity *)e1)->end() - (((LinearEntity *)e1)->start() - ((LinearEntity *)e1)->end());

                     }
                   else
                     {  reverse = ((state.getlastpickpoint() - p2).length() < (state.getlastpickpoint() - p1).length());
                        p3 = ((LinearEntity *)e1)->end();
                        p4 = ((LinearEntity *)e1)->end() - (((LinearEntity *)e1)->start() - ((LinearEntity *)e1)->end());
                     }


                   if (e1->isa(line_entity))
                     {  dir1 = (p2 - p1).normalize();
                        dir2 = (p3 - p4).normalize();
                        if (e2 != 0 && e2->isa(circle_entity))   
                          dir2 = -dir1;  //  Don't rotate the entity when it touches a circle
         
                        if (fabs(dir1.dot(dir2)) < 0.999)
                          normal = ((dir1.cross(dir2)).cross(dir1 + dir2)).normalize();
                        else 
                          normal = dir1;

                        projectlist(nextlist,dir1,p2,normal,rbdata->surface,&nextlist);

                     }
                   else
                     {  //  Just rotate the entities around the arc
                      EntityList nlist;
         
                        SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);

                        normal = (((Circle *)e1)->getxaxis().cross(((Circle *)e1)->getyaxis())).normalize();
                        p1 = ((Circle *)e1)->getorigin();
                        angle = ((Circle *)e1)->getsweepa();
                        if (reverse)
                          angle = -angle; 

                        t.rotate(p1,normal,angle);

                        db.saveundo(UD_STARTBLOCK,NULL);
                        for (nextlist.start() ; (e = nextlist.next()) != NULL ; )
                          {  e->setcopye(0);
                             db.geometry.add(e->clone(t));
                             nlist.add(e->getcopye()); 
                             
                             if (rbdata->surface)
                               {  SurfaceOfRev *surface = new SurfaceOfRev(*e,p1,normal,angle*180.0/M_PI,0.0,(LinearEntity *)e,xmesh,ymesh,options);
                                  if (surface != NULL && surface->getdefined())
                                    db.geometry.add(surface);
                               }
                          }
                        nextlist.destroy();
                        nextlist = nlist;
                        db.saveundo(UD_ENDBLOCK,NULL);

                     }

                   elast = e1;

                }
              db.saveundo(UD_ENDBLOCK,NULL);
 
              *cstate = ExitState;
           }
         break;
     }

  if (rbdata != 0)
    rbdata->state = *cstate;

  if (*cstate == 1)
    {  cadwindow->prompt->normalprompt(NCTRANS+3);
       state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBTranslate,rbdata);
       cadwindow->prompt->normalprompt(NCTRANS+4);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 101)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt(NCTRANS+5);
    }
  else if (*cstate == 102)
    {  rubberband.begin(0);
       rubberband.add(RBTranslate,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCTRANS+6);
    }
  else if (*cstate == 111)
    {  state.selmask.entity.clearandset(xyz_mask,circle_entity,plane_entity,end_list);
       cadwindow->prompt->normalprompt(NCTRANS+7);
    }
  else if (*cstate == 112 || *cstate == 113)
    {  rubberband.begin(0);
       rubberband.add(RBTranslate,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 112)
         cadwindow->prompt->normalprompt(NCTRANS+8);
       else 
         cadwindow->prompt->normalprompt(NCTRANS+9);
    }
  else if (*cstate == 120)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the start of the projection direction or a line");
    }
  else if (*cstate == 121)
    {  rubberband.begin(0);
       rubberband.add(RBTranslate,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the end of the projection direction");
    }
  else if (*cstate == 150)
    {  state.selmask.entity.clearandset(line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the entity that defines the projection");
    }
  else
    delete rbdata;

}

struct RBMirror_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
   int state,type,ncopies;
   Point p1,p2,zaxis;
   EntityList list;
};

#ifdef NEW_RUBBERB
void RBMirror(int dmode,void *data,Point *p2,View3dSurface *)
{
    RBMirror_data *rbdata = (RBMirror_data *) data;
    Transform t;
    Entity *e;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }

     cadwindow->startdrawingbatch();

     if (rbdata->state == 2 || rbdata->state == 12 || rbdata->state == 13)
     {
         Line line(rbdata->p1,*p2);

         //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;

          if (rbdata->state == 2)
          {//  Draw the rubberband image
               if (db.getdragdetail() == 0)
               {
                    t.mirror(*p2,rbdata->zaxis.cross(*p2-rbdata->p1));
                    for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
                      e->draw(DM_INVERT,&t);
               }
          }

     }
     if (rbdata->state == 13)
     {
         Line line(rbdata->p1,rbdata->p2);

         //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
     }

     cadwindow->enddrawingbatch();

}
#else
void RBMirror(int,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
    RBMirror_data *rbdata = (RBMirror_data *) data;
    Transform t;
    Entity *e;

     cadwindow->startdrawingbatch();

     if (rbdata->state == 2 || rbdata->state == 12 || rbdata->state == 13)
     {
         Line line(rbdata->p1,*p2);

         line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;

          if (rbdata->state == 2)
          {//  Draw the rubberband image
               if (db.getdragdetail() == 0)
               {
                    t.mirror(*p2,rbdata->zaxis.cross(*p2-rbdata->p1));
                    for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
                      e->draw(DM_INVERT,&t);
               }
          }

     }
     if (rbdata->state == 13)
     {
         Line line(rbdata->p1,rbdata->p2);

         line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
     }

     cadwindow->enddrawingbatch();
#else
 RBMirror_data *rbdata = (RBMirror_data *) data;
 Transform t;
 Entity *e;

  cadwindow->startdrawingbatch();

  if (rbdata->state == 2 || rbdata->state == 12 || rbdata->state == 13)
    {  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);

       if (rbdata->state == 2)
         {//  Draw the rubberband image
            if (db.getdragdetail() == 0)
              {  t.mirror(*p2,rbdata->zaxis.cross(*p2-rbdata->p1));
                 for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
                   e->draw(DM_INVERT,&t);
              }
         }

    }
  if (rbdata->state == 13)
    {  Line line(rbdata->p1,rbdata->p2);  line.draw(DM_INVERT);
    }

  cadwindow->enddrawingbatch();
#endif
}
#endif

/**
 * @brief mirror_command
 * @param cstate
 * @param event
 * @param data
 *
 * Mirror Command
 *
 * KMJ : Working in Qt version
 * TODO : fix the rubber banding
 */
void mirror_command(int *cstate,HighLevelEvent *event,void **data)
{
	RBMirror_data *rbdata = (RBMirror_data *) *data;
	Transform t;
	int i;
	Entity *e=0;
	
	Point p2,xaxis,yaxis,zaxis;
	Dialog *dialog;
   switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              rbdata = 0;
              break;
           }
		
         rbdata = new RBMirror_data;
         rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         dialog = new Dialog("Mirror_Dialog");
         rbdata->type = v.getinteger("tr::mtype");
         rbdata->ncopies = v.getinteger("tr::mcopies");
         dialog->add(new RadioButtonDialogControl(100,100,101,&rbdata->type));
         dialog->add(new RadioButtonDialogControl(101,100,101,&rbdata->type));
         dialog->add(new CheckBoxDialogControl(102,&rbdata->ncopies));
#if 1
         // KMJ : is this needed?
         if (program->getlicensed() >= 2)
           {  dialog->adddisable(101);
              rbdata->type = 0;
           }
#endif
         if (dialog->process() == TRUE)
           {  v.setinteger("tr::mtype",rbdata->type);
              v.setinteger("tr::mcopies",rbdata->ncopies);
              rbdata->list = state.getselection().copy();
              if (rbdata->ncopies == 0) {
                RemoveLockedEntities(&rbdata->list,0);
                RemovePartialGroupEntities(&rbdata->list,0);
              }
              if (rbdata->type == 0)
                *cstate = 1;
              else
                *cstate = 11;
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              rbdata->zaxis = ((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis());
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
			  
              zaxis = ((EntitySelectedEvent *)event)->getxaxis().cross(((EntitySelectedEvent *)event)->getyaxis());
              if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   if (rbdata->ncopies == 0)
                     transformlist(0,rbdata->ncopies,rbdata->list,t.mirror(l->getp1(),zaxis.cross(l->getp2()-l->getp1())),0);
                   else
                     {  for (i = 1 ; i <= rbdata->ncopies ; i++)
                          {  t.mirror(l->getp1(),zaxis.cross(l->getp2()-l->getp1()));
                             transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                          } 
                     }   
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
              if (rbdata->ncopies == 0)
                transformlist(0,rbdata->ncopies,rbdata->list,t.mirror(p2,zaxis.cross(p2-rbdata->p1)),0);
              else
                for (i = 1 ; i <= rbdata->ncopies ; i++)
                  {  t.mirror(p2,zaxis.cross(p2-rbdata->p1));
                     qDebug() << "mirror matrix";
                     t.debug();
                     qDebug();
                     transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                  }
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
                   if (rbdata->ncopies == 0)
                     transformlist(0,rbdata->ncopies,rbdata->list,t.mirror(c->getorigin(),c->getxaxis().cross(c->getyaxis())),0);
                   else
                     for (i = 1 ; i <= rbdata->ncopies ; i++)
                       {  t.mirror(c->getorigin(),c->getxaxis().cross(c->getyaxis()));
                          transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                       }
                }
              else if (e->isa(plane_entity))
                {Plane *p = (Plane *)e;
                   if (rbdata->ncopies == 0)
                     transformlist(0,rbdata->ncopies,rbdata->list,t.mirror(p->getorigin(),p->getxaxis().cross(p->getyaxis())),0);
                   else
                     for (i = 1 ; i <= rbdata->ncopies ; i++)
                       {  t.mirror(p->getorigin(),p->getxaxis().cross(p->getyaxis()));
                          transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                       }
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
                   if (rbdata->ncopies == 0)
                     transformlist(0,rbdata->ncopies,rbdata->list,t.mirror(p2,xaxis.cross(yaxis)),0);
                   else
                     for (i = 1 ; i <= rbdata->ncopies ; i++)
                       {  t.mirror(p2,xaxis.cross(yaxis));
                          transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                       }
                }
           }
         state.setselection(rbdata->list);
         *cstate = ExitState;
         break;
     }
#ifdef USING_WIDGETS
  if(rbdata && rbdata->qgi)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
  }
#endif
  if (rbdata != 0)
    rbdata->state = *cstate;
  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBMirror,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 11)
    state.selmask.entity.clearandset(xyz_mask,circle_entity,plane_entity,end_list);
  else if (*cstate == 12 || *cstate == 13)
    {  rubberband.begin(0);
       rubberband.add(RBMirror,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRANS+5);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCTRANS+6);
  else if (*cstate == 11)
    cadwindow->prompt->normalprompt(NCTRANS+7);
  else if (*cstate == 12)
    cadwindow->prompt->normalprompt(NCTRANS+8);
  else if (*cstate == 13)
    cadwindow->prompt->normalprompt(NCTRANS+9);
  else
    delete rbdata;
}

struct RBRotate_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
   int state,type,ncopies;
   Point p1,p2;
   double angle,x,y,z;
   EntityList list;
};

#ifdef NEW_RUBBERB
void RBRotate(int dmode,void *data,Point *p2,View3dSurface *)
{
    RBRotate_data *rbdata = (RBRotate_data *) data;
    Transform t1,t2;
    Point xaxis,yaxis,zaxis;
    double cosa;
    Entity *e;

    if(dmode == RB_ERASE)
    {
        cadwindow->clearAllRubberBands();
        return;
    }

     cadwindow->startdrawingbatch();

     if (rbdata->state == 12 || rbdata->state == 22 || rbdata->state == 23)
     {
         Line line(rbdata->p1,*p2);

         //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
     }
     if (rbdata->state == 23)
     {
          Line line(rbdata->p1,rbdata->p2);
          if (rbdata->type != 3)
          {
              //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
          }

          xaxis = rbdata->p2 - rbdata->p1;
          yaxis = *p2 - rbdata->p1;
          if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
          {
               xaxis.normalize();  yaxis.normalize();
               if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
                 zaxis = Point(0.0,0.0,1.0);
               cosa = xaxis.dot(yaxis);
               if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
               rbdata->angle = acos(cosa);
               t1.translate(rbdata->x,rbdata->y,rbdata->z);
               t2.rotate(rbdata->p1,zaxis,rbdata->angle);
               t2.apply(t1);

               for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
                 e->draw(DM_INVERT,&t2);

          }
     }

     cadwindow->enddrawingbatch();

}
#else
void RBRotate(int,void *data,Point *p2,View3dSurface *)
{
#ifdef USING_WIDGETS
    RBRotate_data *rbdata = (RBRotate_data *) data;
    Transform t1,t2;
    Point xaxis,yaxis,zaxis;
    double cosa;
    Entity *e;

     cadwindow->startdrawingbatch();

     if (rbdata->state == 12 || rbdata->state == 22 || rbdata->state == 23)
     {
         Line line(rbdata->p1,*p2);

         line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
         line.draw(DM_INVERT);
         rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
     }
     if (rbdata->state == 23)
     {
          Line line(rbdata->p1,rbdata->p2);
          if (rbdata->type != 3)
          {
              line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
              line.draw(DM_INVERT);
              rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
          }

          xaxis = rbdata->p2 - rbdata->p1;
          yaxis = *p2 - rbdata->p1;
          if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
          {
               xaxis.normalize();  yaxis.normalize();
               if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
                 zaxis = Point(0.0,0.0,1.0);
               cosa = xaxis.dot(yaxis);
               if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
               rbdata->angle = acos(cosa);
               t1.translate(rbdata->x,rbdata->y,rbdata->z);
               t2.rotate(rbdata->p1,zaxis,rbdata->angle);
               t2.apply(t1);

               for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
                 e->draw(DM_INVERT,&t2);

          }
     }

     cadwindow->enddrawingbatch();
#else
 RBRotate_data *rbdata = (RBRotate_data *) data;
 Transform t1,t2;
 Point xaxis,yaxis,zaxis;
 double cosa;
 Entity *e;

  cadwindow->startdrawingbatch();

  if (rbdata->state == 12 || rbdata->state == 22 || rbdata->state == 23)
    {  Line line(rbdata->p1,*p2);  line.draw(DM_INVERT);
    }
  if (rbdata->state == 23)
    {  Line line(rbdata->p1,rbdata->p2);  
       if (rbdata->type != 3)
         line.draw(DM_INVERT);

       xaxis = rbdata->p2 - rbdata->p1;
       yaxis = *p2 - rbdata->p1;
       if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
         {  xaxis.normalize();  yaxis.normalize();
            if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
              zaxis = Point(0.0,0.0,1.0);
            cosa = xaxis.dot(yaxis);
            if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
            rbdata->angle = acos(cosa);
            t1.translate(rbdata->x,rbdata->y,rbdata->z);
            t2.rotate(rbdata->p1,zaxis,rbdata->angle);
            t2.apply(t1);

            for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
              e->draw(DM_INVERT,&t2);

         }

    }

  cadwindow->enddrawingbatch();
#endif
}
#endif

class RORadioButtonDialogControl : public RadioButtonDialogControl
{public:
   RORadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void RORadioButtonDialogControl::load(Dialog *dialog)
{int i;
  RadioButtonDialogControl::load(dialog);
  for (i = 106 ; i <= 1106 ; i += 100)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue < 2);
       if (i == 206)
         i = 1006;
    }
}

int RORadioButtonDialogControl::select(Dialog *dialog)
{int i;
  RadioButtonDialogControl::select(dialog);
  for (i = 106 ; i <= 1106; i += 100)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue < 2);
       if (i == 206)
         i = 1006;
    }
  return 0;
}

void rotate_command(int *cstate,HighLevelEvent *event,void **data)
{RBRotate_data *rbdata = (RBRotate_data *) *data;
 Transform t1,t2;
 Entity *e;
 EntityList list;
 DialogControl *dc;
 int i;
 Point p,p1,p2,xaxis,yaxis,zaxis;
 double cosa;
 Dialog *dialog;
  switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              rbdata = 0;
              break;
           }
         rbdata = new RBRotate_data;
#ifdef USING_WIDGETS
         rbdata->qgi=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
         *data = rbdata;
         dialog = new Dialog("Rotate_Dialog");
         rbdata->angle = v.getreal("tr::angle");
         rbdata->type = v.getinteger("tr::rtype");
         rbdata->ncopies = v.getinteger("tr::nrotatecopies");
         rbdata->x = 0.0;
         rbdata->y = 0.0;
         rbdata->z = 0.0;
         dialog->add(new RORadioButtonDialogControl( 99,99,102,&rbdata->type));
         dialog->add(new RORadioButtonDialogControl(100,99,102,&rbdata->type));
         dialog->add(new RORadioButtonDialogControl(101,99,102,&rbdata->type));
         dialog->add(new RORadioButtonDialogControl(102,99,102,&rbdata->type));
         dialog->add(dc = new RealDialogControl(103,&rbdata->x));
         //dialog->add(new ScrollBarDialogControl(1103,dc));
          dialog->add(dc = new RealDialogControl(104,&rbdata->y));
         //dialog->add(new ScrollBarDialogControl(1104,dc));
          dialog->add(dc = new RealDialogControl(105,&rbdata->z));
         //dialog->add(new ScrollBarDialogControl(1105,dc));
          dialog->add(dc = new RealDialogControl(106,&rbdata->angle));
         //dialog->add(new ScrollBarDialogControl(1106,dc));
         dialog->add(dc = new IntegerDialogControl(107,&rbdata->ncopies,0,10000));
         //dialog->add(new ScrollBarDialogControl(1107,dc));
#if 1
         // KMJ : is this needed?
         if (program->getlicensed() >= 2)
           {
              dialog->adddisable(105);
              //dialog->adddisable(1105);
              rbdata->z = 0.0;
           }
#endif
         if (dialog->process() == TRUE)
           {  v.setreal("tr::angle",rbdata->angle);
              v.setinteger("tr::rtype",rbdata->type);
              v.setinteger("tr::nrotatecopies",rbdata->ncopies);
              rbdata->list = state.getselection().copy();
              if (rbdata->ncopies == 0) {
                RemoveLockedEntities(&rbdata->list,0);  
                RemovePartialGroupEntities(&rbdata->list,0);
              }
              if (rbdata->type == 0)
                *cstate = 1;
              else if (rbdata->type == 1)
                *cstate = 11;
              else if (rbdata->type == 2 || rbdata->type == 3)
                *cstate = 21;
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  p = ((Coordinate3dEvent *)event)->getp();
              xaxis = ((Coordinate3dEvent *)event)->getxaxis();
              yaxis = ((Coordinate3dEvent *)event)->getyaxis();
              if (rbdata->ncopies == 0)
                {  t1.translate(rbdata->x,rbdata->y,rbdata->z);
                   t2.rotate(p,xaxis.cross(yaxis),rbdata->angle * M_PI / 180.0);
                   t2.apply(t1);
                   transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t1.translate(rbdata->x*i,rbdata->y*i,rbdata->z*i);
                        t2.rotate(p,xaxis.cross(yaxis),rbdata->angle * i * M_PI / 180.0);
                        t2.apply(t1);
                        transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
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
              if (e->isa(line_entity))
                {Line *l = (Line *)e;
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     {  p1 = l->getp1();  p2 = l->getp2();
                     }
                   else
                     {  p1 = l->getp2();  p2 = l->getp1();
                     }
                   if (rbdata->ncopies == 0)
                     {  t1.translate(rbdata->x,rbdata->y,rbdata->z);
                        t2.rotate(p1,p2 - p1,rbdata->angle * M_PI / 180.0);
                        t2.apply(t1);
                        transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                     }
                   else
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (i = 1 ; i <= rbdata->ncopies ; i++)
                          {  t1.translate(rbdata->x*i,rbdata->y*i,rbdata->z*i);
                             t2.rotate(p1,p2 - p1,rbdata->angle * i * M_PI / 180.0);
                             t2.apply(t1);
                             transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
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
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              if (rbdata->ncopies == 0)
                {  t1.translate(rbdata->x,rbdata->y,rbdata->z);
                   t2.rotate(rbdata->p1,p2 - rbdata->p1,rbdata->angle * M_PI / 180.0);
                   t2.apply(t1);
                   transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                }
              else
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t1.translate(rbdata->x*i,rbdata->y*i,rbdata->z*i);
                        t2.rotate(rbdata->p1,p2 - rbdata->p1,rbdata->angle * M_PI / 180.0 * i);
                        t2.apply(t1);
                        transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                } 
           }
         state.setselection(rbdata->list);
         *cstate = ExitState;
         break;
       case 21 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              if (rbdata->type == 3)
                {  rbdata->p2 = rbdata->p1 + ((Coordinate3dEvent *)event)->getxaxis();
                   *cstate = 23;
                }
              else 
                *cstate = 22;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              if (e->isa(circle_entity))
                {Circle *c = (Circle *)e;
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     rbdata->angle =  c->getsweepa();
                   else
                     rbdata->angle = -c->getsweepa();
                   if (rbdata->ncopies == 0)
                     {  t1.translate(rbdata->x,rbdata->y,rbdata->z);
                        t2.rotate(c->getorigin(),c->getxaxis().cross(c->getyaxis()),rbdata->angle);
                        t2.apply(t1);
                        transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                     }
                   else
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (i = 1 ; i <= rbdata->ncopies ; i++)
                          {  t1.translate(rbdata->x*i,rbdata->y*i,rbdata->z*i);
                             t2.rotate(c->getorigin(),c->getxaxis().cross(c->getyaxis()),rbdata->angle * i);
                             t2.apply(t1);
                             transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                }
              state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 22 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 23;
           }
         else if (event->isa(Exit) || event->isa(Abort))
           {  state.setselection(rbdata->list);
              *cstate = ExitState;
           }
         break;
       case 23 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *)event)->getp();
              xaxis = rbdata->p2 - rbdata->p1;
              yaxis = p1 - rbdata->p1;
              if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
                {  xaxis.normalize();  yaxis.normalize();
                   if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
                     zaxis = ((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis());
                   cosa = xaxis.dot(yaxis);
                   if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
                   rbdata->angle = acos(cosa);
                   if (rbdata->ncopies == 0)
                     {  t1.translate(rbdata->x,rbdata->y,rbdata->z);
                        t2.rotate(rbdata->p1,zaxis,rbdata->angle);
                        t2.apply(t1);
                        transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                     }
                   else
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (i = 1 ; i <= rbdata->ncopies ; i++)
                          {  t1.translate(rbdata->x*i,rbdata->y*i,rbdata->z*i);
                             t2.rotate(rbdata->p1,zaxis,rbdata->angle * i);
                             t2.apply(t1);
                             transformlist(0,rbdata->ncopies,rbdata->list,t2,0);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                }
           }
         state.setselection(rbdata->list);
         *cstate = ExitState;
         break;
     }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata && rbdata->qgi)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
  }
#endif
#endif
  if (rbdata != 0)
    rbdata->state = *cstate;
  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  else if (*cstate == 11)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  else if (*cstate == 12)
    {  rubberband.begin(0);
       rubberband.add(RBRotate,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  else if (*cstate == 21)
    state.selmask.entity.clearandset(xyz_mask,circle_entity,end_list);
  else if (*cstate == 22 || *cstate == 23)
    {  rubberband.begin(0);
       rubberband.add(RBRotate,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRANS+10);
  else if (*cstate == 11)
    cadwindow->prompt->normalprompt(NCTRANS+11);
  else if (*cstate == 12)
    cadwindow->prompt->normalprompt(NCTRANS+12);
  else if (*cstate == 21)
    cadwindow->prompt->normalprompt(NCTRANS+13);
  else if (*cstate == 22)
    cadwindow->prompt->normalprompt(NCTRANS+14);
  else if (*cstate == 23)
    cadwindow->prompt->normalprompt(NCTRANS+15);
  else
    delete rbdata;
}

class SCButtonDialogControl : public ButtonDialogControl
{public:
   SCButtonDialogControl(int i) : ButtonDialogControl(i) {}
   int select(Dialog *);
};

int SCButtonDialogControl::select(Dialog *dialog)
{RCCHAR text[300];

  if (id == 107)
    {  GetDlgItemText(dialog->gethdlg(),102,text,300);
       SetDlgItemText(dialog->gethdlg(),103,text);
    }
  else if (id == 108)
    {  GetDlgItemText(dialog->gethdlg(),102,text,300);
       SetDlgItemText(dialog->gethdlg(),104,text);
    }
  else if (id == 109)
    {  GetDlgItemText(dialog->gethdlg(),103,text,300);
       SetDlgItemText(dialog->gethdlg(),104,text);
    }

  return 0;
}

class SCRadioButtonDialogControl : public RadioButtonDialogControl
{public:
   SCRadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void SCRadioButtonDialogControl::load(Dialog *dialog)
{   int i;
    RadioButtonDialogControl::load(dialog);
    // KMJ : is this needed?
    for (i = 0 ; i < 2 + (program->getlicensed() < 2) ; i++)
    //for (i = 0 ; i < 3 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),102+i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),202+i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1102+i),! oldvalue);
    }

  EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),205),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1105),oldvalue);

}

int SCRadioButtonDialogControl::select(Dialog *dialog)
{int i;
  RadioButtonDialogControl::select(dialog);

  // KMJ : is this needed
    for (i = 0 ; i < 2 + (program->getlicensed() < 2) ; i++)
    //for (i = 0 ; i < 3 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),102+i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),202+i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1102+i),! oldvalue);
    }

  EnableWindow(GetDlgItem(dialog->gethdlg(),105),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),205),oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1105),oldvalue);

  return 0;
}

struct RBScale_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
   int type,ncopies;
   Point p1,p2,scale;
   double length;
   EntityList list;
};

#ifdef NEW_RUBBERB
void RBScale(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBScale_data *rbdata = (RBScale_data *) data;
  Line line(rbdata->p1,*p2);

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  //line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
  line.draw(DM_INVERT);
  //rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
}
#else
void RBScale(int,void *data,Point *p2,View3dSurface *)
{
  RBScale_data *rbdata = (RBScale_data *) data;
  Line line(rbdata->p1,*p2);

  line.qgi = rbdata->qgi; line.zValue = rbdata->zValue;
  line.draw(DM_INVERT);
  rbdata->qgi = line.qgi; rbdata->zValue = line.zValue;
}
#endif

/**
 * @brief scale_command
 * @param cstate
 * @param event
 * @param data
 *
 * Scale Command
 *
 * KMJ : Working in Qt version
 *
 */
void scale_command(int *cstate,HighLevelEvent *event,void **data)
{RBScale_data *rbdata = (RBScale_data *) *data;
 Transform t;
 EntityList list;
 DialogControl *dc;
 int i;
 Point p,xaxis,yaxis;
 Dialog *dialog;

  switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              rbdata = 0;
              break;
           }
		
         rbdata = new RBScale_data;
         rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
         dialog = new Dialog("Scale_Dialog");
         rbdata->type = v.getinteger("tr::scaletype");
         rbdata->scale.setxyz(v.getreal("tr::sx"),v.getreal("tr::sy"),v.getreal("tr::sz"));
         rbdata->length = v.getreal("tr::length");
         rbdata->ncopies = v.getinteger("tr::ncopies");
         dialog->add(new SCRadioButtonDialogControl(100,100,101,&rbdata->type));
         dialog->add(new SCRadioButtonDialogControl(101,100,101,&rbdata->type));
         dialog->add(dc = new RealDialogControl(102,&rbdata->scale.x,DontScale));
         //dialog->add(new ScrollBarDialogControl(1102,dc));
         dialog->add(dc = new RealDialogControl(103,&rbdata->scale.y,DontScale));
         //dialog->add(new ScrollBarDialogControl(1103,dc));
         dialog->add(dc = new RealDialogControl(104,&rbdata->scale.z,DontScale));
         //dialog->add(new ScrollBarDialogControl(1104,dc));
         dialog->add(dc = new RealDialogControl(105,&rbdata->length,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1105,dc));
         dialog->add(dc = new IntegerDialogControl(106,&rbdata->ncopies,0,10000));
         //dialog->add(new ScrollBarDialogControl(1106,dc));
         dialog->add(new SCButtonDialogControl(107));
         dialog->add(new SCButtonDialogControl(108));
         dialog->add(new SCButtonDialogControl(109));
         if (program->getlicensed() >= 2)
           {
              dialog->adddisable(108);
              dialog->adddisable(109);
              dialog->adddisable(104);
              //dialog->adddisable(1104);
              rbdata->scale.z = 1.0;
           }
         if (dialog->process() == TRUE)
           {  v.setreal("tr::sx",rbdata->scale.x);
              v.setreal("tr::sy",rbdata->scale.y);
              v.setreal("tr::sz",rbdata->scale.z);
              v.setreal("tr::length",rbdata->length);
              v.setinteger("tr::ncopies",rbdata->ncopies);
              v.setinteger("tr::scaletype",rbdata->type);

              rbdata->list = state.getselection().copy();
              if (rbdata->ncopies == 0){
                RemoveLockedEntities(&rbdata->list,0);  
                RemovePartialGroupEntities(&rbdata->list,0);
              }
              if (rbdata->type == 0)
                *cstate = 1;
              else
                *cstate = 100;
           }
         else
           *cstate = ExitState;
         delete dialog;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {Point scale;
              p = ((Coordinate3dEvent *)event)->getp();
              xaxis = ((Coordinate3dEvent *)event)->getxaxis();
              yaxis = ((Coordinate3dEvent *)event)->getyaxis();

              if (rbdata->type == 1)
                {  if ((rbdata->p2 - rbdata->p1).length() > db.getptoler() && rbdata->length > db.getptoler())
                     rbdata->scale.setxyz(rbdata->length / (rbdata->p2 - rbdata->p1).length(),rbdata->length / (rbdata->p2 - rbdata->p1).length(),rbdata->length / (rbdata->p2 - rbdata->p1).length());
                   else
                     rbdata->scale.setxyz(1.0,1.0,1.0);
                }

              if (rbdata->ncopies == 0)
                transformlist(0,rbdata->ncopies,rbdata->list,t.scale(p,xaxis,yaxis,xaxis.cross(yaxis),rbdata->scale),0);
              else
                {  scale = rbdata->scale;
                   db.saveundo(UD_STARTBLOCK,NULL);
                   for (i = 1 ; i <= rbdata->ncopies ; i++)
                     {  t.scale(p,xaxis,yaxis,xaxis.cross(yaxis),scale);
                        transformlist(0,rbdata->ncopies,rbdata->list,t,0);
                        scale.x *= rbdata->scale.x; 
                        scale.y *= rbdata->scale.y; 
                        scale.z *= rbdata->scale.z; 
                     }
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
              state.setselection(rbdata->list);
           }
         *cstate = ExitState;
         break;
       case 100 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 101;
           }
         else if (event->isa(EntitySelected))
           {Line *line = (Line *)((EntitySelectedEvent *)event)->getentity();
              rbdata->p1 = line->getp1();
              rbdata->p2 = line->getp2();
              *cstate = 1;
           }
         else 
           *cstate = ExitState;
         break;
       case 101 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 1;
           }
         else 
           *cstate = ExitState;
         break;
     }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata && rbdata->qgi)
  {
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
  }
#endif
#endif
  if (*cstate == 1 || *cstate == 101)
    state.selmask.entity.clearandset(xyz_mask,end_list);
  else if (*cstate == 100)
    state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCTRANS+16);
  else if (*cstate == 100)
    cadwindow->prompt->normalprompt(NCTRANS+19);
  else if (*cstate == 101)
    {  cadwindow->prompt->normalprompt(NCTRANS+20);
       rubberband.begin(0);
       rubberband.add(RBScale,rbdata);
    }
  else
    delete rbdata;
}


class CARadioButtonDialogControl : public RadioButtonDialogControl
{public:
   CARadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void CARadioButtonDialogControl::load(Dialog *dialog)
{int i;
  RadioButtonDialogControl::load(dialog);
  for (i = 102 ; i <= 105 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
    }
  for (i = 106 ; i <= 108 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
    }
}

int CARadioButtonDialogControl::select(Dialog *dialog)
{int i;
  RadioButtonDialogControl::select(dialog);
  for (i = 102 ; i <= 105 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
    }
  for (i = 106 ; i <= 108 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
    }
  return 0;
}

/**
 * @brief copyarray_command
 * @param cstate
 * @param event
 * @param data
 *
 * Copy array command
 *
 * KMJ : Working in Qt version
 *
 */
void copyarray_command(int *cstate,HighLevelEvent *event,void **data)
{Transform t;
 static EntityList list;
 static Point p1;
 DialogControl *dlgc;
 Dialog dialog("CopyArray_Dialog");
 Point p2;
 double dx,dy,dd,dc;
 int i,j,n,type,nx,ny,nc;

  switch (*cstate)
    {  case InitialState :
         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              break;
           }
         dx = v.getreal("ca::dx");
         dy = v.getreal("ca::dy");
         dd = v.getreal("ca::dd");
         dc = v.getreal("ca::dc");
         nx = v.getinteger("ca::nx");
         ny = v.getinteger("ca::ny");
         nc = v.getinteger("ca::nc");
         type = v.getinteger("ca::type");

         dialog.add(new CARadioButtonDialogControl(100,100,101,&type));
         dialog.add(new CARadioButtonDialogControl(101,100,101,&type));
         dialog.add(dlgc = new RealDialogControl(102,&dx,ScaleByLength));
         //dialog.add(new ScrollBarDialogControl(1102,dlgc));
         dialog.add(dlgc = new RealDialogControl(103,&dy,ScaleByLength));
         //dialog.add(new ScrollBarDialogControl(1103,dlgc));
         dialog.add(dlgc = new IntegerDialogControl(104,&nx,1,100000));
         //dialog.add(new ScrollBarDialogControl(1104,dlgc));
         dialog.add(dlgc = new IntegerDialogControl(105,&ny,1,100000));
         //dialog.add(new ScrollBarDialogControl(1105,dlgc));
         dialog.add(dlgc = new RealDialogControl(106,&dd,ScaleByLength));
         //dialog.add(new ScrollBarDialogControl(1106,dlgc));
         dialog.add(dlgc = new RealDialogControl(107,&dc,ScaleByLength));
         //dialog.add(new ScrollBarDialogControl(1107,dlgc));
         dialog.add(dlgc = new IntegerDialogControl(108,&nc));
         //dialog.add(new ScrollBarDialogControl(1108,dlgc));
         if (dialog.process() == TRUE)
           {  v.setinteger("ca::type",type);
              v.setreal("ca::dx",dx);
              v.setreal("ca::dy",dy);
              v.setreal("ca::dd",dd);
              v.setreal("ca::dc",dc);
              v.setinteger("ca::nx",nx);
              v.setinteger("ca::ny",ny);
              v.setinteger("ca::nc",nc);

              list = state.getselection().copy();

              if (type == 0)
                {Transform t;

                   db.saveundo(UD_STARTBLOCK,NULL);
                   cadwindow->startdrawingbatch();
                   for (i = 0 ; i < nx ; i++)
                     for (j = i == 0 ? 1 : 0 ; j < ny ; j++)
                       {  t.translate(db.workplanes.getcurrent()->getxaxis() * dx * i + db.workplanes.getcurrent()->getyaxis() * dy * j);
                          transformlist(0,1,list,t,0);
                       }
                   cadwindow->enddrawingbatch();
                   db.saveundo(UD_ENDBLOCK,NULL);

                   *cstate = ExitState;

                }
              else
                *cstate = 1;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *)event)->getp();
              db.saveundo(UD_STARTBLOCK,NULL);
              dd = v.getreal("ca::dd");
              dc = v.getreal("ca::dc");
              nc = v.getinteger("ca::nc");

              for (i = 1 ; i < nc ; i++)
                {  n = int(dd * i * 2.0 * acos(-1.0) / dc);

                   if (n > 3)
                     for (j = 0 ; j < n ; j++)
                       {Transform t1,t2;

                          t1.translate(-p1);
                          t2.rotate(Point(0.0,0.0,0.0),((Coordinate3dEvent *)event)->getxaxis().cross(((Coordinate3dEvent *)event)->getyaxis()),2.0*acos(-1.0)*j/n);
                          t1.apply(t2);
                          t2.translate(p2 + ((Coordinate3dEvent *)event)->getxaxis() * cos(2.0*acos(-1.0)*j/n) * dd * i + ((Coordinate3dEvent *)event)->getyaxis() * sin(2.0*acos(-1.0)*j/n) * dd * i);
                          t1.apply(t2);
                            
                          transformlist(0,1,list,t1,0);

                       }
                  }

              db.saveundo(UD_ENDBLOCK,NULL);

           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the selection");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the centre of the circular array");
    }
  else
    {EntityList elist;
       list.destroy();
       list = elist;
    }

}



class CALRadioButtonDialogControl : public RadioButtonDialogControl
{public:
   CALRadioButtonDialogControl(int i,int i1,int i2,int *v) :
      RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void CALRadioButtonDialogControl::load(Dialog *dialog)
{int i;
  RadioButtonDialogControl::load(dialog);
  for (i = 104 ; i <= 104 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
    }
  for (i = 105 ; i <= 105 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
    }
}

int CALRadioButtonDialogControl::select(Dialog *dialog)
{int i;
  RadioButtonDialogControl::select(dialog);
  for (i = 104 ; i <= 104 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),! oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),! oldvalue);
    }
  for (i = 105 ; i <= 105 ; i++)
    {  EnableWindow(GetDlgItem(dialog->gethdlg(),i),oldvalue);
       EnableWindow(GetDlgItem(dialog->gethdlg(),1000+i),oldvalue);
    }
  return 0;
}

/**
 * @brief copyalong_command
 * @param cstate
 * @param event
 * @param data
 *
 * Copy Along Command
 *
 * KMJ : Working in Qt version
 *
 */

void copyAlongCmdImplement(LinearEntity *entity, bool changeLocation);

void copyalong_command(int *cstate,HighLevelEvent *event,void **data)
{static EntityList list;
 static Point p1;
 Entity* e;
 Transform t,t1,t2;
 DialogControl *dc;
 Dialog dialog("CopyAlong_Dialog");
 int n,orientation,spacingmode;
 double x,length,spacing,u,ustart;
 LinearEntity *le;
 Point xaxis,yaxis,zaxis,xa,ya,p2;
 double x1,y1,z1;
 

  switch (*cstate)
    {  case InitialState :

         if (state.getselection().length() == 0)
           {ResourceString rs(NCMAINB+89);
              cadwindow->MessageBox(rs.gets(),"",MB_OK);
              *cstate = ExitState;
              break;
           }

         orientation = v.getinteger("cal::orientation");
         spacingmode = v.getinteger("cal::spacingmode");
         n = v.getinteger("cal::n");
         spacing = v.getreal("cal::spacing");

         dialog.add(new RadioButtonDialogControl(100,100,101,&orientation));
         dialog.add(new RadioButtonDialogControl(101,100,101,&orientation));
         dialog.add(new CALRadioButtonDialogControl(102,102,103,&spacingmode));
         dialog.add(new CALRadioButtonDialogControl(103,102,103,&spacingmode));
         dialog.add(dc = new RealDialogControl(104,&spacing,1.0,1.0e-5,1e38,ScaleByLength));
         //dialog.add(new ScrollBarDialogControl(1104,dc));
         dialog.add(dc = new IntegerDialogControl(105,&n));
         //dialog.add(new ScrollBarDialogControl(1105,dc));

         if (dialog.process() == TRUE)
           {  v.setinteger("cal::orientation",orientation);
              v.setreal("cal::spacing",spacing);
              v.setinteger("cal::n",n);
              v.setinteger("cal::spacingmode",spacingmode);

              list = state.getselection().copy();

              *cstate = 1;
 
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2; 
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         if (event->isa(EntitySelected))
           {  le = (LinearEntity *) ((EntitySelectedEvent *)event)->getentity();
              length = le->length();
			  
			  orientation = v.getinteger("cal::orientation");
			  spacingmode = v.getinteger("cal::spacingmode");
			  n = v.getinteger("cal::n");
			  spacing = v.getreal("cal::spacing");
			
              db.saveundo(UD_STARTBLOCK,NULL);

			  if( (((EntitySelectedEvent *)event)->getendselected() == 2) && 
				  (le->end() - p1).length() < (le->start() - p1).length() ) 				
				  ustart = 1.0;			  
              else                
                ustart = 0.0;
			  db.CopyAlogEntityCallback = &copyAlongCmdImplement;
			  if (!db.copyAlogMap.contains(le->getid())) {
				  copyAlongDialogData dlgData;
				  dlgData.n = n;
				  dlgData.orientation = orientation;
				  dlgData.spacingMode = spacingmode;
				  dlgData.spacing = spacing;
				  dlgData.pt = p1;
				  dlgData.ustart = ustart;
				  for (list.start(); (e = list.next()) != NULL; ) {
					  if (e->getRefId() < 0)
						e->setRefId(e->getid());
					  dlgData.list.append(e->getid());
				  }
				  if(le->getRefId()<0)
					 le->setRefId(le->getid());
				  db.copyAlogMap[le->getid()] = dlgData;
				  copyAlongCmdImplement(le, false);
			  }
			  else
				  cadwindow->MessageBox("You can't attache this entity one more time!", "Copy Along", MB_ICONSTOP);
             
              db.saveundo(UD_ENDBLOCK,NULL);
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
     }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the selection");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the entity to copy along");
    }
  else
    {EntityList elist;
       list.destroy();
       list = elist;
    }

}

//
void copyAlongCmdImplement(LinearEntity *entity,bool changeLocation)
{
	if (entity->getid() == -1) return;
	if (entity->getRefId() == -1) return;
	
	copyAlongDialogData dlgData;
	long entityId = entity->getid();
	
	if (changeLocation) {	
		cadwindow->startdrawingbatch();		
		db.geometry.clearstatus();
		db.saveundo(UD_STARTBLOCK, NULL);
		
		if (db.copyAlogMap.contains(entityId))
			dlgData = db.copyAlogMap[entityId];
		else if (db.copyAlogMap.contains(entity->getRefId())) {
			dlgData = db.copyAlogMap[entity->getRefId()];
			db.copyAlogMap[entityId] = dlgData;
			if (entityId != entity->getRefId()) {
				//db.copyAlogMap.remove(entity->getRefId());
				entity->setRefId(entityId);
			}
		}
		else {
			cadwindow->MessageBox("Path entity is not fond!", "Copy Along", MB_ICONINFORMATION);
			cadwindow->enddrawingbatch();
			return;// something wrong happened.
		}
		db.copyAlogMap[entityId].level++;
	}
	else {
		dlgData = db.copyAlogMap[entity->getid()];
		db.copyAlogMap[entityId].level = 1;
		db.copyAlogMap[entityId].undoMap[1].pt= dlgData.pt;
	}
	int level = db.copyAlogMap[entityId].level;
	
	if (entity->isa(circle_entity)) {
		Circle* c = (Circle*)entity;

		int l = changeLocation ? level - 1 : level;
		Point pt = db.copyAlogMap[entityId].undoMap[l].pt;
		Point dir = (pt - c->getorigin()).normalize();
		Point newPt = c->getorigin() + dir * c->getradius();
		db.copyAlogMap[entityId].pt = newPt;
		db.copyAlogMap[entityId].dir = newPt - pt;
		db.copyAlogMap[entityId].undoMap[level].pt = newPt;
		dlgData.pt = newPt;
		Transform t;
		Entity *e = 0;
		for (int i = 0; i < dlgData.list.length(); i++) {
			e = copyAlongDialogData::getEntity(dlgData.list[i]);
			if (e != nullptr)
				e->transform(t.translate(newPt - pt), 0);
		}

	}
	else
		db.copyAlogMap[entityId].undoMap[level].pt = dlgData.pt;
	EntityList list;
	for (int i = 0; i < dlgData.list.length(); i++) {
		if (changeLocation) {
			Entity* e = copyAlongDialogData::getEntity(dlgData.list[i]);
			if (e != nullptr) {
				db.copyAlogMap[entityId].list[i] = e->getid();
				e->setRefId(e->getid());
				list.add(e);
			}
		}
		else
			list.add(db.geometry.getentity(dlgData.list[i]));
	}
	if (list.length() == 0) {
		cadwindow->enddrawingbatch();
		return;
	}
	double ustart = dlgData.ustart;
	Point p1 = dlgData.pt;

	Point xaxis = entity->direction(ustart);
	Point zaxis = xaxis.cross(p1 - entity->position(ustart));
	if (zaxis.length() < 1.0E-5)
		zaxis.setxyz(0.0, 0.0, 1.0);
	Point yaxis = zaxis.cross(xaxis);
	xaxis.normalize();
	yaxis.normalize();
	zaxis.normalize();

	double x1 = xaxis.dot(p1 - entity->position(ustart));
	double y1 = yaxis.dot(p1 - entity->position(ustart));
	double z1 = zaxis.dot(p1 - entity->position(ustart));
	int spacingmode = dlgData.spacingMode;
	int n = dlgData.n;
	double spacing = dlgData.spacing;
	if (spacingmode == 1)
		spacing = entity->length() / n;
	int orientation =dlgData.orientation;
	EntityList copyList;
	
	db.copyAlogMap[entityId].undoMap[level].undoList.clear();
	for (int i = 0; i < dlgData.undoMap[level-1].copyList.length(); i++) {
		Entity* e = 0;
		long id = dlgData.undoMap[level-1].copyList[i];		
		e=copyAlongDialogData::getEntity(id);			
		if (e != nullptr) {
			copyList.add(e);
			db.copyAlogMap[entityId].undoMap[level].undoList.append(id);
		}	
		//copy/paste can cause e being null. In such a case ,the copyList will be recreated later
	}
	
	db.copyAlogMap[entityId].undoMap[level].copyList.clear();
	
	for (double x = spacing; x <= entity->length() + spacing / 1.0E10; x += spacing)
	{
		Transform t1, t2;
		t1.translate(-p1);
		double u;
		entity->positionbylength(x, &u);
		if (ustart != 0.0)
			u = 1.0 - u;
		Point xa = entity->direction(u);
		Point ya = zaxis.cross(xa);
		xa.normalize();
		ya.normalize();

		if (orientation == 1)
		{
			t2.fromcoordinatesystem(xaxis, yaxis, zaxis);
			t1.apply(t2);
			t2.tocoordinatesystem(xa, ya, zaxis);
			t1.apply(t2);
			t2.translate(entity->position(u) + xa * x1 + ya * y1 + zaxis * z1);
		}
		else
			t2.translate(entity->position(u));

		t1.apply(t2);	
		
		transformCopyAlogList(entityId,level, list, t1);
		
	}
	
	if (changeLocation ) {
		//delete previous copyied entites
		if (copyList.length() > 0) {
			state.setselection(copyList, 1);
			//this setting is special for copy along to make diffrent from
			//other delete cases.
			int cstate = -100;
			delete_command(&cstate, 0, 0);
		}
		
		db.saveundo(UD_ENDBLOCK, NULL);		
		cadwindow->enddrawingbatch();
	}
}
/**
 * @brief Nudge entities by grid spacing in given direction
 * @param list - should be currently selected
 * @param dir - must be VK_<dir> hex value from qtdef.h that matches an arrow key
 */
bool nudgelist(EntityList list, UINT dir) {
    Transform t;
    if (list.length() <= 0) {return false;}
    if (dir == VK_LEFT) {
        db.saveundo(UD_STARTBLOCK,NULL);
        cadwindow->startdrawingbatch();
        int gridx = v.getreal("gd::xspacing");
        t.translate(-db.workplanes.getcurrent()->getxaxis() * gridx);
        transformlist(0,0,list,t,0);
        cadwindow->enddrawingbatch();
        db.saveundo(UD_ENDBLOCK,NULL);
    }
    else if (dir == VK_RIGHT) {
        db.saveundo(UD_STARTBLOCK,NULL);
        cadwindow->startdrawingbatch();
        int gridx = v.getreal("gd::xspacing");
        t.translate(db.workplanes.getcurrent()->getxaxis() * gridx);
        transformlist(0,0,list,t,0);
        cadwindow->enddrawingbatch();
        db.saveundo(UD_ENDBLOCK,NULL);
    }
    else if (dir == VK_UP) {
        db.saveundo(UD_STARTBLOCK,NULL);
        cadwindow->startdrawingbatch();
        int gridy = v.getreal("gd::yspacing");
        t.translate(db.workplanes.getcurrent()->getyaxis() * gridy);
        transformlist(0,0,list,t,0);
        cadwindow->enddrawingbatch();
        db.saveundo(UD_ENDBLOCK,NULL);
    }
    else if (dir == VK_DOWN) {
        db.saveundo(UD_STARTBLOCK,NULL);
        cadwindow->startdrawingbatch();
        int gridy = v.getreal("gd::yspacing");
        t.translate(-db.workplanes.getcurrent()->getyaxis() * gridy);
        transformlist(0,0,list,t,0);
        cadwindow->enddrawingbatch();
        db.saveundo(UD_ENDBLOCK,NULL);
    }
    else {
        return false;
    }
    return true;
}



