
#include "ncwin.h"

class ApplicationActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     ApplicationActive() {  active = 0;  buttonmenu = NULL;  }
     ~ApplicationActive()  
                           {  delete buttonmenu;  
                           }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class ApplicationMenuWindow : public MenuWindow
  {private:
     ButtonMenu *buttonmenu;
     ApplicationActive *applicationactive;
   public:
     ApplicationMenuWindow(Window *,int,int,int,int,ApplicationActive *);
     ~ApplicationMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

ApplicationMenuWindow::ApplicationMenuWindow(Window *win,int x,int y,int w,int h,ApplicationActive *aa) :
  MenuWindow(win,x,y,w,h)
{ applicationactive = aa;
  buttonmenu = applicationactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  applicationactive->setactive(1);
}

ApplicationMenuWindow::~ApplicationMenuWindow()
{ applicationactive->setactive(0);
}

ApplicationActive applicationactive;

