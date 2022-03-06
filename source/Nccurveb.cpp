
#include "ncwin.h"

void curve_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match(_RCT("Curve")));
  *cstate = ExitState;
}

#define NPOINT_INC 20

struct RBCurve1_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcurve;
    QGraphicsItem *qgline;
#endif
    int n,degree,closed,max;
   Point *polygon;
};

#ifdef NEW_RUBBERB
void RBCurve1(int mode,void *data,Point *p2,View3dSurface *)
{
 RBCurve1_data *rbdata = (RBCurve1_data *) data;
 BitMask options(32);
  rbdata->polygon[rbdata->n] = *p2;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (rbdata->n == 0)
    {}
  else if (rbdata->n == 1)
    {  Line line(rbdata->polygon[0],rbdata->polygon[1]);

       line.draw(DM_INVERT);
    }
  else if (rbdata->n+rbdata->closed < rbdata->degree)
    {  rbdata->polygon[rbdata->n+1] = rbdata->polygon[0];
       Curve curve(rbdata->n+1+rbdata->closed,rbdata->n,rbdata->polygon,NULL,NULL,options);
       if (curve.getdefined())
       {
           curve.draw(DM_INVERT);
       }
    }
  else
    {  rbdata->polygon[rbdata->n+1] = rbdata->polygon[0];
       Curve curve(rbdata->n+1+rbdata->closed,rbdata->degree,rbdata->polygon,NULL,NULL,options);
       if (curve.getdefined())
       {
           curve.draw(DM_INVERT);
       }
    }
}
#else
void RBCurve1(int,void *data,Point *p2,View3dSurface *)
{
 RBCurve1_data *rbdata = (RBCurve1_data *) data;
 BitMask options(32);
  rbdata->polygon[rbdata->n] = *p2;
  if (rbdata->n == 0)
    {}
  else if (rbdata->n == 1)
    {  Line line(rbdata->polygon[0],rbdata->polygon[1]);

       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
    }
  else if (rbdata->n+rbdata->closed < rbdata->degree)
    {  rbdata->polygon[rbdata->n+1] = rbdata->polygon[0];
       Curve curve(rbdata->n+1+rbdata->closed,rbdata->n,rbdata->polygon,NULL,NULL,options);
       if (curve.getdefined())
       {
           curve.qgi = rbdata->qgcurve; curve.zValue = rbdata->zValue;
           curve.draw(DM_INVERT);
           rbdata->qgcurve = curve.qgi; rbdata->zValue = curve.zValue;
       }
    }
  else
    {  rbdata->polygon[rbdata->n+1] = rbdata->polygon[0];
       Curve curve(rbdata->n+1+rbdata->closed,rbdata->degree,rbdata->polygon,NULL,NULL,options);
       if (curve.getdefined())
       {
           curve.qgi = rbdata->qgcurve; curve.zValue = rbdata->zValue;
           curve.draw(DM_INVERT);
           rbdata->qgcurve = curve.qgi; rbdata->zValue = curve.zValue;
       }
    }
}
#endif

/**
 * @brief curve_command1
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert curve (by control points) command
 *
 */
void curve_command1(int *cstate,HighLevelEvent *event,void **data)
{
 RBCurve1_data *rbdata = (RBCurve1_data *) *data;
 BitMask options(32);
 int i;
 ResourceString rs0(NCCURVEB+0),rs1(NCCURVEB+1),rs2(NCCURVEB+2),rs3(NCCURVEB+3);
 DialogControl *dc;

  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBCurve1_data) == NULL || (rbdata->polygon = new Point[NPOINT_INC]) == NULL)
           {
             rbdata->qgcurve = 0;
             rbdata->qgline = 0;
             state.fail(Memory,rs0.gets());
              *cstate = ExitState;
           }
         else
           {  *data = rbdata;
              rbdata->n = 0;  rbdata->max = NPOINT_INC;
              rbdata->degree = v.getinteger("cu::degree");
              rbdata->closed = v.getinteger("cu::closed");
              rbdata->qgcurve = 0;
              rbdata->qgline = 0;
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              Dialog dialog("Curve_Dialog");
              dialog.add(dc = new IntegerDialogControl(100,&rbdata->degree,1,10));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(new CheckBoxDialogControl(101,&rbdata->closed)); 
              if (dialog.process() == TRUE)
                {  v.setinteger("cu::degree",rbdata->degree);
                   v.setinteger("cu::closed",rbdata->closed);
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->polygon[rbdata->n++] = ((Coordinate3dEvent *) event)->getp();
              if (rbdata->n+1 >= rbdata->max)
                {Point *polygon;
                   if ((polygon = new Point[rbdata->max + NPOINT_INC]) == NULL)
                     {  state.fail(Memory,rs3.gets());
                        *cstate = ExitState;
                     }
                   else
                     {  for (i = 0 ; i < rbdata->n ; i++)
                          polygon[i] = rbdata->polygon[i];
                        delete [] rbdata->polygon;
                        rbdata->polygon = polygon;
                        rbdata->max += NPOINT_INC;
                     }
                }
           }
         else if (event->isa(NewCommand))
           {  if (rbdata->n > 0)
                {  state.setposition(rbdata->polygon[rbdata->n-1]);
                   rbdata->n--;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  if (rbdata->n <= 2)
                *cstate = ExitState;
              else if (rbdata->n >= 3)
                {  rbdata->polygon[rbdata->n] = rbdata->polygon[0];
                   if (rbdata->n + rbdata->closed < rbdata->degree + 1)
                     {  Curve *curve = new Curve(rbdata->n + rbdata->closed,rbdata->n-1,rbdata->polygon,NULL,NULL,options);
                        if (curve != NULL && curve->getdefined())
                        {
                          curve->zValue = rbdata->zValue;
                          db.geometry.add(curve);
                        }
                        else
                          delete curve;
                     }
                   else
                     {  Curve *curve = new Curve(rbdata->n + rbdata->closed,rbdata->degree,rbdata->polygon,NULL,NULL,options);
                        if (curve != NULL && curve->getdefined())
                        {
                           curve->zValue = rbdata->zValue;
                           db.geometry.add(curve);
                        }
                        else
                          delete curve;
                     }
                   rbdata->n = 0;
                }
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcurve)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcurve);
  delete rbdata->qgcurve; rbdata->qgcurve=0;
  if(rbdata->qgline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
  delete rbdata->qgline; rbdata->qgline=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBCurve1,rbdata);
       state.selmask.entity.clearandset(xyz_mask,undo_mask,end_list);
       if (rbdata->n == 0)
         cadwindow->prompt->normalprompt(NCCURVEB+4);
       else
         cadwindow->prompt->normalprompt(NCCURVEB+5);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL)  delete [] rbdata->polygon;
       delete rbdata;
    }
}

struct RBCurve2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcurve;
#endif
   Curve *curve,*copy;
   EntityList plist;
   int n;
};

#ifdef NEW_RUBBERB
void RBCurve2(int mode,void *data,Point *p2,View3dSurface *)
{
  RBCurve2_data *rbdata = (RBCurve2_data *) data;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  rbdata->copy->movepoint(0,rbdata->n,*p2);

  rbdata->copy->draw(DM_INVERT);
}
#else
void RBCurve2(int,void *data,Point *p2,View3dSurface *)
{
  RBCurve2_data *rbdata = (RBCurve2_data *) data;
  rbdata->copy->movepoint(0,rbdata->n,*p2);

  rbdata->copy->qgi = rbdata->qgcurve; rbdata->copy->zValue = rbdata->zValue;
  rbdata->copy->draw(DM_INVERT);
  rbdata->qgcurve = rbdata->copy->qgi; rbdata->zValue = rbdata->copy->qgi->zValue();
}
#endif

/**
 * @brief curve_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Move curve control point command
 *
 */
void curve_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBCurve2_data *rbdata = (RBCurve2_data *) *data;
 EntityList list;
 Entity *e;
 int i,imin;
 Transform t;
 View3dSurface *surface;
 Point p1,p2;
 double d,dmin;
 ResourceString rs6(NCCURVEB+6),rs7(NCCURVEB+7),rs8(NCCURVEB+8);
 ResourceString rs9(NCCURVEB+9),rs10(NCCURVEB+10),rs11(NCCURVEB+11);
 ResourceString rs12(NCCURVEB+12),rs13(NCCURVEB+13),rs14(NCCURVEB+14);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBCurve2_data) == NULL)
           {
             rbdata->qgcurve=NULL;
             state.fail(Memory,rs6.gets());
              *cstate = ExitState;
           }
         else if (state.getselection().length() == 0)
           {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONINFORMATION);
              rbdata->copy = rbdata->curve = NULL;
              rbdata->qgcurve=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = ExitState;
           }
         else if (state.getselection().length() > 1)
           {  cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
              rbdata->copy = rbdata->curve = NULL;
              rbdata->qgcurve=NULL; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = ExitState;
           }
         else
           {Transform ident;
              ident.identity();
              list = state.getselection();
              list.start();  e = list.next();
              if (e->isa(curve_entity))
                {
                  rbdata->qgcurve=0;
                  rbdata->curve = (Curve *) e;  rbdata->curve->setcopye(NULL);
                   rbdata->copy = (Curve *) rbdata->curve->clone(ident);
                   t.identity();
                   for (i = 0 ; i < rbdata->curve->npoints() ; i++)
                     {  e = new PointE(*rbdata->curve,rbdata->curve->point(i));
                        rbdata->plist.add(e);
                        e->zValue = rbdata->zValue;
                        db.geometry.add(e,3,0);
                     }
                   state.destroyselection(1);
                   *cstate = 1;
                }
              else
                {  cadwindow->MessageBox(rs11.gets(),rs12.gets(),MB_ICONINFORMATION);
                   rbdata->copy = rbdata->curve = NULL;
                   *cstate = ExitState;
                }
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinateuv))
           {  if ((surface = (View3dSurface *) ((CoordinateuvEvent *) event)->window()) != NULL)
                {  p1 = surface->uvtoscreen(((CoordinateuvEvent *) event)->coordinate());
                   p1.y = surface->getheight() - p1.y;
                   p1.z = 0.0;
                   for (i = 0,dmin = -1.0 ; i < rbdata->curve->npoints() ; i++)
                     {  p2 = surface->modeltoscreen(rbdata->curve->point(i));
                        p2.z = 0.0;
                        d = (p2 - p1).length();
                        if (i == 0 || d < dmin)
                          {  imin = i;  dmin = d;
                          }
                     }
                   if (dmin < 10.0)
                     {  rbdata->n = imin;
                        state.setposition(rbdata->curve->point(imin));
                        *cstate = 2;
                     }
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
           db.geometry.del(e,1,0);
         rbdata->plist.destroy();
         if (event->isa(Coordinate3d))
           rbdata->curve->movepoint(1,rbdata->n,((Coordinate3dEvent *) event)->getp());
         delete rbdata->copy;
         t.identity();  rbdata->curve->setcopye(NULL);
         rbdata->copy = (Curve *) rbdata->curve->clone(t);
         for (i = 0 ; i < rbdata->curve->npoints() ; i++)
           {  e = new PointE(*rbdata->curve,rbdata->curve->point(i));
              rbdata->plist.add(e);
              e->zValue = rbdata->zValue;
              db.geometry.add(e,3,0);
           }
         *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcurve)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcurve);
  delete rbdata->qgcurve; rbdata->qgcurve=0;
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCCURVEB+13);
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       rubberband.add(RBCurve2,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCCURVEB+14);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->curve != 0)
              {  for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                   db.geometry.del(e,1,0);
                 rbdata->plist.destroy();
              }
            delete rbdata->copy;
            delete rbdata;
         }
    }
}

/**
 * @brief curve_command3
 * @param cstate
 *
 * Entry point for the Change curve degree command
 *
 */
void curve_command3(int *cstate,HighLevelEvent *,void **)
{int degree;
 ResourceString rs15(NCCURVEB+15),rs16(NCCURVEB+16),rs17(NCCURVEB+17),rs25(NCCURVEB+25);
 Dialog dialog(rs16.gets(),rs17.gets(),&degree,1,99);
 BitMask options(32),change(32);
 EntityList list;
 Entity *e;
 EntityHeader header;
  *cstate = ExitState;
  if (state.getselection().length() == 0)
    {  cadwindow->MessageBox(rs25.gets(),rs16.gets(),MB_ICONINFORMATION);
       return;
    }
  list = state.getselection();
  for (list.start() ; (e = list.next()) != NULL ; )
    if (! e->isa(curve_entity))
      {  cadwindow->MessageBox(rs15.gets(),rs16.gets(),MB_ICONINFORMATION);
         return;
      }
  degree = v.getinteger("cu::degree");
  if (dialog.process() == FALSE) return;
  v.setinteger("cu::degree",degree);
  db.saveundo(UD_STARTBLOCK,NULL);
  change.set(4);
  for (list.start() ; (e = list.next()) != NULL ; )
    ((Curve *)e)->change(change,header,degree,options);
  db.saveundo(UD_ENDBLOCK,NULL);
}

struct RBCurve4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcurve;
    QGraphicsItem *qgline;
#endif
   int n,max;
   Point *points;
};

#ifdef NEW_RUBBERB
void RBCurve4(int mode,void *data,Point *p2,View3dSurface *)
{RBCurve4_data *rbdata = (RBCurve4_data *) data;
 BitMask options(32);

 if(mode == RB_ERASE)
 {
     cadwindow->clearAllRubberBands();
     return;
 }

  rbdata->points[rbdata->n] = *p2;
  if (rbdata->n == 1)
    {  Line line(rbdata->points[0],rbdata->points[1]);

       line.draw(DM_INVERT);
    }
  else if (rbdata->n > 1)
    {  Curve curve(rbdata->n+1,3,rbdata->points,options);
       if (curve.getdefined())
       {
            curve.draw(DM_INVERT);
       }
    }
}
#else
void RBCurve4(int,void *data,Point *p2,View3dSurface *)
{RBCurve4_data *rbdata = (RBCurve4_data *) data;
 BitMask options(32);
  rbdata->points[rbdata->n] = *p2;
  if (rbdata->n == 1)
    {  Line line(rbdata->points[0],rbdata->points[1]);

       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
    }
  else if (rbdata->n > 1)
    {  Curve curve(rbdata->n+1,3,rbdata->points,options);
       if (curve.getdefined())
       {
           curve.qgi = rbdata->qgcurve; curve.zValue = rbdata->zValue;
            curve.draw(DM_INVERT);
           rbdata->qgcurve = curve.qgi; rbdata->zValue = curve.zValue;
       }
    }
}
#endif

/**
 * @brief curve_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Entry point for the Insert a curve through points command
 *
 */
void curve_command4(int *cstate,HighLevelEvent *event,void **data)
{
 RBCurve4_data *rbdata = (RBCurve4_data *) *data;
 BitMask options(32);
 int i;
 ResourceString rs0(NCCURVEB+0),rs1(NCCURVEB+1),rs2(NCCURVEB+2),rs3(NCCURVEB+3);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBCurve4_data) == NULL || (rbdata->points = new Point[NPOINT_INC]) == 0)
         {
             rbdata->qgcurve=0; rbdata->qgline=0; rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
             *cstate = ExitState;
         }
         else
           {
             *data = rbdata;
              rbdata->n = 0;  rbdata->max = NPOINT_INC;
              rbdata->qgcurve=0; rbdata->qgline=0;rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = 1;
           }
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d) && (rbdata->n == 0 || (rbdata->points[rbdata->n-1] - ((Coordinate3dEvent *) event)->getp()).length() > db.getptoler()))
           {  rbdata->points[rbdata->n++] = ((Coordinate3dEvent *) event)->getp();
              if (rbdata->n >= rbdata->max)
                {Point *points;
                   if ((points = new Point[rbdata->max + NPOINT_INC]) == 0)
                     *cstate = ExitState;
                   else
                     {  for (i = 0 ; i < rbdata->n ; i++)
                          points[i] = rbdata->points[i];
                        delete [] rbdata->points;
                        rbdata->points = points;
                        rbdata->max += NPOINT_INC;
                     }
                }
           }
         else if (event->isa(NewCommand))
           {  if (rbdata->n > 0)
                {  state.setposition(rbdata->points[rbdata->n-1]);
                   rbdata->n--;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  if (rbdata->n <= 2)
                *cstate = ExitState;
              else if (rbdata->n >= 3)
                {  Curve *curve = new Curve(rbdata->n,3,rbdata->points,options);
                   if (curve != NULL && curve->getdefined())
                   {
                     curve->zValue = rbdata->zValue;
                     db.geometry.add(curve);
                   }
                   else
                     delete curve;
                   rbdata->n = 0;
                }
           }
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcurve)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcurve);
  delete rbdata->qgcurve; rbdata->qgcurve=0;
  if(rbdata->qgline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
  delete rbdata->qgline; rbdata->qgline=0;
#endif
#endif
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBCurve4,rbdata);
       state.selmask.entity.clearandset(xyz_mask,undo_mask,end_list);
       if (rbdata->n == 0)
         cadwindow->prompt->normalprompt(NCCURVEB+18);
       else
         cadwindow->prompt->normalprompt(NCCURVEB+19);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL)  delete [] rbdata->points;
       delete rbdata;
    }
}

struct RBCurve5_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgcurve;
    QGraphicsItem *qgline;
    QGraphicsItem *qgpoint;
    Point point;
    PointE *pe;
#endif
   Curve *curve,*copy;
   EntityList plist;
   int state,n1,n2;
};

#ifdef NEW_RUBBERB
void RBCurve5(int mode,void *data,Point *p2,View3dSurface *)
{
  RBCurve5_data *rbdata = (RBCurve5_data *) data;

  if(mode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }

  if (rbdata->state == 2)
    {  Line line(rbdata->curve->point(rbdata->n1),*p2);

       line.draw(DM_INVERT);
    }
  else if (rbdata->state == 3)
    {
       rbdata->copy->movepoint(0,rbdata->n1+1,*p2);

       PointE point(*p2);
       point.draw(DM_INVERT);

       rbdata->copy->draw(DM_INVERT);
    }
}
#else
void RBCurve5(int,void *data,Point *p2,View3dSurface *)
{RBCurve5_data *rbdata = (RBCurve5_data *) data;
  if (rbdata->state == 2)
    {  Line line(rbdata->curve->point(rbdata->n1),*p2);

       line.qgi = rbdata->qgline; line.zValue = rbdata->zValue;
       line.draw(DM_INVERT);
       rbdata->qgline = line.qgi; rbdata->zValue = line.zValue;
    }
  else if (rbdata->state == 3)
    {
       rbdata->copy->movepoint(0,rbdata->n1+1,*p2);

       PointE point(*p2);
       point.qgi = rbdata->qgpoint; point.zValue = rbdata->zValue;
       point.draw(DM_INVERT);
       rbdata->qgpoint = point.qgi; rbdata->zValue = point.zValue;

       rbdata->copy->qgi = rbdata->qgcurve; rbdata->copy->zValue = rbdata->zValue;
       rbdata->copy->draw(DM_INVERT);
       rbdata->qgcurve = rbdata->copy->qgi; rbdata->zValue = rbdata->copy->zValue;
    }
}
#endif
/**
 * @brief curve_command5
 * @param cstate
 * @param event
 * @param data
 *
 * entry point for Insert a new control point on an existing curve
 *
 */
void curve_command5(int *cstate,HighLevelEvent *event,void **data)
{
 RBCurve5_data *rbdata = (RBCurve5_data *) *data;
 EntityList list;
 Entity *e;
 int i,imin;
 Transform t;
 View3dSurface *surface;
 Point p1,p2,pmin;
 double d,d1,d2,dmin;
 ResourceString rs7(NCCURVEB+7),rs23(NCCURVEB+23);
 ResourceString rs9(NCCURVEB+9),rs10(NCCURVEB+10),rs11(NCCURVEB+11);
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBCurve5_data) == 0)
         {
           rbdata->qgcurve=0; rbdata->qgline=0;rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
           *cstate = ExitState;
         }
         else if (state.getselection().length() == 0)
           {  cadwindow->MessageBox(rs7.gets(),rs23.gets(),MB_ICONINFORMATION);
              rbdata->copy = rbdata->curve = NULL;
              rbdata->qgcurve=0; rbdata->qgline=0;rbdata->qgpoint=0;rbdata->pe=NULL;
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = ExitState;
           }
         else if (state.getselection().length() > 1)
           {  cadwindow->MessageBox(rs9.gets(),rs23.gets(),MB_ICONINFORMATION);
              rbdata->copy = rbdata->curve = NULL;
              rbdata->qgcurve=0; rbdata->qgline=0;rbdata->qgpoint=0;rbdata->pe=NULL;
              rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
              *cstate = ExitState;
           }
         else
           {Transform ident;
              ident.identity();
              list = state.getselection();
              list.start();  e = list.next();
              if (e->isa(curve_entity))
              {
                   rbdata->qgcurve=0; rbdata->qgline=0;rbdata->qgpoint=0;rbdata->pe=NULL;
                   rbdata->curve = (Curve *) e;  rbdata->curve->setcopye(NULL);
                   rbdata->copy = (Curve *) rbdata->curve->clone(ident);
                   rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
                   t.identity();
                   for (i = 0 ; i < rbdata->curve->npoints() ; i++)
                   {
                        e = new PointE(*rbdata->curve,rbdata->curve->point(i));
                        rbdata->plist.add(e);
                        e->zValue = rbdata->zValue;
                        db.geometry.add(e,3,0);
                   }
                   state.destroyselection(1);
                   *cstate = 1;
                }
              else
                {  cadwindow->MessageBox(rs11.gets(),rs23.gets(),MB_ICONINFORMATION);
                   rbdata->copy = rbdata->curve = NULL;
                   *cstate = ExitState;
                }
           }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(Coordinateuv))
           {  if ((surface = (View3dSurface *) ((CoordinateuvEvent *) event)->window()) != NULL)
                {  p1 = surface->uvtoscreen(((CoordinateuvEvent *) event)->coordinate());
                   p1.y = surface->getheight() - p1.y;
                   p1.z = 0.0;
                   for (i = 0,dmin = -1.0 ; i < rbdata->curve->npoints() ; i++)
                     {  p2 = surface->modeltoscreen(rbdata->curve->point(i));
                        p2.z = 0.0;
                        d = (p2 - p1).length();
                        if (i == 0 || d < dmin)
                          {  imin = i;  dmin = d;
                          }
                     }
                   if (dmin < 10.0)
                     {  rbdata->n1 = imin;
                       state.setposition(rbdata->curve->point(imin));
                       //

                       //
                        *cstate = 2;
                     }
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinateuv))
           {  if ((surface = (View3dSurface *) ((CoordinateuvEvent *) event)->window()) != NULL)
                {  p1 = surface->uvtoscreen(((CoordinateuvEvent *) event)->coordinate());
                   p1.y = surface->getheight() - p1.y;
                   p1.z = 0.0;
                   if (rbdata->n1 > 0)
                     {  p2 = surface->modeltoscreen(rbdata->curve->point(rbdata->n1-1));
                        p2.z = 0.0;
                        d1 = (p2 - p1).length();
                     }
                   else
                     d1 = MAXDOUBLE;
                   if (rbdata->n1 < rbdata->curve->getn() - 1)
                     {  p2 = surface->modeltoscreen(rbdata->curve->point(rbdata->n1+1));
                        p2.z = 0.0;
                        d2 = (p2 - p1).length();
                     }
                   else
                     d2 = MAXDOUBLE;
                   if (d1 < d2)
                     {  rbdata->n2 = rbdata->n1 - 1;
                        *cstate = 3;
                     }
                   else if (d1 > d2)
                     {  rbdata->n2 = rbdata->n1 + 1;
                        *cstate = 3;
                     }
                   if (*cstate == 3)
                     {  if (rbdata->n2 < rbdata->n1)
                          {int t;
                             t = rbdata->n1;  rbdata->n1 = rbdata->n2;  rbdata->n2 = t;
                          }
                        rbdata->copy->insertpoint(0,rbdata->n1,rbdata->n2,p1);
                        //
                        rbdata->point = p1;
                        //

                        for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                          db.geometry.del(e,1,0);
                        rbdata->plist.destroy();
                        for (i = 0 ; i < rbdata->copy->npoints() ; i++)
                        {
                            if(!(rbdata->copy->point(i) == p1))
                            {
                                e = new PointE(*rbdata->copy,rbdata->copy->point(i));
                                rbdata->plist.add(e);
                                e->zValue = rbdata->zValue;
                                db.geometry.add(e,3,0);
                            }
                        }
                     }
                }
           }
         else if (event->isa(Abort))
              *cstate = 1;
         else if (event->isa(Exit))
              *cstate = ExitState;

         break;
       case 3 :
         rubberband.end(0);
         for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
             db.geometry.del(e,1,0);
           rbdata->plist.destroy();
         if (event->isa(Coordinate3d))
           rbdata->curve->insertpoint(1,rbdata->n1,rbdata->n2,((Coordinate3dEvent *) event)->getp());

         delete rbdata->copy;
         t.identity();  rbdata->curve->setcopye(NULL);
         rbdata->copy = (Curve *) rbdata->curve->clone(t);
         for (i = 0 ; i < rbdata->curve->npoints() ; i++)
         {
              e = new PointE(*rbdata->curve,rbdata->curve->point(i));
              rbdata->plist.add(e);
              e->zValue = rbdata->zValue;
              db.geometry.add(e,3,0);
         }
         *cstate = 1;
         break;
    }
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
  if(rbdata->qgcurve)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgcurve);
  delete rbdata->qgcurve; rbdata->qgcurve=0;
  if(rbdata->qgline)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgline);
  delete rbdata->qgline; rbdata->qgline=0;
  if(rbdata->qgpoint)
      cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgpoint);
  delete rbdata->qgpoint; rbdata->qgpoint=0;
#endif
#endif
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCCURVEB+20);
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       rubberband.begin(0);
       rubberband.add(RBCurve5,rbdata);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCCURVEB+21);
    }
  else if (*cstate == 3)
    {  rbdata->state = 3;
       rubberband.begin(0);
       rubberband.add(RBCurve5,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCCURVEB+22);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != NULL)
         {  if (rbdata->curve != NULL)
              {
                for (rbdata->plist.start() ; (e = rbdata->plist.next()) != 0 ; )
                    db.geometry.del(e,1,0);
                rbdata->plist.destroy();
              }
            delete rbdata->copy;
            delete rbdata;
         }
    }
}

class CCIntegerDialogControl : public IntegerDialogControl
  {public:
     CCIntegerDialogControl(int id,int *v,int min,int max) :
       IntegerDialogControl(id,v,min,max) {}
     void changefocus(Dialog *dialog,int updatescreen = 1);
     void vscrollevent(Dialog *dialog,int s1,int s2,int s3);
  };

void CCIntegerDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int degree,ncontrol;
  dialog->currentvalue(100,&degree);
  dialog->currentvalue(101,&ncontrol);
  if (id == 100)
    {  if (degree + 1 > ncontrol)
         ncontrol = degree + 1;
       ((IntegerDialogControl *) dialog->getcontrol(101))->change(dialog,ncontrol);
    }
  else
    {  if (degree + 1 > ncontrol)
         degree = ncontrol - 1;
       ((IntegerDialogControl *) dialog->getcontrol(100))->change(dialog,degree);
    }
}

void CCIntegerDialogControl::vscrollevent(Dialog *dialog,int s1,int s2,int s3)
{ IntegerDialogControl::vscrollevent(dialog,s1,s2,s3);
  changefocus(dialog);
}

/**
 * @brief curve_command6
 * @param cstate
 * Convert lines, arcs or circle to curves
 */
void curve_command6(int *cstate,HighLevelEvent *,void **)
{EntityList list;
 Entity *e;
 int i,ns,degree,ncontrol,deleteentity,nsegments;
 ResourceString rs2(NCMAINB+2),rs24(NCCURVEB+24);
 Dialog dialog("ConvertToCurve_Dialog");
 DialogControl *dc;
 Point *polygon;
 double *knot,*weight;
 BitMask options(32);
  *cstate = ExitState;
  if (state.getselection().length() == 0)
    {  cadwindow->MessageBox(rs24.gets(),rs2.gets(),MB_ICONINFORMATION);
       return;
    }
  degree = v.getinteger("cu::degree");
  ncontrol = v.getinteger("cu::ncontrol");
  deleteentity = v.getinteger("cu::deleteentity");
  dialog.add(dc = new CCIntegerDialogControl(101,&ncontrol,3,9999));
  //dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new CCIntegerDialogControl(100,&degree,2,10));
  //dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(new CheckBoxDialogControl(102,&deleteentity));
  if (dialog.process() == FALSE)
    return;
  v.setinteger("cu::degree",degree);
  v.setinteger("cu::ncontrol",ncontrol);
  v.setinteger("cu::deleteentity",deleteentity);
  list = state.getselection().copy();
  db.saveundo(UD_STARTBLOCK,NULL);
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(line_entity))
      {  if ((polygon = new Point[ncontrol]) != 0)
           {  for (i = 0 ; i < ncontrol ; i++)
                polygon[i] = ((Line *)e)->getp1() + (((Line *)e)->getp2() - ((Line *)e)->getp1()) * double(i) / double(ncontrol-1);
              Curve *curve = new Curve(ncontrol,degree,polygon,0,0,options);
              if (curve != 0 && curve->getdefined())
                {
                   curve->zValue = QGMAXZVALUE+QGZVALINC;
                   db.geometry.add(curve);
                   if (deleteentity && e->getalist().empty())
                     {  state.removefromselection(e);
                        db.geometry.del(e,0);
                     }
                }
              else
                delete curve;
              delete [] polygon;
           }
      }
    else if (e->isa(circle_entity))
      {  ns = (ncontrol - 1) / 2;
         nsegments = int(((Circle *)e)->getsweepa() / M_PI * 2.0 + 1.0);
         if (nsegments < 0) nsegments = 1; else if (nsegments > 4) nsegments = 4;
         if (ns > nsegments)
           nsegments = ns;
         polygon = new Point[nsegments*2+1];
         weight = new double[nsegments*2+1];
         knot = new double[nsegments*2+1+2+1];
         if (polygon != 0 && weight != 0 && knot != 0)
           {  for (i = 0 ; i < nsegments ; i++)
                {  polygon[i*2] = ((Circle *)e)->position(double(i)/double(nsegments));
                   polygon[i*2+2] = ((Circle *)e)->position(double(i+1)/double(nsegments));
                   Line line1(polygon[i*2],polygon[i*2]+((Circle *)e)->direction(double(i)/double(nsegments)));
                   Line line2(polygon[i*2+2],polygon[i*2+2]+((Circle *)e)->direction(double(i+1)/double(nsegments)));
                   Intersect i1(&line1,&line2);
                   if (i1.nintersections() == 1)
                     polygon[i*2+1] = ((PointE *)i1.intersection(0))->getp();
                   else
                     polygon[i*2+1] = (polygon[i*2] + polygon[i*2+2])/2.0;
                   weight[i*2] = 1.0;
                   weight[i*2+1] = cos(((Circle *)e)->getsweepa()/nsegments/2.0);
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
                {
                   curve->zValue = QGMAXZVALUE+QGZVALINC;
                   db.geometry.add(curve);
                   if (deleteentity && e->getalist().empty())
                     {  state.removefromselection(e);
                        db.geometry.del(e,0);
                     }
                }
              else
                delete curve;
           }
         delete [] polygon;  delete [] weight;  delete [] knot;
      }
  db.saveundo(UD_ENDBLOCK,NULL);
}



