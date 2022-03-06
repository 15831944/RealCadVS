
#include "ncwin.h"

class ViewActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     ViewActive() {  active = 0;  buttonmenu = NULL;  }
     ~ViewActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class ViewMenuWindow : public MenuWindow
  {private:
     ButtonMenu *buttonmenu;
     ViewActive *viewactive;
   public:
     ViewMenuWindow(Window *,int,int,int,int,ViewActive *);
     ~ViewMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

ViewMenuWindow::ViewMenuWindow(Window *win,int x,int y,int w,int h,ViewActive *va) :
  MenuWindow(win,x,y,w,h)
{ viewactive = va;
  buttonmenu = viewactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  viewactive->setactive(1);
}

ViewMenuWindow::~ViewMenuWindow()
{ viewactive->setactive(0);
}

ViewActive viewactive;

void view_command19(int *cstate,HighLevelEvent *,void **)
{
#if 1
    *cstate = ExitState;
#else
  ResourceString rs152(NCVIEWB+152),rs153(NCVIEWB+153);
  *cstate = ExitState;
  if (viewactive.getactive()) return;
  if (viewactive.getbuttonmenu() == 0)
  {
      ButtonMenu *menu = new ButtonMenu(0,0,14,14,15);
       if (menu != NULL)
         {  menu->add(new Button(-1,-1,"View1",542));
            menu->add(new Button(-1,-1,"View2",543));
            menu->add(new Button(-1,-1,"View3",544));
            menu->add(new Button(-1,-1,"View4",545));
            menu->add(new Button(-1,-1,"View5",546));
            menu->add(new Button(-1,-1,"View6",547));
            menu->add(new Button(-1,-1,"View7",548));
            menu->add(new Button(-1,-1,"View8",549));
            menu->add(new Button(-1,-1,"View9",550));
            menu->add(new Button(-1,-1,"View10",551));
            menu->add(new Button(-1,-1,"View11",552));
            menu->add(new Button(-1,-1,"View12",553));
            menu->add(new Button(-1,-1,"View14",554));
            viewactive.setbuttonmenu(menu);
         }
  }
  if (viewactive.getbuttonmenu() == 0)
  {
      cadwindow->MessageBox(rs153.gets(),rs152.gets(),MB_ICONSTOP);
       return;
  }
  new ViewMenuWindow(cadwindow->getsubwindow(),
                      int(cadwindow->getsubwindow()->getwidth())-60,0,24*2,24*7,&viewactive);
  *cstate = ExitState;
#endif
}

void view_command20(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        dir = (ref - eye).normalize() * v.getreal("vw::distance");
        eye += dir;
        ref += dir;
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command21(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        dir = (ref - eye).normalize() * v.getreal("vw::distance");
        eye -= dir;
        ref -= dir;
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command22(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(ref,up,-v.getreal("vw::distance") / (ref - eye).length());
        eye = t.transform(eye);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command23(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(ref,up,v.getreal("vw::distance") / (ref - eye).length());
        eye = t.transform(eye);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command24(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
 double focaldistance;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        focaldistance = (ref - eye).length();
        eye += up.normalize() * v.getreal("vw::distance");
        ref = eye + (ref - eye).normalize() * focaldistance;
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}


void view_command25(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
 double focaldistance;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        focaldistance = (ref - eye).length();
        eye -= up.normalize() * v.getreal("vw::distance");
        ref = eye + (ref - eye).normalize() * focaldistance;
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command26(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(eye,up,v.getreal("vw::distance") / (ref - eye).length());
        ref = t.transform(ref);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command27(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(eye,up,-v.getreal("vw::distance") / (ref - eye).length());
        ref = t.transform(ref);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command28(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(eye,(eye - ref).cross(up),-v.getreal("vw::distance") / (ref - eye).length());
        ref = t.transform(ref);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command29(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {Transform t;
        cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        t.rotate(eye,(eye - ref).cross(up),v.getreal("vw::distance") / (ref - eye).length());
        ref = t.transform(ref);
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command30(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        if ((ref - eye).length() > v.getreal("vw::distance"))
          eye = ref + (eye - ref).normalize() * ((eye -ref).length() - v.getreal("vw::distance"));
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command31(int *cstate,HighLevelEvent *,void **)
{ResourceString rs154(NCVIEWB+154),rs155(NCVIEWB+155);
 Point eye,ref,up,dir;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL || cadwindow->getcurrentwindow()->getsurfacetype() == View2dSurf)
    cadwindow->MessageBox(rs154.gets(),rs155.gets(),MB_ICONINFORMATION);
  else
    {   cadwindow->getcurrentsurface()->getview();
        db.savezoomundo(1,cadwindow->getcurrentwindow());
        eye = cadwindow->getcurrentsurface()->getview()->geteye();
        ref = cadwindow->getcurrentsurface()->getview()->getref();
        up = cadwindow->getcurrentsurface()->getview()->getup();
        eye = ref + (eye - ref).normalize() * ((eye -ref).length() + v.getreal("vw::distance"));
        if (cadwindow->getcurrentsurface()->getview()->changeview(eye,ref,up))
          cadwindow->updatewindows(cadwindow->getcurrentsurface()->getview());
    }
}

void view_command32(int *cstate,HighLevelEvent *,void **)
{double distance;
 ResourceString rs156(NCVIEWB+156),rs157(NCVIEWB+157);
 Dialog dialog(rs156.gets(),rs157.gets(),&distance,ScaleByLength);
  distance = v.getreal("vw::distance");
  if (dialog.process() == TRUE)
    v.setreal("vw::distance",distance);
  *cstate = ExitState;
}

ComposeMenuWindow::ComposeMenuWindow(Window *win,int x,int y,int w,int h,ComposeActive *aa) :
  MenuWindow(win,x,y,w,h)
{ composeactive = aa;
  buttonmenu = composeactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  composeactive->setactive(1);
}

ComposeMenuWindow::~ComposeMenuWindow()
{ composeactive->setactive(0);
}

ComposeActive composeactive;

void view_command39(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
  if (composeactive.getactive()) return;
  if (composeactive.getbuttonmenu() == NULL)
    {ButtonMenu *menu = new ButtonMenu(0,0,10,10,15);
       if (menu != NULL)
         {  menu->add(new Button(-1,-1, (char*)"Compose1",524));
            menu->add(new Button(-1,-1, (char*)"Compose2",525));
            menu->add(new Button(-1,-1, (char*)"Compose3",526));
            menu->add(new Button(-1,-1, (char*)"Compose4",556));
            menu->add(new Button(-1,-1, (char*)"Compose5",527));
            menu->add(new Button(-1,-1, (char*)"Compose6",530));
            menu->add(new Button(-1,-1, (char*)"Compose7",558));
            menu->add(new Button(-1,-1, (char*)"Compose9",581));
            menu->add(new Button(-1,-1, (char*)"Compose10",566));
            composeactive.setbuttonmenu(menu);
         }
    }
  if (composeactive.getbuttonmenu() == NULL)
    {  cadwindow->MessageBox("Failed to create the compose button menu","Compose",MB_ICONSTOP);
       return;
    }
  new ComposeMenuWindow(cadwindow->getsubwindow(),
                          int(cadwindow->getsubwindow()->getwidth())-120,0,
                          48,96,&composeactive);
}


