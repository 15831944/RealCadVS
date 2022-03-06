
#include "ncwin.h"
#include "RCView.h"

void line_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Line"));
  *cstate = ExitState;
}

struct RBLine1_data
{
   Entity *e;
   Point   p1,xaxis,yaxis,zaxis,offset;
   double angle,lasttextheight;
   int shiftstate;
   GeneralList list;
#ifdef USING_WIDGETS
   View3dSurface *lastv;
   qreal zValue;
   QGraphicsItem *qgroup;
   QGraphicsItem *qgline;
   QGraphicsItem *qglinelast;
   QGraphicsItem *qgtext;
#endif
};

void StartGLRB(View3dSurface *window)
{
    View *view;
    Entity *e;
    Point p1,p2;
    Point eye,pmin,pmax;

    view = window->getview();

    OpenOpenGl(((RCView*)window->gethwnd())->view(),3,0,0,((RCView*)window->gethwnd())->viewport()->width(),
                                                 ((RCView*)window->gethwnd())->viewport()->height());

    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
    BeginScene(BS_NOCLEAR/*| BS_FRONTBUFFERONLY */);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glClear( GL_DEPTH_BUFFER_BIT );
    glFinish();
}

void EndGLRB()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glFinish();

    //glPopMatrix();

    EndScene();
}


#ifdef NEW_RUBBERB
void RBLine1(int dm,void *data,Point *p2,View3dSurface *v)
{
 RBLine1_data *rbdata = (RBLine1_data *) data;
 Point p3,p4,offset;
 double l,angle;
 BitMask toptions(32);
 RCCHAR text[300];
 QGraphicsItemGroup * qgroup=0;

 //qDebug() << "RBLine1 : rbdata         " << rbdata;
 //qDebug() << "RBLine1 : rbdata->qgline " << rbdata->qgline;
 //qDebug() << "RBLine1 v : " << v;
 //if (v != 0)
 //    qDebug() << "RBLine1 scene : " << v->getScene();

  rbdata->qgline = rbdata->qglinelast;

  if (dm == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if (v != 0)
  {
       rbdata->lasttextheight = 6.0 * v->getscale();
       rbdata->offset = v->getuaxis() * 6.0 * v->getscale() +  v->getvaxis() * 6.0 * v->getscale();
       rbdata->lastv = v;
  }


  if (dm == RB_ERASE && rbdata->shiftstate ||
      dm != RB_ERASE && cadwindow->getshiftstate())
  {
       rbdata->shiftstate = 1;
       if (rbdata->e == 0)
       {  p3 = rbdata->p1;
            p4.x = rbdata->xaxis.dot(*p2 - p3);
            p4.y = rbdata->yaxis.dot(*p2 - p3);
            if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
            p4.x /= l;  p4.y /= l;
            if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
            angle = acos(p4.x);
            if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
            if (fabs(rbdata->angle) > 1.0E-20)
              angle = floor(angle / rbdata->angle + 0.5) * rbdata->angle;
            p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                      rbdata->yaxis * (l * sin(angle));
            Line line1(p3,p4);
            Line line(p3,line1.nearp(*p2));

            line.draw(DM_INVERT);
            //line.drawGL(DM_INVERT);

            strcpy(text,db.formatnumber(text,line.length(),0));
            Text texte(CHAR2RCCHR("Arial"),p4+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
            if (::v.getinteger("wn::distancetracking"))
            {
                texte.draw(DM_INVERT);
            }
       }
       else
       {
            Line line1(rbdata->xaxis*cos(rbdata->angle)+rbdata->yaxis*sin(rbdata->angle),
                       rbdata->e,rbdata->p1,NULL,*p2,0.0);
            Line line2(rbdata->xaxis*cos(-rbdata->angle)+rbdata->yaxis*sin(-rbdata->angle),
                       rbdata->e,rbdata->p1,NULL,*p2,0.0);
            if (line1.getdefined() && line2.getdefined() &&
                (line1.getp2() - rbdata->p1).length() < (line2.getp2() - rbdata->p1).length())
              {

                line1.draw(DM_INVERT);
                //line1.drawGL(DM_INVERT);

                strcpy(text,db.formatnumber(text,line1.length(),0));
                 Text _text(_RCT("Arial"),line1.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.draw(DM_INVERT);
                 }
              }
            else
              {
                 line2.draw(DM_INVERT);
                 //line2.drawGL(DM_INVERT);

                 strcpy(text,db.formatnumber(text,line2.length(),0));
                 Text _text(_RCT("Arial"),line2.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.draw(DM_INVERT);
                 }
              }
        }
  }
  //if (dm == RB_ERASE)
  //{
  //    cadwindow->clearAllRubberBands();
  //    rbdata->qgline = 0;
  //    rbdata->qgtext = 0;
  //}
  //else
  //{
       rbdata->shiftstate = 0;
       if (rbdata->e == NULL)
       {
            Line line(rbdata->p1,*p2);
            if (line.getdefined())
            {

                 line.draw(DM_INVERT);
                 //line.drawGL(DM_INVERT);

                 strcpy(text,db.formatnumber(text,line.length(),0));
                 Text _text(_RCT("Arial"),line.getp2()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.draw(DM_INVERT);
                 }
            }
       }
       else
       {
            Line line(rbdata->e,rbdata->p1,*p2);
            if (line.getdefined())
            {
                line.draw(DM_INVERT);
                //line.drawGL(DM_INVERT);

                 strcpy(text,db.formatnumber(text,line.length(),0));
                 Text _text(_RCT("Arial"),line.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.draw(DM_INVERT);
                 }
            }
       }
    //}

    rbdata->qglinelast = rbdata->qgline;
}
#else
void RBLine1(int dm,void *data,Point *p2,View3dSurface *v)
{
 RBLine1_data *rbdata = (RBLine1_data *) data;
 Point p3,p4,offset;
 double l,angle;
 BitMask toptions(32);
 RCCHAR text[300];
 QGraphicsItemGroup * qgroup=0;

 //qDebug() << "RBLine1 : rbdata         " << rbdata;
 //qDebug() << "RBLine1 : rbdata->qgline " << rbdata->qgline;
 qDebug() << "RBLine1 v : " << v;
 if (v != 0)
     qDebug() << "RBLine1 scene : " << v->getScene();

  rbdata->qgline = rbdata->qglinelast;

  if (v != 0)
  {
       rbdata->lasttextheight = 6.0 * v->getscale();
       rbdata->offset = v->getuaxis() * 6.0 * v->getscale() +  v->getvaxis() * 6.0 * v->getscale();
       rbdata->lastv = v;
  }
 
  if (dm == RB_ERASE && rbdata->shiftstate ||
      dm != RB_ERASE && cadwindow->getshiftstate())
  {
       rbdata->shiftstate = 1;
       if (rbdata->e == 0)
       {  p3 = rbdata->p1;
            p4.x = rbdata->xaxis.dot(*p2 - p3);
            p4.y = rbdata->yaxis.dot(*p2 - p3);
            if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) < db.getptoler()) return;
            p4.x /= l;  p4.y /= l;
            if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
            angle = acos(p4.x);
            if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
            if (fabs(rbdata->angle) > 1.0E-20)
              angle = floor(angle / rbdata->angle + 0.5) * rbdata->angle;
            p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                      rbdata->yaxis * (l * sin(angle));
            Line line1(p3,p4);
            Line line(p3,line1.nearp(*p2));

                line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                line.draw(DM_INVERT);
                rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

            strcpy(text,db.formatnumber(text,line.length(),0));
            Text texte(CHAR2RCCHR("Arial"),p4+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
            if (::v.getinteger("wn::distancetracking"))
            {
                texte.qgi = rbdata->qgtext; texte.zValue = rbdata->zValue;
                texte.draw(DM_INVERT);
                rbdata->qgtext = texte.qgi; rbdata->zValue = texte.zValue;
            }
       }
       else
       {
            Line line1(rbdata->xaxis*cos(rbdata->angle)+rbdata->yaxis*sin(rbdata->angle),
                       rbdata->e,rbdata->p1,NULL,*p2,0.0);
            Line line2(rbdata->xaxis*cos(-rbdata->angle)+rbdata->yaxis*sin(-rbdata->angle),
                       rbdata->e,rbdata->p1,NULL,*p2,0.0);
            if (line1.getdefined() && line2.getdefined() &&
                (line1.getp2() - rbdata->p1).length() < (line2.getp2() - rbdata->p1).length())
              {

                line1.qgi = rbdata->qgline; line1.zValue = rbdata->zValue;
                line1.draw(DM_INVERT);
                rbdata->qgline = line1.qgi; rbdata->zValue = line1.zValue;

                strcpy(text,db.formatnumber(text,line1.length(),0));
                 Text _text(_RCT("Arial"),line1.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.qgi = rbdata->qgtext; _text.zValue = rbdata->zValue;
                     _text.draw(DM_INVERT);
                     rbdata->qgtext = _text.qgi; rbdata->zValue = _text.zValue;
                 }
              }
            else
              {
                line2.qgi = rbdata->qgline; line2.zValue = rbdata->zValue;
                line2.draw(DM_INVERT);
                rbdata->qgline = line2.qgi; rbdata->zValue = line2.zValue;

                strcpy(text,db.formatnumber(text,line2.length(),0));
                 Text _text(_RCT("Arial"),line2.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.qgi = rbdata->qgtext; _text.zValue = rbdata->zValue;
                     _text.draw(DM_INVERT);
                     rbdata->qgtext = _text.qgi; rbdata->zValue = _text.zValue;
                 }
              }
        }
  }
  if (dm == RB_ERASE)
  {
          delete rbdata->qgline;
          rbdata->qgline = 0;

          delete rbdata->qgtext;
          rbdata->qgtext = 0;

          rbdata->lastv->qgirb=0;
          rbdata->qgroup=0;
  }
  else
  {
       rbdata->shiftstate = 0;
       if (rbdata->e == NULL)
       {
            Line line(rbdata->p1,*p2);
            if (line.getdefined()) 
            {

                 line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                 line.draw(DM_INVERT);
                 rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

                 strcpy(text,db.formatnumber(text,line.length(),0));
                 Text _text(_RCT("Arial"),line.getp2()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.qgi = rbdata->qgtext; _text.zValue = rbdata->zValue;
                     _text.draw(DM_INVERT);
                     rbdata->qgtext = _text.qgi; rbdata->zValue = _text.zValue;
                 }
            }
       }
       else
       {
            Line line(rbdata->e,rbdata->p1,*p2);
            if (line.getdefined()) 
            {
                line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                line.draw(DM_INVERT);
                rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

                 strcpy(text,db.formatnumber(text,line.length(),0));
                 Text _text(_RCT("Arial"),line.getp1()+rbdata->offset,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,text,0,toptions);
                 if (::v.getinteger("wn::distancetracking"))
                 {
                     _text.qgi = rbdata->qgtext;_text.zValue = rbdata->zValue;
                     _text.draw(DM_INVERT);
                     rbdata->qgtext = _text.qgi; rbdata->zValue = _text.zValue;
                 }
            }
       }
    }

    rbdata->qglinelast = rbdata->qgline;
}
#endif

/**
 * @brief line_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Line command
 *
 */
void line_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine1_data *rbdata = (RBLine1_data *) *data;
 Point  p1,p2,p3,p4;
 Entity *e;
 Line *line,*line1,*line2;
 double l,angle;

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBLine1_data;  *data = rbdata;
         rbdata->qgline=0; rbdata->qgtext=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         rbdata->qglinelast=0;rbdata->qgroup=0;
         rbdata->lastv = 0;
         cadwindow->clearAllRubberBands();
         if (rbdata != 0)
           {  rbdata->angle = v.getreal("ln::autosnapangle") * M_PI / 180.0;
              rbdata->angle = 0.0;  //  Disable the constrained angle
              *cstate = 1;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->e = NULL;
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              rbdata->list.add((void*)1);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(0);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              rbdata->list.add((void*)1);
              *cstate = 2;
           }
         else if (event->isa(NewCommand))
           {  rbdata->list.start();
              *cstate = (LONG)rbdata->list.next();
              rbdata->list.del((void*)*cstate);
              if (*cstate == 0)
                {  *cstate = 1;
                   break;
                }
              else if (*cstate == 2)
                {  db.geometry.start();
                   line = (Line *) db.geometry.next();
                   rbdata->p1 = line->getp2();
                   state.setposition(rbdata->p1);
                   rbdata->list.add((void*)2);
                   break;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
         {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline; rbdata->qgline=0;
             rbdata->qglinelast=0;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             delete rbdata->qgtext; rbdata->qgtext=0;
#endif
#endif
             *cstate = ExitState;
         }
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              if (cadwindow->getshiftstate())
                {  if (rbdata->e == 0)
                     {  p3 = rbdata->p1;
                        p4.x = rbdata->xaxis.dot(p2 - p3);
                        p4.y = rbdata->yaxis.dot(p2 - p3);
                        if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                          {  p4.x /= l;  p4.y /= l;
                             if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                             angle = acos(p4.x);
                             if (p4.y < 0.0) angle = 2.0 * M_PI - angle;
                             if (fabs(rbdata->angle) > 1.0E-10)
                               angle = floor(angle / rbdata->angle + 0.5) * rbdata->angle;
                             p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                                       rbdata->yaxis * (l * sin(angle));
                             line = new Line(p3,p4);
                             p2 = p4;
                          }
                        else
                          line = 0;
                     }
                   else
                     {  rbdata->e->unhighlight();
                        line1 = new Line(rbdata->xaxis*cos(rbdata->angle)+rbdata->yaxis*sin(rbdata->angle),
                                         rbdata->e,rbdata->p1,0,p2,0.0);
                        line2 = new Line(rbdata->xaxis*cos(-rbdata->angle)+rbdata->yaxis*sin(-rbdata->angle),
                                         rbdata->e,rbdata->p1,0,p2,0.0);
                        if (line1 != 0 && line2 != 0)
                          {  if ((line1->getp2() - rbdata->p1).length() < (line2->getp2() - rbdata->p1).length())
                               {  line = line1;  delete line2;
                               }
                             else
                               {  line = line2;  delete line1;
                               }
                          }
                        else
                          line = 0;
                        if (line != 0) p2 = line->getp1();
                     }
                   if (line != 0 && line->getdefined())
                     {
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        rbdata->list.add((void*)2);
                     }
                   rbdata->p1 = p2;
                   rbdata->e = NULL;
                }
              else
                {  if (rbdata->e != NULL)
                     {  rbdata->e->unhighlight();
                        line = new Line(rbdata->e,rbdata->p1,p2);
                     }
                   else
                     line = new Line(rbdata->p1,p2);
                   if (line != 0 && line->getdefined())
                     {
#ifdef USING_WIDGETS
                        line->qgi = rbdata->qgline;
                        line->zValue = rbdata->zValue;
#endif
                        db.geometry.add(line);
                        rbdata->list.add((void*)2);
                     }
                   rbdata->p1 = p2;
                   rbdata->e = NULL;
#ifdef USING_WIDGETS
                   if(rbdata->lastv != 0)rbdata->lastv->qgirb=0;
                   rbdata->qgroup=0;
                   rbdata->qgline = 0;
                   rbdata->qglinelast = 0;
#endif
                }
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              if (cadwindow->getshiftstate())
                {  p1 = ((EntitySelectedEvent *) event)->getp();
                   p1 = db.grid.snappoint(((EntitySelectedEvent *) event)->getwindow(),p1);
                   Line *line1 = new Line(rbdata->xaxis*cos(rbdata->angle)+rbdata->yaxis*sin(rbdata->angle),
                                          rbdata->e,rbdata->p1,e,p1,0.0);
                   Line *line2 = new Line(rbdata->xaxis*cos(-rbdata->angle)+rbdata->yaxis*sin(-rbdata->angle),
                                          rbdata->e,rbdata->p1,e,p1,0.0);
                   if (line1 != 0 && line2 != 0)
                     {  if ((line1->getp2() - p1).length() + (line1->getp1() - rbdata->p1).length() <
                            (line2->getp2() - p1).length() + (line2->getp1() - rbdata->p1).length())
                          {  line = line1;  delete line2;
                          }
                        else
                          {  line = line2;  delete line1;
                          }
                        if (line->getdefined())
                          {
                             line->zValue = rbdata->zValue;
                             db.geometry.add(line);
                             rbdata->angle = fabs(rbdata->angle);
                             rbdata->p1 = line->getp2();
                             rbdata->list.add((void*)2);
                          }
                        else
                          delete line;
                     }
                   if (rbdata->e != NULL) rbdata->e->unhighlight();
                   rbdata->e = 0;
                }
              else
                {  if (rbdata->e == NULL)
                     line = new Line(e,((EntitySelectedEvent *)event)->getp(),rbdata->p1);
                   else
                     line = new Line(rbdata->e,rbdata->p1,e,((EntitySelectedEvent *) event)->getp());
                   if (line->getdefined())
                     {
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        rbdata->p1 = line->getp2();
                        rbdata->list.add((void*)2);
                     }
                   if (rbdata->e != NULL) rbdata->e->unhighlight();
                   rbdata->e = 0;
                }
           }
         else if (event->isa(Abort))
           {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline; rbdata->qgline=0;
             rbdata->qglinelast=0;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             delete rbdata->qgtext; rbdata->qgtext=0;
#endif
#endif
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
             rbdata->qgroup=0;
             rbdata->qglinelast=0;
             rbdata->lastv=0;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             delete rbdata->qgtext;rbdata->qgtext=0;
#endif
#endif
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = ExitState;
           }
         else if (event->isa(NewCommand))
           {  rbdata->list.start();
              *cstate = (LONG)rbdata->list.next();
              rbdata->list.del((void*)*cstate);
              if (*cstate == 0)
                *cstate = 1;
              else if (*cstate == 1)
                {  rbdata->list.start();
                   if ((LONG)rbdata->list.next() == 2)
                     {  db.geometry.start();
                        line = (Line *) db.geometry.next();
                        rbdata->p1 = line->getp2();
                        state.setposition(rbdata->p1);
                        *cstate = 2;
                     }
                }
              else if (*cstate == 2)
                {  db.geometry.start();
                   line = (Line *) db.geometry.next();
                   rbdata->p1 = line->getp1();
                   state.setposition(rbdata->p1);
                   db.geometry.del(line);
                }
           }
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,curve_entity,undo_mask,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBLine1,rbdata);
       cadwindow->prompt->normalprompt(NCLINEB+1);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0) rbdata->list.destroy();
       delete rbdata;
    }
}

struct RBLine2_data
{
#ifdef USING_WIDGETS
   qreal zValue;
   QGraphicsItem *qgline;
   QGraphicsItem *qgtext;
#endif

   Entity *e;
   Point p,xaxis,yaxis,zaxis;
   double lasttextheight;
};

#ifdef NEW_RUBBERB
void RBLine2(int mode,void *data,Point *p2,View3dSurface *v)
{
 RBLine2_data *rbdata = (RBLine2_data *) data;
 Point p3,p4,p5;
 int l,c,s,w;
 RCCHAR text[300];
 BitMask toptions(32);

  l = db.header.getlayer();
  c = db.header.getcolour();
  s = db.header.getstyle();
  w = db.header.getweight();

  if (v != 0)
    rbdata->lasttextheight = 6.0 * v->getscale();

  if (rbdata->e == NULL)
  {
       p3 = rbdata->p;
       p4 = rbdata->xaxis * (rbdata->xaxis.dot(*p2)) +
            rbdata->yaxis * (rbdata->yaxis.dot(p3)) +
            rbdata->zaxis * (rbdata->zaxis.dot(*p2));
  }
  else
  {
       p3 = *p2;
       Line l(p3,p3+rbdata->xaxis);
       Intersect i(rbdata->e,&l,rbdata->p);
       if (i.nintersections() == 1)
         {  p3 = ((PointE *)i.intersection(0))->getp();
            p4 = *p2;
         }
       else if (rbdata->e->isa(circle_entity))
         {  Line line(rbdata->xaxis,rbdata->e,rbdata->p,NULL,*p2,0.0);
            p3 = line.getp1();
            p4 = line.getp2();
         }
       else
         p4 = p3 + rbdata->xaxis * 20.0;
       if (db.grid.getsnap() == 0  && s == 0)
         s = 1;
  }

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
  }
  else
  {
  Line line(p3,p4);
  line.EntityHeader::change(c,l,s,w);

  //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
  line.draw(DM_INVERT);
  //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

  strcpy(text,db.formatnumber(text,line.length(),0));
  Text rtext(CHAR2RCCHR("Arial"),Point(line.getp2()),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);
  if (::v.getinteger("wn::distancetracking"))
  {
      //rtext.qgi = rbdata->qgtext; rtext.zValue = rbdata->zValue;
      rtext.draw(DM_INVERT);
      //rbdata->qgtext = rtext.qgi; rbdata->zValue = rtext.zValue;
  }
  }

}
#else
void RBLine2(int,void *data,Point *p2,View3dSurface *v)
{
 RBLine2_data *rbdata = (RBLine2_data *) data;
 Point p3,p4,p5;
 int l,c,s,w;
 RCCHAR text[300];
 BitMask toptions(32);

  l = db.header.getlayer();
  c = db.header.getcolour();
  s = db.header.getstyle();
  w = db.header.getweight();

  if (v != 0)
    rbdata->lasttextheight = 6.0 * v->getscale();

  if (rbdata->e == NULL)
  {
       p3 = rbdata->p;
       p4 = rbdata->xaxis * (rbdata->xaxis.dot(*p2)) +
            rbdata->yaxis * (rbdata->yaxis.dot(p3)) +
            rbdata->zaxis * (rbdata->zaxis.dot(*p2));
  }
  else
  {
       p3 = *p2;
       Line l(p3,p3+rbdata->xaxis);
       Intersect i(rbdata->e,&l,rbdata->p);
       if (i.nintersections() == 1)
         {  p3 = ((PointE *)i.intersection(0))->getp();
            p4 = *p2;
         }
       else if (rbdata->e->isa(circle_entity))
         {  Line line(rbdata->xaxis,rbdata->e,rbdata->p,NULL,*p2,0.0);
            p3 = line.getp1();
            p4 = line.getp2();
         }
       else
         p4 = p3 + rbdata->xaxis * 20.0;
       if (db.grid.getsnap() == 0  && s == 0)
         s = 1;
  }

  Line line(p3,p4);
  line.EntityHeader::change(c,l,s,w);

  line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;

  line.draw(DM_INVERT);

  rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

  strcpy(text,db.formatnumber(text,line.length(),0));
  Text rtext(CHAR2RCCHR("Arial"),Point(line.getp2()),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);
  if (::v.getinteger("wn::distancetracking"))
  {
      rtext.qgi = rbdata->qgtext; rtext.zValue = rbdata->zValue;

      rtext.draw(DM_INVERT);

      rbdata->qgtext = rtext.qgi; rbdata->zValue = rtext.zValue;
  }
}
#endif

/**
 * @brief line_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for inserting horizontal lines
 *
 */
void line_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine2_data *rbdata = (RBLine2_data *) *data;
 View3dSurface *surface;
 Point  p1,p2,p3;
 Entity *e;

  switch (*cstate)
  {
       case InitialState :
         rbdata = new RBLine2_data;  *data = rbdata;
         rbdata->qgline=0; rbdata->qgtext=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->e = NULL;
              rbdata->p = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p = ((EntitySelectedEvent *) event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              rbdata->e->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;

         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
         {
             if (rbdata->e == NULL)
                {  p1 = rbdata->p;
                   p2 = ((Coordinate3dEvent *) event)->getp();
                   p2 = rbdata->xaxis * (rbdata->xaxis.dot(p2)) +
                        rbdata->yaxis * (rbdata->yaxis.dot(p1)) +
                        rbdata->zaxis * (rbdata->zaxis.dot(p2));
                   Line *line = new Line(p1,p2);
                   line->zValue = rbdata->zValue;
                   db.geometry.add(line);
                   *cstate = 1;
                }
              else
                {  p1 = ((Coordinate3dEvent *) event)->getp();
                   Line l(p1,p1+rbdata->xaxis);
                   Intersect i(rbdata->e,&l,rbdata->p);
                   if (i.nintersections() == 1)
                     {  p2 = ((PointE *)i.intersection(0))->getp();
                        Line *line = new Line(p1,p2);
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                   else
                     {  Line *line = new Line(rbdata->xaxis,rbdata->e,rbdata->p,NULL,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                          db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                }
         }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              if (rbdata->e == NULL)
                {  e = ((EntitySelectedEvent *) event)->getentity();
                   p1 = ((EntitySelectedEvent *) event)->getp();
                   Line l(rbdata->p,rbdata->p+rbdata->xaxis);
                   Intersect i(e,&l,p1);
                   if (i.nintersections() == 1)
                     {  p1 = rbdata->p;
                        p2 = ((PointE *)i.intersection(0))->getp();
                        Line *line = new Line(p1,p2);
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        *cstate = 1;
                     }
                   else
                     {  Line *line = new Line(rbdata->xaxis,rbdata->e,rbdata->p,e,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                            db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                     }
                }
              else
                {  e = ((EntitySelectedEvent *) event)->getentity();
                   p1 = p3 = ((EntitySelectedEvent *) event)->getp();
                   if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                     {  if ((surface = ((EntitySelectedEvent *) event)->getwindow()) != NULL)
                          {  p1 = surface->modeltouv(p1);
                             p1 = db.grid.snappoint(p1);
                             p1 = surface->uvtomodel(p1);
                          }
                     }
                   else
                     {  p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
                        p1 = db.grid.snappoint(p1);
                        p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
                     }
                   Line l(p1,p1+rbdata->xaxis);
                   Intersect i1(rbdata->e,&l,rbdata->p);
                   Intersect i2(e,&l,p3);
                   if (i1.nintersections() == 1 && i2.nintersections() == 1)
                     {  Line *line = new Line(((PointE *)i1.intersection(0))->getp(),((PointE *)i2.intersection(0))->getp());
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        rbdata->e->unhighlight();
                        *cstate = 1;
                     }
                   else
                     {  Line *line = new Line(rbdata->xaxis,rbdata->e,rbdata->p,e,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                            db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                }
           }
         else if (event->isa(Abort))
           {
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = ExitState;
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             delete rbdata->qgtext;rbdata->qgtext=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+2);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBLine2,rbdata);
       cadwindow->prompt->normalprompt(NCLINEB+3);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

#ifdef NEW_RUBBERB
void RBLine3(int mode,void *data,Point *p2,View3dSurface *v)
{
 RBLine2_data *rbdata = (RBLine2_data *) data;
 Point p3,p4,p5;
 int l,c,s,w;
 RCCHAR text[300];
 BitMask toptions(32);

  if (v != 0)
    rbdata->lasttextheight = 6.0 * v->getscale();

  l = db.header.getlayer();
  c = db.header.getcolour();
  s = db.header.getstyle();
  w = db.header.getweight();
  if (rbdata->e == NULL)
    {  p3 = rbdata->p;
       p4 = rbdata->xaxis * (rbdata->xaxis.dot(p3)) +
            rbdata->yaxis * (rbdata->yaxis.dot(*p2)) +
            rbdata->zaxis * (rbdata->zaxis.dot(*p2));
    }
  else
    {  p3 = *p2;
       Line l(p3,p3+rbdata->yaxis);
       Intersect i(rbdata->e,&l,rbdata->p);
       if (i.nintersections() == 1)
         {  p3 = ((PointE *)i.intersection(0))->getp();
            p4 = *p2;
         }
       else if (rbdata->e->isa(circle_entity))
         {  Line line(rbdata->yaxis,rbdata->e,rbdata->p,NULL,*p2,0.0);
            p3 = line.getp1();
            p4 = line.getp2();
         }
       else
         p4 = p3 + rbdata->yaxis * 20.0;
       if (db.grid.getsnap() == 0 && s == 0)
         s = 1;
    }

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
  }
  else
  {
      Line line(p3,p4);
      line.EntityHeader::change(c,l,s,w);

      //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

      strcpy(text,db.formatnumber(text,line.length(),0));
      Text rtext(CHAR2RCCHR("Arial"),Point(line.getp2()),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);
      if (::v.getinteger("wn::distancetracking"))
      {
          //rtext.qgi = rbdata->qgtext; rtext.zValue = rbdata->zValue;
          rtext.draw(DM_INVERT);
          //rbdata->qgtext = rtext.qgi; rbdata->zValue = rtext.zValue;
      }
  }
}
#else
void RBLine3(int mode,void *data,Point *p2,View3dSurface *v)
{
 RBLine2_data *rbdata = (RBLine2_data *) data;
 Point p3,p4,p5;
 int l,c,s,w;
 RCCHAR text[300];
 BitMask toptions(32);

  if (v != 0)
    rbdata->lasttextheight = 6.0 * v->getscale();

  l = db.header.getlayer();
  c = db.header.getcolour();
  s = db.header.getstyle();
  w = db.header.getweight();
  if (rbdata->e == NULL)
    {  p3 = rbdata->p;
       p4 = rbdata->xaxis * (rbdata->xaxis.dot(p3)) +
            rbdata->yaxis * (rbdata->yaxis.dot(*p2)) +
            rbdata->zaxis * (rbdata->zaxis.dot(*p2));
    }
  else
    {  p3 = *p2;
       Line l(p3,p3+rbdata->yaxis);
       Intersect i(rbdata->e,&l,rbdata->p);
       if (i.nintersections() == 1)
         {  p3 = ((PointE *)i.intersection(0))->getp();
            p4 = *p2; 
         }
       else if (rbdata->e->isa(circle_entity))
         {  Line line(rbdata->yaxis,rbdata->e,rbdata->p,NULL,*p2,0.0);
            p3 = line.getp1();
            p4 = line.getp2();
         }
       else
         p4 = p3 + rbdata->yaxis * 20.0;
       if (db.grid.getsnap() == 0 && s == 0)
         s = 1;
    }

    Line line(p3,p4);
    line.EntityHeader::change(c,l,s,w);

    //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
    line.draw(DM_INVERT);

    rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

    strcpy(text,db.formatnumber(text,line.length(),0));
    Text rtext(CHAR2RCCHR("Arial"),Point(line.getp2()),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,rbdata->lasttextheight,0,0.0,0,CHAR2RCCHR(text),0,toptions);
    if (::v.getinteger("wn::distancetracking"))
    {
      rtext.qgi = rbdata->qgtext; rtext.zValue = rbdata->zValue;

      rtext.draw(DM_INVERT);

      rbdata->qgtext = rtext.qgi; rbdata->zValue = rtext.zValue;
    }
}
#endif

/**
 * @brief line_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for inserting vertical Lines
 *
 */
void line_command3(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine2_data *rbdata = (RBLine2_data *) *data;
 Point  p1,p2,p3;
 Entity *e;
 View3dSurface *surface;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBLine2_data;  *data = rbdata;
         rbdata->qgline=0; rbdata->qgtext=0;rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->e = NULL;
              rbdata->p = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p = ((EntitySelectedEvent *) event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
              rbdata->e->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
             *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (rbdata->e == NULL)
                {  p1 = rbdata->p;
                   p2 = ((Coordinate3dEvent *) event)->getp();
                   p2 = rbdata->xaxis * (rbdata->xaxis.dot(p1)) +
                        rbdata->yaxis * (rbdata->yaxis.dot(p2)) +
                        rbdata->zaxis * (rbdata->zaxis.dot(p2));
                   Line *line = new Line(p1,p2);
                   line->zValue = rbdata->zValue;
                   db.geometry.add(line);
                   *cstate = 1;
                }
              else
                {  p1 = ((Coordinate3dEvent *) event)->getp();
                   Line l(p1,p1+rbdata->yaxis);
                   Intersect i(rbdata->e,&l,rbdata->p);
                   if (i.nintersections() == 1)
                     {  p2 = ((PointE *)i.intersection(0))->getp();
                        Line *line = new Line(p1,p2);
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                   else
                     {  Line *line = new Line(rbdata->yaxis,rbdata->e,rbdata->p,NULL,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                            db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                }
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              if (rbdata->e == NULL)
                {  e = ((EntitySelectedEvent *) event)->getentity();
                   p1 = ((EntitySelectedEvent *) event)->getp();
                   Line l(rbdata->p,rbdata->p+rbdata->yaxis);
                   Intersect i(e,&l,p1);
                   if (i.nintersections() == 1)
                     {  p1 = rbdata->p;
                        p2 = ((PointE *)i.intersection(0))->getp();
                        Line *line = new Line(p1,p2);
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        *cstate = 1;
                     }
                   else
                     {  Line *line = new Line(rbdata->yaxis,rbdata->e,rbdata->p,e,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                            db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                     }
                }
              else
                {  e = ((EntitySelectedEvent *) event)->getentity();
                   p1 = p3 = ((EntitySelectedEvent *) event)->getp();
                   if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                     {  if ((surface = ((EntitySelectedEvent *) event)->getwindow()) != NULL)
                          {  p1 = surface->modeltouv(p1);
                             p1 = db.grid.snappoint(p1);
                             p1 = surface->uvtomodel(p1);
                          }
                     }
                   else
                     {  p1 = db.workplanes.getcurrent()->modeltoworkplane(p1);
                        p1 = db.grid.snappoint(p1);
                        p1 = db.workplanes.getcurrent()->workplanetomodel(p1);
                     }
                   Line l(p1,p1+rbdata->yaxis);
                   Intersect i1(rbdata->e,&l,rbdata->p);
                   Intersect i2(e,&l,p3);
                   if (i1.nintersections() == 1 && i2.nintersections() == 1)
                     {  Line *line = new Line(((PointE *)i1.intersection(0))->getp(),((PointE *)i2.intersection(0))->getp());
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                        rbdata->e->unhighlight();
                        *cstate = 1;
                     }
                   else
                     {  Line *line = new Line(rbdata->yaxis,rbdata->e,rbdata->p,e,p1,0.0);
                        if (line != NULL && line->getdefined())
                        {
                            line->zValue = rbdata->zValue;
                            db.geometry.add(line);
                        }
                        else
                          delete line;
                        *cstate = 1;
                        rbdata->e->unhighlight();
                     }
                }
           }
         else if (event->isa(Abort))
           {
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = ExitState;
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
             if(rbdata->qgtext)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgtext);
             delete rbdata->qgtext;rbdata->qgtext=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,curve_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+4);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBLine3,rbdata);
       cadwindow->prompt->normalprompt(NCLINEB+5);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBLine4_data
  {
#ifdef USING_WIDGETS
   qreal zValue;
   QGraphicsItem *qgline;
#endif
    Entity *e,*refe;
   double angle,offsetangle,length;
   int refline,moverefline,constrainlength;
   Point p,xaxis,yaxis,zaxis;
  };

#ifdef NEW_RUBBERB
void RBLine4(int mode,void *data,Point *p2,View3dSurface *)
{
 RBLine4_data *rbdata = (RBLine4_data *) data;
 Point p3,p4,p5,p6;
 double angle,l,mind;
 int i;

 if(mode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
 }
 else
 {
      if (rbdata->e == NULL)
        {  p3 = rbdata->p;
           l = (*p2 - rbdata->p).length();
           if (rbdata->constrainlength && rbdata->length > 1.0E-10)
             {  l = rbdata->length * floor(l / rbdata->length + 0.5);
                if (l == 0.0)
                  l = rbdata->length;
             }
           mind = -1.0;

           for (i = 0 ; i < 4 ; i++)
             {  if (i == 0)
                  angle = rbdata->offsetangle + rbdata->angle;
                else if (i == 1)
                  angle = rbdata->offsetangle - rbdata->angle;
                else if (i == 2)
                  angle = (rbdata->offsetangle + acos(-1.0)) + rbdata->angle;
                else if (i == 3)
                  angle = (rbdata->offsetangle + acos(-1.0)) - rbdata->angle;
                p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                          rbdata->yaxis * (l * sin(angle));
                Line line(p3,p4);
                if (mind < 0.0 || (*p2 - p4).length() < mind)
                  {  p5 = p3;  p6 = p4;
                     mind = (*p2 - p4).length();
                  }
             }

           Line line(p5,p6);

           //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
           line.draw(DM_INVERT);
           //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

        }
      else
      {
          //qDebug() << "RBLine4 : length1 : " << rbdata->constrainlength*rbdata->length;

           Line line(rbdata->xaxis*cos(rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(rbdata->angle+rbdata->offsetangle),
                 rbdata->e,rbdata->p,NULL,*p2,rbdata->constrainlength*rbdata->length);
           if (line.getdefined())
           {
               //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
               line.draw(DM_INVERT);
               //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
           }
           else
           {
               //qDebug() << "RBLine4 : length2 : " << rbdata->constrainlength*rbdata->length;

                Line line(rbdata->xaxis*cos(-rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(-rbdata->angle+rbdata->offsetangle),
                          rbdata->e,rbdata->p,NULL,*p2,rbdata->constrainlength*rbdata->length);
                if (line.getdefined())
                {
                    //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                    line.draw(DM_INVERT);
                    //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
                }
           }
      }
    }
}
#else
void RBLine4(int,void *data,Point *p2,View3dSurface *)
{
 RBLine4_data *rbdata = (RBLine4_data *) data;
 Point p3,p4,p5,p6;
 double angle,l,mind;
 int i;

  if (rbdata->e == NULL)
    {  p3 = rbdata->p;
       l = (*p2 - rbdata->p).length();
       if (rbdata->constrainlength && rbdata->length > 1.0E-10)
         {  l = rbdata->length * floor(l / rbdata->length + 0.5);
            if (l == 0.0)
              l = rbdata->length;  
         } 
       mind = -1.0;

       for (i = 0 ; i < 4 ; i++)
         {  if (i == 0)
              angle = rbdata->offsetangle + rbdata->angle;
            else if (i == 1)
              angle = rbdata->offsetangle - rbdata->angle;
            else if (i == 2)
              angle = (rbdata->offsetangle + acos(-1.0)) + rbdata->angle;
            else if (i == 3)
              angle = (rbdata->offsetangle + acos(-1.0)) - rbdata->angle;
            p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                      rbdata->yaxis * (l * sin(angle));
            Line line(p3,p4);
            if (mind < 0.0 || (*p2 - p4).length() < mind)
              {  p5 = p3;  p6 = p4; 
                 mind = (*p2 - p4).length();
              }
         }

       Line line(p5,p6);

       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;

       line.draw(DM_INVERT);

       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;

    }
  else
  {
      //qDebug() << "RBLine4 : length1 : " << rbdata->constrainlength*rbdata->length;

       Line line(rbdata->xaxis*cos(rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(rbdata->angle+rbdata->offsetangle),
             rbdata->e,rbdata->p,NULL,*p2,rbdata->constrainlength*rbdata->length);
       if (line.getdefined())
       {
           line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;

           line.draw(DM_INVERT);

           rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
       }
       else
       {
           //qDebug() << "RBLine4 : length2 : " << rbdata->constrainlength*rbdata->length;

            Line line(rbdata->xaxis*cos(-rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(-rbdata->angle+rbdata->offsetangle),
                      rbdata->e,rbdata->p,NULL,*p2,rbdata->constrainlength*rbdata->length);
            if (line.getdefined())
            {
                line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;

                line.draw(DM_INVERT);

                rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
            }
       }
  }
}
#endif

/**
 * @brief line_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Angled Line command
 *
 */
void line_command4(int *cstate,HighLevelEvent *event,void **data)
{RBLine4_data *rbdata = (RBLine4_data *) *data;
 Point  p1,p2,p3,p4,p5,p6;
 Entity *e,*newline;
 double l,angle,mind;
 int i;
 DialogControl *dc;
 Dialog dialog("LineAngle_Dialog");
 ResourceString rs6(NCLINEB+6);
 switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBLine4_data) == NULL)
           {  state.fail(Memory,rs6.gets());
              *cstate = ExitState;
           }
         else
           {
             *data = rbdata;
              rbdata->angle = v.getreal("ln::angle");
              rbdata->refline = v.getinteger("ln::refline");  
              rbdata->length = v.getreal("ln::length");
              rbdata->constrainlength = v.getinteger("ln::constrainlength");  
              rbdata->moverefline = v.getinteger("ln::moverefline");  

              rbdata->qgline=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);

              dialog.add(dc = new RealDialogControl(100,&rbdata->angle));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(new CheckBoxDialogControl(101,&rbdata->refline));
              dialog.add(dc = new RealDialogControl(102,&rbdata->length,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              dialog.add(new CheckBoxDialogControl(103,&rbdata->constrainlength));
              dialog.add(new CheckBoxDialogControl(104,&rbdata->moverefline));

              if (dialog.process() == TRUE)
                {  v.setreal("ln::angle",rbdata->angle);
                   v.setinteger("ln::refline",rbdata->refline);
                   v.setreal("ln::length",rbdata->length);
                   v.setinteger("ln::constrainlength",rbdata->constrainlength);
                   v.setinteger("ln::moverefline",rbdata->moverefline);
                   rbdata->angle *= M_PI / 180.0;
                   if (rbdata->angle == 0.0) rbdata->angle = M_PI * 2.0;
                   rbdata->offsetangle = 0.0;
                   rbdata->refe = 0;
                   if (rbdata->refline)
                     *cstate = 1;
                   else
                     *cstate = 2;
                }
              else
              {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
                *cstate = ExitState;
              }
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->refe = ((EntitySelectedEvent *) event)->getentity();
              if (rbdata->refe->isa(line_entity))
                {Line *line = (Line *) rbdata->refe;
                   rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
                   rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
                   rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                   p4.x = rbdata->xaxis.dot(line->getp2() - line->getp1());
                   p4.y = rbdata->yaxis.dot(line->getp2() - line->getp1());
                   if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                     {  p4.x /= l;  p4.y /= l;
                        if (rbdata->constrainlength && rbdata->length > 1.0E-10)
                          {  l = rbdata->length * floor(l / rbdata->length + 0.5);
                             if (l == 0.0)
                               l = rbdata->length;  
                          } 
                        if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                        rbdata->offsetangle = acos(p4.x);
                        if (p4.y < 0.0) rbdata->offsetangle = 2.0 * M_PI - rbdata->offsetangle;
                        if (rbdata->offsetangle > M_PI)
                          rbdata->offsetangle = rbdata->offsetangle - M_PI;
                        if (rbdata->offsetangle > M_PI / 2.0)
                          rbdata->offsetangle = rbdata->offsetangle - M_PI;
                        rbdata->refe->highlight();
                        *cstate = 2;
                     }
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
         {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
           *cstate = ExitState;
         }
         break;
       case 2 :
         if (event->isa(Coordinate3d))
           {  rbdata->e = NULL;
              rbdata->p = ((Coordinate3dEvent *) event)->getp();
              if (! rbdata->refline)
                {  rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
                   rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();
                   rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                }
              *cstate = 3;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p = ((EntitySelectedEvent *) event)->getp();
              if (((EntitySelectedEvent *)event)->getendselected() == 2)
                rbdata->angle = -fabs(rbdata->angle);
              else
                rbdata->angle = fabs(rbdata->angle);
              if (! rbdata->refline)
                {  rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
                   rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
                   rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                }
              rbdata->e->highlight();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
         {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif

             if (rbdata->refe != NULL) rbdata->refe->unhighlight();
              *cstate = ExitState;
         }
         break;
       case 3 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  newline = 0;
              if (rbdata->e == NULL)
                {  p2 = ((Coordinate3dEvent *) event)->getp();
                   p3 = rbdata->p;

                   l = (p2 - p3).length();
                   if (rbdata->constrainlength && rbdata->length > 1.0E-10)
                     {  l = rbdata->length * floor(l / rbdata->length + 0.5);
                        if (l == 0.0)
                          l = rbdata->length;  
                     } 

                   mind = -1.0;

                   for (i = 0 ; i < 4 ; i++)
                     {  if (i == 0)
                          angle = rbdata->offsetangle + rbdata->angle;
                        else if (i == 1)
                          angle = rbdata->offsetangle - rbdata->angle;
                        else if (i == 2)
                          angle = (rbdata->offsetangle + acos(-1.0)) + rbdata->angle;
                        else if (i == 3)
                          angle = (rbdata->offsetangle + acos(-1.0)) - rbdata->angle;
                        p4 = p3 + rbdata->xaxis * (l * cos(angle)) +
                                  rbdata->yaxis * (l * sin(angle));
                        Line line(p3,p4);
                        if (mind < 0.0 || (p2 - p4).length() < mind)
                          {  p5 = p3;  p6 = p4; 
                             mind = (p2 - p4).length();
                          }
                     }

                   newline = new Line(p5,p6);
                   newline->zValue = rbdata->zValue;
                   db.geometry.add(newline);
                   rbdata->angle = fabs(rbdata->angle);
                   rbdata->p = p6;

                }
              else
                {  p1 = ((Coordinate3dEvent *) event)->getp();
                   newline = new Line(rbdata->xaxis*cos(rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(rbdata->angle+rbdata->offsetangle),
                                         rbdata->e,rbdata->p,NULL,p1,rbdata->constrainlength*rbdata->length);
                   if (newline != NULL && ! newline->getdefined())
                     {  delete newline;
                        newline = new Line(rbdata->xaxis*cos(-rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(-rbdata->angle+rbdata->offsetangle),
                                        rbdata->e,rbdata->p,NULL,p1,rbdata->constrainlength*rbdata->length);
                     }
                   if (newline != NULL && newline->getdefined())
                     {
                        newline->zValue = rbdata->zValue;
                        db.geometry.add(newline);
                        *cstate = 2;
                        rbdata->e->unhighlight();
                        rbdata->angle = fabs(rbdata->angle);
                     }
                   else
                     delete newline;
                }

#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
              //  Update the details
              rbdata->angle /= M_PI / 180.0;
              dialog.add(dc = new RealDialogControl(100,&rbdata->angle));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(new CheckBoxDialogControl(101,&rbdata->refline));
              dialog.add(dc = new RealDialogControl(102,&rbdata->length,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              dialog.add(new CheckBoxDialogControl(103,&rbdata->constrainlength));
              dialog.add(new CheckBoxDialogControl(104,&rbdata->moverefline));
              if (dialog.process() == TRUE)
                {  v.setreal("ln::angle",rbdata->angle);
                   v.setinteger("ln::refline",rbdata->refline);
                   v.setreal("ln::length",rbdata->length);
                   v.setinteger("ln::constrainlength",rbdata->constrainlength);
                   v.setinteger("ln::moverefline",rbdata->moverefline);
                   rbdata->angle *= M_PI / 180.0;
                   if (rbdata->angle == 0.0) rbdata->angle = M_PI * 2.0;
                   rbdata->offsetangle = 0.0;
                   if (rbdata->refe != NULL) rbdata->refe->unhighlight();
                   rbdata->refe = NULL;
                   if (rbdata->moverefline && newline != 0)
                   {
                       rbdata->refe = newline;
                        Line *line = (Line *) rbdata->refe;
                        rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
                        rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
                        rbdata->zaxis = rbdata->xaxis.cross(rbdata->yaxis);
                        p4.x = rbdata->xaxis.dot(line->getp2() - line->getp1());
                        p4.y = rbdata->yaxis.dot(line->getp2() - line->getp1());
                        if ((l = sqrt(p4.x*p4.x + p4.y*p4.y)) > db.getptoler())
                        {  p4.x /= l;  p4.y /= l;
                             if (p4.x > 1.0) p4.x = 1.0;  else if (p4.x < -1.0) p4.x = -1.0;
                             rbdata->offsetangle = acos(p4.x);
                             if (p4.y < 0.0) rbdata->offsetangle = 2.0 * M_PI - rbdata->offsetangle;
                             if (rbdata->offsetangle > M_PI)
                               rbdata->offsetangle = rbdata->offsetangle - M_PI;
                             if (rbdata->offsetangle > M_PI / 2.0)
                               rbdata->offsetangle = rbdata->offsetangle - M_PI;
                             rbdata->refe->highlight();
                        }
                   }
                }
              else
              {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
                  if (rbdata->refe != NULL) rbdata->refe->unhighlight();
                   *cstate = ExitState;
              }
         }
         else if (event->isa(EntitySelected))
         {
             state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p1 = ((EntitySelectedEvent *) event)->getp();
              p1 = db.grid.snappoint(((EntitySelectedEvent *) event)->getwindow(),p1);
              Line *line = new Line(rbdata->xaxis*cos(rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(rbdata->angle+rbdata->offsetangle),
                                    rbdata->e,rbdata->p,e,p1,rbdata->constrainlength*rbdata->length);
              if (line != NULL && ! line->getdefined())
                {  delete line;
                   line = new Line(rbdata->xaxis*cos(-rbdata->angle+rbdata->offsetangle)+rbdata->yaxis*sin(-rbdata->angle+rbdata->offsetangle),
                                   rbdata->e,rbdata->p,e,p1,rbdata->constrainlength*rbdata->length);
                }
              if (line != NULL && line->getdefined())
                {
                   line->zValue = rbdata->zValue;
                   db.geometry.add(line);
                   *cstate = 2;
                   if (rbdata->e != NULL) rbdata->e->unhighlight();
                   rbdata->angle = fabs(rbdata->angle);
                }
              else
                delete line;
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
         }
         else if (event->isa(Abort))
         {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
             if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = 2;
         }
         else if (event->isa(Exit))
         {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
             if (rbdata->refe != NULL) rbdata->refe->unhighlight();
              if (rbdata->e != NULL) rbdata->e->unhighlight();
              *cstate = ExitState;
         }
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    state.selmask.entity.clearandset(line_entity,end_list);
  else if (*cstate == 2 || *cstate == 3)
    state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+7);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCLINEB+8);
  else if (*cstate == 3)
    {  rubberband.begin(0);
       rubberband.add(RBLine4,rbdata);
       cadwindow->prompt->normalprompt(NCLINEB+9);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBLine5_data
{
#ifdef USING_WIDGETS
   qreal zValue;
   QGraphicsItem *qgline;
#endif
    Point p;
    Entity *e;
};

#ifdef NEW_RUBBERB
void RBLine5(int mode,void *data,Point *p2,View3dSurface *v)
{
  RBLine5_data *rbdata = (RBLine5_data *) data;
  Line line(rbdata->p,*p2);

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
  }
  else
  {
      //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      //rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
  }
}
#else
void RBLine5(int mode,void *data,Point *p2,View3dSurface *v)
{
  RBLine5_data *rbdata = (RBLine5_data *) data;
  Line line(rbdata->p,*p2);

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
  }
  else
  {
      line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
      line.draw(DM_INVERT);
      rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
  }
}
#endif

class CHRadioButtonDialogControl : public RadioButtonDialogControl
  {public:
    CHRadioButtonDialogControl(int id,int id1,int id2,int *value) : RadioButtonDialogControl(id,id1,id2,value)  { }
    void load(Dialog *dialog);
    int select(Dialog *dialog);
  };


void CHRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),102),*value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),202),*value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),203),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),204),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1102),*value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),*value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),*value == 1);
}

int CHRadioButtonDialogControl::select(Dialog *dialog)
{int status,value;
  status = RadioButtonDialogControl::select(dialog);
  value = 0;
  currentvalue(dialog,&value);
  EnableWindow(GetDlgItem(dialog->gethdlg(),102),value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),202),value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),203),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),204),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1102),value == 0);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),value == 1);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),value == 1);
  return status;
}

/**
 * @brief line_command5
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the line chamfer command
 *
 */
void line_command5(int *cstate,HighLevelEvent *event,void **data)
{
 RBLine5_data *rbdata = (RBLine5_data *) *data;
 static int type;
 static double dist1,dist2,dist3;
 Point  dir1,dir2,p,pint,p1,p2;
 double d1,d2,cosa,sina;
 int status;
 Entity *e;
 DialogControl *dc;        
 Dialog *dialog;
  switch (*cstate)
    {  case InitialState :
         rbdata = new RBLine5_data;   *data = rbdata;
         rbdata->qgline=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
         dist1 = v.getreal("ln::chamferd1");
         dist2 = v.getreal("ln::chamferd2");
         dist3 = v.getreal("ln::chamferd3");
         type = v.getinteger("ln::type");
         dialog = new Dialog("Chamfer_Dialog");
         dialog->add(new CHRadioButtonDialogControl(100,100,101,&type));
         dialog->add(new CHRadioButtonDialogControl(101,100,101,&type));
         dialog->add(dc = new RealDialogControl(102,&dist1,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1102,dc));
         dialog->add(dc = new RealDialogControl(103,&dist2,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1103,dc));
         dialog->add(dc = new RealDialogControl(104,&dist3,ScaleByLength));
         //dialog->add(new ScrollBarDialogControl(1104,dc));
         status = dialog->process();
         delete dialog;
         if (status == TRUE)
           {  v.setreal("ln::chamferd1",dist1);
              v.setreal("ln::chamferd2",dist2);
              v.setreal("ln::chamferd3",dist3);
              v.setinteger("ln::type",type);
              *cstate = 1;
           }
         else
         {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
           *cstate = ExitState;
         }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p = ((EntitySelectedEvent *) event)->getp();
              rbdata->e->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
         {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
           *cstate = ExitState;
         }
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              rbdata->e->unhighlight();
              Intersect i(rbdata->e,e);
              if (i.nintersections() == 1)
                {  pint = ((PointE *)i.intersection(0))->getp();
                   dir1 = rbdata->p - pint;
                   dir2 = p - pint;
                   if (dir1.length() > db.getptoler() && dir2.length() > db.getptoler())
                     {  dir1.normalize();  dir2.normalize();
                        if (type == 1)
                          {  d1 = dist2;  d2 = dist3;
                          }
                        else
                          {  cosa = dir1.dot(dir2);
                             if (cosa < -1.0) cosa = -1.0;  else if (cosa > 1.0) cosa = 1.0;
                             sina = sin(acos(cosa)/2.0);
                             if (fabs(sina) < db.getatoler())
                               {  d1 = d2 = 0.0;
                               }
                             else
                               {  d1 = d2 = dist1 / 2.0 / sina;
                               }
                          }
                        p1 = pint + dir1 * d1;  p2 = pint + dir2 * d2;
                        if (dir1.dot(((Line *)rbdata->e)->direction(0)) > 0.0)
                          ((Line *)rbdata->e)->moveendpoint(1,1,p1);
                        else
                          ((Line *)rbdata->e)->moveendpoint(1,2,p1);
                        if (dir2.dot(((Line *)e)->direction(0)) > 0.0)
                          ((Line *)e)->moveendpoint(1,1,p2);
                        else
                          ((Line *)e)->moveendpoint(1,2,p2);
                        Line *line = new Line(p1,p2);
                        line->zValue = rbdata->zValue;
                        db.geometry.add(line);
                     }
                }
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
              *cstate = 1;
           }
         else if (event->isa(Abort))
           {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
             rbdata->e->unhighlight();
              *cstate = 1;
           }
         else if (event->isa(Exit))
           {
#ifdef USING_WIDGETS
#ifdef NEW_RUBBERB
             if(rbdata->qgline)
                 cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
             delete rbdata->qgline;rbdata->qgline=0;
#endif
#endif
             rbdata->e->unhighlight();
              *cstate = ExitState;
           }
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1 || *cstate == 2)
    state.selmask.entity.clearandset(line_entity,end_list);
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCLINEB+10);
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBLine5,rbdata);
       cadwindow->prompt->normalprompt(NCLINEB+11);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}


