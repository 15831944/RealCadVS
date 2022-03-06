
//
//  The implementation of the User Interface Classes
//
#include <QTimer>
#include <QStringList>
#include "ncwin.h"
#include "RCView.h"
#include "RCGraphicsView.h"
#include "RCGraphicsScene.h"
#include "RCCentralWidget.h"
#include "RCOpenGLWidget.h"
#include "rcgraphicspreview.h"
#include "rconecoordinateentry.h"

//#pragma comment(lib,"Msimg32.lib");

extern bool nudgelist(EntityList entities, UINT dir);
extern void RemovePartialGroupEntities(EntityList *list,int unhighlight);

BitMask &View3dWindow::getvisibility(void)
{int i;
 View *v;
  visible.clearall();
  if (view->getvisibility())
    visible.set(view->getviewno());
  else
    {  for (i = 0 ; i < MaxViews ; i++)
         if ((v = db.views3d.match(i)) != 0 && ! v->getvisibility())
           visible.set(i);
    }
  return visible;
}

void View3dWindow::vscrollevent(int code,int pos)
{int start,end;
 double v,u1,v1,u2,v2,dv,svmin,svmax;
  if (offscreenbitmap == 0 || ! offscreenbitmap->displaylist.getextents(&u1,&v1,&u2,&v2))
    {  v1 = vmin;  v2 = vmax;
    }
  dv = v2 - v1;
  if (dv < vmax - vmin)
    dv = vmax - vmin;
  svmin = (v1 + v2) / 2.0 - dv * 1.5;
  svmax = (v1 + v2) / 2.0 + dv * 1.5 - (vmax - vmin);
  if (svmin > vmin) svmin = vmin;
  if (svmax < vmin) svmax = vmin;
  v = vmin;

  GetScrollRange(gethwnd(),SB_VERT,&start,&end);
  if (start != end)
    {  switch (code)
         {case SB_BOTTOM : v = svmin;  break;
          case SB_LINEDOWN : v = v - (vmax - vmin) / 50.0;  break;
          case SB_LINEUP : v = v + (vmax - vmin) / 50.0;  break;
          case SB_PAGEDOWN : v = v - (vmax - vmin) / 5.0;  break;
          case SB_PAGEUP : v = v + (vmax - vmin) / 5.0;  break;
          case SB_THUMBTRACK	:
          case SB_THUMBPOSITION	:  v = double(pos - end) / double(start - end) * (svmax - svmin) + svmin;  break;
          case SB_TOP : v = svmax;  break;
         }
       if (v < svmin) v = svmin;
       if (v > svmax) v = svmax;
       SetScrollPos(gethwnd(),SB_VERT,int(end + double(start - end) * (v - svmin) / (svmax - svmin)),TRUE);
       if (code < 1000)
         zoom(umin,v,umax,v + (vmax - vmin));
    }
}

void View3dWindow::mousewheelevent(int delta)
{
}

static int lastdelta=0;
void View3dWindow::mousewheelevent(int delta,int px, int py)
{double u,v,f,du,dv,t1,t2;
 POINT p;
 Point eye,ref,up;
 Transform t;
 scrolling = 0;

 //qDebug() << "View3dWindow::mousewheelevent : delta : " << delta;

  if (state.getcommandid() == 518 ||
      state.getcommandid() == 561 ||
      state.getcommandid() == 555 ||
      state.getcommandid() == 519 ||
      state.getcommandid() == 520)
    return;  //  Disable mouse wheel during these operations

  // KMJ added check for surface type when rotating the view
  // only rotate View3dSurf type views
  if ((GetKeyState(VK_MENU) & 0x8000) && getsurfacetype() == View2dSurf)
      return;

  delta *= wheeldirection;

  if (GetKeyState(VK_MENU) & 0x8000 )
    {
       //qDebug() << "wheel delta : " << delta;
       t.rotate(getview()->getref(),Point(0.0,0.0,1.0), delta * M_PI / 180.0 * 5.0);
       eye = t.transform(getview()->geteye());
       ref = t.transform(getview()->getref());
       up = t.transform(getview()->getref()+getview()->getup()) - ref;
       if (getview()->changeview(eye,ref,up))
         cadwindow->updatewindows(getview());

       //  Send an escape key to try to cancel the ALT function
       SendMessage(cadwindow->gethwnd(),WM_KEYDOWN,VK_ESCAPE,0);
       SendMessage(cadwindow->gethwnd(),WM_KEYUP,VK_ESCAPE,0);

       scrolling = 2;
    }
  else
    {
       p.x = px;
       p.y = height - 1 - py;
       //GetCursorPos(&p);
       //qDebug() << "View3dWindow::mousewheelevent : " << p.x;
       //qDebug() << "View3dWindow::mousewheelevent : " << p.y;

       //qDebug() << "hWnd : " << hWnd;
       //ScreenToClient(hWnd,&p);
       //qDebug() << "View3dWindow::mousewheelevent : " << p.x;
       //qDebug() << "View3dWindow::mousewheelevent : " << p.y;

       //qDebug() << "height : " << height;
       //qDebug() << "width  : " << width;

       t1 = p.x / width;
       t2 = p.y / height;
       //t2 = 1.0 - p.y / height;
       //qDebug() << "t1 : " << t1;
       //qDebug() << "t2  : " << t2;

       du = umax - umin;
       dv = vmax - vmin;
       u = umin + du * t1;
       v = vmin + dv * t2;

       double rate = 1.1;
       switch(db.getzoomstep())
       {
       case 0:
           rate = 1.1;
           break;
       case 1:
       default:
           rate = 1.25;
           break;
       case 2:
           rate = 1.5;
           break;
       }
       //f = double(delta) / 120.0 * 1.1;
       //f = double(delta) / 120 * rate;
       f = double(delta) * rate;
       if (delta < 0)
         {  du /= -f;  dv /= -f;
         }
       if (delta > 0)
         {  du *= f;  dv *= f;
         }

       scrolling = 4;

       zoom(u - du * t1,v - dv * t2,u + du * (1.0-t1),v + dv * (1.0-t2));


#ifdef USING_WIDGETS
       rubberband.mousemoveevent(this,p.x,p.y,0);
#endif
  }

  //scrolling = 0;
  //mousemoveevent(p.x,p.y,0);
  //QTimer::singleShot(::v.getinteger("wn::screenrefreshdelay")+1,[&](){this->mousemoveevent(p.x,p.y,0);});
  //QTimer::singleShot(::v.getinteger("wn::screenrefreshdelay")+1,[&](){this->scrolling = 0;});
  //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->setTimer(::v.getinteger("wn::screenrefreshdelay"));
  //app->processEvents();
}

void View3dWindow::hscrollevent(int code,int pos)
{int start,end;
 double u,u1,v1,u2,v2,du,sumin,sumax;
  if (offscreenbitmap == 0 || ! offscreenbitmap->displaylist.getextents(&u1,&v1,&u2,&v2))
    {  u1 = umin;  u2 = umax;
    }
  du = u2 - u1;
  if (du < umax - umin)
    du = umax - umin;
  sumin = (u1 + u2) / 2.0 - du * 1.5;
  sumax = (u1 + u2) / 2.0 + du * 1.5 - (umax - umin);
  if (sumin > umin) sumin = umin;
  if (sumax < umin) sumax = umin;
  u = umin;

  GetScrollRange(gethwnd(),SB_HORZ,&start,&end);
  if (start != end)
    {  switch (code)
         {case SB_BOTTOM : u = sumin;  break;
          case SB_LINEDOWN : u = u + (umax - umin) / 50.0;  break;
          case SB_LINEUP : u = u - (umax - umin) / 50.0;  break;
          case SB_PAGEDOWN : u = u + (umax - umin) / 5.0;  break;
          case SB_PAGEUP : u = u - (umax - umin) / 5.0;  break;
          case SB_THUMBTRACK	:
          case SB_THUMBPOSITION	:  u = double(pos - start) / double(end - start) * (sumax - sumin) + sumin;  break;
          case SB_TOP : u = sumax;  break;
         }
       if (u < sumin) u = sumin;
       if (u > sumax) u = sumax;
       SetScrollPos(gethwnd(),SB_HORZ,int(start + double(end - start) * (u - sumin) / (sumax - sumin)),TRUE);
       if (code < 1000)
         zoom(u,vmin,u + (umax - umin),vmax);
    }
}

void View3dWindow::rubberband3d(Point p)
{ p = modeltoscreen(p);
  cursor->rubberband(int(p.x),int(height - p.y));
}

void View3dWindow::rubberbanduv(Point p)
{ p = uvtoscreen(p);
  cursor->rubberband(int(p.x),int(p.y));
}

void View3dWindow::rubberbandGL()
{
    //qDebug() << "window buttondown : " << buttondown;
    //qDebug() << "cursor isdrawing  : " << cursor->isdrawing();
    //qDebug() << "cursor action  : " << cursor->getaction();
    //if(buttondown == 2 || cursor->isdrawing())
    if(buttondown == 2 || cursor->isdrawing() || cursor->getaction() == 2)
    {
        cursor->drawGL(this);
    }
}

void View3dWindow::cursoraction(int act)
{ cursor->action(act);
}

void View3dWindow::cursorstyle(int sty)
{ cursor->style(sty);
}

View3dWindow *MainWindow::matchview3dwindow(RCHWND hwnd)
{
  View3dWindow *w;
  for (view3dlist.start() ; (w = view3dlist.next()) != 0 ; )
    if ((RCView*)w->gethwnd() == (RCView*)hwnd)
      return w;
  return 0;
}

void View3dWindow::mousemoveevent(int x,int y,int buttons)
{Point p;
 Entity *e,*secondentity;
 int snap,colour,layer,style,weight;
 RCHWND overhwnd;
 POINT pt;
 View3dWindow *w;
 UINT oldmode;
 RCHPEN hpen;

 //qDebug() << "View3dWindow mousemovevent : " << getWidget()->objectName();
 //qDebug() << "View3dWindow mousemoveevent x : " << x;
 //qDebug() << "View3dWindow mousemoveevent y : " << y;
 //qDebug() << "buttondown at0 : " << buttondown;

  // if the style entry has the focus - remove the focus
#ifdef USING_WIDGETS
  if (cadwindow->styleentry->containsWidget((QWidget*)GetFocus()))
      SetFocus(cadwindow->subwindow->gethwnd());
#else
  if (GetFocus() == cadwindow->styleentry->gettypehwnd())
      SetFocus(cadwindow->subwindow->gethwnd());
#endif

  if (x >= -32000 && y >= -32000)
  {
       if (ndownpoints > 0 && ndownpoints < MAX_DOWNPOINTS && y != downpoint[ndownpoints].y)
       {    downpoint[ndownpoints].x = x;
            downpoint[ndownpoints].y = y;
            ndownpoints++;

#ifdef USING_WIDGETS
            RCCOLORREF cr = RGB(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                              ::v.getreal("wn::backgroundcolour.green")*255.0+0.5,
                              ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5);
            cadwindow->getcurrentwindow()->setbackground(cr);
            cadwindow->getcurrentwindow()->setup(0,0,DM_GRID,0,0,0,0);
            MoveToEx(cadwindow->getcurrentwindow(),(int)downpoint[ndownpoints-2].x,cadwindow->getcurrentwindow()->getheight()-(int)downpoint[ndownpoints-2].y,0);
            LineTo(cadwindow->getcurrentwindow(),x,cadwindow->getcurrentwindow()->getheight()-y);
#else
            oldmode = SetROP2(offscreenbitmap->gethdc(),R2_NOT);
            hpen = (RCHPEN)SelectObject(offscreenbitmap->gethdc(),(RCHPEN)GetStockObject(WHITE_PEN));
            MoveToEx(offscreenbitmap->gethdc(),(int)downpoint[ndownpoints-2].x,(int)downpoint[ndownpoints-2].y,0);
            LineTo(offscreenbitmap->gethdc(),x,y);
            SetROP2(offscreenbitmap->gethdc(),oldmode);
            SelectObject(offscreenbitmap->gethdc(),hpen);
            UpdateWindow();
#endif

            if (state.getcommandactive() == 0)
              {  if (downcommand == 0)
                   {  cursor->action(0);
                      cursor->style(1);
                      cursor->setshape(3);
                   }
                 downcommand = 1;
              }

         }

       if (downcommand)
         {  cursor->mousemoveevent(x,y,buttons);
            //rubberband.mousemoveevent(this,x,y,buttons);//fg
            return;
         }

       if (scrolling) // some kind of scrolling happening
         {
           // KMJ: added surface type check when doing 3d rotation
           if ((buttons & MK_MBUTTON) == MK_MBUTTON )
              {double du,dv;
                 if((GetKeyState(VK_MENU) & 0x8000) && getsurfacetype() == View2dSurf)
                     return;
                 if (GetKeyState(VK_MENU) & 0x8000)
                 {
                    Transform t1,t2;
                    double uangle,vangle;
                    Point eye,ref,up;

                      db.undoview();
                      db.savezoomundo(1,this);

                      uangle = -((double) (y - downy) / (double) getheight()) * M_PI;
                      vangle = -((double) (x - downx) / (double) getwidth()) * M_PI;
                      t1.rotate(getview()->getref(),getuaxis(),uangle);
                      t2.rotate(getview()->getref(),getvaxis(),vangle);
                      t1.apply(t2);
                      eye = t1.transform(getview()->geteye());
                      ref = t1.transform(getview()->getref());
                      up = t1.transform(getview()->getref() + getview()->getup()) - ref;
                      if (getview()->changeview(eye,ref,up))
                        cadwindow->updatewindows(getview());

                      scrolling = 2; // 3D rotation

                 }
                 else
                   {
                      du = (umax - umin) * (x - downx) / getwidth();
                      dv = (vmax - vmin) * (y - downy) / getheight();
#ifdef USING_WIDGETS
                      if (du != 0.0 || dv != 0.0)
                      {
                        zoom(umin - du,vmin + dv,umax - du,vmax + dv);
                      }
                      downx = x;  downy = y;
                      scrolling = 3; // panning
                      rubberband.mousemoveevent(this,x,y,buttons);
#else
                      if (du != 0.0 || dv != 0.0)
                        zoom(umin - du,vmin + dv,umax - du,vmax + dv);
                      downx = x;  downy = y;
                      scrolling = 3;
#endif
                   }
                 return;
              }
            else
              scrolling = 0;// not scrolling
         }

       //  Make sure that the colour/layer/style and weight are correct for this view
       db.header.setvisible(visible);
       if (selectionlayertable != view->getselectionlayertable())
         selectionlayertable = view->getselectionlayertable();
       if (displaylayertable != view->getdisplaylayertable())
         displaylayertable = view->getdisplaylayertable();
       if (lockedlayertable != view->getlockedlayertable())
         lockedlayertable = view->getlockedlayertable();
       if (view->getworkplanename() != 0 && strcmp(db.workplanes.getcurrent()->getname(),view->getworkplanename()) != 0)
         {//  If the view has an associated workplane, select the workplane when the cursor moves into this view
            if (db.workplanes.match(view->getworkplanename()) != 0)
              db.workplanes.setcurrent(db.workplanes.match(view->getworkplanename()));
         }

       colour = view->getcolour() >= 0 ? view->getcolour() : v.getinteger("df::colour");
       layer = view->getlayer() >= 0 ? view->getlayer() : v.getinteger("df::layer");;
       style = view->getstyle() >= 0 ? view->getstyle() : v.getinteger("df::style");
       weight = view->getweight() >= 0 ? view->getweight() : v.getinteger("df::weight");

       if (colour != db.header.getcolour() ||
           layer != db.header.getlayer() ||
           style != db.header.getstyle() ||
           weight != db.header.getweight())
         {  db.header.change(colour,layer,style,weight);
            if (cadwindow->getlayerwindow() != 0)
              cadwindow->getlayerwindow()->paintevent();
            cadwindow->updatetitle();
         }

       if (downentity != 0 && ! state.dragging() && coordinate3d(downx,downy,&p) && (abs(x - downx) > 5 || abs(y - downy) > 5))
         {  if (GetPrivateProfileInt("Configuration","EnableDrag",1,home.getinifilename()) != 0 ||
                state.getselection().inlist(downentity))
              {  //  Add downentity to the selection as well
                 state.addtoselection(downentity);
#ifndef NEW_DRAGGING
                 state.startdrag(p,this,0);
#else
                 state.startdragGL(p,this,0);
#endif

                 buttondown = 3;
                 cadwindow->prompt->setcolour(RGB(0,0,0));
                 cadwindow->prompt->normalprompt(NCWIN+29);
              }
            else
              {  downentity->unhighlight();
                 downentity = 0;
              }
         }

       if (state.dragging())
         {  if (buttons & MK_LBUTTON)
              {  if (buttondown != 4)
                   buttondown = 3;
                 pt.x = x;  pt.y = y;
                 //qDebug() << "dragging gethwnd() : " << (QObject*)gethwnd();
                 ClientToScreen(gethwnd(),&pt);
                 //qDebug() << "dragging cadwindow->subwindow->gethwnd() : " << (QObject*)cadwindow->subwindow->gethwnd();
                 ScreenToClient(cadwindow->subwindow->gethwnd(),&pt);
                 overhwnd = ChildWindowFromPoint(cadwindow->subwindow->gethwnd(),pt);
#if 1
                 if(dynamic_cast<QGLWidget*>((QObject*)overhwnd) != 0)
                     overhwnd = ((QGLWidget*)overhwnd)->parent()->parent();
#else
                 if(dynamic_cast<RCGLWidget*>((QObject*)overhwnd) != 0)
                     overhwnd = ((RCGLWidget*)overhwnd)->parent()->parent();
#endif
                 //qDebug() << "dragging overhwnd : " << (QObject*)overhwnd;
                 //qDebug() << "dragging ((RCView*)gethwnd())->view() : " << ((RCView*)gethwnd())->view()->viewport();
                 //qDebug() << "dragging cadwindow->matchview3dwindow(overhwnd) : " << cadwindow->matchview3dwindow(overhwnd);
                 if (overhwnd == 0 || overhwnd == gethwnd() || (w = cadwindow->matchview3dwindow(overhwnd)) == 0)
                   w = this;
                 else
                   w = cadwindow->matchview3dwindow(overhwnd);
                 //if(cadwindow->matchview3dwindow(overhwnd))
                 //qDebug() << "dragging cadwindow->matchview3dwindow(overhwnd) : " << cadwindow->matchview3dwindow(overhwnd);
                 pt.x = x;  pt.y = y;
                 //qDebug() << "dragging w : " << w->getScene()->objectName();
                 ClientToScreen(gethwnd(),&pt);
                 //qDebug() << "dragging w->gethwnd() : " << (QObject*)w->gethwnd();
                 ScreenToClient(w->gethwnd(),&pt);
                 if (w->coordinate3d(pt.x,pt.y,&p))
#ifndef NEW_DRAGGING
                     state.dragto(p,w);
#else
                     state.dragtoGL(p,w);
#endif
              }
            else
              {
#ifndef NEW_DRAGGING
               state.enddrag();
#else
               state.enddragGL();
#endif
                 cadwindow->prompt->setcolour(RGB(0,150,0));
                 cadwindow->prompt->normalprompt(NCWIN+1);
              }
         }
    }

  if (buttondown)
  {
      //qDebug() << "buttondown before1 : " << buttondown;
      if (buttons & MK_LBUTTON)
      {
          if (state.selmask.entity.test(many_mask) &&
               buttondown == 1 && (abs(downx - x) > 7 || abs(downy - y) > 7))
             {
#if 1
                //qDebug() << "buttondown at1 : " << buttondown;
                // freeze the backgound as an image to allow fast rubberbanding
                ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->freezeBackImage();
                //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->backImage
                // = new QImage(((QGLWidget*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view()->viewport())->grabFrameBuffer());
                //
#endif
                buttondown = 2;
                cursor->style(0);  cursor->action(2);
                cursor->rubberband(downx,downy);
                downentity = 0;
             }
      }
      else
      {
          buttondown = 0;  cursor->style(1);  cursor->action(0);
      }
  }

  snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask) || state.dragging();

  if (db.getdynamiccursor() && (state.getcommandid() == 0 || (buttons & MK_CONTROL) != MK_CONTROL))
   {
      // v8 enhancement for snapping to the model space origin point
      //if (snap && db.snapmask.test(SnapMaskOrigin) && db.geometry.pickoriginpoint((View3dSurface *)this,100,x,int(this->getheight()) - 1 - y,&p) != 0)
      if (snap && db.snapmask.test(SnapMaskOrigin) && (db.geometry.pickoriginpoint((View3dSurface *)this,100,x,int(this->getheight()) - 1 - y,&p) != 0 ||
                                                       db.geometry.pickmodelspaceoriginpoint((View3dSurface *)this,100,x,int(this->getheight()) - 1 - y,&p) != 0))
        {
          cursor->setshape(4);
          //qDebug() << "set shape 4";
        }
      else if ((e = db.geometry.pick((View3dSurface *)this,1,snap,x,y,&secondentity,(Point *)0)) != 0)
        {int snapped;

           snapped = 0;
           if (snap && secondentity != 0 && db.snapmask.test(SnapMaskIntersection) && e->islinear())
             {//  Search for the intersecting entity
              BitMask entity1(32),entity2(32);

                entity1 = state.selmask.entity;
                entity2 = state.tempentitymask;
                state.selmask.entity.set(linear_mask);
                state.tempentitymask.set(linear_mask);

                for (db.geometry.start() ; ! snapped && (secondentity = db.geometry.next()) != 0 ; )
                  if (secondentity->islinear() &&  secondentity->isselectable(this) && secondentity->pick(this,x,getheight() - 1 - y,0) >= 0.0)
                    {Intersect in(e,secondentity);
                     int i;
                       for (i = 0 ; i < in.nintersections() ; i++)
                         if (in.intersection(i)->isa(point_entity))
                           {  p = ((PointE *)in.intersection(i))->getp();
                              if (pick(p,100,x,int(this->getheight()) - 1 - y) >= 0.0)
                              {
                                snapped = 1;
                                qDebug() << "snapped";
                              }
                           }
                    }

                state.selmask.entity = entity1;
                state.tempentitymask = entity2;

             }

           if (snapped)
              cursor->setshape(5);
           else if (snap && e->islinear() && ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,0,x,int(this->getheight()) - 1 - y,&p) && db.snapmask.test(SnapMaskEnd))
             cursor->setshape(1);
           else if (snap && e->isa(wall_entity) && ((Wall *)e)->pickpoint((View3dSurface *)this,0,x,int(this->getheight()) - 1 - y,&p) && db.snapmask.test(SnapMaskEnd))
             cursor->setshape(1);
           else if (snap && e->isa(beam_entity) && ((Beam *)e)->pickpoint((View3dSurface *)this,0,x,int(this->getheight()) - 1 - y,&p) && db.snapmask.test(SnapMaskEnd))
             cursor->setshape(1);
           else if (snap && e->isa(point_entity))
             cursor->setshape(1);
           else if (e->isselectable(this))
           {
             cursor->setshape(2);
             //qDebug() << "set shape 2";
           }
           else
             cursor->setshape(0);
        }
      else
      {
        cursor->setshape(0);
        //qDebug() << "set shape 0";
      }
   }
 else
   cursor->setshape(0);
 cursor->mousemoveevent(x,y,buttons);
 rubberband.mousemoveevent(this,x,y,buttons);
}

bool View3dWindow::SwapCurrentSelectedEntity()
{
    Entity *tempEntity, *secondEntity;
    int clearhandle, alreadyselected;
    int buttons = QGuiApplication::queryKeyboardModifiers();

    tempEntity = db.geometry.pickNext(this,0,0,downx,downy,&secondEntity,(Point *)0);


    if (!tempEntity) //Can't swap if we don't have an entity
        return false;
    if (tempEntity == lastEntitySelected) //Can't swap if we don't have other entities
        return false;
    else
    {
        if (lastEntitySelected)
        {
            secondEntity = lastEntitySelected;
            secondEntity->unhighlight();
            state.removefromselection(secondEntity);
        }
        downentity = tempEntity;
        if ((buttons & (Qt::ControlModifier | Qt::ShiftModifier)) == 0) //Ensure we are starting with a new selection unless SHIFT or CTRL are being held down
        {
            state.destroyselection(1);
            clearhandle = 0;  //  handle has now been cleared
        }
        lastEntitySelected = downentity;
        state.addtoselection(downentity);
    }
    /*//old function
    if (!tempEntity) //Can't swap if we don't have both entities
        return;
    if (!secondEntity) //Can't swap if we don't have both entities
        return;
    if (tempEntity == lastEntitySelected)
    {
        tempEntity->unhighlight();
        state.removefromselection(tempEntity);
        downentity = secondEntity;
        if ((buttons & (Qt::ControlModifier | Qt::ShiftModifier)) == 0) //Ensure we are starting with a new selection unless SHIFT or CTRL are being held down
        {
            state.destroyselection(1);
            clearhandle = 0;  //  handle has now been cleared
        }
        lastEntitySelected = downentity;
        state.addtoselection(downentity);
    }
    else if (secondEntity == lastEntitySelected)
    {
        secondEntity->unhighlight();
        state.removefromselection(secondEntity);
        downentity = tempEntity;
        if ((buttons & (Qt::ControlModifier | Qt::ShiftModifier)) == 0) //Ensure we are starting with a new selection unless SHIFT or CTRL are being held down
        {
            state.destroyselection(1);
            clearhandle = 0;  //  handle has now been cleared
        }
        lastEntitySelected = downentity;
        state.addtoselection(downentity);
    }
    */
    return true;
}

void View3dWindow::lbuttondownevent(int x,int y,int buttons)
{Point p;
 Entity *handledentity;
 EntityList list;
 int handlepoint,clearhandle,alreadyselected;

    downentity = 0;
    clearhandle = 0;

    //qDebug() << "View3dWindow downevent x : " << x;
    //qDebug() << "View3dWindow downevent y : " << y;
    if(getRaytracing() == 1)
        return;

    if ((handledentity = state.gethandledentity()) != 0)
    {   if ((handlepoint = handledentity->pickhandlepoint((View3dSurface *)this,0,x,int(getheight())-1-y,&p)) != 0)
        {   state.startdrag(p,this,handlepoint);
            buttondown = 4;
            return;
        }
        else if (handledentity->pick((View3dSurface *)this,x,int(getheight())-1-y,NULL) < 0.0)
            clearhandle = 1;
    }
    else
    {   state.setpickedentity(0);
    }

    if (state.getcommandactive() == 0)
    {
        //BringWindowToTop(gethwnd());
        //if (cadwindow->viewtab && cadwindow->viewtab->get_tiled())
        cadwindow->selectwindow(this);
    }
    //cadwindow->selectwindow(this);

    SetCapture(gethwnd());

    buttondown = 1;  downx = x;  downy = y;

    if ((GetKeyState(VK_MENU) & 0x8000) && state.getcommandid() == 0)
    {  //  Start a new entity selection by dragging
        ndownpoints = 1;
        downpoint[0].x = x;
        downpoint[0].y = y;
#ifdef USING_WIDGETS
        if(qgi && getScene())
            getScene()->removeItem(qgi);
        qgi = new QGraphicsItemGroup();
        if(qgi)
        {
            //qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
            if(getScene())
                getScene()->addItem(qgi);
        }
#endif
    }

    alreadyselected = 0;
    if (((buttons & (MK_CONTROL | MK_SHIFT)) != 0 || state.getcommandid() == 0) && state.selmask.entity.test(many_mask) && coordinate3d(downx,downy,&p) &&
       (downentity = db.geometry.pick((View3dSurface *)this,0,0,downx,downy,0,(Point *)0)) != 0)
    {   alreadyselected = downentity->getselected();
        if (! alreadyselected)
        {
            downentity->highlight();
            lastEntitySelected = downentity;
        }
    }

    if ((buttons & (MK_CONTROL | MK_SHIFT)) == 0 && (downentity == 0 || ! alreadyselected))
    {   state.destroyselection(1);
        clearhandle = 0;  //  handle has now been cleared
    }

    if (clearhandle)
    {   state.sethandledentity(0);
        db.Selection(0);
    }
}

extern void drawworkplane(int drawmode,void *workplane,View3dSurface *window);

void View3dWindow::lbuttonupevent(int x,int y,int buttons)
{Point p,snapp,endp,pi,pp;
 Entity *e,*e1,*secondentity;
 EntityList list;
 int i,j,snap,addto,end,control,uvposition,handlenumber;
 BitMask mask(32);
 POINT pt;
 View3dWindow *w;
 RCHWND overhwnd;
 int n,fullcircle;
 double t,t1,t2;
 UINT oldmode;
 RCHPEN hpen;

  ReleaseCapture();

  downentity = 0;

  if (downcommand)
    {
       //  Split the line from the initial down point to the first minimum/maximum
       //  int segments based on snap tolerance and search for an entity.

#if 0
       oldmode = SetROP2(offscreenbitmap->gethdc(),R2_NOT);
       hpen = (RCHPEN)SelectObject(offscreenbitmap->gethdc(),(RCHPEN)GetStockObject(WHITE_PEN));

       for (i = 1 ; i < ndownpoints ; i++)
       {
            MoveToEx(offscreenbitmap->gethdc(),(int)downpoint[i-1].x,(int)downpoint[i-1].y,0);
            LineTo(offscreenbitmap->gethdc(),(int)downpoint[i].x,(int)downpoint[i].y);
       }

       SetROP2(offscreenbitmap->gethdc(),oldmode);
       SelectObject(offscreenbitmap->gethdc(),hpen);
#endif
       UpdateWindow();
       db.saveundo(UD_STARTBLOCK,NULL);
       db.geometry.clearstatus();
       for (i = 1 ; i < ndownpoints ; i++)
         {  n = (int)(sqrt((downpoint[i].x - downpoint[i-1].x)*(downpoint[i].x - downpoint[i-1].x) + (downpoint[i].y - downpoint[i-1].y)*(downpoint[i].y - downpoint[i-1].y)) / db.getstoler() * 1.2);
            if (n < 1) n = 1;
            for (j = i != 1 ; j <= n ; j++)
              {  downx = (int)(downpoint[i-1].x + (downpoint[i].x - downpoint[i-1].x) * double(j) / double(n));
                 downy = (int)(downpoint[i-1].y + (downpoint[i].y - downpoint[i-1].y) * double(j) / double(n));
                 if ((e = db.geometry.pick((View3dSurface *)this,0,0,downx,downy,0,(Point *)0)) != 0 && e->islinear() && e->getstatus() == 0)
                   {  e->pick((View3dSurface *)this,downx,int(getheight())-1-downy,&p);
                      e->setstatus(1);
                      if (e->islinear())
                        end = ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,1,downx,int(getheight()) - 1 - downy,&endp);
                      else if (e->isdimension())
                        end = ((Dimension *)e)->picktextindex(this,downx,int(getheight()) - 1 - downy,&p);
                      else
                        end = 1;

                      if (buttons && MK_CONTROL)
                        {  //  Select the entity
                           e->pick((View3dSurface *)this,downx,int(getheight())-1-downy,&p);
                           EntitySelectedEvent *eevent = new EntitySelectedEvent(e,p,getxaxis(),getyaxis(),this,1,end);
                           state.sendevent(eevent);
                           if(cadwindow->statusprompt != 0)
                               cadwindow->statusprompt->normalprompt(NCWIN+16);
                           if (v.getinteger("db::showhandleswhenselected") != 0 && state.getcommandid() == 0)
                             {BitMask *lockedlayertable;
                                lockedlayertable = view->getlockedlayertable() != 0 ? view->getlockedlayertable() : & db.lockedmask.layer;
                                if (! lockedlayertable->test(e->getlayer()))
                                  state.sethandledentity(e);
                             }
                           if (state.getcommandid() == 0)
                             db.Selection(e);
                        }
                      else if (e->islinear())
                        {Segments segments;
                           end = ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,1,downx,int(getheight()) - 1 - downy,&endp);
                           t = ((LinearEntity *)e)->gett(p);

                           //  Find all other intersecting entities
                           for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
                             if (e1->islinear())
                               {Intersect i(e,e1);
                                int j;
                                  for (j = 0 ; j < i.nintersections() ; j++)
                                    if (i.intersection(j)->isa(point_entity))
                                      {  t1 = ((LinearEntity *)e)->gett( ((PointE *)i.intersection(j))->getp() );
                                         t2 = ((LinearEntity *)e1)->gett( ((PointE *)i.intersection(j))->getp() );
                                         if (t1 > -0.000001 && t1 < 1.000001 && t2 > -0.000001 && t2 < 1.000001)
                                           segments.add(t1);
                                      }
                               }

                           fullcircle = e->isa(circle_entity) && ((Circle *)e)->getsweepa() > acos(-1.0) * 2.0 - 0.001;

                           if (segments.length() == 0)
                             {EntityList list;
                              int cstate,d;
                                list.add(e);
                                state.setselection(list,0);
                                d = 0;
                                delete_command(&cstate,0,(void **)&d);
                             }
                           else if (segments.length() == 1)
                             {  segments.start();
                                t1 = segments.next();
                                if (fullcircle)
                                  end = t < 0.5 ? 1 : 2;
                                else
                                  end = t < t1 ? 1 : 2;
                                ((LinearEntity *)e)->moveendpoint(1,end,((LinearEntity *)e)->position(t1));
                             }
                           else
                             {  segments.start();
                                t1 = segments.next();
                                if (t <= t1)
                                  {Point ps,pe;
                                     ps = ((LinearEntity *)e)->position(t1);
                                     t2 = t1;
                                     while (! segments.atend())
                                       t2 = segments.next();
                                     pe = ((LinearEntity *)e)->position(t2);
                                     if (fullcircle)
                                       {  ((LinearEntity *)e)->moveendpoint(1,1,ps);
                                          ((LinearEntity *)e)->moveendpoint(1,2,pe);
                                       }
                                     else
                                       ((LinearEntity *)e)->moveendpoint(1,t < t1 ? 1 : 2,ps);
                                  }
                                else
                                  {Point ps,pe;

                                     while (! segments.atend())
                                       {  t2 = segments.next();
                                          if (t1 < 0.000001 && t2 > 0.999999)
                                            {EntityList list;
                                             int cstate,d;
                                               list.add(e);
                                               state.setselection(list,0);
                                               d = 0;
                                               delete_command(&cstate,0,(void **)&d);
                                               t1 = 1.0;  t2 = 1.0;
                                               break;
                                            }
                                          else if (t1 <= t && t <= t2)
                                            ((LinearEntity *)e)->breakanddelete(fullcircle,((LinearEntity *)e)->position(t1),((LinearEntity *)e)->position(t2));
                                          t1 = t2;
                                       }
                                     if (t >= t1 && t < 1.0)
                                       {  segments.start();
                                          t1 = segments.next();
                                          ps = ((LinearEntity *)e)->position(t1);
                                          t2 = t1;
                                          while (! segments.atend())
                                            t2 = segments.next();
                                          pe = ((LinearEntity *)e)->position(t2);
                                          if (fullcircle)
                                            {  ((LinearEntity *)e)->moveendpoint(1,1,ps);
                                               ((LinearEntity *)e)->moveendpoint(1,2,pe);
                                            }
                                          else
                                            ((LinearEntity *)e)->moveendpoint(1,t < t1 ? 1 : 2,pe);
                                       }
                                  }
                             }
                        }
                   }
              }
         }

       db.saveundo(UD_ENDBLOCK,NULL);

#ifdef USING_WIDGETS
       // start the magic eraser rubberband
       if(qgi && getScene())
       {
           getScene()->removeItem(qgi);
           delete qgi;
           qgi=0;
       }
#endif

       ndownpoints = 0;
       downcommand = 0;
       buttondown = 0;
       return;

    }

  snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask);
  control = (buttons & MK_CONTROL) == MK_CONTROL;

  uvposition = state.tempentitymask.test(uv_mask) && state.selmask.entity.test(uv_mask);
  e = 0;
  if ((! control || state.getcommandid() == 0 || state.getcommandid() == 2006) && buttondown == 1 && ! uvposition)
    {
        if (snap)
        {
            // v8 enhancement for snapping to the model space origin
            // if (db.snapmask.test(SnapMaskOrigin) && (db.geometry.pickoriginpoint((View3dSurface *)this,100,downx,int(this->getheight()) - 1 - downy,&p) != 0)
            if (db.snapmask.test(SnapMaskOrigin) && (db.geometry.pickoriginpoint((View3dSurface *)this,100,downx,int(this->getheight()) - 1 - downy,&p) != 0 ||
                                                      db.geometry.pickmodelspaceoriginpoint((View3dSurface *)this,100,downx,int(this->getheight()) - 1 - downy,&p) != 0))
            {
                  Coordinate3dEvent *cevent=0;
                  if(cadwindow->coordinateentry != 0)
                       cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(p),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                  else
                       cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);

                  state.sendevent(cevent);
                  if(cadwindow->statusprompt != 0)
                      cadwindow->statusprompt->normalprompt(NCWIN+17);
                  buttondown = 0;
                  return;
            }
        }

       if ((e = db.geometry.pick((View3dSurface *)this,0,snap,downx,downy,&secondentity,(Point *)0)) != 0)
         {  if (snap)
              {  if (secondentity != 0 && db.snapmask.test(SnapMaskIntersection))
                  {//  Search for the intersecting entity
                   BitMask entity1(32),entity2(32);

                     entity1 = state.selmask.entity;
                     entity2 = state.tempentitymask;
                     state.selmask.entity.set(linear_mask);
                     state.tempentitymask.set(linear_mask);

                     for (db.geometry.start() ; (secondentity = db.geometry.next()) != 0 ; )
                       if (secondentity->islinear() &&  secondentity->isselectable(this) && secondentity->pick(this,x,getheight() - 1 - y,0) >= 0.0)
                         {Intersect in(e,secondentity);
                          int i;
                            for (i = 0 ; i < in.nintersections() ; i++)
                              if (in.intersection(i)->isa(point_entity))
                               {  p = ((PointE *)in.intersection(i))->getp();
                                  if (pick(p,100,downx,int(this->getheight()) - 1 - downy) >= 0.0)
                                    {
                                       Coordinate3dEvent *cevent=0;
                                       if(cadwindow->coordinateentry != 0)
                                           cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(p),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                                       else
                                           cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                                       state.sendevent(cevent);
                                       if(cadwindow->statusprompt != 0)
                                           cadwindow->statusprompt->normalprompt(NCWIN+17);
                                       buttondown = 0;
                                       return;
                                    }
                               }
                         }
                     state.selmask.entity = entity1;
                     state.tempentitymask = entity2;
                  }


                 if (e->islinear() && ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,0,downx,int(this->getheight()) - 1 - downy,&p) && db.snapmask.test(SnapMaskEnd))
                   {
                      Coordinate3dEvent *cevent=0;
                      if(cadwindow->coordinateentry != 0)
                         cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(p),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      else
                         cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);

                      state.sendevent(cevent);
                      if(cadwindow->statusprompt != 0)
                          cadwindow->statusprompt->normalprompt(NCWIN+17);
                      buttondown = 0;
                      return;
                   }
#if VERSION != FLEXICAD
                 else if (e->isa(wall_entity) && ((Wall *)e)->pickpoint((View3dSurface *)this,0,downx,int(this->getheight()) - 1 - downy,&p) && db.snapmask.test(SnapMaskEnd))
                   {
                      Coordinate3dEvent *cevent=0;
                      if(cadwindow->coordinateentry != 0)
                        cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(p),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      else
                        cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      state.sendevent(cevent);
                      if(cadwindow->statusprompt != 0)
                          cadwindow->statusprompt->normalprompt(NCWIN+17);
                      buttondown = 0;
                      return;
                   }
                 else if (e->isa(beam_entity) && ((Beam *)e)->pickpoint((View3dSurface *)this,0,downx,int(this->getheight()) - 1 - downy,&p) && db.snapmask.test(SnapMaskEnd))
                   {
                      Coordinate3dEvent *cevent=0;
                      if(cadwindow->coordinateentry != 0)
                         cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(p),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      else
                         cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      state.sendevent(cevent);
                      if(cadwindow->statusprompt != 0)
                          cadwindow->statusprompt->normalprompt(NCWIN+17);
                      buttondown = 0;
                      return;
                   }
#endif
                 else if (e->isa(point_entity))
                   {
                      Coordinate3dEvent *cevent=0;
                      if(cadwindow->coordinateentry != 0)
                        cevent = new Coordinate3dEvent(cadwindow->coordinateentry->fix(((PointE *)e)->getp()),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      else
                        cevent = new Coordinate3dEvent(((PointE *)e)->getp(),getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                      state.sendevent(cevent);
                      if(cadwindow->statusprompt != 0)
                          cadwindow->statusprompt->normalprompt(NCWIN+17);
                      buttondown = 0;
                      return;
                   }
                 else
                   {  if (! e->isselectable(this))
                        {  state.destroyselection(1);
                           if (mask.test(uv_mask))
                             {  CoordinateuvEvent *uevent = new CoordinateuvEvent(uv(downx,downy),control,this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                                if(cadwindow->statusprompt != 0)
                                    cadwindow->statusprompt->normalprompt(NCWIN+18);
                                state.sendevent(uevent);
                             }
                           else if (coordinate3d(downx,downy,&p))
                             {  Coordinate3dEvent *cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
                                if(cadwindow->statusprompt != 0)
                                    cadwindow->statusprompt->normalprompt(NCWIN+18);
                                state.sendevent(cevent);
                             }
                           buttondown = 0;
                           return;
                        }
                   }
              }
            e->pick((View3dSurface *)this,downx,int(getheight())-1-downy,&p);
            if (e->islinear())
              end = ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,1,downx,int(getheight()) - 1 - downy,&endp);
            else if (e->isdimension())
              end = ((Dimension *)e)->picktextindex(this,downx,int(getheight()) - 1 - downy,&p);
            else
              end = 1;
            EntitySelectedEvent *eevent = new EntitySelectedEvent(e,p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0,end);
            state.sendevent(eevent);
            if(cadwindow->statusprompt != 0)
                cadwindow->statusprompt->normalprompt(NCWIN+16);
            if (v.getinteger("db::showhandleswhenselected") != 0 && state.getcommandid() == 0)
              {BitMask *lockedlayertable;
                 lockedlayertable = view->getlockedlayertable() != 0 ? view->getlockedlayertable() : & db.lockedmask.layer;
                 if (! lockedlayertable->test(e->getlayer()) && ! e->isa(plane_entity) && ! e->isa(patch_entity) && ! e->isa(ruledsurface_entity) && ! e->isa(surfaceofrev_entity))
                   state.sethandledentity(e);
              }
            if (state.getcommandid() == 0)
              db.Selection(e);

           if ((GetKeyState(VK_SHIFT) & 0x8000) != 0 && (GetKeyState(VK_CONTROL) & 0x8000) != 0 && e->isa(plane_entity))
             {//  Align the workplane to this plane
                cadwindow->drawcallback(DM_ERASE,db.workplanes.getcurrent(),drawworkplane);
                if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                  cadwindow->drawcallback(DM_ERASE,&db.grid,drawgrid);
                db.saveundo(UD_CHANGEWORKPLANE,db.workplanes.getcurrent());
                db.workplanes.getcurrent()->change(((Plane *)e)->getorigin(),((Plane *)e)->getxaxis(),((Plane *)e)->getyaxis());
                cadwindow->drawcallback(DM_NORMAL,db.workplanes.getcurrent(),drawworkplane);
                if (db.grid.getplane() == onworkplane && db.grid.getdisplay())
                  cadwindow->drawcallback(DM_NORMAL,&db.grid,drawgrid);
             }
         }
       else
         db.workplanes.pick(0,downx,int(this->getheight()) - 1 - downy,this);
    }
  else if ((! control || state.getcommandid() == 0 || state.getcommandid() == 2006) && ! uvposition && buttondown == 2)
    {int c;
     EntityList selection,newselectionlist;

#if 1
     // unfreeze the background image and draw to the graphic view again
     ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->unFreezeBackImage();
     //delete ((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->backImage;
     //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->backImage = 0;
     //
#endif
       cadwindow->startdrawingbatch();

       p.setxyz(0.0,0.0,0.0);
       list = db.geometry.pick((View3dSurface *)this,0,downx,downy,x,y);
       if (! (buttons & (MK_SHIFT | MK_CONTROL)))
         {  addto = 2;  state.destroyselection(1);
         }
       else
         addto = 1;


       for (list.start() ;  (e = list.next()) != NULL ; )
         e->setstatus(0);
       selection = state.getselection();
       for (selection.start() ;  (e = selection.next()) != NULL ; )
         e->setstatus(1);

       list.reverse();
       for (list.start(),c = 0 ;  (e = list.next()) != NULL ; c++)
         {RCMSG msg;
            EntitySelectedEvent *eevent = new EntitySelectedEvent(e,p,getxaxis(),getyaxis(),this,addto + 10 + 10 * e->getstatus(),0);
            state.sendevent(eevent);
            if (c % 100 == 0 &&
                  (PeekMessage(&msg,0,WM_RBUTTONDOWN,WM_RBUTTONDOWN,PM_NOREMOVE) ||
                   PeekMessage(&msg,0,WM_KEYDOWN,WM_KEYDOWN,PM_NOREMOVE) && msg.wParam == VK_ESCAPE))
              break;
         }

       cadwindow->enddrawingbatch();

       if (list.length() > 0)
       {
         if(cadwindow->statusprompt != 0)
             cadwindow->statusprompt->normalprompt(NCWIN+16);
       }
       //  Update the selection list - removing any entities that are no longer highlighted
       //  This is faster than removing them from the list one by one
       selection = state.getselection();
       for (selection.start() ; (e = selection.next()) != NULL ; )
         if (e->getselected() != 0)
           newselectionlist.add(e);
       newselectionlist.reverse();
       state.setselection(newselectionlist,3);  //  Don't draw the old or new selection list

       cursor->style(1);  cursor->action(0);
       cursor->mousemoveevent(x,y,buttons);
    }
  else if (! uvposition && (buttondown == 3 || buttondown == 4))
    {  handlenumber = state.getdragdrawn() - 1;
#ifndef NEW_DRAGGING
       state.enddrag();
#else
      state.enddragGL();
#endif
       pt.x = x;  pt.y = y;
       ClientToScreen(gethwnd(),&pt);
       ScreenToClient(cadwindow->subwindow->gethwnd(),&pt);
       overhwnd = ChildWindowFromPoint(cadwindow->subwindow->gethwnd(),pt);
#if 1
       if(dynamic_cast<QGLWidget*>((QObject*)overhwnd) != 0)
           overhwnd = ((QGLWidget*)overhwnd)->parent()->parent();
#else
       if(dynamic_cast<RCGLWidget*>((QObject*)overhwnd) != 0)
           overhwnd = ((RCGLWidget*)overhwnd)->parent()->parent();
#endif
       if (overhwnd == 0 || overhwnd == gethwnd() || (w = cadwindow->matchview3dwindow(overhwnd)) == 0)
         w = this;
       else
         w = cadwindow->matchview3dwindow(overhwnd);
       pt.x = x;  pt.y = y;
       ClientToScreen(gethwnd(),&pt);
       ScreenToClient(w->gethwnd(),&pt);
       if (w->coordinate3d(pt.x,pt.y,&p))
         {  if (buttondown == 4)
              {  control = (buttons & MK_CONTROL) == MK_CONTROL;
                 e = 0;
                 if (! control)
                   {  if ((e = db.geometry.pick(w,0,1,pt.x,pt.y,0,(Point *)0)) != 0)
                        {  if (e->islinear() && ((LinearEntity *)e)->pickendpoint((View3dSurface *)this,0,pt.x,int(this->getheight()) - 1 - pt.y,&snapp))
                             {  if(cadwindow->statusprompt != 0)
                                 cadwindow->statusprompt->normalprompt(NCWIN+17);
                                p = snapp;
                             }
//                           else if (e->islinear() && ((LinearEntity *)e)->picksnappoint((View3dSurface *)this,0,pt.x,int(this->getheight())- 1 - pt.x,&snapp))
//                             {  if(cadwindow->statusprompt != 0) cadwindow->statusprompt->normalprompt(NCWIN+17);
//                                p = snapp;
//                             }
#if VERSION != FLEXICAD
                           else if (e->isa(wall_entity) && ((Wall *)e)->pickpoint((View3dSurface *)this,0,pt.x,int(this->getheight()) - 1 - pt.y,&snapp))
                             {  if(cadwindow->statusprompt != 0)
                                 cadwindow->statusprompt->normalprompt(NCWIN+17);
                                p = snapp;
                             }
                           else if (e->isa(beam_entity) && ((Beam *)e)->pickpoint((View3dSurface *)this,0,pt.x,int(this->getheight()) - 1 - pt.y,&snapp))
                             {  if(cadwindow->statusprompt != 0)
                                 cadwindow->statusprompt->normalprompt(NCWIN+17);
                                p = snapp;
                             }
#endif
                           else if (e->isa(point_entity))
                             {  if(cadwindow->statusprompt != 0)
                                 cadwindow->statusprompt->normalprompt(NCWIN+17);
                                p = ((PointE *)e)->getp();
                             }
                          else
                             {  state.excludelist.add(e);
                                snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask);
                                if ((e1 = db.geometry.pick(this,0,snap,pt.x,pt.y,0,&p)) != 0)
                                  {Intersect i(e,e1,p);
                                     if (i.nintersections() == 1)
                                       { if(cadwindow->statusprompt != 0)
                                             cadwindow->statusprompt->normalprompt(NCWIN+17);
                                          p = ((PointE*)i.intersection(0))->getp();
                                       }
                                  }
                                state.excludelist.del(e);
                             }
                        }
                   }
                 state.gethandledentity()->movehandlepoint(this,1,handlenumber,p);
              }
            else if (buttons & MK_CONTROL)
#ifdef NEW_DRAGGING
              state.copyselectionGL(p,w);
#else
               state.copyselection(p,w);
#endif
            else
#ifdef NEW_DRAGGING
              state.moveselectionGL(p,w);
#else
               state.moveselection(p,w);
#endif
         }
       cadwindow->prompt->setcolour(RGB(0,150,0));
       cadwindow->prompt->normalprompt(NCWIN+1);
    }
  else if (buttondown == 2)
    {  cursor->style(1);  cursor->action(0);
       cursor->mousemoveevent(x,y,buttons);
    }

  if ((buttondown == 0 || buttondown == 1 && e == NULL) && ! state.selmask.entity.test(many_mask))
    {  mask = state.tempentitymask;
       mask &= state.selmask.entity;
       if (mask.test(uv_mask))
         {  CoordinateuvEvent *uevent = new CoordinateuvEvent(uv(downx,downy),control,this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
            state.sendevent(uevent);
            if(cadwindow->statusprompt != 0)
                cadwindow->statusprompt->normalprompt(NCWIN+18);
         }
       else if (mask.test(xyz_mask) && coordinate3d(downx,downy,&p))
         {  state.destroyselection(1);
            Coordinate3dEvent *cevent = new Coordinate3dEvent(p,getxaxis(),getyaxis(),this,(buttons & (MK_SHIFT | MK_CONTROL)) != 0);
            state.sendevent(cevent);
            if(cadwindow->statusprompt != 0)
                cadwindow->statusprompt->normalprompt(NCWIN+18);
         }
       else
         state.destroyselection(1);
    }
  else if ((buttondown == 0 || buttondown == 1 && e == NULL) && (buttons & (MK_SHIFT | MK_CONTROL)) == 0)
    state.destroyselection(1);
  buttondown = 0;

  UpdateWindow();
  //((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
  //cadwindow->update_properties(-1);

}

void View3dWindow::mbuttondownevent(int x,int y,int buttons)
{ SetCapture(hWnd);
  //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->setTimer(v.getinteger("wn::screenrefreshdelay"));
  downx = x;
  downy = y;
  scrolling = 1;
  db.savezoomundo(1,this);
  //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->setTimer(v.getinteger("wn::screenrefreshdelay"));
  //app->processEvents();
}

void View3dWindow::mbuttonupevent(int x,int y,int buttons)
{ ReleaseCapture();

    /*
  QElapsedTimer timer;
  timer.start();
  int delay = v.getinteger("wn::screenrefreshdelay");
  while(!timer.hasExpired(delay));
    */
  scrolling = 0;

  //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->regenGLlists();
  //((RCGraphicsView*)((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view())->setTimer(v.getinteger("wn::screenrefreshdelay"));
  //app->processEvents();

}

void View3dWindow::mbuttondblclkevent(int x,int y,int buttons)
{int cstate = InitialState;
 Point pmin,pmax;
 double du,dv;
 View2dWindow *window;

  program->setbusy(1);

  if (getsurfacetype() == View2dSurf)
    {  window = (View2dWindow *) this;
       pmin.setxyz(0.0,0.0,0.0);
       pmax.setxyz(window->getview2d()->getpaperwidth(),window->getview2d()->getpaperheight(),0.0);
    }
  else
    db.geometry.extents(this,&pmin,&pmax);

  if (! program->getinterrupt())
    {  if (pmax.x - pmin.x > db.getptoler() || pmax.y - pmin.y > db.getptoler())
         {  du = (pmax.x - pmin.x) * 0.02;
            dv = (pmax.y - pmin.y) * 0.02;
            db.savezoomundo(0,this);
                  zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
          }
    }

  program->setbusy(0);

}

void View3dWindow::lbuttondblclkevent(int,int,int)
{EntityList list;
 Entity *e;
 BitMask *lockedlayertable;

  list = state.getselection();
  if (list.length() == 1)
     {  cadwindow->cursoroff();
        list.start();
        e = list.next();
        lockedlayertable = view->getlockedlayertable() != 0 ? view->getlockedlayertable() : & db.lockedmask.layer;
        if (! lockedlayertable->test(e->getlayer()) && ! e->isa(plane_entity) && ! e->isa(patch_entity) && ! e->isa(ruledsurface_entity) && ! e->isa(surfaceofrev_entity))
          state.sethandledentity(e);
        // KMJ: debug added this line because of a problem when verifying surface entities
        // will remove it if I find a better solution
        downentity=0;
        //
        db.Verify(e);
        cadwindow->cursoron();
        buttondown = 5;
     }
  else
    db.workplanes.pick(1,downx,int(this->getheight()) - 1 - downy,this);
}

void View3dWindow::zoom(double umin1,double vmin1,double umax1,double vmax1)
{
 double du,dv;
 RCMSG msg;
 int scalechanged;

 //qDebug() <<  "Entering : View3dWindow::zoom";
 //qDebug() <<  "View3dWindow::zoom : View3dWindow  : " << (View3dWindow*)this;
 //qDebug() <<  "View3dWindow::zoom : Window        : " << (Window*)this;
 //qDebug() <<  "View3dWindow::zoom : View3dSurface : " << (View3dSurface*)this;
 //qDebug() <<  "View3dWindow::zoom : parent        : " << this->parent;
 //qDebug() <<  "View3dWindow::zoom : scene         : " << (View3dWindow*)this->getScene();

 //qDebug() <<  "View3dWindow::zoom in umin1 : " << umin1;
 //qDebug() <<  "View3dWindow::zoom in vmin1 : " << vmin1;
 //qDebug() <<  "View3dWindow::zoom in umax1 : " << umax1;
 //qDebug() <<  "View3dWindow::zoom in vmax1 : " << vmax1;

 //qDebug() << "zoom center u : " << umin1 + (umax1 - umin1) * 0.5;
 //qDebug() << "zoom center v : " << vmin1 + (vmax1 - vmin1) * 0.5;

 //qDebug() <<  "View3dWindow::zoom in umin : " << umin;
 //qDebug() <<  "View3dWindow::zoom in vmin : " << vmin;
 //qDebug() <<  "View3dWindow::zoom in umax : " << umax;
 //qDebug() <<  "View3dWindow::zoom in vmax : " << vmax;

  du = fabs(umax1 - umin1) / 2.0;  dv = fabs(vmax1 - vmin1) / 2.0;
  if (du < db.getptoler() && dv < db.getptoler())
      return;

  scalechanged = (fabs(fabs(umax1-umin1) - fabs(umax-umin)) > fabs(umax-umin) / 100000.0||
                  fabs(fabs(vmax1-vmin1) - fabs(vmax-vmin))  > fabs(vmax-vmin) / 100000.0);
  if (v.getinteger("vw::autoviewrefresh") && scalechanged && offscreenbitmap != 0)
    offscreenbitmap->displaylist.clear();

  umin = umin1;
  vmin = vmin1;
  umax = umax1;
  vmax = vmax1;
  fixzoom();
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  if (db.getenableautoplotscale() != 0 && getsurfacetype() != View2dSurf)
    plotscale = (umax - umin) / getwidthmm() * db.getautoplotscale();
  if (offscreenbitmap != 0)
    offscreenbitmap->displaylist.setdrawtransform(&umin,&vmin,&umax,&vmax);
  updatematrix();
#ifdef USING_WIDGETS
   paintevent();
#else
  if (PeekMessage(&msg,gethwnd(),WM_PAINT,WM_PAINT,PM_NOREMOVE) == 0)
    InvalidateRect(gethwnd(),0,TRUE);
#endif
  //qDebug() <<  "View3dWindow::zoom out umin : " << umin;
  //qDebug() <<  "View3dWindow::zoom out vmin : " << vmin;
  //qDebug() <<  "View3dWindow::zoom out umax : " << umax;
  //qDebug() <<  "View3dWindow::zoom out vmax : " << vmax;

  //qDebug() <<  "Leaving : View3dWindow::zoom";
}

void View3dWindow::fixzoom()
{double umin1,umax1,vmin1,vmax1,aspect,du,dv;
  if (IsIconic(gethwnd()) || getheight() < 2 || getwidth() < 2) return;
  umin1 = umin;
  vmin1 = vmin;
  umax1 = umax;
  vmax1 = vmax;
  du = fabs(umax1 - umin1) / 2.0;
  dv = fabs(vmax1 - vmin1) / 2.0;
  if (du < db.getptoler() && dv < db.getptoler()) return;
  if (du < db.getptoler())
    du = dv * 0.01;
  if (dv < db.getptoler())
    dv = du * 0.01;
  aspect = getwidthmm() / getheightmm() * v.getreal("vw::aspectadjustment");
  if (du / dv > aspect) dv = du / aspect;
  if (du / dv < aspect) du = dv * aspect;
  umin = (umin1 + umax1) / 2.0 - du;
  vmin = (vmin1 + vmax1) / 2.0 - dv;
  umax = (umin1 + umax1) / 2.0 + du;
  vmax = (vmin1 + vmax1) / 2.0 + dv;
}

View3dWindow *View3dWindow::load(CadFile *infile)
{
 short x,y,w,h;
 long l;
 char *cviewname;
 RCCHAR *viewname;
 double umin,umax,vmin,vmax,plotscale;
 View *v;
 View3dWindow *win;
 int restorewindowplacement;
 WINDOWPLACEMENT wp;

 if( infile->getversion() > 2 )
 {
     if (! infile->read(&viewname))
         return NULL;
 }
 else
 {
     if (! infile->read(&cviewname)) return NULL;
     viewname = new RCCHAR[strlen(cviewname)+1];
     strcpy(viewname,cviewname);
     delete [] cviewname;
     cviewname=0;
 }
  if (viewname[0] == '\001')
    {//  This is the WindowPlacement save format.
      delete [] viewname;
      viewname=0;
      if( infile->getversion() == 2 )
      {
          if (! infile->read(&cviewname)) return NULL;
          viewname = new RCCHAR[strlen(cviewname)+1];
          strcpy(viewname,cviewname);
          delete [] cviewname;
          cviewname=0;
      }
      else
      {
          if (! infile->read(&viewname))
              return NULL;
      }
      qDebug() << "view name : " << QString(viewname);
       restorewindowplacement = 1;
       memset(&wp,0,sizeof(wp));
       wp.flags = WPF_SETMINPOSITION;
       wp.length = sizeof(wp);
       if (! infile->read(&l)) return NULL;
       wp.showCmd = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMinPosition.x = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMinPosition.y = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMaxPosition.x = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMaxPosition.y = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.left = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.top = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.right = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.bottom = l;
       if (! infile->read(&umin)) return NULL;
       if (! infile->read(&vmin)) return NULL;
       if (! infile->read(&umax)) return NULL;
       if (! infile->read(&vmax)) return NULL;
       if (! infile->read(&plotscale)) return NULL;

       x = (short)wp.rcNormalPosition.left;
       y = (short)wp.rcNormalPosition.top;
       w = (short)wp.rcNormalPosition.right - x;
       h = (short)wp.rcNormalPosition.right - y;

       if(w < 0)
           w=1;
       if(h < 0)
           h=1;

    }
  else
    {  restorewindowplacement = 0;
       if (! infile->read(&x)) return NULL;
       if (! infile->read(&y)) return NULL;
       if (! infile->read(&w)) return NULL;
       if (! infile->read(&h)) return NULL;
       if (! infile->read(&umin)) return NULL;
       if (! infile->read(&vmin)) return NULL;
       if (! infile->read(&umax)) return NULL;
       if (! infile->read(&vmax)) return NULL;
       if (! infile->read(&plotscale)) return NULL;
    }

  if ((v = db.views3d.match(viewname)) == NULL) return NULL;
  delete [] viewname;
  viewname=0;
  if (cadwindow != 0)
    {
       if (x + w > cadwindow->subwindow->getwidth())
         x = int(cadwindow->subwindow->getwidth() - w - 1);
       if (x < 0)
         x = 0;
       if (y + h > cadwindow->subwindow->getheight())
         y = int(cadwindow->subwindow->getheight() - h - 1);
       if (y < 0)
         y = 0;


       // KMJ: ToDo: fix Window load save stuff

       //qDebug() << "cadwindow-subwindow :" << ((QWidget*)cadwindow->subwindow->gethwnd());
       //qDebug() << "cadwindow-subwindow :" << ((QWidget*)cadwindow->subwindow->gethwnd())->geometry();
       //qDebug() << "cadwindow-subwindow x :" << x;
       //qDebug() << "cadwindow-subwindow y :" << y;
       //qDebug() << "cadwindow-subwindow w :" << w;
       //qDebug() << "cadwindow-subwindow h :" << h;
       //qDebug() << "cadwindow-subwindow width :" << cadwindow->subwindow->getwidth();
       //qDebug() << "cadwindow-subwindow height :" << cadwindow->subwindow->getheight();
       //qDebug() << "cadwindow-subwindow x :" << int(cadwindow->subwindow->getwidth() - w - 1);
       //qDebug() << "cadwindow-subwindow y :" << int(cadwindow->subwindow->getheight() - h - 1);

       RCCentralWidget *cw = (RCCentralWidget*)cadwindow->subwindow->gethwnd();
       win = new View3dWindow(cadwindow,cadwindow->subwindow,v,x,y,int(cw->geometry().width()),int(cw->geometry().height()));
       //win = new View3dWindow(cadwindow,cadwindow->subwindow,v,x,y,w,h,umin,vmin,umax,vmax);
       if (win != NULL)
         {
            win->setplotscale(plotscale);
            if (restorewindowplacement)
            {
                 if (wp.showCmd == SW_SHOWMINIMIZED || wp.showCmd == SW_SHOWMAXIMIZED)
                   win->setshowcommand(wp.showCmd);
                 wp.showCmd = SW_HIDE;   //  Windows are now hidden at first
                 SetWindowPlacement(win->gethwnd(),&wp);
                 //((RCView*)win->gethwnd())->move(QPoint(wp.rcNormalPosition.left,wp.rcNormalPosition.top));
                 //((RCView*)win->gethwnd())->resize(wp.rcNormalPosition.right-wp.rcNormalPosition.left,
                 //                                  wp.rcNormalPosition.bottom-wp.rcNormalPosition.top);
            }
            // KMJ: ToDo : fix window loading
            cadwindow->sizeevent(-1,-1);
            win->zoom(umin,vmin,umax,vmax);
         }
       return win;
    }
  else
    return (View3dWindow *) -1;
}

int View3dWindow::save(CadFile *outfile)
{
  short type;
  long x;
  WINDOWPLACEMENT wp;

  if (GetPrivateProfileInt("Save","NoMaskTables",0,home.getinifilename()) != 0)
  {
      short type,x,y,w,h;
      RECT rect1,rect2;
       GetWindowRect(gethwnd(),&rect1);
       GetWindowRect(GetParent(gethwnd()),&rect2);
       x = short(rect1.left - rect2.left);  y = short(rect1.top - rect2.top);
       w = short(rect1.right - rect1.left); h = short(rect1.bottom - rect1.top);
       type = View3dSurf;
       if (! outfile->write(&type)) return 0;
       if (! outfile->write(view->getname())) return 0;
       if (! outfile->write(&x)) return 0;
       if (! outfile->write(&y)) return 0;
       if (! outfile->write(&w)) return 0;
       if (! outfile->write(&h)) return 0;
       if (! outfile->write(&umin)) return 0;
       if (! outfile->write(&vmin)) return 0;
       if (! outfile->write(&umax)) return 0;
       if (! outfile->write(&vmax)) return 0;
       if (! outfile->write(&plotscale)) return 0;
       return 1;
  }

#if 0 // new version for Qt style splitter windows
  type = View3dSurf;
  if (! outfile->write(&type)) return 0;
  if (! outfile->write(_RCT("\002"))) return 0;
  if (! outfile->write(view->getname())) return 0;
  QByteArray splitter;
  splitter = ((RCCentralWidget*)((RCView*)this->gethwnd())->centralWidget())->mSplitter->saveState();
  long spsize = splitter.size();
  outfile->write(&spsize);
  outfile->blockwrite(splitter.data(),spsize);
#else // old version for windows only
  memset(&wp,0,sizeof(wp));
  wp.length = sizeof(wp);
  GetWindowPlacement(gethwnd(),&wp);
  type = View3dSurf;
  if (! outfile->write(&type)) return 0;
  if (! outfile->write(_RCT("\001"))) return 0;
  if (! outfile->write(view->getname())) return 0;
  qDebug() << "view name : " << QString(view->getname());
  x = wp.showCmd;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMinPosition.x;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMinPosition.y;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMaxPosition.x;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMaxPosition.y;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.left;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.top;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.right;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.bottom;
  if (! outfile->write(&x)) return 0;
#endif
  if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;
  if (! outfile->write(&plotscale)) return 0;
  return 1;
}

int View3dWindow::load(UndoFile *infile)
{RCMSG msg;
 short loadview;
  if (! infile->read(&loadview)) return 0;
  if (loadview)
    {  if (! view->load(infile)) return 0;
       if (offscreenbitmap != 0)
         offscreenbitmap->displaylist.clear();
    }
  else
    {  if (! infile->read(&umin)) return 0;
       if (! infile->read(&vmin)) return 0;
       if (! infile->read(&umax)) return 0;
       if (! infile->read(&vmax)) return 0;
       if (! infile->read(&plotscale)) return 0;
    }
  fixzoom();
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  if (offscreenbitmap != 0)
    offscreenbitmap->displaylist.setdrawtransform(&umin,&vmin,&umax,&vmax);
  updatematrix();
  //if (! PeekMessage(&msg,gethwnd(),WM_PAINT,WM_PAINT,PM_NOREMOVE))
  //  InvalidateRect(gethwnd(),0,TRUE);
  paintevent();
  return 1;
}

int View3dWindow::save(UndoFile *outfile,int saveview)
{short v;
  v = short(saveview);
  if (! outfile->write(&v)) return 0;
  if (v)
    {  if (! view->save(outfile)) return 0;
    }
  else
    {  if (! outfile->write(&umin)) return 0;
       if (! outfile->write(&vmin)) return 0;
       if (! outfile->write(&umax)) return 0;
       if (! outfile->write(&vmax)) return 0;
       if (! outfile->write(&plotscale)) return 0;
    }
  return 1;
}

View3dWindow::View3dWindow(MainWindow *p,Window *window,View *v,int x,int y,int w,int h,DWORD wintype,RCPVOID params) :
    // p is the MainWindow, window is the QMidArea
    //Window(_RCT("View3dWindow"),window != 0 ? window->gethwnd() : 0,(DWORD)(wintype | ::v.getinteger("wn::scrollbars")*(WS_HSCROLL   | WS_VSCROLL)) * (window != 0),x,y,w,h,SW_SHOW),
  Window(_RCT("CadWindow"),window != 0 ? window->gethwnd() : 0,(DWORD)( wintype | ::v.getinteger("wn::scrollbars")*(WS_HSCROLL   | WS_VSCROLL)) * (window != 0),x,y,w,h,SW_SHOW,params),
  View3dSurface(v,-w/2.0,-h/2.0,w/2.0,h/2.0,1,0),
  visible(MaxViews)
{
 RECT rect;
 double scale;
 char text[300];

#ifdef USING_WIDGETS
    mcount = ++v3dcount;
    qgi = 0;
    qgich = 0;
    qgirb = 0;
#endif
    qDebug() << "int View3dWindow CTOR1: " << mcount;

  //qDebug() << "Window hWnd : " << (QObject*)window->gethwnd();
  // the Window of  View3dWindow is an RCCentralWidget object

  GetPrivateProfileString("MouseWheel","Direction","1",text,300,home.getinifilename());
  wheeldirection = atoi(text);
  if (wheeldirection == 0)
      wheeldirection = -1;
  else
      wheeldirection = 1;

  parent = p;

  if (::v.getinteger("wn::scrollbars") != 0)
    {  SetScrollRange(gethwnd(),SB_HORZ,0,1000,TRUE);
       SetScrollRange(gethwnd(),SB_VERT,0,1000,TRUE);
    }

  //qDebug() << "View3dWindow hWnd : " << (QObject*)hWnd;
  //qDebug() << "View3dWindow hCD : " << (QObject*)hDC;
  // the hWnd of a View3dWindow is an RCView object
  //GetClientRect(gethwnd(),&rect);
  GetClientRect(((RCView*)hWnd)->view(),&rect);
  //GetClientRect(((RCView*)hWnd)->view()->viewport(),&rect);

  scale = ::v.getreal("zm::scale");
  if (::v.getinteger("vw::enableautoplotscale") != 0  && getsurfacetype() != View2dSurf)
    plotscale = scale * ::v.getreal("vw::autoplotscale");

  offscreenbitmap = 0;
  cursor = new Cursor(this,rect.right,rect.bottom);
  umin = -getwidthmm() / 2.0 * scale;   umax = getwidthmm() / 2.0 * scale;
  vmin = -getheightmm() / 2.0 * scale;  vmax = getheightmm() / 2.0 * scale;
  buttondown = 0;
  offscreenpaint = 0;
  scrolling = 0;

  Surface::sizeevent(rect.right,rect.bottom);
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  //qDebug() << "View3dWindow hDC : " << (QObject*)hDC;
  // the hDC of this View3dWindow is an RCView object
  SetBkColor(((RCView*)hWnd)->view(),getbackground());

  // give the RCView our pointer
  ((RCView*)hWnd)->setView3dWindow(this);

  updatematrix();
  getvisibility();

  showcommand = 0;
  ndownpoints = 0;
  downcommand = 0;
  downentity = 0;

  if (selectionlayertable != view->getselectionlayertable())
    selectionlayertable = view->getselectionlayertable();
  if (displaylayertable != view->getdisplaylayertable())
    displaylayertable = view->getdisplaylayertable();
  if (lockedlayertable != view->getlockedlayertable())
    lockedlayertable = view->getlockedlayertable();

#ifdef USING_WIDGETS
  if(hWnd)
  {
      //qDebug() << "window widget : " << (QWidget*)hWnd;
      //qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
  }
#ifdef USING_TABBED_WINDOWS
  if(hWnd && ((QWidget*)hWnd)->objectName() == "QGraphicsView" )
  {
      //qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
      qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((QGraphicsView*)hWnd)->scene();
      qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
      setScene(((QGraphicsView*)hWnd)->scene());
      qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
      //qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
      qDebug() << "current tab : " << ((QTabWidget*)window->gethwnd())->currentIndex();
      qDebug() << "new window tab : " << ((QTabWidget*)window->gethwnd())->indexOf((QWidget*)hWnd);
      if(((QWidget*)window->gethwnd())->objectName() == "QTabWidget")
          ((QTabWidget*)window->gethwnd())->setTabText(((QTabWidget*)window->gethwnd())->indexOf((QWidget*)hWnd),QString(view->getname()));
#endif
#ifdef USING_QMDAREA
   if(hWnd && ((QWidget*)hWnd)->objectName() == "QGraphicsView" )
   {
      qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
      qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((QGraphicsView*)hWnd)->scene();
      qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
      setScene(((QGraphicsView*)hWnd)->scene());
      qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
      qDebug() << "window widget parent object : " << ((QWidget*)window->gethwnd());
      qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
      qDebug() << "window widget parent parent object : " << ((QMdiArea*)window->gethwnd());
      qDebug() << "window widget parent parent name   : " << ((QMdiArea*)window->gethwnd())->objectName();
      //qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->centralWidget())->activeSubWindow();
      qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->MdiArea)->activeSubWindow();

      if(((QWidget*)window->gethwnd())->objectName() == "QMdiSubWindow")
          ((QMdiSubWindow*)window->gethwnd())->setWindowTitle(QString(view->getname()));
#endif
   if(hWnd && ((QWidget*)hWnd)->objectName() == "RCView" )
   {
      //qDebug() << "window widget name   : " << ((QWidget*)hWnd)->objectName();
      //qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((RCView*)hWnd)->view()->scene();
      //qDebug() << "Graphics View Scene  : " << ((RCView*)hWnd)->view()->scene()->objectName();
      //qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
      setScene(((RCView*)hWnd)->view()->scene());
      //qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
      //qDebug() << "window widget parent object : " << ((QWidget*)window->gethwnd());
      //qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
      //qDebug() << "window widget parent parent object : " << ((RCCentralWidget*)window->gethwnd());
      //qDebug() << "window widget parent parent name   : " << ((RCCentralWidget*)window->gethwnd())->objectName();
      //qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->centralWidget())->activeSubWindow();
      //qDebug() << "current window : " << ((RCCentralWidget*)((QtMainWindow*)p->gethwnd())->centerWidget)->activeWindow();

      //if(((QWidget*)window->gethwnd())->objectName() == "centerWidget")
      //    if(((RCCentralWidget*)((QtMainWindow*)p->gethwnd())->centerWidget)->activeWindow() !=0)
      //      ((RCView*)((RCCentralWidget*)((QtMainWindow*)p->gethwnd())->centerWidget)->activeWindow())->setWindowTitle(QString(view->getname()));

      ((RCView*)hWnd)->setWindowTitle(QString(view->getname()),"","");

  }
  else
    setScene(0);
  UpdateWindow( );
#endif
//  UpdateWindow(gethwnd());
}

View3dWindow::View3dWindow(MainWindow *p,Window *window,View *v,int x,int y,int w,int h,double umin1,double vmin1,double umax1,double vmax1,DWORD wintype,RCPVOID params) :
    //Window(_RCT("View3dWindow"),window->gethwnd(),(DWORD)(wintype),x,y,w,h,SW_SHOW),
    Window(_RCT("CadWindow"),window->gethwnd(),(DWORD)(wintype),x,y,w,h,SW_SHOW,params),
  View3dSurface(v,umin1,vmin1,umax1,vmax1,1,0),
  visible(MaxViews)
{RECT rect;
 char text[300];

#ifdef USING_WIDGETS
     mcount = ++v3dcount;
    qgi = 0;
    qgich = 0;
    qgirb = 0;
#endif
    qDebug() << "int View3dWindow CTOR2: " << mcount;

  GetPrivateProfileString("MouseWheel","Direction","1",text,300,home.getinifilename());
  wheeldirection = atoi(text);
  if (wheeldirection == 0)
      wheeldirection = -1;
  else
      wheeldirection = 1;

  showcommand = 0;

  parent = p;

  // the hWnd of a View3dWindow is an RCView object
  //GetClientRect(gethwnd(),&rect);
  GetClientRect(((RCView*)hWnd)->view(),&rect);
  // give the RCView our pointer
  ((RCView*)hWnd)->setView3dWindow(this);

  offscreenbitmap = 0;
  cursor = new Cursor(this,rect.right,rect.bottom);
  buttondown = 0;
  offscreenpaint = 0;
  scrolling = 0;
  fixzoom();
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
  getvisibility();
  //qDebug() << "View3dWindow hDC : " << (QObject*)hDC;
  // the hDC of this window is an RCView object
  //SetBkColor(hDC,getbackground());
  SetBkColor(((RCView*)hWnd)->view(),getbackground());

  ndownpoints = 0;
  downcommand = 0;
  downentity = 0;


  if (selectionlayertable != view->getselectionlayertable())
    selectionlayertable = view->getselectionlayertable();
  if (displaylayertable != view->getdisplaylayertable())
    displaylayertable = view->getdisplaylayertable();
  if (lockedlayertable != view->getlockedlayertable())
    lockedlayertable = view->getlockedlayertable();

#ifdef USING_WIDGETS
#ifdef USING_TABBED_WINDOWS
  if(hWnd && ((QWidget*)hWnd)->objectName() == "QGraphicsView" )
  {
      //qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
      qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((QGraphicsView*)hWnd)->scene();
      qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
      setScene(((QGraphicsView*)hWnd)->scene());
      qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
      //qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
      if(((QWidget*)window->gethwnd())->objectName() == "QTabWidget")
          ((QTabWidget*)window->gethwnd())->setTabText(((QTabWidget*)window->gethwnd())->indexOf((QWidget*)hWnd),QString(view->getname()));
#endif
#ifdef USING_MDIAREA
   if(hWnd && ((QWidget*)hWnd)->objectName() == "QGraphicsView" )
   {
      qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
      qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((QGraphicsView*)hWnd)->scene();
      qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
      setScene(((QGraphicsView*)hWnd)->scene());
      qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
      qDebug() << "window widget parent object : " << ((QWidget*)window->gethwnd());
      qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
      qDebug() << "window widget parent parent object : " << ((QMdiSubWindow*)window->gethwnd())->mdiArea();
      qDebug() << "window widget parent parent name   : " << ((QMdiSubWindow*)window->gethwnd())->mdiArea()->objectName();
      //qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->centralWidget())->activeSubWindow();
      qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->MdiArea)->activeSubWindow();

      if(((QWidget*)window->gethwnd())->objectName() == "QMdiSubWindow")
          ((QMdiSubWindow*)window->gethwnd())->setWindowTitle(QString(view->getname()));
#endif
    if(hWnd && ((QWidget*)hWnd)->objectName() == "RCView" )
    {
         qDebug() << "window widget name : " << ((QWidget*)hWnd)->objectName();
         qDebug() << "Graphics View Scene  : " << (RCGraphicsScene*)((RCView*)hWnd)->view()->scene();
         qDebug() << "This scene before    : " << (RCGraphicsScene*)this->getScene();
         setScene(((RCView*)hWnd)->view()->scene());
         qDebug() << "This scene after     : " << (RCGraphicsScene*)this->getScene();
         qDebug() << "window widget parent object : " << ((QWidget*)window->gethwnd());
         qDebug() << "window widget parent name : " << ((QWidget*)window->gethwnd())->objectName();
         qDebug() << "window widget parent parent object : " << ((RCCentralWidget*)window->gethwnd());
         qDebug() << "window widget parent parent name   : " << ((RCCentralWidget*)window->gethwnd())->objectName();
         //qDebug() << "current window : " << ((QMdiArea*)((QtMainWindow*)p->gethwnd())->centralWidget())->activeSubWindow();
         qDebug() << "current window : " << ((RCCentralWidget*)((QtMainWindow*)p->gethwnd())->centerWidget)->activeWindow();

         //if(((QWidget*)window->gethwnd())->objectName() == "centerWidget")
         ((RCView*)hWnd)->setWindowTitle(QString(view->getname()),"","");
  }
  else
      setScene(0);
  UpdateWindow( );
#endif
  //UpdateWindow(gethwnd());
}

View3dWindow::~View3dWindow()
{
  qDebug() << "int View3dWindow DTOR: " << mcount;
  if (cadwindow != 0)
    parent->deletesubwindow(this);
  delete cursor;
  wm.del(this);
  setScene(0);
  hWnd=0;
  hDC=0;
  //qDebug() << "leaving ~View3dWindow";
}


void View3dWindow::clearScene()
{
    // clear any rubberbands
    //if(qgirb)
    //    if(getScene())
    //        getScene()->removeItem(qgirb);
    //delete qgirb;
    //qgirb=0;

    // clear any markers
    if(qgimk)
        if(getScene())
            getScene()->removeItem(qgimk);
    delete qgimk;
    qgimk=0;

    /*
     // magic eraser
    if(qgi)
        if(getScene())
            getScene()->removeItem(qgi);
    delete qgi;
    qgi=0;
    */

    if(cursor && getScene())
    {
        if(cursor->qgi)
            getScene()->removeItem(cursor->qgi);
        delete cursor->qgi;
        cursor->qgi = 0;
    }
    //View3dSurface::clearScene();
}

void View3dWindow::paintevent(void)
{
#ifdef USING_WIDGETS
    //qDebug() << "Entering : View3dWindow::paintEvent ";
    //return;
#if 1
    // using a QGraphicsView
    //RCCHAR label[300];
    //ResourceString rs5(NCWIN+5);
    RCCHAR label1[300],label2[300],label3[300];
    ResourceString rs5(4123),rs6(4124),rs7(4125);
    RECT rect;
    // KMJ: TODO:
    // this should be from this 3dviews surface object
    QGraphicsScene *theScene = getScene();

    //qDebug() << "the RCView view is : " << ((RCView*)gethwnd())->view();
    //qDebug() << "the RCView scene is : " << ((RCView*)gethwnd())->view()->scene();
    //qDebug() << "the View3dWindow scene is : " << theScene;

    // the plot scale stuff for the window title
    //sprintf(label,rs5.gets(),this->view->getname(),(umax - umin) / getwidthmm(),MatchPlotScale(plotscale));
    //title(label);
    double scl = (umax - umin) / getwidthmm();
    sprintf(label1,rs5.gets(),this->view->getname());
    sprintf(label2,rs6.gets(),(umax - umin) / getwidthmm());
    sprintf(label3,rs7.gets(),MatchPlotScale(plotscale));
    title(label1,label2,label3);

    //return;

    // not using this stuff but realcad needs it for now
        if (::v.getinteger("vw::enableautoplotscale") != 0 && getsurfacetype() != View2dSurf)
            plotscale = (umax - umin) / getwidthmm() * ::v.getreal("vw::autoplotscale");
        vscrollevent(1000,1000);
        hscrollevent(1000,1000);

        if (offscreenbitmap != 0 && (fabs(offscreenbitmap->getwidth() - getwidth()) > 0.5 || fabs(offscreenbitmap->getheight() - getheight()) > 0.5))
        {
            delete offscreenbitmap;
            offscreenbitmap = 0;
        }

        if (offscreenbitmap == 0)
        {
            offscreenbitmap = new View3dOffScreenBitmap(this);
            offscreenbitmap->displaylist.initialise(EXTEND_TABLE_SIZE,offscreenbitmap);
            offscreenbitmap->displaylist.setorgtransform(umin,vmin,umax,vmax);
        }

        offscreenbitmap->zoom(getumin(),getvmin(),getumax(),getvmax());
        offscreenbitmap->setplotscale(getplotscale());
        offscreenbitmap->setdisplaylayertable(getdisplaylayertable());
        offscreenbitmap->setscale(getscale());
        offscreenbitmap->setrepaintstyle(getrepaintstyle());
        // this code makes each window background colour different
        //offscreenbitmap->setbackground(getbackground());
        // this code will make each window the same background colour
#ifndef NEW_DRAGGING
        offscreenbitmap->setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                                           ::v.getreal("wn::backgroundcolour.green")*255.0+0.5,
                                           ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5));
#endif
        //qDebug() << "this View3dWindow width  is : " << getwidth();
        //qDebug() << "this View3dWindow height is : " << getheight();
        // this is what is drawing
        if(theScene != 0)
        {
            // clear any markers etc
            clearScene();

            //qDebug() << "this widget gview width is     : " << ((RCView*)gethwnd())->view()->width();
            //qDebug() << "this widget gview height is    : " << ((RCView*)gethwnd())->view()->height();
            //qDebug() << "this View3dWindow Qt widget is : " << ((QObject*)gethwnd())->objectName();
            //qDebug() << "this widget vport geometry is  : " << ((RCView*)gethwnd())->viewport()->geometry();
            //qDebug() << "this widget vport rect is      : " << ((RCView*)gethwnd())->viewport()->rect();
            //qDebug() << "this widget vport width is     : " << ((RCView*)gethwnd())->viewport()->width();
            //qDebug() << "this widget vport height is    : " << ((RCView*)gethwnd())->viewport()->height();
            //qDebug() << "the scene rect is              : " << theScene->sceneRect();

            //theScene->setSceneRect(0.0,0.0,((RCView*)gethwnd())->view()->width(),
            //                              ((RCView*)gethwnd())->view()->height());
            //qDebug() << "the scene rect is              : " << theScene->sceneRect();

            // KMJ: if you want to have a different background colour for each view uncomment this code
            //RCCOLORREF bcr = getbackground();
            //theScene->setBackgroundBrush(QBrush(QColor(GetRValue(bcr),GetGValue(bcr),GetBValue(bcr))));

            // this code makes all windows the same background colour

            theScene->setBackgroundBrush(QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
                                                       int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
                                                       int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));
            // for debugging the scene rect
            //qDebug() << "View3dWindow::paintevent sceneRect : " << theScene->sceneRect();
            //theScene->addRect(1,0.0,sw-2,sh-2,QPen(QColor(0,0,0),1)/*,QBrush(QColor(255,0,0))*/);
        }
        ((RCView*)gethwnd())->view()->setFocus();
        return;
        //scene->clear();
        //program->setbusy(1);
        setclip(0.0,0.0,((RCView*)gethwnd())->view()->width(),
                        ((RCView*)gethwnd())->view()->height());
        //setclip(0.0,0.0,getwidth(),getheight());
        //setclip(theScene->sceneRect().x(),theScene->sceneRect().y(),theScene->width(),theScene->height());
        //qDebug() << "getrepaintstyle() : " << getrepaintstyle();

        if (getrepaintstyle() == Wireframe || getrepaintstyle() == RayTraced)
        {
            // paint the background here for QT???
            rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
            //hbrush = CreateSolidBrush(getbackground());
            //FillRect(offscreenbitmap->gethdc(),&rect,hbrush);
            //DeleteObject(hbrush);
            Window::paintevent();

            //
            if (SetLight != 0 && requiresopengl())
            {
#ifdef USING_WIDGETS

                // open GL stuff here
                View *view;

                OpenOpenGl(((RCView*)offscreenbitmap->gethdc())->view(),3,0,0,(int)getwidth(),(int)getheight());
                view = getview();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(getumin(),getvmin(),getumax(),getvmax());

                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
                //BeginScene(/*BS_FRONTBUFFERONLY*/BS_NOCLEAR);
                //BeginScene(/*BS_FRONTBUFFERONLY |*/ BS_IGNOREDEPTH);
                //SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

                // using Gl for normal drawing so this is not used now
                // using it makes things be drawn with GL and scene
                //db.draw(DM_NORMAL,offscreenbitmap);

                //EndScene();
#else
                View *view;
                view = getview();

                SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
                SetViewport(getumin(),getvmin(),getumax(),getvmax());

                SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
                db.draw(DM_NORMAL,offscreenbitmap);
#endif
            }
            // using Gl for normal drawing so this is not used now
            // using it makes things be drawn with GL and scene
            //else // 2D drawing here pass the pointer to the scene object here
                //db.draw(DM_NORMAL,offscreenbitmap);
        }
        else if (getrepaintstyle() == Hidden || getrepaintstyle() == HiddenDashed)
        {
#if 1
            //HideImage image(offscreenbitmap,1+v.getinteger("hd::dashhiddenlines") * 2);
            //image.hide();
#else
            HideImage image(offscreenbitmap,1+v.getinteger("hd::dashhiddenlines") * 2);
            image.hideGL();
#endif
        }
        else
        {
#ifdef USING_WIDGETS
#if 0
            OpenOpenGl(gethdc(),3,0,0,width,height);
            BeginScene(/*BS_FRONTBUFFERONLY*/BS_NOCLEAR);
            //BeginScene(/*BS_FRONTBUFFERONLY |*/ BS_IGNOREDEPTH);
            //SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
            db.draw(DM_NORMAL,offscreenbitmap);
            //EndScene();
#else
#if 0
            double ambient,diffuse;
            int hiddenlines;
            ambient = v.getreal("sh::ambient");
            diffuse = v.getreal("sh::diffuse");
            hiddenlines = v.getinteger("sh::hiddenlines");
            offscreenbitmap->setrepaintstyle(getrepaintstyle());
            RenderImage image(offscreenbitmap,1,ambient,diffuse);
            image.render();
            if (hiddenlines)
            {
                HideImage image(offscreenbitmap,v.getinteger("hd::dashhiddenlines") * 2);
                //image.hide();
                image.hideGL();
            }
#endif
#endif
#else
            double ambient,diffuse;
            int hiddenlines;
            ambient = v.getreal("sh::ambient");
            diffuse = v.getreal("sh::diffuse");
            hiddenlines = v.getinteger("sh::hiddenlines");
            RenderImage image(offscreenbitmap,1,ambient,diffuse);
            image.render();
            if (hiddenlines)
            {HideImage image(offscreenbitmap,v.getinteger("hd::dashhiddenlines") * 2);
               image.hide();
            }
#endif
        }
        //program->setbusy(0);
    //}
#endif
    //qDebug() << "Leaving : View3dWindow::paintEvent ";
#else
 RECT rect;
 RCHBRUSH hbrush;
 RCCHAR label[300];
 ResourceString rs5(NCWIN+5);
//qDebug() << "Entering : View3dWindow::paintEvent ";

    sprintf(label,rs5.gets(),this->view->getname(),(umax - umin) / getwidthmm(),MatchPlotScale(plotscale));
    title(label);

    if (showcommand)
    {   InvalidateRect(gethwnd(),0,TRUE);
        ShowWindow(gethwnd(),showcommand);
        showcommand = 0;
        return;
    }

    if (IsIconic(gethwnd()) || getwidth() < 4 || getheight() < 4)
    {   rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
        hbrush = CreateSolidBrush(getbackground());
        FillRect(gethdc(),&rect,hbrush);
        DeleteObject(hbrush);
        return;
    }

    if (::v.getinteger("vw::enableautoplotscale") != 0 && getsurfacetype() != View2dSurf)
        plotscale = (umax - umin) / getwidthmm() * ::v.getreal("vw::autoplotscale");
    vscrollevent(1000,1000);
    hscrollevent(1000,1000);

    if (offscreenbitmap != 0 && (fabs(offscreenbitmap->getwidth() - getwidth()) > 0.5 || fabs(offscreenbitmap->getheight() - getheight()) > 0.5))
    {   delete offscreenbitmap;
        offscreenbitmap = 0;
    }

    if (offscreenbitmap == 0)
    {   offscreenbitmap = new View3dOffScreenBitmap(this);
        offscreenbitmap->displaylist.initialise(EXTEND_TABLE_SIZE,offscreenbitmap);
        offscreenbitmap->displaylist.setorgtransform(umin,vmin,umax,vmax);
    }

    offscreenbitmap->zoom(getumin(),getvmin(),getumax(),getvmax());
    offscreenbitmap->setplotscale(getplotscale());
    offscreenbitmap->setdisplaylayertable(getdisplaylayertable());
    offscreenbitmap->setscale(getscale());
    offscreenbitmap->setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                    ::v.getreal("wn::backgroundcolour.green")*255.0,
                                    ::v.getreal("wn::backgroundcolour.blue")*255.0));
    program->setbusy(1);
    setclip(0.0,0.0,getwidth(),getheight());
    if (getrepaintstyle() == Wireframe ||getrepaintstyle() == RayTraced)
    {   rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
        hbrush = CreateSolidBrush(getbackground());
        FillRect(offscreenbitmap->gethdc(),&rect,hbrush);
        DeleteObject(hbrush);
        Window::paintevent();

        if (SetLight != 0 && requiresopengl())
        {View *view;

            OpenOpenGl(offscreenbitmap->gethdc(),3,0,0,(int)getwidth(),(int)getheight());
            view = getview();

            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(getumin(),getvmin(),getumax(),getvmax());

            SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

            BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
            SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
            db.draw(DM_NORMAL,offscreenbitmap);
            EndScene();

        }
        else
            db.draw(DM_NORMAL,offscreenbitmap);
    }
    else if (getrepaintstyle() == Hidden)
    {HideImage image(offscreenbitmap,1+v.getinteger("hd::dashhiddenlines") * 2);
        image.hide();
    }
    else
    {double ambient,diffuse;
     int hiddenlines;
        ambient = v.getreal("sh::ambient");
        diffuse = v.getreal("sh::diffuse");
        hiddenlines = v.getinteger("sh::hiddenlines");
        RenderImage image(offscreenbitmap,1,ambient,diffuse);
        image.render();
        if (hiddenlines)
        {HideImage image(offscreenbitmap,v.getinteger("hd::dashhiddenlines") * 2);
           image.hide();
        }
    }
    program->setbusy(0);

    BitBlt(gethdc(),0,0,int(getwidth()),int(getheight()),offscreenbitmap->gethdc(),0,0,SRCCOPY);
#endif
}

void View3dWindow::UpdateWindow(void)
{
#ifdef USING_WIDGETS

    //regenGLlists();
    if(cadwindow &&
       cadwindow->getDrawingBatch() == 0 &&
       ((RCView*)gethwnd())->isVisible() &&
                 getRaytracing() == 0)
    {
        ((RCView*)gethdc())->view()->scene()->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                                             ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                                             ::v.getreal("wn::backgroundcolour.blue")*255.0)));
    }
    //app->processEvents();
#else

    if (offscreenbitmap != 0)
        BitBlt(gethdc(),0,0,int(getwidth()),int(getheight()),offscreenbitmap->gethdc(),0,0,SRCCOPY);
#endif
}

void View3dWindow::sizeevent(int w,int h)
{
 double scale,u,v,du,dv;
 RECT rect;

    if (IsIconic(gethwnd()) || w < 2 || w < 2)
        return;

    if(getRaytracing() == 1)
    {
        // turn off raytracing
        setrepaintstyle(Wireframe);
        setRaytracing(0);
        program->setinterrupt(1);
        // remove the backimage
        if((RCView*)gethwnd() != 0)
            ((RCView*)gethwnd())->unFreezeBackImage();
        return;
    }

    // remove the backimage
    if((RCView*)gethwnd() != 0)
        ((RCView*)gethwnd())->unFreezeBackImage();


    scale = (umax - umin) / getwidthmm();
    if (::v.getinteger("vw::enableautoplotscale") != 0 && getsurfacetype() != View2dSurf)
        plotscale = scale * ::v.getreal("vw::autoplotscale");
    u = (umin + umax) / 2.0;
    v = (vmin + vmax) / 2.0;
    Surface::sizeevent(w,h);
    cursor->sizeevent(w,h);
    du = getwidthmm() * scale / 2.0;
    dv = getheightmm() * scale / 2.0 / ::v.getreal("vw::aspectadjustment");
    zoom(u - du,v - dv,u + du,v + dv);

    //qDebug() << "View3dWindow::sizeevent getwidth() : " << getwidth();
    //qDebug() << "View3dWindow::sizeevent getheight() : " << getheight();

    rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
    InvalidateRect(gethwnd(),&rect,TRUE);

    //CCDEBUG
    //WINDOWPLACEMENT wp;
    //wp.length = sizeof(WINDOWPLACEMENT);
    //GetWindowPlacement(gethwnd(),&wp);
}

void View3dWindow::getminmaxinfoevent(MINMAXINFO *minmaxinfo)
{RECT rect;
return;
  GetClientRect(cadwindow->subwindow->gethwnd(),&rect);
  minmaxinfo->ptMaxSize.x = rect.right + GetSystemMetrics(SM_CXFRAME) * 2;
  minmaxinfo->ptMaxSize.y = rect.bottom + GetSystemMetrics(SM_CYFRAME) * 2;
  minmaxinfo->ptMaxPosition.x = 0;
  minmaxinfo->ptMaxPosition.y = 0;
  minmaxinfo->ptMinTrackSize.y = GetSystemMetrics(SM_CYMINIMIZED) + 6;
}

//
//  View2dWindows
//

View2dWindow::View2dWindow(MainWindow *p,Window *win,View2d *v,View *t,int w,int h,DWORD wintype,RCPVOID param) :
  View3dWindow(p,win,t,0,0,w,h, wintype, param )
{
#ifdef USING_WIDGETS
    qgi=0;
    qgi2=0;
#endif

  OneView *ov;
  View3dSubSurface *ss;
  plotscale = 1.0;
  top = t;
  view2d = v;
  umin = -v->getpaperwidth() * 0.05;
  vmin = -v->getpaperheight() * 0.05;
  umax =  v->getpaperwidth() * 1.05;
  vmax =  v->getpaperheight() * 1.05;
  SetBkColor(((RCView*)hDC)->view(),getbackground());
  fixzoom();
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
  getvisibility();
  for (v->start() ;  (ov = v->next()) != NULL ; )
    if ((ss = new View3dSubSurface(this,ov)) != NULL)
      subsurfaces.add(ss);
}

View2dWindow::View2dWindow(MainWindow *p,Window *win,View2d *v,View *t,int x,int y,int w,int h,double umin1,double vmin1,double umax1,double vmax1,DWORD wintype,RCPVOID param) :
  View3dWindow(p,win,t,x,y,w,h,umin1,vmin1,umax1,vmax1,wintype,param)
{
#ifdef USING_WIDGETS
    qgi=0;
    qgi2=0;
#endif

    OneView *ov;
    View3dSubSurface *ss;

    plotscale = 1.0;
    top = t;
    view2d = v;
    fixzoom();
    Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
    SetBkColor(((RCView*)hDC)->view(),getbackground());
    updatematrix();
    getvisibility();
    for (v->start() ;  (ov = v->next()) != NULL ; )
        if ((ss = new View3dSubSurface(this,ov)) != NULL)
            subsurfaces.add(ss);

}




BitMask &View2dWindow::getvisibility(void)
{   visible.clearandset(view->getviewno(),end_list);
    return visible;
}

int View2dWindow::requiresopengl()
{View3dSubSurface *ss;

  if (View3dWindow::requiresopengl())
    return 1;

  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    if (ss->requiresopengl())
      return 1;

  return 0;

}

void View2dWindow::drawboundary(double umin,double vmin,double umax,double vmax,int drawmode)
{
  Point p1,p2,o,xaxis,yaxis;

  //qDebug() << "entering : drawboundary";

  if(qgi2)
      getScene()->removeItem(qgi2);
  delete qgi2;
  qgi2=0;

  if(drawmode == DM_ERASE)
      return;

  qgi2 = new QGraphicsItemGroup();

  o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
  xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
  yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
  p1 = o + xaxis * umin / oneview->getscale() + yaxis * vmin / oneview->getscale();
  p2 = o + xaxis * umax / oneview->getscale() + yaxis * vmin / oneview->getscale();


  EntityHeader header(0,0,0,0);
  Line line1(header,p1,p2);
  //line1.draw(DM_INVERT,NULL,this);
  line1.draw(DM_SELECT,NULL,this);
  if(line1.qgi)
      ((QGraphicsItemGroup*)qgi2)->addToGroup(line1.qgi);
  line1.qgi=0;

  p1 = o + xaxis * umax / oneview->getscale() + yaxis * vmin / oneview->getscale();
  p2 = o + xaxis * umax / oneview->getscale() + yaxis * vmax / oneview->getscale();

  Line line2(header,p1,p2);
  //line2.draw(DM_INVERT,NULL,this);
  line2.draw(DM_SELECT,NULL,this);
  if(line2.qgi)
      ((QGraphicsItemGroup*)qgi2)->addToGroup(line2.qgi);
  line2.qgi=0;

  p1 = o + xaxis * umax / oneview->getscale() + yaxis * vmax / oneview->getscale();
  p2 = o + xaxis * umin / oneview->getscale() + yaxis * vmax / oneview->getscale();

  Line line3(header,p1,p2);
  line2.qgi = qgi2;
  //line3.draw(DM_INVERT,NULL,this);
  line3.draw(DM_SELECT,NULL,this);
  if(line3.qgi)
      ((QGraphicsItemGroup*)qgi2)->addToGroup(line3.qgi);
  line3.qgi=0;

  p1 = o + xaxis * umin / oneview->getscale() + yaxis * vmax / oneview->getscale();
  p2 = o + xaxis * umin / oneview->getscale() + yaxis * vmin / oneview->getscale();

  Line line4(header,p1,p2);
  //line4.draw(DM_INVERT,NULL,this);
  line4.draw(DM_SELECT,NULL,this);
  if(line4.qgi)
      ((QGraphicsItemGroup*)qgi2)->addToGroup(line4.qgi);
  line4.qgi=0;

  if(qgi2)
      getScene()->addItem(qgi2);

  drawn = 1;
  lastumin = umin;  lastvmin = vmin;  lastumax = umax;  lastvmax = vmax;

}

void View2dWindow::vscrollevent(int code,int pos)
{int start,end;
 double v,v1,v2,dv,svmin,svmax;
  v1 = 0.0;  v2 = view2d->getpaperheight();
  dv = v2 - v1;
  if (dv < vmax - vmin)
    dv = vmax - vmin;
  svmin = (v1 + v2) / 2.0 - dv * 1.5;
  svmax = (v1 + v2) / 2.0 + dv * 1.5 - (vmax - vmin);
  if (svmin > vmin) svmin = vmin;
  if (svmax < vmin) svmax = vmin;
  v = vmin;

  GetScrollRange(gethwnd(),SB_VERT,&start,&end);
  if (start != end)
    {  switch (code)
         {case SB_BOTTOM : v = svmin;  break;
          case SB_LINEDOWN : v = v - (vmax - vmin) / 50.0;  break;
          case SB_LINEUP : v = v + (vmax - vmin) / 50.0;  break;
          case SB_PAGEDOWN : v = v - (vmax - vmin) / 5.0;  break;
          case SB_PAGEUP : v = v + (vmax - vmin) / 5.0;  break;
          case SB_THUMBTRACK	:
          case SB_THUMBPOSITION	:  v = double(pos - end) / double(start - end) * (svmax - svmin) + svmin;  break;
          case SB_TOP : v = svmax;  break;
         }
       if (v < svmin) v = svmin;
       if (v > svmax) v = svmax;
       SetScrollPos(gethwnd(),SB_VERT,int(end + double(start - end) * (v - svmin) / (svmax - svmin)),TRUE);
       if (code < 1000)
         zoom(umin,v,umax,v + (vmax - vmin));
    }
}

void View2dWindow::hscrollevent(int code,int pos)
{int start,end;
 double u,u1,u2,du,sumin,sumax;
  u1 = 0.0;  u2 = view2d->getpaperwidth();
  du = u2 - u1;
  if (du < umax - umin)
    du = umax - umin;
  sumin = (u1 + u2) / 2.0 - du * 1.5;
  sumax = (u1 + u2) / 2.0 + du * 1.5 - (umax - umin);
  if (sumin > umin) sumin = umin;
  if (sumax < umin) sumax = umin;
  u = umin;

  GetScrollRange(gethwnd(),SB_HORZ,&start,&end);
  if (start != end)
    {  switch (code)
         {case SB_BOTTOM : u = sumin;  break;
          case SB_LINEDOWN : u = u + (umax - umin) / 50.0;  break;
          case SB_LINEUP : u = u - (umax - umin) / 50.0;  break;
          case SB_PAGEDOWN : u = u + (umax - umin) / 5.0;  break;
          case SB_PAGEUP : u = u - (umax - umin) / 5.0;  break;
          case SB_THUMBTRACK	:
          case SB_THUMBPOSITION	:  u = double(pos - start) / double(end - start) * (sumax - sumin) + sumin;  break;
          case SB_TOP : u = sumax;  break;
         }
       if (u < sumin) u = sumin;
       if (u > sumax) u = sumax;
       SetScrollPos(gethwnd(),SB_HORZ,int(start + double(end - start) * (u - sumin) / (sumax - sumin)),TRUE);
       if (code < 1000)
         zoom(u,vmin,u + (umax - umin),vmax);
    }
}

void View2dWindow::mousemoveevent(int x,int y,int buttons)
{Point p,p1,p2;
 double x1,y1,cosa,sina;
 int snap;
 View3dSubSurface *ss;
 Entity *e,*secondentity;
 BitMask entity1(32),entity2(32);

  //if (x != -32767)
  //  SetCursor(program->gethcursor());
  // if the style entry has the focus - remove the focus
#ifdef USING_WIDGETS
  if (cadwindow->styleentry->containsWidget((QWidget*)GetFocus()))
    SetFocus(cadwindow->subwindow->gethwnd());
#else
 if (GetFocus() == cadwindow->styleentry->gettypehwnd())
   SetFocus(cadwindow->subwindow->gethwnd());
#endif

  if (buttondown >= 101 && buttondown <= 108 && coordinate3d(x,y,&p))
    {
      scrolling = 3;
      if (drawn)
         drawboundary(lastumin,lastvmin,lastumax,lastvmax);

       cosa = cos(oneview->getangle() * acos(-1.0) / 180.0);
       sina = sin(oneview->getangle() * acos(-1.0) / 180.0);
       x1 = p.x - oneview->getxpos();
       y1 = p.y - oneview->getypos();
       p.x = ( cosa * x1 + sina * y1) * oneview->getscale();
       p.y = (-sina * x1 + cosa * y1) * oneview->getscale();

       switch (buttondown-100)
         {  case 1 : case 2 : case 3 :
              lastvmin = p.y;
              break;
            case 5 : case 6 : case 7 :
              lastvmax = p.y;
              break;
         }
       switch (buttondown-100)
         {  case 3 : case 4 : case 5 :
              lastumax = p.x;
              break;
            case 7 : case 8 : case 1 :
              lastumin = p.x;
              break;
         }

       drawboundary(lastumin,lastvmin,lastumax,lastvmax);
       cursor->mousemoveevent(x,y,buttons);
       return;
    }
  View3dWindow::mousemoveevent(x,y,buttons);

  //  Search for a snap point inside a subwindow as well

  snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask) || state.dragging();
  if (db.getdynamiccursor() && (state.getcommandid() == 0 || (buttons & MK_CONTROL) != MK_CONTROL))
    {  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
         //if ((picked = ss->pick(x,int(getheight())-1-y)) != 0)
         {double disc,a1,a2,b1,b2,c1,c2,cosa,sina;

            cosa = cos(ss->getoneview()->getangle()/180.0*acos(-1.0));
            sina = sin(ss->getoneview()->getangle()/180.0*acos(-1.0));

            a1 = cosa;
            b1 = -sina;
            c1 = ((ss->getoneview()->getumin()*cosa-ss->getoneview()->getvmin()*sina) / ss->getoneview()->getscale() + ss->getoneview()->getxpos() - getumin()) / (getumax() - getumin()) * getwidth(),
            a2 = -sina;
            b2 = -cosa;
            c2 = getheight() - 1.0 - ((ss->getoneview()->getumin()*sina+ss->getoneview()->getvmin()*cosa) / ss->getoneview()->getscale() + ss->getoneview()->getypos() - getvmin())  / (getvmax() - getvmin()) * getheight();

            disc = a1 * b2 - b1 * a2;
            if (fabs(disc) < 1.0e-10) disc = 1.0;
            p.x = ( b2 * (x - c1) - b1 * (y - c2)) / disc;
            p.y = (-a2 * (x - c1) + a1 * (y - c2)) / disc;

            //  Only interested in snap points
            entity1 = state.selmask.entity;
            entity2 = state.tempentitymask;
            state.tempentitymask.clearandset(linear_mask,point_entity,wall_entity,beam_entity,end_list);
            state.selmask.entity.clearandset(linear_mask,point_entity,wall_entity,beam_entity,end_list);

            if ((e = db.geometry.pick(ss,1,snap,int(p.x),int(ss->getheight() - 1 - p.y),&secondentity,(Point *)0)) != 0)
              {int snapped;
                 snapped = 0;
                 if (snap && secondentity != 0 && db.snapmask.test(SnapMaskIntersection))
                   {Intersect in(e,secondentity);
                    int i;
                      for (i = 0 ; i < in.nintersections() ; i++)
                        if (in.intersection(i)->isa(point_entity))
                          {  p = ((PointE *)in.intersection(i))->getp();
                             if (View3dSurface::pick(p,100,downx,downy) >= 0.0)
                               snapped = 1;
                          }
                   }
                 if (snapped)
                    cursor->setshape(5);
                 else if (snap && e->islinear() && ((LinearEntity *)e)->pickendpoint(ss,0,int(p.x),int(p.y),&p1))
                   cursor->setshape(1);
                 else if (snap && e->isa(wall_entity) && ((Wall *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1))
                   cursor->setshape(1);
                 else if (snap && e->isa(beam_entity) && ((Beam *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1))
                   cursor->setshape(1);
                 else if (snap && e->isa(point_entity))
                   cursor->setshape(1);
                 else if (e->isselectable(this))
                   cursor->setshape(2);
              }

            state.selmask.entity = entity1;
            state.tempentitymask = entity2;

         }
       cursor->mousemoveevent(x,y,buttons);
   }



}

void View2dWindow::lbuttondownevent(int x,int y,int buttons)
{View3dSubSurface *ss;
 int picked,snap;
 Point p,p1;
 Entity *e;

  if (cadwindow->getdragviewboundary())
    for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
      if ((picked = ss->pick(x,int(getheight())-1-y)) != 0)
        {  downx = x;  downy = y;
           oneview = ss->getoneview();
           lastumin = oneview->getumin();
           lastumax = oneview->getumax();
           lastvmin = oneview->getvmin();
           lastvmax = oneview->getvmax();
           buttondown = 100 + picked;
           drawn = 0;
           ss->regenerate();
           return;
        }

  //  Search for a snap point inside a subwindow as well
  snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask) || state.dragging();
  if (db.getdynamiccursor() && (state.getcommandid() == 0 || (buttons & MK_CONTROL) != MK_CONTROL))
    {  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
         //if ((picked = ss->pick(x,int(getheight())-1-y)) != 0)
         {double disc,a1,a2,b1,b2,c1,c2,cosa,sina;

            cosa = cos(ss->getoneview()->getangle()/180.0*acos(-1.0));
            sina = sin(ss->getoneview()->getangle()/180.0*acos(-1.0));

            a1 = cosa;
            b1 = -sina;
            c1 = ((ss->getoneview()->getumin()*cosa-ss->getoneview()->getvmin()*sina) / ss->getoneview()->getscale() + ss->getoneview()->getxpos() - getumin()) / (getumax() - getumin()) * getwidth(),
            a2 = -sina;
            b2 = -cosa;
            c2 = getheight() - 1.0 - ((ss->getoneview()->getumin()*sina+ss->getoneview()->getvmin()*cosa) / ss->getoneview()->getscale() + ss->getoneview()->getypos() - getvmin())  / (getvmax() - getvmin()) * getheight();

            disc = a1 * b2 - b1 * a2;
            if (fabs(disc) < 1.0e-10) disc = 1.0;
            p.x = ( b2 * (x - c1) - b1 * (y - c2)) / disc;
            p.y = (-a2 * (x - c1) + a1 * (y - c2)) / disc;

            if ((e = db.geometry.pick(ss,0,snap,int(p.x),int(ss->getheight() - 1 - p.y),0,(Point *)0)) != 0)
              {  if (snap && (e->islinear() && ((LinearEntity *)e)->pickendpoint(ss,0,int(p.x),int(p.y),&p1) ||
                              e->isa(wall_entity) && ((Wall *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1) ||
                              e->isa(beam_entity) && ((Beam *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1) ||
                              e->isa(point_entity)))
                   return;
              }
         }
    }

  View3dWindow::lbuttondownevent(x,y,buttons);
}

void View2dWindow::lbuttonupevent(int x,int y,int buttons)
{View3dSubSurface *ss;
 double x1,y1,cosa,sina;
 Point p,p1,o,xaxis,yaxis;
 int snap;
 Entity *e;
 Coordinate3dEvent *cevent;

  if ((abs(x-downx) > 0 || abs(y-downy) > 0) &&
      buttondown >= 101 && buttondown <= 108 && coordinate3d(x,y,&p))
    {
       if (drawn)
         drawboundary(lastumin,lastvmin,lastumax,lastvmax, 2);
       cosa = cos(oneview->getangle() * acos(-1.0) / 180.0);
       sina = sin(oneview->getangle() * acos(-1.0) / 180.0);
       x1 = p.x - oneview->getxpos();
       y1 = p.y - oneview->getypos();
       p.x = ( cosa * x1 + sina * y1) * oneview->getscale();
       p.y = (-sina * x1 + cosa * y1) * oneview->getscale();
       switch (buttondown-100)
         {  case 1 : case 2 : case 3 :
              lastvmin = p.y;
              break;
            case 5 : case 6 : case 7 :
              lastvmax = p.y;
              break;
         }
       switch (buttondown-100)
         {  case 3 : case 4 : case 5 :
              lastumax = p.x;
              break;
            case 7 : case 8 : case 1 :
              lastumin = p.x;
              break;
         }
       if (fabs((double)(downx - x)) > 0 && fabs((double)(downy - y)) > 0)
         {  db.saveundo(UD_CHANGEVIEW2D,view2d);
            oneview->setclip(lastumin,lastvmin,lastumax,lastvmax);
            zoom(umin,vmin,umax,vmax);
         }
       buttondown = 0;
       return;
    }
  if (buttondown >= 101 && buttondown <= 108)
    {
      if (drawn)
         drawboundary(lastumin,lastvmin,lastumax,lastvmax);
       buttondown = 0;
    }
  else
    {
       cevent = 0;

       //  Search for a snap point inside a subwindow as well
       snap = state.tempentitymask.test(xyz_mask) && state.selmask.entity.test(xyz_mask) || state.dragging();
       if (db.getdynamiccursor() && (state.getcommandid() == 0 || (buttons & MK_CONTROL) != MK_CONTROL))
         {  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
              {double disc,a1,a2,b1,b2,c1,c2,cosa,sina;

                 cosa = cos(ss->getoneview()->getangle()/180.0*acos(-1.0));
                 sina = sin(ss->getoneview()->getangle()/180.0*acos(-1.0));

                 a1 = cosa;
                 b1 = -sina;
                 c1 = ((ss->getoneview()->getumin()*cosa-ss->getoneview()->getvmin()*sina) / ss->getoneview()->getscale() + ss->getoneview()->getxpos() - getumin()) / (getumax() - getumin()) * getwidth(),
                 a2 = -sina;
                 b2 = -cosa;
                 c2 = getheight() - 1.0 - ((ss->getoneview()->getumin()*sina+ss->getoneview()->getvmin()*cosa) / ss->getoneview()->getscale() + ss->getoneview()->getypos() - getvmin())  / (getvmax() - getvmin()) * getheight();

                 disc = a1 * b2 - b1 * a2;
                 if (fabs(disc) < 1.0e-10) disc = 1.0;
                 p.x = ( b2 * (x - c1) - b1 * (y - c2)) / disc;
                 p.y = (-a2 * (x - c1) + a1 * (y - c2)) / disc;

                 if ((e = db.geometry.pick(ss,0,snap,int(p.x),int(ss->getheight() - 1 - p.y),0,(Point *)0)) != 0)
                   {  if (snap && (e->islinear() && ((LinearEntity *)e)->pickendpoint(ss,0,int(p.x),int(p.y),&p1) ||
                                   e->isa(wall_entity) && ((Wall *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1) ||
                                   e->isa(beam_entity) && ((Beam *)e)->pickpoint(ss,0,int(p.x),int(p.y),&p1) ||
                                   e->isa(point_entity)))

                        {

                           p1 = ss->modeltouv(p1);


                           o.setxyz(ss->getoneview()->getxpos(),ss->getoneview()->getypos(),0.0);
                           xaxis.setxyz(cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);
                           yaxis.setxyz(-sin(ss->getoneview()->getangle()/180.0*acos(-1.0)),cos(ss->getoneview()->getangle()/180.0*acos(-1.0)),0.0);

                           p1 = o + xaxis * p1.x / ss->getoneview()->getscale() + yaxis * p1.y / ss->getoneview()->getscale();

                           cevent = new Coordinate3dEvent(p1,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),this,0);
                           state.sendevent(cevent);
                        }
                   }
             }
         }
       if (cevent == 0)
         View3dWindow::lbuttonupevent(x,y,buttons);
    }
}

void View2dWindow::lbuttondblclkevent(int x,int y,int buttons)
{View3dSubSurface *ss;
 double d;
 int picked,status;
 Point p1,p2;
  y = int(getheight()) - 1 - y;
  picked = 0;
  p1.setxyz(0.0,0.0,0.0);
  p2.setxyz(view2d->getpaperwidth(),0.0,0.0);
  picked |= (d = View3dWindow::pick(p1,p2,x,y,NULL)) >= 0.0 && d < db.getstoler();
  p1.setxyz(view2d->getpaperwidth(),0.0,0.0);
  p2.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);
  picked |= (d = View3dWindow::pick(p1,p2,x,y,NULL)) >= 0.0 && d < db.getstoler();
  p1.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);
  p2.setxyz(0.0,view2d->getpaperheight(),0.0);
  picked |= (d = View3dWindow::pick(p1,p2,x,y,NULL)) >= 0.0 && d < db.getstoler();
  p1.setxyz(0.0,view2d->getpaperheight(),0.0);
  p2.setxyz(0.0,0.0,0.0);
  picked |= (d = View3dWindow::pick(p1,p2,x,y,NULL)) >= 0.0 && d < db.getstoler();
  if (picked)
    {  if (view2d->verify(&db))
         {  zoom(-view2d->getpaperwidth() * 0.05,
                 -view2d->getpaperheight() * 0.05,
                  view2d->getpaperwidth() * 1.05,
                  view2d->getpaperheight() * 1.05);
         }
       return;
    }
  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    if ((picked = ss->pick(x,y)) != 0)
      {
        //if ((status = ss->getoneview()->verify(&db,view2d)) == 1)
        if ((status = ss->getoneview()->change(&db,view2d)) == 1)
           {  ss->setplotscale(ss->getoneview()->getscale());
              ss->setdisplaylayertable(ss->getoneview()->getlayertable());
              zoom(umin,vmin,umax,vmax);
           }
         else if (status == 2)
           {  db.saveundo(UD_CHANGEVIEW2D,view2d);
              view2d->del(ss->getoneview());
              update();
              zoom(umin,vmin,umax,vmax);
           }
         return;
      }
  View3dWindow::lbuttondblclkevent(x,y,buttons);
}


void View2dWindow::regenerate(void)
{View3dSubSurface *ss;
  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    ss->regenerate();
}


View3dSubSurface *View2dWindow::pick(Point p)
{View3dSubSurface *ss;
  p = modeltoscreen(p);
  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    if (ss->pick(int(p.x),int(p.y)) != 0)
      return ss;
  return 0;
}

#ifdef USING_WIDGETS
void View2dWindow::clearScene()
{
    View3dSubSurface *ss;

    //int sscnt=0;
    for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    {
        ss->setdisplaylayertable(ss->getoneview()->getlayertable());
        //ss->ssidx=sscnt;
        ss->drawborder(DM_ERASE);
        db.geometry.draw(DM_ERASE,ss);
        //sscnt++;
    }

    for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
        ((View3dSubSurface *)ss)->clearScene();


    if(qgi)
       if(getScene())
            getScene()->removeItem(qgi);
    delete qgi;
    qgi = 0;

    if(qgirb)
        if(getScene())
            getScene()->removeItem(qgirb);
    delete qgirb;
    qgirb = 0;

    if(qgimk)
        if(getScene())
            getScene()->removeItem(qgimk);
    delete qgimk;
    qgimk = 0;

    //View3dSurface::clearScene();

    //if(getScene())
    //    getScene()->clear();

}
#endif

void View2dWindow::paintevent(void)
{RECT rect;
 RCHBRUSH hbrush;
 RCHPEN hpen,holdpen;
 RCCHAR label1[50],label2[50];
 Point p1,p2;
 View3dSubSurface *ss;

 QGraphicsItem *qline=0,*qdummy=0;
 QGraphicsItemGroup *qgroup=0;

    sprintf(label1,_RCT("Layout - %ls - "),view2d->getname());
    sprintf(label2,_RCT("Zoom scale %.2f"),(umax - umin) / getwidthmm());
    title(label1,label2,_RCT(""));
    //title(label);

#ifdef USING_WIDGETS
    // we dont use the offscreen bitmap for painting at the moment
    if(offscreenbitmap != 0)
    {
        delete offscreenbitmap;
        offscreenbitmap = 0;
    }
#endif

    if (showcommand)
    {   InvalidateRect(gethwnd(),0,TRUE);
        ShowWindow(gethwnd(),showcommand);
        showcommand = 0;
        return;
    }

    if (offscreenpaint == 1)
    {   offscreenpaint = 0;
        return;
    }

    if (IsIconic(gethwnd()) || getwidth() < 4 || getheight() < 4)
    {   rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
        hbrush = CreateSolidBrush(getbackground());
        FillRect(gethdc(),&rect,hbrush);
        DeleteObject(hbrush);
        return;
    }

    offscreenpaint = 0;
    //program->setbusy(1);
    vscrollevent(1000,1000);
    hscrollevent(1000,1000);

    setclip(0.0,0.0,getwidth(),getheight());
    rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());

    // KMJ : may need new stuff here to paint the backg?
    /*
    hbrush = CreateSolidBrush(getbackground());
    FillRect(gethdc(),&rect,hbrush);
    DeleteObject(hbrush);
    Window::paintevent();
    hpen  = CreatePen(PS_DOT,0, RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background)));
    holdpen = (RCHPEN)SelectObject(gethdc(),hpen);
    DeleteObject(holdpen);
    SetROP2(gethdc(), R2_COPYPEN);
    */
    clearScene();

    getScene()->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                 ::v.getreal("wn::backgroundcolour.blue")*255.0)));

#if 0 // doing this in graphics view background draw
    if(qgi)
        getScene()->removeItem(qgi);
    delete qgi;
    qgi=0;

    qgroup = new QGraphicsItemGroup();

    setup(1.0,0.0,DM_DASH,0,0,0,0);
    p1.setxyz(0.0,0.0,0.0);  p2.setxyz(view2d->getpaperwidth(),0.0,0.0);

    qline = line3d(qdummy,p1,p2);
    if(qline)
        if(qgroup) qgroup->addToGroup(qline);

    p1.setxyz(view2d->getpaperwidth(),0.0,0.0);  p2.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);

    qline = line3d(qdummy,p1,p2);
    if(qline)
        if(qgroup) qgroup->addToGroup(qline);

    p1.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);  p2.setxyz(0.0,view2d->getpaperheight(),0.0);

    qline = line3d(qdummy,p1,p2);
    if(qline)
        if(qgroup) qgroup->addToGroup(qline);

    p1.setxyz(0.0,view2d->getpaperheight(),0.0);  p2.setxyz(0.0,0.0,0.0);

    qline = line3d(qdummy,p1,p2);
    if(qline)
        if(qgroup) qgroup->addToGroup(qline);

    if(qgroup)
    {
        qgi = qgroup;
        getScene()->addItem(qgi);
    }

    setup(1.0,0.0,DM_GRID,0,0,0,0);
#endif

#if 0
    for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    {
        ss->setdisplaylayertable(ss->getoneview()->getlayertable());
        ss->paintevent();
        ss->drawborder(DM_DASH);
    }
#endif
#if 0
    colour = 256;  //  Invalidate the current drawing information
    setup(1.0,0.0,DM_DASH,0,0,0,0);
    db.geometry.draw(DM_NORMAL,this);
    //db.grid.draw(DM_NORMAL,this);
    colour = 256;  //  Invalidate the current drawing information
    //program->setbusy(0);
#endif
}

void View2dWindow::zoom(double umin1,double vmin1,double umax1,double vmax1)
{View3dSubSurface *ss;
  View3dWindow::zoom(umin1,vmin1,umax1,vmax1);
  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    ss->sizeevent(0,0);
}

void View2dWindow::drawent(int drawmode,Transform *t,Entity *e)
{
   View3dSubSurface *ss;
   if (e->getvisible()->test(view->getviewno()))
     e->draw(drawmode,t,this);
   // KMJ: work around for rubberband when putting a 3d view on a layout
   // ignore the subsurfaces when rubberbanding in this command
   if(state.getcommandid() == 526)
       return;
   for (subsurfaces.start(); (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
     if (e->getvisible()->test(ss->getview()->getviewno()))
     {
        e->draw(drawmode,t,ss);
     }
}

#ifdef USING_WIDGETS
void View2dWindow::drawentGL(int drawmode,Transform *t,Entity *e)
{
   View3dSubSurface *ss;
   if (e->getvisible()->test(view->getviewno()))
     e->drawGL(drawmode,t,this);
   // KMJ: on hold for debugging
   int sscnt=0;
   for (subsurfaces.start(); (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
     if (e->getvisible()->test(ss->getview()->getviewno()))
     {
        //ss->qsidx=sscnt;
        e->drawGL(drawmode,t,ss);
        //sscnt++;
     }
}
#endif

void View2dWindow::sizeevent(int w,int h)
{View3dSubSurface *ss;
  View3dWindow::sizeevent(w,h);
  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
    ss->sizeevent(0,0);
}

void View2dWindow::update(int clear)
{View3dSubSurface *ss;
 OneView *ov;
//  Process new OneView objects

  if (clear)
    {  for (subsurfaces.start() ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; )
         delete ss;
       subsurfaces.destroy();
    }

  for (view2d->start() ; (ov = view2d->next()) != NULL ; )
    {
      int sscnt=0;
       for (subsurfaces.start(),sscnt=0 ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ; sscnt++ )
         if (ss->getoneview() == ov) break;
       if (ss == NULL)
         {  if ((ss = new View3dSubSurface(this,ov)) != NULL)
              {
                 //ss->qsidx=sscnt;
                 subsurfaces.add(ss);
                 ss->drawborder(DM_DASH);
#ifdef NEW_DRAGGING
                 // KMJ : nothing to do for opengl
#else
                 ss->paintevent();
#endif
              }
         }
    }
//  Delete old OneView objects
  int sscnt=0;
  for (subsurfaces.start(),sscnt=0 ; (ss = (View3dSubSurface *) subsurfaces.next()) != NULL ;sscnt++ )
    {
       for (view2d->start() ; (ov = view2d->next()) != NULL ;)
         if (ss->getoneview() == ov) break;
       if (ov == NULL)
         {
            //ss->qsidx=sscnt;
            subsurfaces.del(ss);
            subsurfaces.start();
            delete ss;
         }
    }
}

View2dWindow *View2dWindow::load(CadFile *infile)
{short x,y,w,h;
 long l;
 char *cviewname;
 RCCHAR *viewname;
 double umin,umax,vmin,vmax;
 View2d *view2d;
 View *top;
 View2dWindow *win;
 int restorewindowplacement;
 WINDOWPLACEMENT wp;

  //if (! infile->read(&viewname)) return NULL;
  if( infile->getversion() > 2 )
  {
      if (! infile->read(&viewname))
          return NULL;
  }
  else
  {
      if (! infile->read(&cviewname)) return NULL;
      viewname = new RCCHAR[strlen(cviewname)+1];
      strcpy(viewname,cviewname);
      delete [] cviewname;
  }
  if (viewname[0] == '\001')
    {//  This is the WindowPlacement save format.
       delete [] viewname;
       //if (! infile->read(&viewname)) return NULL;
       if( infile->getversion() == 2 )
       {
           if (! infile->read(&cviewname)) return NULL;
           viewname = new RCCHAR[strlen(cviewname)+1];
           strcpy(viewname,cviewname);
           delete [] cviewname;
       }
       else
       {
           if (! infile->read(&viewname))
               return NULL;
       }

       restorewindowplacement = 1;
       memset(&wp,0,sizeof(wp));
       wp.flags = WPF_SETMINPOSITION;
       wp.length = sizeof(wp);
       if (! infile->read(&l)) return NULL;
       wp.showCmd = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMinPosition.x = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMinPosition.y = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMaxPosition.x = l;
       if (! infile->read(&l)) return NULL;
       wp.ptMaxPosition.y = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.left = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.top = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.right = l;
       if (! infile->read(&l)) return NULL;
       wp.rcNormalPosition.bottom = l;
       if (! infile->read(&umin)) return NULL;
       if (! infile->read(&vmin)) return NULL;
       if (! infile->read(&umax)) return NULL;
       if (! infile->read(&vmax)) return NULL;

       x = (short)wp.rcNormalPosition.left;
       y = (short)wp.rcNormalPosition.top;
       w = (short)wp.rcNormalPosition.right - x;
       h = (short)wp.rcNormalPosition.right - y;

    }
  else
    {  restorewindowplacement = 0;
       if (! infile->read(&x)) return NULL;
       if (! infile->read(&y)) return NULL;
       if (! infile->read(&w)) return NULL;
       if (! infile->read(&h)) return NULL;
       if (! infile->read(&umin)) return NULL;
       if (! infile->read(&vmin)) return NULL;
       if (! infile->read(&umax)) return NULL;
       if (! infile->read(&vmax)) return NULL;
    }
  if ((view2d = db.views2d.match(viewname)) == NULL) return NULL;
  if (cadwindow != 0)
    {  top = new View(view2d->getname(),0,view2d->getviewno(),0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-1E10,1E10);
       win = new View2dWindow(cadwindow,cadwindow->subwindow,view2d,top,x,y,w,h,umin,vmin,umax,vmax);
       if (restorewindowplacement)
         {  if (wp.showCmd == SW_SHOWMINIMIZED || wp.showCmd == SW_SHOWMAXIMIZED)
              win->setshowcommand(wp.showCmd);
            wp.showCmd = SW_HIDE;  //  Windows are now hidden at first
            SetWindowPlacement(win->gethwnd(),&wp);
         }

       return win;
    }
  else
    return (View2dWindow *)-1;
}

int View2dWindow::save(CadFile *outfile)
{short type;
 long x;
 WINDOWPLACEMENT wp;

  if (GetPrivateProfileInt("Save","NoMaskTables",0,home.getinifilename()) != 0)
    {short type,x,y,w,h;
      RECT rect1,rect2;
       GetWindowRect(gethwnd(),&rect1);
       GetWindowRect(GetParent(gethwnd()),&rect2);
       x = short(rect1.left - rect2.left);   y = short(rect1.top - rect2.top);
       w = short(rect1.right - rect1.left);  h = short(rect1.bottom - rect1.top);
       type = View2dSurf;
       if (! outfile->write(&type)) return 0;
       if (! outfile->write(view2d->getname())) return 0;
       if (! outfile->write(&x)) return 0;
       if (! outfile->write(&y)) return 0;
       if (! outfile->write(&w)) return 0;
       if (! outfile->write(&h)) return 0;
       if (! outfile->write(&umin)) return 0;
       if (! outfile->write(&vmin)) return 0;
       if (! outfile->write(&umax)) return 0;
       if (! outfile->write(&vmax)) return 0;
       return 1;
    }

  memset(&wp,0,sizeof(wp));
  wp.length = sizeof(wp);
  GetWindowPlacement(gethwnd(),&wp);
  type = View2dSurf;
  if (! outfile->write(&type)) return 0;
  if (! outfile->write(_RCT("\001"))) return 0;
  if (! outfile->write(view2d->getname())) return 0;
  x = wp.showCmd;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMinPosition.x;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMinPosition.y;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMaxPosition.x;
  if (! outfile->write(&x)) return 0;
  x = wp.ptMaxPosition.y;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.left;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.top;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.right;
  if (! outfile->write(&x)) return 0;
  x = wp.rcNormalPosition.bottom;
  if (! outfile->write(&x)) return 0;
  if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;

  return 1;
}

int View2dWindow::load(UndoFile *infile)
{ if (! infile->read(&umin)) return 0;
  if (! infile->read(&vmin)) return 0;
  if (! infile->read(&umax)) return 0;
  if (! infile->read(&vmax)) return 0;
  zoom(umin,vmin,umax,vmax);
  return 1;
}

int View2dWindow::save(UndoFile *outfile,int)
{ if (! outfile->write(&umin)) return 0;
  if (! outfile->write(&vmin)) return 0;
  if (! outfile->write(&umax)) return 0;
  if (! outfile->write(&vmax)) return 0;
  if (! outfile->write(&plotscale)) return 0;
  return 1;
}

View3dSubSurface::View3dSubSurface(View3dSurface *s,OneView *ov) :
  View3dSurface(ov->getview(),ov->getumin(),ov->getvmin(),ov->getumax(),ov->getvmax(),0,ov->getlayertable())
{
#ifdef USING_WIDGETS
    //for(int i=0; i < 4; i++)
    //    qgi[i] = 0;
    qgi=0;
    qgi2=0;
    qgicp=0;
#endif

    blackandwhite = s->getblackandwhite();
    parent = s;
    oneview = ov;
    setplotscale(ov->getscale());
    angle = ov->getangle();

    if (parent->gethwnd() == 0 || ov->getangle() != 0.0)
    {   sethdc(parent->gethwnd(),parent->gethdc());
        hWnd = 0;
    }
    else
    {   hWnd = CreateWindow(_RCT("CadSubWindow"),_RCT("PrintPreview"),WS_CHILD | WS_VISIBLE | WS_DISABLED ,0,0,2,2,parent->gethwnd(),0,program->gethinst(),0);
        sethdc(hWnd,0);
    }

    sizeevent(0,0);

    setxdotspermm(s->getxdotspermm());
    setydotspermm(s->getydotspermm());

    setprintxdotspermm(s->getprintxdotspermm());
    setprintydotspermm(s->getprintydotspermm());

    view3doffscreenbitmap = 0;

#ifdef USING_WIDGETS
    setScene(parent->getScene());
    //hWnd = parent->gethwnd();
    //
#endif
}

View3dSubSurface::~View3dSubSurface()
{
  for(int i=0; i<4; i++)
  {
      if(qgi)
          qgi->scene()->removeItem(qgi);
      delete qgi;
      qgi=0;
  }
  if(qgi2 && qgi2->scene())
      qgi2->scene()->removeItem(qgi2);
  if (hWnd != parent->gethwnd())
    DestroyWindow(hWnd);
  delete view3doffscreenbitmap;
}

void View3dSubSurface::regenerate(void)
{   delete view3doffscreenbitmap;
    view3doffscreenbitmap = 0;
}

void View3dSubSurface::paintevent(void)
{

    Point p1,p2,p3,p4,o,xaxis,yaxis;

    setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                        ::v.getreal("wn::backgroundcolour.green")*255.0,
                        ::v.getreal("wn::backgroundcolour.blue")*255.0));

    if (v.getinteger("vw::bitmapsforcomposedviews") != 0 && GetDeviceCaps(parent->gethdc(),TECHNOLOGY) == DT_RASDISPLAY)
    {//  Draw the image to the offscreen bitmap first

        if (view3doffscreenbitmap == 0)
        {   view3doffscreenbitmap = new View3dOffScreenBitmap(this,8);
            if (view3doffscreenbitmap->getdefined() == 0)
                view3doffscreenbitmap = new View3dOffScreenBitmap(this,2);
            view3doffscreenbitmap->setdisplaylayertable(displaylayertable);
            view3doffscreenbitmap->paintevent();
        }

        scale = (umax - umin) / getwidthmm();
        setup(1.0,0.0,DM_INVERT,0,0,0,0);

        o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
        xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
        yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
        p1 = parent->modeltoscreen(o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale());
        p2 = parent->modeltoscreen(o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale());
        p3 = parent->modeltoscreen(o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale());
        p4 = parent->modeltoscreen(o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale());

        /*
        BeginPath(parent->gethdc());
        parent->transform2d(&p1.x,&p1.y);
        MoveToEx(parent->gethdc(),(int)p1.x,(int)p1.y,0);
        parent->transform2d(&p2.x,&p2.y);
        LineTo(parent->gethdc(),(int)p2.x,(int)p2.y);
        parent->transform2d(&p3.x,&p3.y);
        LineTo(parent->gethdc(),(int)p3.x,(int)p3.y);
        parent->transform2d(&p4.x,&p4.y);
        LineTo(parent->gethdc(),(int)p4.x,(int)p4.y);
        EndPath(parent->gethdc());
        */

        if (view3doffscreenbitmap != 0)
        {   if (GetRValue(background) < 128 && GetGValue(background) < 128 && GetBValue(background) < 128)
                SetStretchBltMode(parent->gethdc(),WHITEONBLACK);
            else
                SetStretchBltMode(parent->gethdc(),BLACKONWHITE);
            StretchBlt(parent->gethdc(),(int)p1.x,(int)p3.y,(int)(p3.x - p1.x),-(int)(p3.y - p1.y),view3doffscreenbitmap->gethdc(),0,0,(int)view3doffscreenbitmap->getwidth(),(int)view3doffscreenbitmap->getheight(),SRCCOPY);
            //TransparentBlt(parent->gethdc(),(int)p1.x,(int)p3.y,(int)(p3.x - p1.x),-(int)(p3.y - p1.y),view3doffscreenbitmap->gethdc(),0,0,(int)view3doffscreenbitmap->getwidth(),(int)view3doffscreenbitmap->getheight(),background);
        }

    }
    else
    {
        scale = (umax - umin) / getwidthmm();
        setup(1.0,0.0,DM_INVERT,0,0,0,0);

        o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
        xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
        yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
        p1 = parent->modeltoscreen(o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale());
        p2 = parent->modeltoscreen(o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale());
        p3 = parent->modeltoscreen(o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale());
        p4 = parent->modeltoscreen(o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale());

        /*
        BeginPath(parent->gethdc());
        parent->transform2d(&p1.x,&p1.y);
        MoveToEx(parent->gethdc(),(int)p1.x,(int)p1.y,0);
        parent->transform2d(&p2.x,&p2.y);
        LineTo(parent->gethdc(),(int)p2.x,(int)p2.y);
        parent->transform2d(&p3.x,&p3.y);
        LineTo(parent->gethdc(),(int)p3.x,(int)p3.y);
        parent->transform2d(&p4.x,&p4.y);
        LineTo(parent->gethdc(),(int)p4.x,(int)p4.y);
        EndPath(parent->gethdc());
        */

        if (0 /*SetLight != 0 && requiresopengl() && oneview->getangle() == 0.0*/)
        {
            View *view;

            if (gethwnd() == parent->gethwnd())
                OpenOpenGl(gethdc(),2,int(p1.x),int(parent->getheight()-int(p1.y)),(int)getwidth(),(int)getheight());
            else
            {
                cleartransform();
                updatematrix();
                MoveWindow(hWnd,int(p1.x),int(p3.y),(int)getwidth(),(int)getheight(),FALSE);
                OpenOpenGl(gethdc(),0,0,0,(int)getwidth(),(int)getheight());
            }
            view = getview();

            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(getumin(),getvmin(),getumax(),getvmax());

            SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

            BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH | BS_NOCLEAR);//  Allow the clear for now | BS_NOCLEAR);
            SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
            db.geometry.draw(DM_NORMAL,this);
            EndScene();

        }
        else
        {  // Just draw into the parent device context
            //if (oneview->getangle() == 0.0 && hWnd != 0)
            //if (oneview->getangle() == 0.0)
            //{
                //cleartransform();
                //MoveWindow(hWnd,int(p1.x),int(p3.y),(int)getwidth(),(int)getheight(),FALSE);
            //}
            //else
                //SelectClipPath(gethdc(),RGN_COPY);

            updatematrix();
            db.geometry.draw(DM_NORMAL,this);
            //db.geometry.draw(DM_NORMAL,parent);
        }

        //ExtSelectClipRgn(parent->gethdc(),0,RGN_COPY);

    }

}

void View3dSubSurface::sizeevent(int,int)
{double x,y,x1,y1,x2,y2,a1,b1,c1,a2,b2,c2,a,b,cosa,sina;
 int i;
 Point p1,p3,o,xaxis,yaxis;

  umin = oneview->getumin();
  vmin = oneview->getvmin();
  umax = oneview->getumax();
  vmax = oneview->getvmax();
  Surface::sizeevent((oneview->getumax() - oneview->getumin()) / oneview->getscale() / (parent->getumax() - parent->getumin()) * parent->getwidth(),
                     (oneview->getvmax() - oneview->getvmin()) / oneview->getscale() / (parent->getvmax() - parent->getvmin()) * parent->getheight());

  cosa = cos(oneview->getangle()/180.0*acos(-1.0));
  sina = sin(oneview->getangle()/180.0*acos(-1.0));

  if (parent->gettransform2d())
    {  a = ((umin*cosa-vmin*sina) / oneview->getscale() + oneview->getxpos() - parent->getumin()) / (parent->getumax() - parent->getumin()) * parent->getwidth();
       b = ((umin*sina+vmin*cosa) / oneview->getscale() + oneview->getypos() - parent->getvmin()) / (parent->getvmax() - parent->getvmin()) * parent->getheight();
       parent->gettransform2d(&a1,&b1,&c1,&a2,&b2,&c2);
       settransform2d(a1*cosa+b1*sina,-a1*sina+b1*cosa,a1*a + b1*b + c1,a2*cosa+b2*sina,-a2*sina+cosa * b2,a2*a + b2*b + c2);
    }
  else
    settransform2d(cosa,-sina,((umin*cosa-vmin*sina) / oneview->getscale() + oneview->getxpos() - parent->getumin()) / (parent->getumax() - parent->getumin()) * parent->getwidth(),
                   -sina,-cosa,parent->getheight() - 1.0 - ((umin*sina+vmin*cosa) / oneview->getscale() + oneview->getypos() - parent->getvmin())  / (parent->getvmax() - parent->getvmin()) * parent->getheight());
  x1 = 0.0;  y1 = 0.0;  transform2d(&x1,&y1);
  x2 = x1;  y2 = y1;
  for (i = 1 ; i < 4 ; i++)
    {  if (i == 1)
         {  x = getwidth();  y = getheight();
         }
       else if (i == 2)
         {  x = getwidth();  y = 0.0;
         }
       else if (i == 3)
         {  x = 0.0;  y = getheight();
         }
       transform2d(&x,&y);

       if (x < x1) x1 = x;
       if (y < y1) y1 = y;
       if (x > x2) x2 = x;
       if (y > y2) y2 = y;


    }

  if (x1 < 0.0) x1 = 0.0;
  else if (x1 > parent->getwidth()) x1 = parent->getwidth();
  if (y1 < 0.0) y1 = 0.0;
  else if (y1 > parent->getheight()) y1 = parent->getheight();

  setclip(x1,y1,x2,y2);
  updatematrix();

  scale = (umax - umin) / getwidthmm();

  o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
  xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
  yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
  p1 = parent->modeltoscreen(o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale());
  p3 = parent->modeltoscreen(o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale());

  /*
  if (oneview->getangle() == 0.0&& hWnd != 0)
    {
       cleartransform();
       updatematrix();

       MoveWindow(hWnd,int(p1.x),int(parent->getheight()-int(p3.y)),(int)getwidth(),(int)getheight(),FALSE);
    }
    */
  originx = p1.x;
  originy = parent->getheight() - p1.y - getheight();// + parent->getbandoffset();

}

#ifdef USING_WIDGETS
void View3dSubSurface::clearScene()
{
    // these are the borders
    if(qgi)
        if(parent->getScene())
            parent->getScene()->removeItem(qgi);
    delete qgi;
    qgi=0;
    // this group is for the handles
    if(qgi2)
        if(parent->getScene())
            parent->getScene()->removeItem(qgi2);
    delete qgi2;
    qgi2 = 0;

    if(qgimk)
        if(parent->getScene())
            parent->getScene()->removeItem(qgimk);
    delete qgimk;
    qgimk = 0;
}
#endif

#ifdef USING_WIDGETS
void View3dSubSurface::drawborderGL(int drawmode)
{
  Point o,p1,p2,p[4],*ptr,xaxis,yaxis;
  int i,nvert;

  drawborder(DM_ERASE);

  //QPainterPath qpath;
  //qpath.setFillRule(Qt::OddEvenFill);

  //parent->setupGL(1.0,0.0,DM_SELECT,0,0,0,0);   // Force the next setup to work.
  parent->setupGL(1.0,0.0,drawmode,0,0,0,0);
  //setupGL(1.0,0.0,DM_DASH,0,0,0,0);   // Force the next setup to work.
  setupGL(1.0,0.0,drawmode,0,0,0,0);

  o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
  xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
  yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
  p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  p2 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  Line line1(p1,p2);  line1.EntityHeader::change(0,0,0,0);

  line1.drawGL(drawmode,NULL,parent);

  p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  p2 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  Line line2(p1,p2);  line2.EntityHeader::change(0,0,0,0);

  line2.drawGL(drawmode,NULL,parent);

  p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p2 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  Line line3(p1,p2);  line3.EntityHeader::change(0,0,0,0);

  line3.drawGL(drawmode,NULL,parent);

  p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p2 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  Line line4(p1,p2);  line4.EntityHeader::change(0,0,0,0);

  line4.drawGL(drawmode,NULL,parent);

  //  Draw the handles
  //parent->setup(1.0,0.0,DM_GRID,0,0,0,0);   // Force the next setup to work.
  for (i = 0 ; i < 8 ; i++)
    {  if (i == 0)
         p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 1)
         p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 2)
         p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 3)
         p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 4)
         p1 = o + xaxis * (oneview->getumin()+oneview->getumax())/2.0/oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 5)
         p1 = o + xaxis * (oneview->getumin()+oneview->getumax())/2.0/oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 6)
         p1 = o + xaxis * oneview->getumin()/oneview->getscale() + yaxis * (oneview->getvmin() + oneview->getvmax())/2.0 / oneview->getscale();
       else if (i == 7)
         p1 = o + xaxis * oneview->getumax()/oneview->getscale() + yaxis * (oneview->getvmin() + oneview->getvmax())/2.0 / oneview->getscale();

       p[0] = parent->modeltransform.transform(p1);
       //p[0] = parent->modeltoscreen(p[0]);

       p[1] = p[0];  p[2] = p[0];  p[3] = p[0];
       p[0].x += -2;  p[0].y += -2;
       p[1].x += -2;  p[1].y +=  2;
       p[2].x +=  2;  p[2].y +=  2;
       p[3].x +=  2;  p[3].y += -2;
       nvert = 4;
       ptr = p;

       double sc = parent->getpixelscale() * 6.0;
       Point xa(1.0,0.0,0.0), ya(0.0,1.0,0.0);
       cfillbox2dGL(p1, xa , ya ,sc, sc);
       //parent->fillGL(&ptr,&nvert,1);
    }
}
#endif

void View3dSubSurface::drawborder(int drawmode)
{
  Point o,p1,p2,p[4],*ptr,xaxis,yaxis;
  int i,nvert;

  if(drawmode != DM_PRINT)
  {
      if(qgi)
          if(parent->getScene())
              parent->getScene()->removeItem(qgi);
      delete qgi;
      qgi=0;

      if(qgi2)
          if(parent->getScene())
            parent->getScene()->removeItem(qgi2);
      delete qgi2;
      qgi2 =0;
  }
  if(drawmode == DM_ERASE)
      return;

  QPainterPath qpath;
  qpath.setFillRule(Qt::OddEvenFill);
  //qppath.setFillRule(Qt::WindingFill);

  parent->setup(1.0,0.0,DM_GRID,0,0,0,0);   // Force the next setup to work.
  parent->setup(1.0,0.0,drawmode,0,0,0,0);
  setup(1.0,0.0,DM_GRID,0,0,0,0);   // Force the next setup to work.
  setup(1.0,0.0,drawmode,0,0,0,0);

  o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
  xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
  yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);
  p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  p2 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  Line line1(p1,p2);  line1.EntityHeader::change(0,0,0,0);

  //line1.qgi = qgi[0];
  line1.draw(drawmode,NULL,parent);
  //qgi[0] = line1.qgi;
  if(line1.qgi)
  {
      QLineF qlinef1 = ((QGraphicsLineItem*)line1.qgi)->line();
      parent->getScene()->removeItem(line1.qgi);
      //qpath.moveTo(qlinef1.x1(),qlinef1.y1());
      //qpath.lineTo(qlinef1.x2(),qlinef1.y2());
      p1 = parent->modeltoscreen(p1);
      p2 = parent->modeltoscreen(p2);
      qpath.moveTo(p1.x,parent->getheight() - 1.0 - p1.y);
      qpath.lineTo(p2.x,parent->getheight() - 1.0 - p2.y);
      qDebug() << "parent window height in drawborder : " <<  parent->getheight();
  }

  p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  p2 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  Line line2(p1,p2);  line2.EntityHeader::change(0,0,0,0);

  //line2.qgi = qgi[1];
  //line2.qgi = 0;
  line2.draw(drawmode,NULL,parent);
  //qgi[1] = line2.qgi;
  if(line2.qgi)
  {
      QLineF qlinef2 = ((QGraphicsLineItem*)line2.qgi)->line();
      parent->getScene()->removeItem(line2.qgi);
      //qpath.moveTo(qlinef2.x1(),qlinef2.y1());
      //qpath.lineTo(qlinef2.x2(),qlinef2.y2());
      p1 = parent->modeltoscreen(p1);
      p2 = parent->modeltoscreen(p2);
      qpath.moveTo(p1.x,parent->getheight() - 1.0 - p1.y);
      qpath.lineTo(p2.x,parent->getheight() - 1.0 - p2.y);
  }

  p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p2 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  Line line3(p1,p2);  line3.EntityHeader::change(0,0,0,0);

  //line3.qgi = qgi[2];
  line3.draw(drawmode,NULL,parent);
  //qgi[2] = line3.qgi;
  if(line3.qgi)
  {
      QLineF qlinef3 = ((QGraphicsLineItem*)line3.qgi)->line();
      parent->getScene()->removeItem(line3.qgi);
      //qpath.moveTo(qlinef3.x1(),qlinef3.y1());
      //qpath.lineTo(qlinef3.x2(),qlinef3.y2());
      p1 = parent->modeltoscreen(p1);
      p2 = parent->modeltoscreen(p2);
      qpath.moveTo(p1.x,parent->getheight() - 1.0 - p1.y);
      qpath.lineTo(p2.x,parent->getheight() - 1.0 - p2.y);
  }

  p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p2 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  Line line4(p1,p2);  line4.EntityHeader::change(0,0,0,0);

  //line4.qgi = qgi[3];
  line4.draw(drawmode,NULL,parent);
  //qgi[3] = line4.qgi;
  if(line4.qgi)
  {
      QLineF qlinef4 = ((QGraphicsLineItem*)line4.qgi)->line();
      parent->getScene()->removeItem(line4.qgi);
      //qpath.moveTo(qlinef4.x1(),qlinef4.y1());
      //qpath.lineTo(qlinef4.x2(),qlinef4.y2());
      p1 = parent->modeltoscreen(p1);
      p2 = parent->modeltoscreen(p2);
      qpath.moveTo(p1.x,parent->getheight() - 1.0 - p1.y);
      qpath.lineTo(p2.x,parent->getheight() - 1.0 - p2.y);
  }
  //qpath.closeSubpath();

  QGraphicsPathItem *qgpath = new QGraphicsPathItem();
  if(qgpath)
  {
      qgpath->setPen(parent->curPen);
      //qgpath->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
      qgpath->setPath(qpath);

      qgi = qgpath;
      if(parent->getScene())
          parent->getScene()->addItem(qgi);
  }

  qgi2 = new QGraphicsItemGroup();

  //  Draw the handles
  parent->setup(1.0,0.0,DM_GRID,0,0,0,0);   // Force the next setup to work.
  for (i = 0 ; i < 8 ; i++)
    {  if (i == 0)
         p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 1)
         p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 2)
         p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 3)
         p1 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 4)
         p1 = o + xaxis * (oneview->getumin()+oneview->getumax())/2.0/oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
       else if (i == 5)
         p1 = o + xaxis * (oneview->getumin()+oneview->getumax())/2.0/oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
       else if (i == 6)
         p1 = o + xaxis * oneview->getumin()/oneview->getscale() + yaxis * (oneview->getvmin() + oneview->getvmax())/2.0 / oneview->getscale();
       else if (i == 7)
         p1 = o + xaxis * oneview->getumax()/oneview->getscale() + yaxis * (oneview->getvmin() + oneview->getvmax())/2.0 / oneview->getscale();

       p[0] = parent->modeltoscreen(p1);

       p[1] = p[0];  p[2] = p[0];  p[3] = p[0];
       p[0].x += -2;  p[0].y += -2;
       p[1].x += -2;  p[1].y +=  2;
       p[2].x +=  2;  p[2].y +=  2;
       p[3].x +=  2;  p[3].y += -2;
       nvert = 4;
       ptr = p;


       QGraphicsItem *qghandle = parent->fill(0,&ptr,&nvert,1);
       if(qghandle)
           if(qgi2)
               ((QGraphicsItemGroup*)qgi2)->addToGroup(qghandle);
    }

    if(qgi2)
       if(parent->getScene())
            parent->getScene()->addItem(qgi2);

}

int View3dSubSurface::pick(int x,int y)
{double d;
 Point p1,p2,p3,p4,pos,o,xaxis,yaxis;

  o.setxyz(oneview->getxpos(),oneview->getypos(),0.0);
  xaxis.setxyz(cos(oneview->getangle()/180.0*acos(-1.0)),sin(oneview->getangle()/180.0*acos(-1.0)),0.0);
  yaxis.setxyz(-sin(oneview->getangle()/180.0*acos(-1.0)),cos(oneview->getangle()/180.0*acos(-1.0)),0.0);

  p1 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();
  p2 = o + xaxis * oneview->getumin() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p3 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmax() / oneview->getscale();
  p4 = o + xaxis * oneview->getumax() / oneview->getscale() + yaxis * oneview->getvmin() / oneview->getscale();

  if ((d = parent->pick(p1,0,x,y)) >= 0.0)
    return 1;
  if ((d = parent->pick((p1+p4)/2.0,0,x,y)) >= 0.0)
    return 2;
  if ((d = parent->pick(p4,0,x,y)) >= 0.0)
    return 3;
  if ((d = parent->pick((p3+p4)/2.0,0,x,y)) >= 0.0)
    return 4;
  if ((d = parent->pick(p3,0,x,y)) >= 0.0)
    return 5;
  if ((d = parent->pick((p2+p3)/2.0,0,x,y)) >= 0.0)
    return 6;
  if ((d = parent->pick(p2,0,x,y)) >= 0.0)
    return 7;
  if ((d = parent->pick((p1+p2)/2.0,0,x,y)) >= 0.0)
    return 8;

  return 0;
}

//
//  The offscreen bitmap classes
//

OffScreenBitmap::OffScreenBitmap(RCHDC hDCold,int w,int h)
{
#ifdef _RCDEBUG
    goffscreenbmUseCnt++;
    //qDebug() << "(const) goffscreenbmUseCnt : " << goffscreenbmUseCnt;
#endif
#ifdef USING_WIDGETS
    RCHDC hDC=0,screenhDC=0;

    screenhDC = GetDC(0);
    Surface::sizeevent(w,h);
    QPixmap *pixmap = new QPixmap(w,h);
    if(pixmap == 0)
        return;
    hbitmapold = (RCHBITMAP)pixmap;
    status = pixmap->isNull() ? 0 : 1;
    //sethdc(NULL,hbitmapold);
#else
  RCHBITMAP hbitmap;
  RCHDC hDC,screenhDC;
  status = 0;

  screenhDC = GetDC(0);

  if ((hDC = CreateCompatibleDC(screenhDC )) == NULL) return;
  if ((hbitmap = CreateCompatibleBitmap(screenhDC ,w,h)) == NULL)
    {  DeleteDC(hDC);  return;
    }
  ReleaseDC(0,screenhDC);

  hbitmapold = (RCHBITMAP) SelectObject(hDC,hbitmap);
  Surface::sizeevent(w,h);
  status = 1;
  sethdc(NULL,hDC);
#endif
}

#if defined(AVI) && (MENUS != FLEXICADMENUS)
int OffScreenBitmap::saveavistream(PAVISTREAM avistream,int frame)
{static char *pbuffer = 0;
 static long size = 0;
 BITMAPINFO bmi;
 long numwords,numbytes;
 RCHBITMAP hbitmap;
  numwords = long((width * 3L + 3L) / 4L);
  numbytes = long(numwords * 4L * height);
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = int(width);
  bmi.bmiHeader.biHeight = int(height);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = numbytes;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  if (pbuffer == 0 || size < numbytes)
    {  delete [] pbuffer;
       size = 0;
       if ((pbuffer = new char [int(numbytes)]) == 0)
         return 0;
       size = numbytes;
    }
  hbitmap = (RCHBITMAP) SelectObject(gethdc(),hbitmapold);
  GetDIBits(gethdc(),hbitmap,0,int(height),pbuffer,&bmi,DIB_RGB_COLORS);
  SelectObject(gethdc(),hbitmap);

  if (frame == 1)
    {  if (AVIStreamSetFormat(avistream,0,&bmi,bmi.bmiHeader.biSize) != AVIERR_OK)
         return 0;
    }
  if (AVIStreamWrite(avistream,frame-1,1,pbuffer,numbytes,0,0,0) != AVIERR_OK)
    return 0;
  return 1;
}

int View3dWindow::saveavistream(PAVISTREAM avistream,int frame)
{static char *pbuffer = 0;
 static long size = 0;
 BITMAPINFO bmi;
 long numwords,numbytes;
 RCHBITMAP hbitmap,hbitmapold;
 HDC hdc;
  numwords = long((width * 3L + 3L) / 4L);
  numbytes = long(numwords * 4L * height);
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = int(getwidth());
  bmi.bmiHeader.biHeight = int(getheight());
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = numbytes;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;
  if (pbuffer == 0 || size < numbytes)
    {  delete [] pbuffer;
       size = 0;
       if ((pbuffer = new char [int(numbytes)]) == 0)
         return 0;
       size = numbytes;
    }

  if ((hdc = CreateCompatibleDC(gethdc())) == NULL) return 0;
  if ((hbitmap = CreateCompatibleBitmap(gethdc(),int(getwidth()),int(getheight()))) == NULL)
    {  DeleteDC(hdc);  return 0;
    }
  hbitmapold = (RCHBITMAP) SelectObject(hdc,hbitmap);
  BitBlt(hdc,0,0,int(getwidth()),int(getheight()),gethdc(),0,0,SRCCOPY);
  SelectObject(hdc,hbitmapold);

  GetDIBits(hdc,hbitmap,0,int(getheight()),pbuffer,&bmi,DIB_RGB_COLORS);

  DeleteObject(hbitmap);
  DeleteDC(hdc);

  if (frame == 1)
    if (AVIStreamSetFormat(avistream,0,&bmi,bmi.bmiHeader.biSize) != AVIERR_OK)
      return 0;
  if (AVIStreamWrite(avistream,frame-1,1,pbuffer,numbytes,0,0,0) != AVIERR_OK)
    return 0;
  return 1;
}

#else
#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)

int OffScreenBitmap::saveavistream(PAVISTREAM,int)
{ return 1;
}
int View3dWindow::saveavistream(PAVISTREAM,int)
{ return 1;
}
#endif
#endif

OffScreenBitmap::~OffScreenBitmap()
{
#ifdef _RCDEBUG
    goffscreenbmUseCnt--;
    //qDebug() << "(dest) goffscreenbmUseCnt : " << goffscreenbmUseCnt;
#endif
#ifdef USING_WIDGETS

#else
 RCHBITMAP hbitmap;
 RCHPEN holdpen;
  if (status)
    {  hbitmap = (RCHBITMAP) SelectObject(gethdc(),hbitmapold);
       DeleteObject(hbitmap);
       holdpen = (RCHPEN) SelectObject(hDC,horiginalpen);
       DeleteObject(holdpen);
       DeleteDC(hDC);
       if (SetLight != 0)
         ReleaseTextures(hDC);
       hDC = 0;
    }
#endif
}

View3dOffScreenBitmap::View3dOffScreenBitmap(View3dSurface *window,int pixelscale) :
  OffScreenBitmap(window->gethdc(),int(window->getwidth())*pixelscale,int(window->getheight()*pixelscale)),
  View3dSurface(window->getview(),window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),0,0)

{
  QGraphicsScene *scene = new QGraphicsScene(0,0,getwidth(),getheight());
  setrepaintstyle(window->getrepaintstyle());
  setbackground(window->getbackground());
  hDC = window->gethdc();
  hWnd = window->gethwnd();
  setScene(scene);
}

View3dOffScreenBitmap::View3dOffScreenBitmap(View3dSurface *window,int width,int height) :
  OffScreenBitmap(window->gethdc(),width,height),
  View3dSurface(window->getview(),window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),0,0)

{
   QGraphicsScene *scene = new QGraphicsScene(0,0,getwidth(),getheight());

  setrepaintstyle(window->getrepaintstyle());
  setbackground(window->getbackground());

  //  Take the dots per mm settings from the original surface
  //setxdotspermm(window->getxdotspermm());
  //setydotspermm(window->getydotspermm());
  hDC = window->gethdc();
  hWnd = window->gethwnd();
  setScene(scene);
}

void View3dOffScreenBitmap::ChangeView(View3dSurface *vs)
{ view = vs->getview();
  sizeevent(vs->getwidth(),vs->getheight());
}

void View3dOffScreenBitmap::paintevent(void)
{
#if 1
 RECT rect;

 scale = (umax - umin) / getwidthmm();
 rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
 getScene()->setSceneRect(rect.left,rect.right,rect.right,rect.bottom);
 //getScene()->setBackgroundBrush(QBrush(QColor(int(::v.getreal("wn::backgroundcolour.red")*255.0+0.5),
 //                                             int(::v.getreal("wn::backgroundcolour.green")*255.0+0.5),
 //                                             int(::v.getreal("wn::backgroundcolour.blue")*255.0+0.5))));

 QPainter painter(getBitmap());
 getScene()->render(&painter,QRectF(getScene()->sceneRect()),QRectF(getScene()->sceneRect()));
 painter.end();

 //db.draw(DM_NORMAL,this);
#else
 RECT rect;
 RCHBRUSH hbrush;
 scale = (umax - umin) / getwidthmm();
 rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
 hbrush = CreateSolidBrush(getbackground());
 FillRect(gethdc(),&rect,hbrush);
 DeleteObject(hbrush);
 if (getrepaintstyle() == Wireframe || getrepaintstyle() == RayTraced)
   {  if (SetLight != 0 && requiresopengl())
        {View *view;
           OpenOpenGl(gethdc(),2,0,0,(int)getwidth(),(int)getheight());
           view = getview();

           SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
           SetViewport(getumin(),getvmin(),getumax(),getvmax());

           SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

           BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
           SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
           db.draw(DM_NORMAL,this);
           EndScene();

        }
      else
        db.draw(DM_NORMAL,this);


   }
 else if (getrepaintstyle() == Hidden)
   {HideImage image(this,1+v.getinteger("hd::dashhiddenlines") * 2);
      image.hide();
   }
 else
   {double ambient,diffuse;
    int hiddenlines;
      ambient = v.getreal("sh::ambient");
      diffuse = v.getreal("sh::diffuse");
      hiddenlines = v.getinteger("sh::hiddenlines");
      RenderImage image(this,1,ambient,diffuse);
      image.render();
      if (hiddenlines)
        {HideImage image(this,v.getinteger("hd::dashhiddenlines") * 2);
           image.hide();
        }
   }
#endif
}

void View2dOffScreenBitmap::paintevent(void)
{RECT rect;
 RCHBRUSH hbrush;
 RCHPEN hpen,holdpen;
 Point p1,p2;
 View3dSubSurface *ss;
 OneView *ov;

  rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
  hbrush = CreateSolidBrush(getbackground());
  FillRect(gethdc(),&rect,hbrush);
  DeleteObject(hbrush);

  program->setbusy(1);
  setclip(0.0,0.0,getwidth(),getheight());
  rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
  hpen  = CreatePen(PS_DOT,0, RGB(255-GetRValue(background),255-GetGValue(background),255-GetBValue(background)));
  holdpen = (RCHPEN)SelectObject(gethdc(),hpen);
  DeleteObject(holdpen);
  SetROP2(gethdc(), R2_COPYPEN);
  p1.setxyz(0.0,0.0,0.0);  p2.setxyz(view2d->getpaperwidth(),0.0,0.0);
  line3d(p1,p2);
  p1.setxyz(view2d->getpaperwidth(),0.0,0.0);  p2.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);
  line3d(p1,p2);
  p1.setxyz(view2d->getpaperwidth(),view2d->getpaperheight(),0.0);  p2.setxyz(0.0,view2d->getpaperheight(),0.0);
  line3d(p1,p2);
  p1.setxyz(0.0,view2d->getpaperheight(),0.0);  p2.setxyz(0.0,0.0,0.0);
  line3d(p1,p2);

  for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
    if ((ss = new View3dSubSurface(this,ov)) != NULL)
      {  ss->setdisplaylayertable(ss->getoneview()->getlayertable());
         ss->setprinterhdc(getprinterhdc());
         if (ov->gethiddenlines() || ov->getshaded())
           {  if (ov->getshaded())
                {double ambient,diffuse;
                  ambient = v.getreal("sh::ambient");
                  diffuse = v.getreal("sh::diffuse");
                  RenderImage image(ss,0,ambient,diffuse);
                  image.render();
                }
              if (ov->gethiddenlines())
                {  HideImage image(ss,ov->getdashhidden() * 2);
                   image.hide();
                }
           }
         else
           ss->paintevent();
         delete ss;
      }

  setup(1.0,0.0,DM_DASH,0,0,0,0);
  db.geometry.draw(DM_NORMAL,this);
  db.grid.draw(DM_NORMAL,this);

  program->setbusy(0);

}

void Control::paintEvent(QPaintEvent *)
{
    drawborder();
}

void Control::drawborder(void)
{RECT rect;
 int i;
 //hDC = this; ??
  //  Draw the boundary around the coordinate display area
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x + w + MenuBorderOffset+borderx2;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y - MenuBorderOffset + 1 + bordery1;
  FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x - MenuBorderOffset  - MenuHandleOffset + 1 + borderx1;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x + w + MenuBorderOffset + borderx2;
  rect.top = y + h + MenuBorderOffset - 1 + bordery2;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
  rect.left = x + w + MenuBorderOffset - 1 + borderx2;  rect.right = x + w + MenuBorderOffset + borderx2;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));

  if (w > h)
    {  for (i = 1 ; i < 5 ; i += 4)
         {  //  Draw the menu handle
            rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 3;
            rect.top = y;  rect.bottom = y + 1;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
            rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 1;
            rect.top = y;  rect.bottom = y + h;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));

            rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 3;
            rect.top = y + h - 1;  rect.bottom = y + h;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
            rect.left = x - MenuBorderOffset - MenuHandleOffset + 4 + i;  rect.right = rect.left + 1;
            rect.top = y;  rect.bottom = y + h;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
         }
    }
  else
    {  for (i = 1 ; i < 5 ; i += 4)
         {  //  Draw the menu handle
            rect.left = x;  rect.right = x + 1;
            rect.top = y - MenuBorderOffset - MenuHandleOffset + 2;  rect.bottom = rect.top + 3;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
            rect.left = x;  rect.right = x + w;
            rect.top = y - MenuBorderOffset - MenuHandleOffset + 2;  rect.bottom = rect.top + 1;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNHILIGHT));

            rect.left = x + w - 1;  rect.right = x + w;
            rect.top = y - MenuBorderOffset - MenuHandleOffset + 2;  rect.bottom = rect.top + 3;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
            rect.left = x;  rect.right = x + w;
            rect.top = y - MenuBorderOffset - MenuHandleOffset + 4;  rect.bottom = rect.top + 1;
            FillRect(hDC,&rect,GetSysColorBrush(COLOR_BTNSHADOW));
         }
    }

}

int Control::drag(int xp,int yp,int offset,int extraheight,int buts)
{int i,thickness;
 int result,y1,y2;
 Control *c;

  if (xp == -32767 && yp == -32767)
    return 0;

  if (buts == -1)
    {  dragoffsetx = dragoffsety = 0;
       return 2;  //  Force this to be a floating menu - This is a startup flag
    }

  result = 0;

  for (i = 0 ; (c = cadwindow->getcontrol(i)) != 0 ; i++)
    if (c != this && c->dragging)
      return 0;  //  Only drag one menu at a time

  //  Control is being or about to be dragged.
  if (dragging ||
      w >= h && xp >= x - MenuHandleOffset && xp < x && yp > y && yp <= y  + h - 1 && buts == 1 ||
      w < h && xp >= x && xp < x + w + 1 && yp > y - MenuHandleOffset - MenuBorderOffset && yp <= y && buts == 1)
    {RCHWND hwnd;
     RCHDC hdc;
     RCHBRUSH holdbrush;
     RCHPEN hpen,holdpen;
     POINT p1,p2;
     RECT rect;

       if (buts == 1 && dragging != 1 && parent == cadwindow)
         {  //  Setup the drag offset
            dragoffsetx = x - xp;
            dragoffsety = y - yp;
         }
       else if (buts == 1 && dragging != 1 && parent != cadwindow)
         {POINT p1;
            p1.x = p1.y = 0;
            ClientToScreen(parent->gethwnd(),&p1);
            ScreenToClient(cadwindow->gethwnd(),&p1);
            dragoffsetx = p1.x - xp;
            dragoffsety = p1.y - yp;
         }

       hwnd = GetDesktopWindow();
       hdc = GetDC(0);
       hpen = CreatePen(PS_SOLID,1,RGB(255,255,255));
       holdpen = (RCHPEN) SelectObject(hdc,hpen);
       holdbrush = (RCHBRUSH) SelectObject(hdc,GetStockObject(NULL_BRUSH));
       SetROP2(hdc,R2_NOT);

       if (dragging == 1)
         {  p1.x = lastdragx + dragoffsetx;  p1.y = lastdragy + dragoffsety;
            ClientToScreen(cadwindow->gethwnd(),&p1);
            if (WindowFromPoint(p1) == cadwindow->gethwnd())
              thickness = 1;
            else
              {  ScreenToClient(cadwindow->subwindow->gethwnd(),&p1);
                 GetClientRect(cadwindow->subwindow->gethwnd(),&rect);
                 if (p1.x >= 0 && p1.x <= 25 ||
                     p1.x >= rect.right-25 && p1.x <= rect.right ||
                     p1.y >= 0 && p1.y <= 25 ||
                     p1.y >= rect.bottom-25 && p1.y <= rect.bottom)
                   thickness = 1;
                 else
                   thickness = 3;
              }

            p1.x = lastdragx + dragoffsetx - MenuBorderOffset - MenuHandleOffset;  p1.y = lastdragy + dragoffsety - MenuBorderOffset;
            ClientToScreen(cadwindow->gethwnd(),&p1);
            p2.x = lastdragx + dragoffsetx + w + MenuBorderOffset;  p2.y = lastdragy + dragoffsety + h + MenuBorderOffset;
            ClientToScreen(cadwindow->gethwnd(),&p2);
            for (i = 0 ; i < thickness ; i++)
              if (extraheight > 0)
                Rectangle(hdc,p1.x-i,p1.y-i,p2.x+i,p2.y+i+extraheight);
              else
                Rectangle(hdc,p1.x-i,p1.y-i+extraheight,p2.x+i,p2.y+i);
         }

       lastdragx = xp; lastdragy = yp;
       if (buts == 1)
         {  p1.x = lastdragx + dragoffsetx;  p1.y = lastdragy + dragoffsety;
            ClientToScreen(cadwindow->gethwnd(),&p1);
            if (WindowFromPoint(p1) == cadwindow->gethwnd())
              thickness = 1;
            else
              {  ScreenToClient(cadwindow->subwindow->gethwnd(),&p1);
                 GetClientRect(cadwindow->subwindow->gethwnd(),&rect);
                 if (p1.x >= 0 && p1.x <= 25 ||
                     p1.x >= rect.right-25 && p1.x <= rect.right ||
                     p1.y >= 0 && p1.y <= 25 ||
                     p1.y >= rect.bottom-25 && p1.y <= rect.bottom)
                   thickness = 1;
                 else
                   thickness = 3;
              }
            dragging = 1;
            SetCapture(cadwindow->gethwnd());
            p1.x = lastdragx + dragoffsetx - MenuBorderOffset - MenuHandleOffset;  p1.y = lastdragy + dragoffsety - MenuBorderOffset;
            ClientToScreen(cadwindow->gethwnd(),&p1);
            p2.x = lastdragx + dragoffsetx + w + MenuBorderOffset;  p2.y = lastdragy + dragoffsety + h + MenuBorderOffset;
            ClientToScreen(cadwindow->gethwnd(),&p2);
            for (i = 0 ; i < thickness ; i++)
              if (extraheight > 0)
                Rectangle(hdc,p1.x-i,p1.y-i,p2.x+i,p2.y+i+extraheight);
              else
                Rectangle(hdc,p1.x-i,p1.y-i+extraheight,p2.x+i,p2.y+i);
         }
       else
         {//  Dragging has finished
            ReleaseCapture();

            y1 = yp + dragoffsety + h + MenuBorderOffset;
            y2 = yp + dragoffsety - MenuBorderOffset;
            if (extraheight > 0)
              y1 += extraheight;
            if (extraheight < 0)
              y2 += extraheight;

            //  Make sure it doesn't go over an existing control.
            for (i = 0 ; (c = cadwindow->getcontrol(i)) != 0 ; i++)
              if ((c != this &&  //  Don't match itself
                    (c->x > x + w || c->x + c->w < x || c->y > y + h || c->y + c->h <y) &&    //  Don't match children controls
                    (c->x != x || c->y != y + extraheight)) &&
                    !(xp + dragoffsetx + w + MenuBorderOffset < c->x ||
                      xp + dragoffsetx - MenuBorderOffset - MenuHandleOffset > c->x + c->w ||
                      y1 < c->y || y2 > c->y + c->h))
                break;

            if (c == 0 && y1 >= 0 && y2 >= 0)
              {  if (yp + dragoffsety < MenuBorderOffset)
                   yp = MenuBorderOffset - dragoffsety;
                 p1.x = lastdragx + dragoffsetx;  p1.y = lastdragy + dragoffsety;
                 ClientToScreen(cadwindow->gethwnd(),&p1);

                 if (WindowFromPoint(p1) == cadwindow->gethwnd())
                   result = 1;
                 else
                   {  ScreenToClient(cadwindow->subwindow->gethwnd(),&p1);
                      GetClientRect(cadwindow->subwindow->gethwnd(),&rect);
                      if (p1.x >= 0 && p1.x <= 25 ||
                          p1.x >= rect.right-25 && p1.x <= rect.right ||
                          p1.y >= 0 && p1.y <= 25 ||
                          p1.y >= rect.bottom-25 && p1.y <= rect.bottom)
                        result = 1;
                      else
                        result = 2;
                   }

              }

            if (parent != cadwindow)
              cadwindow->delcontrol(this,0);

            dragging = 0;
         }

       SelectObject(hdc,holdpen);
       SelectObject(hdc,holdbrush);
       DeleteObject(hpen);
       ReleaseDC(hwnd,hdc);

    }

  return result;

}

void Control::move(int dx,int dy)
{
}


//
//  The coordinate entry class
//

void OneCoordinateEntry::setvisible(int v)
{
  ShowWindow(window->gethwnd(),v);
  ShowWindow(fixwindow->gethwnd(),v);
}

void OneCoordinateEntry::disablew(void)
{
  EnableWindow(window->gethwnd(),FALSE);
  EnableWindow(fixwindow->gethwnd(),FALSE);
}

void OneCoordinateEntry::setenabled(bool set)
{
  EnableWindow(window->gethwnd(),set);
  EnableWindow(fixwindow->gethwnd(),set);
}

#ifdef USING_WIDGETS
OneCoordinateEntry::OneCoordinateEntry(Window *win,int xp,int yp,int width,int height,int n,QWidget *qparent)
{
  RCCHAR message[300];
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;
  prompt = new Prompt(x+5,y+5,0,qparent);
  prompt->fixup(win);
  name = n;
  sprintf(message,"%c",name);
  prompt->normalprompt(message);
  incremental = 0;
  hlocked = LoadIcon(program->gethinst(),MAKEINTRESOURCE(20000));
  hunlocked = LoadIcon(program->gethinst(),MAKEINTRESOURCE(20001));
  isLocked = false;

  // add the widgets pointers
  // initialize the coordinate label
  m_qwPrompt = (QWidget*)prompt->getQW();
  ((QLabel*)m_qwPrompt)->setFrameStyle(QFrame::NoFrame);
  ((QLabel*)m_qwPrompt)->setMinimumHeight(h);
  ((QLabel*)m_qwPrompt)->setMaximumHeight(h);
  ((QLabel*)m_qwPrompt)->setMinimumWidth(h-6);
  ((QLabel*)m_qwPrompt)->setMinimumWidth(h-6);

  // initialize the lock icon
  // using a QLabel for the lock icon
  // using a QLabel for the lock icon
  fixwindow = new Window(_RCT("STATIC"),qparent,WS_CHILD | WS_VISIBLE | SS_ICON | SS_REALSIZEIMAGE | SS_CENTERIMAGE,x+18,y,20,20,SW_SHOW);
  isLocked=false;
  m_qwIcon = (QWidget*)fixwindow->gethwnd();
  QPixmap pixmap(hunlocked.pixmap(QSize(h+3,h+3)));
  ((QLabel*)m_qwIcon)->setPixmap(pixmap);
  ((QLabel*)m_qwIcon)->setScaledContents(true);
  ((QLabel*)m_qwIcon)->setFrameStyle(QFrame::NoFrame);
  ((QLabel*)m_qwIcon)->setContentsMargins(0,0,0,0);
  ((QLabel*)m_qwIcon)->setAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
  ((QLabel*)m_qwIcon)->setMinimumHeight(h+3);
  ((QLabel*)m_qwIcon)->setMaximumHeight(h+3);
  ((QLabel*)m_qwIcon)->setMinimumWidth(h+3);
  ((QLabel*)m_qwIcon)->setMaximumWidth(h+3);

  // initialize the edit box
  window = new Window(_RCT("EDIT"),qparent,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,x+38,y,w-20,h,SW_SHOW);
  m_qwEdit = (QWidget*)window->gethwnd();
  ((QLineEdit*)m_qwEdit)->setMinimumHeight(h);
  ((QLineEdit*)m_qwEdit)->setMaximumHeight(h);
  ((QLineEdit*)m_qwEdit)->setMinimumWidth(100);
  ((QLineEdit*)m_qwEdit)->setMaximumWidth(100);
  ((QLineEdit*)m_qwEdit)->setTextMargins(2,2,2,2);
  ((QLineEdit*)m_qwEdit)->setText("0.00");
  ((QLineEdit*)m_qwEdit)->setCursorMoveStyle(Qt::VisualMoveStyle);
  ((QLineEdit*)m_qwEdit)->setFocusPolicy(Qt::StrongFocus);

  // set the validator for the line edit
  RCCoordindateEntryValidator *validator = new RCCoordindateEntryValidator((QLineEdit*)m_qwEdit);
  ((QLineEdit*)m_qwEdit)->setValidator(validator);

}
#else
OneCoordinateEntry::OneCoordinateEntry(Window *win,int xp,int yp,int width,int height,int n)
{
  RCCHAR message[300];
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;
  prompt = new Prompt(x+5,y+5,0);
  prompt->fixup(win);
  name = n;
  sprintf(message,"%c",name);
  prompt->normalprompt(message);
  incremental = 0;
  hlocked = LoadIcon(program->gethinst(),MAKEINTRESOURCE(20000));
  hunlocked = LoadIcon(program->gethinst(),MAKEINTRESOURCE(20001));
  window = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                       x+38,y,w-20,h,SW_SHOW);
  fixwindow = new Window(_RCT("STATIC"),win->gethwnd(),WS_CHILD | WS_VISIBLE | SS_ICON | SS_REALSIZEIMAGE | SS_CENTERIMAGE,x+18,y,20,20,SW_SHOW);
  SendMessage(fixwindow->gethwnd(),STM_SETICON,(LPARAM)hunlocked,0);
  PostMessage(window->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
}
#endif

OneCoordinateEntry::~OneCoordinateEntry()
{
  delete prompt;
  DestroyWindow(window->gethwnd());
  delete window;
  delete fixwindow;
}

int OneCoordinateEntry::getlocked(void)
{
    return (int)isLocked;
}

void OneCoordinateEntry::setlocked(int locked)
{
    isLocked = locked;
    if(isLocked)
    {
        QPixmap pixmap(hlocked.pixmap(QSize(h+3,h+3)));
        ((QLabel*)m_qwIcon)->setPixmap(pixmap);
    }
    else
    {
         QPixmap pixmap(hunlocked.pixmap(QSize(h+3,h+3)));
        ((QLabel*)m_qwIcon)->setPixmap(pixmap);
    }
}

void OneCoordinateEntry::enablelock(bool flag)
{
    //QPixmap pixmap(hunlocked.pixmap(QSize(h-4,h-4)));
    //((QLabel*)m_qwIcon)->setPixmap(pixmap);
    ((QLabel*)m_qwIcon)->setEnabled(flag);
}

void OneCoordinateEntry::toggle(void)
{
 RCCHAR mess[300];
 ResourceString rs6(NCWIN+6);
  incremental = ! incremental;
  if (incremental)
    sprintf(mess,rs6.gets(),name);
  else
    sprintf(mess,"%c",name);
  prompt->normalprompt(mess);
}

void OneCoordinateEntry::cleartoggle(void)
{
 RCCHAR mess[300];
 ResourceString rs6(NCWIN+6);
  incremental = 0;
  sprintf(mess,"%c",name);
  prompt->normalprompt(mess);
}

void OneCoordinateEntry::select(void)
{
    ((QLineEdit*)m_qwEdit)->activateWindow();
    ((QLineEdit*)m_qwEdit)->setFocus();
    ((QLineEdit*)m_qwEdit)->deselect();
    ((QLineEdit*)m_qwEdit)->selectAll();
    ((QLineEdit*)m_qwEdit)->update();
}

void OneCoordinateEntry::deselect(void)
{
    //((QLineEdit*)m_qwEdit)->activateWindow();
    ((QLineEdit*)m_qwEdit)->deselect();
    ((QLineEdit*)m_qwEdit)->clearFocus();
    ((QLineEdit*)m_qwEdit)->update();
}

bool OneCoordinateEntry::isallselected(void)
{
    if(((QLineEdit*)m_qwEdit)->selectedText().length() == ((QLineEdit*)m_qwEdit)->text().length())
        return true;
    else
        return false;
}

void OneCoordinateEntry::set(double value)
{
  RCCHAR mess[300];
  sprintf(mess,"%.2f",value/db.getmainunitscale());
#ifdef USING_WIDGETS
  ((QLineEdit*)window->gethwnd())->setText(QString(mess));
  //SetWindowText(window->gethwnd(),mess);
#else
  SetWindowText(window->gethwnd(),_RCT(mess));
#endif
  incremental = 1;
  toggle();
}

int OneCoordinateEntry::get(double last,double *value)
{
  RCCHAR oldtext[300],text[300],*s,*dc;
  double x,bearing,direction;
  sprintf(oldtext,"%.2f",last/db.getmainunitscale());
#ifdef USING_WIDGETS
  ((QLineEdit*)window->gethwnd())->text().constData();
  GetWindowText(window->gethwnd(),text,128);
#else
  GetWindowText(window->gethwnd(),text,128);
#endif
  if (strcmp(oldtext,text) == 0 && ! incremental)
    {  *value = last;  return 1;
    }
  else
    {  s = text;
       _strlwr(text);
       bearing = 0.0;
       if (text[0] == 'n')
         {  bearing = 0.0;  s++;
         }
       else if (text[0] == 's')
         {  bearing = 180.0;  s++;
         }
       else if (text[0] == 'e')
         {  bearing = 90.0;  s++;
         }
       else if (text[0] == 'w')
         {  bearing = 270.0;  s++;
         }
       if (strlen(text) > 0)
         {  dc = text + strlen(text) - 1;
         }
       else
         dc = 0;

       direction = 1.0;
       if (dc != 0 && *dc == 'n')
         {  direction = bearing == 90.0 ? -1.0 : 1.0;
            *dc = 0;
         }
       else if (dc != 0 && *dc == 'e')
         {  direction = bearing == 180.0 ? -1.0 : 1.0;
            *dc = 0;
         }
       else if (dc != 0 && *dc == 's')
         {  direction = bearing == 270.0 ? -1.0 : 1.0;
            *dc = 0;
         }
       else if (dc != 0 && *dc == 'w')
         {  direction = bearing == 0.0 ? -1.0 : 1.0;
            *dc = 0;
         }

       Expression expression(s);
       if (expression.evaluate(&x) == 0)
         {  *value = 0.0;  return 0;
         }

       x = bearing + x * direction;

       x *= db.getmainunitscale();
       if (incremental) *value = x + last; else *value = x;
       return 1;
    }
}

void OneCoordinateEntry::move(Window *win,int xp,int yp,int width,int height)
{
#ifdef USING_WIDGETS
    ((QWidget*)window->gethwnd())->update();
#else
  prompt->draw(DM_ERASE);
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;

  if (! IsWindow(window->gethwnd()))
  {
       delete window;
       window = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                             x+38,y,w-20,h,SW_SHOW);
       PostMessage(window->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
  }
  if (! IsWindow(fixwindow->gethwnd()))
  {
       delete fixwindow;
       fixwindow = new Window(_RCT("STATIC"),win->gethwnd(),WS_CHILD | WS_VISIBLE | SS_ICON | SS_REALSIZEIMAGE | SS_CENTERIMAGE,x+18,y,20,20,SW_SHOW);
       //SendMessage(fixwindow->gethwnd(),STM_SETICON,(LPARAM)hunlocked,0);
  }
  SetParent(window->gethwnd(),win->gethwnd());
  SetParent(fixwindow->gethwnd(),win->gethwnd());
  prompt->move(win,x+5,y+2);
  SetWindowPos(window->gethwnd(),HWND_TOP,x+38,y,w-20,h,SWP_DRAWFRAME);
  SetWindowPos(fixwindow->gethwnd(),HWND_TOP,x+18,y,20,20,SWP_DRAWFRAME);
  prompt->draw(DM_NORMAL);
#endif
}

CoordinateEntry::CoordinateEntry(Window *pw,int x1,int y1,int width)
{
  csystem = 0;
  lastbuttons = 0;
  x = xpos = x1;
  y = ypos = y1;
  w = width;
  parent = pw;
  newcoordinate = 0;

  RCCoordindateEntryToolBar *ToolBar = new RCCoordindateEntryToolBar("Coordinate Entry",(QWidget*)pw->gethdc());
  ToolBar->setObjectName("RCCoordinateEntry");
  xentry = new OneCoordinateEntry(pw,x,y+2,width/3-25,MainWindow::getcoordheight()-4,'X',ToolBar);
  yentry = new OneCoordinateEntry(pw,x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-4,'Y',ToolBar);
  zentry = new OneCoordinateEntry(pw,x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-4,'Z',ToolBar);

  //QLineEdit *test = new QLineEdit(ToolBar);
  //ToolBar->addWidget(test);
  //RCCoordindateEntryValidator *validator = new RCCoordindateEntryValidator(test);
  //test->setValidator(validator);


  xentry->set(0.0);
  yentry->set(0.0);
  zentry->set(0.0);
  h = xentry->h;

  borderx1 = 0;
  bordery1 = 1;
  borderx2 = 0;
  bordery2 = -1;

  ToolBar->addWidget(xentry->get_QWprompt());
  ToolBar->addWidget(xentry->get_QWIcon());

  //QWidgetAction *widgetAction = new QWidgetAction(ToolBar);
  //widgetAction->setDefaultWidget((QLineEdit*)xentry->get_QWedit());
  //ToolBar->addAction(widgetAction);

  ToolBar->addWidget((QLineEdit*)xentry->get_QWedit());
  xentry->get_QWedit()->setObjectName(xentry->get_QWedit()->objectName() += QString("_X_CEdit") );
  xentry->get_QWprompt()->setObjectName(xentry->get_QWprompt()->objectName() += QString("_X_Prompt") ) ;
  xentry->get_QWIcon()->setObjectName(xentry->get_QWIcon()->objectName() += QString("_X_Icon") ) ;
  w = xentry->get_QWedit()->width();
  w += xentry->get_QWprompt()->width();
  w += xentry->get_QWIcon()->width();
  int minWidth = w+4+10;

  ToolBar->addWidget(yentry->get_QWprompt());
  ToolBar->addWidget(yentry->get_QWIcon());
  ToolBar->addWidget(yentry->get_QWedit());
  yentry->get_QWedit()->setObjectName(yentry->get_QWedit()->objectName() += QString("_Y_CEdit") );
  yentry->get_QWprompt()->setObjectName(yentry->get_QWprompt()->objectName() += QString("_Y_Prompt"));
  yentry->get_QWIcon()->setObjectName(yentry->get_QWIcon()->objectName() += QString("_Y_Icon"));
  w += yentry->get_QWedit()->width();
  w += yentry->get_QWprompt()->width();
  w += yentry->get_QWIcon()->width();

  ToolBar->addWidget(zentry->get_QWprompt());
  ToolBar->addWidget(zentry->get_QWIcon());
  ToolBar->addWidget(zentry->get_QWedit());
  zentry->get_QWedit()->setObjectName(zentry->get_QWedit()->objectName() += QString("_Z_CEdit") );
  zentry->get_QWprompt()->setObjectName(zentry->get_QWprompt()->objectName() += QString("_Z_Prompt"));
  zentry->get_QWIcon()->setObjectName(zentry->get_QWIcon()->objectName() += QString("_Z_Icon"));
  w += zentry->get_QWedit()->width();
  w += zentry->get_QWprompt()->width();
  w += zentry->get_QWIcon()->width();
  w += 4*10+10;

  ToolBar->setMinimumWidth(minWidth);
  //ToolBar->setMaximumWidth(w);
  ToolBar->setStyleSheet("spacing: 4px;");

  m_qw = ToolBar;
  h = ToolBar->height();
  w = ToolBar->width();
  x = xpos = ToolBar->pos().x();
  y = ypos = ToolBar->pos().y();

}

CoordinateEntry::~CoordinateEntry()
{
  delete xentry;
  delete yentry;
  delete zentry;
  delete m_qw; m_qw=0; // the QToolBar
}

void CoordinateEntry::position(Point p)
{
  double r,cosa,sina,a;

  if (v.getinteger("db::incrementalcoordinates") == 1)  //  Try not updating this data.
    return;

  if (db.workplanes.getcurrent() != NULL)
    p = db.workplanes.getcurrent()->modeltoworkplane(p);
  switch (csystem)
    {  case 0 :
         xentry->set(p.x);
         yentry->set(p.y);
         zentry->set(p.z);
         break;
       case 1 :
         r = sqrt(p.x * p.x + p.y * p.y);
         if (fabs(r) > db.getptoler())
           {  cosa = p.x / r;  sina = p.y / r;
           }
         else
           {  cosa = sina = 0.0;
           }
         if (cosa < -1.0) cosa = -1.0; else if (cosa > 1.0) cosa = 1.0;
         a = acos(cosa);
         if (sina < 0.0) a = 2.0 * M_PI - a;
         a *= 180.0 / M_PI;
         xentry->set(r);
         yentry->set(a);
         zentry->set(p.z);
         break;
       case 2 :
         xentry->set(0.0);
         yentry->set(0.0);
         zentry->set(0.0);
         xentry->toggle();
         yentry->toggle();
         zentry->toggle();
         break;
    }
}

Point CoordinateEntry::fix(Point p)
{Point p1,lastp,poriginal;
  double a,r,angle,l,length;
 int ok;

  if (cadwindow->getshiftstate() == 0 && db.workplanes.getcurrent() != 0 && (xentry->getlocked() || yentry->getlocked() || zentry->getlocked()))
    {  p = db.workplanes.getcurrent()->modeltoworkplane(p);
      // p is the current cursor position
      qDebug() << "p : " << p.x << "," << p.y << "," << p.z;

       //  Get the current keyboard position
       // lastp is the previous cursor pick (LMB) position
       lastp = state.getposition();
       if (db.workplanes.getcurrent() != NULL)
         lastp = db.workplanes.getcurrent()->modeltoworkplane(lastp);
       switch (csystem)
         {  case 0 :
              ok = xentry->get(lastp.x,&p1.x) &&
                   yentry->get(lastp.y,&p1.y) &&
                   zentry->get(lastp.z,&p1.z);
              break;
            case 1 :
              ok = xentry->get(0.0,&p1.x) &&
                   yentry->get(0.0,&p1.y) &&
                   zentry->get(lastp.z,&p1.z);
              if (ok)
                {  r = p1.x;
                   a = p1.y / db.getmainunitscale() * M_PI / 180.0;
                   if (r == 0.0) r = 1.0;
                   p1.x = fabs(r) * cos(a);  p1.y = fabs(r) * sin(a);
                   if (xentry->incremental || yentry->incremental)
                     {  p1.x += lastp.x;  p1.y += lastp.y;
                     }
                }
              break;
            case 2 :
              ok = xentry->get(0.0,&p1.x) &&
                   yentry->get(0.0,&p1.y) &&
                   zentry->get(lastp.z,&p1.z);
              if (ok)
                {  r = p1.y;  a = p1.x / db.getmainunitscale() * M_PI / 180.0;
                   if (r == 0.0) r = 1.0;
                   p1.x = r * sin(a);  p1.y = r * cos(a);
                   if (xentry->incremental || yentry->incremental)
                     {  p1.x += lastp.x;  p1.y += lastp.y;
                     }
                }
              break;
#if MAJOR_VERSION > 7
              // Enhancement u9 for v8
           case 3 :
             ok = xentry->get(0.0,&p1.x) &&
                  yentry->get(0.0,&p1.y) &&
                  zentry->get(lastp.z,&p1.z);
             if(ok)
             {
                 double tempz = p1.z;
                 p1 = lastp + (p - lastp).normalize() * p1.x;
                 p1.z = tempz;
                 qDebug() << "p1 : " << p1.x << "," << p1.y << "," << p1.z;
             }
           break;
             //
#endif
       }

       //  p1 is the position of the current keyboard entry
       switch (csystem)
         {case 0 :
            if (xentry->getlocked())
              p.x = p1.x;
            if (yentry->getlocked())
              p.y = p1.y;
            if (zentry->getlocked())
              p.z = p1.z;
            break;
          case 1 :
          case 2 :
            p -= lastp;
            p1 -= lastp;
            a = angle = atan2(p.y,p.x);  length = sqrt(p.x*p.x+p.y*p.y);
            if (csystem == 1 && xentry->getlocked() || csystem == 2 && yentry->getlocked())
              {//  Allow a multiple of this length
                 if (sqrt(p1.x*p1.x+p1.y*p1.y) > 1.0E-10)
                   {  if (length > sqrt(p1.x*p1.x+p1.y*p1.y))
                        length = floor(length / sqrt(p1.x*p1.x+p1.y*p1.y) + 0.5) * sqrt(p1.x*p1.x+p1.y*p1.y);
                      else
                        length = sqrt(p1.x*p1.x+p1.y*p1.y);
                   }
              }
            if (csystem == 1 && yentry->getlocked() || csystem == 2 && xentry->getlocked())
              {  angle = atan2(p1.y,p1.x);
                 //  Allow a multiple of this angle
                 if (angle == 0.0)
                   angle = a;
                 else
                   angle = floor(a/angle + 0.5) * angle;
              }
            if (zentry->getlocked())
              p.z = p1.z;
            p.x = cos(angle) * length;
            p.y = sin(angle) * length;
            p += lastp;
            break;
#if MAJOR_VERSION > 7
            // Enhancement u9 for v8
          case 3 :
           p -= lastp;
           if(xentry->getlocked())
           {
               p.x = p1.x; p.y = p1.y;
           }
           else
               p += lastp;
           if (zentry->getlocked())
             p.z = p1.z;
          break;
           //
#endif
         }
       p = db.workplanes.getcurrent()->workplanetomodel(p);
    }
  else if (cadwindow->getshiftstate() != 0 && db.workplanes.getcurrent() != 0)
    {  if (fabs(angle = v.getreal("ln::autosnapangle") * M_PI / 180.0) > 1.0E-20)
         {  p = db.workplanes.getcurrent()->modeltoworkplane(p);
            lastp = db.workplanes.getcurrent()->modeltoworkplane(state.getposition());
            p1 = p - lastp;
            if ((l = sqrt(p1.x*p1.x + p1.y*p1.y)) > db.getptoler())
              {  p1.x /= l;  p1.y /= l;
                 if (p1.x > 1.0) p1.x = 1.0;  else if (p1.x < -1.0) p1.x = -1.0;
                 a = acos(p1.x);
                 if (p1.y < 0.0) a = 2.0 * M_PI - a;
                 if (fabs(angle) > 1.0E-20)
                   a = floor(a / angle + 0.5) * angle;

                 p = Line(lastp,lastp + Point(l * cos(a),l * sin(a),0.0)).nearp(p);

                   //  p1 is the position of the current keyboard entry
                   switch (csystem)
                     {case 0 :
                        if (xentry->getlocked())
                          p.x = p1.x;
                        if (yentry->getlocked())
                          p.y = p1.y;
                        if (zentry->getlocked())
                          p.z = p1.z;
                        break;
                      case 1 :
                      case 2 :
                        p -= lastp;
                        p1 -= lastp;
                        a = angle = atan2(p.y,p.x);  length = sqrt(p.x*p.x+p.y*p.y);
                        if (csystem == 1 && xentry->getlocked() || csystem == 2 && yentry->getlocked())
                          {//  Allow a multiple of this length
                             if (sqrt(p1.x*p1.x+p1.y*p1.y) > 1.0E-10)
                               {  if (length > sqrt(p1.x*p1.x+p1.y*p1.y))
                                    length = floor(length / sqrt(p1.x*p1.x+p1.y*p1.y) + 0.5) * sqrt(p1.x*p1.x+p1.y*p1.y);
                                  else
                                    length = sqrt(p1.x*p1.x+p1.y*p1.y);
                               }
                          }
                        if (csystem == 1 && yentry->getlocked() || csystem == 2 && xentry->getlocked())
                          {  angle = atan2(p1.y,p1.x);
                             //  Allow a multiple of this angle
                             if (angle == 0.0)
                               angle = a;
                             else
                               angle = floor(a/angle + 0.5) * angle;
                          }
                        if (zentry->getlocked())
                          p.z = p1.z;
                        p.x = cos(angle) * length;
                        p.y = sin(angle) * length;
                        p += lastp;
                        break;
#if MAJOR_VERSION > 7
                        // Enhancement u9 for v8
                   case 3:
                       ok = xentry->get(0.0,&p1.x) &&
                            yentry->get(0.0,&p1.y) &&
                            zentry->get(lastp.z,&p1.z);
                       if(ok)
                       {
                           double z = p1.z;
                           length = p1.x;
                           p -= lastp;
                           p1 -= lastp;
                           a = angle = atan2(p.y,p.x);
                           if (zentry->getlocked())
                                p.z = z;
                           p.x = cos(angle) * length;
                           p.y = sin(angle) * length;
                           p += lastp;
                       }
                       break;
                       //
#endif
                     }
                 p = db.workplanes.getcurrent()->workplanetomodel(p);
              }
         }
    }

  return p;
}

void CoordinateEntry::paintEvent(QPaintEvent *)
{
    draw(DM_NORMAL);
}

void CoordinateEntry::draw(int drawmode)
{
#ifdef USING_WIDGETS

    xentry->m_qwEdit->update();
    xentry->prompt->draw(drawmode);

    yentry->m_qwEdit->update();
    yentry->prompt->draw(drawmode);

    zentry->m_qwEdit->update();
    zentry->prompt->draw(drawmode);

#else
  xentry->prompt->draw(drawmode);
  yentry->prompt->draw(drawmode);
  zentry->prompt->draw(drawmode);
  RedrawWindow(xentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(yentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(zentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(xentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(yentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(zentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);

  Control::drawborder();
#endif
}

void CoordinateEntry::move(Window *win,int x1,int y1,int width)
{
  x = xpos = x1;
  y = ypos = y1;
  w = width;
  hDC = win->gethdc();
  if (parent != 0)
    parent->delcontrol(this);
  win->addcontrol(this);
#ifdef USING_WIDGETS
  parent = win;
  xentry->move(win,1+x,y+2,width/3-25,MainWindow::getcoordheight()-5);
  yentry->move(win,1+x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
  zentry->move(win,1+x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
#else
  parent = win;
  xentry->move(win,1+x,y+2,width/3-25,MainWindow::getcoordheight()-5);
  yentry->move(win,1+x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
  zentry->move(win,1+x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
  RedrawWindow(xentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT);
  RedrawWindow(yentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT);
  RedrawWindow(zentry->window->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT);
  RedrawWindow(xentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(yentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
  RedrawWindow(zentry->fixwindow->gethwnd(),NULL,NULL,RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
#endif
}

void CoordinateEntry::move(int dx,int dy)
{
    move(parent,x + dx,y + dy,w);
}

int CoordinateEntry::pick(int xp,int yp)
{
    return dragging || xp >= x - MenuHandleOffset && xp <= x + w - 1 && yp >= y && yp <= y + h - 1;
}

int CoordinateDisplay::pick(int xp,int yp)
{
    return dragging || xp >= x - MenuHandleOffset && xp <= x + w - 1 && yp >= y && yp <= y + h - 1;
}

int Prompt::pick(int xp,int yp)
{
    return dragging || xp >= x - MenuHandleOffset && xp <= x + w - 1 && yp >= y && yp <= y + h - 1;
}

void CoordinateEntry::mousemoveevent(int xp,int yp,int buts)
{
#ifdef USING_WIDGETS
    // NOTE. this is currenty called from RCCoordinateEntry::MousePressEvent
    // Not using mousemove event for this so lastbuttons is always 0
    // if we need to use with the MousemoveEvent this might change
    if (buts == 1 /*&& lastbuttons == 0*/)
    {
        QWidget *child=0;
        if(m_qw && (child=m_qw->childAt(xp,yp)) != 0)
        {
            if(qobject_cast<QLabel*>((QObject*)child) != 0)
            {
                if(child->objectName().contains("_X_Icon"))
                    xentry->setlocked(! xentry->getlocked());
                else if(child->objectName().contains("_Y_Icon") && csystem != 3)
                    yentry->setlocked(! yentry->getlocked());
                else if(child->objectName().contains("_Z_Icon"))
                    zentry->setlocked(! zentry->getlocked());
                else if(child->objectName().contains("_Prompt"))
#if MAJOR_VERSION > 7
                    // v8 enhancement u9 added csystem 3
                    setcsystem((csystem+1)%4);
#else
                    setcsystem((csystem+1)%3);
#endif
            }
            else if(qobject_cast<QLineEdit*>((QObject*)child) != 0)
            {
                child->setFocus();
            }
        }
    }

    //lastbuttons = buts;
#else

 POINT p1;
 MenuWindow *mw;
 RCHWND hwnd;
 char text[300];

  //qDebug() << "mousemoveevent";

  if (buts == 1 && lastbuttons == 0 && xp >= 0)
  {
    if (xp > x + 18 && xp < x + 36)
     xentry->setlocked(! xentry->getlocked());
    else if (xp > x + w / 3 + 18 && xp < x + w / 3 + 36)
     yentry->setlocked(! yentry->getlocked());
    else if (xp > x + w * 2 / 3 + 18 && xp < x + w * 2 / 3 + 36)
     zentry->setlocked(! zentry->getlocked());
    else
     setcsystem((csystem+1)%3);
  }

  lastbuttons = buts;

  switch(drag(xp,yp,0,0,buts))
    {case 1 : // docked
       if (parent != cadwindow)
         hwnd = parent->gethwnd();
       else
         hwnd = 0;
       move(cadwindow,xp + dragoffsetx,yp + dragoffsety,w);
       if (hwnd != 0)
         DestroyWindow(hwnd);
       parent = cadwindow;
       InvalidateRect(parent->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,0,0",xp + dragoffsetx,yp + dragoffsety,w);
       WritePrivateProfileString("Layout","CoordinateEntry",text,home.getinifilename());

       break;
     case 2 : // floating
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       p1.x = xp + dragoffsetx;  p1.y = yp + dragoffsety;
       if (buts != -1)
         ClientToScreen(cadwindow->gethwnd(),&p1);
       mw = new MenuWindow(cadwindow->getsubwindow(),p1.x,p1.y,w,h);
       move(mw,0,0,w);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       InvalidateRect(mw->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,%d,1",p1.x,p1.y,w,h);
       WritePrivateProfileString("Layout","CoordinateEntry",text,home.getinifilename());

       break;
    }
#endif
}

void CoordinateDisplay::mousemoveevent(int xp,int yp,int buts)
{POINT p1;
 MenuWindow *mw;
 char text[300];

  switch(drag(xp,yp,0,0,buts))
    {case 1 :
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       move(cadwindow,xp + dragoffsetx,yp + dragoffsety,w);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,0,0",xp + dragoffsetx,yp + dragoffsety,w);
       WritePrivateProfileString("Layout","CoordinateDisplay",text,home.getinifilename());

       break;
     case 2 :
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       p1.x = xp + dragoffsetx;  p1.y = yp + dragoffsety;
       if (buts != -1)
         ClientToScreen(cadwindow->gethwnd(),&p1);
       mw = new MenuWindow(cadwindow->getsubwindow(),p1.x,p1.y,w,h);
       move(mw,0,0,w);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       InvalidateRect(mw->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,%d,1",p1.x,p1.y,w,h);
       WritePrivateProfileString("Layout","CoordinateDisplay",text,home.getinifilename());

       break;
    }

}


StyleEntry::StyleEntry(Window *pw,int x1,int y1,int width)
{
  SIZE size;
  RCHFONT hfont;

  lastbuttons = 0;
  x = xpos = x1;
  y = ypos = y1;
  w = width;
  int tbHeight=0;
  int tbWidth=0;

  hDC = pw->gethdc();
  hfont = (RCHFONT)SelectObject(hDC,MainWindow::gethsfont());
  GetTextExtentPoint32(hDC,_RCT("A"),1,&size);
  h = size.cy+10;
  SelectObject(hDC,hfont);
  h = MainWindow::getcoordheight();
  //h = 21;

  parent = pw;
#ifdef USING_WIDGETS
  //QSize iconSize(32,32);
  QSize iconSize(h-2,h-2);
  //QToolBar *ToolBar = new QToolBar("Style Entry",(QWidget*)pw->gethdc());
  RCStyleEntryToolBar *ToolBar = new RCStyleEntryToolBar("Style Entry",(QWidget*)pw->gethdc());
  ToolBar->setObjectName("RCStyleEntry");
  ToolBar->setIconSize(iconSize);
  ToolBar->setMinimumHeight(h+6);
  ToolBar->setMaximumHeight(h+6);

  entitybutton = new Button(x+4,y,_RCT("Entity"),3105,ToolBar);
  //entitybutton = new Button(x+4,y,_RCT("Entity"),3107,ToolBar);
  //entitybutton = new Button(x+4,y,_RCT("Entity"),3104,ToolBar);
  entitybutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)entitybutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)entitybutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)entitybutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)entitybutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = ((RCStyleButton*)entitybutton->getQW())->height();
  tbWidth  = ((RCStyleButton*)entitybutton->getQW())->width();

  //colourbutton = new Button(x+4,y,_RCT("Colour"),0,ToolBar);
  colourbutton = new Button(x+4,y,_RCT("Colour"),3105,ToolBar);
  //colourbutton = new Button(x+4,y,_RCT("Colour"),3107,ToolBar);
  //colourbutton = new Button(x+4,y,_RCT("Colour"),3100,ToolBar);
  //colourbutton = new Button(x+4,y,_RCT("Colour"),3000,ToolBar);
  colourbutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)colourbutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)colourbutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)colourbutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)colourbutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)colourbutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)colourbutton->getQW())->width();

  layerbutton = new Button(x+74,y,_RCT("Layer"),3105,ToolBar);
  //layerbutton = new Button(x+74,y,_RCT("Layer"),3101,ToolBar);
  //layerbutton = new Button(x+74,y,_RCT("Layer"),3001,ToolBar);
  layerbutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)layerbutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)layerbutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)layerbutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)layerbutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)layerbutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)layerbutton->getQW())->width();

  stylebutton = new Button(x+194,y,_RCT("Style"),3105,ToolBar);
  //stylebutton = new Button(x+194,y,_RCT("Style"),3102,ToolBar);
  //stylebutton = new Button(x+194,y,_RCT("Style"),3002,ToolBar);
  stylebutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)stylebutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)stylebutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)stylebutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)stylebutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)stylebutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)stylebutton->getQW())->width();

  weightbutton = new Button(x+264,y,_RCT("Weight"),3105,ToolBar);
  //weightbutton = new Button(x+264,y,_RCT("Weight"),3103,ToolBar);
  //weightbutton = new Button(x+264,y,_RCT("Weight"),3003,ToolBar);
  weightbutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)weightbutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)weightbutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)weightbutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)weightbutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)weightbutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)weightbutton->getQW())->width();

  typebutton = new Button(x+334,y,_RCT("StyleSet"),3004,ToolBar);
  typebutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)typebutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)typebutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)typebutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)typebutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)typebutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)typebutton->getQW())->width();

  // line weight check box
  ToolBar->mWeightCB = new QCheckBox(ToolBar);
  ToolBar->mWeightCB->setObjectName(QStringLiteral("RClineWeightDisplayCheckBox"));
  ToolBar->mWeightCB->setEnabled(true);
  ToolBar->mWeightCB->setMinimumHeight(h-8);
  ToolBar->mWeightCB->setMaximumHeight(h-8);
  ToolBar->mWeightCB->setMinimumWidth(h-8);
  ToolBar->mWeightCB->setMaximumWidth(h-8);
  ToolBar->mWeightCB->setLayoutDirection(Qt::LeftToRight);
  ToolBar->mWeightCB->setCheckable(true);
  ToolBar->mWeightCB->setChecked((db.getlineweightdisplay() == 1 ? true : false));
  ToolBar->mWeightCB->setTristate(false);
  ToolBar->mWeightCB->setStyleSheet("margin-left: 2px");

  clonebutton = new Button(x+580,y,_RCT("Clone"),216,ToolBar);
  clonebutton->getQW()->setObjectName("RCToolButton");
  ((RCStyleButton*)clonebutton->getQW())->setMaximumHeight(iconSize.height());
  ((RCStyleButton*)clonebutton->getQW())->setMinimumHeight(iconSize.height());
  ((RCStyleButton*)clonebutton->getQW())->setMaximumWidth(iconSize.width());
  ((RCStyleButton*)clonebutton->getQW())->setMinimumWidth(iconSize.width());
  tbHeight = max(tbHeight,((RCStyleButton*)clonebutton->getQW())->height());
  tbWidth  += ((RCStyleButton*)clonebutton->getQW())->width();

#else
  colourbutton = new Button(x+4,y,_RCT("Colour"),3000);
  layerbutton = new Button(x+74,y,_RCT("Layer"),3001);
  stylebutton = new Button(x+194,y,_RCT("Style"),3002);
  weightbutton = new Button(x+264,y,_RCT("Weight"),3003);
  typebutton = new Button(x+334,y,_RCT("StyleSet"),3004);
  clonebutton = new Button(x+580,y,_RCT("Clone"),216);
#endif
  pw->addcontrol(colourbutton);
  pw->addcontrol(layerbutton);
  pw->addcontrol(stylebutton);
  pw->addcontrol(weightbutton);
  pw->addcontrol(typebutton);
  pw->addcontrol(clonebutton);

  colourbutton->fixup(pw);  colourbutton->parent = pw;
  layerbutton->fixup(pw);  layerbutton->parent = pw;
  stylebutton->fixup(pw);  stylebutton->parent = pw;
  weightbutton->fixup(pw);  weightbutton->parent = pw;
  typebutton->fixup(pw);  typebutton->parent = pw;
  clonebutton->fixup(pw);  clonebutton->parent = pw;

#ifdef USING_WIDGETS
  colourwindow = new Window(_RCT("EDIT"),ToolBar,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+30,y+1,30,h-2,SW_SHOW);
  layerwindow = new Window(_RCT("EDIT"),ToolBar,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+100,y+1,80,h-2,SW_SHOW);
  stylewindow = new Window(_RCT("EDIT"),ToolBar,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+220,y+1,30,h-2,SW_SHOW);
  weightwindow = new Window(_RCT("EDIT"),ToolBar,WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+290,y+1,30,h-2,SW_SHOW);
  //typewindow = new Window(_RCT("COMBOBOX"),ToolBar,WS_CHILD | WS_BORDER | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL,
  //                                   x+360,y+1,100,h-2+200,SW_SHOW);
  typeCombo = new QComboBox(ToolBar);

  // add the controls to the toolbar
  ToolBar->addWidget(entitybutton->getQW());
  ToolBar->connect((RCToolButton*)entitybutton->getQW(),SIGNAL(pressed()),ToolBar,SLOT(entityButtonPressed()));

  ToolBar->addWidget(colourbutton->getQW());
  ToolBar->connect((RCToolButton*)colourbutton->getQW(),SIGNAL(pressed()),ToolBar,SLOT(colourButtonPressed()));
  ((RCToolButton*)colourbutton->getQW())->raise();

  ToolBar->addWidget((QWidget*)colourwindow->gethwnd());
  //ToolBar->connect((QLineEdit*)colourwindow->gethwnd(),SIGNAL(textEdited(QString)),ToolBar,SLOT(colourTextEdited(QString)));
  ToolBar->connect((QLineEdit*)colourwindow->gethwnd(),SIGNAL(editingFinished()),ToolBar,SLOT(colourTextEditFinished()));

  // initialise the edit control
  ((QLineEdit*)colourwindow->gethwnd())->setValidator(new RCColourEditValidator((QLineEdit*)colourwindow->gethwnd()));
  ((QLineEdit*)colourwindow->gethwnd())->setFocusPolicy(Qt::ClickFocus);
  ((QLineEdit*)colourwindow->gethwnd())->setMinimumHeight(h-6);
  ((QLineEdit*)colourwindow->gethwnd())->setMaximumHeight(h-6);
  ((QLineEdit*)colourwindow->gethwnd())->setMinimumWidth(size.cx*4+2);
  ((QLineEdit*)colourwindow->gethwnd())->setMaximumWidth(size.cx*4+2);
  ((QLineEdit*)colourwindow->gethwnd())->setTextMargins(1,1,1,1);
  QString colourText = QString("%1").arg(db.header.getcolour(),-3);
  ((QLineEdit*)colourwindow->gethwnd())->setText(colourText);
  tbHeight = max(tbHeight,((QLineEdit*)colourwindow->gethwnd())->height());
  tbWidth  += ((QLineEdit*)colourwindow->gethwnd())->width();

  ToolBar->addWidget(layerbutton->getQW());
  ((QLineEdit*)layerwindow->gethwnd())->raise();

  ToolBar->connect((RCToolButton*)layerbutton->getQW(),SIGNAL(pressed()),ToolBar,SLOT(layerButtonPressed()));
  ToolBar->addWidget((QWidget*)layerwindow->gethwnd());
  //ToolBar->connect((QLineEdit*)layerwindow->gethwnd(),SIGNAL(textEdited(QString)),ToolBar,SLOT(layerTextEdited(QString)));
  ToolBar->connect((QLineEdit*)layerwindow->gethwnd(),SIGNAL(editingFinished()),ToolBar,SLOT(layerTextEditFinished()));
  // initialise the edit control
  ((QLineEdit*)layerwindow->gethwnd())->setFocusPolicy(Qt::ClickFocus);
  ((QLineEdit*)layerwindow->gethwnd())->setMinimumHeight(h-6);
  ((QLineEdit*)layerwindow->gethwnd())->setMaximumHeight(h-6);
  ((QLineEdit*)layerwindow->gethwnd())->setMinimumWidth(size.cx*25+2);
  ((QLineEdit*)layerwindow->gethwnd())->setMaximumWidth(size.cx*25+2);
  ((QLineEdit*)layerwindow->gethwnd())->setTextMargins(1,1,1,1);
  ((QLineEdit*)layerwindow->gethwnd())->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  QString layerText = QString(db.layernames.getlayername(db.header.getlayer()));
  ((QLineEdit*)layerwindow->gethwnd())->setText(layerText);
  tbHeight = max(tbHeight,((QLineEdit*)layerwindow->gethwnd())->height());
  tbWidth  += ((QLineEdit*)layerwindow->gethwnd())->width();

  ToolBar->addWidget(stylebutton->getQW());
  ((QLineEdit*)stylewindow->gethwnd())->raise();

  ToolBar->connect((RCToolButton*)stylebutton->getQW(),SIGNAL(pressed()),ToolBar,SLOT(styleButtonPressed()));
  ToolBar->addWidget((QWidget*)stylewindow->gethwnd());

  //ToolBar->connect((QLineEdit*)stylewindow->gethwnd(),SIGNAL(textEdited(QString)),ToolBar,SLOT(styleTextEdited(QString)));
  ToolBar->connect((QLineEdit*)stylewindow->gethwnd(),SIGNAL(editingFinished()),ToolBar,SLOT(styleTextEditFinished()));
  // initialise the edit control
  ((QLineEdit*)stylewindow->gethwnd())->setValidator(new RCStyleEditValidator((QLineEdit*)stylewindow->gethwnd()));
  ((QLineEdit*)stylewindow->gethwnd())->setFocusPolicy(Qt::ClickFocus);
  ((QLineEdit*)stylewindow->gethwnd())->setMinimumHeight(h-6);
  ((QLineEdit*)stylewindow->gethwnd())->setMaximumHeight(h-6);
  ((QLineEdit*)stylewindow->gethwnd())->setMinimumWidth(size.cx*4+2);
  ((QLineEdit*)stylewindow->gethwnd())->setMaximumWidth(size.cx*4+2);
  ((QLineEdit*)stylewindow->gethwnd())->setTextMargins(2,2,2,2);
  QString styleText = QString("%1").arg(db.header.getstyle(),3);
  ((QLineEdit*)stylewindow->gethwnd())->setText(styleText);
  tbHeight = max(tbHeight,((QLineEdit*)stylewindow->gethwnd())->height());
  tbWidth  += ((QLineEdit*)stylewindow->gethwnd())->width();
  //qDebug() << "stylewindow width : " << ((QLineEdit*)stylewindow->gethwnd())->width();

  ToolBar->addWidget(weightbutton->getQW());
  ((QLineEdit*)weightwindow->gethwnd())->raise();
  ToolBar->connect((RCToolButton*)weightbutton->getQW(),SIGNAL(pressed()),ToolBar,SLOT(weightButtonPressed()));

  ToolBar->addWidget(ToolBar->mWeightCB);
  ToolBar->connect(ToolBar->mWeightCB,SIGNAL(toggled(bool)),ToolBar,SLOT(styleDisplayChanged(bool)));

  ToolBar->addWidget((QWidget*)weightwindow->gethwnd());
  //ToolBar->connect((QLineEdit*)weightwindow->gethwnd(),SIGNAL(textEdited(QString)),ToolBar,SLOT(weightTextEdited(QString)));
  ToolBar->connect((QLineEdit*)weightwindow->gethwnd(),SIGNAL(editingFinished()),ToolBar,SLOT(weightTextEditFinished()));
  // initialise the edit control
  ((QLineEdit*)weightwindow->gethwnd())->setValidator(new RCWeightEditValidator((QLineEdit*)weightwindow->gethwnd()));
  ((QLineEdit*)weightwindow->gethwnd())->setFocusPolicy(Qt::ClickFocus);
  ((QLineEdit*)weightwindow->gethwnd())->setMinimumHeight(h-6);
  ((QLineEdit*)weightwindow->gethwnd())->setMaximumHeight(h-6);
  ((QLineEdit*)weightwindow->gethwnd())->setMinimumWidth(size.cx*4+2);
  ((QLineEdit*)weightwindow->gethwnd())->setMaximumWidth(size.cx*4+2);
  ((QLineEdit*)weightwindow->gethwnd())->setTextMargins(2,2,2,2);
  QString weightText = QString("%1").arg(db.lineweights.weighttomm(v.getinteger("df::weight")),0,'f',2,'0');
  ((QLineEdit*)weightwindow->gethwnd())->setText(weightText);
  tbHeight = max(tbHeight,((QLineEdit*)weightwindow->gethwnd())->height());
  tbWidth  += ((QLineEdit*)weightwindow->gethwnd())->width();
  //qDebug() << "weightwindow width : " << ((QLineEdit*)weightwindow->gethwnd())->width();

  ToolBar->addWidget(typebutton->getQW());
  //ToolBar->addWidget((QWidget*)typewindow->gethwnd());
  ToolBar->addWidget(typeCombo);
  //((QComboBox*)typewindow->gethwnd())->raise();

  //ToolBar->connect((QComboBox*)typewindow->gethwnd(),SIGNAL(currentIndexChanged(int)),ToolBar,SLOT(styleSetIndexChanged(int)));
  //ToolBar->connect((QComboBox*)typewindow->gethwnd(),SIGNAL(activated(int)),ToolBar,SLOT(styleSetIndexChanged(int)));
  ToolBar->connect(typeCombo,SIGNAL(activated(int)),ToolBar,SLOT(styleSetIndexChanged(int)));
  // initialise the combo box control
  typeCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  typeCombo->setMinimumHeight(h-6);
  typeCombo->setMaximumHeight(h-6);
  typeCombo->setMinimumWidth(size.cx*25);
  typeCombo->setMaximumWidth(size.cx*25);
  tbWidth  += typeCombo->width();
  //qDebug() << "typewindow width : " << ((QComboBox*)typewindow->gethwnd())->width();

  // add the current style display widget
  // this only exists in the Qt version the old RealCAD painted it on to it's toolbar
  RCstyleIndicator *Indicator = new RCstyleIndicator(ToolBar);
  if(Indicator)
  {
      Indicator->setObjectName("RCStyleIndicator");
      ToolBar->addWidget(Indicator);
      Indicator->setMinimumHeight(h-8);
      Indicator->setMaximumHeight(h-8);
      Indicator->setMinimumWidth(150);
      Indicator->setMaximumWidth(150);
      m_ind = Indicator;
      m_ind->update();
      tbWidth += Indicator->width();
  }

  ToolBar->addWidget(clonebutton->getQW());

  ToolBar->setMaximumHeight(h+6);
  ToolBar->setMinimumHeight(h+6);
  ToolBar->setStyleSheet("spacing: 4px");

  ToolBar->setMinimumWidth(tbWidth/4);
  //ToolBar->setMaximumWidth(tbWidth+4*15);

  ToolBar->setTabOrder((RCToolButton*)entitybutton->getQW(),(RCToolButton*)colourbutton->getQW());
  ToolBar->setTabOrder((RCToolButton*)colourbutton->getQW(),(QLineEdit*)colourwindow->gethwnd());
  ToolBar->setTabOrder((QLineEdit*)colourwindow->gethwnd(),(RCToolButton*)layerbutton->getQW());
  ToolBar->setTabOrder((RCToolButton*)layerbutton->getQW(),(QLineEdit*)layerwindow->gethwnd());
  ToolBar->setTabOrder((QLineEdit*)layerwindow->gethwnd(),(RCToolButton*)stylebutton->getQW());
  ToolBar->setTabOrder((RCToolButton*)stylebutton->getQW(),(QLineEdit*)stylewindow->gethwnd());
  ToolBar->setTabOrder((QLineEdit*)stylewindow->gethwnd(),(RCToolButton*)weightbutton->getQW());
  ToolBar->setTabOrder((RCToolButton*)weightbutton->getQW(),(QLineEdit*)weightwindow->gethwnd());
  ToolBar->setTabOrder((QLineEdit*)weightwindow->gethwnd(),(RCToolButton*)typebutton->getQW());
  //ToolBar->setTabOrder((RCToolButton*)typebutton->getQW(),(QComboBox*)typewindow->gethwnd());
  ToolBar->setTabOrder((RCToolButton*)typebutton->getQW(),typeCombo);
  //ToolBar->setTabOrder((QComboBox*)typewindow->gethwnd(),(RCToolButton*)clonebutton->getQW());
  ToolBar->setTabOrder(typeCombo,(RCToolButton*)clonebutton->getQW());

  m_qw = ToolBar;
  h = ToolBar->height();
  w = ToolBar->width();
  x = xpos = ToolBar->pos().x();
  y = ypos = ToolBar->pos().y();

#else
  colourwindow = new Window(_RCT("EDIT"),pw->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+30,y+1,30,h-2,SW_SHOW);
  layerwindow = new Window(_RCT("EDIT"),pw->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+100,y+1,80,h-2,SW_SHOW);
  stylewindow = new Window(_RCT("EDIT"),pw->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+220,y+1,30,h-2,SW_SHOW);
  weightwindow = new Window(_RCT("EDIT"),pw->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+290,y+1,30,h-2,SW_SHOW);
  typewindow = new Window(_RCT("COMBOBOX"),pw->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL,
                                     x+360,y+1,100,h-2+200,SW_SHOW);

  SendMessage(typewindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
  SendMessage(typewindow->gethwnd(),CB_SETDROPPEDWIDTH,200,0);
  SendMessage(typewindow->gethwnd(),CB_SETITEMHEIGHT,-1,200);

  SendMessage(colourwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
  SendMessage(layerwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
  SendMessage(stylewindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
  SendMessage(weightwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),MAKELPARAM(TRUE,0));
#endif

}

#ifdef USING_WIDGETS
 Button * StyleEntry::getButton(QString bname)
 {
     if(bname == "Weight")
         return weightbutton;
     else if(bname == "Colour")
         return colourbutton;
     else if(bname == "Layer")
         return layerbutton;
     else if(bname == "Style")
         return stylebutton;
     else if(bname == "Clone")
         return clonebutton;
     else if(bname == "StyleSet")
         return typebutton;
     else if(bname == "Entity")
         return entitybutton;
     else
         return 0;
 }
 void StyleEntry::selectButton(Button *button)
 {
     button->select();
 }
 void StyleEntry::deselectButton(Button *button)
 {
     button->deselect();
     ((RCToolButton*)button->getQW())->toggle();
     ((RCToolButton*)button->getQW())->setDown(false);
     ((RCToolButton*)button->getQW())->update();
 }
 /**
  * @brief StyleEntry::getFocusHwnd
  * @return
  *
  * confirms if the given widget is used in the styleEntry control
  */
 bool StyleEntry::containsWidget(QWidget *w)
 {
     if(w == weightbutton->getQW())
         return true;
     else if(w ==  colourbutton->getQW())
         return true;
     else if(w ==  layerbutton->getQW())
         return true;
     else if(w ==  stylebutton->getQW())
         return true;
     else if(w ==  clonebutton->getQW())
         return true;
     else if(w ==  typebutton->getQW())
         return true;
     else if(w ==  entitybutton->getQW())
         return true;
     else if(w ==  (QWidget*)colourwindow->gethwnd())
         return true;
     else if(w ==  (QWidget*)layerwindow->gethwnd())
         return true;
     else if(w ==  (QWidget*)stylewindow->gethwnd())
         return true;
     else if(w ==  (QWidget*)weightwindow->gethwnd())
         return true;
     //else if(w ==  (QWidget*)typewindow->gethwnd())
     else if(w ==  (QWidget*)typeCombo)
         return true;
     //
     return false;
 }

#endif

RCHWND StyleEntry::gettypehwnd(void)
{
    //return typewindow->gethwnd();
    return typeCombo;
}

void StyleEntry::fixup(Window *window)
{
#ifdef USING_WIDGETS
    m_qw->update();
#else
  if (window != 0)
    {  Control::fixup(window);

       //  Move the windows
       SetWindowPos(colourwindow->gethwnd(),HWND_TOP,x+30,y,30,h-2,SWP_DRAWFRAME);
       SetWindowPos(layerwindow->gethwnd(),HWND_TOP,x+100,y,80,h-2,SWP_DRAWFRAME);
       SetWindowPos(stylewindow->gethwnd(),HWND_TOP,x+220,y,30,h-2,SWP_DRAWFRAME);
       SetWindowPos(weightwindow->gethwnd(),HWND_TOP,x+290,y,30,h-2,SWP_DRAWFRAME);
       SetWindowPos(typewindow->gethwnd(),HWND_TOP,x+360,y,100,h-2,SWP_DRAWFRAME);
    }
  else
    {  DestroyWindow(colourwindow->gethwnd());
       DestroyWindow(layerwindow->gethwnd());
       DestroyWindow(stylewindow->gethwnd());
       DestroyWindow(weightwindow->gethwnd());
       DestroyWindow(typewindow->gethwnd());

       if (parent != 0)
         {  parent->delcontrol(colourbutton);
            parent->delcontrol(layerbutton);
            parent->delcontrol(stylebutton);
            parent->delcontrol(weightbutton);
            parent->delcontrol(typebutton);
            parent->delcontrol(clonebutton);
         }

       parent = 0;
       hDC = 0;



    }

  if (window != 0)
    {  colourbutton->fixup(window);
       layerbutton->fixup(window);
       stylebutton->fixup(window);
       weightbutton->fixup(window);
       typebutton->fixup(window);
       clonebutton->fixup(window);
    }

  colourbutton->parent = window;
  layerbutton->parent = window;
  stylebutton->parent = window;
  weightbutton->parent = window;
  typebutton->parent = window;
  clonebutton->parent = window;

  colourbutton->x = colourbutton->xpos = x + 4;
  colourbutton->y = colourbutton->ypos = y;
  layerbutton->x = layerbutton->xpos = x + 74;
  layerbutton->y = layerbutton->ypos = y;
  stylebutton->x = stylebutton->xpos = x + 194;
  stylebutton->y = stylebutton->ypos = y;
  weightbutton->x = weightbutton->xpos = x + 264;
  weightbutton->y = weightbutton->ypos = y;
  typebutton->x = typebutton->xpos = x + 334;
  typebutton->y = typebutton->ypos = y;
  clonebutton->x = clonebutton->xpos = x + 580;
  clonebutton->y = clonebutton->ypos = y;
#endif


}


StyleEntry::~StyleEntry()
{
    delete m_ind; m_ind =0; // the style indicator widget
    delete m_qw; m_qw=0; // the toolbar widget
}

void StyleEntry::move(Window *win,int x1,int y1,int width)
{
#ifdef USING_WIDGETS
    if (cadwindow != 0)
      cadwindow->updatetitle();
#else
  x = xpos = x1;
  y = ypos = y1;
  if (width > 0)
    w = width;
  hDC = win->gethdc();
  if (parent != 0)
    {  parent->delcontrol(this);
       parent->delcontrol(colourbutton);
       parent->delcontrol(layerbutton);
       parent->delcontrol(stylebutton);
       parent->delcontrol(weightbutton);
       parent->delcontrol(typebutton);
       parent->delcontrol(clonebutton);
    }
  win->addcontrol(colourbutton);
  win->addcontrol(layerbutton);
  win->addcontrol(stylebutton);
  win->addcontrol(weightbutton);
  win->addcontrol(typebutton);
  win->addcontrol(clonebutton);
  win->addcontrol(this);

  parent = win;

  if (! IsWindow(colourwindow->gethwnd()))
    {
      delete colourwindow;
      colourwindow = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+30,y+1,30,h-2,SW_SHOW);
       PostMessage(colourwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),(LPARAM)MAKELPARAM(TRUE,0));
    }
  if (! IsWindow(layerwindow->gethwnd()))
    {
       delete layerwindow;
       layerwindow = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+100,y+1,80,h-2,SW_SHOW);
       PostMessage(layerwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),(LPARAM)MAKELPARAM(TRUE,0));
    }
  if (! IsWindow(stylewindow->gethwnd()))
    {  delete stylewindow;
       stylewindow = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+220,y+1,30,h-2,SW_SHOW);
       PostMessage(stylewindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),(LPARAM)MAKELPARAM(TRUE,0));
    }
  if (! IsWindow(weightwindow->gethwnd()))
    {  delete weightwindow;
       weightwindow = new Window(_RCT("EDIT"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL,
                                     x+290,y+1,30,h-2,SW_SHOW);
       PostMessage(weightwindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),(LPARAM)MAKELPARAM(TRUE,0));
    }
  if (! IsWindow(typewindow->gethwnd()))
    {  delete typewindow;
       typewindow = new Window(_RCT("COMBOBOX"),win->gethwnd(),WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL | CBS_DROPDOWNLIST | CBS_SORT,
                                     x+360,y+1,100,h-2+200,SW_SHOW);
       SendMessage(typewindow->gethwnd(),WM_SETFONT,(LPARAM)MainWindow::gethsfont(),(LPARAM)MAKELPARAM(TRUE,0));
       SendMessage(typewindow->gethwnd(),CB_SETDROPPEDWIDTH,(LPARAM)200,(LPARAM)0);
       SendMessage(typewindow->gethwnd(),CB_SETITEMHEIGHT,(LPARAM)-1,(LPARAM)200);

    }

  SetParent(colourwindow->gethwnd(),win->gethwnd());
  SetParent(layerwindow->gethwnd(),win->gethwnd());
  SetParent(stylewindow->gethwnd(),win->gethwnd());
  SetParent(weightwindow->gethwnd(),win->gethwnd());
  SetParent(typewindow->gethwnd(),win->gethwnd());

  SetWindowPos(colourwindow->gethwnd(),HWND_TOP,x+30,y,30,h-2,SWP_DRAWFRAME);
  SetWindowPos(layerwindow->gethwnd(),HWND_TOP,x+100,y,80,h-2,SWP_DRAWFRAME);
  SetWindowPos(stylewindow->gethwnd(),HWND_TOP,x+220,y,30,h-2,SWP_DRAWFRAME);
  SetWindowPos(weightwindow->gethwnd(),HWND_TOP,x+290,y,30,h-2,SWP_DRAWFRAME);
  SetWindowPos(typewindow->gethwnd(),HWND_TOP,x+360,y,100,h-2,SWP_DRAWFRAME);

  colourbutton->fixup(win);
  layerbutton->fixup(win);
  stylebutton->fixup(win);
  weightbutton->fixup(win);
  typebutton->fixup(win);
  clonebutton->fixup(win);

  colourbutton->parent = win;
  layerbutton->parent = win;
  stylebutton->parent = win;
  weightbutton->parent = win;
  typebutton->parent = win;
  clonebutton->parent = win;

  colourbutton->x = colourbutton->xpos = x + 4;
  colourbutton->y = colourbutton->ypos = y;
  layerbutton->x = layerbutton->xpos = x + 74;
  layerbutton->y = layerbutton->ypos = y;
  stylebutton->x = stylebutton->xpos = x + 194;
  stylebutton->y = stylebutton->ypos = y;
  weightbutton->x = weightbutton->xpos = x + 264;
  weightbutton->y = weightbutton->ypos = y;
  typebutton->x = typebutton->xpos = x + 334;
  typebutton->y = typebutton->ypos = y;
  clonebutton->x = clonebutton->xpos = x + 580;
  clonebutton->y = clonebutton->ypos = y;

  if (cadwindow != 0)
    cadwindow->updatetitle();
#endif
}

void StyleEntry::move(int dx,int dy)
{
    move(parent,xpos + dx,ypos + dy,w);
}

void StyleEntry::updatedefaults(void)
{
 int colour,layer,style,weight;
 RCCHAR text[300];
 RCCHAR rctext[300];

  colour = db.header.getcolour();
  layer = db.header.getlayer();
  style = db.header.getstyle();
  weight = db.header.getweight();

  if (IsWindow(colourwindow->gethwnd()))
  {
#ifdef USING_WIDGETS
       QString text = ((QLineEdit*)colourwindow->gethwnd())->text();
       if(text.toInt() != colour)
       {
           colour = text.toInt();
           db.header.change(colour,layer,style,weight);
           v.setinteger("df::colour",colour);
           cadwindow->updatetitle();
       }
#else
       GetWindowText(colourwindow->gethwnd(),text,300);
       if (atoi(text) != colour)
       {
            colour = atoi(text);
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::colour",colour);
            cadwindow->updatetitle();
       }
#endif
  }

  if (IsWindow(layerwindow->gethwnd()))
  {
#ifdef USING_WIDGETS
      QString text = ((QLineEdit*)layerwindow->gethwnd())->text();
      if(atoi(text.data()) != layer)
      {
          layer = atoi(text.data());
          db.header.change(colour,layer,style,weight);
          v.setinteger("df::layer",layer);
          cadwindow->updatetitle();
      }
  }
#else
  if (IsWindow(layerwindow->gethwnd()))
    {  GetWindowText(layerwindow->gethwnd(),text,300);
       if (atoi(text) != layer)
         {  layer = atoi(text);
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::layer",layer);
            cadwindow->updatetitle();
         }
    }
#endif

  if (IsWindow(stylewindow->gethwnd()))
  {
#ifdef USING_WIDGETS
      QString text = ((QLineEdit*)stylewindow->gethwnd())->text();
      if (text.toInt() != style)
      {
          style = text.toInt();
           db.header.change(colour,layer,style,weight);
           v.setinteger("df::style",style);
           cadwindow->updatetitle();
      }
#else
       GetWindowText(stylewindow->gethwnd(),text,300);
       if (atoi(text) != style)
       {
           style = atoi(text);
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::style",style);
            cadwindow->updatetitle();
       }
#endif
  }

  if (IsWindow(weightwindow->gethwnd()))
  {
       GetWindowText(weightwindow->gethwnd(),text,300);
       if (db.lineweights.mmtoweight(atof(text)) != weight)
       {
            weight = db.lineweights.mmtoweight(atof(text));
            db.header.change(colour,layer,style,weight);
            v.setinteger("df::weight",weight);
            cadwindow->updatetitle();
       }
   }
}

void StyleEntry::setlayer(int l)
{
#ifdef USING_WIDGETS
    if (IsWindow(layerwindow->gethwnd()))
    {
         QString text = QString("%1:").arg(l,4) + QString(db.layernames.getlayername(db.header.getlayer(),l));
         QString oldText = ((QLineEdit*)layerwindow->gethwnd())->text();
         if(text != oldText)
         {
             ((QLineEdit*)layerwindow->gethwnd())->setText(text);
             ((QLineEdit*)layerwindow->gethwnd())->setCursorPosition(0);
             ((QLineEdit*)layerwindow->gethwnd())->update();
         }
    }
#else
  TCHAR text[300],oldtext[300];

  if (IsWindow(layerwindow->gethwnd()))
  {
       sprintf(text,"%d:%ls",l,db.layernames.getlayername(db.header.getlayer(),1));
       GetWindowTextW(layerwindow->gethwnd(),(LPWSTR)oldtext,300);
       if (strcmp(oldtext,text) != 0)
           SetWindowTextW(layerwindow->gethwnd(),(LPCWSTR)text);

       GetWindowText(layerwindow->gethwnd(),oldtext,300);
       if (strcmp(oldtext,text) != 0)
         SetWindowText(layerwindow->gethwnd(),text);
  }
#endif
}

void StyleEntry::setstyle(int s)
{
#ifdef USING_WIDGETS
    if (IsWindow(stylewindow->gethwnd()))
    {
        QString oldText = ((QLineEdit*)stylewindow->gethwnd())->text();
        QString text = QString("%1").arg(s,3);
        if(text != oldText)
        {
            ((QLineEdit*)stylewindow->gethwnd())->setText(text);
            ((QLineEdit*)stylewindow->gethwnd())->update();
        }
    }
#else
  RCCHAR text[300],oldtext[300];

  if (IsWindow(stylewindow->gethwnd()))
    {  sprintf(text,"%d",s);
       GetWindowText(stylewindow->gethwnd(),oldtext,300);
       if (strcmp(oldtext,text) != 0)
         SetWindowText(stylewindow->gethwnd(),(RCLPCTSTR)text);
    }
#endif
}

void StyleEntry::setcolour(int c)
{
#ifdef USING_WIDGETS
    if (colourwindow->gethwnd())
    {
        QString oldText = ((QLineEdit*)colourwindow->gethwnd())->text();
        QString text = QString("%1").arg(c,3);
        if(text != oldText)
        {
            ((QLineEdit*)colourwindow->gethwnd())->setText(text);
            ((QLineEdit*)colourwindow->gethwnd())->update();
        }
     }
#else
  TCHAR text[300],oldtext[300];
  sprintf(text,"%d",c);
   GetWindowText(colourwindow->gethwnd(),oldtext,300);
   if (strcmp(oldtext,text) != 0)
     SetWindowText(colourwindow->gethwnd(),(RCLPCTSTR)text);
#endif
}

void StyleEntry::setweight(double d)
{
#ifdef USING_WIDGETS
    //qDebug() << "setweight : " << d;

  if (weightwindow->gethwnd())
  {
      QString oldText = ((QLineEdit*)weightwindow->gethwnd())->text();
      int w = db.lineweights.mmtoweight(d);
      QString text = QString("%1").arg(db.lineweights.weighttomm(w),0,'f',2,'0');
      if(text != oldText)
      {
        ((QLineEdit*)weightwindow->gethwnd())->setText(text);
        ((QLineEdit*)weightwindow->gethwnd())->update();
      }
  }
#else
    TCHAR text[300],oldtext[300];
    sprintf(text,"%.2lf",d);
    GetWindowText(weightwindow->gethwnd(),oldtext,300);
    if (strcmp(oldtext,text) != 0)
      SetWindowText(weightwindow->gethwnd(),(RCLPCTSTR)text);
#endif
}

void StyleEntry::settype(RCCHAR *t)
{
#ifdef USING_WIDGETS
    RCCHAR key[300],name[300],value[300],*s;
    int i;
	QStringList items;
    //qDebug() << "Entering :  settype";
    //qDebug() << "settype t : " << QString(t);

     //if (IsWindow(typewindow->gethwnd()))
     if (IsWindow(typeCombo))
       {
          //i = ((QComboBox*)typewindow->gethwnd())->findText(QString(t));
          i = typeCombo->findText(QString(t));
          //if (i != ((QComboBox*)typewindow->gethwnd())->currentIndex() || i < 0)
          if (i != typeCombo->currentIndex() || i < 0)
          {
               //((RCStyleEntryToolBar*)getQW())->disconnect((QComboBox*)typewindow->gethwnd(),
               //                                             SIGNAL(currentIndexChanged(int)),
               //                                             (RCStyleEntryToolBar*)getQW(),SLOT(styleSetIndexChanged(int)));
               //((QComboBox*)typewindow->gethwnd())->clear();
               typeCombo->clear();
               for (i = 1 ; ; i++)
                 {  sprintf(key,"StyleSet%d",i);
                    strcpy(name,"");
                    GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
                    if (strlen(value) == 0)
                      break;
                    s = strtok(value,_RCT(","));
                    strcpy(name,s);

                    //((QComboBox*)typewindow->gethwnd())->addItem(QString(name));
                   // typeCombo->addItem(QString(name));//fg.2022
					items.append(QString(name));
                 }
			   items.sort();
                 //((RCStyleEntryToolBar*)getQW())->connect((QComboBox*)typewindow->gethwnd(),
                 //                                           SIGNAL(currentIndexChanged(int)),
                 //                                           (RCStyleEntryToolBar*)getQW(),SLOT(styleSetIndexChanged(int)));
                 //i = ((QComboBox*)typewindow->gethwnd())->findText(QString(t));
			    for (QStringList::iterator it = items.begin(); it != items.end(); ++it)
					typeCombo->addItem(*it);
                 i = typeCombo->findText(QString(t));
                 //qDebug() << "setting Combo box to " << i << " " << QString(t);
                 //((QComboBox*)typewindow->gethwnd())->setCurrentIndex(i);
                 typeCombo->setCurrentIndex(i);
            }
       }
#else
 RCCHAR key[300],name[300],value[300],*s;
 int i;

  if (IsWindow(typewindow->gethwnd()))
    {
         i = SendMessage(typewindow->gethwnd(),CB_FINDSTRINGEXACT,0,(LPARAM)t);
         if (i != SendMessage(typewindow->gethwnd(),CB_GETCURSEL,i,0) || i == CB_ERR)
         {
            SendMessage(typewindow->gethwnd(),CB_RESETCONTENT,0,0);

            for (i = 1 ; ; i++)
              {  sprintf(key,"StyleSet%d",i);
                 strcpy(name,"");
                 GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
                 if (strlen(value) == 0)
                   break;
                 s = strtok(value,_RCT(","));
                 strcpy(name,s);

                 SendMessage(typewindow->gethwnd(),CB_ADDSTRING,0,(LPARAM)name);
              }

            i = SendMessage(typewindow->gethwnd(),CB_FINDSTRINGEXACT,0,(LPARAM)t);
            SendMessage(typewindow->gethwnd(),CB_SETCURSEL,i,0);
         }

    }
#endif
}

void StyleEntry::charevent(UINT,DWORD)
{
}

void StyleEntry::draw(int)
{
#ifdef USING_WIDGETS
    entitybutton->draw(DM_NORMAL);
    colourbutton->draw(DM_NORMAL);
    ((QLineEdit*)colourwindow->gethwnd())->update();
    layerbutton->draw(DM_NORMAL);
    ((QLineEdit*)layerwindow->gethwnd())->update();
    stylebutton->draw(DM_NORMAL);
    ((QLineEdit*)stylewindow->gethwnd())->update();
    weightbutton->draw(DM_NORMAL);
    ((QLineEdit*)weightwindow->gethwnd())->update();
    typebutton->draw(DM_NORMAL);
    //((QComboBox*)typewindow->gethwnd())->update();
    typeCombo->update();
    clonebutton->draw(DM_NORMAL);
    m_ind->update();
#else
    RECT rect;
 RCHBRUSH hbrush;
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 LineStyle *linestyle;

  if (parent == 0)
    return;

  View3dOutput output(parent->gethwnd(),hDC,&top,1.0);

  colourbutton->draw(DM_NORMAL);
  layerbutton->draw(DM_NORMAL);
  stylebutton->draw(DM_NORMAL);
  weightbutton->draw(DM_NORMAL);
  typebutton->draw(DM_NORMAL);
  clonebutton->draw(DM_NORMAL);

  Control::drawborder();

  rect.left = x + 470;
  rect.right = x + 570;
  rect.top = y + 1;
  rect.bottom = y + h - 1;

  hbrush = CreateSolidBrush(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,::v.getreal("wn::backgroundcolour.green")*255.0,::v.getreal("wn::backgroundcolour.blue")*255.0));
  FillRect(hDC,&rect,hbrush);
  DeleteObject(hbrush);

  /*
  IntersectClipRect(hDC,rect.left,rect.top,rect.right,rect.bottom);
  output.setup(1.0,0.0,DM_NORMAL,db.header.getcolour(),db.header.getstyle(),db.header.getweight(),0);
  if ((linestyle = db.linestyles.indextostyle(db.header.getstyle())) == NULL)
    {  output.moveto(x+472,parent->getheight() - (y + h/2));
       output.lineto(x+565,parent->getheight() - (y + h/2));
    }
  else
    {  linestyle->start(1.0,113);
       linestyle->draw(&output,x+452,y+h/2,x+565,y+h/2);
    }
  SelectClipRgn(hDC,0);
  */

  InvalidateRect(colourwindow->gethwnd(),0,TRUE);
  InvalidateRect(layerwindow->gethwnd(),0,TRUE);
  InvalidateRect(stylewindow->gethwnd(),0,TRUE);
  InvalidateRect(weightwindow->gethwnd(),0,TRUE);
  InvalidateRect(typewindow->gethwnd(),0,TRUE);

  SetBkMode(hDC,TRANSPARENT);
#endif
}

void StyleEntry::mousemoveevent(int xp,int yp,int buts)
{
#ifdef USING_WIDGETS
    //qDebug() << "Entering : StyleEntry::mousemoveevent";
#else
    POINT p1;
 MenuWindow *mw;
 char text[300];

  switch(drag(xp,yp,0,0,buts))
    {case 1 :
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       move(cadwindow,xp + dragoffsetx,yp + dragoffsety,w);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,0,0",xp + dragoffsetx,yp + dragoffsety,w);
       WritePrivateProfileString("Layout","StyleEntry",text,home.getinifilename());

       break;
     case 2 :
       if (parent != cadwindow && cadwindow != 0)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       p1.x = xp + dragoffsetx;  p1.y = yp + dragoffsety;
       if (buts != -1)
         ClientToScreen(cadwindow->gethwnd(),&p1);
       mw = new MenuWindow(cadwindow->getsubwindow(),p1.x,p1.y,w,h);
       move(mw,0,0,w);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       InvalidateRect(mw->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);
       sprintf(text,"%d,%d,%d,%d,1",p1.x,p1.y,w,h);
       WritePrivateProfileString("Layout","StyleEntry",text,home.getinifilename());

       break;
    }
#endif
}

int StyleEntry::pick(int xp,int yp)
{
    return dragging || xp >= x - MenuHandleOffset && xp <= x + w - 1 && yp >= y && yp <= y + h - 1;
}

void Prompt::move(int dx,int dy)
{
    move(cadwindow,xpos + dx,ypos + dy);
}

void Prompt::mousemoveevent(int xp,int yp,int buts)
{
 MenuWindow *mw;
 POINT p1;
 char text[300];

  switch(drag(xp,yp,0,0,buts))
    {case 1 :
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       move(cadwindow,xp+dragoffsetx,yp+dragoffsety);
       cadwindow->delcontrol(this);
       cadwindow->addcontrol(this);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,0,0,0",xp+dragoffsetx,yp+dragoffsety);
       if (this == cadwindow->statusprompt)
         WritePrivateProfileString("Layout","StatusPrompt",text,home.getinifilename());
       else
         WritePrivateProfileString("Layout","Prompt",text,home.getinifilename());

       break;
     case 2 :
       if (parent != cadwindow)
         {  DestroyWindow(parent->gethwnd());
            parent = 0;
         }
       p1.x = xp + dragoffsetx;  p1.y = yp + dragoffsety;
       if (buts != -1)
         ClientToScreen(cadwindow->gethwnd(),&p1);
       mw = new MenuWindow(cadwindow->getsubwindow(),p1.x,p1.y,w,h);
       mw->addcontrol(this);
       move(mw,0,0);
       cadwindow->delcontrol(this);
       InvalidateRect(cadwindow->gethwnd(),0,TRUE);
       InvalidateRect(mw->gethwnd(),0,TRUE);
       cadwindow->sizeevent(-1,-1);

       sprintf(text,"%d,%d,%d,%d,1",p1.x,p1.y,w,h);
       if (this == cadwindow->statusprompt)
         WritePrivateProfileString("Layout","StatusPrompt",text,home.getinifilename());
       else
         WritePrivateProfileString("Layout","Prompt",text,home.getinifilename());

       break;
    }
}

bool CoordinateEntry::containsWidget(QWidget *w)
{
    if(w == xentry->get_QWedit())
        return true;
    if(w == xentry->get_QWIcon())
        return true;
    if(w == xentry->get_QWprompt())
        return true;

    if(w == yentry->get_QWedit())
        return true;
    if(w == yentry->get_QWIcon())
        return true;
    if(w == yentry->get_QWprompt())
        return true;

    if(w == zentry->get_QWedit())
        return true;
    if(w == zentry->get_QWIcon())
        return true;
    if(w == zentry->get_QWprompt())
        return true;

    return false;
}

void CoordinateEntry::setcsystem(int cs)
{
    if(cs !=3)
    {
        yentry->setenabled(true);
        yentry->setlocked(0);
        yentry->enablelock(true);
    }

  switch (cs)
    {  case 0 :
         csystem = cs;
         xentry->name = 'X';
         yentry->name = 'Y';
         zentry->name = 'Z';
         break;
       case 1 :
         csystem = cs;
         xentry->name = 'D';
         yentry->name = 'A';
         zentry->name = 'Z';
         break;
       case 2 :
         csystem = cs;
         xentry->name = 'B';
         yentry->name = 'L';
         zentry->name = 'Z';
         break;
#if MAJOR_VERSION > 7
         // Enhamcement u9 for v8
      case 3 :
        csystem = cs;
        xentry->name = '/';
        yentry->name = ' ';
        yentry->set(0.0);
        yentry->setenabled(false);
        yentry->setlocked(1);
        yentry->enablelock(false);
        zentry->name = 'Z';
        break;
       default: // just updating the incremental
         break;
#endif
    }
  if (v.getinteger("db::incrementalcoordinates") == 1 && cs != 3)
    {  if (newcoordinate)
         {  xentry->set(0.0);
            yentry->set(0.0);
            zentry->set(0.0);
         }
       newcoordinate = 0;
       xentry->incremental = 1;
       yentry->incremental = 1;
       zentry->incremental = 1;
       xentry->toggle();  xentry->toggle();
       yentry->toggle();  yentry->toggle();
       zentry->toggle();  zentry->toggle();
    }
  else
    {
       xentry->incremental = 0;
       yentry->incremental = 0;
       zentry->incremental = 0;
       xentry->toggle();  xentry->toggle();
       yentry->toggle();  yentry->toggle();
       zentry->toggle();  zentry->toggle();
    }

  draw(DM_NORMAL);
}

void CoordinateEntry::setEntryFocus(RCCHAR *name, bool onoff)
{
    QString text(name);
    POINT p;
    GetCursorPos(&p);
    QPoint gqp(p.x,p.y);
    if(text.contains("X"))
    {
        if(onoff)
        {
            ((QLineEdit*)xentry->get_QWedit())->setFocus();
        }
        else
        {
            ((QLineEdit*)xentry->get_QWedit())->clearFocus();
            ((QLineEdit*)xentry->get_QWedit())->deselect();
        }
    }
    if(text.contains("Y"))
    {
        if(onoff)
        {
            ((QLineEdit*)yentry->get_QWedit())->setFocus();
        }
        else
        {
            ((QLineEdit*)yentry->get_QWedit())->clearFocus();
            ((QLineEdit*)yentry->get_QWedit())->deselect();
        }
    }
    if(text.contains("Z"))
    {
        if(onoff)
        {
            ((QLineEdit*)zentry->get_QWedit())->setFocus();
        }
        else
        {
            ((QLineEdit*)zentry->get_QWedit())->clearFocus();
            ((QLineEdit*)zentry->get_QWedit())->deselect();
        }
    }
    if(text.contains("/"))
    {
        if(onoff)
        {
            ((QLineEdit*)xentry->get_QWedit())->setFocus();
        }
        else
        {
            ((QLineEdit*)xentry->get_QWedit())->deselect();
            ((QLineEdit*)xentry->get_QWedit())->clearFocus();
            ((QLineEdit*)yentry->get_QWedit())->setEnabled(true);
        }
    }
}

void CoordinateEntry::charevent(UINT c,DWORD)
{Point p,lastp;
 double r,a,x;
 int ok,focus;
 RCCHAR *eptr,*s,*dc;
 RCCHAR string[256];
 double bearing,direction;
 RCHWND hwnd;
 ResourceString rs7(NCWIN+7);
 ResourceString rs8(NCWIN+8);

  if ((c == 0x000D || c == 0x000A) && state.getcommandactive() == 0)
  //if ((c == '\r' || c == '\n') && state.getcommandactive() == 0)
  {  state.sendevent(new NewCommandEvent(state.getlastcommandid(),0,1));
       state.process();
       newcoordinate = 1;
       return ;
  }

  focus = 0;

  // Enhancement u9 for v8
#if MAJOR_VERSION > 7
  if(getcsystem() != 3)
  {
      xentry->setenabled(true);
      yentry->setenabled(true);
      zentry->setenabled(true);
      yentry->enablelock(true);
  }
#endif

  //qDebug() << "GetFocus() : " << (QWidget*)GetFocus();
  //qDebug() << "xentry->window->gethwnd() : " << (QWidget*)xentry->window->gethwnd();
  //qDebug() << "yentry->window->gethwnd() : " << (QWidget*)yentry->window->gethwnd();
  //qDebug() << "zentry->window->gethwnd() : " << (QWidget*)zentry->window->gethwnd();
  if (GetFocus() == xentry->window->gethwnd())
    focus = 1;
  else if (GetFocus() == yentry->window->gethwnd())
    focus = 2;
  else if (GetFocus() == zentry->window->gethwnd())
    focus = 3;

  if (focus != 0 && (c == 'P' || c == 'X' || c == 'x' || c == 'Y' || c == 'y' || c == 'Z' || c == 'z' || c == '\t' || c == '\r' || c == '\r'))
    {  strcpy(string,"1");
       if (focus == 1)
         GetWindowText(GetFocus(),string,256);
       else if (focus == 2)
         GetWindowText(GetFocus(),string,256);
       else if (focus == 3)
         GetWindowText(GetFocus(),string,256);
       strtod(string,&eptr);
       while (*eptr == ' ') eptr++;
       if (*eptr != 0)
         {
            s = string;
            _strlwr(string);
            bearing = 0.0;
            if (string[0] == 'n')
              {  bearing = 0.0;  s++;
              }
            else if (string[0] == 's')
              {  bearing = 180.0;  s++;
              }
            else if (string[0] == 'e')
              {  bearing = 90.0;  s++;
              }
            else if (string[0] == 'w')
              {  bearing = 270.0;  s++;
              }
            if (strlen(string) > 0)
              {  dc = string + strlen(string) - 1;
              }
            else
              dc = 0;

            direction = 1.0;
            if (dc != 0 && *dc == 'n')
              {  direction = bearing == 90.0 ? -1.0 : 1.0;
                 *dc = 0;
              }
            else if (dc != 0 && *dc == 'e')
              {  direction = bearing == 180.0 ? -1.0 : 1.0;
                 *dc = 0;
              }
            else if (dc != 0 && *dc == 's')
              {  direction = bearing == 270.0 ? -1.0 : 1.0;
                 *dc = 0;
              }
            else if (dc != 0 && *dc == 'w')
              {  direction = bearing == 0.0 ? -1.0 : 1.0;
                 *dc = 0;
              }

           Expression expression(s);
           if (expression.evaluate(&x) != 0)
             {  x = bearing + direction * x;
                if (csystem == 0 || csystem == 1 && focus != 2 || csystem == 2 && focus != 1)
                    db.formatnumber(string,x * db.getmainunitscale(),FormatForEditField);
                else
                    sprintf(string,"%.16g",x);
                SetWindowText(GetFocus(),(RCLPCTSTR)string);
             }
           else
             {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONSTOP);
                return ;
             }
         }
         else // this piece of code is not in V5
         {  Expression expression(string);
           if (expression.evaluate(&x) != 0)
             {  if (csystem == 0 || csystem == 1 && focus != 2 || csystem == 2 && focus != 1)
                    db.formatnumber(string,x * db.getmainunitscale(),FormatForEditField);
                else
                    sprintf(string,"%.16g",x);
                SetWindowText(GetFocus(),(RCLPCTSTR)string);
             }
           else
             {  cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONSTOP);
                return ;
             }
          }
     }

   if (focus == 1 && (c == 'P' || c == 'p'))
     xentry->setlocked(! xentry->getlocked());
   else if (focus == 2 && (c == 'P' || c == 'p'))
     yentry->setlocked(! yentry->getlocked());
   else if (focus == 3 && (c == 'P' || c == 'p'))
     zentry->setlocked(! zentry->getlocked());
   else if (c == 'X' || c == 'x')
     {  setcsystem(0);  xentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       yentry->setenabled(true);
       yentry->deselect();
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);
     }
   else if (c == 'Y' || c == 'y')
     {  setcsystem(0);  yentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       xentry->deselect();
       yentry->setenabled(true);
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);
     }
   else if (c == 'Z' || c == 'z')
     {  setcsystem(csystem);  zentry->select();
       if(getcsystem() != 3)
       {
           xentry->setenabled(true);
           xentry->setlocked(false);
           xentry->deselect();
           yentry->setenabled(true);
           yentry->deselect();
           zentry->setenabled(true);
           zentry->enablelock(true);
       }
     }
   else if (c == 'R' || c == 'r' || c == 'D' || c == 'd')
     {  setcsystem(1);  xentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       yentry->setenabled(true);
       yentry->deselect();
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);
     }
   else if (c == 'T' || c == 't' || c == 'A' || c == 'a')
     {  setcsystem(1);  yentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       xentry->deselect();
       yentry->setenabled(true);
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);

     }
   else if (c == 'B' || c == 'b')
     {  setcsystem(2);  xentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       yentry->setenabled(true);
       yentry->deselect();
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);

     }
   else if (c == 'L' || c == 'l')
     {  setcsystem(2);  yentry->select();
       xentry->setenabled(true);
       xentry->setlocked(false);
       xentry->deselect();
       yentry->setenabled(true);
       zentry->setenabled(true);
       zentry->deselect();
       yentry->enablelock(true);

     }
   else if (c == 'I' || c == 'i')
     {
       // Enhancement u9 for v7 csystem 3 was added
        if(getcsystem() != 3)
        {
            if      (GetFocus() == xentry->window->gethwnd()) xentry->toggle();
            else if (GetFocus() == yentry->window->gethwnd()) yentry->toggle();
            else if (GetFocus() == zentry->window->gethwnd()) zentry->toggle();
        }
     }
   else if (c == 'M' || c == 'm')
     {
       // Enhancement u9 for v7 csystem 3 was added
       if(getcsystem() != 3)
       {
            if(GetFocus() == xentry->window->gethwnd())
                xentry->select();
            else if(GetFocus() == yentry->window->gethwnd())
                yentry->select();
            else if(GetFocus() == zentry->window->gethwnd())
                zentry->select();
            xentry->toggle();  yentry->toggle();  zentry->toggle();

            xentry->setenabled(true);
            xentry->setlocked(false);
            yentry->setenabled(true);
            zentry->setenabled(true);
            yentry->enablelock(true);
       }
     }
   else if (c == '\t' || c == ' ')
     {
       //Switch to the next closest, or back to the closest entity to the location point
       View3dWindow *window;
       window = cadwindow->getcurrentwindow();
       if(window->getlastselectedentity())
       {
            if(!window->SwapCurrentSelectedEntity())
            {
                if (program->getlicensed() >= 2)
                {
                   if(GetFocus() == xentry->window->gethwnd())
                       yentry->select();
                   else
                       xentry->select();
                }
                else
                {
                    // Enhancement u9 for v7 csystem 3 was added
                    if(csystem != 3)
                    {
                        if (GetFocus() == xentry->window->gethwnd())
                        {
                            yentry->select();
                        }
                        else if (GetFocus() == yentry->window->gethwnd())
                        {
                            zentry->select();
                        }
                        else
                        {
                            xentry->select();
                        }
                    }
                    else
                    {
                        if (GetFocus() == xentry->window->gethwnd())
                            zentry->select();
                        else
                            xentry->select();
                    }
                }
            }
       }
       else
       {
           if (program->getlicensed() >= 2)
           {
              if(GetFocus() == xentry->window->gethwnd())
                  yentry->select();
              else
                  xentry->select();
           }
           else
           {
               // Enhancement u9 for v7 csystem 3 was added
               if(csystem != 3)
               {
                   if (GetFocus() == xentry->window->gethwnd())
                   {
                       yentry->select();
                   }
                   else if (GetFocus() == yentry->window->gethwnd())
                   {
                       zentry->select();
                   }
                   else
                   {
                       xentry->select();
                   }
               }
               else
               {
                   if (GetFocus() == xentry->window->gethwnd())
                       zentry->select();
                   else
                       xentry->select();
               }
            }
        }
   }
   else if (c == 'N' || c == 'n')
   {
       //Switch to the next closest, or back to the closest entity to the location point
       View3dWindow *window;
       window = cadwindow->getcurrentwindow();
       if(window->getlastselectedentity())
       {
            window->SwapCurrentSelectedEntity();
       }
   }
   else if (c == '\r')
     { newcoordinate = 1;
       // lastp is the previous cursor pick (LMB) position
       lastp = state.getposition();
       if (db.workplanes.getcurrent() != NULL)
         lastp = db.workplanes.getcurrent()->modeltoworkplane(lastp);
       switch (csystem)
         {  case 0 :
              ok = xentry->get(lastp.x,&p.x) &&
                   yentry->get(lastp.y,&p.y) &&
                   zentry->get(lastp.z,&p.z);
              break;
            case 1 :
              ok = xentry->get(0.0,&p.x) &&
                   yentry->get(0.0,&p.y) &&
                   zentry->get(lastp.z,&p.z);
              if (ok)
                {  r = p.x;  a = p.y / db.getmainunitscale() * M_PI / 180.0;
                   p.x = r * cos(a);  p.y = r * sin(a);
                   if (xentry->incremental || yentry->incremental)
                     {  p.x += lastp.x;  p.y += lastp.y;
                     }
                }
              break;
            case 2 :
              ok = xentry->get(0.0,&p.x) &&
                   yentry->get(0.0,&p.y) &&
                   zentry->get(lastp.z,&p.z);
              if (ok)
                {  r = p.y;  a = p.x / db.getmainunitscale() * M_PI / 180.0;
                   p.x = r * sin(a);  p.y = r * cos(a);
                   if (xentry->incremental || yentry->incremental)
                     {  p.x += lastp.x;  p.y += lastp.y;
                     }
                }
              break;
#if MAJOR_VERSION > 7
            // Enhancement u9 for v8
            case 3 :
              ok = xentry->get(lastp.x,&p.x) &&
                   yentry->get(lastp.y,&p.y) &&
                   zentry->get(lastp.z,&p.z);
              qDebug() << "lastp : " << lastp.x << "," << lastp.y << "," << lastp.z;
              qDebug() << "p : " << p.x << "," << p.y << "," << p.z;
              if(ok)
              {
                  //View3dWindow *window;
                  Point nextp = state.getposition();
                  QPoint qp = ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->view()->mapFromGlobal(QCursor::pos());
                  cadwindow->getcurrentwindow()->coordinate3d(qp.x(),qp.y(),&nextp);
                  if (db.workplanes.getcurrent() != NULL)
                    nextp = db.workplanes.getcurrent()->workplanetomodel(nextp);
                  double tempz = p.z;
                  p = lastp + (nextp - lastp).normalize() * p.x;
                  p.z = tempz;
                  qDebug() << "p : " << p.x << "," << p.y << "," << p.z;
              }
            break;
#endif
         }
       if (ok)
         {Point xaxis,yaxis;
          View3dWindow *window;
            if (db.workplanes.getcurrent() != NULL)
              p = db.workplanes.getcurrent()->workplanetomodel(p);
            if (cadwindow->getcurrentwindow() == NULL)
              {  if (db.workplanes.getcurrent() == NULL)
                   {  xaxis.setxyz(1.0,0.0,0.0);  yaxis.setxyz(0.0,1.0,0.0);
                   }
                 else
                   {  xaxis = db.workplanes.getcurrent()->getxaxis();
                      yaxis = db.workplanes.getcurrent()->getyaxis();
                   }
                 window = NULL;
              }
            else
              {  xaxis = cadwindow->getcurrentwindow()->getxaxis();
                 yaxis = cadwindow->getcurrentwindow()->getyaxis();
                 window = cadwindow->getcurrentwindow();
              }
            Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xaxis,yaxis,window,0);
            state.sendevent(cevent);
            cadwindow->updateAllViews();
            cadwindow->paintall();
            //app->processEvents();
         }
       else
         cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONSTOP);
   }
   else if (c == 'F' || c == 'f')
     state.fromposition();
   else if (c == '!')
   {
        if (GetFocus() == xentry->window->gethwnd() ||
            GetFocus() == yentry->window->gethwnd() ||
            GetFocus() == zentry->window->gethwnd())
        {
             hwnd = GetFocus();
             GetWindowText(hwnd,string,256);
             if (calculator.process(cadwindow->gethwnd(), string) == TRUE)
             {
                  SetWindowText(hwnd,(RCLPCTSTR)calculator.getreal());
                  SetFocus(hwnd);
             }
        }
   }
#if MAJOR_VERSION > 7
   // Enhancement u9 for v8 csystem 3 was added
   else if(c == '\/')
   {
       setcsystem(3);
       xentry->cleartoggle();
       yentry->cleartoggle();
       zentry->cleartoggle();
       xentry->setlocked(true);
       xentry->select();
       ((QLineEdit*)xentry->get_QWedit())->setFocus();
       ((QLineEdit*)xentry->get_QWedit())->setCursorPosition(0);
       xentry->select();
       ((QLineEdit*)yentry->get_QWedit())->clearFocus();
       ((QLineEdit*)yentry->get_QWedit())->deselect();
       yentry->setenabled(false);
       yentry->enablelock(false);
       ((QLineEdit*)zentry->get_QWedit())->clearFocus();
       ((QLineEdit*)zentry->get_QWedit())->deselect();
   }
#endif
   else if (c == VK_LEFT || c == VK_RIGHT || c == VK_UP || c == VK_DOWN)
   {
       //arrow key nudge
       View3dWindow *window;
       window = cadwindow->getcurrentwindow();
       EntityList list_selected = state.getselection().copy();
       RemovePartialGroupEntities(&list_selected,1);
       if(list_selected.length() > 0)
       {
            nudgelist(list_selected,c);
       }
   }
   bool allselected=false;
   if(focus == 1)
   {
       //bool allselected = xentry->isallselected();
       GetWindowText(xentry->window->gethwnd(),string,256);
       SetWindowText(xentry->window->gethwnd(),string);
       //if(allselected)
       //    xentry->select();
   }
   else if(focus == 2)
   {
       //bool allselected = yentry->isallselected();
       GetWindowText(yentry->window->gethwnd(),string,256);
       SetWindowText(yentry->window->gethwnd(),string);
       //if(allselected)
       //    yentry->select();
   }
   else if(focus == 3)
   {
       //bool allselected = zentry->isallselected();
       GetWindowText(zentry->window->gethwnd(),string,256);
       SetWindowText(zentry->window->gethwnd(),string);
       //if(allselected)
       //    zentry->select();
   }
}

//
//  The CoordinateDisplay class
//
#ifdef USING_WIDGETS
OneCoordinateDisplay::OneCoordinateDisplay(Window *win,int xp,int yp,int width,int height,int n,QWidget *qparent)
{
  RCCHAR message[300];
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;
  prompt = new Prompt(x+5,y+2,0,qparent);
  prompt->fixup(win);
  name = n;
  sprintf(message,"%c",name);
  prompt->normalprompt(message);

  // add the widgets pointers
  // initialize the coordinate label
  m_qwPrompt = (QWidget*)prompt->getQW();
  ((QLabel*)m_qwPrompt)->setFrameStyle(QFrame::NoFrame);
  ((QLabel*)m_qwPrompt)->setMinimumHeight(h);
  ((QLabel*)m_qwPrompt)->setMaximumHeight(h);
  ((QLabel*)m_qwPrompt)->setMinimumWidth(w);
  ((QLabel*)m_qwPrompt)->setMaximumWidth(h*22);
}
#else
OneCoordinateDisplay::OneCoordinateDisplay(Window *win,int xp,int yp,int width,int height,int n)
{
  RCCHAR message[300];
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;
  prompt = new Prompt(x+5,y+2,0);
  prompt->fixup(win);
  name = n;
  sprintf(message,"%c",name);
  prompt->normalprompt(message);
}
#endif

OneCoordinateDisplay::~OneCoordinateDisplay()
{  delete prompt;
}

void OneCoordinateDisplay::set(double value)
{
  RCCHAR mess[300],text[300];
  sprintf(mess,"%c%ls",name,db.formatnumber(text,value));
  prompt->normalprompt(mess);
}

void OneCoordinateDisplay::move(Window *win,int xp,int yp,int width,int height)
{
  prompt->draw(DM_ERASE);
  x = xpos = xp;  y = ypos = yp;
  w = width; h = height;
  prompt->move(win,x+5,y+2);
  prompt->draw(DM_NORMAL);
}

CoordinateDisplay::CoordinateDisplay(Window *pw,int x1,int y1,int width)
#ifdef DERIVE_FROM_WIDGETS
    : Control(_qparent)
#endif
{
#ifdef USING_WIDGETS
  QToolBar *ToolBar = new QToolBar("Coordinate Display",(QWidget*)pw->gethdc());
  ToolBar->setObjectName("RCCoordinateDisplay");
  xdisplay = new OneCoordinateDisplay(pw,x,y+2,width/3-25,MainWindow::getcoordheight()-2,'X',ToolBar);
  ydisplay = new OneCoordinateDisplay(pw,x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-2,'Y',ToolBar);
  zdisplay = new OneCoordinateDisplay(pw,x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-2,'Z',ToolBar);
#else
  xdisplay = new OneCoordinateDisplay(pw,x,y+2,width/3-25,MainWindow::getcoordheight()-5,'X');
  ydisplay = new OneCoordinateDisplay(pw,x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-5,'Y');
  zdisplay = new OneCoordinateDisplay(pw,x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-5,'Z');
#endif
  x = xpos = x1;
  y = ypos = y1;
  w = width;
  h = xdisplay->h;
  parent = pw;
  hDC = ToolBar;

  borderx1 = 0;
  bordery1 = 0;
  borderx2 = 0;
  bordery2 = 1;

#ifdef USING_WIDGETS
  ToolBar->addWidget(xdisplay->get_QWprompt());
  ToolBar->addWidget(ydisplay->get_QWprompt());
  ToolBar->addWidget(zdisplay->get_QWprompt());

  m_qw = ToolBar;
  h = ToolBar->height();
  w = ToolBar->width();
  x = xpos = ToolBar->pos().x();
  y = ypos = ToolBar->pos().y();
#endif
}

CoordinateDisplay::~CoordinateDisplay()
{
  //RCCHAR text[300];
  //sprintf(text,"%d,%d,%d,%d,1",x,y,w,h);
  //WritePrivateProfileString(_RCT("Layout"),_RCT("CoordinateDisplay"),text,home.getinifilename());
  if(m_qw->isVisible())
  {
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    settings.setValue("Layout/CoordinateDisplay", m_qw->saveGeometry());
  }

  delete xdisplay;
  delete ydisplay;
  delete zdisplay;

  m_qw->deleteLater();
  m_qw=0; // the QToolBar
}

void CoordinateDisplay::position(Point p)
{ if (db.workplanes.getcurrent() != NULL)
    p = db.workplanes.getcurrent()->modeltoworkplane(p);
  if (parent == 0 || hDC == 0)
    return;
  xdisplay->set(p.x);
  ydisplay->set(p.y);
  zdisplay->set(p.z);
}

RCHDC OneCoordinateDisplay::gethDC(void)
{  return prompt->hDC;
}

void CoordinateDisplay::draw(int drawmode)
{RECT rect;
 int i;

  if (parent == 0 || hDC == 0)
    return;

#ifdef USING_WIDGETS
  xdisplay->prompt->draw(drawmode);
  ydisplay->prompt->draw(drawmode);
  zdisplay->prompt->draw(drawmode);
#else
  xdisplay->prompt->draw(drawmode);
  ydisplay->prompt->draw(drawmode);
  zdisplay->prompt->draw(drawmode);

  //  Draw the boundary around the coordinate display area
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x + w + MenuBorderOffset + borderx2;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y - MenuBorderOffset + 1 + bordery1;
  FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x - MenuBorderOffset  - MenuHandleOffset + 1 + borderx1;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
  rect.left = x - MenuBorderOffset - MenuHandleOffset + borderx1;  rect.right = x + w + MenuBorderOffset + borderx2;
  rect.top = y + h + MenuBorderOffset - 1 + bordery2;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));
  rect.left = x + w + MenuBorderOffset - 1 + borderx2;  rect.right = x + w + MenuBorderOffset + borderx2;
  rect.top = y - MenuBorderOffset + bordery1;  rect.bottom = y + h + MenuBorderOffset + bordery2;
  FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));

  for (i = 1 ; i < 5 ; i += 4)
    {  //  Draw the menu handle
       rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 3;
       rect.top = y;  rect.bottom = y + 1;
       FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNHILIGHT));
       rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 1;
       rect.top = y;  rect.bottom = y + h;
       FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNHILIGHT));

       rect.left = x - MenuBorderOffset - MenuHandleOffset + 2 + i;  rect.right = rect.left + 3;
       rect.top = y + h - 1;  rect.bottom = y + h;
       FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));
       rect.left = x - MenuBorderOffset - MenuHandleOffset + 4 + i;  rect.right = rect.left + 1;
       rect.top = y;  rect.bottom = y + h;
       FillRect(xdisplay->gethDC(),&rect,GetSysColorBrush(COLOR_BTNSHADOW));
    }
#endif
}

void CoordinateDisplay::move(Window *win,int x1,int y1,int width)
{
  x = xpos = x1;
  y = ypos = y1;
  w = width;
  if (parent != 0)
    parent->delcontrol(this);
  win->addcontrol(this);
  parent = win;
  hDC = parent->gethdc();
  //if(m_qw && m_qw->parent())
  //    ((QWidget*)m_qw->parent())->move(x,y);
  xdisplay->move(win,1+x,y+2,width/3-25,MainWindow::getcoordheight()-5);
  ydisplay->move(win,1+x+width/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
  zdisplay->move(win,1+x+width*2/3,y+2,width/3-25,MainWindow::getcoordheight()-5);
}

void CoordinateDisplay::move(int dx,int dy)
{
    move(parent,x + dx,y + dy,w);
}


//
//  An output only View3d OpenGl drawing window, created from an existing HWND.
//  draws onto a gl context
//  the RCHWND is an RCOpenGLView (QGraphicsView)
//  the RCHDC is an RCOpenGLWidget (QOpenGLWidget)
//
View3dGLOutput::View3dGLOutput(RCHWND hWnd,RCHDC hDC,View *v,double scale) :
  View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
  RECT rect;
  displaylayertable = view->getdisplaylayertable();
  if (hWnd == 0)
  {
      rect.right = 10;  rect.bottom = 10;
  }
  else
    GetClientRect(hWnd,&rect);
  sethdc(hWnd,hDC);

  //((RCGraphicsPreview*)gethwnd())->mview3dsurface = this;

  Surface::sizeevent(rect.right,rect.bottom);
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
}

void View3dGLOutput::drawbegin()
{
}

void View3dGLOutput::draw(Entity *e, Transform *t, int drawmode)
{
    if (e->isvisible(this) && e->isa(group_entity) == 0)
    {
         e->drawGL(DM_NORMAL,NULL,this);
    }
}

void View3dGLOutput::drawend()
{
}

void View3dGLOutput::paintevent(void)
{
    /*
    scale = (umax - umin) / getwidthmm();

    View *view;
    int x=0,y=0;

    OpenOpenGl(gethdc(),3,x,y,(int)getwidth(),(int)getheight());
    view = getview();

    SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
    SetViewport(getumin(),getvmin(),getumax(),getvmax());

    SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

    BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
    SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

    EntityList dblist;
    int nlights=0;

    dblist = *db.geometry.getlist();

    Entity *e=0;
    for (dblist.end() ; (e = dblist.last()) != 0 ; )
    {
        if (e->isvisible(this) && (e->isa(group_entity) == 0 || e->getselected()))
        {
             e->drawGL(DM_NORMAL,NULL,this);
        }
    }

    EndScene();
    */
}

View3dOutput::View3dOutput(RCHWND hWindow,RCHDC oldhDC,View *v,double scale) :
  View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
  RECT rect;
  displaylayertable = view->getdisplaylayertable();
  if (hWindow == 0)
    {  rect.right = 10;  rect.bottom = 10;
    }
  else
    GetClientRect(hWindow,&rect);
  sethdc(hWindow,oldhDC);
  Surface::sizeevent(rect.right,rect.bottom);
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
}

void View3dOutput::setfbclip(double f, double b)
{
    clip.d5 = f;
    clip.d6 = b;
}

void View3dOutput::paintevent(void)
{
  scale = (umax - umin) / getwidthmm();

  if (SetLight != 0 && requiresopengl())
    {View *view;
     int x,y;

       if (transform)
         {  x = -GetDeviceCaps(gethdc(),PHYSICALOFFSETX);
            y = int(GetDeviceCaps(gethdc(),VERTRES) - (getheight()-1) + GetDeviceCaps(gethdc(),PHYSICALOFFSETY));
         }
       else
         x = y = 0;
       OpenOpenGl(gethdc(),3,x,y,(int)getwidth(),(int)getheight());
       view = getview();

       SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
       SetViewport(getumin(),getvmin(),getumax(),getvmax());

       SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

       BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
       SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
       db.geometry.draw(DM_NORMAL,this);
       EndScene();

    }
  else
    db.geometry.draw(DM_NORMAL,this);

}

View3dBitmapOutput::View3dBitmapOutput(View *v,double scale, double width, double height) :
  View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0), status(0)
{
  QRectF rect(0,0,width,height);
  if(width < 1)
      rect.setWidth(100);
  if(height < 1)
      rect.setHeight(100);
  displaylayertable = view->getdisplaylayertable();
  sethdc(0,0);
  //QGraphicsScene *scene = new QGraphicsScene(-rect.width(),-rect.height(),rect.width(),rect.height());
  QGraphicsScene *scene = new QGraphicsScene();
  scene->setSceneRect(rect);
  setScene(scene);
  //setxdotspermm(4.0);
  //setydotspermm(4.0);
  Surface::sizeevent(width,height);
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();

  this->mbitmap = new QPixmap(getwidth(),getheight());
  if(mbitmap->isNull())
     return;
  this->mbitmap->fill(QColor(255,255,255));
  status = 1;
}

View3dBitmapOutput::View3dBitmapOutput(RCHWND hWindow,RCHDC oldhDC,View *v,double scale) :
    View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
  RECT rect;
  displaylayertable = view->getdisplaylayertable();
  if (hWindow == 0)
  {
      rect.right = 10;  rect.bottom = 10;
  }
  else
    GetClientRect(hWindow,&rect);
  sethdc(hWindow,oldhDC);
  Surface::sizeevent(rect.right,rect.bottom);
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();

  this->mbitmap = new QPixmap(getwidth(),getheight());
  if(mbitmap->isNull())
     return;
  this->mbitmap->fill(QColor(255,255,255));
  status = 1;
}

void View3dBitmapOutput::setWidth(double width)
{
    Surface::sizeevent(width,getheight());
    updatematrix();
}

void View3dBitmapOutput::setHeight(double height)
{
    Surface::sizeevent(getwidth(),height);
    updatematrix();
}

void View3dBitmapOutput::setSize(double width, double height)
{
    Surface::sizeevent(width,height);
    updatematrix();
}

RCHBITMAP * View3dBitmapOutput::getBitmap()
{
    if(status)
        return (RCHBITMAP*)mbitmap;
    else return 0;
}

void View3dBitmapOutput::fixzoom()
{
  double umin1,umax1,vmin1,vmax1,aspect,du,dv;
  if (getheight() < 2 || getwidth() < 2) return;
  umin1 = umin;
  vmin1 = vmin;
  umax1 = umax;
  vmax1 = vmax;
  du = fabs(umax1 - umin1) / 2.0;
  dv = fabs(vmax1 - vmin1) / 2.0;
  if (du < db.getptoler() && dv < db.getptoler()) return;
  if (du < db.getptoler())
    du = dv * 0.01;
  if (dv < db.getptoler())
    dv = du * 0.01;
  aspect = getwidthmm() / getheightmm() * v.getreal("vw::aspectadjustment");
  if (du / dv > aspect) dv = du / aspect;
  if (du / dv < aspect) du = dv * aspect;
  umin = (umin1 + umax1) / 2.0 - du;
  vmin = (vmin1 + vmax1) / 2.0 - dv;
  umax = (umin1 + umax1) / 2.0 + du;
  vmax = (vmin1 + vmax1) / 2.0 + dv;
}

void View3dBitmapOutput::zoom(double umin1,double vmin1,double umax1,double vmax1)
{
#if 0
    umin = umin1;  vmin = vmin1;  umax= umax1;  vmax = vmax1;
    fixzoom();
    Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
    updatematrix();
#endif
#if 1
    umin = umin1;  vmin = vmin1;  umax= umax1;  vmax = vmax1;
    updatematrix();
#endif
#if 0
    double du,dv;
    int scalechanged;
    du = fabs(umax1 - umin1) / 2.0;
    dv = fabs(vmax1 - vmin1) / 2.0;

    if (du < db.getptoler() && dv < db.getptoler())
        return;

    scalechanged = (fabs(fabs(umax1-umin1) - fabs(umax-umin)) > fabs(umax-umin) / 100000.0||
                    fabs(fabs(vmax1-vmin1) - fabs(vmax-vmin))  > fabs(vmax-vmin) / 100000.0);

    umin = umin1;
    vmin = vmin1;
    umax = umax1;
    vmax = vmax1;

    fixzoom();

    Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
    updatematrix();
#endif
}

void View3dBitmapOutput::paintevent(void)
{
    scale = (umax - umin) / getwidthmm();

  if (SetLight != 0 && requiresopengl())
    {View *view;
     int x,y;

       if (transform)
         {  x = -GetDeviceCaps(gethdc(),PHYSICALOFFSETX);
            y = int(GetDeviceCaps(gethdc(),VERTRES) - (getheight()-1) + GetDeviceCaps(gethdc(),PHYSICALOFFSETY));
         }
       else
         x = y = 0;
       OpenOpenGl(gethdc(),3,x,y,(int)getwidth(),(int)getheight());
       view = getview();

       SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
       SetViewport(getumin(),getvmin(),getumax(),getvmax());

       SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

       BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
       SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
       db.geometry.draw(DM_NORMAL,this);
       EndScene();

    }
  else
    db.geometry.draw(DM_NORMAL,this);
}

void View3dBitmapOutput::render(void)
{
    if(status)
    {
        QPainter painter(mbitmap);
        painter.setRenderHints(painter.renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

        QRectF srect = getScene()->sceneRect();
        QRectF prect = mbitmap->rect();

        // translate the painter to put the scene bounding box in the middle of the image
        double xscale = fabs(prect.right() - prect.left()) / fabs(srect.right() - srect.left());
        double yscale = fabs(prect.bottom() - prect.top()) / fabs(srect.bottom() - srect.top());
        double pscale = qMin(xscale,yscale);

        double dx = fabs(prect.right() - prect.left())/2.0 - fabs(srect.right() - srect.left())*pscale/2.0;
        double dy = fabs(prect.bottom() - prect.top())/2.0 - fabs(srect.bottom() - srect.top())*pscale/2.0;
        painter.translate(dx,dy);

        getScene()->render(&painter,QRectF(),srect);

        painter.end();
    }
}

View2dOutput::View2dOutput(RCHWND hWindow,RCHDC oldhDC,View2d *v2d,View *v) :
    View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
    RECT rect;
  if (hWindow == 0)
    {  rect.right = 10;  rect.bottom = 10;
    }
  else
    GetClientRect(hWindow,&rect);
  sethdc(hWindow,oldhDC);
  Surface::sizeevent(rect.right,rect.bottom);
  view2d = v2d;
  view = v;
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
}

void View2dOutput::paintevent(void)
{OneView *ov;
 View3dSubSurface *ss;
  for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
    if ((ss = new View3dSubSurface(this,ov)) != NULL)
      {  ss->setdisplaylayertable(ss->getoneview()->getlayertable());
         ss->setprinterhdc(getprinterhdc());
         if (ov->gethiddenlines() || ov->getshaded())
           {  if (ov->getshaded())
                {double ambient,diffuse;
                  ambient = v.getreal("sh::ambient");
                  diffuse = v.getreal("sh::diffuse");
                  RenderImage image(ss,0,ambient,diffuse);
                  image.render();
                }
              if (ov->gethiddenlines())
                {  HideImage image(ss,ov->getdashhidden() * 2);
                   image.hide();
                }
           }
         else
           ss->paintevent();
         delete ss;
      }
  db.geometry.draw(DM_NORMAL,this);
}

//
//  The Control classes
//

void Control::fixup(Window *window)
{
   //hDC = (QWidget*)window->gethdc();
   x = xpos;  y = ypos;
   if (x == CN_CENTRED) x = int(window->getwidth()) / 2 - w / 2;
   if (y == CN_CENTRED) y = int(window->getheight()) / 2 - h / 2;
   if (x < 0) x = xpos + int(window->getwidth());
   if (y < 0) y = ypos + int(window->getheight());

}


ResourceString::ResourceString(int id,int size)
{
  string = new TCHAR[size];
  RCstring = new RCCHAR[size];
  if (string == nullptr)
    {
      allocated = 0;
      string = (TCHAR*)L"Internal : Memory allocation error";
      strcpy(RCstring,"Internal : Memory allocation error");
    }
  else
    {
       allocated = 1;
       if (! LoadString(program->gethinst(),id,string,size))
       {
         strcpy(string,L"Internal : Unknown resource string");
         strcpy(RCstring,"Internal : Unknown resource string");
       }
       else
       {
           //LOCAL2RCHR(RCstring,string);
           strcpy(RCstring,string);
       }
  }
}

ResourceString::~ResourceString()
{
    if (allocated)
    {
        delete [] string;
        delete [] RCstring;
    }
}

//
//  The list of all possible commands
//

CommandInfo::CommandInfo(int appid,int comid,int subcom,Command c,RCCHAR *desc)
{
   applicationid = appid;  subcommand = subcom;
   commandid = comid;  command = c;
   description = desc;
   shortdescription = desc;
   allocated = 0;
   if ((description = new RCCHAR[strlen(desc) + 1]) == NULL)
     description = _RCT("Internal : Memory allocation error");
   else
   {
        strcpy(description,desc);
        allocated |= 1;
   }
   if ((shortdescription = new RCCHAR[strlen(desc) + 1]) == NULL)
     description = _RCT("Internal : Memory allocation error");
   else
   {
        strcpy(shortdescription,desc);
        allocated |= 2;
   }
   implemented = 1;
}

CommandInfo::CommandInfo(int appid,int comid,int subcom,Command c,int desc,int shortdesc)
{
  ResourceString rs1(desc),rs2(shortdesc != 0 ? shortdesc : desc);
  applicationid = appid;  subcommand = subcom;
  commandid = comid;  command = c;
  allocated = 0;
  implemented = 1; // kmj new
  if ((description = new RCCHAR[strlen(rs1.gets()) + 1]) == NULL)
    description = _RCT("Internal : Memory allocation error");
  else
    {  strcpy(description,rs1.gets());
       allocated |= 1;
    }
  if ((shortdescription = new RCCHAR[strlen(rs2.gets()) + 1]) == NULL)
    description = _RCT("Internal : Memory allocation error");
  else
    {  strcpy(shortdescription,rs2.gets());
       allocated |= 2;
    }
  if(shortdesc == 999999)
      implemented=0;
}

CommandInfo::~CommandInfo()
{ if (allocated & 1) delete [] description;
  if (allocated & 2) delete [] shortdescription;
}
void Commands::add(int appid,int comid,int subcommand,Command com,char *desc)
{
    add( appid, comid, subcommand, com,CHAR2RCCHR(desc));
}

void Commands::add(int appid,int comid,int subcommand,Command com,RCCHAR *desc)
{
    GeneralList::add(new CommandInfo(appid,comid,subcommand,com,desc));
}

void Commands::add(int appid,int comid,int subcommand,Command com,int desc,int shortdesc)
{
    GeneralList::add(new CommandInfo(appid,comid,subcommand,com,desc,shortdesc));
}

Command Commands::command(int comid)
{CommandInfo *ci;
  for (start() ; (ci = (CommandInfo *) next()) != NULL ;  )
    if (ci->commandid == comid) return ci->command;
  return 0;
}

int Commands::command(RCCHAR *description)
{CommandInfo *ci;
  for (start() ; (ci = (CommandInfo *) next()) != 0 ;  )
    if (_stricmp(ci->description,description) == 0)
      return ci->commandid;
  return -1;
}

int Commands::subcommand(int comid)
{CommandInfo *ci;
  for (start() ; (ci = (CommandInfo *) next()) != NULL ;  )
    if (ci->commandid == comid) return ci->subcommand;
  return 0;
}

RCCHAR *Commands::description(int comid)
{CommandInfo *ci;
 ResourceString rs11(NCWIN+11);
  for (start() ; (ci = (CommandInfo *) next()) != nullptr ;  )
  {
    if (ci->commandid == comid) return ci->description;
  }
  return rs11.getws();
}

RCCHAR *Commands::shortdescription(int comid)
{CommandInfo *ci;
 ResourceString rs11(NCWIN+11);
  for (start() ; (ci = (CommandInfo *) next()) != NULL ;  )
    if (ci->commandid == comid) return ci->shortdescription;
  return rs11.getws();
}

int Commands::implemented(int comid)
{
 CommandInfo *ci;
  for (start() ; (ci = (CommandInfo *) next()) != NULL ;  )
    if (ci->commandid == comid) return ci->implemented;
  return 0;
}

Commands::~Commands()
{CommandInfo *ci;
  for (start() ; (ci = (CommandInfo *) next()) != NULL ;  )
    delete ci;
  destroy();
}

#ifdef USING_WIDGETS
OffScreenQtSurface::OffScreenQtSurface(RCHDC oldhDC,int w,int h)
{
    RCHDC hDC=0,screenhDC=0;

    screenhDC = 0;
    Surface::sizeevent(w,h);
    status = 1;
    sethdc(NULL,hDC);
}

OffScreenQtSurface::~OffScreenQtSurface()
{
}

View3dOffScreenQtSurface::View3dOffScreenQtSurface(View3dSurface *window,int pixelscale) :
    OffScreenQtSurface(window->gethdc(),int(window->getwidth())*pixelscale,int(window->getheight()*pixelscale)),
    View3dSurface(window->getview(),window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),0,0)
{
    setrepaintstyle(window->getrepaintstyle());
    setbackground(window->getbackground());
    hDC = window->gethdc();
    hWnd = window->gethwnd();
    setScene(window->getScene());
}

View3dOffScreenQtSurface::View3dOffScreenQtSurface(View3dSurface *window,int width,int height) :
    OffScreenQtSurface(window->gethdc(),width,height),
    View3dSurface(window->getview(),window->getumin(),window->getvmin(),window->getumax(),window->getvmax(),0,0)
{
    setrepaintstyle(window->getrepaintstyle());
    setbackground(window->getbackground());

    //  Take the dots per mm settings from the original surface
    setxdotspermm(window->getxdotspermm());
    setydotspermm(window->getydotspermm());
    hDC = window->gethdc();
    hWnd = window->gethwnd();
    setScene(window->getScene());
}

void View3dOffScreenQtSurface::ChangeView(View3dSurface *vs)
{
    view = vs->getview();
    sizeevent(vs->getwidth(),vs->getheight());
}

void View3dOffScreenQtSurface::paintevent(void)
{
    RECT rect;
    RCHBRUSH hbrush;
    scale = (umax - umin) / getwidthmm();
    rect.left = 0;  rect.top = 0;  rect.right = int(getwidth());  rect.bottom = int(getheight());
    //hbrush = CreateSolidBrush(getbackground());
    //FillRect(gethdc(),&rect,hbrush);
    //DeleteObject(hbrush);
    if (getrepaintstyle() == Wireframe || getrepaintstyle() == RayTraced)
    {
        if (SetLight != 0 && requiresopengl())
        {
           View *view;
           OpenOpenGl(gethdc(),2,0,0,(int)getwidth(),(int)getheight());
           view = getview();

           SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
           SetViewport(getumin(),getvmin(),getumax(),getvmax());

           SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

           BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
           SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
           db.draw(DM_NORMAL,this);
           EndScene();

        }
        else
            db.draw(DM_NORMAL,this);
    }
    else if (getrepaintstyle() == Hidden)
    {
        HideImage image(this,1+v.getinteger("hd::dashhiddenlines") * 2);
        image.hide();
    }
    else
    {
        double ambient,diffuse;
        int hiddenlines;
        ambient = v.getreal("sh::ambient");
        diffuse = v.getreal("sh::diffuse");
        hiddenlines = v.getinteger("sh::hiddenlines");
        RenderImage image(this,1,ambient,diffuse);
        image.render();
        if (hiddenlines)
        {
            HideImage image(this,v.getinteger("hd::dashhiddenlines") * 2);
            image.hide();
        }
    }
}

View2dOffScreenQtSurface::View2dOffScreenQtSurface(View3dSurface *vw,View2d *v2d,int pixelscale)
    : View3dOffScreenQtSurface(vw,pixelscale)
{
    view2d = v2d;
    plotscale = 1.0;
}


 View2dOffScreenQtSurface::View2dOffScreenQtSurface(View3dSurface *vw,View2d *v2d,int width,int height)
    : View3dOffScreenQtSurface(vw,width,height)
{
    view2d = v2d;
    plotscale = 1.0;
}

void View2dOffScreenQtSurface::paintevent(void)
{

}

View3dQtOutput::View3dQtOutput(QPrinter *printer,QPainter *painter,View *v,double scale)
  : View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
  RECT rect;

  mPrinter = printer;
  mPainter = painter;

  setScene(new QGraphicsScene(printer->paperRect()));

  width = printer->paperRect().width();
  height = printer->paperRect().height();

  int pdpix = printer->physicalDpiX();
  int pdpiY = printer->physicalDpiY();
  int lpdpix = printer->logicalDpiX();
  int lpdpiy = printer->logicalDpiY();
  xdotspermm = printer->physicalDpiX() / 25.4;
  ydotspermm = printer->physicalDpiY() / 25.4;

  displaylayertable = view->getdisplaylayertable();

  Surface::sizeevent(rect.right,rect.bottom);
  umin = -getwidthmm() * 0.1 * scale;
  umax =  getwidthmm() * 0.9 * scale;
  vmin = -getheightmm() * 0.1 * scale;
  vmax =  getheightmm() * 0.9 * scale;
  Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
  updatematrix();
}

void View3dQtOutput::paintevent(void)
{
    db.clearScene(DM_ERASE,cadwindow->getcurrentsurface(),0);

    scale = (umax - umin) / getwidthmm();

    if (0 /*SetLight != 0 && requiresopengl()*/)
    {
     View *view;
     int x,y;

       if (transform)
         {  x = -GetDeviceCaps(gethdc(),PHYSICALOFFSETX);
            y = int(GetDeviceCaps(gethdc(),VERTRES) - (getheight()-1) + GetDeviceCaps(gethdc(),PHYSICALOFFSETY));
         }
       else
         x = y = 0;
       OpenOpenGl(gethdc(),3,x,y,(int)getwidth(),(int)getheight());
       view = getview();

       SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
       SetViewport(getumin(),getvmin(),getumax(),getvmax());

       SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));

       BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH);
       SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);
       db.geometry.draw(DM_NORMAL,this);
       EndScene();

    }
    else
        db.drawforQtPrint(DM_NORMAL,this,0);
        //db.geometry.draw(DM_NORMAL,this);
}

View2dQtOutput::View2dQtOutput(RCHWND hWindow,RCHDC oldhDC,View2d *v2d,View *v)
 : View3dSurface(v,-1.0,-1.0,1.0,1.0,0,0)
{
    RECT rect;
    if (hWindow == 0)
    {
        rect.right = 10;  rect.bottom = 10;
    }
    else
    GetClientRect(hWindow,&rect);
    sethdc(hWindow,oldhDC);
    Surface::sizeevent(rect.right,rect.bottom);
    view2d = v2d;
    view = v;
    umin = -getwidthmm() * 0.1 * scale;
    umax =  getwidthmm() * 0.9 * scale;
    vmin = -getheightmm() * 0.1 * scale;
    vmax =  getheightmm() * 0.9 * scale;
    Surface::setscale((umax - umin) / getwidth() * getxdotspermm());
    updatematrix();
}

void View2dQtOutput::paintevent(void)
{
    OneView *ov;
    View3dSubSurface *ss;
    for (view2d->start() ;  (ov = view2d->next()) != NULL ; )
    if ((ss = new View3dSubSurface(this,ov)) != NULL)
      {  ss->setdisplaylayertable(ss->getoneview()->getlayertable());
         ss->setprinterhdc(getprinterhdc());
         if (ov->gethiddenlines() || ov->getshaded())
           {  if (ov->getshaded())
                {double ambient,diffuse;
                  ambient = v.getreal("sh::ambient");
                  diffuse = v.getreal("sh::diffuse");
                  RenderImage image(ss,0,ambient,diffuse);
                  image.render();
                }
              if (ov->gethiddenlines())
                {  HideImage image(ss,ov->getdashhidden() * 2);
                   image.hide();
                }
           }
         else
           ss->paintevent();
         delete ss;
      }
    db.geometry.draw(DM_NORMAL,this);
}
#endif
