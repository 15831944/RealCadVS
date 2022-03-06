

#include "ncwin.h"

typedef int (WINAPI *runcommand)(char *);

struct RBRev1_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgline1,*qgline2,*qgline3;
    qreal zValue;
#endif
   int state,copyprofile,insertsides;
   EntityList list;
   Point p1,p2;
};

#ifdef NEW_RUBBERB
void RBRev1(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBRev1_data *rbdata = (RBRev1_data *) data;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if (rbdata->state == 2)
    {
       Line line(rbdata->p1,*p2);

       //line.qgi = rbdata->qgline1; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       //rbdata->qgline1 = line.qgi;
    }
  else if (rbdata->state == 3)
    {
       Line line1(rbdata->p1,rbdata->p2);

       //line1.qgi = rbdata->qgline1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       //rbdata->qgline1 = line1.qgi;

       Line line2(rbdata->p1,*p2);

       //line2.qgi = rbdata->qgline1; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       //rbdata->qgline1 = line2.qgi;

       Line line3(rbdata->p2,*p2);

       //line3.qgi = rbdata->qgline1; line3.zValue = rbdata->zValue;
       line3.draw(DM_INVERT);
       //rbdata->qgline1 = line3.qgi;
    }
}
#else
#ifdef USING_WIDGETS
void RBRev1(int,void *data,Point *p2,View3dSurface *)
{
  RBRev1_data *rbdata = (RBRev1_data *) data;

  if (rbdata->state == 2)
    {
       Line line(rbdata->p1,*p2);

       line.qgi = rbdata->qgline1; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline1 = line.qgi;
    }
  else if (rbdata->state == 3)
    {
       Line line1(rbdata->p1,rbdata->p2);

       line1.qgi = rbdata->qgline1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgline1 = line1.qgi;

       Line line2(rbdata->p1,*p2);

       line2.qgi = rbdata->qgline1; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgline1 = line2.qgi;

       Line line3(rbdata->p2,*p2);

       line3.qgi = rbdata->qgline1; line3.zValue = rbdata->zValue;
       line3.draw(DM_INVERT);
       rbdata->qgline1 = line3.qgi;
    }
}
#else
void RBRev1(int,void *data,Point *p2,View3dSurface *)
{
  RBRev1_data *rbdata = (RBRev1_data *) data;
  if (rbdata->state == 2)
    {
       Line line(rbdata->p1,*p2);
       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {
       Line line1(rbdata->p1,rbdata->p2);
       line1.draw(DM_INVERT);
       Line line2(rbdata->p1,*p2);
       line2.draw(DM_INVERT);
       Line line3(rbdata->p2,*p2);
       line3.draw(DM_INVERT);
    }
}
#endif
#endif

static void insertsurfaces(RBRev1_data *rbdata,double *a)
{Entity *e;
 double angle,deltaz;
 int i,xmesh,ymesh;
 BitMask options(32);
 Circle *circle,*circle1,*circle2;
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;

  db.saveundo(UD_STARTBLOCK,NULL);
  SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);
  if (a != 0)
    angle = *a;
  for (rbdata->list.start() ; (e = rbdata->list.next()) != NULL ; )
    {  SurfaceOfRev *surface = new SurfaceOfRev(rbdata->p1,(rbdata->p2-rbdata->p1).normalize(),angle,deltaz,(LinearEntity *)e,xmesh,ymesh,options);
       if (surface != NULL && surface->getdefined())
         db.geometry.add(surface);
       else
         delete surface;
       if (rbdata->copyprofile || rbdata->insertsides)
         {Transform t1,t2;
          Point p1,p2,p3,p4;
           t1.rotate(rbdata->p1,(rbdata->p2-rbdata->p1).normalize(),angle*M_PI/180.0);
           t2.translate((rbdata->p2-rbdata->p1).normalize()*deltaz);
           t1.apply(t2);
           ((LinearEntity *)e)->setcopye(0);
           db.geometry.add(((LinearEntity *)e)->clone(t1));
         }
       if (rbdata->insertsides)
         {  for (i = 0 ; i < 2 ; i++)
              {Point p1,p2,p3,p4;
               EntityList list,elist;
               double xspacing,yspacing,pangle,crossangle,transparency,red,green,blue,transparencyrange;
               BitMask options(32);
                 p1 = ((LinearEntity *)(i == 0 ? e : e->getcopye()))->start();
                 p2 = ((LinearEntity *)(i == 0 ? e : e->getcopye()))->end();
                 p3 = Line(rbdata->p1,rbdata->p2).nearp(p1);
                 p4 = Line(rbdata->p1,rbdata->p2).nearp(p2);
                 list.add((i == 0 ? e : e->getcopye()));
                 if ((p2-p1).length() > db.getptoler())
                   {  if ((p3-p4).length() > db.getptoler())
                        {  if ((p3-p1).length() > db.getptoler())
                             list.add(new Line(p1,p3));
                           list.add(new Line(p3,p4));
                           if ((p4-p2).length() > db.getptoler())
                             list.add(new Line(p4,p2));
                        }
                      else
                        list.add(new Line(p1,p2));
                   }
                 Plane::getdefaults(&xspacing,&yspacing,&pangle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                 Plane *plane = new Plane(list,xspacing,yspacing,pangle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 if (plane->getdefined())
                   db.geometry.add(plane);
                 else
                   delete plane;
                 if (i == 0 && (p1 - p2).length() > db.getptoler() && deltaz == 0.0)
                   {  if ((p3-p4).length() < db.getptoler())
                        {  list = elist;
                           list.add(circle1 = new Circle(p3,(p1 - p3).normalize(),(p1 - p3).cross(rbdata->p1-rbdata->p2).normalize(),(p1 - p3).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
                           list.add(new Line (p1,p2));
                           list.add(circle2 = new Circle(p4,(p2 - p4).normalize(),(p2 - p3).cross(rbdata->p1-rbdata->p2).normalize(),(p2 - p4).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
                           list.add(new Line(circle1->end(),circle2->end()));
                           Plane *plane = new Plane(list,xspacing,yspacing,pangle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                           if (plane->getdefined())
                             db.geometry.add(plane);
                           else
                             delete plane;
                        }
                      else
                        {  if ((p1-p3).length() > db.getptoler())
                             {  list = elist;
                                list.add(circle = new Circle(p3,(p1 - p3).normalize(),(p1 - p3).cross(rbdata->p1-rbdata->p2).normalize(),(p3 - p1).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
                                list.add(new Line (p1,p3));
                                list.add(new Line(p3,circle->end()));
                                Plane *plane = new Plane(list,xspacing,yspacing,pangle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                                if (plane->getdefined())
                                  db.geometry.add(plane);
                                else
                                  delete plane;
                             }
                           if ((p2-p4).length() > db.getptoler() && deltaz == 0.0)
                             {  list = elist;
                                list.add(circle = new Circle(p4,(p2 - p4).normalize(),(p2 - p4).cross(rbdata->p1-rbdata->p2).normalize(),(p4 - p2).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
                                list.add(new Line (p2,p4));
                                list.add(new Line(p4,circle->end()));
                                Plane *plane = new Plane(list,xspacing,yspacing,pangle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                                if (plane->getdefined())
                                  db.geometry.add(plane);
                                else
                                  delete plane;
                            }
                        }
                   }
              }
         }
       else if (rbdata->copyprofile)
         {Point p1,p2,p3,p4;
            p1 = ((LinearEntity *)e)->start();
            p2 = ((LinearEntity *)e)->end();
            p3 = Line(rbdata->p1,rbdata->p2).nearp(p1);
            p4 = Line(rbdata->p1,rbdata->p2).nearp(p2);
            if ((p1 - p3).length() > db.getptoler())
              db.geometry.add(new Circle(p3,(p1 - p3).normalize(),(p1 - p3).cross(rbdata->p1-rbdata->p2).normalize(),(p3 - p1).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
            if ((p2 - p4).length() > db.getptoler() && (p1-p2).length() > db.getptoler())
              db.geometry.add(new Circle(p4,(p2 - p4).normalize(),(p2 - p4).cross(rbdata->p1-rbdata->p2).normalize(),(p4 - p2).length(),0.0,(angle > 360.0 ? 360.0 : angle < -360.0 ? -360.0 : angle) * M_PI / 180.0));
         }
    }
  db.saveundo(UD_ENDBLOCK,NULL);
}

/**
 * @brief surfaceofrev_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Surface of Revolution command
 *
 *
 */
void surfaceofrev_command1(int *cstate,HighLevelEvent *event,void **data)
{RBRev1_data *rbdata = (RBRev1_data *) *data;
 Entity *e;
 Line *line;
 Circle *circle;
 int error;
 double angle;
 ResourceString rs0(NCREVB);
 ResourceString rs14(NCREVB+14);
 ResourceString rs15(NCREVB+15);
 ResourceString rs16(NCREVB+16);
 Dialog dialog("SurfaceOfRevInsert_Dialog");

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(411,0));
       *cstate = ExitState;
       return;
    }

   switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBRev1_data) == NULL)
           {  state.fail(Memory,rs0.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->copyprofile = v.getinteger("rs::copyprofile");
              rbdata->insertsides = v.getinteger("rs::insertsides");

#ifdef USING_WIDGETS
              rbdata->qgline1=0; rbdata->qgline2=0; rbdata->qgline3=0;
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif

              dialog.add(new CheckBoxDialogControl(100,&rbdata->copyprofile));
              dialog.add(new CheckBoxDialogControl(101,&rbdata->insertsides));
              if (dialog.process() == FALSE)
                *cstate = ExitState;
              else
                {  v.setinteger("rs::copyprofile",rbdata->copyprofile);
                   v.setinteger("rs::insertsides",rbdata->insertsides);
                   if (state.getselection().length() == 0)
                     {  cadwindow->MessageBox(rs16.gets(),rs15.gets(),MB_ICONINFORMATION);
                        *cstate = ExitState;
                     }
                   else
                     {  rbdata->list = state.getselection().copy();
                        for (rbdata->list.start(),error = 0 ; (e = rbdata->list.next()) != NULL ; )
                          if (! e->islinear()) error = 1;
                        if (error)
                          {  cadwindow->MessageBox(rs14.gets(),rs15.gets(),MB_ICONINFORMATION);
                             *cstate = ExitState;
                          }
                        else
                          *cstate = 1;
                        *data = rbdata;
                     }
                }
           }
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *)event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *)event)->getentity();
              if (e->isa(line_entity))
                {  line = (Line *) e;
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     {  rbdata->p1 = line->start();  rbdata->p2 = line->end();
                     }
                   else
                     {  rbdata->p1 = line->end();  rbdata->p2 = line->start();
                     }
                   if ((rbdata->p2 - rbdata->p1).length() > db.getptoler())
                     {  insertsurfaces(rbdata,0);
                        *cstate = ExitState;
                     }
                }
              else if (e->isa(circle_entity))
                {  circle = (Circle *)e;
                   if (((EntitySelectedEvent *)event)->getendselected() == 1)
                     angle =  circle->getsweepa() * 180.0 / M_PI;
                   else
                     angle = -circle->getsweepa() * 180.0 / M_PI;
                   if (circle->getxaxis().cross(circle->getyaxis()).length() > db.getptoler())
                     {  rbdata->p1 = circle->getorigin();
                        rbdata->p2 = rbdata->p1 + circle->getxaxis().cross(circle->getyaxis()).normalize();
                        insertsurfaces(rbdata,&angle);
                        *cstate = ExitState;
                     }
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *)event)->getp();
              if ((rbdata->p2 - rbdata->p1).length() > db.getptoler())
                {  insertsurfaces(rbdata,0);
                   *cstate = ExitState;
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
   if(rbdata)
   {
       if(rbdata->qgline1)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline1);
       delete rbdata->qgline1; rbdata->qgline1=0;
       if(rbdata->qgline2)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline2);
       delete rbdata->qgline2; rbdata->qgline2=0;
       if(rbdata->qgline3)
           cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline3);
       delete rbdata->qgline3; rbdata->qgline3=0;
   }
#endif
#endif

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCREVB+1);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBRev1,rbdata);
       cadwindow->prompt->normalprompt(NCREVB+2);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL) rbdata->list.destroy();
       delete rbdata;
    }
}

#if MENUS != STUDENTMENUS

struct RBRev2_data
{
#ifdef USING_WIDGETS
    QGraphicsItem *qgarc,*qgcircle,*qgsr,*qgline;
    qreal zValue;
#endif
    int state,xmesh,ymesh;
    Point p1,p2,p3,origin,xaxis,yaxis;
    double a,eradius,radius;
};

static int create_elbow(double radius,int xmesh,int ymesh,Circle *arc,Circle **c,SurfaceOfRev **sr)
{Point p1,xaxis,yaxis;
 BitMask options(32);
  p1 = arc->position(0.0);
  xaxis = p1 - arc->getorigin();
  yaxis = arc->getxaxis().cross(arc->getyaxis());
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) return 0;
  xaxis.normalize();  yaxis.normalize();
  *c = new Circle(p1,xaxis,yaxis,radius,0.0,M_PI * 2.0);
  if (*c == NULL || ! (*c)->getdefined())
    {  delete c;  return 0;
    }
  *sr = new SurfaceOfRev(arc->getorigin(),arc->getxaxis().cross(arc->getyaxis()),arc->getsweepa() * 180.0 / M_PI,0.0,*c,xmesh,ymesh,options);
  return 1;
}

static int calculate_arc(RBRev2_data *rbdata,Circle **c)
{Point xaxis,yaxis,zaxis;
 double sweepa;
  *c = NULL;
  if (rbdata->state == 2)
    {  if ((rbdata->eradius = (rbdata->p1 - rbdata->origin).length()) < db.getptoler())
         return 0;
       *c = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->eradius,0.0,M_PI*2.0);
       if (! (*c)->getdefined())
         {  delete *c;  return 0;
         }
    }
  else if (rbdata->state == 3)
    {  xaxis = rbdata->p2 - rbdata->origin;
       if (xaxis.length() < db.getptoler()) return 0;
       xaxis.normalize();
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
       *c = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->eradius,0.0,M_PI_4);
       if (! (*c)->getdefined())
         {  delete *c;  return 0;
         }
    }
  else if (rbdata->state == 4)
    {  yaxis = rbdata->p3 - rbdata->origin;
       if (yaxis.length() < db.getptoler()) return 0;
       yaxis.normalize();
       zaxis = rbdata->xaxis.cross(yaxis);
       if (zaxis.dot(rbdata->xaxis.cross(rbdata->yaxis)) < 0.0)
         zaxis = -zaxis;
       if (zaxis.length() > db.getatoler())
         {  yaxis = zaxis.cross(rbdata->xaxis);
            yaxis.normalize();
            rbdata->yaxis = yaxis;  
          }
       Circle circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->eradius,0.0,2.0 * M_PI);
       if (! circle.getdefined()) return 0;
       sweepa = circle.angle(rbdata->p3);
       if (sweepa < 0.0) sweepa += M_PI * 2.0;
       *c = new Circle(rbdata->origin,rbdata->xaxis,rbdata->yaxis,rbdata->eradius,0.0,sweepa);
       if (*c == NULL || ! (*c)->getdefined())
         {  delete c;  return 0;
         }
    }
  return 1;
}

#ifdef NEW_RUBBERB
void RBRev2(int dmode,void *data,Point *p2,View3dSurface *surface)
{
 RBRev2_data *rbdata = (RBRev2_data *) data;
 Circle *arc,*circle;
 SurfaceOfRev *sr;
 int i,n,index;
 RCCHAR **namelist;
 View *view;
 BitMask visibility(MaxViews);
 Point p3,p4;

 if(dmode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  if (rbdata->state == 2)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       rbdata->p1 = *p2;
    }
  else if (rbdata->state == 3)
    rbdata->p2 = *p2;
  else if (rbdata->state == 4)
    rbdata->p3 = *p2;
  if (calculate_arc(rbdata,&arc))
    {  if (db.getdragdetail() == 0)
         {  if (create_elbow(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,arc,&circle,&sr))
              {
                 //circle->qgi = rbdata->qgcircle; circle->zValue = rbdata->zValue;
                 circle->draw(DM_INVERT);
                 //rbdata->qgcircle = circle->qgi;

                 //sr->qgi = rbdata->qgsr; sr->zValue = rbdata->zValue;
                 sr->draw(DM_INVERT);
                 //rbdata->qgsr = sr->qgi;

                 delete sr;
                 EntityHeader hd;
                 db.views3d.names(&namelist,&n,&index);
                 for (i = 0 ; i < n ; i++)
                   if ((view = db.views3d.match(namelist[i])) != 0)
                     {  if (fabs(arc->getxaxis().cross(arc->getyaxis()).normalize().dot((view->geteye() - view->getref()).normalize())) > cos(0.01))
                          visibility.set(view->getviewno());
                     }
                 hd.setvisible(visibility);
                 for (i = 0 ; i < (rbdata->ymesh/2)*2+1-(arc->getsweepa() >= M_PI*2.0) ; i++)
                   {  p3 = arc->getorigin();
                      p4 = arc->position(double(i) / double((rbdata->ymesh/2)*2));
                      Line line(hd,p4+(p3-p4).normalize()*rbdata->radius,p4+(p4-p3).normalize()*rbdata->radius);

                      //line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                      line.draw(DM_INVERT);
                      //rbdata->qgline = line.qgi;
                   }
                 delete [] namelist;
              }
         }
       else
      {
          //arc->qgi = rbdata->qgarc; arc->zValue = rbdata->zValue;
          arc->draw(DM_INVERT);
          //rbdata->qgarc = arc->qgi;
      }
       delete arc;
    }
}
#else
#ifdef USING_WIDGETS
void RBRev2(int,void *data,Point *p2,View3dSurface *surface)
{
 RBRev2_data *rbdata = (RBRev2_data *) data;
 Circle *arc,*circle;
 SurfaceOfRev *sr;
 int i,n,index;
 RCCHAR **namelist;
 View *view;
 BitMask visibility(MaxViews);
 Point p3,p4;
  if (rbdata->state == 2)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       rbdata->p1 = *p2;
    }
  else if (rbdata->state == 3)
    rbdata->p2 = *p2;
  else if (rbdata->state == 4)
    rbdata->p3 = *p2;
  if (calculate_arc(rbdata,&arc))
    {  if (db.getdragdetail() == 0)
         {  if (create_elbow(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,arc,&circle,&sr))
              {
                 circle->qgi = rbdata->qgcircle; circle->zValue = rbdata->zValue;
                 circle->draw(DM_INVERT);
                 rbdata->qgcircle = circle->qgi;

                 sr->qgi = rbdata->qgsr; sr->zValue = rbdata->zValue;
                 sr->draw(DM_INVERT);
                 rbdata->qgsr = sr->qgi;

                 delete sr;
                 EntityHeader hd;
                 db.views3d.names(&namelist,&n,&index);
                 for (i = 0 ; i < n ; i++)
                   if ((view = db.views3d.match(namelist[i])) != 0)
                     {  if (fabs(arc->getxaxis().cross(arc->getyaxis()).normalize().dot((view->geteye() - view->getref()).normalize())) > cos(0.01))
                          visibility.set(view->getviewno());
                     }
                 hd.setvisible(visibility);
                 for (i = 0 ; i < (rbdata->ymesh/2)*2+1-(arc->getsweepa() >= M_PI*2.0) ; i++)
                   {  p3 = arc->getorigin();
                      p4 = arc->position(double(i) / double((rbdata->ymesh/2)*2));
                      Line line(hd,p4+(p3-p4).normalize()*rbdata->radius,p4+(p4-p3).normalize()*rbdata->radius);

                      line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
                      line.draw(DM_INVERT);
                      rbdata->qgline = line.qgi;
                   }
                 delete [] namelist;
              }
         }
       else
      {
          arc->qgi = rbdata->qgarc; arc->zValue = rbdata->zValue;
          arc->draw(DM_INVERT);
          rbdata->qgarc = arc->qgi;
      }
       delete arc;
    }
}
#else
void RBRev2(int,void *data,Point *p2,View3dSurface *surface)
{
 RBRev2_data *rbdata = (RBRev2_data *) data;
 Circle *arc,*circle;
 SurfaceOfRev *sr;
 int i,n,index;
 RCCHAR **namelist;
 View *view;
 BitMask visibility(MaxViews);
 Point p3,p4;
  if (rbdata->state == 2)
    {  if (surface != NULL)
         {  rbdata->xaxis = surface->getxaxis();
            rbdata->yaxis = surface->getyaxis();
         }
       rbdata->p1 = *p2;
    }
  else if (rbdata->state == 3)
    rbdata->p2 = *p2;
  else if (rbdata->state == 4)
    rbdata->p3 = *p2;
  if (calculate_arc(rbdata,&arc))
    {  if (db.getdragdetail() == 0)
         {  if (create_elbow(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,arc,&circle,&sr))
              {
                 circle->draw(DM_INVERT);
                 sr->draw(DM_INVERT);
                 delete sr;
                 EntityHeader hd;
                 db.views3d.names(&namelist,&n,&index);
                 for (i = 0 ; i < n ; i++)
                   if ((view = db.views3d.match(namelist[i])) != 0)
                     {  if (fabs(arc->getxaxis().cross(arc->getyaxis()).normalize().dot((view->geteye() - view->getref()).normalize())) > cos(0.01))
                          visibility.set(view->getviewno());
                     }
                 hd.setvisible(visibility);
                 for (i = 0 ; i < (rbdata->ymesh/2)*2+1-(arc->getsweepa() >= M_PI*2.0) ; i++)
                   {  p3 = arc->getorigin();
                      p4 = arc->position(double(i) / double((rbdata->ymesh/2)*2));
                      Line line(hd,p4+(p3-p4).normalize()*rbdata->radius,p4+(p4-p3).normalize()*rbdata->radius);
                      line.draw(DM_INVERT);
                   }
                 delete [] namelist;
              }
         }
       else
         arc->draw(DM_INVERT);
       delete arc;
    }
}
#endif
#endif

/**
 * @brief surfaceofrev_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert Elbow command
 *
 *
 */
void surfaceofrev_command2(int *cstate,HighLevelEvent *event,void **data)
{RBRev2_data *rbdata = (RBRev2_data *) *data;
 Dialog *dialog;
 Point p2;
 Circle *arc,*circle;
 SurfaceOfRev *sr;
 ResourceString rs4(NCREVB+4);
 ResourceString rs5(NCREVB+5);
 ResourceString rs6(NCREVB+6);
 ResourceString rs7(NCREVB+7);

  if (*cstate == InitialState && ((NewCommandEvent *)event)->getrepeat() == 0 && ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000 || (GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000))
    {  ::state.sendevent(new NewCommandEvent(411,0));
       *cstate = ExitState;
       return;
    }

  switch (*cstate)
    {  case InitialState :
         state.destroyselection(1);
         if ((rbdata = new RBRev2_data) == NULL)
           {  state.fail(Memory,rs4.gets());
              *cstate = ExitState;
           }
         else
           {  rbdata->radius = v.getreal("sr::radius");
              rbdata->xmesh = v.getinteger("sr::xmesh");
              rbdata->ymesh = v.getinteger("sr::ymesh");

#ifdef USING_WIDGETS
              rbdata->qgarc=0; rbdata->qgcircle=0; rbdata->qgline=0; rbdata->qgsr=0;
              rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
#endif

              if (db.grid.getplane() == onviewplane || db.workplanes.getcurrent() == NULL)
                {  rbdata->xaxis.setxyz(1.0,0.0,0.0);
                   rbdata->yaxis.setxyz(0.0,1.0,0.0);
                }
              else
                {  rbdata->xaxis = db.workplanes.getcurrent()->getxaxis();
                   rbdata->yaxis = db.workplanes.getcurrent()->getyaxis();
                }
              if ((dialog = new Dialog(rs5.gets(),rs6.gets(),&rbdata->radius,ScaleByLength)) == NULL)
                {  state.fail(Memory,rs7.gets());
                   *cstate = ExitState;
                }
              else if (dialog->process() == TRUE)
                {  v.setreal("sr::radius",rbdata->radius);
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
           {  rbdata->origin = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              arc = (Circle *) ((EntitySelectedEvent *)event)->getentity();
              if (create_elbow(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,arc,&circle,&sr))
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   db.geometry.add(circle);
                   db.geometry.add(sr);
                   db.saveundo(UD_ENDBLOCK,NULL);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->xaxis = ((Coordinate3dEvent *) event)->getxaxis();
              rbdata->yaxis = ((Coordinate3dEvent *) event)->getyaxis();

              if (calculate_arc(rbdata,&arc))
                {  delete arc;
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
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              if (calculate_arc(rbdata,&arc))
                {  delete arc;
                   *cstate = 4;
                }
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
              if (calculate_arc(rbdata,&arc))
                {  if (create_elbow(rbdata->radius,(rbdata->xmesh/2)*2+1,(rbdata->ymesh/2)*2+1,arc,&circle,&sr))
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        db.geometry.add(circle);
                        db.geometry.add(sr);
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                   delete arc;
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
  if(rbdata)
  {
      if(rbdata->qgarc)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgarc);
      delete rbdata->qgarc; rbdata->qgarc=0;
      if(rbdata->qgcircle)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcircle);
      delete rbdata->qgcircle; rbdata->qgcircle=0;
      if(rbdata->qgline)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
      delete rbdata->qgline; rbdata->qgline=0;
      if(rbdata->qgsr)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgsr);
      delete rbdata->qgsr; rbdata->qgsr=0;
  }
#endif
#endif

  if (*cstate == 1)
    state.selmask.entity.clearandset(xyz_mask,circle_entity,end_list);
  else if (*cstate >= 2 && *cstate <= 4)
    {  rbdata->state = *cstate;
       rubberband.begin(0);
       rubberband.add(RBRev2,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(NCREVB+8);
  else if (*cstate == 2)
    cadwindow->prompt->normalprompt(NCREVB+9);
  else if (*cstate == 3)
    cadwindow->prompt->normalprompt(NCREVB+10);
  else if (*cstate == 4)
    cadwindow->prompt->normalprompt(NCREVB+11);
  else
    delete rbdata;
}

#endif

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief surfaceofrev_command3
 * @param cstate
 * @param event
 *
 * Entry point for the Surface of Revolution Options command
 *
 */
void surfaceofrev_command3(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 int xmesh,ymesh;
 BitMask options(32),change(32);
 double angle,deltaz;
 Entity *e;
 Point p;
 EntityHeader header;
 ResourceString rs12(NCREVB+12),rs13(NCREVB+13);
 int status,length;
 SurfaceOfRev *surfaceofrev;

  if (*cstate == InitialState)
    {  list = state.getselection().copy();
       if (list.empty())
         {  SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);
            status = SurfaceOfRev::setup(NULL,&angle,&deltaz,&xmesh,&ymesh,&options);
            if (status == TRUE)
              {  SurfaceOfRev::setdefaults(angle,deltaz,xmesh,ymesh,options);
                 *cstate = ExitState;
              }
            else if (status == 112) // clone (105->112)
              *cstate = 1;
            else if (status == 113) // clone all (106->113)
              *cstate = 3;
            else if (status == 110) // surface properties
                *cstate = ExitState;
            else if (status == 111) // triangulate
                *cstate = ExitState;
            else
              *cstate = ExitState;
         }
       else
         {  SurfaceOfRev::getdefaults(&angle,&deltaz,&xmesh,&ymesh,&options);
            status = SurfaceOfRev::setup(&change,&angle,&deltaz,&xmesh,&ymesh,&options,&header);
            if (status == TRUE)
              {  length = list.length();
                 RemoveLockedEntities(&list,0);

                 db.saveundo(UD_STARTBLOCK,NULL);
                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(surfaceofrev_entity))
                     ((SurfaceOfRev *)e)->change(change,header,p,p,angle,deltaz,xmesh,ymesh,options);
                 db.saveundo(UD_ENDBLOCK,NULL);

                 if (list.length() != length)
                   cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

                 list.destroy();

                 *cstate = ExitState;
              }   
            else if (status == 112) // clone (105->112)
              *cstate = 2;
            else if (status == 113) // clone all (106->113)
              *cstate = 4;
            else if (status == 110) // surface properties
            {
                //SurfaceProperties sp;
                //sp.verify(this,0);
                *cstate = ExitState;
            }
            else if (status == 111) // triangulate
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
                    if ((tsurf = ((SurfaceOfRev*)e)->triangulatedclone(1)) != 0)
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
            surfaceofrev = (SurfaceOfRev *)((EntitySelectedEvent *) event)->getentity();
            if (*cstate == 1 || *cstate == 3) 
              {  SurfaceOfRev::setdefaults(surfaceofrev->getangle(),surfaceofrev->getdeltaz(),surfaceofrev->getxmesh(),surfaceofrev->getymesh(),surfaceofrev->getoptions());
                 if (*cstate == 3)
                   {  db.header.change(surfaceofrev->getcolour(),surfaceofrev->getlayer(),surfaceofrev->getstyle(),surfaceofrev->getweight());
                      v.setinteger("df::colour",surfaceofrev->getcolour());
                      v.setinteger("df::layer",surfaceofrev->getlayer());
                      v.setinteger("df::style",surfaceofrev->getstyle());
                      v.setinteger("df::weight",surfaceofrev->getweight());
                      if (cadwindow->getlayerwindow() != 0)
                        cadwindow->getlayerwindow()->paintevent();
                      cadwindow->updatetitle();
                   }   
              }
            else
              {  if (*cstate == 4)
                   for (int i = 0 ; i < 4 ; i++) 
                     change.set(i,1);
                 for (int i = 6 ; i <= 10 ; i++) change.set(i,1);
                 db.saveundo(UD_STARTBLOCK,NULL);

                 length = list.length();
                 RemoveLockedEntities(&list,0);

                 for (list.start() ; (e = list.next()) != NULL ; )
                   if (e->isa(surfaceofrev_entity))
                     ((SurfaceOfRev *)e)->change(change,*surfaceofrev,p,p,surfaceofrev->getangle(),surfaceofrev->getdeltaz(),surfaceofrev->getxmesh(),surfaceofrev->getymesh(),surfaceofrev->getoptions());
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
    {  state.selmask.entity.clearandset(surfaceofrev_entity,end_list);
       cadwindow->prompt->normalprompt("Select the surface of revolution entity to clone from");
    }

}

void insertspiral_command(int *cstate,HighLevelEvent *,void **)
{
    QLibrary *appinstance;
    runcommand RunCommand;

    if(*cstate == InitialState)
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libInsertSpiral.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("InsertSpiral.dll")));
#endif

        if(appinstance)
        {
            RunCommand = (runcommand)appinstance->resolve("runcommand");
            *cstate = ExitState;
            if(RunCommand)
                RunCommand((char*)"Insert Spiral"); // Insert Spiral function
        }
        else
        {
            // put up a message to tell the user the Insert Spiral module is not installed
            QMessageBox::warning(0,"Insert Spiral","The Insert Spiral module is not installed\nContact CAD International Support for more information");
            *cstate = ExitState;
        }
    }
}
