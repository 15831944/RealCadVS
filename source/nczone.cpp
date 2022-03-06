
#include "ncwin.h"
#include "RCView.h"

void zoom_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match(_RCT("Zoom")));
  *cstate = ExitState;
}

/**
 * @brief zoom_command1
 * @param cstate
 * @param event
 *
 * zoom window command
 *
 */
void zoom_command1(int *cstate,HighLevelEvent *event,void **)
{static Point p1;
 static View3dWindow *zoomwindow;
 View3dWindow *window;
 Point p2;
 qDebug() << "Entering zoom_command1";
  switch (*cstate)
    {  case InitialState :
         program->setcursorshape(CursorZoom);
         *cstate = 1;
         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                {
                   qDebug() << "in zoom_command1 @1";
                   ((RCView*)window->gethwnd())->freezeBackImage();
                   zoomwindow = window;
                   p1 = ((CoordinateuvEvent *) event)->coordinate();
                   window->rubberbanduv(p1);
                   window->cursoraction(2);
                   window->cursorstyle(0);
                   *cstate = 2;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         cadwindow->cursoraction(0);
         cadwindow->cursorstyle(1);
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                {  if (window == zoomwindow)
                     {
                        qDebug() << "in zoom_command1 @2";
                        ((RCView*)window->gethwnd())->unFreezeBackImage();
                        p2 = ((CoordinateuvEvent *) event)->coordinate();
                        db.savezoomundo(0,zoomwindow);
                        zoomwindow->zoom(p1.x,p1.y,p2.x,p2.y);
                        *cstate = 1;
                     }
                   else
                     {
                        qDebug() << "in zoom_command1 @2.1";
                        ((RCView*)zoomwindow->gethwnd())->unFreezeBackImage();
                        ((RCView*)window->gethwnd())->freezeBackImage();
                        p1 = ((CoordinateuvEvent *) event)->coordinate();
                        window->rubberbanduv(p1);
                        window->cursoraction(2);
                        window->cursorstyle(0);
                        *cstate = 3;
                     }
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         cadwindow->cursoraction(0);
         cadwindow->cursorstyle(1);
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                {
                 qDebug() << "in zoom_command1 @3";
                  ((RCView*)window->gethwnd())->unFreezeBackImage();
                  p2 = ((CoordinateuvEvent *) event)->coordinate();
                   db.savezoomundo(0,zoomwindow);
                   zoomwindow->zoom(p1.x,p1.y,p2.x,p2.y);
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate >= 1 && *cstate <= 3) rubberband.begin(0);
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+116);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+117);
    }
  else if (*cstate == ExitState)
  {
    if(zoomwindow)
       ((RCView*)zoomwindow->gethwnd())->unFreezeBackImage();
    program->setcursorshape(CursorRestore);
  }
  qDebug() << "Leaving zoom_command1 (cstate) : " << *cstate;
}

struct RBZoom2_data
  {View3dWindow *scrollwindow;
   View3dOffScreenBitmap *bitmap;
   double umin,vmin,umax,vmax,lastdu,lastdv,lastscale;
   int scrollstarted;
  };

void RBZoom2(int draw,void *data,int x,int y,View3dSurface *surface)
{
#if 0
    RBZoom2_data *rbdata = (RBZoom2_data *) data;
    double du,dv;
    View3dWindow *w;

    // if the left mouse button is down
    // get the window surface
    // scroll the window like the normal m button scrolling

    qDebug() << "RBZoom2 draw flag : " << draw;

    if (surface == 0)
      return;  //  Timer event, and no dynamic scroll is active

    if( surface != 0 && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
    {
        //  Start dynamic scroll
           //program->setbusy(1);
           rbdata->scrollwindow = (View3dWindow *) surface;
           /*
           if (1 || SetLight == 0)
             {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
                  rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
                else
                  rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
                if (! rbdata->bitmap->getdefined())
                  {  delete rbdata->bitmap;
                     rbdata->bitmap = 0;
                  }
             }
           b = rbdata->bitmap;
           */

           du = ((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
           dv = - ((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());

           rbdata->umin = rbdata->scrollwindow->getumin();
           rbdata->vmin = rbdata->scrollwindow->getvmin();
           rbdata->umax = rbdata->scrollwindow->getumax();
           rbdata->vmax = rbdata->scrollwindow->getvmax();
           rbdata->lastdu = rbdata->lastdv = 0.0;
           db.savezoomundo(0,rbdata->scrollwindow);
    }

    if (draw == RB_ERASE || (w = rbdata->scrollwindow) != surface || surface == 0)
      return;
    if (x >= 0 && y >= 0)
      {  rbdata->lastdu = du = ((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
         rbdata->lastdv = dv = -((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());
      }
    else
      {  du = rbdata->lastdu;
         dv = rbdata->lastdv;
      }
    if (rbdata->umin + du < -1.0E20 || rbdata->umax + du > 1.0E20 ||
        rbdata->vmin + dv < -1.0E20 || rbdata->vmax + dv > 1.0E20)
      return;

    if(rbdata->scrollwindow)
    {
          //du = (rbdata->umax - umin) * (x - downx) / getwidth();
          //dv = (rbdata->vmax - vmin) * (y - downy) / getheight();
          if (du != 0.0 || dv != 0.0)
            rbdata->scrollwindow->zoom(rbdata->umin + du,rbdata->vmin - dv,rbdata->umax + du,rbdata->vmax - dv);
    }
#else
 RBZoom2_data *rbdata = (RBZoom2_data *) data;
 double du,dv;
 View3dWindow *w;
 View3dOffScreenBitmap *b = rbdata->bitmap;

  if (b == 0 && surface == 0)
    return;  //  Timer event, and no dynamic scroll is active

  // not using offscreen bitmap now
  /*
  if (b == 0 && surface != 0 && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
    {//  Start dynamic scroll
       program->setbusy(1);
       rbdata->scrollwindow = (View3dWindow *) surface;
       if (1 || SetLight == 0)
         {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = 0;
              }
         }
       b = rbdata->bitmap;


       du = ((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
       dv = - ((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());

       rbdata->umin = rbdata->scrollwindow->getumin() + du;
       rbdata->vmin = rbdata->scrollwindow->getvmin() + dv;
       rbdata->umax = rbdata->scrollwindow->getumax() + du;
       rbdata->vmax = rbdata->scrollwindow->getvmax() + dv;
       rbdata->lastdu = rbdata->lastdv = 0.0;
       db.savezoomundo(0,rbdata->scrollwindow);
    }

  // not using offscreen bit map now
  if (rbdata->bitmap == 0)
    return;
  */

  if (draw == RB_TIMER)
    surface = rbdata->scrollwindow;
  if (draw == RB_ERASE || (w = rbdata->scrollwindow) != surface || surface == 0)
    return;
  if (draw == RB_TIMER)
    {  du = dv = 0;
       if (x >= 0 && x < 5)
         du =  (rbdata->umax - rbdata->umin) / 20.0;
       if (x > surface->getwidth() - 5)
         du = -(rbdata->umax - rbdata->umin) / 20.0;
       if (y >= 0 && y < 5)
         dv = -(rbdata->vmax - rbdata->vmin) / 20.0;
       if (y > surface->getheight() - 5)
         dv =  (rbdata->vmax - rbdata->vmin) / 20.0;
       rbdata->umin += du;
       rbdata->umax += du;
       rbdata->vmin += dv;
       rbdata->vmax += dv;
    }
  if (x >= 0 && y >= 0)
    {  rbdata->lastdu = du = -((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
       rbdata->lastdv = dv =  ((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());
    }
  else
    {  du = rbdata->lastdu;
       dv = rbdata->lastdv;
    }
  if (rbdata->umin + du < -1.0E20 || rbdata->umax + du > 1.0E20 ||
      rbdata->vmin + dv < -1.0E20 || rbdata->vmax + dv > 1.0E20)
    return;
  if (rbdata->bitmap == NULL)
    {  w->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       if (w->getoffscreenbitmap() != 0)
         w->getoffscreenbitmap()->displaylist.clear();
       w->paintevent();
    }
  else
    {  w->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       b->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       if (w->getsurfacetype() == View2dSurf)
         b->setplotscale(1.0);
       b->paintevent();
       BitBlt(w->gethdc(),0,0,int(w->getwidth()),int(w->getheight()),
              b->gethdc(),0,0,SRCCOPY);
    }
#endif
}

// zoom scroll command
void zoom_command2(int *cstate,HighLevelEvent *event,void **data)
{static Point p1;
 RBZoom2_data *rbdata = (RBZoom2_data *) *data;
 Point p2;
 View3dWindow *window;
 View3dSurface *surface;
 double umin,vmin,umax,vmax;

  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         *data = rbdata = new RBZoom2_data;
         rbdata->bitmap = 0;
         program->setcursorshape(CursorZoom);
         break;
       case 1 :
         rubberband.end(0);
         if (rbdata->bitmap != 0)
           {  program->setbusy(0);
              rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                         rbdata->scrollwindow->getvmin(),
                                         rbdata->scrollwindow->getumax(),
                                         rbdata->scrollwindow->getvmax());
              delete rbdata->bitmap;
              rbdata->bitmap = 0;
              *cstate = event->isa(Coordinateuv) ? 1 : *cstate = ExitState;
           } 
         else if (event->isa(Coordinateuv))
           {  if (((CoordinateuvEvent *)event)->getcontrol() != 0 &&
                  ((CoordinateuvEvent *)event)->window() != 0)
                {  if (rbdata != 0)
                     {  rbdata->scrollwindow = ((CoordinateuvEvent *)event)->window();
                        if (1 || SetLight == 0)
                          {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
                               rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
                             else  
                               rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
                             if (! rbdata->bitmap->getdefined())
                               {  delete rbdata->bitmap;
                                  rbdata->bitmap = 0;
                               }
                          }
                        rbdata->umin = rbdata->scrollwindow->getumin();
                        rbdata->vmin = rbdata->scrollwindow->getvmin();
                        rbdata->umax = rbdata->scrollwindow->getumax();
                        rbdata->vmax = rbdata->scrollwindow->getvmax();
                        rbdata->lastdu = rbdata->lastdv = 0.0;
                        *cstate = 3;
                        db.savezoomundo(0,rbdata->scrollwindow);
                     }
                   else
                     *cstate = ExitState;
                }
              else
                {  p1 = ((CoordinateuvEvent *) event)->coordinate();
                   cadwindow->rubberbanduv(p1);
                   cadwindow->cursoraction(1);
                   *cstate = 2;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         cadwindow->cursoraction(0);
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                {  p2 = ((CoordinateuvEvent *) event)->coordinate();
                   surface = (View3dSurface *) window;
                   umin = surface->getumin() - (p2.x-p1.x);
                   vmin = surface->getvmin() - (p2.y-p1.y);
                   umax = surface->getumax() - (p2.x-p1.x);
                   vmax = surface->getvmax() - (p2.y-p1.y);
                   db.savezoomundo(0,window);
                   window->zoom(umin,vmin,umax,vmax);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         rubberband.end(0);
         program->setbusy(0);
         rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                    rbdata->scrollwindow->getvmin(),
                                    rbdata->scrollwindow->getumax(),
                                    rbdata->scrollwindow->getvmax());
         *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBZoom2,rbdata);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+118);
    }
  else if (*cstate == 2)
    {  rubberband.begin(0);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+119);
    }
  else if (*cstate == 3)
    {  program->setbusy(1);
       state.selmask.entity.clearandset(uv_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBZoom2,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+161);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->bitmap != 0)
              delete rbdata->bitmap;
            delete rbdata;
         }
       program->setcursorshape(CursorRestore);
    }

}

void RBZoom3(int draw,void *data,int,int y,View3dSurface *surface)
{RBZoom2_data *rbdata = (RBZoom2_data *) data;
 double u,v,du,dv,scale;
 View3dWindow *w;
 View3dOffScreenBitmap *b = rbdata->bitmap;

  if (b == 0 && surface == 0)
    return;  //  Timer event, and no dynamic scroll is active

  if (b == 0 && surface != 0 && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
    {//  Start dynamic zoom
       program->setbusy(1);
       rbdata->scrollwindow = (View3dWindow *) surface;
       if (1 || SetLight == 0)
         {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
           if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = 0;
              }
         }
       b = rbdata->bitmap;

       rbdata->umin = rbdata->scrollwindow->getumin();
       rbdata->vmin = rbdata->scrollwindow->getvmin();
       rbdata->umax = rbdata->scrollwindow->getumax();
       rbdata->vmax = rbdata->scrollwindow->getvmax();
       
       u = (rbdata->umax + rbdata->umin) / 2.0;
       du = (rbdata->umax - rbdata->umin) / 2.0;
       v = (rbdata->vmax + rbdata->vmin) / 2.0;
       dv = (rbdata->vmax - rbdata->vmin) / 2.0;
       scale = 0.25 + ((double) y / (double) surface->getheight()) * 1.5;
       if (scale > 0.0)
         {  du /= scale;
            dv /= scale;
         }

       rbdata->umin = u - du;
       rbdata->vmin = v - dv;
       rbdata->umax = u + du;
       rbdata->vmax = v + dv;
       rbdata->lastdu = rbdata->lastdv = 0.0;
       db.savezoomundo(0,rbdata->scrollwindow);
    }

  if (rbdata->bitmap == 0)
    return;

  if (draw == RB_TIMER)
    surface = rbdata->scrollwindow;
  if (draw == RB_ERASE || (w = rbdata->scrollwindow) != surface || surface == 0)
    return;
  u = (rbdata->umax + rbdata->umin) / 2.0;
  du = (rbdata->umax - rbdata->umin) / 2.0;
  v = (rbdata->vmax + rbdata->vmin) / 2.0;
  dv = (rbdata->vmax - rbdata->vmin) / 2.0;
  if (draw == RB_TIMER)
    {  scale = 1.0;
       if (y >= 0 && y < 5)
         scale = 19.0 / 20.0;
       if (y > surface->getheight() - 5)
         scale = 21.0 / 20.0;
       du *= scale;  dv *= scale;
       rbdata->umin = u - du;
       rbdata->umax = u + du;
       rbdata->vmin = v - dv;
       rbdata->vmax = v + dv;
    }
  if (y >= 0)
    rbdata->lastscale = scale = 0.25 + ((double) y / (double) surface->getheight()) * 1.5;
  else
    scale = rbdata->lastscale;
  du *= scale;
  dv *= scale;
  if (du < 1.0E-10 || dv < 1.0E-10 ||
      du >  1.0E20 || dv >  1.0E20)
    return;
  if (rbdata->bitmap == NULL)
    {  w->setuvminmax(u-du,v-dv,u+du,v+dv);
       if (w->getoffscreenbitmap() != 0)
         w->getoffscreenbitmap()->displaylist.clear();
       w->paintevent();
    }
  else
    {  w->setuvminmax(u-du,v-dv,u+du,v+dv);
       b->setuvminmax(u-du,v-dv,u+du,v+dv);
       if (w->getsurfacetype() == View2dSurf)
         b->setplotscale(1.0);
       b->paintevent();
       BitBlt(w->gethdc(),0,0,int(w->getwidth()),int(w->getheight()),
              b->gethdc(),0,0,SRCCOPY);
    }
}

void zoom_command3(int *cstate,HighLevelEvent *event,void **data)
{RBZoom2_data *rbdata = (RBZoom2_data *) *data;
 Point p;
 View3dWindow *window;
 View3dSurface *surface;
 double du,dv;
  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         *data = rbdata = new RBZoom2_data;
         rbdata->bitmap = 0;
         rbdata->scrollwindow = 0;
         program->setcursorshape(CursorZoom);
         break;
       case 1 :
         rubberband.end(0);
         if (rbdata->bitmap != 0)
           {  program->setbusy(0);
              rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                         rbdata->scrollwindow->getvmin(),
                                         rbdata->scrollwindow->getumax(),
                                         rbdata->scrollwindow->getvmax());
              delete rbdata->bitmap;
              rbdata->bitmap = 0;
              *cstate = event->isa(Coordinateuv) ? 1 : *cstate = ExitState;
           } 
         else if (event->isa(Coordinateuv))
           {  if (((CoordinateuvEvent *)event)->getcontrol() != 0 &&
                  ((CoordinateuvEvent *)event)->window() != 0)
                {  if (rbdata != 0)
                     {  rbdata->scrollwindow = ((CoordinateuvEvent *)event)->window();
                        rbdata->bitmap = 0;
                        if (1 || SetLight == 0)
                          {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
                               rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
                             else  
                               rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
                             if (! rbdata->bitmap->getdefined())
                               {  delete rbdata->bitmap;
                                  rbdata->bitmap = 0;
                               }
                          }
                        rbdata->umin = rbdata->scrollwindow->getumin();
                        rbdata->vmin = rbdata->scrollwindow->getvmin();
                        rbdata->umax = rbdata->scrollwindow->getumax();
                        rbdata->vmax = rbdata->scrollwindow->getvmax();
                        rbdata->lastscale = 1.0;
                        *cstate = 2;
                        db.savezoomundo(0,rbdata->scrollwindow);
                     }
                   else
                     *cstate = ExitState;
                }
              else
                {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                     {  surface = (View3dSurface *) window;
                        du = (surface->getumax() - surface->getumin()) / 4.0;
                        dv = (surface->getvmax() - surface->getvmin()) / 4.0;
                        p = ((CoordinateuvEvent *) event)->coordinate();
                        db.savezoomundo(0,window);
                        window->zoom(p.x-du,p.y-dv,p.x+du,p.y+dv);
                     }
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         program->setbusy(0);
         rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                    rbdata->scrollwindow->getvmin(),
                                    rbdata->scrollwindow->getumax(),
                                    rbdata->scrollwindow->getvmax());
         *cstate = ExitState;
         break;
     }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBZoom3,rbdata);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+120);
    }
  else if (*cstate == 2)
    {  program->setbusy(1);
       state.selmask.entity.clearandset(uv_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBZoom3,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+162);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->bitmap != 0)
              delete rbdata->bitmap;
            delete rbdata;
         }
       program->setcursorshape(CursorRestore);
    }

}

void zoom_command4(int *cstate,HighLevelEvent *event,void **data)
{RBZoom2_data *rbdata = (RBZoom2_data *) *data;
 Point p;
 View3dWindow *window;
 View3dSurface *surface;
 double du,dv;
  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         *data = rbdata = new RBZoom2_data;
         rbdata->bitmap = 0;
         program->setcursorshape(CursorZoom);
         break;
       case 1 :
         rubberband.end(0);
         if (rbdata->bitmap != 0)
           {  program->setbusy(0);
              rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                         rbdata->scrollwindow->getvmin(),
                                         rbdata->scrollwindow->getumax(),
                                         rbdata->scrollwindow->getvmax());
              delete rbdata->bitmap;
              rbdata->bitmap = 0;
              *cstate = event->isa(Coordinateuv) ? 1 : *cstate = ExitState;
           } 
         else if (event->isa(Coordinateuv))
           {  if (((CoordinateuvEvent *)event)->getcontrol() != 0 &&
                  ((CoordinateuvEvent *)event)->window() != 0)
                {  if (rbdata != 0)
                     {  rbdata->scrollwindow = ((CoordinateuvEvent *)event)->window();
                        rbdata->bitmap = 0;
                        if (1 || SetLight == 0)
                          {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
                               rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
                             else  
                               rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
                             if (! rbdata->bitmap->getdefined())
                               {  delete rbdata->bitmap;
                                  rbdata->bitmap = 0;
                               }
                          }
                        rbdata->umin = rbdata->scrollwindow->getumin();
                        rbdata->vmin = rbdata->scrollwindow->getvmin();
                        rbdata->umax = rbdata->scrollwindow->getumax();
                        rbdata->vmax = rbdata->scrollwindow->getvmax();
                        rbdata->lastscale = 1.0;
                        *cstate = 2;
                        db.savezoomundo(0,rbdata->scrollwindow);
                     }
                   else
                     *cstate = ExitState;
                }
              else
                {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                     {  surface = (View3dSurface *) window;
                        du = (surface->getumax() - surface->getumin());
                        dv = (surface->getvmax() - surface->getvmin());
                        p = ((CoordinateuvEvent *) event)->coordinate();
                        db.savezoomundo(0,window);
                        window->zoom(p.x-du,p.y-dv,p.x+du,p.y+dv);
                     }
                   *cstate = 1;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         program->setbusy(0);
         rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                    rbdata->scrollwindow->getvmin(),
                                    rbdata->scrollwindow->getumax(),
                                    rbdata->scrollwindow->getvmax());
         *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  rubberband.begin(0);
       rubberband.add(RBZoom3,rbdata);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+121);
    }
  else if (*cstate == 2)
    {  program->setbusy(1);
       state.selmask.entity.clearandset(uv_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBZoom3,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+162);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->bitmap != 0)
              delete rbdata->bitmap;
            delete rbdata;
         }
       program->setcursorshape(CursorRestore);
    }

}

void zoom_command5(int *cstate,HighLevelEvent *event,void **)
{static double scale;
 Point p;
 View3dWindow *window;
 View3dSurface *surface;
 double du,dv;
 ResourceString rs122(NCVIEWB+122);
 ResourceString rs123(NCVIEWB+123);

 Dialog dialog(rs122.gets(),rs123.gets(),&scale,DontScale);
  switch (*cstate)
    {  case InitialState :
         //program->setcursorshape(CursorZoom);
         scale = v.getreal("zm::scale");
         if (dialog.process() == TRUE)
           {  v.setreal("zm::scale",scale);
             program->setcursorshape(CursorZoom);
              *cstate = 1;
           }
         else
           *cstate = ExitState;
         break;
       case 1 :
         if (event->isa(Coordinateuv))
           {  if ((window = ((CoordinateuvEvent *) event)->window()) != NULL)
                {  surface = (View3dSurface *) window;
                   du = scale * surface->getwidthmm() / 2.0;
                   dv = scale * surface->getheightmm() / 2.0;
                   p = ((CoordinateuvEvent *) event)->coordinate();
                   db.savezoomundo(0,window);
                   window->zoom(p.x-du,p.y-dv,p.x+du,p.y+dv);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCVIEWB+124);
    }
  else if (*cstate == ExitState)
    program->setcursorshape(CursorRestore);

}

/**
 * @brief zoom_command6
 * @param cstate
 *
 * Entry point for the zoom extents command
 *
 */
void zoom_command6(int *cstate,HighLevelEvent *,void **)
{
 //Point pmin(RCREAL_MAX,RCREAL_MAX,RCREAL_MAX),pmax(-RCREAL_MAX,-RCREAL_MAX,-RCREAL_MAX);
 //Point pmin(0.0,0.0,0.0),pmax(0.0,0.0,0.0);
 Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
 double du,dv;
 ResourceString rs125(NCVIEWB+125);
 ResourceString rs126(NCVIEWB+126);
 View2dWindow *window;
  *cstate = ExitState;
  if (cadwindow->getcurrentsurface() == NULL)
	   {  cadwindow->MessageBox(rs125.gets(),rs126.gets(),MB_ICONINFORMATION);
       *cstate = ExitState;
       return;
    }
  program->setbusy(1);
  if (cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    {  window = (View2dWindow *) cadwindow->getcurrentwindow();
       pmin.setxyz(0.0,0.0,0.0);
       pmax.setxyz(window->getview2d()->getpaperwidth(),window->getview2d()->getpaperheight(),0.0);
    }
  else
    db.geometry.extents(cadwindow->getcurrentsurface(),&pmin,&pmax);
  if (! program->getinterrupt())
/*    {  if (pmax.x - pmin.x > db.getptoler() ||
           pmax.y - pmin.y > db.getptoler() )
         {  du = (pmax.x - pmin.x) * 0.02;
*/
  {
       // KMJ: ignore insane bboxes
       //if (pmax.x - pmin.x > db.getptoler() || pmax.y - pmin.y > db.getptoler())
       if ( pmax.x - pmin.x > db.getptoler() && pmax.y - pmin.y > db.getptoler() &&
            pmax.x - pmin.x < RCDBL_MAX && pmax.y - pmin.y < RCDBL_MAX )
       {
            du = (pmax.x - pmin.x) * 0.02;
            dv = (pmax.y - pmin.y) * 0.02;
            db.savezoomundo(0,cadwindow->getcurrentwindow());
            cadwindow->getcurrentwindow()->zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
       }
  }
  program->setbusy(0);

}

void zoom_command7(int *cstate,HighLevelEvent *,void **)
{ db.undoview();
  *cstate = ExitState;
}

void RBZoom8(int draw,void *data,int,int y,View3dSurface *surface)
{RBZoom2_data *rbdata = (RBZoom2_data *) data;
 double scale,du,dv,u,v;
 View3dWindow *w;
 View3dOffScreenBitmap *b;
  if (! rbdata->scrollstarted && surface != 0 && ((View3dWindow *)surface)->getcanscroll())
    {  rbdata->scrollwindow = (View3dWindow *) surface;
       rbdata->scrollstarted = 1;
       rbdata->bitmap = 0; 
       if (1 || SetLight == 0)
         {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = 0;
              }
         }

       rbdata->umin = rbdata->scrollwindow->getumin();
       rbdata->vmin = rbdata->scrollwindow->getvmin();
       rbdata->umax = rbdata->scrollwindow->getumax();
       rbdata->vmax = rbdata->scrollwindow->getvmax();

       scale = 0.25 + ((double) y / (double) surface->getheight()) * 1.5;

       u = (rbdata->umax + rbdata->umin) / 2.0;
       du = (rbdata->umax - rbdata->umin) / 2.0;
       v = (rbdata->vmax + rbdata->vmin) / 2.0;
       dv = (rbdata->vmax - rbdata->vmin) / 2.0;
       du /= scale;  dv /= scale;

       rbdata->umin = u - du;
       rbdata->umax = u + du;
       rbdata->vmin = v - dv;
       rbdata->vmax = v + dv;

       rbdata->lastdu = rbdata->lastdv = 0.0;

       db.savezoomundo(0,rbdata->scrollwindow);
    }

  if (draw == RB_TIMER)
    surface = rbdata->scrollwindow;
  if (draw == RB_ERASE || (w = rbdata->scrollwindow) != surface || surface == 0)
    return;
  b = rbdata->bitmap;

  u = (rbdata->umax + rbdata->umin) / 2.0;
  du = (rbdata->umax - rbdata->umin) / 2.0;
  v = (rbdata->vmax + rbdata->vmin) / 2.0;
  dv = (rbdata->vmax - rbdata->vmin) / 2.0;
  if (draw == RB_TIMER)
    {  scale = 1.0;
       if (y >= 0 && y < 5)
         scale = 19.0 / 20.0;
       if (y > surface->getheight() - 5)
         scale = 21.0 / 20.0;
       du *= scale;  dv *= scale;
       rbdata->umin = u - du;
       rbdata->umax = u + du;
       rbdata->vmin = v - dv;
       rbdata->vmax = v + dv;
    }
  if (y >= 0)
    rbdata->lastscale = scale = 0.25 + ((double) y / (double) surface->getheight()) * 1.5;
  else
    scale = rbdata->lastscale;
  du *= scale;
  dv *= scale;
  if (du < 1.0E-10 || dv < 1.0E-10 ||
      du >  1.0E20 || dv >  1.0E20)
    return;
  if (rbdata->bitmap == NULL)
    {  w->setuvminmax(u-du,v-dv,u+du,v+dv);
       if (w->getoffscreenbitmap() != 0)
         w->getoffscreenbitmap()->displaylist.clear();
       w->paintevent();
    }
  else
    {  w->setuvminmax(u-du,v-dv,u+du,v+dv);
       b->setuvminmax(u-du,v-dv,u+du,v+dv);
       if (w->getsurfacetype() == View2dSurf)
         b->setplotscale(1.0);
       b->paintevent();
       BitBlt(w->gethdc(),0,0,int(w->getwidth()),int(w->getheight()),
              b->gethdc(),0,0,SRCCOPY);
    }
}

void zoom_command8(int *cstate,HighLevelEvent *,void **data)
{static Point p1;
 RBZoom2_data *rbdata = (RBZoom2_data *) *data;
 Point p2;
  switch (*cstate)
    {  case InitialState :
         *cstate = ExitState;
         *data = rbdata = new RBZoom2_data;
         if (rbdata != 0)
           {  rbdata->scrollwindow = 0;
              rbdata->bitmap = 0;
              rbdata->scrollstarted = 0;
              *cstate = 1;
           }
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->scrollstarted)
           rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                      rbdata->scrollwindow->getvmin(),
                                      rbdata->scrollwindow->getumax(),
                                      rbdata->scrollwindow->getvmax());
         *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearandset(uv_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBZoom8,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+162);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->bitmap != 0)
              delete rbdata->bitmap;
            delete rbdata;
         }
    }
}

void RBZoom9(int draw,void *data,int x,int y,View3dSurface *surface)
{RBZoom2_data *rbdata = (RBZoom2_data *) data;
 double du,dv;
 View3dWindow *w;
 View3dOffScreenBitmap *b;
  if (! rbdata->scrollstarted && surface != 0 && ((View3dWindow *)surface)->getcanscroll())
    {  rbdata->scrollwindow = (View3dWindow *) surface;
       rbdata->scrollstarted = 1;
       rbdata->bitmap = 0;
       if (1 || SetLight == 0)
         {  if (rbdata->scrollwindow->getsurfacetype() == View2dSurf)
              rbdata->bitmap = new View2dOffScreenBitmap((View2dWindow *)rbdata->scrollwindow,((View2dWindow *)rbdata->scrollwindow)->getview2d());
            else  
              rbdata->bitmap = new View3dOffScreenBitmap(rbdata->scrollwindow);
            if (! rbdata->bitmap->getdefined())
              {  delete rbdata->bitmap;
                 rbdata->bitmap = 0;
              }
         }

       rbdata->umin = rbdata->scrollwindow->getumin();
       rbdata->vmin = rbdata->scrollwindow->getvmin();
       rbdata->umax = rbdata->scrollwindow->getumax();
       rbdata->vmax = rbdata->scrollwindow->getvmax();

       du =  ((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
       dv = -((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());

       rbdata->umin += du;
       rbdata->umax += du;
       rbdata->vmin += dv;
       rbdata->vmax += dv;

       rbdata->lastdu = rbdata->lastdv = 0.0;

       db.savezoomundo(0,rbdata->scrollwindow);
    }

  if (draw == RB_TIMER)
    surface = rbdata->scrollwindow;
  if (draw == RB_ERASE || (w = rbdata->scrollwindow) != surface || surface == 0)
    return;
  b = rbdata->bitmap;

  if (draw == RB_TIMER)
    {  du = dv = 0;
       if (x >= 0 && x < 5)
         du =  (rbdata->umax - rbdata->umin) / 20.0;
       if (x > surface->getwidth() - 5)
         du = -(rbdata->umax - rbdata->umin) / 20.0;
       if (y >= 0 && y < 5)
         dv = -(rbdata->vmax - rbdata->vmin) / 20.0;
       if (y > surface->getheight() - 5)
         dv =  (rbdata->vmax - rbdata->vmin) / 20.0;
       rbdata->umin += du;
       rbdata->umax += du;
       rbdata->vmin += dv;
       rbdata->vmax += dv;
    }
  if (x >= 0 && y >= 0)
    {  rbdata->lastdu = du = -((double) x / (double) surface->getwidth() - 0.5) * (surface->getumax() - surface->getumin());
       rbdata->lastdv = dv =  ((double) y / (double) surface->getheight() - 0.5) * (surface->getvmax() - surface->getvmin());
    }
  else
    {  du = rbdata->lastdu;
       dv = rbdata->lastdv;
    }
  if (rbdata->umin + du < -1.0E20 || rbdata->umax + du > 1.0E20 ||
      rbdata->vmin + dv < -1.0E20 || rbdata->vmax + dv > 1.0E20)
    return;
  if (b == NULL)
    {  w->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       if (w->getoffscreenbitmap() != 0)
         w->getoffscreenbitmap()->displaylist.clear();
       w->paintevent();
    }
  else
    {  w->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       b->setuvminmax(rbdata->umin+du,rbdata->vmin+dv,
                      rbdata->umax+du,rbdata->vmax+dv);
       if (w->getsurfacetype() == View2dSurf)
         b->setplotscale(1.0);
       b->paintevent();
       BitBlt(w->gethdc(),0,0,int(w->getwidth()),int(w->getheight()),
              b->gethdc(),0,0,SRCCOPY);
    }
}

void zoom_command9(int *cstate,HighLevelEvent *,void **data)
{static Point p1;
 RBZoom2_data *rbdata = (RBZoom2_data *) *data;
 Point p2;
  switch (*cstate)
    {  case InitialState :
         *cstate = ExitState;
         *data = rbdata = new RBZoom2_data;
         if (rbdata != 0)
           {  rbdata->scrollwindow = 0;
              rbdata->bitmap = 0;
              rbdata->scrollstarted = 0;
              *cstate = 1;
           }
         break;
       case 1 :
         rubberband.end(0);
         program->setbusy(0);
         if (rbdata->scrollstarted)
           rbdata->scrollwindow->zoom(rbdata->scrollwindow->getumin(),
                                      rbdata->scrollwindow->getvmin(),
                                      rbdata->scrollwindow->getumax(),
                                      rbdata->scrollwindow->getvmax());
         *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearandset(uv_mask,end_list);
       rubberband.begin(0);
       rubberband.add(RBZoom9,rbdata);
       cadwindow->prompt->normalprompt(NCVIEWB+161);
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         {  if (rbdata->bitmap != 0)
              delete rbdata->bitmap;
            delete rbdata;
         }
    }
}

/**
 * @brief zone_command1
 * @param cstate
 *
 *  Create a View Zone
 *
 * KMJ : working in Qt version
 *
 */
void zone_command1(int *cstate,HighLevelEvent *,void **)
{ZoneList *zl;
 View3dWindow *w;
 Zone *zone;
 RCCHAR **namelist,newname[256];
 ResourceString rs146(NCVIEWB+146);
 ResourceString rs147(NCVIEWB+147);
 ResourceString rs148(NCVIEWB+148);
 int i;
  if ((w = cadwindow->getcurrentwindow()) == 0)
	 cadwindow->MessageBox(rs146.gets(),rs147.gets(),MB_ICONSTOP);
  else
     {Dialog dialog(_RCT("CreateZone_Dialog"));
		 if (w->getsurfacetype() == View2dSurf)
			zl = & ((View2dWindow *)w)->getview2d()->zonelist;
		 else
			zl = & w->getview()->zonelist;
		 strcpy(newname,"");
         namelist = new RCCHAR *[zl->length()];
		 for (zl->start(),i = 0 ; (zone = zl->next()) != 0 ; i++)
			namelist[i] = zone->getname();
		 dialog.add(new StringDialogControl(100,newname,256));
		 dialog.add(new ListDialogControl(101,zl->length(),namelist));
		 if (dialog.process() == TRUE)
			{//  Create new Zone object, and insert into the view list
				for (zl->start() ; (zone = zl->next()) != 0 ;)
				  if (strcmp(newname,zone->getname()) == 0)
                {  if (cadwindow->MessageBox(rs148.gets(),rs147.gets(),MB_YESNO) == IDYES)
                     zone->change(w->getumin(),w->getvmin(),w->getumax(),w->getvmax());
                   break;
                }
				if (zone == 0)
                {
                    if ((zone = new Zone(newname,w->getumin(),w->getvmin(),w->getumax(),w->getvmax())) != 0)
                        zl->add(zone);
                }
			}
		 delete [] namelist;
	 }
  *cstate = ExitState;
}

/**
 * @brief zone_command2
 * @param cstate
 *
 * Delete View Zone
 *
 * KMJ : working in Qt version
 *
 */
void zone_command2(int *cstate,HighLevelEvent *,void **)
{ZoneList *zl;
 View3dWindow *w;
 Zone *zone;
 RCCHAR **namelist,*viewname,message[256];
 int i,index;
 ResourceString rs149(NCVIEWB+149);
 ResourceString rs150(NCVIEWB+150);
 ResourceString rs151(NCVIEWB+151);
  if ((w = cadwindow->getcurrentwindow()) == 0)
	 cadwindow->MessageBox(rs149.gets(),rs150.gets(),MB_ICONSTOP);
  else
     {Dialog dialog(_RCT("DeleteZone_Dialog"));
		 if (w->getsurfacetype() == View2dSurf)
			{  zl = & ((View2dWindow *)w)->getview2d()->zonelist;
            viewname = ((View2dWindow *)w)->getview2d()->getname();
         }
		 else
			{  zl = & w->getview()->zonelist;
            viewname = w->getview()->getname();
         }
		 index = 0;
         namelist = new RCCHAR *[zl->length()];
		 for (zl->start(),i = 0 ; (zone = zl->next()) != 0 ; i++)
			namelist[i] = zone->getname();
		 dialog.add(new ListDialogControl(100,zl->length(),namelist,&index));
         if (dialog.process() == TRUE)
         {  sprintf(message,rs151.gets(),namelist[index],viewname);
				if (cadwindow->MessageBox(message,rs150.gets(),MB_YESNO) == IDYES)
				  {  for (i = 0,zl->start() ; i <= index ; i++)
				   	  zone = zl->next();
					  zl->del(zone);
				     delete zone;
              }
			}
		 delete [] namelist;
	 }
  *cstate = ExitState;
}

/**
 * @brief zone_command3
 * @param cstate
 *
 * Zoom to view zone
 *
 * KMJ : working in Qt version
 *
 */
void zone_command3(int *cstate,HighLevelEvent *,void **)
{ZoneList *zl;
 View3dWindow *w;
 Zone *zone;
 RCCHAR **namelist;
 int i,index;
 ResourceString rs158(NCVIEWB+158);
 ResourceString rs159(NCVIEWB+159);
 ResourceString rs160(NCVIEWB+160);
  if ((w = cadwindow->getcurrentwindow()) == 0)
	 cadwindow->MessageBox(rs158.gets(),rs159.gets(),MB_ICONSTOP);
  else
     {Dialog dialog(_RCT("ZoomZone_Dialog"));
		 if (w->getsurfacetype() == View2dSurf)
			zl = & ((View2dWindow *)w)->getview2d()->zonelist;
		 else
			zl = & w->getview()->zonelist;
       if (zl->empty())
         cadwindow->MessageBox(rs160.gets(),rs159.gets(),MB_ICONSTOP);
       else
         {  index = 0;
            namelist = new RCCHAR *[zl->length()];
		      for (zl->start(),i = 0 ; (zone = zl->next()) != 0 ; i++)
			     namelist[i] = zone->getname();
		      dialog.add(new ListDialogControl(100,zl->length(),namelist,&index));
            if (dialog.process() == TRUE)
		        {  for (i = 0,zl->start() ; i <= index ; i++)
				     zone = zl->next();
				     db.savezoomundo(0,w);
                 w->zoom(zone->getumin(),zone->getvmin(),zone->getumax(),zone->getvmax());
			     }
            delete [] namelist;
         }
	 }
  *cstate = ExitState;
}

